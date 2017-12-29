#ifndef INC_LRAY_CAMERA_H__
#define INC_LRAY_CAMERA_H__
/**
@file Camera.h
@author t-sakai
@date 2017/12/14 create
*/
#include "core/ReferenceCounted.h"
#include "math/Vector3.h"

namespace lray
{
    class Ray;

    /**
    */
    class Camera : public ReferenceCounted<Camera>
    {
    public:
        static constexpr f32 DefaultAngleInDegree = 60.0f;
        static constexpr f32 DefaultNear = 1.0e-2f;
        static constexpr f32 DefaultFar = 1.0e2f;

        Camera();
        ~Camera();

        /**
        @brief Generate a ray from a screen point
        @param screenX ...
        @param screenY ...
        */
        Ray generateRay(f32 screenX, f32 screenY) const;

        /**
        @param width ... must be 0<width
        @param height ... must be 0<height
        */
        inline void setResolution(s32 width, s32 height);

        inline const Vector3& getPosition() const;

        /**
        @warning 
        */
        void setNearFarClip(f32 near, f32 far);
        void perspective(f32 aspect, f32 radFovx);

        /**
        @warning view direction (at-eye) must not equals to up
        */
        void lookAt(const Vector3& eye, const Vector3& at, const Vector3& up);
    protected:
        s32 width_;
        s32 height_;
        f32 invHalfWidth_;
        f32 invHalfHeight_;
        Vector3 position_;

        Vector3 right_;
        Vector3 up_;
        Vector3 forward_;
        f32 fovx_;
        f32 aspect_;
        f32 nearClip_;
        f32 farClip_;
        f32 rayDx_;
        f32 rayDy_;
    };

    inline void Camera::setResolution(s32 width, s32 height)
    {
        LASSERT(0<width);
        LASSERT(0<height);

        width_ = width;
        height_ = height;

        invHalfWidth_ = 2.0f/width_;
        invHalfHeight_ = 2.0f/height_;
    }

    inline const Vector3& Camera::getPosition() const
    {
        return position_;
    }
}
#endif //INC_LRAY_CAMERA_H__
