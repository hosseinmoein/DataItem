// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdlib>

#include <UCharItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

std::string UCharItem::to_stdstring () const  {

    char    buf [64];

    ::sprintf (buf, "%u", static_cast<unsigned int>(val_));

    return (buf);
}

// ----------------------------------------------------------------------------

bool UCharItem::_set_hook (const DataItem &b)  {

    val_ = b.to_uchar ();
    return (true);
}

// ----------------------------------------------------------------------------

DataItem &UCharItem::operator = (double rh)  {

    const   UCharItem   tmp (static_cast<value_type>(rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &UCharItem::operator = (int rh)  {

    const   UCharItem   tmp (static_cast<value_type>(rh));

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &UCharItem::operator = (UCharItem::value_type rh)  {

    const   UCharItem   tmp (rh);

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

DataItem &UCharItem::operator = (const char *rh)  {

    const   value_type  u = static_cast<value_type>(::strtol (rh, NULL, 0));
    const   UCharItem   tmp (u);

    return (*this = tmp);
}

// ----------------------------------------------------------------------------

int UCharItem::_compare_hook (const DataItem &cp) const  {

    if (cp.is_numeric ())
        return (static_cast<int>(val_) - cp.to_int ());

    DMScu_FixedSizeString<63>   err;

    err = "UCharItem::_compare_hook(): cannot compare UCharItem to "
          "non-numeric type";
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

// A Byte to specify the type (i.e. ucharType) +
// The TypeByte itself
//
DataItem::size_type UCharItem::bin_size () const  {

    return (sizeof (TypeByte) + sizeof (value_type));
}

// ----------------------------------------------------------------------------

void UCharItem::bin_copy (unsigned char *buffer) const  {

    *buffer = static_cast<TypeByte>(type_api());

    *(buffer + sizeof (TypeByte)) = val_;

    return;
}

// ----------------------------------------------------------------------------

int UCharItem::bin_construct (const unsigned char *buffer)  {

    const   types   typ = static_cast<types>(*buffer);

    if (typ == ucharType)  {
        val_ = *(buffer + sizeof (TypeByte));

        return (0);
    }

    DMScu_FixedSizeString<63>   err;

    err.printf ("UCharItem::bin_construct(): Type must be unsigned char but "
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
