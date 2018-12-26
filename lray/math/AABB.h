#ifndef INC_LRAY_AABB_H_
#define INC_LRAY_AABB_H_
/**
@file AABB.h
@author t-sakai
@date 2018/12/14 create
*/
#include "lray.h"
#include "Vector3.h"

namespace lray
{
    class Ray;

    //--- AABB
    //--------------------------------------------------------------
    struct AABB
    {
        AABB()
        {}

        AABB(const Vector3& bmin, const Vector3& bmax)
            :bmin_(bmin)
            ,bmax_(bmax)
        {}

        void zero()
        {
            bmin_.zero();
            bmax_.zero();
        }

        void setInvalid()
        {
            bmin_.x_ = bmin_.y_ = bmin_.z_ = F32_MAX;
            bmax_.x_ = bmax_.y_ = bmax_.z_ = -F32_MAX;
        }

        Vector3 extent() const
        {
            return bmax_ - bmin_;
        }

        Vector3 diagonal() const
        {
            return Vector3(
                bmax_.x_-bmin_.x_,
                bmax_.y_-bmin_.y_,
                bmax_.z_-bmin_.z_);
        }

        void extend(const AABB& bbox);
        s32 maxExtentAxis() const;

        f32 halfArea() const;

        bool testRay(f32& tmin, f32& tmax, const Ray& ray) const;

        Vector3 bmin_;
        Vector3 bmax_;
    };
    static_assert(std::is_trivially_copyable<AABB>::value == true, "AABB must be trivially copyable.");
}

#endif //INC_LRAY_AABB_H_
