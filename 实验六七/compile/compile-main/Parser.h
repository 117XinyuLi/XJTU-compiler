#pragma once
/*经过分析
FIRST集：
FIRST(PROG) = { {,}
FIRST(DECLS) = { int, bool, ε }
FIRST(DECLS') = { int, bool, ε }
FIRST(DECL) = { int, bool }
FIRST(NAMES) = { id }
FIRST(NAMES') = { ,, ε }
FIRST(NAME) = { id }
FIRST(STMTS) = { id, if, while, {, read, write, ε }
FIRST(STMTS') = { id, if, while, {, read, write, ε }
FIRST(STMT) = { id, if, while, {, read, write }
FIRST(ELSESTMT) = { else, ε }
FIRST(EXPR) = { id, number, (, -, }
FIRST(ADD') = { +, -, ε }
FIRST(ADD) = { +, - }
FIRST(TERM) = { id, number, (, -, }
FIRST(MUL') = { *, /, ε }
FIRST(MUL) = { *, / }
FIRST(NEGA) = { id, number, (, -, }
FIRST(FACTOR) = { id, number, ( }
FIRST(BOOL) = { id, number, (, !, -, }
FIRST(BOOL') = { ||, ε }
FIRST(JOIN) = { id, number, (, !, -, }
FIRST(JOIN') = { &&, ε }
FIRST(NOT) = { id, number, (, !, -, }
FIRST(REL) = { id, number, (, -, }
FIRST(ROP) = { >, >=, <, <=, ==, != }


FOLLOW集：
FOLLOW(PROG) = {$}
FOLLOW(DECLS) = FOLLOW(DECLS') = {id, if, while, {, read, write, $}
FOLLOW(DECL) = {int, bool, id, if, while, {, read, write, $}
FOLLOW(NAMES) = FOLLOW(NAMES') = {;}
FOLLOW(NAME) = {,, ;}
FOLLOW(STMTS) = FOLLOW(STMTS') = {id, if, while, {, read, write, }, $}
FOLLOW(STMT) = {id, if, while, {, read, write, }, $}
FOLLOW(ELSESTMT) = {id, if, while, {, read, write, }, $}
FOLLOW(EXPR) = {), ;, >, >=, <, <=, ==, !=}
FOLLOW(ADD') = FOLLOW(ADD) = {), ;, >, >=, <, <=, ==, !=}
FOLLOW(TERM) = {+, -, ), ;, >, >=, <, <=, ==, !=}
FOLLOW(MUL') = FOLLOW(MUL) = {+, -, ), ;, >, >=, <, <=, ==, !=}
FOLLOW(NEGA) = {*, /, +, -, ), ;, >, >=, <, <=, ==, !=}
FOLLOW(FACTOR) = {*, /, +, -, ), ;, >, >=, <, <=, ==, !=}
FOLLOW(BOOL) = FOLLOW(BOOL') = {;, }
FOLLOW(JOIN) = FOLLOW(JOIN') = {||, ;, }
FOLLOW(NOT) = {&&, ||, ;, }
FOLLOW(REL) = {&&, ||, ;, }
FOLLOW(ROP) = {id, number, (, -, }

SELECT集:
SELECT(PROG → { DECLS STMTS }) = { { }
SELECT(DECLS → DECL DECLS') = { int, bool }
SELECT(DECLS → empty) = { id, if, while, {, read, write, $ }
SELECT(DECLS' → DECL DECLS') = { int, bool }
SELECT(DECLS' → empty) = { id, if, while, {, read, write, $ }
SELECT(DECL → int NAMES ;) = { int }
SELECT(DECL → bool NAMES ;) = { bool }
SELECT(NAMES → NAME NAMES') = { id }
SELECT(NAMES' → , NAME NAMES') = { , }
SELECT(NAMES' → empty) = { ; }
SELECT(NAME → id) = { id }
SELECT(STMTS → STMT STMTS') = { id, if, while, {, read, write }
SELECT(STMTS → empty) = { }, $ }
SELECT(STMTS' → STMT STMTS') = { id, if, while, {, read, write }
SELECT(STMTS' → empty) = { }, $ }

SELECT(STMT → id STMT')={id}
SELECT(STMT →  = EXPR;)={=}
SELECT(STMT → := BOOL;)={:=}

SELECT(STMT → if id then STMT ELSESTMT) = { if }
SELECT(ELSESTMT → else STMT) = { else }
SELECT(ELSESTMT → empty) = { id, if, while, {, read, write, }, $ }

SELECT(STMT → while id do STMT) = { while }
SELECT(STMT → { STMTS }) = { { }
SELECT(STMT → read id ;) = { read }
SELECT(STMT → write id ;) = { write }

SELECT(EXPR → TERM ADD') = { id, number, (, - }
SELECT(ADD' → ADD TERM ADD') = { +, - }
SELECT(ADD' → empty) = { ), ;, >, >=, <, <=, ==, != }
SELECT(ADD → +) = { + }
SELECT(ADD → -) = { - }
SELECT(TERM → NEGA MUL') = { id, number, (, - }
SELECT(MUL' → MUL NEGA MUL') = { *, / }
SELECT(MUL' → empty) = { +, -, ), ;, >, >=, <, <=, ==, != }
SELECT(MUL → *) = { * }
SELECT(MUL → /) = { / }
SELECT(NEGA → FACTOR) = { id, number, ( }
SELECT(NEGA → - FACTOR) = { - }
SELECT(FACTOR → ( EXPR )) = { ( }
SELECT(FACTOR → id) = { id }
SELECT(FACTOR → number) = { number }
SELECT(BOOL → JOIN BOOL') = { id, number, (, !, - }
SELECT(BOOL' → || JOIN BOOL') = { || }
SELECT(BOOL' → empty) = { ;, } }
SELECT(JOIN → NOT JOIN') = { id, number, (, !, - }
SELECT(JOIN' → && NOT JOIN') = { && }
SELECT(JOIN' → empty) = { ;, ||, } }
SELECT(NOT → REL) = { id, number, (, - }
SELECT(NOT → ! REL) = { ! }
SELECT(REL → EXPR ROP EXPR) = { id, number, (, - }
SELECT(ROP → >) = { > }
SELECT(ROP → >=) = { >= }
SELECT(ROP → <) = { < }
SELECT(ROP → <=) = { <= }
SELECT(ROP → ==) = { == }
SELECT(ROP → !=) = { != }
*/
/*以下是我的代码*/
#include "lexical_analyzer.h"
#include <iostream>
#include <fstream>
#include <string>
#include<sstream>
#include <vector>
#include<stack>
using namespace std;
struct TreeNode {
    pair<string, string> val;
    TreeNode* N1;
    TreeNode* N2;
    TreeNode* N3;
    TreeNode* N4;
    TreeNode* N5;
    TreeNode* N6;
    TreeNode(pair<string, string> x) : val(x), N1(nullptr), N2(nullptr), N3(nullptr), N4(nullptr), N5(nullptr), N6(nullptr) {}
};
//语法树模块及相关操作
class tree {
public:
    tree();
    ~tree();
    TreeNode* getroot() {
        return root;
    }
    bool add_node(TreeNode* current, pair<string, string> data, TreeNode*& NowInsert);//在current下面插入一个子节点。从左向右的顺序，如果满了，返回false.
    string serialize(TreeNode* root);               // 树转化为字符串
    TreeNode* NewNode(pair<string, string>);       //创建一个新的树节点
    TreeNode* deserialize(string data);  // 字符串转化为树
    bool isEndNode(TreeNode* cur);
private:
    TreeNode* root;
    TreeNode* deserializeHelper(const vector<pair<string, string>>& vals, int& pos);//字符串转化为树的辅助函数
};
bool tree::isEndNode(TreeNode* cur) {
    if (cur->N1 == NULL)return true;
    else
        return false;
}
tree::tree() {
    root = nullptr;
}
tree::~tree() {
    root = nullptr;
}

