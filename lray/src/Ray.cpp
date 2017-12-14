/**
@file Ray.cpp
@author t-sakai
@date 2017/12/13 create
*/
#include "Ray.h"

namespace lray
{
    Ray::Ray(const Vector3& origin, const Vector3& direction, f32 t, f32 tmin, f32 tmax)
        :origin_(origin)
        ,direction_(normalize(direction))
        ,t_(t)
        ,tmin_(tmin)
        ,tmax_(tmax)
    {
    }
}
