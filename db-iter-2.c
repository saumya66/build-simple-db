#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer()
{
    InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void read_input(InputBuffer *input_buffer)
{
    // ssize_t bytesRead = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    size_t bufferSize = 1024;
    input_buffer->buffer = (char *)malloc(bufferSize);
    if (fgets(input_buffer->buffer, bufferSize, stdin) == NULL)
    {
        printf("error reading input\n");
        exit(EXIT_FAILURE);
    }
    // Calculate the length of the input
    input_buffer->input_length = strlen(input_buffer->buffer);

    // Remove the trailing newline character if present
    if (input_buffer->buffer[input_buffer->input_length - 1] == '\n')
    {
        input_buffer->buffer[input_buffer->input_length - 1] = '\0';
        input_buffer->input_length--;
    }
}

void close_input_buffer(InputBuffer *input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

void print_prompt()
{
    printf("db > ");
}

typedef enum
{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED,
} META_COMMAND;

typedef enum
{
    STATEMENT_INPUT_TYPE,
    STATEMENT_SELECT_TYPE,
} StatementType;

typedef struct
{
    StatementType type;
} Statement;

typedef enum
{
    PREPARE_STATEMENT_SUCCESS,
    PREPARE_STATEMENT_UNRECOGNIZED,
} PREPARE_STATEMENT;

META_COMMAND is_meta_command(InputBuffer *inputBuffer)
{
    if (strcmp(inputBuffer->buffer, ".exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        return META_COMMAND_UNRECOGNIZED;
    }
}

PREPARE_STATEMENT prepare_statement(InputBuffer *inputBuffer, Statement *statement)
{
    if (strncmp(inputBuffer->buffer, "insert", 6) == 0)
    {
        statement->type = STATEMENT_INPUT_TYPE;
        return PREPARE_STATEMENT_SUCCESS;
    }
    else if (strncmp(inputBuffer->buffer, "select", 6) == 0)
    {
        statement->type = STATEMENT_SELECT_TYPE;
        return PREPARE_STATEMENT_SUCCESS;
    }
    else
    {
        return PREPARE_STATEMENT_UNRECOGNIZED;
    }
}

void execute_statement(Statement *statement)
{
    switch (statement->type)
    {
    case STATEMENT_INPUT_TYPE:
        printf("INPUT OP EXECUTED\n");
        break;
    case STATEMENT_SELECT_TYPE:
        printf("SELECT OP EXECUTED\n");
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    InputBuffer *input_buffer = new_input_buffer();
    for (;;) // equvalue to a infinite while loop
    {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') // this if block handles Meta commands or basically non-sql commands
        {
            switch (is_meta_command(input_buffer)) // checks if command is a valid meta command and executes it
            {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED):
                printf("meta command is unrecongnized: %s\n", input_buffer->buffer);
                continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) // check if the input_buffer that user entered is a valid sql command, if so then assign it to our Statement instance
        {
        case (PREPARE_STATEMENT_SUCCESS):
            break;
        case (PREPARE_STATEMENT_UNRECOGNIZED):
            printf("unrecognized keyword at start of statement'%s'.\n", input_buffer->buffer);
            continue;
        }

        execute_statement(&statement); // if we have reached here then our statement has a valid command and we know need to execute the operation corresponding to that command
    };
}