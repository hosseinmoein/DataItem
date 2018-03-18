// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <assert.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include <sys/types.h>

#include <DMScu_FixedSizeString.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

class   SystemItem;
class   ContainerItem;
class   ReferenceItem;

// ----------------------------------------------------------------------------

// Turning off the type conversion operators is very often a handy thing to do,
// especially when you're trying to eliminate implicit conversions.
//
#define ENABLE_TYPE_CONVERSION_OPERATORS

class   DataItem  {

    friend class    SystemItem;
    friend class    ContainerItem;
    friend class    ReferenceItem;

    public:

        typedef unsigned int    size_type;
        typedef unsigned char   TypeByte;

        typedef void (DataItem::*ChangeReporter)(size_type row, size_type col);

        static  const   size_type   npos;

        inline DataItem (const DataItem &that) noexcept
            : change_reporter_ (that.change_reporter_),
              this_ptr_ (nullptr),
              col_ (npos),
              is_inside_dependency_ (false)  {   }

        inline DataItem (DataItem &&that) noexcept
            : change_reporter_ (that.change_reporter_),
              this_ptr_ (that.this_ptr_),
              col_ (that.col_),
              is_inside_dependency_ (that.is_inside_dependency_)  {   }

	    DataItem() noexcept  {   }
        virtual ~DataItem ()  {  }

        virtual bool is_null () const  { return (false); }
        virtual bool is_container () const  { return (false); }
        virtual bool is_numeric () const  { return (false); }
        virtual bool is_reference () const  { return (false); }

       // "types" of which DataItem derived classes can be
       //
       // This might be considered a bad "C++ O-O" design, however it
       // provides a valuable run-time info that C++ lacks and
       // that is "metadata". It also helps in (de)serializing.
       // Also it would help in Croba-tizing objects.
       //
        enum   types { unknownType = 0, intType, shortType, ucharType,
                       floatType, doubleType, binaryType, dateType,
                       stringType, longlongType, containerType,
                       referenceType, nullType };

        virtual const char *name_of () const = 0;
        virtual types type_api () const = 0;

        virtual std::string to_stdstring () const = 0;
        virtual std::ostream &operator << (std::ostream &) const = 0;

        virtual size_type bin_size () const = 0;
        virtual void bin_copy (unsigned char *buffer) const = 0;
        virtual int bin_construct (const unsigned char *buffer) = 0;

    protected:

       // Called by set() which is called by operator =()
       //
        virtual bool _set_hook (const DataItem &b) = 0;

       // Called by clone()
       //
        virtual DataItem *_clone_hook () const = 0;

       // Called by compare()
       //
        virtual int _compare_hook (const DataItem &) const = 0;

    public:

        DataItem *clone () const noexcept;
        inline void set (const DataItem &b) noexcept  {

            if (_set_hook (b))
                touch ();
        }

       // To trigger dependencies
       //
        inline void touch () noexcept  {

            if (change_reporter_ && this_ptr_ && ! is_inside_dependency_)
                (this_ptr_->*change_reporter_) (0, col_);
        }

        inline DataItem &operator = (const DataItem &rhs) noexcept  {

            if (this != &rhs)
                set (rhs);
            return (*this);
        }

        inline int compare (const DataItem &di) const noexcept  {

           // Any null item is smaller then a non-null
           // item
           //
            if ((! is_null ()) && (di.is_null ()))
                return (1);

            return (_compare_hook (di));
        }

        virtual DataItem &operator = (double rh)  {

            throw std::runtime_error ("DataItem::"
                                      "operator=(double) "
                                      "is not implemented");
        }
        virtual DataItem &operator = (int rh)  {

            throw std::runtime_error ("DataItem::"
                                      "operator=(int) "
                                      "is not implemented");
        }
        virtual DataItem &operator = (long long int rh)  {

            return (this->operator = (static_cast<double>(rh)));
        }
        virtual DataItem &operator = (unsigned char rh)  {

            throw std::runtime_error ("DataItem::"
                                      "operator=(unsigned char) "
                                      "is not implemented");
        }
        virtual DataItem &operator = (const char *rh)  {

            throw std::runtime_error ("DataItem::"
                                      "operator=(char *) "
                                      "is not implemented");
        }
        DataItem &operator = (const std::string &rh) noexcept  {

            return (*this = rh.c_str());
        }

        virtual double to_double () const  {

            throw std::runtime_error ("DataItem::"
                                      "double() "
                                      "is not implemented");
        }

        virtual long int to_long () const  {

            throw std::runtime_error ("DataItem::"
                                      "long() "
                                      "is not implemented");
        }

