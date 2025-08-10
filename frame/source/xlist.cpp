#include <stdlib.h>
#include "stdafx.h"
#include "xlist.h"

void INIT_LIST_HEAD(struct list_head* list)
{
    list->next = list;
    list->prev = list;
    
    return;
}

void __list_add(struct list_head* newn, struct list_head* prev, struct list_head* next)
{
    next->prev = newn;
    newn->next = next;
    newn->prev = prev;
    prev->next = newn;
    
    return;
}

void list_add(struct list_head* newn, struct list_head* head)
{
	__list_add(newn, head, head->next);
	
	return;
}

void list_add_tail(struct list_head* newn, struct list_head* head)
{
    __list_add(newn, head->prev, head);

    return;
}

void __list_del(struct list_head* prev, struct list_head* next)
{
    next->prev = prev;
    prev->next = next;

    return;
}

void list_del(struct list_head* entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;

    return;
}

void list_replace(struct list_head* old, struct list_head* newn)
{
    newn->next = old->next;
    newn->next->prev = newn;
    newn->prev = old->prev;
    newn->prev->next = newn;

    return;
}

void list_replace_init(struct list_head* old, struct list_head* newn)
{
    list_replace(old, newn);
    INIT_LIST_HEAD(old);

    return;
}

void list_del_init(struct list_head* entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
    return;
}

void list_move(struct list_head* list, struct list_head* head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
    return;
}

void list_move_tail(struct list_head* list, struct list_head* head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
    return;
}

int list_is_last(const struct list_head* list, const struct list_head* head)
{
    return list->next == head;
}

int list_empty(const struct list_head* head)
{
    return head->next == head;
}

int list_is_singular(const struct list_head* head)
{
    return !list_empty(head) && (head->next == head->prev);
}

void __list_cut_position(struct list_head* list, struct list_head* head, struct list_head* entry)
{
    struct list_head *new_first = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = new_first;
    new_first->prev = head;
    return;
}

void list_cut_position(struct list_head* list, struct list_head* head, struct list_head* entry)
{
	if (list_empty(head))
		return;
	if (list_is_singular(head) && (head->next != entry && head != entry))
		return;
	if (entry == head)
		INIT_LIST_HEAD(list);
	else
		__list_cut_position(list, head, entry);	
	return;
}

void __list_splice(const struct list_head* list, struct list_head* prev, struct list_head* next)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
	return;
}

void list_splice(const struct list_head* list, struct list_head* head)
{
	if (!list_empty(list))
		__list_splice(list, head, head->next);
	return;
}

void list_splice_tail(struct list_head* list, struct list_head* head)
{
	if (!list_empty(list))
		__list_splice(list, head->prev, head);
	return;
}

void list_splice_init(struct list_head* list, struct list_head* head)
{
	if (!list_empty(list)) {
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
	return;
}
void list_splice_tail_init(struct list_head* list, struct list_head* head)
{
	if (!list_empty(list)) {
		__list_splice(list, head->prev, head);
		INIT_LIST_HEAD(list);
	}
	return;
}
