/**
@file Vector4.cpp
@author t-sakai
@date 2017/12/13 create
*/
#include "math/Vector4.h"
#include "math/Vector3.h"
#include "math/Matrix34.h"
#include "math/Matrix44.h"
#include "math/Quaternion.h"

namespace lray
{
    const LALIGN16 f32 Vector4::Identity[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    const Vector4 Vector4::Forward = {0.0f, 0.0f, 1.0f, 0.0f};
    const Vector4 Vector4::Backward = {0.0f, 0.0f, -1.0f, 0.0f};
    const Vector4 Vector4::Up = {0.0f, 1.0f, 0.0f, 0.0f};
    const Vector4 Vector4::Down = {0.0f, -1.0f, 0.0f, 0.0f};
    const Vector4 Vector4::Right = {1.0f, 0.0f, 0.0f, 0.0f};
    const Vector4 Vector4::Left = {-1.0f, 0.0f, 0.0f, 0.0f};

    Vector4::Vector4(const Vector3& v)
        :x_(v.x_)
        ,y_(v.y_)
        ,z_(v.z_)
        ,w_(0.0f)
    {
    }

    Vector4::Vector4(const Vector3& v, f32 w)
        :x_(v.x_)
        ,y_(v.y_)
        ,z_(v.z_)
        ,w_(w)
    {
    }

    void Vector4::set(const Vector3& v)
    {
        x_ = v.x_;
        y_ = v.y_;
        z_ = v.z_;
        w_ = 0.0f;
    }

    void Vector4::set(const Vector3& v, f32 w)
    {
        x_ = v.x_;
        y_ = v.y_;
        z_ = v.z_;
        w_ = w;
    }

    bool Vector4::equals(const Vector4& v) const
    {
        return ( lray::isEqual(x_, v.x_)
            && lray::isEqual(y_, v.y_)
            && lray::isEqual(z_, v.z_)
            && lray::isEqual(w_, v.w_));
    }

    bool Vector4::equals(const Vector4& v, f32 epsilon) const
    {
        return ( lray::isEqual(x_, v.x_, epsilon)
            && lray::isEqual(y_, v.y_, epsilon)
            && lray::isEqual(z_, v.z_, epsilon)
            && lray::isEqual(w_, v.w_, epsilon));
    }

    void Vector4::setLength()
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = load(*this);
        r0 = _mm_mul_ps(r0, r0);
        r0 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0x4E), r0);
        r0 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0xB1), r0);

        r0 = _mm_sqrt_ss(r0);
        r0 = _mm_shuffle_ps(r0, r0, 0x00);
        store(*this, r0);
#else
        x_ = x_ * x_;
        y_ = y_ * y_;
        z_ = z_ * z_;
        w_ = w_ * w_;
#endif
    }

    f32 Vector4::length() const
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = load(*this);
        r0 = _mm_mul_ps(r0, r0);
        r0 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0x4E), r0);
        r0 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0xB1), r0);

        r0 = _mm_sqrt_ss(r0);
        f32 ret;
        _mm_store_ss(&ret, r0);
        return ret;
#else
        return lray::sqrt(lengthSqr());
#endif
    }

    f32 Vector4::lengthSqr() const
    {
        return (x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
    }

    //--- Friend functions
    //--------------------------------------------------
    Vector4&& normalize(const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v);
        lm128 r1 = r0;
        r0 = _mm_mul_ps(r0, r0);
        r0 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0x4E), r0);
        r0 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0xB1), r0);

        r0 = _mm_sqrt_ss(r0);
        r0 = _mm_shuffle_ps(r0, r0, 0);
        
#if 0
        r0 = rcp(r0);
        r1 = _mm_mul_ps(r1, r0);
#else
        r1 = _mm_div_ps(r1, r0);
#endif
        return move(Vector4(r1));
#else
        f32 l = v.lengthSqr();
        LASSERT(!lray::isZero(l));
        l = 1.0f/ lray::sqrt(l);
        return move(Vector4(v.x_*l, v.y_*l, v.z_*l, v.w_*l));
