/**
@file Scene.cpp
@author t-sakai
@date 2018/05/25 create
*/
#define CPPIMG_IMPLEMENTATION
#include <cppimg/cppimg.h>
#define CPPGLTF_IMPLEMENTATION
#include <cppgltf/cppgltf.h>

#include "scene/Scene.h"
#include "math/Quaternion.h"

namespace lray
{
    Scene::Scene()
    {
    }

    Scene::Scene(Scene&& rhs)
        :meshes_(move(rhs.meshes_))
        ,nodes_(move(rhs.nodes_))
    {
    }

    Scene::Scene(const Char* name, MeshArray&& meshes, NodeArray&& nodes)
        :meshes_(move(meshes))
        ,nodes_(move(nodes))
    {
        if(NULL != name){
            name_.assign(name);
        }
    }

    Scene& Scene::operator=(Scene&& rhs)
    {
        if(this == &rhs){
            return *this;
        }
        name_ = move(rhs.name_);
        meshes_ = move(rhs.meshes_);
        nodes_ = move(rhs.nodes_);
        return *this;
    }

    Result Scene::test(Intersection& intersection, Ray& ray)
    {
        HitRecord hitRecord = accelerator_.intersect(ray);
        if(Result_Fail != hitRecord.result_){
            intersection.result_ = Result_Success;
            intersection.t_ = hitRecord.t_;
            intersection.b0_ = 1.0f-hitRecord.v_-hitRecord.w_;
            intersection.b1_ = hitRecord.v_;
            intersection.b2_ = hitRecord.w_;
            const TriangleProxy& proxy = *reinterpret_cast<const TriangleProxy*>(hitRecord.primitive_);
            const Primitive& primitive = *proxy.primitive_;
            const Triangle& triangle = primitive.getTriangle(proxy.index_);

            //Calc normal
            f32 w0 = intersection.b0_;
            f32 w1 = intersection.b1_;
            f32 w2 = intersection.b2_;
            const Vector3& n0 = primitive.getNormal(triangle.indices_[0]);
            const Vector3& n1 = primitive.getNormal(triangle.indices_[1]);
            const Vector3& n2 = primitive.getNormal(triangle.indices_[2]);
            intersection.shadingNormal_ = weightedAverage(w0, w1, w2, n0, n1, n2);
        }
        return intersection.result_;
    }

    void Scene::updateFrame()
    {
        refinedMeshes_.resize(meshes_.size());

        //Update world matrices and meshes of root nodes
        s32 inode=0;
        for(; inode<nodes_.size(); ++inode){
            Node& node = nodes_[inode];
            if(0<=node.getParent()){
                break;
            }
            node.getWorldMatrix() = node.getMatrix();

            s32 imesh = node.getMesh();
            if(0<=imesh){
                refinedMeshes_[imesh].refine(meshes_[imesh], node.getWorldMatrix());
            }
        }
        //Update descendant's
        for(; inode<nodes_.size(); ++inode){
            Node& node = nodes_[inode];
            s32 iparent = node.getParent();
            node.getWorldMatrix().mul(node.getMatrix(), nodes_[iparent].getWorldMatrix());

            s32 imesh = node.getMesh();
            if(0<=imesh){
                refinedMeshes_[imesh].refine(meshes_[imesh], node.getWorldMatrix());
            }
        }

        //Get triangle proxies
        //---------------------
        //Create a buffer for triangle proxies
        s32 numTriangles = 0;
        for(s32 i=0; i<refinedMeshes_.size(); ++i){
            for(s32 j=0; j<refinedMeshes_[i].getNumPrimitives(); ++j){
                const Primitive& primitive = refinedMeshes_[i].getPrimitive(j);
                numTriangles += primitive.getNumTriangles();
            }
        }
        triangleProxies_.resize(numTriangles);

        numTriangles = 0;
        for(s32 i = 0; i<refinedMeshes_.size(); ++i){
            for(s32 j = 0; j<refinedMeshes_[i].getNumPrimitives(); ++j){
                refinedMeshes_[i].getPrimitive(j).getTriangleProxies(&triangleProxies_[numTriangles]);
                numTriangles += refinedMeshes_[i].getPrimitive(j).getNumTriangles();
            }
        }
        accelerator_.build(numTriangles, &triangleProxies_[0]);
    }


