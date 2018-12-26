/**
@file Vector3.cpp
@author t-sakai
@date 2017/12/13 create
*/
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix34.h"
#include "math/Matrix44.h"
#include "math/Quaternion.h"

namespace lray
{
namespace
{
    inline lm128 load(const Vector3& v)
    {
        return load3(&v.x_);
    }

    inline lm128 load(f32 x, f32 y, f32 z)
    {
        return set_m128(x, y, z, 1.0f);
    }

    inline lm128 load(f32 x, f32 y, f32 z, f32 w)
    {
        return set_m128(x, y, z, w);
    }

    inline lm128 load(f32 v)
    {
        return _mm_set1_ps(v);
    }

    inline void store(Vector3& v, const lm128& r)
    {
        store3(&v.x_, r);
    }

    inline Vector3 store(const lm128& r)
    {
        Vector3 v;
        store3(&v.x_, r);
        return move(v);
    }

    inline void store(Vector4& v, const lm128& r)
    {
        _mm_storeu_ps(&v.x_, r);
    }
}

    const Vector3 Vector3::Forward = {0.0f, 0.0f, 1.0f};
    const Vector3 Vector3::Backward= {0.0f, 0.0f, -1.0f};
    const Vector3 Vector3::Up = {0.0f, 1.0f, 0.0f};
    const Vector3 Vector3::Down = {0.0f, -1.0f, 0.0f};
    const Vector3 Vector3::Right = {1.0f, 0.0f, 0.0f};
    const Vector3 Vector3::Left = {-1.0f, 0.0f, 0.0f};

    Vector3::Vector3(const Vector4& v)
        :x_(v.x_)
        ,y_(v.y_)
        ,z_(v.z_)
    {
    }

    void Vector3::set(const lray::Vector4& v)
    {
        x_ = v.x_; y_ = v.y_; z_ = v.z_;
    }

    Vector3& Vector3::operator/=(f32 f)
    {
        LASSERT(!lray::isZero(f));

#ifdef LRAY_USE_SSE
        lm128 xv0 = load(*this);
        lm128 xv1 = load(f);
        lm128 xv2 = _mm_div_ps(xv0, xv1);
        store(*this, xv2);

#else
        f = 1.0f / f;
        x_ *= f;
        y_ *= f;
        z_ *= f;
#endif
        return *this;
    }

    Vector3& Vector3::operator*=(const Vector3& v)
    {
        x_ *= v.x_;
        y_ *= v.y_;
        z_ *= v.z_;
        return *this;
    }

    Vector3& Vector3::operator/=(const Vector3& v)
    {
        LASSERT(!lray::isZero(v.x_));
        LASSERT(!lray::isZero(v.y_));
        LASSERT(!lray::isZero(v.z_));

        x_ /= v.x_;
        y_ /= v.y_;
        z_ /= v.z_;
        return *this;
    }

    f32 Vector3::halfArea() const
    {
        return x_*y_ + y_*z_ + z_*x_;
    }

    //--- Friend functions
    //--------------------------------------------------
    Vector3 operator+(const Vector3& v0, const Vector3& v1)
    {
        return {v0.x_+v1.x_, v0.y_+v1.y_, v0.z_+v1.z_};
    }

    Vector3 operator-(const Vector3& v0, const Vector3& v1)
    {
        return {v0.x_-v1.x_, v0.y_-v1.y_, v0.z_-v1.z_};
    }

    Vector3 operator*(f32 f, const Vector3& v)
    {
        return {f*v.x_, f*v.y_, f*v.z_};
    }

    Vector3 operator*(const Vector3& v, f32 f)
    {
        return {v.x_*f, v.y_*f, v.z_*f};
    }

    Vector3 operator*(const Vector3& v0, const Vector3& v1)
    {
        return {v0.x_*v1.x_, v0.y_*v1.y_, v0.z_*v1.z_};
    }

