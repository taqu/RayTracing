#ifndef INC_LRAY_H_
#define INC_LRAY_H_
/**
@file lray.h
@author t-sakai
@date 2017/12/13 create
*/
#include <cassert>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <limits>
#include <new>
#include <type_traits>

//#include <mmintrin.h>  //MMX instructions
//#include <xmmintrin.h> //SSE instructions
//#include <emmintrin.h> //SSE2 instructions
#include <immintrin.h> //AVX instructions

#if defined(ANDROID)
#include <android/log.h>
#endif //ANDROID

namespace lray
{
    //C++98 199711L
    //C++11 201103L
#ifdef __cplusplus
#   if 201103L<=__cplusplus || 1900<=_MSC_VER
#       define LCPP11 1
#   endif
#endif

#ifdef NULL
#   ifdef __cplusplus
#       ifdef LCPP11
#           undef NULL
#           define NULL nullptr
#       endif
#   endif
#else
#   ifdef __cplusplus
#       ifdef LCPP11
#           define NULL nullptr
#       else
#           define NULL 0
#       endif
#   else
#       define NULL ((void*)0)
#   endif
#endif

#ifdef __cplusplus
#   ifdef LCPP11
#       define LNOTHROW noexcept
#   else
#       define LNOTHROW throw()
#   endif
#else
#    define LNOTHROW
#endif

    typedef char Char;
    typedef unsigned char UChar;
    typedef char16_t Char16;
    typedef wchar_t WChar;
    typedef int8_t s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;

    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef float f32;
    typedef double f64;

    typedef intptr_t  intptr_t;
    typedef uintptr_t  uintptr_t;
    typedef ptrdiff_t  ptrdiff_t;
    typedef size_t size_t;

    typedef __m128 lm128;
    typedef __m128i lm128i;
    typedef __m64 lm64;

#if defined(ANDROID) || defined(__GNUC__)
    typedef clock_t ClockType;
#else
    typedef u64 ClockType;
#endif

    static const u32 SSE_ALIGN = 16;
    static const u32 SSE_ALIGN_MASK = 15;
    static const uintptr_t LALIGN16_MASK = (0xFU);
    static const Char CharNull = '\0';
    static const Char CharLF = '\n'; //Line Feed
    static const Char CharCR = '\r'; //Carriage Return
    static const Char PathDelimiter = '/';

#define LRAY_USE_SSE

    //--- Assertion
    //---------------------------------------------------------
#if defined(_DEBUG)
#   if defined(ANDROID)
#       define LASSERT(expression) {if(false == (expression)){__android_log_assert("assert", "lime", "%s (%d)", __FILE__, __LINE__);}}while(0)

#   elif defined(__GNUC__)
#       define LASSERT(expression) assert(expression)
#   else
#      define LASSERT(expression) assert(expression)
#   endif
#else
#   define LASSERT(expression)
#endif
}

//--- Overload new/delete functions
//---------------------------------------------------------
void* operator new(std::size_t size);
void operator delete(void* ptr);
void* operator new[](std::size_t size);
void operator delete[](void* ptr);

void* operator new(std::size_t size, const char* file, int line);
void operator delete(void* ptr, const char* file, int line);
void* operator new[](std::size_t size, const char* file, int line);
void operator delete[](void* ptr, const char* file, int line);

namespace lray
{
#ifdef _DEBUG
#    define LNEW new(__FILE__,__LINE__)
#    define LNEW_RAW new
#else //_DEBUG
#    define LNEW new
#    define LNEW_RAW new
#endif

    //--- Alloc/Deallocate
    //---------------------------------------------------------
    void* lmalloc(size_t size);
    void* lmalloc(size_t size, u32 alignment);

    void lfree(void* ptr);
    void lfree(void* ptr, u32 alignment);

    void* lmalloc(size_t size, const Char* file, s32 line);
    void* lmalloc(size_t size, u32 alignment, const Char* file, s32 line);

    //--- Alignment 
    //---------------------------------------------------------
#ifdef _MSC_VER
#define LALIGN16 __declspec(align(16))
#define LALIGN(a) __declspec(align(a))
#define LALIGN_VAR16(type,x) __declspec(align(16)) type x
#define LALIGN_VAR(a,type,x) __declspec(align(a)) type x
#else
#define LALIGN16 __attribute__((aligned(16)))
#define LALIGN(a) __attribute__((aligned(a)))
#define LALIGN_VAR16(type,x) type x __attribute__((aligned(16)))
#define LALIGN_VAR(a,type,x) type x __attribute__((aligned(a)))
#endif

