/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "source/xcw_parser.y" /* yacc.c:339  */

#define YYSTYPE void*
#define INTSIZE 4
#define ToStr(k) ((string*)(k))
#define ToInt(k) ((int*)(k))
#define ToPtrnum(k) ((Ptr_num*)(k))

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<vector>

using namespace std;

void yyerror(const char *);
void yyerror(const string&);
extern int yylex();
extern int yyparse();

ostream &out = cout;       // 用于输出

int VAR_T_num = 0, VAR_t_num = 0, VAR_p_num = 0;       //这三个全局变量分别表示Eeyore中的原生变量、临时变量和函数参数的编号

struct Ptr_num{             // 用来传递参数，用IF_ptr_int表示传上来的是否为常量
    int ptr_int;
    string ptr_str;
    bool IF_ptr_int;
    Ptr_num(int p_int){
        ptr_int = p_int;
        IF_ptr_int = 1; 
    }
    Ptr_num(string p_str){
        ptr_str = p_str;
        IF_ptr_int = 0; 
    }
    Ptr_num(){}
};

struct Params_num{      //函数参数的元素类型，需要包括 INT型变量和INT型数组指针
    string para_str;       //变量名字
    string para_ir_name;    //在IR中，函数参数的名字
};


int DEEP;      //当前的深度
string IF_DEEP(){
    if(DEEP==0)
        return "";
    return "\t";
}



struct IDENT_scope{     //符号表元素
    string IDENT_name;
    string IDENT_num;          // 变量的值可变，因此用string存储
    int  IDENT_const_num;      // const常量直接用INT型数字表示其内容
    int Array_size;
    vector<Ptr_num>* IDENT_array;     // 指向数组头部的指针
    vector<int>* IDENT_dim_array;     // 用于存储数组的维度
    //vector<Params_num>* IDENT_func_param;     // 用一个数组存储所有的参数

    int IDENT_func_param_num;      //参数的个数
    int IDENT_deep;
    bool IDENT_if_const;
    string IR_name;          // 在Eeyore中的变量名

    bool IDENT_if_array;       //是否为数组变量
    bool IDENT_if_func;        //是否为函数变量
    bool IDENT_if_ret_int;    //返回是否为INT

    IDENT_scope(string name, int num, int deep, bool if_const){       //常量的构造函数
        IDENT_name = name;
        IDENT_const_num = num;
        IDENT_deep = deep;
        IDENT_if_const = if_const;
    }
    IDENT_scope(string name, string num, int deep, bool if_const){          //变量的构造函数
        IDENT_name = name;
        IDENT_num = num;
        IDENT_deep = deep;
        IDENT_if_const = if_const;
    }

    void Print_IDENT(){          // 输出所有变量，方便调试
        out << "Name = " << IDENT_name << endl;
        if(IDENT_if_const)
            out << "    Num = " << IDENT_const_num << endl;
        else
            out << "    Num = " << IDENT_num << endl;
        out << "    Deep = " << IDENT_deep << endl;
        out << "    IF_const = " << IDENT_if_const << endl;
        if(!IDENT_if_const)
            out << "    IR_name = " << IR_name << endl;
    }
};

vector<IDENT_scope> Scope;     //符号表

bool check_define(string str){       // 检查当前域中是否存在重复
    int i = Scope.size() - 1;
    if(i == -1)
        return true;
    while(Scope[i].IDENT_deep == DEEP && i >= 0){      //需要深度一致
        if(str == Scope[i].IDENT_name){
            return false;
        }
        i--;
    }
    return true;
}

IDENT_scope* find_define(string name){
    int i = Scope.size() - 1;
    if(i == -1)
        return nullptr;
    while(i >= 0){
        if(name == Scope[i].IDENT_name){
            return & Scope[i];
        }
        i--;
    }
    return nullptr;
}


//-----------------数组相关变量------------------------------
vector<int> Array_dim;        // 该数据结构用于数组声明时，从前到后，用于存放数组的各个维度
vector<Ptr_num> Array_LVal_dim;       //在右侧作为表达式的时候，用来存储数组对象的各个维度
int path_length = 1;
int Array_loc;
int Array_dest, old_Array_dest;    //old_Array_dest用来临时存之前的Array_dest
int Array_deep;
string Array_name;
//----------------------------------------------------------

//-----------------函数相关变量------------------------------


//----------------------------------------------------------


#line 210 "build/xcw_parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "xcw_parser.tab.h".  */
#ifndef YY_YY_BUILD_XCW_PARSER_TAB_H_INCLUDED
# define YY_YY_BUILD_XCW_PARSER_TAB_H_INCLUDED
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
    ADD = 258,
    SUB = 259,
    MUL = 260,
    DIV = 261,
    MOD = 262,
    IDENT = 263,
    LPAREN = 264,
    RPAREN = 265,
    LCURLY = 266,
    RCURLY = 267,
    LBRAC = 268,
    RBRAC = 269,
    INT = 270,
    CONST = 271,
    VOID = 272,
    LE = 273,
    LEQ = 274,
    GE = 275,
    GEQ = 276,
    EQ = 277,
    NEQ = 278,
    AND = 279,
    OR = 280,
    NOT = 281,
    IF = 282,
    ELSE = 283,
    WHILE = 284,
    BREAK = 285,
    CONT = 286,
    RETURN = 287,
    ASSIGN = 288,
    SEMI = 289,
    COMMA = 290,
    PERIOD = 291,
    NUMBER = 292
  };
