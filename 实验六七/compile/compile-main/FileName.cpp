#include"lexical_analyzer.h"
#include"Parser.h"
#include"getx86.h"
int main() {
    string a = "C:\\Users\\13151\\Desktop\\编译原理\\实验\\实验六七\\compile\\compile-main\\test.txt";                      //源代码
    string b = "C:\\Users\\13151\\Desktop\\编译原理\\实验\\实验六七\\compile\\compile-main\\result.txt";   //词法分析结果
    string e = "C:\\Users\\13151\\Desktop\\编译原理\\实验\\实验六七\\compile\\compile-main\\result_parser.txt";             //语法分析结果
    string test = "test_parser_result.txt";     //根据语法分析结果重新构造树之后再进行序列化的结果
    Parser d(a, b, e);
    if (d.do_and_write()) { 
        TreeNode* root = d.deserialize(e);//反序列化构造树
        d.serialize(test, root);                    //序列化反序列化结果
        if (file_same(e, test))
            cout << "反序列化成功" << endl;                 //判断两个序列化结果是否一致
        else
            cout << "反序列化失败" << endl;
        processPROG(root);
    }

    return 0;
}

