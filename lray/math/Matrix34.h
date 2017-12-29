#ifndef INC_LRAY_MATRIX34_H_
#define INC_LRAY_MATRIX34_H_
/**
@file Matrix34.h
@author t-sakai
@date 2017/12/13 create
*/
#include "lray.h"

namespace lray
{
    class Vector3;
    class Vector4;
    class Matrix44;
    class Quaternion;

    //--------------------------------------------
    //---
    //--- Matrix34
    //---
    //--------------------------------------------
    /**
    Left handed
    */
    class Matrix34
    {
    public:
        Matrix34(){}

        Matrix34(
            f32 m00, f32 m01, f32 m02, f32 m03,
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23);

        explicit Matrix34(const Matrix44& rhs);

        void set(
            f32 m00, f32 m01, f32 m02, f32 m03,
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23);

        inline f32 operator()(s32 r, s32 c) const;

        inline f32& operator()(s32 r, s32 c);

        Matrix34& operator*=(f32 f);
        Matrix34& operator*=(const Matrix34& rhs);
        Matrix34& operator+=(const Matrix34& rhs);
        Matrix34& operator-=(const Matrix34& rhs);

        Matrix34 operator-() const;

        void identity();

        void transpose33();

        f32 determinant33() const;

        void invert33();

        void invert();

        void setTranslate(const Vector3& v);

        inline void setTranslate(f32 x, f32 y, f32 z);

        void translate(const Vector3& v);

        void translate(const Vector4& v);

        void translate(f32 x, f32 y, f32 z);

        void preTranslate(const Vector3& v);

        void preTranslate(f32 x, f32 y, f32 z);

        void rotateX(f32 radian);

        void rotateY(f32 radian);

        void rotateZ(f32 radian);

        void setRotateAxis(const Vector3& axis, f32 radian);

        void setRotateAxis(f32 x, f32 y, f32 z, f32 radian);

        inline void setScale(f32 s);
        inline void scale(f32 s);

        void mul(const Matrix34& m0, const Matrix34& m1);

        Matrix34& operator*=(const Matrix44& rhs);

        bool isNan() const;

        void getRotation(Quaternion& rotation) const;

        f32 m_[3][4];
    };

    static_assert(std::is_trivially_copyable<Matrix34>::value == true, "Matrix34 must be trivially copyable.");

    inline f32 Matrix34::operator()(s32 r, s32 c) const
    {
        LASSERT(0<= r && r < 3);
        LASSERT(0<= c && c < 4);

        return m_[r][c];
    }

    inline f32& Matrix34::operator()(s32 r, s32 c)
    {
        LASSERT(0<= r && r < 3);
        LASSERT(0<= c && c < 4);

        return m_[r][c];
    }

    inline void Matrix34::setTranslate(f32 x, f32 y, f32 z)
    {
        m_[0][3] = x;
        m_[1][3] = y;
        m_[2][3] = z;
    }

    inline void Matrix34::setScale(f32 s)
    {
        m_[0][0] = s;
        m_[1][1] = s;
        m_[2][2] = s;
    }

    inline void Matrix34::scale(f32 s)
    {
        m_[0][0] *= s;
        m_[1][1] *= s;
        m_[2][2] *= s;
    }
}
#endif //INC_LRAY_MATRIX34_H_
