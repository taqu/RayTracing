/**
@file Matrix44.cpp
@author t-sakai
@date 2017/12/13 create
*/
#include "math/Matrix44.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix34.h"
#include "math/Quaternion.h"

namespace lray
{
#ifdef LRAY_USE_SSE
namespace
{
    //SSEZbgEXgA½ß
    inline void load(lm128& r0, lm128& r1, lm128& r2, lm128& r3, const Matrix44& m)
    {
        r0 = _mm_loadu_ps(&m.m_[0][0]);
        r1 = _mm_loadu_ps(&m.m_[1][0]);
        r2 = _mm_loadu_ps(&m.m_[2][0]);
        r3 = _mm_loadu_ps(&m.m_[3][0]);
    }

    inline void store(Matrix44& m, const lm128& r0, const lm128& r1, const lm128& r2, const lm128& r3)
    {
        _mm_storeu_ps(&m.m_[0][0], r0);
        _mm_storeu_ps(&m.m_[1][0], r1);
        _mm_storeu_ps(&m.m_[2][0], r2);
        _mm_storeu_ps(&m.m_[3][0], r3);
    }
}
#endif

    //--------------------------------------------
    //---
    //--- Matrix44
    //---
    //--------------------------------------------
    const Matrix44 Matrix44::identity_ = Matrix44(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);


//    Matrix44::Matrix44(const Matrix44& rhs)
//    {
//#ifdef LRAY_USE_SSE
//        lm128 r0 = _mm_loadu_ps(&rhs.m_[0][0]);
//        lm128 r1 = _mm_loadu_ps(&rhs.m_[1][0]);
//        lm128 r2 = _mm_loadu_ps(&rhs.m_[2][0]);
//        lm128 r3 = _mm_loadu_ps(&rhs.m_[3][0]);
//
//        _mm_storeu_ps(&m_[0][0], r0);
//        _mm_storeu_ps(&m_[1][0], r1);
//        _mm_storeu_ps(&m_[2][0], r2);
//        _mm_storeu_ps(&m_[3][0], r3);
//#else
//        ::memcpy(m_, rhs.m_, sizeof(Matrix44));
//#endif
//    }


    Matrix44::Matrix44(const Matrix34& rhs)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = _mm_loadu_ps(&rhs.m_[0][0]);
        lm128 r1 = _mm_loadu_ps(&rhs.m_[1][0]);
        lm128 r2 = _mm_loadu_ps(&rhs.m_[2][0]);

        _mm_storeu_ps(&m_[0][0], r0);
        _mm_storeu_ps(&m_[1][0], r1);
        _mm_storeu_ps(&m_[2][0], r2);
#else
        ::memcpy(m_, rhs.m_, sizeof(Matrix34));
#endif
        m_[3][0] = m_[3][1] = m_[3][2] = 0.0f;
        m_[3][3] = 1.0f;
    }

    Matrix44::Matrix44(
        f32 m00, f32 m01, f32 m02, f32 m03,
        f32 m10, f32 m11, f32 m12, f32 m13,
        f32 m20, f32 m21, f32 m22, f32 m23,
        f32 m30, f32 m31, f32 m32, f32 m33)
    {
        m_[0][0] = m00; m_[0][1] = m01; m_[0][2] = m02; m_[0][3] = m03;
        m_[1][0] = m10; m_[1][1] = m11; m_[1][2] = m12; m_[1][3] = m13;
        m_[2][0] = m20; m_[2][1] = m21; m_[2][2] = m22; m_[2][3] = m23;
        m_[3][0] = m30; m_[3][1] = m31; m_[3][2] = m32; m_[3][3] = m33;
    }

    void Matrix44::set(f32 m00, f32 m01, f32 m02, f32 m03,
        f32 m10, f32 m11, f32 m12, f32 m13,
        f32 m20, f32 m21, f32 m22, f32 m23,
        f32 m30, f32 m31, f32 m32, f32 m33)
    {
        m_[0][0] = m00; m_[0][1] = m01; m_[0][2] = m02; m_[0][3] = m03;
        m_[1][0] = m10; m_[1][1] = m11; m_[1][2] = m12; m_[1][3] = m13;
        m_[2][0] = m20; m_[2][1] = m21; m_[2][2] = m22; m_[2][3] = m23;
        m_[3][0] = m30; m_[3][1] = m31; m_[3][2] = m32; m_[3][3] = m33;
    }

    // lZbg
    void Matrix44::set(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = _mm_loadu_ps(&row0.x_);
        lm128 r1 = _mm_loadu_ps(&row1.x_);
        lm128 r2 = _mm_loadu_ps(&row2.x_);
        lm128 r3 = _mm_loadu_ps(&row3.x_);

        _mm_storeu_ps(&m_[0][0], r0);
        _mm_storeu_ps(&m_[1][0], r1);
        _mm_storeu_ps(&m_[2][0], r2);
        _mm_storeu_ps(&m_[3][0], r3);
#else
        ::memcpy(&m_[0][0], &row0, sizeof(Vector4));
        ::memcpy(&m_[1][0], &row1, sizeof(Vector4));
        ::memcpy(&m_[2][0], &row2, sizeof(Vector4));
        ::memcpy(&m_[3][0], &row3, sizeof(Vector4));
#endif
    }

    Matrix44& Matrix44::operator=(const Matrix34& rhs)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = _mm_loadu_ps(&rhs.m_[0][0]);
        lm128 r1 = _mm_loadu_ps(&rhs.m_[1][0]);
        lm128 r2 = _mm_loadu_ps(&rhs.m_[2][0]);

        _mm_storeu_ps(&m_[0][0], r0);
        _mm_storeu_ps(&m_[1][0], r1);
        _mm_storeu_ps(&m_[2][0], r2);
#else
        ::memcpy(m_, rhs.m_, sizeof(Matrix34));
