#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <stack>
#include <fstream>
const int num_of_keyword=25;
const int num_of_MO=13;
const int num_of_BO=12;
const int num_of_D=9;
const char keyword[50][12]={"break","case","char","continue","do","default","double","getint",
"else","float","for","if","int","include","long","main","return","switch","typedef","void","unsigned","while","iostream","const","printf"};
const char keyword_Token[50][12]={"BREAKTK","CASETK","CHARTK","CONTINUETK","DOTK","DEFAULTTK","DOUBLETK","GETINTTK","ELSETK","FLOATTK","FORTK",
"IFTK","INTTK","INCLUDETK","LONGTK","MAINTK","RETURNTK","SWITCHTK","TYPEDEFTK","VOIDTK","UNSIGNEDTK","WHILETK","IOSTREAMTK","CONSTTK","PRINTFTK"};
const char Monocular_Operator[20]={'+','-','*','/','!','%','~','&','|','^','=','<','>'};   //单目运算符
const char MO_Token[20][10]={"PLUS","MINU","MULT","DIV","NOT","MOD","NEG","ANDBIT","ORBIT","XOR","ASSIGN","LSS","GRE"}; //单目运算符
const char Binocular_Operator[20][5]={"++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/="}; //双目运算符
const char BO_Token[20][15]={"PLUSPLUS","MINUSMINUS","AND","OR","LEQ","NEQ","EQL","GEQ","PLUSE","MINUSE","MULTE","DIVE"}; //双目运算符
const char Delimiter[20]={',','(',')','{','}',';','#','[',']'}; //界符
const char Delimiter_Token[20][10]={"COMMA","LPARENT","RPARENT","LBRACE","RBRACE","SEMICN","SHARP","LBRACK","RBRACK"}; //界符
const char IDENTIFIER[10]="IDENFR";
const char INTCON[10]="INTCON";
const char FLOATCON[10]="FLOATCON";
const char FORMATSTRING[10]="STRCON";
FILE* file_source=NULL;
std::ifstream input;
std::ofstream output;

char state[1000];
int len_state;
char start;
char final[1000];
int len_final;
bool is_final[1000];

struct NFA_set
{
	char set[1000];
	int len=0;
};
char buffer[1000];
int buffer_size=0;

NFA_set move[1000][1000];

char N_state[1000];
int N_len_state;
char N_start;
char N_final[1000];
int N_len_final;
bool N_is_final[1000];

NFA_set new_set[1000];
int num_new_set=0;
int dfa[1000][1000];

bool IsInteger(char a)
{
	if(a>='0' && a<='9')
		return true;
	return false;
}
bool IsLetter(char a)
{
	if(a>='a' && a<='z')
		return true;
	if(a>='A' && a<='Z')
		return true;
	return false;
}
bool IsKeyword(char a[])
{
	int len=strlen(a);
	for(int j=0;j<num_of_keyword;++j)
	{
		if(strlen(keyword[j])==len)
		{
			if(strcmp(keyword[j],a)==0)
				return true;
		}
	}
	return false;
}
bool IsMO(char a)
{
	for(int i=0;i<num_of_MO;++i)
	{
		if(Monocular_Operator[i]==a)
			return true;
	}
	return false;
}
bool IsBO(char a[])
{
	for(int i=0;i<num_of_BO;++i)
	{
		if(strcmp(Binocular_Operator[i],a)==0)
			return true;
	}
	return false;
}
bool IsDelimiter(char a)
{
	for(int i=0;i<num_of_D;++i)
	{
		if(Delimiter[i]==a)
			return true;
	}
	return false;
}

