#ifndef INC_LRAY_VECTOR4_H_
#define INC_LRAY_VECTOR4_H_
/**
@file Vector4.h
@author t-sakai
@date 2017/12/13 create
*/
#include "lray.h"

namespace lray
{
    class Vector3;
    class Matrix34;
    class Matrix44;
    class Quaternion;

    //--------------------------------------------
    //---
    //--- Vector4
    //---
    //--------------------------------------------
    class Vector4
    {
    public:
        static const LALIGN16 f32 Identity[4];
        static const Vector4 Forward;
        static const Vector4 Backward;
        static const Vector4 Up;
        static const Vector4 Down;
        static const Vector4 Right;
        static const Vector4 Left;

        Vector4(){}
        explicit inline Vector4(f32 xyzw);
        inline Vector4(f32 x, f32 y, f32 z);
        inline Vector4(f32 x, f32 y, f32 z, f32 w);
        inline Vector4(const lm128& v);
        explicit Vector4(const Vector3& v);
        Vector4(const Vector3& v, f32 w);

        inline static Vector4&& zero();
        inline static Vector4&& one();
        inline static Vector4&& identity();

        inline operator lm128() const;

        inline void set(f32 x, f32 y, f32 z, f32 w);
        void set(const Vector3& v);
        void set(const Vector3& v, f32 w);
        inline void set(f32 v);

        inline f32 operator[](s32 index) const;
        inline f32& operator[](s32 index);
        inline Vector4&& operator-() const;

        inline Vector4& operator+=(const Vector4& v);
        inline Vector4& operator-=(const Vector4& v);

        inline Vector4& operator*=(f32 f);
        inline Vector4& operator/=(f32 f);

        inline Vector4& operator*=(const Vector4& v);
        inline Vector4& operator/=(const Vector4& v);

        bool equals(const Vector4& v) const;
        bool equals(const Vector4& v, f32 epsilon) const;

        inline bool operator==(const Vector4& v) const;
        inline bool operator!=(const Vector4& v) const;

        void setLength();
        f32 length() const;
        f32 lengthSqr() const;

        inline void swap(Vector4& rhs);

        inline bool isNan() const;
        inline bool isZero() const;

        inline Vector4&& getParallelComponent(const Vector4& basis) const
        {
            f32 cs = dot(*this, basis);
            return Vector4(basis*cs);
        }

        inline Vector4&& getPerpendicularComponent(const Vector4& basis) const
        {
            return Vector4(*this - getParallelComponent(basis));
        }

        inline static lm128&& load(const Vector4& v)
        {
            return move(_mm_loadu_ps(&v.x_));
        }

        inline static void store(Vector4& dst, const lm128& src)
        {
            _mm_storeu_ps(&dst.x_, src);
        }

        inline static Vector4&& store(lm128& r)
        {
            Vector4 v;
            _mm_storeu_ps(&v.x_, r);
            return move(v);
        }

        //--- Friend functions
        //--------------------------------------------------
        friend Vector4&& normalize(const Vector4& v);
        friend Vector4&& normalize(const Vector4& v, f32 lengthSqr);
        friend Vector4&& normalizeChecked(const Vector4& v);
        friend Vector4&& absolute(const Vector4& v);

        friend f32 dot(const Vector4& v0, const Vector4& v1);
        friend lm128 cross3(const Vector4& v0, const Vector4& v1);
        friend f32 distanceSqr(const Vector4& v0, const Vector4& v1);
        friend f32 distance(const Vector4& v0, const Vector4& v1);
        friend f32 manhattanDistance(const Vector4& v0, const Vector4& v1);

        friend f32 distanceSqr3(const Vector3& v0, const Vector4& v1);
        friend f32 distanceSqr3(const Vector4& v0, const Vector3& v1);
        friend f32 distanceSqr3(const Vector4& v0, const Vector4& v1);
        friend f32 distance3(const Vector3& v0, const Vector4& v1);
        friend f32 distance3(const Vector4& v0, const Vector3& v1);
        friend f32 distance3(const Vector4& v0, const Vector4& v1);
        friend f32 manhattanDistance3(const Vector3& v0, const Vector4& v1);
        friend f32 manhattanDistance3(const Vector4& v0, const Vector3& v1);
        friend f32 manhattanDistance3(const Vector4& v0, const Vector4& v1);

