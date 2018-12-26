#ifndef INC_LRAY_INTERSECTION_H__
#define INC_LRAY_INTERSECTION_H__
/**
@file Intersection.h
@author t-sakai
@date 2018/05/24 create
*/
#include "../math/Vector3.h"

namespace lray
{
    class Intersection
    {
    public:
        Intersection()
            :result_(Result_Fail)
            ,t_(F32_INFINITY)
            ,b0_(0.0f)
            ,b1_(0.0f)
            ,b2_(0.0f)
        {}

        Result result_;
        f32 t_;
        f32 b0_;
        f32 b1_;
        f32 b2_;

        Vector3 point_;
        Vector3 shadingNormal_;
        Vector3 geometricNormal_;
    };

    static_assert(std::is_trivially_copyable<Intersection>::value == true, "Intersection must be trivially copyable.");
}
#endif //INC_LRAY_INTERSECTION_H__
