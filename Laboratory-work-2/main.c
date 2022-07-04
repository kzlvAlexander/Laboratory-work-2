#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

//Структура
typedef struct
{
    uint32_t *ints;
} uint1024_t;

//Перевод из uint в unint_1024
uint1024_t from_uint(unsigned int i)
{
    uint1024_t temp;
    temp.ints = (uint32_t *)malloc(32 * sizeof(uint32_t));
    int j;
    for (j = 1; j < 32; j++)
    {
        temp.ints[j] = 0;
    }
    temp.ints[0] = i;
    return temp;
}

//Костыли для базовой инициализации нулевых переменных и единичных переменных
uint1024_t uint1024_0()
{
    return from_uint(0);
}

uint1024_t uint1024_1()
{
    return from_uint(1);
}
//Костылики кончились

//Перевод из 16тиричной в 10тиричную
uint32_t hex_сhar_to32(char c)
{
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else
        return c - '0';
}

//Преобразование из 16тирички
uint1024_t hex_to_1024(char *hex)
{
    int len = strlen(hex);
    uint1024_t temp = uint1024_0();
    int parts = len / 8;
    int remain = len % 8;
    int i = 0, j;
    int start_loc = len - 1;
    for (i = 0; i < parts; i++)
    {
        uint32_t num = 0;
        for (j = 0; j < 8; j++)
            num = num | (hex_сhar_to32(hex[start_loc - j]) << (j * 4));
        temp.ints[i] = num;
        start_loc = len - i * 8 - 9;
    }
    uint32_t num = 0;
    for (j = 0; j < remain; j++)
        num = num | (hex_сhar_to32(hex[start_loc - j]) << (j * 4));
    temp.ints[i] = num;
    return temp;
}

//Перевод в 16тиричку
char *u1024_to_hex(uint1024_t temp)
{
    char *res = (char *)malloc(257 * sizeof(char));
    res[0] = '0';
    res[1] = '\0';
    char hex_string[17] = "0123456789abcdef";
    int count = 31;
    for (; count >= 0 && temp.ints[count] == 0; count--)
        ;
    int i;
    int position = 0;
    for (; count >= 0; count--)
    {
        uint32_t num = temp.ints[count];
        for (i = 0; i < 8; i++)
        {
            int val = num >> (4 * i);
            res[position + 7 - i] = hex_string[val & 15];
        }
        position += 8;
    }
    res[position] = '\0';
    return res;
}

//Вывод в поток, перед выводом перевод в 10-ую 
//void printf_value(uint1024_t temp)
//{
//    char *t = (char *)malloc(256 * sizeof(char));
//    t = u1024_to_hex(temp);
//    printf("%s\n", t);
//     unsigned long long i, s, k, p;
//     s = 0;
//     p = sizeof(t)-1;
//     printf("%llu",p);
//     for (i = 0; t[i] != '\0'; i++)
//     {
//         switch (t[i])
//         {
//         case 'a':
//             k = 10;
//             break;
//         case 'b':
//             k = 11;
//             break;
//         case 'c':
//             k = 12;
//             break;
//         case 'd':
//             k = 13;
//             break;
//         case 'e':
//             k = 14;
//             break;
//         case 'f':
//             k = 15;
//             break;
//         case '1':
//             k = 1;
//             break;
//         case '2':
//             k = 2;
//             break;
//         case '3':
//             k = 3;
//             break;
//         case '4':
//             k = 4;
//             break;
//         case '5':
//             k = 5;
//             break;
//         case '6':
//             k = 6;
//             break;
//         case '7':
//             k = 7;
//             break;
//         case '8':
//             k = 8;
//             break;
//         case '9':
//             k = 9;
//             break;
//         case '0':
//             k = 0;
//             break;
//         }
//         s = s + k * pow(16, p);
//         p--;
//     }
//     printf("%lld\n", s);
//}

//Сложение
uint1024_t add_op(uint1024_t a, uint1024_t b)
{
    uint1024_t temp;
    temp.ints = (uint32_t *)malloc(32 * sizeof(uint32_t));
    temp.ints[0] = a.ints[0] + b.ints[0];
    int i;
    for (i = 1; i < 32; i++)
    {
        temp.ints[i] = a.ints[i] + b.ints[i];
        if (temp.ints[i - 1] < a.ints[i - 1])
            temp.ints[i]++;
    }
    return temp;
}

//Вычитание
uint1024_t subtr_op(uint1024_t a, uint1024_t b)
{
    uint1024_t temp;
    temp.ints = (uint32_t *)malloc(32 * sizeof(uint32_t));
    temp.ints[0] = a.ints[0] - b.ints[0];
    int i;
    for (i = 1; i < 32; i++)
    {
        temp.ints[i] = a.ints[i] - b.ints[i];
        if (temp.ints[i - 1] > a.ints[i - 1])
            temp.ints[i]--;
    }
    return temp;
}

//Умножение
uint1024_t mult_op(uint1024_t a, uint1024_t b)
{
    uint1024_t temp;
    temp.ints = (uint32_t *)malloc(32 * sizeof(uint32_t));

    uint64_t top[32];
    uint64_t bottom[32];
    uint64_t products[32][32];
    int i, j;
    for (i = 0; i < 32; i++)
    {
        top[31 - i] = (uint64_t)a.ints[i];
        bottom[31 - i] = (uint64_t)b.ints[i];
    }

    for (i = 31; i > -1; i--)
    {
        for (j = 31; j > -1; j--)
        {
            products[31 - j][i] = top[j] * bottom[i];
        }
    }

    uint64_t results[32] = {0};
    for (i = 0; i < 32; i++)
    {
        results[i] += (products[i][31] & 0xffffffff);
        for (j = 30; j >= i; j--)
        {
            results[i + 31 - j] += (products[i][j] & 0xffffffff) + (products[i][j + 1] >> 32);
        }
    }

    for (i = 1; i < 32; i++)
        results[i] += results[i - 1] >> 32;

    for (i = 0; i < 32; i++)
    {
        results[i] &= 0xffffffff;
        temp.ints[i] = results[i];
    }

    return temp;
}

//Ввод переменной
uint1024_t scanf_value()
{
    char temp[257];
    printf("Enter number in hex\n");
    scanf("%s", temp);
       
    return (hex_to_1024(temp));
}

int main()
{
    uint1024_t a, b;
    a = scanf_value();
    b = scanf_value();

    uint1024_t add = add_op(a, b);
    uint1024_t sub = subtr_op(a, b);
    uint1024_t mul = mult_op(a, b);

    printf_value(add);
    printf_value(sub);
    printf_value(mul);

    return 0;
}