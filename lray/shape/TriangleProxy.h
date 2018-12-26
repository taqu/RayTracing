#ifndef INC_LRAY_TRIANGLEPROXY_H__
#define INC_LRAY_TRIANGLEPROXY_H__
/**
@file TriangleProxy.h
@author t-sakai
@date 2018/12/14 create
*/
#include "../lray.h"
#include "../math/Vector3.h"
#include "../math/AABB.h"
#include "../core/Sort.h"

namespace lray
{
    class Primitive;

    struct TriangleProxy
    {
        const Primitive* primitive_;
        s32 index_;

        Vector3 getCentroid() const;

        AABB getBBox() const;

        Result testRay(f32& t, f32& v, f32& w, const Ray& ray) const;
    };

    static_assert(std::is_trivially_copyable<TriangleProxy>::value == true, "TriangleProxy must be trivially copyable.");

    struct SortFuncCentroid
    {
        SortFuncCentroid(const f32* centroids)
            :centroids_(centroids)
        {}

        bool operator()(s32 i0, s32 i1) const
        {
            return centroids_[i0] < centroids_[i1];
        }

        const f32* centroids_;
    };

    template<class T>
    class PrimitivePolicy
    {
    public:

        //static inline Vector3 getCentroid(const T& primitive)
        //{
        //    return primitive.getCentroid();
        //}

        //static inline f32 getCentroid(const T& primitive, s32 axis)
        //{
        //    return primitive.getCentroid(axis);
        //}

        //static inline AABB getBBox(const T& primitive)
        //{
        //    return primitive.getBBox();
        //}

        //static inline bool testRay(f32& t, const T& primitive, const Ray& ray)
        //{
        //    return primitive.testRay(t, ray);
        //}

        static inline void sort(s32 numPrimitives, s32* primitiveIndices, const f32* centroids)
        {
            introsort(numPrimitives, primitiveIndices, SortFuncCentroid(centroids));
        }

        static inline void insertionsort(s32 numPrimitives, s32* primitiveIndices, const f32* centroids)
        {
            insertionsort(numPrimitives, primitiveIndices, SortFuncCentroid(centroids));
        }
    };
}
#endif //INC_LRAY_TRIANGLEPROXY_H__
