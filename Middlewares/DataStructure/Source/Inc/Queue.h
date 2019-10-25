 /**************************************************************************************************
   Filename:       Queue.h
   Revised:        $Date: 2014-06-16 15:12:16 -0700 (Mon, 16 Jun 2014) $
   Revision:       $Revision: 39036 $

   Description:    Software Definitions for the basic queing package. 
 **************************************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

struct   q_elt    {     /* queue element: cast to right type */
   struct   q_elt   *   qe_next; /* it's just a pointer to next elt */
};

typedef struct q_elt * qp;    /* q pointer */

#define  q_elt qp

typedef   struct   queue   
{        /* queue header */
   q_elt q_head;        /* first element in queue */
   q_elt q_tail;        /* last element in queue */
   int   q_len;         /* number of elements in queue */
   int   q_max;         /* maximum length */
   int   q_min;         /* minimum length */
} queue;

/* The following macros implement most of the common queue operations */

/* Add an element to the head of the queue */

#define   q_addh(q, elt)    { \
   if ((q)->q_head == 0) (q)->q_tail = (elt); \
   (elt)->qe_next = (q)->q_head; \
   (q)->q_head = (elt); \
   (q)->q_len++; \
   if(++((q)->q_len) > (q)->q_max) (q)->q_max = (q)->q_len; \
}

/* Add an element to the tail of a queue */

#define   q_addt(q, elt)   { \
   (elt)->qe_next = 0; \
   if ((q)->q_head == 0) { \
      (q)->q_head = (elt); \
   } else { \
      (q)->q_tail->qe_next = (elt); \
   } \
   (q)->q_tail = (elt); \
   if(++((q)->q_len) > (q)->q_max) (q)->q_max = (q)->q_len; \
}

/* Add an element after a specified element in the queue.  If prev == */
/* &q->q_head, can be used to add an element to the head of the queue */

#define   q_adda(q, prev, new)   { \
   if ((q)->q_tail == (prev) || (q)->q_tail == 0) { \
      (q)->q_tail = (new); \
   } \
   (new)->qe_next = (prev)->qe_next; \
   (prev)->qe_next = (new); \
   if(++((q)->q_len) > (q)->q_max) (q)->q_max = (q)->q_len; \
}

/* Delete an element from a queue, given a pointer to the preceeding element */
/* Will delete the first element if prev == &q->q_head */

#define   q_dela(q, elt, prev)   { \
   if ((q)->q_tail == (elt)) { \
      if ((q)->q_head == (elt)) \
         (q)->q_tail = 0; \
      else \
         (q)->q_tail = (prev); \
   } \
   (prev)->qe_next = (elt)->qe_next; \
   (elt)->qe_next = 0; \
   if(--((q)->q_len) < (q)->q_min) (q)->q_min = (q)->q_len; \
}

/* The above MACROs are quite dangerous; because of lack of 
 * prototypeing, compile time snags, and no built in protection from 
 * premeption. Though they are fast, Code seems to be less buggy if 
 */
/* full prototypes: */
void     putq(queue*, void*);    /* add item to tail of queue */
void *   getq(queue*);           /* de-queue item from head of queue */
qp       qdel(queue*, void*);    /* delete an item from a queue */

#define  q_add(q,p)  putq(q,p)   /* allow old MIT names */
#define  q_deq(q) getq(q)
#define  aq_deq(q)   getq(q)     /* putq() and getq() are always atomic */

#endif   /* QUEUE_H */


