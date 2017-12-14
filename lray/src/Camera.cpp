/**
@file Camera.cpp
@author t-sakai
@date 2017/12/14 create
*/
#include "Camera.h"
#include "Ray.h"

namespace lray
{
    Camera::Camera()
        :width_(0)
        ,height_(0)
        ,invHalfWidth_(1.0f)
        ,invHalfHeight_(1.0f)
        ,position_(0.0f)
        ,right_(1.0f, 0.0f, 0.0f)
        ,up_(0.0f, 1.0f, 0.0f)
        ,forward_(0.0f, 0.0f, 1.0f)
        ,fovx_(DefaultAngleInDegree*DEG_TO_RAD*PI)
        ,aspect_(1.0f)
        ,nearClip_(DefaultNear)
        ,farClip_(DefaultFar)
        ,rayDx_(1.0f)
        ,rayDy_(1.0f)
    {}

    Camera::~Camera()
    {}

    Ray&& Camera::generateRay(f32 screenX, f32 screenY, f32 tmin, f32 tmax) const
    {
        Vector3 dx = mul((invHalfWidth_*screenX - 1.0f)*rayDx_, right_);
        Vector3 dy = mul((1.0f-invHalfHeight_*screenY)*rayDy_, up_);

        Vector3 direction = normalize(dx+dy+forward_);
        return move(Ray(position_, direction, tmax, tmin, tmax));
    }

    void Camera::perspective(f32 aspect, f32 radFovx)
    {
        fovx_ = radFovx;
        aspect_ = aspect;

        f32 fov = lray::tan(0.5f * radFovx);
        rayDx_ = fov;
        rayDy_ = fov/aspect_;
    }

    void Camera::lookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
    {
        position_ = eye;
        forward_ = normalize(at - eye);

        f32 cs = dot(forward_, up);
        up_ = (F32_ANGLE_COSINE_ALMOST_ONE<absolute(cs))? Vector3(forward_.z_, forward_.x_, forward_.y_) : up;

        right_ = normalize(cross(forward_, up_));
        up_ = normalize(cross(right_, forward_));
    }
}