#endif
    }

    Vector4&& normalize(const Vector4& v, f32 lengthSqr)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = _mm_set1_ps(lengthSqr);
        lm128 r1 = Vector4::load(v);
        r0 = _mm_sqrt_ps(r0);

#if 0
        r0 = rcp(r0);
        r1 = _mm_mul_ps(r1, r0);
#else
        r1 = _mm_div_ps(r1, r0);
#endif
        return move(Vector4(r1));
#else
        LASSERT(!lray::isZero(lengthSqr));
        f32 l = 1.0f/ lray::sqrt(lengthSqr);
        return move(Vector4(v.x_*l, v.y_*l, v.z_*l, v.w_*l));
#endif
    }

    Vector4&& normalizeChecked(const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        f32 l = v.lengthSqr();
        if(lray::isZeroPositive(l)){
            return move(Vector4(_mm_setzero_ps()));
        }else{
            return normalize(v, l);
        }
#else
        f32 l = v.lengthSqr();
        if(lray::isZeroPositive(l)){
            return move(Vector4(0.0f));
        }else{
            return normalize(v, l);
        }
#endif
    }

    Vector4&& absolute(const Vector4& v)
    {
        return move(Vector4(_mm_andnot_ps(_mm_set1_ps(-0.0f), Vector4::load(v))));
    }

    f32 dot(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        r0 = _mm_mul_ps(r0, r1);
        r0 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0x4E), r0);
        r0 = _mm_add_ps( _mm_shuffle_ps(r0, r0, 0xB1), r0);

        f32 ret;
        _mm_store_ss(&ret, r0);
        return ret;
#else
        return (v0.x_ * v1.x_ + v0.y_ * v1.y_ + v0.z_ * v1.z_ + v0.w_ * v1.w_);
