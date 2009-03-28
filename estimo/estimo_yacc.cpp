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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse estimoparse
#define yylex   estimolex
#define yyerror estimoerror
#define yylval  estimolval
#define yychar  estimochar
#define yydebug estimodebug
#define yynerrs estimonerrs


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




/* Copy the first part of user declarations.  */
#line 1 "estimo.y"

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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 38 "estimo.y"
typedef union YYSTYPE {
  float n;
  char str[80];
  Entity * esobject;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 270 "estimo.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 282 "estimo.tab.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   877

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  80
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  26
/* YYNRULES -- Number of rules. */
#define YYNRULES  105
/* YYNRULES -- Number of states. */
#define YYNSTATES  279

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   322

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    71,     2,     2,
      76,    77,    69,    68,    74,    67,     2,    70,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    73,    75,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    78,     2,    79,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    72
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     7,    18,    20,    23,    25,    26,
      29,    33,    37,    39,    44,    47,    49,    52,    55,    60,
      65,    70,    75,    80,    82,    85,    90,    93,    97,   102,
     104,   107,   110,   113,   114,   116,   119,   121,   123,   129,
     137,   140,   145,   151,   159,   165,   173,   185,   189,   191,
     194,   197,   203,   209,   219,   221,   223,   225,   227,   232,
     238,   244,   252,   258,   266,   270,   274,   276,   278,   280,
     282,   284,   286,   288,   290,   298,   300,   303,   310,   318,
     329,   331,   334,   336,   338,   341,   344,   347,   351,   355,
     359,   363,   367,   371,   375,   379,   383,   387,   391,   395,
     399,   403,   408,   413,   418,   423
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      81,     0,    -1,    93,    -1,    82,    -1,    45,    83,    46,
      83,    88,    49,    83,    85,    48,    84,    -1,    41,    -1,
      83,    41,    -1,    83,    -1,    -1,    86,    83,    -1,    85,
      86,    83,    -1,     3,    73,    87,    -1,    87,    -1,     3,
      73,    83,    87,    -1,    51,     3,    -1,    50,    -1,    52,
       3,    -1,    53,     3,    -1,    58,    92,    74,     3,    -1,
      34,    99,    74,    92,    -1,    33,    99,    74,    92,    -1,
      32,    99,    74,    92,    -1,    37,    99,    74,    92,    -1,
      89,    -1,    88,    89,    -1,    47,     3,    83,    90,    -1,
      91,    83,    -1,    90,    91,    83,    -1,    53,    92,    74,
      92,    -1,     5,    -1,    68,     5,    -1,    67,     5,    -1,
      93,    94,    -1,    -1,    75,    -1,    42,    75,    -1,    89,
      -1,   103,    -1,    43,    76,     3,    77,    75,    -1,    43,
      76,     3,    77,    78,    96,    79,    -1,   105,    75,    -1,
       3,     6,   105,    75,    -1,    27,    76,   105,    77,    94,
      -1,    28,    94,    27,    76,   105,    77,    75,    -1,    25,
      76,   105,    77,    94,    -1,    25,    76,   105,    77,    94,
      26,    94,    -1,    29,    76,     3,     6,   105,    30,   105,
      31,   105,    77,    94,    -1,    78,   104,    79,    -1,    97,
      -1,    59,    75,    -1,    60,    75,    -1,    20,    76,   105,
      77,    75,    -1,    20,    76,     4,    77,    75,    -1,    61,
      76,   105,    74,   105,    74,    95,    77,    75,    -1,    62,
      -1,    63,    -1,    64,    -1,    65,    -1,    12,    92,    13,
     104,    -1,    96,    12,    92,    13,   104,    -1,    21,    76,
      98,    77,    94,    -1,    21,    76,    98,    77,    94,    22,
      94,    -1,    24,    76,    98,    77,    94,    -1,    23,    94,
      24,    76,    98,    77,    75,    -1,    99,   100,    92,    -1,
      92,   100,    99,    -1,     7,    -1,    11,    -1,    33,    -1,
      34,    -1,    32,    -1,    35,    -1,    36,    -1,    37,    -1,
      44,    76,     3,    77,    78,   101,    79,    -1,   102,    -1,
     101,   102,    -1,    43,    76,    92,    74,    92,    77,    -1,
      43,    76,   105,    74,   105,    77,    75,    -1,    43,    76,
     105,    74,   105,    77,    10,    78,   104,    79,    -1,    94,
      -1,   104,    94,    -1,     5,    -1,     3,    -1,    67,   105,
      -1,    68,   105,    -1,    40,   105,    -1,   105,    68,   105,
      -1,   105,    67,   105,    -1,   105,    71,   105,    -1,   105,
      69,   105,    -1,   105,    70,   105,    -1,   105,    34,   105,
      -1,   105,    33,   105,    -1,   105,    35,   105,    -1,   105,
      36,   105,    -1,   105,    37,   105,    -1,   105,    32,   105,
      -1,   105,    38,   105,    -1,   105,    39,   105,    -1,    76,
     105,    77,    -1,    18,    76,   105,    77,    -1,    14,    76,
     105,    77,    -1,    15,    76,   105,    77,    -1,    16,    76,
     105,    77,    -1,    17,    76,   105,    74,   105,    77,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    72,    72,    96,   106,   112,   113,   116,   117,   120,
     121,   127,   129,   130,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   146,   147,   150,   155,   156,   160,   163,
     164,   165,   169,   170,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   188,   189,   190,
     191,   192,   193,   194,   198,   199,   200,   201,   204,   205,
     209,   210,   211,   212,   216,   217,   227,   228,   232,   233,
     234,   235,   236,   237,   241,   245,   246,   250,   254,   255,
     259,   260,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_ID", "T_STRING", "T_NUM", "T_SET",
  "T_SAD", "T_MX", "T_MY", "T_ARROW", "T_WINID", "T_CASE", "T_SEMICOL",
  "T_FUN_LOG2", "T_FUN_LN", "T_FUN_LOG10", "T_FUN_POW", "T_FUN_SQRT",
  "T_FUN_FABS", "T_FUN_PRINT", "T_IFS", "T_ELSES", "T_DOS", "T_WHILES",
  "T_IF", "T_ELSE", "T_WHILE", "T_DO", "T_FOR", "T_FORTO", "T_FORSTEP",
  "T_EQ", "T_GT", "T_LT", "T_GE", "T_LE", "T_NE", "T_AND", "T_OR", "T_NOT",
  "T_ENDL", "T_UPDATE", "T_CHECK", "T_PATTERN", "T_ASSEMBLY", "T_POINTS",
  "T_DEF", "T_END", "T_BEGIN", "T_HALT", "T_BRUNCH", "T_JMP", "T_CHK",
  "T_CMPL", "T_CMPG", "T_CMPE", "T_CMPNE", "T_CHKJMP", "T_BREAK",
  "T_BREAK_SHARP", "T_MVC_MODE", "T_MODE_16x16", "T_MODE_8x16",
  "T_MODE_16x8", "T_MODE_8x8", "IFX", "'-'", "'+'", "'*'", "'/'", "'%'",
  "UMINUS", "':'", "','", "';'", "'('", "')'", "'{'", "'}'", "$accept",
  "program", "assembly", "endlist", "optendl", "asmbody", "lasmcmd",
  "asmcmd", "patternlist", "pattern", "pointlist", "point", "constnum",
  "entity", "stmt", "partition", "checkcaselist", "asmcommand", "asmcond",
  "asmvar", "asmopr", "checklist", "constcheck", "check", "stmt_list",
  "expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,    45,    43,    42,
      47,    37,   322,    58,    44,    59,    40,    41,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    80,    81,    81,    82,    83,    83,    84,    84,    85,
      85,    86,    86,    86,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    88,    88,    89,    90,    90,    91,    92,
      92,    92,    93,    93,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    95,    95,    95,    95,    96,    96,
      97,    97,    97,    97,    98,    98,    99,    99,   100,   100,
     100,   100,   100,   100,    89,   101,   101,   102,   103,   103,
     104,   104,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,    10,     1,     2,     1,     0,     2,
       3,     3,     1,     4,     2,     1,     2,     2,     4,     4,
       4,     4,     4,     1,     2,     4,     2,     3,     4,     1,
       2,     2,     2,     0,     1,     2,     1,     1,     5,     7,
       2,     4,     5,     7,     5,     7,    11,     3,     1,     2,
       2,     5,     5,     9,     1,     1,     1,     1,     4,     5,
       5,     7,     5,     7,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     7,     1,     2,     6,     7,    10,
       1,     2,     1,     1,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     4,     4,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
      33,     0,     0,     3,     2,     5,     0,     1,    83,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,     0,    36,    32,    48,    37,
       0,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    86,    35,
       0,     0,     0,    49,    50,     0,    84,    85,     0,    80,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    66,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,     0,
       0,     0,     0,   100,    47,    81,    97,    93,    92,    94,
      95,    96,    98,    99,    88,    87,    90,    91,    89,     0,
      23,    41,   102,   103,   104,     0,   101,     0,     0,    31,
      30,    70,    68,    69,    71,    72,    73,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,    24,     0,    52,    51,    65,    60,
      64,     0,    62,    44,    42,     0,     0,    38,     0,     0,
       0,     0,     0,    26,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,     0,    27,
       0,     0,     0,     0,     0,     0,    15,     0,     0,     0,
       0,     0,     0,    12,    61,    63,    45,    43,     0,     0,
       0,    39,     0,    78,     0,    74,    76,    28,    54,    55,
      56,    57,     0,     0,     0,     0,     0,     0,    14,    16,
      17,     0,     8,     0,     9,     0,     0,     0,     0,     0,
       0,     0,    11,     0,     0,     0,     0,     0,     7,     4,
      10,     0,    58,     0,     0,     0,    53,    13,    21,    20,
      19,    22,    18,     0,    59,    79,     0,    46,    77
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     2,     3,     6,   259,   211,   212,   213,   129,    36,
     160,   161,    99,     4,    69,   232,   193,    38,   100,   101,
     147,   196,   197,    39,    70,    40
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -216
static const short int yypact[] =
{
     -39,   -33,    14,  -216,   324,  -216,    55,  -216,     9,  -216,
     -60,   -47,   -37,   -36,    -1,    10,    18,   324,    27,    42,
      47,   324,    52,    62,    40,    58,    63,    34,    56,    76,
      74,    62,    62,  -216,    62,   324,  -216,  -216,  -216,  -216,
     643,  -216,   -33,    62,    62,    62,    62,    62,    62,    30,
      78,   113,    78,    62,    62,   125,   151,  -216,    75,  -216,
     306,   157,   -33,  -216,  -216,    62,    75,    75,   371,  -216,
     150,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,  -216,    46,   652,   382,   393,   439,
     661,   450,    85,   461,  -216,  -216,  -216,   167,   171,   170,
     103,   170,   105,   107,   507,   518,   110,   181,   111,   669,
     112,   -34,   712,  -216,  -216,  -216,    75,   222,   222,   222,
     222,    75,   779,   779,    72,    72,    79,    79,    79,    61,
    -216,  -216,  -216,  -216,  -216,    62,  -216,   116,   120,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,    81,   324,     6,
      78,   324,   324,   324,    62,    62,    39,    62,   118,     6,
     145,   -33,    62,   -33,  -216,   529,  -216,  -216,  -216,   177,
    -216,   123,  -216,   182,  -216,   575,   730,  -216,   201,   586,
     172,   140,   -33,   175,   720,   198,  -216,   324,   144,   324,
     146,    62,     6,    -8,    -7,   148,   -38,  -216,     6,   175,
      94,   147,    81,    81,    81,    81,  -216,   219,   224,   231,
       6,   328,   -33,  -216,  -216,  -216,  -216,  -216,   739,   223,
       6,  -216,   159,  -216,     6,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,   161,   785,   166,   173,   178,   179,  -216,  -216,
    -216,   183,   -33,   -33,   175,    62,   324,   228,   324,   188,
     169,   819,  -216,     6,     6,     6,     6,   242,   175,  -216,
     175,   597,   324,   324,   258,     6,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,   324,   324,  -216,   187,  -216,  -216
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -216,  -216,  -216,     0,  -216,  -216,    35,  -215,  -216,   -57,
    -216,    95,  -129,  -216,    -4,  -216,  -216,  -216,   -50,   -83,
     164,  -216,    70,  -216,  -164,   -22
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const short int yytable[] =
{
      37,    58,   103,   222,   220,   195,     1,    41,     5,    66,
      67,    94,    68,    51,     7,    43,    44,    55,   252,   159,
     170,    86,    87,    88,    89,    90,    91,    93,   130,    45,
     181,   104,   105,    57,    92,     9,   267,    62,   109,    46,
      47,   225,    85,   112,    10,    11,    12,    13,    14,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   111,   219,   168,    57,   115,     9,   223,   227,
      23,   221,   164,    97,    98,    48,    10,    11,    12,    13,
      14,   241,   262,    94,   264,    95,    49,    41,    95,    96,
      26,   247,    96,    27,    50,   249,    41,    31,    32,   274,
     171,    42,    23,    52,    71,    26,    34,    -1,    27,    76,
     163,    71,    -1,   165,   177,    59,    76,   178,    53,   234,
     235,   236,   237,    54,   268,   269,   270,   271,    56,    31,
      32,    63,   175,   176,    60,   179,   276,   102,    34,    61,
     184,    81,    82,    83,   169,    97,    98,   172,   173,   174,
      65,    64,   106,     8,   107,     9,   228,   229,   230,   231,
     110,   183,   137,   185,    10,    11,    12,    13,    14,   218,
      15,    16,   139,    17,    18,    19,   140,    20,    21,    22,
     148,   150,   199,   214,   151,   216,   154,   155,   156,   158,
      23,   166,    24,    25,    26,   167,   180,    27,   159,   187,
     188,   201,   141,   142,   143,   144,   145,   146,   189,    28,
      29,    30,   244,   192,   198,   195,    41,    31,    32,   215,
     233,   217,   238,   261,   224,    33,    34,   239,    35,   114,
     202,   203,   204,   251,   240,   205,   246,   248,   250,    41,
     253,   263,   258,   260,   266,   272,   243,   254,   206,   207,
     208,   209,   255,   256,    71,   182,   210,   257,   115,    76,
     115,     8,   265,     9,   278,   149,   226,     0,     0,   277,
     115,     0,    10,    11,    12,    13,    14,     0,    15,    16,
       0,    17,    18,    19,     0,    20,    21,    22,     0,    79,
      80,    81,    82,    83,     0,     0,     0,     0,    23,     0,
      24,    25,    26,     0,     0,    27,     0,     0,     0,   108,
       0,     9,     0,     0,     0,     0,     0,    28,    29,    30,
      10,    11,    12,    13,    14,    31,    32,     8,     0,     9,
       0,   201,     0,    33,    34,     0,    35,   275,    10,    11,
      12,    13,    14,     0,    15,    16,    23,    17,    18,    19,
       0,    20,    21,    22,     0,     0,     0,     0,     0,     0,
     202,   203,   204,     0,    23,   205,    24,    25,    26,     0,
       0,    27,     0,    31,    32,     0,   242,     0,   206,   207,
     208,   209,    34,    28,    29,    30,   210,     0,     0,     0,
       0,    31,    32,     0,     0,     0,     0,     0,     0,    33,
      34,     0,    35,    71,    72,    73,    74,    75,    76,    77,
      78,     0,     0,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,     0,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,     0,     0,     0,     0,    79,    80,
      81,    82,    83,     0,     0,     0,     0,     0,   113,    79,
      80,    81,    82,    83,     0,     0,     0,     0,     0,   132,
      79,    80,    81,    82,    83,     0,     0,     0,     0,     0,
     133,    71,    72,    73,    74,    75,    76,    77,    78,     0,
       0,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,     0,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,     0,     0,     0,     0,    79,    80,    81,    82,
      83,     0,     0,     0,     0,     0,   134,    79,    80,    81,
      82,    83,     0,     0,     0,     0,     0,   136,    79,    80,
      81,    82,    83,     0,     0,     0,     0,     0,   138,    71,
      72,    73,    74,    75,    76,    77,    78,     0,     0,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,     0,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
       0,     0,     0,     0,    79,    80,    81,    82,    83,     0,
       0,     0,     0,     0,   152,    79,    80,    81,    82,    83,
       0,     0,     0,     0,     0,   153,    79,    80,    81,    82,
      83,     0,     0,     0,     0,     0,   186,    71,    72,    73,
      74,    75,    76,    77,    78,     0,     0,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,     0,     0,    71,
      72,    73,    74,    75,    76,    77,    78,     0,     0,     0,
       0,     0,    79,    80,    81,    82,    83,     0,     0,     0,
       0,     0,   190,    79,    80,    81,    82,    83,     0,     0,
       0,     0,     0,   194,    79,    80,    81,    82,    83,     0,
       0,     0,     0,     0,   273,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    71,    72,    73,    74,    75,    76,    77,
      78,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,    81,    82,    83,     0,     0,     0,    84,    79,
      80,    81,    82,    83,     0,     0,     0,   131,    79,    80,
      81,    82,    83,     0,     0,   135,    79,    80,    81,    82,
      83,     0,     0,   157,    71,    72,    73,    74,    75,    76,
      77,    78,    71,    72,    73,    74,    75,    76,    77,    78,
     191,     0,    71,    72,    73,    74,    75,    76,    77,    78,
     245,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,     0,     0,   162,    79,    80,    81,
      82,    83,     0,     0,   200,     0,     0,    79,    80,    81,
      82,    83,     0,     0,     0,     0,    79,    80,    81,    82,
      83,    71,    72,    73,    74,    75,    76,   202,   203,   204,
       0,     0,   205,     0,     0,     0,     5,     0,     0,     0,
       0,     0,     0,     0,     0,   206,   207,   208,   209,     0,
       0,     0,     0,   210,     0,     0,    79,    80,    81,    82,
      83,   202,   203,   204,     0,     0,   205,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,   206,
     207,   208,   209,     0,     0,     0,     0,   210
};

static const short int yycheck[] =
{
       4,    23,    52,    10,    12,    43,    45,    41,    41,    31,
      32,     5,    34,    17,     0,     6,    76,    21,   233,    53,
     149,    43,    44,    45,    46,    47,    48,    49,    85,    76,
     159,    53,    54,     3,     4,     5,   251,     3,    60,    76,
      76,    79,    42,    65,    14,    15,    16,    17,    18,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    62,   192,   147,     3,    70,     5,    75,   198,
      40,    79,   129,    67,    68,    76,    14,    15,    16,    17,
      18,   210,   246,     5,   248,     7,    76,    41,     7,    11,
      44,   220,    11,    47,    76,   224,    41,    67,    68,   263,
     150,    46,    40,    76,    32,    44,    76,    32,    47,    37,
      49,    32,    37,   135,    75,    75,    37,    78,    76,   202,
     203,   204,   205,    76,   253,   254,   255,   256,    76,    67,
      68,    75,   154,   155,    76,   157,   265,    24,    76,    76,
     162,    69,    70,    71,   148,    67,    68,   151,   152,   153,
      76,    75,    27,     3,     3,     5,    62,    63,    64,    65,
       3,   161,    77,   163,    14,    15,    16,    17,    18,   191,
      20,    21,     5,    23,    24,    25,     5,    27,    28,    29,
      77,    76,   182,   187,    77,   189,    76,     6,    77,    77,
      40,    75,    42,    43,    44,    75,    78,    47,    53,    22,
      77,     3,    32,    33,    34,    35,    36,    37,    26,    59,
      60,    61,   212,    12,    74,    43,    41,    67,    68,    75,
      73,    75,     3,   245,    76,    75,    76,     3,    78,    79,
      32,    33,    34,   233,     3,    37,    13,    78,    77,    41,
      74,    13,   242,   243,    75,     3,   211,    74,    50,    51,
      52,    53,    74,    74,    32,   160,    58,    74,   262,    37,
     264,     3,    74,     5,    77,   101,   196,    -1,    -1,   273,
     274,    -1,    14,    15,    16,    17,    18,    -1,    20,    21,
      -1,    23,    24,    25,    -1,    27,    28,    29,    -1,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    -1,    40,    -1,
      42,    43,    44,    -1,    -1,    47,    -1,    -1,    -1,     3,
      -1,     5,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,
      14,    15,    16,    17,    18,    67,    68,     3,    -1,     5,
      -1,     3,    -1,    75,    76,    -1,    78,    79,    14,    15,
      16,    17,    18,    -1,    20,    21,    40,    23,    24,    25,
      -1,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    34,    -1,    40,    37,    42,    43,    44,    -1,
      -1,    47,    -1,    67,    68,    -1,    48,    -1,    50,    51,
      52,    53,    76,    59,    60,    61,    58,    -1,    -1,    -1,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    -1,    78,    32,    33,    34,    35,    36,    37,    38,
      39,    -1,    -1,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    -1,    -1,    77,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,    77,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      77,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      -1,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    77,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    77,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    -1,    -1,    77,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    77,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    77,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    77,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      -1,    -1,    77,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    77,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    77,    32,    33,    34,    35,    36,
      37,    38,    39,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    75,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    75,    67,    68,
      69,    70,    71,    -1,    -1,    74,    67,    68,    69,    70,
      71,    -1,    -1,    74,    32,    33,    34,    35,    36,    37,
      38,    39,    32,    33,    34,    35,    36,    37,    38,    39,
      30,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    67,
      68,    69,    70,    71,    -1,    -1,    74,    67,    68,    69,
      70,    71,    -1,    -1,    74,    -1,    -1,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    32,    33,    34,    35,    36,    37,    32,    33,    34,
      -1,    -1,    37,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    67,    68,    69,    70,
      71,    32,    33,    34,    -1,    -1,    37,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    58
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    45,    81,    82,    93,    41,    83,     0,     3,     5,
      14,    15,    16,    17,    18,    20,    21,    23,    24,    25,
      27,    28,    29,    40,    42,    43,    44,    47,    59,    60,
      61,    67,    68,    75,    76,    78,    89,    94,    97,   103,
     105,    41,    46,     6,    76,    76,    76,    76,    76,    76,
      76,    94,    76,    76,    76,    94,    76,     3,   105,    75,
      76,    76,     3,    75,    75,    76,   105,   105,   105,    94,
     104,    32,    33,    34,    35,    36,    37,    38,    39,    67,
      68,    69,    70,    71,    75,    83,   105,   105,   105,   105,
     105,   105,     4,   105,     5,     7,    11,    67,    68,    92,
      98,    99,    24,    98,   105,   105,    27,     3,     3,   105,
       3,    83,   105,    77,    79,    94,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,    88,
      89,    75,    77,    77,    77,    74,    77,    77,    77,     5,
       5,    32,    33,    34,    35,    36,    37,   100,    77,   100,
      76,    77,    77,    77,    76,     6,    77,    74,    77,    53,
      90,    91,    74,    49,    89,   105,    75,    75,    99,    94,
      92,    98,    94,    94,    94,   105,   105,    75,    78,   105,
      78,    92,    91,    83,   105,    83,    77,    22,    77,    26,
      77,    30,    12,    96,    77,    43,   101,   102,    74,    83,
      74,     3,    32,    33,    34,    37,    50,    51,    52,    53,
      58,    85,    86,    87,    94,    75,    94,    75,   105,    92,
      12,    79,    10,    75,    76,    79,   102,    92,    62,    63,
      64,    65,    95,    73,    99,    99,    99,    99,     3,     3,
       3,    92,    48,    86,    83,    31,    13,    92,    78,    92,
      77,    83,    87,    74,    74,    74,    74,    74,    83,    84,
      83,   105,   104,    13,   104,    74,    75,    87,    92,    92,
      92,    92,     3,    77,   104,    79,    92,    94,    77
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 72 "estimo.y"
    {
                     MemoryPool pool;
                    
                     try {
                        (yyvsp[0].esobject)->eval(0);
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
                      
                      
                      
                   ;}
    break;

  case 3:
#line 96 "estimo.y"
    {
                      (yyvsp[0].esobject)->eval(0);
                      Entity::doc.dumpMemory(outPath     + "/" + PATTERNS_OUTPUT);
                      Entity::doc.outputAssembly(outPath + "/" + ASSEMBLY_OUTPUT, outPath   + "/" + PROGRAM_OUTPUT);                       
                      MemoryPool pool;
                      printf("Estimo Assembly Compiler. done!\n");
                    ;}
    break;

  case 4:
#line 108 "estimo.y"
    {(yyval.esobject) = (yyvsp[-2].esobject);;}
    break;

  case 8:
#line 117 "estimo.y"
    {;}
    break;

  case 9:
#line 120 "estimo.y"
    {(yyval.esobject) = new AsmList((yyvsp[-1].esobject));;}
    break;

  case 10:
#line 121 "estimo.y"
    {AsmList * list = static_cast<AsmList *>((yyvsp[-2].esobject));
                                       list->push_back((yyvsp[-1].esobject)); (yyval.esobject) = (yyvsp[-2].esobject);
                                      ;}
    break;

  case 11:
#line 127 "estimo.y"
    { AsmCommand *cmd = static_cast<AsmCommand *>((yyvsp[0].esobject));
                           cmd->setLabel((yyvsp[-2].str)); (yyval.esobject) = cmd; ;}
    break;

  case 12:
#line 129 "estimo.y"
    { (yyval.esobject) = (yyvsp[0].esobject); ;}
    break;

  case 13:
#line 130 "estimo.y"
    {AsmCommand *cmd = static_cast<AsmCommand *>((yyvsp[0].esobject));
                                   cmd->setLabel((yyvsp[-3].str)); (yyval.esobject) = cmd; ;}
    break;

  case 14:
#line 135 "estimo.y"
    { (yyval.esobject) = new AsmJmp(T_BRUNCH, (yyvsp[0].str));;}
    break;

  case 15:
#line 136 "estimo.y"
    { (yyval.esobject) = new AsmHalt(); ;}
    break;

  case 16:
#line 137 "estimo.y"
    { (yyval.esobject) = new AsmJmp(T_JMP, (yyvsp[0].str)); ;}
    break;

  case 17:
#line 138 "estimo.y"
    { (yyval.esobject) = new AsmChk((yyvsp[0].str)); ;}
    break;

  case 18:
#line 139 "estimo.y"
    { (yyval.esobject) =  0;;}
    break;

  case 19:
#line 140 "estimo.y"
    { (yyval.esobject) = new AsmCmp(T_LT, (yyvsp[-2].n), (yyvsp[0].n));;}
    break;

  case 20:
#line 141 "estimo.y"
    { (yyval.esobject) = new AsmCmp(T_GT, (yyvsp[-2].n), (yyvsp[0].n));;}
    break;

  case 21:
#line 142 "estimo.y"
    { (yyval.esobject) = new AsmCmp(T_EQ, (yyvsp[-2].n), (yyvsp[0].n));;}
    break;

  case 22:
#line 143 "estimo.y"
    { (yyval.esobject) = new AsmCmp(T_NE, (yyvsp[-2].n), (yyvsp[0].n));;}
    break;

  case 25:
#line 150 "estimo.y"
    { new PatternClass((yyvsp[-2].str), (yyvsp[0].esobject)); ;}
    break;

  case 26:
#line 155 "estimo.y"
    {(yyval.esobject) = new CheckListClass(0, (yyvsp[-1].esobject));;}
    break;

  case 27:
#line 156 "estimo.y"
    {(yyval.esobject) = new CheckListClass((yyvsp[-2].esobject), (yyvsp[-1].esobject));;}
    break;

  case 28:
#line 160 "estimo.y"
    {(yyval.esobject) = new ConstCheckClass((yyvsp[-2].n), (yyvsp[0].n)); ;}
    break;

  case 29:
#line 163 "estimo.y"
    {(yyval.n) = (yyvsp[0].n);;}
    break;

  case 30:
#line 164 "estimo.y"
    {(yyval.n) = (yyvsp[0].n);;}
    break;

  case 31:
#line 165 "estimo.y"
    {(yyval.n) = - (yyvsp[0].n);;}
    break;

  case 32:
#line 169 "estimo.y"
    { (yyval.esobject) = new SeqClass((yyvsp[-1].esobject), (yyvsp[0].esobject));;}
    break;

  case 33:
#line 170 "estimo.y"
    {(yyval.esobject) = 0; ;}
    break;

  case 34:
#line 174 "estimo.y"
    {(yyval.esobject) = new SeqClass(0, 0);;}
    break;

  case 35:
#line 175 "estimo.y"
    {(yyval.esobject) = new UpdateClass();;}
    break;

  case 36:
#line 176 "estimo.y"
    {(yyval.esobject) = (yyvsp[0].esobject);;}
    break;

  case 37:
#line 177 "estimo.y"
    {(yyval.esobject) = (yyvsp[0].esobject);;}
    break;

  case 38:
#line 178 "estimo.y"
    {(yyval.esobject) = new CheckJumpClass((yyvsp[-2].str));;}
    break;

  case 39:
#line 179 "estimo.y"
    {(yyval.esobject) = new CheckJumpClass((yyvsp[-4].str), (yyvsp[-1].esobject));;}
    break;

  case 40:
#line 180 "estimo.y"
    {(yyval.esobject) = (yyvsp[-1].esobject);;}
    break;

  case 41:
#line 181 "estimo.y"
    {(yyval.esobject) = new AssignClass((yyvsp[-3].str), (yyvsp[-1].esobject));;}
    break;

  case 42:
#line 182 "estimo.y"
    {(yyval.esobject) = new WhileClass((yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 43:
#line 183 "estimo.y"
    {(yyval.esobject) = new DoWhileClass((yyvsp[-2].esobject), (yyvsp[-5].esobject));;}
    break;

  case 44:
#line 184 "estimo.y"
    {(yyval.esobject) = new IFClass((yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 45:
#line 185 "estimo.y"
    {(yyval.esobject) = new IFClass((yyvsp[-4].esobject), (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 46:
#line 187 "estimo.y"
    {(yyval.esobject) = new ForClass((yyvsp[-8].str), (yyvsp[-6].esobject), (yyvsp[-4].esobject), (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 47:
#line 188 "estimo.y"
    {(yyval.esobject) = (yyvsp[-1].esobject);;}
    break;

  case 48:
#line 189 "estimo.y"
    {(yyval.esobject) = (yyvsp[0].esobject);;}
    break;

  case 49:
#line 190 "estimo.y"
    {(yyval.esobject) = new BreakClass(); ;}
    break;

  case 50:
#line 191 "estimo.y"
    {(yyval.esobject) = new BreakClass(true); ;}
    break;

  case 51:
#line 192 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_FUN_PRINT, (yyvsp[-2].esobject));;}
    break;

  case 52:
#line 193 "estimo.y"
    {(yyval.esobject) = new PrintClass((yyvsp[-2].str)); ;}
    break;

  case 53:
#line 194 "estimo.y"
    { (yyval.esobject) = new SetMVCmodeClass((yyvsp[-6].esobject), (yyvsp[-4].esobject), (yyvsp[-2].n)); ;}
    break;

  case 54:
#line 198 "estimo.y"
    { (yyval.n) = E_MODE_16x16; ;}
    break;

  case 55:
#line 199 "estimo.y"
    { (yyval.n) = E_MODE_8x16; ;}
    break;

  case 56:
#line 200 "estimo.y"
    { (yyval.n) = E_MODE_16x8; ;}
    break;

  case 57:
#line 201 "estimo.y"
    { (yyval.n) = E_MODE_8x8; ;}
    break;

  case 58:
#line 204 "estimo.y"
    { (yyval.esobject) = new CasesListClass(0, (yyvsp[0].esobject), (yyvsp[-2].n)); ;}
    break;

  case 59:
#line 205 "estimo.y"
    { (yyval.esobject) = new CasesListClass((yyvsp[-4].esobject), (yyvsp[0].esobject), (yyvsp[-2].n)); ;}
    break;

  case 60:
#line 209 "estimo.y"
    {(yyval.esobject) = new IFSharp((yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 61:
#line 210 "estimo.y"
    {(yyval.esobject) = new IFSharp((yyvsp[-4].esobject), (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 62:
#line 211 "estimo.y"
    {(yyval.esobject) = new WhileSharp((yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 63:
#line 212 "estimo.y"
    {(yyval.esobject) = new DoWhileSharp((yyvsp[-2].esobject), (yyvsp[-5].esobject));;}
    break;

  case 64:
#line 216 "estimo.y"
    {(yyval.esobject) = new AsmCondition((yyvsp[-2].n), (yyvsp[-1].n), (yyvsp[0].n));;}
    break;

  case 65:
#line 217 "estimo.y"
    {float inv = (yyvsp[-1].n); 
                                                if((yyvsp[-1].n) == T_GT) inv = T_LT;
                                                if((yyvsp[-1].n) == T_GE) inv = T_LE;
                                                if((yyvsp[-1].n) == T_LT) inv = T_GT;
                                                if((yyvsp[-1].n) == T_LE) inv = T_GE;
                                                (yyval.esobject) = new AsmCondition((yyvsp[0].n), inv, (yyvsp[-2].n));
                                               ;}
    break;

  case 66:
#line 227 "estimo.y"
    {(yyval.n) = T_SAD;;}
    break;

  case 67:
#line 228 "estimo.y"
    {(yyval.n) = T_WINID;;}
    break;

  case 68:
#line 232 "estimo.y"
    {(yyval.n) = T_GT;;}
    break;

  case 69:
#line 233 "estimo.y"
    {(yyval.n) = T_LT;;}
    break;

  case 70:
#line 234 "estimo.y"
    {(yyval.n) = T_EQ;;}
    break;

  case 71:
#line 235 "estimo.y"
    {(yyval.n) = T_GE;;}
    break;

  case 72:
#line 236 "estimo.y"
    {(yyval.n) = T_LE;;}
    break;

  case 73:
#line 237 "estimo.y"
    {(yyval.n) = T_NE;;}
    break;

  case 74:
#line 241 "estimo.y"
    {(yyval.esobject) = new PatternClass((yyvsp[-4].str), (yyvsp[-1].esobject));;}
    break;

  case 75:
#line 245 "estimo.y"
    { (yyval.esobject) = new CheckListClass(0, (yyvsp[0].esobject)); ;}
    break;

  case 76:
#line 246 "estimo.y"
    { (yyval.esobject) = new CheckListClass((yyvsp[-1].esobject), (yyvsp[0].esobject)); ;}
    break;

  case 77:
#line 250 "estimo.y"
    {(yyval.esobject) = new ConstCheckClass((yyvsp[-3].n), (yyvsp[-1].n));;}
    break;

  case 78:
#line 254 "estimo.y"
    {(yyval.esobject) = new CheckJumpClass((yyvsp[-4].esobject), (yyvsp[-2].esobject));;}
    break;

  case 79:
#line 255 "estimo.y"
    { (yyval.esobject) = new CheckJumpClass((yyvsp[-7].esobject), (yyvsp[-5].esobject), (yyvsp[-1].esobject)); ;}
    break;

  case 80:
#line 259 "estimo.y"
    {(yyval.esobject) = (yyvsp[0].esobject);;}
    break;

  case 81:
#line 260 "estimo.y"
    {(yyval.esobject) = new SeqClass((yyvsp[-1].esobject), (yyvsp[0].esobject));;}
    break;

  case 82:
#line 264 "estimo.y"
    {(yyval.esobject) = new NumClass((yyvsp[0].n));;}
    break;

  case 83:
#line 265 "estimo.y"
    {(yyval.esobject) = new VarClass((yyvsp[0].str));;}
    break;

  case 84:
#line 266 "estimo.y"
    {(yyval.esobject) = new ExprClass('-', ZERO, (yyvsp[0].esobject));;}
    break;

  case 85:
#line 267 "estimo.y"
    {(yyval.esobject) = new ExprClass('+', ZERO, (yyvsp[0].esobject));;}
    break;

  case 86:
#line 268 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_NOT, (yyvsp[0].esobject));;}
    break;

  case 87:
#line 269 "estimo.y"
    {(yyval.esobject) = new ExprClass('+', (yyvsp[-2].esobject), (yyvsp[0].esobject)); ;}
    break;

  case 88:
#line 270 "estimo.y"
    {(yyval.esobject) = new ExprClass('-', (yyvsp[-2].esobject), (yyvsp[0].esobject)); ;}
    break;

  case 89:
#line 271 "estimo.y"
    {(yyval.esobject) = new ExprClass('%', (yyvsp[-2].esobject), (yyvsp[0].esobject)); ;}
    break;

  case 90:
#line 272 "estimo.y"
    {(yyval.esobject) = new ExprClass('*', (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 91:
#line 273 "estimo.y"
    {(yyval.esobject) = new ExprClass('/', (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 92:
#line 274 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_LT, (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 93:
#line 275 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_GT, (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 94:
#line 276 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_GE, (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 95:
#line 277 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_LE, (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 96:
#line 278 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_NE, (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 97:
#line 279 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_EQ, (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 98:
#line 280 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_AND, (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 99:
#line 281 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_OR, (yyvsp[-2].esobject), (yyvsp[0].esobject));;}
    break;

  case 100:
#line 282 "estimo.y"
    {(yyval.esobject) = (yyvsp[-1].esobject);;}
    break;

  case 101:
#line 283 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_FUN_SQRT,  (yyvsp[-1].esobject));;}
    break;

  case 102:
#line 284 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_FUN_LOG2,  (yyvsp[-1].esobject));;}
    break;

  case 103:
#line 285 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_FUN_LN,    (yyvsp[-1].esobject));;}
    break;

  case 104:
#line 286 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_FUN_LOG10, (yyvsp[-1].esobject));;}
    break;

  case 105:
#line 287 "estimo.y"
    {(yyval.esobject) = new ExprClass(T_FUN_POW,   (yyvsp[-3].esobject), (yyvsp[-1].esobject));;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 2159 "estimo.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 290 "estimo.y"


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

