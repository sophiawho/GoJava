CC = gcc

CFLAGS = -Wall

main: parser.tab.o lex.yy.o main.o
	$(CC) lex.yy.o parser.tab.o main.o -o golite -lfl

parser.tab.c parser.tab.h: golite.y
	bison --yacc --defines=parser.tab.c --output=parser.tab.h golite.y

lex.yy.c: golite.l parser.tab.h
	flex golite.l
	
clean:
	rm parser.tab.* lex.yy.*; rm *.o
