/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "source/xcw_parser.y" /* yacc.c:337  */

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
#include<stack>

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
    int IF_ptr_int;
    Ptr_num(int p_int){
        ptr_int = p_int;
        IF_ptr_int = 1; 
    }
    Ptr_num(string p_str){
        ptr_str = p_str;
        IF_ptr_int = 0; 
    }
    Ptr_num(){}
    void Print(){       //打印出数值，用于调试
        //out << "-------------Ptr_print_in------------"<<endl;
        if(IF_ptr_int){
            //out << "IF_ptr_int = " << IF_ptr_int << endl;
            out << ptr_int;
        }
        else{
            //out << "IF_ptr_int = " << IF_ptr_int << endl;
            out << ptr_str;
        }
        //out << endl;
        //out << "-------------Ptr_print_out------------"<<endl;
    }
};

struct Params_num{      //函数参数的元素类型，需要包括 INT型变量和INT型数组指针
    string para_str;       //变量名字
    string para_ir_name;    //在IR中，函数参数的名字
};


int DEEP;      //当前的深度
string IF_DEEP(){
    string str_if_deep = "";
    for(int i = 0;i < DEEP;i++){
        str_if_deep += "\t";
    }
    //str_if_deep = DEEP * "\t";
    return str_if_deep;
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

//-----------------IF_ELSE相关变量------------------------------
int LABEL_l_num_st = 0;       //表示一个IF语句开始，需要的label标记数   默认为0
int LABEL_l_num_end = 0;
stack<int> Stk_IF_ELSE;

//----------------------------------------------------------


#line 233 "build/xcw_parser.tab.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
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
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
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
#  define YYSIZE_T unsigned
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYLAST   149

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  58
/* YYNRULES -- Number of rules.  */
#define YYNRULES  99
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  159

#define YYUNDEFTOK  2
#define YYMAXUTOK   292

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
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
       0,   179,   179,   180,   184,   185,   189,   190,   194,   198,
     199,   203,   219,   248,   218,   270,   274,   282,   288,   289,
     293,   309,   337,   373,   406,   372,   426,   429,   438,   439,
     443,   462,   461,   490,   495,   503,   513,   522,   540,   541,
     575,   612,   617,   651,   691,   734,   739,   768,   775,   785,
     805,   804,   896,   897,   901,   910,   925,   929,   909,   946,
     960,   964,   945,   980,   983,   987,   991,  1007,  1025,  1030,
    1036,  1043,  1042,  1060,  1061,  1064,  1068,  1072,  1079,  1083,
    1100,  1104,  1108,  1113,  1112,  1137,  1146,  1136,  1158,  1157,
    1179,  1182,  1191,  1200,  1204,  1208,  1212,  1218,  1221,  1232
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
  "$@13", "BlockItems", "BlockItem", "Stmt", "$@14", "$@15", "$@16",
  "$@17", "IF_Else", "Cond", "LOrExp", "LAndExp", "EqExp", "RelExp",
  "FuncRParams", YY_NULLPTR
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

#define YYPACT_NINF -134

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-134)))

