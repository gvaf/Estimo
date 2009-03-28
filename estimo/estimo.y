%{
/*
   Estimo Parser Generator
   SharpEye Studio Project
   Copyright (C) George Vafiadis
   email: vafiadis@ieee.org
   web: http://gr.metamath.org/sharpeye
   Under GPL v2 License.
*/
 #include <stdio.h>
 #include <stdlib.h>
 #include <QList>
 #include <QDir>
 #include <QString>
 #include <QFileInfo>
 #include <QMessageBox>
 #include <iostream>
 #include "estimodule.h"

 #pragma warning( disable : 4273 )
 #pragma warning( disable : 4065 )
 #pragma warning( disable : 4996 )

  void yyerror( const char *s);
  extern int yylex();
  extern FILE *estimoin;
  extern int lines;
  QString inputFile;
  QString outPath;

  #define ZERO    new NumClass(0)
  #define ASSEMBLY_OUTPUT  "assembly.asm"
  #define PATTERNS_OUTPUT  "patterns.bin"
  #define PROGRAM_OUTPUT   "program.bin"
  #define ESTIMO_OUTPUT    "estimo.output"
%}

%union {
  float n;
  char str[80];
  Entity * esobject;
};

%token<str> T_ID T_STRING
%token<n>   T_NUM T_SET T_SAD T_MX T_MY T_ARROW T_WINID T_CASE T_SEMICOL 
%token<n>   T_FUN_LOG2 T_FUN_LN T_FUN_LOG10 T_FUN_POW T_FUN_SQRT T_FUN_FABS T_FUN_PRINT
%token<n>   T_IFS T_ELSES T_DOS T_WHILES
%token<n>   T_IF  T_ELSE T_WHILE T_DO T_FOR T_FORTO T_FORSTEP
%token<n>   T_EQ  T_GT T_LT T_GE T_LE T_NE
%token<n>   T_AND T_OR T_NOT T_ENDL
%token<n>   T_UPDATE T_CHECK T_PATTERN
%token<n>   T_ASSEMBLY T_POINTS T_DEF T_END T_BEGIN T_HALT
%token<n>   T_BRUNCH T_JMP T_CHK T_CMPL T_CMPG T_CMPE T_CMPNE T_CHKJMP T_BREAK T_BREAK_SHARP
%token<n>   T_MVC_MODE T_MODE_16x16 T_MODE_8x16 T_MODE_16x8 T_MODE_8x8
%nonassoc IFX 
%nonassoc T_ELSE T_ELSES
%left T_AND T_OR  
%left T_GE T_LE T_GT T_LT 
%left '-' '+'
%left '*' '/' '%'
%right T_NOT
%nonassoc UMINUS T_EQ T_NE T_SET

%type<esobject> expr stmt pattern asmcond check stmt_list  entity assembly  patternlist 
%type<esobject> checklist constcheck asmcommand lasmcmd asmcmd pointlist point asmbody checkcaselist
%type<n>  asmopr asmvar constnum endlist optendl partition

%start program
%%

program : 
            entity {
                     MemoryPool pool;
                    
                     try {
                        $1->eval(0);
                         Entity::doc.endDoc();
                         Entity::doc.dumpMemory(outPath     + "/" + PATTERNS_OUTPUT);
                         Entity::doc.outputAssembly(outPath + "/" + ASSEMBLY_OUTPUT, outPath   + "/" + PROGRAM_OUTPUT);                       
                         printf("Estimo Compiler. done!\n"); 
                      } catch(RunTimeException &ex)
                      {
                         if( ex.getType() == RunTimeException::BREAK_LOOP )
	                      {
	                         printf( "Error: 'Break' must be inside a loop!" );
	                      } else 
                         if( ex.getType() == RunTimeException::INVALID_WIN_COMPARE )
	                      {
	                         printf( "Error: You can only compare WINID for equality!" );
	                      }
                      };
                      
                      
                      
                   }  
          | assembly {
                      $1->eval(0);
                      Entity::doc.dumpMemory(outPath     + "/" + PATTERNS_OUTPUT);
                      Entity::doc.outputAssembly(outPath + "/" + ASSEMBLY_OUTPUT, outPath   + "/" + PROGRAM_OUTPUT);                       
                      MemoryPool pool;
                      printf("Estimo Assembly Compiler. done!\n");
                    }
           ;

assembly :
         T_ASSEMBLY  endlist
         T_POINTS endlist patternlist 
         T_BEGIN  endlist asmbody T_END optendl {$$ = $8;}
        ;

endlist : 
          T_ENDL
        | endlist T_ENDL
        ;

optendl : endlist
        | {};

asmbody : 
             lasmcmd endlist  {$$ = new AsmList($1);}
           | asmbody lasmcmd endlist  {AsmList * list = static_cast<AsmList *>($1);
                                       list->push_back($2); $$ = $1;
                                      }
           ;

