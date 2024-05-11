#include <stdio.h>

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
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;
    case '/':
        result = a / b;
        break;
    default:
        printf("Invalid operator, exiting...\n");
        return 1;
    }

    printf("Result: %f\n", result);
    return 0;
}