#endif
    }

    lm128 cross3(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 xv0 = Vector4::load(v0);
        lm128 xv1 = Vector4::load(v1);

        lm128 xv2 = _mm_shuffle_ps(xv1, xv1, 0xC9);

        lm128 tmp0 = _mm_mul_ps(xv0, xv2);
        xv0 = _mm_shuffle_ps(xv0, xv0, 0xC9);
        lm128 tmp1 = _mm_mul_ps(xv0, xv1);

        tmp0 = _mm_sub_ps(tmp0, tmp1);

        tmp0 = _mm_shuffle_ps(tmp0, tmp0, 0xC9);
        return tmp0;

#else
        f32 x = v0.y_ * v1.z_ - v0.z_ * v1.y_;
        f32 y = v0.z_ * v1.x_ - v0.x_ * v1.z_;
        f32 z = v0.x_ * v1.y_ - v0.y_ * v1.x_;
        return Vector4(x,y,z,0.0f);
#endif
    }

    f32 distanceSqr(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        lm128 d = _mm_sub_ps(r0, r1);

        d = _mm_mul_ps(d, d);
        d = _mm_add_ps( _mm_shuffle_ps(d, d, 0x4E), d);
        d = _mm_add_ps( _mm_shuffle_ps(d, d, 0xB1), d);

        f32 ret;
        _mm_store_ss(&ret, d);
        return ret;

#else
        const f32 dx = v0.x_ - v1.x_;
        const f32 dy = v0.y_ - v1.y_;
        const f32 dz = v0.z_ - v1.z_;
        const f32 dw = v0.w_ - v1.w_;
        return (dx * dx + dy * dy + dz * dz + dw * dw);
#endif
    }

    f32 distance(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        lm128 d = _mm_sub_ps(r0, r1);

        d = _mm_mul_ps(d, d);
        d = _mm_add_ps( _mm_shuffle_ps(d, d, 0x4E), d);
        d = _mm_add_ps( _mm_shuffle_ps(d, d, 0xB1), d);

        d = _mm_sqrt_ss(d);

        f32 ret;
        _mm_store_ss(&ret, d);
        return ret;
#else
        return lray::sqrt(distanceSqr(v0, v1));
#endif
    }
    f32 manhattanDistance(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        lm128 d = _mm_sub_ps(r0, r1);
        f32 f;
        *((u32*)&f) = 0x7FFFFFFFU;
        lm128 m = _mm_set1_ps(f);
        d = _mm_and_ps(d, m);

        d = _mm_add_ps( _mm_shuffle_ps(d, d, 0x4E), d);
        d = _mm_add_ps( _mm_shuffle_ps(d, d, 0xB1), d);

        f32 ret;
        _mm_store_ss(&ret, d);
        return ret;
#else
        Vector4 tmp = absolute(v0-v1);
        return tmp.x_ + tmp.y_ + tmp.z_ + tmp.w_;
#endif
    }

    f32 distanceSqr3(const Vector3& v0, const Vector4& v1)
    {
        const f32 dx = v0.x_ - v1.x_;
        const f32 dy = v0.y_ - v1.y_;
        const f32 dz = v0.z_ - v1.z_;
        return (dx * dx + dy * dy + dz * dz);
    }

    f32 distanceSqr3(const Vector4& v0, const Vector4& v1)
    {
        const f32 dx = v0.x_ - v1.x_;
        const f32 dy = v0.y_ - v1.y_;
        const f32 dz = v0.z_ - v1.z_;
        return ( dx * dx + dy * dy + dz * dz );
    }

    f32 distance3(const Vector3& v0, const Vector4& v1)
    {
        const f32 dx = v0.x_ - v1.x_;
        const f32 dy = v0.y_ - v1.y_;
        const f32 dz = v0.z_ - v1.z_;
        return lray::sqrt(dx * dx + dy * dy + dz * dz);
    }

    f32 distance3(const Vector4& v0, const Vector4& v1)
    {
        const f32 dx = v0.x_ - v1.x_;
        const f32 dy = v0.y_ - v1.y_;
        const f32 dz = v0.z_ - v1.z_;
        return lray::sqrt( dx * dx + dy * dy + dz * dz );
    }

    f32 manhattanDistance3(const Vector3& v0, const Vector4& v1)
    {
        const f32 dx = v0.x_ - v1.x_;
        const f32 dy = v0.y_ - v1.y_;
        const f32 dz = v0.z_ - v1.z_;;
        return absolute(dx) + absolute(dy) + absolute(dz);
    }

    f32 manhattanDistance3(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        lm128 d = _mm_sub_ps(r0, r1);
        f32 f;
        *((u32*)&f) = 0x7FFFFFFFU;
        lm128 m = _mm_set1_ps(f);
        d = _mm_and_ps(d, m);

        LALIGN16 f32 tmp[4];
        _mm_store_ps(tmp, d);
        return tmp[0] + tmp[1] + tmp[2];
#else
        const f32 dx = v0.x_ - v1.x_;
        const f32 dy = v0.y_ - v1.y_;
        const f32 dz = v0.z_ - v1.z_;;
        return absolute(dx) + absolute(dy) + absolute(dz);
#endif
    }

    Vector4&& mul(const Matrix34& m, const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 tm0 = _mm_loadu_ps(&m.m_[0][0]);
        lm128 tm1 = _mm_loadu_ps(&m.m_[1][0]);
        lm128 tm2 = _mm_loadu_ps(&m.m_[2][0]);
        lm128 tm3 = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

        _MM_TRANSPOSE4_PS(tm0, tm1, tm2, tm3);

        lm128 tv0 = _mm_set1_ps(v.x_);
        lm128 tv1 = _mm_set1_ps(v.y_);
        lm128 tv2 = _mm_set1_ps(v.z_);
        lm128 tv3 = _mm_set1_ps(v.w_);


        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);
        tm3 = _mm_mul_ps(tm3, tv3);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);
        tm0 = _mm_add_ps(tm0, tm3);

        return move(Vector4(tm0));
