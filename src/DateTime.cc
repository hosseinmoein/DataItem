// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include <DateTime.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

#ifdef WIN32
const   char    *DateTime::TIMEZONES_ [] =
{
    "TZ=GMT",
    "TZ=GMT+03",    // "America/Buenos_Aires",
    "TZ=GMT+06",    // "America/Chicago",
    "TZ=GMT+08",    // "America/Los_Angeles",
    "TZ=GMT+06",    // "America/Mexico_City",
    "TZ=GMT+05",    // "America/New_York",
    "TZ=GMT-04",    // "Asia/Dubai",
    "TZ=GMT-08",    // "Asia/Hong_Kong",
    "TZ=GMT-08",    // "Asia/Shanghai",
    "TZ=GMT-08",    // "Asia/Singapore",
    "TZ=GMT-03:30", // "Asia/Tehran",
    "TZ=GMT-02",    // "Asia/Tel_Aviv",
    "TZ=GMT-09",    // "Asia/Tokyo",
    "TZ=GMT-11",    // "Australia/Melbourne",
    "TZ=GMT-10",    // "Australia/NSW",
    "TZ=GMT+03",    // "Brazil/East",
    "TZ=GMT-01",    // "Europe/Berlin",
    "TZ=GMT-00",    // "Europe/London",
    "TZ=GMT-03",    // "Europe/Moscow",
    "TZ=GMT-01",    // "Europe/Paris",
    "TZ=GMT-01",    // "Europe/Rome",
    "TZ=GMT-01",    // "Europe/Vienna",
    "TZ=GMT-01",    // "Europe/Zurich",
    "TZ=GMT+00",    // "UTC",
    "TZ=GMT-07",    // "Asia/Seoul",
    "TZ=GMT-08",    // "Asia/Taipei",
    "TZ=GMT-01",    // "Eurpoe/Sweden",
    "TZ=GMT-13",    // "NZ",
    "TZ=GMT-01",    // "Europe/Oslo",
    "TZ=GMT-01",    // "Europe/Warsaw",
    "TZ=GMT-01",    // "Europe/Budapest"
};
#else
const   char    *DateTime::TIMEZONES_ [] =
{
    "GMT",
    "America/Buenos_Aires",
    "America/Chicago",
    "America/Los_Angeles",
    "America/Mexico_City",
    "America/New_York",
    "Asia/Dubai",
    "Asia/Hong_Kong",
    "Asia/Shanghai",
    "Asia/Singapore",
    "Asia/Tehran",
    "Asia/Tel_Aviv",
    "Asia/Tokyo",
    "Australia/Melbourne",
    "Australia/NSW",
    "Brazil/East",
    "Europe/Berlin",
    "Europe/London",
    "Europe/Moscow",
    "Europe/Paris",
    "Europe/Rome",
    "Europe/Vienna",
    "Europe/Zurich",
    "UTC",
    "Asia/Seoul",
    "Asia/Taipei",
    "Eurpoe/Stockholm",
    "NZ",
    "Europe/Oslo",
    "Europe/Warsaw",
    "Europe/Budapest"
};
#endif // WIN32

const   DateTime::DI_initializer   DateTime::di_init_;

// ----------------------------------------------------------------------------

DateTime::DI_initializer::DI_initializer () throw ()  {

#ifdef WIN32 // Can not locate GetDynamicTimeZoneInformation() in kernel32.dll
   // DYNAMIC_TIME_ZONE_INFORMATION    tz_info;
   // char                             buffer [33];
   // size_t                           s = 0;

   // GetDynamicTimeZoneInformation (&tz_info);

   // wcstombs_s (&s, buffer, 32, tz_info.StandardName, 32);
   // DateTime::tzname_ = buffer;

   // wcstombs_s (&s, buffer, 32, tz_info.DaylightName, 32);
   // DateTime::alt_tzname_ = buffer;
    _tzset ();
#else
    ::tzset ();
#endif // WIN32
}

// ----------------------------------------------------------------------------

