#include <stdio.h>
#include <stdlib.h>
#define SIZE 309

typedef struct bigint
{
    int a[SIZE];
    int size;
    int sign;
} bigint;

bigint default_bigint()
{
    bigint num;
    num.size = 0;
    num.sign = 1;
    return num;
}

int isnumber(bigint num)
{
    int is_valid = 1;
    if (num.size > SIZE)
    {
        is_valid = 0;
    }
    else if (num.size == 0)
    {
        is_valid = 0;
    }
    else
    {
        int i = 0;
        while (i < num.size && !(is_valid))
        {
            char ch = num.a[i];
            if (ch < '0' || ch > '9')
            {
                is_valid = 0;
            }
            i++;
        }
    }
    return is_valid;
}

int compare_absolute(bigint num1, bigint num2)
{
    int result = 0;
    if (num1.size > num2.size)
    {
        result = 1;
    }
    else if (num1.size < num2.size)
    {
        result = -1;
    }
    else
    {
        int flag = 1;
        for (int i = num1.size - 1; ((i >= 0) && (flag == 1)); i--)
        {
            if (num1.a[i] > num2.a[i])
            {
                result = 1;
                flag = 0;
            }
            if (num1.a[i] < num2.a[i])
            {
                result = -1;
                flag = 0;
            }
        }
    }
    return result;
}

bigint add(bigint num1, bigint num2)
{
    bigint result;
    int x = isnumber(num1);
    int y = isnumber(num2);
    if ((x == 1) && (y == 1))
    {
        if (num1.sign == num2.sign)
        {
            result.sign = num1.sign;
            int carry = 0, i = 0;

            for (; i < num1.size || i < num2.size || carry; i++)
            {
                int sum = carry;
                if (i < num1.size)
                    sum += num1.a[i];
                if (i < num2.size)
                    sum += num2.a[i];

                result.a[i] = sum % 10;
                carry = sum / 10;
            }
            result.size = i;
        }
        else
        {
            num2.sign *= -1;
            bigint subtract();
            result = subtract(num1, num2);
            num2.sign *= -1;
        }
    }
    else
        result = default_bigint();
    return result;
}

bigint subtract(bigint num1, bigint num2)
{
    bigint result;
    int x = isnumber(num1);
    int y = isnumber(num2);
    if ((x == 1) && (y == 1))
    {
        if (num1.sign != num2.sign)
        {
            num2.sign *= -1;
            bigint add();
            result = add(num1, num2);
            num2.sign *= -1;
        }
        else
        {
            if (compare_absolute(num1, num2) >= 0)
            {
                result.sign = num1.sign;
                int borrow = 0;

                for (int i = 0; i < num1.size || borrow; i++)
                {
                    int diff = num1.a[i] - (i < num2.size ? num2.a[i] : 0) - borrow;
                    if (diff < 0)
                    {
                        diff += 10;
                        borrow = 1;
                    }
                    else
                    {
                        borrow = 0;
                    }
                    result.a[i] = diff;
                }
                result.size = num1.size;
                while (result.size > 1 && result.a[result.size - 1] == 0)
                {
                    result.size--;
                }
            }
            else
            {
                result = subtract(num2, num1);
                result.sign = -num1.sign;
            }
        }
    }
    else
        result = default_bigint();
    return result;
}

bigint multiply(bigint num1, bigint num2)
{
    bigint result = default_bigint();
    int x = isnumber(num1);
    int y = isnumber(num2);

    if (x == 1 && y == 1)
    {
        for (int i = 0; i < SIZE; i++)
        {
            result.a[i] = 0;
        }

        result.sign = num1.sign * num2.sign;

        for (int i = 0; i < num1.size; i++)
        {
            int carry = 0;

            for (int j = 0; j < num2.size || carry; j++)
            {
                int product = result.a[i + j] +
                              num1.a[i] * (j < num2.size ? num2.a[j] : 0) + carry;

                result.a[i + j] = product % 10;
                carry = product / 10;
            }
        }

        result.size = num1.size + num2.size;
        while (result.size > 0 && result.a[result.size - 1] == 0)
        {
            result.size--;
        }

        if (result.size == 0)
        {
            result.size = 1;
            result.sign = 1;
        }
    }
    else
    {
        result = default_bigint();
    }

    return result;
}
void print(bigint num)
{
    if (num.size == 0)
    {
        printf("enter only digits in limited range");
        return;
    }
    if (num.sign == -1)
    {
        printf("-");
    }
    for (int i = num.size - 1; i >= 0; i--)
    {
        printf("%d", num.a[i]);
    }
    printf("\n");
}

bigint create_bigint(int size, FILE *file, int use_file)
{
    bigint num = default_bigint();
    num.size = size;

    if (use_file) // Read from file
    {
        for (int i = size - 1; i >= 0; i--)
        {
            char ch;
            fscanf(file, " %c", &ch);
            if (ch < '0' || ch > '9')
            {
                printf("Error: Invalid digit in file input.\n");
                return default_bigint();
            }
            num.a[i] = ch - '0';
        }
    }
    else // Read from console
    {
        printf("Enter the number (max %d digits): ", size);
        for (int i = size - 1; i >= 0; i--)
        {
            char ch;
            scanf(" %c", &ch);
            if (ch < '0' || ch > '9')
            {
                printf("Error: Invalid digit in console input.\n");
                return default_bigint();
            }
            num.a[i] = ch - '0';
        }
    }
    return num;
}

int main()
{
    int choice, use_file;
    FILE *file = NULL;

    printf("Choose input mode (0 for console, 1 for file): ");
    scanf("%d", &use_file);

    if (use_file)
    {
        file = fopen("input.txt", "r");
        if (!file)
        {
            printf("Error: Unable to open input file.\n");
            return 1;
        }
    }

    printf("Choose among 1 (create and print), 2 (addition), 3 (subtraction), 4 (multiplication): ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
    {
        int n1;
        printf("Enter the size of the number: ");
        scanf("%d", &n1);
        bigint num1 = create_bigint(n1, file, use_file);
        print(num1);
        break;
    }
    case 2:
    {
        int n2, n3;
        printf("Enter the size of the first number: ");
        scanf("%d", &n2);
        bigint num2 = create_bigint(n2, file, use_file);
        printf("Enter the size of the second number: ");
        scanf("%d", &n3);
        bigint num3 = create_bigint(n3, file, use_file);
        bigint sum = add(num2, num3);
        print(sum);
        break;
    }
    case 3:
    {
        int n4, n5;
        printf("Enter the size of the first number: ");
        scanf("%d", &n4);
        bigint num4 = create_bigint(n4, file, use_file);
        printf("Enter the size of the second number: ");
        scanf("%d", &n5);
        bigint num5 = create_bigint(n5, file, use_file);
        bigint diff = subtract(num4, num5);
        print(diff);
        break;
    }
    case 4:
    {
        int n6, n7;
        printf("Enter the size of the first number: ");
        scanf("%d", &n6);
        bigint num6 = create_bigint(n6, file, use_file);
        printf("Enter the size of the second number: ");
        scanf("%d", &n7);
        bigint num7 = create_bigint(n7, file, use_file);
        bigint product = multiply(num6, num7);
        print(product);
        break;
    }
    }

    if (file)
    {
        fclose(file);
    }

    return 0;
}