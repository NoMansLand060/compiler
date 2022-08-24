#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Pcode.h"

#define boolean int
#define true 1
#define false 0

FILE *fp1, *fp2,*fp3;

char c;										
char token[3000];
char symbol[3000];
int valid;
int Cnum;

int if_num = 0;
int while_num = 0;


void clearToken(){						
    memset(token, 0, sizeof(token));
}

boolean isSpace(){
    if (c == ' ') return true;
    else return false;
}

boolean isNewline(){
    if (c == '\n' || c == '\r') return true;
    else return false;

}

boolean isTab(){
    if (c == '\t') return true;
    else return false;
}

boolean isLetter(){
    if (c >= 'a' && c <= 'z') return true;
    if (c >= 'A' && c <= 'Z') return true;
    return false;
}

boolean isDigit(){
    if (c >= '0' && c <= '9') return true;
    else return false;
}

boolean isColon(){
    if (c == ':') return true;
    else return false;
}

boolean isComma(){
    if (c == ',') return true;
    else return false;
}

boolean isSemi(){
    if (c == ';') return true;
    else return false;
}

boolean isEqu(){
    if (c == '=') return true;
    else return false;
}

boolean isPlus(){
    if (c == '+') return true;
    else return false;
}

boolean isMinus(){
    if (c == '-') return true;
    else return false;
}

boolean isDivi(){
    if (c == '/') return true;
    else return false;
}

boolean isStar(){
    if (c == '*') return true;
    else return false;
}

boolean isLpar(){
    if (c == '(') return true;
    else return false;
}

boolean isRpar(){
    if (c == ')') return true;
    else return false;
}

boolean isLparM(){
    if (c == '[') return true;
    else return false;
}

boolean isRparM(){
    if (c == ']') return true;
    else return false;
}

boolean isLparB(){
    if (c == '{') return true;
    else return false;
}

boolean isRparB(){
    if (c == '}') return true;
    else return false;
}

boolean isNot(){
    if (c == '!') return true;
    else return false;
}

boolean isAnd(){
    if (c == '&') return true;
    else return false;
}

boolean isOr(){
    if (c == '|') return true;
    else return false;
}

boolean isMod(){
    if (c == '%') return true;
    else return false;
}

boolean isLess(){
    if (c == '<') return true;
    else return false;
}

boolean isMore(){
    if (c == '>') return true;
    else return false;
}

boolean isSTR(){
    if (c == '\"') return true;
    else return false;
}

boolean isXHX(){
    if (c == '_') return true;
    else return false;
}

void catToken(){
    token[strlen(token)] = c;
    token[strlen(token) + 1] = '\0';
}

void retract(FILE *fp1){
    fseek(fp1,-1,SEEK_CUR);
    Cnum--;
}

void reserver(){
    if (strcmp(token,"main")==0) strcpy(symbol,"MAINTK");
    else if (strcmp(token,"const")==0) strcpy(symbol,"CONSTTK");
    else if (strcmp(token,"int")==0) strcpy(symbol,"INTTK");
    else if (strcmp(token,"break")==0) strcpy(symbol,"BREAKTK");
    else if (strcmp(token,"continue")==0) strcpy(symbol,"CONTINUETK");
    else if (strcmp(token,"if")==0) strcpy(symbol,"IFTK");
    else if (strcmp(token,"else")==0) strcpy(symbol,"ELSETK");
    else if (strcmp(token,"while")==0) strcpy(symbol,"WHILETK");
    else if (strcmp(token,"getint")==0) strcpy(symbol,"GETINTTK");
    else if (strcmp(token,"printf")==0) strcpy(symbol,"PRINTFTK");
    else if (strcmp(token,"return")==0) strcpy(symbol,"RETURNTK");
    else if (strcmp(token,"void")==0) strcpy(symbol,"VOIDTK");
    else strcpy(symbol,"IDENFR");
    return;
}

void getC(FILE *fp1){
    c = fgetc(fp1);
    //printf("!! %c\n",c);
    Cnum++;
}

