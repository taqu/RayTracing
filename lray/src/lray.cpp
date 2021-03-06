/**
@file lray.cpp
@author t-sakai
@date 2017/12/13 create
*/
#include "lray.h"
#ifdef _WIN32
#include <Windows.h>
#endif

//Overload new/delete functions
//---------------------------------------------------------
void* operator new(std::size_t size)
{
    return lray::lmalloc(size);
}

void operator delete(void* ptr)
{
    lray::lfree(ptr);
}

void* operator new[](std::size_t size)
{
    return lray::lmalloc(size);
}

void operator delete[](void* ptr)
{
    lray::lfree(ptr);
}

void* operator new(std::size_t size, const char* file, int line)
{
    return lray::lmalloc(size, file, line);
}

void operator delete(void* ptr, const char* /*file*/, int /*line*/)
{
    lray::lfree(ptr);
}

void* operator new[](std::size_t size, const char* file, int line)
{
    return lray::lmalloc(size, file, line);
}

void operator delete[](void* ptr, const char* /*file*/, int /*line*/)
{
    lray::lfree(ptr);
}

namespace lray
{
    // Alloc/Deallocate
    //---------------------------------------------------------
    void* lmalloc(size_t size)
    {
        return malloc(size);
    }

    void* lmalloc(size_t size, u32 alignment)
    {
#ifdef _MSC_VER
        return _aligned_malloc(size, alignment);
#else
        void* memptr = NULL;
        return 0==posix_memalign(&memptr, alignment, size)? memptr : NULL;
#endif
    }

    void lfree(void* ptr)
    {
        free(ptr);
    }

    void lfree(void* ptr, u32 /*alignment*/)
    {
#ifdef _MSC_VER
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }

    void* lmalloc(size_t size, const Char* /*file*/, s32 /*line*/)
    {
        return lmalloc(size);
    }

    void* lmalloc(size_t size, u32 alignment, const Char* /*file*/, s32 /*line*/)
    {
        return lmalloc(size, alignment);
    }

    //--- Utilities
    //---------------------------------------------------------
    template<>
    bool isSameSign<f32>(f32 x0, f32 x1)
    {
        static const u32 mask = 0x1U<<31;
        UnionU32F32 u0,u1;
        u0.f32_ = x0;
        u1.f32_ = x1;
        u32 t = u0.u32_ ^ u1.u32_;
        return 0 == (t&mask);
    }

    template<>
    bool isSameSign<f64>(f64 x0, f64 x1)
    {
        static const u64 mask = 0x1ULL<<63;
        UnionU64F64 u0, u1;
        u0.f64_ = x0;
        u1.f64_ = x1;
        u64 t = u0.u64_ ^ u1.u64_;
        return 0 == (t&mask);
    }

    //--- String functions
    //---------------------------------------------------------
    s32 extractDirectoryPath(Char* dst, s32 length, const Char* path)
    {
        LASSERT(NULL != path);
        LASSERT(0<=length);
        if(length<=0){
            if(NULL != dst){
                dst[0] = CharNull;
            }
            return 0;
        }

        s32 i = length-1;
        for(; 0<=i; --i){
            if(PathDelimiter == path[i]){
                break;
            }
        }
        s32 dstLen = i+1;
        if(NULL != dst){
            for(s32 j=0; j<dstLen; ++j){
                dst[j] = path[j];
            }
            dst[dstLen] = CharNull;
        }
        return dstLen;
    }

    //---------------------------------------------------------
    f32 clamp01(f32 v)
    {
        UnionS32F32 u;
        u.f32_ = v;

        s32 s = u.s32_ >> 31;
        s = ~s;
        u.s32_ &= s;

        u.f32_ -= 1.0f;
        s = u.s32_ >> 31;
        u.s32_ &= s;
        u.f32_ += 1.0f;
        return u.f32_;
    }

    u16 toBinary16Float(f32 f)
    {
        UnionU32F32 t;
        t.f32_ = f;

        u16 sign = (t.u32_>>16) & 0x8000U;
        s32 exponent = (t.u32_>>23) & 0x00FFU;
        u32 fraction = t.u32_ & 0x007FFFFFU;

        if(exponent == 0){
            return sign; //Return signed zero

        }else if(exponent == 0xFFU){
            if(fraction == 0){
                return sign | 0x7C00U; //Return signed Inf
            }else{
                return static_cast<u16>((fraction>>13) | 0x7C00U); //Return NaN
            }
        }else {
            exponent += (-127 + 15);
            if(exponent>=0x1F){ //Overflow
                return sign | 0x7C00U;
            }else if(exponent<=0){ //Underflow
                s32 shift = 14 - exponent;
                if(24<shift){ //Too small, return zero
                    return sign;
                }else{
                    fraction |= 0x800000U; //Add hidden bit
                    u16 frac = static_cast<u16>(fraction >> shift);
                    if((fraction>>(shift-1)) & 0x01U){ //Round fraction
                        frac += 1;
                    }
                    return sign | frac;
                }
            }
        }

        u16 ret = static_cast<u16>(sign | ((exponent<<10) & 0x7C00U) | (fraction>>13));
        if((fraction>>12) & 0x01U){ //Round fraction
            ret += 1;
        }
        return ret;
    }

