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

OPERATOR operators[7] = {
    {'+', 1, LEFT},
    {'-', 1, LEFT},
    {'*', 2, LEFT},
    {'/', 2, LEFT},
    {'^', 3, RIGHT},
    {'(', 0, NONE},
    {')', 0, NONE},
};

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

    while (*tokens)
    {
        printf("%s\n", *tokens);
        tokens++;
    }

    return 0;
}