    //--- Allocation
    //---------------------------------------------------------
#if defined(_DEBUG)

#define LPLACEMENT_NEW(ptr) new(ptr)
#define LDELETE(ptr) delete (ptr); (ptr)=NULL
#define LDELETE_RAW(ptr) delete (ptr)

#define LDELETE_ARRAY(ptr) delete[] (ptr); (ptr)=NULL

#define LMALLOC(size) lmalloc(size, __FILE__, __LINE__)
#define LMALLOC_DEBUG(size, file, line) lmalloc(size, file, line)
#define LFREE(ptr) lfree(ptr); (ptr)=NULL
#define LFREE_RAW(ptr) lfree(ptr)

    ///Aligned alloc
#define LALIGNED_MALLOC(size, align) lmalloc(size, align, __FILE__, __LINE__)
#define LALIGNED_MALLOC_DEBUG(size, align, file, line) lmalloc(size, align, file, line)
    ///Aligned free
#define LALIGNED_FREE(ptr, align) lfree(ptr, align); (ptr)=NULL
#define LALIGNED_FREE_RAW(ptr, align) lfree(ptr, align)

#else //defined(_DEBUG)

#define LPLACEMENT_NEW(ptr) new(ptr)
#define LDELETE(ptr) delete ptr; (ptr)=NULL
#define LDELETE_RAW(ptr) delete (ptr)

#define LDELETE_ARRAY(ptr) delete[] (ptr); (ptr)=NULL

#define LMALLOC(size) lmalloc(size)
#define LMALLOC_DEBUG(size, file, line) lmalloc(size)
#define LFREE(ptr) lfree(ptr); (ptr)=NULL
#define LFREE_RAW(ptr) lfree(ptr)

    ///Aligned alloc
#define LALIGNED_MALLOC(size, align) lmalloc(size, align)
#define LALIGNED_MALLOC_DEBUG(size, align, file, line) lmalloc(size, align)
    ///Aligned free
#define LALIGNED_FREE(ptr, align) lfree(ptr, align); (ptr)=NULL
#define LALIGNED_FREE_RAW(ptr, align) lfree(ptr, align)
#endif
    
    //--- STL utilities
    //---------------------------------------------------------
    using std::move;
    using std::forward;
    using std::is_pod;
    using std::true_type;
    using std::false_type;
    using std::declval;

    //--- numeric_limits
    //---------------------------------------------------------
    template<typename T>
    class numeric_limits
    {
    public:
        static constexpr T epsilon()
        {
            return std::numeric_limits<T>::epsilon();
        }

        static constexpr T minimum()
        {
            return (std::numeric_limits<T>::min)();
        }

        static constexpr T maximum()
        {
            return (std::numeric_limits<T>::max)();
        }

        static constexpr T lowest()
        {
            return std::numeric_limits<T>::lowest();
        }

        static constexpr T inifinity()
        {
            return std::numeric_limits<T>::infinity();
        }
    };

    //--- Constants
    //---------------------------------------------------------
#if defined(ANDROID)
    static constexpr f32 F32_EPSILON = 1.192092896e-07F;
    static constexpr f64 F64_EPSILON = 2.2204460492503131e-016;
    static constexpr f32 F32_MAX = 3.402823466e+38F;
    static constexpr f64 F64_MAX = 1.7976931348623158e+308;
#else
    static constexpr f32 F32_EPSILON = FLT_EPSILON;
    static constexpr f64 F64_EPSILON = DBL_EPSILON;
    static constexpr f32 F32_MAX = numeric_limits<f32>::maximum();
    static constexpr f64 F64_MAX = numeric_limits<f64>::maximum();
    static constexpr f32 F32_INFINITY = numeric_limits<f32>::inifinity();
    static constexpr f64 F64_INFINITY = numeric_limits<f64>::inifinity();
#endif

    static constexpr f32 F32_ANGLE_COSINE_ALMOST_ONE = 0.999f;
    static constexpr f32 F32_DOT_EPSILON = 1.0e-6f;
    static constexpr f32 F32_HITEPSILON = 1.0e-5f;

