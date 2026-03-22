#include <list.h>

// 1. 准备一个打印函数给 list_map 用
void print_int(void *data, void *cl) {
    int val = *(int*)data;
    printf("%d ", val);
}

int main() {
    // 2. 创建存 int 的链表
    T my_list = List_new(sizeof(int));

    // 3. 存数据
    for(int i=1; i<=5; i++) list_push_back(my_list, &i);

    // 4. 遍历打印
    list_map(my_list, print_int, NULL);

    // 5. 查找并删除
    int target = 3;
    void *found = list_find(my_list, &target, my_int_compare);
    if(found) list_remove(my_list, found);

    list_free(&my_list);
    return 0;
}
