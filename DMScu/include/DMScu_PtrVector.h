// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#ifndef _INCLUDED_DMScu_PtrVector_h
#define _INCLUDED_DMScu_PtrVector_h 0

#include <cstdlib>
#include <vector>

// ----------------------------------------------------------------------------

template<class cu_Data>
class   DMScu_SliceCopier;

// ----------------------------------------------------------------------------

template <class cu_Data, class cu_COPIER = DMScu_SliceCopier<cu_Data> >
class   DMScu_PtrVector  {

    private:

        typedef std::vector<cu_Data *>  vector_type;

        vector_type vector_;

    public:

        typedef cu_Data                                 data_type;
        typedef cu_COPIER                               copier_type;
        typedef typename vector_type::value_type        value_type;
        typedef typename vector_type::pointer           pointer;
        typedef typename vector_type::reference         reference;
        typedef typename vector_type::const_reference   const_reference;
        typedef unsigned int                            size_type;
        typedef typename vector_type::difference_type   difference_type;
        typedef typename vector_type::iterator          iterator;
        typedef typename vector_type::const_iterator    const_iterator;
        typedef typename vector_type::reverse_iterator  reverse_iterator;
        typedef typename vector_type::const_reverse_iterator
                                                        const_reverse_iterator;

        inline iterator begin () throw ()  { return (vector_.begin ()); }
        inline iterator end () throw ()  { return (vector_.end ()); }
        inline const_iterator
        begin () const throw ()  { return (vector_.begin ()); }
        inline const_iterator
        end () const throw ()  { return (vector_.end ()); }
        inline reverse_iterator
        rbegin () throw ()  { return (vector_.rbegin ()); }
        inline reverse_iterator rend () throw ()  { return (vector_.rend ()); }
        inline const_reverse_iterator
        rbegin () const throw ()  { return (vector_.rbegin ()); }
        inline const_reverse_iterator
        rend () const throw ()  { return (vector_.rend ()); }

        inline size_type size () const throw ()  { return (vector_.size ()); }
        inline size_type
        max_size () const throw ()  { return (vector_.max_size ()); }
        inline size_type
        capacity () const throw ()  { return (vector_.capacity ()); }

        inline bool empty () const throw ()  { return (vector_.empty ()); }

        inline reference
        operator[] (size_type n) throw ()  { return (vector_ [n]); }
        inline const_reference
        operator[] (size_type n) const throw ()  { return (vector_ [n]); }

        inline void reserve (size_type n) throw ()  { vector_.reserve (n); }

        inline reference front () throw ()  { return (vector_.front ()); }
        inline const_reference
        front () const throw ()  { return (vector_.front ()); }
        inline reference back () throw ()  { return (vector_.back ()); }
        inline const_reference
        back () const throw ()  { return (vector_.back ()); }

        inline void
        push_back (const value_type &e) throw ()  { vector_.push_back (e); }
        inline void pop_back () throw ()  { vector_.pop_back (); }

        inline void
        swap (DMScu_PtrVector &that) throw ()  { vector_.swap (that.vector_); }

        inline iterator insert (iterator pos, const value_type &x) throw ()  {

            return (vector_.insert (pos, x));
        }
        inline iterator
        insert (iterator pos, size_type n, const value_type &x) throw ()  {

            return (vector_.insert (pos, n, x));
        }

        inline friend bool operator == (const DMScu_PtrVector &lhs,
                                        const DMScu_PtrVector &rhs) throw ()  {

            return (lhs.vector_ == rhs.vector_);
        }
        inline friend bool operator < (const DMScu_PtrVector &lhs,
                                       const DMScu_PtrVector &rhs) throw ()  {

            return (lhs.vector_ < rhs.vector_);
        }

        inline DMScu_PtrVector () throw () : vector_ ()  {   }
        inline DMScu_PtrVector (size_type n) throw () : vector_ (n)  {   }
        inline DMScu_PtrVector (const DMScu_PtrVector &that)
            throw () : vector_ ()  {

            (*this) = that;
        }

        ~DMScu_PtrVector () throw ();

       // NOTE: Pay attention to the role of Copier
       //
        inline DMScu_PtrVector &operator = (const DMScu_PtrVector &rhs)  {

            const   copier_type copier;

            copier.copy (*this, rhs);
            return (*this);
        }

        iterator erase (iterator pos) throw ();
        iterator erase (iterator fisrt, iterator last) throw ();
        inline void clear () throw ()  { erase (begin (), end ()); }

        inline void resize (size_type n) throw ()  {

            const   size_type   current_size = vector_.size ();

            if (n < current_size)
                for (size_type idx = n; idx < current_size; ++idx)
                    delete vector_ [idx];

            vector_.resize (n, NULL);
        }

       // The followings are some convenient methods that are not a part
       // of STL.
       //
        bool erase (const value_type &x) throw ();
        iterator find (const data_type &x) throw ();
        const_iterator find (const data_type &x) const throw ();
};

// ----------------------------------------------------------------------------

//
// NOTE: Since this vector owns its collection of pointers (the destructor
//       calls delete on its collection), we cannot do a shallow copy.
//
//       The following two classes both do deep copies.
//

template <class cu_Data>
class   DMScu_SliceCopier  {

    public:

        typedef DMScu_PtrVector<cu_Data, DMScu_SliceCopier> VectorType;

        inline DMScu_SliceCopier () throw ()  {   }

       // NOTE: The following method does a slice copy.
       //       This method will not compile, if DMScu_PtrVector is
       //       parameterized off of an abstract type. In other words if you
       //       parameterize the DMScu_PtrVector off of an abstract type, make
       //       sure you don't call this method or will get nowhere fast.
       //
        inline void
        copy (VectorType &lhs, const VectorType &rhs) const throw ()  {

            VectorType  tmp_vec;

            tmp_vec.reserve (rhs.size ());
            for (typename VectorType::const_iterator itr = rhs.begin ();
                 itr != rhs.end (); ++itr)
                tmp_vec.push_back (new typename VectorType::data_type (**itr));

            lhs.swap (tmp_vec);
            return;
        }
};

// ----------------------------------------------------------------------------

template <class cu_Data>
class   DMScu_CloneCopier  {

    public:

        typedef DMScu_PtrVector<cu_Data, DMScu_CloneCopier> VectorType;

        inline DMScu_CloneCopier () throw ()  {   }

       // NOTE: The following method copies by the way of cloning.
       //       If you are parameterizing your vector off of an abstract calss,
       //       then this is for you. of course your template parameter must
       //       have a method called clone().
       //
        inline void
        copy (VectorType &lhs, const VectorType &rhs) const throw ()  {

            VectorType  tmp_vec;

            tmp_vec.reserve (rhs.size ());
            for (typename VectorType::const_iterator itr = rhs.begin ();
                 itr != rhs.end (); ++itr)
                tmp_vec.push_back ((*itr)->clone ());

            lhs.swap (tmp_vec);
            return;
        }
};

// ----------------------------------------------------------------------------

#  ifdef DMS_INCLUDE_SOURCE
#    include <DMScu_PtrVector.tcc>
#  endif // DMS_INCLUDE_SOURCE

// ----------------------------------------------------------------------------

#undef _INCLUDED_DMScu_PtrVector_h
#define _INCLUDED_DMScu_PtrVector_h 1
#endif  // _INCLUDED_DMScu_PtrVector_h

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