#endif
        m_[3][0] = m_[3][1] = m_[3][2] = 0.0f;
        m_[3][3] = 1.0f;
        return *this;
    }


    Matrix44& Matrix44::operator*=(f32 f)
    {
#ifdef LRAY_USE_SSE
        lm128 r0, r1, r2, r3;
        load(r0, r1, r2, r3, *this);

        lm128 v = _mm_set1_ps(f);

        r0 = _mm_mul_ps(r0, v);
        r1 = _mm_mul_ps(r1, v);
        r2 = _mm_mul_ps(r2, v);
        r3 = _mm_mul_ps(r3, v);

        store(*this, r0, r1, r2, r3);

#else
        for(s32 i=0; i<4; ++i){
            for(s32 j=0; j<4; ++j){
                m_[i][j] *= f;
            }
        }
#endif
        return *this;
    }


    Matrix44& Matrix44::operator+=(const Matrix44& rhs)
    {
#ifdef LRAY_USE_SSE
        lm128 r0, r1, r2, r3;
        load(r0, r1, r2, r3, rhs);

        lm128 t0, t1, t2, t3;
        load(t0, t1, t2, t3, *this);

        t0 = _mm_add_ps(t0, r0);
        t1 = _mm_add_ps(t1, r1);
        t2 = _mm_add_ps(t2, r2);
        t3 = _mm_add_ps(t3, r3);

        store(*this, t0, t1, t2, t3);

#else
        for(s32 i=0; i<4; ++i){
            for(s32 j=0; j<4; ++j){
                m_[i][j] += rhs.m_[i][j];
            }
        }
#endif
        return *this;
    }

    Matrix44& Matrix44::operator-=(const Matrix44& rhs)
    {
#ifdef LRAY_USE_SSE
        lm128 r0, r1, r2, r3;
        load(r0, r1, r2, r3, rhs);

        lm128 t0, t1, t2, t3;
        load(t0, t1, t2, t3, *this);

        t0 = _mm_sub_ps(t0, r0);
        t1 = _mm_sub_ps(t1, r1);
        t2 = _mm_sub_ps(t2, r2);
        t3 = _mm_sub_ps(t3, r3);

        store(*this, t0, t1, t2, t3);

#else
        for(s32 i=0; i<4; ++i){
            for(s32 j=0; j<4; ++j){
                m_[i][j] -= rhs.m_[i][j];
            }
        }
#endif
        return *this;
    }

    Matrix44 Matrix44::operator-() const
    {
        Matrix44 ret;
#ifdef LRAY_USE_SSE
        f32 f;
        *((u32*)&f) = 0x80000000U;
        lm128 mask = _mm_set1_ps(f);
        lm128 r0, r1, r2, r3;
        load(r0, r1, r2, r3, *this);

        r0 = _mm_xor_ps(r0, mask);
        r1 = _mm_xor_ps(r1, mask);
        r2 = _mm_xor_ps(r2, mask);
        r3 = _mm_xor_ps(r3, mask);

        store(ret, r0, r1, r2, r3);

#else
        for(s32 i=0; i<4; ++i){
            for(s32 j=0; j<4; ++j){
                ret.m_[i][j] = -m_[i][j];
            }
        }
#endif
        return ret;
    }

    void Matrix44::mul(const Matrix44& m0, const Matrix44& m1)
    {
#ifdef LRAY_USE_SSE
        lm128 rm0 = _mm_loadu_ps(&(m1.m_[0][0]));
        lm128 rm1 = _mm_loadu_ps(&(m1.m_[1][0]));
        lm128 rm2 = _mm_loadu_ps(&(m1.m_[2][0]));
        lm128 rm3 = _mm_loadu_ps(&(m1.m_[3][0]));

        lm128 t0, t1, t2, t3;
        for(s32 c=0; c<4; ++c){
            t0 = _mm_load1_ps(&(m0.m_[c][0]));
            t1 = _mm_load1_ps(&(m0.m_[c][1]));
            t2 = _mm_load1_ps(&(m0.m_[c][2]));
            t3 = _mm_load1_ps(&(m0.m_[c][3]));

            t0 = _mm_mul_ps(t0, rm0);
            t1 = _mm_mul_ps(t1, rm1);
            t2 = _mm_mul_ps(t2, rm2);
            t3 = _mm_mul_ps(t3, rm3);

            t0 = _mm_add_ps(t0, t1);
            t0 = _mm_add_ps(t0, t2);
            t0 = _mm_add_ps(t0, t3);

            _mm_storeu_ps(&(m_[c][0]), t0);
        }

#else
        Matrix44 tmp = m1;

        f32 x, y, z, w;
        for(s32 c=0; c<4; ++c){
            x = m0.m_[c][0] * tmp.m_[0][0]
                + m0.m_[c][1] * tmp.m_[1][0]
                + m0.m_[c][2] * tmp.m_[2][0]
                + m0.m_[c][3] * tmp.m_[3][0];

            y = m0.m_[c][0] * tmp.m_[0][1]
                + m0.m_[c][1] * tmp.m_[1][1]
                + m0.m_[c][2] * tmp.m_[2][1]
                + m0.m_[c][3] * tmp.m_[3][1];

            z = m0.m_[c][0] * tmp.m_[0][2]
                + m0.m_[c][1] * tmp.m_[1][2]
                + m0.m_[c][2] * tmp.m_[2][2]
                + m0.m_[c][3] * tmp.m_[3][2];

            w = m0.m_[c][0] * tmp.m_[0][3]
                + m0.m_[c][1] * tmp.m_[1][3]
                + m0.m_[c][2] * tmp.m_[2][3]
                + m0.m_[c][3] * tmp.m_[3][3];

            m_[c][0] = x;
            m_[c][1] = y;
            m_[c][2] = z;
            m_[c][3] = w;
        }
#endif
    }

    void Matrix44::mul(const Matrix34& m0, const Matrix44& m1)
    {
#ifdef LRAY_USE_SSE
        lm128 rm0 = _mm_loadu_ps(&(m1.m_[0][0]));
        lm128 rm1 = _mm_loadu_ps(&(m1.m_[1][0]));
        lm128 rm2 = _mm_loadu_ps(&(m1.m_[2][0]));
        lm128 rm3 = _mm_loadu_ps(&(m1.m_[3][0]));

        lm128 t0, t1, t2, t3;
        for(s32 c=0; c<3; ++c){
            t0 = _mm_load1_ps(&(m0.m_[c][0]));
            t1 = _mm_load1_ps(&(m0.m_[c][1]));
            t2 = _mm_load1_ps(&(m0.m_[c][2]));
            t3 = _mm_load1_ps(&(m0.m_[c][3]));

            t0 = _mm_mul_ps(t0, rm0);
            t1 = _mm_mul_ps(t1, rm1);
            t2 = _mm_mul_ps(t2, rm2);
            t3 = _mm_mul_ps(t3, rm3);

            t0 = _mm_add_ps(t0, t1);
            t0 = _mm_add_ps(t0, t2);
            t0 = _mm_add_ps(t0, t3);

            _mm_storeu_ps(&(m_[c][0]), t0);
        }
        _mm_storeu_ps(&(m_[3][0]), rm3);

#else
        Matrix44 tmp = m1;

        f32 x, y, z, w;
        for(s32 c=0; c<3; ++c){
            x = m0.m_[c][0] * tmp.m_[0][0]
                + m0.m_[c][1] * tmp.m_[1][0]
                + m0.m_[c][2] * tmp.m_[2][0]
                + m0.m_[c][3] * tmp.m_[3][0];

            y = m0.m_[c][0] * tmp.m_[0][1]
                + m0.m_[c][1] * tmp.m_[1][1]
                + m0.m_[c][2] * tmp.m_[2][1]
                + m0.m_[c][3] * tmp.m_[3][1];

            z = m0.m_[c][0] * tmp.m_[0][2]
                + m0.m_[c][1] * tmp.m_[1][2]
                + m0.m_[c][2] * tmp.m_[2][2]
                + m0.m_[c][3] * tmp.m_[3][2];

            w = m0.m_[c][0] * tmp.m_[0][3]
                + m0.m_[c][1] * tmp.m_[1][3]
                + m0.m_[c][2] * tmp.m_[2][3]
                + m0.m_[c][3] * tmp.m_[3][3];

            m_[c][0] = x;
            m_[c][1] = y;
            m_[c][2] = z;
            m_[c][3] = w;
        }

        m_[3][0] = tmp.m_[3][0];
        m_[3][1] = tmp.m_[3][1];
        m_[3][2] = tmp.m_[3][2];
        m_[3][3] = tmp.m_[3][3];
#endif
    }

    void Matrix44::mul(const Matrix44& m0, const Matrix34& m1)
    {
#ifdef LRAY_USE_SSE
        lm128 rm0 = _mm_loadu_ps(&(m1.m_[0][0]));
        lm128 rm1 = _mm_loadu_ps(&(m1.m_[1][0]));
        lm128 rm2 = _mm_loadu_ps(&(m1.m_[2][0]));

        lm128 t0, t1, t2, t3;
        for(s32 c=0; c<4; ++c){
            t0 = _mm_load1_ps(&(m0.m_[c][0]));
            t1 = _mm_load1_ps(&(m0.m_[c][1]));
            t2 = _mm_load1_ps(&(m0.m_[c][2]));
            t3 = _mm_load_ss(&(m0.m_[c][3]));
            t3 = _mm_shuffle_ps(t3, t3, 0x3F);

            t0 = _mm_mul_ps(t0, rm0);
            t1 = _mm_mul_ps(t1, rm1);
            t2 = _mm_mul_ps(t2, rm2);

            t0 = _mm_add_ps(t0, t1);
            t0 = _mm_add_ps(t0, t2);
            t0 = _mm_add_ps(t0, t3);

            _mm_storeu_ps(&(m_[c][0]), t0);
        }

#else

        f32 x, y, z, w;
        for(s32 c=0; c<4; ++c){
            x = m0.m_[c][0] * m1.m_[0][0]
                + m0.m_[c][1] * m1.m_[1][0]
                + m0.m_[c][2] * m1.m_[2][0];

            y = m0.m_[c][0] * m1.m_[0][1]
                + m0.m_[c][1] * m1.m_[1][1]
                + m0.m_[c][2] * m1.m_[2][1];

            z = m0.m_[c][0] * m1.m_[0][2]
                + m0.m_[c][1] * m1.m_[1][2]
                + m0.m_[c][2] * m1.m_[2][2];

            w = m0.m_[c][0] * m1.m_[0][3]
                + m0.m_[c][1] * m1.m_[1][3]
                + m0.m_[c][2] * m1.m_[2][3]
                + m0.m_[c][3];

            m_[c][0] = x;
            m_[c][1] = y;
            m_[c][2] = z;
            m_[c][3] = w;
        }
#endif
    }

    void Matrix44::zero()
    {
#ifdef LRAY_USE_SSE
        lm128 zero = _mm_setzero_ps();
        _mm_storeu_ps(&(m_[0][0]), zero);
        _mm_storeu_ps(&(m_[1][0]), zero);
        _mm_storeu_ps(&(m_[2][0]), zero);
        _mm_storeu_ps(&(m_[3][0]), zero);
#else
        m_[0][0] = 0.0f; m_[0][1] = 0.0f; m_[0][2] = 0.0f; m_[0][3] = 0.0f;
        m_[1][0] = 0.0f; m_[1][1] = 0.0f; m_[1][2] = 0.0f; m_[1][3] = 0.0f;
        m_[2][0] = 0.0f; m_[2][1] = 0.0f; m_[2][2] = 0.0f; m_[2][3] = 0.0f;
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 0.0f;
#endif
    }

    void Matrix44::identity()
    {
#ifdef LRAY_USE_SSE
        f32 one = 1.0f;
        static const u32 rotmask = 147; //ãÊûüÖñ]
        lm128 t = _mm_load_ss(&one);
        _mm_storeu_ps(&(m_[0][0]), t);

        t = _mm_shuffle_ps(t, t, rotmask);
        _mm_storeu_ps(&(m_[1][0]), t);

        t = _mm_shuffle_ps(t, t, rotmask);
        _mm_storeu_ps(&(m_[2][0]), t);

        t = _mm_shuffle_ps(t, t, rotmask);
        _mm_storeu_ps(&(m_[3][0]), t);
#else
        m_[0][0] = 1.0f; m_[0][1] = 0.0f; m_[0][2] = 0.0f; m_[0][3] = 0.0f;
        m_[1][0] = 0.0f; m_[1][1] = 1.0f; m_[1][2] = 0.0f; m_[1][3] = 0.0f;
        m_[2][0] = 0.0f; m_[2][1] = 0.0f; m_[2][2] = 1.0f; m_[2][3] = 0.0f;
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
#endif
    }

    void Matrix44::getTranspose(Matrix44& dst) const
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = _mm_loadu_ps(&m_[0][0]);
        lm128 r1 = _mm_loadu_ps(&m_[1][0]);
        lm128 r2 = _mm_loadu_ps(&m_[2][0]);
        lm128 r3 = _mm_loadu_ps(&m_[3][0]);

        _MM_TRANSPOSE4_PS(r0, r1, r2, r3);

        _mm_storeu_ps(&dst.m_[0][0], r0);
        _mm_storeu_ps(&dst.m_[1][0], r1);
        _mm_storeu_ps(&dst.m_[2][0], r2);
        _mm_storeu_ps(&dst.m_[3][0], r3);

#else
        dst = *this;
        lray::swap(dst.m_[0][1], dst.m_[1][0]);
        lray::swap(dst.m_[0][2], dst.m_[2][0]);
        lray::swap(dst.m_[0][3], dst.m_[3][0]);
        lray::swap(dst.m_[1][2], dst.m_[2][1]);
        lray::swap(dst.m_[1][3], dst.m_[3][1]);
        lray::swap(dst.m_[2][3], dst.m_[3][2]);
#endif
    }

    void Matrix44::transpose(const Matrix44& src)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = _mm_loadu_ps(&src.m_[0][0]);
        lm128 r1 = _mm_loadu_ps(&src.m_[1][0]);
        lm128 r2 = _mm_loadu_ps(&src.m_[2][0]);
        lm128 r3 = _mm_loadu_ps(&src.m_[3][0]);

        _MM_TRANSPOSE4_PS(r0, r1, r2, r3);

        _mm_storeu_ps(&m_[0][0], r0);
        _mm_storeu_ps(&m_[1][0], r1);
        _mm_storeu_ps(&m_[2][0], r2);
        _mm_storeu_ps(&m_[3][0], r3);

#else
        for(u32 i=0; i<4; ++i){
            for(u32 j=0; j<4; ++j){
                m_[i][j] = src.m_[j][i];
            }
        }