        friend Vector4&& mul(const Matrix34& m, const Vector4& v);
        friend Vector4&& mul(const Vector4& v, const Matrix34& m);

        friend Vector4&& mul(const Matrix44& m, const Vector4& v);
        friend Vector4&& mul(const Vector4& v, const Matrix44& m);

        friend Vector4&& mulPoint(const Matrix44& m, const Vector4& v);
        friend Vector4&& mulVector(const Matrix44& m, const Vector4& v);

        friend Vector4 rotate(const Vector4& v, const Quaternion& rotation);
        friend Vector4 rotate(const Quaternion& rotation, const Vector4& v);

        friend Vector4&& operator+(const Vector4& v0, const Vector4& v1);
        friend Vector4&& operator-(const Vector4& v0, const Vector4& v1);
        friend Vector4&& operator*(f32 f, const Vector4& v);
        friend Vector4&& operator*(const Vector4& v, f32 f);
        friend Vector4&& operator*(const Vector4& v0, const Vector4& v1);
        friend Vector4&& operator/(const Vector4& v, f32 f);
        friend Vector4&& operator/(const Vector4& v0, const Vector4& v1);
        friend Vector4&& add(const Vector4& v0, const Vector4& v1);
        friend Vector4&& sub(const Vector4& v0, const Vector4& v1);
        friend Vector4&& mul(const Vector4& v0, const Vector4& v1);
        friend Vector4&& mul(f32 f, const Vector4& v);
        friend Vector4&& mul(const Vector4& v0, f32 f);
        friend Vector4&& div(const Vector4& v0, const Vector4& v1);
        friend Vector4&& div(const Vector4& v0, f32 f);

        friend Vector4&& add(const Vector4& v, f32 f);
        friend Vector4&& sub(const Vector4& v, f32 f);

        friend Vector4&& minimum(const Vector4& v0, const Vector4& v1);
        friend Vector4&& maximum(const Vector4& v0, const Vector4& v1);

        friend f32 minimum(const Vector4& v);
        friend f32 maximum(const Vector4& v);

        /**
        @brief v0*v1 + v2
        */
        friend Vector4&& muladd(const Vector4& v0, const Vector4& v1, const Vector4& v2);

        /**
        @brief x*v0 + v1
        */
        friend Vector4&& muladd(f32 x, const Vector4& v0, const Vector4& v1);

        friend Vector4&& floor(const Vector4& v);
        friend Vector4&& ceil(const Vector4& v);
        friend Vector4&& invert(const Vector4& v);

        friend Vector4&& sqrt(const Vector4& v);

        /**
        @brief v0 * (1-t) + v1 * t
        */
        friend Vector4&& lerp(const Vector4& v0, const Vector4& v1, f32 t);

        /**
        @brief v0 * (1-t) + v1 * t
        */
        friend Vector4&& slerp(const Vector4& v0, const Vector4& v1, f32 t);
        /**
        @brief v0 * (1-t) + v1 * t
        */
        friend Vector4&& slerp(const Vector4& v0, const Vector4& v1, f32 t, f32 cosine);

        //--- Members
        //--------------------------------------------------
        f32 x_;
        f32 y_;
        f32 z_;
        f32 w_;
    };

    static_assert(std::is_trivially_copyable<Vector4>::value == true, "Vector4 must be trivially copyable.");

    inline Vector4::Vector4(f32 xyzw)
    {
        _mm_storeu_ps(&x_, _mm_set1_ps(xyzw));
    }

    inline Vector4::Vector4(f32 x, f32 y, f32 z)
        :x_(x)
        ,y_(y)
        ,z_(z)
        ,w_(0.0f)
    {
    }

    inline Vector4::Vector4(f32 x, f32 y, f32 z, f32 w)
        :x_(x)
        ,y_(y)
        ,z_(z)
        ,w_(w)
    {
    }

    inline Vector4::Vector4(const lm128& v)
    {
        _mm_storeu_ps(&x_, v);
    }

    inline Vector4&& Vector4::zero()
    {
        Vector4 v;
        _mm_storeu_ps(&v.x_, _mm_setzero_ps());
        return move(v);
    }

    inline Vector4&& Vector4::one()
    {
        Vector4 v;
        _mm_storeu_ps(&v.x_, _mm_set1_ps(1.0f));
        return move(v);
    }

    inline Vector4&& Vector4::identity()
    {
        Vector4 v;
        _mm_storeu_ps(&v.x_, _mm_load_ps(Identity));
        return move(v);
    }

