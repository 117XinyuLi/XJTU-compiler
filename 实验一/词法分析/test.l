%{
	#include <stdio.h>
	#include <stdlib.h>
    #include <string.h>

    int bracket_num = 0;
    int total_len = 100;
    char* bracket = NULL; // 不允许函数调用
    int* bracket_line = NULL;
    FILE* output_file;

    int nline = 1;

    void Double_sqace(char*bracket, int*bracket_line, int total_len)
    {
        char*tmp = (char*)malloc(sizeof(char)*total_len*2);
        memcpy(tmp, bracket, total_len*sizeof(char));
        free(bracket);
        bracket = tmp;

        int*temp = (int*)malloc(sizeof(int)*total_len*2);
        memcpy(temp, bracket_line, total_len*sizeof(int));
        free(bracket);
        bracket_line = temp;
    }

    int Set_Brack(char* bracket,int*bracket_line, int bracket_num,int total_len,char*yytext,int nline)
    {
        if(total_len-2==bracket_num) //-2因为最后要放\0
        {
            Double_sqace(bracket, bracket_line, total_len);
        }
        strcpy(bracket+bracket_num,yytext);
        bracket_line[bracket_num]=nline;
        bracket_num++;
        return bracket_num;
    }

    int isMatching(char bracket[], int bracket_num, int bracket_line[]) 
    {
        int* stack = (int*)malloc(bracket_num*sizeof(int));  // 使用栈来存储左括号的索引
        int top = -1;  // 栈顶指针初始化为-1

        // 遍历所有括号
        for (int i = 0; i < bracket_num; i++) {
            // 如果是左括号，则将其索引压入栈中
            if (bracket[i] == '(' || bracket[i] == '[' || bracket[i] == '{') {
                top++;
                stack[top] = i;
            }
            // 如果是右括号
            else if (bracket[i] == ')' || bracket[i] == ']' || bracket[i] == '}') {
                // 如果栈为空或栈顶的左括号与当前右括号不匹配，则返回false
                if (top == -1 || (bracket[i] == ')' && bracket[stack[top]] != '(') ||
                    (bracket[i] == ']' && bracket[stack[top]] != '[') ||
                    (bracket[i] == '}' && bracket[stack[top]] != '{')) {
                    printf("Missing ");
                    switch (bracket[i]) {
                        case ')':
                            printf("left parenthesis '('.");
                            break;
                        case ']':
                            printf("left square bracket '['.");
                            break;
                        case '}':
                            printf("left curly brace '{'.");
                            break;
                    }
                    printf(" The other half is at line %d\n", bracket_line[i]);
                    free(stack);
                    return 0;
                }
                // 匹配成功，将栈顶元素出栈
                top--;
            }
        }

        // 如果栈为空，则所有括号都匹配，否则返回false
        if (top == -1) {
            printf("All brackets are matching\n");
            free(stack);
            return 1;
        } else {
            printf("Missing ");
            switch (bracket[stack[top]]) {
                case '(':
                    printf("right parenthesis ')'");
                    break;
                case '[':
                    printf("right square bracket ']'");
                    break;
                case '{':
                    printf("right curly brace '}'");
                    break;
            }
            printf(" The other half is at line %d\n", bracket_line[stack[top]]);
            free(stack);
            return 0;
        }
    }

%}
digit		[0-9]
letter		[a-zA-Z]
Ident	({letter}|_)({letter}|{digit}|_)*
IntConst	{digit}+
FloatConst	{digit}+(\.{digit}+)?([eE][-+]?{digit}+)?
FormatString \"[^"]*\"
MAIN [m][a][i][n]
CONST [c][o][n][s][t]
INT [i][n][t]
BREAK [b][r][e][a][k]
CONTINUE [c][o][n][t][i][n][u][e]
IF [i][f]
ELSE [e][l][s][e]
NOT !
AND &&
OR \|\|
WHILE [w][h][i][l][e]
GETINT [g][e][t][i][n][t]
PRINTF [p][r][i][n][t][f]
RETURN [r][e][t][u][r][n]
SWITCH [s][w][i][t][c][h]
CASE [c][a][s][e]
PLUS \+
MINU -
VOID [v][o][i][d]
MULT \*
DIV [/]
MOD %
LSS <
LEQ <=
GRE >
GEQ >=
EQL ==
NEQ !=
ASSIGN =
COMMA ,
delim		[ \t\r]
whitespace	{delim}+
other		.
%%
{MAIN}  {printf("MAINTK %s\n",yytext);fprintf(output_file, "MAINTK %s\n",yytext);}
{CONST}  {printf("CONSTTK %s\n",yytext);fprintf(output_file,"CONSTTK %s\n",yytext);}
{INT}  {printf("INTTK %s\n",yytext);fprintf(output_file, "INTTK %s\n",yytext);}
{BREAK}  {printf("BREAKTK %s\n",yytext);fprintf(output_file, "BREAKTK %s\n",yytext);}
{CONTINUE}  {printf("CONTINUETK %s\n",yytext);fprintf(output_file,"CONTINUETK %s\n",yytext);}
{IF}  {printf("IFTK %s\n",yytext);fprintf(output_file,"IFTK %s\n",yytext);}
{ELSE}  {printf("ELSE %s\n",yytext);fprintf(output_file,"ELSE %s\n",yytext);}
{WHILE}  {printf("WHILETK %s\n",yytext);fprintf(output_file,"WHILETK %s\n",yytext);}
{GETINT}  {printf("GETINTTK %s\n",yytext);fprintf(output_file,"GETINTTK %s\n",yytext);}
{PRINTF}  {printf("PRINTFTK %s\n",yytext);fprintf(output_file,"PRINTFTK %s\n",yytext);}
{RETURN}  {printf("RETURNTK %s\n",yytext);fprintf(output_file,"RETURNTK %s\n",yytext);}
{VOID}  {printf("VOIDTK %s\n",yytext);fprintf(output_file,"VOIDTK %s\n",yytext);}
{SWITCH}  {printf("SWITCH %s\n",yytext);fprintf(output_file,"SWITCH %s\n",yytext);}
{CASE}  {printf("CASE %s\n",yytext);fprintf(output_file,"CASE %s\n",yytext);}
{Ident}    {printf("IDENFR %s\n",yytext);fprintf(output_file,"IDENFR %s\n",yytext);}
{IntConst}	{printf("INTCON %s\n",yytext);fprintf(output_file,"INTCON %s\n",yytext);}
{FloatConst} {printf("FLOCON %s\n",yytext);fprintf(output_file,"FLOCON %s\n",yytext);}
{AND} {printf("AND %s\n",yytext);fprintf(output_file,"AND %s\n",yytext);}
{OR} {printf("OR %s\n",yytext);fprintf(output_file,"OR %s\n",yytext);}
{LEQ} {printf("LEQ %s\n",yytext);fprintf(output_file,"LEQ %s\n",yytext);}
{GEQ} {printf("GEQ %s\n",yytext);fprintf(output_file,"GEQ %s\n",yytext);}
{EQL} {printf("EQL %s\n",yytext);fprintf(output_file,"EQL %s\n",yytext);}
{NEQ} {printf("NEQ %s\n",yytext);fprintf(output_file,"NEQ %s\n",yytext);}
{NOT} {printf("NOT %s\n",yytext);fprintf(output_file,"NOT %s\n",yytext);}
{PLUS} {printf("PLUS %s\n",yytext);fprintf(output_file,"PLUS %s\n",yytext);}
{MINU} {printf("MINU %s\n",yytext);fprintf(output_file,"MINU %s\n",yytext);}
{MULT} {printf("MULT %s\n",yytext);fprintf(output_file,"MULT %s\n",yytext);}
{DIV} {printf("DIV %s\n",yytext);fprintf(output_file,"DIV %s\n",yytext);}
{MOD} {printf("MOD %s\n",yytext);fprintf(output_file,"MOD %s\n",yytext);}
{LSS} {printf("LSS %s\n",yytext);fprintf(output_file,"LSS %s\n",yytext);}
{GRE} {printf("GRE %s\n",yytext);fprintf(output_file,"GRE %s\n",yytext);}
{ASSIGN} {printf("ASSIGN %s\n",yytext);fprintf(output_file,"ASSIGN %s\n",yytext);}
{COMMA} {printf("COMMA %s\n",yytext);fprintf(output_file,"COMMA %s\n",yytext);}
\n {nline++;}
{whitespace}    { /*不处理*/ }
{FormatString} {printf("STRCON %s\n",yytext);fprintf(output_file,"STRCON %s\n",yytext);}
"(" {
    printf("LPARENT %s\n",yytext);fprintf(output_file,"LPARENT %s\n",yytext);
    bracket_num = Set_Brack(bracket, bracket_line, bracket_num, total_len, yytext, nline);
}
")" {
    printf("RPARENT %s\n",yytext);fprintf(output_file,"RPARENT %s\n",yytext);
    bracket_num = Set_Brack(bracket, bracket_line, bracket_num, total_len, yytext, nline);
}
"[" {
    printf("LBRACK %s\n",yytext);fprintf(output_file,"LBRACK %s\n",yytext);
    bracket_num = Set_Brack(bracket, bracket_line, bracket_num, total_len, yytext, nline);
}
"]" {
    printf("RBRACK %s\n",yytext);fprintf(output_file,"RBRACK %s\n",yytext);
    bracket_num = Set_Brack(bracket, bracket_line, bracket_num, total_len, yytext, nline);
}
"{" {
    printf("LBRACE %s\n",yytext);fprintf(output_file,"LBRACE %s\n",yytext);
    bracket_num = Set_Brack(bracket, bracket_line, bracket_num, total_len, yytext, nline);
}
"}" {
    printf("RBRACE %s\n",yytext);fprintf(output_file,"RBRACE %s\n",yytext);
    bracket_num = Set_Brack(bracket, bracket_line, bracket_num, total_len, yytext, nline);
}
":" {printf("COLON %s\n",yytext);fprintf(output_file,"COLON %s\n",yytext);}
";" {printf("SEMICN %s\n",yytext);fprintf(output_file,"SEMICN %s\n",yytext);}
"#".* {printf("HEAD %s\n",yytext);fprintf(output_file,"HEAD %s\n",yytext);}
"//"(.)*\n { nline++; }
"/*"([^*]|\*[^/])*"\*/" { 
    // 统计其中的\n的个数
    for(int i=0; i<strlen(yytext);i++){
        if (yytext[i]=='\n')
        {
            nline++;
        }
    }
}
{other}		{printf("illegal character, line %d:\'%s\'\n",nline,yytext);fprintf(output_file,"illegal character, line %d:\'%s\'\n",nline,yytext);}
%%
int main(){
    bracket = (char*)malloc(sizeof(char)*total_len);
    bracket_line = (int*)malloc(sizeof(int)*total_len);
	yyin=fopen("test.c","r");
    output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }
	yylex();


    isMatching(bracket, bracket_num, bracket_line);

    printf("总行数为：%d",nline);


    free(bracket);
    free(bracket_line);

	return 0;
}
 int yywrap(){
 	return 1;
 }