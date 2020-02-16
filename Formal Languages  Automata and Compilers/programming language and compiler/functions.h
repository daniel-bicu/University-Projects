#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char tipuriDate[150];
char argumente[150];

int nrIdentificatori = 0;
int nrFct = 0;
int nrClase = 0;

struct Identificator
{
    int     valoare;
    char*   nume;
    char*   tipData;
    int     eConst;
    int     SCOPE;
}identificatori[250];

struct Functie
{
    int   inside; // inafara = 0 , inauntru = 1
    char* nume;
    char* tip;
    char* argumente;

}functii[250];

struct Clase{

    char *nume;

}clase[250];

extern int yylineno;

void afisFunctii()
{
     FILE *out = fopen("symbol_table.txt", "w");
    
    fprintf(out, " \n-- Functii -- \n");

    fprintf(out, " NUME  | TIP | ARGS \n");
    for(int i = 0 ; i<nrFct; ++i)
        {
            fprintf(out, "%s %s \n", functii[i].nume, functii[i].tip);
        }
}

void stocareTipDate(char * tipData){

    int i = 0 ;

    while(tipData[i] !='\0')
        tipuriDate[i] = tipData[i], i++;

}

char* preluareDate(){
    return tipuriDate;
}

int eDuplicat(char * identificator){

    for(int i = 0 ; i < nrIdentificatori; ++i){

        if( strcmp(identificator, identificatori[i].nume)==0)
                return 1;

    }

    return 0;
}

int fctDuplicat(char * numefct){

    for(int i = 0; i < nrFct; ++i){

         if( strcmp(numefct, functii[i].nume)==0)
                return 1;
    }
    return 0;
}

void stocareFunctii(char * name, char*fct_tipData)
{
     functii[nrFct].nume = name;
     functii[nrFct].tip  = fct_tipData;

    // printf("%s %s\n", functii[nrFct].tip , functii[nrFct].nume);
    // functii[nrFct].argumente = args;

    nrFct++;
  //  printf("%d", nrFct);
}

void stocareClase(char* name){

    clase[nrClase].nume= name;
    nrClase++;
}


int clasaDuplicat(char* nume)
{

    for(int i = 0; i < nrClase; ++i){

         if( strcmp(nume, clase[i].nume)==0)
                return 1;
    }
    return 0;
}

// check for duplicates


void stocareIdentificator(char * identificator, char* identificator_tipData, int constant){

    identificatori[nrIdentificatori].nume    = identificator;
    identificatori[nrIdentificatori].tipData = identificator_tipData;
    identificatori[nrIdentificatori].eConst  = constant;
   // identificatori[nrIdentificatori].SCOPE   = scop;

    //printf("%s %s \n", identificatori[nrIdentificatori].nume, identificatori[nrIdentificatori].tipData);

    nrIdentificatori++;
}


void stocareIdentificatorValue(char * identificator, char* identificator_tipData, int val,int constant){

    identificatori[nrIdentificatori].nume    = identificator;
    identificatori[nrIdentificatori].tipData = identificator_tipData;
    identificatori[nrIdentificatori].eConst  = constant;
    identificatori[nrIdentificatori].valoare = val;
   // identificatori[nrIdentificatori].SCOPE = scop;
    //printf("%s %s \n", identificatori[nrIdentificatori].nume, identificatori[nrIdentificatori].tipData);

    nrIdentificatori++;
}


void errAsignare(char* tipData){

    printf("\nErr on line %d: \nInvalid assignment! Expected '%s', but found %s \n", yylineno, tipuriDate, tipData);
    exit(0);

}

void identificatorDuplicatErr(char *identif){

    printf("\nErr on line %d: \nDuplicate identifier '%s'  found. \n", yylineno, identif);
    exit(0);
}


void fctDuplicatErr(char *fct){

    printf("\nErr on line %d: \nDuplicate function '%s'  found. \n", yylineno, fct);
    exit(0);
}

void clasaDuplicatErr(char *cls){
    printf("\nErr on line %d: \nDuplicate module '%s'  found. \n", yylineno, cls);
    exit(0);
}

int cautareId(char *nnumeID)
{
    printf("nrid: %d \n", nrIdentificatori);
    for(int i = 0; i <nrIdentificatori; ++i){

        if(identificatori[i].nume == nnumeID)
            return 1;
    }

    return 0;
}



int cautareFunctie(char *nnumeFCT)
{
  //  printf("nrid: %d \n", nrFct);
    for(int i = 0; i <nrFct; ++i){

        if(functii[i].nume == nnumeFCT)
            return 1;
    }

    return 0;
}


void identificatorNedeclaratErr(char *cls){
    printf("\nErr on line %d: \n The variable '%s' was not declared before. \n", yylineno, cls);
    exit(0);
}

void fctNedeclaratErr(char *cls){
    printf("\nErr on line %d: \n The function(action) '%s' was not declared before. \n", yylineno, cls);
    exit(0);
}


void afisIdentificators()
{ 
    FILE *out = fopen("symbol_table.txt", "w");
    
    fprintf(out," \n-- Variabile -- \n");
    fprintf(out,"Nr. total: %d\n\n", nrIdentificatori);
    fprintf(out, " NUME  | TIP | VALOARE | CONST \n");
      for(int i = 0 ; i<nrIdentificatori; ++i)
        {
            fprintf(out,"%s   %s   %d    ", identificatori[i].nume, identificatori[i].tipData, identificatori[i].valoare);

            if(identificatori[i].eConst==1)
                fprintf(out, "constant\n");
            else
                fprintf( out,"neconstant\n");
            
            // if(identificatori[i].SCOPE==1)
            //      fprintf(out, "global\n");
            // else
            //     fprintf( out,"local in functie\n");
        }

    fprintf( out," \n-- Functii -- \n");
    fprintf( out,"Nr. total: %d\n\n", nrFct);
    fprintf( out," NUME  | TIP \n");
  //  printf( "%d", nrFct);
    for(int j = 0 ; j<nrFct; ++j)
        {
            fprintf( out,"%s %s \n", functii[j].nume, functii[j].tip);
        }

    fprintf( out," \n-- Clase -- \n");
    fprintf( out,"Nr. total: %d\n\n", nrClase);
    fprintf( out," NUME  CLASA \n");
  //  printf( "%d", nrFct);
    for(int j = 0 ; j<nrClase; ++j)
        {
            fprintf( out,"%s \n", clase[j].nume);
        }

    fclose(out);

}
