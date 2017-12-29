/**
@file Plane.cpp
@author t-sakai
@date 2017/12/19 create
*/
#include "math/Plane.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

namespace lray
{
    Plane::Plane(const Vector3& point, const Vector3& normal)
        :nx_(normal.x_)
        ,ny_(normal.y_)
        ,nz_(normal.z_)
        ,d_(-lray::dot(normal, point))
    {}

    Vector3 Plane::normal() const
    {
        return {nx_, ny_, nz_};
    }

    void Plane::normalize()
    {
#if defined(LRAY_USE_SSE)
        lm128 r0 = _mm_loadu_ps(&nx_);
        lm128 r1 = r0;
        r0 = _mm_mul_ps(r0, r0);
        lm128 r2 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0xE1), r0);
        r2 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0xE2), r2);

        r2 = _mm_sqrt_ss(r2);
        r2 = _mm_shuffle_ps(r2, r2, 0);

        r1 = _mm_div_ps(r1, r2);
        _mm_storeu_ps(&nx_, r1);
#else
        f32 l = v_.x_*v_.x_ + v_.y_*v_.y_ + v_.z_*v_.z_;

        LASSERT(!isEqual(l, 0.0f));
        l = 1.0f/ lray::sqrt(l);
        v_ *= l;
#endif
    }

    f32 Plane::dot(const Vector3& p) const
    {
        return (nx_*p.x_ + ny_*p.y_ + nz_*p.z_ + d_);
    }

    f32 Plane::dot(const Vector4& p) const
    {
        return lray::dot((const Vector4&)*this, p);
    }

    void Plane::translate(f32 x, f32 y, f32 z)
    {
        d_ -= (x*nx_ + y*ny_ + z*nz_);
    }

    void Plane::translate(const Vector3& v)
    {
        translate(v.x_, v.y_, v.z_);
    }

    void Plane::translate(const Vector4& v)
    {
        translate(v.x_, v.y_, v.z_);
    }

    Plane::operator const Vector4&() const
    {
        return *((const Vector4*)&nx_);
    }

    Plane::operator Vector4&()
    {
        return *((Vector4*)&nx_);
    }
}
