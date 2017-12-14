#ifndef INC_LRAY_MATRIX44_H_
#define INC_LRAY_MATRIX44_H_
/**
@file Matrix44.h
@author t-sakai
@date 2017/12/13 create
*/
#include "lray.h"

namespace lray
{
    class Vector3;
    class Vector4;
    class Matrix34;
    class Quaternion;

    //--------------------------------------------
    //---
    //--- Matrix44
    //---
    //--------------------------------------------
    /**
    左手座標系、列ベクトル、ベクトルに対して左から掛けて変換する。
    */
    class Matrix44
    {
    public:
        static const Matrix44 identity_;

        Matrix44(){}

        explicit Matrix44(const Matrix34& mat);

        Matrix44(f32 m00, f32 m01, f32 m02, f32 m03,
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23,
            f32 m30, f32 m31, f32 m32, f32 m33);

        /// 値セット
        void set(f32 m00, f32 m01, f32 m02, f32 m03,
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23,
            f32 m30, f32 m31, f32 m32, f32 m33);

        /// 値セット
        void set(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);

        inline f32 operator()(s32 r, s32 c) const;
        inline f32& operator()(s32 r, s32 c);

        Matrix44& operator*=(f32 f);
        Matrix44& operator*=(const Matrix44& rhs)
        {
            mul(*this, rhs);
            return *this;
        }

        Matrix44& operator+=(const Matrix44& rhs);
        Matrix44& operator-=(const Matrix44& rhs);
        Matrix44& operator=(const Matrix34& rhs);

        Matrix44& operator*=(const Matrix34& rhs)
        {
            mul(*this, rhs);
            return *this;
        }

        Matrix44 operator-() const;

        void mul(const Matrix44& m0, const Matrix44& m1);
        void mul(const Matrix34& m0, const Matrix44& m1);
        void mul(const Matrix44& m0, const Matrix34& m1);

        void zero();
        void identity();
        inline void transpose();
        void transpose(const Matrix44& src);
        void getTranspose(Matrix44& dst) const;

        f32 determinant() const;
        inline void invert();
        void getInvert(Matrix44& dst) const;

        /// 3x3部分行列の転置
        void transpose33();

        /// 3x3部分行列の行列式
        f32 determinant33() const;

        /// 3x3部分行列の逆行列
        inline void invert33();
        void getInvert33(Matrix44& dst) const;

        /// 平行移動セット
        void setTranslate(const Vector3& v);
        void setTranslate(const Vector4& v);

        /// 平行移動セット
        inline void setTranslate(f32 x, f32 y, f32 z);

        /// 後から平行移動
        void translate(const Vector3& v);
        void translate(const Vector4& v);

        /// 後から平行移動
        void translate(f32 x, f32 y, f32 z);

        /// 前から平行移動
        void preTranslate(const Vector3& v);

        /// 前から平行移動
        void preTranslate(f32 x, f32 y, f32 z);

        /// 後から回転
        void rotateX(f32 radian);

        /// 後から回転
        void rotateY(f32 radian);

        /// 後から回転
        void rotateZ(f32 radian);

        /// X軸回転
        void setRotateX(f32 radian);

        /// Y軸回転
        void setRotateY(f32 radian);

        /// Z軸回転
        void setRotateZ(f32 radian);

        /// 軸回転
        void setRotateAxis(const Vector3& axis, f32 radian);

        /// 軸回転
        void setRotateAxis(f32 x, f32 y, f32 z, f32 radian);

        inline void setScale(f32 s);
        inline void scale(f32 s);

        inline void setScale(f32 x, f32 y, f32 z);
        inline void scale(f32 x, f32 y, f32 z);

        void lookAt(const Vector4& eye, const Vector4& at, const Vector4& up);
        void lookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

        void lookAt(const Vector4& at);
        void lookAt(const Vector3& at);

        void lookAt(const Vector4& eye, const Quaternion& rotation);
        void lookAt(const Vector3& eye, const Quaternion& rotation);

        void viewPointAlign(const Matrix44& view, const Vector4& position);
        void axisAlign(const Vector4& axis, const Matrix44& view, const Vector4& position);

        /**
        @brief 透視投影
        */
        void perspective(f32 width, f32 height, f32 znear, f32 zfar);

        /**
        @brief 透視投影
        */
        void perspectiveFov(f32 fovy, f32 aspect, f32 znear, f32 zfar);

