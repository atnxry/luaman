#ifndef __XLIST_H__
#define __XLIST_H__

struct list_head
{
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)


void INIT_LIST_HEAD(struct list_head* list);

void __list_add(struct list_head* newn, struct list_head* prev, struct list_head* next);

void list_add(struct list_head* newn, struct list_head* head);

void list_add_tail(struct list_head* newn, struct list_head* head);

void __list_del(struct list_head* prev, struct list_head* next);

void list_del(struct list_head* entry);

void list_replace(struct list_head* old, struct list_head* newn);

void list_replace_init(struct list_head* old, struct list_head* newn);

void list_del_init(struct list_head* entry);
void list_move(struct list_head* list, struct list_head* head);

void list_move_tail(struct list_head* list, struct list_head* head);

int list_is_last(const struct list_head* list, const struct list_head* head);
int list_empty(const struct list_head* head);

int list_is_singular(const struct list_head* head);

void __list_cut_position(struct list_head* list, struct list_head* head, struct list_head* entry);

void list_cut_position(struct list_head* list, struct list_head* head, struct list_head* entry);

void __list_splice(const struct list_head* list, struct list_head* prev, struct list_head* next);
void list_splice(const struct list_head* list, struct list_head* head);
void list_splice_tail(struct list_head* list, struct list_head* head);

void list_splice_init(struct list_head* list, struct list_head* head);
void list_splice_tail_init(struct list_head* list, struct list_head* head);

#define ledo_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define ledo_container_of(ptr, nodetype, member) (			\
	(nodetype *)((char *)ptr - ledo_offsetof(nodetype, member)))

#define list_entry(ptr, nodetype, member) \
	ledo_container_of(ptr, nodetype, member)

#define list_first_entry(ptr, nodetype, member) \
	list_entry((ptr)->next, nodetype, member)	

#define list_for_each_entry_safe(pos, n, head, member, nodetype)			\
	for (pos = list_entry((head)->next, nodetype, member),	\
		n = list_entry(pos->member.next, nodetype, member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, nodetype, member))
		 
#endif
