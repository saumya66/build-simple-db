#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct // Here we declare the type definitions of the InputBuffer Class
{
    char *buffer;
    size_t buffer_length; // size_t is same as ssize_t but it is unsigned and can only hold +ve values
    ssize_t input_length; // ssize_t is a signed data type meaning it can hold +ve as well as -ve values, it is mainly used to count no of bytes
} InputBuffer;

InputBuffer *new_input_buffer() // on call of this we initialize a new instance of InputBuffer with default initial values and return
{
    InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer *input_buffer)
{
    // ssize_t bytesRead = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin); //i don't know why getline was not working for me, i ended up using fgets
    size_t bufferSize = 1024;
    input_buffer->buffer = (char *)malloc(bufferSize); // malloc is used to dynamically allocate memory on heap and is freed when the free function is called
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
void close_input_buffer(InputBuffer *input_buffer) // on call of this, memory being used by the program is freed
{
    free(input_buffer->buffer);
    free(input_buffer);
}

int main(int argc, char *argv[])
{
    InputBuffer *input_buffer = new_input_buffer(); // get a new instance of InputBuffer
    while (true)                                    // start a repl or Read, Evaluate, Print, and Loop, like any shell
    {
        print_prompt();
        read_input(input_buffer); // reads input from user and assigns to the input_buffer

        if (strcmp(input_buffer->buffer, ".exit") == 0)
        {
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Unrecognized command '%s'.\n", input_buffer->buffer);
        }
    }
}