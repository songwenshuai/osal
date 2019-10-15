/******************************************************************************

 Group: WCS, LPC, BTS
 Target Device: cc13x2_26x2
 *****************************************************************************/

/**
 *  @file  List.h
 *  @brief      Linked List interface for use in the stack
 *
 *  This module provides simple doubly-link list implementation. There are two
 *  main structures:
 *     - ::list_list: The structure that holds the start of a linked list. There
 *  is no API to create one. It is up to the user to provide the structure
 *  itself.
 *     - ::list_elem: The structure that must be in the structure that is placed
 *  onto a linked list. Generally it is the first field in the structure. For
 *  example:
 *  @code
 *  typedef struct MyStruct {
 *      list_elem elem;
 *      void *buffer;
 *  } MyStruct;
 *  @endcode
 *
 *  The following shows how to create a linked list with three elements.
 *
 *  @code
 *  + denotes null-terminated
 *          _______        _______        _______      _______
 *         |_______|----->|_______|----->|_______|--->|_______|--//---,
 *    ,----|_______|    ,-|_______|<-----|_______|<---|_______|<-//-, +
 *    |      List       +   elem           elem          elem       |
 *    |_____________________________________________________________|
 *  @endcode
 *
 *  Initializing and adding an element to the tail and removing it
 *  @code
 *  typedef struct MyStruct {
 *      list_elem elem;
 *      void *buffer;
 *  } MyStruct;
 *
 *  list_list list;
 *  MyStruct foo;
 *  MyStruct *bar;
 *
 *  list_clearList(&list);
 *  list_put(&list, (list_elem *)&foo);
 *  bar = (MyStruct *)list_get(&list);
 *  @endcode
 *
 *  The ::list_put and ::list_get APIs are used to maintain a first-in first-out
 *  (FIFO) linked list.
 *
 *  The ::list_putHead and ::list_get APIs are used to maintain a last-in first-out
 *  (LIFO) linked list.
 *
 *  Traversing a list from head to tail. Note: thread-safety calls are
 *  not shown here.
 *  @code
 *  list_list list;
 *  list_elem *temp;
 *
 *  for (temp = list_head(&list); temp != NULL; temp = list_next(temp)) {
 *       printf("address = 0x%x\n", temp);
 *  }
 *  @endcode
 *
 *  Traversing a list from tail to head. Note: thread-safety calls are
 *  not shown here.
 *  @code
 *  list_list list;
 *  list_elem *temp;
 *
 *  for (temp = list_tail(&list); temp != NULL; temp = list_prev(temp)) {
 *       printf("address = 0x%x\n", temp);
 *  }
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */


/*********************************************************************
 * TYPEDEFS
 */
typedef struct list_elem {
    struct list_elem *next;
    struct list_elem *prev;
} list_elem;

typedef struct list_list {
    list_elem *head;
    list_elem *tail;
} list_list;

/*********************************************************************
 * FUNCTIONS
 */


/**
 *  @brief  Function to initialize the contents of an list_list
 *
 *  @param  list Pointer to an list_list structure that will be used to
 *               maintain a linked list
 */
extern void list_clearList(list_list *list);

/**
 *  @brief  Function to test whether a linked list is empty
 *
 *  @param  list A pointer to a linked list
 *
 *  @return true if empty, false if not empty
 */
extern bool list_empty(list_list *list);

/**
 *  @brief  Function to atomically get the first elem in a linked list
 *
 *  @param  list A pointer to a linked list
 *
 *  @return Pointer the first elem in the linked list or NULL if empty
 */
extern list_elem *list_get(list_list *list);

/**
 *  @brief  Function to return the head of a linked list
 *
 *  This function does not remove the head, it simply returns a pointer to
 *  it. This function is typically used when traversing a linked list.
 *
 *  @param  list A pointer to the linked list
 *
 *  @return Pointer to the first elem in the linked list or NULL if empty
 */
extern list_elem *list_head(list_list *list);

/**
 *  @brief  Function to insert an elem into a linked list
 *
 *  @param  list A pointer to the linked list
 *
 *  @param  newElem New elem to insert
 *
 *  @param  curElem Elem to insert the newElem in front of.
 *          This value cannot be NULL.
 */
extern void list_insert(list_list *list, list_elem *newElem,
                            list_elem *curElem);

/**
 *  @brief  Function to return the next elem in a linked list
 *
 *  This function does not remove the elem, it simply returns a pointer to
 *  next one. This function is typically used when traversing a linked list.
 *
 *  @param  elem Elem in the list
 *
 *  @return Pointer to the next elem in linked list or NULL if at the end
 */
extern list_elem *list_next(list_elem *elem);

/**
 *  @brief  Function to return the previous elem in a linked list
 *
 *  This function does not remove the elem, it simply returns a pointer to
 *  previous one. This function is typically used when traversing a linked list.
 *
 *  @param  elem Elem in the list
 *
 *  @return Pointer to the previous elem in linked list or NULL if at the
 *  beginning
 */
extern list_elem *list_prev(list_elem *elem);

/**
 *  @brief  Function to put an elem onto the end of a linked list
 *
 *  @param  list A pointer to the linked list
 *
 *  @param  elem Element to place onto the end of the linked list
 */
extern void list_put(list_list *list, list_elem *elem);

/**
 *  @brief  Function to put an elem onto the head of a linked list
 *
 *  @param  list A pointer to the linked list
 *
 *  @param  elem Element to place onto the beginning of the linked list
 */
extern void list_putHead(list_list *list, list_elem *elem);

/**
 *  @brief  Function to remove an elem from a linked list
 *
 *  @param  list A pointer to the linked list
 *
 *  @param  elem Element to be removed from a linked list
 */
extern void list_remove(list_list *list, list_elem *elem);

/**
 *  @brief  Function to return the tail of a linked list
 *
 *  This function does not remove the tail, it simply returns a pointer to
 *  it. This function is typically used when traversing a linked list.
 *
 *  @param  list A pointer to the linked list
 *
 *  @return Pointer to the last elem in the linked list or NULL if empty
 */
extern list_elem *list_tail(list_list *list);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LIST_H */
