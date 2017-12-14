#ifndef INC_LRAY_RAY_H_
#define INC_LRAY_RAY_H_
/**
@file Ray.h
@author t-sakai
@date 2017/12/13 create
*/
#include "lray.h"
#include "Vector3.h"

namespace lray
{
    class Ray
    {
    public:
        Ray(){}
        Ray(const Vector3& origin, const Vector3& direction, f32 t, f32 tmin, f32 tmax);

        Vector3 origin_;
        Vector3 direction_;
        f32 t_;
        f32 tmin_;
        f32 tmax_;
    };

    static_assert(std::is_trivially_copyable<Ray>::value == true, "Ray must be trivially copyable.");
}

#endif //INC_LRAY_RAY_H_
