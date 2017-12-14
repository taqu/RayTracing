#ifndef INC_LRAY_VECTOR3_H_
#define INC_LRAY_VECTOR3_H_
/**
@file Vector3.h
@author t-sakai
@date 2017/12/13 create
*/
#include "lray.h"

namespace lray
{
    class Vector4;
    class Matrix34;
    class Matrix44;
    class Quaternion;

    //--------------------------------------------
    //---
    //--- Vector3
    //---
    //--------------------------------------------
    class Vector3
    {
    public:
        static const Vector3 Forward;
        static const Vector3 Backward;
        static const Vector3 Up;
        static const Vector3 Down;
        static const Vector3 Right;
        static const Vector3 Left;

        Vector3(){}
        explicit inline Vector3(f32 xyz);
        inline Vector3(f32 x, f32 y, f32 z);
        explicit Vector3(const Vector4& v);

        inline static Vector3 zero();
        inline static Vector3 one();

        inline void set(f32 x, f32 y, f32 z);
        void set(const Vector4& v);

        inline f32 operator[](s32 index) const;
        inline f32& operator[](s32 index);
        inline Vector3 operator-() const;

        inline Vector3& operator+=(const Vector3& v);
        inline Vector3& operator-=(const Vector3& v);

        inline Vector3& operator*=(f32 f);
        Vector3& operator/=(f32 f);

        Vector3& operator*=(const Vector3& v);
        Vector3& operator/=(const Vector3& v);

        inline bool equals(const Vector3& v) const;
        inline bool equals(const Vector3& v, f32 epsilon) const;
        inline bool operator==(const Vector3& v) const;
        inline bool operator!=(const Vector3& v) const;

        inline f32 length() const;
        inline f32 lengthSqr() const;

        inline void swap(Vector3& rhs);

        inline bool isNan() const;

        //--- Friend functions
        //--------------------------------------------------
        friend Vector3 operator+(const Vector3& v0, const Vector3& v1);
        friend Vector3 operator-(const Vector3& v0, const Vector3& v1);
        friend Vector3 operator*(f32 f, const Vector3& v);
        friend Vector3 operator*(const Vector3& v, f32 f);
        friend Vector3 operator*(const Vector3& v0, const Vector3& v1);
        friend Vector3&& operator/(const Vector3& v, f32 f);
        friend Vector3&& operator/(const Vector3& v0, const Vector3& v1);
        friend Vector3&& normalize(const Vector3& v);
        friend Vector3&& normalize(const Vector3& v, f32 lengthSqr);
        friend Vector3&& normalizeChecked(const Vector3& v);
        friend Vector3 absolute(const Vector3& v);

        friend f32 dot(const Vector3& v0, const Vector3& v1);
        friend f32 distanceSqr(const Vector3& v0, const Vector3& v1);
        friend f32 distance(const Vector3& v0, const Vector3& v1);

        friend Vector3 cross(const Vector3& v0, const Vector3& v1);

        /**
        @brief linear interporation 
        @param v0 ... 
        @param v1 ... 
        */
        friend Vector3&& lerp(const Vector3& v0, const Vector3& v1, f32 t);

        /**
        @brief linear interporation
        @param v0 ... 
        @param v1 ... 
        */
        friend Vector3 lerp(const Vector3& v0, const Vector3& v1, f32 t0, f32 t1);

        friend Vector3 mul(f32 f, const Vector3& v);
        friend Vector3&& mul(const Vector3& v, f32 f);

        friend Vector3&& mul(const Matrix34& m, const Vector3& v);
        friend Vector3&& mul(const Vector3& v, const Matrix34& m);

        friend Vector3&& mul33(const Matrix34& m, const Vector3& v);
        friend Vector3&& mul33(const Vector3& v, const Matrix34& m);

        friend Vector3&& mul33(const Matrix44& m, const Vector3& v);
        friend Vector3&& mul33(const Vector3& v, const Matrix44& m);

