/**
@file TriangleProxy.cpp
@author t-sakai
@date 2018/12/14 create
*/
#include "shape/TriangleProxy.h"
#include "shape/Primitive.h"
#include "math/Ray.h"
#include "math/RayTest.h"

namespace lray
{
#define GetVertices(v0,v1,v2)\
    const Triangle& triangle = primitive_->getTriangle(index_);\
    const Vector3& v0 = primitive_->getPosition(triangle.indices_[0]);\
    const Vector3& v1 = primitive_->getPosition(triangle.indices_[1]);\
    const Vector3& v2 = primitive_->getPosition(triangle.indices_[2])

    Vector3 TriangleProxy::getCentroid() const
    {
        GetVertices(v0,v1,v2);
        return (v0 + v1 + v2)* (1.0f/3.0f);
    }

    AABB TriangleProxy::getBBox() const
    {
        const Triangle& triangle = primitive_->getTriangle(index_);
        const Vector3& v0 = primitive_->getPosition(triangle.indices_[0]);

        Vector3 bmin = v0;
        Vector3 bmax = v0;
        for(s32 i=1; i<3; ++i){
            const Vector3& v = primitive_->getPosition(triangle.indices_[i]);
            bmin.x_ = minimum(bmin.x_, v.x_);
            bmin.y_ = minimum(bmin.y_, v.y_);
            bmin.z_ = minimum(bmin.z_, v.z_);

            bmax.x_ = maximum(bmax.x_, v.x_);
            bmax.y_ = maximum(bmax.y_, v.y_);
            bmax.z_ = maximum(bmax.z_, v.z_);
        }
        return AABB(bmin, bmax);
    }

    Result TriangleProxy::testRay(f32& t, f32& v, f32& w, const Ray& ray) const
    {
        GetVertices(v0,v1,v2);

        return testRayTriangleBoth(t, v, w, ray, v0, v1, v2);
    }
}