#define YYTABLE_NINF -84

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       4,    39,    59,    61,    76,  -134,  -134,  -134,  -134,  -134,
      47,    65,  -134,    71,  -134,  -134,  -134,     6,     6,    22,
    -134,    85,  -134,   100,    40,    67,  -134,   102,     0,  -134,
      95,   101,    82,  -134,  -134,  -134,  -134,  -134,   101,  -134,
    -134,  -134,    45,  -134,     6,    48,  -134,    71,  -134,     6,
      97,  -134,     6,     6,     6,     6,     6,   105,    98,  -134,
    -134,  -134,  -134,    98,  -134,     2,     6,    97,  -134,    82,
      82,  -134,  -134,  -134,  -134,   104,    15,  -134,   106,    16,
    -134,     6,   107,  -134,    -3,   109,  -134,    98,  -134,  -134,
    -134,  -134,  -134,   108,    83,  -134,  -134,   110,   112,  -134,
      -3,   112,    -3,  -134,    -3,   113,  -134,  -134,   115,  -134,
     117,  -134,  -134,   113,    25,  -134,  -134,  -134,   100,   116,
     121,     8,  -134,  -134,    99,    86,  -134,    -5,  -134,  -134,
       6,     6,  -134,   103,  -134,   111,  -134,  -134,   101,   122,
    -134,  -134,  -134,  -134,   124,  -134,     6,  -134,  -134,   114,
      38,    38,  -134,  -134,  -134,   118,    38,  -134,  -134
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
      34,    56,    20,    19,     0,     0,     8,     0,    60,    97,
       0,    35,     0,     0,     0,     0,     0,     0,    63,    11,
      15,    12,    10,    63,    98,     0,     0,    51,    52,    39,
      40,    42,    43,    44,    24,     0,     0,    64,     0,     0,
      46,     0,     0,    53,    26,    66,    57,     0,    13,    61,
      99,    54,    31,     0,    27,    28,    30,     0,     0,    65,
      26,     0,    26,    25,     0,    70,    71,    58,     0,    62,
       0,    29,    68,    67,    75,    14,    32,    69,     0,     0,
       0,     0,    81,    76,     0,    48,    82,     0,    73,    77,
       0,     0,    78,     0,    80,     0,    72,    74,    96,     0,
      92,    93,    94,    95,     0,    79,     0,    85,    88,     0,
       0,     0,    84,    86,    89,    90,     0,    87,    91
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -134,  -134,   127,   -50,  -134,  -134,    88,  -134,  -134,  -134,
      92,  -134,  -134,   119,  -134,  -134,   -17,  -134,    34,  -134,
     123,   -15,  -134,   -18,   -16,    54,    41,  -134,  -112,  -134,
    -134,    72,  -134,  -134,  -134,  -134,  -134,  -134,  -134,    77,
      56,  -134,   -30,  -134,  -134,    14,  -133,  -134,  -134,  -134,
    -134,  -134,    18,  -134,  -134,  -134,  -134,  -134
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    25,    26,    78,   100,    59,
      30,     8,    11,    12,    57,    84,    93,    94,    95,   102,
      19,    20,    36,   124,    38,    32,    33,    34,    35,    50,
      67,    68,     9,    21,    58,    98,    27,    63,   101,    76,
      77,   113,   126,   114,   127,   128,   129,   135,   150,   155,
     151,   157,   139,   140,   141,   142,   143,    65
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      37,    31,   125,    28,    40,    28,   106,   136,    92,    49,
     118,     2,    80,   -50,    28,   125,    28,   153,   154,     1,
       2,     3,   119,   158,   120,    86,    89,   121,    31,   122,
      40,    64,    29,    28,    29,    17,   106,    81,   125,   125,
     118,     2,   132,    29,   125,    29,    28,    10,    82,   106,
      87,    87,   119,    17,   120,    39,   -55,   121,    17,   122,
      17,    17,    29,    90,   123,   119,    96,   120,   107,    14,
     121,   109,   122,    44,    13,    29,    15,   123,    18,    24,
      18,    61,    96,   108,    96,   110,    96,    54,    55,    56,
     112,     1,     2,     3,    41,    71,    72,    73,   117,    22,
      23,    46,    47,   133,    52,    53,    69,    70,    42,    51,
      66,    48,    85,    75,   138,   138,    74,    88,   104,   -83,
     103,    91,    97,   106,   105,   130,    17,   115,   149,   116,
     131,    16,   147,   134,   148,    62,    60,   145,   111,    83,
      79,   137,    43,    99,   146,     0,   156,    45,   152,   144
};