const   char    *const  DateTime::MONTH_ [] =
{
    "January", "February", "March", "April", "May", "June", "July",
    "August", "September", "October", "November", "December"
};
const   char    *const  DateTime::MONTH_BR_ [] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
    "Sep", "Oct", "Nov", "Dec"
};
const   char    *const  DateTime::WDAY_ [] =
{
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
    "Friday", "Saturday"
};
const   char    *const  DateTime::WDAY_BR_ [] =
{
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

// ----------------------------------------------------------------------------

DateTime::DateTime (TIME_ZONE time_zone) throw () : time_zone_ (time_zone)  {

#ifdef WIN32
    SYSTEMTIME  syst;

    GetLocalTime (&syst);

    struct  tm  stm;

    stm.tm_year = syst.wYear - 1900;
    stm.tm_mon = syst.wMonth - 1;
    stm.tm_mday = syst.wDay;
    stm.tm_hour = syst.wHour;
    stm.tm_min = syst.wMinute;
    stm.tm_sec = syst.wSecond;
    stm.tm_isdst = -1;

    // set_time (mktime (&stm),
    //           static_cast<MillisecondType>(syst.wMilliseconds));
    set_time (::time (NULL), 0);
#else
    struct  timespec    ts;

    ::clock_gettime (CLOCK_REALTIME, &ts);
    set_time (ts.tv_sec, 0);

   // This is a hack. Since the original resolution of DateTime was
   // milliseconds, set_time() take a millisecond second argument. We have to
   // convert to nanosecond manually here.
   //
    nanosecond_ = ts.tv_nsec;
#endif // WIN32
}

// ----------------------------------------------------------------------------

void DateTime::set_time (EpochType the_time, MillisecondType millis) throw()  {

    date_ = DateType (INVALID_TIME_T_);
    hour_ = HourType (INVALID_TIME_T_);
    minute_ = MinuteType (INVALID_TIME_T_);
    second_ = SecondType (INVALID_TIME_T_);
    week_day_ = BAD_DAY;

    nanosecond_ = millis * 1000000;
    time_ = the_time;
}

// ----------------------------------------------------------------------------

DateTime::DateTime (DateType d,
                    HourType hr,
                    MinuteType mn,
                    SecondType sc,
                    MillisecondType ms,
                    TIME_ZONE tzone) throw ()
    : date_ (d),
      hour_ (hr),
      minute_ (mn),
      second_ (sc),
      nanosecond_ (ms * 1000000),

     // Refer to the comment in the header file, as why we are assigning
     // INVALID_TIME_T_ to time_.
     //
      time_ (INVALID_TIME_T_),
      week_day_ (BAD_DAY),
      time_zone_ (tzone)  {
}

// ----------------------------------------------------------------------------

// I'm adding the following formats:
//
// AME_STYLE:
//  (1)  DD/MM/YYYY
//  (2)  DD/MM/YYYY HH
//  (3)  DD/MM/YYYY HH:MM
//  (4)  DD/MM/YYYY HH:MM:SS
//  (5)  DD/MM/YYYY HH:MM:SS.MMM
//
// EUR_STYLE:
//  (1)  YYYY/MM/DD
//  (2)  YYYY/MM/DD HH
//  (3)  YYYY/MM/DD HH:MM
//  (4)  YYYY/MM/DD HH:MM:SS
//  (5)  YYYY/MM/DD HH:MM:SS.MMM
//
DateTime::DateTime (const char *s, DATE_STYLE ds, TIME_ZONE tz)
    : time_ (INVALID_TIME_T_),
      week_day_ (BAD_DAY)  {

    time_zone_ = tz;

    const   char    *str = s;

    while (::isspace (*str)) ++str;

    if (ds == YYYYMMDD)
        *this = str;
    else if (ds == AME_STYLE)  {
        const   size_t   str_len = ::strlen (str);

        if (str_len == 10)  {
            hour_ = minute_ = second_ = nanosecond_ = 0;

            int year, month, day;

            ::sscanf (str, "%d/%d/%d", &month, &day, &year);
            date_ = year * 10000 + month * 100 + day;
        }
        else if (str_len == 13)  {
            minute_ = second_ = nanosecond_ = 0;

            int year, month, day;

            ::sscanf (str, "%d/%d/%d %hd", &month, &day, &year, &hour_);
            date_ = year * 10000 + month * 100 + day;
        }
        else if (str_len == 16)  {
            second_ = nanosecond_ = 0;

            int year, month, day;

            ::sscanf (str, "%d/%d/%d %hd:%hd",
                      &month, &day, &year, &hour_, &minute_);
            date_ = year * 10000 + month * 100 + day;
        }
        else if (str_len == 19)  {
            nanosecond_ = 0;

            int year, month, day;

            ::sscanf (str, "%d/%d/%d %hd:%hd:%hd",
                      &month, &day, &year, &hour_, &minute_, &second_);
            date_ = year * 10000 + month * 100 + day;
        }
        else if (str_len == 23)  {
            int             year, month, day;
            MillisecondType ms;

            ::sscanf (str, "%d/%d/%d %hd:%hd:%hd.%hd",
                      &month, &day, &year, &hour_, &minute_, &second_, &ms);
            date_ = year * 10000 + month * 100 + day;
            nanosecond_ = ms * 1000000;;
        }
        else  {
            DMScu_FixedSizeString<511>  err;

            err.printf ("DateTime::DateTime(const char *): Don't know how to "
                        "parse '%s'", s);
            throw std::runtime_error (err.c_str ());
        }
    }
    else if (ds == EUR_STYLE)  {
        const   size_t   str_len = ::strlen (str);

        if (str_len == 10)  {
            hour_ = minute_ = second_ = nanosecond_ = 0;

            int year, month, day;

            ::sscanf (str, "%d/%d/%d", &year, &month, &day);
            date_ = year * 10000 + month * 100 + day;
        }
        else if (str_len == 13)  {
            minute_ = second_ = nanosecond_ = 0;

            int year, month, day;

            ::sscanf (str, "%d/%d/%d %hd", &year, &month, &day, &hour_);
            date_ = year * 10000 + month * 100 + day;
        }
        else if (str_len == 16)  {
            second_ = nanosecond_ = 0;

            int year, month, day;

            ::sscanf (str, "%d/%d/%d %hd:%hd",
                      &year, &month, &day, &hour_, &minute_);
            date_ = year * 10000 + month * 100 + day;
        }
        else if (str_len == 19)  {
            nanosecond_ = 0;

            int year, month, day;

            ::sscanf (str, "%d/%d/%d %hd:%hd:%hd",
                      &year, &month, &day, &hour_, &minute_, &second_);
            date_ = year * 10000 + month * 100 + day;
        }
        else if (str_len == 23)  {
            int             year, month, day;
            MillisecondType ms;

            ::sscanf (str, "%d/%d/%d %hd:%hd:%hd.%hd",
                      &year, &month, &day, &hour_, &minute_, &second_, &ms);
            date_ = year * 10000 + month * 100 + day;
            nanosecond_ = ms * 1000000;;
        }
        else  {
            DMScu_FixedSizeString<511>  err;

            err.printf ("DateTime::DateTime(const char *): Don't know how to "
                        "parse '%s'", s);
            throw std::runtime_error (err.c_str ());
        }
    }
}

// ----------------------------------------------------------------------------

// There are many different ways of representing
// a date-time value. We will add code as need arises.
//
// Currently, the following formats are supported:
//  (1)  YYYYMMDD
//  (2)  YYYYMMDD HH
//  (3)  YYYYMMDD HH:MM
//  (4)  YYYYMMDD HH:MM:SS
//  (5)  YYYYMMDD HH:MM:SS.MMM
//
DateTime &DateTime::operator = (const char *s)  {

    const   char    *str = s;

    while (::isspace (*str)) ++str;

    const   size_t   str_len = ::strlen (str);

    if (str_len == 8)  {
        hour_ = minute_ = second_ = nanosecond_ = 0;
        ::sscanf (str, "%d", &date_);
    }
    else if (str_len == 11)  {
        minute_ = second_ = nanosecond_ = 0;
        ::sscanf (str, "%d %hd", &date_, &hour_);
    }
    else if (str_len == 14)  {
        second_ = nanosecond_ = 0;
        ::sscanf (str, "%d %hd:%hd", &date_, &hour_, &minute_);
    }
    else if (str_len == 17)  {
        nanosecond_ = 0;
        ::sscanf (str, "%d %hd:%hd:%hd", &date_, &hour_, &minute_, &second_);
    }
    else if (str_len == 21)  {
        MillisecondType ms;

        ::sscanf (str, "%d %hd:%hd:%hd.%hd",
                  &date_, &hour_, &minute_, &second_, &ms);
        nanosecond_ = ms * 1000000;;
    }
    else  {
        DMScu_FixedSizeString<511>  err;

        err.printf ("DateTime::operator=(const char *): Don't know how to "
                    "parse '%s'", s);
        throw std::runtime_error (err.c_str ());
    }

    return (*this);
}

// ----------------------------------------------------------------------------

DateTime &DateTime::operator = (DateType the_date)  {

    DMScu_FixedSizeString<32>   buffer;

    buffer.printf ("%u", the_date);
    *this = buffer.c_str ();
    return (*this);
}

// ----------------------------------------------------------------------------

int DateTime::dt_compare (const DateTime &rhs) const  {

    const EpochType t = time() - rhs.time();

    return (t == 0 ? nanosec () - rhs.nanosec () : t);
}

// ----------------------------------------------------------------------------

DateTime::DatePartType DateTime::days_in_month () const throw ()  {

    switch (month ())  {
        case APR:
        case JUN:
        case SEP:
        case NOV:
            return (30);
        case FEB:
           // This I remember from CML.
           //
            if (year () % 4 == 0 && year () % 100 != 0 || year () % 400 == 0)
                return (29);
            else
                return (28);
        default:
            return (31);
    }
}

// ----------------------------------------------------------------------------

// Notice that we are not checking time_ to be valid.
// This is potentially a bug
//
bool DateTime::is_valid () const throw ()  {

    return (year () > 1900 && year () < 2525 &&
            month () > BAD_MONTH && month () <= DEC &&
            dmonth () > 0 && dmonth () <= days_in_month () &&
            hour () >= 0 && hour () < 24 &&
            minute () >= 0 && minute () < 60 &&
            sec () >= 0 && sec () < 60 &&
            nanosec () >= 0 && nanosec () < 1000000000);
}

// ----------------------------------------------------------------------------

// Is Good Friday a business day?
// No, it isn't, but calculating its date is complicated.
// To add calculation of Good Friday, see
// http://en.wikipedia.org/wiki/Computus
// for calculation of Easter Sunday.
// Good Friday is the Friday before Easter Sunday.
//
bool DateTime::is_us_business_day () const throw ()  {

    const   DatePartType    m_day = dmonth (); // 1 - 31
    const   DatePartType    mon = month ();    // JAN - DEC
    const   WEEKDAY         w_day = dweek ();  // SUN - SAT

    return (! (is_weekend () ||
               is_newyear () ||

              // President Harry S. Truman day of mourning (December 28, 1972)
              //
               (year () == 1972 && mon == DEC && m_day == 28) ||

              // President Lyndon B. Johnson day of mourning (January 25, 1973)
              //
               (year () == 1973 && mon == JAN && m_day == 25) ||

              // New York City black out (July 14, 1977)
              //
               (year () == 1977 && mon == JUL && m_day == 14) ||

              // President Richard M. Nixon day of mourning (April 27, 1994)
              //
               (year () == 1994 && mon == APR && m_day == 27) ||

              // World Trade Center attack (September 11-14, 2001)
              //
               (year () == 2001 && mon == SEP && m_day >= 11 && m_day <= 14) ||

              // President Ronald Reagan day of mourning (June 11, 2004)
              //
               (year () == 2004 && mon == JUN && m_day == 11) ||

              // President Gerald R. Ford day of mourning (January 2, 2007)
              //
               (mon == JAN && m_day == 2 && w_day == TUE && year () == 2007) ||

              // Martin Luther King Day (third Monday of January)
              //
               (w_day == MON && (m_day >= 15 && m_day <= 21) && mon == JAN) ||

              // President's Day (third Monday of February)
              //
               (w_day == MON && (m_day >= 15 && m_day <= 21) && mon == FEB) ||

              // Memorial Day (last Monday of May)
              //
               (w_day == MON && m_day >= 25 && mon == MAY) ||

              // Independence Day (July 4 or Monday, July 5 or Friday, July 3)
              //
               ((m_day == 4 || (w_day == MON && m_day == 5) ||
                (w_day == FRI && m_day == 3)) && mon == JUL) ||

              // Labor Day (first Monday of September)
              //
               (w_day == MON && m_day <= 7 && mon == SEP) ||

              // Thanksgiving Day (fourth Thursday of November)
              //
               (w_day == THU && (m_day >= 22 && m_day <= 28) && mon == NOV) ||

               is_xmas ()));
}

// ----------------------------------------------------------------------------

// Is Veteran's Day a bank holiday?
//
bool DateTime::is_us_bank_holiday () const throw ()  {

    if (! is_us_business_day ())
        return (true);
    else  {
        const   DatePartType    m_day = dmonth (); // 1 - 31
        const   DatePartType    mon = month ();    // JAN - DEC
        const   WEEKDAY         w_day = dweek ();  // SUN - SAT

       // Columbus Day (second Monday of October)
       //
        if (w_day == MON && m_day >= 8 && m_day <= 14 && mon == OCT)
            return (true);
    }

    return (false);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
