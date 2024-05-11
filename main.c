#include <stdio.h>

int main()
{
    // ask for user input e.g. 1+1
    // for now, read 2 integers separately
    int firstNum, secondNum;

    printf("Enter first integer: ");
    scanf("%d", &firstNum);
    printf("Enter second integer: ");
    scanf("%d", &secondNum);

    // split string by operator
    // for now, only support addition
    int result = firstNum + secondNum;

    // print the result
    printf("Result: %d\n", result);

    return 0;
}