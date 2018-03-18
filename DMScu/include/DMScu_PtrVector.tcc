// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <DMScu_PtrVector.h>

// ----------------------------------------------------------------------------

template <class cu_Data, class cu_COPIER>
DMScu_PtrVector<cu_Data, cu_COPIER>::~DMScu_PtrVector () throw ()  {

    for (iterator itr = begin (); itr != end (); ++itr)
        delete *itr;
}

// ----------------------------------------------------------------------------

template <class cu_Data, class cu_COPIER>
typename DMScu_PtrVector<cu_Data, cu_COPIER>::iterator
DMScu_PtrVector<cu_Data, cu_COPIER>::
erase (iterator pos) throw ()  {

    delete *pos;
    return (vector_.erase (pos));
}

// ----------------------------------------------------------------------------

template <class cu_Data, class cu_COPIER>
typename DMScu_PtrVector<cu_Data, cu_COPIER>::iterator
DMScu_PtrVector<cu_Data, cu_COPIER>::
erase (iterator first, iterator last) throw ()  {

    for (iterator itr = first; itr < last; ++itr)
        delete *itr;

    return (vector_.erase (first, last));
}

// ----------------------------------------------------------------------------

template <class cu_Data, class cu_COPIER>
bool DMScu_PtrVector<cu_Data, cu_COPIER>::
erase (const value_type &x) throw ()  {

    for (iterator itr = begin (); itr != end (); ++itr)
        if (*itr == x)  {
            erase (itr);
            return (true);
        }

    return (false);
}

// ----------------------------------------------------------------------------

template <class cu_Data, class cu_COPIER>
typename DMScu_PtrVector<cu_Data, cu_COPIER>::iterator
DMScu_PtrVector<cu_Data, cu_COPIER>::
find (const data_type &x) throw ()  {

    for (iterator itr = begin (); itr != end (); ++itr)
        if (**itr == x)
            return (itr);

    return (end ());
}

// ----------------------------------------------------------------------------

template <class cu_Data, class cu_COPIER>
typename DMScu_PtrVector<cu_Data, cu_COPIER>::const_iterator
DMScu_PtrVector<cu_Data, cu_COPIER>::
find (const data_type &x) const throw ()  {

    for (const_iterator itr = begin (); itr != end (); ++itr)
        if (**itr == x)
            return (itr);

    return (end ());
}

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