 namespace
 {
     cppgltf::Attribute* findPrimitiveAttributes(cppgltf::Primitive& primitive, s32 semanticType, s32 semanticIndex)
     {
         for(s32 i=0; i<primitive.attributes_.size(); ++i){
             cppgltf::Attribute& attribute = primitive.attributes_[i];
             if(semanticType == attribute.semanticType_
                 && semanticIndex == attribute.semanticIndex_){
                 return &attribute;
             }
         }
         return NULL;
     }

     void createPositions(s32& numPositions, Vector3** positions, cppgltf::glTF& gltf, cppgltf::Primitive& primitive)
     {
         numPositions = 0;
         *positions = NULL;
         //Check having position attribute
         cppgltf::Attribute* attribute = findPrimitiveAttributes(primitive, cppgltf::GLTF_ATTRIBUTE_POSITION, 0);
         if(NULL == attribute){
             return;
         }
         if(attribute->accessor_<0 || gltf.accessors_.size()<=attribute->accessor_){
             return;
         }
         cppgltf::Accessor& accessor = gltf.accessors_[attribute->accessor_];
         //Check that component-type and type are VEC3 and FLOAT respectively.
         if(accessor.componentType_ != cppgltf::GLTF_TYPE_FLOAT
             || accessor.type_ != cppgltf::GLTF_TYPE_VEC3){
             return;
         }
         if(accessor.bufferView_<0 || gltf.bufferViews_.size()<=accessor.bufferView_){
             return;
         }

         cppgltf::BufferView& bufferView = gltf.bufferViews_[accessor.bufferView_];
         if(bufferView.buffer_<0 || gltf.buffers_.size()<=bufferView.buffer_){
             return;
         }

         cppgltf::Buffer& buffer = gltf.buffers_[bufferView.buffer_];
         s32 byteStride = (bufferView.byteStride_<=0)? sizeof(f32)*3 : bufferView.byteStride_;
         numPositions = accessor.count_;
         *positions = LNEW Vector3[numPositions];
         Vector3 bmin(F32_INFINITY), bmax(-F32_INFINITY);
         u8* data = buffer.data_ + bufferView.byteOffset_ + accessor.byteOffset_;
         for(s32 i=0; i<numPositions; ++i, data+=byteStride){
             Vector3* p = reinterpret_cast<Vector3*>(data);
             bmin = minimum(*p, bmin);
             bmax = maximum(*p, bmax);
             (*positions)[i] = *p;
         }
     }

     void createNormals(s32& numNormals, Vector3** normals, cppgltf::glTF& gltf, cppgltf::Primitive& primitive)
     {
         numNormals = 0;
         *normals = NULL;
         //Check having normal attribute
         cppgltf::Attribute* attribute = findPrimitiveAttributes(primitive, cppgltf::GLTF_ATTRIBUTE_NORMAL, 0);
         if(NULL == attribute){
             return;
         }
         if(attribute->accessor_<0 || gltf.accessors_.size()<=attribute->accessor_){
             return;
         }
         cppgltf::Accessor& accessor = gltf.accessors_[attribute->accessor_];
         //Check that component-type and type are VEC3 and FLOAT respectively.
         if(accessor.componentType_ != cppgltf::GLTF_TYPE_FLOAT
             || accessor.type_ != cppgltf::GLTF_TYPE_VEC3){
             return;
         }
         if(accessor.bufferView_<0 || gltf.bufferViews_.size()<=accessor.bufferView_){
             return;
         }

         cppgltf::BufferView& bufferView = gltf.bufferViews_[accessor.bufferView_];
         if(bufferView.buffer_<0 || gltf.buffers_.size()<=bufferView.buffer_){
             return;
         }

         cppgltf::Buffer& buffer = gltf.buffers_[bufferView.buffer_];
         s32 byteStride = (bufferView.byteStride_<=0)? sizeof(f32)*3 : bufferView.byteStride_;
         numNormals = accessor.count_;
         *normals = LNEW Vector3[numNormals];
         u8* data = buffer.data_ + bufferView.byteOffset_ + accessor.byteOffset_;
         for(s32 i=0; i<numNormals; ++i, data+=byteStride){
             Vector3* p = reinterpret_cast<Vector3*>(data);
             (*normals)[i] = normalize(*p);
         }
     }

