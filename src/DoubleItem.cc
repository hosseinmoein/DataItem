// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdio>

#include <DoubleItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

std::string DoubleItem::to_stdstring () const  {

    char    buf [128];

    sprintf (buf, "%lf", val_);
    return (buf);
}

// ----------------------------------------------------------------------------

bool DoubleItem::_set_hook (const DataItem &b)  {

    val_ = b.to_double ();
    return (true);
}

// ----------------------------------------------------------------------------

DataItem &DoubleItem::operator = (int rh)  {

    return (*this = DoubleItem (static_cast<value_type>(rh)));
}

// ----------------------------------------------------------------------------

DataItem &DoubleItem::operator = (unsigned char rh)  {

    const   DoubleItem  tmp (static_cast<value_type>(rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &DoubleItem::operator = (const char *rh)  {

    const   double      d = ::strtod (rh, NULL);
    const   DoubleItem  tmp (d);

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

int DoubleItem::_compare_hook (const DataItem &cp) const  {

    if (cp.is_numeric ())  {
        const   value_type  rslt = val_ - cp.to_double ();

        return (rslt > 0.0 ? 1 : (rslt < 0.0 ? -1 : 0));
    }

    DMScu_FixedSizeString<63>   err;

    err = "DoubleItem::_compare_hook(): cannot compare DoubleItem to "
          "non-numeric type";
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

// A Byte to specify the type (i.e. doubleType) +
// The double value itself
//
DataItem::size_type DoubleItem::bin_size () const  {

    return (sizeof (TypeByte) + sizeof (value_type));
}

// ----------------------------------------------------------------------------

void DoubleItem::bin_copy (unsigned char *buffer) const  {

    *buffer = static_cast<TypeByte>(type_api ());

   // NOTE: we're calling memcpy() in order to avoid alignment issues.
   //
    memcpy (buffer + sizeof (TypeByte), &val_, sizeof (value_type));

    return;
}

// ----------------------------------------------------------------------------

int DoubleItem::bin_construct (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types>(*buffer);

    if (typ == doubleType)  {
        memcpy (&val_, (buffer + sizeof (TypeByte)), sizeof (value_type));

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
