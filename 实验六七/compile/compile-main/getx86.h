#pragma once
#include"Parser.h"
#include <iostream>
#include <string>
#include <fstream>
#include<vector>
#include<utility>
using namespace std;
ofstream outputFile;
ifstream inputFile;
void processPROG(TreeNode* node);
void processPROG(TreeNode* node);
void processDECLS(TreeNode* node);
void processDECLS_(TreeNode* node);
void processDECL(TreeNode* node);
vector<string> processNAMES(TreeNode* node);
vector<string> processNAMES_(TreeNode* node);
void processSTMTS(TreeNode* node);
void processSTMTS_(TreeNode* node);
void processSTMT(TreeNode* node);
void processSTMT_(TreeNode* root, string varName);
void processIfStmt(TreeNode* node);
void processELSESTMT(TreeNode* node);
void processWhileStmt(TreeNode* node);
void processReadStmt(TreeNode* node);
void processWriteStmt(TreeNode* node);
void processEXPR(TreeNode* node);
void processADD_(TreeNode* node);
void processTERM(TreeNode* node);
void processMUL_(TreeNode* node);
void processNEGA(TreeNode* node);
void processFACTOR(TreeNode* node);
void processBOOL(TreeNode* node);
void processBOOL_(TreeNode* node);
void processJOIN(TreeNode* node);
void processJOIN_(TreeNode* node);
void processNOT(TreeNode* node);
void processREL(TreeNode* node);
//定义符号表
vector<pair<pair<string,string>,int>>symboltable;
int label_id = 0;
int getlabel_id() {
    return label_id++;
}
void emit(string instruction) {
    outputFile << "\t" << instruction << endl;
}
void int_stack_init() {
    emit("#int栈初始化,t8为int栈指针");
    emit("la $t8, int_stack");
    emit("addiu $t8, $t8, 508");
}
//t1入栈
void int_stack_push() {
    emit("#t1入栈");
    emit("addiu $t8, $t8, -4");
    emit("sw $t1, 0($t8)");
}
//int出栈
void int_stack_pop_1() {
    emit("#出栈到t1");
    emit("lw $t1, 0($t8)");
    emit("addiu $t8, $t8, 4");
}
void int_stack_pop_1_2() {
    emit("#出栈到t1与t2");
    emit("lw $t1, 0($t8)");
    emit("addiu $t8, $t8, 4");
    emit("lw $t2, 0($t8)");
    emit("addiu $t8, $t8, 4");
}
void bool_stack_init() {
    emit("#bool栈初始化");
    emit("la $t9, bool_stack");
    emit("addiu $t9, $t9, 127");
}
void bool_stack_push_1() {
    emit("#t1入栈");
    emit("addiu $t9, $t9, -1");
    emit("sb $t1, 0($t9)");
}
void bool_stack_pop_1() {
    emit("#出栈到t1");
    emit("lb $t1, 0($t9)");
    emit("addiu $t9, $t9, 1");
}
void bool_stack_pop_1_2() {
    emit("#出栈到t1和t2");
    emit("lb $t1, 0($t9)");
    emit("addiu $t9, $t9, 1");
    emit("lb $t2, 0($t9)");
    emit("addiu $t9, $t9, 1");
}
void processPROG(TreeNode* node) {
    outputFile.open("final_result.txt");
    if (node->val.first == "PROG") {
        // 输出汇编代码的开头部分
        emit(".data");
        emit("input_stack:.space 1024");
        emit("input_buffer: .word 0");
        emit("int_stack:  .word 128");
        emit("bool_stack:  .byte 128 ");
        emit(".text\n");
        emit(".globl main\n");
        emit("main:");
        //初始化栈
        emit("#初始化栈");
        int_stack_init();
        bool_stack_init();
        // 处理DECLS和STMTS节点
        if (node->N2 != nullptr) processDECLS(node->N2);
        if (node->N3 != nullptr) processSTMTS(node->N3);
        // 输出汇编代码的结尾部分
        emit("li $v0, 10\n");
        emit("syscall\n");

    }
    outputFile.close();
    inputFile.open("final_result.txt");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();
    outputFile.open("final_result.txt");
    for (int i = 0; i < 5; i++) {
        emit(lines[i]);
    }
    for (int i = 0; i < symboltable.size(); i++) {
        if (symboltable[i].first.second == "int")
            emit("\t"+symboltable[i].first.first + ": .word 0");
        else
            emit("\t"+symboltable[i].first.first + ": .byte 0");
    }
    for (int i = 5; i < lines.size(); i++) {
        emit(lines[i]);
    }
    outputFile.close();
}   
    void processDECLS(TreeNode * node) {
        if (node == nullptr) {
            return; // 如果节点为空，则直接返回
        }
        // 如果不为空，处理第一个DECL节点
        processDECL(node->N1);
        // 处理后续的DECLS'节点
        processDECLS_(node->N2);
    }

    void processDECLS_(TreeNode * node) {
        if ((node == nullptr)||node->N1->val.first=="empty") {
            return; // 如果节点为空，则直接返回
        }
        // 如果不为空，处理DECL节点
        processDECL(node->N1);
        // 处理后续的DECLS'节点
        processDECLS_(node->N2);
    }
    void processDECL(TreeNode* node) {
        if (node == nullptr) {
            return; // 如果节点为空，则直接返回
        }
        // 从node的val获取类型
        string type = node->N1->val.first;
        // 处理NAMES节点，并返回变量名列表
        vector<string> varNames = processNAMES(node->N2);
        pair<string, string> cur;
        // 对于每个变量名，添加到符号表中
        for (const string& varName : varNames) {
            // 在符号表中添加变量名和类型
            cur.first = varName;
            cur.second = type;
            symboltable.push_back(make_pair(cur,-1));

        }
    }
    vector<string> processNAMES(TreeNode* node) {
        vector<string> varNames;
        if (node == nullptr) {
            return varNames;
        }

        // 处理NAME节点
        string varName = node->N1->N1->N1->val .second;
        varNames.push_back(varName);

        // 递归处理NAMES'节点
        vector<string> otherNames = processNAMES_(node->N2);
        varNames.insert(varNames.end(), otherNames.begin(), otherNames.end());
        return varNames;
    }

    vector<string> processNAMES_(TreeNode* node) {
        vector<string> varNames;
        if (node == nullptr|| node->N1->val.first == "empty") {
            return varNames;
        }
        // 处理NAME节点
        string varName = node->N2->N1->N1->val.second;
        varNames.push_back(varName);
        // 递归处理NAMES'节点
        vector<string> otherNames = processNAMES_(node->N3);
        varNames.insert(varNames.end(), otherNames.begin(), otherNames.end());
        return varNames;
    }
    void processSTMTS(TreeNode* node) {
        if (node == nullptr || node->N1->val.first == "empty") {
            return;
        }
        // 处理STMT节点
        processSTMT(node->N1);
        // 递归处理STMTS'节点
        processSTMTS_(node->N2);
    }

    void processSTMTS_(TreeNode* node) {
        if (node == nullptr || node->N1->val.first == "empty") {
            return;
        }
        // 处理STMT节点
        processSTMT(node->N1);
        // 递归处理STMTS'节点
        processSTMTS_(node->N2);
    }
    void processSTMT(TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        if (node->N1->val.first == "id") {  // 根据节点的子节点数量或特定标识来判断节点类型
            // id STMT'
            string varName = node->N1->N1->val.second; // 获取变量名
            processSTMT_(node->N2, varName);
        }
        else if (node->N1->val.first == "if") {
            // if id then STMT ELSESTMT
            processIfStmt(node);
        }
        else if (node->N1->val.first == "while") {
            // while id do STMT
            processWhileStmt(node);
        }
        else if (node->N1->val.first == "read") {
            // read id ;
            processReadStmt(node);
        }
        else if (node->N1->val.first == "write") {
            // write id ;
            processWriteStmt(node);
        }
        else if (node->N1->val.first == "{") {
            // { STMTS }
            processSTMTS(node->N2);
        }
    }
        void processSTMT_(TreeNode* root, string varName) {
            if (root->N1->val.first == "=") {
                processEXPR(root->N2);
                emit("# 将栈顶元素写回");
                int_stack_pop_1();
                emit("la $t2, "+ varName);   
                emit("sw $t1, 0($t2)");    
               }
            else if (root->N1->val.first == ":=") {
                processBOOL(root->N2);
                emit("# 将栈顶元素写回");
                bool_stack_pop_1();
                emit("la $t2, " + varName);
                emit("sb $t1, 0($t2)");
            }
            return;
    }
        void processIfStmt(TreeNode* node) {
            string label_else = "else_" + std::to_string(label_id); // 标记else语句块
            string label_end = "endif_" + std::to_string(label_id); // 标记if-else语句结束
            label_id++; // 更新标签编号
            // 将条件变量加载到$t0寄存器
            string id_type;
            string id_name = node->N2->N1->val.second;
            for (int i = 0; i < symboltable.size(); i++) {
                if (symboltable[i].first.first == id_name) {
                    id_type = symboltable[i].first.second;
                    break;
                }
            }
            if(id_type=="int")
            emit("lw $t0, " + id_name);
            else
                emit("lb $t0, " + id_name);
            // 根据条件跳转到else语句块或者if语句块
            emit("beqz $t0, " + label_else); // 如果条件为假，则跳转到else语句块
            processSTMT(node->N4); // 生成if语句块的代码
            emit("j " + label_end); // 无条件跳转到if-else语句结束
            emit(label_else + ":"); // 插入else语句块的标签
            processELSESTMT(node->N5); // 如果存在else语句，则需要生成跳转到if语句块后面的代码
            emit(label_end + ":"); // 插入if-else语句结束的标签
        }

        void processELSESTMT(TreeNode* node) {
            if (node == nullptr || node->N1->val.first == "empty")
                return;
            else
                processSTMT(node->N2);
        }
        void processWhileStmt(TreeNode* node) {
            std::string loopLabel = "loop" + std::to_string(label_id); // 循环起始位置的标签
            std::string endLoopLabel = "endloop" + std::to_string(label_id); // 循环结束位置的标签
            label_id++; // 更新标签编号
            // 生成循环条件部分的代码
            string id_type;
            string id_name = node->N2->N1->val.second;
            for (int i = 0; i < symboltable.size(); i++) {
                if (symboltable[i].first.first == id_name) {
                    id_type = symboltable[i].first.second;
                    break;
                }
            }
            emit(loopLabel + ":"); // 插入循环起始位置的标签
            // 生成循环条件判断部分的代码
            if (id_type == "int")
                emit("lw $t0, " + id_name);
            else
                emit("lb $t0, " + id_name);
            emit("beqz $t0, " + endLoopLabel); // 如果条件为假，则跳出循环
            // 生成循环体部分的代码
            processSTMT(node->N4);
            emit("j " + loopLabel); // 跳回循环起始位置
            emit(endLoopLabel + ":"); // 插入循环结束位置的标签
        }

        void processReadStmt(TreeNode* node) {
            string id_name = node->N2->N1->val.second;
            string id_type;

            for (int i = 0; i < symboltable.size(); i++) {
                if (symboltable[i].first.first == id_name) {
                    id_type = symboltable[i].first.second;
                }
            }

            // 初始化缓冲区，假设 buffer 是一个已经分配了空间的字符数组
            emit("# 读入");
            emit("la $t0, input_stack");
            emit("addiu $t0, $t0, 1024");
            emit("sw $t0, input_buffer");

            if (id_type == "int") {
                // 系统调用5用于读取一个整数
                emit("li $v0, 5");
                emit("syscall");
                emit("sw $v0, " + id_name); // 将读取的整数保存到变量中
            }
            else {
                // 系统调用8用于读取一个字符串
                emit("li $v0, 8");
                emit("syscall");
                string set_true = "set_true" + std::to_string(label_id);
                string end_set = "end_set" + std::to_string(label_id);
                string set_false = "set_false" + std::to_string(label_id);
                label_id++;
                // 检查字符串的首字符是否为't'或'T'
                emit("lb $t0, 0($a0)"); // 读取第一个字符
                emit("li $t1, 't'"); // 将 't' 载入到 $t1 寄存器
                emit("li $t2, 'T'"); // 将 'T' 载入到 $t2 寄存器
                emit("beq $t0, $t1, "+set_true); // 如果字符等于 't'，跳转到 set_true 标签
                emit("beq $t0, $t2, "+set_true); // 如果字符等于 'T'，跳转到 set_true 标签
                emit("j "+set_false); // 如果字符既不等于 't' 也不等于 'T'，跳转到 set_false 标签
                emit(set_true+":"); // set_true 标签
                emit("li $t0, 1"); // 如果字符串是"true"，设置变量为1
                emit("j "+end_set); // 跳转到 end_set 标签
                emit(set_false+":"); // set_false 标签
                emit("li $t0, 0"); // 如果字符串不是"true"，设置变量为0
                emit(end_set+":"); // end_set 标签
                emit("sw $t0, " + id_name); // 将结果保存到变量中
            }
        }
        void processWriteStmt(TreeNode* node) {
            string id_type;
            string id_name = node->N2->N1->val.second;
            //查找符号表以确定类型
            emit("# 打印");
            for (int i = 0; i < symboltable.size(); i++) {
                if (symboltable[i].first.first == id_name) {
                    id_type = symboltable[i].first.second;
                    break;
                }
            }
            // 负责加载变量到寄存器 $t0 中
            if(id_type=="int")
            emit("lw $t0, " + id_name);
            else
                emit("lb $t0, " + id_name);
            // 输出的整数和布尔值的处理方式可能有所不同，因此我们在这里进行区分。
            if (id_type == "int") {
                emit("li $v0, 1"); // 系统调用代码1表示打印整数
                emit("move $a0, $t0"); // 移动我们的整数到 $a0
                emit("syscall"); // 进行系统调用
            }
            else if (id_type == "bool") {
                // 布尔值可以表示为整数0和1，也可以使用某种打印布尔值的方法处理
                emit("li $v0, 1"); // 系统调用代码1表示打印整数
                emit("move $a0, $t0"); // 移动我们的布尔值到 $a0
                emit("syscall"); // 进行系统调用
            }
        }
        // 处理表达式
        void processEXPR(TreeNode* node) {
            processTERM(node->N1); // 处理TERM
            processADD_(node->N2); // 处理ADD'
        }

        // 处理ADD'表达式，对应于非终结符 ADD' → ADD TERM ADD'
        // ADD' → empty
        void processADD_(TreeNode* node) {
            if (node== nullptr||node->N1->val.first=="empty") {
                // 如果 ADD' → empty, 则不进行任何操作
                return;
            }
            // 处理 TERM
            processTERM(node->N2);
            // 将栈顶的两个元素出栈进行运算，并将结果压入栈中
            emit("# 运算取两个栈顶");
            int_stack_pop_1_2();
            if (node->N1->N1->val.second == "+") {
                emit("# 加法");
                emit("addu $t1, $t1, $t2");
            }
            else {
                emit("# 减法");
                emit("subu $t1, $t2, $t1");
            }
            int_stack_push();
            // 继续处理 ADD'
            processADD_(node->N3);
        }

        // 处理TERM，对应于非终结符 TERM → NEGA MUL'
        void processTERM(TreeNode* node) {
            processNEGA(node->N1); // 处理NEGA
            processMUL_(node->N2); // 处理MUL'
        }

        // 处理MUL'表达式，对应于非终结符 MUL' → MUL NEGA MUL'
        // MUL' → empty
        void processMUL_(TreeNode* node) {
            if (node== nullptr||node->N1->val.first=="empty") {
                // 如果 MUL' → empty, 则不进行任何操作
                return;
            }
            // 处理 NEGA
            processNEGA(node->N2);
            // 将栈顶的两个元素出栈进行运算，并将结果压入栈中
            int_stack_pop_1_2();
            if (node->N1->N1->val.second == "*") {
                emit("# 乘法");
                emit("mul $t1, $t1, $t2");
            }
            else {
                emit("# 除法");
                emit("div $t1, $t2, $t1");
            }
            emit("# 将栈顶元素写回");
            int_stack_push();
            // 继续处理 MUL'
            processMUL_(node->N3);
        }


        // 处理NEGA，对应于非终结符 NEGA → FACTOR
        // NEGA → - FACTOR
        void processNEGA(TreeNode* node) {
            // Step 1: Process FACTOR
            if (node->N1->val.second == "-") {
                processFACTOR(node->N2); // 处理 FACTOR
            }
            else {
                processFACTOR(node->N1); // 处理 FACTOR
            }

                        
            // Step 3: If necessary, negate
            if (node->N1->val.second == "-") {
                // Step 2: Int_buffer 出栈到 $t1
                int_stack_pop_1();
                emit("# 取反");
                emit("negu $t1, $t1"); // If it's a negative number, negate the value in $t1
                // Step 4: Push the result back to the Int_num stack
                emit("# 写回");
                int_stack_push();
            }
        }


        // 处理FACTOR，对应于非终结符 FACTOR → ( EXPR )
        // FACTOR → id
        // FACTOR → number
        void processFACTOR(TreeNode* node) {
            if (node->N1->val.second == "(") {
                processEXPR(node->N2); // 如果FACTOR是一个括号表达式，处理EXPR
            }
            else if (node->N1->val.first == "id") {
                string id_name = node->N1->N1->val.second;
                string id_type;

                // Search symbol table
                for (int i = 0; i < symboltable.size(); i++) {
                    if (symboltable[i].first.first == id_name) {
                        id_type = symboltable[i].first.second;
                        break; // Once found, break the loop
                    }
                }
            // Load the address of the top of the stack into $t0
                string cur_num;
                if (id_type == "int") {
                    emit("lw $t1, " + id_name);
                    int_stack_push();
                    }
                    else {
                        emit("lb $t1, " + id_name);
                        bool_stack_push_1();
                    }
                }
         
            else {
                string num_val = node->N1->N1->val.second;
                emit("li $t1, " + num_val);
                int_stack_push();
            }
        }
        // 处理BOOL，对应于非终结符 BOOL → JOIN BOOL'
        void processBOOL(TreeNode* node) {
            processJOIN(node->N1); // 处理JOIN
            processBOOL_(node->N2); // 处理BOOL'
        }

        // 处理BOOL'表达式，对应于非终结符 BOOL' → || JOIN BOOL'
        // BOOL' → empty
        void processBOOL_(TreeNode* node) {
            if (node== nullptr||node->N1->val.first=="empty") {
                return; // 如果当前节点为空，则返回
            }
            processJOIN(node->N2); // 处理JOIN
            processBOOL_(node->N3); // 递归处理BOOL'
            bool_stack_pop_1_2();
            emit("andi $t1, $t1, 1");
            emit("or $t1, $t1, $t2"); // 执行逻辑或操作
            bool_stack_push_1();
        }

        // 处理JOIN，对应于非终结符 JOIN → NOT JOIN'
        void processJOIN(TreeNode* node) {
            if (node == nullptr)return;
            processNOT(node->N1); // 处理NOT
            processJOIN_(node->N2); // 处理JOIN'
        }

        // 处理JOIN'表达式，对应于非终结符 JOIN' → && NOT JOIN'
        // JOIN' → empty
        void processJOIN_(TreeNode* node) {
            if (node== nullptr||node->N1->val.first=="empty") {
                return; // 如果当前节点为空，则返回
            }
            processNOT(node->N2); // 处理NOT
            processJOIN_(node->N3); // 递归处理JOIN'
            bool_stack_pop_1_2();
            emit("andi $t1, $t1, 1");
            emit("and $t1, $t1, $t2"); // 执行逻辑与操作
            bool_stack_push_1();
        }

        // 处理NOT，对应于非终结符 NOT → REL
        // NOT → ! REL
        void processNOT(TreeNode* node) {
            if (node->N1->val.second == "!") {
                processREL(node->N2); // 处理REL
                bool_stack_pop_1();
                emit("andi $t1, $t1, 1");
                emit("xori $t1, $t1, 1"); // 如果是逻辑非操作，对寄存器t0执行取反操作
                bool_stack_push_1();
            }
            else {
                processREL(node->N1); // 处理REL
            }
        }

        // 处理REL，对应于非终结符 REL → EXPR ROP EXPR
        void processREL(TreeNode* node) {
            processEXPR(node->N1); // 处理EXPR
            processEXPR(node->N3); // 处理下一个EXPR
            int_stack_pop_1_2();
            string operation = node->N2->N1->val.second;
            // 根据运算符生成相应的汇编指令
            if (operation == ">") {
                emit("sgt $t1, $t2, $t1"); // 对应 > 操作
            }
            else if (operation == ">=") {
                emit("sge $t1, $t2, $t1"); // 对应 >= 操作
            }
            else if (operation == "<") {
                emit("slt $t1, $t2, $t1"); // 对应 < 操作
            }
            else if (operation == "<=") {
                emit("sle $t1, $t2, $t1"); // 对应 <= 操作
            }
            else if (operation == "==") {
                emit("seq $t1, $t2, $t1"); // 对应 == 操作
            }
            else if (operation == "!=") {
                emit("sne $t1, $t2, $t1"); // 对应 != 操作
            }
            bool_stack_push_1();
        }






    



