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
        if (string[i] == '*' || string[i] == '/')
        {
            push(&operator_stack, string[i]);
        }
        else if (string[i] == '+' || string[i] == '-')
        {
            char top = first(&operator_stack);
            printf("Top of operator stack: %c\n", top);
            // if top of operator stack has higher precedence, pop operators in the operator stack to the output queue
            while (operator_stack.size != -1)
            {
                char operator= pop(&operator_stack);
                push(&output_queue, operator);
            }
            // finally push the current operator to the operator stack
            push(&operator_stack, string[i]);
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
    char exp[100] = "a+b*c-d";
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