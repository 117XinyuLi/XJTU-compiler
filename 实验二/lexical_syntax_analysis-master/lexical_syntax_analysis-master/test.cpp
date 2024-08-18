#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <stack>
#include <fstream>
const int num_of_keyword=22;
const int num_of_MO=11;
const int num_of_BO=12;
const int num_of_D=9;
const char keyword[50][12]={"break","case","char","continue","do","default","double",
"else","float","for","if","int","include","long","main","return","switch","typedef","void","unsigned","while","iostream"};
const char Monocular_Operator[20]={'+','-','*','/','!','%','~','&','|','^','='};   //单目运算符
const char Binocular_Operator[20][5]={"++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/="}; //双目运算符
const char Delimiter[20]={',','(',')','{','}',';','<','>','#'}; //界符
FILE* file_source=NULL;


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

void scan()
{
	char str[10000];
	char ch;
	int i,j;
	int point;
	int flag;

	ch=fgetc(file_source);
	bool finish=false;
	while(!finish)
	{
	 	flag=-1;
		point=0;
		if(IsInteger(ch))  
		{
			flag=1;
			str[point++]=ch;
			ch=fgetc(file_source);
			while(IsLetter(ch) || IsInteger(ch) || ch=='.' || ch=='+' || ch=='-')
			{
				flag=1;
				str[point++]=ch;
				ch=fgetc(file_source);
			}
			str[point]='\0';
		}
		if(IsLetter(ch))
		{
			flag=2;
			str[point++]=ch;
			ch=fgetc(file_source);
			while(IsLetter(ch) || IsInteger(ch))
			{
				flag=2;
				str[point++]=ch;
				ch=fgetc(file_source);
			}
			str[point]='\0';
		}
		if(flag==2)
		{
			if(IsKeyword(str))
			{
				std::cout<<str<<" "<<"关键字"<<std::endl;
			}
			point=0;
			flag=-1;
		}
		if(IsDelimiter(ch))
		{
			std::cout<<ch<<" "<<"界符"<<std::endl;
			if((ch=fgetc(file_source))==EOF)
			{
				finish=true;
				break;
			}
		}
		if(IsMO(ch))
		{
			str[point++]=ch;
			if((ch=fgetc(file_source))==EOF)
			{
				finish=true;
			}
			str[point++]=ch;
			str[point]='\0';
			if(finish==false && IsBO(str))
			{
				std::cout<<str<<" "<<"双目运算符"<<std::endl;
				ch=fgetc(file_source);
			}
			else
			{
				std::cout<<str[0]<<" "<<"单目运算符"<<std::endl;
			}
			point=0;
		}

		if(ch==' ' || ch=='\n' || ch=='\t')
		{
			if((ch=fgetc(file_source))==EOF)
			{
				finish=true;
				break;
			}
			continue;
		}
	}
}
int main()
{
	file_source=fopen("test.txt","r+");
	scan();
	fclose(file_source);
	return 0;
}