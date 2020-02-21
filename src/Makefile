CC = gcc

CFLAGS = -Wall

main: parser.tab.o lex.yy.o main.o ast/tree.h ast/tree.o ast/stmt.h ast/stmt.o error.o weed.o pretty.o
	$(CC) lex.yy.o parser.tab.o ast/tree.o ast/stmt.o error.o weed.o pretty.o main.o -o golite -lfl

parser.tab.c parser.tab.h: golite.y
	bison -v --yacc --defines=parser.tab.c --output=parser.tab.h golite.y

lex.yy.c: golite.l parser.tab.h
	flex golite.l

error.o: error.h error.c
	$(CC) -c error.c error.h

weed.o: weed.c weed.h error.o
	$(CC) -c weed.c weed.h
	
clean:
	rm parser.tab.* lex.yy.*; rm *.o
