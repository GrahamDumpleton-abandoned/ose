#ifndef OTC_COLLCTN_DEQUE_HH
#define OTC_COLLCTN_DEQUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/deque.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/dequei.hh>
#include <OTC/collctn/shlwcopy.hh>
#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/modifier.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/bucket.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if defined(EXPAND_TEMPLATES)
#pragma implementation "OTC/collctn/deque.hh"
#endif
#pragma interface "OTC/collctn/deque.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Deque
    // = TITLE
    //     A deque.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_Deque> implements a list of objects where access to and
    //     manipulation of items, is only permitted on items occurring at the
    //     ends of the list.
    //     
    // = NOTES
    //     The <OTC_Bucket> class is used internally to hold items in the
    //     list. Thus, the <OTC_BaseActions> class may be used to provide
    //     actions to be performed, when items are inserted and removed from
    //     the list.
    //
    // = SEE ALSO
    //     <OTC_Iterator>, <OTC_Modifier>, <OTC_Bucket>, <OTC_BaseActions>
{
  public:

			~OTC_Deque() {}

    // = INITIALISATION

			OTC_Deque() {}
				// Creates an empty list.

			OTC_Deque(OTC_Deque<T> const& theDeque);
				// Creates a copy of <theDeque>.

			OTC_Deque(OTC_Deque<T>& theDeque, OTC_ShallowCopy);
				// Creates an alias for <theDeque>.

    // = ASSIGNMENT

    OTC_Deque<T>&	operator=(OTC_Deque<T> const& theDeque);
				// Replaces the contents of this list with
				// the items contained in <theDeque>.

    // = QUERY

    u_int               population() const
                                { return list_.population(); }
                                // Returns the number of items in the
                                // deque.

    OTC_Boolean         isEmpty() const
                                { return list_.isEmpty(); }
                                // Returns <OTCLIB_TRUE> if there are no
                                // items in the deque.

    // = ADDITION

    void		addFirst(T const& theItem)
				{ list_.addFirst(link_(theItem)); }
				// Inserts <theItem> at the head of the list.

    void		addLast(T const& theItem)
				{ list_.addLast(link_(theItem)); }
				// Appends <theItem> to the tail of the list.

    // = ACCESS

    T&			first()
				{
				  return ((OTC_Bucket<T>*)
				   list_.first())->item();
				}
				// Returns a reference to the first item in
				// the list. If the list is empty, an
				// exception is raised.

    T const&		first() const
				{
				  return ((OTC_Bucket<T>*)
				   list_.first())->item();
				}
				// Returns a reference to the first item in
				// the list. If the list is empty, an
				// exception is raised.

    T&			last()
				{
				  return ((OTC_Bucket<T>*)
				   list_.last())->item();
				}
				// Returns a reference to the last item in
				// the list. If the list is empty, an
				// exception is raised.

    T const&		last() const
				{
				  return ((OTC_Bucket<T>*)
				   list_.last())->item();
				}
				// Returns a reference to the last item in
				// the list. If the list is empty, an
				// exception is raised.

    T*			pFirst()
				{
				  return list_.isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_.first())->item();
				}
				// Returns a pointer to the first item in
				// the list. If the list is empty, a null
				// pointer is returned.

    T const*		pFirst() const
				{
				  return list_.isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_.first())->item();
				}
				// Returns a pointer to the first item in
				// the list. If the list is empty, a null
				// pointer is returned.

    T*			pLast()
				{
				  return list_.isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_.last())->item();
				}
				// Returns a pointer to the last item in
				// the list. If the list is empty, a null
				// pointer is returned.

    T const*		pLast() const
				{
				  return list_.isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_.last())->item();
				}
				// Returns a pointer to the last item in
				// the list. If the list is empty, a null
				// pointer is returned.

    // = REMOVAL

    void                removeAll()
                                { list_.removeAll(); }
                                // Removes all items from the deque.

    void                removeFirst(OTC_Protection theProtection=OTCLIB_SAFE)
				{ list_.removeFirst(theProtection); }
                                // Removes the first item in the deque. If
                                // the deque is empty, an exception is
				// raised, unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void                removeLast(OTC_Protection theProtection=OTCLIB_SAFE)
				{ list_.removeLast(theProtection); }
                                // Removes the last item in the deque. If
				// the deque is empty, an exception is
				// raised, unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    // = ITERATION
    //     By default, iterators will perform reference counts on the
    //     buckets in the collection as the iterator moves over the items.
    //     Performing the reference counts ensures that the iterator
    //     is not corrupted by additions or removals to the collection.
    //     If an unsafe iterator is required, for example, to avoid
    //     grabbing a write lock when using ObjectStore, a second
    //     argument can be passed to the following functions. The value
    //     of this argument is either <OTCLIB_SAFE> or <OTCLIB_UNSAFE>.
    //     To get an unsafe iterator, the <OTCLIB_UNSAFE> argument should
    //     be used.
    //
    //     The first argument to the following functions indicates the
    //     direction of traversal of the iterator. Traversal in the
    //     direction of first to last item is indicated by a value of
    //     <OTCLIB_FORWARD>. Traversal in the reverse direction is
    //     indicated by a value of <OTCLIB_BACKWARD>. The default value
    //     is <OTCLIB_FORWARD>.

    OTC_Iterator<T>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{ return items_(theDirection,theProtection); }
				// Returns an iterator over the list.

    OTC_Modifier<T>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{ return items_(theDirection,theProtection); }
				// Returns an iterator over the list.

  public:

                        operator OTC_DequeHI<T>() const
                                { return list_; }
                                // Special to use of <OTC_MListIterator>
                                // and <OTC_MListModifier>.

                        operator OTC_DequeHI<T>()
                                { return list_; }
                                // Special to use of <OTC_MListIterator>
                                // and <OTC_MListModifier>.

#if defined(ENV_OSTORE)
    static os_typespec* get_os_typespec();
#endif

  private:

    OTC_Cursor<T>*	items_(
			 OTC_Direction theDirection,
			 OTC_Protection theProtection
			) const;
				// Returns a cursor over the list.

    OTC_Link*		link_(T const& theItem);
				// Creates an instance of <OTC_Bucket>
				// to hold <theItem>.

    OTC_DequeI		list_;
				// The actual list.
};

/* ------------------------------------------------------------------------- */

#if defined(EXPAND_TEMPLATES)
#include <OTC/collctn/deque.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_DEQUE_HH */
