// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <DataItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

class   FloatItem : public NumericItem  {

    public:

        typedef float   value_type;

        inline FloatItem () : val_ (0.0)  {   }
        inline explicit FloatItem (value_type f) : val_ (f)  {   }
        inline FloatItem (const FloatItem &r)
            : NumericItem (r), val_ (r.val_)  {   }
        inline FloatItem (FloatItem &&r) : NumericItem (r), val_ (r.val_)  {   }
        inline ~FloatItem ()  {   }

        inline FloatItem &operator = (const FloatItem &rh)  {

            if (this != &rh)
                set (rh);
            return (*this);
        }

       // override class DataItem virtual functions
       //
        std::string to_stdstring () const;

        inline std::ostream &operator << (std::ostream &o) const  {

            return (o << val_);
        }
        inline const char *name_of () const  { return ("FloatItem"); }
        inline types type_api () const  { return (floatType); }

    protected:

        inline DataItem *_clone_hook () const  {

            return (new FloatItem (*this));
        }
        bool _set_hook (const DataItem &b);
        int _compare_hook (const DataItem &) const;

    public:

        size_type bin_size () const;
        void bin_copy (unsigned char *buffer) const;
        int bin_construct (const unsigned char *buffer);

        inline double to_double () const  {

            return (static_cast<double>(val_));
        }
        inline float to_float () const  { return (static_cast<float>(val_)); }
        inline int to_int () const  { return (static_cast<int>(val_)); }
        inline short int to_short () const  {

            return (static_cast<short int>(val_));
        }
        inline unsigned char to_uchar () const  {

            return (static_cast<unsigned char>(val_));
        }
        inline long int to_long () const  {

            return (static_cast<long int>(val_));
        }
        inline long long int to_longlong () const  {

            return (static_cast<long long int>(val_));
        }

        DataItem &operator = (double rh);
        DataItem &operator = (int rh);
        DataItem &operator = (unsigned char rh);
        DataItem &operator = (const char *rh);

    private:

        value_type  val_;

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

            val_ = rhs.to_float();
            return (*this);
        }
};

// ----------------------------------------------------------------------------

inline std::ostream &operator << (std::ostream &o, const FloatItem &r)  {

    return r.operator << (o);
}

// ----------------------------------------------------------------------------

inline bool operator == (const FloatItem &lhs, const FloatItem &rhs) throw () {

    return (lhs.compare (rhs) == 0);
}

// ----------------------------------------------------------------------------

inline bool operator != (const FloatItem &lhs, const FloatItem &rhs) throw () {

    return (lhs.compare (rhs) != 0);
}

// ----------------------------------------------------------------------------

inline bool operator < (const FloatItem &lhs, const FloatItem &rhs) throw ()  {

    return (lhs.compare (rhs) < 0);
}

// ----------------------------------------------------------------------------

inline bool operator <= (const FloatItem &lhs, const FloatItem &rhs) throw () {

    return (lhs.compare (rhs) <= 0);
}

// ----------------------------------------------------------------------------

inline bool operator > (const FloatItem &lhs, const FloatItem &rhs) throw ()  {

    return (lhs.compare (rhs) > 0);
}

// ----------------------------------------------------------------------------

inline bool operator >= (const FloatItem &lhs, const FloatItem &rhs) throw () {

    return (lhs.compare (rhs) >= 0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
