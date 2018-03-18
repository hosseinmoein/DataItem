// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdlib>

#include <NullItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

int NullItem::_compare_hook (const DataItem &cp) const  {

    return (reinterpret_cast<unsigned long int>(this) -
            reinterpret_cast<unsigned long int>(&cp));
}

// ----------------------------------------------------------------------------

// Silently do nothing. This is a bottomless well.
// Kind of like UNIX's /dev/null.
//
bool NullItem::_set_hook (const DataItem &)  { return (true); }

// ----------------------------------------------------------------------------

// Just a Byte to specify the type (i.e. nullType)
//
DataItem::size_type NullItem::bin_size () const  {

    return (sizeof (DataItem::TypeByte));
}

// ----------------------------------------------------------------------------

void NullItem::bin_copy (unsigned char *buffer) const  {

    *buffer = static_cast<TypeByte>(type_api ());

    return;
}

// ----------------------------------------------------------------------------

int NullItem::bin_construct (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types>(*buffer);

    if (typ != nullType)  {
        DMScu_FixedSizeString<63>   err;

        err.printf("NullItem::bin_construct(): Type must be null but it is %s",
                   DataItem::type_to_str (typ));

        throw std::runtime_error (err.c_str ());
    }

    return (0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
