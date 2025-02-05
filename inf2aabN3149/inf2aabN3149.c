#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


unsigned long long power(unsigned long long base, int exp) {
    unsigned long long result = 1;

    for (int i = 0; i < exp; i++) {
        result *= base;
    }

    return result;
}


void print_binary(uint16_t num) {
    for (int i = sizeof(num) * 8 - 1; i >= 0; i--) {
        if (i % 8 == 0){
            printf("%d ", (num >> i) & 1);
            } // Сдвиг вправо и маскирование последнего бита
        else {
            printf("%d", (num >> i) & 1);
        }
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    // Проверка запуска с переменной среды, включающей отладочный вывод.
    // Пример запуска с установкой переменной LAB1DEBUG в 1:
    // $ LAB1DEBUG=1 ./lab1abcNXXXXX 123
    long mx_num;
    unsigned char bytes_cnt;
    uint16_t res = 0;
    uint16_t num;

    char *DEBUG = getenv("LAB1DEBUG");


    bytes_cnt = sizeof(uint16_t); // кол-во байтов в нашем типе данных
    mx_num = power(2,bytes_cnt*8)-1;
    srand(time(NULL));

    if (argc != 2) { // Если ввели не два аргумента, то и работать не с чем
        num = rand() % mx_num;
        printf("Число не введено - сгенерирую случайное %d \n", num);
        printf("А вот оно в двоичной системе: ");
        print_binary(num);
            
    }
    else {
        long temp = strtol(argv[1], NULL, 10); // строку в число
        if (strcmp(argv[1], "0") != 0 && temp == 0){ // если введен не ноль, а strtol вернула ноль
            fprintf(stderr, "Ошибка: \'%s\' не является числом.\n", argv[1]);
            return EXIT_FAILURE;
            }
        else if ((0 <= temp) && (temp < mx_num)) { // входит ли число в диапазон типа данных
            num = (uint16_t)temp;
            printf("Ввод прошел успешно, Вы ввели %d !\n", num);
            printf("А вот оно в двоичной системе: ");
            print_binary(num);
            }
        else{
            fprintf(stderr, "Ошибка: \'%s\' не является числом типа unsigned short(от 0 до 65535).\n", argv[1]);
            return EXIT_FAILURE;
            }
    }

    
    for(unsigned char cur_byte_id = 1; cur_byte_id <= bytes_cnt; cur_byte_id++) { // Будем идти по каждому байту числа и работать с ним отдельно
        uint16_t cur_byte;
        unsigned char new_byte;
        unsigned char shift;
        unsigned char tetra_bits_cnt;
        unsigned char l_bit_mask;
        unsigned char sen_tetra;
        unsigned char jun_tetra;
        bool hig_bit;
        
        cur_byte = num % ( 1 << (8 * cur_byte_id)); // для этого отрежем все байты левее нашего с помощью остатка по модулю (если работаем со 2 и левее байтом, то нужно будет убрать хвост из байтов правее)
        new_byte = cur_byte >> (8 * (cur_byte_id-1)); // если работаем со >=2 байтом (справа налево), то его нужно сдвинуть до первого байта
        printf("current byte: %d \n", new_byte);
        print_binary(new_byte);

        shift = new_byte & 0xC0; // применим на байт побитовую коньюкцию с 1100_0000 чтобы остались только 2 старших бита
        shift >>= 6; // так как условный shift=0100_0000 даст сдвиг на 1, то сдвинем его налево как раз на 6
        tetra_bits_cnt = 4;
        l_bit_mask = 1 << (tetra_bits_cnt-1); // по номеру старшего бита младшей тетрады ставим "1", получаем маску 0000_1000
        sen_tetra = new_byte & 0xF0; // запомним старшую тетраду с помощью побитового И с 1111_0000

        for (unsigned char i = 0; i < shift; i++){
                hig_bit = (bool) (new_byte & l_bit_mask); // запоминаем старший бит для дальнейшего вдвигяния в младший
                new_byte <<= 1; 
                new_byte |= hig_bit; // применяя побитовое "или" мы как раз вдвигаем в младший бит 1 или 0
        }
        jun_tetra = new_byte & 0x0F; // Выделяем изменненную младшую тетраду
        new_byte = sen_tetra | jun_tetra;

        printf("После сдвига на %d байт стал равен %d \n", shift, new_byte);
        print_binary(new_byte);
        printf("\n");

        res += (new_byte << (8 * (cur_byte_id-1))); // на сколько изначально сдвигали вправо, настолько и вдвигаем влево, чтобы порядок новых байтов был правильным
    }


    if (DEBUG) {
    fprintf(stderr, "Включен вывод отладочных сообщений\n");
    fprintf(stderr,"Исходное число: %d\n",num);
    print_binary(num);
    fprintf(stderr,"Результат: %hu \n",res);
    print_binary(res);
    }
    else {
        printf("\n В итоге мы получили %hu \n", res);
        print_binary(res);
    }

    return EXIT_SUCCESS;


}

