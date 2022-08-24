//
// Created by NoMansLand on 2021/11/20.
//

#include <string.h>
#include <stdlib.h>
#include "Pcode.h"

char *Operate_S[] = {"CON", "ARRAY", "INIT", "VAR", "FUNC", "ARGINIT", "RETURN", "MAIN", "ARG", "BST", "BND", "PRINT",
                     "GET", "POP", "PUSH", "SELECT", "POS", "NEG", "NOT", "JF", "MUL", "DIV", "MOD", "ADD", "MIN",
                     "Smaller", "Bigger", "SmallerOrEqual", "BiggerOrEqual", "Equal", "NotEqual", "And", "Or",
                     "MAINEND","IFBEGIN","IFEND","ELSE","IFBEZ","J_IFEND",
                     "WHILEBEGIN","WHILEEND","WHILEBEZ","WHILE_LOOP",
                     "BREAK","CONTINUE"};
char *Operate_T[] = {
                    "Var", "Con", "Arg", "Ra", "Bst", "Mid", "If", "While"
                    };
enum Type {
    Var, Con, Arg, Ra, Bst, Mid, If, While
};

struct Run {
    enum Type type;
    char Name[3000];
    int value;
    int *addr;
    int dimension;
    int x;
    int y;
};

struct Run RunStack[10000];
int RunStack_top = 0;


struct Sign {
    enum Type type;
    char Name[3000];
    int value;
    int *addr;
    int dimension;
    int x;
    int y;
};

struct Sign SignList[10000];
int SignList_top = 0;

int pc = 0;

void OutPcodes() {
    for (int i = 0; i < pnum; i++) {
        printf("%d %s %s\n",i, Operate_S[Pcodes[i].OperateName], Pcodes[i].OperateObj);
    }
}