lasmcmd :
         T_ID ':' asmcmd { AsmCommand *cmd = static_cast<AsmCommand *>($3);
                           cmd->setLabel($1); $$ = cmd; }
       | asmcmd          { $$ = $1; }
       | T_ID ':' endlist asmcmd  {AsmCommand *cmd = static_cast<AsmCommand *>($4);
                                   cmd->setLabel($1); $$ = cmd; }
       ;

asmcmd : 
         T_BRUNCH T_ID  { $$ = new AsmJmp(T_BRUNCH, $2);} 
       | T_HALT         { $$ = new AsmHalt(); }
       | T_JMP T_ID     { $$ = new AsmJmp(T_JMP, $2); } 
       | T_CHK T_ID     { $$ = new AsmChk($2); } 
       | T_CHKJMP constnum ',' T_ID { $$ =  0;} 
       | T_LT asmvar ',' constnum  { $$ = new AsmCmp(T_LT, $2, $4);} 
       | T_GT asmvar ',' constnum  { $$ = new AsmCmp(T_GT, $2, $4);} 
       | T_EQ asmvar ',' constnum  { $$ = new AsmCmp(T_EQ, $2, $4);}
       | T_NE asmvar ',' constnum  { $$ = new AsmCmp(T_NE, $2, $4);}  
       ;

patternlist: pattern
          | patternlist pattern
        ;

pattern : T_DEF T_ID endlist pointlist { new PatternClass($2, $4); }
          ;


pointlist:
            point endlist  {$$ = new CheckListClass(0, $1);}
          | pointlist point endlist {$$ = new CheckListClass($1, $2);}
          ;

point :
      T_CHK constnum ',' constnum  {$$ = new ConstCheckClass($2, $4); }      
      ;
                
constnum : T_NUM {$$ = $1;}
             | '+' T_NUM {$$ = $2;}
             | '-' T_NUM {$$ = - $2;}
             ;

entity : 
          entity stmt { $$ = new SeqClass($1, $2);}
        |  {$$ = 0; } 
        ;

stmt : 
         ';'                       {$$ = new SeqClass(0, 0);}
       | T_UPDATE ';'              {$$ = new UpdateClass();}
       | pattern                   {$$ = $1;}
       | check                                       {$$ = $1;}
       | T_CHECK '(' T_ID ')' ';'                    {$$ = new CheckJumpClass($3);}
       | T_CHECK '(' T_ID ')' '{' checkcaselist '}'  {$$ = new CheckJumpClass($3, $6);}
       | expr ';'                                  {$$ = $1;}
       | T_ID T_SET expr ';'                       {$$ = new AssignClass($1, $3);}
       | T_WHILE '(' expr ')' stmt                 {$$ = new WhileClass($3, $5);}
       | T_DO  stmt  T_WHILE '(' expr ')' ';'      {$$ = new DoWhileClass($5, $2);}
       | T_IF '(' expr ')' stmt %prec IFX          {$$ = new IFClass($3, $5);}
       | T_IF '(' expr ')' stmt T_ELSE stmt        {$$ = new IFClass($3, $5, $7);}
       | T_FOR '(' T_ID T_SET expr T_FORTO expr T_FORSTEP expr ')' stmt 
                                                         {$$ = new ForClass($3, $5, $7, $9, $11);}
       | '{' stmt_list '}'                               {$$ = $2;}
       | asmcommand                                      {$$ = $1;}
       | T_BREAK ';'                                     {$$ = new BreakClass(); }
       | T_BREAK_SHARP ';'                                {$$ = new BreakClass(true); }
       | T_FUN_PRINT     '(' expr ')' ';'              {$$ = new ExprClass(T_FUN_PRINT, $3);}
       | T_FUN_PRINT     '(' T_STRING ')' ';'          {$$ = new PrintClass($3); } 
       | T_MVC_MODE '(' expr  ',' expr ',' partition ')' ';' { $$ = new SetMVCmodeClass($3, $5, $7); }
       ;

partition : 
         T_MODE_16x16 { $$ = E_MODE_16x16; }  
       | T_MODE_8x16  { $$ = E_MODE_8x16; }
       | T_MODE_16x8  { $$ = E_MODE_16x8; }
       | T_MODE_8x8   { $$ = E_MODE_8x8; }
       ;
       
checkcaselist : T_CASE constnum T_SEMICOL stmt_list                 { $$ = new CasesListClass(0, $4, $2); }
              | checkcaselist T_CASE constnum T_SEMICOL stmt_list   { $$ = new CasesListClass($1, $5, $3); }
          ;
           
asmcommand : 
          T_IFS '(' asmcond ')' stmt %prec IFX        {$$ = new IFSharp($3, $5);}
        | T_IFS '(' asmcond ')' stmt T_ELSES stmt     {$$ = new IFSharp($3, $5, $7);}
        | T_WHILES '(' asmcond ')' stmt               {$$ = new WhileSharp($3, $5);}
        | T_DOS stmt T_WHILES '(' asmcond ')' ';'     {$$ = new DoWhileSharp($5, $2);}
        ;

