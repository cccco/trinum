#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include <errno.h>
#include <limits.h>

// max кол-во цифр в числе - 19 символов,
// возможен в начале строки знак '+' - 1 символ,
// возможны в конце символы '\n' или '\0' - 1 символ,
// т.е. всего max - 21 символ.
#define BUFFER_SIZE 21

int main(int argc, char **argv)
{
    char buffer[BUFFER_SIZE];
    bzero((void*)buffer, sizeof(char) * BUFFER_SIZE);
    int res = read(STDIN_FILENO, (void*)buffer, (size_t)BUFFER_SIZE);

    // Если ошибка чтения или ничего не прочитали.
    if (res == -1 || res == 0) {
        printf("0\n");
        return 0;
    }

    // Если прочитали максимальное количество символов, а крайние символы - несоответсвующие.
    if (res == BUFFER_SIZE) {
        if (buffer[0] != '+') {
            printf("0\n");
            return 0;
        }
        if (buffer[BUFFER_SIZE - 1] != '\0' && buffer[BUFFER_SIZE - 1] != '\n') {
            printf("0\n");
            return 0;
        }
    }

    // Если прочитали количество символов на 1 менше максимального, а один из крайних символов - несоответсвующий.
    if (res == BUFFER_SIZE - 1) {
        if (buffer[0] != '+' && buffer[res - 1] != '\0' && buffer[res - 1] != '\n') {
            printf("0\n");
            return 0;
        }
    }

    // Отсекаем строку с начальными пробельными символами, которые может пропустить в дальнейшем strtoll().
    if (buffer[0] != '+' && buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3' && buffer[0] != '4'
                         && buffer[0] != '5' && buffer[0] != '6' && buffer[0] != '7' && buffer[0] != '8'
                         && buffer[0] != '9' && buffer[0] != '0') {
        printf("0\n");
        return 0;
    }

    // Если ввод закончился переводом строки.
    // Необходимо, чтобы в дальнейшем правильно определить наличие невалидных символов.
    if (buffer[res - 1] == '\n') {
        buffer[res - 1] = '\0';
    }

    char *end;
    long long int num = strtoll(buffer, &end, 10);

    // Если переполнение буфера.
    if ((num == LLONG_MAX || num == LLONG_MIN) && errno == ERANGE) {
        printf("0\n");
        return 0;
    }

    // Если строка имеет невалидные символы.
    if (*end != '\0') {
        printf("0\n");
        return 0;
    }

    // Если невалидное число, т.е. <= 0.
    if (num <= 0) {
        printf("0\n");
        return 0;
    }

    unsigned long long int index = 0;

    while (num > 0) {
        num -= ++index;
    }

    if (num < 0) {
        printf("0\n");
        return 0;
    }

    printf("%llu\n", index);

    return 0;
}
