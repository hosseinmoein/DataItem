// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdio>
#include <cstdlib>

#include <ReferenceItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

ReferenceItem::ReferenceItem (value_type v) : val_ (v)  {

    if (v->is_reference ())
        throw std::runtime_error ("ReferenceItem::ReferenceItem(): "
                                  "You can potentially create an infinite loop "
                                  "by constructing a ReferenceItem of a "
                                  "ReferenceItem.");
}

// ----------------------------------------------------------------------------

ReferenceItem::size_type ReferenceItem::bin_size () const  {

    throw std::runtime_error ("ReferenceItem::bin_size(): A reference item "
                              "cannot be (de)serialized.");
}

// ----------------------------------------------------------------------------

void ReferenceItem::bin_copy(unsigned char *buffer) const  {

    throw std::runtime_error ("ReferenceItem::bin_copy(): A reference item "
                              "cannot be (de)serialized.");
}

// ----------------------------------------------------------------------------

int ReferenceItem::bin_construct (const unsigned char *buffer)  {

    throw std::runtime_error ("ReferenceItem::bin_construct(): A reference item "
                              "cannot be (de)serialized.");
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