        /**
        @brief 透視投影. Reverse-Z
        */
        void perspectiveReverseZ(f32 width, f32 height, f32 znear, f32 zfar);

        /**
        @brief 透視投影. Reverse-Z
        */
        void perspectiveFovReverseZ(f32 fovy, f32 aspect, f32 znear, f32 zfar);

        /**
        @brief 透視投影。リニアＺ版
        */
        void perspectiveLinearZ(f32 width, f32 height, f32 znear, f32 zfar);

        /**
        @brief 透視投影。リニアＺ版
        */
        void perspectiveFovLinearZ(f32 fovy, f32 aspect, f32 znear, f32 zfar);

        /**
        @brief 平行投影
        */
        void ortho(f32 width, f32 height, f32 znear, f32 zfar);
        void orthoOffsetCenter(f32 left, f32 right, f32 top, f32 bottom, f32 znear, f32 zfar);

        void orthoReverseZ(f32 width, f32 height, f32 znear, f32 zfar);
        void orthoOffsetCenterReverseZ(f32 left, f32 right, f32 top, f32 bottom, f32 znear, f32 zfar);

        void getTranslate(f32& x, f32& y, f32& z) const
        {
            x = m_[0][3];
            y = m_[1][3];
            z = m_[2][3];
        }

        void getTranslate(Vector3& trans) const;
        void getTranslate(Vector4& trans) const;

        void getRow(Vector3& dst, s32 row) const;
        void getRow(Vector4& dst, s32 row) const;

        bool isNan() const;

        void getRotation(Quaternion& rotation) const;

        f32 m_[4][4];
    };

    static_assert(std::is_trivially_copyable<Matrix44>::value == true, "Matrix44 must be trivially copyable.");

    inline f32 Matrix44::operator()(s32 r, s32 c) const
    {
        LASSERT(0<=r && r<4);
        LASSERT(0<=c && c<4);
        return m_[r][c];
    }

    inline f32& Matrix44::operator()(s32 r, s32 c)
    {
        LASSERT(0<=r && r<4);
        LASSERT(0<=c && c<4);
        return m_[r][c];
    }

    inline void Matrix44::transpose()
    {
        getTranspose(*this);
    }

    inline void Matrix44::invert()
    {
        getInvert(*this);
    }

    inline void Matrix44::invert33()
    {
        getInvert33(*this);
    }

    // 平行移動セット
    inline void Matrix44::setTranslate(f32 x, f32 y, f32 z)
    {
        m_[0][3] = x;
        m_[1][3] = y;
        m_[2][3] = z;
    }

    inline void Matrix44::setScale(f32 s)
    {
        m_[0][0] = s;
        m_[1][1] = s;
        m_[2][2] = s;
    }

    inline void Matrix44::scale(f32 s)
    {
        m_[0][0] *= s;
        m_[1][1] *= s;
        m_[2][2] *= s;
    }

    inline void Matrix44::setScale(f32 x, f32 y, f32 z)
    {
        m_[0][0] = x;
        m_[1][1] = y;
        m_[2][2] = z;
    }

    inline void Matrix44::scale(f32 x, f32 y, f32 z)
    {
        m_[0][0] *= x;
        m_[1][1] *= y;
        m_[2][2] *= z;
    }

    //------------------------------------------------------------------------------------------------
    void lookAt(Matrix44& view, Matrix44& invview, const Vector4& eye, const Vector4& at, const Vector4& up);
    void lookAt(Matrix44& view, Matrix44& invview, const Vector3& eye, const Vector3& at, const Vector3& up);

    void lookAt(Matrix44& view, Matrix44& invview, const Vector4& at);
    void lookAt(Matrix44& view, Matrix44& invview, const Vector3& at);

    void lookAt(Matrix44& view, Matrix44& invview, const Vector4& eye, const Quaternion& rotation);
    void lookAt(Matrix44& view, Matrix44& invview, const Vector3& eye, const Quaternion& rotation);

    void perspective(Matrix44& proj, Matrix44& invproj, f32 width, f32 height, f32 znear, f32 zfar);
    void perspectiveFov(Matrix44& proj, Matrix44& invproj, f32 fovy, f32 aspect, f32 znear, f32 zfar);

    void ortho(Matrix44& proj, Matrix44& invproj, f32 width, f32 height, f32 znear, f32 zfar);
}

#endif //INC_LRAY_MATRIX44_H_
