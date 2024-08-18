#pragma once
#pragma once
#include<sstream>
#include <vector>
#include<iostream>
#include<fstream>
#include<string>
#include<unordered_map>
//本程序实现的是一个叫little_c的语言
/*a. 该语言的一个程序由且仅由一个块结构组成，该块中包含两部分：可选的声明语句和必须的执行语句。声明语句用于声明变量，执行语句用于表达计算过程，过程中使用这些变量来存储内部数据和计算结果。

b. 该语言支持两种数据类型：整型和布尔型。整型数据占4个字节，布尔型占1个字节。整型数据的常量是整常数，布尔型数据的常量是 true 和 false 。

c. 整型数据支持 +、-、*、/ 四种算术运算，运算结果为整型值。整型数据可与>、>=...等六个关系运算符结合，运算结果为布尔型值。

d. 布尔型数据支持 || 、&& 、！三种逻辑运算，运算结果为布尔常量。

e. 整型的变量和常量可以用算术运算符组合成算术表达式，其值为整常数；布尔型的变量和常量可以用逻辑运算符组成成布尔表达式，其值为布尔常量；算术表达式可以加上关系运算符构造出布尔表达式。

f. 整常数和算术表达式可以用于整型变量赋值，布尔常量和布尔表达式可以用于布尔型变量赋值；两者不兼容（文法中使用不同的赋值运算符以示区别）。

g. 各种运算符的优先级和结合规则参照C语言语法理解。

h. 控制流语句中的控制条件限定为1个布尔型变量，该量应该在引用之前定值。

i. 该语言不支持数组、结构体、指针等复杂数据类型。

j. 该语言不含有子程序结构，也不支持过程调用。

k. 该语言有简单的I/O功能：使用read指令能够从键盘读入1个整常数，使用write指令能向屏幕打印输出1个整常数。*/
/*文法定义：
PROG        →    {  DECLS  STMTS  }
DECLS       →    DECLS  DECL    |   empty
DECL         →    int  NAMES  ;  |  bool  NAMES  ;
NAMES     →    NAMES ,  NAME  |  NAME
NAME       →    id
STMTS    →    STMTS  STMT  |   STMT
STMT      →    id  =  EXPR ;    |   id := BOOL ;
STMT      →    if  id   then  STMT
STMT      →    if   id   then  STMT  else STMT
STMT      →    while   id  do  STMT
STMT      →    {  STMTS   STMT  }
STMT      →    read  id  ;
STMT      →    write  id  ;
EXPR    →    EXPR  ADD  TERM  |  TERM
ADD     →    + | -
TERM    →    TERM  MUL NEGA  |  NEGA
MUL     →    * | /
NEGA   →    FACTOR  |  - FACTOR
FACTOR→    (  EXPR ) |  id  |  number
BOOL    →    BOOL  ||  JOIN    |    JOIN
JOIN     →    JOIN   &&   NOT  |   NOT
NOT      →    REL   |  ! REL
REL       →    EXPR   ROP  EXPR
ROP      →     >  |  >=  |  <  |  <=  |  ==  |   !=*/

