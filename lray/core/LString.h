#ifndef INC_LRAY_LSTRING_H_
#define INC_LRAY_LSTRING_H_
/**
@file LString.h
@author t-sakai
@date 2017/12/21 create
*/
#include "../lray.h"

namespace lray
{
    //--------------------------------------------
    //---
    //--- String
    //---
    //--------------------------------------------
    class String
    {
    public:
        static const s32 ExpandSize = 8;
        static const u32 ExpandMask = ExpandSize-1;

        typedef s32(*FilterFunc)(s32 c);

        static const String& empty(){ return empty_;}

        String();
        explicit String(s32 capacity);
        explicit String(const Char* str);
        String(s32 length, const Char* str);
        String(s32 length, const Char* str, FilterFunc filter);
        String(const String& rhs);
        String(String&& rhs);
        ~String();

        inline s32 capacity() const;
        inline s32 length() const;
        inline const Char* c_str() const;
        inline const Char& operator[](s32 index) const;
        inline Char& operator[](s32 index);

        inline void clear();
        void clear(s32 capacity);
        void fill(s32 length, Char c=CharNull);

        void replace(Char match, Char c);

        void append(Char c);
        s32 pop_back();
        inline void append(const Char* str);
        void append(s32 length, const Char* str);

        bool operator==(const String& rhs) const;
        bool operator!=(const String& rhs) const;

        s32 printf(const Char* format, ...);
        String& operator=(const String& rhs);
        String& operator=(String&& rhs);
        String& operator=(const Char* str);
        inline void assign(const Char* str);
        void assign(s32 length, const Char* str);
        void assign(s32 length, const Char* str, FilterFunc filter);
        void swap(String& rhs);
    private:
        static inline s32 getCapacity(s32 capacity)
        {
            return (capacity+ExpandMask) & ~ExpandMask;
        }

        inline const Char* getBuffer() const
        {
            return (capacity_<=ExpandSize)? buffer_.small_ : buffer_.elements_;
        }
        inline Char* getBuffer()
        {
            return (capacity_<=ExpandSize)? buffer_.small_ : buffer_.elements_;
        }


        void initBuffer(s32 length);
        void createBuffer(s32 length);
        void expandBuffer(s32 length);

        static String empty_;

        s32 capacity_;
        s32 length_;

        union Buffer
        {
            Char small_[ExpandSize];
            Char* elements_;
        };
        Buffer buffer_;
    };

    inline s32 String::capacity() const
    {
        return capacity_;
    }

    inline s32 String::length() const
    {
        return length_;
    }

    inline const Char* String::c_str() const
    {
        return (ExpandSize<capacity_)? buffer_.elements_ : buffer_.small_;
    }

    inline const Char& String::operator[](s32 index) const
    {
        LASSERT(0<=index && index<length_);
        return getBuffer()[index];
    }

    inline Char& String::operator[](s32 index)
    {
        LASSERT(0<=index && index<length_);
        return getBuffer()[index];
    }

    inline void String::clear()
    {
        length_ = 0;
        Char* buffer = getBuffer();
        buffer[0] = CharNull;
    }

    inline void String::append(const Char* str)
    {
        append(strlen_s32(str), str);
    }

    inline void String::assign(const Char* str)
    {
        assign(strlen_s32(str), str);
    }

    s32 compare(const String& str0, const String& str1);

    inline void swap(String& str0, String& str1)
    {
        str0.swap(str1);
    }

}
#endif //INC_LRAY_LSTRING_H_