#endif
/* Tokens.  */
#define ADD 258
#define SUB 259
#define MUL 260
#define DIV 261
#define MOD 262
#define IDENT 263
#define LPAREN 264
#define RPAREN 265
#define LCURLY 266
#define RCURLY 267
#define LBRAC 268
#define RBRAC 269
#define INT 270
#define CONST 271
#define VOID 272
#define LE 273
#define LEQ 274
#define GE 275
#define GEQ 276
#define EQ 277
#define NEQ 278
#define AND 279
#define OR 280
#define NOT 281
#define IF 282
#define ELSE 283
#define WHILE 284
#define BREAK 285
#define CONT 286
#define RETURN 287
#define ASSIGN 288
#define SEMI 289
#define COMMA 290
#define PERIOD 291
#define NUMBER 292

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_BUILD_XCW_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 335 "build/xcw_parser.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   111

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  131

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   160,   160,   161,   165,   166,   170,   171,   175,   179,
     180,   184,   200,   229,   199,   251,   255,   263,   269,   270,
     274,   290,   318,   354,   387,   353,   407,   410,   419,   420,
     424,   443,   442,   471,   476,   484,   494,   503,   507,   508,
     542,   579,   580,   614,   654,   697,   701,   730,   737,   746,
     766,   765,   842,   843,   847,   856,   871,   875,   855,   892,
     906,   910,   891,   926,   929,   933,   937,   953,   971,   976,
     982,   989,   988,  1006,  1007,  1010,  1014,  1018,  1025,  1029,
    1041,  1048,  1051,  1062
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ADD", "SUB", "MUL", "DIV", "MOD",
  "IDENT", "LPAREN", "RPAREN", "LCURLY", "RCURLY", "LBRAC", "RBRAC", "INT",
  "CONST", "VOID", "LE", "LEQ", "GE", "GEQ", "EQ", "NEQ", "AND", "OR",
  "NOT", "IF", "ELSE", "WHILE", "BREAK", "CONT", "RETURN", "ASSIGN",
  "SEMI", "COMMA", "PERIOD", "NUMBER", "$accept", "CompUnit", "CompDecl",
  "Decl", "ConstDecl", "ConstDefs", "ConstDef", "$@1", "$@2",
  "ConstInitVal", "ConstExp", "VarDecl", "VarDefs", "VarDef", "$@3", "$@4",
  "ArrayInit", "ArrayExps", "ArrayExp", "$@5", "ArrayDef", "ArrayUnit",
  "InitVal", "Exp", "AddExp", "MulExp", "UnaryExp", "PrimaryExp", "LVal",
  "$@6", "ArrayLVals", "ArrayLVal", "FuncDef", "$@7", "$@8", "$@9", "$@10",
  "$@11", "$@12", "FuncFParams", "FuncFParam", "ArrayParamDef", "Block",
  "$@13", "BlockItems", "BlockItem", "Stmt", "FuncRParams", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292
};
# endif

#define YYPACT_NINF -100

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-100)))

#define YYTABLE_NINF -56

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      40,    31,     6,    51,    45,  -100,  -100,  -100,  -100,  -100,
       7,    15,  -100,    65,  -100,  -100,  -100,     5,     5,     4,
    -100,    58,  -100,    69,    13,    18,  -100,    67,    38,  -100,
      64,    68,    59,  -100,  -100,  -100,  -100,  -100,    68,  -100,
    -100,  -100,    21,  -100,     5,    25,  -100,    65,  -100,     5,
      66,  -100,     5,     5,     5,     5,     5,    70,    71,  -100,
    -100,  -100,  -100,    71,  -100,    -8,     5,    66,  -100,    59,
      59,  -100,  -100,  -100,  -100,    72,     8,  -100,    73,     9,
    -100,     5,    74,  -100,    -1,    77,  -100,    71,  -100,  -100,
    -100,  -100,  -100,    75,    47,  -100,  -100,    78,    80,  -100,
      -1,    80,    -1,  -100,    -1,    81,  -100,  -100,    83,  -100,
      86,  -100,  -100,    81,    -4,  -100,  -100,  -100,    69,    -2,
    -100,    49,    -7,  -100,  -100,  -100,    76,  -100,  -100,  -100,
    -100
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,     6,     7,     5,
      20,     0,    18,     0,    59,     1,     3,     0,     0,    22,
      33,     0,    17,     0,     0,     0,     9,     0,    49,    47,
       0,    16,    38,    41,    45,    48,    21,    36,    37,    23,
      34,    56,    20,    19,     0,     0,     8,     0,    60,    81,
       0,    35,     0,     0,     0,     0,     0,     0,    63,    11,
      15,    12,    10,    63,    82,     0,     0,    51,    52,    39,
      40,    42,    43,    44,    24,     0,     0,    64,     0,     0,
      46,     0,     0,    53,    26,    66,    57,     0,    13,    61,
      83,    54,    31,     0,    27,    28,    30,     0,     0,    65,
      26,     0,    26,    25,     0,    70,    71,    58,     0,    62,
       0,    29,    68,    67,    75,    14,    32,    69,     0,     0,
      76,     0,     0,    73,    77,    78,     0,    80,    72,    74,
      79
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -100,  -100,    89,   -99,  -100,  -100,    52,  -100,  -100,  -100,
      41,  -100,  -100,    79,  -100,  -100,   -78,  -100,     1,  -100,
      82,   -16,  -100,   -18,    -3,    22,    14,  -100,  -100,  -100,
    -100,    33,  -100,  -100,  -100,  -100,  -100,  -100,  -100,    44,
      16,  -100,    10,  -100,  -100,   -14,  -100,  -100
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    25,    26,    78,   100,    59,
      30,     8,    11,    12,    57,    84,    93,    94,    95,   102,
      19,    20,    36,    96,    38,    32,    33,    34,    35,    50,
      67,    68,     9,    21,    58,    98,    27,    63,   101,    76,
      77,   113,   107,   114,   122,   123,   124,    65
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      37,    28,    80,    40,    28,   128,    28,    28,   118,     2,
      92,   118,     2,    28,    31,   120,   -55,    17,    86,    89,
      17,    13,   108,   120,   110,   119,    17,    81,   119,    40,
      29,    64,   125,    29,    17,    29,    29,    39,    17,    10,
      18,    31,    29,    87,    87,    15,    44,    49,    82,    22,
      23,   -50,    46,    47,    18,     1,     2,     3,    61,    14,
       1,     2,     3,    90,    54,    55,    56,    41,    71,    72,
      73,    52,    53,    24,    69,    70,    48,    42,    51,    66,
      85,    74,   104,   127,    88,    60,    75,   103,    91,   112,
      97,   106,   105,    16,    17,   115,   121,   117,   116,    62,
      83,   126,    43,    99,   121,   111,    45,    79,   129,     0,
     130,   109
};