    f32 fromBinary16Float(u16 s)
    {
        u32 sign = (s & 0x8000U) << 16;
        u32 exponent = ((s & 0x7C00U) >> 10);
        u32 fraction = (s & 0x03FFU);

        if(exponent == 0){
            if(fraction != 0){
                fraction <<= 1;
                while(0==(fraction & 0x0400U)){
                    ++exponent;
                    fraction <<= 1;
                }
                exponent = (127 - 15) - exponent;
                fraction &= 0x03FFU;
            }

        }else if(exponent == 0x1FU){
            exponent = 0xFFU;

        }else{
            exponent += (127 - 15);
        }

        UnionU32F32 t;
        t.u32_ = sign | (exponent<<23) | (fraction<<13);

        return t.f32_;
    }
    //---------------------------------------------------------
    //---
    //--- Time
    //---
    //---------------------------------------------------------
    void sleep(u32 milliSeconds)
    {
#if defined(_WIN32)
        ::Sleep(milliSeconds);
#else
        timespec ts;
        ts.tv_sec = 0;
        while(1000<milliSeconds){
            ts.tv_sec += 1;
            milliSeconds -= 1000;
        }
        ts.tv_nsec = 1000000L * milliSeconds;
        nanosleep(&ts, NULL);
#endif
    }

    ClockType getPerformanceCounter()
    {
#if defined(_WIN32)
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        return count.QuadPart;
#else
        clock_t t = 0;
        t = clock();
        return t;
#endif
    }

    ClockType getPerformanceFrequency()
    {
#if defined(_WIN32)
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        return freq.QuadPart;
#else
        return CLOCKS_PER_SEC;
#endif
    }

    f64 calcTime64(ClockType prevTime, ClockType currentTime)
    {
        ClockType d = (currentTime>=prevTime)? currentTime - prevTime : numeric_limits<ClockType>::maximum() - prevTime + currentTime;
        f64 delta = static_cast<f64>(d)/getPerformanceFrequency();
        return delta;
    }

    u32 getTimeMilliSec()
    {
#if defined(_WIN32)
        DWORD time = timeGetTime();
        return static_cast<u32>(time);
#else
        struct timeval tv;
        gettimeofday(&tv, NULL);

        return static_cast<u32>(tv.tv_sec*1000 + tv.tv_usec/1000);
#endif
    }

#ifdef LRAY_USE_SSE
    lm128 normalize(const lm128& v)
    {
        lm128 tv = v;
        lm128 tmp = _mm_mul_ps(v, v);
        tmp = _mm_add_ps(_mm_shuffle_ps(tmp, tmp, 0x4E), tmp);
        tmp = _mm_add_ps(_mm_shuffle_ps(tmp, tmp, 0xB1), tmp);

        tmp = _mm_sqrt_ss(tmp);
        tmp = _mm_shuffle_ps(tmp, tmp, 0);

        tv = _mm_div_ps(tv, tmp);
        return tv;
    }

    lm128 floor(const lm128& v)
    {
        lm128 tv = _mm_cvtepi32_ps(_mm_cvttps_epi32(v));

        return _mm_sub_ps(tv, _mm_and_ps(_mm_cmplt_ps(v, tv), _mm_set1_ps(1.0f)));
    }

    lm128 ceil(const lm128& v)
    {
        lm128 tv = _mm_cvtepi32_ps(_mm_cvttps_epi32(v));
        return _mm_add_ps(tv, _mm_and_ps(_mm_cmplt_ps(v, tv), _mm_set1_ps(1.0f)));
    }

    //---------------------------------------------------------------------------------------------------
    lm128 MMX::normalize(f32 x, f32 y, f32 z)
    {
        lm128 r0 = _mm_set_ps(0.0f, z, y, x);
        lm128 r1 = r0;
        r0 = _mm_mul_ps(r0, r0);
        r0 = _mm_add_ps(_mm_shuffle_ps(r0, r0, 0x4E), r0);
        r0 = _mm_add_ps(_mm_shuffle_ps(r0, r0, 0xB1), r0);

        r0 = _mm_sqrt_ss(r0);
        r0 = _mm_shuffle_ps(r0, r0, 0);

        return _mm_div_ps(r1, r0);
    }

    lm128 MMX::normalizeLengthSqr(f32 x, f32 y, f32 z, f32 lengthSqr)
    {
        lm128 r0 = _mm_set1_ps(lengthSqr);
        lm128 r1 = _mm_set_ps(0.0f, z, y, x);
        r0 = _mm_sqrt_ps(r0);
        return _mm_div_ps(r1, r0);
    }

    lm128 MMX::normalizeChecked(f32 x, f32 y, f32 z)
    {
        f32 l = x*x + y*y + z*z;
        if(isZeroPositive(l)){
            return _mm_setzero_ps();
        } else{
            return normalizeLengthSqr(x, y, z, l);
        }
    }

    lm128 MMX::normalize(f32 x, f32 y, f32 z, f32 w)
    {
        lm128 r0 = _mm_set_ps(w, z, y, x);
        lm128 r1 = r0;
        r0 = _mm_mul_ps(r0, r0);
        r0 = _mm_add_ps(_mm_shuffle_ps(r0, r0, 0x4E), r0);
        r0 = _mm_add_ps(_mm_shuffle_ps(r0, r0, 0xB1), r0);

        r0 = _mm_sqrt_ss(r0);
        r0 = _mm_shuffle_ps(r0, r0, 0);

        return _mm_div_ps(r1, r0);
    }

    lm128 MMX::normalizeLengthSqr(f32 x, f32 y, f32 z, f32 w, f32 lengthSqr)
    {
        lm128 r0 = _mm_set1_ps(lengthSqr);
        lm128 r1 = _mm_set_ps(w, z, y, x);
        r0 = _mm_sqrt_ps(r0);
        return _mm_div_ps(r1, r0);
    }

    lm128 MMX::normalizeChecked(f32 x, f32 y, f32 z, f32 w)
    {
        f32 l = x*x + y*y + z*z + w*w;
        if(isZeroPositive(l)){
            return _mm_setzero_ps();
        } else{
            return normalizeLengthSqr(x, y, z, w, l);
        }
    }
#endif
}
