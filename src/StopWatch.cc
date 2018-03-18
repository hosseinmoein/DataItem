// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdlib>

#include <DateItem.h>
#include <StopWatch.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

StopWatch::StopWatch () throw ()
    : time_vector_ (), running_ (false)  {   }

// ----------------------------------------------------------------------------

bool StopWatch::reset ()  {

    if (is_running ())
        throw std::runtime_error ("StopWatch::reset(): "
                                  "The stop watch is running and "
                                  "cannot be reset.");

    time_vector_.clear ();
    return (true);
}

// ----------------------------------------------------------------------------

bool StopWatch::start (const char *label) throw ()  {

    if (is_running ())
        return (false);

    if (time_vector_.empty ())
        time_vector_.reserve (4);

    const   DateItem    now;
    const   Node        node (static_cast<double>(now.time ()) +
                                  (static_cast<double>(now.msec ()) / 1000.0),
                              label);

    time_vector_.push_back (node);
    running_ = true;
    return (true);
}

// ----------------------------------------------------------------------------

// NOTE: It is OK to stop a watch that is not running. This is done so
//       exception safe-guards can be built around this class.
//
bool StopWatch::stop () throw ()  {

    if (is_running ())  {
        const   DateItem    now;
        const   Node        node (static_cast<double>(now.time ()) +
                                    (static_cast<double>(now.msec()) / 1000.0),
                                  NULL);

        time_vector_.push_back (node);
    }

    running_ = false;
    return (true);
}

// ----------------------------------------------------------------------------

double StopWatch::elapsed_time () const  {

    if (is_running () || time_vector_.size () < 2)
        throw std::runtime_error ("StopWatch::elapsed_time(): "
                                  "There is no time elapsed to measure.");

    return (time_vector_ [time_vector_.size () - 1].t -
            time_vector_ [time_vector_.size () - 2].t);
}

// ----------------------------------------------------------------------------

double StopWatch::total_elapsed_time () const  {

    if (is_running () || time_vector_.size () < 2)
        throw std::runtime_error ("StopWatch::total_elapsed_time(): "
                                  "There is no time elapsed to measure.");

    double  elapsed = 0.0;

    for (TimeVector::const_iterator itr = time_vector_.begin ();
         itr != time_vector_.end (); itr += 2)
        elapsed  += ((itr + 1)->t - itr->t);

    return (elapsed);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
