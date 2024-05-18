#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

const char LEFT_PARENTHESIS = '(';
const char RIGHT_PARENTHESIS = ')';
const char PLUS = '+';
const char MINUS = '-';
const char ASTERISK = '*';
const char SLASH = '/';
const char CARET = '^';

const char DELIMITER = ',';
const char END = '\0';

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

bool isOperator(char c)
{
    for (size_t i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
    {
        if (c == operators[i].op)
        {
            return true;
        }
    }
    return false;
}

bool isNumber(char c)
{
    return (c >= '0' && c <= '9') || c == '.';
}

/**
 * @brief Parse infix expression to postfix expression
 * @param char* infix expression
 * @return char* postfix expression separated by DELIMITER
 * @example "3+4*2/(1-5)^2^3" -> "3,4,2,*,1,5,−2,3,^,^,÷,+"
 */
char *parseInput(char *infix)
{
    STACK operator_stack;
    STACK output_queue;

    // initialize
    init(&operator_stack);
    init(&output_queue);

    // Shunting yard algorithm
    for (int i = 0; i < strlen(infix); i++)
    {
        bool cIsOperator = isOperator(infix[i]);
        // ✅ left parenthesis: always push to the output queue
        if (infix[i] == LEFT_PARENTHESIS)
        {
            printf("%c | Push token to stack\n", infix[i]);
            push(&operator_stack, infix[i]);
        }
        // ✅ right parenthesis: pop operators in the operator stack to the output queue until left parenthesis
        else if (infix[i] == RIGHT_PARENTHESIS)
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
                printf("%c | Add token to output\n", infix[i]);
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
            printf("%c | Pop stack\n", infix[i]);
            pop(&operator_stack);
        }
        // ✅ Operators (+, -, *, /, ^): from the operator stack to the output queue (while ...) + push to the operator stack
        else if (cIsOperator)
        {
            OPERATOR current = getOperator(infix[i]);
            char top_char = last(&operator_stack);
            OPERATOR top = getOperator(top_char);

            while ((!isEmpty(&operator_stack) && top_char != LEFT_PARENTHESIS) &&
                   (current.precedence < top.precedence || (current.precedence == top.precedence && current.associativity == LEFT)))
            {
                printf("%c | Pop stack to output\n", infix[i]);
                char operator= pop(&operator_stack);
                push(&output_queue, operator);
                push(&output_queue, DELIMITER);
                top = getOperator(first(&operator_stack));
            }

            printf("%c | Push token to stack\n", infix[i]);
            push(&operator_stack, current.op);
        }
        // ✅ number: to the output queue
        else
        {
            while (isNumber(infix[i]))
            {
                printf("%c | Add token to output\n", infix[i]);
                push(&output_queue, infix[i]);
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

// TODO: welp this wont work without changing the type of stack elements :/
double evaluatePostfix(char *postfix)
{
    STACK stack;
    init(&stack);

    for (int i = 0; i < strlen(postfix); i++)
    {
        double result;

        // Deliminater: skip
        if (postfix[i] == DELIMITER)
        {
            continue;
        }
        // Number: push to stack
        else if (!isOperator(postfix[i]))
        {
            push(&stack, postfix[i]);
        }
        // Operator: perform operation with the last two elements in the stack
        else
        {
            // pop two elements from stack
            char *yc = pop(&stack);
            char *xc = pop(&stack);

            double x, y;

            sscanf(&xc, "%lf", &x);
            sscanf(&yc, "%lf", &y);

            // perform operation with current operator on both the operands
            switch (postfix[i])
            {
            case PLUS:
                result = x + y;
                break;
            case MINUS:
                result = x - y;
                break;
            case ASTERISK:
                result = x * y;
                break;
            case SLASH:
                if (y == 0)
                {
                    printf("Division by zero, exiting...\n");
                    exit(1);
                }
                result = x / y;
                break;
            case CARET:
                result = pow(x, y);
                break;
            default:
                printf("Invalid operator, exiting...\n");
                exit(1);
            }

            // push the result back into the stack
            push(&stack, result);
        }

        return result;
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
    // char *postfix = "2,3,1,*,+,9,-";

    char *postfix = parseInput(exp);

    printf("Postfix: %s\n", postfix);

    // double result = evaluatePostfix(postfix);
    // printf("Result: %lf\n", result);
    return 0;
}