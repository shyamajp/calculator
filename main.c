#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

enum ASSOCIATIVITY
{
    LEFT,
    RIGHT,
    NONE,
};

typedef struct OPERATOR
{
    char op;
    int precedence;
    enum ASSOCIATIVITY associativity;
} OPERATOR;

typedef struct TOKEN
{
    char *value;
    int size;
} TOKEN;

typedef struct STACK
{
    char *elm[100]; // TODO: malloc this?
    int size;
} STACK;

OPERATOR operators[7] = {
    {'+', 1, LEFT},
    {'-', 1, LEFT},
    {'*', 2, LEFT},
    {'/', 2, LEFT},
    {'^', 3, RIGHT},
    {'(', 0, NONE},
    {')', 0, NONE},
};

void init(STACK *stack)
{
    stack->size = -1;
}

void print(STACK *stack)
{
    for (int i = 0; i <= stack->size; i++)
    {
        printf("%s ", stack->elm[i]);
    }
    printf("\n");
}

void push(STACK *stack, char *c)
{
    stack->size++;
    stack->elm[stack->size] = c;
}

char *first(STACK *stack)
{
    char *c = stack->elm[0];
    return c;
}

char *last(STACK *stack)
{
    char *c = stack->elm[stack->size];
    return c;
}

char *pop(STACK *stack)
{
    char *c = last(stack);
    stack->elm[stack->size] = NULL;
    stack->size--;
    return c;
}

bool isEmpty(STACK *stack)
{
    return stack->size == -1;
}

bool is_operator(char c)
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

/**
OPERATOR getOperator(char *token)
{
    for (size_t i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
    {
        if (token[0] == operators[i].op)
        {
            return operators[i];
        }
    }
}

 * @brief Parse infix expression to postfix expression
 * @param char* infix expression
 * @return char* postfix expression separated by DELIMITER
 * @example "3+4*2/(1-5)^2^3" -> "3,4,2,*,1,5,−2,3,^,^,÷,+"
char **parseInput(char **infix)
{

    STACK operator_stack;
    STACK output_queue;

    // initialize
    init(&operator_stack);
    init(&output_queue);

    char **token = infix;

    // Shunting yard algorithm
    while (*token)
    {
        printf("token: %s ", *token);

        char c = *token[0];

        if (is_operator(c))
        {
            // ✅ left parenthesis: always push to the output queue
            if (c == '(')
            {
                printf("%s | Push token to stack\n", *infix);
                push(&operator_stack, *infix);
            }
            // ✅ right parenthesis: pop operators in the operator stack to the output queue until left parenthesis
            else if (c == ')')
            {
                char *top_char = last(&operator_stack);

                while (top_char[0] != ')')
                {
                    // 🚨 assert: the operator stack is not empty
                    if (isEmpty(&operator_stack))
                    {
                        printf("Invalid expression, exiting...\n");
                        exit(1);
                    }
                    char *operator= pop(&operator_stack);
                    printf("%s | Add token to output\n", *infix);
                    push(&output_queue, operator);

                    top_char = last(&operator_stack);
                }

                // 🚨 assert: there is a left parenthesis at the top of the operator stack
                if (top_char[0] != ')')
                {
                    printf("Invalid expression, exiting...\n");
                    exit(1);
                }
                printf("%s | Pop stack\n", *infix);
                pop(&operator_stack);
            }
            // ✅ Operators (+, -, *, /, ^): from the operator stack to the output queue (while ...) + push to the operator stack
            else
            {
                OPERATOR current_operator = getOperator(*infix);
                char *top_char = last(&operator_stack);
                OPERATOR top_operator = getOperator(top_char);

                while ((!isEmpty(&operator_stack) && top_char[0] != '(') &&
                       (current_operator.precedence < top_operator.precedence || (current_operator.precedence == top_operator.precedence && current_operator.associativity == LEFT)))
                {
                    printf("%s | Pop stack to output\n", *infix);
                    char *operator= pop(&operator_stack);
                    push(&output_queue, operator);
                    top_operator = getOperator(first(&operator_stack));
                }

                printf("%s | Push token to stack\n", *infix);
                char *temp = {current_operator.op, '\0'};
                push(&operator_stack, temp);
            }
        }
        // ✅ number: to the output queue
        else
        {
            printf("%s | Add token to output\n", *token);
            push(&output_queue, *token);
        }
        printf("\n");
        token++;
    }

    // For debugging purpose
    printf("output: ");
    print(&output_queue);
    printf("operator: ");
    print(&operator_stack);
    printf("=====================================\n");

    // ✅ all remainings: from the operator stack to the output queue
    while (!isEmpty(&operator_stack))
    {
        // 🚨 assert: the operator on top of the stack is not a (left) parenthesis
        if (last(&operator_stack) == ')')
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
 */

/**
 * @brief Tokenize math expression
 * @param char* math expression
 * @return char** pointers to each token in string
 * @example "(1+23)*456" -> {"(", "1", "+", "23", ")", "*", "456"}
 */
char **tokenize(char *exp)
{
    char **tokens = malloc(sizeof(char *) * 10);

    char *ptr = exp;
    int i = 0;
    while (*ptr != '\0')
    {
        char *value = malloc(sizeof(char) * 10);

        if (is_operator(*ptr))
        {
            value[0] = *ptr;
            value[1] = '\0';
        }
        else
        {
            int size = 0;
            do
            {
                value[size] = *ptr;
                ptr++;
                size++;
            } while (!is_operator(*ptr) && *ptr != '\0');
            ptr--;
            value[size] = '\0';
        }

        tokens[i] = value;

        i++;
        ptr++;
    }

    return tokens;
}

int main(void)
{
    char exp[100] = "(1+23)*456";
    char **tokens = tokenize(exp);

    // parseInput(tokens);

    return 0;
}