     void generateTriangles(s32& numTriangles, Triangle** triangles, s32 primitiveMode, s32 numPositions)
     {
         switch(primitiveMode)
         {
         case cppgltf::GLTF_PRIMITIVE_TRIANGLES:
         {
             numTriangles = numPositions/3;
             *triangles = LNEW Triangle[numTriangles];
             Triangle* tri = *triangles;
             for(s32 i=0; i<numPositions; i+=3, ++tri){
                 tri->indices_[0] = i;
                 tri->indices_[1] = i+1;
                 tri->indices_[2] = i+2;
             }
         }
         break;
         case cppgltf::GLTF_PRIMITIVE_TRIANGLE_STRIP:
         {
             numTriangles = maximum(numPositions-2, 0);
             *triangles = LNEW Triangle[numTriangles];
             Triangle* tri = *triangles;
             for(s32 i=2; i<numPositions; ++i, ++tri){
                 tri->indices_[0] = i-2;
                 tri->indices_[1] = i-1;
                 tri->indices_[2] = i;
             }
         }
         break;
         case cppgltf::GLTF_PRIMITIVE_TRIANGLE_FAN:
         {
             numTriangles = maximum(numPositions-2, 0);
             *triangles = LNEW Triangle[numTriangles];
             Triangle* tri = *triangles;
             for(s32 i=2; i<numPositions; ++i, ++tri){
                 tri->indices_[0] = 0;
                 tri->indices_[1] = i-1;
                 tri->indices_[2] = i;
             }
         }
         break;
         default:
             numTriangles = 0;
             *triangles = NULL;
             break;
         }
     }

     template<class T>
     void createTriangles(s32& numTriangles, Triangle** triangles, s32 primitiveMode, s32 numIndices, s32 byteStride, u8* data)
     {
         switch(primitiveMode)
         {
         case cppgltf::GLTF_PRIMITIVE_TRIANGLES:
         {
             numTriangles = numIndices/3;
             *triangles = LNEW Triangle[numTriangles];
             Triangle* tri = *triangles;
             for(s32 i=0; i<numIndices; i+=3, ++tri){
                 tri->indices_[0] = *reinterpret_cast<T*>(data);
                 data += byteStride;
                 tri->indices_[1] = *reinterpret_cast<T*>(data);
                 data += byteStride;
                 tri->indices_[2] = *reinterpret_cast<T*>(data);
                 data += byteStride;
             }
         }
         break;
         case cppgltf::GLTF_PRIMITIVE_TRIANGLE_STRIP:
         {
             numTriangles = maximum(numIndices-2, 0);
             *triangles = LNEW Triangle[numTriangles];
             Triangle* tri = *triangles;
             data += byteStride;
             for(s32 i=2; i<numIndices; ++i, ++tri){
                 tri->indices_[0] = *reinterpret_cast<T*>(data-byteStride);
                 tri->indices_[1] = *reinterpret_cast<T*>(data);
                 tri->indices_[2] = *reinterpret_cast<T*>(data+byteStride);
                 data += byteStride;
             }
         }
         break;
         case cppgltf::GLTF_PRIMITIVE_TRIANGLE_FAN:
         {
             numTriangles = maximum(numIndices-2, 0);
             *triangles = LNEW Triangle[numTriangles];
             Triangle* tri = *triangles;
             T index0 = *reinterpret_cast<T*>(data);
             data += byteStride;
             for(s32 i=2; i<numIndices; ++i, ++tri){
                 tri->indices_[0] = index0;
                 tri->indices_[1] = *reinterpret_cast<T*>(data);
                 tri->indices_[2] = *reinterpret_cast<T*>(data+byteStride);
                 data += byteStride;
             }
         }
         break;
         default:
             numTriangles = 0;
             *triangles = NULL;
             break;
         }
     }

