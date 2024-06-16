#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const char LEFT_PARENTHESIS = '(';
const char RIGHT_PARENTHESIS = ')';
const char PLUS = '+';
const char MINUS = '-';
const char ASTERISK = '*';
const char SLASH = '/';
const char CARET = '^';

bool is_operator(char c);
char **tokenize(char exp[]);
int main(void);

bool is_operator(char c)
{
    return c == LEFT_PARENTHESIS || c == RIGHT_PARENTHESIS || c == PLUS || c == MINUS || c == ASTERISK || c == SLASH || c == CARET;
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
