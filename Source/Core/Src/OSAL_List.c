/**************************************************************************************************
  Filename:       OSAL_List.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:    This implements a doubly linked list module for the stack
**************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

#include "OSAL_List.h"

/*
 *  ======== osal_list_clearList ========
 */
void osal_list_clearList(osal_list_list *list)
{
  list->head = list->tail = NULL;
}

/*
 *  ======== osal_list_empty ========
 */
bool osal_list_empty(osal_list_list *list)
{
  return (list->head == NULL);
}

/*
 *  ======== osal_list_get ========
 */
osal_list_elem *osal_list_get(osal_list_list *list)
{
  osal_list_elem *elem;

  elem = list->head;

  /* See if the List was empty */
  if(elem != NULL)
  {
    list->head = elem->next;

    if(elem->next != NULL)
    {
      elem->next->prev = NULL;
    }
    else
    {
      list->tail = NULL;
    }
  }

  return (elem);
}

/*
 *  ======== osal_list_head ========
 */
osal_list_elem *osal_list_head(osal_list_list *list)
{
  return (list->head);
}

/*
 *  ======== osal_list_insert ========
 */
void osal_list_insert(osal_list_list *list, osal_list_elem *newElem,
                     osal_list_elem *curElem)
{
  newElem->next = curElem;
  newElem->prev = curElem->prev;

  if(curElem->prev != NULL)
  {
    curElem->prev->next = newElem;
  }
  else
  {
    list->head = newElem;
  }

  curElem->prev = newElem;
}

/*
 *  ======== osal_list_next ========
 */
osal_list_elem *osal_list_next(osal_list_elem *elem)
{
  return (elem->next);
}

/*
 *  ======== osal_list_prev ========
 */
osal_list_elem *osal_list_prev(osal_list_elem *elem)
{
  return (elem->prev);
}

/*
 *  ======== osal_list_put ========
 */
void osal_list_put(osal_list_list *list, osal_list_elem *elem)
{
  elem->next = NULL;
  elem->prev = list->tail;

  if(list->tail != NULL)
  {
    list->tail->next = elem;
  }
  else
  {
    list->head = elem;
  }

  list->tail = elem;
}

/*
 *  ======== osal_list_putHead ========
 */
void osal_list_putHead(osal_list_list *list, osal_list_elem *elem)
{
  elem->next = list->head;
  elem->prev = NULL;

  if(list->head != NULL)
  {
    list->head->prev = elem;
  }
  else
  {
    list->tail = elem;
  }

  list->head = elem;
}

/*
 *  ======== osal_list_remove ========
 */
void osal_list_remove(osal_list_list *list, osal_list_elem *elem)
{
  /* Handle the case where the elem to remove is the last one */
  if(elem->next == NULL)
  {
    list->tail = elem->prev;
  }
  else
  {
    elem->next->prev = elem->prev;
  }

  /* Handle the case where the elem to remove is the first one */
  if(elem->prev == NULL)
  {
    list->head = elem->next;
  }
  else
  {
    elem->prev->next = elem->next;
  }
}

/*
 *  ======== osal_list_tail ========
 */
osal_list_elem *osal_list_tail(osal_list_list *list)
{
  return (list->tail);
}

/*********************************************************************
 */
