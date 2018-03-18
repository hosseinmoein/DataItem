// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#pragma once

#include <stdexcept>

#include <ContainerItem.h>

// ----------------------------------------------------------------------------

namespace hmdi
{

// This class implements a working system (e.g. an Eco-system)
// that is driven by a dependency/action mechanism. A derivative
// of SystemItem (a specific system) implements and installs
// callback methods which are triggered when a specific item
// of the system changes.
//
// In an eco-system an event will trigger a chain of other
// events until the system comes to an equilibrium. Similarly
// in a SystemItem a change to a DataItem will cause a chain
// of callbacks to be called until there can be no more changes,
// hance equiliberium.
//
class   SystemItem : public ContainerItem  {

    public:

        enum    EXEC_RES  {_success_ = 0, _failure_, _nochange_};

       // There can be multiple dependencies for each field
       //
       // Independent: The column that was just changed
       // Dependent:   The column that will be changed as a result
       //
        typedef EXEC_RES
            (SystemItem::*DpndcyMethod) (const DataItem &independent,
                                         DataItem &dependent,
                                         const void *user_data) const;

       // There can be only one action for each field
       //
        typedef EXEC_RES
            (SystemItem::*ActionMethod) (const DataItem &independent,
                                         const void *user_data) const;

        inline explicit SystemItem (size_type col_size = 16,
                                    size_type row_size = 1,
                                    size_type hdict_size = 79)
            : ContainerItem (col_size, row_size, hdict_size),
              dependency_vec_ (col_size),
              dependency_switch_ (true)  {   }

        inline SystemItem (const SystemItem &that)
            : dependency_vec_ (that.num_cols ()),
              dependency_switch_ (that.dependency_switch_)  {

            *this = that;
        };

        inline SystemItem (SystemItem &&that)
            : dependency_vec_ (that.dependency_vec_),
              dependency_switch_ (that.dependency_switch_)  {
        };

        inline explicit SystemItem (const ContainerItem &con)
            : ContainerItem (con),
              dependency_vec_ (con.num_cols ()),
              dependency_switch_ (true)  {   }

        inline ~SystemItem ()  {   }

        inline SystemItem &operator = (const SystemItem &rh)  {

            if (this != &rh)
                set (rh);
            return (*this);
        }

        inline virtual const char *name_of () const  { return ("SystemItem"); }

        virtual DataItem *add_col (const char *name, DataItem *value);
        virtual size_type add_row (size_type at, size_type like = 0);
        inline virtual size_type add_row ()  {

            return (add_row (num_rows ()));
        }

        size_type add_dependency (size_type independent,
                                  size_type dependent,
                                  DpndcyMethod method,
                                  void *user_data = NULL);
        inline size_type add_dependency (const char *independent,
                                         const char *dependent,
                                         DpndcyMethod method,
                                         void *user_data = NULL)  {

            return (add_dependency (col_name_to_num (independent),
                                    col_name_to_num (dependent),
                                    method, user_data));
        }

        size_type remove_dependency (size_type independent,
                                     size_type dependent);
        inline size_type remove_dependency (const char *independent,
                                            const char *dependent)  {

            return (remove_dependency (col_name_to_num (independent),
                                       col_name_to_num (dependent)));
        }

        size_type add_action (size_type independent,
                              ActionMethod method,
                              void *user_data = NULL);
        inline size_type add_action (const char *independent,
                                     ActionMethod method,
                                     void *user_data = NULL)  {

            return (add_action (col_name_to_num (independent),
                                method,
                                user_data));
        }

        size_type remove_action (size_type independent);
        inline size_type remove_action (const char *independent)  {

            return (remove_action (col_name_to_num (independent)));
        }

        inline void dependency_on ()  { dependency_switch_ = true; }
        inline void dependency_off ()  { dependency_switch_ = false; }
        inline bool dependency_switch () const  { return(dependency_switch_); }

    private:

        class   DependencyNode  {

            public:

                inline DependencyNode (size_type ic,
                                       size_type ec,
                                       SystemItem::DpndcyMethod mthd,
                                       void *ud)
                    : independent_col (ic),
                      dependent_col (ec),
                      user_data (ud)  { method.dmthd = mthd; }
                inline DependencyNode (size_type ic,
                                       SystemItem::ActionMethod mthd,
                                       void *ud)
                    : independent_col (ic),
                      dependent_col (DataItem::npos),
                      user_data (ud)  { method.amthd = mthd; }

               // For searching
               //
                inline DependencyNode (size_type ic, size_type ec)
                    : independent_col (ic),
                      dependent_col (ec),
                      user_data (NULL)  { method.dmthd = NULL; }

                inline bool operator == (const DependencyNode &rhs) const  {

                    return (independent_col == rhs.independent_col &&
                            dependent_col == rhs.dependent_col);
                }

                union   method  {
                    DpndcyMethod    dmthd;
                    ActionMethod    amthd;
                }           method;
                void        *user_data;
                size_type   independent_col;
                size_type   dependent_col;

                inline DependencyNode ()
                    : user_data (NULL),
                      independent_col (DataItem::npos),
                      dependent_col (DataItem::npos)  { method.dmthd = NULL; };
        };

        typedef std::vector<DependencyNode> DepVector;
        typedef std::vector<DepVector>      DepVectorVector;

        DepVectorVector dependency_vec_;
        bool            dependency_switch_;

        static inline DependencyNode *
        contains_ (DepVector &v, const DependencyNode &dn)  {

            for (DepVector::iterator itr = v.begin (); itr != v.end (); ++itr)
                if (*itr == dn)
                    return (&(*itr));

            return (NULL);
        }

        inline void special_add_ (size_type i)  {

            if (dependency_vec_.size () <= i)
                dependency_vec_.resize (i + 1);
            return;
        }

    protected:

        inline virtual DataItem *_clone_hook () const  {

            return (new SystemItem (*this));
        }

        void _set_hook2 (const DataItem &rhs);
        void _report_change (size_type, size_type);

    public:

        virtual DataItem &project (const DataItem &rhs)  {

            throw std::runtime_error ("SystemItem::project() "
                                      "by-passes dependencies "
                                      "and should not be used");
        }
};

// ----------------------------------------------------------------------------

inline bool
operator == (const SystemItem &lhs, const SystemItem &rhs) throw ()  {

    return (lhs.compare (rhs) == 0);
}

// ----------------------------------------------------------------------------

inline bool
operator != (const SystemItem &lhs, const SystemItem &rhs) throw ()  {

    return (lhs.compare (rhs) != 0);
}

// ----------------------------------------------------------------------------

inline bool
operator < (const SystemItem &lhs, const SystemItem &rhs) throw ()  {

    return (lhs.compare (rhs) < 0);
}

// ----------------------------------------------------------------------------

inline bool
operator <= (const SystemItem &lhs, const SystemItem &rhs) throw ()  {

    return (lhs.compare (rhs) <= 0);
}

// ----------------------------------------------------------------------------

inline bool
operator > (const SystemItem &lhs, const SystemItem &rhs) throw ()  {

    return (lhs.compare (rhs) > 0);
}

// ----------------------------------------------------------------------------

inline bool
operator >= (const SystemItem &lhs, const SystemItem &rhs) throw ()  {

    return (lhs.compare (rhs) >= 0);
}

} // namespace hmdi

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