    static constexpr f32 PI = static_cast<f32>(3.14159265358979323846);
    static constexpr f32 PI2 = static_cast<f32>(6.28318530717958647692);
    static constexpr f32 INV_PI = static_cast<f32>(0.31830988618379067153);
    static constexpr f32 INV_PI2 = static_cast<f32>(0.15915494309189533576);
    static constexpr f32 PI_2 = static_cast<f32>(1.57079632679489661923);
    static constexpr f32 INV_PI_2 = static_cast<f32>(0.63661977236758134308);
    static constexpr f32 LOG2 = static_cast<f32>(0.693147180559945309417);
    static constexpr f32 INV_LOG2 = static_cast<f32>(1.0/0.693147180559945309417);

    static constexpr f64 PI_64 = static_cast<f64>(3.14159265358979323846);
    static constexpr f64 PI2_64 = static_cast<f64>(6.28318530717958647692);
    static constexpr f64 INV_PI_64 = static_cast<f64>(0.31830988618379067153);
    static constexpr f64 INV_PI2_64 = static_cast<f64>(0.15915494309189533576);
    static constexpr f64 PI_2_64 = static_cast<f64>(1.57079632679489661923);
    static constexpr f64 INV_PI_2_64 = static_cast<f64>(0.63661977236758134308);
    static constexpr f64 LOG2_64 = static_cast<f64>(0.693147180559945309417);
    static constexpr f64 INV_LOG2_64 = static_cast<f64>(1.0/0.693147180559945309417);

    static constexpr f32 DEG_TO_RAD = static_cast<f32>(1.57079632679489661923/90.0);
    static constexpr f32 RAD_TO_DEG = static_cast<f32>(90.0/1.57079632679489661923);

    enum Result
    {
        Result_Fail = 0,
        Result_Success = (0x01U<<0),
        Result_Front = (0x01U<<0)|(0x01U<<1),
        Result_Back = (0x01U<<0)|(0x01U<<2),
    };

    //--- Utilities
    //---------------------------------------------------------
    union UnionS32F32
    {
        s32 s32_;
        f32 f32_;
    };

    union UnionU32F32
    {
        u32 u32_;
        f32 f32_;
    };

    union UnionS64F64
    {
        s64 s64_;
        f64 f64_;
    };

    union UnionU64F64
    {
        u64 u64_;
        f64 f64_;
    };

    template<class T>
    inline T* align16(T* ptr)
    {
        return (T*)(((uintptr_t)(ptr)+LALIGN16_MASK) & ~LALIGN16_MASK);
    }

    template<class T>
    inline void swap(T& l, T& r)
    {
        T tmp(move(l));
        l = move(r);
        r = move(tmp);
    }

    template<class T>
    inline T lerp(const T& v0, const T& v1, f32 ratio)
    {
        return v0 + ratio*(v1-v0);
    }

    template<class T>
    inline const T& maximum(const T& left, const T& right)
    {
        return (left<right)? right : left;
    }

    template<class T>
    inline const T& minimum(const T& left, const T& right)
    {
        return (left<right)? left : right;
    }

    template<class T>
    inline T clamp(T val, T low, T high)
    {
        if(val <= low) return low;
        else if(val >= high) return high;
        else return val;
    }

    f32 clamp01(f32 v);

    u16 toBinary16Float(f32 f);
    f32 fromBinary16Float(u16 s);

    template<class T>
    inline T absolute(T val)
    {
        return abs(val);
    }


    template<>
    inline u8 absolute<u8>(u8 val)
    {
        return val;
    }

    template<>
    inline u16 absolute<u16>(u16 val)
    {
        return val;
    }

    template<>
    inline u32 absolute<u32>(u32 val)
    {
        return val;
    }

    template<>
    inline f32 absolute<f32>(f32 val)
    {
        UnionU32F32 u;
        u.f32_ = val;
        u.u32_ &= 0x7FFFFFFFU;
        return u.f32_;
    }

    template<>
    inline f64 absolute<f64>(f64 val)
    {
        return fabs(val);
    }

    inline bool isNan(f32 f)
    {
        return std::isnan(f);
    }

    inline bool isNan(f64 f)
    {
        return std::isnan(f);
    }

    inline int isEOL(const Char* str)
    {
        if(0x0D == *str){
            return (0x0A == *(str+1))? 2 : 1;
        }else if(0x0A == *str){
            return 1;
        }
        return 0;
    }

    inline bool isEqual(f32 x1, f32 x2)
    {
        return (lray::absolute<f32>(x1 - x2) <= F32_EPSILON);
    }

