// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <cstdlib>

#include <DataItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

class   StringItem : public DataItem  {

    private:

        static  const   char    *const  EMPTY_STR;

    public:

        typedef std::string value_type;

        inline StringItem () : val_ (nullptr)  {   }
        inline StringItem (const StringItem &that)
            : DataItem (that), val_ (nullptr)  { *this = that; }
        inline StringItem (StringItem &&that)
            : DataItem (that), val_ (that.val_)  {  }
        inline explicit StringItem (const char *s) : val_ (nullptr)  {

            val_ = new char [strlen (s) + 1];
            strcpy (val_, s);
        }
        inline ~StringItem ()  { delete[] val_; }

        inline StringItem &operator = (const StringItem &rh)  {

            if (this != &rh)
                set (rh);
            return *this;
        }

       // NOTE: This method is specific to the StringItem class. Its purpose is
       //       efficiency. It doesn't produce the dreaded std:string
       //
        inline const char *const to_const_char_str () const  {

            return (val_ ? val_ : EMPTY_STR);
        }

        // override class DataItem virtual functions

        inline value_type to_stdstring () const  {

            return (to_const_char_str ());
        }

        inline std::ostream &operator << (std::ostream &o) const  {

            return (o << (val_ ? val_ : EMPTY_STR));
        }
        inline const char *name_of () const  { return ("StringItem"); }
        inline types type_api () const  { return (stringType); }

    protected:

        inline DataItem *_clone_hook () const  {

            return (new StringItem (*this));
        }
        bool _set_hook (const DataItem &b);
        int _compare_hook (const DataItem &) const;

    public:

        size_type bin_size () const;
        void bin_copy (unsigned char *buffer) const;
        int bin_construct (const unsigned char *buffer);

        inline double to_double () const  {

            return (::strtod (val_ ? val_ : EMPTY_STR, nullptr));
        }

        inline float to_float () const  {

            return (static_cast<float> (to_double ()));
        }

        inline int to_int () const  {

            return (static_cast<int> (to_long ()));
        }

        inline short int to_short () const  {

            return (static_cast<short int> (to_long ()));
        }
        inline unsigned char to_uchar () const  {

            return (static_cast<unsigned char>
                        (::strtoul (val_ ? val_ : EMPTY_STR, nullptr, 0)));
        }

        inline long int to_long () const  {

            return (strtol (val_ ? val_ : EMPTY_STR, nullptr, 0));
        }

        inline long long int to_long_long () const  {

#ifdef WIN32
            return (_strtol_l (val_ ? val_ : EMPTY_STR, nullptr, 0, nullptr));
#else
            return (strtoll (val_ ? val_ : EMPTY_STR, nullptr, 0));
#endif
        }

        DataItem &operator = (double rh);
        DataItem &operator = (int rh);
        DataItem &operator = (unsigned char rh);
        inline DataItem &operator = (const char *rhs)  {

            const   StringItem tmp (rhs);

            return (*this = tmp);
        }

        DataItem &operator = (long long int rh);

    private:

        char    *val_;

    public:

       //
       // NOTE:  The following methods bypasse the dependency mechanism.
       //        In other words the dependencies will not be triggered as a
       //        result of using the following methods. Their sole purpose
       //        is performance.
       //

       // The project() method is similar but not identical to the
       // assignment operator. It is provided only for performance
       // reasons.
       //
        inline DataItem &project (const DataItem &rhs)  {

            const   value_type rhs_str = rhs.to_stdstring ();

            if (val_ == nullptr || rhs_str.size () > strlen (val_))  {
                delete[] val_;
                val_ = new char [rhs_str.size () + 1];
            }

            strcpy (val_, rhs_str.c_str ());
            return (*this);
        }

       // take_ownership() method is similar to the assignment operator. It is
       // assumed that str was dynamically allocated using the new operator.
       //
        inline StringItem &take_ownership (char *str)  {

            delete[] val_;
            val_ = str;
            return (*this);
        }
};

// ----------------------------------------------------------------------------

inline std::ostream &operator << (std::ostream &o, const StringItem &r)  {

    return (r.operator << (o));
}

// ----------------------------------------------------------------------------

inline bool
operator == (const StringItem &lhs, const StringItem &rhs) throw ()  {

    return (lhs.compare (rhs) == 0);
}

// ----------------------------------------------------------------------------

inline bool
operator != (const StringItem &lhs, const StringItem &rhs) throw ()  {

    return (lhs.compare (rhs) != 0);
}

// ----------------------------------------------------------------------------

inline bool
operator < (const StringItem &lhs, const StringItem &rhs) throw ()  {

    return (lhs.compare (rhs) < 0);
}

// ----------------------------------------------------------------------------

inline bool
operator <= (const StringItem &lhs, const StringItem &rhs) throw ()  {

    return (lhs.compare (rhs) <= 0);
}

// ----------------------------------------------------------------------------

inline bool
operator > (const StringItem &lhs, const StringItem &rhs) throw ()  {

    return (lhs.compare (rhs) > 0);
}

// ----------------------------------------------------------------------------

inline bool
operator >= (const StringItem &lhs, const StringItem &rhs) throw ()  {

    return (lhs.compare (rhs) >= 0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
