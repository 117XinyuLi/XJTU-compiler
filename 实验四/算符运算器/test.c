#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SIZE 100

int priority(char op){
    if(op == '+'|| op == '-')
        return 1;
    if(op == '*' || op == '/')
        return 2;
    return 0;
}

int applyOp(int a, int b, char op){
    switch(op){
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
}

int evaluate(char *tokens){
    int i;

    int values[SIZE];
    int top1 = -1;

    int ops[SIZE];
    int top2 = -1;

    for(i = 0; i < strlen(tokens); i++){

        if(tokens[i] == ' ')
            continue;

        else if(tokens[i] == '('){
            ops[++top2] = tokens[i];
        }

        else if(isdigit(tokens[i])){
            int val = 0;

            while(i < strlen(tokens) && isdigit(tokens[i])){
                val = (val*10) + (tokens[i]-'0');
                i++;
            }

            values[++top1] = val;
            i--;
        }

        else if(tokens[i] == ')'){
            while(top2 != -1 && ops[top2] != '('){
                int val2 = values[top1--];
                int val1 = values[top1--];
                char op = ops[top2--];

                values[++top1] = applyOp(val1, val2, op);
            }

            if(top2 != -1)
               top2--;
        }

        else{
            while(top2 != -1 && priority(ops[top2]) >= priority(tokens[i])){
                int val2 = values[top1--];
                int val1 = values[top1--];
                char op = ops[top2--];

                values[++top1] = applyOp(val1, val2, op);
            }

            ops[++top2] = tokens[i];
        }
    }

    while(top2 != -1){
        int val2 = values[top1--];
        int val1 = values[top1--];
        char op = ops[top2--];

        values[++top1] = applyOp(val1, val2, op);
    }

    return values[top1];
}

int main(){
    char exp[SIZE];
    printf("Enter an expression: ");
    gets(exp);
    printf("Result: %d", evaluate(exp));
    return 0;
}