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
    {
        invertDirection();
    }

    void Ray::invertDirection()
    {
        for(s32 i=0; i<3; ++i){
            if(0.0f<=direction_[i]){
                invDirection_[i] = (isZeroPositive(direction_[i]))? F32_MAX : 1.0f/direction_[i];
            }else{
                invDirection_[i] = (isZeroNegative(direction_[i]))? -F32_MAX : 1.0f/direction_[i];
            }
        }
    }
}