static const yytype_int8 yycheck[] =
{
      18,     8,    10,    19,     8,    12,     8,     8,    15,    16,
      11,    15,    16,     8,    17,   114,     9,    13,    10,    10,
      13,    15,   100,   122,   102,    32,    13,    35,    32,    45,
      37,    49,    34,    37,    13,    37,    37,    33,    13,     8,
      33,    44,    37,    35,    35,     0,    33,     9,    66,    34,
      35,    13,    34,    35,    33,    15,    16,    17,    33,     8,
      15,    16,    17,    81,     5,     6,     7,     9,    54,    55,
      56,     3,     4,     8,    52,    53,     9,     8,    14,    13,
       8,    11,    35,    34,    11,    44,    15,    12,    14,   105,
      13,    11,    14,     4,    13,    12,   114,   113,    12,    47,
      67,   119,    23,    87,   122,   104,    24,    63,   122,    -1,
      34,   101
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    15,    16,    17,    39,    40,    41,    42,    49,    70,
       8,    50,    51,    15,     8,     0,    40,    13,    33,    58,
      59,    71,    34,    35,     8,    43,    44,    74,     8,    37,
      48,    62,    63,    64,    65,    66,    60,    61,    62,    33,
      59,     9,     8,    51,    33,    58,    34,    35,     9,     9,
      67,    14,     3,     4,     5,     6,     7,    52,    72,    47,
      48,    33,    44,    75,    61,    85,    13,    68,    69,    63,
      63,    64,    64,    64,    11,    15,    77,    78,    45,    77,
      10,    35,    61,    69,    53,     8,    10,    35,    11,    10,
      61,    14,    11,    54,    55,    56,    61,    13,    73,    78,
      46,    76,    57,    12,    35,    14,    11,    80,    54,    80,
      54,    56,    59,    79,    81,    12,    12,    59,    15,    32,
      41,    61,    82,    83,    84,    34,    61,    34,    12,    83,
      34
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    38,    39,    39,    40,    40,    41,    41,    42,    43,
      43,    44,    45,    46,    44,    47,    48,    49,    50,    50,
      51,    51,    51,    52,    53,    51,    54,    54,    55,    55,
      56,    57,    56,    58,    58,    59,    60,    61,    62,    62,
      62,    63,    63,    63,    63,    64,    64,    65,    65,    66,
      67,    66,    68,    68,    69,    71,    72,    73,    70,    74,
      75,    76,    70,    77,    77,    77,    78,    78,    79,    79,
      79,    81,    80,    82,    82,    83,    83,    83,    84,    84,
      84,    85,    85,    85
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     4,     1,
       3,     3,     0,     0,     8,     1,     1,     3,     1,     3,
       1,     3,     2,     0,     0,     8,     0,     1,     1,     3,
       1,     0,     4,     1,     2,     3,     1,     1,     1,     3,
       3,     1,     3,     3,     3,     1,     4,     1,     1,     1,
       0,     3,     1,     2,     3,     0,     0,     0,     9,     0,
       0,     0,     9,     0,     1,     3,     2,     5,     1,     2,
       0,     0,     4,     1,     2,     0,     1,     1,     2,     3,
       2,     0,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
#ifndef YYINITDEPTH
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
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
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
      YYSIZE_T yyn = 0;
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

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
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 11:
#line 185 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        int num = *ToInt((yyvsp[0]));
        IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[-2])), num, DEEP, 1);
        //tmp.Print_IDENT();
        if(check_define(*ToStr((yyvsp[-2])))){       //如果在当前域中未被定义过
            //out << "11111" << endl;
            Scope.push_back(tmp);
            //VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr((yyvsp[-2])) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
#line 1523 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 200 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        // 因为常量数组的下标仍有可能是变量，因此考虑将其用变量数组的形式做
        Array_deep = 0;    //将深度初始化为0
        Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
        old_Array_dest = 0;
        path_length = ToPtrnum((yyvsp[-1]))->ptr_int;
        //首先检查当前域中是否出现
        if(!check_define(*ToStr((yyvsp[-2])))){
            string err = "\"" +  *ToStr((yyvsp[-2])) + "\" already defined in this scope.";
            yyerror(err);
        }
        int n = ToPtrnum((yyvsp[-1]))->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        // 输出 var 24 T0 
        out << "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;

        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;
        IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[-2])), "0", DEEP, 1);      //这个是常量数组
        tmp.IDENT_if_array = 1;      //表示这个量是数组
        tmp.IDENT_array = Ident_array;     //指向这个新生成的vector数组
        tmp.Array_size = n;
        tmp.IR_name = "T" + to_string(VAR_T_num);
        for(int i = 0; i < Array_dim.size();i++){
            Ident_dim_array->push_back(Array_dim[i]);
        }
        tmp.IDENT_dim_array = Ident_dim_array;
        Scope.push_back(tmp);
    }
