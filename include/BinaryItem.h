// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <cstdlib>
#include <string>
#include <DataItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

class   BinaryItem : public DataItem  {

    public:

        typedef unsigned char   value_type;

        inline BinaryItem (const BinaryItem &br)
            : DataItem (br),
              buffer_ (nullptr),
              length_ (0),
              buf_size_ (0)  {

            *this = br;
         }

        inline BinaryItem (BinaryItem &&br)
            : DataItem (br),
              buffer_ (br.buffer_),
              length_ (br.length_),
              buf_size_ (br.buf_size_)  {

            br.buffer_ = nullptr;
            *this = br;
         }

        inline BinaryItem (size_type len, const value_type *p)
            : length_ (len),
              buf_size_ (len),
              buffer_ (NULL)  {

            buffer_ = new value_type [buf_size_];
            memcpy (buffer_, p, length_);
        }

        inline explicit BinaryItem (const char *hexstr)  { (*this) = hexstr; }
        BinaryItem () noexcept  {   }
        ~BinaryItem ()  { delete[] buffer_;  }

        inline BinaryItem &operator = (const BinaryItem &rh)  {

            if (this != &rh)
                set (rh);
            return (*this);
        }

        // override class DataItem virtual functions

        std::string to_stdstring () const;

        inline const char *name_of () const  { return ("BinaryItem"); }
        inline types type_api () const  { return (binaryType); }

    protected:

        inline DataItem *_clone_hook () const  {

            return (new BinaryItem(*this));
        }
        bool _set_hook (const DataItem &b);
        int _compare_hook (const DataItem &) const;

    public:

        size_type bin_size () const;
        void bin_copy (value_type *buffer) const;
        int bin_construct (const value_type *buffer);

        inline std::ostream &operator << (std::ostream & o) const  {

            return (o << to_stdstring ());
        }

        void reset_to (size_type len, const value_type *s = NULL);

        inline size_type length () const  { return (length_); }

       // NOTE: Don't do any funky stuff with this.
       //       It is strictly for reading.
       //
        inline const value_type *data () const  { return (buffer_); }

        DataItem &operator = (const char *const hexstr);

   private:

	    value_type  *buffer_ = { nullptr };
	    size_type   length_ = { 0 };
        size_type   buf_size_ = { 0 };

        static inline value_type htod_ (char uc);

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

            const   std::string str = rhs.to_stdstring ();

            reset_to (str.size (),
                      reinterpret_cast<const value_type *>(str.data ()));

            return (*this);
        }
};

// ----------------------------------------------------------------------------

inline std::ostream &operator << (std::ostream &o, const BinaryItem &r)  {

   return r.operator << (o);
}

// ----------------------------------------------------------------------------

inline bool
operator == (const BinaryItem &lhs, const BinaryItem &rhs) throw ()  {

    return (lhs.compare (rhs) == 0);
}

// ----------------------------------------------------------------------------

inline bool
operator != (const BinaryItem &lhs, const BinaryItem &rhs) throw ()  {

    return (lhs.compare (rhs) != 0);
}

// ----------------------------------------------------------------------------

inline bool
operator < (const BinaryItem &lhs, const BinaryItem &rhs) throw ()  {

    return (lhs.compare (rhs) < 0);
}

// ----------------------------------------------------------------------------

inline bool
operator <= (const BinaryItem &lhs, const BinaryItem &rhs) throw ()  {

    return (lhs.compare (rhs) <= 0);
}

// ----------------------------------------------------------------------------

inline bool
operator > (const BinaryItem &lhs, const BinaryItem &rhs) throw ()  {

    return (lhs.compare (rhs) > 0);
}

// ----------------------------------------------------------------------------

inline bool
operator >= (const BinaryItem &lhs, const BinaryItem &rhs) throw ()  {

    return (lhs.compare (rhs) >= 0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