void error(){
    fprintf(fp2,"ERROR!!!!!!!!!!!!!!!\n");
}

void Getsym(FILE *fp1, FILE *fp2){
    clearToken();
    //Cnum = 0;
    do{
        getC(fp1);
    } while (isSpace()||isNewline()||isTab());
    if (isLetter()||isXHX()){
        while (isLetter()||isDigit()||isXHX()){
            catToken();
            getC(fp1);
        }
        retract(fp1);
        reserver();
    }
    else if (isDigit())
    {
        while (isDigit()){
            catToken();
            getC(fp1);
        }
        retract(fp1);
        strcpy(symbol,"INTCON");
    }
    else if (isNot()) {
        catToken();
        getC(fp1);
        if (isEqu()) {catToken(); strcpy(symbol,"NEQ");}
        else {
            retract(fp1);
            strcpy(symbol,"NOT");
        }
    }
    else if (isAnd()) {
        catToken();
        getC(fp1);
        if (isAnd()) {catToken(); strcpy(symbol,"AND");}
    }
    else if (isOr()) {
        catToken();
        getC(fp1);
        if (isOr()) {catToken(); strcpy(symbol,"OR");}
    }
    else if (isPlus()) {catToken(); strcpy(symbol,"PLUS");}
    else if (isMinus()) {catToken(); strcpy(symbol,"MINU");}
    else if (isStar()) {catToken(); strcpy(symbol,"MULT");}
    else if (isDivi()) {
        catToken();
        getC(fp1);
        if (isStar()){
            do{
                do{
                    getC(fp1);
                } while (!isStar());
                do{
                    getC(fp1);
                    if (isDivi()) {
                        valid = 0;
                        return;
                    }
                } while (isStar());
            } while (!isStar());
        }
        if (isDivi()){
            do{
                getC(fp1);
            } while (!isNewline());
            valid = 0;
            return;
        }
        retract(fp1);
        strcpy(symbol,"DIV");   //!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    else if (isMod()) {catToken(); strcpy(symbol,"MOD");}
    else if (isLess()) {
        catToken();
        getC(fp1);
        if (isEqu()) {catToken(); strcpy(symbol,"LEQ");}
        else {
            retract(fp1);
            strcpy(symbol,"LSS");
        }
    }
    else if (isMore()) {
        catToken();
        getC(fp1);
        if (isEqu()) {catToken(); strcpy(symbol,"GEQ");}
        else {
            retract(fp1);
            strcpy(symbol,"GRE");
        }
    }
    else if (isEqu()) {
        catToken();
        getC(fp1);
        if (isEqu()) {catToken(); strcpy(symbol,"EQL");}
        else {
            retract(fp1);
            strcpy(symbol,"ASSIGN");
        }
    }
    else if (isSemi()) {catToken(); strcpy(symbol,"SEMICN");}
    else if (isComma()) {catToken(); strcpy(symbol,"COMMA");}
    else if (isLpar()) {catToken(); strcpy(symbol,"LPARENT");}
    else if (isRpar()) {catToken(); strcpy(symbol,"RPARENT");}
    else if (isLparM()) {catToken(); strcpy(symbol,"LBRACK");}
    else if (isRparM()) {catToken(); strcpy(symbol,"RBRACK");}
    else if (isLparB()) {catToken(); strcpy(symbol,"LBRACE");}
    else if (isRparB()) {catToken(); strcpy(symbol,"RBRACE");}
    else if (isSTR()){
        catToken();
        do{
            getC(fp1);
            catToken();
        } while (!isSTR());
        strcpy(symbol,"STRCON");
    }
    else if (feof(fp1)) {
        //printf("!!!!!");
        valid = 0;
        return;
    }
    else {
        error();
        valid = 0;
        return;
    }
    valid = 1;
    return;
}

void CompUnit(); void Decl(); void ConstDecl(); void BType(); void ConstDef();
void ConstInitVal(); void VarDecl(); void VarDef(); void InitVal(); void FuncDef();
void MainFuncDef(); void FuncType(); void FuncFParams(); void FuncFParam(); void Block();
void BlockItem(); void Stmt(); void Exp(); void Cond(); void LVal(); void LVal_l();
void PrimaryExp(); void Number(); void UnaryExp(); void UnaryOp(); void FuncRParams();
void MulExp(); void AddExp(); void RelExp(); void EqExp(); void LAndExp();
void LOrExp(); void ConstExp(); void OtherExp();
void getsym(FILE *fp1,FILE*fp2);

boolean isDecl(char str1[], char str2[], char str3[]){
    if (strcmp(str1,"CONSTTK") == 0 && strcmp(str2,"INTTK") == 0 && strcmp(str3,"IDENFR") == 0)
        return true;
    else if (strcmp(str1,"INTTK") == 0 && strcmp(str2,"IDENFR") == 0 && (strcmp(str3,"LBRACK") == 0 || strcmp(str3,"ASSIGN") == 0 || strcmp(str3,"COMMA") == 0 || strcmp(str3,"SEMICN") == 0))
        return true;
    return false;
}

boolean isFuncDef(char str1[], char str2[], char str3[]){
    if ((strcmp(str1,"VOIDTK") == 0 || strcmp(str1,"INTTK") == 0) && strcmp(str2,"IDENFR") == 0 && strcmp(str3,"LPARENT") == 0)
        return true;
    return false;

}

boolean isMainFuncDef(char str1[], char str2[], char str3[]){
    if (strcmp(str1,"INTTK") == 0 && strcmp(str2,"MAINTK") == 0 && strcmp(str3,"LPARENT") == 0)
        return true;
    return false;
}

void CompUnit(){
    char str1[3000]; strcpy(str1,symbol);
    char strtoken[3000]; strcpy(strtoken,token);
    getsym(fp1,fp2);
    char str2[3000]; strcpy(str2,symbol);
    int len1 = Cnum;
    //printf("%d\n",len1);
    getsym(fp1,fp2);
    char str3[3000]; strcpy(str3,symbol);
    int len2 = Cnum;
    //printf("%d\n",len2);
    //printf("%d\n",len1+len2);
    fseek(fp1,-len1-len2,SEEK_CUR);
    strcpy(symbol, str1);
    strcpy(token, strtoken);
    //printf("!!%s %s %s\n",str1,str2,str3);
    while (isDecl(str1,str2,str3) == true) {
        Decl();
        strcpy(str1,symbol);
        strcpy(strtoken,token);
        getsym(fp1,fp2);
        strcpy(str2,symbol);
        int len1 = Cnum;
        //printf("%d\n",len1);
        getsym(fp1,fp2);
        strcpy(str3,symbol);
        int len2 = Cnum;
        //printf("%d\n",len2);
        //printf("%d\n",len1+len2-2);
        fseek(fp1,-len1-len2,SEEK_CUR);
        strcpy(symbol, str1);
        strcpy(token, strtoken);
    }
    while (isFuncDef(str1,str2,str3) == true) {
        FuncDef();
        strcpy(str1,symbol);
        strcpy(strtoken,token);
        getsym(fp1,fp2);
        strcpy(str2,symbol);
        int len1 = Cnum;
        //printf("%d\n",len1);
        getsym(fp1,fp2);
        strcpy(str3,symbol);
        int len2 = Cnum;
        //printf("%d\n",len2);
        //printf("%d\n",len1+len2-2);
        fseek(fp1,-len1-len2,SEEK_CUR);
        strcpy(symbol, str1);
        strcpy(token, strtoken);
    }
    //printf("!!\n");
    MainFuncDef();
    fprintf(fp2,"<CompUnit>\n");
    //printf("<CompUnit>\n");
}
// decl = const int ident | int ident
// funcdef = void/int ident (
// MainFuncdef = int main (

void Decl(){
    if (strcmp(token,"const") == 0)
        ConstDecl();
    else if (strcmp(token,"int") == 0) {
        //printf("!!\n");
        VarDecl();
    }
    //fprintf(fp2,"<Decl>\n");
}

void ConstDecl(){
    OtherExp(); // const
    BType();  // int
    ConstDef();
    while (strcmp(token,",") == 0){
        OtherExp();
        ConstDef();
    }
    OtherExp();
    fprintf(fp2,"<ConstDecl>\n");
    //printf("<ConstDecl>\n");
}

void BType(){
    OtherExp();
    //getsym(fp1,fp2);
    //fprintf(fp2,"<BType>\n");
}

void ConstDef(){
    char ident[3000];
    strcpy(ident,token);
    addPcode(CON,ident);
    //printf("debug->con\n");
    OtherExp(); //Ident
    int dimension = 0;
    while (strcmp(token,"[") == 0){
        dimension++;
        OtherExp();  //[
        ConstExp();
        OtherExp();  //]
    }
    if (dimension == 0) addPcode(ARRAY,"0");
    else if (dimension == 1) addPcode(ARRAY,"1");
    else if (dimension == 2) addPcode(ARRAY,"2");
    OtherExp();  // =
    //printf("%s %s\n",symbol,token);
    ConstInitVal();
    addPcode(INIT,ident);
    fprintf(fp2,"<ConstDef>\n");
    //printf("<ConstDef>\n");
}

void ConstInitVal(){
    if (strcmp(token,"{") == 0){
        OtherExp();
        if (strcmp(token,"}") == 0) {
            OtherExp();
        }
        else {
            ConstInitVal();
            while (strcmp(token,",") == 0) {
                OtherExp();
                ConstInitVal();
            }
            OtherExp();
        }
    }
    else {
        ConstExp();
    }

    fprintf(fp2,"<ConstInitVal>\n");
    //printf("<ConstInitVal>\n");
}

void VarDecl(){
    //printf("INTO BType\n");
    //printf("%s %s\n",symbol,token);
    BType();
    //printf("INTO VarDef\n");
    //printf("%s %s\n",symbol,token);
    VarDef();
    while (strcmp(token,",") == 0) {
        OtherExp();
        VarDef();
    }
    OtherExp();
    fprintf(fp2,"<VarDecl>\n");
    //printf("<VarDecl>\n");
}

void VarDef(){
    //printf("%s %s\n",symbol,token);
    char ident[3000];
    strcpy(ident,token);
    addPcode(VAR,ident);
    OtherExp(); //Ident
    int dimension = 0;
    //OtherExp();
    while (strcmp(token,"[") == 0){
        dimension++;
        OtherExp();
        ConstExp();
        OtherExp();
    }
    if (dimension == 0) addPcode(ARRAY,"0");
    else if (dimension == 1) addPcode(ARRAY,"1");
    else if (dimension == 2) addPcode(ARRAY,"2");
    if (strcmp(token,"=") == 0){
        OtherExp();
        InitVal();
        addPcode(INIT,ident);
    }
    //printf("<VarDef>\n");
}

void InitVal(){
    if (strcmp(token,"{") == 0) {
        OtherExp(); // {
        if (strcmp(token,"}") == 0) {
            OtherExp(); // }
        }
        else{
            InitVal();
            while (strcmp(token,",") == 0){
                OtherExp(); // ,
                InitVal();
            }
            OtherExp(); // }
        }
    }
    else {
        //////fprintf(fp2,"!!!!!!!InitVal\n");
        Exp();
    }
    fprintf(fp2,"<InitVal>\n");
    //printf("<InitVal>\n");
}

void FuncDef(){
    FuncType();
    addPcode(FUNC,token);
    OtherExp(); // ident
    OtherExp(); // (
    if (strcmp(token,")") == 0) {
        OtherExp();
    }
    else {
        FuncFParams();
        OtherExp();
        addPcode(ARGINIT,"");
    }
    Block();
    addPcode(RETURN,"");
    fprintf(fp2,"<FuncDef>\n");
    //printf("<FuncDef>\n");
}

void MainFuncDef(){
    //printf("%s %s\n",symbol,token);
    OtherExp(); // int
    //printf("%s %s\n",symbol,token);
    OtherExp(); // main
    //printf("%s %s\n",symbol,token);
    OtherExp(); // (
    //printf("%s %s\n",symbol,token);
    OtherExp(); // )
    addPcode(MAIN,"");
    Block();
    addPcode(MAINEND,"");
    fprintf(fp2,"<MainFuncDef>\n");
    //printf("<MainFuncDef>\n");
}

void FuncType(){
    OtherExp();
    fprintf(fp2,"<FuncType>\n");
    //printf("<FuncType>\n");
}

void FuncFParams(){
    FuncFParam();
    while (strcmp(token,",") == 0){
        OtherExp();
        FuncFParam();
    }
    fprintf(fp2,"<FuncFParams>\n");
    //printf("<FuncFParams>\n");
}

void FuncFParam(){
    BType(); // int
    addPcode(ARG, token);
    OtherExp();
    if (strcmp(token,"[") == 0){
        OtherExp();
        OtherExp();
        while (strcmp(token,"[") == 0){
            OtherExp();
            ConstExp();
            OtherExp();
            addPcode(POP,"");
        }
    }
    fprintf(fp2,"<FuncFParam>\n");
    //printf("<FuncFParam>\n");
}

void Block(){
    //printf("#####%s %s\n",symbol,token);
    addPcode(BST,"");
    OtherExp(); // {
    //printf("#####%s %s\n",symbol,token);
    while (strcmp(token,"}") != 0){
        BlockItem();
    }
    addPcode(BND,"");
    OtherExp();
    fprintf(fp2,"<Block>\n");
    //printf("<Block>\n");
}

void BlockItem(){
    if (strcmp(token,"const") == 0 || strcmp(token,"int") == 0){
        Decl();
    }
    else {
        Stmt();
    }
    //fprintf(fp2,"<BlockItem>\n");
}

void ITOA(int a, char *s){
    int t = 0;
    do{
        char c = a % 10 + 48;
        s[t++] = c;
        a = a / 10;
    } while (a != 0);
    s[t] = '\0';
}

void Stmt(){
    if (strcmp(token,"{") == 0){
        Block();
    }
    else if (strcmp(token,"if") == 0){
        OtherExp(); // if
        if_num++;
        char temp[3000];
        ITOA(if_num,temp);
        addPcode(IFBEGIN, temp);
        OtherExp(); // (
        Cond();
        addPcode(IFBEZ,temp);
        OtherExp(); // )
        Stmt();
        addPcode(J_IFEND,temp);
        addPcode(ELSE,temp);
        if (strcmp(token,"else") == 0){
            OtherExp();
            Stmt();
        }
        addPcode(IFEND,temp);
    }
    else if (strcmp(token,"while") == 0){
        //printf("while\n");
        OtherExp(); // while
        while_num ++;
        char temp[3000];
        ITOA(while_num,temp);
        addPcode(WHILEBEGIN,temp);
        OtherExp(); // (
        Cond();
        addPcode(WHILEBEZ,temp);
        //printf("%s %s\n",symbol,token);
        OtherExp(); //)
        Stmt();
        addPcode(WHILE_LOOP,temp);
        addPcode(WHILEEND,temp);
    }
    else if (strcmp(token,"break") == 0){
        OtherExp(); // break
        OtherExp(); // ;
        addPcode(BREAK,"");
    }
    else if (strcmp(token,"continue") == 0){
        OtherExp(); // continue
        OtherExp(); // ;
        addPcode(CONTINUE,"");
    }
    else if (strcmp(token,"return") == 0){
        OtherExp();
        if (strcmp(token,";") == 0) {
            addPcode(RETURN,"");
            OtherExp();
        }
        else {
            //////fprintf(fp2,"!!!!!!!Stmt\n");
            Exp();
            addPcode(RETURN,"TOP");
            OtherExp();
        }
    }
    else if (strcmp(token,"printf") == 0){
        OtherExp(); // printf
        OtherExp(); // (
        char FormatString[3000];
        strcpy(FormatString,token);
        OtherExp(); // FormatString
        while (strcmp(token,",") == 0) {
            OtherExp(); // ,
            //////fprintf(fp2,"!!!!!!!Stmtt\n");
            Exp();
        }
        addPcode(PRINT,FormatString);
        OtherExp();
        OtherExp();
    }
    else {
        int S=0;
        char strtoken[3000]; strcpy(strtoken,token);
        char strsymbol[3000]; strcpy(strsymbol,symbol);
        while (strcmp(token,"=") != 0 && strcmp(token,";") != 0){
            getsym(fp1,fp2);
            S = S + Cnum;
        }
        if (strcmp(token,"=") == 0) {
            fseek(fp1,-S,SEEK_CUR);
            strcpy(token,strtoken); strcpy(symbol,strsymbol);
            char ident[3000];
            strcpy(ident,token);
            LVal_l();
            //printf
            OtherExp();
            if (strcmp(token,"getint") == 0){
                OtherExp(); // getint
                OtherExp(); // (
                OtherExp(); // )
                OtherExp(); // ;
                //printf("((((((((((((((((\n");
                addPcode(GET,"");
            }
            else {
                ////fprintf(fp2,"!!!!!!!Stmttt\n");
                Exp();
                OtherExp();
            }
            addPcode(POP,ident);
        }
        else {
            fseek(fp1,-S,SEEK_CUR);
            strcpy(token,strtoken); strcpy(symbol,strsymbol);
            if (strcmp(token,";") == 0){
                OtherExp();
            }
            else {
                ////fprintf(fp2,"!!!!!!!Stmtttt\n");
                Exp();
                OtherExp();
            }
        }
    }
    fprintf(fp2,"<Stmt>\n");
    //printf("<Stmt>\n");
}

void Exp(){
    AddExp();
    fprintf(fp2,"<Exp>\n");
    //printf("<Exp>\n");
}

void Cond(){
    LOrExp();
    fprintf(fp2,"<Cond>\n");
    //printf("<Cond>\n");
}

void LVal(){
    //printf("!!@@%s %s\n",symbol,token);
    addPcode(PUSH,token);
    OtherExp(); // ident
    //printf("!!%s %s\n",symbol,token);
    while (strcmp(token,"[") == 0){
        OtherExp();
        ////fprintf(fp2,"!!!!!!!LVal\n");
        Exp();
        OtherExp();
    }
    addPcode(SELECT,"");
    fprintf(fp2,"<LVal>\n");
    //printf("<LVal>\n");
}

void LVal_l(){
    //printf("!!@@%s %s\n",symbol,token);
    OtherExp();
    //printf("!!%s %s\n",symbol,token);
    while (strcmp(token,"[") == 0){
        OtherExp();
        ////fprintf(fp2,"!!!!!!!LVal\n");
        Exp();
        OtherExp();
    }
    fprintf(fp2,"<LVal>\n");
    //printf("<LVal>\n");
}

void PrimaryExp(){
    if (strcmp(token,"(") == 0){
        OtherExp();
        ////fprintf(fp2,"!!!!!!!PrimaryExp\n");
        Exp();
        OtherExp();
    }
    else if (strcmp(symbol,"IDENFR") == 0){
        LVal();
    }
    else Number();
    fprintf(fp2,"<PrimaryExp>\n");
    //printf("<PrimaryExp>\n");
}

void Number(){
    addPcode(PUSH,token);
    OtherExp();
    fprintf(fp2,"<Number>\n");
    //printf("<Number>\n");
}

void UnaryExp(){
    //printf("@@@%s %s\n",symbol,token);
    if (strcmp(token,"(") == 0 || strcmp(symbol,"INTCON") == 0) {
        //printf("@$$####@!@$\n");
        PrimaryExp();
    }
    else if (strcmp(token,"+") == 0 || strcmp(token,"-") == 0 || strcmp(token,"!") == 0){
        char temp[3000];
        strcpy(temp,token);
        char sign[3000];
        strcpy(sign,token);
        UnaryOp();
        UnaryExp();
        if (strcmp(temp,"+") == 0) addPcode(POS,"");
        else  if (strcmp(temp,"-") == 0) addPcode(NEG,"");
        else  if (strcmp(temp,"!") == 0) addPcode(NOT,"");
    }
    else if (strcmp(symbol,"IDENFR") == 0){
        char strtoken[3000]; strcpy(strtoken,token);
        char strsymbol[3000]; strcpy(strsymbol,symbol);
        getsym(fp1,fp2);
        int S = Cnum;
        //printf("*****%d\n",S);
        char str2[3000]; strcpy(str2,token);
        fseek(fp1,-S,SEEK_CUR);
        strcpy(token,strtoken); strcpy(symbol,strsymbol);
        //printf("%s %s\n",symbol,token);
        if (strcmp(str2,"(") == 0){
            char ident[3000];
            strcpy(ident,token);
            OtherExp();  // ident
            OtherExp();  // (
            if (strcmp(token,")") == 0){
                OtherExp();
            }
            else {
                FuncRParams();
                OtherExp();
            }
            addPcode(JF,ident);
        }
        else {
            PrimaryExp();
        }
    }
    fprintf(fp2,"<UnaryExp>\n");
    //printf("<UnaryExp>\n");
}

void UnaryOp(){
    OtherExp();
    fprintf(fp2,"<UnaryOp>\n");
    //printf("<UnaryOp>\n");
}

void FuncRParams(){
    ////fprintf(fp2,"!!!!!!!FuncRParams\n");
    Exp();
    while (strcmp(token,",") == 0) {
        OtherExp();
        ////fprintf(fp2,"!!!!!!!FuncRParamss\n");
        Exp();
    }
    fprintf(fp2,"<FuncRParams>\n");
    //printf("<FuncRParams>\n");
}

/*void MulExp(){
    UnaryExp();
	MulExp1();
    fprintf(fp2,"<MulExp>\n");
}

void MulExp1(){
	if (strcmp(token,"*") == 0 || strcmp(token,"/") == 0 || strcmp(token,"%") == 0) {
		fprintf(fp2,"<MulExp>\n");
		OtherExp();
		UnaryExp();
		MulExp1();
	}
}*/

void MulExp(){
    UnaryExp();
    while ((strcmp(token,"*") == 0 || strcmp(token,"/") == 0 || strcmp(token,"%") == 0)){
        char sign[3000];
        strcpy(sign,token);
        OtherExp(); // * / %
        UnaryExp();
        fprintf(fp2,"<MulExp>\n");
        //("<MulExp>\n");
        if (strcmp(sign,"*") == 0) addPcode(MUL,"");
        else if (strcmp(sign,"/") == 0) addPcode(DIV,"");
        else if (strcmp(sign,"%") == 0) addPcode(MOD,"");
    }
}

/*void AddExp(){
    MulExp();
	AddExp1();
	fprintf(fp2,"<AddExp>\n");
}

void AddExp1(){
	if (strcmp(token,"+") == 0 || strcmp(token,"-") == 0) {
		fprintf(fp2,"<AddExp>\n");
		OtherExp();
		MulExp();
		AddExp1();
	}
}*/

void AddExp(){
    MulExp();
    while (strcmp(token,"+") == 0 || strcmp(token,"-") == 0) {
        char sign[3000];
        strcpy(sign,token);
        OtherExp(); // + -
        MulExp();
        fprintf(fp2,"<AddExp>\n");
        //printf("<AddExp>\n");
        if (strcmp(sign,"+") == 0) addPcode(ADD,"");
        else if (strcmp(sign,"-") == 0) addPcode(MIN,"");
    }
}

/*void RelExp(){
    AddExp();
	RelExp1();
	fprintf(fp2,"<RelExp>\n");
}

void RelExp1(){
    if (strcmp(token,"<") == 0 || strcmp(token,">") == 0 || strcmp(token,"<=") == 0 || strcmp(token,">=") == 0) {
		fprintf(fp2,"<RelExp>\n");
        OtherExp();
        AddExp();
		RelExp1();
    }
}*/

void RelExp(){
    AddExp();
    while (strcmp(token,"<") == 0 || strcmp(token,">") == 0 || strcmp(token,"<=") == 0 || strcmp(token,">=") == 0) {
        char sign[3000];
        strcpy(sign,token);
        OtherExp(); //
        AddExp();
        fprintf(fp2,"<RelExp>\n");
        //printf("<RelExp>\n");
        if (strcmp(sign,"<") == 0) addPcode(Smaller,"");
        else if (strcmp(sign,">") == 0) addPcode(Bigger,"");
        else if (strcmp(sign,"<=") == 0) addPcode(SmallerOrEqual,"");
        else if (strcmp(sign,">=") == 0) addPcode(BiggerOrEqual,"");
    }
}

/*void EqExp(){
    RelExp();
	EqExp1();
	fprintf(fp2,"<EqExp>\n");
}

void EqExp1(){
    if (strcmp(token,"==") == 0 || strcmp(token,"!=") == 0 ) {
		fprintf(fp2,"<EqExp>\n");
        OtherExp();
        RelExp();
		EqExp1();
    }
}*/

void EqExp(){
    RelExp();
    while (strcmp(token,"==") == 0 || strcmp(token,"!=") == 0 ) {
        char sign[3000];
        strcpy(sign,token);
        OtherExp(); // == !=
        RelExp();
        fprintf(fp2,"<EqExp>\n");
        //printf("<EqExp>\n");
        if (strcmp(sign,"==") == 0 ) addPcode(Equal,"");
        else if (strcmp(sign,"!=") == 0 ) addPcode(NotEqual,"");
    }
}

/*void LAndExp(){
    EqExp();
	LAndExp1();
	fprintf(fp2,"<LAndExp>\n");
}

void LAndExp1(){
    if (strcmp(token,"&&") == 0){
		fprintf(fp2,"<LAndExp>\n");
        OtherExp();
        EqExp();
		LAndExp1();
    }
}*/

void LAndExp(){
    EqExp();
    while (strcmp(token,"&&") == 0){
        OtherExp(); // &&
        EqExp();
        fprintf(fp2,"<LAndExp>\n");
        //printf("<LAndExp>\n");
        addPcode(And,"");
    }
}

/*void LOrExp(){
    LAndExp();
	LOrExp1();
	fprintf(fp2,"<LOrExp>\n");
}

void LOrExp1(){
    if (strcmp(token,"||") == 0){
		fprintf(fp2,"<LOrExp>\n");
        OtherExp();
        LAndExp();
		LOrExp1();
    }
}*/

void LOrExp(){
    LAndExp();
    while (strcmp(token,"||") == 0){
        OtherExp(); // ||
        LAndExp();
        fprintf(fp2,"<LOrExp>\n");
        //printf("<LOrExp>\n");
        addPcode(Or,"");
    }
}

void ConstExp(){
    AddExp();
    fprintf(fp2,"<ConstExp>\n");
    //printf("<ConstExp>\n");
}

void OtherExp(){
    //printf("!!@@%s %s\n",symbol,token);
    fprintf(fp2,"%s %s\n",symbol,token);
    //printf("%s %s\n",symbol,token);
    getsym(fp1,fp2);
    //printf("!!%s %s\n",symbol,token);
}

void getsym(FILE *fp1,FILE*fp2){
	Cnum = 0;
	do{
		Getsym(fp1,fp2);
        //printf("%s %s\n",token,symbol);
        //printf("%d %d\n",valid == 0,!feof(fp1));
	}
	while (valid == 0 && !feof(fp1));
    //printf("$$$$$\n");
	//printf("$$%s %s\n",symbol,token);
}


int main()
{
    fp1=fopen("testfile.txt","r");
    fp2=fopen("output.txt","w");
    fp3=fopen("pcoderesult.txt","w");
    //fprintf(fp2,"!!!\n");
    //printf("!!!");
    getsym(fp1,fp2);
    //fprintf(fp2,"!!!\n");
    CompUnit();
    //printf("!!!");
    OutPcodes();
    printf("!!!\n");
    Runner(fp3);
    //getchar();
    //getchar();
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    return 0;
}