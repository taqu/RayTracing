#ifndef INC_LRAY_REFERENCECOUNTED_H__
#define INC_LRAY_REFERENCECOUNTED_H__
/**
@file ReferenceCounted.h
@author t-sakai
@date 2017/12/14 create
*/
#include "IntrusivePtr.h"

namespace lray
{
    //---------------------------------------------------------
    //---
    //--- ReferenceCounted
    //---
    //---------------------------------------------------------
    template<class Derived> class ReferenceCounted;
    template<class T>
    void intrusive_ptr_addref(ReferenceCounted<T>* ptr)
    {
        ++ptr->referenceCount_;
    }
    template<class T>
    void intrusive_ptr_release(ReferenceCounted<T>* ptr)
    {
        if(0==--ptr->referenceCount_){
            LDELETE_RAW(ptr);
        }
    }

    template<class Derived>
    class ReferenceCounted
    {
    public:
        typedef ReferenceCounted<Derived> this_type;
        typedef IntrusivePtr<Derived> pointer_type;

    protected:
        ReferenceCounted(const this_type&) = delete;
        this_type& operator=(const this_type&) = delete;

        friend void intrusive_ptr_addref<Derived>(ReferenceCounted<Derived>* ptr);
        friend void intrusive_ptr_release<Derived>(ReferenceCounted<Derived>* ptr);

        ReferenceCounted()
            :referenceCount_(0)
        {}

        virtual ~ReferenceCounted()
        {}

        s32 referenceCount_;
    };
}
#endif //INC_LRAY_REFERENCECOUNTED_H__
