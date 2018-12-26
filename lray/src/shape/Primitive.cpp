/**
@file Primitive.cpp
@author t-sakai
@date 2018/05/24 create
*/
#include "shape/Primitive.h"

namespace lray
{
    Primitive::Primitive()
        :components_(0)
        ,numVertices_(0)
        ,positions_(NULL)
        ,normals_(NULL)
        ,numTriangles_(0)
        ,triangles_(NULL)
    {
    }

    Primitive::Primitive(
        s32 numVertices,
        Vector3* positions,
        Vector3* normals,
        s32 numTriangles,
        Triangle* triangles)
        :components_(0)
        ,numVertices_(numVertices)
        ,positions_(positions)
        ,normals_(normals)
        ,numTriangles_(numTriangles)
        ,triangles_(triangles)
    {
        if(NULL != normals_){
            addComponent(Component_Normal);
        }
    }

    Primitive::Primitive(Primitive&& rhs)
        :components_(rhs.components_)
        ,numVertices_(rhs.numVertices_)
        ,positions_(rhs.positions_)
        ,normals_(rhs.normals_)
        ,numTriangles_(rhs.numTriangles_)
        ,triangles_(rhs.triangles_)
    {
        rhs.components_ = 0;
        rhs.numVertices_ = 0;
        rhs.positions_ = NULL;
        rhs.normals_ = NULL;
        rhs.numTriangles_ = 0;
        rhs.triangles_ = NULL;
    }

    Primitive::~Primitive()
    {
        LDELETE_ARRAY(triangles_);
        LDELETE_ARRAY(normals_);
        LDELETE_ARRAY(positions_);
    }

    void Primitive::refine(const Primitive& src, const lray::Matrix44& matrix)
    {
        components_ = src.components_;

        if(numVertices_<src.numVertices_){
            LDELETE_ARRAY(normals_);
            LDELETE_ARRAY(positions_);
            positions_ = LNEW Vector3[src.numVertices_];
            if(src.hasComponent(Component_Normal)){
                normals_ = LNEW Vector3[src.numVertices_];
            }
        }
        numVertices_ = src.numVertices_;

        // transform positions
        for(s32 i=0; i<numVertices_; ++i){
            positions_[i] = mul(matrix, src.positions_[i]);
        }

        // transform normals
        if(src.hasComponent(Component_Normal)){
            normals_ = LNEW Vector3[src.numVertices_];
            for(s32 i=0; i<numVertices_; ++i){
                normals_[i] = mul33(matrix, src.normals_[i]);
            }
        }

        // copy triangles
        if(numTriangles_<src.numTriangles_){
            LDELETE_ARRAY(triangles_);
            triangles_ = LNEW Triangle[src.numTriangles_];
        }
        numTriangles_ = src.numTriangles_;
        ::memcpy(triangles_, src.triangles_, sizeof(Triangle)*numTriangles_);
    }

    void Primitive::getTriangleProxies(TriangleProxy* dst) const
    {
        for(s32 i=0; i<numTriangles_; ++i){
            dst[i].primitive_ = this;
            dst[i].index_ = i;
        }
    }

    Primitive& Primitive::operator=(Primitive&& rhs)
    {
        if(this == &rhs){
            return *this;
        }
        LDELETE_ARRAY(triangles_);
        LDELETE_ARRAY(normals_);
        LDELETE_ARRAY(positions_);

        components_ = rhs.components_;
        numVertices_ = rhs.numVertices_;
        positions_ = rhs.positions_;
        normals_ = rhs.normals_;
        numTriangles_ = rhs.numTriangles_;
        triangles_ = rhs.triangles_;

        rhs.components_ = 0;
        rhs.numVertices_ = 0;
        rhs.positions_ = NULL;
        rhs.normals_ = NULL;
        rhs.numTriangles_ = 0;
        rhs.triangles_ = NULL;
        return *this;
    }
}
