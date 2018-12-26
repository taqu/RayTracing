#ifndef INC_LRAY_TRIANGLE_H__
#define INC_LRAY_TRIANGLE_H__
/**
@file Triangle.h
@author t-sakai
@date 2018/05/24 create
*/
#include "../lray.h"

namespace lray
{
    struct Triangle
    {
        s32 indices_[3];
    };

    static_assert(std::is_trivially_copyable<Triangle>::value == true, "Triangle must be trivially copyable.");
}
#endif //INC_LRAY_TRIANGLE_H__
