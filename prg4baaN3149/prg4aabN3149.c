#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>


typedef struct Node {
    char *value;
    struct Node *prev;
    struct Node *next;
} Node;

static bool modified = false;  // Признак изменений для сохранения


static Node* create_node(const char *s) {
    Node *n = malloc(sizeof(Node));
    if (!n) { perror("malloc"); exit(EXIT_FAILURE); }
    n->value = strdup(s);
    n->prev = n->next = NULL;
    return n;
}


static void free_list(Node *head) {
    while (head) {
        Node *nx = head->next;
        free(head->value);
        free(head);
        head = nx;
    }
}


static void push_front(Node **head, const char *s) {
    Node *n = create_node(s);
    if (*head) {
        (*head)->prev = n;
        n->next = *head;
    }
    *head = n;
    modified = true;
}


static void push_back(Node **head, const char *s) {
    Node *n = create_node(s);
    if (!*head) {
        *head = n;
    } else {
        Node *t = *head;
        while (t->next) t = t->next;
        t->next = n;
        n->prev = t;
    }
    modified = true;
}


static void pop_front(Node **head) {
    if (!*head) return;
    Node *t = *head;
    *head = t->next;
    if (*head) (*head)->prev = NULL;
    free(t->value);
    free(t);
    modified = true;
}


static void pop_back(Node **head) {
    if (!*head) return;
    Node *t = *head;
    while (t->next) t = t->next;
    if (t->prev) {
        t->prev->next = NULL;
    } else {
        *head = NULL;
    }
    free(t->value);
    free(t);
    modified = true;
}


static void min_first(Node **head) {
    if (!*head) return;
    Node *it = *head;
    char *minv = it->value;
    for (; it; it = it->next) {
        if (strcmp(it->value, minv) < 0) minv = it->value;
    }
    it = *head;
    while (it) {
        Node *next = it->next;
        if (strcmp(it->value, minv) == 0) {
            if (it->prev) it->prev->next = it->next;
            else *head = it->next;
            if (it->next) it->next->prev = it->prev;
            it->prev = NULL;
            it->next = *head;
            if (*head) (*head)->prev = it;
            *head = it;
        }
        it = next;
    }
    modified = true;
}


static void dump(Node *head, FILE *out) {
    if (!head) {
        fprintf(out, "List is empty\n");
        return;
    }
    for (Node *it = head; it; it = it->next) {
        fprintf(out, "%p %p %p %s\n",
                (void*)it, (void*)it->prev, (void*)it->next, it->value);
    }
}


// p указывает на первую '"'. Возвращает выделенный буфер и сдвигает *p_out за закрывающую кавычку.
static char* extract_quoted(const char *p, const char **p_out) {
    if (*p != '"') return NULL;
    const char *cur = p + 1;
    size_t cap = 16, len = 0;
    char *res = malloc(cap);
    while (*cur) {
        if (*cur == '"') {
            size_t bs = 0;
            const char *tmp = cur - 1;
            while (tmp >= p && *tmp == '\\') { bs++; tmp--; }
            if (bs % 2 == 0) {
                res[len] = '\0';
                *p_out = cur + 1;
                return res;
            }
        }
        if (*cur == '\\' && (*(cur+1) == '"' || *(cur+1) == '\\')) {
            char ch = *(cur+1);
            if (len + 1 >= cap) { cap *= 2; res = realloc(res, cap); }
            res[len++] = ch;
            cur += 2;
        } else {
            if (len + 1 >= cap) { cap *= 2; res = realloc(res, cap); }
            res[len++] = *cur;
            cur++;
        }
    }
    free(res);
    return NULL;
}


static void process_command(Node **head, const char *cmdline) {
    const char *p = cmdline;
    while (isspace((unsigned char)*p)) p++;
    if (!*p) return;
    char cmd[32]; size_t ci = 0;
    while (*p && !isspace((unsigned char)*p) && ci + 1 < sizeof(cmd)) cmd[ci++] = *p++;
    cmd[ci] = '\0';
    while (isspace((unsigned char)*p)) p++;
    if (strcmp(cmd, "push_front") == 0) {
        while (*p) {
            while (isspace((unsigned char)*p)) p++;
            if (!*p) break;
            if (*p != '"') {
                fprintf(stderr, "Error: bad string start: %c\n", *p);
                break;
            }
            const char *next;
            char *s = extract_quoted(p, &next);
            if (!s) { fprintf(stderr, "Error: invalid quoted string\n"); break; }
            push_front(head, s);
            free(s);
            p = next;
        }
    } else if (strcmp(cmd, "push_back") == 0) {
        while (*p) {
            while (isspace((unsigned char)*p)) p++;
            if (!*p) break;
            if (*p != '"') {
                fprintf(stderr, "Error: bad string start: %c\n", *p);
                break;
            }
            const char *next;
            char *s = extract_quoted(p, &next);
            if (!s) { fprintf(stderr, "Error: invalid quoted string\n"); break; }
            push_back(head, s);
            free(s);
            p = next;
        }
    } else if (strcmp(cmd, "pop_front") == 0) {
        if (*p) {
            fprintf(stderr, "Error: pop_front shouldn't take args\n");
        } else {
            pop_front(head);
        }
    } else if (strcmp(cmd, "pop_back") == 0) {
        if (*p) {
            fprintf(stderr, "Error: pop_back shouldn't take args\n");
        } else {
            pop_back(head);
        }
    } else if (strcmp(cmd, "min_first") == 0) {
        if (*p) {
            fprintf(stderr, "Error: min_first shouldn't take args\n");
        } else {
            min_first(head);
        }
    } else if (strcmp(cmd, "dump") == 0) {
        if (*p) {
            char fname[256]; size_t fi = 0;
            while (*p && !isspace((unsigned char)*p) && fi + 1 < sizeof(fname)) fname[fi++] = *p++;
            fname[fi] = '\0';
            FILE *o = fopen(fname, "w");
            if (!o) { perror(fname); return; }
            dump(*head, o);
            fclose(o);
        } else {
            dump(*head, stdout);
        }
    } else if (strcmp(cmd, "exit") == 0) {
        // Обработка выхода будет в main, криво - но работает
    } else {
        fprintf(stderr, "Unknown command: %s\n", cmd);
    }
}