static const yytype_int16 yycheck[] =
{
      18,    17,   114,     8,    19,     8,    11,    12,    11,     9,
      15,    16,    10,    13,     8,   127,     8,   150,   151,    15,
      16,    17,    27,   156,    29,    10,    10,    32,    44,    34,
      45,    49,    37,     8,    37,    13,    11,    35,   150,   151,
      15,    16,    34,    37,   156,    37,     8,     8,    66,    11,
      35,    35,    27,    13,    29,    33,     9,    32,    13,    34,
      13,    13,    37,    81,   114,    27,    84,    29,    98,     8,
      32,   101,    34,    33,    15,    37,     0,   127,    33,     8,
      33,    33,   100,   100,   102,   102,   104,     5,     6,     7,
     105,    15,    16,    17,     9,    54,    55,    56,   113,    34,
      35,    34,    35,   121,     3,     4,    52,    53,     8,    14,
      13,     9,     8,    15,   130,   131,    11,    11,    35,    33,
      12,    14,    13,    11,    14,     9,    13,    12,   146,    12,
       9,     4,    10,    34,    10,    47,    44,    34,   104,    67,
      63,   127,    23,    87,    33,    -1,    28,    24,    34,   131
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
      48,    33,    44,    75,    61,    95,    13,    68,    69,    63,
      63,    64,    64,    64,    11,    15,    77,    78,    45,    77,
      10,    35,    61,    69,    53,     8,    10,    35,    11,    10,
      61,    14,    11,    54,    55,    56,    61,    13,    73,    78,
      46,    76,    57,    12,    35,    14,    11,    80,    54,    80,
      54,    56,    59,    79,    81,    12,    12,    59,    15,    27,
      29,    32,    34,    41,    61,    66,    80,    82,    83,    84,
       9,     9,    34,    61,    34,    85,    12,    83,    62,    90,
      91,    92,    93,    94,    90,    34,    33,    10,    10,    61,
      86,    88,    34,    84,    84,    87,    28,    89,    84
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
      84,    84,    84,    85,    84,    86,    87,    84,    88,    84,
      89,    89,    90,    91,    92,    93,    94,    95,    95,    95
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
       2,     1,     1,     0,     5,     0,     0,     8,     0,     6,
       0,     2,     1,     1,     1,     1,     1,     0,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
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
  unsigned long yylno = yyrline[yyrule];
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
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
            else
              goto append;

          append:
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

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
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
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
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
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
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
# else /* defined YYSTACK_RELOCATE */
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
| yyreduce -- do a reduction.  |
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
#line 204 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        int num = *ToInt(yyvsp[0]);
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-2]), num, DEEP, 1);
        //tmp.Print_IDENT();
        if(check_define(*ToStr(yyvsp[-2]))){       //如果在当前域中未被定义过
            //out << "11111" << endl;
            Scope.push_back(tmp);
            //VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr(yyvsp[-2]) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
#line 1566 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 12:
#line 219 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        // 因为常量数组的下标仍有可能是变量，因此考虑将其用变量数组的形式做
        Array_deep = 0;    //将深度初始化为0
        Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
        old_Array_dest = 0;
        path_length = ToPtrnum(yyvsp[-1])->ptr_int;
        //首先检查当前域中是否出现
        if(!check_define(*ToStr(yyvsp[-2]))){
            string err = "\"" +  *ToStr(yyvsp[-2]) + "\" already defined in this scope.";
            yyerror(err);
        }
        int n = ToPtrnum(yyvsp[-1])->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        // 输出 var 24 T0 
        out << "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;

        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-2]), "0", DEEP, 1);      //这个是常量数组
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
#line 1599 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 13:
#line 248 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            Array_deep = 0;    //将深度初始化为0
            Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
            Array_dest = Array_loc + path_length;
        }
#line 1609 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 14:
#line 254 "source/xcw_parser.y" /* yacc.c:1652  */
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
#line 1627 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 16:
#line 275 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
        //out << "ADD EXP" << endl;
    }
#line 1636 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 20:
#line 294 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[0]), "0", DEEP, 0);
        tmp.IR_name = "T" + to_string(VAR_T_num);
        //tmp.Print_IDENT();
        if(check_define(*ToStr(yyvsp[0]))){       //如果在当前域中未被定义过
            Scope.push_back(tmp);
            out << IF_DEEP() + "var T" << VAR_T_num << endl;
            out << IF_DEEP() + "T" << VAR_T_num << " = " << 0 << endl;
            VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr(yyvsp[0]) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
#line 1656 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 21:
#line 310 "source/xcw_parser.y" /* yacc.c:1652  */
    {

        if(ToPtrnum(yyvsp[0])->IF_ptr_int == 1){       //传递的是常量
            int num = ToPtrnum(yyvsp[0])->ptr_int;
            IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-2]), to_string(num), DEEP, 0);  
            tmp.IR_name = "T" + to_string(VAR_T_num);   
            Scope.push_back(tmp);
            out << IF_DEEP() + "var T" << VAR_T_num << endl;
            out << IF_DEEP() + "T" << VAR_T_num << " = " << num << endl;
            VAR_T_num ++ ;
        }
        else{              //传递的是变量
            // out << "in the else " << endl;
            // out << "----------IF_ptr_str = " << ToPtrnum($3)->ptr_str << endl;
            string num = ToPtrnum(yyvsp[0])->ptr_str;

            // out << "------------num = " << num << endl;
            IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-2]), num, DEEP, 0);
            tmp.IR_name = "T" + to_string(VAR_T_num);   
            Scope.push_back(tmp);
            out << IF_DEEP() + "var T" << VAR_T_num << endl;
            out << IF_DEEP() + "T" << VAR_T_num << " = " << num << endl;
            VAR_T_num ++ ;
        }
        
        //tmp.Print_IDENT();
    }
