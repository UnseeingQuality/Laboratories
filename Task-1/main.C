#include <stdlib.h>
#include <stdio.h>
// Добавьте необходимые заголовочные файлы
#include <math.h>

// Структура S
typedef struct S {
    short x;
    float y;
    char z;
} S;


// TODO: Реализовать функцию init_array(). См. вызов ниже. Подумайте, какой должен быть заголовок.
void init_array(S *arr, size_t arr_size) {
    for (size_t i = 0; i < arr_size; i++) {
        arr[i].x = rand() % (2 << sizeof(short) * 8);
        arr[i].y = (float)rand() / RAND_MAX * 100;
        arr[i].z = rand() % (2 << sizeof(char) * 8);
    }
}

// TODO: Реализовать функцию print_array(). См. вызов ниже. Подумайте, какой должен быть заголовок.
void print_array(S *arr, size_t arr_size) {
    for (size_t i = 0; i < arr_size; i++) {
        printf("arr[%zu]: x = %d, y = %f, z = %d\n", i, arr[i].x, arr[i].y, arr[i].z);
    }
}

int compare(const void *a, const void *b) {
    S *s1 = (S *)a;
    S *s2 = (S *)b;
    return (trunc(s2->y) - trunc(s1->y));
}

// TODO: Реализовать функцию sort_array(). См. вызов ниже. Подумайте, какой должен быть заголовок.
void sort_array(S *arr, size_t arr_size) {
    qsort(arr, arr_size, sizeof(S), compare);
}

int main(int argc, char *argv[]) {
    size_t arr_size = 0;
    // 1. TODO: Проверить аргументы командной строки и получить размер массива.
    if (argc != 2) {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }
    arr_size = atoi(argv[1]);
    if (arr_size <= 0) {
        printf("Invalid array size!\n");
        return 1;
    }

    // S *arr = NULL;
    // 2. TODO: Выделить память из кучи для массива. Указатель arr должен
    // указывать на начало массива.
    S *arr = (S *)malloc(arr_size * sizeof(S));
    if (arr == NULL) {
        printf("Memory malloc() error\n");
        return 1;
    }
    
    // 3. Заполнить массив структур случайными значениями
    init_array(arr, arr_size);
    printf("Initial array: \n");
    // 4. Вывести массив на экран
    print_array(arr, arr_size);
    // 5. Отсортировать массив по заданному критерию
    sort_array(arr, arr_size);
    printf("\n Sorted array: \n");
    // 6. Вывести отсортированный массив на экран
    print_array(arr, arr_size);
    
    return 0;
}