asmcond : 
                asmvar asmopr constnum   {$$ = new AsmCondition($1, $2, $3);}
            |   constnum asmopr asmvar  {float inv = $2; 
                                                if($2 == T_GT) inv = T_LT;
                                                if($2 == T_GE) inv = T_LE;
                                                if($2 == T_LT) inv = T_GT;
                                                if($2 == T_LE) inv = T_GE;
                                                $$ = new AsmCondition($3, inv, $1);
                                               }
           ;

asmvar :
             T_SAD   {$$ = T_SAD;}
          |  T_WINID {$$ = T_WINID;}
          ;

asmopr : 
             T_GT    {$$ = T_GT;}
           | T_LT    {$$ = T_LT;} 
           | T_EQ    {$$ = T_EQ;}
           | T_GE    {$$ = T_GE;}
           | T_LE    {$$ = T_LE;}
           | T_NE    {$$ = T_NE;}
           ;
            
pattern : 
            T_PATTERN '(' T_ID ')' '{' checklist '}'  {$$ = new PatternClass($3, $6);} 
          ;

checklist :
           constcheck { $$ = new CheckListClass(0, $1); }
          | checklist constcheck  { $$ = new CheckListClass($1, $2); }
          ;

constcheck: 
       T_CHECK '(' constnum ',' constnum ')' {$$ = new ConstCheckClass($3, $5);}
      ;

check: 
       T_CHECK '(' expr ',' expr ')' ';' {$$ = new CheckJumpClass($3, $5);}     
     | T_CHECK '(' expr ',' expr ')' T_ARROW '{' stmt_list '}'  { $$ = new CheckJumpClass($3, $5, $9); }  
      ;

stmt_list : 
              stmt {$$ = $1;}
            | stmt_list stmt  {$$ = new SeqClass($1, $2);}  
            ;

expr :
         T_NUM                             {$$ = new NumClass($1);}
      |  T_ID                              {$$ = new VarClass($1);}
      | '-' expr %prec UMINUS              {$$ = new ExprClass('-', ZERO, $2);}
      | '+' expr %prec UMINUS              {$$ = new ExprClass('+', ZERO, $2);}
      | T_NOT expr %prec UMINUS            {$$ = new ExprClass(T_NOT, $2);}
      | expr '+' expr                      {$$ = new ExprClass('+', $1, $3); }
      | expr '-' expr                      {$$ = new ExprClass('-', $1, $3); }
      | expr '%' expr                      {$$ = new ExprClass('%', $1, $3); }
      | expr '*' expr                      {$$ = new ExprClass('*', $1, $3);}
      | expr '/' expr                      {$$ = new ExprClass('/', $1, $3);}
      | expr T_LT expr                     {$$ = new ExprClass(T_LT, $1, $3);}
      | expr T_GT expr                     {$$ = new ExprClass(T_GT, $1, $3);}
      | expr T_GE expr                     {$$ = new ExprClass(T_GE, $1, $3);}
      | expr T_LE expr                     {$$ = new ExprClass(T_LE, $1, $3);}
      | expr T_NE expr                     {$$ = new ExprClass(T_NE, $1, $3);}
      | expr T_EQ expr                     {$$ = new ExprClass(T_EQ, $1, $3);}
      | expr T_AND expr                    {$$ = new ExprClass(T_AND, $1, $3);}
      | expr T_OR expr                     {$$ = new ExprClass(T_OR, $1, $3);}
      | '(' expr ')'                       {$$ = $2;}
      | T_FUN_SQRT  '(' expr ')'            {$$ = new ExprClass(T_FUN_SQRT,  $3);}
      | T_FUN_LOG2  '(' expr ')'            {$$ = new ExprClass(T_FUN_LOG2,  $3);}
      | T_FUN_LN    '(' expr ')'            {$$ = new ExprClass(T_FUN_LN,    $3);}
      | T_FUN_LOG10 '(' expr ')'            {$$ = new ExprClass(T_FUN_LOG10, $3);}
      | T_FUN_POW   '(' expr ',' expr ')'   {$$ = new ExprClass(T_FUN_POW,   $3, $5);}
      ;
     
%%

/// Parse input file generate syntax tree
void parser(const char *input)
{
 inputFile = input;

 QFileInfo source(inputFile);

 if( !source.exists() )
 {
  QString err = QString("Error: Cannot locate file '%1'").arg(inputFile);
  std::cout << err.toStdString() << std::endl;
  return;
 }
 else
 {
   outPath = source.absolutePath() + "/" + ESTIMO_OUTPUT;                       
    
   QDir outdir (outPath);

   /// If output directory does not exist create it
   if( !outdir.exists() )
     outdir.mkdir(outPath);
 }

 /// Open file
 estimoin = fopen(input, "r");

 /// Call Bison
 estimoparse();

 /// Close file
 fclose(estimoin);
}

/// Error Handling routine
void yyerror( const char *s)
{
 printf("Error: %s at %d \n", s, lines);
}