#line 1688 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 22:
#line 338 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //首先检查当前域中是否出现
        if(!check_define(*ToStr(yyvsp[-1]))){
            string err = "\"" +  *ToStr(yyvsp[-1]) + "\" already defined in this scope.";
            yyerror(err);
        }

        // out << "Array num = " << ToPtrnum($2)->ptr_int << endl;
        // yyerror("test");
        int n = ToPtrnum(yyvsp[0])->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        // 输出 var 24 T0 
        out << IF_DEEP() + "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;
        
        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-1]), "0", DEEP, 0);
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
#line 1727 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 23:
#line 373 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        
        // 先进行初始化
        Array_deep = 0;    //将深度初始化为0
        Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
        old_Array_dest = 0;
        path_length = ToPtrnum(yyvsp[-1])->ptr_int;
        // 对应 a[4][2] = {1,2,{3},{5},7,8} 这些情况

        //首先检查当前域中是否出现
        if(!check_define(*ToStr(yyvsp[-2]))){
            string err = "\"" +  *ToStr(yyvsp[-2]) + "\" already defined in this scope.";
            yyerror(err);
        }
        int n = ToPtrnum(yyvsp[-1])->ptr_int;      //当前数组的元素总数，例如a[2][3], n=6
        // 输出 var 24 T0 
        out << IF_DEEP() + "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;

        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-2]), "0", DEEP, 0);
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
#line 1764 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 24:
#line 406 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            Array_dest = Array_loc + path_length;
        }
#line 1772 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 25:
#line 410 "source/xcw_parser.y" /* yacc.c:1652  */
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
#line 1790 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 26:
#line 426 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        // 类似于{}的情况，推出空值
    }
#line 1798 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 27:
#line 430 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //类似于 {1},{1,2}
       // out << "ArrayExps " << endl;
        yyval = yyvsp[0];
    }
#line 1808 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 444 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "Array_loc = " << Array_loc << endl;
        //out << "EXP" << endl;
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //为常量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum(yyvsp[0])->ptr_int);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            out << IF_DEEP() + ir_name << " = " << ToPtrnum(yyvsp[0])->ptr_int << endl;
        }       
        else{          //为变量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum(yyvsp[0])->ptr_str);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            out << IF_DEEP() + ir_name << " = " << ToPtrnum(yyvsp[0])->ptr_str << endl;
        }
        Array_loc ++;     //位置向前进1
    }
#line 1830 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 31:
#line 462 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "LCURLY" << endl;
        path_length = path_length / Array_dim[Array_deep];
        Array_deep ++;      //遇到左括号，深度+1
        old_Array_dest = Array_dest;
        Array_dest = Array_loc + path_length;
    }
#line 1842 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 32:
#line 470 "source/xcw_parser.y" /* yacc.c:1652  */
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
#line 1861 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 491 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "Array num1111 = " << ToPtrnum($1)->ptr_int << endl;
        yyval = yyvsp[0];
    }
#line 1870 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 496 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        ToPtrnum(yyvsp[-1])->ptr_int = ToPtrnum(yyvsp[-1])->ptr_int * ToPtrnum(yyvsp[0])->ptr_int;
        yyval = yyvsp[-1];
    }
#line 1879 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 35:
#line 504 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[-1];
        //out << "dim = " << ToPtrnum($2)->ptr_int << endl;
        Array_dim.push_back(ToPtrnum(yyvsp[-1])->ptr_int);      //把数组数据放到Array_dim中，记录数组维度信息
        
    }
#line 1890 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 514 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        // out << "LVAL end"<<endl;
        // out << "IF_ptr_str = "<<ToPtrnum($1)->ptr_str<<endl;
        // $$ = $1;
    }
#line 1900 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 523 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
        //ToPtrnum($1)->Print();
        // out << IF_DEEP() + "++++if_int = " << ToPtrnum($1)->IF_ptr_int << endl;
        // if(ToPtrnum($1)->IF_ptr_int)
        //     out << IF_DEEP() + "aaaaaa " << ToPtrnum($1)->ptr_int << endl;
        // else{
        //     out << "test" << endl;
        //     ToPtrnum($1)->Print();
        //     out << IF_DEEP() + "bbbbbb " << endl;
        //     out << ToPtrnum($1)->ptr_str << endl;
        // }
            
    }
