//
// Created by NoMansLand on 2021/11/19.
//

#include "Pcode.h"
#include <string.h>

int pnum = 0;


void addPcode(enum Operate a, char b[100]){
    Pcodes[pnum].OperateName = a;
    strcpy(Pcodes[pnum].OperateObj,b);
    pnum++;
}
