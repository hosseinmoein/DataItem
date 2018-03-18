// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <math.h>

#include <DMScu_FixedSizeString.h>

#include <ContainerItem.h>
#include <ShortItem.h>
#include <FloatItem.h>
#include <BinaryItem.h>
#include <DateItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

NullItem    ContainerItem::NullValue;

// ----------------------------------------------------------------------------

bool ContainerItem::_set_hook (const DataItem &rhs)  {

    if (_is_same_shape (rhs))  {
        const   size_type   rowsz = num_rows ();
        const   size_type   colsz = num_cols ();

        for (size_type row = 0; row < rowsz; ++row)
            for (size_type col = 0; col < colsz; ++col)
                (*this) (row, col) = rhs (row, col);
    }
    else  {
        _clear ();

        const   ContainerItem   &con =
            static_cast<const ContainerItem &>(rhs);
        const   size_type       column_size = con.columns_.size ();
        const   size_type       row_size =
            con.columns_.empty () ? 0 : con.columns_.front ().size ();

        columns_.reserve (column_size);
        for (size_type i = 0; i < column_size; ++i)  {
            columns_.push_back (DataItemPtrVector());
            columns_.back ().reserve (row_size);

            const   size_type   columns_i_size = con.columns_ [i].size ();

            for (size_type j = 0; j < columns_i_size; ++j)  {
                columns_ [i].push_back ((con.columns_ [i] [j])->clone ());
                columns_ [i].back ()->col_ = (con.columns_ [i] [j])->col_;
                columns_ [i].back ()->this_ptr_ = this;
            }
        }

        row_size_ = columns_.empty () ? 0 : columns_.front ().size ();

        size_type   col = 0;

        column_names_.reserve (column_size);
        for (ColNameVector::const_iterator itr = con.column_names_.begin ();
             itr != con.column_names_.end (); ++itr, ++col)  {
            char    *ptr = new char [::strlen (*itr) + 1];

            ::strcpy (ptr, *itr);
            column_names_.push_back (ptr);
            col_name_dict_.insert (CIHashDic::value_type (ptr, col));
        }

        _set_hook2 (rhs);
    }

    return (true);
}

// ----------------------------------------------------------------------------

ContainerItem::size_type
ContainerItem::add_row (size_type at_row, size_type like_row)  {

    if (columns_.empty ())
        throw std::runtime_error ("ContainerItem::add_row(): "
                                  "This ContainerItem has not been initialized "
                                  "yet. It has no defined column(s).");

   // I am not very proud of this logic, but it works for now. I don't have
   // time to do a major redesign, at this time.
   //
    if (like_row == 0 && row_size_ == 0 && columns_.front ().size () == 1)
        return (++row_size_);

    if (! is_valid_row_num (like_row))
        throw std::runtime_error ("ContainerItem::add_row(): Bad row number");

    const   size_type   columns_size = num_cols ();

    for (size_type col = 0; col < columns_size; col++)  {
        const   DataItem    *bp = get (like_row, col);
        DataItemPtrVector   &fstptr =
            const_cast<DataItemPtrVector &>(_col_to_vector (col));

        fstptr.insert (fstptr.begin () + at_row, bp->clone ());
    }

    return (++row_size_);
}

// ----------------------------------------------------------------------------

ContainerItem::size_type ContainerItem::remove_row (size_type at_row)  {

    if (! is_valid_row_num (at_row))
      return (npos);

    const   size_type   columns_size = num_cols ();

    for (size_type col = 0; col < columns_size; ++col)  {
        DataItemPtrVector   &fstptr =
            const_cast<DataItemPtrVector &>(_col_to_vector (col));

        fstptr.erase (fstptr [at_row]);
    }

    if (row_size_ > 0)
        row_size_ -= 1;

    return (row_size_);
}

// ----------------------------------------------------------------------------

void ContainerItem::_clear ()  {

   // clear contents of all columns
   //
    for (ColumnsVector::iterator col_iter = columns_.begin ();
         col_iter != columns_.end (); ++col_iter)
        col_iter->clear ();

    columns_.clear ();  // clear out columns themselves
    row_size_ = 0;

    for (ColNameVector::iterator itr = column_names_.begin ();
         itr != column_names_.end (); ++itr)
        delete[] *itr;

    column_names_.clear ();  // ...and their names
    col_name_dict_.clear ();
    _clear_hook ();

    return;
}