bool Is_In_state(char a)
{
	for(int i=0;i<len_state;++i)
	{
		if(a==state[i])
			return true;
	}
	return false;
}
bool Is_In_final(char a)
{
	for(int i=0;i<len_final;++i)
	{
		if(a==final[i])
			return true;
	}
	return false;
}
void createNFA()
{
	int N;
	bool flag=true;   //是不是第一个
	char ch;    //用来读 文法左边的
	char nouse;
	char str[10];    //用来读 文法 右边的
	input.open("1.txt");
	input>>N;
	while(N--)
	{
		input>>ch>>nouse>>nouse>>str;
		if(flag)
		{
			start=ch;
			flag=false;
		}
		if(!Is_In_state(ch))
		{
			state[len_state++]=ch;
		}
		if(!Is_In_final(str[0]))
		{
			final[len_final++]=str[0];
		}
		if(strlen(str)>1)
		{
			move[ch][str[0]].set[move[ch][str[0]].len++]=str[1];
		}
		else
		{
			move[ch][str[0]].set[move[ch][str[0]].len++]='Y';  //终态
		}
	}
}


bool Is_in_set(char a,NFA_set temp)
{
	for(int i=0;i<temp.len;++i)
	{
		if(a==temp.set[i])
			return true;
	}
	return false;
}
int Is_in(NFA_set temp)   //和已有的newset有没有重复的，有就返回重复的编号
{
	bool flag[1000];
	bool flag1;
	for(int i=0;i<temp.len;++i)
	{
		flag[i]=false;
	}
	for(int i=0;i<num_new_set;++i)
	{
		for(int k=0;k<temp.len;++k)
		{
			for(int j=0;j<new_set[i].len;++j)
			{
				if(temp.set[k]==new_set[i].set[j])
				{
					flag[k]=true;
				}
			}
		}
		flag1=true;
		for(int m=0;m<temp.len;++m)
		{
			if(flag[m]==false)
			{
				flag1=false;
				break;
			}
		}
		if(flag1==true)
			return i;
		for(int m=0;m<temp.len;++m)
		{
			flag[m]=false;
		}
	}
	return -1;
}
void get_closure(NFA_set &temp)    //得到一个完整的子集 
{
	for(int i=0;i<temp.len;++i)
	{
		for(int j=0;j<move[temp.set[i]]['@'].len;++j)
		{
			if(!Is_in_set(move[temp.set[i]]['@'].set[j],temp))
			{
				temp.set[temp.len++]=move[temp.set[i]]['@'].set[j];
			}
		}
	}
}
bool Is_contained_Y(NFA_set temp)   //判断是否是终态
{
	for(int i=0;i<temp.len;++i)
	{
		if(temp.set[i]=='Y')
			return true;
	}
	return false;
}
void NFA_to_DFA()
{
	num_new_set=0;
	NFA_set work_set;
	NFA_set worked_set;
	work_set.set[work_set.len++]=start;
	worked_set.len=0;
	std::stack<NFA_set> s;
	get_closure(work_set);
	s.push(work_set);
	new_set[num_new_set++]=work_set;
	for(int i=0;i<1000;++i)
	{
		for(int j=0;j<1000;++j)
		{
			dfa[i][j]=-1;
		}
	}
	for(int i=0;i<1000;++i)
		is_final[i]=false;
	if(Is_contained_Y(work_set))
		is_final[num_new_set-1]=true;
	while(!s.empty())
	{
		work_set=s.top();
		s.pop();
		for(int i=0;i<len_final;++i)
		{
			for(int j=0;j<work_set.len;++j)
			{
				for(int k=0;k<move[work_set.set[j]][final[i]].len;++k)
				{
					if(move[work_set.set[j]][final[i]].set[k]!='#' && move[work_set.set[j]][final[i]].set[k]!='Y' && 
					!Is_in_set(move[work_set.set[j]][final[i]].set[k],worked_set))
					{
						worked_set.set[worked_set.len++]=move[work_set.set[j]][final[i]].set[k];
					}
					if(move[work_set.set[j]][final[i]].set[k]=='Y' && !Is_in_set(move[work_set.set[j]][final[i]].set[k],worked_set))
					{
						worked_set.set[worked_set.len++]='Y';    //用Y表示终态
					}
				}
			}
			get_closure(worked_set);
			if(worked_set.len>0 && Is_in(worked_set)==-1)
			{
				dfa[num_new_set-1][final[i]]=num_new_set;
				s.push(worked_set);
				new_set[num_new_set++]=worked_set;
				if(Is_contained_Y(worked_set))
				{
					is_final[num_new_set-1]=true;
				}
			}
			if(worked_set.len>0 && Is_in(worked_set)>-1 && final[i]!='@')
			{				
				dfa[Is_in(work_set)][final[i]]=Is_in(worked_set);
			}
			worked_set.len=0;
		}
	}

}

