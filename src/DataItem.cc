// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <DataItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

const   DataItem::size_type DataItem::npos = DataItem::size_type (-1);

// ----------------------------------------------------------------------------

DataItem *DataItem::clone () const throw ()  {

    const   bool    tmp = is_inside_dependency_;

    is_inside_dependency_ = true;

    DataItem    *dip = _clone_hook ();

    is_inside_dependency_ = tmp;
    dip->is_inside_dependency_ = tmp;
    dip->change_reporter_ = change_reporter_;
    dip->this_ptr_ = this_ptr_;
    dip->col_ = col_;

    return (dip);
}

// ----------------------------------------------------------------------------

const char *DataItem::type_to_str (DataItem::types type)  {

    switch (type)  {
        case intType:
            return "int";
        case shortType:
            return "short";
        case ucharType:
            return "unsigned char";
        case floatType:
            return "float";
        case doubleType:
            return "double";
        case binaryType:
            return "binary";
        case dateType:
            return "date";
        case stringType:
            return "string";
        case unknownType:
            return "unknown";
        case nullType:
            return "null";
        case longlongType:
            return "longlong";
        case containerType:
            return "container";
        case referenceType:
            return "reference";
    }

    DMScu_FixedSizeString<1023> err;

    err.printf ("DataItem::type_to_str(): Invalid Type: %d",
                static_cast<int>(type));
    throw std::runtime_error (err.c_str ());
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
