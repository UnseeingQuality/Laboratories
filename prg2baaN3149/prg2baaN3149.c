#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "matrix.h" // самописные заголовочные файлы
#include "valid.h"

#define ull unsigned long long

ull N; // Кол-во строк
ull M; // Кол-во столбцов
unsigned int **matr;

const unsigned char bytes_cnt = sizeof(unsigned int); // Кол-во байтов в нашем типе данных
const ull mx_num = ((ull)1<<(bytes_cnt*8))-1;


// Циклический сдвиг матрицы
void shift_matrix(ull shift) {
    ull total_elements = N * M;
    shift = shift % total_elements; // Обрезаем лишний сдвиг
    if (shift == 0) return;

    unsigned int *temp = malloc(total_elements * sizeof(unsigned int)); // Временный массив
    if (!temp) {
        printf("Ошибка: не удалось выделить память для временного массива.\n");
        exit(EXIT_FAILURE);
    }

    // Копируем элементы матрицы в одномерный временный массив (запись идем в порядке прохода полоски из примера)
    ull index = 0; // Индекс для записи в одномерный массив temp
    for (ull i = 0; i < N; i++) { // Проходим по строкам
        if (i % 2 == 0) { // Чётная строка записывается в прямом порядке
            for (ull j = 0; j < M; j++) {
                temp[index++] = matr[j][i];
            }
        } else { // Нечётная строка записывается в обратном порядке
            for (ull j = M; j > 0; j--) {
                temp[index++] = matr[j - 1][i];
            }
        }
    }

    // Выполняем циклический сдвиг в массиве (перестановка элементов в порядке хода стрелки сдвига)
    unsigned int *shifted = malloc(total_elements * sizeof(unsigned int));
    if (!shifted) {
        printf("Ошибка: не удалось выделить память для массива сдвига.\n");
        free(temp);
        exit(EXIT_FAILURE);
    }

    for (ull k = 0; k < total_elements; k++) {
        ull new_index = (k + total_elements - shift) % total_elements; // "-" так как сдвиг влево
        shifted[new_index] = temp[k];
    }

    // Отладочный вывод
    char *DEBUG = getenv("LAB2DEBUG");
    if (DEBUG) {
        fprintf(stderr, "Вывод отладочных сообщений:\n");

        fprintf(stderr, "Одномерный массив temp:\n");
        for (ull i = 0; i < total_elements; i++) {
            fprintf(stderr, "%u ", temp[i]);
        }
        fprintf(stderr, "\n");

        fprintf(stderr, "Одномерный массив shifted:\n");
        for (ull i = 0; i < total_elements; i++) {
            fprintf(stderr, "%u ", shifted[i]);
        }
        fprintf(stderr, "\n");
    }


    // Переносим shifted обратно в матрицу
    index = 0;
    for (ull i = 0; i < N; i++) { // Проходим по строкам
        if (i % 2 == 0) { // Чётная строка записывается в прямом порядке
            for (ull j = 0; j < M; j++) {
                matr[j][i] = shifted[index++];
            }
        } else { // Нечётная строка записывается в обратном порядке
            for (ull j = M; j > 0; j--) {
                matr[j - 1][i] = shifted[index++];
            }
        }
    }

    free(temp);
    free(shifted);
}



int main(int argc, char *argv[]) {
    int manual_input;

    // Прочитаем аргументы
    if (validate_arguments(argc, argv)) {
        if (argc == 4){
            N = strtoull(argv[2], NULL, 10);
            M = strtoull(argv[3], NULL, 10);
            manual_input = ((argc == 4) && (strcmp(argv[1], "-m"))) == 0;
            } else {
            manual_input = 0;
            N = strtoull(argv[1], NULL, 10);
            M = strtoull(argv[2], NULL, 10);
        }
    } else {
        return EXIT_FAILURE;
    }

    allocate_matrix(N, M); // Выделение памяти для матрицы

    if (manual_input) {
        fill_matrix_manual();
    } else {
        fill_matrix_random();
    }

    printf("Исходная матрица:\n");
    print_matrix();

    printf("Введите величину сдвига shift: ");
    ull shift;
    char *shift_input = (char *)malloc(sizeof(ull)); // Динамический буфер для ввода
    if (shift_input == NULL){
        fprintf(stderr, "Ошибка: не получилось выделить память(");
        return EXIT_FAILURE;
    }
    scanf("%s", shift_input);
    if (is_num(shift_input)) { // Проверка адекватности
        shift = (ull)strtoul(shift_input, NULL, 10); // Преобразование и запись
    } else {
        fprintf(stderr, "Ошибка: введено некорректное значение \'%s\'.\n", shift_input);
        free(shift_input);
        return EXIT_FAILURE;
    }

    shift_matrix(shift);

    printf("Матрица после сдвига:\n");
    print_matrix();


    free_matrix(); // Освобождение памяти
    return 0;
}
