// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <stdexcept>

#include <DataItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

class   NullItem : public DataItem  {

    public:

        inline NullItem ()  {   }
        inline NullItem (const NullItem &r) : DataItem (r)  {   }
        inline NullItem (NullItem &&r) : DataItem (r)  {   }

        inline NullItem &operator = (const NullItem &rh)  {

            if (this != &rh)
                set (rh);
            return (*this);
        }

       // override class DataItem virtual functions

        inline std::string to_stdstring () const  { return ("~~NULL"); }

        inline std::ostream &operator << (std::ostream &o) const  {

            return (o << "~~NULL");
        }
        inline const char *name_of () const  { return ("NullItem"); }
        inline types type_api () const  { return (nullType); }
        inline bool is_null () const  { return (true); }

    protected:

        inline DataItem *_clone_hook () const  {

            return (new NullItem (*this));
        }
        bool _set_hook (const DataItem &b);
        int _compare_hook (const DataItem &) const;

    public:

        size_type bin_size () const;
        void bin_copy (unsigned char *buffer) const;
        int bin_construct (const unsigned char *buffer);

        inline double to_double () const  {

            throw std::runtime_error ("NullItem::to_double() NullItem "
                                      "lacks any value");
        }
        inline float to_float () const  {

            throw std::runtime_error ("NullItem::to_float() NullItem "
                                      "lacks any value");
        }
        inline int to_int () const  {

            throw std::runtime_error ("NullItem::to_int() NullItem "
                                      "lacks any value");
        }
        inline long to_long () const  {

            throw std::runtime_error ("NullItem::to_long() NullItem "
                                      "lacks any value");
        }
        inline short int to_short () const  {

            throw std::runtime_error ("NullItem::to_short() NullItem "
                                      "lacks any value");
        }
        inline unsigned char to_uchar () const  {

            throw std::runtime_error ("NullItem::to_uchar() NullItem "
                                      "lacks any value");
        }
        inline long long int to_longlong () const  {

            throw std::runtime_error ("NullItem::to_longlong() "
                                      "NullItem lacks any value");
        }

    public:

       // Silently do nothing. This is a bottomless well.
       // Kind of like UNIX's /dev/null.
       //
        inline DataItem &project (const DataItem &rhs)  {

            return (*this);
        }
};

// ----------------------------------------------------------------------------

inline std::ostream &operator << (std::ostream &o, const NullItem &r)  {

    return (r.operator << (o));
}

// ----------------------------------------------------------------------------

inline bool operator == (const NullItem &lhs, const NullItem &rhs) throw ()  {

    return (lhs.compare (rhs) == 0);
}

// ----------------------------------------------------------------------------

inline bool operator != (const NullItem &lhs, const NullItem &rhs) throw ()  {

    return (lhs.compare (rhs) != 0);
}

// ----------------------------------------------------------------------------

inline bool operator < (const NullItem &lhs, const NullItem &rhs) throw ()  {

    return (lhs.compare (rhs) < 0);
}

// ----------------------------------------------------------------------------

inline bool operator <= (const NullItem &lhs, const NullItem &rhs) throw ()  {

    return (lhs.compare (rhs) <= 0);
}

// ----------------------------------------------------------------------------

inline bool operator > (const NullItem &lhs, const NullItem &rhs) throw ()  {

    return (lhs.compare (rhs) > 0);
}

// ----------------------------------------------------------------------------

inline bool operator >= (const NullItem &lhs, const NullItem &rhs) throw ()  {

    return (lhs.compare (rhs) >= 0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
