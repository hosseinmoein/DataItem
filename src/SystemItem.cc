// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <sstream>

#include <SystemItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

static  const   unsigned    int DPNDCY_SIZE = 2;

// ----------------------------------------------------------------------------

void SystemItem::_set_hook2 (const DataItem &rhs)  {

    if (::strcmp (rhs.name_of (), "SystemItem"))  {
        std::ostringstream  err;

        err << "SystemItem::_set_hook2(): A SystemItem cannot be set be a "
            << rhs.name_of ();
        throw std::runtime_error (err.str ().c_str ());
    }

    const   SystemItem  &si = reinterpret_cast<const SystemItem &>(rhs);
    size_type           i = 0;

    for (DepVectorVector::const_iterator citr = si.dependency_vec_.begin ();
         citr != si.dependency_vec_.end (); ++citr, ++i)
        if (! citr->empty ())  {
            special_add_ (i);

            const   size_type   dsize = citr->size ();

            dependency_vec_ [i].reserve (dsize);
            for (size_type j = 0; j < dsize; ++j)  {
                const   DependencyNode  the_node ((*citr) [j]);

                dependency_vec_ [i].push_back (the_node);
            }
        }

    return;
}

// ----------------------------------------------------------------------------

// NOTE: For now we'er going to allow only 1 level of dependencies for
//       ContainerItem columns
//
DataItem *SystemItem::add_col (const char *name, DataItem *value)  {

    DataItem    *ret_value = ContainerItem::add_col (name, value);

    ret_value->change_reporter_ =
        static_cast<ChangeReporter>(&SystemItem::_report_change);
    ret_value->this_ptr_ = this;

    if (dependency_vec_.size () <= num_cols ())
        dependency_vec_.resize (num_cols () + 1);
    return (ret_value);
}

// ----------------------------------------------------------------------------

SystemItem::size_type
SystemItem::add_row (size_type at_row, size_type like_row)  {

    throw std::runtime_error ("SystemItem::add_row(): is not implemented");
}

// ----------------------------------------------------------------------------

void SystemItem::_report_change (size_type row, size_type col)  {

    if (! dependency_vec_ [col].empty () && dependency_switch_)  {
        const   size_type   dsize = dependency_vec_ [col].size ();

/*
        std::cout << "_report_change(): DSIZE: " << dsize << std::endl;
*/

        for (size_type i = 0; i < dsize; ++i)  {
            const   DependencyNode  &dn = dependency_vec_ [col] [i];

            if (dn.independent_col != DataItem::npos)
               // Is this a dependency or an action? If it doesn't
               // have an dependent DataItem then it is an action.
               //
                if (dn.dependent_col == DataItem::npos)  {
                    // const   ActionMethod    fp = dn.method.amthd;

/*
                    std::cout << "ACTION: "
                              << i << ", " << col << " -- "
                              << col_name (col) << "<"
                              << (*this) (row, col).name_of () << ": "
                              << (*this) (row, col).to_stdstring () << ">"
                              << std::endl;
*/
 
                    (this->*(dn.method.amthd))((*this)(row, col), dn.user_data);
                }
                else  {
                    DataItem    &dependent = (*this) (row, dn.dependent_col);

                    if (! dependent.is_inside_dependency_)  {
                        ((*this) (row, col)).is_inside_dependency_ = true;

                        // const   DpndcyMethod    fp = dn.method.dmthd;

/*
                        std::cout << "DEPENDENCY: "
                                  << i << ", " << col << " -- "
                                  << col_name (col) << "<"
                                  << (*this) (row, col).name_of () << ": "
                                  << (*this) (row, col).to_stdstring () << ">"
                                  << " To "
                                  << col_name(dn.dependent_col) << "<"
                                  << dependent.name_of() << ">"
                                  << std::endl;
*/
 
                        (this->*(dn.method.dmthd)) ((*this) (row, col),
                                                    dependent,
                                                    dn.user_data);

                        ((*this) (row, col)).is_inside_dependency_ = false;
                    }
                }
        }
    }

   // NOTE: This is a duplication of logic in DataItem::set().
   //       Maybe we can be smarter and put them in one place.
   //
    if (change_reporter_ && this_ptr_ && ! is_inside_dependency_)
        (this_ptr_->*change_reporter_) (0, col_);

    return;
}

// ----------------------------------------------------------------------------

SystemItem::size_type
SystemItem::add_dependency (size_type independent,
                            size_type dependent,
                            DpndcyMethod method,
                            void *user_data)  {

    if (! is_valid_col_num (independent) || ! is_valid_col_num (dependent))  {
        std::cerr << "ERROR: SystemItem::add_dependency(): "
                  << "Bad Column numbers: " << independent
                  << ", " << dependent << std::endl;
        return (DataItem::npos);
    }

    const   DependencyNode  dn (independent, dependent, method, user_data);

    special_add_ (independent);

    DependencyNode  *slug;

    if (slug = contains_ (dependency_vec_ [independent], dn))  {
        slug->method = dn.method;
        slug->user_data = dn.user_data;
    }
    else  {
        if (dependency_vec_ [independent].empty ())
            dependency_vec_ [independent].reserve (DPNDCY_SIZE);
        dependency_vec_ [independent].push_back (dn);
    }

    return (1);
}

// ----------------------------------------------------------------------------

// NOTE: The following is almost a duplication of code in add_dependency()
//
SystemItem::size_type
SystemItem::add_action (size_type independent,
                        ActionMethod method,
                        void *user_data)  {

    if (! is_valid_col_num (independent))  {
        std::cerr << "ERROR: SystemItem::add_action(): "
                  << "Bad Column number: " << independent << std::endl;
        return (DataItem::npos);
    }

    const   DependencyNode  dn (independent, method, user_data);

    special_add_ (independent);

    DependencyNode  *slug;

    if (slug = contains_ (dependency_vec_ [independent], dn))  {
        slug->method = dn.method;
        slug->user_data = dn.user_data;
    }
    else  {
        if (dependency_vec_ [independent].empty ())
            dependency_vec_ [independent].reserve (DPNDCY_SIZE);
        dependency_vec_ [independent].push_back (dn);
    }

    return (1);
}

// ----------------------------------------------------------------------------

SystemItem::size_type
SystemItem::remove_dependency (size_type independent, size_type dependent)  {

    DependencyNode  dn (independent, dependent);

    if (dependency_vec_.size () > independent &&
        ! dependency_vec_ [independent].empty ())  {
        DepVector   &fs = dependency_vec_ [independent];

        for (DepVector::iterator itr = fs.begin ();
             itr != fs.end (); ++itr)  {
            if (*itr == dn)  {
                fs.erase (itr);
                return (1);
            }
        }
    }

    return (0);
}

// ----------------------------------------------------------------------------

// NOTE: The following is almost a duplication of code in remove_dependency()
//
SystemItem::size_type SystemItem::remove_action (size_type independent)  {

    DependencyNode  dn (independent, DataItem::npos);

    if (dependency_vec_.size () > independent &&
        ! dependency_vec_ [independent].empty ())  {
        DepVector   &fs = dependency_vec_ [independent];

        for (DepVector::iterator itr = fs.begin ();
             itr != fs.end (); ++itr)  {
            if (*itr == dn)  {
                fs.erase (itr);
                return (1);
            }
        }
    }

    return (0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
