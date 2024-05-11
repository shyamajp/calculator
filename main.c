#include <stdio.h>
#include <stdlib.h>

double add(double a, double b)
{
    return a + b;
}

double subtract(double a, double b)
{
    return a - b;
}

double multiply(double a, double b)
{
    return a * b;
}

double divide(double a, double b)
{
    if (b == 0)
    {
        printf("Division by zero is not allowed, exiting...\n");
        exit(1);
    }
    return a / b;
}

int main()
{
    // ask for user input e.g. 1+1
    // 2 numbers only
    char operator;
    double a, b;

    char input[100];

    printf("Enter a math expression (e.g. 1+1): ");
    scanf("%s", input);
    sscanf(input, "%lf%c%lf", &a, &operator, & b);

    printf("First number: %f\n", a);
    printf("Second number: %f\n", b);
    printf("Operator: %c\n", operator);

    // calculate result based on operator
    double result;
    switch (operator)
    {
    case '+':
        result = add(a, b);
        break;
    case '-':
        result = subtract(a, b);
        break;
    case '*':
        result = multiply(a, b);
        break;
    case '/':
        result = divide(a, b);
        break;
    default:
        printf("Invalid operator, exiting...\n");
        return 1;
    }

    printf("Result: %f\n", result);
    return 0;
}