#endif
    }

    f32 Matrix44::determinant() const
    {
//#ifdef LRAY_USE_SSE
#if 0
        lm128 det;

        lm128 c0 = _mm_loadu_ps(&m_[0][0]);
        lm128 c1 = _mm_loadu_ps(&m_[1][0]);
        lm128 c2 = _mm_loadu_ps(&m_[2][0]);
        lm128 c3 = _mm_loadu_ps(&m_[3][0]);

        lm128 t0 = _mm_shuffle_ps(c1, c1, 0x01);//11 10 10 10
        lm128 t1 = _mm_shuffle_ps(c2, c2, 0x9E);//22 23 21 22
        lm128 t2 = _mm_shuffle_ps(c3, c3, 0x6A);//33 32 32 31

        det = _mm_mul_ps(t0, t1);
        det = _mm_mul_ps(det, t2);


        t0 = _mm_shuffle_ps(c1, c1, 0x5A);//12 12 11 11
        t1 = _mm_shuffle_ps(c2, c2, 0x33);//23 20 23 20
        t2 = _mm_shuffle_ps(c3, c3, 0x87);//31 33 30 32

        t0 = _mm_mul_ps(t0, t1);
        t0 = _mm_mul_ps(t0, t2);
        det = _mm_add_ps(det, t0);


        t0 = _mm_shuffle_ps(c1, c1, 0xBF);//13 13 13 12
        t1 = _mm_shuffle_ps(c2, c2, 0x49);//21 22 20 21
        t2 = _mm_shuffle_ps(c3, c3, 0x21);//32 30 31 30

        t0 = _mm_mul_ps(t0, t1);
        t0 = _mm_mul_ps(t0, t2);
        det = _mm_add_ps(det, t0);


        t0 = _mm_shuffle_ps(c1, c1, 0x01);//11 10 10 10
        t1 = _mm_shuffle_ps(c2, c2, 0x7B);//23 22 23 21
        t2 = _mm_shuffle_ps(c3, c3, 0x9E);//32 33 31 32

        t0 = _mm_mul_ps(t0, t1);
        t0 = _mm_mul_ps(t0, t2);
        det = _mm_sub_ps(det, t0);


        t0 = _mm_shuffle_ps(c1, c1, 0x5A);//12 12 11 11
        t1 = _mm_shuffle_ps(c2, c2, 0x87);//21 23 20 22
        t2 = _mm_shuffle_ps(c3, c3, 0x33);//33 30 33 30

        t0 = _mm_mul_ps(t0, t1);
        t0 = _mm_mul_ps(t0, t2);
        det = _mm_sub_ps(det, t0);


        t0 = _mm_shuffle_ps(c1, c1, 0xBF);//13 13 13 12
        t1 = _mm_shuffle_ps(c2, c2, 0x12);//22 20 21 20
        t2 = _mm_shuffle_ps(c3, c3, 0x49);//31 32 30 31

        t0 = _mm_mul_ps(t0, t1);
        t0 = _mm_mul_ps(t0, t2);
        det = _mm_sub_ps(det, t0);

        det = _mm_mul_ps(det, c0);

        LALIGN16 f32 buffer[4];
        _mm_store_ps(buffer, det);

        return (buffer[0] + buffer[1] + buffer[2] + buffer[3]);
#else
        f32 tmp[4];
        tmp[0] = m_[0][0] * (m_[1][1]*m_[2][2]*m_[3][3] + m_[1][2]*m_[2][3]*m_[3][1] + m_[1][3]*m_[2][1]*m_[3][2]
                           - m_[1][1]*m_[2][3]*m_[3][2] - m_[1][2]*m_[2][1]*m_[3][3] - m_[1][3]*m_[2][2]*m_[3][1]);

        tmp[1] = m_[0][1] * (m_[1][0]*m_[2][3]*m_[3][2] + m_[1][2]*m_[2][0]*m_[3][3] + m_[1][3]*m_[2][2]*m_[3][0]
                           - m_[1][0]*m_[2][2]*m_[3][3] - m_[1][2]*m_[2][3]*m_[3][0] - m_[1][3]*m_[2][0]*m_[3][2]);

        tmp[2] = m_[0][2] * (m_[1][0]*m_[2][1]*m_[3][2] + m_[1][1]*m_[2][3]*m_[3][0] + m_[1][3]*m_[2][0]*m_[3][1]
                           - m_[1][0]*m_[2][3]*m_[3][1] - m_[1][1]*m_[2][0]*m_[3][3] - m_[1][3]*m_[2][1]*m_[3][0]);

        tmp[3] = m_[0][3] * (m_[1][0]*m_[2][2]*m_[3][1] + m_[1][1]*m_[2][0]*m_[3][2] + m_[1][2]*m_[2][1]*m_[3][0]
                           - m_[1][0]*m_[2][1]*m_[3][2] - m_[1][1]*m_[2][2]*m_[3][0] - m_[1][2]*m_[2][0]*m_[3][1]);

        return (tmp[0] + tmp[1] + tmp[2] + tmp[3]);
#endif
    }

    void Matrix44::getInvert(Matrix44& dst) const
    {
#ifdef LRAY_USE_SSE


        // -----------------------------------------------
        lm128 m0, m1, m2, m3;
        lm128 r0, r1, r2, r3;

        lm128 tmp;

        tmp = _mm_setzero_ps();
        r1 = tmp;
        r3 = tmp;

        tmp = _mm_loadh_pi(_mm_loadl_pi(tmp, (lm64*)&m_[0][0]), (lm64*)&m_[1][0]);
        r1 = _mm_loadh_pi(_mm_loadl_pi(r1, (lm64*)&m_[2][0]), (lm64*)&m_[3][0]);
        r0 = _mm_shuffle_ps(tmp, r1, 0x88);
        r1 = _mm_shuffle_ps(r1, tmp, 0xDD);
        tmp = _mm_loadh_pi(_mm_loadl_pi(tmp, (lm64*)&m_[0][2]), (lm64*)&m_[1][2]);
        r3 = _mm_loadh_pi(_mm_loadl_pi(r3, (lm64*)&m_[2][2]), (lm64*)&m_[3][2]);
        r2 = _mm_shuffle_ps(tmp, r3, 0x88);
        r3 = _mm_shuffle_ps(r3, tmp, 0xDD);

        // -----------------------------------------------
        tmp = _mm_mul_ps(r2, r3);
        tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
        m0 = _mm_mul_ps(r1, tmp);
        m1 = _mm_mul_ps(r0, tmp);
        tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
        m0 = _mm_sub_ps( _mm_mul_ps(r1, tmp), m0);
        m1 = _mm_sub_ps( _mm_mul_ps(r0, tmp), m1);
        m1 = _mm_shuffle_ps(m1, m1, 0x4E);

        // -----------------------------------------------
        tmp = _mm_mul_ps(r1, r2);
        tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
        m0 = _mm_add_ps( _mm_mul_ps(r3, tmp), m0);
        m3 = _mm_mul_ps(r0, tmp);
        tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
        m0 = _mm_sub_ps(m0, _mm_mul_ps(r3, tmp));
        m3 = _mm_sub_ps( _mm_mul_ps(r0, tmp), m3);
        m3 = _mm_shuffle_ps(m3, m3, 0x4E);

        // -----------------------------------------------
        tmp = _mm_mul_ps( _mm_shuffle_ps(r1, r1, 0x4E), r3);
        tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
        r2 = _mm_shuffle_ps(r2, r2, 0x4E);
        m0 = _mm_add_ps( _mm_mul_ps(r2, tmp), m0);
        m2 = _mm_mul_ps(r0, tmp);
        tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
        m0 = _mm_sub_ps(m0, _mm_mul_ps(r2, tmp));
        m2 = _mm_sub_ps( _mm_mul_ps(r0, tmp), m2);
        m2 = _mm_shuffle_ps(m2, m2, 0x4E);

        // -----------------------------------------------
        tmp = _mm_mul_ps(r0, r1);
        tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
        m2 = _mm_add_ps( _mm_mul_ps(r3, tmp), m2);
        m3 = _mm_sub_ps( _mm_mul_ps(r2, tmp), m3);
        tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
        m2 = _mm_sub_ps( _mm_mul_ps(r3, tmp), m2);
        m3 = _mm_sub_ps(m3, _mm_mul_ps(r2, tmp));

        // -----------------------------------------------
        tmp = _mm_mul_ps(r0, r3);
        tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
        m1 = _mm_sub_ps(m1, _mm_mul_ps(r2, tmp));
        m2 = _mm_add_ps( _mm_mul_ps(r1, tmp), m2);
        tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
        m1 = _mm_add_ps( _mm_mul_ps(r2, tmp), m1);
        m2 = _mm_sub_ps(m2, _mm_mul_ps(r1, tmp));

        // -----------------------------------------------
        tmp = _mm_mul_ps(r0, r2);
        tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
        m1 = _mm_add_ps( _mm_mul_ps(r3, tmp), m1);
        m3 = _mm_sub_ps(m3, _mm_mul_ps(r1, tmp));
        tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
        m1 = _mm_sub_ps(m1, _mm_mul_ps(r3, tmp));
        m3 = _mm_add_ps( _mm_mul_ps(r1, tmp), m3);

        // -----------------------------------------------
        lm128 det = _mm_mul_ps(r0, m0);
        det = _mm_add_ps( _mm_shuffle_ps(det, det, 0x4E), det);
        det = _mm_add_ps( _mm_shuffle_ps(det, det, 0xB1), det);

#if 0
        {//Newton-Raphson@ÅAsñ®ÌtðvZ
            tmp = _mm_rcp_ss(det);
            det = _mm_mul_ss(det, tmp);
            det = _mm_mul_ss(det, tmp);
            tmp = _mm_add_ss(tmp, tmp);
            det = _mm_sub_ss(tmp, det);
            det = _mm_shuffle_ps(det, det, 0);
        }
#else
        {//sñ®ÌtðvZ
            tmp = _mm_set_ss(1.0f);
            det = _mm_div_ss(tmp, det);
            det = _mm_shuffle_ps(det, det, 0);
        }
#endif

        m0 = _mm_mul_ps(det, m0);
        _mm_storel_pi((lm64*)&dst.m_[0][0], m0);
        _mm_storeh_pi((lm64*)&dst.m_[0][2], m0);

        m1 = _mm_mul_ps(det, m1);
        _mm_storel_pi((lm64*)&dst.m_[1][0], m1);
        _mm_storeh_pi((lm64*)&dst.m_[1][2], m1);

        m2 = _mm_mul_ps(det, m2);
        _mm_storel_pi((lm64*)&dst.m_[2][0], m2);
        _mm_storeh_pi((lm64*)&dst.m_[2][2], m2);

        m3 = _mm_mul_ps(det, m3);
        _mm_storel_pi((lm64*)&dst.m_[3][0], m3);
        _mm_storeh_pi((lm64*)&dst.m_[3][2], m3);

#else
        Matrix44 tmp(*this);
        Matrix44 tmp2;
        tmp2.identity();

        for(s32 j=0; j<4; j++){
            // Find the row with max value in this column
            s32 rowMax = j; // Points to max abs value row in this column
            for(s32 i=j+1; i<4; i++){
                if( absolute(tmp.m_[j][i]) > absolute(tmp.m_[j][rowMax]) ){
                    rowMax = i;
                }
            }

            // If the max value here is 0, we can't invert.
            if( lray::isEqual(tmp.m_[j][rowMax], 0.0f) ){
                return;
            }

            // Swap row "rowMax" with row "j"
            for(s32 cc=0; cc<4; ++cc){
                lray::swap(tmp.m_[cc][j], tmp.m_[cc][rowMax]);
                lray::swap(tmp2.m_[cc][j], tmp2.m_[cc][rowMax]);
            }

            // Now everything we do is on row "c".
            // Set the max cell to 1 by dividing the entire row by that value
            f32 invPivot = 1.0f/tmp(j, j);
            for(s32 cc=0; cc<4; ++cc){
                tmp.m_[cc][j] *= invPivot;
                tmp2.m_[cc][j] *= invPivot;
            }

            f32 pivot;
            // Now do the other rows, so that this column only has a 1 and 0's
            for(s32 i=0; i<4; ++i){
                if(i != j){
                    pivot = tmp.m_[j][i];
                    for(s32 cc=0; cc<4; cc++)
                    {
                        tmp.m_[cc][i] -= tmp.m_[cc][j] * pivot;
                        tmp2.m_[cc][i] -= tmp2.m_[cc][j] * pivot;
                    }
                }
            }

        }

        dst = tmp2;
#endif
    }

    // 3x3ªsñÌ]u
    void Matrix44::transpose33()
    {
        lray::swap(m_[0][1], m_[1][0]);
        lray::swap(m_[0][2], m_[2][0]);
        lray::swap(m_[1][2], m_[2][1]);

    }

    f32 Matrix44::determinant33() const
    {
        return m_[0][0] * (m_[1][1]*m_[2][2] - m_[1][2]*m_[2][1])
             + m_[0][1] * (m_[1][2]*m_[2][0] - m_[1][0]*m_[2][2])
             + m_[0][2] * (m_[1][0]*m_[2][1] - m_[1][1]*m_[2][0]);
    }

    // 3x3ªsñÌtsñ
    void Matrix44::getInvert33(Matrix44& dst) const
    {
#ifdef LRAY_USE_SSE
        LALIGN16 f32 buffer[4];

        lm128 c0 = _mm_loadu_ps(&m_[0][0]);
        lm128 c1 = _mm_loadu_ps(&m_[1][0]);
        lm128 c2 = _mm_loadu_ps(&m_[2][0]);

        lm128 t0 = _mm_shuffle_ps(c1, c1, 0xC9);//11 12 10 13
        lm128 t1 = _mm_shuffle_ps(c2, c2, 0xD2);//22 20 21 23

        lm128 t2 = _mm_shuffle_ps(c1, c1, 0xD2);//12 10 11 13
        lm128 t3 = _mm_shuffle_ps(c2, c2, 0xC9);//21 22 20 23

        t0 = _mm_mul_ps(t0, t1);
        t2 = _mm_mul_ps(t2, t3);
        t0 = _mm_sub_ps(t0, t2); //]öqsñ@0ñÚ

        //sñ®ÌtvZ
        //-------------------------------------------------------
        lm128 det = _mm_mul_ps(c0, t0);
        //_mm_store_ps(buffer, det);
        //buffer[0] = buffer[0] + buffer[1] + buffer[2]; //½Z
        //det = _mm_load_ss(buffer);

        //½Z
        det = _mm_add_ps( _mm_shuffle_ps(det, det, 0x4E), det);
        det = _mm_add_ps( _mm_shuffle_ps(det, det, 0xB1), det);

        //Newton-Raphson@ÅAsñ®ÌtðvZ
        {
            t1 = _mm_rcp_ss(det);
            det = _mm_mul_ss(det, t1);
            det = _mm_mul_ss(det, t1);
            t1 = _mm_add_ss(t1, t1);
            det = _mm_sub_ss(t1, det);
            det = _mm_shuffle_ps(det, det, 0);
        }

        //0ñÚ
        t0 = _mm_mul_ps(t0, det);
        _mm_store_ps(buffer, t0);

        dst.m_[0][0] = buffer[0];
        dst.m_[1][0] = buffer[1];
        dst.m_[2][0] = buffer[2];


        t0 = _mm_shuffle_ps(c0, c0, 0xD2);//02 00 01 03
        t1 = _mm_shuffle_ps(c2, c2, 0xC9);//21 22 20 23

        t2 = _mm_shuffle_ps(c0, c0, 0xC9);//01 02 00 03
        t3 = _mm_shuffle_ps(c2, c2, 0xD2);//22 20 21 23

        t0 = _mm_mul_ps(t0, t1);
        t2 = _mm_mul_ps(t2, t3);
        t0 = _mm_sub_ps(t0, t2); //]öqsñ@1ñÚ

        //1ñÚ
        t0 = _mm_mul_ps(t0, det);
        _mm_store_ps(buffer, t0);

        dst.m_[0][1] = buffer[0];
        dst.m_[1][1] = buffer[1];
        dst.m_[2][1] = buffer[2];


        t0 = _mm_shuffle_ps(c0, c0, 0xC9);//01 02 00 03
        t1 = _mm_shuffle_ps(c1, c1, 0xD2);//12 10 11 13

        t2 = _mm_shuffle_ps(c0, c0, 0xD2);//02 00 01 03
        t3 = _mm_shuffle_ps(c1, c1, 0xC9);//11 12 10 13

        t0 = _mm_mul_ps(t0, t1);
        t2 = _mm_mul_ps(t2, t3);
        t0 = _mm_sub_ps(t0, t2); //]öqsñ@2ñÚ

        //2ñÚ
        t0 = _mm_mul_ps(t0, det);
        _mm_store_ps(buffer, t0);

        dst.m_[0][2] = buffer[0];
        dst.m_[1][2] = buffer[1];
        dst.m_[2][2] = buffer[2];
#else
        f32 det = determinant33();

        LASSERT(isEqual(det, 0.0f) == false);

        f32 invDet = 1.0f / det;

        Matrix34 ret;
        ret.m_[0][0] = (m_[1][1]*m_[2][2] - m_[1][2]*m_[2][1]) * invDet;
        ret.m_[0][1] = (m_[0][2]*m_[2][1] - m_[0][1]*m_[2][2]) * invDet;
        ret.m_[0][2] = (m_[0][1]*m_[1][2] - m_[0][2]*m_[1][1]) * invDet;

        ret.m_[1][0] = (m_[1][2]*m_[2][0] - m_[1][0]*m_[2][2]) * invDet;
        ret.m_[1][1] = (m_[0][0]*m_[2][2] - m_[0][2]*m_[2][0]) * invDet;
        ret.m_[1][2] = (m_[0][2]*m_[1][0] - m_[0][0]*m_[1][2]) * invDet;

        ret.m_[2][0] = (m_[1][0]*m_[2][1] - m_[1][1]*m_[2][0]) * invDet;
        ret.m_[2][1] = (m_[0][1]*m_[2][0] - m_[0][0]*m_[2][1]) * invDet;
        ret.m_[2][2] = (m_[0][0]*m_[1][1] - m_[0][1]*m_[1][0]) * invDet;

        ret.m_[0][3] = m_[0][3];
        ret.m_[1][3] = m_[1][3];
        ret.m_[2][3] = m_[2][3];

        dst = ret;
#endif
    }

    void Matrix44::setTranslate(const Vector3& v)
    {
        m_[0][3] = v.x_;
        m_[1][3] = v.y_;
        m_[2][3] = v.z_;
    }

        void Matrix44::setTranslate(const Vector4& v)
        {
            m_[0][3] = v.x_;
        m_[1][3] = v.y_;
        m_[2][3] = v.z_;
        }

    void Matrix44::translate(const Vector3& v)
    {
        m_[0][3] += v.x_;
        m_[1][3] += v.y_;
        m_[2][3] += v.z_;
    }

    void Matrix44::translate(const Vector4& v)
    {
        m_[0][3] += v.x_;
        m_[1][3] += v.y_;
        m_[2][3] += v.z_;
    }

    void Matrix44::translate(f32 x, f32 y, f32 z)
    {
        m_[0][3] += x;
        m_[1][3] += y;
        m_[2][3] += z;
    }

    void Matrix44::preTranslate(const Vector3& v)
    {
        preTranslate(v.x_, v.y_, v.z_);
    }

    void Matrix44::preTranslate(f32 x, f32 y, f32 z)
    {
        m_[0][3] += m_[0][0] * x + m_[0][1] * y + m_[0][2] * z;
        m_[1][3] += m_[1][0] * x + m_[1][1] * y + m_[1][2] * z;
        m_[2][3] += m_[2][0] * x + m_[2][1] * y + m_[2][2] * z;
    }


    void Matrix44::rotateX(f32 radian)
    {
        f32 cosA = lray::cosf(radian);
        f32 sinA = lray::sinf(radian);

#if 0
        Matrix44 rotation;
        rotation(0, 0) = 1.0f; rotation(0, 1) = 0.0f;  rotation(0, 2) = 0.0f; rotation(0, 3) = 0.0f;
        rotation(1, 0) = 0.0f; rotation(1, 1) = cosA;  rotation(1, 2) = sinA; rotation(1, 3) = 0.0f;
        rotation(2, 0) = 0.0f; rotation(2, 1) = -sinA; rotation(2, 2) = cosA; rotation(2, 3) = 0.0f;
        rotation(3, 0) = 0.0f; rotation(3, 1) = 0.0f;  rotation(3, 2) = 0.0f; rotation(3, 3) = 1.0f;

        *this *= rotation;

#else
        f32 tmp[6];
        tmp[0] = m_[1][0]*cosA + m_[2][0]*sinA;
        tmp[1] = m_[1][1]*cosA + m_[2][1]*sinA;
        tmp[2] = m_[1][2]*cosA + m_[2][2]*sinA;

        tmp[3] = -m_[1][0]*sinA + m_[2][0]*cosA;
        tmp[4] = -m_[1][1]*sinA + m_[2][1]*cosA;
        tmp[5] = -m_[1][2]*sinA + m_[2][2]*cosA;

        m_[1][0] = tmp[0]; m_[1][1] = tmp[1]; m_[1][2] = tmp[2];
        m_[2][0] = tmp[3]; m_[2][1] = tmp[4]; m_[2][2] = tmp[5];
#endif

    }

    void Matrix44::rotateY(f32 radian)
    {
        f32 cosA = lray::cosf(radian);
        f32 sinA = lray::sinf(radian);

#if 0
        Matrix44 rotation;
        rotation(0, 0) = cosA; rotation(0, 1) = 0.0f;  rotation(0, 2) = -sinA; rotation(0, 3) = 0.0f;
        rotation(1, 0) = 0.0f; rotation(1, 1) = 1.0f;  rotation(1, 2) = 0.0f; rotation(1, 3) = 0.0f;
        rotation(2, 0) = sinA; rotation(2, 1) = 0.0f; rotation(2, 2) = cosA; rotation(2, 3) = 0.0f;
        rotation(3, 0) = 0.0f; rotation(3, 1) = 0.0f;  rotation(3, 2) = 0.0f; rotation(3, 3) = 1.0f;

        *this *= rotation;

#else
        f32 tmp[6];
        tmp[0] = m_[0][0]*cosA - m_[2][0]*sinA;
        tmp[1] = m_[0][1]*cosA - m_[2][1]*sinA;
        tmp[2] = m_[0][2]*cosA - m_[2][2]*sinA;

        tmp[3] = m_[0][0]*sinA + m_[2][0]*cosA;
        tmp[4] = m_[0][1]*sinA + m_[2][1]*cosA;
        tmp[5] = m_[0][2]*sinA + m_[2][2]*cosA;

        m_[0][0] = tmp[0]; m_[0][1] = tmp[1]; m_[0][2] = tmp[2];
        m_[2][0] = tmp[3]; m_[2][1] = tmp[4]; m_[2][2] = tmp[5];
#endif
    }

    void Matrix44::rotateZ(f32 radian)
    {
        f32 cosA = lray::cosf(radian);
        f32 sinA = lray::sinf(radian);

#if 0
        Matrix44 rotation;
        rotation(0, 0) = cosA; rotation(0, 1) = sinA;  rotation(0, 2) = 0.0f; rotation(0, 3) = 0.0f;
        rotation(1, 0) = -sinA; rotation(1, 1) = cosA;  rotation(1, 2) = 0.0f; rotation(1, 3) = 0.0f;
        rotation(2, 0) = 0.0f; rotation(2, 1) = 0.0f; rotation(2, 2) = 1.0f; rotation(2, 3) = 0.0f;
        rotation(3, 0) = 0.0f; rotation(3, 1) = 0.0f;  rotation(3, 2) = 0.0f; rotation(3, 3) = 1.0f;

        *this *= rotation;

#else
        f32 tmp[6];
        tmp[0] = m_[0][0]*cosA + m_[1][0]*sinA;
        tmp[1] = m_[0][1]*cosA + m_[1][1]*sinA;
        tmp[2] = m_[0][2]*cosA + m_[1][2]*sinA;

        tmp[3] = -m_[0][0]*sinA + m_[1][0]*cosA;
        tmp[4] = -m_[0][1]*sinA + m_[1][1]*cosA;
        tmp[5] = -m_[0][2]*sinA + m_[1][2]*cosA;

        m_[0][0] = tmp[0]; m_[0][1] = tmp[1]; m_[0][2] = tmp[2];
        m_[1][0] = tmp[3]; m_[1][1] = tmp[4]; m_[1][2] = tmp[5];
#endif
    }

    // X²ñ]
    void Matrix44::setRotateX(f32 radian)
    {
        f32 cosA = lray::cosf(radian);
        f32 sinA = lray::sinf(radian);

        m_[0][0] = 1.0f; m_[0][1] = 0.0f; m_[0][2] = 0.0f; m_[0][3] = 0.0f;
        m_[1][0] = 0.0f; m_[1][1] = cosA; m_[1][2] = sinA; m_[1][3] = 0.0f;
        m_[2][0] = 0.0f; m_[2][1] = -sinA; m_[2][2] = cosA; m_[2][3] = 0.0f;
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    // Y²ñ]
    void Matrix44::setRotateY(f32 radian)
    {
        f32 cosA = lray::cosf(radian);
        f32 sinA = lray::sinf(radian);

        m_[0][0] = cosA; m_[0][1] = 0.0f; m_[0][2] = -sinA; m_[0][3] = 0.0f;
        m_[1][0] = 0.0f; m_[1][1] = 1.0f; m_[1][2] = 0.0f; m_[1][3] = 0.0f;
        m_[2][0] = sinA; m_[2][1] = 0.0f; m_[2][2] = cosA; m_[2][3] = 0.0f;
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    // Z²ñ]
    void Matrix44::setRotateZ(f32 radian)
    {
        f32 cosA = lray::cosf(radian);
        f32 sinA = lray::sinf(radian);

        m_[0][0] = cosA; m_[0][1] = sinA; m_[0][2] = 0.0f; m_[0][3] = 0.0f;
        m_[1][0] = -sinA; m_[1][1] = cosA; m_[1][2] = 0.0f; m_[1][3] = 0.0f;
        m_[2][0] = 0.0f; m_[2][1] = 0.0f; m_[2][2] = 1.0f; m_[2][3] = 0.0f;
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    void Matrix44::setRotateAxis(const Vector3& axis, f32 radian)
    {
        setRotateAxis(axis.x_, axis.y_, axis.z_, radian);
    }

    void Matrix44::setRotateAxis(f32 x, f32 y, f32 z, f32 radian)
    {
        f32 norm = lray::sqrt(x*x + y*y + z*z);
        LASSERT(!lray::isEqual(norm, 0.0f));

        norm = 1.0f / norm;
        x *= norm;
        y *= norm;
        z *= norm;

        f32 xx = x*x;
        f32 xy = x*y;
        f32 xz = x*z;

        f32 yx = y*x;
        f32 yy = y*y;
        f32 yz = y*z;

        f32 zx = z*x;
        f32 zy = z*y;
        f32 zz = z*z;

        f32 cosA = lray::cosf(radian);
        f32 sinA = lray::sinf(radian);
        f32 sx = sinA * x;
        f32 sy = sinA * y;
        f32 sz = sinA * z;

        f32 invCosA = 1.0f - cosA;

        m_[0][0] = (invCosA * xx) + cosA;
        m_[1][0] = (invCosA * xy) - (sz);
        m_[2][0] = (invCosA * xz) + (sy);
        m_[3][0] = 0.0f;

        m_[0][1] = (invCosA * yx) + (sz);
        m_[1][1] = (invCosA * yy) + (cosA);
        m_[2][1] = (invCosA * yz) - (sx);
        m_[3][1] = 0.0f;

        m_[0][2] = (invCosA * zx) - (sy);
        m_[1][2] = (invCosA * zy) + (sx);
        m_[2][2] = (invCosA * zz) + (cosA);
        m_[3][2] = 0.0f;

        m_[0][3] = 0.0f;
        m_[1][3] = 0.0f;
        m_[2][3] = 0.0f;
        m_[3][3] = 1.0f;
    }

#ifdef LRAY_USE_SSE
    namespace
    {
        lm128 dot(const lm128& v0, const lm128& v1)
        {
            lm128 tmp = _mm_mul_ps(v0, v1);
            tmp = _mm_add_ps( _mm_shuffle_ps(tmp, tmp, 0x4E), tmp);
            tmp = _mm_add_ps( _mm_shuffle_ps(tmp, tmp, 0xB1), tmp);
            tmp = _mm_shuffle_ps(tmp, tmp, 0);
            return tmp;
        }

        lm128 dotForLookAt(const lm128& v, const lm128& eye)
        {
            f32 f;
            *((u32*)&f) = 0x80000000U;
            lm128 mask = _mm_set1_ps(f);

            lm128 tmp = _mm_mul_ps(v, eye);
            tmp = _mm_add_ps( _mm_shuffle_ps(tmp, tmp, 0x4E), tmp);
            tmp = _mm_add_ps( _mm_shuffle_ps(tmp, tmp, 0xB1), tmp);
            tmp = _mm_xor_ps(tmp, mask);

            //x,y,z,v
            tmp = _mm_shuffle_ps(v, tmp, 0x0A);
            tmp = _mm_shuffle_ps(v, tmp, 0xC4);

            return tmp;
        }

        void rcp(lm128& r)
        {
#if 0
            //Newton-Raphson@ÅAsñ®ÌtðvZ
            lm128 tmp = _mm_rcp_ss(r);
            r = _mm_mul_ss(r, tmp);
            r = _mm_mul_ss(r, tmp);
            tmp = _mm_add_ss(tmp, tmp);
            r = _mm_sub_ss(tmp, r);
            r = _mm_shuffle_ps(r, r, 0);
#else
            lm128 tmp = _mm_set_ss(1.0f);
            r = _mm_div_ss(tmp, r);
            r = _mm_shuffle_ps(r, r, 0);
#endif
        }

        void normalize(lm128& v)
        {
            lm128 r1 = v;
            v = _mm_mul_ps(v, v);
            v = _mm_add_ps( _mm_shuffle_ps(v, v, 0x4E), v);
            v = _mm_add_ps( _mm_shuffle_ps(v, v, 0xB1), v);

            v = _mm_sqrt_ss(v);
            v = _mm_shuffle_ps(v, v, 0);

            rcp(v);
            v = _mm_mul_ps(r1, v);
        }

        lm128 cross3(const lm128& xv0, const lm128& xv1)
        {
            lm128 xv2 = _mm_shuffle_ps(xv1, xv1, 0xC9);

            lm128 tmp0 = _mm_mul_ps(xv0, xv2);
            lm128 tmp1 = _mm_shuffle_ps(xv0, xv0, 0xC9);
            tmp1 = _mm_mul_ps(tmp1, xv1);

            tmp0 = _mm_sub_ps(tmp0, tmp1);

            tmp0 = _mm_shuffle_ps(tmp0, tmp0, 0xC9);
            return tmp0;
        }
    }
#endif

    void Matrix44::lookAt(const Vector4& eye, const Vector4& at, const Vector4& up)
    {
#ifdef LRAY_USE_SSE

        lm128 xaxis, yaxis, zaxis, teye;
        teye = _mm_loadu_ps(&eye.x_);

        zaxis = _mm_loadu_ps(&at.x_);
        zaxis = _mm_sub_ps(zaxis, teye);

        normalize(zaxis);

        xaxis = cross3(_mm_loadu_ps(&up.x_), zaxis);
        normalize(xaxis);

        yaxis = cross3(zaxis, xaxis);

        xaxis = dotForLookAt(xaxis, teye);
        yaxis = dotForLookAt(yaxis, teye);
        zaxis = dotForLookAt(zaxis, teye);

        _mm_storeu_ps(&m_[0][0], xaxis);
        _mm_storeu_ps(&m_[1][0], yaxis);
        _mm_storeu_ps(&m_[2][0], zaxis);


        LALIGN16 f32 buffer[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        lm128 t = _mm_load_ps(buffer);
        _mm_storeu_ps(&m_[3][0], t);

#else
        Vector4 xaxis, yaxis, zaxis = at;
        zaxis -= eye;
        zaxis = normalize(zaxis);

        xaxis = Vector4(cross3(up, zaxis));
        xaxis = normalize(xaxis);

        yaxis = Vector4(cross3(zaxis, xaxis));

        m_[0][0] = xaxis.x_; m_[0][1] = xaxis.y_; m_[0][2] = xaxis.z_; m_[0][3] = -dot(eye, xaxis);
        m_[1][0] = yaxis.x_; m_[1][1] = yaxis.y_; m_[1][2] = yaxis.z_; m_[1][3] = -dot(eye, yaxis);
        m_[2][0] = zaxis.x_; m_[2][1] = zaxis.y_; m_[2][2] = zaxis.z_; m_[2][3] = -dot(eye, zaxis);
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
#endif
    }

    void Matrix44::lookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
    {
        Vector3 xaxis, yaxis, zaxis = normalize(at-eye);

        xaxis = normalize(cross(up, zaxis));
        yaxis = cross(zaxis, xaxis);

        m_[0][0] = xaxis.x_; m_[0][1] = xaxis.y_; m_[0][2] = xaxis.z_; m_[0][3] = -dot(eye, xaxis);
        m_[1][0] = yaxis.x_; m_[1][1] = yaxis.y_; m_[1][2] = yaxis.z_; m_[1][3] = -dot(eye, yaxis);
        m_[2][0] = zaxis.x_; m_[2][1] = zaxis.y_; m_[2][2] = zaxis.z_; m_[2][3] = -dot(eye, zaxis);
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    void Matrix44::lookAt(const Vector4& at)
    {
        Vector4 xaxis, yaxis, zaxis = normalize(at);

        f32 d = dot(zaxis, Vector4::Up);
        if(lray::isEqual(d, -1.0f, 0.000001f)){
            setRotateX(-PI_2);
            return;
        }
        if(lray::isEqual(d, 1.0f, 0.000001f)){
            setRotateX(PI_2);
            return;
        }

        xaxis = Vector4(normalize(cross3(Vector4::Up, zaxis)));
        yaxis = Vector4(cross3(zaxis, xaxis));

        m_[0][0] = xaxis.x_; m_[0][1] = xaxis.y_; m_[0][2] = xaxis.z_; m_[0][3] = 0.0f;
        m_[1][0] = yaxis.x_; m_[1][1] = yaxis.y_; m_[1][2] = yaxis.z_; m_[1][3] = 0.0f;
        m_[2][0] = zaxis.x_; m_[2][1] = zaxis.y_; m_[2][2] = zaxis.z_; m_[2][3] = 0.0f;
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    void Matrix44::lookAt(const Vector3& at)
    {
        Vector3 xaxis, yaxis, zaxis = normalize(at);

        f32 d = dot(zaxis, Vector3::Up);
        if(lray::isEqual(d, -1.0f, 0.000001f)){
            setRotateX(-PI_2);
            return;
        }
        if(lray::isEqual(d, 1.0f, 0.000001f)){
            setRotateX(PI_2);
            return;
        }

        xaxis = normalize(cross(Vector3::Up, zaxis));
        yaxis = cross(zaxis, xaxis);

        m_[0][0] = xaxis.x_; m_[0][1] = xaxis.y_; m_[0][2] = xaxis.z_; m_[0][3] = 0.0f;
        m_[1][0] = yaxis.x_; m_[1][1] = yaxis.y_; m_[1][2] = yaxis.z_; m_[1][3] = 0.0f;
        m_[2][0] = zaxis.x_; m_[2][1] = zaxis.y_; m_[2][2] = zaxis.z_; m_[2][3] = 0.0f;
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    void Matrix44::lookAt(const Vector4& eye, const Quaternion& rotation)
    {
        Vector4 zaxis = rotate(rotation, Vector4::Forward);
        Vector4 xaxis, yaxis;

        Vector4 up = (lray::isEqual(zaxis.y_, 1.0f))? Vector4::Backward : Vector4::Up;
        xaxis = Vector4(normalize(cross3(up, zaxis)));
        yaxis = Vector4(cross3(zaxis, xaxis));

        m_[0][0] = xaxis.x_; m_[0][1] = xaxis.y_; m_[0][2] = xaxis.z_; m_[0][3] = -dot(eye, xaxis);
        m_[1][0] = yaxis.x_; m_[1][1] = yaxis.y_; m_[1][2] = yaxis.z_; m_[1][3] = -dot(eye, yaxis);
        m_[2][0] = zaxis.x_; m_[2][1] = zaxis.y_; m_[2][2] = zaxis.z_; m_[2][3] = -dot(eye, zaxis);
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    void Matrix44::lookAt(const Vector3& eye, const Quaternion& rotation)
    {
        Vector3 zaxis = rotate(rotation, Vector3::Forward);
        Vector3 xaxis, yaxis;

        Vector3 up = (lray::isEqual(zaxis.y_, 1.0f))? Vector3::Backward : Vector3::Up;
        xaxis = normalize(cross(up, zaxis));
        yaxis = cross(zaxis, xaxis);

        m_[0][0] = xaxis.x_; m_[0][1] = xaxis.y_; m_[0][2] = xaxis.z_; m_[0][3] = -dot(eye, xaxis);
        m_[1][0] = yaxis.x_; m_[1][1] = yaxis.y_; m_[1][2] = yaxis.z_; m_[1][3] = -dot(eye, yaxis);
        m_[2][0] = zaxis.x_; m_[2][1] = zaxis.y_; m_[2][2] = zaxis.z_; m_[2][3] = -dot(eye, zaxis);
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    void Matrix44::viewPointAlign(const Matrix44& view, const Vector4& position)
    {
        Vector4 axis = {position.x_-view(0,3), position.y_-view(1,3), position.z_-view(2,3), 0.0f};
        f32 l = axis.lengthSqr();
        if(lray::isZeroPositive(l)){
            axis.set(view(2,0), view(2,1), view(2,2), 0.0f);
            axis = normalize(axis);
        }else{
            axis /= lray::sqrt(l);
        }
        axisAlign(axis, view, position);
    }

    void Matrix44::axisAlign(const Vector4& axis, const Matrix44& view, const Vector4& position)
    {
        Vector4 zaxis = normalize(axis);

        Vector4 yaxis = {view(1,0), view(1,1), view(1,2), 0.0f};
        f32 d = dot(yaxis, zaxis);
        if(0.999f<d){
            yaxis.set(-view(2,0), -view(2,1), -view(2,2), 0.0f);
        }

        Vector4 xaxis = Vector4(normalize(cross3(yaxis, zaxis)));
        yaxis = Vector4(cross3(zaxis, xaxis));

        m_[0][0] = xaxis.x_; m_[0][1] = yaxis.x_; m_[0][2] = zaxis.x_; m_[0][3] = position.x_;
        m_[1][0] = xaxis.y_; m_[1][1] = yaxis.y_; m_[1][2] = zaxis.y_; m_[1][3] = position.y_;
        m_[2][0] = xaxis.z_; m_[2][1] = yaxis.z_; m_[2][2] = zaxis.z_; m_[2][3] = position.z_;
        m_[3][0] = 0.0f; m_[3][1] = 0.0f; m_[3][2] = 0.0f; m_[3][3] = 1.0f;

    }

    void Matrix44::perspective(f32 width, f32 height, f32 znear, f32 zfar)
    {
        f32 invDepth = zfar / (zfar - znear);
        m_[0][0] = 2.0f*znear/width; m_[0][1] = 0.0f;              m_[0][2] = 0.0f;              m_[0][3] = 0.0f;
        m_[1][0] = 0.0f;             m_[1][1] = 2.0f*znear/height; m_[1][2] = 0.0f;              m_[1][3] = 0.0f;
        m_[2][0] = 0.0f;             m_[2][1] = 0.0f;              m_[2][2] = invDepth; m_[2][3] = -znear*invDepth;
        m_[3][0] = 0.0f;             m_[3][1] = 0.0f;              m_[3][2] = 1.0f; m_[3][3] = 0.0f;
    }

    void Matrix44::perspectiveFov(f32 fovy, f32 aspect, f32 znear, f32 zfar)
    {
        f32 yscale = 1.0f/lray::tan(0.5f * fovy);
        f32 xscale = yscale / aspect;
        f32 invDepth = zfar / (zfar - znear);

        m_[0][0] = xscale; m_[0][1] = 0.0f;   m_[0][2] = 0.0f;              m_[0][3] = 0.0f;
        m_[1][0] = 0.0f;   m_[1][1] = yscale; m_[1][2] = 0.0f;              m_[1][3] = 0.0f;
        m_[2][0] = 0.0f;   m_[2][1] = 0.0f;   m_[2][2] = invDepth; m_[2][3] = -znear*invDepth;
        m_[3][0] = 0.0f;   m_[3][1] = 0.0f;   m_[3][2] = 1.0f; m_[3][3] = 0.0f;
    }

    void Matrix44::perspectiveReverseZ(f32 width, f32 height, f32 znear, f32 zfar)
    {
        f32 invDepth = znear / (znear - zfar);
        m_[0][0] = 2.0f*znear/width; m_[0][1] = 0.0f;              m_[0][2] = 0.0f;              m_[0][3] = 0.0f;
        m_[1][0] = 0.0f;             m_[1][1] = 2.0f*znear/height; m_[1][2] = 0.0f;              m_[1][3] = 0.0f;
        m_[2][0] = 0.0f;             m_[2][1] = 0.0f;              m_[2][2] = invDepth; m_[2][3] = -zfar*invDepth;
        m_[3][0] = 0.0f;             m_[3][1] = 0.0f;              m_[3][2] = 1.0f; m_[3][3] = 0.0f;
    }

    void Matrix44::perspectiveFovReverseZ(f32 fovy, f32 aspect, f32 znear, f32 zfar)
    {
        f32 yscale = 1.0f/lray::tan(0.5f * fovy);
        f32 xscale = yscale / aspect;
        f32 invDepth = znear / (znear - zfar);

        m_[0][0] = xscale; m_[0][1] = 0.0f;   m_[0][2] = 0.0f;              m_[0][3] = 0.0f;
        m_[1][0] = 0.0f;   m_[1][1] = yscale; m_[1][2] = 0.0f;              m_[1][3] = 0.0f;
        m_[2][0] = 0.0f;   m_[2][1] = 0.0f;   m_[2][2] = invDepth; m_[2][3] = -zfar*invDepth;
        m_[3][0] = 0.0f;   m_[3][1] = 0.0f;   m_[3][2] = 1.0f; m_[3][3] = 0.0f;
    }

    void Matrix44::perspectiveLinearZ(f32 width, f32 height, f32 znear, f32 zfar)
    {
        f32 invDepth = 1.0f/(zfar-znear);
        m_[0][0] = 2.0f*znear/width; m_[0][1] = 0.0f;              m_[0][2] = 0.0f;              m_[0][3] = 0.0f;
        m_[1][0] = 0.0f;             m_[1][1] = 2.0f*znear/height; m_[1][2] = 0.0f;              m_[1][3] = 0.0f;
        m_[2][0] = 0.0f;             m_[2][1] = 0.0f;              m_[2][2] = invDepth; m_[2][3] = -znear*invDepth;
        m_[3][0] = 0.0f;             m_[3][1] = 0.0f;              m_[3][2] = 1.0f; m_[3][3] = 0.0f;
    }

    void Matrix44::perspectiveFovLinearZ(f32 fovy, f32 aspect, f32 znear, f32 zfar)
    {
        f32 yscale = 1.0f / lray::tan(0.5f * fovy);
        f32 xscale = yscale / aspect;
        f32 invDepth = 1.0f/(zfar-znear);

        m_[0][0] = xscale; m_[0][1] = 0.0f;   m_[0][2] = 0.0f;              m_[0][3] = 0.0f;
        m_[1][0] = 0.0f;   m_[1][1] = yscale; m_[1][2] = 0.0f;              m_[1][3] = 0.0f;
        m_[2][0] = 0.0f;   m_[2][1] = 0.0f;   m_[2][2] = invDepth; m_[2][3] = -znear*invDepth;
        m_[3][0] = 0.0f;   m_[3][1] = 0.0f;   m_[3][2] = 1.0f; m_[3][3] = 0.0f;
    }

    void Matrix44::ortho(f32 width, f32 height, f32 znear, f32 zfar)
    {
        f32 invDepth = 1.0f/(zfar-znear);
        m_[0][0] = 2.0f/width; m_[0][1] = 0.0f;        m_[0][2] = 0.0f;              m_[0][3] = 0.0f;
        m_[1][0] = 0.0f;       m_[1][1] = 2.0f/height; m_[1][2] = 0.0f;              m_[1][3] = 0.0f;
        m_[2][0] = 0.0f;       m_[2][1] = 0.0f;        m_[2][2] = invDepth; m_[2][3] = -znear*invDepth;
        m_[3][0] = 0.0f;       m_[3][1] = 0.0f;        m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    void Matrix44::orthoOffsetCenter(f32 left, f32 right, f32 top, f32 bottom, f32 znear, f32 zfar)
    {
        f32 invW = 1.0f/(right-left);
        f32 invH = 1.0f/(top-bottom);
        f32 invDepth = 1.0f/(zfar-znear);

        m_[0][0] = 2.0f*invW; m_[0][1] = 0.0f;        m_[0][2] = 0.0f;              m_[0][3] = (right+left)*(-invW);
        m_[1][0] = 0.0f;       m_[1][1] = 2.0f*invH; m_[1][2] = 0.0f;              m_[1][3] = (top+bottom)*(-invH);
        m_[2][0] = 0.0f;       m_[2][1] = 0.0f;        m_[2][2] = invDepth; m_[2][3] = -znear*invDepth;
        m_[3][0] = 0.0f;       m_[3][1] = 0.0f;        m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    void Matrix44::orthoReverseZ(f32 width, f32 height, f32 znear, f32 zfar)
    {
        f32 invDepth = 1.0f/(znear-zfar);
        m_[0][0] = 2.0f/width; m_[0][1] = 0.0f;        m_[0][2] = 0.0f;              m_[0][3] = 0.0f;
        m_[1][0] = 0.0f;       m_[1][1] = 2.0f/height; m_[1][2] = 0.0f;              m_[1][3] = 0.0f;
        m_[2][0] = 0.0f;       m_[2][1] = 0.0f;        m_[2][2] = invDepth; m_[2][3] = -zfar*invDepth;
        m_[3][0] = 0.0f;       m_[3][1] = 0.0f;        m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    void Matrix44::orthoOffsetCenterReverseZ(f32 left, f32 right, f32 top, f32 bottom, f32 znear, f32 zfar)
    {
        f32 invW = 1.0f/(right-left);
        f32 invH = 1.0f/(top-bottom);
        f32 invDepth = 1.0f/(znear-zfar);

        m_[0][0] = 2.0f*invW; m_[0][1] = 0.0f;        m_[0][2] = 0.0f;              m_[0][3] = (right+left)*(-invW);
        m_[1][0] = 0.0f;       m_[1][1] = 2.0f*invH; m_[1][2] = 0.0f;              m_[1][3] = (top+bottom)*(-invH);
        m_[2][0] = 0.0f;       m_[2][1] = 0.0f;        m_[2][2] = invDepth; m_[2][3] = -zfar*invDepth;
        m_[3][0] = 0.0f;       m_[3][1] = 0.0f;        m_[3][2] = 0.0f; m_[3][3] = 1.0f;
    }

    //------------------------------------------------------------------------------------------------
    void Matrix44::getTranslate(Vector3& trans) const
    {
        getTranslate(trans.x_, trans.y_, trans.z_);
    }

    void Matrix44::getTranslate(Vector4& trans) const
    {
        getTranslate(trans.x_, trans.y_, trans.z_);
        trans.w_ = 0.0f;
    }

    void Matrix44::getRow(Vector3& dst, s32 row) const
    {
        LASSERT(0<=row && row<4);
        dst.set(m_[row][0], m_[row][1], m_[row][2]);
    }

    void Matrix44::getRow(Vector4& dst, s32 row) const
    {
        LASSERT(0<=row && row<4);

#ifdef LRAY_USE_SSE
        lm128 r = _mm_loadu_ps(&m_[row][0]);
        Vector4::store(dst, r);
#else
        dst.set(m_[row][0], m_[row][1], m_[row][2], m_[row][3]);
#endif
    }

    bool Matrix44::isNan() const
    {
        for(u32 i=0; i<4; ++i){
            for(u32 j=0; j<4; ++j){
                if(lray::isNan(m_[i][j])){
                    return true;
                }
            }
        }
        return false;
    }

    void Matrix44::getRotation(Quaternion& rotation) const
    {
        f32 trace0 = m_[0][0] + m_[1][1] + m_[2][2];
        f32 trace1 = m_[0][0] - m_[1][1] - m_[2][2];
        f32 trace2 = m_[1][1] - m_[0][0] - m_[2][2];
        f32 trace3 = m_[2][2] - m_[0][0] - m_[1][1];

        s32 index = 0;
        f32 trace = trace0;
        if(trace1>trace){
            index = 1;
            trace = trace1;
        }
        if(trace2>trace){
            index = 2;
            trace = trace2;
        }
        if(trace3>trace){
            index = 3;
            trace = trace3;
        }

        f32 value = lray::sqrt(trace + 1.0f) * 0.5f;
        f32 m = 0.25f/value;

        switch(index)
        {
        case 0:
            rotation.w_ = value;
            rotation.x_ = (m_[2][1] - m_[1][2]) * m;
            rotation.y_ = (m_[0][2] - m_[2][0]) * m;
            rotation.z_ = (m_[1][0] - m_[0][1]) * m;
            break;

        case 1:
            rotation.x_ = value;
            rotation.w_ = (m_[2][1] - m_[1][2]) * m;
            rotation.y_ = (m_[1][0] + m_[0][1]) * m;
            rotation.z_ = (m_[0][2] + m_[2][0]) * m;
            break;

        case 2:
            rotation.y_ = value;
            rotation.w_ = (m_[0][2] - m_[2][0]) * m;
            rotation.x_ = (m_[1][0] + m_[0][1]) * m;
            rotation.z_ = (m_[2][1] + m_[1][2]) * m;
            break;

        case 3:
            rotation.z_ = value;
            rotation.w_ = (m_[1][0] - m_[0][1]) * m;
            rotation.x_ = (m_[0][2] + m_[2][0]) * m;
            rotation.y_ = (m_[2][1] + m_[1][2]) * m;
            break;
        }
    }

    //------------------------------------------------------------------------------------------------
    void lookAt(Matrix44& view, Matrix44& invview, const Vector4& eye, const Vector4& at, const Vector4& up)
    {
        LASSERT(lray::isZero(eye.w_));
        LASSERT(lray::isZero(at.w_));
        LASSERT(lray::isZero(up.w_));

        lm128 xaxis, yaxis, zaxis, teye;
        teye = _mm_loadu_ps(&eye.x_);

        zaxis = _mm_loadu_ps(&at.x_);
        zaxis = _mm_sub_ps(zaxis, teye);

        normalize(zaxis);

        xaxis = cross3(_mm_loadu_ps(&up.x_), zaxis);
        normalize(xaxis);

        yaxis = cross3(zaxis, xaxis);
        normalize(yaxis);

        lm128 col0 = dotForLookAt(xaxis, teye);
        lm128 col1 = dotForLookAt(yaxis, teye);
        lm128 col2 = dotForLookAt(zaxis, teye);

        _mm_storeu_ps(&view.m_[0][0], col0);
        _mm_storeu_ps(&view.m_[1][0], col1);
        _mm_storeu_ps(&view.m_[2][0], col2);


        LALIGN16 f32 buffer[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        lm128 t = _mm_load_ps(buffer);
        _mm_storeu_ps(&view.m_[3][0], t);
        
        _MM_TRANSPOSE4_PS(xaxis, yaxis, zaxis, teye);
        _mm_storeu_ps(&invview.m_[0][0], xaxis);
        _mm_storeu_ps(&invview.m_[1][0], yaxis);
        _mm_storeu_ps(&invview.m_[2][0], zaxis);
        _mm_storeu_ps(&invview.m_[3][0], teye);
        invview.m_[3][0] = invview.m_[3][1] = invview.m_[3][2] = 0.0f; invview.m_[3][3] = 1.0f;
    }

    void lookAt(Matrix44& view, Matrix44& invview, const Vector3& eye, const Vector3& at, const Vector3& up)
    {
        lookAt(view, invview, Vector4(eye), Vector4(at), Vector4(up));
    }

    void lookAt(Matrix44& view, Matrix44& invview, const Vector4& at)
    {
        LASSERT(lray::isZero(at.w_));

        Vector4 xaxis, yaxis, zaxis = normalize(at);

        f32 d = zaxis.y_;
        if(lray::isEqual(d, -1.0f, 0.000001f)){
            view.setRotateX(-PI_2);
            invview.setRotateX(PI_2);
            return;
        }
        if(lray::isEqual(d, 1.0f, 0.000001f)){
            view.setRotateX(PI_2);
            invview.setRotateX(-PI_2);
            return;
        }

        xaxis = Vector4(normalize(cross3(Vector4::Up, zaxis)));
        yaxis = Vector4(normalize(cross3(zaxis, xaxis)));

        view.m_[0][0] = xaxis.x_; view.m_[0][1] = xaxis.y_; view.m_[0][2] = xaxis.z_; view.m_[0][3] = 0.0f;
        view.m_[1][0] = yaxis.x_; view.m_[1][1] = yaxis.y_; view.m_[1][2] = yaxis.z_; view.m_[1][3] = 0.0f;
        view.m_[2][0] = zaxis.x_; view.m_[2][1] = zaxis.y_; view.m_[2][2] = zaxis.z_; view.m_[2][3] = 0.0f;
        view.m_[3][0] = 0.0f; view.m_[3][1] = 0.0f; view.m_[3][2] = 0.0f; view.m_[3][3] = 1.0f;

        invview.m_[0][0] = xaxis.x_; invview.m_[0][1] = yaxis.x_; invview.m_[0][2] = zaxis.x_; invview.m_[0][3] = 0.0f;
        invview.m_[1][0] = xaxis.y_; invview.m_[1][1] = yaxis.y_; invview.m_[1][2] = zaxis.y_; invview.m_[1][3] = 0.0f;
        invview.m_[2][0] = xaxis.z_; invview.m_[2][1] = yaxis.z_; invview.m_[2][2] = zaxis.z_; invview.m_[2][3] = 0.0f;
        invview.m_[3][0] = 0.0f; invview.m_[3][1] = 0.0f; invview.m_[3][2] = 0.0f; invview.m_[3][3] = 1.0f;
    }

    void lookAt(Matrix44& view, Matrix44& invview, const Vector3& at)
    {
        lookAt(view, invview, Vector4(at));
    }

    void lookAt(Matrix44& view, Matrix44& invview, const Vector4& eye, const Quaternion& rotation)
    {
        LASSERT(lray::isZero(eye.w_));

        Vector4 zaxis = rotate(rotation, Vector4::Forward);
        Vector4 xaxis, yaxis;

        Vector4 up;
        if(zaxis.y_<-0.999f){
            up = Vector4::Forward;
        }else if(0.999f<zaxis.y_){
            up = Vector4::Backward;
        }else{
            up = Vector4::Up;
        }
        xaxis = Vector4(normalize(cross3(up, zaxis)));
        yaxis = Vector4(normalize(cross3(zaxis, xaxis)));

        view.m_[0][0] = xaxis.x_; view.m_[0][1] = xaxis.y_; view.m_[0][2] = xaxis.z_; view.m_[0][3] = -dot(eye, xaxis);
        view.m_[1][0] = yaxis.x_; view.m_[1][1] = yaxis.y_; view.m_[1][2] = yaxis.z_; view.m_[1][3] = -dot(eye, yaxis);
        view.m_[2][0] = zaxis.x_; view.m_[2][1] = zaxis.y_; view.m_[2][2] = zaxis.z_; view.m_[2][3] = -dot(eye, zaxis);
        view.m_[3][0] = 0.0f; view.m_[3][1] = 0.0f; view.m_[3][2] = 0.0f; view.m_[3][3] = 1.0f;

        invview.m_[0][0] = xaxis.x_; invview.m_[0][1] = yaxis.x_; invview.m_[0][2] = zaxis.x_; invview.m_[0][3] = eye.x_;
        invview.m_[1][0] = xaxis.y_; invview.m_[1][1] = yaxis.y_; invview.m_[1][2] = zaxis.y_; invview.m_[1][3] = eye.y_;
        invview.m_[2][0] = xaxis.z_; invview.m_[2][1] = yaxis.z_; invview.m_[2][2] = zaxis.z_; invview.m_[2][3] = eye.z_;
        invview.m_[3][0] = 0.0f; invview.m_[3][1] = 0.0f; invview.m_[3][2] = 0.0f; invview.m_[3][3] = 1.0f;
    }

    void lookAt(Matrix44& view, Matrix44& invview, const Vector3& eye, const Quaternion& rotation)
    {
        lookAt(view, invview, Vector4(eye), rotation);
    }

    void perspective(Matrix44& proj, Matrix44& invproj, f32 width, f32 height, f32 znear, f32 zfar)
    {
        f32 invDepth = zfar / (zfar - znear);
        proj.m_[0][0] = 2.0f*znear/width; proj.m_[0][1] = 0.0f; proj.m_[0][2] = 0.0f; proj.m_[0][3] = 0.0f;
        proj.m_[1][0] = 0.0f; proj.m_[1][1] = 2.0f*znear/height; proj.m_[1][2] = 0.0f; proj.m_[1][3] = 0.0f;
        proj.m_[2][0] = 0.0f; proj.m_[2][1] = 0.0f; proj.m_[2][2] = invDepth; proj.m_[2][3] = -znear*invDepth;
        proj.m_[3][0] = 0.0f; proj.m_[3][1] = 0.0f; proj.m_[3][2] = 1.0f; proj.m_[3][3] = 0.0f;

        invproj.m_[0][0] = 0.5f*width/znear; invproj.m_[0][1] = 0.0f; invproj.m_[0][2] = 0.0f; invproj.m_[0][3] = 0.0f;
        invproj.m_[1][0] = 0.0f; invproj.m_[1][1] = 0.5f*height/znear; invproj.m_[1][2] = 0.0f; invproj.m_[1][3] = 0.0f;
        invproj.m_[2][0] = 0.0f; invproj.m_[2][1] = 0.0f; invproj.m_[2][2] = 0.0f; invproj.m_[2][3] = 1.0f;
        invproj.m_[3][0] = 0.0f; invproj.m_[3][1] = 0.0f; invproj.m_[3][2] = -1.0f/(znear*invDepth); invproj.m_[3][3] = 1.0f/znear;
    }

    void perspectiveFov(Matrix44& proj, Matrix44& invproj, f32 fovy, f32 aspect, f32 znear, f32 zfar)
    {
        f32 htan = lray::tan(0.5f * fovy);
        f32 yscale = 1.0f/htan;
        f32 xscale = yscale / aspect;
        f32 invDepth = zfar / (zfar - znear);

        proj.m_[0][0] = xscale; proj.m_[0][1] = 0.0f; proj.m_[0][2] = 0.0f; proj.m_[0][3] = 0.0f;
        proj.m_[1][0] = 0.0f; proj.m_[1][1] = yscale; proj.m_[1][2] = 0.0f; proj.m_[1][3] = 0.0f;
        proj.m_[2][0] = 0.0f; proj.m_[2][1] = 0.0f; proj.m_[2][2] = invDepth; proj.m_[2][3] = -znear*invDepth;
        proj.m_[3][0] = 0.0f; proj.m_[3][1] = 0.0f; proj.m_[3][2] = 1.0f; proj.m_[3][3] = 0.0f;

        invproj.m_[0][0] = aspect*htan; invproj.m_[0][1] = 0.0f; invproj.m_[0][2] = 0.0f; invproj.m_[0][3] = 0.0f;
        invproj.m_[1][0] = 0.0f; invproj.m_[1][1] = htan; invproj.m_[1][2] = 0.0f; invproj.m_[1][3] = 0.0f;
        invproj.m_[2][0] = 0.0f; invproj.m_[2][1] = 0.0f; invproj.m_[2][2] = 0.0f; invproj.m_[2][3] = 1.0f;
        invproj.m_[3][0] = 0.0f; invproj.m_[3][1] = 0.0f; invproj.m_[3][2] = -1.0f/(znear*invDepth); invproj.m_[3][3] = 1.0f/znear;
    }

    void ortho(Matrix44& proj, Matrix44& invproj, f32 width, f32 height, f32 znear, f32 zfar)
    {
        f32 invDepth = 1.0f/(zfar-znear);
        proj.m_[0][0] = 2.0f/width; proj.m_[0][1] = 0.0f; proj.m_[0][2] = 0.0f; proj.m_[0][3] = 0.0f;
        proj.m_[1][0] = 0.0f; proj.m_[1][1] = 2.0f/height; proj.m_[1][2] = 0.0f; proj.m_[1][3] = 0.0f;
        proj.m_[2][0] = 0.0f; proj.m_[2][1] = 0.0f; proj.m_[2][2] = invDepth; proj.m_[2][3] = -znear*invDepth;
        proj.m_[3][0] = 0.0f; proj.m_[3][1] = 0.0f; proj.m_[3][2] = 0.0f; proj.m_[3][3] = 1.0f;

        invproj.m_[0][0] = 0.5f*width; invproj.m_[0][1] = 0.0f; invproj.m_[0][2] = 0.0f; invproj.m_[0][3] = 0.0f;
        invproj.m_[1][0] = 0.0f; invproj.m_[1][1] = 0.5f*height; invproj.m_[1][2] = 0.0f; invproj.m_[1][3] = 0.0f;
        invproj.m_[2][0] = 0.0f; invproj.m_[2][1] = 0.0f; invproj.m_[2][2] = 1.0f/invDepth; invproj.m_[2][3] = znear;
        invproj.m_[3][0] = 0.0f; invproj.m_[3][1] = 0.0f; invproj.m_[3][2] = 0.0f; invproj.m_[3][3] = 1.0f;
    }
}
