#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "my_bc.h"


int main (int argc, char** argv) {

    //checks number of command line args
    if (argc != 2) {
        printf("invalid input\n");
        return 1;
    }

    //initializes stack
    char* stack = malloc((my_strlen(argv[1]) + 1) * sizeof(char));
    stack[0] = '\0';

    //initializes output queue
    string_array* output = malloc(sizeof(string_array));
    output->size = my_strlen(argv[1]);
    output->array = malloc(output->size * sizeof(char*));

    for (int i = 0; i < output->size; i++) {
        output->array[i] = malloc((output->size + 1) * sizeof(char));
        output->array[i][0] = '\0';
    }

    //counter for number of elements in output
    int j = -1;

    //variable to track the previous element that was added to detect parsing errors
    //1 - number, 2 - operator, 3 - opening bracket, 4 - closing bracket
    int prev_element = 0;
    
    //parses input string
    for (int i = 0; i < my_strlen(argv[1]); i++) {
        //numbers incl. numbers with multiple digits 
        if (is_digit(argv[1][i]) == 0) {
            
            if (prev_element == 1 || prev_element == 4) {
                write(2, "parse error\n", 12);
                free_output_and_stack(output, stack);
                return 1;
            }
            
            j++;
            while (is_digit(argv[1][i]) == 0) {
                add_char(output->array[j], argv[1][i]);
                i++;
            }
            i--;
            prev_element = 1;

        //operators
        } else if (is_operator(argv[1][i]) == 0) {

            if (prev_element == 0 || prev_element == 2 || prev_element == 3 || i == my_strlen(argv[1]) - 1) {
                write(2, "parse error\n", 12);
                free_output_and_stack(output, stack);             
                return 1;
            }

            //if stack is empty, adds operator to stack
            if (stack[0] == '\0') {
                add_char(stack, argv[1][i]);
                prev_element = 2;

            } else {

                //if stack is not empty, compares precedence of current operator with operators on stack
                //if current operator has higher precedence than last operator on stack, adds operator to stack
                if (precedence(argv[1][i], stack[my_strlen(stack) - 1]) == 0) {
                    add_char(stack, argv[1][i]);
                    prev_element = 2;

                //else moves last operator from stack to queue, repeats this until current operator has higher precedence, then adds current operator to stack
                } else if (precedence(argv[1][i], stack[my_strlen(stack) - 1]) == 1) {
                    while (my_strlen(stack) > 0 && precedence(argv[1][i], stack[my_strlen(stack) - 1]) == 1) {
                        j++;
                        add_char(output->array[j], stack[my_strlen(stack) - 1]);
                        stack[my_strlen(stack) - 1] = '\0';
                    }
                    
                    add_char(stack, argv[1][i]);
                    prev_element = 2;
                }
            }

        //brackets   
        } else if (is_bracket(argv[1][i]) == 0) {

            //opening bracket: adds bracket to stack
            if (argv[1][i] == '(') {

                if (prev_element == 1 || prev_element == 4) {
                    write(2, "parse error\n", 12);
                    free_output_and_stack(output, stack);               
                    return 1;
                }

                add_char(stack, argv[1][i]);
                prev_element = 3;

            //closing bracket: moves existing operators from stack to output until we reach opening bracket
            } else if (argv[1][i] == ')') {

                if (prev_element == 0 || prev_element == 2 || prev_element == 3) {
                    write(2, "parse error\n", 12);
                    free_output_and_stack(output, stack);
                    return 1;
                }

                while (my_strlen(stack) > 0 && stack[my_strlen(stack) - 1] != '(') {
                    j++;
                    add_char(output->array[j], stack[my_strlen(stack) - 1]);
                    stack[my_strlen(stack) - 1] = '\0';
                }

                //removes opening bracket from stack
                if (my_strlen(stack) > 0 && stack[my_strlen(stack) - 1] == '(') {
                    stack[my_strlen(stack) - 1] = '\0';
                    prev_element = 4;
                } else {
                    write(2, "parse error\n", 12);
                    free_output_and_stack(output, stack);                   
                    return 1;
                }
            }

        //spaces are skipped unless they cause a parsing error
        } else if (argv[1][i] == ' ') {
            
            //handles trailing spaces and empty strings
            if (i == my_strlen(argv[1]) - 1 && (prev_element == 2 || prev_element == 0)) {
                write(2, "parse error\n", 12);
                free_output_and_stack(output, stack);              
                return 1;
            }
            continue;

        //everything else is always a parsing error
        } else {
            write(2, "parse error\n", 12);
            free_output_and_stack(output, stack);
            return 1;
        }        
    }

    //moves remaining operators from stack to output
    if (my_strlen(stack) > 0) {
        for (int i = my_strlen(stack) - 1; i > -1; i--) {
            
            //if opening brackets remain on stack, brackets were incorrectly placed in input
            if (stack[i] == '(') {
                write(2, "parse error\n", 12);
                free_output_and_stack(output, stack);
                return 1;
            }

            j++;
            add_char(output->array[j], stack[i]);
        }
        stack[0] = '\0';
    }

    int output_len = j + 1;

    //initializes postfix for reverse polish notation
    int* postfix = malloc((output_len) * sizeof(int));
    int pos = 0;

    //calculates results
    for (int i = 0; i < output_len; i++) {
        //for digits - converts from string to int and ads digit to postfix
        if (is_digit(output->array[i][0]) == 0) {
            postfix[pos] = my_atoi(output->array[i]);
            pos++;

        //for operator - pops the last two digits from postfix, performs calculation depending on operator, adds result to postfix    
        } else if (is_operator(output->array[i][0]) == 0) {
            int num_1 = postfix[pos - 2];
            int num_2 = postfix[pos - 1];
            int result = 0;
            pos = pos - 2;

            if (output->array[i][0] == '*') {
                result = num_1 * num_2;
            } else if (output->array[i][0] == '/') {
                if (num_2 == 0) {
                    write(2, "divide by zero\n", 15);
                    free_output_and_stack(output, stack);
                    free(postfix);
                    return 1;
                }
                result = num_1 / num_2;
            } else if (output->array[i][0] == '%') {
                if (num_2 == 0) {
                    write(2, "divide by zero\n", 15);
                    free_output_and_stack(output, stack);
                    free(postfix);
                    return 1;
                }                
                result = num_1 % num_2;
            } else if (output->array[i][0] == '+') {
                result = num_1 + num_2;
            } else if (output->array[i][0] == '-') {
                result = num_1 - num_2;
            } 
            postfix[pos] = result;
            pos++;
        }
    }

    printf("%d\n", postfix[0]);

    free(postfix);
    free_output_and_stack(output, stack);

    return 0;

}