    inline Vector4::operator lm128() const
    {
        return _mm_loadu_ps(&x_);
    }

    inline void Vector4::set(f32 x, f32 y, f32 z, f32 w)
    {
        x_ = x; y_ = y; z_ = z; w_ = w;
    }

    inline void Vector4::set(f32 v)
    {
#ifdef LRAY_USE_SSE
        lm128 t = _mm_set1_ps(v);
        store(*this, t);
#else
        x_ = y_ = z_ = w_ = v;
#endif
    }

    inline f32 Vector4::operator[](s32 index) const
    {
        LASSERT(0<=index && index<4);
        return (&x_)[index];
    }

    inline f32& Vector4::operator[](s32 index)
    {
        LASSERT(0<=index && index<4);
        return (&x_)[index];
    }

    inline Vector4&& Vector4::operator-() const
    {
#ifdef LRAY_USE_SSE
        f32 f;
        *((u32*)&f) = 0x80000000U;
        lm128 mask = _mm_set1_ps(f);
        lm128 r0 = load(*this);
        r0 = _mm_xor_ps(r0, mask);

        Vector4 ret;
        store(ret, r0);
        return move(ret);
#else
        return Vector4::construct(-x_, -y_, -z_, -w_);
#endif
    }

    inline Vector4& Vector4::operator+=(const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = load(*this);
        lm128 r1 = load(v);
        r0 = _mm_add_ps(r0, r1);
        store(*this, r0);
#else
        x_ += v.x_;
        y_ += v.y_;
        z_ += v.z_;
        w_ += v.w_;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator-=(const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = load(*this);
        lm128 r1 = load(v);
        r0 = _mm_sub_ps(r0, r1);
        store(*this, r0);
#else
        x_ -= v.x_;
        y_ -= v.y_;
        z_ -= v.z_;
        w_ -= v.w_;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator*=(f32 f)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = load(*this);
        lm128 r1 = _mm_set1_ps(f);
        r0 = _mm_mul_ps(r0, r1);
        store(*this, r0);
#else
        x_ *= f;
        y_ *= f;
        z_ *= f;
        w_ *= f;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator/=(f32 f)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = load(*this);
        lm128 r1 = _mm_set1_ps(f);
        r0 = _mm_div_ps(r0, r1);
        store(*this, r0);
#else
        LASSERT(!lray::isEqual(f, 0.0f));
        f = 1.0f / f;
        x_ *= f;
        y_ *= f;
        z_ *= f;
        w_ *= f;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator*=(const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = load(*this);
        lm128 r1 = load(v);
        r0 = _mm_mul_ps(r0, r1);
        store(*this, r0);
#else
        x_ *= v.x_;
        y_ *= v.y_;
        z_ *= v.z_;
        w_ *= v.w_;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator/=(const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = load(*this);
        lm128 r1 = load(v);
        r0 = _mm_div_ps(r0, r1);
        store(*this, r0);
#else
        x_ /= v.x_;
        y_ /= v.y_;
        z_ /= v.z_;
        w_ /= v.w_;
#endif
        return *this;
    }

    inline bool Vector4::operator==(const Vector4& v) const
    {
        return equals(v);
    }

    inline bool Vector4::operator!=(const Vector4& v) const
    {
        return !equals(v);
    }

    inline void Vector4::swap(Vector4& rhs)
    {
#ifdef LRAY_USE_SSE
        lm128 t0 = load(*this);
        lm128 t1 = load(rhs);

        store(*this, t1);
        store(rhs, t0);
#else
        lray::swap(x_, rhs.x_);
        lray::swap(y_, rhs.y_);
        lray::swap(z_, rhs.z_);
        lray::swap(w_, rhs.w_);
#endif
    }

    inline bool Vector4::isNan() const
    {
        return (lray::isNan(x_) || lray::isNan(y_) || lray::isNan(z_) || lray::isNan(w_));
    }

    inline bool Vector4::isZero() const
    {
        return (lray::isZero(x_) && lray::isZero(y_) && lray::isZero(z_) && lray::isZero(w_));
    }


    //--- Friend functions
    //--------------------------------------------------
    Vector4&& normalize(const Vector4& v);
    Vector4&& normalize(const Vector4& v, f32 lengthSqr);
    Vector4&& normalizeChecked(const Vector4& v);
    Vector4&& absolute(const Vector4& v);

    f32 dot(const Vector4& v0, const Vector4& v1);
    lm128 cross3(const Vector4& v0, const Vector4& v1);
    f32 distanceSqr(const Vector4& v0, const Vector4& v1);
    f32 distance(const Vector4& v0, const Vector4& v1);
    f32 manhattanDistance(const Vector4& v0, const Vector4& v1);

    f32 distanceSqr3(const Vector3& v0, const Vector4& v1);
    inline f32 distanceSqr3(const Vector4& v0, const Vector3& v1)
    {
        return distanceSqr3(v1, v0);
    }
    f32 distanceSqr3(const Vector4& v0, const Vector4& v1);
    f32 distance3(const Vector3& v0, const Vector4& v1);
    inline f32 distance3(const Vector4& v0, const Vector3& v1)
    {
        return distance3(v1, v0);
    }
    f32 distance3(const Vector4& v0, const Vector4& v1);
    f32 manhattanDistance3(const Vector3& v0, const Vector4& v1);
    inline f32 manhattanDistance3(const Vector4& v0, const Vector3& v1)
    {
        return manhattanDistance3(v1, v0);
    }
    f32 manhattanDistance3(const Vector4& v0, const Vector4& v1);

    Vector4&& mul(const Matrix34& m, const Vector4& v);
    Vector4&& mul(const Vector4& v, const Matrix34& m);

    Vector4&& mul(const Matrix44& m, const Vector4& v);
    Vector4&& mul(const Vector4& v, const Matrix44& m);    

    Vector4&& mulPoint(const Matrix44& m, const Vector4& v);
    Vector4&& mulVector(const Matrix44& m, const Vector4& v);

    Vector4 rotate(const Vector4& v, const Quaternion& rotation);
    Vector4 rotate(const Quaternion& rotation, const Vector4& v);

    inline Vector4&& operator+(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        return Vector4(_mm_add_ps(r0, r1));
#else
        f32 x = v0.x_ + v1.x_;
        f32 y = v0.y_ + v1.y_;
        f32 z = v0.z_ + v1.z_;
        f32 w = v0.w_ + v1.w_;
        return Vector4(x,y,z,w);
#endif
    }

    inline Vector4&& operator-(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        return move(Vector4(_mm_sub_ps(r0, r1)));
#else
        f32 x = v0.x_ - v1.x_;
        f32 y = v0.y_ - v1.y_;
        f32 z = v0.z_ - v1.z_;
        f32 w = v0.w_ - v1.w_;
        return Vector4(x,y,z,w);
#endif
    }

    inline Vector4&& operator*(f32 f, const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = _mm_set1_ps(f);
        lm128 r1 = Vector4::load(v);
        return Vector4(_mm_mul_ps(r0, r1));
#else
        f32 x = f * v.x_;
        f32 y = f * v.y_;
        f32 z = f * v.z_;
        f32 w = f * v.w_;
        return Vector4(x,y,z,w);
#endif
    }

    inline Vector4&& operator*(const Vector4& v, f32 f)
    {
        return f*v;
    }

    inline Vector4&& operator*(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        return Vector4(_mm_mul_ps(r0, r1));
#else
        return {v0.x_*v1.x_, v0.y_*v1.y_, v0.z_*v1.z_, v0.w_*v1.w_};
#endif
    }

    inline Vector4&& operator/(const Vector4& v, f32 f)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v);
        lm128 r1 = _mm_set1_ps(f);
        return Vector4(_mm_div_ps(r0, r1));
#else
        f = 1.0f/f;
        return v*f;
#endif
    }

    inline Vector4&& operator/(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        return Vector4(_mm_div_ps(r0, r1));
#else
        return {v0.x_/v1.x_, v0.y_/v1.y_, v0.z_/v1.z_, v0.w_/v1.w_};
#endif
    }

    inline Vector4&& add(const Vector4& v0, const Vector4& v1)
    {
        return v0+v1;
    }

    inline Vector4&& sub(const Vector4& v0, const Vector4& v1)
    {
        return v0-v1;
    }

    inline Vector4&& mul(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        return Vector4(_mm_mul_ps(r0, r1));
#else
        f32 x = v0.x_ * v1.x_;
        f32 y = v0.y_ * v1.y_;
        f32 z = v0.z_ * v1.z_;
        f32 w = v0.w_ * v1.w_;
        return {x,y,z,w};
#endif
    }

    inline Vector4&& mul(f32 f, const Vector4& v)
    {
        return f*v;
    }

    inline Vector4&& mul(const Vector4& v, f32 f)
    {
        return v*f;
    }

    inline Vector4&& div(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        return Vector4(_mm_div_ps(r0, r1));
#else
        f32 x = v0.x_ / v1.x_;
        f32 y = v0.y_ / v1.y_;
        f32 z = v0.z_ / v1.z_;
        f32 w = v0.w_ / v1.w_;
        return {x,y,z,w};
#endif
    }

    inline Vector4&& div(const Vector4& v0, f32 f)
    {
        return v0/f;
    }


    inline Vector4&& add(const Vector4& v, f32 f)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v);
        lm128 r1 = _mm_set1_ps(f);
        return Vector4(_mm_add_ps(r0, r1));
#else
        return {v.x_+f, v.y_+f, v.z_+f, v.w_+f};
#endif
    }

    inline Vector4&& sub(const Vector4& v, f32 f)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v);
        lm128 r1 = _mm_set1_ps(f);
        return Vector4(_mm_sub_ps(r0, r1));
