#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>  // Для функции isdigit()

// Функция для вывода числа в двоичном формате
void printbin(unsigned int num) {
    for (int i = 0; i < sizeof(num) * 8; i++) {
        if (i % 8 == 0 && i != 0) printf(" ");
        printf("%d", (num & (1 << (31 - i))) ? 1 : 0);
    }
    printf("\n");
}

// Функция для реверсирования 6-битных последовательностей
unsigned int reverse6bits(unsigned int num) {
    unsigned int maxvalue = 0;
    int max_start_index = -1;

    // Проходим по всем 6-битным последовательностям
    for (int i = 0; i <= sizeof(num) * 8 - 6; i++) {
        unsigned int value = (num >> i) & 0x3F; // 0x3F = 111111
        if (value > maxvalue) {
            maxvalue = value;
            max_start_index = i;
        }
    }

    // Инвертируем найденную последовательность
    if (max_start_index != -1) {
        unsigned int mask = 0x3F << max_start_index;
        unsigned int reversed = ((maxvalue & 0x01) << 5) |
                                ((maxvalue & 0x02) << 3) |
                                ((maxvalue & 0x04) << 1) |
                                ((maxvalue & 0x08) >> 1) |
                                ((maxvalue & 0x10) >> 3) |
                                ((maxvalue & 0x20) >> 5);
        num = (num & ~mask) | (reversed << max_start_index);
    }

    return num;
}

// Функция для проверки, что строка состоит только из цифр
bool is_numeric(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    unsigned int num;

    // Инициализация генератора случайных чисел
    srand((unsigned int)time(NULL));

    if (argc == 2) {
        // Проверяем, что аргумент содержит только цифры
        if (!is_numeric(argv[1])) {
            fprintf(stderr, "Ошибка: '%s' не является корректным числом.\n", argv[1]);
            return 1;
        }

        // Преобразуем строку в число
        sscanf(argv[1], "%u", &num);
    } else {
        // Генерируем случайное число, если аргумент не передан
        num = rand();
    }

    // Выводим число до преобразования
    printbin(num);

    // Преобразуем число
    num = reverse6bits(num);

    // Выводим число после преобразования
    printbin(num);

    return 0;
}