#line 1919 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 542 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* add_1 = ToPtrnum(yyvsp[-2]);
        Ptr_num* mul_1 = ToPtrnum(yyvsp[0]);
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
        yyval = tmp_ptr; 
    }
#line 1957 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 576 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* add_1 = ToPtrnum(yyvsp[-2]);
        Ptr_num* mul_1 = ToPtrnum(yyvsp[0]);
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
        yyval = tmp_ptr; 
    }
#line 1994 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 41:
#line 613 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
//        ToPtrnum($1)->Print();
    }
#line 2003 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 618 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* add_1 = ToPtrnum(yyvsp[-2]);
        Ptr_num* mul_1 = ToPtrnum(yyvsp[0]);
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
        yyval = tmp_ptr; 
    }
#line 2041 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 43:
#line 652 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* add_1 = ToPtrnum(yyvsp[-2]);
        Ptr_num* mul_1 = ToPtrnum(yyvsp[0]);
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
        yyval = tmp_ptr; 
    }
#line 2085 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 44:
#line 692 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* add_1 = ToPtrnum(yyvsp[-2]);
        Ptr_num* mul_1 = ToPtrnum(yyvsp[0]);
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
        yyval = tmp_ptr; 
    }
#line 2129 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 45:
#line 735 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
//        ToPtrnum($1)->Print();
    }
#line 2138 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 740 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        IDENT_scope* tmp = find_define(*ToStr(yyvsp[-3]));

        if( tmp == nullptr){          //变量尚未定义
            string err = "\"" +  *ToStr(yyvsp[-3]) + "\" was not declared in this scope.";
            yyerror(err);
        }

        out << IF_DEEP() + "t" + to_string(VAR_t_num) + " = call f_" + *(ToStr(yyvsp[-3])) << endl;

        Ptr_num* tmp_ptr = new Ptr_num;
        tmp_ptr->ptr_str = "t" + to_string(VAR_t_num);
        tmp_ptr->IF_ptr_int = 0;

        VAR_t_num ++;
        yyval = tmp_ptr;
        //出现在左边,函数必然为INT型，有返回值
        // IDENT_scope tmp_ret = IDENT_scope("ret"+*ToStr($1),"0",DEEP,0);
        // tmp_ret.IR_name = "T" + to_string(VAR_T_num);
        // out << IF_DEEP() + "var " + tmp_ret.IR_name << endl;
        // Scope.push_back(tmp_ret);
        // out << IF_DEEP() + tmp_ret.IR_name + " = call f_" + *(ToStr($1)) << endl;

        // 类似于 a(1,3)这样，表示调用函数
    }
#line 2168 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 47:
#line 769 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* tmp_ptr = new Ptr_num;
        tmp_ptr->ptr_int = *ToInt(yyvsp[0]);
        tmp_ptr->IF_ptr_int = 1;
        yyval = tmp_ptr;
    }
#line 2179 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 776 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        // out << "LVaL end"<<endl;
        // out << "IF_ptr_str = "<<ToPtrnum($1)->ptr_str<<endl;
        yyval = yyvsp[0];
        //ToPtrnum($1)->Print();
    }
#line 2190 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 786 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        IDENT_scope* tmp = find_define(*ToStr(yyvsp[0]));
        //out << "ToStr -> " << *ToStr($1) << endl;
        if( tmp == nullptr){          //变量尚未定义
            string err = "\"" +  *ToStr(yyvsp[0]) + "\" was not declared in this scope.";
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
        yyval = tmp_ptr;
    }
#line 2213 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 805 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        IDENT_scope* tmp = find_define(*ToStr(yyvsp[0]));    //搜索这个数组的定义
        Array_name = tmp->IR_name;
        Array_dim.clear();
        for(int i = 0;i < (*tmp).IDENT_dim_array->size();i++){
            Array_dim.push_back((*(*tmp).IDENT_dim_array)[i]);
        }
    }
