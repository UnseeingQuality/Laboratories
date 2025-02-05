#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

void print_usage() {
    fprintf(stderr, "Использование: prg3aabN3149 [опции] [имя_вход_файла [имя_выход_файла]]\n");
    fprintf(stderr, "Опции:\n");
    fprintf(stderr, "  -v          Вывести информацию о студенте и варианте\n");
    fprintf(stderr, "  -c          Использовать цвет для выделения\n");
    fprintf(stderr, "  -n          Искать только в пределах одной строки\n");
    fprintf(stderr, "  -f=M        Начать с M-го найденного объекта (нумерация с 1)\n");
    fprintf(stderr, "  -t=N        Закончить на N-м найденном объекте (нумерация с 1)\n");
}

bool is_valid_quoted_string(const char *str) {
    if (*str != '\'') return false;

    const char* p = str + 1;
    while (*p) {
        if (*p == '\'') {
            if (*(p - 1) == '\\') {

                int backslash_count = 0;
                const char* temp = p - 1;
                while (temp >= str && *temp == '\\') {
                    backslash_count++;
                    temp--;
                }
                if (backslash_count % 2 == 0) {
                    p++;
                    continue;
                } else {
                    return *(p + 1) == '\0' || *(p + 1) == '\n';
                }
            } else {
                return *(p + 1) == '\0' || *(p + 1) == '\n';
            }
        }
        p++;
    }
    return false;
}
char *read_entire_input(FILE *input) {
    size_t buffer_size = 1024;
    char *buffer = malloc(buffer_size);
    size_t length = 0;

    if (!buffer) {
        perror("Ошибка выделения памяти");
        return NULL;
    }

    while (!feof(input)) {
        if (length + 1024 >= buffer_size) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                perror("Ошибка выделения памяти");
                return NULL;
            }
        }

        size_t read = fread(buffer + length, 1, 1024, input);
        length += read;
    }

    buffer[length] = '\0';
    return buffer;
}

void process_line(const char *line, bool use_color, int start, int end, int *count, FILE *output) {
    const char *p = line;
    while (*p) {
        while (*p && *p != '\'') {
            fputc(*p, output);
            p++;
        }

        if (*p == '\'') {
            const char *start_ptr = p;
            p++;

            while (*p) {
                if (*p == '\'') {
                    if (*(p - 1) == '\\') {
                        int backslash_count = 0;
                        const char* temp = p - 1;
                        while (temp >= start_ptr && *temp == '\\') {
                            backslash_count++;
                            temp--;
                        }
                        if (backslash_count % 2 == 0) {
                            p++;
                            continue;
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                p++;
            }

            if (*p == '\'') {
                char *quoted_string = malloc(p - start_ptr + 2);
                strncpy(quoted_string, start_ptr, p - start_ptr + 1);
                quoted_string[p - start_ptr + 1] = '\0';

                if (is_valid_quoted_string(quoted_string)) {
                    (*count)++;
                    if (*count >= start && (end == -1 || *count <= end)) {
                        if (use_color) {
                            fprintf(output, ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, quoted_string);
                        } else {
                            fprintf(output, "`%s`", quoted_string);
                        }
                    } else {
                        fprintf(output, "%s", quoted_string);
                    }
                } else {
                    fprintf(output, "%s", quoted_string);
                }
                free(quoted_string);
                p++;
            } else {

                fprintf(output, "%s", start_ptr);
                return;
            }
        }
    }
}

void process_text(FILE *input, bool use_color, bool single_line, int start, int end, FILE *output) {
    int count = 0;
    if (single_line) {
        char line[1024];
        while (fgets(line, sizeof(line), input)) {
            int line_count = 0;
            process_line(line, use_color, start, end, &line_count, output);
            if (line_count > 0) {
                count += line_count;
            }
            fputc('\n', output);
        }
    } else {
        char *text = read_entire_input(input);
        if (!text) {
            return;
        }
        process_line(text, use_color, start, end, &count, output);
        free(text);
    }
}



int main(int argc, char *argv[]) {
    bool use_color = false;
    bool single_line = false;
    int start = 1;
    int end = -1;
    char *input_file = NULL;
    char *output_file = NULL;
    bool invalid_arg = false; 

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            printf("Балакин Алексей Антонвич, группа: N3149\n");
            printf("Вариант: 7-2-3-2\n");
            EXIT_SUCCESS;
        } else if (strcmp(argv[i], "-c") == 0) {
            use_color = true;
        } else if (strcmp(argv[i], "-n") == 0) {
            single_line = true;
        } else if (strncmp(argv[i], "-f=", 3) == 0) {
            start = atoi(argv[i] + 3);
        } else if (strncmp(argv[i], "-t=", 3) == 0) {
            end = atoi(argv[i] + 3);
        } else if (argv[i][0] == '-') {
            invalid_arg = true;
            break;
        } else if (input_file == NULL) {
            input_file = argv[i];
        } else if (output_file == NULL) {
            output_file = argv[i];
        } 
    }

    if (invalid_arg) {
        print_usage();
        EXIT_FAILURE;
    }

    if (start < 1 || (end != -1 && end < start)) {
        fprintf(stderr, "Ошибка: некорректные значения для -f и -t.\n");
        EXIT_FAILURE;
    }

    FILE *input = input_file ? fopen(input_file, "r") : stdin;
    if (input_file && !input) {
        perror("Ошибка открытия файла");
        EXIT_FAILURE;
    }

    FILE *output = output_file ? fopen(output_file, "w") : stdout;
     if (output_file && !output) {
        perror("Ошибка открытия файла");
        EXIT_FAILURE;
    }

    process_text(input, use_color, single_line, start, end, output);

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    EXIT_SUCCESS;
}