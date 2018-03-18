// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstring>

#include <StringItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

const   char    *const  StringItem::EMPTY_STR = "";

// ----------------------------------------------------------------------------

int StringItem::_compare_hook (const DataItem &rhs) const  {

    if (rhs.type_api () == stringType)  {
        const   StringItem  &si = static_cast<const StringItem &>(rhs);

        return (::strcmp (val_ ? val_ : EMPTY_STR,
                          si.val_ ? si.val_ : EMPTY_STR));
    }

    DMScu_FixedSizeString<63>   err;

    err = "StringItem::_compare_hook(): cannot compare StringItem to "
          "non-string type";
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

DataItem &StringItem::operator = (double rh)  {

    char    slug [128];

    ::sprintf (slug, "%lf", rh);
    return (*this = slug);
}

// ----------------------------------------------------------------------------

DataItem &StringItem::operator = (int rh)  {

    char    slug [32];

    ::sprintf (slug, "%d", rh);
    return (*this = slug);
}

// ----------------------------------------------------------------------------

DataItem &StringItem::operator = (long long int rh)  {

    char    slug [128];

    ::sprintf (slug, "%lld", rh);
    return (*this = slug);
}

// ----------------------------------------------------------------------------

DataItem &StringItem::operator = (unsigned char rh)  {

    char    slug [4];

    ::sprintf (slug, "%c", rh);
    return (*this = slug);
}

// ----------------------------------------------------------------------------

bool StringItem::_set_hook (const DataItem &rhs)  {

    const   std::string rhs_as_stdstring = rhs.to_stdstring ();
    const   size_type   rhs_size = rhs_as_stdstring.size ();

    if (val_ == NULL || rhs_size > ::strlen (val_))  {
        delete[] val_;
        val_ = new char [rhs_size + 1];
    }
    ::strcpy (val_, rhs_as_stdstring.c_str ());
    return (true);
}

// ----------------------------------------------------------------------------

// A Byte to specify the type (i.e. stringType) +
// A short value to specify the string length +
// The string itself
//
DataItem::size_type StringItem::bin_size () const  {

    return (sizeof (TypeByte) + sizeof (size_type) +
            ::strlen (val_ ? val_ : EMPTY_STR));
}

// ----------------------------------------------------------------------------

void StringItem::bin_copy (unsigned char *buffer) const  {

    unsigned    char    *ptr = buffer;
    const   size_type   len = strlen (val_ ? val_ : EMPTY_STR);

    *ptr = static_cast<TypeByte>(type_api ());

   // NOTE: we're calling memcpy() in order to avoid alignment issues.
   //
    ptr += sizeof (TypeByte);
    ::memcpy (ptr, &len, sizeof (size_type));
    ptr += sizeof (size_type);
    ::memcpy (ptr, val_ ? val_ : EMPTY_STR, len);

    return;
}

// ----------------------------------------------------------------------------

int StringItem::bin_construct (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types> (*buffer);

    if (typ == stringType)  {
        const   unsigned    char    *ptr = buffer + sizeof (TypeByte);
        size_type                   len = 0;

       // NOTE: we're calling memcpy() in order to avoid alignment issues.
       //
        ::memcpy (&len, ptr, sizeof (size_type));
        ptr += sizeof (size_type);

        delete[] val_;
        val_ = NULL;
        if (len != 0)  {
            val_ = new char [len + 1];
            ::memcpy (val_, ptr, len);
            val_ [len] = 0;
        }

        return (0);
    }

    DMScu_FixedSizeString<63>   err;

    err.printf("StringItem::bin_construct(): Type must be string but it is %s",
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
