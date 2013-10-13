#include <stdio.h>
#include <ctype.h>

int isop(char c);
int prec(char a, char b);
void push(char c);
char pop(void);
char top(void);

#define BUFF_SIZE 420

int main(void) {
    int i, j, t;
    char token;
    char buffer[BUFF_SIZE];
    
    scanf("%d", &t);
    
    // get rid of newline
    while(getchar() != '\n');
    
    for(i = 0; i < t; i++) {
        fgets(buffer, BUFF_SIZE, stdin);
        for(j = 0; (token = buffer[j]) != '\n' && buffer[j] != '\0'; j++) {
            
            // if it's a letter, then echo
            if(isalpha(token))
                putchar(token);
            
            // if it's an operator
            else if(isop(token)) {
                while(isop(top()) && (prec(token, top()) == -1 || prec(token, top()) == 0)) {
                        putchar(pop());
                }
                push(token);
            }
            
            // if it's a left parentheses then push to stack
            else if(token == '(')
                push(token);
            
            // if it's a right parentheses, then pop operators
            // until the left parentheses... and pop it and discard it
            else if(token == ')') {
                while(top() != '(')
                    putchar(pop());
                
                // pop the left parenthesis
                pop();
            }
        }
        
        // pop everything in stack
        while(top() != EOF) {
            putchar(pop());
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

int prec(char a, char b) {
    int av, bv;
    
    if(a == b)
        return 0;
    
    switch(a) {
        case '+':
            av = 0;
            break;
        case '-':
            av = 1;
            break;
        case '*':
            av = 2;
            break;
        case '/':
            av = 3;
            break;
        case '^':
            av = 4;
            break;
    }
    
    switch(b) {
        case '+':
            bv = 0;
            break;
        case '-':
            bv = 1;
            break;
        case '*':
            bv = 2;
            break;
        case '/':
            bv = 3;
            break;
        case '^':
            bv = 4;
            break;
    }
    
    if(av < bv)
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