        friend Vector3 rotate(const Vector3& v, const Quaternion& rotation);
        friend Vector3 rotate(const Quaternion& rotation, const Vector3& v);

        friend Vector3 add(const Vector3& v0, const Vector3& v1);
        friend Vector3 sub(const Vector3& v0, const Vector3& v1);
        friend Vector3 mul(const Vector3& v0, const Vector3& v1);
        friend Vector3&& div(const Vector3& v0, const Vector3& v1);

        friend Vector3 minimum(const Vector3& v0, const Vector3& v1);
        friend Vector3 maximum(const Vector3& v0, const Vector3& v1);

        friend f32 minimum(const Vector3& v);
        friend f32 maximum(const Vector3& v);

        /**
        @brief v0*v1 + v2
        */
        friend Vector3&& muladd(const Vector3& v0, const Vector3& v1, const Vector3& v2);

        /**
        @brief x*v1 + v2
        */
        friend Vector3&& muladd(f32 x, const Vector3& v0, const Vector3& v1);

        //--- Members
        //--------------------------------------------------
        f32 x_;
        f32 y_;
        f32 z_;
    };

    static_assert(std::is_trivially_copyable<Vector3>::value == true, "Vector3 must be trivially copyable.");

    inline Vector3::Vector3(f32 xyz)
        :x_(xyz)
        ,y_(xyz)
        ,z_(xyz)
    {
    }

    inline Vector3::Vector3(f32 x, f32 y, f32 z)
        :x_(x)
        ,y_(y)
        ,z_(z)
    {
    }

    inline Vector3 Vector3::zero()
    {
        return {0.0f, 0.0f, 0.0f};
    }

    inline Vector3 Vector3::one()
    {
        return {1.0f, 1.0f, 1.0f};
    }

    inline void Vector3::set(f32 x, f32 y, f32 z)
    {
        x_ = x; y_ = y; z_ = z;
    }

    inline f32 Vector3::operator[](s32 index) const
    {
        LASSERT(0<=index && index < 3);
        return (&x_)[index];
    }

    inline f32& Vector3::operator[](s32 index)
    {
        LASSERT(0<=index && index < 3);
        return (&x_)[index];
    }

    inline Vector3 Vector3::operator-() const
    {
        return {-x_, -y_, -z_};
    }

    inline Vector3& Vector3::operator+=(const Vector3& v)
    {
        x_ += v.x_;
        y_ += v.y_;
        z_ += v.z_;
        return *this;
    }

    inline Vector3& Vector3::operator-=(const Vector3& v)
    {
        x_ -= v.x_;
        y_ -= v.y_;
        z_ -= v.z_;
        return *this;
    }

    inline Vector3& Vector3::operator*=(f32 f)
    {
        x_ *= f;
        y_ *= f;
        z_ *= f;
        return *this;
    }

    inline bool Vector3::equals(const Vector3& v) const
    {
        return lray::isEqual(x_, v.x_)
            && lray::isEqual(y_, v.y_)
            && lray::isEqual(z_, v.z_);
    }

    inline bool Vector3::equals(const Vector3& v, f32 epsilon) const
    {
        return lray::isEqual(x_, v.x_, epsilon)
            && lray::isEqual(y_, v.y_, epsilon)
            && lray::isEqual(z_, v.z_, epsilon);
    }

    inline bool Vector3::operator==(const Vector3& v) const
    {
        return equals(v);
    }

    inline bool Vector3::operator!=(const Vector3& v) const
    {
        return !equals(v);
    }

    inline f32 Vector3::length() const
    {
        return lray::sqrt( lengthSqr() );
    }

    inline f32 Vector3::lengthSqr() const
    {
        return ( x_ * x_ + y_ * y_ + z_ * z_);
    }

    inline void Vector3::swap(Vector3& rhs)
    {
        lray::swap(x_, rhs.x_);
        lray::swap(y_, rhs.y_);
        lray::swap(z_, rhs.z_);
    }

    inline bool Vector3::isNan() const
    {
        return (lray::isNan(x_) || lray::isNan(y_) || lray::isNan(z_));
    }

