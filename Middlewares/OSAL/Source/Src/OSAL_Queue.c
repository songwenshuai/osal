/**************************************************************************************************
  Filename:       Queue.c
  Revised:        $Date: 2014-11-21 16:17:37 -0800 (Fri, 21 Nov 2014) $
  Revision:       $Revision: 41218 $

  Description:    Simple queuing system for internal use by the stack.
                  All other general-purpose queue manipulation routines are macros defined in "q.h".
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

#include "printf.h"
#include "OSAL_Queue.h"

#define QUEUE_CHECKING    1

/* Optional macro to dtrap if queue is corrupted. It tests that the queue 
 * head & item count are either both set to zero(NULL), or neither is zero.
 * It also checks the length of the queue with the number of elements
 * actually present.
 */

#ifdef QUEUE_CHECKING
#define QUEUE_CHECK(q) \
   {                                                    \
      q_elt temp=q->q_head;                             \
      int len=0;                                        \
      if(((q->q_head != NULL) && (q->q_len == 0)) ||    \
         ((q->q_head == NULL) && (q->q_len != 0)))      \
            HAL_PANIC(q);                               \
      while (temp)                                      \
      {                                                 \
         len++;                                         \
         temp=temp->qe_next;                            \
      }                                                 \
      if (len != q->q_len)                              \
      {                                                 \
         HAL_ASSERT_FORCED();                           \
      }                                                 \
   }

#else
#define QUEUE_CHECK(q)      /* define away */
#endif

/* FUNCTION: getq()
 *
 * Dequeue and return the first element of the specified queue.  
 *
 * PARAM1: queue * q
 *
 * RETURNS: pointer to the first element if any, or 0 if the queue is empty.
 */

void*
getq(queue * q)
{
   q_elt   temp;        /* temp for result */
   halIntState_t   intState;

   HAL_ENTER_CRITICAL_SECTION(intState);     /* shut off ints, save old state */

   if ((temp = q->q_head) == 0)  /* queue empty? */
   {
      HAL_EXIT_CRITICAL_SECTION(intState);
      return (0);             /* yes, show none */
   }

   q->q_head = temp->qe_next; /* else unlink */
   temp->qe_next = 0;         /* avoid dangling pointers */
   if (q->q_head == 0)        /* queue empty? */
      q->q_tail = 0;          /* yes, update tail pointer too */
   q->q_len--;                /* update queue length */
   if (q->q_len < q->q_min)
      q->q_min = q->q_len;

   QUEUE_CHECK(q);         /* make sure queue is not corrupted */

   HAL_EXIT_CRITICAL_SECTION(intState);   /* restore caller's int state */

   return ((void*)temp);
}


/* FUNCTION: putq()
 *
 * q_add(que, item) - Adds an item to tail of queue. 
 *
 * 
 * PARAM1: queue * q - queue to add item to
 * PARAM2: void * elt - item to add
 *
 * RETURNS: void
 */

void
putq(
   queue   *   q,       /* the queue */
   void *   elt)        /* element to delete */
{
   halIntState_t   intState;

   HAL_ENTER_CRITICAL_SECTION(intState);
   q_addt(q, (qp)elt);     /* use macro to do work */
   QUEUE_CHECK(q);         /* make sure queue is not corrupted */
   HAL_EXIT_CRITICAL_SECTION(intState);   /* restore int state */
}

/* FUNCTION: qdel()
 *
 * delete an item from the midst of a queue. Fix the head & tail and 
 * all counters. 
 *
 * 
 * PARAM1: queue * q
 * PARAM2: void * elt
 *
 * RETURNS: Return pointer to queue member if found, else NULL. 
 */

qp
qdel(queue * q, void * elt)
{
   qp qptr;
   qp qlast;
   halIntState_t   intState;

   /* search queue for element passed */
   HAL_ENTER_CRITICAL_SECTION(intState);
   qptr = q->q_head;
   qlast = NULL;
   while (qptr)
   {
      if (qptr == (qp)elt)
      {
         /* found our item; dequeue it */
         if (qlast)
            qlast->qe_next = qptr->qe_next;
         else     /* item was at head of queqe */
            q->q_head = qptr->qe_next;

         /* fix queue tail pointer if needed */
         if (q->q_tail == (qp)elt)
            q->q_tail = qlast;

         /* fix queue counters */
         q->q_len--;
         if (q->q_len < q->q_min)
            q->q_min = q->q_len;
         HAL_EXIT_CRITICAL_SECTION(intState);   /* restore int state */
         return (qp)elt;   /* success exit point */
      }
      qlast = qptr;
      qptr = qptr->qe_next;
   }
   HAL_EXIT_CRITICAL_SECTION(intState);   /* restore int state */
   return NULL;   /* item not found in queue */
}


