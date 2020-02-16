/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    EVAL = 258,
    VAR = 259,
    CONST_VAR = 260,
    ASSIGN = 261,
    FUNC = 262,
    MODULE = 263,
    BOOL = 264,
    CONSTRUCTOR = 265,
    STATE = 266,
    HANDLERS = 267,
    BOOL_OPERATION = 268,
    COMP = 269,
    IF = 270,
    ELSE = 271,
    FOR = 272,
    WHILE = 273,
    NR_OPERATION = 274,
    PROGRAM = 275,
    FLOAT = 276,
    CHAR = 277,
    STRING = 278,
    INT = 279,
    charValue = 280,
    intValue = 281,
    floatValue = 282,
    stringValue = 283,
    TIP = 284,
    ID = 285
  };
#endif
/* Tokens.  */
#define EVAL 258
#define VAR 259
#define CONST_VAR 260
#define ASSIGN 261
#define FUNC 262
#define MODULE 263
#define BOOL 264
#define CONSTRUCTOR 265
#define STATE 266
#define HANDLERS 267
#define BOOL_OPERATION 268
#define COMP 269
#define IF 270
#define ELSE 271
#define FOR 272
#define WHILE 273
#define NR_OPERATION 274
#define PROGRAM 275
#define FLOAT 276
#define CHAR 277
#define STRING 278
#define INT 279
#define charValue 280
#define intValue 281
#define floatValue 282
#define stringValue 283
#define TIP 284
#define ID 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 54 "main.y" /* yacc.c:1909  */

        char* tipData;
        int integer;        
        float floatV;
        char charact;
        char* string;

#line 122 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