using namespace std;
enum Token {
    IDENTIFIER,            //标识符
    NUMBER,             //整常数
    BOOL,               //布尔常量
    KEYWORD,            //关键字
    OPERATOR,           //运算符
    SEPARATOR,          //分隔符
    INVALID_TOKEN,     //无效词
};
class lexical_analyzer {
public:
    lexical_analyzer(const string& input_file_name, const string& output_file_name);
    ~lexical_analyzer();
    std::vector<pair<string, string>> result_to_token(const string& ifn);
    bool analyze();
private:
    unordered_map<string, Token> keyword_map = {
        {"int",KEYWORD},
        {"bool",KEYWORD},
        {"if",KEYWORD},
        {"then",KEYWORD},
        {"while",KEYWORD},
        {"do",KEYWORD},
        {"read",KEYWORD},
        {"write",KEYWORD},
        {"else",KEYWORD},
    };
    unordered_map<string, Token> operator_map = {
        {"+", OPERATOR},
        {"-", OPERATOR},
        {"*", OPERATOR},
        {"/", OPERATOR},
        {"||", OPERATOR},
        {"&&", OPERATOR},
        {"!", OPERATOR},
        {">=", OPERATOR},
        {">", OPERATOR},
        {"<=", OPERATOR},
        {"<", OPERATOR},
        {"==", OPERATOR},
        {":=", OPERATOR},
        {"=", OPERATOR},
    };
    unordered_map<string, Token> separator_map = {
        {"(", SEPARATOR},
        {")", SEPARATOR},
        {"{", SEPARATOR},
        {"}", SEPARATOR},
        {",", SEPARATOR},
        {";", SEPARATOR},
    };
    unordered_map<string, Token> bool_map = {
        {"true",BOOL},
        {"false",BOOL},
    };
    Token get_keyword(const string& str);    //判断是否是关键字
    Token get_operator(const string& str);   //判断是否是运算符
    Token get_separator(const string& str);  //判断是否是分隔符
    Token get_number(const string& str);    //判断是否是整数常量
    Token get_bool(const string& str);      //判断是否是字符常量
    Token get_explanatorynote(const string& str); //判断是否是注释
    void write_token(Token token, const string& word);
    string in;
    string out;
    ifstream input_file;
    ofstream output_file;
    string find_operator(const string& str, int i);
    string find_bool(const string& str, int i);
    string find_separator(const string& str, int i);
    int findNthCharLine(const std::string& filename, int n, string& result);
};
lexical_analyzer::lexical_analyzer(const string& input_file_name, const string& output_file_name) {
    input_file.open(input_file_name);
    output_file.open(output_file_name);
    in = input_file_name;
    out = input_file_name;
}
lexical_analyzer::~lexical_analyzer() {
    input_file.close();
    output_file.close();
}
Token lexical_analyzer::get_keyword(const string& str) {
    auto it = keyword_map.find(str);
    if (it != keyword_map.end()) {
        return it->second;
    }
    return IDENTIFIER;
}
Token lexical_analyzer::get_bool(const string& str) {
    auto it = bool_map.find(str);
    if (it != bool_map.end()) {
        return it->second;
    }
    return IDENTIFIER;
}
Token lexical_analyzer::get_operator(const string& str) {
    auto it = operator_map.find(str);
    if (it != operator_map.end()) {
        return it->second;
    }
    return INVALID_TOKEN;
}
Token lexical_analyzer::get_separator(const string& str) {
    auto it = separator_map.find(str);
    if (it != separator_map.end()) {
        return it->second;
    }
    return INVALID_TOKEN;
}
Token lexical_analyzer::get_number(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return INVALID_TOKEN;
        }
    }
    return NUMBER;
}
void lexical_analyzer::write_token(Token token, const string& word) {
    switch (token) {
        /*enum Token {
    IDENTIFIER,            //标识符
    NUMBER,             //整常数
    BOOL,               //布尔常量
    KEYWORD,            //关键字
    OPERATOR,           //运算符
    SEPARATOR,          //分隔符
    INVALID_TOKEN,     //无效词
};*/
    case BOOL:
        output_file << "BL " << word << endl;
        break;
    case KEYWORD:
        output_file << "KW " << word << endl;
        break;
    case IDENTIFIER:
        output_file << "ID " << word << endl;
        break;
    case NUMBER:
        output_file << "NU " << word << endl;
        break;
    case OPERATOR:
        output_file << "OP " << word << endl;
        break;
    case SEPARATOR:
        output_file << "SP " << word << endl;
        break;
    default:
        break;
    }
}
void CreatNext_E(string p, int next[]) {
    int lenP = p.size();
    int i = 0, j = -1;
    next[0] = -1;
    while (i < lenP - 1) {
        if (j < 0 || p[i] == p[j]) {
            i++;
            j++;
            next[i] = p[i] == p[j] ? next[j] : j;  //此句代码进行了改进
        }
        else {
            j = next[j];
        }
    }
}
int KMP_find_string_in_string(string t, string p, int current) {
    int lenT = t.size();
    int lenP = p.size();
    int* next = new int[lenP];
    //CreatNext(p, next);
    CreatNext_E(p, next);
    int i, j;
    for (i = current; i <= lenT - lenP; ) {
        for (j = 0; j < lenP; ++j) {
            if (t[i + j] != p[j]) {
                i += j - next[j];
                break;
            }
        }
        if (j == lenP) {
            return i;
        }
    }
    return -1;
}
int BF_find_string_in_string(string t, string p, int current) {
    int lenT = t.size();
    int lenP = p.size();
    int i, j;
    for (i = current; i <= lenT - lenP; ++i) {
        for (j = 0; j < lenP; ++j) {
            if (t[i + j] != p[j]) {
                break;
            }
        }
        if (j == lenP) {
            return i;
        }
    }
    return -1;
}
string lexical_analyzer::find_operator(const string& str, int i) {
    int flag = 0;
    unordered_map<string, Token>::iterator iter;
    for (iter = operator_map.begin(); iter != operator_map.end(); iter++)
    {
        flag = 0;
        flag = KMP_find_string_in_string(str, iter->first, i);
        if (flag == i) { return iter->first; }
    }
    return "";
}
string lexical_analyzer::find_bool(const string& str, int i) {
    int flag = 0;
    unordered_map<string, Token>::iterator iter;
    for (iter = bool_map.begin(); iter != bool_map.end(); iter++)
    {
        flag = 0;
        flag = KMP_find_string_in_string(str, iter->first, i);
        if (flag == i) { return iter->first; }
    }
    return "";
}
string lexical_analyzer::find_separator(const string& str, int i) {
    int flag = 0;
    unordered_map<string, Token>::iterator iter;
    for (iter = separator_map.begin(); iter != separator_map.end(); iter++)
    {
        flag = 0;
        flag = KMP_find_string_in_string(str, iter->first, i);
        if (flag == i) { return iter->first; }
    }
    return "";
}
std::vector<pair<string, string>> lexical_analyzer::result_to_token(const string& ifn) {
    vector<pair<string, string>> result;
    ifstream input(ifn);
    pair<string, string> current;
    input >> current.first;
    input >> current.second;
    while (!input.eof()) {
        result.push_back(current);
        input >> current.first;
        input >> current.second;
    }
    return result;
}
int lexical_analyzer::findNthCharLine(const std::string& filename, int n, string& result) {
    using namespace std;
    ifstream file(filename);  // 打开文件
    if (!file) {
        cerr << "无法打开文件: " << filename << std::endl;  // 如果无法打开文件，输出错误信息
        return -1;
    }
    int lineNumber = 0;
    int charCount = 0;
    string line;
    bool inMultilineComment = false;  // 标记是否处于多行注释中
    while (std::getline(file, line)) {  // 逐行读取文件
        charCount += line.size() + 1;
        lineNumber++;  // 行号递增
        if (charCount > n)break;
    }
    if (charCount < n) {
        std::cerr << "文件中字符数量少于 " << n << std::endl;  // 如果文件中的字符串数量少于 n，输出错误信息
        return -1;
    }
    return lineNumber;
}

