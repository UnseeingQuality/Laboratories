#include "matrix.h"


// Выделим матрице память
void allocate_matrix(ull rows, ull cols) {
    M = cols;
    N = rows;

    matr = malloc(M * sizeof(unsigned int*)); // Выделяем память нашему массиву
    if (!matr) {
        printf("Ошибка: не удалось выделить память для массива указателей.\n");
        exit(EXIT_FAILURE);
    }

    for (ull j = 0; j < M; j++) {
        matr[j] = malloc(N * sizeof(unsigned int)); // Выделяем память для каждого столбца
        if (!matr[j]) {
            printf("Ошибка: не удалось выделить память для столбца %llu.\n", j);
            exit(EXIT_FAILURE);
        }
    }
}

// Освобождение памяти матрицы
void free_matrix() {
    for (ull j = 0; j < M; j++) {
        free(matr[j]); // Освобождаем каждый столбец
    }
    free(matr); // Освобождаем и весь массив
}

// Заполнение матрицы случайными числами
void fill_matrix_random() {
    srand(time(NULL));
    for (ull j = 0; j < M; j++) {
        for (ull i = 0; i < N; i++) {
            matr[j][i] = rand() % mx_num; // Случайное число от 0 до максимума в типе uint
        }
    }
}

// Заполнение матрицы руками
void fill_matrix_manual() {
    printf("Введите элементы матрицы (%llu x %llu):\n", N, M);
    char *val = (char *)malloc(sizeof(unsigned int)); // Динамический буфер для ввода
    if (val == NULL){
        fprintf(stderr, "Ошибка: не получилось выделить память(");
        exit(EXIT_FAILURE);
    }

    for (ull i = 0; i < N; i++) {
        for (ull j = 0; j < M; j++) {
            printf("Элемент [%llu][%llu]: ", i + 1, j + 1);
            scanf("%s", val); 

            if (is_num(val)) { // Проверка адекватности
                matr[j][i] = (unsigned int)strtoul(val, NULL, 10); // Преобразование и запись
            } else {
                fprintf(stderr, "Ошибка: введено некорректное значение \'%s\'.\n", val);
                free(val);
                exit(EXIT_FAILURE);
            }
        }
    }

    free(val);
}



// Печать матрицы
void print_matrix() {
    for (ull i = 0; i < N; i++) {
        for (ull j = 0; j < M; j++) {
            printf("%4u ", matr[j][i]); // Обращаемся через matr[j][i] (столбец, строка)
        }
        printf("\n");
    }
}
