#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <locale.h>
#include <malloc.h>
#include <math.h>

//Прототипы функций
size_t getsize(const char *file_name); //Получение размера файла
void show(char *file); //прототип реализации функции --show
void get(char *file, char frame[5]); //прототип реализации функции --get=prop_name
void set(char *file, char frame[5], char *value); //прототип реализации функции --set=prop_name --value=prop_value

//Тело программы и реализация парсинга параметров
int main(int argc, char *argv[]) {
    int sh = 0;
    int g = 0;
    int s = 0;
    char fr[5] = {0};
    char *val = (char *) malloc(sizeof(char) * 60);
    for (int i = 0; i < 60; i++)
        val[i] = 0;
    char *file = (char *) malloc(sizeof(char) * 9);
    //Парсинг параметров
    for (int i = 0; i < argc; i++) {
        if (argv[i][2] == 'f') {
            for (int j = 0; j < 8; j++)
                file[j] = argv[i][j + 11];
            file[8] = '\0';
        }
        if (!strcmp("--show", argv[i]))
            sh = 1;
        if (argv[i][2] == 'g') {
            g = 1;
            file[8] = '\0';
            for (int j = 0; j < 4; j++)
                fr[j] = argv[i][j + 6];
            fr[4] = '\0';
        }
        if (argv[i][2] == 's' && argv[i][3] == 'e') {
            s = 1;
            for (int j = 0; j < 4; j++)
                fr[j] = argv[i][j + 6];
            fr[4] = '\0';
            int c = 8;
            for (int j = 0; j < strlen(argv[i + 1]); j++, c++) {
                if (argv[i + 1][c] >= 'a' && argv[i + 1][c] <= 'z' || argv[i + 1][c] >= 'A' && argv[i + 1][c] <= 'Z'
                    || argv[i + 1][c] >= '0' && argv[i + 1][c] <= '9' || argv[i][c] == ' ')
                    val[j] = argv[i + 1][c];
                else
                    break;
            }
            val[c] = '\0';
        }
    }
    if (sh)
        show(file);
    if (g)
        get(file, fr);
    if (s)
        set(file, fr, val);
    return 0;
}

//Реализации

size_t getsize(const char *file_name) { //Беззнаковый метод
    size_t size = 0;
    FILE *f = fopen(file_name, "rb");
    if (f == NULL)
        size = -1;
    else {
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fclose(f);
    }
    return size;
}

void show(char *file) { //реализация --show
    char temp[10];
    FILE *f = fopen(file, "rb");
    fread(temp, 1, 10, f);
    char *b = (char *) malloc(sizeof(char) * getsize(file));
    fread(b, 1, getsize(file) - 10, f);
    int i = 0;
    while (b[i] != 0) {
        char name[5] = {b[i], b[i + 1], b[i + 2], b[i + 3], '\0'};
        printf("%s: ", name);
        i += 4;
        int size = b[i] * pow(256, 3) + b[i + 1] * pow(256, 2) + b[i + 2] * 256 + b[i + 3];
        i += 6;
        for (int j = 0; j < size; j++, i++)
            printf("%c", b[i]);
        printf("\n");
    }
    fclose(f);
    free(b);
}

void get(char *file, char frame[5]) {
    char *b = (char *) malloc(sizeof(char) * (getsize(file) + 1));
    FILE *f = fopen(file, "rb");
    fread(b, 1, getsize(file), f);

    int i = 10;
    while (b[i] != 0) {
        int size = 0;
        if (b[i] == frame[0] && b[i + 1] == frame[1] && b[i + 2] == frame[2] && b[i + 3] == frame[3]) {
            printf("%c", b[i]);
            printf("%c", b[i + 1]);
            printf("%c", b[i + 2]);
            printf("%c: ", b[i + 3]);
            i += 4;
            size = b[i] * pow(256, 3) + b[i + 1] * pow(256, 2) + b[i + 2] * 256 + b[i + 3];
            i += 6;
            for (int j = 0; j < size; i++, j++)
                printf("%c", b[i]);
            break;
        } else {
            i += 4;
            int size = b[i] * pow(256, 3) + b[i + 1] * pow(256, 2) + b[i + 2] * 256 + b[i + 3];
            i += size + 6;
        }
    }
    fclose(f);
    free(b);
}

void set(char *file, char frame[5], char *value) { //Реализация функции --get=prop_name
    FILE *f = fopen(file, "rb");
    char *b = (char *) malloc(sizeof(char) * getsize(file));
    fread(b, 1, getsize(file), f);
    for (int i = 0; i < getsize(file) - 4; i++) {
        if (b[i] == frame[0] && b[i + 1] == frame[1] && b[i + 2] == frame[2] && b[i + 3] == frame[3]) {
            FILE *nf = fopen("t.mp3", "ab");
            fwrite(b, 1, i + 10, nf);
            i += 7;
            int size = b[i];
            fwrite(value, 1, size, nf);
            fseek(f, i + size + 3, SEEK_SET);
            char temp[1000] = {0};
            int cnt;
            while (!feof(f)) {
                cnt = fread(&temp, 1, 1000 * sizeof(char), f);
                fwrite(&temp, 1, cnt * sizeof(char), nf);
            }
            fclose(f);
            fclose(nf);
            nf = fopen("t.mp3", "rb");
            f = fopen(file, "wb");
            fseek(nf, 0, SEEK_SET);
            while (!feof(nf)) {
                cnt = fread(&temp, 1, 1000 * sizeof(char), nf);
                fwrite(&temp, 1, cnt * sizeof(char), f);
            }
            fclose(nf);
            remove("t.mp3");
            break;
        }
    }
    free(b);
}

