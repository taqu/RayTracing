#ifndef INC_LRAY_PLANE_H__
#define INC_LRAY_PLANE_H__
/**
@file Plane.h
@author t-sakai
@date 2017/12/19 create
*/
#include "../lray.h"

namespace lray
{
    class Vector3;
    class Vector4;

    /// Plane
    class Plane
    {
    public:
        Plane()
        {}

        Plane(f32 nx, f32 ny, f32 nz, f32 d)
            :nx_(nx)
            ,ny_(ny)
            ,nz_(nz)
            ,d_(d)
        {}

        Plane(const Vector3& point, const Vector3& normal);

        f32 dot(f32 x, f32 y, f32 z) const
        {
            return (nx_*x + ny_*y + nz_*z + d_);
        }

        f32 dot(const Vector3& p) const;
        f32 dot(const Vector4& p) const;

        Vector3 normal() const;

        f32 d() const
        {
            return d_;
        }

        void normalize();

        void translate(f32 x, f32 y, f32 z);
        void translate(const Vector3& v);
        void translate(const Vector4& v);

        operator const Vector4&() const;
        operator Vector4&();

        f32 nx_; ///< plane's normal
        f32 ny_; ///< plane's normal
        f32 nz_; ///< plane's normal
        f32 d_; ///< distance from the origin to a plane
    };

    static_assert(std::is_trivially_copyable<Plane>::value == true, "Plane must be trivially copyable.");
}

#endif //INC_LRAY_PLANE_H__
