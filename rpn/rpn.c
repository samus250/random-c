/*******************************************************************************
    rpn.c - written by samus250
    Converts infix expressions to rpn expressions.
    Oh, and it is extremely fucking fast, but not as fast as my
    redirected input version...
*******************************************************************************/

#include <stdio.h>
#include <ctype.h>

int isop(char c);
int assoc(char c);
int prec(char a, char b);
void push(char c);
char pop(void);
char top(void);

#define BUFF_SIZE 1048576 // 1MiB
char buffer[BUFF_SIZE];

int main(void) {
    int i;
    char token;
    
    for(;;) {
        fgets(buffer, BUFF_SIZE, stdin);
        for(i = 0; (token = buffer[i]) != '\n' && buffer[i] != '\0'; i++) {
            if(token == ' ')
                continue;
            
            if(isalnum(token) || token == '.')
                putchar(token);
            
            else if(isop(token)) {
                putchar(' ');
                while(isop(top()) && 
                    // the operator is left associative and its precedence is less than or equal to top's
                    (assoc(token) == -1 && (prec(token, top()) == -1 || prec(token, top()) == 0)) || 
                    // the operator is right associative and its precedence is less than top's
                    (assoc(token) == 1 && prec(token, top()) == -1)) {
                        putchar(pop());
                        putchar(' ');
                }
                push(token);
            }
            
            else if(token == '(')
                push(token);
            
            else if(token == ')') {
                while(top() != '(') {
                    putchar(' ');
                    putchar(pop());
                }
                
                // left parenthesis
                pop();
            }
        }
        
        putchar(' ');
        // pop everything else
        while(top() != EOF) {
            putchar(pop());
            putchar(' ');
        }
        
        putchar('\n');
    }
    
    return 0;
}

int isop(char c) {
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
        return 1;
    else
        return 0;
}

int assoc(char c) {
    if(c == '^') // right associatives
        return 1;
    else
        return -1;
}

int prec(char a, char b) {
    int av, bv;
    
    if(a == b)
        return 0;
    
    switch(a) {
        case '+':
            av = 0;
            break;
        case '-':
            av = 0;
            break;
        case '*':
            av = 1;
            break;
        case '/':
            av = 1;
            break;
        case '^':
            av = 2;
            break;
    }
    
    switch(b) {
        case '+':
            bv = 0;
            break;
        case '-':
            bv = 0;
            break;
        case '*':
            bv = 1;
            break;
        case '/':
            bv = 1;
            break;
        case '^':
            bv = 2;
            break;
    }
    
    if(av == bv)
        return 0;
    else if(av < bv)
        return -1;
    else
        return 1;
    
}

int i;
char stack[BUFF_SIZE];

void push(char c) {
    stack[i++] = c;
}

char pop(void) {
    if(!i)
        return EOF;
    
    return stack[--i];
}

char top(void) {
    if(!i)
        return EOF;
    
    return stack[i - 1];
}

