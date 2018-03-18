// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#include <cstdio>
#include <iostream>
#include <ctime>
#include <string>
#include <stdexcept>

#include <sys/timeb.h>

#include <DMScu_FixedSizeString.h>
#include <DateTime.h>
#include <DataItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

class   DateItem : public DateTime, public DataItem  {

    public:

        explicit DateItem (TIME_ZONE time_zone = LOCAL)
            : DateTime(time_zone), DataItem()  {   }
        explicit DateItem (DateType d,
                           HourType hr = 0,
                           MinuteType mn = 0,
                           SecondType sc = 0,
                           MillisecondType ms = 0,
                           TIME_ZONE tzone = LOCAL)
            : DateTime (d, hr, mn, sc, ms, tzone), DataItem()  {  }
        inline DateItem (const DateItem &that) = default;
        inline DateItem (DateItem &&that) = default;

        explicit DateItem (const char *s,
                           DATE_STYLE ds = YYYYMMDD,
                           TIME_ZONE tz = LOCAL)
            : DateTime (s, ds, tz), DataItem()  {  }

        inline ~DateItem () = default;

        inline DateItem &operator = (const DateItem &rhs)  {

            if (this != &rhs)
                set (rhs);
            return (*this);
        }

        inline std::string to_stdstring () const  {

            return (format_value (DT_TM2));
        }

        inline std::string format_value (unsigned int format) const  {

            return (string_format(format));
        }

        DateItem &operator = (const char *rhs)  {

            static_cast<DateTime &>(*this) = rhs;
            return (*this);
        }

        DateItem &operator = (DateType rhs)  {

            static_cast<DateTime &>(*this) = rhs;
            return (*this);
        }

        inline const char *name_of () const   { return ("DateItem"); }
        inline types type_api () const  { return (dateType); }

    protected:

        inline DataItem *_clone_hook () const  {

            return (new DateItem (*this));
        }
        bool _set_hook (const DataItem &b);
        int _compare_hook (const DataItem &) const;

    public:

        size_type bin_size () const;
        void bin_copy (unsigned char *buffer) const;
        int bin_construct (const unsigned char *buffer);

        inline std::ostream &operator << (std::ostream &o) const  {

            return (o << to_stdstring ());
        }

        inline int to_int () const  { return (date ()); }
        inline float to_float () const  {

            return (static_cast<float>(this->operator double ()));
        }
        inline long int to_long () const  {

            return (static_cast<long int>(this->operator int ()));
        }
        inline double to_double () const  {

            DMScu_FixedSizeString<32>   buffer;

            buffer.printf ("%d%002d%002d%002d.%0003d",
                           static_cast<int>(date ()),
                           static_cast<int>(hour ()),
                           static_cast<int>(minute ()),
                           static_cast<int>(sec ()),
                           static_cast<int>(msec ()));

           return (strtod (buffer.c_str (), nullptr));
        }
        inline short int to_short () const  {

            return (static_cast<short int>(this->operator int ()));
        }
        inline long long int to_longlong () const  {

            return (1000000000 * static_cast<long long int>(date ()) +
                    10000000 * static_cast<long long int>(hour ()) +
                    100000 * static_cast<long long int>(minute ()) +
                    1000 * static_cast<long long int>(sec ()) +
                    static_cast<long long int>(msec ()));
        }

    public:

       // The project() method is similar but not identical to the
       // assignment operator. It is provided only for performance
       // reasons.
       //
       // NOTE:  project() bypasses the dependency mechanism.
       //        In other words the dependencies will not be
       //        triggered as a result of using project().
       //
        inline DataItem &project (const DataItem &rhs)  {

            std::string  str = rhs.to_stdstring ();

            *this = str.c_str ();
            return (*this);
        }
};

// ----------------------------------------------------------------------------

inline bool operator == (const DateItem &lhs, const DateItem &rhs) noexcept  {

    return (lhs.compare (rhs) == 0);
}

// ----------------------------------------------------------------------------

inline bool operator != (const DateItem &lhs, const DateItem &rhs) noexcept  {

    return (lhs.compare (rhs) != 0);
}

// ----------------------------------------------------------------------------

inline bool operator < (const DateItem &lhs, const DateItem &rhs) noexcept  {

    return (lhs.compare (rhs) < 0);
}

// ----------------------------------------------------------------------------

inline bool operator <= (const DateItem &lhs, const DateItem &rhs) noexcept  {

    return (lhs.compare (rhs) <= 0);
}

// ----------------------------------------------------------------------------

inline bool operator > (const DateItem &lhs, const DateItem &rhs) noexcept  {

    return (lhs.compare (rhs) > 0);
}

// ----------------------------------------------------------------------------

inline bool operator >= (const DateItem &lhs, const DateItem &rhs) noexcept  {

    return (lhs.compare (rhs) >= 0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:

