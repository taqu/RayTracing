/**
@file Mesh.cpp
@author t-sakai
@date 2018/05/24 create
*/
#include "shape/Mesh.h"
#include "math/RayTest.h"

namespace lray
{
    Mesh::Mesh()
    {}

    Mesh::Mesh(Mesh&& rhs)
        :primitives_(move(rhs.primitives_))
    {}

    Mesh::Mesh(PrimitiveArray&& primitives)
        :primitives_(move(primitives))
    {}

    Mesh::~Mesh()
    {}

    void Mesh::clear()
    {
        primitives_.clear();
    }

    Result Mesh::test(Intersection& intersection, Ray& ray)
    {
        for(s32 prim = 0; prim<primitives_.size(); ++prim){

            Primitive& primitive = primitives_[prim];
            for(s32 tri=0; tri<primitive.getNumTriangles(); ++tri){
                const Triangle& triangle = primitive.getTriangle(tri);
                const Vector3& p0 = primitive.getPosition(triangle.indices_[0]);
                const Vector3& p1 = primitive.getPosition(triangle.indices_[1]);
                const Vector3& p2 = primitive.getPosition(triangle.indices_[2]);

                f32 tmpT = F32_INFINITY;
                f32 tmpV;
                f32 tmpW;
                Result tmpResult = testRayTriangleBoth(tmpT, tmpV, tmpW, ray, p0, p1, p2);
                if(tmpResult != Result_Fail && tmpT<intersection.t_){
                    intersection.result_ = tmpResult;
                    ray.t_ = intersection.t_ = tmpT;
                    intersection.b0_ = 1.0f-tmpV-tmpW;
                    intersection.b1_ = tmpV;
                    intersection.b2_ = tmpW;

                    //Calc normal
                    f32 w0 = intersection.b0_;
                    f32 w1 = intersection.b1_;
                    f32 w2 = intersection.b2_;
                    const Vector3& n0 = primitive.getNormal(triangle.indices_[0]);
                    const Vector3& n1 = primitive.getNormal(triangle.indices_[1]);
                    const Vector3& n2 = primitive.getNormal(triangle.indices_[2]);
                    intersection.shadingNormal_ = weightedAverage(w0, w1, w2, n0, n1, n2);
                }//if(tmpResult
            }//for(s32 tri=0
        }//for(s32 prim = 0
        return intersection.result_;
    }

    void Mesh::refine(const Mesh& src, const lray::Matrix44& matrix)
    {
        primitives_.resize(src.primitives_.size());
        for(s32 i=0; i<primitives_.size(); ++i){
            primitives_[i].refine(src.primitives_[i], matrix);
        }
    }

    Mesh& Mesh::operator=(Mesh&& rhs)
    {
        if(this == &rhs){
            return *this;
        }
        primitives_ = move(rhs.primitives_);
        return *this;
    }
}
