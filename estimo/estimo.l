%{
/*
   Estimo Lexical Analyzer
   SharpEye Studio Project
   Copyright (C) George Vafiadis
   email: vafiadis@ieee.org
   web: http://sharpeye.borelspace.com
   Under GPL v2 License.
*/

#pragma warning(disable : 4996)

#include <cstdio>
#include <math.h>
#include <vector>
#include <string.h>

class Entity;

#include "estimo_yacc.h"

extern int isatty(int)
{return 0;}

extern YYSTYPE estimolval;
int lines = 0;

%}
%x COMMENT
%x ASM

%%
"if"        {return T_IF;}
"else"      {return T_ELSE;}
"while"     {return T_WHILE;}
"do"        {return T_DO;}
"for"       {return T_FOR;}
"to"        {return T_FORTO;}
"step"      {return T_FORSTEP;}
"=="        {return T_EQ;}
">"         {return T_GT;}
"<"         {return T_LT;}
"&&"        {return T_AND;}
"||"        {return T_OR;}
"<="        {return T_LE;}
">="        {return T_GE;}
"!="        {return T_NE;}
"!"         {return T_NOT;}
"="         {return T_SET;}
"->"        {return T_ARROW;}
"update"    {return T_UPDATE;}
"check"     {return T_CHECK;}
"Pattern"   {return T_PATTERN;}
"SAD"       {return T_SAD;}
"COST"      {return T_COST;}
"LENGTH"    {return T_LENGTH;}
"WINID"     {return T_WINID;}
"#if"       {return T_IFS;}
"#else"     {return T_ELSES;}
"#do"       {return T_DOS;}
"#while"    {return T_WHILES;}
"#break"    {return T_BREAK_SHARP;}
"break"     {return T_BREAK;}
"case"      {return T_CASE;}
":"         {return T_SEMICOL;}
"log2"      {return T_FUN_LOG2;}
"ln"        {return T_FUN_LN;}
"log10"     {return T_FUN_LOG10; }
"pow"       {return T_FUN_POW;}
"sqrt"      {return T_FUN_SQRT;}
"fabs"      {return T_FUN_FABS;}
"print"     {return T_FUN_PRINT;}
"mode"      {return T_MVC_MODE;}
"16x16"     {return T_MODE_16x16;}
"8x16"      {return T_MODE_8x16;}
"16x8"      {return T_MODE_16x8;}
"8x8"       {return T_MODE_8x8;}
 
".ASSEMBLY" {BEGIN ASM;return T_ASSEMBLY;}
<ASM>".points"   {return T_POINTS;}
<ASM>".def"      {return T_DEF;}
<ASM>".check"    {return T_CHK;}
<ASM>".begin"    {return T_BEGIN;}
<ASM>".end"      {BEGIN 0; return T_END;}
<ASM>"b"         {return T_BRUNCH;}
<ASM>"jmp"       {return T_JMP;}
<ASM>"halt"      {return T_HALT;}
<ASM>"chk"       {return T_CHK;}
<ASM>"cmpl"      {return T_LT; /* compare R < Num */}
<ASM>"cmpg"      {return T_GT; /* compare R > Num */}
<ASM>"cmpe"      {return T_EQ; /* compare R == Num */}
<ASM>"cmpne"     {return T_NE; /* compare R != Num */}
<ASM>"chkjmp"    {return T_CHKJMP;}
<ASM>"sad"       {return T_SAD;}
<ASM>"Mx"        {return T_MX;}
<ASM>"My"        {return T_MY;}
<ASM>";".*\n     {++lines;return T_ENDL;}
<ASM>[ \t]+      {}
<ASM>[A-Za-z][A-Za-z0-9]*       {strncpy(estimolval.str, yytext, 79); return T_ID;}
<ASM>[-+]?[0-9]+"."?[0-9]*      {estimolval.n = (float)atof(yytext); return T_NUM;}
<ASM>[\n]+                      {lines += strlen(yytext); return T_ENDL; }
<ASM>[-+*/{}().,:]             {return yytext[0];}
<ASM>.                          {}

[A-Za-z][A-Za-z0-9_]*       { strncpy(estimolval.str, yytext, 79); return T_ID;}
[0-9]+"."?[0-9]*            {estimolval.n = (float)atof(yytext); return T_NUM;}
"0x"[ABCDEF0-9]+            { const char * hexNum = yytext; int n; sscanf(hexNum, "%x", &n); estimolval.n = (float)n; return T_NUM;}

\"[A-Za-z0-9_.:+-=()\\@#$%^&*\t ]*\"  {strcpy(estimolval.str, yytext); return T_STRING;}

"/*"                  {BEGIN COMMENT;}
<COMMENT>\n   { ++lines; }
<COMMENT>\t   {}
<COMMENT>"*/" { BEGIN 0; }
<COMMENT>.     {}

"//".*\n             {++lines;}

[ \t]+                 {}
[\n]                   {++lines;}
[-+*/%{}().,;]           {return yytext[0];}
.                      {}
%%

/// When the input exhausted (end of file)
int yywrap()
{
 return 1;
}