#else
        return {v.x_-f, v.y_-f, v.z_-f, v.w_-f};
#endif
    }

    inline Vector4&& minimum(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        return Vector4(_mm_min_ps(r0, r1));
#else
        f32 x = lray::minimum(v0.x_, v1.x_);
        f32 y = lray::minimum(v0.y_, v1.y_);
        f32 z = lray::minimum(v0.z_, v1.z_);
        f32 w = lray::minimum(v0.w_, v1.w_);
        return {x,y,z,w};
#endif
    }

    inline Vector4&& maximum(const Vector4& v0, const Vector4& v1)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v0);
        lm128 r1 = Vector4::load(v1);
        return Vector4(_mm_max_ps(r0, r1));
#else
        f32 x = lray::maximum(v0.x_, v1.x_);
        f32 y = lray::maximum(v0.y_, v1.y_);
        f32 z = lray::maximum(v0.z_, v1.z_);
        f32 w = lray::maximum(v0.w_, v1.w_);
        return {x,y,z,w};
#endif
    }

    inline f32 minimum(const Vector4& v)
    {
        return lray::minimum(lray::minimum(v.x_, v.y_), lray::minimum(v.z_, v.w_));
    }

    inline f32 maximum(const Vector4& v)
    {
        return lray::maximum(lray::maximum(v.x_, v.y_), lray::maximum(v.z_, v.w_));
    }

    /**
    @brief v0*v1 + v2
    */
    Vector4&& muladd(const Vector4& v0, const Vector4& v1, const Vector4& v2);

    /**
    @brief a*v0 + v1
    */
    Vector4&& muladd(f32 a, const Vector4& v0, const Vector4& v1);

    Vector4&& floor(const Vector4& v);
    Vector4&& ceil(const Vector4& v);

    inline Vector4&& invert(const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = _mm_set1_ps(1.0f);
        lm128 r1 = Vector4::load(v);
        return Vector4(_mm_div_ps(r0, r1));
#else
        f32 x = 1.0f/v.x_;
        f32 y = 1.0f/v.y_;
        f32 z = 1.0f/v.z_;
        f32 w = 1.0f/v.w_;
        return {x,y,z,w};
#endif
    }

    inline Vector4&& sqrt(const Vector4& v)
    {
#ifdef LRAY_USE_SSE
        lm128 r0 = Vector4::load(v);
        return Vector4(_mm_sqrt_ps(r0));
#else
        f32 x = lray::sqrt(v.x_);
        f32 y = lray::sqrt(v.y_);
        f32 z = lray::sqrt(v.z_);
        f32 w = lray::sqrt(v.w_);
        return {x,y,z,w};
#endif
    }

    /**
    @brief v0 * (1-t) + v1 * t
    */
    Vector4&& lerp(const Vector4& v0, const Vector4& v1, f32 t);

    /**
    @brief v0 * (1-t) + v1 * t
    */
    Vector4&& slerp(const Vector4& v0, const Vector4& v1, f32 t);

    /**
    @brief v0 * (1-t) + v1 * t
    */
    Vector4&& slerp(const Vector4& v0, const Vector4& v1, f32 t, f32 cosine);

}
#endif //INC_LRAY_VECTOR4_H_