bool tree::add_node(TreeNode* current, pair<string, string> data, TreeNode*& NowInsert) {
    if (root == nullptr) {
        root = NewNode(data);
        return true;
    }
    if (current->N1 == nullptr) {
        current->N1 = NewNode(data);
        NowInsert = current->N1;
    }
    else if (current->N2 == nullptr) {
        current->N2 = NewNode(data);
        NowInsert = current->N2;
    }
    else if (current->N3 == nullptr) {
        current->N3 = NewNode(data);
        NowInsert = current->N3;
    }
    else if (current->N4 == nullptr) {
        current->N4 = NewNode(data);
        NowInsert = current->N4;
    }
    else if (current->N5 == nullptr) {
        current->N5 = NewNode(data);
        NowInsert = current->N5;
    }
    else if (current->N6 == nullptr) {
        current->N6 = NewNode(data);
        NowInsert = current->N6;
    }
    else
        return false;
    return true;
}
string tree::serialize(TreeNode* root) {
    if (root == nullptr) return "";
    stack<TreeNode*> s;
    s.push(root);
    string res = "";
    while (!s.empty()) {
        TreeNode* cur = s.top();
        s.pop();
        if (cur != nullptr) {
            res += (cur->val.first) + " ";
            res += (cur->val.second) + " ";
            s.push(cur->N6);
            s.push(cur->N5);
            s.push(cur->N4);
            s.push(cur->N3);
            s.push(cur->N2);
            s.push(cur->N1);
        }
        else {
            res += "#";
            res += " ";
        }
    }
    return res;
}
TreeNode* tree::deserialize(string data) {
    stringstream ss(data);
    pair<string, string> current_pair;
    vector<pair<string, string>> val;
    string current;
    while (ss >> current) {
        current_pair.first = current;
        if (current != "#")
            ss >> current;
        current_pair.second = current;
        val.push_back(current_pair);
    }
    int pos = 0;
    return deserializeHelper(val, pos);
}
TreeNode* tree::deserializeHelper(const vector<pair<string, string>>& vals, int& pos) {
    if (vals[pos].first == "#") {
        pos++;
        return nullptr;
    }
    TreeNode* node = new TreeNode(vals[pos++]);
    node->N1 = deserializeHelper(vals, pos);
    node->N2 = deserializeHelper(vals, pos);
    node->N3 = deserializeHelper(vals, pos);
    node->N4 = deserializeHelper(vals, pos);
    node->N5 = deserializeHelper(vals, pos);
    node->N6 = deserializeHelper(vals, pos);
    return node;
}
TreeNode* tree::NewNode(pair<string, string> data) {
    TreeNode* node = new TreeNode(data);
    return node;
}
// 语法分析模块
class Parser {
public:
    Parser(const string& InputFile, const string& middleFile, const string& OutputFile);
    ~Parser();
    void print();                                                                           //输出预测分析表
    TreeNode* deserialize(const string& input);                                             //序列化结果文件的反序列化，反序列化是否成功
    void serialize(const string& output, TreeNode* root);                                   //序列化结果文件的反序列化结果文件的序列化，通过与序列化结果文件对比来验证序列化和反序列化函数
    bool do_parser(const vector<pair<string, string>>& input, TreeNode*& result);          //语法分析函数
    bool do_and_write();                                                                  //词法分析、语法分析及所有结果输出函数
private:
    bool isComment(const std::string& line, size_t& pos);                                 //当前行是否存在注释
    bool is_end_character(const string& a);                                               //当前字符串是否是终结符
    string in;                                                                          //输入文件名
    string out;                                                                           //语法分析结果输出文件名
    string middle;                                                                        //词法分析结果输出文件名
    int findNthStringLine(const std::string& filename, int n, string& result);             //找到当前文件中第i个字符串的函数。方便语法分析错误处理
    vector<vector<pair<vector<string>, vector<string>>>>LL1_t;                              //预测分析表。最外层数组表示不同的非终结符对应的每个状态。第二层数组表示每个状态的多个语句。第三层数组，表示每个状态的产生式右部非终结符集合和相应select集中终结符结合    
    void init_LL1_able();
    bool do_parser_input_change(vector<pair<string, string>>& result, const vector<pair<string, string>>& input);      //处理词法分析结果，使其更适合进行语法分析                                                
    int find_status(string a);                                   //在预测分析表中找到相应的非终结符
    string true_string(const string& a);                         //当前字符串如果分隔符算一个字符串，则当前字符串的所有分隔符用空格隔开                
    int judge_status(string a, int status);                         //在预测分析表中，找到相应的产生式
};
TreeNode* Parser::deserialize(const string& input) {
    ifstream in(input);
    TreeNode* result;
    string current;
    string middle;
    middle.clear();
    while (in >> current) {
        middle += current + " ";
    }
    middle += '\0';
    tree t;
    result = t.deserialize(middle);
    return result;
}
void Parser::serialize(const string& output, TreeNode* root) {
    ofstream out(output);
    tree t;
    string middle;
    middle = t.serialize(root);
    out << middle;
}
bool Parser::isComment(const std::string& line, size_t& pos) {
    pos = line.find("//");
    if (pos != std::string::npos) {
        return true;
    }
    return false;
}
string Parser::true_string(const string& a) {
    string result;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] == '(' || a[i] == ')' || a[i] == '{' || a[i] == '}' || a[i] == ',' || a[i] == ';') {
            result += " ";
            result += a[i];
            result += " ";
        }
        else {
            result += a[i];
        }

    }
    return result;
}
int Parser::findNthStringLine(const std::string& filename, int n, string& result) {
    using namespace std;
    ifstream file(filename);  // 打开文件
    if (!file) {
        cerr << "无法打开文件: " << filename << std::endl;  // 如果无法打开文件，输出错误信息
        return -1;
    }
    int lineNumber = 0;
    int stringCount = 0;
    string line;
    bool inMultilineComment = false;  // 标记是否处于多行注释中
    while (std::getline(file, line)) {  // 逐行读取文件
        lineNumber++;  // 行号递增
        size_t commentPos;
        if (isComment(line, commentPos)) {  // 判断当前行是否为单行注释
            line = line.substr(0, commentPos);  // 删除注释部分
        }
        size_t multiCommentStart = line.find("/*");  // 查找多行注释起始符
        size_t multiCommentEnd = line.find("*/");  // 查找多行注释结束符
        if (inMultilineComment) {  // 如果处于多行注释中
            if (multiCommentEnd != std::string::npos) {  // 如果找到多行注释结束符
                inMultilineComment = false;  // 设置多行注释标记为 false
                line = line.substr(multiCommentEnd + 2);  // 删除注释部分
            }
            else {
                continue;  // 继续读取下一行
            }
        }
        else {
            if (multiCommentStart != std::string::npos) {  // 如果找到多行注释起始符
                inMultilineComment = true;  // 设置多行注释标记为 true
                line = line.substr(0, multiCommentStart);  // 删除注释部分
            }
        }
        if (line.empty()) {  // 如果当前行为空，跳过处理
            continue;
        }
        line = true_string(line);
        std::istringstream iss(line);  // 创建字符串流以处理当前行
        std::string word;
        bool is_first = true;
        while (iss >> word) {  // 读取当前行的每个字符串
            stringCount++;  // 字符串计数递增
            if (stringCount == n - 1)result = word;
            if (stringCount == n && is_first) {  // 如果达到目标字符串数量
                return lineNumber - 1;  // 返回行号
            }
            if (stringCount == n && !is_first) {  // 如果达到目标字符串数量
                return lineNumber;  // 返回行号
            }
            if (is_first)
                is_first = false;
        }
    }

    std::cerr << "文件中字符串数量少于 " << n << std::endl;  // 如果文件中的字符串数量少于 n，输出错误信息
    return -1;
}
bool Parser::is_end_character(const string& a) {
    if (a == "int" || a == "bool" || a == "if" || a == "then" || a == "while" || a == "do" || a == "read" || a == "write" || a == "else" || a == "+" || a == "-" || a == "*" || a == "/" || a == "||" || a == "&&" || a == "!" || a == ">=" || a == ">" || a == "<=" || a == "<" || a == ":=" || a == "==" || a == "=" || a == "(" || a == ")" || a == "{" || a == "}" || a == ";" || a == "," || a == "number" || a == "id")
        return true;
    return false;
}
std::string readFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}
Parser::Parser(const string& InputFile, const string& middleFile, const string& OutputFile) {
    in = InputFile;
    out = OutputFile;
    middle = middleFile;
    pair<vector<string>, vector<string>> current;
    vector<pair<vector<string>, vector<string>>> current1;
    //PROG
    current.first = { "{","DECLS","STMTS","}" };
    current.second = { "{" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //DECLS
    current.first = { "DECL","DECLS'" };
    current.second = { "int", "bool" };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { "id", "if", "while", "{", "read", "write", "$" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //DELCS'
    current.first = { "DECL","DECLS'" };
    current.second = { "int", "bool" };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { "id", "if", "while", "{", "read", "write", "$" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //DECL
    current.first = { "int","NAMES",";" };
    current.second = { "int" };
    current1.push_back(current);
    current.first = { "bool","NAMES",";" };
    current.second = { "bool" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //NAMES
    current.first = { "NAME","NAMES'" };
    current.second = { "id" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //NAMES'

    current.first = { ",","NAME","NAMES'" };
    current.second = { "," };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { ";" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //NAME
    current.first = { "id" };
    current.second = { "id" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //STMTS
    current.first = { "STMT","STMTS'" };
    current.second = { "id","if","while", "{", "read", "write" };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { "}", "$" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //STMTS'
    current.first = { "STMT","STMTS'" };
    current.second = { "id","if","while", "{", "read", "write" };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { "}", "$" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //STMT
    current.first = { "id","STMT'" };
    current.second = { "id" };
    current1.push_back(current);
    current.first = { "if","id","then","STMT","ELSESTMT" };
    current.second = { "if" };
    current1.push_back(current);
    current.first = { "while","id","do","STMT" };
    current.second = { "while" };
    current1.push_back(current);
    current.first = { "{","STMTS","}" };
    current.second = { "{" };
    current1.push_back(current);
    current.first = { "read","id",";" };
    current.second = { "read" };
    current1.push_back(current);
    current.first = { "write","id",";" };
    current.second = { "write" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //STMT'
    current.first = { "=","EXPR",";" };
    current.second = { "=" };
    current1.push_back(current);
    current.first = { ":=","BOOL",";" };
    current.second = { ":=" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //ELSESTMT
    current.first = { "else","STMT" };
    current.second = { "else" };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { "id","if","while","{","read","write","}","$" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //EXPR
    current.first = { "TERM","ADD'" };
    current.second = { "id","number","(","-" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //ADD'
    current.first = { "ADD","TERM","ADD'" };
    current.second = { "+","-" };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { ")",";",">",">=","<","<=","==","!=" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //ADD
    current.first = { "+" };
    current.second = { "+" };
    current1.push_back(current);
    current.first = { "-" };
    current.second = { "-" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //TERM
    current.first = { "NEGA","MUL'" };
    current.second = { "id","number","(","-" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //MUL'
    current.first = { "MUL","NEGA","MUL'" };
    current.second = { "*","/" };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { "+","-",")",";",">",">=","<","<=" ,"==" ,"!=" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //MUL
    current.first = { "*" };
    current.second = { "*" };
    current1.push_back(current);
    current.first = { "/" };
    current.second = { "/" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //NEGA
    current.first = { "FACTOR" };
    current.second = { "id","number", "(" };
    current1.push_back(current);
    current.first = { "-","FACTOR" };
    current.second = { "-" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //FACTOR
    current.first = { "(","EXPR",")" };
    current.second = { "(" };
    current1.push_back(current);
    current.first = { "id" };
    current.second = { "id" };
    current1.push_back(current);
    current.first = { "number" };
    current.second = { "number" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //BOOL
    current.first = { "JOIN","BOOL'" };
    current.second = { "id","number", "(", "!", "-" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //BOOL'
    current.first = { "||","JOIN","BOOL'" };
    current.second = { "||" };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { ";","}" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //JOIN
    current.first = { "NOT","JOIN'" };
    current.second = { "id","number", "(", "!", "-" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //JOIN'
    current.first = { "&&","NOT","JOIN'" };
    current.second = { "&&" };
    current1.push_back(current);
    current.first = { "empty" };
    current.second = { ";", "||" ,"}" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //NOT
    current.first = { "REL" };
    current.second = { "id","number","(","-" };
    current1.push_back(current);
    current.first = { "!","REL" };
    current.second = { "!" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //REL
    current.first = { "EXPR","ROP","EXPR" };
    current.second = { "id","number","(","-" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
    //ROP
    current.first = { ">" };
    current.second = { ">" };
    current1.push_back(current);
    current.first = { ">=" };
    current.second = { ">=" };
    current1.push_back(current);
    current.first = { "<" };
    current.second = { "<" };
    current1.push_back(current);
    current.first = { "<=" };
    current.second = { "<=" };
    current1.push_back(current);
    current.first = { "==" };
    current.second = { "==" };
    current1.push_back(current);
    current.first = { "!=" };
    current.second = { "!=" };
    current1.push_back(current);
    LL1_t.push_back(current1);
    current1.clear();
}
int status_find(const string a) {
    if (a == "PROG")return 1;
    else if (a == "DECLS")return 2;
    else if (a == "DECLS'")return 3;
    else if (a == "DECL")return 4;
    else if (a == "NAMES")return 5;
    else if (a == "NAMES'")return 6;
    else if (a == "NAME")return 7;
    else if (a == "STMTS")return 8;
    else if (a == "STMTS'")return 9;
    else if (a == "STMT")return 10;
    else if (a == "STMT'")return 11;
    else if (a == "ELSESTMT")return 12;
    else if (a == "EXPR")return 13;
    else if (a == "ADD'")return 14;
    else if (a == "ADD")return 15;
    else if (a == "TERM")return 16;
    else if (a == "MUL'")return 17;
    else if (a == "MUL")return 18;
    else if (a == "NEGA")return 19;
    else if (a == "FACTOR")return 20;
    else if (a == "BOOL")return 21;
    else if (a == "BOOL'")return 22;
    else if (a == "JOIN")return 23;
    else if (a == "JOIN'")return 24;
    else if (a == "NOT")return 25;
    else if (a == "REL")return 26;
    else if (a == "ROP")return 27;
    else return -1;

}
Parser::~Parser() {
}
void Parser::print() {
    for (int i = 0; i < LL1_t.size(); i++) {
        cout << i + 1 << endl;
        for (int j = 0; j < LL1_t[i].size(); j++) {
            for (int m = 0; m < LL1_t[i][j].first.size(); m++) {
                cout << LL1_t[i][j].first[m] << " ";
            }
            cout << "--------------------";
            for (int m = 0; m < LL1_t[i][j].second.size(); m++) {
                cout << LL1_t[i][j].second[m] << " ";
            }
            cout << endl;
        }
    }
}
int Parser::find_status(string a) {
    if (a == "PROG")return 1;
    else if (a == "DECLS")return 2;
    else if (a == "DECLS'")return 3;
    else if (a == "DECL")return 4;
    else if (a == "NAMES")return 5;
    else if (a == "NAMES'")return 6;
    else if (a == "NAME")return 7;
    else if (a == "STMTS")return 8;
    else if (a == "STMTS'")return 9;
    else if (a == "STMT")return 10;
    else if (a == "STMT'")return 11;
    else if (a == "ELSESTMT")return 12;
    else if (a == "EXPR")return 13;
    else if (a == "ADD'")return 14;
    else if (a == "ADD")return 15;
    else if (a == "TERM")return 16;
    else if (a == "MUL'")return 17;
    else if (a == "MUL")return 18;
    else if (a == "NEGA")return 19;
    else if (a == "FACTOR")return 20;
    else if (a == "BOOL")return 21;
    else if (a == "BOOL'")return 22;
    else if (a == "JOIN")return 23;
    else if (a == "JOIN'")return 24;
    else if (a == "NOT")return 25;
    else if (a == "REL")return 26;
    else if (a == "ROP")return 27;
    else return -1;
}
int Parser::judge_status(string a, int status) {
    for (int i = 0; i < LL1_t[status - 1].size(); i++) {
        for (int m = 0; m < LL1_t[status - 1][i].second.size(); m++) {
            if (LL1_t[status - 1][i].second[m] == a)return i;
        }
    }
    return -1;
}
bool Parser::do_parser_input_change(vector<pair<string, string>>& result, const vector<pair<string, string>>& input) {
    result.clear();
    for (int i = 0; i < input.size(); i++) {
        if (input[i].first == "BL")
            result.push_back({ "bool",input[i].second });
        else if (input[i].first == "KW")
            result.push_back({ input[i].second,input[i].second });
        else if (input[i].first == "ID")
            result.push_back({ "id",input[i].second });
        else if (input[i].first == "NU")
            result.push_back({ "number",input[i].second });
        else if (input[i].first == "OP")
            result.push_back({ input[i].second,input[i].second });
        else if (input[i].first == "SP")
            result.push_back({ input[i].second,input[i].second });
        else {
            return false;
        }
    }
    result.push_back({ "$","$" });
    return true;
}
bool Parser::do_parser(const vector<pair<string, string>>& input, TreeNode*& result) {
    tree t;
    stack<TreeNode*> stack_b;
    TreeNode* tree_node_current = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode* tree_node_current1 = (TreeNode*)malloc(sizeof(TreeNode));
    vector<pair<string, string>> true_input;
    if (!do_parser_input_change(true_input, input))return false;   //对词法分析器结果进行处理
    int judge_seperator[2] = { 0,0 };
    if (true_input[0].first != "{") {
        string error_string;
        int current_line = findNthStringLine(in, 1, error_string);
        cout << "第" << current_line << "行: " << endl;
        cout << "起始符号错误！代码应该以 \"{\" 为起始符号，你的起始符号为: " << true_input[0].first << endl;
        return false;
    }
    for (int i = 0; i < true_input.size(); i++) {
        if (true_input[i].first == "{")judge_seperator[0]++;
        else if (true_input[i].second == "}")judge_seperator[0]--;
        else if (true_input[i].second == "(")judge_seperator[1]++;
        else if (true_input[i].second == ")")judge_seperator[1]--;
    }
    if (judge_seperator[0] != 0 || judge_seperator[1] != 0) {
        if (judge_seperator[0] != 0)
            cout << "{}未得到匹配，请仔细检查你的语法" << endl;
        if (judge_seperator[1] != 0)
            cout << "()未得到匹配，请仔细检查你的语法" << endl;
        return false;
    }
    stack<TreeNode*> a;                   //语法分析程序的栈
    string current;                //当前语法分析栈的栈顶元素
    int flag = 0;                  //词法分析结果的指针
    t.add_node(t.getroot(), { "PROG","PROG" }, tree_node_current1);   //创建根节点
    TreeNode* end_note = new TreeNode({ "$","$" });
    a.push(end_note);
    a.push(t.getroot());                //根节点入栈
    int status;                          //当前字符串对应状态
    int number;                       //当前字符串对应状态下的第几语句
    while (flag < true_input.size()) {
        tree_node_current = a.top();        //取出栈顶元素
        a.pop();
        current = tree_node_current->val.first;
        if (current == "empty") {
            t.add_node(tree_node_current, tree_node_current->val, tree_node_current1);
            continue;
        }
        else if (current == true_input[flag].first) {                //如果栈顶元素是终结符且符合，栈顶元素出栈，分析向后移动一位
            t.add_node(tree_node_current, { true_input[flag].first ,true_input[flag].second }, tree_node_current1);
            flag++;
            continue;
        }
        else if (current == "$" && true_input[flag].first != "$") {
            string error_string;
            int current_line = findNthStringLine(in, flag + 1, error_string);
            cout << "第" << current_line << "行: " << endl;
            cout << "代码末尾多余非法字符：" << endl;
            while (flag < true_input.size() - 1)cout << true_input[flag++].second << " ";
            cout << endl;
            return false;
        }
        else if (is_end_character(current)) {                                        //终结符不匹配
            string error_string;
            int current_line = findNthStringLine(in, flag + 1, error_string);
            cout << "第" << current_line << "行: " << endl;
            cout << "字符串: " << error_string << endl << "是一个无法匹配的字符串，请检查你的语法" << endl;
            return false;
        }
        status = find_status(current);
        number = judge_status(true_input[flag].first, status);
        if (number == -1) {
            string error_string;
            int current_line = findNthStringLine(in, flag + 1, error_string);
            cout << "第" << current_line << "行: " << endl;
            cout << "字符串: " << error_string << endl << "是一个无法匹配的字符串，请检查你的语法" << endl;
            return false;
        }
        for (int i = 0; i < LL1_t[status - 1][number].first.size(); i++) {
            if (!t.add_node(tree_node_current, { LL1_t[status - 1][number].first[i],LL1_t[status - 1][number].first[i] }, tree_node_current1))return false;             //分析入栈
            stack_b.push(tree_node_current1);
        }
        while (!stack_b.empty()) {
            a.push(stack_b.top());
            stack_b.pop();
        }
    }
    result = t.getroot();
    return true;
}
// 将结果写入文件
void writeFile(const std::string& fileName, const std::string& content) {
    std::ofstream file(fileName);
    file << content;
}


bool Parser::do_and_write() {
    ifstream input_string(in);
    if (!input_string.is_open()) {
        ofstream out(in);
        string path = __FILE__;
        string now_path = path.substr(0, path.size() - 6);
        cout << "未设置源代码文件，请在目录: " << now_path << "下，设置test.txt文件为非空!!!,您的源代码不能为空!!!" << endl;
        input_string.close();
        return false;
    }
    char c;
    input_string >> c;
    if (input_string.eof()) {
        string path = __FILE__;
        string now_path = path.substr(0, path.size() - 6);
        cout << "未设置源代码文件，请在目录: " << now_path << "下，设置test.txt文件为非空!!!,您的源代码不能为空!!!" << endl;
        input_string.close();
        return false;
    }
    input_string.close();
    lexical_analyzer a(in, middle);
    if (a.analyze()) {
        tree t;
        TreeNode* result = nullptr;
        vector<pair<string, string>> in_put;
        in_put = a.result_to_token(middle);
        if (do_parser(in_put, result)) {
            ofstream out__put(out);
            string result_string = t.serialize(result);
            out__put << result_string;
            out__put.close();
            cout << "语法正确" << endl;
        }
        return true;
    }
    else
        return false;

}
/*
int、bool、if、then、while、do、read、write、else、+、-、*、/、||、&&、!、>=、>、<=、<、:=、==、=、(、)、{、}、;、,、number、id
   */
bool file_same(const string& file1, const string& file2) {
    ifstream input1(file1);
    ifstream input2(file2);
    string current1;
    string current2;
    bool result = true;
    while ((input1 >> current1) && (input2 >> current2))
        if (current1 != current2)
            result = false;
    return result;
}