#line 1556 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 229 "source/xcw_parser.y" /* yacc.c:1646  */
    {
            Array_deep = 0;    //将深度初始化为0
            Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
            Array_dest = Array_loc + path_length;
        }
#line 1566 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 235 "source/xcw_parser.y" /* yacc.c:1646  */
    {
            //没填满的元素用0填充
            //out << "------- Array_dest = "<<Array_dest<<endl;
            for(; Array_loc < Array_dest; Array_loc++){
                //out << "Array_loc = " << Array_loc << endl;
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                out << ir_name << " = " << 0 << endl;
            }
            VAR_T_num ++;     //定义结束后，把变量名数字 + 1
            Array_dim.clear();     //初始化数组维度
        }
#line 1584 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 256 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        (yyval) = (yyvsp[0]);
        //out << "ADD EXP" << endl;
    }
#line 1593 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 275 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[0])), "0", DEEP, 0);
        tmp.IR_name = "T" + to_string(VAR_T_num);
        //tmp.Print_IDENT();
        if(check_define(*ToStr((yyvsp[0])))){       //如果在当前域中未被定义过
            Scope.push_back(tmp);
            out << IF_DEEP() + "var T" << VAR_T_num << endl;
            out << IF_DEEP() + "T" << VAR_T_num << " = " << 0 << endl;
            VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr((yyvsp[0])) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
#line 1613 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 291 "source/xcw_parser.y" /* yacc.c:1646  */
    {

        if(ToPtrnum((yyvsp[0]))->IF_ptr_int == 1){       //传递的是常量
            int num = ToPtrnum((yyvsp[0]))->ptr_int;
            IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[-2])), to_string(num), DEEP, 0);  
            tmp.IR_name = "T" + to_string(VAR_T_num);   
            Scope.push_back(tmp);
            out << IF_DEEP() + "var T" << VAR_T_num << endl;
            out << IF_DEEP() + "T" << VAR_T_num << " = " << num << endl;
            VAR_T_num ++ ;
        }
        else{              //传递的是变量
            // out << "in the else " << endl;
            // out << "----------IF_ptr_str = " << ToPtrnum($3)->ptr_str << endl;
            string num = ToPtrnum((yyvsp[0]))->ptr_str;

            // out << "------------num = " << num << endl;
            IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[-2])), num, DEEP, 0);
            tmp.IR_name = "T" + to_string(VAR_T_num);   
            Scope.push_back(tmp);
            out << IF_DEEP() + "var T" << VAR_T_num << endl;
            out << IF_DEEP() + "T" << VAR_T_num << " = " << num << endl;
            VAR_T_num ++ ;
        }
        
        //tmp.Print_IDENT();
    }
#line 1645 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 319 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //首先检查当前域中是否出现
        if(!check_define(*ToStr((yyvsp[-1])))){
            string err = "\"" +  *ToStr((yyvsp[-1])) + "\" already defined in this scope.";
            yyerror(err);
        }

        // out << "Array num = " << ToPtrnum($2)->ptr_int << endl;
        // yyerror("test");
        int n = ToPtrnum((yyvsp[0]))->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        // 输出 var 24 T0 
        out << IF_DEEP() + "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;
        
        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;
        IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[-1])), "0", DEEP, 0);
        tmp.IDENT_if_array = 1;      //表示这个量是数组
        tmp.IDENT_array = Ident_array;     //指向这个新生成的vector数组
        tmp.Array_size = n;
        tmp.IR_name = "T" + to_string(VAR_T_num);
        for(int i = 0; i < Array_dim.size();i++){
            Ident_dim_array->push_back(Array_dim[i]);
        }
        tmp.IDENT_dim_array = Ident_dim_array;
        Scope.push_back(tmp);

        for(int i = 0; i < n; i++){
            Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * i) + "]";
            Ident_array->push_back(tmp_ptr);
            //out << IF_DEEP() + ir_name << " = " << 0 << endl;
        }
        VAR_T_num ++;
    }
