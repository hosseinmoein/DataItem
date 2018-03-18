// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdlib>

#include <ShortItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

std::string ShortItem::to_stdstring () const  {

    char    buf [16];

    sprintf (buf, "%d", static_cast<int>(val_));
    return (buf);
}

// ----------------------------------------------------------------------------

bool ShortItem::_set_hook (const DataItem &b)  {

    val_ = b.to_short ();
    return (true);
}

// ----------------------------------------------------------------------------

DataItem &ShortItem::operator = (double rh)  {

    const   ShortItem   tmp (static_cast<value_type> (rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &ShortItem::operator = (int rh)  {

    const   ShortItem   tmp (static_cast<value_type> (rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &ShortItem::operator = (unsigned char rh)  {

    const   ShortItem   tmp (static_cast<value_type> (rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &ShortItem::operator = (const char *rh)  {

    const   short       s = static_cast<value_type> (::strtol (rh, NULL, 0));
    const   ShortItem   tmp (s);

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

int ShortItem::_compare_hook (const DataItem &cp) const  {

    if (cp.is_numeric ())
        return (val_ - cp.to_short ());

    DMScu_FixedSizeString<63>   err;

    err = "ShortItem::_compare_hook(): cannot compare ShortItem to "
          "non-numeric type";
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

// A Byte to specify the type (i.e. shortType) +
// The short value itself
//
DataItem::size_type ShortItem::bin_size () const  {

    return (sizeof (TypeByte) + sizeof (value_type));
}

// ----------------------------------------------------------------------------

void ShortItem::bin_copy (unsigned char *buffer) const  {

    *buffer = static_cast<TypeByte>(type_api());

   // NOTE: we're calling memcpy() in order to avoid alignment issues.
   //
    ::memcpy (buffer + sizeof (TypeByte), &val_, sizeof (value_type));

    return;
}

// ----------------------------------------------------------------------------

int ShortItem::bin_construct (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types>(*buffer);

    if (typ == shortType)  {

       // NOTE: we're calling memcpy() in order to avoid alignment issues.
       //
        ::memcpy (&val_, buffer + sizeof (TypeByte), sizeof (value_type));

        return (0);
    }

    DMScu_FixedSizeString<63>   err;

    err.printf("DoubleItem::bin_construct(): Type must be double but it is %s",
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
