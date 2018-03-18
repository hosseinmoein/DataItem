// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <DataItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

// ReferenceItem is really a PointerItem. It contains a pointer to an
// instance of a DataItem. Its purpose is to prevent duplications of
// large ContainerItems. Although it contains a pointer, its interface
// resembles a reference more. That is why it is called ReferenceItem.
//
class   ReferenceItem : public DataItem  {

    public:

        typedef DataItem *  value_type;

        explicit ReferenceItem (value_type v);
        inline ReferenceItem (const ReferenceItem &r) : val_ (r.val_)  {   }
        inline ReferenceItem (ReferenceItem &&r) : val_ (r.val_)  {   }
        inline ~ReferenceItem ()  {   }

        inline ReferenceItem &operator = (const ReferenceItem &rh)  {

            if (this != &rh)
                set (rh);
            return (*this);
        }

        // override class DataItem virtual functions

        inline std::string to_stdstring () const  {

            return (val_->to_stdstring ());
        }

        inline std::ostream &operator << (std::ostream &o) const  {

            return (o << *val_);
        }
        inline const char *name_of () const  { return (val_->name_of ()); }
        inline types type_api () const  { return (val_->type_api ()); }

        inline bool is_null () const  { return (val_->is_null ()); }
        inline bool is_container () const  { return (val_->is_container ()); }
        inline bool is_numeric () const  { return (val_->is_numeric ()); }
        inline bool is_reference () const  { return (true); }

    protected:

        inline DataItem *_clone_hook () const  {

            return (new ReferenceItem (*this));
        }
        inline bool _set_hook (const DataItem &rhs)  {

            // if (val_ != rhs)  {
            //     val_ = const_cast<DataItem *>(rhs);
            //     return (true);
            // }
            // else
            //     return (false);

            val_ = const_cast<DataItem *>(&rhs);
            return (true);
        }
        inline int _compare_hook (const DataItem &cp) const  {

            return (val_->_compare_hook (cp));
        }

    public:

        size_type bin_size () const;
        void bin_copy (unsigned char *buffer) const;
        int bin_construct (const unsigned char *buffer);

        inline double to_double () const  { return (val_->to_double ()); }
        inline float to_float () const  { return (val_->to_float ()); }
        inline int to_int () const  { return (val_->to_int ()); }
        inline short int to_short () const  { return (val_->to_short ()); }
        inline unsigned char to_uchar () const  { return (val_->to_uchar ()); }
        inline long int to_long () const  { return (val_->to_long ()); }
        inline long long int to_longlong () const {

            return (val_->to_longlong ());
        }

        inline DataItem &operator = (double rh)  {

            return ((*val_) = rh);
        }
        inline DataItem &operator = (int rh)  {

            return ((*val_) = rh);
        }
        inline DataItem &operator = (unsigned char rh)  {

            return ((*val_) = rh);
        }
        inline DataItem &operator = (const char *rh)  {

            return ((*val_) = rh);
        }
        inline DataItem &operator = (long long rh)  {

            return ((*val_) = rh);
        }

        // ContainerItem methods:

        inline bool contains (const char *col) const  {

            return (val_->contains (col));
        }

        inline size_type num_cols () const  {

            return (val_->num_cols ());
        }

        inline size_type num_rows () const  {

            return (val_->num_rows ());
        }

        inline const DataItem *get (size_type row, size_type col) const  {

            return (val_->get (row, col));
        }

        inline const DataItem *get (size_type row, const char *col) const  {

            return (val_->get (row, col));
        }

        inline const char *const col_name (size_type col) const  {

            return (val_->col_name (col));
        }

    private:

        value_type  val_;

       // This is not implemented purposely
       //
        ReferenceItem ();

    public:

       // Reference item specific methods.
       //
        inline void delete_reference ()  {

            delete val_;
            val_ = NULL;
        }

        inline DataItem &get_refed_di ()  { return (*val_); }
        inline const DataItem &get_refed_di () const  { return (*val_); }

       // The project() method is similar but not identical to the
       // assignment operator. It is provided only for performance
       // reasons.
       //
       // NOTE:  project() bypasses the dependency mechanism.
       //        In other words the dependencies will not be
       //        triggered as a result of using project().
       //
        inline DataItem &project (const DataItem &rhs)  {

            val_->project(rhs);
            return (*this);
        }
};

// ----------------------------------------------------------------------------

inline std::ostream &operator << (std::ostream &o, const ReferenceItem &r)  {

    return (r.operator << (o));
}

// ----------------------------------------------------------------------------

inline bool
operator == (const ReferenceItem &lhs, const ReferenceItem &rhs) throw ()  {

    return (lhs.compare (rhs) == 0);
}

// ----------------------------------------------------------------------------

inline bool
operator != (const ReferenceItem &lhs, const ReferenceItem &rhs) throw ()  {

    return (lhs.compare (rhs) != 0);
}

// ----------------------------------------------------------------------------

inline bool
operator < (const ReferenceItem &lhs, const ReferenceItem &rhs) throw ()  {

    return (lhs.compare (rhs) < 0);
}

// ----------------------------------------------------------------------------

inline bool
operator <= (const ReferenceItem &lhs, const ReferenceItem &rhs) throw ()  {

    return (lhs.compare (rhs) <= 0);
}

// ----------------------------------------------------------------------------

inline bool
operator > (const ReferenceItem &lhs, const ReferenceItem &rhs) throw ()  {

    return (lhs.compare (rhs) > 0);
}

// ----------------------------------------------------------------------------

inline bool
operator >= (const ReferenceItem &lhs, const ReferenceItem &rhs) throw ()  {

    return (lhs.compare (rhs) >= 0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
