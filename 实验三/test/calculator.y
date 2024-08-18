%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

double regs[26];
int base;

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
    expr { printf("%f\n", $1); }
    | VARIABLE EQUALS expr { regs[$1 - 'a'] = $3; }
    ;

expr:
    expr PLUS term { $$ = $1 + $3; }
    | expr MINUS term { $$ = $1 - $3; }
    | term
    ;

term:
    term TIMES factor { $$ = $1 * $3; }
    | term DIVIDE factor { $$ = $1 / $3; }
    | factor
    ;

factor:
    primary MOD primary { $$ = (int)$1 % (int)$3; }
    | primary AND primary { $$ = (int)$1 & (int)$3; }
    | primary OR primary { $$ = (int)$1 | (int)$3; }
    | primary
    ;

primary:
    INTEGER { $$ = $1; }
    | FLOAT { $$ = $1; }
    | VARIABLE { $$ = regs[$1 - 'a']; }
    | LPAREN expr RPAREN { $$ = $2; }
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
        // Discard all trailing whitespace, including the newline
        do {
            c = getchar();
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
    }
    printf("Invalid character: %c\n", c);

    return c;
}