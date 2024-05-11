#include <stdio.h>

int main()
{
    // ask for user input e.g. 1+1
    // 2 single digit numbers only
    char input[3];
    printf("Enter a simple math expression (e.g. 1+1): ");
    scanf("%s", input);

    // subtract '0' to get the actual integer value
    // kinda hacky isnt it?
    int firstNum = input[0] - '0';
    int secondNum = input[2] - '0';
    char operator= input[1];

    printf("First number: %d\n", firstNum);
    printf("Second number: %d\n", secondNum);
    printf("Operator: %c\n", operator);

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