#else
        f32 x, y, z;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[0][1] + v.z_ * m.m_[0][2] + v.w_ * m.m_[0][3];
        y = v.x_ * m.m_[1][0] + v.y_ * m.m_[1][1] + v.z_ * m.m_[1][2] + v.w_ * m.m_[1][3];
        z = v.x_ * m.m_[2][0] + v.y_ * m.m_[2][1] + v.z_ * m.m_[2][2] + v.w_ * m.m_[2][3];
        return {x,y,z,v.w_};
#endif
    }

    Vector4&& mul(const Vector4& v, const Matrix34& m)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = _mm_set1_ps(v.x_);
        lm128 tv1 = _mm_set1_ps(v.y_);
        lm128 tv2 = _mm_set1_ps(v.z_);
        lm128 tv3 = _mm_set1_ps(v.w_);

        lm128 tm0 = _mm_loadu_ps(&m.m_[0][0]);
        lm128 tm1 = _mm_loadu_ps(&m.m_[1][0]);
        lm128 tm2 = _mm_loadu_ps(&m.m_[2][0]);
#if 0
        const f32 one = 1.0f;
        lm128 tm3 = _mm_load_ss(&one);
        tm3 = _mm_shuffle_ps(tm3, tm3, 0x2F);
#else
        f32 buffer[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        lm128 tm3 = _mm_loadu_ps(buffer);
#endif

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);
        tm3 = _mm_mul_ps(tm3, tv3);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);
        tm0 = _mm_add_ps(tm0, tm3);

        return move(Vector4(tm0));

#else
        f32 x, y, z, w;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[1][0] + v.z_ * m.m_[2][0];
        y = v.x_ * m.m_[0][1] + v.y_ * m.m_[1][1] + v.z_ * m.m_[2][1];
        z = v.x_ * m.m_[0][2] + v.y_ * m.m_[1][2] + v.z_ * m.m_[2][2];
        w = v.x_ * m.m_[0][3] + v.y_ * m.m_[1][3] + v.z_ * m.m_[2][3] + v.w_;

        return {x,y,z,w};
#endif
    }



    Vector4&& mul(const Matrix44& m, const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 tm0 = _mm_loadu_ps(&m.m_[0][0]);
        lm128 tm1 = _mm_loadu_ps(&m.m_[1][0]);
        lm128 tm2 = _mm_loadu_ps(&m.m_[2][0]);
        lm128 tm3 = _mm_loadu_ps(&m.m_[3][0]);

        _MM_TRANSPOSE4_PS(tm0, tm1, tm2, tm3);


        lm128 tv0 = _mm_set1_ps(v.x_);
        lm128 tv1 = _mm_set1_ps(v.y_);
        lm128 tv2 = _mm_set1_ps(v.z_);
        lm128 tv3 = _mm_set1_ps(v.w_);


        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);
        tm3 = _mm_mul_ps(tm3, tv3);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);
        tm0 = _mm_add_ps(tm0, tm3);

        return move(Vector4(tm0));

#elif defined(LMATH_USE_SSE)
        lm128 tm0 = _mm_loadu_ps(&m.m_[0][0]);
        lm128 tm1 = _mm_loadu_ps(&m.m_[1][0]);
        lm128 tm2 = _mm_loadu_ps(&m.m_[2][0]);
        lm128 tm3 = _mm_loadu_ps(&m.m_[3][0]);
        lm128 tv = _mm_loadu_ps(&v.x_);

        LALIGN16 f32 tmp[4];
        _mm_store_ps(tmp, _mm_mul_ps(tv, tm0));
        x_ = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        _mm_store_ps(tmp, _mm_mul_ps(tv, tm1));
        y_ = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        _mm_store_ps(tmp, _mm_mul_ps(tv, tm2));
        z_ = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        _mm_store_ps(tmp, _mm_mul_ps(tv, tm3));
        w_ = tmp[0] + tmp[1] + tmp[2] + tmp[3];
