#include <stdbool.h>
#include <stdio.h>

/*
概念：
    头指针：指向链表头结点或首元结点
    头结点：不存数据的节点，其存在意义是为了统一表其他节点操作
    首元结点
*/

typedef struct single_list_node
{
    struct single_list_node* next;
    int val;
} single_list_node;

typedef struct single_list
{
    struct single_list_node* head;
} single_list;

void insert(single_list_node** pre_next_ptr, single_list_node* elem) // NT : 注意当链表为空时pre_node_next由单独一个single_list的next节点提供这个指针
{
    if (!pre_next_ptr) // NT : 指向head指针的指针
        return;

    elem->next = *pre_next_ptr;
    *pre_next_ptr = elem;
}

void insert_head(single_list* L, single_list_node* elem)
{
    insert(&L->head, elem);
}

void dump(single_list* L)
{
    static int count = 0;
    count = 0;
    single_list_node* node = L->head;
    int index = 0;

    while (node)
    {
        printf("[%02d]:%08d, next_ptr[%p]\n", index++, node->val, &node->next);
        node = node->next;
        count = count + 1;
        if (count >= 30)
            break;
    }
}

bool is_empty(single_list* L)
{
    return L->head == NULL;
}

single_list_node* del(single_list_node** pre_next_ptr) // NT : 传入指向某个节点next指针成员的指针，删除其后面元素
{
    if (!pre_next_ptr)
        return NULL;

    if (!(*pre_next_ptr)) // NT : 点前节点 next 节点为空
        return NULL;

    single_list_node* del_node = *pre_next_ptr;
    *pre_next_ptr = (*pre_next_ptr)->next; // NT : 当前节点的next指针赋值为
    del_node->next = NULL;

    return del_node;
}

single_list_node* delete_head(single_list* L)
{
    if (!L->head)
        return NULL;

    single_list_node* first_node = L->head;
    L->head = L->head->next;
    first_node->next = NULL;

    return first_node;
}

single_list_node** lower_search(single_list* L, int target) // NT : 查找小于target的最后一个位置
{
    single_list_node **pre_next_ptr, *node;
    for (pre_next_ptr = &L->head, node = L->head;
         node && node->val < target;
         pre_next_ptr = &node->next, node = node->next)
        ;

    return pre_next_ptr;
}

void reverse(single_list* L)
{
    single_list tmpl = {NULL};

    single_list_node* node = L->head;
    single_list_node* next_node = NULL; // NT : 核心是有个指向下个节点的next_node指针，主要是解决将当前节点插入新链表中时会破坏当前节点的next指针数据，访问不到下个节点的问题
    while (node)
    {
        next_node = node->next;
        node->next = tmpl.head;
        tmpl.head = node;
        node = next_node;
    }

    L->head = tmpl.head;
}

bool is_circle(single_list* L)
{
    if (!L->head)
        return false;

    single_list_node* slow_ptr = L->head;
    single_list_node* fast_ptr = L->head;

    while (slow_ptr && fast_ptr)
    {
        slow_ptr = slow_ptr->next;
        fast_ptr = fast_ptr->next ? fast_ptr->next->next : NULL;

        if (slow_ptr == fast_ptr)
            return true;
    }

    return false;
}

single_list_node* middle(single_list* L)
{
    if (!L->head)
        return NULL;

    single_list_node pseudo_head; // NT : 创建了虚拟节点保证slow_ptr从第一个节点开始向下递进，如果都从head节点开始，偶数size时会返回右边的中间节点
    pseudo_head.next = L->head;

    single_list_node* slow_ptr = &pseudo_head;
    single_list_node* fast_ptr = &pseudo_head;

    while (true)
    {
        if (!fast_ptr || !fast_ptr->next)
            return slow_ptr;
        slow_ptr = slow_ptr->next;
        fast_ptr = fast_ptr->next->next;
    }

    return slow_ptr;
}

int main()
{
    single_list L = {NULL};
    single_list_node lists[10];

    printf("head_next_ptr = %p\n", &L.head);

    for (int i = 0; i < 10; ++i)
    {
        lists[i].val = i;
        lists[i].next = NULL;
    }

    insert_head(&L, &lists[6]);
    insert_head(&L, &lists[5]);
    insert_head(&L, &lists[4]);
    insert_head(&L, &lists[2]);
    insert_head(&L, &lists[1]);

    dump(&L);

    single_list_node** pre_next_ptr = lower_search(&L, 3);

    insert(pre_next_ptr, &lists[3]);

    printf("\n");
    dump(&L);

    delete_head(&L);

    printf("\n");
    dump(&L);

    pre_next_ptr = lower_search(&L, 5);

    del(pre_next_ptr);

    printf("\n");
    dump(&L);

    // printf("the single list is have circule %s\n", is_circle(&L) ? "true" : "false");
    // lists[6].next = &lists[3];
    // printf("the single list is have circule %s\n", is_circle(&L) ? "true" : "false");
    // insert_head(&L, &lists[1]);
    printf("\n");
    dump(&L);

    single_list_node* node = middle(&L);
    printf("list middle node %d\n", node->val);

    reverse(&L);

    printf("\n");
    dump(&L);
}