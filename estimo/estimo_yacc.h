/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_ID = 258,
     T_STRING = 259,
     T_NUM = 260,
     T_SET = 261,
     T_SAD = 262,
     T_MX = 263,
     T_MY = 264,
     T_ARROW = 265,
     T_WINID = 266,
     T_CASE = 267,
     T_SEMICOL = 268,
     T_FUN_LOG2 = 269,
     T_FUN_LN = 270,
     T_FUN_LOG10 = 271,
     T_FUN_POW = 272,
     T_FUN_SQRT = 273,
     T_FUN_FABS = 274,
     T_FUN_PRINT = 275,
     T_IFS = 276,
     T_ELSES = 277,
     T_DOS = 278,
     T_WHILES = 279,
     T_IF = 280,
     T_ELSE = 281,
     T_WHILE = 282,
     T_DO = 283,
     T_FOR = 284,
     T_FORTO = 285,
     T_FORSTEP = 286,
     T_EQ = 287,
     T_GT = 288,
     T_LT = 289,
     T_GE = 290,
     T_LE = 291,
     T_NE = 292,
     T_AND = 293,
     T_OR = 294,
     T_NOT = 295,
     T_ENDL = 296,
     T_UPDATE = 297,
     T_CHECK = 298,
     T_PATTERN = 299,
     T_ASSEMBLY = 300,
     T_POINTS = 301,
     T_DEF = 302,
     T_END = 303,
     T_BEGIN = 304,
     T_HALT = 305,
     T_BRUNCH = 306,
     T_JMP = 307,
     T_CHK = 308,
     T_CMPL = 309,
     T_CMPG = 310,
     T_CMPE = 311,
     T_CMPNE = 312,
     T_CHKJMP = 313,
     T_BREAK = 314,
     T_BREAK_SHARP = 315,
     T_MVC_MODE = 316,
     T_MODE_16x16 = 317,
     T_MODE_8x16 = 318,
     T_MODE_16x8 = 319,
     T_MODE_8x8 = 320,
     IFX = 321,
     UMINUS = 322
   };
#endif
/* Tokens.  */
#define T_ID 258
#define T_STRING 259
#define T_NUM 260
#define T_SET 261
#define T_SAD 262
#define T_MX 263
#define T_MY 264
#define T_ARROW 265
#define T_WINID 266
#define T_CASE 267
#define T_SEMICOL 268
#define T_FUN_LOG2 269
#define T_FUN_LN 270
#define T_FUN_LOG10 271
#define T_FUN_POW 272
#define T_FUN_SQRT 273
#define T_FUN_FABS 274
#define T_FUN_PRINT 275
#define T_IFS 276
#define T_ELSES 277
#define T_DOS 278
#define T_WHILES 279
#define T_IF 280
#define T_ELSE 281
#define T_WHILE 282
#define T_DO 283
#define T_FOR 284
#define T_FORTO 285
#define T_FORSTEP 286
#define T_EQ 287
#define T_GT 288
#define T_LT 289
#define T_GE 290
#define T_LE 291
#define T_NE 292
#define T_AND 293
#define T_OR 294
#define T_NOT 295
#define T_ENDL 296
#define T_UPDATE 297
#define T_CHECK 298
#define T_PATTERN 299
#define T_ASSEMBLY 300
#define T_POINTS 301
#define T_DEF 302
#define T_END 303
#define T_BEGIN 304
#define T_HALT 305
#define T_BRUNCH 306
#define T_JMP 307
#define T_CHK 308
#define T_CMPL 309
#define T_CMPG 310
#define T_CMPE 311
#define T_CMPNE 312
#define T_CHKJMP 313
#define T_BREAK 314
#define T_BREAK_SHARP 315
#define T_MVC_MODE 316
#define T_MODE_16x16 317
#define T_MODE_8x16 318
#define T_MODE_16x8 319
#define T_MODE_8x8 320
#define IFX 321
#define UMINUS 322




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 38 "estimo.y"
typedef union YYSTYPE {
  float n;
  char str[80];
  Entity * esobject;
} YYSTYPE;
/* Line 1403 of yacc.c.  */
#line 178 "estimo.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE estimolval;



