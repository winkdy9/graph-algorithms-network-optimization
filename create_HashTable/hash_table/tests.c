#include "generic.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "../../lab3/macros.h"
#include "../../lab3/comparators.h"

void log_result(const char *task, const char *message) {
    printf("[%s] %s\n", task, message);
    }

int main()
{
    //Пустая таблица
    log_result("Test 1", "Starting testing...");

    HashTable *table1 = createHashTable(sizeof(int), sizeof(int));
    int key1 = 123;

    int *r1 = getItemHashTable(table1, &key1, HashInt, intEquals);
    assert(r1 == NULL);
    int *pop1 = popItemHashTable(table1, &key1, HashInt, intEquals);
    assert( pop1 == NULL);

    freeHashTable(table1);

    log_result("Test 1", "All tests passed!");

    //Один элемент
    log_result("Test 2", "Starting testing...");

    HashTable *table2 = createHashTable(sizeof(int), sizeof(int));

    int key2 = 10, value2 = 100;
    setItemHashTable(table2, &key2, &value2, HashInt, intEquals);

    int *res2 = getItemHashTable(table2, &key2, HashInt, intEquals);
    assert(res2 != NULL);
    assert(*res2 == 100);

    freeHashTable(table2);

    log_result("Test 2", "All tests passed!");
    
    //Обновление
    log_result("Test 3", "Starting testing...");
    HashTable *table3 = createHashTable(sizeof(int), sizeof(int));

    int key3 = 10, v1 = 100, v2 = 200;

    setItemHashTable(table3, &key3, &v1, HashInt, intEquals);
    setItemHashTable(table3, &key3, &v2, HashInt, intEquals);

    int *res3 = getItemHashTable(table3, &key3, HashInt, intEquals);
    assert(res3 != NULL);
    assert(*res3 == 200);

    freeHashTable(table3);
    log_result("Test 3", "All tests passed!");

    //Коллизии
    log_result("Test 4", "Starting testing...");
    HashTable *table4 = createHashTable(sizeof(int), sizeof(int));

    int k1 = 10, k2 = 20, k3 = 30;
    int a1 = 1, a2 = 2, a3 = 3;

    setItemHashTable(table4, &k1, &a1, HashInt, intEquals);
    setItemHashTable(table4, &k2, &a2, HashInt, intEquals);
    setItemHashTable(table4, &k3, &a3, HashInt, intEquals);

    int *r2 = getItemHashTable(table4, &k1, HashInt, intEquals);
    int *r3 = getItemHashTable(table4, &k2, HashInt, intEquals);
    int *r4 = getItemHashTable(table4, &k3, HashInt, intEquals);
    assert(*r2 == 1);
    assert(*r3 == 2);
    assert(*r4 == 3);

    freeHashTable(table4);
    log_result("Test 4", "All tests passed!");

    //Rehash
    log_result("Test 5", "Starting testing...");
    HashTable *table5 = createHashTable(sizeof(int), sizeof(int));

    for (int i = 0; i < 6; i++){
        int val5 = i * 10;
        setItemHashTable(table5, &i, &val5, HashInt, intEquals);
    }

    assert(table5->capacity > 10);

    for (int i = 0; i < 6; i++){
        int *res5 = getItemHashTable(table5, &i, HashInt, intEquals);
        assert(res5 != NULL);
        assert(*res5 == i * 10);
    }    

    freeHashTable(table5);
    log_result("Test 5", "All tests passed!");

    //Удаление и добавление
    log_result("Test 6", "Starting testing...");
    HashTable *table6 = createHashTable(sizeof(int), sizeof(int));

    int key6 = 10, b1 = 100, b2 = 200;

    setItemHashTable(table6, &key6, &b1, HashInt, intEquals);

    int *p1 = popItemHashTable(table6, &key6, HashInt, intEquals);
    assert(p1 != NULL);
    assert(*p1 == 100);
    free(p1);

    int *r5 = getItemHashTable(table6, &key6, HashInt, intEquals);
    assert(r5 == NULL);

    setItemHashTable(table6, &key6, &b2, HashInt, intEquals);
    int *r6 = getItemHashTable(table6, &key6, HashInt, intEquals);
    assert(*r6 == 200);

    freeHashTable(table6);
    log_result("Test 6", "All tests passed!");

    //Некорректные данные
    log_result("Test 7", "Starting testing...");
    HashTable *table7 = createHashTable(sizeof(int), sizeof(int));

    int key7 = 5, val7 = 100;
    setItemHashTable(table7, NULL, &val7, HashInt, intEquals);
    setItemHashTable(table7, &key7, NULL, HashInt, intEquals);
    setItemHashTable(table7, &key7, &val7, NULL, intEquals);
    setItemHashTable(table7, &key7, &val7, HashInt, NULL);

    int *r7 = getItemHashTable(NULL, &key7, HashInt, intEquals);
    assert(r7 == NULL);
    assert(popItemHashTable(NULL, &key7, HashInt, intEquals) == NULL);

    freeHashTable(table7);
    log_result("Test 7", "All tests passed!");

    //Базовый тест
    log_result("Test 8", "Starting testing...");

    HashTable *table8 = createHashTable(sizeof(int), sizeof(int));

    int q1 = 1, w1 = 10;
    int q2 = 2, w2 = 20;

    setItemHashTable(table8, &q1, &w1, HashInt, intEquals);
    setItemHashTable(table8, &q2, &w2, HashInt, intEquals);

    int *r8 = getItemHashTable(table8, &q1, HashInt, intEquals);
    int *r9 = getItemHashTable(table8, &q2, HashInt, intEquals);
    assert(*r8 == 10);
    assert(*r9 == 20);

    w1 = 100;
    setItemHashTable(table8, &q1, &w1, HashInt, intEquals);
    int *r10 = getItemHashTable(table8, &q1, HashInt, intEquals);
    assert(*r10 == 100);

    int *p = popItemHashTable(table8, &q2, HashInt, intEquals);
    assert(p != NULL);
    assert(p && *p == 20);
    free(p);

    int *r11 = getItemHashTable(table8, &q2, HashInt, intEquals);
    assert(r11 == NULL);

    int w3 = 777;
    setItemHashTable(table8, &q2, &w3, HashInt, intEquals);
    int *r12 = getItemHashTable(table8, &q2, HashInt, intEquals);
    assert(*r12 == 777);

    freeHashTable(table8);

    log_result("Test 8", "All tests passed!");

    log_result("Test 9", "Starting testing...");

    // Строки
    HashTable *ts = createHashTable(256, sizeof(int));

    char s1[256] = "aaa";
    char s2[256] = "bbb";
    char s3[256] = "ccc";

    int v1s = 10;
    int v2s = 20;
    int v3s = 30;

    setItemHashTable(ts, s1, &v1s, HashString, stringEquals);
    setItemHashTable(ts, s2, &v2s, HashString, stringEquals);
    setItemHashTable(ts, s3, &v3s, HashString, stringEquals);

    int *t1 = getItemHashTable(ts, s1, HashString, stringEquals);
    int *t2 = getItemHashTable(ts, s2, HashString, stringEquals);
    int *t3 = getItemHashTable(ts, s3, HashString, stringEquals);

    assert(t1 && *t1 == 10);
    assert(t2 && *t2 == 20);
    assert(t3 && *t3 == 30);

    int nv = 777;
    setItemHashTable(ts, s2, &nv, HashString, stringEquals);
    int *t4 = getItemHashTable(ts, s2, HashString, stringEquals);
    assert(t4 && *t4 == 777);

    int *pop_s = popItemHashTable(ts, s1, HashString, stringEquals);
    assert(pop_s && *pop_s == 10);
    free(pop_s);

    int *t5 = getItemHashTable(ts, s1, HashString, stringEquals);
    assert(t5 == NULL);

    int vnew = 999;
    setItemHashTable(ts, s1, &vnew, HashString, stringEquals);
    int *t6 = getItemHashTable(ts, s1, HashString, stringEquals);
    assert(t6 && *t6 == 999);

    freeHashTable(ts);

    log_result("Test 9", "All tests passed!");

    return 0;
}
