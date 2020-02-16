%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//include .h for functions

#include "functions.h"

extern char tipuriDate[150];
extern char argumente[150];

int valoarea=0;
int sum;
int in = 0;
int cautareId(char *);
void identificatorNedeclaratErr(char *);

int cautareFunctie(char *);
void fctNedeclaratErr(char *);

void afisIdentificatori();
void stocareTipDate(char*);
void stocareIdentificator(char*, char*, int);

void stocareFunctii(char*, char*);
int fctDuplicat(char*);

void stocareClase(char*);
int clasaDuplicat(char*);
void clasaDuplicatErr(char*);

void identificatorDuplicatErr(char*);
int asignareValida(char*);
int eDuplicat(char*);
void errAsignare(char*);
void fctDuplicatErr(char *);

void afisFunctii();
void stocareIdentificatorValue(char*, char*, int, int);


extern FILE* yyin;
extern int yylineno;
%}


// add these for full message of an error like: 
//  eg. unexpected '=' sy mbol
// otherwise it will show: Syntax error
%define parse.lac full 
%define parse.error verbose

%union{
        char* tipData;
        int integer;        
        float floatV;
        char charact;
        char* string;
}


%token  EVAL VAR CONST_VAR ASSIGN FUNC MODULE BOOL CONSTRUCTOR STATE HANDLERS  BOOL_OPERATION  COMP IF ELSE FOR WHILE NR_OPERATION PROGRAM


%token <floatV> FLOAT
%token <charact> CHAR
%token <string> STRING
%token <integer> INT

%token <charact> charValue
%token <integer> intValue
%token <floatV> floatValue
%token <string> stringValue
%token <tipData> TIP
%token  <string> ID

%type <string> declarationList
%type <string>varAssign
%type <string> functionDef
%type <string> moduleDef


%start s
%%

s: program  {  afisIdentificators();   printf("\nAccepted input\n");}
;
program:  program options
 | options 
 ;

options: varDeclaration    // {printf("VAR DECLARATION\n");}
       | functionDef  //{printf("FUNCTION DEFINITION\n");}
       | booleanExpression// {printf("BOOLEAN EXPRESSION\n");}
       | block // {printf("BLOCK\n");}
       | moduleDef //{printf("MODULE\n");}
       | varAssign ';'
       | eval ';'
       ;

varDeclaration: VAR declarationList ';'  
              | CONST_VAR constDeclarationList ';' 
              ;

declarationList: declarationList ',' ID ':' TIP  {
                                if(!eDuplicat($3)){                                            
                                       // stocareTipDate($5);
                                        stocareIdentificator($3, $5, 0);
                                       
                                }
                                else{
                                        identificatorDuplicatErr($3);
                                        //  exit(0);
                                }
                                                 }
               | declarationList ',' varAssign ':' TIP 
                {

                                  if(!eDuplicat($3)){                                            
                                        
                                        //stocareTipDate($3);
                                        stocareIdentificatorValue($3, $5, valoarea, 0);
                                           valoarea = 0;
                                        
                                }
                                else{
                                        identificatorDuplicatErr($3);
                                          exit(0);
                                }

                                    }

               | ID ':' TIP {

                                 if(!eDuplicat($1)){                                            
                                       
                                       
                                      //  stocareTipDate($3); 
                                        stocareIdentificator($1,$3, 0);
                                
                                       
                                }
                                else{
                                        identificatorDuplicatErr($1);
                                        
                                                                        }
                            }
               | varAssign ':' TIP {

                                  if(!eDuplicat($1)){                                            
                                        
                                        //stocareTipDate($3);
                                        stocareIdentificatorValue($1, $3, valoarea, 0);
                                           valoarea = 0;
                                        
                                }
                                else{
                                        identificatorDuplicatErr($1);
                                          exit(0);
                                }

                                    }
               ;

constDeclarationList: constDeclarationList ',' varAssign ':' TIP
                        {
                          if(!eDuplicat($3)){                                            
                                       // stocareTipDate($5);
                                        stocareIdentificatorValue($3, $5, valoarea, 1);
                                        valoarea = 0;
                                       
                                }
                                else{
                                        identificatorDuplicatErr($3);
                                        //  exit(0);
                                }
                        }
                    | varAssign ':' TIP
                         {
                          if(!eDuplicat($1)){                                            
                                       // stocareTipDate($5);
                                        stocareIdentificatorValue($1, $3, valoarea, 1);
                                        valoarea=0;
                                       
                                }
                                else{
                                        identificatorDuplicatErr($1);
                                        //  exit(0);
                                }
                        }
                    ;

