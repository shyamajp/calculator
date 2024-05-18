#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const char LEFT_PARENTHESIS = '(';
const char RIGHT_PARENTHESIS = ')';
const char PLUS = '+';
const char MINUS = '-';
const char ASTERISK = '*';
const char SLASH = '/';
const char CARET = '^';

const char DELIMITER = ',';
const char END = '\0';

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
typedef struct STACK STACK;
struct STACK
{
    char elm[100]; // TODO: malloc this?
    int size;
};

void init(STACK *stack)
{
    stack->size = -1;
}

void print(STACK *stack)
{
    for (int i = 0; i <= stack->size; i++)
    {
        printf("%c ", stack->elm[i]);
    }
    printf("\n");
}

void push(STACK *stack, char c)
{
    stack->size++;
    stack->elm[stack->size] = c;
}

char first(STACK *stack)
{
    char c = stack->elm[0];
    return c;
}

char last(STACK *stack)
{
    char c = stack->elm[stack->size];
    return c;
}

char shift(STACK *stack)
{
    char c = first(stack);
    for (int i = 0; i < stack->size; i++)
    {
        stack->elm[i] = stack->elm[i + 1];
    }
    stack->elm[stack->size] = END;
    stack->size--;
    return c;
}

char pop(STACK *stack)
{
    char c = last(stack);
    stack->elm[stack->size] = END;
    stack->size--;
    return c;
}

bool isEmpty(STACK *stack)
{
    return stack->size == -1;
}

enum ASSOCIATIVITY
{
    NONE,
    LEFT,
    RIGHT
};

typedef struct OPERATOR OPERATOR;
struct OPERATOR
{
    char op;
    int precedence;
    enum ASSOCIATIVITY associativity;
};

OPERATOR operators[7] = {
    {PLUS, 1, LEFT},
    {MINUS, 1, LEFT},
    {ASTERISK, 2, LEFT},
    {SLASH, 2, LEFT},
    {CARET, 3, RIGHT},
    {LEFT_PARENTHESIS, 0, NONE},
    {RIGHT_PARENTHESIS, 0, NONE},
};

OPERATOR getOperator(char c)
{
    for (size_t i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
    {
        if (c == operators[i].op)
        {
            return operators[i];
        }
    }
}

// infix to postfix (e.g. 3+4*2/(1-5)^2^3 -> 3,4,2,*,1,5,−2,3,^,^,÷,+)
char *parseInput(char *string)
{
    printf("Original String: %s\n", string);

    STACK operator_stack;
    STACK output_queue;

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
        if (string[i] == LEFT_PARENTHESIS)
        {
            printf("%c | Push token to stack\n", string[i]);
            push(&operator_stack, string[i]);
        }
        // ✅ right parenthesis: pop operators in the operator stack to the output queue until left parenthesis
        else if (string[i] == RIGHT_PARENTHESIS)
        {
            char top_char = last(&operator_stack);
            while (top_char != LEFT_PARENTHESIS)
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
                push(&output_queue, DELIMITER);

                top_char = last(&operator_stack);
            }

            // 🚨 assert: there is a left parenthesis at the top of the operator stack
            if (top_char != LEFT_PARENTHESIS)
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
            OPERATOR current = getOperator(string[i]);
            char top_char = last(&operator_stack);
            OPERATOR top = getOperator(top_char);

            while ((!isEmpty(&operator_stack) && top_char != LEFT_PARENTHESIS) &&
                   (current.precedence < top.precedence || (current.precedence == top.precedence && current.associativity == LEFT)))
            {
                printf("%c | Pop stack to output\n", string[i]);
                char operator= pop(&operator_stack);
                push(&output_queue, operator);
                push(&output_queue, DELIMITER);
                top = getOperator(first(&operator_stack));
            }

            printf("%c | Push token to stack\n", string[i]);
            push(&operator_stack, current.op);
        }
        // ✅ number: to the output queue
        else
        {
            while (string[i] >= '0' && string[i] <= '9' || string[i] == '.')
            {
                printf("%c | Add token to output\n", string[i]);
                push(&output_queue, string[i]);
                i++;
            }
            i--;
            // comma indicates the end of a number
            push(&output_queue, DELIMITER);
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
        if (last(&operator_stack) == LEFT_PARENTHESIS)
        {
            printf("Invalid expression, exiting...\n");
            exit(1);
        }

        printf("%c | Pop stack to output\n", last(&operator_stack));
        char operator= pop(&operator_stack);
        push(&output_queue, operator);
        push(&output_queue, DELIMITER);

        // For debugging purpose
        printf("output: ");
        print(&output_queue);
        printf("operator: ");
        print(&operator_stack);
        printf("=====================================\n");
    }
    // remove the last comma
    pop(&output_queue);

    // For debugging purpose
    printf("FINAL: ");
    print(&output_queue);

    return output_queue.elm;
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

    char *postfix = parseInput(exp);

    printf("Postfix: %s\n", postfix);

    return 0;
}