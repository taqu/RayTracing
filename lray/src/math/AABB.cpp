/**
@file AABB.cpp
@author t-sakai
@date 2018/12/14 create
*/
#include "math/AABB.h"
#include "math/RayTest.h"

namespace lray
{
    void AABB::extend(const AABB& bbox)
    {
        bmin_.x_ = minimum(bmin_.x_, bbox.bmin_.x_);
        bmin_.y_ = minimum(bmin_.y_, bbox.bmin_.y_);
        bmin_.z_ = minimum(bmin_.z_, bbox.bmin_.z_);

        bmax_.x_ = maximum(bmax_.x_, bbox.bmax_.x_);
        bmax_.y_ = maximum(bmax_.y_, bbox.bmax_.y_);
        bmax_.z_ = maximum(bmax_.z_, bbox.bmax_.z_);
    }

    s32 AABB::maxExtentAxis() const
    {
        Vector3 extent = bmax_ - bmin_;
        s32 axis = (extent.x_<extent.y_)? 1 : 0;
        axis = (extent.z_<extent[axis])? axis : 2;
        return axis;
    }

    f32 AABB::halfArea() const
    {
        f32 dx = bmax_.x_ - bmin_.x_;
        f32 dy = bmax_.y_ - bmin_.y_;
        f32 dz = bmax_.z_ - bmin_.z_;
        return (dx*dy + dy*dz + dz*dx);
    }

    bool AABB::testRay(f32& tmin, f32& tmax, const Ray& ray) const
    {
        return testRayAABB(tmin, tmax, ray, bmin_, bmax_);
    }
}
