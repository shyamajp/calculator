#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void parseInput(char *string)
{
    printf("Original String: %s\n", string);

    // Shunting yard algorithm
    char *output_queue = malloc(sizeof(char) * 100);
    char *output_queue_ptr = output_queue;

    char *operator_stack = malloc(sizeof(char) * 100);
    char *operator_stack_ptr = operator_stack;

    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == '+')
        {
            *operator_stack_ptr = string[i];
            operator_stack_ptr++;
        }
        else
        {
            *output_queue_ptr = string[i];
            output_queue_ptr++;
        }
    }

    printf("Output Queue: %s\n", output_queue);
    printf("Operator Stack: %s\n", operator_stack);
}

int main()
{
    // // ask for user input e.g. 1+1
    // // 2 numbers only
    // char operator;
    // double a, b;

    // char input[100];

    // printf("Enter a math expression (e.g. 1+1): ");
    // scanf("%s", input);
    char exp[100] = "3+4+5";
    parseInput(exp);
    // sscanf(input, "%lf%c%lf", &a, &operator, & b);

    // printf("First number: %f\n", a);
    // printf("Second number: %f\n", b);
    // printf("Operator: %c\n", operator);

    // // calculate result based on operator
    // double result;
    // switch (operator)
    // {
    // case '+':
    //     result = add(a, b);
    //     break;
    // case '-':
    //     result = subtract(a, b);
    //     break;
    // case '*':
    //     result = multiply(a, b);
    //     break;
    // case '/':
    //     result = divide(a, b);
    //     break;
    // default:
    //     printf("Invalid operator, exiting...\n");
    //     return 1;
    // }

    // printf("Result: %f\n", result);
    return 0;
}