/**
 * @file LinkList.c
 * Handle linked lists.
 * The nodes are dynamically allocated by the 'lv_mem' module,
 */

/*********************
 *      INCLUDES
 *********************/
#include "OSAL.h"

#include "OSAL_Helper.h"

#include "lwmem.h"

#include "OSAL_LinkList.h"

/*********************
 *      DEFINES
 *********************/
#define LINK_LIST_BUF_MEMSET                      osal_memset
#define LINK_LIST_BUF_MEMCPY                      osal_memcpy
#define LINK_LIST_MEM_ALLOC                       lwmem_malloc
#define LINK_LIST_MEM_FREE                        lwmem_free

#define LL_NODE_META_SIZE (sizeof(ll_node_t *) + sizeof(ll_node_t *))
#define LL_PREV_P_OFFSET(ll_p) (ll_p->n_size)
#define LL_NEXT_P_OFFSET(ll_p) (ll_p->n_size + sizeof(ll_node_t *))

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void node_set_prev(ll_t * ll_p, ll_node_t * act, ll_node_t * prev);
static void node_set_next(ll_t * ll_p, ll_node_t * act, ll_node_t * next);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize linked list
 * @param ll_dsc pointer to ll_dsc variable
 * @param node_size the size of 1 node in bytes
 */
void ll_init(ll_t * ll_p, uint32 node_size)
{
    ll_p->head = NULL;
    ll_p->tail = NULL;

    /*Round the size up to 4*/
    if(node_size & 0x3) {
        node_size = node_size & (~0x3);
        node_size += 4;
    }

    ll_p->n_size = node_size;
}

/**
 * Add a new head to a linked list
 * @param ll_p pointer to linked list
 * @return pointer to the new head
 */
void * ll_ins_head(ll_t * ll_p)
{
    ll_node_t * n_new;

    n_new = LINK_LIST_MEM_ALLOC(ll_p->n_size + LL_NODE_META_SIZE);

    if(n_new != NULL) {
        node_set_prev(ll_p, n_new, NULL);       /*No prev. before the new head*/
        node_set_next(ll_p, n_new, ll_p->head); /*After new comes the old head*/

        if(ll_p->head != NULL) { /*If there is old head then before it goes the new*/
            node_set_prev(ll_p, ll_p->head, n_new);
        }

        ll_p->head = n_new;      /*Set the new head in the dsc.*/
        if(ll_p->tail == NULL) { /*If there is no tail (1. node) set the tail too*/
            ll_p->tail = n_new;
        }
    }

    return n_new;
}

/**
 * Insert a new node in front of the n_act node
 * @param ll_p pointer to linked list
 * @param n_act pointer a node
 * @return pointer to the new head
 */
void * ll_ins_prev(ll_t * ll_p, void * n_act)
{
    ll_node_t * n_new;
    ll_node_t * n_prev;

    if(NULL == ll_p || NULL == n_act) return NULL;

    if(ll_get_head(ll_p) == n_act) {
        n_new = ll_ins_head(ll_p);
        if(n_new == NULL) return NULL;
    } else {
        n_new = LINK_LIST_MEM_ALLOC(ll_p->n_size + LL_NODE_META_SIZE);
        if(n_new == NULL) return NULL;

        n_prev = ll_get_prev(ll_p, n_act);
        node_set_next(ll_p, n_prev, n_new);
        node_set_prev(ll_p, n_new, n_prev);
        node_set_prev(ll_p, n_act, n_new);
        node_set_next(ll_p, n_new, n_act);
    }

    return n_new;
}

/**
 * Add a new tail to a linked list
 * @param ll_p pointer to linked list
 * @return pointer to the new tail
 */
void * ll_ins_tail(ll_t * ll_p)
{
    ll_node_t * n_new;

    n_new = LINK_LIST_MEM_ALLOC(ll_p->n_size + LL_NODE_META_SIZE);
    if(n_new == NULL) return NULL;

    if(n_new != NULL) {
        node_set_next(ll_p, n_new, NULL);       /*No next after the new tail*/
        node_set_prev(ll_p, n_new, ll_p->tail); /*The prev. before new is tho old tail*/
        if(ll_p->tail != NULL) {                /*If there is old tail then the new comes after it*/
            node_set_next(ll_p, ll_p->tail, n_new);
        }

        ll_p->tail = n_new;      /*Set the new tail in the dsc.*/
        if(ll_p->head == NULL) { /*If there is no head (1. node) set the head too*/
            ll_p->head = n_new;
        }
    }

    return n_new;
}

