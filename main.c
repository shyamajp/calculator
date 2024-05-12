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

void debug(char *operator_stack, char *output_queue)
{
    printf("=====================================\n");
    printf("Operator Stack: %s\n", operator_stack);
    printf("Output Queue: %s\n", output_queue);
}

struct STACK
{
    char elm[100]; // TODO: malloc this?
    char size;
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
    init(&operator_stack);

    push(&operator_stack, 'a');
    push(&operator_stack, 'b');
    push(&operator_stack, 'c');

    first(&operator_stack);
    last(&operator_stack);

    printf("all elements\n");
    print(&operator_stack);

    printf("remove last element\n");
    pop(&operator_stack);
    print(&operator_stack);

    printf("remove first element\n");
    shift(&operator_stack);
    print(&operator_stack);
}

void parseInput2(char *string)
{
    printf("Original String: %s\n", string);

    // Shunting yard algorithm
    char *operator_stack = malloc(sizeof(char) * 100);
    char *operator_stack_ptr = operator_stack;

    char *output_queue = malloc(sizeof(char) * 100);
    char *output_queue_ptr = output_queue;

    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == '*' || string[i] == '/')
        {
            *operator_stack_ptr = string[i];
            operator_stack_ptr++;
            debug(operator_stack, output_queue);
        }
        else if (string[i] == '+' || string[i] == '-')
        {
            printf("Top of operator stack: %c\n", *(operator_stack_ptr - 1));
            // if top of operator stack has higher precedence, pop operators in the operator stack to the output queue
            while (operator_stack_ptr != operator_stack)
            {
                *output_queue_ptr = *(operator_stack_ptr - 1);
                *(operator_stack_ptr - 1) = '\0';
                output_queue_ptr++;
                operator_stack_ptr--;
                debug(operator_stack, output_queue);
            }

            *operator_stack_ptr = string[i];
            operator_stack_ptr++;
            debug(operator_stack, output_queue);
        }
        // Numbers are always pushed to the output queue
        else
        {
            *output_queue_ptr = string[i];
            output_queue_ptr++;
            debug(operator_stack, output_queue);
        }
    }

    printf("=====================================\n");
    // pop the remaining operators from the operator stack to the output queue
    while (operator_stack_ptr != operator_stack)
    {
        *output_queue_ptr = *(operator_stack_ptr - 1);
        *(operator_stack_ptr - 1) = '\0';
        output_queue_ptr++;
        operator_stack_ptr--;
        debug(operator_stack, output_queue);
    }

    debug(operator_stack, output_queue);
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