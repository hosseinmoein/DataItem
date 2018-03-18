// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdio>

#include <BinaryItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

void BinaryItem::reset_to (size_type len, const value_type *s)  {

    if (len > buf_size_ && s)  {
        delete[] buffer_;
        buffer_ = new value_type [len];
        buf_size_ = len;
    }

    if (s)
        ::memcpy (buffer_, s, len);

    length_ = len;
    return;
}

// ----------------------------------------------------------------------------

DataItem &BinaryItem::operator = (const char *const hexstr)  {

    size_type       len;
    const   char    *ptr = hexstr;

    if (! ptr || ((len = strlen (ptr)) < 2))  {
        reset_to (0);
        return *this;
    }
    if (len % 2)
        len--;

    reset_to (len / 2);
    for (size_type i = 0; i < len / 2; i++)  {
        value_type  uc;
        value_type  uc2;

        uc = htod_ (*ptr++);
        uc <<= 4;
        uc |= (uc2 = htod_ (*ptr++));

        buffer_ [i] = uc;
    }
    return (*this);
}

// ----------------------------------------------------------------------------

// BinaryItem::operator std::string () const
//
std::string BinaryItem::to_stdstring () const  {

    std::string s;
    char        buf [6];

    for (size_type i = 0; i < length_; ++i)  {
       sprintf (buf, "%2.2x", buffer_ [i]);
       s += buf;
    }

    return (s);
}

// ----------------------------------------------------------------------------

bool BinaryItem::_set_hook (const DataItem &b)  {

    if (b.type_api () == binaryType)  {
        reset_to (static_cast<const BinaryItem *>(&b)->length_,
                  static_cast<const BinaryItem *>(&b)->buffer_);
        return (true);
    }

    DMScu_FixedSizeString<63>   err;

    err.printf ("BinaryItem::set(): cannot set BinaryItem to %s",
                DataItem::type_to_str (b.type_api ()));
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

int BinaryItem::_compare_hook (const DataItem &cr) const  {

    if (cr.type_api () == binaryType)  {
        const   BinaryItem  &rf = static_cast<const BinaryItem &> (cr);

        return (length_ != rf.length_
                    ? length_ - rf.length_
                    : ::memcmp (buffer_, rf.buffer_, length_));
    }

    DMScu_FixedSizeString<63>   err;

    err.printf ("BinaryItem::_compare_hook(): cannot compare BinaryItem to "
                "a %s type",
                DataItem::type_to_str (cr.type_api ()));
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

// hex character to decimal
//
inline unsigned char BinaryItem::htod_ (char c)  {

    value_type  uc;

    if (c >= '0' && c <= '9')
       uc = c - '0';
    else if (c >= 'a' && c <= 'f')
       uc =  c - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
       uc = c - 'A' + 10;
    else
       uc = 0;

    return (uc);
}

// ----------------------------------------------------------------------------

// A Byte to specify the type (i.e. binaryType) +
// An integer value to specify the data length +
// The binary data itself
//
DataItem::size_type BinaryItem::bin_size () const  {

    return (sizeof (value_type) + sizeof (size_type) + length_);
}

// ----------------------------------------------------------------------------

void BinaryItem::bin_copy (value_type *buffer) const  {

    value_type  *ptr = buffer;

    *ptr = TypeByte (type_api ());
    ptr += sizeof (TypeByte);

   // NOTE: we're calling memcpy() in order to avoid alignment issues.
   //
    ::memcpy (ptr, &length_, sizeof (size_type));
    ptr += sizeof (size_type);
    ::memcpy (ptr, buffer_, length_);

    return;
}

// ----------------------------------------------------------------------------

int BinaryItem::bin_construct (const value_type *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types>(*buffer);

    if (typ == binaryType)  {
        const   value_type  *ptr = buffer + sizeof (TypeByte);

       // NOTE: we're calling memcpy() in order to avoid alignment issues.
       //
        ::memcpy (&length_, ptr, sizeof (size_type));
        ptr += sizeof (size_type);

        reset_to (length_, ptr);

        return (0);
    }

    DMScu_FixedSizeString<63>   err;

    err.printf ("BinaryItemDateItem::bin_construct(): Type must be binary but "
                "it is %s",
                DataItem::type_to_str (typ));

    throw std::runtime_error (err.c_str ());
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
