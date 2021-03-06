#include "log.h"
#include "error.h"
#include "scanner.h"
#include "parser.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        LOG_ERROR("Usage: %s <program file>", argv[0])
        return ERR_USAGE;
    }

    // Scanning
    int32_t error = ERR_NONE;
    struct scanner_t scanner;

    error = scanner_init(&scanner, argv[1]);

    if (error != ERR_NONE)
    {
        return error;
    }

    struct token_t token;
    error = scan_token(&token, &scanner);

    if (error != ERR_NONE)
    {
        return error;
    }

    while (token.type != TK_EOF)
    {
        error = scan_token(&token, &scanner);

        if (error != ERR_NONE)
        {
            return error;
        }
    };

    // Parsing
    struct parser_t parser;
    parser_init(&parser, &scanner);
    parse_tokens(&parser);

    LOG("Final Result: %d\n", parser_evaluate(parser.ast_root));

    return 0;
}