    Vector3 operator/(const Vector3& v, f32 f)
    {
        LASSERT(!lray::isZero(f));

#ifdef LRAY_USE_SSE
        lm128 xv0 = load(v);
        lm128 xv1 = load(f);
        lm128 xv2 = _mm_div_ps(xv0, xv1);
        return store(xv2);
#else
        f = 1.0f / f;
        f32 x = v.x_*f;
        f32 y = v.y_*f;
        f32 z = v.z_*f;
        return Vector3(x,y,z);
#endif
    }

    Vector3 operator/(const Vector3& v0, const Vector3& v1)
    {
        LASSERT(!lray::isZero(v1.x_));
        LASSERT(!lray::isZero(v1.y_));
        LASSERT(!lray::isZero(v1.z_));

#ifdef LRAY_USE_SSE
        lm128 xv0 = load(v0);
        lm128 xv1 = load(v1);
        lm128 xv2 = _mm_div_ps(xv0, xv1);
        return store(xv2);
#else
        return Vector3(v0.x_/v1.x_, v0.y_/v1.y_, v0.z_/v1.z_);
#endif
    }

    Vector3 normalize(const Vector3& v)
    {
        f32 l = v.lengthSqr();
        LASSERT(!lray::isZero(l));

#ifdef LRAY_USE_SSE
        lm128 xv0 = load(v);
        lm128 xv1 = load(l);
        xv1 = _mm_sqrt_ps(xv1);
        lm128 xv2 = _mm_div_ps(xv0, xv1);
        return store(xv2);
#else
        l = 1.0f/ lray::sqrt(l);
        return Vector3(v.x_*l, v.y_*l, v.z_*l);
#endif
    }

    Vector3 normalize(const Vector3& v, f32 lengthSqr)
    {
        LASSERT(!lray::isZero(lengthSqr));

#ifdef LRAY_USE_SSE
        lm128 xv0 = load(v);
        lm128 xv1 = load(lengthSqr);
        xv1 = _mm_sqrt_ps(xv1);
        lm128 xv2 = _mm_div_ps(xv0, xv1);
        return store(xv2);
#else
        f32 l = 1.0f/ lray::sqrt(lengthSqr);
        return Vector3(v.x_*l, v.y_*l, v.z_*l);
#endif
    }

    Vector3 normalizeChecked(const Vector3& v)
    {
        f32 l = v.lengthSqr();
        if(lray::isZeroPositive(l)){
            return Vector3::zero();
        }else{
            return normalize(v, l);
        }
    }

    Vector3 absolute(const Vector3& v)
    {
        return {lray::absolute(v.x_), lray::absolute(v.y_), lray::absolute(v.z_)};
    }

    f32 distanceSqr(const Vector3& v0, const Vector3& v1)
    {
        const f32 dx = v0.x_ - v1.x_;
        const f32 dy = v0.y_ - v1.y_;
        const f32 dz = v0.z_ - v1.z_;
        return (dx*dx + dy*dy + dz*dz);
    }

    Vector3 cross(const Vector3& v0, const Vector3& v1)
    {
//#ifdef LRAY_USE_SSE
#if 0
        static const u32 RotMaskR = 201;
        static const u32 RotMaskL = 210;

        lm128 xv0 = load(v0.y_, v0.z_, v0.x_);
        lm128 xv1 = load(v1.z_, v1.x_, v1.y_);
        lm128 xv2 = _mm_mul_ps(xv0, xv1);

        xv0 = _mm_shuffle_ps(xv0, xv0, RotMaskR);
        xv1 = _mm_shuffle_ps(xv1, xv1, RotMaskL);

        lm128 xv3 = _mm_mul_ps(xv0, xv1);

        xv3 = _mm_sub_ps(xv2, xv3);
        Vector3 v;
        store(v, xv3);
        return v;

#else
        f32 x = v0.y_ * v1.z_ - v0.z_ * v1.y_;
        f32 y = v0.z_ * v1.x_ - v0.x_ * v1.z_;
        f32 z = v0.x_ * v1.y_ - v0.y_ * v1.x_;
        return {x,y,z};
#endif
    }

