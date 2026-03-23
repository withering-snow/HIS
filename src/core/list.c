#include <../include/core/list.h>
#include <string.h>

typedef struct Node {
    struct Node *prev;
    struct Node *next;
    char data[];
} *PNode;

struct List_T {
    size_t element_size;
    size_t size;
    PNode dummy;
    PNode cursor;
};

#define GET_NODE(ptr) (PNode)((char *)(ptr) - offsetof(struct Node, data))

static PNode __create_node(size_t element_size, const void *data, PNode prev, PNode next) {
    PNode n = safe_malloc(sizeof(struct Node) + element_size);
    n->prev = prev;
    n->next = next;
    prev->next = n;
    next->prev = n;
    if(data)
        memcpy(n->data, data, element_size);
    return n;
}


List_T List_new(size_t element_size) {
    List_T l = safe_malloc(sizeof(struct List_T));
    l->element_size = element_size;
    l->size = 0;
    l->dummy = safe_malloc(sizeof(struct Node));
    l->dummy->next = l->dummy->prev = l->dummy;
    return l;
}

void List_free(List_T *l) {
    ASSERT(l && *l, "不能释放空链表");
    List_T self = *l;
    PNode curr = self->dummy->next;
    while(curr != self->dummy) {
        PNode next = curr->next;
        free(curr);
        curr = next;
    }
    free(self->dummy);
    free(self);
    *l = NULL;
}

size_t List_size(List_T l) { return l ? l->size : 0; }

void List_push_back(List_T l, const void *data) {
    ASSERT(l, "链表不能为空");
    __create_node(l->element_size, data, l->dummy->prev, l->dummy);
    l->size++;
}

void *List_find(List_T l, const void *data_ptr, compare cmp) {
    ASSERT(l && cmp, "参数不能为空");
    for(PNode it = l->dummy->next; it != l->dummy; it = it->next) {
        if(cmp(it->data, data_ptr) == 0)
            return (void *)it->data;
    }
    return NULL;
}

void List_remove(List_T l, void *data_ptr) {
    ASSERT(l && data_ptr, "参数不能为空");
    PNode target = GET_NODE(data_ptr);
    ASSERT(target != l->dummy, "不能删除 dummy 节点");

    target->prev->next = target->next;
    target->next->prev = target->prev;
    free(target);
    l->size--;
}

void *List_first(List_T l) {
    ASSERT(l, "链表不能为空");
    l->cursor = l->dummy->next;
    if(l->cursor == l->dummy)
        return NULL;
    return l->cursor->data;
}

void *List_next(List_T l) {
    ASSERT(l, "链表不能为空");
    if(l->cursor == l->dummy)
        return NULL;
    l->cursor = l->cursor->next;
    if(l->cursor == l->dummy)
        return NULL;
    return l->cursor->data;
}

static PNode __merge(PNode left, PNode right, compare cmp) {
    if(! left)
        return right;
    if(! right)
        return left;

    if(cmp(left->data, right->data) <= 0) {
        left->next = __merge(left->next, right, cmp);
        return left;
    }
    else {
        right->next = __merge(left, right->next, cmp);
        return right;
    }
}

static PNode __merge_sort(PNode head, compare cmp) {
    if(! head || ! head->next)
        return head;

    PNode slow = head;
    PNode fast = head->next;
    while(fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    PNode mid = slow->next;
    slow->next = NULL;

    return __merge(__merge_sort(head, cmp), __merge_sort(mid, cmp), cmp);
}

void List_sort(List_T l, compare cmp) {
    ASSERT(l, "链表不能为空");
    ASSERT(cmp, "比较器不能为空");
    if(l->size < 2)
        return;

    PNode head = l->dummy->next;
    l->dummy->prev->next = NULL;
    head = __merge_sort(head, cmp);

    l->dummy->next = head;
    PNode curr = l->dummy;
    PNode next_node = head;

    while(next_node != NULL) {
        next_node->prev = curr;
        curr->next = next_node;
        curr = next_node;
        next_node = next_node->next;
    }

    curr->next = l->dummy;
    l->dummy->prev = curr;
}