/**
 * Remove the node 'node_p' from 'll_p' linked list.
 * It does not free the the memory of node.
 * @param ll_p pointer to the linked list of 'node_p'
 * @param node_p pointer to node in 'll_p' linked list
 */
void ll_rem(ll_t * ll_p, void * node_p)
{
    if(ll_get_head(ll_p) == node_p) {
        /*The new head will be the node after 'n_act'*/
        ll_p->head = ll_get_next(ll_p, node_p);
        if(ll_p->head == NULL) {
            ll_p->tail = NULL;
        } else {
            node_set_prev(ll_p, ll_p->head, NULL);
        }
    } else if(ll_get_tail(ll_p) == node_p) {
        /*The new tail will be the  node before 'n_act'*/
        ll_p->tail = ll_get_prev(ll_p, node_p);
        if(ll_p->tail == NULL) {
            ll_p->head = NULL;
        } else {
            node_set_next(ll_p, ll_p->tail, NULL);
        }
    } else {
        ll_node_t * n_prev = ll_get_prev(ll_p, node_p);
        ll_node_t * n_next = ll_get_next(ll_p, node_p);

        node_set_next(ll_p, n_prev, n_next);
        node_set_prev(ll_p, n_next, n_prev);
    }
}

/**
 * Remove and free all elements from a linked list. The list remain valid but become empty.
 * @param ll_p pointer to linked list
 */
void ll_clear(ll_t * ll_p)
{
    void * i;
    void * i_next;

    i      = ll_get_head(ll_p);
    i_next = NULL;

    while(i != NULL) {
        i_next = ll_get_next(ll_p, i);

        ll_rem(ll_p, i);
        LINK_LIST_MEM_FREE(i);

        i = i_next;
    }
}

/**
 * Move a node to a new linked list
 * @param ll_ori_p pointer to the original (old) linked list
 * @param ll_new_p pointer to the new linked list
 * @param node pointer to a node
 * @param head true: be the head in the new list
 *             false be the head in the new list
 */
void ll_chg_list(ll_t * ll_ori_p, ll_t * ll_new_p, void * node, _bool head)
{
    ll_rem(ll_ori_p, node);

    if(head) {
        /*Set node as head*/
        node_set_prev(ll_new_p, node, NULL);
        node_set_next(ll_new_p, node, ll_new_p->head);

        if(ll_new_p->head != NULL) { /*If there is old head then before it goes the new*/
            node_set_prev(ll_new_p, ll_new_p->head, node);
        }

        ll_new_p->head = node;       /*Set the new head in the dsc.*/
        if(ll_new_p->tail == NULL) { /*If there is no tail (first node) set the tail too*/
            ll_new_p->tail = node;
        }
    } else {
        /*Set node as tail*/
        node_set_prev(ll_new_p, node, ll_new_p->tail);
        node_set_next(ll_new_p, node, NULL);

        if(ll_new_p->tail != NULL) { /*If there is old tail then after it goes the new*/
            node_set_next(ll_new_p, ll_new_p->tail, node);
        }

        ll_new_p->tail = node;       /*Set the new tail in the dsc.*/
        if(ll_new_p->head == NULL) { /*If there is no head (first node) set the head too*/
            ll_new_p->head = node;
        }
    }
}

/**
 * Return with head node of the linked list
 * @param ll_p pointer to linked list
 * @return pointer to the head of 'll_p'
 */
void * ll_get_head(const ll_t * ll_p)
{
    void * head = NULL;

    if(ll_p != NULL) {
        head = ll_p->head;
    }

    return head;
}

/**
 * Return with tail node of the linked list
 * @param ll_p pointer to linked list
 * @return pointer to the head of 'll_p'
 */
void * ll_get_tail(const ll_t * ll_p)
{
    void * tail = NULL;

    if(ll_p != NULL) {
        tail = ll_p->tail;
    }

    return tail;
}

/**
 * Return with the pointer of the next node after 'n_act'
 * @param ll_p pointer to linked list
 * @param n_act pointer a node
 * @return pointer to the next node
 */
void * ll_get_next(const ll_t * ll_p, const void * n_act)
{
    void * next = NULL;

    if(ll_p != NULL) {
        const ll_node_t * n_act_d = n_act;
        LINK_LIST_BUF_MEMCPY(&next, n_act_d + LL_NEXT_P_OFFSET(ll_p), sizeof(void *));
    }

    return next;
}

