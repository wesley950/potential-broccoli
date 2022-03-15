#include "scanner.h"
#include "error.h"
#include "log.h"

#include <stdlib.h>

int32_t scanner_init(struct scanner_t *scanner, const char *program_path)
{
    // Try to open the file
    FILE *program_file = fopen(program_path, "r");
    if (program_file == NULL)
    {
        LOG_ERROR("File not found or could not be opened: %s\n", program_path)
        return ERR_PROGRAM_NOT_FOUND;
    }

    // Calculate the length of the buffer
    fseek(program_file, 0, SEEK_END);
    int64_t length = ftell(program_file) + 1;
    rewind(program_file);

    // Try to allocate the buffer
    scanner->source = (char *)malloc(sizeof(char) * length);
    if (scanner->source == NULL)
    {
        LOG_ERROR("Could not allocate %ld bytes of memory!\n", length)
        fclose(program_file);
        return ERR_CAN_NOT_ALLOCATE;
    }

    // Read the contents of the file into the buffer
    fread(scanner->source, sizeof(char), length - 1, program_file);
    fclose(program_file);

    // Reset the scanner state and put an EOF mark
    // at the end of it
    scanner->source[length - 1] = EOF;
    scanner->position = 0;
    scanner->line = 1;
    scanner->column = 0;
    scanner->source_length = length;

    // Try to allocate the token list
    scanner->token_count = 0;
    scanner->tokens = (struct token_t *)malloc(sizeof(struct token_t) * SCANNER_MAX_TOKENS);

    return 0;
}

int32_t scan_token(struct token_t *token, struct scanner_t *scanner)
{
    char c = scanner_current_char(scanner);

    // Skip the whitechars
    if (is_whitechar(c))
    {
        if (c == '\n')
        {
            scanner->line++;
            scanner->column = 0;
        }

        while (is_whitechar(c))
        {
            c = scanner_next_char(scanner);
        }
    }

    // Check for end of file
    if (c == EOF)
    {
        token->type = TK_EOF;
        token->value = 0;
        return ERR_NONE;
    }

    // Then scan next token
    if (is_digit(c))
    {
        scan_integer_literal(token, scanner);
    }
    else if (is_operator(c))
    {
        scan_operator(token, scanner);
    }
    else
    {
        LOG_ERROR("Unexpected character '%c' on line %d, column %d.\n", c, scanner->line, scanner->column)
        return ERR_UNEXPECTED_INPUT;
    }

    // Push the token to the end of the list
    scanner->tokens[scanner->token_count] = *token;
    scanner->token_count++;

    return ERR_NONE;
}

void scan_integer_literal(struct token_t *token, struct scanner_t *scanner)
{
    int32_t integer_literal = 0;
    char c = scanner_current_char(scanner);

    // While the current char is a digit, convert it to an integer, multiply the
    // current number by ten and then add to the converted number
    while (is_digit(c))
    {
        int digit = 0;

        switch (c)
        {
        case (int)'0':
            digit = 0;
            break;

        case (int)'1':
            digit = 1;
            break;

        case (int)'2':
            digit = 2;
            break;

        case (int)'3':
            digit = 3;
            break;

        case (int)'4':
            digit = 4;
            break;

        case (int)'5':
            digit = 5;
            break;

        case (int)'6':
            digit = 6;
            break;

        case (int)'7':
            digit = 7;
            break;

        case (int)'8':
            digit = 8;
            break;

        case (int)'9':
            digit = 9;
            break;
        }

        integer_literal *= 10;
        integer_literal += digit;

        c = scanner_next_char(scanner);
    }

    // Set the token fields
    token->type = TK_INT_LITERAL;
    token->value = integer_literal;
}

void scan_operator(struct token_t *token, struct scanner_t *scanner)
{
    char c = scanner_current_char(scanner);
    switch (c)
    {
    case '+':
        token->type = TK_ADD;
        break;

    case '-':
        token->type = TK_SUBTRACT;
        break;

    case '*':
        token->type = TK_MULTIPLY;
        break;

    case '/':
        token->type = TK_DIVIDE;
        break;
    }

    // Skip to the next char since currently operators
    // only have one char
    scanner_next_char(scanner);
}

int32_t scanner_current_char(struct scanner_t *scanner)
{
    if (scanner->position < scanner->source_length)
    {
        return scanner->source[scanner->position];
    }
    return EOF;
}

int32_t scanner_next_char(struct scanner_t *scanner)
{
    scanner->position++;
    scanner->column++;
    return scanner_current_char(scanner);
}

int is_digit(int c)
{
    switch (c)
    {
    case (int)'0':
    case (int)'1':
    case (int)'2':
    case (int)'3':
    case (int)'4':
    case (int)'5':
    case (int)'6':
    case (int)'7':
    case (int)'8':
    case (int)'9':
        return 1;
    }

    return 0;
}

int is_operator(int c)
{
    switch (c)
    {
    case '+':
    case '-':
    case '*':
    case '/':
        return 1;
    }

    return 0;
}

int is_whitechar(int c)
{
    switch (c)
    {
    case ' ':
    case '\n':
    case '\t':
        return 1;
    }

    return 0;
}
