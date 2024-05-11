#include <stdio.h>

int main()
{
    // ask for user input e.g. 1+1
    // for now, read 2 integers separately
    int firstNum, secondNum;
    char operator;

    printf("Enter first integer: ");
    scanf("%d", &firstNum);
    printf("Enter second integer: ");
    scanf("%d", &secondNum);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &operator);

    // split string by operator

    // calculate result based on operator
    // it's an integer so result is not always correct :)
    int result;
    switch (operator)
    {
    case '+':
        result = firstNum + secondNum;
        break;
    case '-':
        result = firstNum - secondNum;
        break;
    case '*':
        result = firstNum * secondNum;
        break;
    case '/':
        result = firstNum / secondNum;
        break;
    default:
        printf("Invalid operator, exiting...\n");
        return 1;
    }

    // print the result
    printf("Result: %d\n", result);

    return 0;
}