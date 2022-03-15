#include "error.h"
#include "scanner.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <program file>\n", argv[0]);
        return ERR_USAGE;
    }

    // Scanning
    int32_t error = ERR_NONE;
    struct scanner_t scanner;

    error = scanner_init(&scanner, argv[1]);

    if (error != ERR_NONE) {
        return error;
    }

    struct token_t token;
    scan_token(&token, &scanner);
    while (token.type != TK_EOF)
    {
        switch (token.type)
        {
        case TK_ADD:
            printf("ADD\n");
            break;

        case TK_SUBTRACT:
            printf("SUBTRACT\n");
            break;

        case TK_MULTIPLY:
            printf("MULTIPLY\n");
            break;

        case TK_DIVIDE:
            printf("DIVIDE\n");
            break;

        case TK_INT_LITERAL:
            printf("INT_LITERAL: %d\n", token.value);
            break;
        }

        scan_token(&token, &scanner);
    };

    return 0;
}