static void write_file(Node *head, const char *fname) {
    FILE *f = fopen(fname, "wb"); if (!f) { perror("fopen"); exit(EXIT_FAILURE);}    
    uint32_t N = 0;
    for (Node* it = head; it; it = it->next) N++;
    uint32_t *offsets = malloc(N*sizeof(uint32_t));
    uint32_t *indices = malloc(N*sizeof(uint32_t));
    uint32_t idx = 0;
    for (Node* it = head; it; it = it->next) {
        offsets[idx] = ftell(f);
        fwrite(it->value, strlen(it->value)+1, 1, f);
        indices[idx] = idx;
        idx++;
    }
    uint32_t offset_pos = ftell(f);
    fwrite(offsets, sizeof(uint32_t), N, f);
    fwrite(indices, sizeof(uint32_t), N, f);
    fwrite(&offset_pos, sizeof(uint32_t), 1, f);
    fclose(f);
    free(offsets); free(indices);
}


static void read_file(Node **head, const char *fname) {
    FILE *f = fopen(fname, "rb");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long fsz = ftell(f);
    if (fsz < 8) { fclose(f); return; }
    
    fseek(f, fsz - 4, SEEK_SET);
    uint32_t offset_pos;
    fread(&offset_pos, sizeof(uint32_t), 1, f);
    if (offset_pos > fsz - 4) { fclose(f); return; }
    
    uint32_t N = (fsz - offset_pos - 4) / 8;
    
    uint32_t *offsets = malloc(N * sizeof(uint32_t));
    uint32_t *indices = malloc(N * sizeof(uint32_t));
    if (!offsets || !indices) { perror("malloc"); fclose(f); return; }
    
    fseek(f, offset_pos, SEEK_SET);
    fread(offsets, sizeof(uint32_t), N, f);
    fread(indices, sizeof(uint32_t), N, f);
    
    for (uint32_t i = 0; i < N; ++i) {
        uint32_t str_off = offsets[indices[i]];
        if (str_off >= (uint32_t)offset_pos) continue; // БИТ против некорректных смещений
        fseek(f, str_off, SEEK_SET);
        char buf[1024];
        fread(buf, 1, sizeof(buf), f);
        size_t L = strnlen(buf, sizeof(buf));
        if (L >= sizeof(buf)) buf[sizeof(buf)-1] = '\0';
        push_back(head, buf);
    }
    
    fclose(f);
    free(offsets);
    free(indices);
}

int main(int argc, char *argv[]) {
    FILE *file;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }
    else{
        if (!strcmp("-v", argv[1])){
            printf("Балакин Алексей Антонович, гр. N3149\nВариант: 2-6-10-7\n");
            return EXIT_SUCCESS;
        }
        file = fopen(argv[1], "r");
        if (!file){
            fprintf(stderr, "Path error: no such file directory!\n");
            return 2;
        }
    }
    // Устанавливаем буферизацию для stdin для поддержки cat
    setvbuf(stdin, NULL, _IOLBF, 0); 

    Node *head = NULL;
    read_file(&head, argv[1]);

    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        size_t L = strlen(line);
        if (L && line[L-1] == '\n') line[L-1] = '\0';
        char tmp[32]; size_t ti = 0; const char *p0 = line;
        while (*p0 && isspace((unsigned char)*p0)) p0++;
        while (*p0 && !isspace((unsigned char)*p0) && ti+1 < sizeof(tmp)) tmp[ti++] = *p0++;
        tmp[ti] = '\0';
        if (strcmp(tmp, "exit") == 0) break;
        process_command(&head, line);
    }

    if (modified) {
        if (head) write_file(head, argv[1]);
        else { FILE *f = fopen(argv[1], "wb"); if (f) fclose(f); }
    }
    free_list(head);
    return EXIT_SUCCESS;
}