#line 1684 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 354 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        
        // 先进行初始化
        Array_deep = 0;    //将深度初始化为0
        Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
        old_Array_dest = 0;
        path_length = ToPtrnum((yyvsp[-1]))->ptr_int;
        // 对应 a[4][2] = {1,2,{3},{5},7,8} 这些情况

        //首先检查当前域中是否出现
        if(!check_define(*ToStr((yyvsp[-2])))){
            string err = "\"" +  *ToStr((yyvsp[-2])) + "\" already defined in this scope.";
            yyerror(err);
        }
        int n = ToPtrnum((yyvsp[-1]))->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        // 输出 var 24 T0 
        out << IF_DEEP() + "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;

        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;
        IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[-2])), "0", DEEP, 0);
        tmp.IDENT_if_array = 1;      //表示这个量是数组
        tmp.IDENT_array = Ident_array;     //指向这个新生成的vector数组
        tmp.Array_size = n;
        tmp.IR_name = "T" + to_string(VAR_T_num);
        // 把Array_dim中的内容复制到IDENT_dim_array中
        for(int i = 0; i < Array_dim.size();i++){
            Ident_dim_array->push_back(Array_dim[i]);
        }
        tmp.IDENT_dim_array = Ident_dim_array;
        Scope.push_back(tmp);
    }
#line 1721 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 387 "source/xcw_parser.y" /* yacc.c:1646  */
    {
            Array_dest = Array_loc + path_length;
        }
#line 1729 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 391 "source/xcw_parser.y" /* yacc.c:1646  */
    {
            //没填满的元素用0填充
            //out << "------- Array_dest = "<<Array_dest<<endl;
            for(; Array_loc < Array_dest; Array_loc++){
                //out << "Array_loc = " << Array_loc << endl;
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                out << IF_DEEP() + ir_name << " = " << 0 << endl;
            }
            VAR_T_num ++;     //定义结束后，把变量名数字 + 1
            Array_dim.clear();     //初始化数组维度
        }
#line 1747 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 407 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        // 类似于{}的情况，推出空值
    }
#line 1755 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 411 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //类似于 {1},{1,2}
       // out << "ArrayExps " << endl;
        (yyval) = (yyvsp[0]);
    }
#line 1765 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 425 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //out << "Array_loc = " << Array_loc << endl;
        //out << "EXP" << endl;
        if(ToPtrnum((yyvsp[0]))->IF_ptr_int){    //为常量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum((yyvsp[0]))->ptr_int);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            out << IF_DEEP() + ir_name << " = " << ToPtrnum((yyvsp[0]))->ptr_int << endl;
        }       
        else{          //为变量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum((yyvsp[0]))->ptr_str);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            out << IF_DEEP() + ir_name << " = " << ToPtrnum((yyvsp[0]))->ptr_str << endl;
        }
        Array_loc ++;     //位置向前进1
    }
#line 1787 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 443 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //out << "LCURLY" << endl;
        path_length = path_length / Array_dim[Array_deep];
        Array_deep ++;      //遇到左括号，深度+1
        old_Array_dest = Array_dest;
        Array_dest = Array_loc + path_length;
    }
#line 1799 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 451 "source/xcw_parser.y" /* yacc.c:1646  */
    {
            //out << "RCURLY" << endl;
            //out << "------- Array_dest = "<<Array_dest<<endl;
            for(; Array_loc < Array_dest; Array_loc++){
                //out << "Array_loc = " << Array_loc << endl;
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                out << IF_DEEP() + ir_name << " = " << 0 << endl;
            }
            Array_dest = old_Array_dest;
            Array_deep --;      //遇到右括号，深度-1
            path_length = path_length * Array_dim[Array_deep];
        }
#line 1818 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 472 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //out << "Array num1111 = " << ToPtrnum($1)->ptr_int << endl;
        (yyval) = (yyvsp[0]);
    }
#line 1827 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 477 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        ToPtrnum((yyvsp[-1]))->ptr_int = ToPtrnum((yyvsp[-1]))->ptr_int * ToPtrnum((yyvsp[0]))->ptr_int;
        (yyval) = (yyvsp[-1]);
    }
#line 1836 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 485 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        (yyval) = (yyvsp[-1]);
        //out << "dim = " << ToPtrnum($2)->ptr_int << endl;
        Array_dim.push_back(ToPtrnum((yyvsp[-1]))->ptr_int);      //把数组数据放到Array_dim中，记录数组维度信息
        
    }
#line 1847 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 495 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        // out << "LVAL end"<<endl;
        // out << "IF_ptr_str = "<<ToPtrnum($1)->ptr_str<<endl;
        // $$ = $1;
    }
#line 1857 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 509 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        Ptr_num* add_1 = ToPtrnum((yyvsp[-2]));
        Ptr_num* mul_1 = ToPtrnum((yyvsp[0]));
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            tmp_ptr->ptr_int = add_1->ptr_int + mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " + " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        //out << "AddExp ADD MulExp" << *ToInt($1) << "+" << *ToInt($3) << endl;
        (yyval) = tmp_ptr; 
    }
#line 1895 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 543 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        Ptr_num* add_1 = ToPtrnum((yyvsp[-2]));
        Ptr_num* mul_1 = ToPtrnum((yyvsp[0]));
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            tmp_ptr->ptr_int = add_1->ptr_int - mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " - " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        (yyval) = tmp_ptr; 
    }
#line 1932 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 581 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        Ptr_num* add_1 = ToPtrnum((yyvsp[-2]));
        Ptr_num* mul_1 = ToPtrnum((yyvsp[0]));
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            tmp_ptr->ptr_int = add_1->ptr_int * mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " * " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        //out << "AddExp ADD MulExp" << *ToInt($1) << "+" << *ToInt($3) << endl;
        (yyval) = tmp_ptr; 
    }
