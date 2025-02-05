#include "valid.h"

bool is_num(const char *var) {
    if (!var) {
        fprintf(stderr, "Ошибка: передан NULL вместо строки.\n");
        return false;
    }

    char *endptr;
    long long temp = strtol(var, &endptr, 10);

    // Проверяем, что вся строка была преобразована
    if (*endptr != '\0') {
        fprintf(stderr, "Ошибка: \'%s\' содержит недопустимые символы.\n", var);
        return false;
    }

    // Проверяем диапазон
    if (temp < 0 || (ull)temp >= mx_num) {
        fprintf(stderr, "Ошибка: число \'%s\' выходит за допустимый диапазон.\n", var);
        return false;
    }

    return true;
}


bool validate_arguments(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Ошибка: недостаточно аргументов. Ожидается: [-m] <число строк> <число столбцов>\n");
        return false;
    }

    if (argc ==3) {
        // Проверяем, что оба аргумента - числа
        if (!is_num(argv[1]) || !is_num(argv[2])) {
            fprintf(stderr, "Ошибка: строки и столбцы должны быть числами.\n");
            return false;
        }
    } else if (argc == 4) {
        if (!is_num(argv[2]) || !is_num(argv[3])) {
            fprintf(stderr, "Ошибка: строки и столбцы должны быть числами.\n");
            return false;
        } 
        if (strcmp(argv[1], "-m")) {
            fprintf(stderr, "Ошибка: нет опции %s", argv[1]);
            return false;
        }
    } else {
        fprintf(stderr, "Ошибка: слишком много аргументов. Ожидается: [-m] <число строк> <число столбцов>\n");
        return false;
    }

    // Преобразуем для проверки границ
    unsigned long long rows;
    unsigned long long cols;
    if (argc == 3){
        rows = strtoull(argv[1], NULL, 10);
        cols = strtoull(argv[2], NULL, 10);
    } else{
        rows = strtoull(argv[2], NULL, 10);
        cols = strtoull(argv[3], NULL, 10);
    }

    if (rows == 0 || cols == 0) {
        fprintf(stderr, "Ошибка: строки и столбцы должны быть положительными числами.\n");
        return false;
    }

    return true;
}
