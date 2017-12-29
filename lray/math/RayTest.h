#ifndef INC_LRAY_RAYTEST_H__
#define INC_LRAY_RAYTEST_H__
/**
@file RayTest.h
@author t-sakai
@date 2017/12/18 create
*/
#include "../lray.h"

namespace lray
{
    class Ray;
    class Plane;
    class Sphere;
    class Vector3;
    class Vector4;

    //-----------------------------------------------------------
    /**
    @brief Calcurate inner product of 4 SOA(structure of arrays) layout vectors
    */
    lm128 dot4vec(
        const lm128& vx0,
        const lm128& vy0,
        const lm128& vz0,
        const lm128& vx1,
        const lm128& vy1,
        const lm128& vz1);

    /**
    @brief Calcurate outer product of 4 SOA(structure of arrays) layout vectors
    */
    void cross4vec(
        lm128& vx,
        lm128& vy,
        lm128& vz,
        const lm128& vx0,
        const lm128& vy0,
        const lm128& vz0,
        const lm128& vx1,
        const lm128& vy1,
        const lm128& vz1);


    //-----------------------------------------------------------
    /**
    @brief Determine intersection of ray and plane
    @return 
    @param t[out] ... intersection point = t*ray.direction_ + ray.origin_
    @param ray ...
    @param plane ...
    */
    bool testRayPlane(f32& t, const Ray& ray, const Plane& plane);

    //-----------------------------------------------------------
    /**
    @brief Determine intersection of ray and sphere
    @return 
    @param t[out] ... Intersection point = t*ray.direction_ + ray.origin_. If ray.origin_ is in sphere, t is zero.
    @param ray ...
    @param plane ...
    */
    bool testRaySphereSolid(f32& t, const Ray& ray, const Sphere& sphere);

    //-----------------------------------------------------------
    /**
    @brief Determine intersection of ray and sphere
    @return
    @param t[out] ... Intersection point = t*ray.direction_ + ray.origin_. If ray.origin_ is in sphere, a front intersection will be detected.
    @param ray ...
    @param sphere ...
    */
    bool testRaySphereSurface(f32& t, const Ray& ray, const Sphere& sphere);

    //-----------------------------------------------------------
    /**
    @brief Determine intersection of ray and triangle
    @return
    @param t[out] ... Intersection point = t*ray.direction_ + ray.origin_
    @param ray ...
    @param v0 ...
    @param v1 ...
    @param v2 ...
    */
    bool testRayTriangleFront(f32& t, f32& v, f32& w, const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2);
    bool testRayTriangleBack(f32& t, f32& v, f32& w, const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2);
    Result testRayTriangleBoth(f32& t, f32& v, f32& w, const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2);

    /**
    @brief Determine intersection of ray and triangle
    @return
    @param u ...
    @param v ...
    @param w ...
    @param startPoint 
    @param endPoint
    @param v0 ...
    @param v1 ...
    @param v2 ...
    */
    bool testRayTriangleFront(f32& u, f32& v, f32& w, const Vector3& startPoint, const Vector3& endPoint, const Vector3& v0, const Vector3& v1, const Vector3& v2);
    bool testRayTriangleBack(f32& u, f32& v, f32& w, const Vector3& startPoint, const Vector3& endPoint, const Vector3& v0, const Vector3& v1, const Vector3& v2);
    Result testRayTriangleBoth(f32& u, f32& v, f32& w, const Vector3& startPoint, const Vector3& endPoint, const Vector3& v0, const Vector3& v1, const Vector3& v2);

    /**
    @return
    */
    s32 testRayTriangleFront(
        lm128& t,
        lm128& v,
        lm128& w,
        const lm128 origin[3],
        const lm128 direction[3],
        const lm128 vx[3],
        const lm128 vy[3],
        const lm128 vz[3]);

    /**
    @return
    */
    s32 testRayTriangleBack(
        lm128& t,
        lm128& v,
        lm128& w,
        const lm128 origin[3],
        const lm128 direction[3],
        const lm128 vx[3],
        const lm128 vy[3],
        const lm128 vz[3]);

    /**
    @return
    */
    s32 testRayTriangleBoth(
        lm128& t,
        lm128& v,
        lm128& w,
        const lm128 origin[3],
        const lm128 direction[3],
        const lm128 vx[3],
        const lm128 vy[3],
        const lm128 vz[3]);

    //-----------------------------------------------------------
    bool testRayRectangle(f32& t, const Ray& ray, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);


    //-----------------------------------------------------------
    /**
    @brief Determine intersection of ray and AABB
    @return
    @param tmin ...
    @param tmax ...
    @param ray ...
    @param bmin ...
    @param bmax ...
    */
    bool testRayAABB(f32& tmin, f32& tmax, const Ray& ray, const Vector3& bmin, const Vector3& bmax);

    bool testRayAABB(const Ray& ray, const Vector3& bmin, const Vector3& bmax);

    bool testRayAABB(f32& tmin, f32& tmax, const Ray& ray, const Vector4& bmin, const Vector4& bmax);

    bool testRayAABB(const Ray& ray, const Vector4& bmin, const Vector4& bmax);

    s32 testRayAABB(
        lm128 tmin,
        lm128 tmax,
        lm128 origin[3],
        lm128 invDir[3],
        const s32 sign[3],
        const lm128 bbox[2][3]);

    //-----------------------------------------------------------
    /**
    @brief Determine intersection of ray and slab
    */
    bool testRaySlab(f32& tmin, f32& tmax, const Ray& ray, f32 slabMin, f32 slabMax, s32 axis);
}

#endif //INC_LRAY_RAYTEST_H__
