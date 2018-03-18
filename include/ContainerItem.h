// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <iostream>
#include <unordered_map>

#include <DMScu_PtrVector.h>

#include <DataItem.h>
#include <DoubleItem.h>
#include <IntItem.h>
#include <LongLongItem.h>
#include <StringItem.h>
#include <UCharItem.h>
#include <NullItem.h>
#include <ReferenceItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

// This function can be used in a debugger
// or elsewhere to view the contents of a
// container.
//
void ShowC (const DataItem *, std::ostream & = std::cout, const char * = "");

// ----------------------------------------------------------------------------

// This class represents a tabular form of
// DataItem objects. All rows have the same number and
// type of columns.
// Each column must be named. Each column can be accessed
// either by its name or its index number.
//
class   ContainerItem : public DataItem  {

    public:

        inline ContainerItem (size_type col_size = 16,
                              size_type row_size = 16,
                              size_type hdict_size = 79)
            : col_name_dict_ (hdict_size),
              columns_ (),
              column_names_ (),
              row_size_ (0)  {

            column_names_.reserve (col_size);
            columns_.reserve (col_size);
        }
        inline ContainerItem (const ContainerItem &that)
            : DataItem (that),
              col_name_dict_ (that.col_name_dict_.size ()),
              columns_ (),
              column_names_ ()  {

            is_inside_dependency_ = true;
            *this = that;
            is_inside_dependency_ = false;
        }

        inline ContainerItem (ContainerItem &&that)
            : DataItem (that),
              col_name_dict_ (that.col_name_dict_),
              columns_ (that.columns_),
              column_names_ (that.column_names_)  {

            is_inside_dependency_ = true;
            *this = that;
            is_inside_dependency_ = false;
        }

        inline virtual ~ContainerItem ()  {

            for (ColNameVector::iterator itr = column_names_.begin ();
                 itr != column_names_.end (); ++itr)
                delete[] *itr;
        }

       // Instead of returning a NULL pointer, the following
       // null item is returned by some methods to indicate
       // lack of data.
       //
        static  NullItem    NullValue;

    protected:

        inline virtual DataItem *_clone_hook () const  {

            return (new ContainerItem (*this));
        }
        bool _set_hook (const DataItem &rhs);
        bool _is_same_shape (const DataItem &) const;
        int _compare_hook (const DataItem &) const;

    private:

        class   CI_eq_func
            : public std::binary_function<const char *, const char *, bool>  {

            public:

                inline CI_eq_func ()  {   }

                inline result_type
                operator () (const first_argument_type &lhs,
                             const second_argument_type &rhs) const  {

                    return (! ::strcmp (lhs, rhs));
                }
        };

	typedef std::unordered_map<std::string, size_type> CIHashDic;

    public:

        inline ContainerItem &operator = (const ContainerItem &rh)  {

            if (this != &rh)
                set (rh);
            return (*this);
        }

        inline bool contains (const char *col) const  {

            return (col_name_dict_.find (col) != col_name_dict_.end ());
        }

        inline bool is_valid_col_num (size_type i) const  {

            return (i < num_cols ());
        }

        inline bool is_valid_row_num (size_type i) const  {

            return (i < num_rows ());
        }

        inline size_type num_cols () const  { return (columns_.size ()); }

       // The reason for this weird behavior is that a ContainerItem,
       // with atleast one column, automatically has one row of data.
       // But if you put the ContainerItem inside a loop to add rows to it
       // it will end up with one extra row.
       //
        inline size_type num_rows () const  {

            if (columns_.empty ())
                return (0);
            else if (row_size_ == 0U && columns_.front ().size () == 1)
                return (1);
            else
                return (row_size_);
        }

        inline const char *const col_name (size_type col) const  {

            return (is_valid_col_num (col) ? column_names_ [col] : NULL);
        }

        inline size_type col_name_to_num (const char *col) const  {

            const   CIHashDic::const_iterator   citr =
                col_name_dict_.find (col);

            return (citr != col_name_dict_.end () ? citr->second : npos);
        }

        inline const DataItem *get (size_type row, size_type col) const  {

            // if (! is_valid_col_num (col) || ! is_valid_row_num (row))
            //     return (&NullValue);

            return (_base_ptr (row, _col_to_vector (col)));
        }
        inline const DataItem *get (size_type row, const char *col) const  {

            const   size_type   colnum = col_name_to_num (col);

            return (colnum != npos ? get (row, colnum) : &NullValue);
        }

        virtual size_type add_row (size_type at, size_type like = 0);
        inline virtual size_type add_row ()  { return (add_row (num_rows())); }

        size_type remove_row (size_type at);

