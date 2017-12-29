#ifndef INC_LRAY_INTRUSIVEPTR_H__
#define INC_LRAY_INTRUSIVEPTR_H__
/**
@file IntrusivePtr.h
@author t-sakai
@date 2017/12/14 create
*/
#include "lray.h"

namespace lray
{
    //---------------------------------------------------
    //---
    //--- IntrusivePtr
    //---
    //---------------------------------------------------
    template<class T>
    struct IntrusivePtrTraits
    {
        typedef T& reference_type;
        typedef const T& const_reference_type;
    };

    template<>
    struct IntrusivePtrTraits<void>
    {
        typedef void reference_type;
        typedef void const_reference_type;
    };

    template<class T>
    class IntrusivePtr
    {
    public:
        typedef IntrusivePtr<T> this_type;
        typedef T value_type;
        typedef T* pointer_type;
        typedef const T* const_pointer_type;
        typedef typename IntrusivePtrTraits<T>::reference_type reference_type;
        typedef typename IntrusivePtrTraits<T>::const_reference_type const_reference_type;

        IntrusivePtr()
            :pointer_(NULL)
        {
        }

        IntrusivePtr(T* pointer)
            :pointer_(pointer)
        {
            if(NULL != pointer_){
                intrusive_ptr_addref(pointer_);
            }
        }

        IntrusivePtr(const IntrusivePtr& rhs)
            :pointer_(rhs.pointer_)
        {
            if(NULL != pointer_){
                intrusive_ptr_addref(pointer_);
            }
        }

        IntrusivePtr(this_type&& rhs)
            :pointer_(rhs.pointer_)
        {
            rhs.pointer_ = NULL;
        }

        template<class U>
        IntrusivePtr(U* ptr)
            :pointer_(ptr)
        {
            if(NULL != pointer_){
                intrusive_ptr_addref(pointer_);
            }
        }

        template<class U>
        IntrusivePtr(const IntrusivePtr<U>& rhs)
            :pointer_(rhs.get())
        {
            if(NULL != pointer_){
                intrusive_ptr_addref(pointer_);
            }
        }

        ~IntrusivePtr()
        {
            if(NULL != pointer_){
                intrusive_ptr_release(pointer_);
            }
        }

        const_pointer_type get() const
        {
            return pointer_;
        }

        pointer_type get()
        {
            return pointer_;
        }

        template<class U>
        U* get_static_cast() const
        {
            return static_cast<U*>(pointer_);
        }

        template<class U>
        U* get_reinterpret_cast() const
        {
            return static_cast<U*>(pointer_);
        }

        template<class U>
        U* get_dynamic_cast() const
        {
            return dynamic_cast<U*>(pointer_);
        }

        const_reference_type operator*() const
        {
            return *pointer_;
        }

        reference_type operator*()
        {
            return *pointer_;
        }

        const_pointer_type operator->() const
        {
            return pointer_;
        }

        pointer_type operator->()
        {
            return pointer_;
        }

        operator bool() const
        {
            return (NULL != pointer_);
        }

        bool operator!() const
        {
            return (NULL == pointer_);
        }

        IntrusivePtr& operator=(const this_type& rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        this_type& operator=(this_type&& rhs);

        IntrusivePtr& operator=(T* rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        void swap(this_type& rhs);
    private:

        T *pointer_;
    };

    template<class T>
    typename IntrusivePtr<T>::this_type& IntrusivePtr<T>::operator=(this_type&& rhs)
    {
        if(this == &rhs){
            return;
        }
        if(NULL != pointer_){
            pointer_->release();
        }
        pointer_ = rhs.pointer_;
        rhs.pointer_ = NULL;
        return *this;
    }

    template<class T>
    void IntrusivePtr<T>::swap(this_type& rhs)
    {
        T* tmp = pointer_;
        pointer_ = rhs.pointer_;
        rhs.pointer_ = tmp;
    }

    template<class T, class U>
    inline bool operator==(const IntrusivePtr<T>& left, const IntrusivePtr<U>& right)
    {
        return left.get() == right.get();
    }

    template<class T, class U>
    inline bool operator!=(const IntrusivePtr<T>& left, const IntrusivePtr<U>& right)
    {
        return left.get() != right.get();
    }

    template<class T, class U>
    inline bool operator==(const IntrusivePtr<T>& left, const U* right)
    {
        return left.get() == right;
    }

    template<class T, class U>
    inline bool operator!=(const IntrusivePtr<T>& left, const U* right)
    {
        return left.get() != right;
    }

    template<class T, class U>
    inline bool operator==(const T* left, const IntrusivePtr<U>& right)
    {
        return left == right.get();
    }

    template<class T, class U>
    inline bool operator!=(const T* left, const IntrusivePtr<U>& right)
    {
        return left != right.get();
    }


    template<class T>
    inline bool operator<(const IntrusivePtr<T>& left, const IntrusivePtr<T>& right)
    {
        return left.get() < right.get();
    }

    template<class T>
    inline bool operator<=(const IntrusivePtr<T>& left, const IntrusivePtr<T>& right)
    {
        return left.get() <= right.get();
    }

    template<class T>
    inline bool operator>(const IntrusivePtr<T>& left, const IntrusivePtr<T>& right)
    {
        return left.get() > right.get();
    }

    template<class T>
    inline bool operator>=(const IntrusivePtr<T>& left, const IntrusivePtr<T>& right)
    {
        return left.get() >= right.get();
    }

    template<class Dst, class Src>
    inline IntrusivePtr<Dst> static_pointer_cast(const IntrusivePtr<Src>& pointer)
    {
        return static_cast<Dst*>(pointer.get());
    }

    template<class Dst, class Src>
    inline IntrusivePtr<Dst> reinterpret_pointer_cast(const IntrusivePtr<Src>& pointer)
    {
        return static_cast<Dst*>(pointer.get());
    }

    template<class Dst, class Src>
    inline IntrusivePtr<Dst> const_pointer_cast(const IntrusivePtr<Src>& pointer)
    {
        return const_cast<Dst*>(pointer.get());
    }

    template<class Dst, class Src>
    inline IntrusivePtr<Dst> dynamic_pointer_cast(const IntrusivePtr<Src>& pointer)
    {
        return dynamic_cast<Dst*>(pointer.get());
    }

}
#endif //INC_LRAY_INTRUSIVEPTR_H__