    inline bool isEqual(f32 x1, f32 x2, f32 epsilon)
    {
        return (lray::absolute<f32>(x1 - x2) <= epsilon);
    }

    inline bool isEqual(f64 x1, f64 x2)
    {
        return (lray::absolute<f64>(x1 - x2) <= F64_EPSILON);
    }

    inline bool isZero(f32 x1)
    {
        return (lray::absolute<f32>(x1) <= F32_EPSILON);
    }

    inline bool isZero(f32 x1, f32 epsilon)
    {
        return (lray::absolute<f32>(x1) <= epsilon);
    }

    inline bool isZero(f64 x1)
    {
        return (lray::absolute<f64>(x1) <= F64_EPSILON);
    }

    inline bool isZero(f64 x1, f64 epsilon)
    {
        return (lray::absolute<f64>(x1) <= epsilon);
    }

    inline bool isZeroPositive(f32 x1)
    {
        LASSERT(0.0f<=x1);
        return (x1 <= F32_EPSILON);
    }

    inline bool isZeroPositive(f32 x1, f32 epsilon)
    {
        LASSERT(0.0f<=x1);
        return (x1 <= epsilon);
    }

    inline bool isZeroPositive(f64 x1)
    {
        LASSERT(0.0f<=x1);
        return (x1 <= F64_EPSILON);
    }

    inline bool isZeroPositive(f64 x1, f64 epsilon)
    {
        LASSERT(0.0f<=x1);
        return (x1 <= epsilon);
    }

    inline bool isZeroNegative(f32 x1)
    {
        LASSERT(x1<=0.0f);
        return (-F32_EPSILON<=x1);
    }

    inline bool isZeroNegative(f32 x1, f32 epsilon)
    {
        LASSERT(x1<0.0f);
        return (epsilon<=x1);
    }

    inline bool isZeroNegative(f64 x1)
    {
        LASSERT(x1<0.0f);
        return (-F64_EPSILON<=x1);
    }

    inline bool isZeroNegative(f64 x1, f64 epsilon)
    {
        LASSERT(x1<0.0f);
        return (epsilon<=x1);
    }

    template<class T>
    inline bool isSameSign(T x0, T x1);

    template<>
    inline bool isSameSign<s8>(s8 x0, s8 x1)
    {
        return 0 == ((x0^x1)&(0x1U<<7));
    }
    template<>
    inline bool isSameSign<s16>(s16 x0, s16 x1)
    {
        return 0 == ((x0^x1)&(0x1U<<15));
    }
    template<>
    inline bool isSameSign<s32>(s32 x0, s32 x1)
    {
        return 0 == ((x0^x1)&(0x1U<<31));
    }
    template<>
    inline bool isSameSign<s64>(s64 x0, s64 x1)
    {
        return 0 == ((x0^x1)&(0x1ULL<<63));
    }

    template<> inline bool isSameSign<u8>(u8, u8){ return true;}
    template<> inline bool isSameSign<u16>(u16, u16){ return true;}
    template<> inline bool isSameSign<u32>(u32, u32){ return true;}
    template<> inline bool isSameSign<u64>(u64, u64){ return true;}

    template<>
    bool isSameSign<f32>(f32 x0, f32 x1);

    template<>
    bool isSameSign<f64>(f64 x0, f64 x1);

    //--- String functions
    //---------------------------------------------------------
    inline s32 strlen_s32(const Char* str)
    {
        return static_cast<s32>(::strlen(str));
    }

    inline s32 strnlen_s32(const Char* str, size_t size)
    {
        return static_cast<s32>(::strnlen(str, size));
    }

    /**
    @brief Extract directory path from path. Return directory path length, if dst is null.
    @return length of dst
    @param dst[out] ... require sufficient length what includes null character
    @param length ... length of path
    @param path ...
    */
    s32 extractDirectoryPath(Char* dst, s32 length, const Char* path);

    //--- Math functions
    //---------------------------------------------------------
    inline f32 sqrt(f32 x)
    {
        f32 ret;
        __m128 tmp = _mm_set_ss(x);
        _mm_store_ss(&ret, _mm_sqrt_ss(tmp));
        return ret;
    }

    inline f64 sqrt(f64 x)
    {
        return ::sqrt(x);
    }

    inline f32 sinf(f32 x)
    {
        return ::sinf(x);
    }

    inline f64 sin(f64 x)
    {
        return ::sin(x);
    }