void Runner(FILE *fp3) {
    enum Operate operate = Pcodes[pc].OperateName;
    char operateObj[3000];
    strcpy(operateObj, Pcodes[pc].OperateObj);
    int pc_main = 0;
    int main_flag = 0;
    for (pc_main = 0; pc_main < pnum; pc_main++) {
        if (Pcodes[pc_main].OperateName == MAIN) break;
    }
    pc_main--;
    while (operate != MAINEND && pc < pnum) {
        //printf("--------%s %s\n",Operate_S[operate],operateObj);
        //printf("%d %s %s\n", pc, Operate_S[Pcodes[pc].OperateName], Pcodes[pc].OperateObj);
        if (operate == CON || operate == VAR) {
            SignList_top++;
            strcpy(SignList[SignList_top].Name, operateObj);
            if (operate == CON) SignList[SignList_top].type = Con;
            else if (operate == VAR) {
                SignList[SignList_top].type = Var;
                SignList[SignList_top].value = 0;
            }
            pc++;
        } else if (operate == ARRAY) {
            int dimension = atoi(operateObj);
            SignList[SignList_top].dimension = dimension;
            if (dimension == 1) {
                int x = RunStack[RunStack_top].value;
                RunStack_top--;
                //if (!SignList[SignList_top].addr) free(SignList[SignList_top].addr);
                SignList[SignList_top].addr = (int *) malloc((x + 1)*sizeof(int ));
                for (int i = 0; i < x; i ++){
                    SignList[SignList_top].addr[i] = 0;
                }
                SignList[SignList_top].x = x;
            } else if (dimension == 2) {
                int y = RunStack[RunStack_top].value;
                RunStack_top--;
                int x = RunStack[RunStack_top].value;
                RunStack_top--;
                SignList[SignList_top].x = x;
                SignList[SignList_top].y = y;
                //if (!SignList[SignList_top].addr) free(SignList[SignList_top].addr);
                SignList[SignList_top].addr = (int *) malloc(((x + 1) * (y + 1))* sizeof(int ));
                for (int i = 0; i < (x + 1) * (y + 1); i ++){
                    SignList[SignList_top].addr[i] = 0;
                }
            }
            pc++;
        } else if (operate == INIT) {
            if (SignList[SignList_top].dimension == 0) {
                SignList[SignList_top].value = RunStack[RunStack_top].value;
                RunStack_top--;
            } else {
                int x = SignList[SignList_top].x;
                int y = SignList[SignList_top].y;
                if (SignList[SignList_top].dimension == 1) {
                    for (int i = x-1; i >= 0; i--) {
                        SignList[SignList_top].addr[i] = RunStack[RunStack_top].value;
                        RunStack_top--;
                    }
                } else if (SignList[SignList_top].dimension == 2) {
                    for (int i = x * y - 1; i >= 0; i--) {
                        //printf("!!!!!%d\n",RunStack[RunStack_top].value);
                        //printf("!!!!%s\n",SignList[SignList_top].Name);
                        SignList[SignList_top].addr[i] = RunStack[RunStack_top].value;
                        RunStack_top--;
                    }
                    for (int i = 0;i<=x*y-1;i++){
                        //printf("%d ",SignList[SignList_top].addr[i] );
                        //printf("*******\n");
                    }
                }
            }
            pc++;
        } else if (operate == FUNC) {
            if (main_flag == 0) {
                pc = pc_main;
                main_flag = 1;
            }
            pc++;
        } else if (operate == ARGINIT) {
            int t = SignList_top;
            int sum = 0;
            while (SignList[t].type != Ra) {
                t--;
                sum++;
            }
            //printf("@@@@@@@@@@@Run:%s %d\n",RunStack[RunStack_top-1].Name,RunStack[RunStack_top - 1].value);
            for (int i = 0; i < sum; i++) {
                SignList[SignList_top - i].value = RunStack[RunStack_top - i - 1].value;
                int x = RunStack[RunStack_top - i - 1].x;
                int y = RunStack[RunStack_top - i - 1].y;
                /*if (RunStack[RunStack_top - i - 1].dimension != 0){
                    SignList[SignList_top - i].addr = (int *) malloc((x + 1)*(y + 1)*sizeof(int ));
                    for (int j = 0; j < (x+1)*(y+1); j++){
                        printf("%d ",j);
                        SignList[SignList_top - i].addr[j] = RunStack[RunStack_top - i - 1].addr[j];
                    }
                printf("\n");
                }*/
                SignList[SignList_top - i].addr = RunStack[RunStack_top - i - 1].addr;
                SignList[SignList_top - i].dimension = RunStack[RunStack_top - i - 1].dimension;
                SignList[SignList_top - i].x = RunStack[RunStack_top - i - 1].x;
                SignList[SignList_top - i].y = RunStack[RunStack_top - i - 1].y;
            }
            //printf("@@@@@@@@@@@Sign:%s %d %d\n",SignList[SignList_top].Name,SignList_top,SignList[SignList_top].value);
            //printf("sum:%d\n",sum);
            int ra = RunStack[RunStack_top].value;
            RunStack_top -= sum;
            RunStack[RunStack_top].type = Ra;
            RunStack[RunStack_top].value = ra;
            pc++;
        } else if (operate == RETURN) {
            while (SignList[SignList_top].type != Ra) SignList_top--;
            pc = SignList[SignList_top].value;
            int ans;
            if (strcmp(operateObj, "TOP") == 0) {
                ans = RunStack[RunStack_top].value;
            }
            while (RunStack[RunStack_top].type != Ra) RunStack_top--;
            SignList_top--;
            //printf("%d %s %s \n",SignList_top,Operate_T[SignList[SignList_top].type],SignList[SignList_top].Name);
            RunStack_top--;
            //printf("%d %s %s \n",RunStack_top,Operate_T[RunStack[RunStack_top].type],RunStack[RunStack_top].Name);
            if (strcmp(operateObj, "TOP") == 0) {
                RunStack_top++;
                RunStack[RunStack_top].type = Mid;
                RunStack[RunStack_top].value = ans;
            }
        } else if (operate == MAIN) {
            RunStack_top++;
            RunStack[RunStack_top].type = Ra;
            RunStack[RunStack_top].value = pnum - 1;
            SignList_top++;
            SignList[SignList_top].type = Ra;
            SignList[SignList_top].value = pnum - 1;
            pc++;
        } else if (operate == ARG) {
            SignList_top++;
            SignList[SignList_top].type = Arg;
            strcpy(SignList[SignList_top].Name, operateObj);
            pc++;
        } else if (operate == BST) {
            SignList_top++;
            SignList[SignList_top].type = Bst;
            RunStack_top++;
            RunStack[RunStack_top].type = Bst;
            pc++;
        } else if (operate == BND) {
            while (SignList[SignList_top].type != Bst) SignList_top--;
            SignList_top--;
            while (RunStack[RunStack_top].type != Bst) RunStack_top--;
            RunStack_top--;
            pc++;
        } else if (operate == PRINT) {
            //printf("###\n");
            int D = 0;
            for (int i = 0; i < strlen(operateObj) - 1; i++) {
                if (operateObj[i] == '%' && operateObj[i + 1] == 'd') D++;
            }
            int d = D;
            int i = 1;
            while (i < strlen(operateObj) - 1) {
                if (operateObj[i] == '%' && operateObj[i + 1] == 'd') {
                    fprintf(fp3, "%d", RunStack[RunStack_top - D + 1].value);
                    D--;
                    i++;
                } else if (operateObj[i] == '\\' && operateObj[i + 1] == 'n') {
                    fprintf(fp3, "\n");
                    i++;
                } else fprintf(fp3, "%c", operateObj[i]);
                i++;
            }
            RunStack_top -= d;
            pc++;
        } else if (operate == GET) {
            int t;
            scanf("%d", &t);
            RunStack_top++;
            RunStack[RunStack_top].type = Mid;
            RunStack[RunStack_top].value = t;
            pc++;
        } else if (operate == POP) {
            if (strcmp(operateObj, "") == 0) {
                RunStack_top--;
            } else {
                int t = SignList_top;
                while (strcmp(SignList[t].Name, operateObj) != 0 || !( SignList[t].type == Var ||SignList[t].type == Con ||SignList[t].type == Arg)) t--;
                if (SignList[t].dimension == 0) {
                    SignList[t].value = RunStack[RunStack_top].value;
                    RunStack_top--;
                } else if (SignList[t].dimension == 1) {
                    SignList[t].addr[RunStack[RunStack_top-1].value] = RunStack[RunStack_top].value;
                    RunStack_top -= 2;
                } else if (SignList[t].dimension == 2) {
                    int x = RunStack[RunStack_top-2].value;
                    int y = RunStack[RunStack_top-1].value;
                    int row = SignList[t].y;
                    SignList[t].addr[x*row+y] = RunStack[RunStack_top].value;
                    RunStack_top -= 3;
                }
            }
            pc++;
        } else if (operate == PUSH) {
            int isINT = 1;
            for (int i = 0; i < strlen(operateObj); i++) {
                if (!(operateObj[i] >= '0' && operateObj[i] <= '9')) isINT = 0;
            }
            if (isINT == 1) {
                RunStack_top++;
                RunStack[RunStack_top].type = Mid;
                RunStack[RunStack_top].value = atoi(operateObj);
                RunStack[RunStack_top].dimension = 0;
            } else {
                int t = SignList_top;
                while (strcmp(SignList[t].Name, operateObj) != 0 || !( SignList[t].type == Var ||SignList[t].type == Con ||SignList[t].type == Arg) ) t--;
                //printf("$$$$$$$$$$$$$Sign:%s %d %d\n",SignList[t].Name,t,SignList[t].value);
                RunStack_top++;
                strcpy(RunStack[RunStack_top].Name, operateObj);
                RunStack[RunStack_top].type = SignList[t].type;
                RunStack[RunStack_top].value = SignList[t].value;
                RunStack[RunStack_top].dimension = SignList[t].dimension;
                RunStack[RunStack_top].addr = SignList[t].addr;
                RunStack[RunStack_top].x = SignList[t].x;
                RunStack[RunStack_top].y = SignList[t].y;
            }
            pc++;
        } else if (operate == SELECT) {
            if (RunStack[RunStack_top].type != Mid) {
                if (RunStack[RunStack_top].dimension == 0)
                    RunStack[RunStack_top].type = Mid;
                //printf("%d:%s->%d\n",pc,RunStack[RunStack_top].Name,RunStack[RunStack_top].value);
            } else if (RunStack[RunStack_top - 1].type != Mid) {
                if (RunStack[RunStack_top - 1].dimension == 1) {
                    RunStack[RunStack_top - 1].type = Mid;
                    RunStack[RunStack_top - 1].value = *(RunStack[RunStack_top - 1].addr +
                                                         RunStack[RunStack_top].value);
                    RunStack[RunStack_top - 1].dimension = 0;
                } else if (RunStack[RunStack_top - 1].dimension == 2) {
                    RunStack[RunStack_top - 1].addr = RunStack[RunStack_top - 1].addr +
                                                      RunStack[RunStack_top].value * RunStack[RunStack_top - 1].y;
                    RunStack[RunStack_top - 1].dimension = 1;
                    RunStack[RunStack_top - 1].x = RunStack[RunStack_top - 1].y;
                }
                RunStack_top --;
            } else if (RunStack[RunStack_top - 2].type != Mid) {
                RunStack[RunStack_top - 2].type = Mid;
                RunStack[RunStack_top - 2].value = *(RunStack[RunStack_top - 2].addr +
                                                     RunStack[RunStack_top - 1].value * RunStack[RunStack_top - 2].y +
                                                     RunStack[RunStack_top].value);
                RunStack[RunStack_top - 2].dimension = 0;
                RunStack_top -= 2;
            }
            //printf("^^^^^^%d\n",R)
            pc++;
        } else if (operate == POS) {
            RunStack[RunStack_top].value = +RunStack[RunStack_top].value;
            pc++;
        } else if (operate == NEG) {
            RunStack[RunStack_top].value = -RunStack[RunStack_top].value;
            pc++;
        } else if (operate == NOT) {
            RunStack[RunStack_top].value = (RunStack[RunStack_top].value == 0)?1:0;
            pc++;
        } else if (operate == JF) {
            int ra = pc + 1;
            RunStack_top++;
            RunStack[RunStack_top].type = Ra;
            RunStack[RunStack_top].value = ra;
            SignList_top++;
            SignList[SignList_top].type = Ra;
            SignList[SignList_top].value = ra;
            while (Pcodes[pc].OperateName != FUNC || strcmp(Pcodes[pc].OperateObj, operateObj) != 0) pc--;
        } else if (operate == MUL) {
            RunStack[RunStack_top - 1].value = RunStack[RunStack_top - 1].value * RunStack[RunStack_top].value;
            RunStack_top--;
            pc++;
        } else if (operate == DIV) {
            RunStack[RunStack_top - 1].value = RunStack[RunStack_top - 1].value / RunStack[RunStack_top].value;
            RunStack_top--;
            pc++;
        } else if (operate == MOD) {
            RunStack[RunStack_top - 1].value = RunStack[RunStack_top - 1].value % RunStack[RunStack_top].value;
            RunStack_top--;
            pc++;
        } else if (operate == ADD) {
            RunStack[RunStack_top - 1].value = RunStack[RunStack_top - 1].value + RunStack[RunStack_top].value;
            RunStack_top--;
            pc++;
        } else if (operate == MIN) {
            RunStack[RunStack_top - 1].value = RunStack[RunStack_top - 1].value - RunStack[RunStack_top].value;
            RunStack_top--;
            pc++;
        } else if (operate == Smaller) {
        RunStack[RunStack_top - 1].value = (RunStack[RunStack_top - 1].value < RunStack[RunStack_top].value)?1:0;
        RunStack_top--;
        pc++;
        } else if (operate == Bigger) {
            RunStack[RunStack_top - 1].value = (RunStack[RunStack_top - 1].value > RunStack[RunStack_top].value)?1:0;
            RunStack_top--;
            pc++;
        } else if (operate == SmallerOrEqual) {
            RunStack[RunStack_top - 1].value = (RunStack[RunStack_top - 1].value <= RunStack[RunStack_top].value)?1:0;
            RunStack_top--;
            pc++;
        }else if (operate == BiggerOrEqual) {
            RunStack[RunStack_top - 1].value = (RunStack[RunStack_top - 1].value >= RunStack[RunStack_top].value)?1:0;
            RunStack_top--;
            pc++;
        } else if (operate == Equal) {
            RunStack[RunStack_top - 1].value = (RunStack[RunStack_top - 1].value == RunStack[RunStack_top].value)?1:0;
            RunStack_top--;
            pc++;
        } else if (operate == NotEqual) {
            RunStack[RunStack_top - 1].value = (RunStack[RunStack_top - 1].value != RunStack[RunStack_top].value)?1:0;
            RunStack_top--;
            pc++;
        } else if (operate == And) {
            RunStack[RunStack_top - 1].value = (RunStack[RunStack_top - 1].value > 0  && RunStack[RunStack_top].value > 0)?1:0;
            RunStack_top--;
            pc++;
        } else if (operate == Or) {
            RunStack[RunStack_top - 1].value = (RunStack[RunStack_top - 1].value > 0 || RunStack[RunStack_top].value > 0)?1:0;
            RunStack_top--;
            pc++;
        } else if (operate == IFBEGIN) {
            SignList_top++;
            SignList[SignList_top].type = If;
            strcpy(SignList[SignList_top].Name,operateObj);
            RunStack_top++;
            RunStack[RunStack_top].type = If;
            strcpy(RunStack[RunStack_top].Name,operateObj);
            pc++;
        } else if (operate == IFEND) {
            while ( !(SignList[SignList_top].type == If && strcmp(SignList[SignList_top].Name,operateObj) == 0) ) SignList_top--;
            SignList_top--;
            while ( !(RunStack[RunStack_top].type == If && strcmp(RunStack[RunStack_top].Name,operateObj) == 0) ) RunStack_top--;
            RunStack_top--;
            pc++;
        } else if (operate == ELSE) {
            pc++;
        } else if (operate == IFBEZ) {
            if (RunStack[RunStack_top].value != 0){
                pc++;
            }else {
                int t = RunStack_top;
                while (RunStack[t].type != If)
                    t--;
                char if_temp[3000];
                strcpy(if_temp,RunStack[t].Name);
                while (!(Pcodes[pc].OperateName == ELSE && strcmp(Pcodes[pc].OperateObj,if_temp) == 0))
                    pc++;
            }
        } else if (operate == J_IFEND) {
            int t = RunStack_top;
            while (RunStack[t].type != If)
                t--;
            char if_temp[3000];
            strcpy(if_temp,RunStack[t].Name);
            while (!(Pcodes[pc].OperateName == IFEND && strcmp(Pcodes[pc].OperateObj,if_temp) == 0))
                pc++;
        } else if (operate == WHILEBEGIN) {
            SignList_top++;
            SignList[SignList_top].type = While;
            strcpy(SignList[SignList_top].Name,operateObj);
            RunStack_top++;
            RunStack[RunStack_top].type = While;
            strcpy(RunStack[RunStack_top].Name,operateObj);
            pc++;
        } else if (operate == WHILEEND) {
            while ( !(SignList[SignList_top].type == While && strcmp(SignList[SignList_top].Name,operateObj) == 0) ) SignList_top--;
            SignList_top--;
            while ( !(RunStack[RunStack_top].type == While && strcmp(RunStack[RunStack_top].Name,operateObj) == 0) ) RunStack_top--;
            RunStack_top--;
            pc++;
        } else if (operate == WHILEBEZ) {
            if (RunStack[RunStack_top].value != 0){
                pc++;
            }else {
                while (!(Pcodes[pc].OperateName == WHILEEND && strcmp(Pcodes[pc].OperateObj,operateObj) == 0))
                    pc++;
            }
        } else if (operate == WHILE_LOOP) {
            while ( !(SignList[SignList_top].type == While && strcmp(SignList[SignList_top].Name,operateObj) == 0) ) SignList_top--;
            SignList_top--;
            while ( !(RunStack[RunStack_top].type == While && strcmp(RunStack[RunStack_top].Name,operateObj) == 0) ) RunStack_top--;
            RunStack_top--;
            while (!(Pcodes[pc].OperateName == WHILEBEGIN && strcmp(Pcodes[pc].OperateObj,operateObj) == 0))
                pc--;
        } else if (operate == BREAK) {
            int t = RunStack_top;
            while (RunStack[t].type != While)
                t--;
            char while_temp[3000];
            strcpy(while_temp,RunStack[t].Name);
            while (!(Pcodes[pc].OperateName == WHILEEND && strcmp(Pcodes[pc].OperateObj,while_temp) == 0))
                pc++;
        } else if (operate == CONTINUE) {
            int t = RunStack_top;
            while (RunStack[t].type != While)
                t--;
            char while_temp[3000];
            strcpy(while_temp,RunStack[t].Name);
            while (!(Pcodes[pc].OperateName == WHILE_LOOP && strcmp(Pcodes[pc].OperateObj,while_temp) == 0)) {
                pc++;
            }
        }
        else {
            //不适配指令
            pc++;
        }
        operate = Pcodes[pc].OperateName;
        strcpy(operateObj, Pcodes[pc].OperateObj);
    }
    /*for (int i = 0;i <10000; i ++){
        if (!SignList[i].addr) free(SignList[i].addr);
        if (!RunStack[i].addr) free(RunStack[i].addr);

    }*/
}