/**
@file Ray.cpp
@author t-sakai
@date 2017/12/13 create
*/
#include "math/Ray.h"

namespace lray
{
    Ray::Ray(const Vector3& origin, const Vector3& direction, f32 t)
        :origin_(origin)
        ,direction_(normalize(direction))
        ,t_(t)
    {}
}