bool DFA(char str[])
{
	char now_state=0;
	for(int i=0;i<strlen(str);++i )
	{
		now_state=dfa[now_state][str[i]];
		if(now_state==-1)
			return false;
	}
	if(is_final[now_state]==true)
		return true;
	return false;
}

char Up_to_Low(char ch)		// 不区分大小写，如果是大写转变为小写
{
	if (ch >= 'A' && ch <= 'Z')
	{
		return ch+32;
	}
	return ch;
}

void output_constant(char buffer[])
{
	// 如果是整型数字，输出INTTK buffer
	bool is_int = true;
	for(int i=0;i<strlen(buffer);++i)
	{
		if(!IsInteger(buffer[i]))
		{
			is_int = false;
			break;
		}
	}

	if(is_int)
	{
		std::cout<< INTCON <<" "<<buffer<<std::endl;
		output<< INTCON <<" "<<buffer<<std::endl;
	}
	else
	{
		std::cout<< FLOATCON <<" "<<buffer<<std::endl;
		output<< FLOATCON <<" "<<buffer<<std::endl;
	}
}
void output_key(char buffer[])
{
	for(int i=0;i<num_of_keyword;++i)
	{
		if(strcmp(buffer,keyword[i])==0)
		{
			std::cout<<keyword_Token[i]<<" "<<buffer<<std::endl;
			output<<keyword_Token[i]<<" "<<buffer<<std::endl;
			return;
		}
	}
}
void output_iden(char buffer[])
{
	std::cout<<IDENTIFIER<<" "<<buffer<<std::endl;
	output<<IDENTIFIER<<" "<<buffer<<std::endl;
}
void output_string(char buffer[])
{
	std::cout<<FORMATSTRING<<" "<<buffer<<std::endl;
	output<<FORMATSTRING<<" "<<buffer<<std::endl;
}
void output_delimiter(char buffer)
{
	for(int i=0;i<num_of_D;++i)
	{
		if(buffer==Delimiter[i])
		{
			std::cout<<Delimiter_Token[i]<<" "<<buffer<<std::endl;
			output<<Delimiter_Token[i]<<" "<<buffer<<std::endl;
			return;
		}
	}
}
void output_BO(char buffer[])
{
	for(int i=0;i<num_of_BO;++i)
	{
		if(strcmp(buffer,Binocular_Operator[i])==0)
		{
			std::cout<<BO_Token[i]<<" "<<buffer<<std::endl;
			output<<BO_Token[i]<<" "<<buffer<<std::endl;
			return;
		}
	}
}
void output_MO(char buffer)
{
	for(int i=0;i<num_of_MO;++i)
	{
		if(buffer==Monocular_Operator[i])
		{
			std::cout<<MO_Token[i]<<" "<<buffer<<std::endl;
			output<<MO_Token[i]<<" "<<buffer<<std::endl;
			return;
		}
	}
}

