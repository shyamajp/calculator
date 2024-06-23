#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

enum ASSOCIATIVITY
{
    LEFT,
    RIGHT,
    NONE,
};

enum TYPE
{
    NUMBER,
    OPERATOR,
    LEFT_PARENTHESES,
    RIGHT_PARENTHESES,
};

typedef struct TOKEN
{
    char *value;
    int precedence;
    enum ASSOCIATIVITY associativity;
    enum TYPE type;
} TOKEN;

typedef struct STACK
{
    TOKEN elm[100]; // TODO: malloc this?
    int size;
} STACK;

void init(STACK *stack)
{
    stack->size = -1;
}

void print(STACK *stack)
{
    for (int i = 0; i <= stack->size; i++)
    {
        printf("%s ", stack->elm[i].value);
    }
    printf("\n");
}

void push(STACK *stack, TOKEN token)
{
    stack->size++;
    stack->elm[stack->size] = token;
}

TOKEN first(STACK *stack)
{
    return stack->elm[0];
}

TOKEN last(STACK *stack)
{
    return stack->elm[stack->size];
}

TOKEN pop(STACK *stack)
{
    TOKEN t = last(stack);
    // TODO: hmm?
    TOKEN n;
    stack->elm[stack->size] = n;
    stack->size--;
    return t;
}

bool isEmpty(STACK *stack)
{
    return stack->size == -1;
}

bool isOperator(char c)
{
    if (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
    {
        return true;
    }

    return false;
}

/**
 * @brief Make string into token
 * @param char* a token in string
 * @return TOKEN token struct with value, precedence, and associativity
 * @example "+" -> value: "+", precedence: 1, associativity: "LEFT"
 * @example "123" -> value: "123", precedence: -1, associativity: "NONE
 */
TOKEN getToken(char *str)
{
    TOKEN token;
    token.value = str;

    if (str[0] == '(')
    {
        token.precedence = 0;
        token.associativity = NONE;
        token.type = LEFT_PARENTHESES;
    }
    else if (str[0] == ')')
    {
        token.precedence = 0;
        token.associativity = NONE;
        token.type = RIGHT_PARENTHESES;
    }
    else if (str[0] == '+' || str[0] == '-')
    {
        token.precedence = 1;
        token.associativity = LEFT;
        token.type = OPERATOR;
    }
    else if (str[0] == '*' || str[0] == '/')
    {
        token.precedence = 2;
        token.associativity = LEFT;
        token.type = OPERATOR;
    }
    else if (str[0] == '^')
    {
        token.precedence = 3;
        token.associativity = RIGHT;
        token.type = OPERATOR;
    }
    else
    {
        token.precedence = -1;
        token.associativity = NONE;
        token.type = NUMBER;
    }

    return token;
}

/**
 * @brief Tokenize math expression
 * @param char* math expression
 * @return char** pointers to each token in string
 * @example "(1+23)*456" -> {"(", "1", "+", "23", ")", "*", "456"}
 */
TOKEN *tokenize(char *exp)
{
    TOKEN *tokens = malloc(sizeof(TOKEN) * 10);

    char *ptr = exp;
    int i = 0;
    while (*ptr != '\0')
    {
        char *value = malloc(sizeof(char) * 10);

        if (isOperator(*ptr))
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
            } while (!isOperator(*ptr) && *ptr != '\0');
            ptr--;
            value[size] = '\0';
        }

        tokens[i] = getToken(value);

        i++;
        ptr++;
    }

    return tokens;
}

/**
 * @brief Parse infix expression to postfix expression
 * @param char* infix expression
 * @return char* postfix expression separated by DELIMITER
 * @example "3+4*2/(1-5)^2^3" -> "3,4,2,*,1,5,−2,3,^,^,÷,+"
 */
