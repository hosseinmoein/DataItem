// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdlib>

#include <IntItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

std::string IntItem::to_stdstring () const  {

    char    buf [16];

    sprintf (buf, "%d", val_);
    return (buf);
}

// ----------------------------------------------------------------------------

bool IntItem::_set_hook (const DataItem &b)  {

    val_ = b.to_int ();
    return (true);
}

// ----------------------------------------------------------------------------

DataItem &IntItem::operator = (double rh)  {

    const   IntItem tmp (static_cast<value_type> (rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &IntItem::operator = (unsigned char rh)  {

    const   IntItem tmp (static_cast<value_type> (rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &IntItem::operator = (const char *rh)  {

    const   int     i = static_cast<value_type>(::strtol (rh, NULL, 0));
    const   IntItem tmp (i);

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

int IntItem::_compare_hook (const DataItem &cp) const  {

    if (cp.is_numeric ())
        return (val_ - cp.to_int ());

    DMScu_FixedSizeString<63>   err;

    err = "IntItem::_compare_hook(): cannot compare IntItem to "
          "non-numeric type";
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

// A Byte to specify the type (i.e. intType) +
// The integer value itself
//
DataItem::size_type IntItem::bin_size () const  {

    return (sizeof (TypeByte) + sizeof (value_type));
}

// ----------------------------------------------------------------------------

void IntItem::bin_copy (unsigned char *buffer) const  {

    *buffer = static_cast<TypeByte>(type_api ());

   // NOTE: we're calling memcpy() in order to avoid alignment issues.
   //
    ::memcpy (buffer + sizeof (TypeByte), &val_, sizeof (value_type));

    return;
}

// ----------------------------------------------------------------------------

int IntItem::bin_construct (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types>(*buffer);

    if (typ == intType)  {
       // NOTE: we're calling memcpy() in order to avoid alignment issues.
       //
        ::memcpy (&val_, (buffer + sizeof (TypeByte)), sizeof (value_type));

        return (0);
    }

    DMScu_FixedSizeString<63>   err;

    err.printf ("IntItem::bin_construct(): Type must be int but it is %s",
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
