#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, unsigned char *argv[])
{
//    argv[1] = "Струпинский";
//    argv[2] = "Егор";
//    argv[3] = "Юрьевич";
//    argv[4] = "25";
//    argv[5] = "24.05.2003";
    unsigned long long nameNums = 0;
    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < strlen(argv[i]); j += 2)
        {
            unsigned long long temp = (argv[i][j] << 8) + argv[i][j + 1];
            printf("%llx\n", temp);
            nameNums += temp * temp;
            //nameNums += (int)argv[i][j] * (int)argv[i][j];
        }
    }

    printf("ASCII code sum: %llu\n", nameNums);
    int days;
    int months;
    int years;

//    int days = (argv[5][0] - '0') * 10 + (argv[5][1] - '0');
//    int month = (argv[5][3] - '0') * 10 + (argv[5][4] - '0');
//    int year = (argv[5][6] - '0') * 1000 + (argv[5][7] - '0') * 100 + (argv[5][8] - '0') * 10 + (argv[5][9] - '0');
    sscanf(argv[4], "%d.%d.%d", &days, &months, &years);

    int daysFromTheStart = (years) * 365 + (months) * 30 + days;

    long long mult = daysFromTheStart *  nameNums;
    printf("Birth date: \n\tDays: %d\n\tMonth: %d\n\tYear: %d\n\n", days, months, years);
    printf("Days from the start: %d\n", daysFromTheStart);

    printf("Multiplication: %lli\n", mult);

    char str[80];
    sprintf(str, "%lli", mult);
    int ind = strlen(str) / 2 - 1;
    int central = (int)((str[ind] - '0') * 100 + (str[ind + 1] - '0') * 10 + str[ind + 2] - '0');
    printf("3 central numbers : %d\n\n", central);

    int k = atoi(argv[5]);

    int answer = central % k + 1;
    printf("Your variant is: %d\n\n", answer);
    return 0;
}

//86.57.157.235/1.html