//frees output and stack
void free_output_and_stack(string_array* output, char* stack) {
    for (int i = 0; i < output->size; i++) {
        free(output->array[i]);
    }
    free(output->array);
    free(output);
    free(stack);
}

//custom version of strlen
int my_strlen(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }

    return i;
}


int is_digit(char c) {
    if (c >= '0' && c <= '9') {
        return 0;
    } else {
        return 1;
    }
}

int is_operator(char c) {
    if (c == '-' || c == '+' || c == '/' || c == '*' || c == '%') {
        return 0;
    } else {
        return 1;
    }
}

int is_bracket(char c) {
    if (c == '(' || c == ')') {
        return 0;
    } else {
        return 1;
    }
}

//checks operator precedence
//0 - current operator is of higher precedence and can be added to stack
//1 - existing operator(s) need to be popped from stack first
int precedence(char a, char b) {
    if ((a == '-' || a == '+') && (b == '-' || b == '+')) {
        return 1;
    } else if ((a == '-' || a == '+') && (b == '/' || b == '*' || b == '%')) {
        return 1;
    } else if ((a == '/' || a == '*' || a == '%') && (b == '/' || b == '*' || b == '%')) {
        return 1;
    } else {
        return 0;
    }
}

//adds one char to the end of a string
void add_char(char* str, char c) {
    int len = my_strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
}

//converts number as a string to int
int my_atoi(char* str) {
    int len = my_strlen(str);
    int result = 0;
    for (int i = 0; i < len; i++) {
        result *= 10;
        int n = str[i] - 48;
        result += n;
    }
    return result;
}