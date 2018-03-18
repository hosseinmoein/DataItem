// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdlib>

#include <LongLongItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

std::string LongLongItem::to_stdstring () const  {

    char    buf [128];

    sprintf (buf, "%lld", val_);
    return (buf);
}

// ----------------------------------------------------------------------------

bool LongLongItem::_set_hook (const DataItem &b)  {

    val_ = b.to_longlong ();
    return (true);
}

// ----------------------------------------------------------------------------

DataItem &LongLongItem::operator = (double rh)  {

    const   LongLongItem    tmp (static_cast<value_type>(rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &LongLongItem::operator = (int rh)  {

    const   LongLongItem    tmp (static_cast<value_type>(rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &LongLongItem::operator = (unsigned char rh)  {

    const   LongLongItem    tmp (static_cast<value_type>(rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &LongLongItem::operator = (const char *rh)  {

#ifdef WIN32
    const   value_type      v = _strtol_l (rh, NULL, 0, NULL);
#else
    const   value_type      v = strtoll (rh, NULL, 0);
#endif
    const   LongLongItem    tmp (v);

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &LongLongItem::operator = (value_type rh)  {

    const   LongLongItem    tmp (rh);

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

int LongLongItem::_compare_hook (const DataItem &cp) const  {

    if (cp.is_numeric ())
        return (static_cast<int>(val_ - cp.to_longlong ()));

    DMScu_FixedSizeString<63>   err;

    err = "LongLongItem::_compare_hook(): cannot compare LongLongItem to "
          "non-numeric type";
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

// A Byte to specify the type (i.e. intType) +
// The integer value itself
//
LongLongItem::size_type LongLongItem::bin_size() const  {

    return (sizeof (TypeByte) + sizeof (value_type));
}

// ----------------------------------------------------------------------------

void LongLongItem::bin_copy (unsigned char *buffer) const  {

    *buffer = static_cast<TypeByte>(type_api ());

   // NOTE: we're calling memcpy() in order to avoid alignment issues.
   //
    ::memcpy (buffer + sizeof (TypeByte), &val_, sizeof (value_type));

    return;
}

// ----------------------------------------------------------------------------

int LongLongItem::bin_construct (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types>(*buffer);

    if (typ == longlongType)  {
       // NOTE: we're calling memcpy() in order to avoid alignment issues.
       //
        ::memcpy (&val_, (buffer + sizeof (TypeByte)), sizeof (value_type));

        return (0);
    }

    DMScu_FixedSizeString<63>   err;

    err.printf ("LongLongItem::bin_construct(): Type must be long long but "
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