     void createTriangles(s32& numTriangles, Triangle** triangles, cppgltf::glTF& gltf, cppgltf::Primitive& primitive)
     {
         numTriangles = 0;
         *triangles = NULL;
         //Check having position attribute
         cppgltf::Attribute* attribute = findPrimitiveAttributes(primitive, cppgltf::GLTF_ATTRIBUTE_POSITION, 0);
         if(NULL == attribute){
             return;
         }
         if(attribute->accessor_<0 || gltf.accessors_.size()<=attribute->accessor_){
             return;
         }
         cppgltf::Accessor& positionAccessor = gltf.accessors_[attribute->accessor_];

         if(primitive.indices_<0 || gltf.accessors_.size()<=primitive.indices_){
             //Don't have indices, then create indices
             generateTriangles(numTriangles, triangles, primitive.mode_, positionAccessor.count_);
             return;
         }

         cppgltf::Accessor& indexAccessor = gltf.accessors_[primitive.indices_];

         //Check that component-type and type are VEC3 and FLOAT respectively.
         if((indexAccessor.componentType_ != cppgltf::GLTF_TYPE_UNSIGNED_SHORT
             && indexAccessor.componentType_ != cppgltf::GLTF_TYPE_INT
             && indexAccessor.componentType_ != cppgltf::GLTF_TYPE_UNSIGNED_INT)
             || indexAccessor.type_ != cppgltf::GLTF_TYPE_SCALAR){
             return;
         }
         if(indexAccessor.bufferView_<0 || gltf.bufferViews_.size()<=indexAccessor.bufferView_){
             return;
         }

         cppgltf::BufferView& bufferView = gltf.bufferViews_[indexAccessor.bufferView_];
         if(bufferView.buffer_<0 || gltf.buffers_.size()<=bufferView.buffer_){
             return;
         }

         cppgltf::Buffer& buffer = gltf.buffers_[bufferView.buffer_];
         s32 byteStride;

         u8* data = buffer.data_ + bufferView.byteOffset_ + indexAccessor.byteOffset_;

         switch(indexAccessor.componentType_){
         case cppgltf::GLTF_TYPE_UNSIGNED_SHORT:
         {
             byteStride = (bufferView.byteStride_<=0)? 2 : bufferView.byteStride_;
             createTriangles<u16>(numTriangles, triangles, primitive.mode_, indexAccessor.count_, byteStride, data);
         }
         break;
         case cppgltf::GLTF_TYPE_INT:
         {
             byteStride = (bufferView.byteStride_<=0)? 4 : bufferView.byteStride_;
             createTriangles<s32>(numTriangles, triangles, primitive.mode_, indexAccessor.count_, byteStride, data);
         }
         break;
         case cppgltf::GLTF_TYPE_UNSIGNED_INT:
         {
             byteStride = (bufferView.byteStride_<=0)? 4 : bufferView.byteStride_;
             createTriangles<u32>(numTriangles, triangles, primitive.mode_, indexAccessor.count_, byteStride, data);
         }
         break;
         }
     }

     Primitive createPrimitive(cppgltf::glTF& gltf, cppgltf::Primitive& gltfPrimitive)
     {
         s32 numVertices;
         Vector3* positions = NULL;
         createPositions(numVertices, &positions, gltf, gltfPrimitive);

         s32 numNormals;
         Vector3* normals = NULL;
         createNormals(numNormals, &normals, gltf, gltfPrimitive);

         s32 numTriangles;
         Triangle* triangles = NULL;
         createTriangles(numTriangles, &triangles, gltf, gltfPrimitive);
         return Primitive(numVertices, positions, normals, numTriangles, triangles);
     }
 }