#else
        f32 x, y, z, w;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[0][1] + v.z_ * m.m_[0][2] + v.w_ * m.m_[0][3];
        y = v.x_ * m.m_[1][0] + v.y_ * m.m_[1][1] + v.z_ * m.m_[1][2] + v.w_ * m.m_[1][3];
        z = v.x_ * m.m_[2][0] + v.y_ * m.m_[2][1] + v.z_ * m.m_[2][2] + v.w_ * m.m_[2][3];
        w = v.x_ * m.m_[3][0] + v.y_ * m.m_[3][1] + v.z_ * m.m_[3][2] + v.w_ * m.m_[3][3];
        return {x,y,z,w};
#endif
    }

    Vector4&& mul(const Vector4& v, const Matrix44& m)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = _mm_set1_ps(v.x_);
        lm128 tv1 = _mm_set1_ps(v.y_);
        lm128 tv2 = _mm_set1_ps(v.z_);
        lm128 tv3 = _mm_set1_ps(v.w_);

        lm128 tm0 = _mm_loadu_ps(&m.m_[0][0]);
        lm128 tm1 = _mm_loadu_ps(&m.m_[1][0]);
        lm128 tm2 = _mm_loadu_ps(&m.m_[2][0]);
        lm128 tm3 = _mm_loadu_ps(&m.m_[3][0]);

        tm0 = _mm_mul_ps(tm0, tv0);
        tm1 = _mm_mul_ps(tm1, tv1);
        tm2 = _mm_mul_ps(tm2, tv2);
        tm3 = _mm_mul_ps(tm3, tv3);

        tm0 = _mm_add_ps(tm0, tm1);
        tm0 = _mm_add_ps(tm0, tm2);
        tm0 = _mm_add_ps(tm0, tm3);

        return move(Vector4(tm0));

#else
        f32 x, y, z, w;
        x = v.x_ * m.m_[0][0] + v.y_ * m.m_[1][0] + v.z_ * m.m_[2][0] + v.w_ * m.m_[3][0];
        y = v.x_ * m.m_[0][1] + v.y_ * m.m_[1][1] + v.z_ * m.m_[2][1] + v.w_ * m.m_[3][1];
        z = v.x_ * m.m_[0][2] + v.y_ * m.m_[1][2] + v.z_ * m.m_[2][2] + v.w_ * m.m_[3][2];
        w = v.x_ * m.m_[0][3] + v.y_ * m.m_[1][3] + v.z_ * m.m_[2][3] + v.w_ * m.m_[3][3];
        return{x,y,z,w};
#endif
    }

    Vector4&& mulPoint(const Matrix44& m, const Vector4& v)
    {
        Vector4 tv=v;
        tv.w_ = 1.0f;
        lm128 t0 = mul(m, tv);
        t0 = _mm_div_ps(t0, _mm_shuffle_ps(t0, t0, _MM_SHUFFLE(3,3,3,3)));
        Vector4::store(tv, t0);
        tv.w_ = v.w_;
        return move(tv);
    }

    Vector4&& mulVector(const Matrix44& m, const Vector4& v)
    {
        Vector4 tv=v;
        tv.w_ = 0.0f;
        lm128 t0 = mul(m, tv);
        Vector4::store(tv, t0);
        tv.w_ = v.w_;
        return move(tv);
    }

    Vector4 rotate(const Vector4& v, const Quaternion& rotation)
    {
        Quaternion conj = conjugate(rotation);
        Quaternion rot = mul(conj, v);
        rot = mul(rot, rotation);
        return {rot.x_, rot.y_, rot.z_, v.w_};
    }

    Vector4 rotate(const Quaternion& rotation, const Vector4& v)
    {
        Quaternion conj = conjugate(rotation);
        Quaternion rot = mul(conj, v);
        rot = mul(rot, rotation);
        return {rot.x_, rot.y_, rot.z_, v.w_};
    }

    /**
    @brief v0*v1 + v2
    */
    Vector4&& muladd(const Vector4& v0, const Vector4& v1, const Vector4& v2)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        lm128 r2 = Vector4::load(v2);
        r0 = _mm_mul_ps(r0, r1);
        r0 = _mm_add_ps(r0, r2);
        return move(Vector4(r0));

