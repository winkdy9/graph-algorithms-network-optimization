#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "generic.h"
#include "../macros.h"
#include "../comparators.h"

const int MEM_BORDER = 3;

// Вспомогательная функция для изменения размера
static bool needToResize(Vector *vector, bool *increase)  // O(1), Θ(1), Ω(1)
{
    if (vector==NULL) {  // O(1)
        RETURN_ERROR("Пустой ввод", false);  // O(1)
    }
    if (increase == NULL) {  // O(1)
        RETURN_ERROR("Пустой ввод", false);  // O(1)
    }

    if (vector->capacity <= vector->size) {  // O(1)
        *increase = true;  // O(1)
        return true;  // O(1)
    }
    if (vector->size > 0 && vector->capacity > MIN_SIZE) {  // O(1)
        if (vector->capacity < (vector->size / MEM_BORDER)) {  // O(1)
            *increase= false;  // O(1)
            return true;  // O(1)
        }
    }
    return false;  // O(1)
}

// Определяем увеличивать размер или уменьшать
static int resize(Vector *vector, bool increase)  // O(n), Θ(n), Ω(1) - если нулевой вектор
{
    if (vector == NULL) {  // O(1)
        RETURN_ERROR("Пустой ввод", -1);  // O(1)
    }
    size_t new_capacity;  // O(1)

    if (increase) {  // O(1)
        new_capacity = vector->capacity * MEM_BORDER;  // O(1)
    }
    else {  // O(1)
        new_capacity = vector->capacity / MEM_BORDER;  // O(1)
    }

    void *new_data = realloc(vector->data, new_capacity * (vector->elem_size));  // O(n)
    if (new_data== NULL) {  // O(1)
        RETURN_ERROR("Ошибка изменения размера", -1);  // O(1)
    }

    vector->data  = new_data;  // O(1)
    vector->capacity = new_capacity;  // O(1)
    return 0;  // O(1)
}

Vector *createVector(size_t elem_size)  // O(1), Θ(1), Ω(1)
{
    if (elem_size == 0) {
        RETURN_ERROR("Нулевой вектор", NULL);
    }
    Vector *v = malloc(sizeof(Vector));
    if (v == NULL) {
        RETURN_ERROR("Ошибка выделения памяти", NULL);
    }
    v->elem_size = elem_size;
    v->size = 0;
    v->capacity = MIN_SIZE;
    v->data = malloc(v->capacity * v->elem_size);
    if (v->data == NULL) {
        free(v);
        RETURN_ERROR("Ошибка выделения памяти", NULL);
    }
    return v;
}

int appendVectorItem(Vector *vector, void *el)
{
    if (vector == NULL) {  // O(1)
        RETURN_ERROR("Нулевой вектор", -1);  // O(1)
    }
    if (el == NULL) {  // O(1)
        RETURN_ERROR("Пустой ввод", -1);  // O(1)
    }
    bool increase = false;  // O(1)
    if (needToResize(vector, &increase)) {  // O(1)
        if (increase) {  // O(1)
            resize(vector, increase);  // O(n)
        }
    }
    void *y = (char *)vector->data + vector->size * vector->elem_size;  // O(1)
    memcpy(y,el, vector->elem_size);  // O(1)
    vector->size++;  // O(1)
    return 0;  // O(1)
}
/*
Оценка сверху - O(1)
Точная оценка - Θ(1)
Оценка снизу - Ω(1)
resize имеет сложность O(n), но он выполняется только тогда, когда size == capacity, 
а capacity это степень двойки. Поэтому resize выполняется редко (примерно log n за n вставок)
и значит оценка сверху - О(1)
*/

void *getVectorItem(Vector *vector, size_t index)
{
    if (vector == NULL) {  // O(1)
        RETURN_ERROR("Нулевой вектор", NULL);  // O(1)
    }
    if (index >= vector->size) {  // O(1)
        RETURN_ERROR("Выход за пределы диапазона", NULL);  // O(1)
    }
    return (char *)vector->data + index * vector->elem_size;  //O(1)
}
/*
Оценка сверху - O(1)
Точная оценка - Θ(1)
Оценка снизу - Ω(1)
все операции О(1))
*/

int setVectorItem(Vector *vector, size_t index, void *value)
{
    if (vector == NULL)  // O(1)
        RETURN_ERROR("Нулевой вектор", -1);  // O(1)
    if (value == NULL)  // O(1)
        RETURN_ERROR("Пустой ввод", -1);  // O(1)
    if (index >= vector->size)  // O(1)
        RETURN_ERROR("Выход за пределы диапазона", -1);  // O(1)

    void *x = (char *)vector->data + index * vector->elem_size;  // O(1)
    memcpy(x, value, vector->elem_size);  // O(1)
    return 0;  // O(1)
}
/*
Оценка сверху - O(1)
Точная оценка - Θ(1)
Оценка снизу - Ω(1)
все операции О(1))
*/

void *popVectorItem(Vector *vector, size_t index)
{
    if (vector == NULL) {  // O(1)
        RETURN_ERROR("Пустой ввод", NULL);  // O(1)
    }
    if (index>= vector->size) {  // O(1)
        RETURN_ERROR("Выход за предел диапазона", NULL);  // O(1)
    }
    void *copy = malloc(vector->elem_size);  // O(1)
    if (copy == NULL) {  // O(1)
        RETURN_ERROR("Ошибка выделения памяти", NULL);  // O(1) 
    }

    void *x = (char *)vector->data + index*(vector->elem_size);  // O(1)
    memcpy(copy, x, vector->elem_size);  // O(1)

    if (index < vector->size - 1) {  // O(1)
        memmove(x, (char *)vector->data + (index + 1) * vector->elem_size, (vector->size - index - 1) * vector->elem_size);
        // O(n) - перемещение n-1 элементов
    }

    vector->size--;  // O(1)

    bool increase = false;  // O(1)
    if (needToResize(vector, &increase)) {  // O(1)
        if (!increase) {  // O(1)
            resize(vector, increase);  // O(n)
        }
    }
    return copy;  // O(1)
}
/*
Оценка сверху - O(n)
Точная оценка - Θ(n)
Оценка снизу - Ω(1) - удаление первого элемента
memmove имеет сложность O(n), а resize выполняется очень редко
*/

long int findVectorItem(Vector *vector, void *value, EqualsFunc cmp)
{
    if (vector == NULL) {  // O(1)
        RETURN_ERROR("Пустой ввод", -1);  // O(1)
    }
    if (value == NULL) {  // O(1)
        RETURN_ERROR("Пустой ввод", -1);  // O(1)
    }
    if (cmp == NULL) {  // O(1)
        RETURN_ERROR("Пустой ввод", -1);  // O(1)
    }

    for (size_t i = 0; i < vector->size; i++) {  // O(n)
        void *el = (char *)vector->data + i * vector->elem_size;  // O(1)
        if (cmp(el, value)) {  // O(1)
            return (long int)i;  // O(1)
        }
    }
    return -1;  // O(1)
}
/*
Оценка сверху - O(n)
Точная оценка - Θ(n)
Оценка снизу - Ω(1) - если это первый элемент
имеет сложность O(n), так как проходится по циклу длины n
*/

int vectorFree(Vector *vector)
{
    if (vector == NULL) {  // O(1)
        RETURN_ERROR("Пустой массив", -1);  // O(1)
    }
    free(vector->data);  // O(1)
    free(vector);  // O(1)
}
/*
Оценка сверху - O(1)
Точная оценка - Θ(1)
Оценка снизу - Ω(1)
все операции О(1))
*/