// ----------------------------------------------------------------------------

// This method can be called by general public to add a column
// to the container. Also look at the specific add<Type>Col()
// methods in the header.
//
DataItem *ContainerItem::add_col (const char *name, DataItem *value)  {

    if (col_name_dict_.find (name) != col_name_dict_.end ())  {
        DMScu_FixedSizeString<511>  err;

        err.printf ("ContainerItem::add_col(): "
                    "There is already a column named '%s'",
                    name);
        throw std::runtime_error (err.c_str ());
    }

    const   size_type   rows_to_reserve = num_rows ();

    columns_.push_back (DataItemPtrVector());
    columns_.back ().reserve (rows_to_reserve ? rows_to_reserve : 1);

    const   size_type   col = num_cols () - 1;

   // Add the value to column "col" for each row
   //
   // If value is null, it means that a mere resize
   // was meant and _append_to_col() will be used later.
   // This is mostly for SybaseManager's sake. Do not
   // use this null value, unless you know what you are
   // doing.
   //
    if (value)  {
        const   size_type rows = num_rows ();

        columns_ [col].push_back (value);
        columns_ [col].back ()->col_ = col;
        for (size_type i = 1; i < rows; ++i)  {
            columns_ [col].push_back (value->clone ());
            columns_ [col].back ()->col_ = col;
        }
    }

   // Add the new name to dictionary
   //
    char    *ptr = new char [::strlen (name) + 1];

    ::strcpy (ptr, name);
    column_names_.push_back (ptr);
    col_name_dict_.insert (CIHashDic::value_type (ptr, col));

    return (columns_ [col].front ());
}

// ----------------------------------------------------------------------------

ContainerItem::size_type ContainerItem::remove_col (size_type col)  {

    if (! is_valid_col_num (col))
        return (npos);

    const   char    *const  colname = col_name (col);

   // remove the entry from hash dictionary
   //
    const   bool    did_erase = col_name_dict_.erase (colname);

    if (! did_erase)  {
        DMScu_FixedSizeString<511>  err;

        err.printf ("ContainerItem::remove_col(): "
                    "Unable to remove column from dictionary: '%s'",
                    colname);
        throw std::runtime_error (err.c_str ());
    }

   // Take out the column itself.
   //
    columns_.erase (columns_.begin () + col);

   // delete the name from name list
   //
    delete[] *(column_names_.begin () + col);
    column_names_.erase (column_names_.begin () + col);

    return (1);
}

// ----------------------------------------------------------------------------