#line 1970 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 615 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        Ptr_num* add_1 = ToPtrnum((yyvsp[-2]));
        Ptr_num* mul_1 = ToPtrnum((yyvsp[0]));
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            if(mul_1->ptr_int == 0){       //除0报错
                yyerror("Integer division by zero.");
            }
            tmp_ptr->ptr_int = add_1->ptr_int / mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                if(mul_1->ptr_int == 0){       //除0报错
                    yyerror("Integer division by zero.");
                }
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " / " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        //out << "AddExp ADD MulExp" << *ToInt($1) << "+" << *ToInt($3) << endl;
        (yyval) = tmp_ptr; 
    }
#line 2014 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 655 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        Ptr_num* add_1 = ToPtrnum((yyvsp[-2]));
        Ptr_num* mul_1 = ToPtrnum((yyvsp[0]));
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            if(mul_1->ptr_int == 0){       //除0报错
                yyerror("Integer mod by zero.");
            }
            tmp_ptr->ptr_int = add_1->ptr_int % mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            string str1, str2;
            if(add_1->IF_ptr_int){
                str1 = to_string(add_1->ptr_int);
            }
            else{
                str1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                if(mul_1->ptr_int == 0){       //除0报错
                    yyerror("Integer mod by zero.");
                }
                str2 = to_string(mul_1->ptr_int);
            }
            else{
                str2 = mul_1->ptr_str;
            }

            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str1 << " % " << str2 << endl;      // 输出类似于 t0 = T0 + 1
        }
        //out << "AddExp ADD MulExp" << *ToInt($1) << "+" << *ToInt($3) << endl;
        (yyval) = tmp_ptr; 
    }
#line 2058 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 698 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        (yyval) = (yyvsp[0]);
    }
#line 2066 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 702 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        IDENT_scope* tmp = find_define(*ToStr((yyvsp[-3])));

        if( tmp == nullptr){          //变量尚未定义
            string err = "\"" +  *ToStr((yyvsp[-3])) + "\" was not declared in this scope.";
            yyerror(err);
        }

        out << IF_DEEP() + "t" + to_string(VAR_t_num) + " = call f_" + *(ToStr((yyvsp[-3]))) << endl;

        Ptr_num* tmp_ptr = new Ptr_num;
        tmp_ptr->ptr_str = "t" + to_string(VAR_t_num);
        tmp_ptr->IF_ptr_int = 0;

        VAR_t_num ++;
        (yyval) = tmp_ptr;
        //出现在左边,函数必然为INT型，有返回值
        // IDENT_scope tmp_ret = IDENT_scope("ret"+*ToStr($1),"0",DEEP,0);
        // tmp_ret.IR_name = "T" + to_string(VAR_T_num);
        // out << IF_DEEP() + "var " + tmp_ret.IR_name << endl;
        // Scope.push_back(tmp_ret);
        // out << IF_DEEP() + tmp_ret.IR_name + " = call f_" + *(ToStr($1)) << endl;

        // 类似于 a(1,3)这样，表示调用函数
    }
#line 2096 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 731 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        Ptr_num* tmp_ptr = new Ptr_num;
        tmp_ptr->ptr_int = *ToInt((yyvsp[0]));
        tmp_ptr->IF_ptr_int = 1;
        (yyval) = tmp_ptr;
    }
#line 2107 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 738 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        // out << "LVAL end"<<endl;
        // out << "IF_ptr_str = "<<ToPtrnum($1)->ptr_str<<endl;
        (yyval) = (yyvsp[0]);
    }
#line 2117 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 747 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        IDENT_scope* tmp = find_define(*ToStr((yyvsp[0])));
        //out << "ToStr -> " << *ToStr($1) << endl;
        if( tmp == nullptr){          //变量尚未定义
            string err = "\"" +  *ToStr((yyvsp[0])) + "\" was not declared in this scope.";
            yyerror(err);
        }
        Ptr_num* tmp_ptr = new Ptr_num;
        if(tmp->IDENT_if_const){      //是常量，此时直接返回数值
            tmp_ptr->ptr_int = tmp->IDENT_const_num;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{       //是变量，此时返回类似于T0
            tmp_ptr->ptr_str = tmp->IR_name;
            tmp_ptr->IF_ptr_int = 0;
        }   
        (yyval) = tmp_ptr;
    }
#line 2140 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 766 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        IDENT_scope* tmp = find_define(*ToStr((yyvsp[0])));    //搜索这个数组的定义
        Array_name = tmp->IR_name;
        Array_dim.clear();
        for(int i = 0;i < (*tmp).IDENT_dim_array->size();i++){
            Array_dim.push_back((*(*tmp).IDENT_dim_array)[i]);
        }
    }
