#ifndef INC_LRAY_QUATERNION_H_
#define INC_LRAY_QUATERNION_H_
/**
@file Quaternion.h
@author t-sakai
@date 2017/12/13 create
*/
#include "lray.h"

namespace lray
{
    class Vector3;
    class Vector4;
    class Matrix34;
    class Matrix44;

    class Quaternion
    {
    public:
        Quaternion(){}
        inline Quaternion(f32 w, f32 x, f32 y, f32 z);
        inline static Quaternion&& identity();

        inline void set(f32 w, f32 x, f32 y, f32 z);
        void set(const Vector4& v);

        inline f32 operator[](s32 index) const;
        inline f32& operator[](s32 index);

        /// 単位四元数
        inline void setIdentity();

        void setRotateX(f32 radian);
        void setRotateY(f32 radian);
        void setRotateZ(f32 radian);

        void setRotateXYZ(f32 radx, f32 rady, f32 radz);
        void setRotateXZY(f32 radx, f32 rady, f32 radz);
        void setRotateYZX(f32 radx, f32 rady, f32 radz);
        void setRotateYXZ(f32 radx, f32 rady, f32 radz);
        void setRotateZXY(f32 radx, f32 rady, f32 radz);
        void setRotateZYX(f32 radx, f32 rady, f32 radz);

        void setRotateAxis(const Vector3& axis, f32 radian);
        void setRotateAxis(const Vector4& axis, f32 radian);
        void setRotateAxis(f32 x, f32 y, f32 z, f32 radian);

        void lookAt(const Vector3& eye, const Vector3& at);
        /**
        @param dir ... 正規化済み方向
        */
        void lookAt(const Vector3& dir);
        void lookAt(const Vector4& eye, const Vector4& at);
        /**
        @param dir ... 正規化済み方向
        */
        void lookAt(const Vector4& dir);

        void getDireciton(Vector4& dir) const;

        f32 length() const;
        inline f32 lengthSqr() const;
        Quaternion operator-() const;

        Quaternion& operator+=(const Quaternion& q);
        Quaternion& operator-=(const Quaternion& q);

        Quaternion& operator*=(f32 f);
        //
        Quaternion& operator*=(const Quaternion& q);

        f32 getRotationAngle() const;

        void getRotationAxis(Vector3& axis) const;
        void getMatrix(Matrix34& mat) const;
        void getMatrix(Matrix44& mat) const;

        void getEulerAngles(f32& x, f32& y, f32& z);

        inline bool isNan() const;

        void swap(Quaternion& rhs);

        static Quaternion rotateX(f32 radian);
        static Quaternion rotateY(f32 radian);
        static Quaternion rotateZ(f32 radian);

        static Quaternion rotateXYZ(f32 radx, f32 rady, f32 radz);
        static Quaternion rotateXZY(f32 radx, f32 rady, f32 radz);
        static Quaternion rotateYZX(f32 radx, f32 rady, f32 radz);
        static Quaternion rotateYXZ(f32 radx, f32 rady, f32 radz);
        static Quaternion rotateZXY(f32 radx, f32 rady, f32 radz);
        static Quaternion rotateZYX(f32 radx, f32 rady, f32 radz);

        static Quaternion rotateAxis(const Vector3& axis, f32 radian);
        static Quaternion rotateAxis(const Vector4& axis, f32 radian);
        static Quaternion rotateAxis(f32 x, f32 y, f32 z, f32 radian);

        friend Quaternion invert(const Quaternion& q);

        friend Quaternion conjugate(const Quaternion& q);

        friend Quaternion normalize(const Quaternion& q);
        friend Quaternion normalize(const Quaternion& q, f32 squaredLength);

        friend f32 dot(const Quaternion& q0, const Quaternion& q1);
        
        friend Quaternion exp(const Quaternion& q, f32 exponent);

        friend Quaternion mul(const Quaternion& q0, const Quaternion& q1);

        friend Quaternion mul(f32 a, const Quaternion& q);
        friend Quaternion mul(const Quaternion& q, f32 a);

        friend Quaternion mul(const Vector3& v, const Quaternion& q);
        friend Quaternion mul(const Quaternion& q, const Vector3& v);

        friend Quaternion mul(const Vector4& v, const Quaternion& q);
        friend Quaternion mul(const Quaternion& q, const Vector4& v);

        friend Quaternion rotateToward(const Vector4& from, const Vector4& to);

