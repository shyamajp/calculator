#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

// TODO: probably no longer stack with shift?
struct STACK
{
    char elm[100]; // TODO: malloc this?
    int size;
};

void init(struct STACK *stack)
{
    stack->size = -1;
}

void print(struct STACK *stack)
{
    for (int i = 0; i <= stack->size; i++)
    {
        printf("%c", stack->elm[i]);
    }
    printf("\n");
}

void push(struct STACK *stack, char c)
{
    stack->size++;
    stack->elm[stack->size] = c;
}

char first(struct STACK *stack)
{
    char c = stack->elm[0];
    return c;
}

char last(struct STACK *stack)
{
    char c = stack->elm[stack->size];
    return c;
}

char shift(struct STACK *stack)
{
    char c = first(stack);
    for (int i = 0; i < stack->size; i++)
    {
        stack->elm[i] = stack->elm[i + 1];
    }
    stack->size--;
    return c;
}

char pop(struct STACK *stack)
{
    char c = last(stack);
    stack->size--;
    return c;
}

enum ASSOCIATIVITY
{
    NONE,
    LEFT,
    RIGHT
};

struct OPERATOR
{
    char op;
    int precedence;
    enum ASSOCIATIVITY associativity;
};

struct OPERATOR operators[5] = {
    {'+', 1, LEFT},
    {'-', 1, LEFT},
    {'*', 2, LEFT},
    {'/', 2, LEFT},
    {'^', 3, RIGHT},
};

struct OPERATOR getOperator(char c)
{
    for (size_t i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
    {
        if (c == operators[i].op)
        {
            return operators[i];
        }
    }
}

void parseInput(char *string)
{
    printf("Original String: %s\n", string);

    struct STACK operator_stack;
    struct STACK output_queue;

    // initialize
    init(&operator_stack);
    init(&output_queue);

    // Shunting yard algorithm
    for (int i = 0; i < strlen(string); i++)
    {
        bool isOperator = false;

        for (size_t j = 0; j < sizeof(operators) / sizeof(operators[0]); j++)
        {
            if (string[i] == operators[j].op)
            {
                isOperator = true;
            }
        }

        if (isOperator)
        {
            struct OPERATOR current = getOperator(string[i]);
            struct OPERATOR top = getOperator(first(&operator_stack));

            // if top of operator stack has higher precedence, pop operators in the operator stack to the output queue
            if (current.precedence <= top.precedence && current.associativity == LEFT)
            {
                while (operator_stack.size != -1)
                {
                    char operator= pop(&operator_stack);
                    push(&output_queue, operator);
                }
            }

            push(&operator_stack, current.op);
        }
        // Numbers are always pushed to the output queue
        else
        {
            push(&output_queue, string[i]);
        }

        // For debugging purpose
        printf("=====================================\n");
        printf("Operator Stack: ");
        print(&operator_stack);
        printf("Output Queue: ");
        print(&output_queue);
    }

    // pop the remaining operators from the operator stack to the output queue
    while (operator_stack.size != -1)
    {

        char operator= pop(&operator_stack);
        push(&output_queue, operator);

        // For debugging purpose
        printf("=====================================\n");
        printf("Operator Stack: ");
        print(&operator_stack);
        printf("Output Queue: ");
        print(&output_queue);
    }
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
    // char exp[100] = "a+b*c-d";       // abc*+d-
    char exp[100] = "3+4*2/(1-5)^2"; // 342*15-2^/+

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