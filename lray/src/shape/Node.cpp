/**
@file Node.cpp
@author t-sakai
@date 2018/05/24 create
*/
#include "shape/Node.h"

namespace lray
{
    Node::Node()
        :parent_(-1)
        ,numChildren_(0)
        ,childrenStart_(-1)
        ,mesh_(-1)
    {
        matrix_.identity();
        worldMatrix_.identity();
    }

    Node::Node(Node&& rhs)
        :parent_(rhs.parent_)
        ,numChildren_(rhs.numChildren_)
        ,childrenStart_(rhs.childrenStart_)
        ,mesh_(rhs.mesh_)
        ,matrix_(rhs.matrix_)
        ,worldMatrix_(rhs.worldMatrix_)
    {
        rhs.parent_ = -1;
        rhs.numChildren_ = 0;
        rhs.childrenStart_ = -1;
        rhs.mesh_ = -1;
        rhs.matrix_.identity();
        rhs.worldMatrix_.identity();
    }

    Node::Node(const Char* name,
        s32 parent,
        s32 numChildren,
        s32 childrenStart,
        s32 mesh)
        :name_(name)
        ,parent_(parent)
        ,numChildren_(numChildren)
        ,childrenStart_(childrenStart)
        ,mesh_(mesh)
    {
        matrix_.identity();
        worldMatrix_.identity();
    }

    Node& Node::operator=(Node&& rhs)
    {
        if(this == &rhs){
            return *this;
        }
        parent_ = rhs.parent_;
        numChildren_ = rhs.numChildren_;
        childrenStart_ = rhs.childrenStart_;
        mesh_ = rhs.mesh_;
        matrix_ = rhs.matrix_;
        worldMatrix_ = rhs.worldMatrix_;
        rhs.parent_ = -1;
        rhs.numChildren_ = 0;
        rhs.childrenStart_ = -1;
        rhs.mesh_ = -1;
        rhs.matrix_.identity();
        rhs.worldMatrix_.identity();
        return *this;
    }
}
