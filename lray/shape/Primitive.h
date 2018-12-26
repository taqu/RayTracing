#ifndef INC_LRAY_PRIMITIVE_H__
#define INC_LRAY_PRIMITIVE_H__
/**
@file Primitive.h
@author t-sakai
@date 2018/05/24 create
*/
#include "../lray.h"
#include "../math/Vector3.h"
#include "../shape/Triangle.h"
#include "../shape/TriangleProxy.h"

namespace lray
{
    /**
    */
    class Primitive
    {
    public:
        enum Component
        {
            Component_Normal = (0x01<<0),
            Component_Texcoord = (0x01<<1),
            Component_Color = (0x01<<2),
        };

        Primitive();
        Primitive(
            s32 numVertices,
            Vector3* positions,
            Vector3* normals,
            s32 numTriangles,
            Triangle* triangles);
        Primitive(Primitive&& rhs);
        ~Primitive();

        inline void addComponent(Component component);
        inline bool hasComponent(Component component) const;

        inline s32 getNumVertices() const;
        inline const Vector3& getPosition(s32 index) const;
        inline const Vector3& getNormal(s32 index) const;
        inline s32 getNumTriangles() const;
        inline const Triangle& getTriangle(s32 index) const;

        /**
        @brief Generate and refine elements from source
        */
        void refine(const Primitive& src, const lray::Matrix44& matrix);

        /**
        @brief Generate proxies to a destination buffer
        */
        void getTriangleProxies(TriangleProxy* dst) const;

        Primitive& operator=(Primitive&& rhs);
    protected:
        Primitive(const Primitive&) = delete;
        Primitive& operator=(const Primitive&) = delete;

        s32 components_;
        s32 numVertices_;
        Vector3* positions_;
        Vector3* normals_;
        s32 numTriangles_;
        Triangle* triangles_;
    };

    inline void Primitive::addComponent(Component component)
    {
        components_ |= component;
    }

    inline bool Primitive::hasComponent(Component component) const
    {
        return 0 != (components_ & component);
    }

    inline s32 Primitive::getNumVertices() const
    {
        return numVertices_;
    }

    inline const Vector3& Primitive::getPosition(s32 index) const
    {
        LASSERT(0<=index && index<numVertices_);
        return positions_[index];
    }

    inline const Vector3& Primitive::getNormal(s32 index) const
    {
        LASSERT(0<=index && index<numVertices_);
        return normals_[index];
    }

    inline s32 Primitive::getNumTriangles() const
    {
        return numTriangles_;
    }

    inline const Triangle& Primitive::getTriangle(s32 index) const
    {
        LASSERT(0<=index && index<numTriangles_);
        return triangles_[index];
    }
}
#endif //INC_LRAY_PRIMITIVE_H__