#line 2226 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 51:
#line 814 "source/xcw_parser.y" /* yacc.c:1652  */
    {        //a[2][3]     a[1][b]  
            //out << "Left = ArrayLVals " << endl;
            // IDENT_scope* tmp = ((IDENT_scope*)$$); 
            Ptr_num tmp_ptr, tmp_ptr_new, tmp_ptr_old;
            int ptr_size = INTSIZE;
            //out << "in ArrayLVals--------------" << endl;

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
                            tmp_ptr_old.ptr_int += tmp_ptr_new.ptr_int;
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
                ptr_size *= Array_dim[i];     //更新ptr_size
                //out << endl << "ptr_size = "<< ptr_size << endl;
                // out << "test ptr_size" << endl;
                // out << (*(tmp->IDENT_dim_array)) << endl;
                // out << "test ptr_size" << endl;
            }
            Array_LVal_dim.clear();
            //out << "Arrayend " << endl;
            if(tmp_ptr_old.IF_ptr_int){
                tmp_ptr_old.ptr_str = Array_name + "[" + to_string(tmp_ptr_old.ptr_int) + "]";
                tmp_ptr_old.IF_ptr_int = 0;     //最后的结果一定是一个字符串类型
                //out << "tmp_ptr_old.ptr_str = " <<  tmp_ptr_old.ptr_str << endl;
            }
            else{
                tmp_ptr_old.ptr_str = Array_name + "[" + tmp_ptr_old.ptr_str + "]";
                tmp_ptr_old.IF_ptr_int = 0;
                //out << "tmp_ptr_old.ptr_str = " <<  tmp_ptr_old.ptr_str << endl;
            }    
            //out << "tmp_ptr_old.ptr_str = " << tmp_ptr_old.ptr_str << endl; 

            //用临时变量储存数组元素
            // out << IF_DEEP() + "t" + to_string(VAR_t_num) << " = "<< tmp_ptr_old.ptr_str << endl;
            // tmp_ptr_old.ptr_str = "t" + to_string(VAR_t_num);
            // VAR_t_num ++;
            yyval = & tmp_ptr_old;


            //out << "tmp_ptr_old.ptr_str = " <<  tmp_ptr_old.ptr_str << endl;
            //out << "out ArrayLVals--------------" << endl;
        }
#line 2310 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 54:
#line 902 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        Array_LVal_dim.push_back(*(ToPtrnum(yyvsp[-1])));    //存入进行引用的维度
    }
#line 2318 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 55:
#line 910 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << " This is a INT Func" << endl;
        //首先检查当前域中是否出现
        if(!check_define(*ToStr(yyvsp[0]))){
            string err = "\"" +  *ToStr(yyvsp[-1]) + "\" already defined in this scope.";
            yyerror(err);
        }
        //  开始函数定义
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[0]), "0", DEEP, 0);    //不是Const
        tmp.IDENT_if_ret_int = 1;
        //tmp.IDENT_func_param_num = 0;      //没有参数
        Scope.push_back(tmp);
        
    }
#line 2337 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 56:
#line 925 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            DEEP ++;
        }
#line 2345 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 57:
#line 929 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            DEEP --;
            //IDENT_scope* tmp_ptr = find_define(*ToStr($2));    //找到函数变量的指针
            //tmp_ptr->IDENT_func_param_num = 
            out << "f_" << *ToStr(yyvsp[-5]) << " [" << VAR_p_num << "]" << endl;  
        }
#line 2356 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 58:
#line 936 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            
            out << "\treturn 0" << endl;
            out << "end " << "f_" << *ToStr(yyvsp[-7]) << endl;

            //声明结束后，把记录参数数量的 VAR_p_num 初始化
            VAR_p_num = 0;
            
        }
#line 2370 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 59:
#line 946 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //首先检查当前域中是否出现
        if(!check_define(*ToStr(yyvsp[0]))){
            string err = "\"" +  *ToStr(yyvsp[-1]) + "\" already defined in this scope.";
            yyerror(err);
        }
        //  开始函数定义
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[0]), "0", DEEP, 0);    //是Const,表示无返回值
        tmp.IDENT_if_ret_int = 0;
        //tmp.IDENT_func_param_num = 0;      //没有参数
        Scope.push_back(tmp);
        //out << "f_" << *ToStr($2) << " [" << tmp.IDENT_func_param_num << "]" << endl;
    }
#line 2388 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 60:
#line 960 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            DEEP ++;
        }
#line 2396 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 61:
#line 964 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            DEEP --;
            out << "f_" << *ToStr(yyvsp[-5]) << " [" << VAR_p_num << "]" << endl;
        }
#line 2405 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 62:
#line 969 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            
            out << "\treturn" << endl;
            out << "end " << "f_" << *ToStr(yyvsp[-7]) << endl;

            
        }