/**
 * Return with the pointer of the previous node after 'n_act'
 * @param ll_p pointer to linked list
 * @param n_act pointer a node
 * @return pointer to the previous node
 */
void * ll_get_prev(const ll_t * ll_p, const void * n_act)
{
    void * prev = NULL;

    if(ll_p != NULL) {
        const ll_node_t * n_act_d = n_act;
        LINK_LIST_BUF_MEMCPY(&prev, n_act_d + LL_PREV_P_OFFSET(ll_p), sizeof(void *));
    }

    return prev;
}

/**
 * Return the length of the linked list.
 * @param ll_p pointer to linked list
 * @return length of the linked list
 */
uint32 ll_get_len(const ll_t * ll_p)
{
    uint32 len = 0;
    void * node;

    for(node = ll_get_head(ll_p); node != NULL; node = ll_get_next(ll_p, node)) {
        len++;
    }

    return len;
}

void ll_swap(ll_t * ll_p, void * n1_p, void * n2_p)
{
    (void)(ll_p);
    (void)(n1_p);
    (void)(n2_p);
    /*TODO*/
}

/**
 * Move a nodw before an other node in the same linked list
 * @param ll_p pointer to a linked list
 * @param n_act pointer to node to move
 * @param n_after pointer to a node which should be after `n_act`
 */
void ll_move_before(ll_t * ll_p, void * n_act, void * n_after)
{
    if(n_act == n_after) return; /*Can't move before itself*/

    void * n_before;
    if(n_after != NULL)
        n_before = ll_get_prev(ll_p, n_after);
    else
        n_before = ll_get_tail(ll_p); /*if `n_after` is NULL `n_act` should be the new tail*/

    if(n_act == n_before) return; /*Already before `n_after`*/

    /*It's much easier to remove from the list and add again*/
    ll_rem(ll_p, n_act);

    /*Add again by setting the prev. and next nodes*/
    node_set_next(ll_p, n_before, n_act);
    node_set_prev(ll_p, n_act, n_before);
    node_set_prev(ll_p, n_after, n_act);
    node_set_next(ll_p, n_act, n_after);

    /*If `n_act` was moved before NULL then it become the new tail*/
    if(n_after == NULL) ll_p->tail = n_act;

    /*If `n_act` was moved before `NULL` then it's the new head*/
    if(n_before == NULL) ll_p->head = n_act;
}

/**
 * Check if a linked list is empty
 * @param ll_p pointer to a linked list
 * @return true: the linked list is empty; false: not empty
 */
_bool ll_is_empty(ll_t * ll_p)
{
    if(ll_p == NULL) return TRUE;

    if(ll_p->head == NULL && ll_p->tail == NULL) return TRUE;

    return FALSE;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Set the 'pervious node pointer' of a node
 * @param ll_p pointer to linked list
 * @param act pointer to a node which prev. node pointer should be set
 * @param prev pointer to a node which should be the previous node before 'act'
 */
static void node_set_prev(ll_t * ll_p, ll_node_t * act, ll_node_t * prev)
{
    if(act == NULL) return; /*Can't set the prev node of `NULL`*/

    uint32 node_p_size = sizeof(ll_node_t *);
    if(prev)
        LINK_LIST_BUF_MEMCPY(act + LL_PREV_P_OFFSET(ll_p), &prev, node_p_size);
    else
        LINK_LIST_BUF_MEMSET(act + LL_PREV_P_OFFSET(ll_p), 0, node_p_size);
}

/**
 * Set the 'next node pointer' of a node
 * @param ll_p pointer to linked list
 * @param act pointer to a node which next node pointer should be set
 * @param next pointer to a node which should be the next node before 'act'
 */
static void node_set_next(ll_t * ll_p, ll_node_t * act, ll_node_t * next)
{
    if(act == NULL) return; /*Can't set the next node of `NULL`*/

    uint32 node_p_size = sizeof(ll_node_t *);
    if(next)
        LINK_LIST_BUF_MEMCPY(act + LL_NEXT_P_OFFSET(ll_p), &next, node_p_size);
    else
        LINK_LIST_BUF_MEMSET(act + LL_NEXT_P_OFFSET(ll_p), 0, node_p_size);
}
