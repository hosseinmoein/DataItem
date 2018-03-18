// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <cstdlib>

#include <vector>
#include <string>
#include <stdexcept>

#include <DMScu_FixedSizeString.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

// This object measures elapsed time up to milliseconds.
//
class   StopWatch  {

    public:

        StopWatch () throw ();

        bool start (const char *label = NULL) throw ();
        bool stop () throw ();
        bool reset ();

        double elapsed_time () const;
        double total_elapsed_time () const;

        template<class di_STRING>
        di_STRING &elapsed_time (di_STRING &str) const  {

            _format_time (
                str,
                elapsed_time (),
                time_vector_ [time_vector_.size () - 2].label.c_str ());
            return (str);
        }
        template<class di_STRING>
        di_STRING &total_elapsed_time (di_STRING &str) const  {

            _format_time (str, total_elapsed_time (), "TOTAL");
            return (str);
        }

        template<class di_STRING>
        di_STRING &all_elapsed_times (di_STRING &str) const  {

            if (is_running () || time_vector_.size () < 2)
                throw std::runtime_error (
                    "StopWatch::all_elapsed_times(): "
                    "There is no time elapsed to measure.");

            DMScu_FixedSizeString<1023> buf;

            str.clear ();
            for (TimeVector::const_iterator citer = time_vector_.begin ();
                 citer != time_vector_.end (); citer += 2)  {
                _format_time (buf,
                              (citer + 1)->t - citer->t,
                              citer->label.c_str ());
                buf += "\n";
                str += buf.c_str ();
            }

            return (str);
        }

        bool is_running () const throw ()  { return (running_); }
        bool any_time_elapsed () const throw ()  {

            return (time_vector_.size () >= 2 ? true : false);
        }

    protected:

       // elapsed must be in seconds.milliseconds format
       //
        template<class di_STRING>
        static inline void _format_time (di_STRING &str,
                                         double elapsed,
                                         const char *label) throw ()  {

            const   time_t  secs = static_cast<time_t>(elapsed);
            const   double  ms =
                (elapsed - static_cast<double>(secs)) * 1000.0;
            char            buffer [1024];
            const   time_t  days = (secs / 3600) / 24;

            if (days > 0)
                sprintf (buffer,
                         "%ld Day(s) and %02ld:%02ld:%02ld.%03ld %s",
                         days,
                         (secs / 3600) - (days * 24),
                         (secs % 3600) / 60,
                         (secs % 3600) % 60,
                         static_cast<time_t>(ms),
                         label);
            else
                sprintf (buffer, "%02ld:%02ld:%02ld.%03ld %s",
                         secs / 3600,
                         (secs % 3600) / 60,
                         (secs % 3600) % 60,
                         static_cast<time_t>(ms),
                         label);

            str = buffer;
            return;
        }

    private:

        class   Node  {

            public:

                inline Node () throw () : t (0), label ()  {   }
                inline Node (double tt, const char *str) throw ()
                    : t (tt), label (str ? str : "")  {   }

                double      t;
                std::string label;
        };

        typedef std::vector<Node>   TimeVector;

        TimeVector  time_vector_;
        bool        running_;

       // These are not implemented.
       //
        StopWatch (const StopWatch &);
        StopWatch &operator = (const StopWatch &);
};

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