#else
        f32 x = v0.x_ * v1.x_ + v2.x_;
        f32 y = v0.y_ * v1.y_ + v2.y_;
        f32 z = v0.z_ * v1.z_ + v2.z_;
        f32 w = v0.w_ * v1.w_ + v2.w_;
        return {x,y,z,w};
#endif
    }

    /**
    @brief a*v0 + v1
    */
    Vector4&& muladd(f32 a, const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = _mm_set1_ps(a);
        lm128 r1 = Vector4::load(v0);
        lm128 r2 = Vector4::load(v1);
        r0 = _mm_mul_ps(r0, r1);
        r0 = _mm_add_ps(r0, r2);
        return move(Vector4(r0));

#else
        f32 x = a * v0.x_ + v1.x_;
        f32 y = a * v0.y_ + v1.y_;
        f32 z = a * v0.z_ + v1.z_;
        f32 w = a * v0.w_ + v1.w_;
        return {x,y,z,w};
#endif
    }

    Vector4&& floor(const Vector4& v)
    {
        lm128 tv0 = Vector4::load(v);
        lm128 tv1 = _mm_cvtepi32_ps(_mm_cvttps_epi32(tv0));

        return move(Vector4(_mm_sub_ps(tv1, _mm_and_ps(_mm_cmplt_ps(tv0, tv1), _mm_set1_ps(1.0f)))));
    }

    Vector4&& ceil(const Vector4& v)
    {
        lm128 tv0 = Vector4::load(v);
        lm128 tv1 = _mm_cvtepi32_ps(_mm_cvttps_epi32(tv0));

        return move(Vector4(_mm_add_ps(tv1, _mm_and_ps(_mm_cmplt_ps(tv0, tv1), _mm_set1_ps(1.0f)))));
    }

    Vector4&& lerp(const Vector4& v0, const Vector4& v1, f32 t)
    {
#ifdef LRAY_USE_SSE
        lm128 tv0 = Vector4::load(v0);
        lm128 tv1 = Vector4::load(v1);
        lm128 t0 = _mm_set1_ps(t);

        tv1 = _mm_sub_ps(tv1, tv0);
        tv1 = _mm_mul_ps(tv1, t0);
        tv1 = _mm_add_ps(tv1, tv0);
        return move(Vector4(tv1));

#else
        Vector4 tmp = v1-v0;
        tmp *= t;
        return tmp+v0;
#endif
    }

    Vector4&& slerp(const Vector4& v0, const Vector4& v1, f32 t)
    {
        f32 cosine = dot(v0, v1);
        if(F32_ANGLE_COSINE_ALMOST_ONE<=absolute(cosine)){
            return lerp(v0, v1, t);
        }else{
            return slerp(v0, v1, t, cosine);
        }
    }

    Vector4&& slerp(const Vector4& v0, const Vector4& v1, f32 t, f32 cosine)
    {
        LASSERT(cosine<F32_ANGLE_COSINE_ALMOST_ONE);

        f32 omega = lray::acos(cosine);

        f32 inv = 1.0f/lray::sqrt(1.0f-cosine*cosine);
        f32 s0 = lray::sinf((1.0f-t)*omega) * inv;
        f32 s1 = lray::sinf(t*omega) * inv;

        lm128 tv0 = Vector4::load(v0);
        lm128 tv1 = Vector4::load(v1);
        lm128 t0 = _mm_set1_ps(s0);
        lm128 t1 = _mm_set1_ps(s1);

        tv0 = _mm_mul_ps(t0, tv0);
        tv1 = _mm_mul_ps(t1, tv1);

        return move(Vector4(_mm_add_ps(tv0, tv1)));
    }
}
