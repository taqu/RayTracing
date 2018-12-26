#ifndef INC_LRAY_MESH_H__
#define INC_LRAY_MESH_H__
/**
@file Mesh.h
@author t-sakai
@date 2018/05/24 create
*/
#include "../lray.h"
#include "../core/Array.h"
#include "../core/Intersection.h"
#include "../math/Ray.h"
#include "Primitive.h"

namespace lray
{
    /**
    */
    class Mesh
    {
    public:
        typedef Array<Primitive> PrimitiveArray;

        Mesh();
        Mesh(Mesh&& rhs);
        explicit Mesh(PrimitiveArray&& primitives);
        ~Mesh();

        void clear();

        Result test(Intersection& intersection, Ray& ray);
        /**
        @brief Generate and refine mesh from source
        */
        void refine(const Mesh& src, const lray::Matrix44& matrix);

        inline s32 getNumPrimitives() const;
        inline const Primitive& getPrimitive(s32 index) const;

        Mesh& operator=(Mesh&& rhs);
    protected:
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        PrimitiveArray primitives_;
    };

    inline s32 Mesh::getNumPrimitives() const
    {
        return primitives_.size();
    }

    inline const Primitive& Mesh::getPrimitive(s32 index) const
    {
        return primitives_[index];
    }
}
#endif //INC_LRAY_MESH_H__
