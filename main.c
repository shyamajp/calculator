#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// TODO globak variables for parentheses etc?

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
        printf("%c ", stack->elm[i]);
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

bool isEmpty(struct STACK *stack)
{
    return stack->size == -1;
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

struct OPERATOR operators[7] = {
    {'+', 1, LEFT},
    {'-', 1, LEFT},
    {'*', 2, LEFT},
    {'/', 2, LEFT},
    {'^', 3, RIGHT},
    {'(', 0, NONE},
    {')', 0, NONE},
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

// FIXME 1+23*456 -> {1, +, 23, *, 456}
void tokenize(char *string)
{
    char ops[7] = "+-*/^";
    char *tokens[10];
    int token_index = 0;

    printf("Tokenizing string: %s\n", string);
    for (char c = *string; c != '\0'; c = *++string)
    {
        // operators
        if (strchr(ops, c) != NULL)
        {
            printf("Operator: %c\n", c);
            // tokens[token_index++] = {c, '\0'}; IDK how to do this
        }
        // numbers
        else
        {
            // TODO(refactor): messy stuff but idea is there
            char *num = (char *)malloc(sizeof(char) * 10);
            int i = 0;

            char next_tmp = *++string;
            num[i++] = c;
            while (strchr(ops, next_tmp) == NULL)
            {
                num[i++] = next_tmp;
                c = next_tmp;
                next_tmp = *++string;
            };
            printf("Number: %s\n", num);
            // tokens[token_index++][0] = num; IDK how to do this
            next_tmp = *--string; // reset
        }
    }

    print("tokens: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", arr[i]);
    }
}

// infix to postfix (e.g. 3+4*2/(1-5)^2^3 -> 342*15−23^^÷+)
// TODO(UPDATE) to use string instead of char so that we can have multi-digit numbers
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
        // ✅ left parenthesis: always push to the output queue
        if (string[i] == '(')
        {
            printf("%c | Push token to stack\n", string[i]);
            push(&operator_stack, string[i]);
        }
        // ✅ right parenthesis: pop operators in the operator stack to the output queue until left parenthesis
        else if (string[i] == ')')
        {
            char top_char = last(&operator_stack);
            while (top_char != '(')
            {
                // 🚨 assert: the operator stack is not empty
                if (isEmpty(&operator_stack))
                {
                    printf("Invalid expression, exiting...\n");
                    exit(1);
                }
                char operator= pop(&operator_stack);
                printf("%c | Add token to output\n", string[i]);
                push(&output_queue, operator);

                top_char = last(&operator_stack);
            }

            // 🚨 assert: there is a left parenthesis at the top of the operator stack
            if (top_char != '(')
            {
                printf("Invalid expression, exiting...\n");
                exit(1);
            }
            printf("%c | Pop stack\n", string[i]);
            pop(&operator_stack);
        }
        // ✅ Operators (+, -, *, /, ^): from the operator stack to the output queue (while ...) + push to the operator stack
        else if (isOperator)
        {
            struct OPERATOR current = getOperator(string[i]);
            char top_char = last(&operator_stack);
            struct OPERATOR top = getOperator(top_char);

            while ((!isEmpty(&operator_stack) && top_char != '(') &&
                   (current.precedence < top.precedence || (current.precedence == top.precedence && current.associativity == LEFT)))
            {
                printf("%c | Pop stack to output\n", string[i]);
                char operator= pop(&operator_stack);
                push(&output_queue, operator);
                top = getOperator(first(&operator_stack));
            }

            printf("%c | Push token to stack\n", string[i]);
            push(&operator_stack, current.op);
        }
        // ✅ number: to the output queue
        else
        {
            printf("%c | Add token to output\n", string[i]);
            push(&output_queue, string[i]);
        }

        // For debugging purpose
        printf("output: ");
        print(&output_queue);
        printf("operator: ");
        print(&operator_stack);
        printf("=====================================\n");
    }

    // ✅ all remainings: from the operator stack to the output queue
    while (!isEmpty(&operator_stack))
    {
        // 🚨 assert: the operator on top of the stack is not a (left) parenthesis
        if (last(&operator_stack) == '(')
        {
            printf("Invalid expression, exiting...\n");
            exit(1);
        }

        printf("%c | Pop stack to output\n", last(&operator_stack));
        char operator= pop(&operator_stack);
        push(&output_queue, operator);

        // For debugging purpose
        printf("output: ");
        print(&output_queue);
        printf("operator: ");
        print(&operator_stack);
        printf("=====================================\n");
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
    char exp[100] = "3+4*2/(1-5)^2^3"; // 342*15-2^/+
    // char exp[100] = "1+23*456"; // {1, +, 23, *, 456}

    parseInput(exp);
    // tokenize(exp);
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