#ifndef MY_BC_H
#define MY_BC_H

typedef struct {
    int size;
    char** array;
} string_array;

int my_strlen(char* str);
int is_digit(char c);
int is_operator(char c);
int is_bracket(char c);
int precedence(char a, char b);
void add_char(char* str, char c);
int my_atoi(char* str);
void free_output_and_stack(string_array* output, char* stack);

#endif