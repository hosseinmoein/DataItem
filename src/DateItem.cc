// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include <DateItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

// ----------------------------------------------------------------------------

bool DateItem::_set_hook (const DataItem &rhs)  {

   if (rhs.type_api () == dateType)  {
		static_cast<DateTime &>(*this) = static_cast<const DateItem &>(rhs);
        return (true);
   }

    DMScu_FixedSizeString<63>   err;

    err.printf ("DateItem::set(): cannot set DateItem to %s",
                DataItem::type_to_str (rhs.type_api ()));
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

int DateItem::_compare_hook (const DataItem &rhs) const  {

    if (rhs.type_api () == dateType)
        return (dt_compare(static_cast<const DateItem &>(rhs)));

    DMScu_FixedSizeString<63>   err;

    err.printf ("DateItem::_compare_hook(): cannot compare DateItem to a %s",
                DataItem::type_to_str (rhs.type_api ()));
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

// A Byte to specify the type (i.e. dateType) +
// Size of all the data parts
//
DataItem::size_type DateItem::bin_size () const  {

    return (sizeof (TypeByte) + sizeof (date_) +
            sizeof (hour_) + sizeof (minute_) +
            sizeof (second_) + sizeof (nanosecond_) +
            sizeof (time_) + sizeof (week_day_) + sizeof (time_zone_));
}

// ----------------------------------------------------------------------------

void DateItem::bin_copy (unsigned char *buffer) const  {

    unsigned    char    *ptr = buffer;

    *ptr = static_cast<TypeByte>(type_api ());
    ptr += sizeof (TypeByte);

    // NOTE: we're calling memcpy() in order to avoid alignment issues.

    ::memcpy (ptr, &date_, sizeof (DateType));
    ptr += sizeof (DateType);

    ::memcpy (ptr, &hour_, sizeof (HourType));
    ptr += sizeof (HourType);

    ::memcpy (ptr, &minute_, sizeof (MinuteType));
    ptr += sizeof (MinuteType);

    ::memcpy (ptr, &second_, sizeof (SecondType));
    ptr += sizeof (SecondType);

    ::memcpy (ptr, &nanosecond_, sizeof (NanosecondType));
    ptr += sizeof (NanosecondType);

    ::memcpy (ptr, &time_, sizeof (EpochType));
    ptr += sizeof (EpochType);

    ::memcpy (ptr, &week_day_, sizeof (week_day_));
    ptr += sizeof (week_day_);

    ::memcpy (ptr, &time_zone_, sizeof (time_zone_));

    return;
}

// ----------------------------------------------------------------------------

int DateItem::bin_construct (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types>(*buffer);

    if (typ == dateType)  {
        const   unsigned    char    *ptr = buffer + sizeof (TypeByte);

        // NOTE: we're calling memcpy() in order to avoid alignment issues.

        ::memcpy (&date_, ptr, sizeof (DateType));
        ptr += sizeof (DateType);

        ::memcpy (&hour_, ptr, sizeof (HourType));
        ptr += sizeof (HourType);

        ::memcpy (&minute_, ptr, sizeof (MinuteType));
        ptr += sizeof (MinuteType);

        ::memcpy (&second_, ptr, sizeof (SecondType));
        ptr += sizeof (SecondType);

        ::memcpy (&nanosecond_, ptr, sizeof (NanosecondType));
        ptr += sizeof (NanosecondType);

        ::memcpy (&time_, ptr, sizeof (EpochType));
        ptr += sizeof (EpochType);

        ::memcpy (&week_day_, ptr, sizeof (week_day_));
        ptr += sizeof (week_day_);

        ::memcpy (&time_zone_, ptr, sizeof (time_zone_));

        return (0);
    }

    DMScu_FixedSizeString<63>   err;

    err.printf ("DateItem::bin_construct(): Type must be date but it is %s",
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