#line 2153 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 775 "source/xcw_parser.y" /* yacc.c:1646  */
    {        //a[2][3]     a[1][b]  
        //out << "Left = ArrayLVals " << endl;
        // IDENT_scope* tmp = ((IDENT_scope*)$$); 
        Ptr_num tmp_ptr, tmp_ptr_new, tmp_ptr_old;
        int ptr_size = INTSIZE;

        for(int i = Array_LVal_dim.size()-1; i >= 0 ;i --){
            //out << "i = "<<i << endl;
            // tmp_ptr_old = tmp_ptr_new;
            tmp_ptr = Array_LVal_dim[i];
            //out << "int = " << tmp_ptr.ptr_int << endl;
            if(tmp_ptr.IF_ptr_int){     //是整数
                //out << "tmp_ptr.IF_ptr_int = 1" << endl;
                tmp_ptr_new.IF_ptr_int = 1;
                tmp_ptr_new.ptr_int = tmp_ptr.ptr_int * ptr_size;
                //out << "tmp_ptr_new.ptr_int = " << tmp_ptr_new.ptr_int << endl;
                if(i != Array_LVal_dim.size()-1){     //第一次，不用考虑和之前相加
                    if(tmp_ptr_old.IF_ptr_int){     //如果前面的也是INT
                        tmp_ptr_old.ptr_int += tmp_ptr_old.ptr_int;
                    }
                    else{
                        out << IF_DEEP() + "t" + to_string(VAR_t_num ) << " = "<< tmp_ptr_new.ptr_int << " + " << tmp_ptr_old.ptr_str << endl;
                        tmp_ptr_old.ptr_str = "t" + to_string(VAR_t_num);
                        VAR_t_num ++;
                    }
                }
                else{
                    tmp_ptr_old = tmp_ptr_new;
                }
            }
            else{
                //out << "tmp_ptr.IF_ptr_int = 0" << endl;
                tmp_ptr_new.IF_ptr_int = 0;
                tmp_ptr_new.ptr_str = "t" + to_string(VAR_t_num);
                tmp_ptr_old.ptr_str = to_string(tmp_ptr_old.ptr_int);    //强制转换为string类型
                tmp_ptr_old.IF_ptr_int = 0;
                VAR_t_num ++;
                out << IF_DEEP() + tmp_ptr_new.ptr_str << " = " << tmp_ptr.ptr_str << " * " << ptr_size << endl;
                if(i != Array_LVal_dim.size()-1){     //第一次不用考虑和之前相加
                    //out << "tmp_ptr_old.ptr_int = " << tmp_ptr_old.ptr_int << endl;
                    out <<IF_DEEP() + "t" + to_string(VAR_t_num ) << " = "<< tmp_ptr_new.ptr_str << " + " << tmp_ptr_old.ptr_str << endl;
                    tmp_ptr_old.ptr_str = "t" + to_string(VAR_t_num);
                    VAR_t_num ++;
                }
                else{
                    tmp_ptr_old = tmp_ptr_new;
                }
            }
            ptr_size *= Array_dim[i];
            // out << "test ptr_size" << endl;
            // out << (*(tmp->IDENT_dim_array)) << endl;
            // out << "test ptr_size" << endl;
        }
        Array_LVal_dim.clear();
        //out << "Arrayend " << endl;
        if(tmp_ptr_old.IF_ptr_int){
            tmp_ptr_old.ptr_str = Array_name + "[" + to_string(tmp_ptr_old.ptr_int) + "]";
            tmp_ptr_old.IF_ptr_int = 0;
        }
        else    
            tmp_ptr_old.ptr_str = Array_name + "[" + tmp_ptr_old.ptr_str + "]";
        //out << "tmp_ptr_old.ptr_str = " << tmp_ptr_old.ptr_str << endl; 
        (yyval) = & tmp_ptr_old;
    }
#line 2222 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 848 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        Array_LVal_dim.push_back(*(ToPtrnum((yyvsp[-1]))));    //存入进行引用的维度
    }
#line 2230 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 856 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //out << " This is a INT Func" << endl;
        //首先检查当前域中是否出现
        if(!check_define(*ToStr((yyvsp[0])))){
            string err = "\"" +  *ToStr((yyvsp[-1])) + "\" already defined in this scope.";
            yyerror(err);
        }
        //  开始函数定义
        IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[0])), "0", DEEP, 0);    //不是Const
        tmp.IDENT_if_ret_int = 1;
        //tmp.IDENT_func_param_num = 0;      //没有参数
        Scope.push_back(tmp);
        
    }
#line 2249 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 871 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        DEEP ++;
    }
#line 2257 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 875 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        DEEP --;
        //IDENT_scope* tmp_ptr = find_define(*ToStr($2));    //找到函数变量的指针
        //tmp_ptr->IDENT_func_param_num = 
        out << "f_" << *ToStr((yyvsp[-5])) << " [" << VAR_p_num << "]" << endl;  
    }
#line 2268 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 882 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        
        out << "\treturn 0" << endl;
        out << "end " << "f_" << *ToStr((yyvsp[-7])) << endl;

        //声明结束后，把记录参数数量的 VAR_p_num 初始化
        VAR_p_num = 0;
        
    }
#line 2282 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 892 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //首先检查当前域中是否出现
        if(!check_define(*ToStr((yyvsp[0])))){
            string err = "\"" +  *ToStr((yyvsp[-1])) + "\" already defined in this scope.";
            yyerror(err);
        }
        //  开始函数定义
        IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[0])), "0", DEEP, 0);    //是Const,表示无返回值
        tmp.IDENT_if_ret_int = 0;
        //tmp.IDENT_func_param_num = 0;      //没有参数
        Scope.push_back(tmp);
        //out << "f_" << *ToStr($2) << " [" << tmp.IDENT_func_param_num << "]" << endl;
    }
#line 2300 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 906 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        DEEP ++;
    }