    void load(Scene& scene, const Char* filepath)
    {
        cppgltf::IFStream ifstream;
        if(!ifstream.open(filepath)){
            return;
        }

        s32 pathLength = strlen_s32(filepath);
        Char* directoryPath = LNEW Char[pathLength+1];
        extractDirectoryPath(directoryPath, pathLength, filepath);

        cppgltf::glTFHandler gltfHandler(directoryPath);
        cppgltf::JSONReader gltfJsonReader(ifstream, gltfHandler);
        bool result = gltfJsonReader.read();
        if(!result){
            LDELETE_ARRAY(directoryPath);
            return;
        }
        ifstream.close();

        cppgltf::glTF& gltf = gltfHandler.get();

        //asset
        LASSERT("2.0" == gltf.asset_.version_);

        //meshes
        //--------------------------------------------
        Scene::MeshArray meshArray;
        for(s32 i=0; i<gltf.meshes_.size(); ++i){
            cppgltf::Mesh& gltfMesh = gltf.meshes_[i];
            Mesh::PrimitiveArray primitiveArray;
            for(s32 j=0; j<gltfMesh.primitives_.size(); ++j){
                cppgltf::Primitive& gltfPrim = gltfMesh.primitives_[j];
                //check
                if(gltfPrim.attributes_.size()<=0
                    || gltfPrim.indices_<0){
                    continue;
                }
                if(gltfPrim.mode_<cppgltf::GLTF_PRIMITIVE_TRIANGLES){
                    continue;
                }
                Primitive primitive = createPrimitive(gltf, gltfPrim);
                if(primitive.getNumVertices()<=0){
                    continue;
                }
                primitiveArray.push_back(move(primitive));
            }
            if(primitiveArray.size()<=0){
                continue;
            }
            Mesh mesh(move(primitiveArray));
            meshArray.push_back(move(mesh));
        }

        //nodes
        //--------------------------------------------
        Scene::NodeArray nodeArray;
        if(0<gltf.sortedNodes_.size()){
            nodeArray.reserve(gltf.sortedNodes_.size());
            for(s32 i=0; i<gltf.sortedNodes_.size(); ++i){
                const cppgltf::glTF::SortNode& sortNode = gltf.sortedNodes_[i];
                const cppgltf::Node& gltfNode = gltf.nodes_[sortNode.oldId_];
                Node node(gltfNode.name_.c_str(), sortNode.parent_, sortNode.numChildren_, sortNode.childrenStart_, gltfNode.mesh_);
                lray::Matrix44& matrix = node.getMatrix();
                if(gltfNode.flags_.check(cppgltf::Node::Flag_Matrix)){
                    for(s32 m=0; m<4; ++m){
                        for(s32 n=0; n<4; ++n){
                            matrix.m_[m][n] = gltfNode.matrix_[n*4+m]; //transpose glTF matrix
                        }
                    }
                }else{
                    matrix.identity();
                    //Translate
                    matrix.setTranslate(gltfNode.translation_[0], gltfNode.translation_[1], gltfNode.translation_[2]);
                    //Rotate
                    lray::Quaternion rotation(gltfNode.rotation_[3], gltfNode.rotation_[0], gltfNode.rotation_[1], gltfNode.rotation_[2]);
                    lray::Matrix44 rotMatrix;
                    rotation.getMatrix(rotMatrix);
                    matrix *= rotMatrix;
                    //Scale
                    lray::Matrix44 scaleMatrix;
                    scaleMatrix.identity();
                    scaleMatrix.setScale(gltfNode.scale_[0], gltfNode.scale_[1],  gltfNode.scale_[2]);
                    matrix *= scaleMatrix;
                }

                nodeArray.push_back(move(node));
            }

        }else{
            nodeArray.reserve(meshArray.size());
            for(s32 i=0; i<meshArray.size(); ++i){
                Node node("", -1, 0, -1, i);
                nodeArray.push_back(move(node));
            }
        }

        const Char* name = (0<gltf.scenes_.size())? gltf.scenes_[0].name_.c_str() : "";
        scene = move(Scene(name, move(meshArray), move(nodeArray)));
        LDELETE_ARRAY(directoryPath);
    }
}