TOKEN *convertToPostfix(TOKEN *infix)
{

    STACK operator_stack;
    STACK output_queue;

    // initialize
    init(&operator_stack);
    init(&output_queue);

    TOKEN *tokens = infix;

    // Shunting yard algorithm
    while (tokens->value != NULL)
    {
        TOKEN token = *tokens;
        printf("token: %s\n", token.value);

        // ✅ left parenthesis: always push to the output queue
        if (token.type == LEFT_PARENTHESES)
        {
            printf("%s | push it onto the operator stack\n", token.value);
            push(&operator_stack, token);
        }
        // ✅ right parenthesis: pop operators in the operator stack to the output queue until left parenthesis
        else if (token.type == RIGHT_PARENTHESES)
        {
            TOKEN top = last(&operator_stack);
            while (top.type != LEFT_PARENTHESES)
            {
                // 🚨 assert: the operator stack is not empty
                if (isEmpty(&operator_stack))
                {
                    printf("ERR1: Invalid expression, exiting...\n");
                    exit(1);
                }

                TOKEN operator= pop(&operator_stack);
                push(&output_queue, operator);
                printf("%s | pop the operator from the operator stack into the output queue\n", operator.value);
                top = last(&operator_stack);
            }

            // 🚨 assert: there is a left parenthesis at the top of the operator stack
            if (top.type != LEFT_PARENTHESES)
            {
                printf("ERR2: Invalid expression, exiting...\n");
                exit(1);
            }
            printf("%s | pop the left parenthesis from the operator stack and discard it\n", top.value);
            pop(&operator_stack);
        }
        // ✅ Operators (+, -, *, /, ^): from the operator stack to the output queue (while ...) + push to the operator stack
        else if (token.type == OPERATOR)
        {
            TOKEN top = last(&operator_stack);

            while ((!isEmpty(&operator_stack) && top.type != LEFT_PARENTHESES) &&
                   (token.precedence < top.precedence || (token.precedence == top.precedence && token.associativity == LEFT)))
            {
                TOKEN operator= pop(&operator_stack);
                push(&output_queue, operator);
                printf("%s | pop o2 from the operator stack into the output queue\n", operator.value);

                top = first(&operator_stack);
            }

            printf("%s | push o1 onto the operator stack\n", token.value);
            push(&operator_stack, token);
        }
        // ✅ number: to the output queue
        else
        {
            printf("%s | Add number to output queue\n", token.value);
            push(&output_queue, token);
        }
        printf("\n");
        tokens++;

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
        TOKEN operator= last(&operator_stack);
        // 🚨 assert: the operator on top of the stack is not a (left) parenthesis
        if (operator.type == RIGHT_PARENTHESES)
        {
            printf("ERR3: Invalid expression, exiting...\n");
            exit(1);
        }

        printf("%s | Pop stack to output\n", operator.value);
        pop(&operator_stack);
        push(&output_queue, operator);

        // For debugging purpose
        printf("output: ");
        print(&output_queue);
        printf("operator: ");
        print(&operator_stack);
        printf("=====================================\n");
    }
    // For debugging purpose
    printf("FINAL: ");
    print(&output_queue);

    return output_queue.elm;
}

double performeOperation(double a, double b, TOKEN operator)
{
    if (operator.value[0] == '+')
    {
        return a + b;
    }
    else if (operator.value[0] == '-')
    {
        return a - b;
    }
    else if (operator.value[0] == '*')
    {
        return a * b;
    }
    else if (operator.value[0] == '/')
    {
        return a / b;
    }
    else if (operator.value[0] == '^')
    {
        return pow(a, b);
    }
    printf("ERR4: Invalid operator, exiting...\n");
    exit(1);
}

double evaluatePostfix(TOKEN *postfix)
{

    STACK stack;
    init(&stack);

    TOKEN *tokens = postfix;

    while (tokens->value != NULL)
    {
        printf("token: %s\n", tokens->value);

        // Number: push to the stack
        if (tokens->type == NUMBER)
        {
            push(&stack, *tokens);
        }
        // Operator: pop two numbers from the stack, calculate, and push the result to the stack
        else if (tokens->type == OPERATOR)
        {
            double a = atof(pop(&stack).value);
            double b = atof(pop(&stack).value);

            double result = performeOperation(b, a, *tokens);

            printf("a: %f | b: %f\n", a, b);
            printf("operator %s | result: %f\n", tokens->value, result);

            char result_str[32];
            sprintf(result_str, "%2.13f", result);

            TOKEN token = getToken(result_str);
            push(&stack, token);
        }
        // TODO: idk how to get out of the loop :(
        else
        {
            break;
        }
        // For debugging purpose
        printf("stack: ");
        print(&stack);
        printf("=====================================\n");

        tokens++;
    }

    char *final_str = pop(&stack).value;
    double final = atof(final_str);
    return final;
}

int main(void)
{

    char exp[100];

    printf("Enter: \n");
    scanf("%s", exp);

    TOKEN *tokens = tokenize(exp);

    TOKEN *postfix = convertToPostfix(tokens);

    double result = evaluatePostfix(postfix);
    printf("result: %f\n", result);
    return 0;
}
