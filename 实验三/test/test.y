%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

double regs[26];
int base;
char error_message[50];
char syntax_tree[1000][1000];
int syntax_tree_index = 0;

int yylex(void);
void yyerror(char *);
%}

%union {
    int intval;
    double floatval;
    char charval;
}

%token <intval> INTEGER
%token <floatval> FLOAT
%token <charval> VARIABLE
%token PLUS MINUS TIMES DIVIDE MOD AND OR EQUALS
%token LPAREN RPAREN
%token ENTER END

%type <floatval> expr term factor primary statement program

%%

program:
    program statement ENTER
    | statement ENTER
    ;

statement:
    expr { // printf("statement -> expr: %f\n", $1); 
           sprintf(syntax_tree[syntax_tree_index], "statement -> expr: %f\n", $1); syntax_tree_index++; 
           printf("Syntax Tree:\n");
           for (int i = syntax_tree_index - 1; i >= 0; i--) {
               printf("%s", syntax_tree[i]);
               syntax_tree[i][0] = '\0';
           }
           syntax_tree_index = 0;
        }
    | VARIABLE EQUALS expr { regs[$1 - 'a'] = $3; 
        // printf("statement -> VARIABLE = expr: regs[%c] = %f\n", $1, $3); 
        sprintf(syntax_tree[syntax_tree_index], "statement -> VARIABLE = expr: regs[%c] = %f\n", $1, $3); syntax_tree_index++; 
        printf("Syntax Tree:\n");
        for (int i = syntax_tree_index - 1; i >= 0; i--) {
            printf("%s", syntax_tree[i]);
            syntax_tree[i][0] = '\0';
        }
    }
    ;

expr:
    expr PLUS term { $$ = $1 + $3; 
        // printf("|-expr -> expr + term: %f + %f = %f\n", $1, $3, $$); 
        sprintf(syntax_tree[syntax_tree_index], "|-expr -> expr + term: %f + %f = %f\n", $1, $3, $$); syntax_tree_index++; }
    | expr MINUS term { $$ = $1 - $3; 
        // printf("|-expr -> expr - term: %f - %f = %f\n", $1, $3, $$); 
        sprintf(syntax_tree[syntax_tree_index], "|-expr -> expr - term: %f - %f = %f\n", $1, $3, $$); syntax_tree_index++; }
    | term {
        // printf("|-expr -> term: %f\n", $1); 
        sprintf(syntax_tree[syntax_tree_index], "|-expr -> term: %f\n", $1); syntax_tree_index++; }
    ;

term:
    term TIMES factor { $$ = $1 * $3; 
        // printf("|--term -> term * factor: %f * %f = %f\n", $1, $3, $$); 
        sprintf(syntax_tree[syntax_tree_index], "|--term -> term * factor: %f * %f = %f\n", $1, $3, $$); syntax_tree_index++; }
    | term DIVIDE factor { $$ = $1 / $3;
        // printf("|--term -> term / factor: %f / %f = %f\n", $1, $3, $$); 
        sprintf(syntax_tree[syntax_tree_index], "|--term -> term / factor: %f / %f = %f\n", $1, $3, $$); syntax_tree_index++; }
    | factor {
        // printf("|--term -> factor: %f\n", $1); 
        sprintf(syntax_tree[syntax_tree_index], "|--term -> factor: %f\n", $1); syntax_tree_index++; }
    ;

factor:
    primary MOD primary { $$ = (int)$1 % (int)$3;
        // printf("|---factor -> primary %% primary: %d %% %d = %d\n", (int)$1, (int)$3, (int)$$);
        sprintf(syntax_tree[syntax_tree_index], "|---factor -> primary %% primary: %d %% %d = %d\n", (int)$1, (int)$3, (int)$$); syntax_tree_index++; }
    | primary AND primary { $$ = (int)$1 & (int)$3; 
        // printf("|---factor -> primary & primary: %d && %d = %d\n", (int)$1, (int)$3, (int)$$); 
        sprintf(syntax_tree[syntax_tree_index], "|---factor -> primary & primary: %d && %d = %d\n", (int)$1, (int)$3, (int)$$); syntax_tree_index++; }
    | primary OR primary { $$ = (int)$1 | (int)$3;
        // printf("|---factor -> primary | primary: %d | %d = %d\n", (int)$1, (int)$3, (int)$$); 
        sprintf(syntax_tree[syntax_tree_index], "|---factor -> primary | primary: %d | %d = %d\n", (int)$1, (int)$3, (int)$$); syntax_tree_index++; }
    | primary {
        // printf("|---factor -> primary: %f\n", $1);
        sprintf(syntax_tree[syntax_tree_index], "|---factor -> primary: %f\n", $1); syntax_tree_index++; }
    ;

primary:
    INTEGER { $$ = $1; // printf("|----primary -> INTEGER: %f\n", $1); 
        sprintf(syntax_tree[syntax_tree_index], "|----primary -> INTEGER: %f\n", $1); syntax_tree_index++; }
    | FLOAT { $$ = $1; // printf("|----primary -> FLOAT: %f\n", $1); 
        sprintf(syntax_tree[syntax_tree_index], "|----primary -> FLOAT: %f\n", $1); syntax_tree_index++; }
    | VARIABLE { $$ = regs[$1 - 'a']; 
        // printf("|----primary -> VARIABLE: %c = %f\n", $1, $$); 
        sprintf(syntax_tree[syntax_tree_index], "|----primary -> VARIABLE: %c = %f\n", $1, $$); syntax_tree_index++; }
    | LPAREN expr RPAREN { $$ = $2; 
        // printf("|----primary -> (expr): %f\n", $2); 
        sprintf(syntax_tree[syntax_tree_index], "|----primary -> (expr): %f\n", $2); syntax_tree_index++; }
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main(void) {
    yyparse();
    return 0;
}

int yylex(void) {
    int c = getchar();
    while (c != EOF && c != '\n' && isspace(c))
        c = getchar();

    if (isdigit(c) || c == '.') {
        ungetc(c, stdin);
        if (scanf("%lf", &yylval.floatval) == 1) {
            // Discard all trailing whitespace, including the newline
            do {
                c = getchar();
            } while (c != EOF && c != '\n' && isspace(c));
            ungetc(c, stdin);  // put back the last non-whitespace character
            return FLOAT;
        } else {
            yyerror("Invalid number");
            return 0;
        }
    }

    if (isalpha(c)) {
        yylval.charval = c;
        // if uppercase, convert to lowercase
        if (c >= 'A' && c <= 'Z') {
            c += 32;
        }
        // Discard all trailing whitespace, including the newline
        do {
            c = getchar();
            // if find alpha, error
            if (isalpha(c)) {
                sprintf(error_message, "Invalid variable");
                yyerror(error_message);
                return 0;
            }
        } while (c != EOF && c != '\n' && isspace(c));
        ungetc(c, stdin);  // put back the last non-whitespace character
        return VARIABLE;
    }


    // printf("c: %c\n", c);
    switch (c) {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return TIMES;
        case '/': return DIVIDE;
        case '%': return MOD;
        case '&': return AND;
        case '|': return OR;
        case '=': return EQUALS;
        case '(': return LPAREN;
        case ')': return RPAREN;
        case '\n': return ENTER;
        case EOF: return END;
        default:
            sprintf(error_message, "Invalid character: %c", c);
            yyerror(error_message);
            return 0;

    }
    printf("Invalid character: %c\n", c);

    return c;
}