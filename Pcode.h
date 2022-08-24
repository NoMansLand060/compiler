//
// Created by NoMansLand on 2021/11/19.
//

#include <stdio.h>

#ifndef MAIN_C_PCODE_H
#define MAIN_C_PCODE_H

#endif //MAIN_C_PCODE_H

enum Operate{CON,ARRAY,INIT,VAR,FUNC,ARGINIT,RETURN,MAIN,ARG,BST,BND,PRINT,GET,POP,PUSH,SELECT
            ,POS,NEG,NOT,JF,MUL,DIV,MOD,ADD,MIN,Smaller,Bigger,SmallerOrEqual,BiggerOrEqual
            ,Equal,NotEqual,And,Or,MAINEND,
            IFBEGIN,IFEND,ELSE,IFBEZ,J_IFEND,
            WHILEBEGIN,WHILEEND,WHILEBEZ,WHILE_LOOP,
            BREAK,CONTINUE};


struct pcode{
    enum Operate OperateName;
    char OperateObj[3000];
};

struct pcode Pcodes[10000];

int pnum;

void addPcode(enum Operate a, char b[3000]);
void Runner();
void OutPcodes();