#define CPPIMG_IMPLEMENTATION
#include <cppimg/cppimg.h>
#define CPPGLTF_IMPLEMENTATION
#include <cppgltf/cppgltf.h>
#include "lray.h"
#include "Camera.h"
#include "core/LString.h"
#include "core/Array.h"
#include "math/Ray.h"
#include "math/RayTest.h"
#include "math/Matrix44.h"
#include "math/Quaternion.h"
#include "core/ReferenceCounted.h"

#include "shape/Mesh.h"
#include "shape/Node.h"

using namespace lray;

namespace lray
{
    class Scene
    {
    public:
        typedef lray::Array<Mesh> MeshArray;
        typedef lray::Array<Node> NodeArray;

        Scene();
        Scene(Scene&& rhs);
        explicit Scene(const Char* name, MeshArray&& meshes, NodeArray&& nodes);

        void updateFrame();
        Result test(Intersection& intersection, Ray& ray);

        Scene& operator=(Scene&& rhs);
    private:
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        String name_;
        MeshArray meshes_;
        MeshArray refinedMeshes_;
        NodeArray nodes_;
    };

}

namespace lray
{
    //--- Scene
    //-----------------------------------------------------
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
        for(s32 i=0; i<refinedMeshes_.size(); ++i){
            Mesh& mesh = refinedMeshes_[i];
            mesh.test(intersection, ray);
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
    }
}

bool checkPrimitiveAttributes(cppgltf::Primitive& primitive)
{
    s32 flags = 0;
    s32 needs = (0x01U<<cppgltf::GLTF_ATTRIBUTE_POSITION
        | 0x01U<<cppgltf::GLTF_ATTRIBUTE_NORMAL);

    for(s32 i=0; i<primitive.attributes_.size(); ++i){
        cppgltf::Attribute& attribute = primitive.attributes_[i];
        switch(attribute.semanticType_){
        case cppgltf::GLTF_ATTRIBUTE_POSITION:
            flags |= (0x01U<<cppgltf::GLTF_ATTRIBUTE_POSITION);
            break;
        case cppgltf::GLTF_ATTRIBUTE_NORMAL:
            flags |= (0x01U<<cppgltf::GLTF_ATTRIBUTE_NORMAL);
            break;
        case cppgltf::GLTF_ATTRIBUTE_TANGENT:
            break;
        case cppgltf::GLTF_ATTRIBUTE_TEXCOORD:
            break;
        case cppgltf::GLTF_ATTRIBUTE_COLOR:
            break;
        case cppgltf::GLTF_ATTRIBUTE_JOINTS:
            break;
        case cppgltf::GLTF_ATTRIBUTE_WEIGHTS:
            break;
        }
    }
    return 0 != (flags & needs);
}

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

int main(int , char** )
{
    static const s32 Width = 800;
    static const s32 Height = 600;
    static const s32 Bpp = 3;
    u8* image = new u8[Width*Height*Bpp];

    Camera camera;
    camera.setResolution(Width, Height);
    camera.perspective(static_cast<f32>(Width)/Height, 60.0f*DEG_TO_RAD);
    camera.lookAt(Vector3(0.0f, 3.0f, 10.0f), Vector3(0.0f, 3.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));

    //Add object
    Scene scene;
    load(scene, "../../data/hatsune_miku_chibi_w_stand/scene.gltf");

    //Set light environment
    Vector3 lightDirection = normalize(Vector3(0.5f, 0.5f, 0.0f));

    //
    scene.updateFrame();

    //Loop over pixels
    ClockType startTime = getPerformanceCounter();
    for(s32 y=0; y<Height; ++y){
        for(s32 x=0; x<Width; ++x){
            Intersection intersection;
            Ray ray = camera.generateRay(static_cast<f32>(x), static_cast<f32>(y));
            u8 r,g,b;
            Result result = scene.test(intersection, ray);
            if(Result_Success & result){
                f32 d = maximum(dot(intersection.shadingNormal_, lightDirection), 0.0f);
                r = g = b = static_cast<u8>(minimum(clamp01(d)*256, 255.0f));
            }else{
                r = g = b = 128;
            }
            s32 pixel = (y*Width + x)*Bpp;
            image[pixel+0] = r;
            image[pixel+1] = g;
            image[pixel+2] = b;
        }
    }
    f64 elapsedTime = calcTime64(startTime, getPerformanceCounter());

    //Output
    cppimg::OFStream file;
    if(file.open("out.bmp")){
        cppimg::BMP::write(file, Width, Height, cppimg::ColorType_RGB, image);
        file.close();
    }
    delete[] image;

    printf("Render time %lf sec\n", elapsedTime);
    return 0;
}