    Vector3 operator+(const Vector3& v0, const Vector3& v1);
    Vector3 operator-(const Vector3& v0, const Vector3& v1);
    Vector3 operator*(f32 f, const Vector3& v);
    Vector3 operator*(const Vector3& v, f32 f);
    Vector3 operator*(const Vector3& v0, const Vector3& v1);
    Vector3&& operator/(const Vector3& v, f32 f);
    Vector3&& operator/(const Vector3& v0, const Vector3& v1);

    Vector3&& normalize(const Vector3& v);
    Vector3&& normalize(const Vector3& v, f32 lengthSqr);
    Vector3&& normalizeChecked(const Vector3& v);

    Vector3 absolute(const Vector3& v);

    inline f32 dot(const Vector3& v0, const Vector3& v1)
    {
        return (v0.x_*v1.x_ + v0.y_*v1.y_ + v0.z_*v1.z_);
    }

    f32 distanceSqr(const Vector3& v0, const Vector3& v1);
    inline f32 distance(const Vector3& v0, const Vector3& v1)
    {
        return lray::sqrt(distanceSqr(v0,v1));
    }

    Vector3 cross(const Vector3& v0, const Vector3& v1);

    /**
    @brief 線形補間。v = (1-t)*v0 + t*v1
    @param v0 ...
    @param v1 ...
    */
    Vector3&& lerp(const Vector3& v0, const Vector3& v1, f32 t);

    /**
    @brief 線形補間。v = t1*v0 + t0*v1
    @param v0 ...
    @param v1 ...
    */
    Vector3 lerp(const Vector3& v0, const Vector3& v1, f32 t0, f32 t1);

    inline Vector3 mul(f32 f, const Vector3& v)
    {
        return f*v;
    }

    inline Vector3&& mul(const Vector3& v, f32 f)
    {
        return v*f;
    }

    Vector3&& mul(const Matrix34& m, const Vector3& v);
    Vector3&& mul(const Vector3& v, const Matrix34& m);

    Vector3&& mul33(const Matrix34& m, const Vector3& v);
    Vector3&& mul33(const Vector3& v, const Matrix34& m);

    Vector3&& mul33(const Matrix44& m, const Vector3& v);
    Vector3&& mul33(const Vector3& v, const Matrix44& m);

    Vector3 rotate(const Vector3& v, const Quaternion& rotation);
    Vector3 rotate(const Quaternion& rotation, const Vector3& v);


    inline Vector3 add(const Vector3& v0, const Vector3& v1)
    {
        return v0+v1;
    }

    inline Vector3 sub(const Vector3& v0, const Vector3& v1)
    {
        return v0-v1;
    }

    Vector3 mul(const Vector3& v0, const Vector3& v1);
    Vector3&& div(const Vector3& v0, const Vector3& v1);


    inline Vector3 minimum(const Vector3& v0, const Vector3& v1)
    {
        return {lray::minimum(v0.x_, v1.x_),
            lray::minimum(v0.y_, v1.y_),
            lray::minimum(v0.z_, v1.z_)};
    }

    inline Vector3 maximum(const Vector3& v0, const Vector3& v1)
    {
        return {lray::maximum(v0.x_, v1.x_),
            lray::maximum(v0.y_, v1.y_),
            lray::maximum(v0.z_, v1.z_)};
    }

    inline f32 minimum(const Vector3& v)
    {
        return lray::minimum(lray::minimum(v.x_, v.y_), v.z_);
    }

    inline f32 maximum(const Vector3& v)
    {
        return lray::maximum(lray::maximum(v.x_, v.y_), v.z_);
    }

    // v0*v1 + v2
    Vector3&& muladd(const Vector3& v0, const Vector3& v1, const Vector3& v2);

    // x*v1 + v2
    Vector3&& muladd(f32 x, const Vector3& v0, const Vector3& v1);
}
#endif //INC_LRAY_VECTOR3_H_