        virtual long long int to_longlong () const  {

            throw std::runtime_error ("DataItem::"
                                      "long long int() "
                                      "is not implemented");
        }

        virtual float to_float () const  {

            throw std::runtime_error ("DataItem::"
                                      "float() "
                                      "is not implemented");
        }

        virtual int to_int () const  {

            throw std::runtime_error ("DataItem::"
                                      "int() "
                                      "is not implemented");
        }

        virtual short int to_short () const  {

            throw std::runtime_error ("DataItem::"
                                      "short int() "
                                      "is not implemented");
        }

        virtual unsigned char to_uchar () const  {

            throw std::runtime_error ("DataItem::"
                                      "unsigned char() "
                                      "is not implemented");
        }

#ifdef ENABLE_TYPE_CONVERSION_OPERATORS

        inline operator std::string () const  { return (to_stdstring ()); }
        inline operator double () const  { return (to_double ()); }
        inline operator long int () const  { return (to_long ()); }
        inline operator long long int () const  { return (to_longlong ()); }
        inline operator float () const  { return (to_float ()); }
        inline operator int () const  { return (to_int ()); }
        inline operator short int () const  { return (to_short ()); }
        inline operator unsigned char () const  { return (to_uchar ()); }

#endif  // ENABLE_TYPE_CONVERSION_OPERATORS

       // The following methods make sense only in the context of a
       // container object. They are provided here only as an interface
       // They are re-implemented in ContainerItem
       //
        virtual bool contains (const char *) const  { return (false); }
        virtual size_type num_cols () const  { return (0); }
        virtual size_type num_rows () const  { return (0); }

        virtual const DataItem *get (size_type row, size_type col) const  {

            throw std::runtime_error ("DataItem::"
                                      "get() "
                                      "is not implemented");
        }
        virtual const DataItem *get (size_type row, const char *col) const  {

            throw std::runtime_error ("DataItem::"
                                      "get() "
                                      "is not implemented");
        }
        inline const DataItem *get (size_type col) const noexcept  {

            return (get (0, col));
        }
        inline const DataItem *get (const char *col) const noexcept  {

            return (get (0, col));
        }

        inline const DataItem &
		operator () (size_type row, size_type col) const noexcept  {

            return (*get (row, col));
        }
        inline const DataItem &operator [] (size_type col) const noexcept  {

            return (operator () (0, col));
        }

        inline DataItem &operator () (size_type row, size_type col) noexcept  {

            return (*(DataItem *) get (row, col));
        }
        inline DataItem &operator [] (size_type col) noexcept  {

            return (operator () (0, col));
        }

        inline const DataItem &
		operator () (size_type row, const char *col) const noexcept  {

            return (*get (row, col));
        }
        inline const DataItem &operator [] (const char *col) const noexcept  {

            return (operator () (0, col));
        }

        inline DataItem &operator () (size_type row, const char *col)
            noexcept  {

            return (*(DataItem *) get (row, col));
        }
        inline DataItem &operator [] (const char *col) noexcept  {

            return (operator () (0, col));
        }

        virtual const char *const col_name (size_type col) const  {

            throw std::runtime_error ("DataItem::"
                                      "col_name() "
                                      "is not implemented");
        }

       // The following methods make sense only in the context of a
       // reference object. It is provided here only as an interface
       // It is re-implemented in ReferenceItem
       //
        virtual void delete_reference ()  {   }

        virtual DataItem &get_refed_di ()  { return (*this); }
        virtual const DataItem &get_refed_di () const  { return (*this); }

       // This method is for derived classes to format their value
       // into a pre-defined format specified by parameter format
       // and return it in a string.
       // e.g. In case of DateItem class, a "AMR_DATE" means format
       // the date value into something like 09/14/1999
       //
        virtual std::string format_value (unsigned int format) const  {

            throw std::runtime_error ("DataItem::"
                                      "format_value() "
                                      "is not implemented");
        }

        static const char *type_to_str (DataItem::types type);

    private:

	    ChangeReporter  change_reporter_ = { nullptr };
        DataItem        *this_ptr_ = { nullptr };
        size_type       col_ = { npos };
	    mutable bool    is_inside_dependency_ = { false };

    public:

       // The project() method is similar but not identical to the
       // assignment operator. It is provided only for performance
       // reasons.
       //
       // NOTE:  project() bypasses the dependency mechanism.
       //        In other words the dependencies will not be
       //        triggered as a result of using project().
       //
        virtual DataItem &project (const DataItem &rhs)  {

            throw std::runtime_error ("DataItem::"
                                      "project() "
                                      "is not implemented");
        }
};

// ----------------------------------------------------------------------------