    Vector3 lerp(const Vector3& v0, const Vector3& v1, f32 t)
    {
        Vector3 tmp = {v1.x_-v0.x_, v1.y_-v0.y_, v1.z_-v0.z_};
        tmp.x_ = tmp.x_*t + v0.x_;
        tmp.y_ = tmp.y_*t + v0.y_;
        tmp.z_ = tmp.z_*t + v0.z_;
        return tmp;
    }

    Vector3 lerp(const Vector3& v0, const Vector3& v1, f32 t0, f32 t1)
    {
        Vector3 tmp0 = {v0.x_*t1, v0.y_*t1, v0.z_*t1};
        Vector3 tmp1 = {v1.x_*t0, v1.y_*t0, v1.z_*t0};
        return {tmp0.x_+tmp1.x_, tmp0.y_+tmp1.y_, tmp0.z_+tmp1.z_};
    }


    Vector3 mul(const Matrix34& m, const Vector3& v)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = load(v.x_);
        lm128 tv1 = load(v.y_);
        lm128 tv2 = load(v.z_);

        lm128 tm0 = load(m.m_[0][0], m.m_[1][0], m.m_[2][0]);
        lm128 tm1 = load(m.m_[0][1], m.m_[1][1], m.m_[2][1]);
        lm128 tm2 = load(m.m_[0][2], m.m_[1][2], m.m_[2][2]);
        lm128 tm3 = load(m.m_[0][3], m.m_[1][3], m.m_[2][3]);

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);
        tm0 = _mm_add_ps(tm0, tm3);

        return store(tm0);
#else
        f32 x,y,z;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[0][1] + v.z_ * m.m_[0][2] + m.m_[0][3];
        y = v.x_ * m.m_[1][0] + v.y_ * m.m_[1][1] + v.z_ * m.m_[1][2] + m.m_[1][3];
        z = v.x_ * m.m_[2][0] + v.y_ * m.m_[2][1] + v.z_ * m.m_[2][2] + m.m_[2][3];
        return {x,y,z};
#endif
    }

    Vector3 mul(const Vector3& v, const Matrix34& m)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = load(v.x_);
        lm128 tv1 = load(v.y_);
        lm128 tv2 = load(v.z_);

        lm128 tm0 = _mm_loadu_ps(&m.m_[0][0]);
        lm128 tm1 = _mm_loadu_ps(&m.m_[1][0]);
        lm128 tm2 = _mm_loadu_ps(&m.m_[2][0]);
        lm128 tm3 = load(m.m_[0][3], m.m_[1][3], m.m_[2][3]);

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);
        tm0 = _mm_add_ps(tm0, tm3);

        return store(tm0);
#else
        f32 x,y,z;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[1][0] + v.z_ * m.m_[2][0] + m.m_[0][3];
        y = v.x_ * m.m_[0][1] + v.y_ * m.m_[1][1] + v.z_ * m.m_[2][1] + m.m_[1][3];
        z = v.x_ * m.m_[0][2] + v.y_ * m.m_[1][2] + v.z_ * m.m_[2][2] + m.m_[2][3];
        return {x,y,z};
#endif
    }

    Vector3 mul33(const Matrix34& m, const Vector3& v)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = load(v.x_);
        lm128 tv1 = load(v.y_);
        lm128 tv2 = load(v.z_);

        lm128 tm0 = load(m.m_[0][0], m.m_[1][0], m.m_[2][0]);
        lm128 tm1 = load(m.m_[0][1], m.m_[1][1], m.m_[2][1]);
        lm128 tm2 = load(m.m_[0][2], m.m_[1][2], m.m_[2][2]);

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);

        return store(tm0);

#else
        f32 x,y,z;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[0][1] + v.z_ * m.m_[0][2];
        y = v.x_ * m.m_[1][0] + v.y_ * m.m_[1][1] + v.z_ * m.m_[1][2];
        z = v.x_ * m.m_[2][0] + v.y_ * m.m_[2][1] + v.z_ * m.m_[2][2];
        return {x,y,z};
