#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const char LEFT_PARENTHESIS = '(';
const char RIGHT_PARENTHESIS = ')';
const char PLUS = '+';
const char MINUS = '-';
const char ASTERISK = '*';
const char SLASH = '/';
const char CARET = '^';

struct TOKEN
{
    char *value;
    int size;
};
typedef struct TOKEN TOKEN;

bool is_operator(char c);
TOKEN *tokenize(char exp[]);
int main(void);

bool is_operator(char c)
{
    return c == LEFT_PARENTHESIS || c == RIGHT_PARENTHESIS || c == PLUS || c == MINUS || c == ASTERISK || c == SLASH || c == CARET;
}

// Tokenize math expression
// e.g. "(1+23)*456" -> {"(", "1", "+", "23", ")", "*", "456"}
TOKEN *tokenize(char exp[])
{
    TOKEN *tokens = malloc(sizeof(TOKEN) * 10);

    char *ptr = exp;
    while (*ptr != '\0')
    {
        char *value = malloc(sizeof(char) * 10);
        int size = 0;
        int i = 0;

        if (is_operator(*ptr))
        {
            value[size++] = *ptr;
            value[size] = '\0';
        }
        else
        {
            while (!is_operator(*ptr))
            {
                value[size++] = *ptr;
                ptr++;
            }
            ptr--;
            value[size] = '\0';
        }

        tokens[i].value = value;
        tokens[i].size = size;
        printf("token: %s\n", tokens[i].value);

        i++;
        ptr++;
    }

    return tokens;
}

int main(void)
{
    char exp[100] = "(1+23)*456";
    TOKEN *tokens = tokenize(exp);

    return 0;
}