#line 2417 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 63:
#line 980 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //表示没有参数的情况
    }
#line 2425 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 64:
#line 984 "source/xcw_parser.y" /* yacc.c:1652  */
    {

    }
#line 2433 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 66:
#line 992 "source/xcw_parser.y" /* yacc.c:1652  */
    {        //a(int b){}
        //out << "INT IDENT" << endl;
        // 检查是否出现过
        if(!check_define(*ToStr(yyvsp[0]))){
            string err = "\"" +  *ToStr(yyvsp[-1]) + "\" already defined in this scope.";
            yyerror(err);
        }
        
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[0]), "0", DEEP, 0);    //是一个变量
        tmp.IDENT_if_array = 0;    //不是数组
        tmp.IR_name = "p" + to_string(VAR_p_num);
        //tmp.Print_IDENT();
        VAR_p_num ++;
        Scope.push_back(tmp);
    }
#line 2453 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 67:
#line 1008 "source/xcw_parser.y" /* yacc.c:1652  */
    {      //void d(int d[])
        // 检查是否出现过
        if(!check_define(*ToStr(yyvsp[-3]))){
            string err = "\"" +  *ToStr(yyvsp[-4]) + "\" already defined in this scope.";
            yyerror(err);
        }

        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-3]), "0", DEEP, 0);    //是一个变量
        tmp.IDENT_if_array = 1;    //是数组
        tmp.IR_name = "p" + to_string(VAR_p_num);
        //tmp.Print_IDENT();
        VAR_p_num ++;
        Scope.push_back(tmp);
    }
#line 2472 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 68:
#line 1026 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "Array num1111 = " << ToPtrnum($1)->ptr_int << endl;
        yyval = yyvsp[0];
    }
#line 2481 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 69:
#line 1031 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        ToPtrnum(yyvsp[-1])->ptr_int = ToPtrnum(yyvsp[-1])->ptr_int * ToPtrnum(yyvsp[0])->ptr_int;
        yyval = yyvsp[-1];
    }
#line 2490 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 70:
#line 1036 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //也有可能为空
    }
#line 2498 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 71:
#line 1043 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        DEEP ++;     // 深度+1，保证这些新加入的元素属于当前域中
    }
#line 2506 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 72:
#line 1047 "source/xcw_parser.y" /* yacc.c:1652  */
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
#line 2521 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 75:
#line 1064 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        // out << "BlockItem"<<endl;
        //暂时先考虑内容为空的情况
    }
#line 2530 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 76:
#line 1069 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "this is decl " << endl;
    }
#line 2538 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 77:
#line 1073 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "this is stmt " << endl;
    }
#line 2546 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 78:
#line 1080 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << IF_DEEP() + "return" << endl;
    }
#line 2554 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 79:
#line 1084 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        // out << IF_DEEP() + "return ";
        //ToPtrnum($2)->Print();
        // out << endl;
        //out << IF_DEEP() + "ToPtrnum($2)->IF_ptr_int = " << ToPtrnum($2)->IF_ptr_int<< endl;
        //out << IF_DEEP() + "ToPtrnum($2)->ptr_str = " << ToPtrnum($2)->ptr_str<< endl;
        if(ToPtrnum(yyvsp[-1])->IF_ptr_int){       //为常量
            out << IF_DEEP() + "return " << ToPtrnum(yyvsp[-1])->ptr_int << endl;
        }
        else{
            //out << "ToPtrnum($2)->ptr_str = " << ToPtrnum($2)->ptr_str << endl;
            out << IF_DEEP() + "return " + ToPtrnum(yyvsp[-1])->ptr_str << endl;
        }

        //out << "test" << endl;
    }
#line 2575 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 80:
#line 1101 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //类似于 直接调用void函数，如f(1,2);
    }
#line 2583 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 81:
#line 1105 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        // 一个;的情况
    }
#line 2591 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 82:
#line 1109 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        // 内部还是一个语句块
    }
#line 2599 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 83:
#line 1113 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        out << IF_DEEP() + ToPtrnum(yyvsp[0])->ptr_str;
    }
