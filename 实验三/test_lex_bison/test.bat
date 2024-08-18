::win_bison -d calc.y
::win_flex calc.l

::gcc lex.yy.c calc.tab.c -o calc
::./calc

::写一个批处理文件，一键编译运行

@echo off
win_bison -d calc.y
win_flex calc.l
gcc lex.yy.c calc.tab.c -o calc
calc.exe




