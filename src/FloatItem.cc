// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdlib>

#include <FloatItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

std::string FloatItem::to_stdstring () const  {

    char    buf [64];

    sprintf (buf, "%f", val_);
    return (buf);
}

// ----------------------------------------------------------------------------

bool FloatItem::_set_hook (const DataItem &b)  {

    val_ = b.to_float ();
    return (true);
}

// ----------------------------------------------------------------------------

DataItem &FloatItem::operator = (double rh)  {

    const   FloatItem   tmp (rh);

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &FloatItem::operator = (int rh)  {

    const   FloatItem   tmp (static_cast<value_type>(rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &FloatItem::operator = (unsigned char rh)  {

    const   FloatItem   tmp (static_cast<value_type> (rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &FloatItem::operator = (const char *rh)  {

    const   float       f = static_cast<value_type>(::strtod (rh, NULL));
    const   FloatItem   tmp (f);

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

int FloatItem::_compare_hook (const DataItem &cp) const  {

    if (cp.is_numeric ())  {
        const   value_type  rslt = val_ - cp.to_float ();

        return (rslt > 0.0F ? 1 : (rslt < 0.0F ? -1 : 0));
    }

    DMScu_FixedSizeString<63>   err;

    err = "FloatItem::_compare_hook(): cannot compare FloatItem to "
          "non-numeric type";
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

// A Byte to specify the type (i.e. floatType) +
// The float value itself
//
DataItem::size_type FloatItem::bin_size () const  {

    return (sizeof (TypeByte) + sizeof (value_type));
}

// ----------------------------------------------------------------------------

void FloatItem::bin_copy (unsigned char *buffer) const  {

    *buffer = static_cast<TypeByte>(type_api ());

   // NOTE: we're calling memcpy() in order to avoid alignment issues.
   //
    memcpy (buffer + sizeof (TypeByte), &val_, sizeof (value_type));

    return;
}

// ----------------------------------------------------------------------------

int FloatItem::bin_construct (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types>(*buffer);

    if (typ == floatType)  {
       // NOTE: we're calling memcpy() in order to avoid alignment issues.
       //
        memcpy (&val_, (buffer + sizeof (TypeByte)), sizeof (value_type));

        return (0);
    }

    DMScu_FixedSizeString<63>   err;

    err.printf ("FloatItem::bin_construct(): Type must be float but it is %s",
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