bool ContainerItem::_is_same_shape (const DataItem &that) const  {

    if (that.type_api () == containerType)  {
        const   size_type   colsz = num_cols ();

        if (colsz != that.num_cols ())
            return (false);
        else if (num_rows () != that.num_rows ())
            return (false);
        else
            for (size_type col = 0; col < colsz; ++col)  {
                if ((*this) [col].type_api () != that [col].type_api ())
                    return (false);
                if (::strcmp (col_name (col), that.col_name (col)))
                    return (false);
            }

        return (true);
    }

    DMScu_FixedSizeString<127>  err;

    err.printf ("ContainerItem::_is_same_shape(): "
                "Cannot compare ContainerItem to a '%s'",
                DataItem::type_to_str (that.type_api ()));
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

int ContainerItem::_compare_hook (const DataItem &that) const  {

   // NOTE: Since we always return 1, this method cannot be used to do any
   //       meaningfull ranking
   //
    if (! _is_same_shape (that))
        return (1);

    const   size_type   rowsz = num_rows ();
    const   size_type   colsz = num_cols ();

    for (size_type row = 0; row < rowsz; ++row)
        for (size_type col = 0; col < colsz; ++col)  {
            const   int com = (*this)(row, col)._compare_hook(that (row, col));

            if (com)
                return (com);
        }

    return (0);
}

// ----------------------------------------------------------------------------

// The magic key is used to identify a serialized ContainerItem.
//
static  const   ContainerItem::size_type    MAGIC_KEY = 0x74F2D647;

// ----------------------------------------------------------------------------

// A byte to specify the type (i.e. containerType) +
// A int value to specify the number of rows +
// A int value to specify the number of columns +
// For each column:
//    A byte to specify the length of the column name +
//    The column name itself +
// For each row and column the sum of item lengths
//
ContainerItem::size_type ContainerItem::bin_size () const  {

    if (columns_.empty ())
        throw std::runtime_error ("ContainerItem::bin_size(): "
                                  "This ContainerItem has not been initialized "
                                  "yet. It has no defined column(s).");

    const   size_type   colsz = num_cols ();
    size_type           len = 0;

    len += sizeof (TypeByte);        // type_api()
    len += sizeof (size_type);       // MAGIC_KEY
    len += sizeof (size_type) * 2;   // number of rows and columns

    const  size_type    rowsz = columns_.front ().size ();

    for (size_type row = 0; row < rowsz; ++row)
        for (size_type col = 0; col < colsz; ++col)  {
            if (! row)  {
                len += sizeof (unsigned char);          // column name length
                len += ::strlen (column_names_ [col]);  // column name itself
            }
            len +=  ((*this) (row, col)).bin_size ();
        }

    return (len);
}

// ----------------------------------------------------------------------------

void ContainerItem::bin_copy (unsigned char *buffer) const  {

    if (columns_.empty ())
        throw std::runtime_error ("ContainerItem::bin_copy(): "
                                  "This ContainerItem has not been initialized "
                                  "yet. It has no defined column(s).");

    unsigned    char                *ptr = buffer;
    const   size_type   colsz = num_cols ();
    const   size_type   rowsz = columns_.front ().size ();

    *ptr = static_cast<TypeByte> (type_api ());
    ptr += sizeof (TypeByte);

    memcpy (ptr, &MAGIC_KEY, sizeof (size_type));
    ptr += sizeof (size_type);

    memcpy (ptr, &rowsz, sizeof (size_type));
    ptr += sizeof (size_type);

    memcpy (ptr, &colsz, sizeof (size_type));
    ptr += sizeof (size_type);

    for (size_type row = 0; row < rowsz; ++row)
        for (size_type col = 0; col < colsz; ++col)  {

           // If this is the first row, store the
           // column names as we go along. This is done,
           // so it would be easier to reconstruct later
           //
            if (! row)  {

               // NOTE: Accroding to this space saving strategy column
               //       names cannot be longer then 256 characters
               //
                *ptr =
                    static_cast<unsigned char>(::strlen (column_names_ [col]));
                ptr += sizeof (unsigned char);
                ::memcpy (ptr,
                          column_names_ [col],
                          ::strlen (column_names_ [col]));
                ptr += ::strlen (column_names_ [col]);
            }
            ((*this) (row, col)).bin_copy (ptr);
            ptr += ((*this) (row, col)).bin_size ();
        }

    return;
}

// ----------------------------------------------------------------------------

int ContainerItem::bin_construct (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<DataItem::types>(*buffer);

    if (typ == containerType)  {
        const   TypeByte    *ptr = static_cast<const TypeByte *>(buffer);

        ptr += sizeof (TypeByte);

        size_type   mk = 0;

        memcpy (&mk, ptr, sizeof (size_type));
        if (mk == MAGIC_KEY)  {
            ptr += sizeof (size_type);

            _clear ();

            size_type   rowsz = 0;
            size_type   colsz   = 0;

            memcpy (&rowsz, ptr, sizeof (size_type));
            ptr += sizeof (size_type);
            memcpy (&colsz, ptr, sizeof (size_type));
            ptr += sizeof (size_type);

            char    colname [1024];

            column_names_.reserve (colsz);
            columns_.reserve (colsz);
            for (size_type row = 0; row < rowsz; ++row)
                for (size_type col = 0; col < colsz; ++col)  {
                    if (row == 0)  {
                        const   unsigned    char    namelen = *ptr;

                        ptr += sizeof (unsigned char);
                        ::strncpy (colname,
                                   reinterpret_cast<const char *>(ptr),
                                   static_cast<size_type>(namelen));
                        colname [static_cast<size_type>(namelen)] = 0;
                        ptr += static_cast<size_type>(namelen);
                    }

                    DataItem    *di = construct_data_item_ (ptr);

                   // If this is the first row, simply add a new column
                   // otherwise append to the columns at the specified row
                   //
                    if (row == 0)  {
                        add_col (colname, di);
                        columns_ [col].reserve (rowsz);
                    }
                    else
                        _append_to_col (col, di);

                    ptr += di->bin_size ();
                }

            row_size_ = columns_.front ().size ();

            return (0);
        }

        DMScu_FixedSizeString<127>  err;

        err.printf ("ContainerItem::bin_construct(): "
                    "Magic Key mismatch: %u != %u",
                    mk, MAGIC_KEY);
        throw std::runtime_error (err.c_str ());
    }

    DMScu_FixedSizeString<127>  err;

    err.printf ("ContainerItem::bin_construct(): "
                "Type must be 'container' but it is '%s'",
                DataItem::type_to_str (typ));
    throw std::runtime_error (err.c_str ());
}

// ----------------------------------------------------------------------------

DataItem &ContainerItem::project (const DataItem &rhs)  {

   // If rhs is not a ContainerItem, silently do nothing
   //
    if (rhs.type_api () != containerType)
        return (*this);

    const   size_type   lhs_rows = num_rows ();
    const   size_type   rhs_rows = rhs.num_rows ();
    const   size_type   rhs_cols = rhs.num_cols ();
    size_type           row;

   // If we have more rows then rhs, drop'em.
   //
    for (row = lhs_rows; row > rhs_rows; --row)
        remove_row (num_rows () - 1);

   // If we have less rows then rhs, add more.
   //
    for (row = lhs_rows; row < rhs_rows; ++row)
        add_row ();

    for (row = 0; row < rhs_rows; ++row)
        for (size_type col = 0; col < rhs_cols; ++col)
            ((*this) (row, rhs.col_name (col))).project (rhs (row, col));

    return (*this);
}

// ----------------------------------------------------------------------------

// - static
//
DataItem *ContainerItem::construct_data_item_ (const unsigned char *buffer)  {

    const   DataItem::types typ = static_cast<const DataItem::types>(*buffer);
    DataItem                *ret = NULL;

    switch (typ)  {
        case intType:
            ret = new IntItem;
            break;
        case longlongType:
            ret = new LongLongItem;
            break;
        case shortType:
            ret = new ShortItem;
            break;
        case ucharType:
            ret = new UCharItem;
            break;
        case floatType:
            ret = new FloatItem;
            break;
        case doubleType:
            ret = new DoubleItem;
            break;
        case binaryType:
            ret = new BinaryItem;
            break;
        case dateType:
            ret = new DateItem;
            break;
        case stringType:
            ret = new StringItem;
            break;
        case nullType:
            ret = new NullItem;
            break;
        case containerType:
            ret = new ContainerItem;
            break;

        case referenceType:
        default:  {
            DMScu_FixedSizeString<127>  err;

            err.printf ("ContainerItem::construct_data_item_ (): "
                        "Unknown DataItem type: %d",
                        typ);
            throw std::runtime_error (err.c_str ());
        }
    }

    if (ret)
        ret->bin_construct (buffer);

    return (ret);
}

// ----------------------------------------------------------------------------

void ShowC (const DataItem *dbp, std::ostream &os, const char *prefix)  {

    if (dbp->type_api () != DataItem::containerType)
        os << prefix
           << "<" << DataItem::type_to_str (dbp->type_api ()) << "> "
           << dbp->to_stdstring () << '\n';
    else  {
        os << "{" << std::endl;

        for (DataItem::size_type row = 0; row < dbp->num_rows (); ++row)  {
            os << '\n' << prefix << "ROW   " << row << "\n\n";

            for (DataItem::size_type col = 0; col < dbp->num_cols (); ++col)  {
                const   DataItem    &diref = (*dbp) (row, col);

                if (diref.type_api () == DataItem::containerType)  {
                    os << prefix << dbp->col_name (col) << "  ";

                    DMScu_FixedSizeString<1023>  this_prefix = prefix;

                    this_prefix += "  ";
                    ShowC (&diref, os, this_prefix.c_str ());
                }
                else  {
                    os << prefix << dbp->col_name (col)
                       << "<" << DataItem::type_to_str (diref.type_api ())
                       << ">" << " = "
                       << diref.to_stdstring () << '\n';
                }
            }
        }

        os << prefix << "}" << std::endl;
    }

    return;
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