bool lexical_analyzer::analyze() {
    string input_string;
    string string_current;
    //读入一整行
    while (!input_file.eof()) {
        getline(input_file, string_current);
        input_string += string_current + "\n";
    }
    string word;
    Token token = INVALID_TOKEN;
    int i = 0;
    while (i < input_string.size()) {
        char c = input_string[i];
        //如果当前运算符是空字符，忽略并继续
        if (input_string[i] < 0 || input_string[i]>127) {
            string w;
            cout << "第" << findNthCharLine(in, i + 1, w) << "行，字符串\"" << word << "\"" << "后有一个非法字符串";
            return false;
        }
        else if (isspace(c)) {
            i++;
            continue;
        }
        word.clear();
        token = INVALID_TOKEN;
        //如果当前字符是'/'，判断是否是注释
        if (i < input_string.size() && input_string[i] == '/' && input_string[i + 1] == '/') {
            while (input_string[i] != '\n')i++;
            i++;
            continue;
        }
        if (i < input_string.size() && input_string[i] == '/' && input_string[i + 1] == '*') {
            while (i < input_string.size() && !(input_string[i] == '*' && input_string[i + 1] == '/'))
                i++;
            i += 2;
            continue;
        }
        if (find_bool(input_string, i) != "") {
            string c = find_bool(input_string, i);
            word += c;
            i += c.length();
            token = get_bool(c);
        }
        //如果当前字符是字母
        else if (isalpha(c)) {
            //如果为字母或数字就全部读入
            while (i < input_string.size() && isalnum(input_string[i])) {
                word += input_string[i];
                i++;
            }
            token = get_keyword(word);
            if (token == IDENTIFIER) {
                token = IDENTIFIER;
            }
        }
        else if (isdigit(c)) {
            while (i < input_string.size() && isdigit(input_string[i])) {
                word += input_string[i];
                i++;
                if (input_string[i] < 0 || input_string[i]>127) {
                    string a;
                    cout << "第" << findNthCharLine(in, i, a) << "行" << "的字串" << word << endl << "后有一个非法字符,请检查相关语法";
                    return false;
                }
            }
            token = get_number(word);
        }
        else if (find_operator(input_string, i) != "") {
            string c = find_operator(input_string, i);
            word += c;
            i += c.length();
            token = get_operator(c);
        }
        else if (find_separator(input_string, i) != "") {
            string c = find_separator(input_string, i);
            word += c;
            i += c.length();
            token = get_separator(c);
        }

        else {

            word += input_string[i];
            string a;
            cout << "第" << findNthCharLine(in, i, a) << "行" << "的字符: " << word << endl << "是一个非法字符,请检查相关语法";
            return false;
            // 无效字符
        }
        if (token != INVALID_TOKEN) {
            write_token(token, word);
        }
    }
    return true;

}
