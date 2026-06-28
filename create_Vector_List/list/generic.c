#include "generic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../macros.h"
#include "../comparators.h"


GenericList *createList(size_t elem_size)
{
    GenericList *list = malloc(sizeof(GenericList));  // О(1)
    if (list == NULL) {  // О(1)
        RETURN_ERROR("Ошибка выделения памяти", NULL);  // О(1)
    }

    list->head = NULL;  // О(1)
    list->elem_size = elem_size;  // О(1)
    return list;  // О(1)
}
/*
Оценка сверху - O(1)
Точная оценка - Θ(1)
Оценка снизу - Ω(1)
все операции О(1)
*/

void appendItem(GenericList *list, void *data)
{
    if (list == NULL) {  // О(1)
        RETURN_ERROR_VOID("Пустой список");  // О(1)
    }
    if (data == NULL) {  // О(1)
        RETURN_ERROR_VOID("Пустые данные");  // О(1)
    }

    Node *new_node = malloc(sizeof(Node));  // О(1)
    if (new_node == NULL) {  // О(1)
        RETURN_ERROR_VOID("Ошибка выделения памяти");  // О(1)
    }

    new_node->data = malloc(list->elem_size);  // О(1)
    if (new_node->data == NULL) {  // О(1)
        free(new_node);  // О(1)
        RETURN_ERROR_VOID("Ошибка выделения памяти");  // О(1)
    }

    memcpy(new_node->data, data, list->elem_size);  // О(elem_size) = О(1)
    new_node->next = NULL;  // О(1)

    if (list->head == NULL) {  // О(1)
        list->head = new_node;  // О(1)

    } else {  // О(1)
        Node *x = list->head;  // О(1)
        while (x->next != NULL) {  // О(n)
            x = x->next;  // О(1)
        }
        x->next = new_node;  // О(1)
    }
}
/*
Оценка сверху - O(n) - добавление в конец списка
Точная оценка - Θ(n)
Оценка снизу - Ω(1) - если пустой цикл
так как есть цикл O(n)
*/

int findItem(GenericList *list, void *value, EqualsFunc cmp)
{
    if (list == NULL) {  // О(1)
        RETURN_ERROR("Пустой ввод", -1);  // О(1)
    }
    if (value == NULL) {  // О(1)
        RETURN_ERROR("Пустой ввод", -1);  // О(1)
    }
    if (cmp == NULL) {  // О(1)
        RETURN_ERROR("Пустой ввод", -1);  // О(1)
    }

    Node *x = list->head;  // О(1)
    int index = 0;  // О(1)

    while (x != NULL) {  // О(n)
        if (cmp(x->data, value)) {  // О(1)
            return index;  // О(1)
        }
        x = x->next;  // О(1)
        index++;  // О(1)
    } 

    return -1;  // О(1)
}
/*
Оценка сверху - O(n)
Точная оценка - Θ(n)
Оценка снизу - Ω(1) - если это head
так как есть цикл O(n)
*/

void *popItem(GenericList *list, size_t index)
{
    if (list == NULL) {  // О(1)
        RETURN_ERROR("Пустой список", NULL);  // О(1) 
    }
    if (list->head == NULL) {  // О(1)
        RETURN_ERROR("Пустой список", NULL);  // О(1) 
    }

    Node *current = list->head;  // О(1)
    Node *previous = NULL;  // О(1)
    size_t i = 0;  // О(1)

    while (current != NULL && i < index) {  // О(n)
        previous = current;  // О(1)
        current = current->next;  // О(1)
        i++;  // О(1)
    }

    if (current == NULL) {  // О(1)
        RETURN_ERROR("Выход из диапазона", NULL);  // О(1)
    }

    void *copy = malloc(list->elem_size);  // О(1)
    if (copy == NULL) {  // О(1)
        RETURN_ERROR("Ошибка выделения памяти", NULL);  // О(1) 
    }

    memcpy(copy, current->data, list->elem_size);  // О(elem_size) = О(1)
    if (previous == NULL) {  // О(1)
        list->head = current->next;  // О(1)
    } 
    else{  // О(1)
        previous->next = current->next;  // О(1)
    }
    free(current->data);  // О(1)
    free(current);  // О(1)
    return copy;  // О(1)
}
/*
Оценка сверху - O(n)
Точная оценка - Θ(n)
Оценка снизу - Ω(1) - если удаляем head
так как есть цикл O(n)
*/

void freeList(GenericList *list)
{
    if (list == NULL) {  // О(1)
        return;  // О(1)
    }
    Node *x = list->head;  // О(1)
    while (x != NULL) {  // О(n)
        Node *next = x->next;  // О(1)
        free(x->data);  // О(1)
        free(x);  // О(1)
        x = next;  // О(1)
    }
    free(list);  // О(1)
}
/*
Оценка сверху - O(n)
Точная оценка - Θ(n)
Оценка снизу - Ω(1) - если пустой список
так как есть цикл O(n)
*/

unsigned int listLength(GenericList *list)
{
    if (list == NULL) {  // О(1)
        return 0;  // О(1)
    }
    unsigned int count = 0;  // О(1)
    Node *current = list->head;  // О(1)
    while (current != NULL) {  // О(n)
        count++;  // О(1)
        current = current->next;  // О(1)
    }
    return count;  // О(1)
}
/*
Оценка сверху - O(n)
Точная оценка - Θ(n)
Оценка снизу - Ω(1) - если пустой список
так как есть цикл O(n)
*/