inline bool operator == (const DataItem &lhs, const DataItem &rhs) noexcept  {

    return (lhs.compare (rhs) == 0);
}

// ----------------------------------------------------------------------------

inline bool operator != (const DataItem &lhs, const DataItem &rhs) noexcept  {

    return (lhs.compare (rhs) != 0);
}

// ----------------------------------------------------------------------------

inline bool operator > (const DataItem &lhs, const DataItem &rhs) noexcept  {

    return (lhs.compare (rhs) > 0);
}

// ----------------------------------------------------------------------------

inline bool operator < (const DataItem &lhs, const DataItem &rhs) noexcept  {

    return (lhs.compare (rhs) < 0);
}

// ----------------------------------------------------------------------------

inline bool operator <= (const DataItem &lhs, const DataItem &rhs) noexcept  {

    return (lhs.compare (rhs) <= 0);
}

// ----------------------------------------------------------------------------

inline bool operator >= (const DataItem &lhs, const DataItem &rhs) noexcept  {

    return (lhs.compare (rhs) >= 0);
}

// ----------------------------------------------------------------------------

inline bool
operator == (const DataItem &lhs, const char *const rhs) noexcept  {

    return (lhs.to_stdstring () == rhs);
}

// ----------------------------------------------------------------------------

inline bool
operator != (const DataItem &lhs, const char *const rhs) noexcept  {

    return (lhs.to_stdstring () != rhs);
}

// ----------------------------------------------------------------------------

inline bool
operator == (const DataItem &lhs, const std::string &rhs) noexcept  {

    return (lhs.to_stdstring () == rhs);
}

// ----------------------------------------------------------------------------

inline bool
operator != (const DataItem &lhs, const std::string &rhs) noexcept  {

    return (lhs.to_stdstring () != rhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator *= (DataItem &lhs, double rhs) noexcept  {

    lhs = lhs.to_double () * rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator += (DataItem &lhs, double rhs) noexcept  {

    lhs = lhs.to_double () + rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator /= (DataItem &lhs, double rhs) noexcept  {

    lhs = lhs.to_double () / rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator -= (DataItem &lhs, double rhs) noexcept  {

    lhs = lhs.to_double() - rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator *= (DataItem &lhs, int rhs) noexcept  {

    lhs = lhs.to_int () * rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator += (DataItem &lhs, int rhs) noexcept  {

    lhs = lhs.to_int () + rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator /= (DataItem &lhs, int rhs) noexcept  {

    lhs = lhs.to_int () / rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator -= (DataItem &lhs, int rhs) noexcept  {

    lhs = lhs.to_int () - rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator *= (DataItem &lhs, float rhs) noexcept  {

    lhs = lhs.to_double () * rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator += (DataItem &lhs, float rhs) noexcept  {

    lhs = lhs.to_double () + rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator /= (DataItem &lhs, float rhs) noexcept  {

    lhs = lhs.to_double () / rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator -= (DataItem &lhs, float rhs) noexcept  {

    lhs = lhs.to_double () - rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator *= (DataItem &lhs, short rhs) noexcept  {

    lhs = lhs.to_int () * rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator += (DataItem &lhs, short rhs) noexcept  {

    lhs = lhs.to_int () + rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator /= (DataItem &lhs, short rhs) noexcept  {

    lhs = lhs.to_int () / rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator -= (DataItem &lhs, short rhs) noexcept  {

    lhs = lhs.to_int () - rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator *= (DataItem &lhs, long long int rhs) noexcept  {

    lhs = lhs.to_longlong () * rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator += (DataItem &lhs, long long int rhs) noexcept  {

    lhs = lhs.to_longlong () + rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator /= (DataItem &lhs, long long int rhs) noexcept  {

    lhs = lhs.to_longlong () / rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator -= (DataItem &lhs, long long int rhs) noexcept  {

    lhs = lhs.to_longlong () - rhs;
    return (lhs);
}

// ----------------------------------------------------------------------------

inline DataItem &operator << (DataItem &lhs, const DataItem &rhs) noexcept  {

    return (lhs.project (rhs));
}

// ----------------------------------------------------------------------------

inline std::ostream &
operator << (std::ostream &o, const DataItem &r) noexcept  {

    return (r.operator << (o));
}

// ----------------------------------------------------------------------------

class   NumericItem : public DataItem  {

    public:

        NumericItem () noexcept  {   }
        NumericItem (const NumericItem &r) noexcept : DataItem (r)  {   }
        NumericItem (NumericItem &&r) noexcept : DataItem (r)  {   }
        inline bool is_numeric () const  { return (true); }
};

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
