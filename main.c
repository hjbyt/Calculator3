/*
 * Main Module
 */

/*
 * GENERAL TODO:
 *      - errors need to be propagated backwards ("messaging") instead of calling panic (see: http://moodle.tau.ac.il/mod/forum/discuss.php?d=28021)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include "tree.h"
#include "parse.h"
#include "calculate.h"
#include "common.h"

/*
 * Structs
 */

typedef struct CommandLineArgs_
{
    char* variable_input_file;
    char* output_file;
} CommandLineArgs;

/*
 * Function Declarations
 */

bool parseCommandLineArguments(int argc, char **argv, CommandLineArgs* parsed_args);
void interact(HashTable variables, FILE* output_file);
void getLine(char* buffer, unsigned int size);

/*
 * Function Implementations
 */

/**
 * Main function.
 * Interact with the user, calculating results of lisp arithmetic expressions,
 * until a quit command is received.
 *
 * @return
 *      EXIT_SUCCESS if no error was encountered,
 *      EXIT_FAILURE otherwise.
 */
int main(int argc, char **argv)
{
    int return_value = EXIT_FAILURE;
    FILE* variable_input_file = NULL;
    FILE* output_file = NULL;

    /* Parse args */
    CommandLineArgs parsed_args;
    bool had_parse_error = parseCommandLineArguments(argc, argv, &parsed_args);
    if (had_parse_error) {
        printf("Invalid command line arguments, use [-v filename1] [-o filename2]\n");
        goto end;
    }
    if (parsed_args.variable_input_file != NULL
        && parsed_args.output_file != NULL
        && strcmp(parsed_args.variable_input_file, parsed_args.output_file) == 0) {
        printf("Files must be different\n");
        goto end;
    }

    /* Open files */
    if (parsed_args.variable_input_file != NULL) {
        variable_input_file = fopen(parsed_args.variable_input_file, "r");
        if (variable_input_file == NULL) {
            printf("Variable init file doesn't exist or is not readable\n");
            goto end;
        }
    }
    if (parsed_args.output_file != NULL) {
        output_file = fopen(parsed_args.output_file, "w");
        if (output_file == NULL) {
            printf("Output file is read-only or cannot be created\n");
            goto end;
        }
    }

    /* Parse initial variables */
    HashTable variables = createHashTable();
    if (variable_input_file != NULL) {
        parseVariableInputFile(variable_input_file, variables);
    }

    /* Interact with user */
    interact(variables, output_file);

    return_value = EXIT_SUCCESS;

end:
    if (variable_input_file != NULL) {
        fclose(variable_input_file);
    }
    if (output_file != NULL) {
        fclose(output_file);
    }
    return return_value;
}

/*
 * Internal Functions
 */

/* TODO: doc */
bool parseCommandLineArguments(int argc, char **argv, CommandLineArgs* parsed_args)
{
    VERIFY(parsed_args != NULL);

    /* Initialize to defaults */
    parsed_args->variable_input_file = NULL;
    parsed_args->output_file = NULL;

    /* Parse args */
    int c;
    while ((c = getopt(argc, argv, "v:o:")) != -1)
    {
        switch (c) {
            case 'v':
                parsed_args->variable_input_file =  optarg;
                break;
            case 'o':
                parsed_args->output_file =  optarg;
                break;
            case '?':
                return true;
            default:
                panic();
        }
    }

    return false;
}

/* TODO: doc */
void interact(HashTable variables, FILE* output_file)
{
    bool should_print_expression = true;
    if (output_file == NULL) {
        output_file = stdout;
        should_print_expression = false;
    }

    while (true)
    {
        char lisp_expression[MAX_LINE_LENGTH + 1];
        getLine(lisp_expression, sizeof(lisp_expression));

        Tree* parse_tree = parseLispExpression(lisp_expression);

        if (should_print_expression) {
            char expression_string[MAX_LINE_LENGTH + 1];
            expressionToString(parse_tree, expression_string, sizeof(expression_string));
            fprintf(output_file, "%s\n", expression_string);
        }

        if (isEndCommand(parse_tree)) {
            destroyTree(parse_tree);
            fprintf(output_file, "Exiting...\n");
            break;
        }

        /* TODO: use variables in evaluateExpressionTree()*/
        double result = evaluateExpressionTree(parse_tree);
        if (isnan((float)result)) {
            fprintf(output_file, "Invalid Result\n");
        } else {
            fprintf(output_file, "res = %.2f\n", result);
        }

        destroyTree(parse_tree);
    }
}

/**
 * Receive a single input line from the user.

 * @param
 *      char* buffer      - Pre-allocated buffer to store line in.
 *      unsigned int size - Buffer size.
 *
 * @preconditions
 *      buffer != NULL
 */
void getLine(char* buffer, unsigned int size)
{
    VERIFY(buffer != NULL);
    char* fgets_result = fgets(buffer, size, stdin);
    VERIFY(0 == ferror(stdin));
    VERIFY(fgets_result != NULL);

    size_t last_char_index = strlen(buffer) - 1;
    if (buffer[last_char_index] == '\n') {
        buffer[last_char_index] = '\0';
    }
}