moduleDef: MODULE ID '{' moduleConstructor moduleState  HANDLERS '{' moduleHandlers '}'  '}'
                {
                        if(!clasaDuplicat($2)){
                                stocareClase($2);
                        }
                        else
                        {
                                clasaDuplicatErr($2);
                        }
                }
         | MODULE ID '{' moduleConstructor moduleState  HANDLERS '{' '}'  '}'
                        {
                              if(!clasaDuplicat($2)){
                                stocareClase($2);
                                  }
                        else
                        {
                                clasaDuplicatErr($2);
                        }
                        }
         ;

moduleAccess: ID '.' ID
            | ID '.' functionCall
            ;

moduleConstructor: CONSTRUCTOR '(' listaParams ')' '{' commands '}'
                 | CONSTRUCTOR '(' ')' '{' commands '}'
                 | CONSTRUCTOR '(' ')' '{' '}'
                 ;

moduleState: STATE '{' varDeclaration '}'
           | STATE '{' '}'
           ;

moduleHandlers: moduleHandlers functionDef 
              | functionDef
              ;

varAssign: ID ASSIGN booleanExpression   
         | ID ASSIGN '$' functionCall         
         | ID ASSIGN '{' listaArgs '}'   
;
bool_char_string: BOOL 
                | CHAR
                | STRING
                ;

stringExpression:'!' STRING
                ;

arithmeticExpression: arithmeticExpression NR_OPERATION ID
                    | arithmeticExpression NR_OPERATION INT
                    | arithmeticExpression NR_OPERATION FLOAT
                    | arithmeticExpression NR_OPERATION functionCall
                    | arithmeticExpression NR_OPERATION moduleAccess
                    | '[' arithmeticExpression ']'
                    | ID 
                    | INT {valoarea=(int)$1;}
                    | FLOAT {valoarea=(int)$1;}
                    | moduleAccess
                    | functionCall //{printf("FUNCTIOON call\n");}
                    ;

booleanExpression: booleanExpression BOOL_OPERATION '!' ID
                 | booleanExpression BOOL_OPERATION bool_char_string
                 | booleanExpression BOOL_OPERATION arithmeticExpression
                 | booleanExpression BOOL_OPERATION stringExpression
                 | booleanExpression BOOL_OPERATION comparisonExpression
                 | bool_char_string
                 | arithmeticExpression
                 | comparisonExpression
                 | stringExpression
                 ;

comparisonExpression: arithmeticExpression COMP arithmeticExpression
                    | arithmeticExpression COMP bool_char_string
                    | bool_char_string COMP arithmeticExpression
                    | '[' comparisonExpression ']'
                    ;

functionCall: ID '(' listaArgs ')'
             | ID '(' ')'
             ;

functionDef: FUNC ID '(' listaParams ')' ':' TIP '{' commands '}'
              {        
                     // clear();
                     // afisFunctii();
                      if(!fctDuplicat($2)){
                       //   stocareTipDate($7);
                          stocareFunctii($2, $7);
                      }
                      else {
                        fctDuplicatErr($2);
                      }
              }
           ;

listaParams: listaParams ',' ID ':' TIP 
         | ID ':' TIP
         ;

listaArgs: listaArgs ',' booleanExpression
         | listaArgs ',' varDeclaration
         | booleanExpression
         | varDeclaration
         ; 

block: ifBlock //{printf("IF\n");}
     | forBlock //{printf("FOR\n");}
     | whileBlock //{printf("WHILE\n");}
     ;

ifBlock: IF '(' booleanExpression ')' '{' commands '}' 
       | IF '(' booleanExpression ')' '{' commands '}' ELSE '{' commands '}'
       | IF '(' booleanExpression ')' '{' commands '}' elseIfBlock ELSE '{' commands '}'
       ;

elseIfBlock: elseIfBlock ELSE IF '{' commands '}'
           | ELSE IF '{' commands '}'
           ;

forBlock: FOR '(' varAssign ';' booleanExpression ';' varAssign ')' '{' commands '}'
        ;

whileBlock: WHILE '(' booleanExpression ')' '{' commands '}'
          ;

commands: commands varDeclaration 
        | commands varAssign ';'
        | commands functionCall ';'
        | commands ifBlock
        | commands forBlock
        | commands whileBlock
        | varDeclaration
        | varAssign ';'
        | functionCall ';'
        ;       

eval: EVAL '(' booleanExpression ')'
    ;

%%
int yyerror(char * s){
 printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
 yyin=fopen(argv[1],"r");
 yyparse();
} 