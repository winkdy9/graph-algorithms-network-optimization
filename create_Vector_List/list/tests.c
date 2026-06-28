#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "generic.h"
#include "../macros.h"
#include "../comparators.h"

void log_result(const char *task, const char *message) {
    printf("[%s] %s\n", task, message);
}


typedef struct {
    char name[32];
    int age;
    float weight;
} Cat;

int structEquals(const void *a, const void *b) {
    const Cat *c1 = (const Cat *)a;
    const Cat *c2 = (const Cat *)b;
    return strcmp(c1->name, c2->name) == 0 &&
           c1->age == c2->age &&
           c1->weight == c2->weight;
}

int main() {
    // ====== appendItem and listLength and freeList ======
    log_result("Test 1", "Starting testing...");

    GenericList *list = createList(sizeof(int));
    int x = 10;
    appendItem(list, &x);
    assert(listLength(list) == 1);

    int a = 1;
    int b = 2;
    int c = 3;
    appendItem(list, &a);
    appendItem(list, &b);
    appendItem(list, &c);
    assert(listLength(list) == 4);

    int count = 0;
    Node *current = list->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    assert(count == listLength(list));

    freeList(list);

    list = createList(sizeof(float));
    float q = 1.0;
    float w = 2.2;
    float e = 3.9;
    appendItem(list, &q);
    appendItem(list, &w);
    appendItem(list, &e);
    assert(listLength(list) == 3);

    count = 0;
    current = list->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    assert(count == listLength(list));

    freeList(list);

    list = createList(sizeof(char *));
    char *k = "ab";
    char *l = "bc";
    char *m = "cd";
    appendItem(list, &k);
    appendItem(list, &l);
    appendItem(list, &m);
    assert(listLength(list) == 3);
    count = 0;
    current = list->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    assert(count == listLength(list));

    freeList(list);

    list = createList(sizeof(Cat));
    Cat c1 = {"Кошка", 3, 4.2};
    Cat c2 = {"Кот", 5, 5.5};
    Cat c3 = {"Котенок", 2, 3.8};
    appendItem(list, &c1);
    appendItem(list, &c2);
    appendItem(list, &c3);
    assert(listLength(list) == 3);

    count = 0;
    current = list->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    assert(count == listLength(list));

    freeList(list);

    log_result("Task 1", "All tests passed!");

     // ====== findItem ======
    log_result("Test 2", "Starting testing...");
    list = createList(sizeof(int));
    appendItem(list, &a);
    appendItem(list, &b);
    appendItem(list, &c);
    assert(findItem(list, &a, intEquals) == 0);
    assert(findItem(list, &b, intEquals) == 1);
    assert(findItem(list, &c, intEquals) == 2);
    assert(findItem(list, &x, intEquals) == -1);
    freeList(list);

    list = createList(sizeof(float));
    float f1 = 1.111;
    appendItem(list, &q);
    appendItem(list, &w);
    appendItem(list, &e);
    assert(findItem(list, &q, floatEquals) == 0);
    assert(findItem(list, &w, floatEquals) == 1);
    assert(findItem(list, &e, floatEquals) == 2);
    assert(findItem(list, &f1, floatEquals) == -1);
    freeList(list);

    list = createList(sizeof(char *));
    char *a1 = "aaaaaa";
    appendItem(list, &k);
    appendItem(list, &l);
    appendItem(list, &m);
    assert(findItem(list, &k, stringEquals) == 0);
    assert(findItem(list, &l, stringEquals) == 1);
    assert(findItem(list, &m, stringEquals) == 2);
    assert(findItem(list, &a1, stringEquals) == -1);
    freeList(list);

    list = createList(sizeof(Cat));
    appendItem(list, &c1);
    appendItem(list, &c2);
    appendItem(list, &c3);
    assert(findItem(list, &c1, structEquals) == 0);
    assert(findItem(list, &c2, structEquals) == 1);
    assert(findItem(list, &c3, structEquals) == 2);
    assert(findItem(list, &a1, structEquals) == -1);
    freeList(list);

    log_result("Task 2", "All tests passed!");


    // ===== popItem =====
    log_result("Test 3", "Starting testing...");

    list = createList(sizeof(int));
    appendItem(list, &a);
    appendItem(list, &b);
    appendItem(list, &c);
    int *poppedInt = (int *)popItem(list, 1);
    assert(*poppedInt == 2);
    free(poppedInt);
    assert(listLength(list) == 2);
    freeList(list);

    list = createList(sizeof(float));
    appendItem(list, &q);
    appendItem(list, &w);
    appendItem(list, &e);
    float *poppedFloat = (float *)popItem(list, 0);
    assert(*poppedFloat == 1.0f);
    free(poppedFloat);
    assert(listLength(list) == 2);
    freeList(list);

    list = createList(sizeof(char *));
    appendItem(list, &k);
    appendItem(list, &l);
    appendItem(list, &m);
    char **poppedStr = (char **)popItem(list, 2);
    assert(strcmp(*poppedStr, m) == 0);
    free(poppedStr);
    assert(listLength(list) == 2);
    freeList(list);

    list = createList(sizeof(Cat));
    appendItem(list, &c1);
    appendItem(list, &c2);
    appendItem(list, &c3);
    Cat *poppedCat = (Cat *)popItem(list, 0);
    assert(structEquals(poppedCat, &c1));
    free(poppedCat);
    assert(listLength(list) == 2);
    freeList(list);

    log_result("Task 3", "All tests passed!");

    //Тесты на ошибки
    int y = 100;
    appendItem(NULL, &y);
    findItem(NULL, &y, NULL);
    popItem(NULL, 0);
    assert(listLength(NULL) == 0);
    freeList(NULL); 
    list = createList(sizeof(int));
    appendItem(list, NULL);
    findItem(list, &y, NULL);

    //нагрузочные тесты

    /*log_result("Test 4", "Starting testing...");
    list = createList(sizeof(int));
    int N1 = 100000;
    int N2 = 1000000;
    for (int i = 0; i < N1; i++) {
        appendItem(list, &i);
    }
    assert(listLength(list) == N1);
    freeList(list);
    log_result("Task 4.1", "All tests passed!");
    list = createList(sizeof(int));
    for (int i = 0; i < N2; i++) {
        appendItem(list, &i);
    }
    assert(listLength(list) == N2);
    freeList(list);
    log_result("Task 4.2", "All tests passed!");*/

    return 0;
}