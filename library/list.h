#ifndef LIST_H
#define LIST_H


struct list_head {
    struct list_head *next, *prev;
};

#define container_of(ptr, type, member) ({ \
    const typeof(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member));})

#define LIST_HEAD_INIT(name) { &(name), &(name) }   // 初始化链表头

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)     // 定义链表头

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}


#endif // LIST_H