    inline f32 cosf(f32 x)
    {
        return ::cosf(x);
    }

    inline f64 cos(f64 x)
    {
        return ::cos(x);
    }

    inline f32 tan(f32 x)
    {
        return ::tanf(x);
    }

    inline f64 tan(f64 x)
    {
        return ::tan(x);
    }

    inline f32 acos(f32 x)
    {
        return ::acosf(x);
    }

    inline f64 acos(f64 x)
    {
        return ::acos(x);
    }

    inline f32 asin(f32 x)
    {
        return ::asinf(x);
    }

    inline f64 asin(f64 x)
    {
        return ::asin(x);
    }

    inline f32 atan(f32 x)
    {
        return ::atanf(x);
    }

    inline f64 atan(f64 x)
    {
        return ::atan(x);
    }

    inline f32 atan2(f32 x, f32 y)
    {
        return ::atan2f(x, y);
    }

    inline f64 atan2(f64 x, f64 y)
    {
        return ::atan2(x, y);
    }

    inline f32 exp(f32 x)
    {
        return ::expf(x);
    }

    inline f64 exp(f64 x)
    {
        return ::exp(x);
    }

    inline f32 exp2(f32 x)
    {
        return ::exp2f(x);
    }

    inline f64 exp2(f64 x)
    {
        return ::exp2(x);
    }

    inline f32 log(f32 x)
    {
        return ::logf(x);
    }

    inline f64 log(f64 x)
    {
        return ::log(x);
    }

    inline f32 log2(f32 x)
    {
        return ::log2f(x);
    }

    inline f64 log2(f64 x)
    {
        return ::log2(x);
    }
    
    inline f32 pow(f32 x, f32 y)
    {
        return ::powf(x, y);
    }

    inline f64 pow(f64 x, f64 y)
    {
        return ::pow(x, y);
    }

    inline s32 round2S32(f64 x)
    {
        const f64 doublemagic = 6755399441055744.0;
        x += doublemagic;
        return ((s32*)&x)[0];
    }

    inline s32 float2S32(f32 x)
    {
        const f64 doublemagicroundeps = .5-1.4e-11;
        return (x<0)? round2S32(x+doublemagicroundeps)
            : round2S32(x - doublemagicroundeps);
    }

    inline f32 floor(f32 val)
    {
        return floorf(val);
    }

    inline f32 ceil(f32 val)
    {
        return ceilf(val);
    }


    inline s32 floorS32(f32 val)
    {
        return static_cast<s32>( floorf(val) );
    }

    inline s32 ceilS32(f32 val)
    {
        return static_cast<s32>( ceilf(val) );
    }

    inline s32 floorS32(f64 val)
    {
        const f64 doublemagicroundeps = .5-1.4e-11;
        return round2S32(val - doublemagicroundeps);
    }

    inline s32 ceilS32(f64 val)
    {
        const f64 doublemagicroundeps = .5-1.4e-11;
        return round2S32(val + doublemagicroundeps);
    }

    inline f32 fmod(f32 x, f32 y)
    {
        return ::fmodf(x, y);
    }

    inline f32 step(f32 x, f32 a)
    {
        return (a<=x)? 1.0f : 0.0f;
    }

    inline f32 boxstep(f32 x, f32 e0, f32 e1)
    {
        LASSERT(!isEqual(e0, e1));
        return clamp01((x-e0)/(e1-e0));
    }

    //3t^2-2t^3
    inline f32 smooth(f32 x)
    {
        LASSERT(0<=x && x<=1.0f);
    	return x * x * (3.0f - 2.0f*x);
    }
    
    inline f32 smoothstep(f32 x, f32 e0, f32 e1)
    {
        x = clamp01((x - e0) / (e1 - e0));
    	return x * x * (3.0f - 2.0f*x);
    }

    inline f32 expstep(f32 x, f32 a, f32 n)
    {
        return exp(-a*pow(x, n));
    }

    inline f32 gamma(f32 x, f32 g)
    {
        LASSERT(!isZero(g));
        return pow(x, 1.0f/g);
    }

    inline f32 bias(f32 a, f32 b)
    {
        static const f32 InvLogHalf = static_cast<f32>(1.0/-0.30102999566);
        return pow(a, log(b)*InvLogHalf);
    }

    //--- Time
    //-----------------------------------------------------
    void sleep(u32 milliSeconds);

    /// Get performance counter
    ClockType getPerformanceCounter();