#endif
    }

    Vector3 mul33(const Vector3& v, const Matrix34& m)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = load(v.x_);
        lm128 tv1 = load(v.y_);
        lm128 tv2 = load(v.z_);

        lm128 tm0 = _mm_loadu_ps(&m.m_[0][0]);
        lm128 tm1 = _mm_loadu_ps(&m.m_[1][0]);
        lm128 tm2 = _mm_loadu_ps(&m.m_[2][0]);

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);

        return store(tm0);

#else
        f32 x,y,z;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[1][0] + v.z_ * m.m_[2][0];
        y = v.x_ * m.m_[0][1] + v.y_ * m.m_[1][1] + v.z_ * m.m_[2][1];
        z = v.x_ * m.m_[0][2] + v.y_ * m.m_[1][2] + v.z_ * m.m_[2][2];
        return {x,y,z};
#endif
    }

    Vector3 mul(const Matrix44& m, const Vector3& v)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = load(v.x_);
        lm128 tv1 = load(v.y_);
        lm128 tv2 = load(v.z_);

        lm128 tm0 = load(m.m_[0][0], m.m_[1][0], m.m_[2][0], m.m_[3][0]);
        lm128 tm1 = load(m.m_[0][1], m.m_[1][1], m.m_[2][1], m.m_[3][1]);
        lm128 tm2 = load(m.m_[0][2], m.m_[1][2], m.m_[2][2], m.m_[3][2]);
        lm128 tm3 = load(m.m_[0][3], m.m_[1][3], m.m_[2][3], m.m_[3][3]);

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);
        tm0 = _mm_add_ps(tm0, tm3);

        Vector4 rv;
        store(rv, tm0);
        f32 iw = 1.0f/rv.w_;
        return {rv.x_*iw, rv.y_*iw, rv.z_*iw};
#else
        f32 x,y,z,w;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[0][1] + v.z_ * m.m_[0][2] + m.m_[0][3];
        y = v.x_ * m.m_[1][0] + v.y_ * m.m_[1][1] + v.z_ * m.m_[1][2] + m.m_[1][3];
        z = v.x_ * m.m_[2][0] + v.y_ * m.m_[2][1] + v.z_ * m.m_[2][2] + m.m_[2][3];
        w = v.x_ * m.m_[3][0] + v.y_ * m.m_[3][1] + v.z_ * m.m_[3][2] + m.m_[3][3];
        f32 iw = 1.0f/w;
        return {x*iw,y*iw,z*iw};
#endif
    }

    Vector3 mul(const Vector3& v, const Matrix44& m)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = load(v.x_);
        lm128 tv1 = load(v.y_);
        lm128 tv2 = load(v.z_);

        lm128 tm0 = _mm_loadu_ps(&m.m_[0][0]);
        lm128 tm1 = _mm_loadu_ps(&m.m_[1][0]);
        lm128 tm2 = _mm_loadu_ps(&m.m_[2][0]);
        lm128 tm3 = _mm_loadu_ps(&m.m_[3][0]);

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);
        tm0 = _mm_add_ps(tm0, tm3);

        Vector4 rv;
        store(rv, tm0);
        f32 iw = 1.0f/rv.w_;
        return {rv.x_*iw, rv.y_*iw, rv.z_*iw};
#else
        f32 x,y,z,w;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[1][0] + v.z_ * m.m_[2][0] + m.m_[3][0];
        y = v.x_ * m.m_[0][1] + v.y_ * m.m_[1][1] + v.z_ * m.m_[2][1] + m.m_[3][1];
        z = v.x_ * m.m_[0][2] + v.y_ * m.m_[1][2] + v.z_ * m.m_[2][2] + m.m_[3][2];
        w = v.x_ * m.m_[0][3] + v.y_ * m.m_[1][3] + v.z_ * m.m_[2][3] + m.m_[3][3];
        f32 iw = 1.0f/w;
        return {x*iw, y*iw, z*iw};
