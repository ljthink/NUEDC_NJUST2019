#ifndef __RT_SERVICE_H__
#define __RT_SERVICE_H__

/* 已知一个结构体里面的成员的地址，反推出该结构体的首地址 */
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
			
#define list_entry(node, type, member) \
    container_of(node, type, member)
		
/*
*************************************************************************
*                           双向链表操作相关函数
*************************************************************************
*/
	
      
/* 链表节点 */
struct list_node
{
  struct list_node *next;
  struct list_node *prev;
};
typedef struct list_node list_t;   
 

struct ui_node
{
  /* rt 对象 */
  char      oled_txt[16];         /* 屏幕是暗示的字符 */
	void        *parameter;	        /* 待修改的参数 */	
};
typedef struct ui_node *ui_node_t;                      


/* 初始化链表节点 */
static inline void list_init(list_t *l)
{
    l->next = l->prev = l;
}

/* 在双向链表头部插入一个节点 */
static inline void list_insert_after(list_t *l, list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

/* 在双向链表尾部插入一个节点 */
static inline void list_insert_before(list_t *l, list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}

/* 从双向链表删除一个节点 */
static inline void list_remove(list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}



#endif /* __RT_SERVICE_H__ */
