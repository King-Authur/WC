#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include <windows.h>
#include <iostream>
using namespace std;
const int maxn_word_of_line = 1000;//某一行代码的最长的长度
const int maxn_word_of_file_path = 1000;//文件路径的最长的长度

struct Result{
    int character, word, line;
    int code_line, empty_line, explain_line;//只检测用\\注释方式，不检测/**/注释的方式
}res;
struct Flag{
    bool s, c, w, l, a, x;
}flag;

void get_flag(int argc, char **argv);
void calculate_information(char *file_path);
void display();
bool is_char(char ch);

bool is_char(char ch){
    return ch != ' ' && ch != '\r' && ch != '\n'  && ch != '\t' && ch!= '{' && ch != '}';///如果是 回车 空格 TAB 左右大括号 的组合，则是空行
}
void get_flag(int argc, char **argv){
    for(int i = 0; i < argc - 1; i++)
    {
        if(strcmp(argv[i], "-x") == 0)//when we use -x,we have only one parameter, so we can break        {
        {
            flag.c = flag.w = flag.l = flag.a = flag.x = true;
            break;
        }
        if(strcmp(argv[i], "-s") == 0)
        {
            flag.s = true;
        }
        if(strcmp(argv[i], "-c") == 0)
        {
            flag.c = true;
        }
        if(strcmp(argv[i], "-w") == 0)
        {
            flag.w = true;
        }
        if(strcmp(argv[i], "-l") == 0)
        {
            flag.l = true;
        }
        if(strcmp(argv[i], "-a") == 0)
        {
            flag.a = true;
        }
    }
}
void calculate_information(char *file_path){
    FILE* fp = fopen(file_path, "r");//只读打开文件路径指向的文件
    if(fp == NULL){//检测是否是有效路径
        printf("!!!错误!!!\n请检查文件路径是否正确\n");
        exit(-1);//关闭程序
    }

    char buffer[maxn_word_of_line];//存取当前行数据的缓存区
    while(!feof(fp))//是否到文件尾
    {
        fgets(buffer, maxn_word_of_line, fp);//一整行读取
        res.line++;

        int len = strlen(buffer);
        bool left_blank = true, is_Empty = true;
        for(int i = 0; i < len; i++)
        {
            if(is_char(buffer[i]))///如果是非空字符
            {
                res.character++;///那么字符数要增加
                is_Empty = false;///并且这一行不为空
            }

            if(isalpha(buffer[i]))///当它是字母时
            {
                if(left_blank)///当左边被其他字符隔开时
                {
                    res.word++;///单词数目+1
                    left_blank = false;
                }
            }
            else///当前字符不是字母
            {
                left_blank = true;///则视为它是隔开单词的字符
            }
        }

        if(is_Empty)///这一行为空行
        {
            res.empty_line++;
        }
        else///这一行非空,判断一下是否为注释行
        {
            bool left_quote = false;///左双引号
            bool slanting_bar = false;///斜杆
            bool explain_flag = false;///注释行的标记
            ///过滤掉包含于字符串输出的“假注释”，例如：printf("//");
            for(int i = 0; i < len; i++)
            {
                if(buffer[i] == '"')///如果是双引号
                {
                    if(!left_quote)///如果没有左边匹配的双引号
                    {
                        left_quote = true;
                    }
                    else
                    {
                        left_quote = false;///左边有可以匹配的双引号
                    }
                }

                if(buffer[i] == '/' && buffer[i + 1] == '/')///如果有连续的两个反斜杠
                {
                    slanting_bar = true;
                }

                if(!left_quote && slanting_bar)///如果斜杠没有被包含在字符串的输出中，则这一行必定为注释行
                {
                    res.explain_line++;
                    explain_flag = true;
                    break;
                }
            }

            if(!explain_flag)///不是注释行，就是代码行
            {
                res.code_line++;
            }
        }
    }
}
void display(){
    if(flag.c)
    {
        printf("%d 字符\n", res.character);
    }
    if(flag.w)
    {
        printf("%d 单词\n", res.word);
    }
    if(flag.l)
    {
        printf("%d 行\n", res.line);
    }
    if(flag.a)
    {
        printf("%d 代码行\n", res.code_line);
        printf("%d 空行\n", res.empty_line);
        printf("%d 注释行\n\n\n", res.explain_line);
    }
}

int main(int argc, char **argv){
    char file_path[maxn_word_of_file_path];
    strcpy(file_path, argv[argc - 1]);//file path is in argv[argc - 1]
    printf("The file path is : %s\n", file_path);
    get_flag(argc, argv);
    if(flag.x)//graph  GUI
    {
        printf("-x function\n");
        printf("Later we will complish it\n");
        //图形化界面还不会
    }
    else
    {
        if(flag.s)//calculate all the file under the path
        {
            printf("-s function\n");
            printf("Later we will complish it\n");
            //递归文件夹还不会
            display();
            //每次display()后,初始化存储答案的结构体。
        }
        else
        {
            calculate_information(file_path);///计算所有的信息
            display();//打印要输出的信息
        }
    }
    return 0;
}