#endif
    }

    Vector3 mul33(const Matrix44& m, const Vector3& v)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = load(v.x_);
        lm128 tv1 = load(v.y_);
        lm128 tv2 = load(v.z_);

        lm128 tm0 = load(m.m_[0][0], m.m_[1][0], m.m_[2][0]);
        lm128 tm1 = load(m.m_[0][1], m.m_[1][1], m.m_[2][1]);
        lm128 tm2 = load(m.m_[0][2], m.m_[1][2], m.m_[2][2]);

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);

        return store(tm0);

#else
        f32 x,y,z;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[0][1] + v.z_ * m.m_[0][2];
        y = v.x_ * m.m_[1][0] + v.y_ * m.m_[1][1] + v.z_ * m.m_[1][2];
        z = v.x_ * m.m_[2][0] + v.y_ * m.m_[2][1] + v.z_ * m.m_[2][2];
        return {x,y,z};
#endif
    }

    Vector3 mul33(const Vector3& v, const Matrix44& m)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = load(v.x_);
        lm128 tv1 = load(v.y_);
        lm128 tv2 = load(v.z_);

        lm128 tm0 = _mm_loadu_ps(&m.m_[0][0]);
        lm128 tm1 = _mm_loadu_ps(&m.m_[1][0]);
        lm128 tm2 = _mm_loadu_ps(&m.m_[2][0]);

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);

        return store(tm0);

#else
        f32 x,y,z;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[1][0] + v.z_ * m.m_[2][0];
        y = v.x_ * m.m_[0][1] + v.y_ * m.m_[1][1] + v.z_ * m.m_[2][1];
        z = v.x_ * m.m_[0][2] + v.y_ * m.m_[1][2] + v.z_ * m.m_[2][2];
        return {x,y,z};
#endif
    }

    Vector3 rotate(const Vector3& v, const Quaternion& rotation)
    {
        //conjugate(Q) x V x Q
        Quaternion conj = conjugate(rotation);
        Quaternion rot = mul(conj, v);
        rot = mul(rot, rotation);

        return {rot.x_, rot.y_, rot.z_};
    }

    Vector3 rotate(const Quaternion& rotation, const Vector3& v)
    {
        //conjugate(Q) x V x Q
        Quaternion conj = conjugate(rotation);
        Quaternion rot = mul(conj, v);
        rot = mul(rot, rotation);

        return {rot.x_, rot.y_, rot.z_};
    }

    Vector3 mul(const Vector3& v0, const Vector3& v1)
    {
        return {v0.x_ * v1.x_, v0.y_ * v1.y_, v0.z_ * v1.z_};
    }

    Vector3 div(const Vector3& v0, const Vector3& v1)
    {
        LASSERT(!lray::isZero(v1.x_));
        LASSERT(!lray::isZero(v1.y_));
        LASSERT(!lray::isZero(v1.z_));

        lm128 xv0 = load(v0);
        lm128 xv1 = load(v1);
        lm128 xv2 = _mm_div_ps(xv0, xv1);
        return store(xv2);
    }

    // v0*v1 + v2
    Vector3 muladd(const Vector3& v0, const Vector3& v1, const Vector3& v2)
    {
        lm128 xv0 = load(v0);
        lm128 xv1 = load(v1);
        lm128 xv2 = load(v2);
        lm128 xv3 = _mm_add_ps(_mm_mul_ps(xv0, xv1), xv2);
        return store(xv3);
    }

    // x*v1 + v2
    Vector3 muladd(f32 x, const Vector3& v0, const Vector3& v1)
    {
        lm128 xv0 = load(x);
        lm128 xv1 = load(v0);
        lm128 xv2 = load(v1);
        lm128 xv3 = _mm_add_ps(_mm_mul_ps(xv0, xv1), xv2);
        return store(xv3);
    }

    Vector3 weightedAverage(f32 w0, f32 w1, f32 w2, const Vector3& v0, const Vector3& v1, const Vector3& v2)
    {
        f32 x = w0*v0.x_ + w1*v1.x_ + w2*v2.x_;
        f32 y = w0*v0.y_ + w1*v1.y_ + w2*v2.y_;
        f32 z = w0*v0.z_ + w1*v1.z_ + w2*v2.z_;
        return Vector3(x, y, z);
    }
}
