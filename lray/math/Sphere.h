#ifndef INC_LRAY_SPHERE_H__
#define INC_LRAY_SPHERE_H__
/**
@file Sphere.h
@author t-sakai
@date 2017/12/18 create
*/
#include "../lray.h"

namespace lray
{
    class Vector3;
    class Vector4;

    /// Sphere
    class Sphere
    {
    public:
        Sphere()
        {}

        Sphere(const Vector3& position, f32 radius);

        Sphere(f32 x, f32 y, f32 z, f32 radius)
            :x_(x)
            ,y_(y)
            ,z_(z)
            ,r_(radius)
        {}

        void set(f32 x, f32 y, f32 z, f32 radius)
        {
            x_ = x;
            y_ = y;
            z_ = z;
            r_ = radius;
        }

        void zero()
        {
            _mm_storeu_ps(&x_, _mm_setzero_ps());
        }

        void setPosition(f32 x, f32 y, f32 z)
        {
            x_ = x;
            y_ = y;
            z_ = z;
        }

        void setPosition(const Vector3& position);
        void setPosition(const Vector4& position);

        void setRadius(f32 radius)
        {
            r_ = radius;
        }

        void translate(const Vector3& position);
        void translate(const Vector4& position);

        /**
        @brief 点に外接する球を計算
        */
        static Sphere circumscribed(const Vector3& p0, const Vector3& p1);

        /**
        @brief 点に外接する球を計算
        */
        static Sphere circumscribed(const Vector3& p0, const Vector3& p1, const Vector3& p2);

        /**
        @brief 点に外接する球を計算
        */
        static Sphere circumscribed(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);


        static Sphere calcMiniSphere(const Vector3* points, u32 numPoints);

        void combine(const Sphere& s0, const Sphere& s1);
        void add(const Sphere& s1){ combine(*this, s1);}
        void add(const Vector4& s1){ combine(*this, reinterpret_cast<const Sphere&>(s1));}

        f32 signedDistanceSqr(const Vector3& p) const;

        f32 distance(const Sphere& rhs) const;

        void swap(Sphere& rhs);

        const Vector3& position() const
        {
            return *((const Vector3*)&x_);
        }

        Vector3& position()
        {
            return *((Vector3*)&x_);
        }

        const f32& radius() const
        {
            return r_;
        }

        f32& radius()
        {
            return r_;
        }

        operator const Vector4&() const
        {
            return *((const Vector4*)&x_);
        }
        operator Vector4&()
        {
            return *((Vector4*)&x_);
        }

        void getAABB(lm128& bmin, lm128& bmax) const;

        f32 x_;
        f32 y_;
        f32 z_;
        f32 r_; //radius
    };

    Sphere lerp(const Sphere& s0, const Sphere& s1, f32 t);
}
#endif //INC_LRAY_SPHERE_H__