#line 2607 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 84:
#line 1117 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            // out << IF_DEEP() + "if_int = " << ToPtrnum($4)->IF_ptr_int << endl;
            // out << IF_DEEP() + "aaaaaa " << ToPtrnum($4)->ptr_int << endl;
            //类似于 a = b   LVal 返回一个 Ptr_num类型的指针tmp_ptr，此时LVal传上来的必定是一个ptr_str
            

            if(ToPtrnum(yyvsp[-1])->IF_ptr_int == 1){       //传递的是常量
                int num = ToPtrnum(yyvsp[-1])->ptr_int;
                out << " = " << num << endl;
            }
            else{              //Exp传递的是变量
                // out << "in else" << endl;
                string num = ToPtrnum(yyvsp[-1])->ptr_str;
                // out << "after string" <<endl;
                out << " = " << num << endl;
                // out << "after out" <<endl;
            }

        }
#line 2631 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 85:
#line 1137 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        LABEL_l_num_st = LABEL_l_num_end;
        Stk_IF_ELSE.push(LABEL_l_num_st);      //当前的label存入栈中
        LABEL_l_num_end += 3;      //因为一个IF语句一般会用到3个label
        out << IF_DEEP() + "if t" + to_string(VAR_t_num) + " == 0 goto l" + to_string(LABEL_l_num_st) << endl;
        out << IF_DEEP() + "goto l" + to_string(LABEL_l_num_st+1) << endl;
        out << IF_DEEP() + "l" + to_string(LABEL_l_num_st+1) + ":" << endl;
    }
#line 2644 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 86:
#line 1146 "source/xcw_parser.y" /* yacc.c:1652  */
    { 
            LABEL_l_num_st = Stk_IF_ELSE.top();
            
            out << IF_DEEP() + "\t" + "goto l" + to_string(LABEL_l_num_st+2) << endl;  //goto l2
            out << IF_DEEP() + "l" + to_string(LABEL_l_num_st) + ":" << endl;
            //
        }
#line 2656 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 87:
#line 1154 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            Stk_IF_ELSE.pop();
        }
#line 2664 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 88:
#line 1158 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        LABEL_l_num_st = LABEL_l_num_end;
        Stk_IF_ELSE.push(LABEL_l_num_st);      //当前的label存入栈中
        LABEL_l_num_end += 3;      //因为一个While语句一般会用到3个label
        out << IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":" << endl;
        
        out << IF_DEEP() + "if t" + to_string(VAR_t_num) + " == 0 goto l" + to_string(LABEL_l_num_st) << endl;
        out << IF_DEEP() + "goto l" + to_string(LABEL_l_num_st+1) << endl;

        out << IF_DEEP() + "l" + to_string(LABEL_l_num_st+1) + ":" << endl;
    }
#line 2680 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 89:
#line 1170 "source/xcw_parser.y" /* yacc.c:1652  */
    {
            LABEL_l_num_st = Stk_IF_ELSE.top();
            Stk_IF_ELSE.pop();
            out << IF_DEEP() + "\t" + "goto l" + to_string(LABEL_l_num_st+2) << endl;
            out << IF_DEEP() + "l" + to_string(LABEL_l_num_st) + ":" << endl;
        }
#line 2691 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 90:
#line 1179 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        // 不存在ELSE的情况
    }
#line 2699 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 91:
#line 1183 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        LABEL_l_num_st = Stk_IF_ELSE.top();
        //Stk_IF_ELSE.pop();
        out << IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":" << endl;
    }
#line 2709 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 92:
#line 1192 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        out << IF_DEEP() + "t" + to_string(VAR_t_num) + " = ";
        ToPtrnum(yyval)->Print();
        out << endl;
    }
#line 2719 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 97:
#line 1218 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        //表示函数调用中没有参数的情况
    }
#line 2727 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 98:
#line 1222 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* param_tmp = ToPtrnum(yyvsp[0]);
        if(param_tmp->IF_ptr_int){       //如果是常量
            out << IF_DEEP() + "param " << param_tmp->ptr_int << endl;
        }
        else{
            out << IF_DEEP() + "param " + param_tmp->ptr_str << endl;
        }

    }
#line 2742 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 99:
#line 1233 "source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* param_tmp = ToPtrnum(yyvsp[0]);
        if(param_tmp->IF_ptr_int){       //如果是常量
            out << IF_DEEP() + "param " << param_tmp->ptr_int << endl;
        }
        else{
            out << IF_DEEP() + "param " + param_tmp->ptr_str << endl;
        }
    }
#line 2756 "build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;


#line 2760 "build/xcw_parser.tab.c" /* yacc.c:1652  */
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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
#line 1253 "source/xcw_parser.y" /* yacc.c:1918  */


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