        virtual DataItem *add_col (const char *name, DataItem *value);
        inline DataItem *add_double_col (const char *name, double value = 0)  {

            return (add_col (name, new DoubleItem (value)));
        }
        inline DataItem *add_int_col (const char *name, int value = 0)  {

            return (add_col (name, new IntItem (value)));
        }
        inline DataItem *
        add_longlong_col (const char *name, long long int value = 0)  {

            return (add_col (name, new LongLongItem (value)));
        }
        inline DataItem *
        add_string_col (const char *name, const char *value = "")  {

            return (add_col (name, new StringItem (value)));
        }
        inline DataItem *
        add_uchar_col (const char *name, unsigned char value = 0)  {

            return (add_col (name, new UCharItem (value)));
        }
        inline DataItem *add_container_col (const char *name)  {

            return (add_col (name, new ContainerItem ()));
        }
        inline DataItem *
        add_reference_col (const char *name, DataItem *value)  {

            return (add_col (name, new ReferenceItem (value)));
        }

        virtual size_type remove_col (size_type col);
        inline size_type remove_col (const char *name)  {

            return (remove_col (col_name_to_num (name)));
        }

       // DataItem methods
       //
        inline virtual const char *name_of () const  {

            return ("ContainerItem");
        }
        inline types type_api () const  { return (containerType); }

       // Serialize and de-serialize methods
       //
        size_type bin_size () const;
        void bin_copy (unsigned char *buffer) const;
        int bin_construct (const unsigned char *buffer);

        inline std::string to_stdstring () const  { return (""); }

        inline bool is_container () const  { return (true); }
        inline virtual std::ostream &operator << (std::ostream &os) const  {

            ShowC (this, os);
            return (os);
        }

        inline void swap (ContainerItem &that)  {

            col_name_dict_.swap (that.col_name_dict_);
            column_names_.swap (that.column_names_);
            columns_.swap (that.columns_);
            row_size_ ^= that.row_size_ ^= row_size_ ^= that.row_size_;
            _swap_hook (that);
            return;
        }

    protected:

        virtual void _swap_hook (ContainerItem &)  {   }

        typedef DMScu_PtrVector<
            DataItem,
            DMScu_CloneCopier<DataItem> >   DataItemPtrVector;

        void _clear ();

       // Does the class specific cleanups in derived classes
       //
        inline virtual void _clear_hook ()  {   }

       // Does the specific settings for derived classes
       //
        inline virtual void _set_hook2 (const DataItem &)  {   }

       // Append a row item to the end of a column.
       // NOTE: This interface is provided specifically for SybaseManager
       //       and ContainerItem's internal use and should not be used in
       //       any other way. It depends on the fact that the caller will
       //       go through all the columns, repeatedly calling this method.
       //
        inline bool _append_to_col (size_type col, DataItem *v)  {

            columns_ [col].push_back (v);
            if (col == 0)
                row_size_ = columns_.front ().size ();

            return (true);
        }

        inline const DataItemPtrVector &_col_to_vector (size_type col) const  {

            return (columns_ [col]);
        }

        static inline const DataItem *
        _base_ptr (size_type row, const DataItemPtrVector &ro)  {

            return (ro [row]);
        }

    private:

        static DataItem *construct_data_item_ (const unsigned char *);

        typedef std::vector<const char *>       ColNameVector;
        typedef std::vector<DataItemPtrVector>  ColumnsVector;

       // Maps names to col number
       //
        CIHashDic           col_name_dict_;

       // Names of each column
       //
        ColNameVector       column_names_;

       // A column is a collection of row elements
       //
        ColumnsVector       columns_;
        mutable size_type   row_size_;

    public:

       // The project() method is similar but not identical to the
       // assignment operator. It is provided only for performance
       // reasons.
       //
       // NOTE:  project() bypasses the dependency mechanism.
       //        In other words the dependencies will not be
       //        triggered as a result of using project().
       //
        virtual DataItem &project (const DataItem &rhs);
};

// ----------------------------------------------------------------------------

inline bool
operator == (const ContainerItem &lhs, const ContainerItem &rhs) throw ()  {

    return (lhs.compare (rhs) == 0);
}

// ----------------------------------------------------------------------------

inline bool
operator != (const ContainerItem &lhs, const ContainerItem &rhs) throw ()  {

    return (lhs.compare (rhs) != 0);
}

// ----------------------------------------------------------------------------

inline bool
operator < (const ContainerItem &lhs, const ContainerItem &rhs) throw ()  {

    return (lhs.compare (rhs) < 0);
}

// ----------------------------------------------------------------------------

inline bool
operator <= (const ContainerItem &lhs, const ContainerItem &rhs) throw ()  {

    return (lhs.compare (rhs) <= 0);
}

// ----------------------------------------------------------------------------

inline bool
operator > (const ContainerItem &lhs, const ContainerItem &rhs) throw ()  {

    return (lhs.compare (rhs) > 0);
}

// ----------------------------------------------------------------------------

inline bool
operator >= (const ContainerItem &lhs, const ContainerItem &rhs) throw ()  {

    return (lhs.compare (rhs) >= 0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