    /// Get performance count per second
    ClockType getPerformanceFrequency();

    /// Calculate duration time from performance count
    f64 calcTime64(ClockType prevTime, ClockType currentTime);

    /// Calculate duration time from performance count
    inline f32 calcTime(ClockType prevTime, ClockType currentTime)
    {
        return static_cast<f32>(calcTime64(prevTime, currentTime));
    }

    /// Get time in milli second
    u32 getTimeMilliSec();

    //--- SSE
    //-----------------------------------------------------
#ifdef LRAY_USE_SSE
    inline lm128 set_m128(f32 x, f32 y, f32 z, f32 w)
    {
        lm128 t0 = _mm_load_ss(&x);
        lm128 t1 = _mm_load_ss(&z);

        lm128 ret = _mm_unpacklo_ps(t0, t1);
        t0 = _mm_load_ss(&y);
        t1 = _mm_load_ss(&w);
        return _mm_unpacklo_ps(ret, _mm_unpacklo_ps(t0, t1));
    }

    inline lm128 load3(const f32* v)
    {
        lm128 t = _mm_load_ss(&v[2]);
        t = _mm_movelh_ps(t, t);
        t = _mm_loadl_pi(t, reinterpret_cast<const __m64*>(v));
        return t;
    }

    inline void store3(f32* v, const lm128& r)
    {
        _mm_storel_pi(reinterpret_cast<__m64*>(v), r);

        static const u32 Shuffle = 170;
        lm128 t = _mm_shuffle_ps(r, r, Shuffle);
        _mm_store_ss(&v[2], t);
    }

    inline lm128 operator+(const lm128& v0, const lm128& v1)
    {
        return _mm_add_ps(v0, v1);
    }

    inline lm128 operator-(const lm128& v0, const lm128& v1)
    {
        return _mm_sub_ps(v0, v1);
    }

    inline lm128 operator*(const lm128& v0, const lm128& v1)
    {
        return _mm_mul_ps(v0, v1);
    }

    inline lm128 operator*(f32 x, const lm128& v)
    {
        return _mm_mul_ps(_mm_set1_ps(x), v);
    }

    inline lm128 operator*(const lm128& v, f32 x)
    {
        return _mm_mul_ps(v, _mm_set1_ps(x));
    }

    inline lm128 operator/(const lm128& v0, const lm128& v1)
    {
        return _mm_div_ps(v0, v1);
    }

    inline lm128 operator/(const lm128& v, f32 x)
    {
        return _mm_div_ps(v, _mm_set1_ps(x));
    }

    inline lm128 sqrt(const lm128& v)
    {
        return _mm_sqrt_ps(v);
    }

    inline lm128 minimum(const lm128& v0, const lm128& v1)
    {
        return _mm_min_ps(v0, v1);
    }

    inline lm128 maximum(const lm128& v0, const lm128& v1)
    {
        return _mm_max_ps(v0, v1);
    }

    inline lm128 muladd(const lm128& v0, const lm128& v1, const lm128& v2)
    {
        return _mm_add_ps(_mm_mul_ps(v0, v1), v2);
    }

    lm128 normalize(const lm128& v);

    lm128 floor(const lm128& v);

    lm128 ceil(const lm128& v);

    //---------------------------------------------------------------------------------------------------
    struct MMX
    {
        static inline lm128 construct(f32 x, f32 y, f32 z)
        {
            return _mm_set_ps(0.0f, z, y, x);
        }
        static inline lm128 construct(f32 x, f32 y, f32 z, f32 w)
        {
            return _mm_set_ps(w, z, y, x);
        }

        static lm128 normalize(f32 x, f32 y, f32 z);
        static lm128 normalizeLengthSqr(f32 x, f32 y, f32 z, f32 lengthSqr);
        static lm128 normalizeChecked(f32 x, f32 y, f32 z);

        static lm128 normalize(f32 x, f32 y, f32 z, f32 w);
        static lm128 normalizeLengthSqr(f32 x, f32 y, f32 z, f32 w, f32 lengthSqr);
        static lm128 normalizeChecked(f32 x, f32 y, f32 z, f32 w);
    };
#endif

    //--- HitRecord
    //--------------------------------------------------------------
    struct HitRecord
    {
        Result result_;
        f32 t_;
        f32 v_;
        f32 w_;
        const void* primitive_;
    };
}
#endif //INC_LRAY_H_
