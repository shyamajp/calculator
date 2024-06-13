#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '^';
}

struct TOKEN
{
    char *value;
    int size;
};
typedef struct TOKEN TOKEN;

// Tokenize math expression
// e.g. "(1+23)*456" -> {"(", "1", "+", "23", ")", "*", "456"}
TOKEN *tokenize(char exp[])
{
    TOKEN *tokens = malloc(sizeof(TOKEN) * 10);

    for (int i = 0; exp[i] != '\0'; i++)
    {
        char *value = malloc(sizeof(char) * 10);
        int size = 0;

        if (is_operator(exp[i]))
        {
            value[size++] = exp[i];
            value[size] = '\0';
        }
        else
        {
            while (!is_operator(exp[i]))
            {
                value[size++] = exp[i];
                i++;
            }
            value[size] = '\0';
            i--;
        }

        tokens[i].value = value;
        tokens[i].size = size;

        printf("value: %s\n", value);
    }

    return tokens;
}

int main(void)
{
    char exp[100] = "(1+23)*456";
    TOKEN *tokens = tokenize(exp);

    return 0;
}
