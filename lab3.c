#include <stdio.h>

#include <string.h>

#include <stdlib.h>

int main() {
    FILE * fin;
    fin = fopen("access_log_Jul95", "r");
    int c_c = 400;
    size_t n = c_c * sizeof(char);
    char * line = (char * ) malloc(c_c);
    size_t count = 0;
    int counter = 0;
    printf("5xx fault requests:");
    while (fgets(line, n, fin) != NULL) {
        counter++;
        for (int i = 70; i < strlen(line); i++) {
            if ((line[i] == ' ') && (line[i + 1] == '5') && (line[i + 4] == ' ')) {
                printf("%s\n", line);
                count++;
            }
        }
    }
    printf("Quantity of 5xx fault requests = %llu\n", count);
    free(line);
    int d = 0, h = 0, m = 0, s = 0, time, f = 0, l = 0, max = 0, j = 0;
    int param;
    count;
    char * lines = (char * ) malloc(c_c);
    int * times = (int * ) malloc(counter * sizeof(int));
    if (times == NULL) exit(1);
    memset(times, '0', counter * sizeof(int));
    printf("Enter time in seconds\n");
    scanf("%d", & param);
    printf("\n");
    while (fgets(lines, n, fin) != NULL) {
        int flag = 0;
        for (int i = 15; i < strlen(lines); i++) {
            if (lines[i] == '[') {
                d = (lines[i + 1] - '0') * 10 + (lines[i + 2] - '0');
            } else if ((lines[i] == ':') && (flag == 0)) {
                h = (lines[i + 1] - '0') * 10 + lines[i + 2] - '0';
                ++flag;
            } else if ((lines[i] == ':') && (flag == 1)) {
                m = (lines[i + 1] - '0') * 10 + lines[i + 2] - '0';
                ++flag;
            } else if ((lines[i] == ':') && (flag == 2)) {
                s = (lines[i + 1] - '0') * 10 + (lines[i + 2] - '0');
                flag = -10;
            } else if (flag == -10) {
                continue;
            }
        }
        time = (d - 1) * 24 * 60 ^ 2 + h * 60 ^ 2 + m * 60 + s;
        times[j] = time;
        j++;
    }
    while (l <= counter) {
        while (times[l] - times[f] > param) {
            f++;
        }
        if (l - f + 1 > max) {
            max = l - f + 1;
        }
        l++;
    }
    free(lines);
    free(times);
    printf("%d", max);
    return 0;
}