#line 2308 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 910 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        DEEP --;
        out << "f_" << *ToStr((yyvsp[-5])) << " [" << VAR_p_num << "]" << endl;
    }
#line 2317 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 915 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        
        out << "\treturn" << endl;
        out << "end " << "f_" << *ToStr((yyvsp[-7])) << endl;

        
    }
#line 2329 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 926 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //表示没有参数的情况
    }
#line 2337 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 930 "source/xcw_parser.y" /* yacc.c:1646  */
    {

    }
#line 2345 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 938 "source/xcw_parser.y" /* yacc.c:1646  */
    {        //a(int b){}
        //out << "INT IDENT" << endl;
        // 检查是否出现过
        if(!check_define(*ToStr((yyvsp[0])))){
            string err = "\"" +  *ToStr((yyvsp[-1])) + "\" already defined in this scope.";
            yyerror(err);
        }
        
        IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[0])), "0", DEEP, 0);    //是一个变量
        tmp.IDENT_if_array = 0;    //不是数组
        tmp.IR_name = "p" + to_string(VAR_p_num);
        //tmp.Print_IDENT();
        VAR_p_num ++;
        Scope.push_back(tmp);
    }
#line 2365 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 954 "source/xcw_parser.y" /* yacc.c:1646  */
    {      //void d(int d[])
        // 检查是否出现过
        if(!check_define(*ToStr((yyvsp[-3])))){
            string err = "\"" +  *ToStr((yyvsp[-4])) + "\" already defined in this scope.";
            yyerror(err);
        }

        IDENT_scope tmp = IDENT_scope(*ToStr((yyvsp[-3])), "0", DEEP, 0);    //是一个变量
        tmp.IDENT_if_array = 1;    //是数组
        tmp.IR_name = "p" + to_string(VAR_p_num);
        //tmp.Print_IDENT();
        VAR_p_num ++;
        Scope.push_back(tmp);
    }
#line 2384 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 972 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //out << "Array num1111 = " << ToPtrnum($1)->ptr_int << endl;
        (yyval) = (yyvsp[0]);
    }
#line 2393 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 977 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        ToPtrnum((yyvsp[-1]))->ptr_int = ToPtrnum((yyvsp[-1]))->ptr_int * ToPtrnum((yyvsp[0]))->ptr_int;
        (yyval) = (yyvsp[-1]);
    }
#line 2402 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 982 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //也有可能为空
    }
#line 2410 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 989 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        DEEP ++;     // 深度+1，保证这些新加入的元素属于当前域中
    }
#line 2418 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 993 "source/xcw_parser.y" /* yacc.c:1646  */
    {
            //结束以后，删除在其中定义过的变量,因为这些实际上是局部变量，但方便起见输出成原生变量
            //在这里正好也同时删除了参数变量（因为事实上声明时并不会用到）
            int i = Scope.size() - 1;
            while(Scope[i].IDENT_deep == DEEP && i >= 0){      //需要深度一致
                Scope.pop_back();
                i--;
            }
            DEEP --;     //还原
        }
#line 2433 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1010 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        // out << "BlockItem"<<endl;
        //暂时先考虑内容为空的情况
    }
#line 2442 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1015 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //out << "this is decl " << endl;
    }
#line 2450 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1019 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //out << "this is stmt " << endl;
    }
#line 2458 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1026 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //out << IF_DEEP() + "return" << endl;
    }
#line 2466 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1030 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        if(ToPtrnum((yyvsp[-1]))->IF_ptr_int){       //为常量
            out << IF_DEEP() + "return " << ToPtrnum((yyvsp[-1]))->ptr_int << endl;
        }
        else{
            //out << "ToPtrnum($2)->ptr_str = " << ToPtrnum($2)->ptr_str << endl;
            out << IF_DEEP() + "return " + ToPtrnum((yyvsp[-1]))->ptr_str << endl;
        }

        //out << "test" << endl;
    }
#line 2482 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1042 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //类似于 直接调用void函数，如f(1,2);
    }
#line 2490 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1048 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        //表示函数调用中没有参数的情况
    }
#line 2498 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1052 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        Ptr_num* param_tmp = ToPtrnum((yyvsp[0]));
        if(param_tmp->IF_ptr_int){       //如果是常量
            out << IF_DEEP() + "param " << param_tmp->ptr_int << endl;
        }
        else{
            out << IF_DEEP() + "param " + param_tmp->ptr_str << endl;
        }

    }
#line 2513 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1063 "source/xcw_parser.y" /* yacc.c:1646  */
    {
        Ptr_num* param_tmp = ToPtrnum((yyvsp[0]));
        if(param_tmp->IF_ptr_int){       //如果是常量
            out << IF_DEEP() + "param " << param_tmp->ptr_int << endl;
        }
        else{
            out << IF_DEEP() + "param " + param_tmp->ptr_str << endl;
        }
    }
#line 2527 "build/xcw_parser.tab.c" /* yacc.c:1646  */
    break;


#line 2531 "build/xcw_parser.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1083 "source/xcw_parser.y" /* yacc.c:1906  */


void yyerror(const char *s) {
    extern int yylineno, charNum;
    cout << "Line " << yylineno << "," << charNum << ": " << s << endl;
    exit(1);
}

void yyerror(const string &s) {
    yyerror(s.c_str());
}

int main() {
    ios::sync_with_stdio(false);
    yyparse();
    return 0;
}