        /**
        @brief 線形補間。q = (1-t)*q1 + t*q2
        @param q0 ... 
        @param q1 ... 
        @param t ... 補間比
        */
        friend Quaternion lerp(const Quaternion& q0, const Quaternion& q1, f32 t);

        /**
        @brief 球面線形補間。q = (1-t)*q1 + t*q2
        @param q0 ... 
        @param q1 ... 
        @param t ... 補間比
        */
        friend Quaternion slerp(const Quaternion& q0, const Quaternion& q1, f32 t);

        friend Quaternion squad(const Quaternion& q0, const Quaternion& q1, const Quaternion& a, const Quaternion& b, f32 t);

        f32 w_;
        f32 x_;
        f32 y_;
        f32 z_;
    };

    static_assert(std::is_trivially_copyable<Quaternion>::value == true, "Quaternion must be trivially copyable.");

#ifdef LRAY_USE_SSE 
    //extern LALIGN16 u32 QuaternionConjugateMask_[4];
    extern LALIGN16 u32 QuaternionMulMask0_[4];
    extern LALIGN16 u32 QuaternionMulMask1_[4];
    extern LALIGN16 u32 QuaternionMulMask2_[4];
#endif

    inline Quaternion::Quaternion(f32 w, f32 x, f32 y, f32 z)
        :w_(w)
        ,x_(x)
        ,y_(y)
        ,z_(z)
    {
    }

    inline Quaternion&& Quaternion::identity()
    {
        Quaternion r;
        r.setIdentity();
        return move(r);
    }

    inline void Quaternion::set(f32 w, f32 x, f32 y, f32 z)
    {
        w_ = w; x_ = x; y_ = y; z_ = z;
    }

    inline f32 Quaternion::operator[](s32 index) const
    {
        LASSERT(0<=index && index<4);
        return (&w_)[index];
    }

    inline f32& Quaternion::operator[](s32 index)
    {
        LASSERT(0<=index && index<4);
        return (&w_)[index];
    }

    //---------------------------------------
    // 単位四元数
    inline void Quaternion::setIdentity()
    {
#ifdef LRAY_USE_SSE 
        const f32 one = 1.0f;
        lm128 t = _mm_load_ss(&one);
        _mm_storeu_ps(&w_, t);
#else
        w_ = 1.0f;
        x_ = y_ = z_ = 0.0f;
#endif
    }

    inline f32 Quaternion::lengthSqr() const
    {
        return (w_*w_ + x_*x_ + y_*y_ + z_*z_);
    }

    inline bool Quaternion::isNan() const
    {
        return (lray::isNan(w_) || lray::isNan(x_) || lray::isNan(y_) || lray::isNan(z_));
    }

    Quaternion invert(const Quaternion& q);

    inline Quaternion conjugate(const Quaternion& q)
    {
#if 0
        lm128 mask = _mm_loadu_ps((f32*)QuaternionConjugateMask_);

        lm128 r0 = load(*this);
        r0 = _mm_xor_ps(r0, mask);

        return store(r0);
#else
        return {q.w_, -q.x_, -q.y_, -q.z_};
#endif
    }

    Quaternion normalize(const Quaternion& q);
    Quaternion normalize(const Quaternion& q, f32 squaredLength);

    f32 dot(const Quaternion& q0, const Quaternion& q1);

    Quaternion exp(const Quaternion& q, f32 exponent);

    Quaternion mul(const Quaternion& q0, const Quaternion& q1);

    Quaternion mul(f32 x, const Quaternion& q);
    inline Quaternion mul(const Quaternion& q, f32 a)
    {
        mul(a, q);
    }

    Quaternion mul(const Vector3& v, const Quaternion& q);
    Quaternion mul(const Quaternion& q, const Vector3& v);

    Quaternion mul(const Vector4& v, const Quaternion& q);
    Quaternion mul(const Quaternion& q, const Vector4& v);

    Quaternion rotateToward(const Vector4& from, const Vector4& to);

    /**
    @brief 線形補間。q = (1-t)*q1 + t*q2
    @param q0 ...
    @param q1 ...
    @param t ... 補間比
    */
    Quaternion lerp(const Quaternion& q0, const Quaternion& q1, f32 t);

    /**
    @brief 球面線形補間。q = (1-t)*q1 + t*q2
    @param q0 ...
    @param q1 ...
    @param t ... 補間比
    */
    Quaternion slerp(const Quaternion& q0, const Quaternion& q1, f32 t);

    Quaternion squad(const Quaternion& q0, const Quaternion& q1, const Quaternion& a, const Quaternion& b, f32 t);

}

#endif //INC_LRAY_QUATERNION_H_