void scan()
{
	char str[100000];
	char ch;
	int i,j;
	int point;
	int flag;
	bool is_success=true;
	
	output.open("output.txt");
	ch=Up_to_Low(fgetc(file_source));
	bool finish=false;
	while(!finish)
	{
	 	flag=-1;
		point=0;
		if(IsInteger(ch))     //多一个ch
		{
			flag=1;
			str[point++]=ch;
			ch=Up_to_Low(fgetc(file_source));
			while(IsLetter(ch) || IsInteger(ch) || ch=='.' || ch=='+' || ch=='-')
			{
				flag=1;
				str[point++]=ch;
				ch=Up_to_Low(fgetc(file_source));
			}
			str[point]='\0';
		}
		if(flag==1)
		{
			if(DFA(str))
			{
				// std::cout<<str<<" "<<"常量"<<std::endl;
				output_constant(str);
			}
			else
			{
				std::cout<<str<<" "<<"illegal constant"<<std::endl;
				is_success=false;
			}
			point=0;
			flag=-1;
		}
		if(IsLetter(ch))
		{
			flag=2;
			str[point++]=ch;
			ch=Up_to_Low(fgetc(file_source));
			while(IsLetter(ch) || IsInteger(ch))
			{
				flag=2;
				str[point++]=ch;
				ch=Up_to_Low(fgetc(file_source));
			}
			str[point]='\0';
		}
		if(flag==2)
		{
			if(IsKeyword(str))
			{
				// std::cout<<str<<" "<<"关键字"<<std::endl;
				output_key(str);
			}
			else
			{
				if(DFA(str))
				{
					// std::cout<<str<<" "<<"标识符"<<std::endl;
					output_iden(str);
				}
				else
				{
					std::cout<<str<<" "<<"illegal identity"<<std::endl;
					is_success=false;
				}
			}
			point=0;
			flag=-1;
		}
		if(ch=='\"')
		{
			str[point++]=ch;
			ch=fgetc(file_source);
			while(ch!='\"' && ch != EOF)
			{
				str[point++]=ch;
				ch=fgetc(file_source);
			}
			ch=fgetc(file_source);
			str[point]='\"';
			str[point+1]='\0';
			// std::cout<<str<<" "<<"字符串"<<std::endl;
			output_string(str);
			if(ch==EOF)
			{
				is_success=false;
				finish=true;
				break;
			}
		}
		if(IsDelimiter(ch))
		{
			// std::cout<<ch<<" "<<"界符"<<std::endl;
			output_delimiter(ch);
			if((ch=Up_to_Low(fgetc(file_source)))==EOF)
			{
				finish=true;
				break;
			}
		}
		if(IsMO(ch))
		{
			str[point++]=ch;
			if((ch=Up_to_Low(fgetc(file_source)))==EOF)
			{
				finish=true;
			}
			str[point++]=ch;
			str[point]='\0';

			// 注释处理
			if(str[0]=='/' && str[1]=='/')
			{
				ch=fgetc(file_source);
				while(ch!='\n' && ch != EOF)
				{
					ch=fgetc(file_source);
				}
				if(ch!=EOF)
				{
					ch=Up_to_Low(fgetc(file_source));
				}
				else
				{
					finish = true;
				}
			}
			else if(str[0]=='/' && str[1]=='*')
			{
				int note_flag = 0;
				while(note_flag!=2 && ch!=EOF)
				{
					ch=fgetc(file_source);
					if(ch=='*')
					{
						note_flag = 1;
						continue;
					}
					if(note_flag==1 && ch=='/')
					{
						note_flag = 2;
					}
					if(note_flag==1 && ch!='/')
					{
						note_flag = 0;
					}
				}
				if(ch!=EOF)
				{
					ch=Up_to_Low(fgetc(file_source));
				}
				else
				{
					is_success = false;
					finish = true;
				}
			}
			else
			{
				if(finish==false && IsBO(str))
				{
					output_BO(str);
					ch=Up_to_Low(fgetc(file_source));
				}
				else
				{
					output_MO(str[0]);
				}
				point=0;
			}
		}

		if(ch==' ' || ch=='\n' || ch=='\t')
		{
			if((ch=Up_to_Low(fgetc(file_source)))==EOF)
			{
				finish=true;
				break;
			}
			continue;
		}
	}
	output.close();
	if(is_success)
	{
		std::cout<<"success analysis"<<std::endl;
	}
	else
	{
		std::cout<<"fail analysis"<<std::endl;
	}
}
void init()
{ 
	len_final=0;
	len_state=0;
	for(int i=0;i<1000;++i)
	{
		//is_final[i]=false;
		for(int j=0;j<1000;++j)
			for(int k=0;k<1000;++k)
				move[i][j].set[k]='#';
	}
}

int main()
{
	init();
	len_final=0;
	len_state=0;
	createNFA();
	NFA_to_DFA();
	file_source=fopen("test.c","r+");
	scan();
	fclose(file_source);
	return 0;
}