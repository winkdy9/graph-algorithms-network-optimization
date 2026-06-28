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

typedef struct{
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

    log_result("Test 1", "Starting testing...");

    //int
    Vector *v = createVector(sizeof(int));
    assert(v != NULL);

    //appendVectorItem
    int a = 10;
    int b = 20;
    int c = 30;
    appendVectorItem(v, &a);
    appendVectorItem(v, &b);
    appendVectorItem(v, &c);

    //getVectorItem
    assert(v->size == 3);
    assert(*(int *)getVectorItem(v, 0) == 10);
    assert(*(int *)getVectorItem(v, 2) == 30);

    //setVectorItem
    int newValue = 99;
    setVectorItem(v, 1, &newValue);
    assert(*(int *)getVectorItem(v, 1) == 99);

    //findVectorItem
    assert(findVectorItem(v, &a, intEquals) == 0);
    assert(findVectorItem(v, &b, intEquals) == -1);
    assert(findVectorItem(v, &newValue, intEquals) == 1);

    // popVectorItem
    int *popInt = (int *)popVectorItem(v, 0);
    assert(*popInt == 10);
    free(popInt);
    assert(v->size == 2);

    vectorFree(v);

    log_result("Test 1", "All tests passed!");

    log_result("Test 2", "Starting testing...");

    //float
    v = createVector(sizeof(float));

    //appendVectorItem and getVectorItem
    float f1 = 1.1;
    float f2 = 2.2;
    float f3 = 3.3;
    appendVectorItem(v, &f1);
    appendVectorItem(v, &f2);
    appendVectorItem(v, &f3);
    assert(v->size == 3);
    assert(*(float *)getVectorItem(v, 1) == 2.2f);

    //setVectorItem
    float f4 = 9.9;
    setVectorItem(v, 2, &f4);
    assert(*(float *)getVectorItem(v, 2) == 9.9f);

    //findVectorItem
    assert(findVectorItem(v, &f1, floatEquals) == 0);
    assert(findVectorItem(v, &f3, floatEquals) == -1);

    //popVectorItem
    float *popF = (float *)popVectorItem(v, 1);
    assert(*popF == 2.2f);
    free(popF);

    vectorFree(v);

    log_result("Test 2", "All tests passed!");

    log_result("Test 3", "Starting testing...");

    // char
    v = createVector(sizeof(char *));
    char *s1 = "a";
    char *s2 = "b";
    char *s3 = "c";
    appendVectorItem(v, &s1);
    appendVectorItem(v, &s2);
    appendVectorItem(v, &s3);
    assert(v->size == 3);
    assert(strcmp(*(char **)getVectorItem(v, 1), "b") == 0);

    char *w = "aa";
    setVectorItem(v, 1, &w);
    assert(strcmp(*(char **)getVectorItem(v, 1), "aa") == 0);

    assert(findVectorItem(v, &s1, stringEquals) == 0);
    assert(findVectorItem(v, &s2, stringEquals) == -1);
    assert(findVectorItem(v, &w, stringEquals) == 1);

    char **popS = (char **)popVectorItem(v, 2);
    assert(strcmp(*popS, "c") == 0);
    free(popS);

    vectorFree(v);
    log_result("Test 3", "All tests passed!");

    log_result("Test 4", "Starting testing...");

    v = createVector(sizeof(Cat));
    Cat c1 = {"Кошка", 3, 4.1};
    Cat c2 = {"Кот", 5, 5.2};
    Cat c3 = {"Котенок", 1, 2.0};

    appendVectorItem(v, &c1);
    appendVectorItem(v, &c2);
    appendVectorItem(v, &c3);
    assert(v->size == 3);

    Cat q = {"Tom", 2, 3.3};
    setVectorItem(v, 1, &q);
    Cat *get = (Cat *)getVectorItem(v, 1);
    assert(strcmp(get->name, "Tom") == 0);

    assert(findVectorItem(v, &c1, structEquals) == 0);
    assert(findVectorItem(v, &c2, structEquals) == -1);
    assert(findVectorItem(v, &q, structEquals) == 1);

    Cat *popC = (Cat *)popVectorItem(v, 2);
    assert(structEquals(popC, &c3));
    free(popC);
    assert(v->size == 2);

    vectorFree(v);
    log_result("Test 4", "All tests passed!");

    //тесты на ошибки
    log_result("Test 5", "Starting testing...");

    Vector *v1 = createVector(0);
    assert(v1 == NULL);

    v = createVector(sizeof(int));
    appendVectorItem(NULL, &a);
    appendVectorItem(v, NULL);
    getVectorItem(v, 5);
    getVectorItem(NULL, 0);
    setVectorItem(v, 3, &a);
    setVectorItem(NULL, 0, &a);
    popVectorItem(v, 10);
    popVectorItem(NULL, 0);
    findVectorItem(NULL, &a, intEquals);
    vectorFree(NULL);

    vectorFree(v);
    log_result("Test 5", "All tests passed!");

    // нагрузочные тесты
    log_result("Test 6", "Starting testing...");

    const int N1 = 100000;
    const int N2 = 1000000;

    Vector *big = createVector(sizeof(int));

    for (int i = 0; i < N1; i++) {
        appendVectorItem(big, &i);
    }
    assert(big->size == N1);

    assert(*(int *)getVectorItem(big, 0) == 0);
    assert(*(int *)getVectorItem(big, N1 - 1) == N1 - 1);

    for (int i = N1; i < N2; i++) {
        appendVectorItem(big, &i);
    }
    assert(big->size == N2);

    int mid = N2 / 2;
    assert(*(int *)getVectorItem(big, mid) == mid);

    vectorFree(big);
    log_result("Test 6", "All tests passed!");

    return 0;
}
