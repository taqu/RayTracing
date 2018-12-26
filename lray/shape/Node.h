#ifndef INC_LRAY_NODE_H__
#define INC_LRAY_NODE_H__
/**
@file Node.h
@author t-sakai
@date 2018/05/24 create
*/
#include "../lray.h"
#include "../core/LString.h"
#include "../math/Matrix44.h"

namespace lray
{
    /**
    */
    class Node
    {
    public:
        Node();
        Node(Node&& rhs);
        Node(const Char* name,
            s32 parent,
            s32 numChildren,
            s32 childrenStart,
            s32 mesh);

        inline Matrix44& getMatrix();
        inline const Matrix44& getMatrix() const;

        inline Matrix44& getWorldMatrix();
        inline const Matrix44& getWorldMatrix() const;

        inline s32 getParent() const;
        inline s32 getNumChildren() const;
        inline s32 getChildrenStart() const;
        inline s32 getMesh() const;

        Node& operator=(Node&& rhs);
    private:
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        String name_;
        s32 parent_;
        s32 numChildren_;
        s32 childrenStart_;
        s32 mesh_;
        Matrix44 matrix_;
        Matrix44 worldMatrix_;
    };

    inline Matrix44& Node::getMatrix()
    {
        return matrix_;
    }

    inline const Matrix44& Node::getMatrix() const
    {
        return matrix_;
    }

    inline Matrix44& Node::getWorldMatrix()
    {
        return worldMatrix_;
    }

    inline const Matrix44& Node::getWorldMatrix() const
    {
        return worldMatrix_;
    }

    inline s32 Node::getParent() const
    {
        return parent_;
    }

    inline s32 Node::getNumChildren() const
    {
        return numChildren_;
    }

    inline s32 Node::getChildrenStart() const
    {
        return childrenStart_;
    }

    inline s32 Node::getMesh() const
    {
        return mesh_;
    }
}
#endif //INC_LRAY_NODE_H__
