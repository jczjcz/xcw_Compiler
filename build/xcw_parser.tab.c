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
#line 1 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:337  */

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
extern FILE *yyin;

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
        out << "-------------Ptr_print_in------------"<<endl;
        if(IF_ptr_int){
            out << "IF_ptr_int = " << IF_ptr_int << endl;
            out << ptr_int;
        }
        else{
            out << "IF_ptr_int = " << IF_ptr_int << endl;
            out << ptr_str;
        }
        out << endl;
        out << "-------------Ptr_print_out------------"<<endl;
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
string IF_DEEP_DEF(){
    string str_if_deep = "";
    if(DEEP!=0)
        str_if_deep += "\t";
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
stack<int> Array_LVal_length;     //在右侧作为表达式的时候，用来存储各个数组对象的长度
int path_length = 1;
int Array_loc;
int Array_dest, old_Array_dest;    //old_Array_dest用来临时存之前的Array_dest
int Array_deep;
stack<string> Array_name;

//----------------------------------------------------------

//-----------------IF_ELSE、While相关变量------------------------------
int LABEL_l_num_st = 0;       //表示一个IF语句开始，需要的label标记数   默认为0
int LABEL_l_num_end = 0;
stack<int> Stk_IF_ELSE;
//----------------------------------------------------------

//-----------------Continue、Break相关变量------------------------------
stack<int> Stk_Break;      // 和Stk_IF_ELSE相比，只用来储存While语句的label

//----------------------------------------------------------

//-----------------EXP相关------------------------------
string str_exp1,str_exp2;
//----------------------------------------------------------

int R_Array_Flag = 0;    //表示这个数组是否在右边表达式
string LVal_Assign_out;

int Cond_Array_Flag = 0;    //表示这个数组是否在条件表达式

stack<int> BRAC_Array_Flag;   //如果为空就说明不在括号内

// stack<int> Func_Array_Flag = 0;  //是否在函数参数里


int Array_in_Assign = 0;     //当且仅当数组被赋值的时候，不需要用临时变量去表示（包括定义和赋值）


//-----------------函数语句打印相关变量------------------------------
vector<string> Func_VarDecl;
vector<string> Func_Other;
vector<string> Func_Def;
//string Func_begin_flag = "funcbegin";     //这句话用来标注 函数开始
string def_out;       // 这个string 用来记录用于def的语句
string other_out;     // 这个string 用来记录其他的语句
string func_def_in = "";   // 进函数和出函数部分
string func_def_out = "";   
void Out_Print(){
    if(func_def_in != "")
        out << func_def_in << endl;
    for(int i = 0;i < Func_VarDecl.size();i++){
        out << Func_VarDecl[i] << endl;
    }
    for(int i = 0;i < Func_Other.size();i++){
        out << Func_Other[i] << endl;
    }
    //out << func_def_out << endl;
    Func_VarDecl.clear();
    Func_Other.clear();
    func_def_in = "";
}
//----------------------------------------------------------



#line 290 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:337  */
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
#ifndef YY_YY_HOME_XCW_XCW_COMPILER_BUILD_XCW_PARSER_TAB_H_INCLUDED
# define YY_YY_HOME_XCW_XCW_COMPILER_BUILD_XCW_PARSER_TAB_H_INCLUDED
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

#endif /* !YY_YY_HOME_XCW_XCW_COMPILER_BUILD_XCW_PARSER_TAB_H_INCLUDED  */



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
#define YYLAST   201

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  67
/* YYNRULES -- Number of rules.  */
#define YYNRULES  123
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  198

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
       0,   236,   236,   237,   241,   245,   252,   253,   258,   259,
     263,   267,   268,   272,   285,   317,   284,   340,   344,   352,
     358,   359,   363,   384,   383,   425,   464,   500,   463,   522,
     525,   533,   534,   538,   559,   558,   589,   593,   601,   610,
     618,   626,   635,   688,   730,   742,   777,   818,   862,   870,
     869,   903,   907,   918,   937,   945,   952,   959,   979,   978,
    1132,  1133,  1138,  1137,  1160,  1174,  1178,  1159,  1199,  1212,
    1216,  1198,  1240,  1243,  1247,  1251,  1267,  1266,  1308,  1312,
    1318,  1328,  1327,  1345,  1346,  1349,  1352,  1355,  1361,  1365,
    1364,  1382,  1386,  1390,  1395,  1394,  1424,  1431,  1438,  1423,
    1454,  1462,  1453,  1480,  1486,  1497,  1505,  1516,  1516,  1538,
    1557,  1577,  1593,  1613,  1618,  1645,  1675,  1683,  1710,  1737,
    1764,  1796,  1799,  1814
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
  "DeclOutFunc", "Decl", "ConstDecl", "ConstDefs", "ConstDef", "$@1",
  "$@2", "ConstInitVal", "ConstExp", "VarDecl", "VarDefs", "VarDef", "$@3",
  "$@4", "$@5", "ArrayInit", "ArrayExps", "ArrayExp", "$@6", "ArrayDef",
  "ArrayUnit", "InitVal", "Exp", "AddExp", "MulExp", "UnaryExp", "$@7",
  "PrimaryExp", "LVal", "$@8", "ArrayLVals", "ArrayLVal", "$@9", "FuncDef",
  "$@10", "$@11", "$@12", "$@13", "$@14", "$@15", "FuncFParams",
  "FuncFParam", "$@16", "ArrayParamDef", "Block", "$@17", "BlockItems",
  "BlockItem", "Stmt", "$@18", "$@19", "$@20", "$@21", "$@22", "$@23",
  "$@24", "IF_Else", "Cond", "$@25", "LOrExp", "LAndExp", "EqExp",
  "RelExp", "FuncRParams", YY_NULLPTR
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

#define YYPACT_NINF -169

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-169)))

#define YYTABLE_NINF -95

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      66,     6,    39,    48,    79,  -169,  -169,  -169,  -169,  -169,
      44,    30,  -169,    51,  -169,  -169,  -169,   103,  -169,     0,
    -169,    53,  -169,    59,    17,    75,  -169,    62,   103,   103,
       3,   103,   103,  -169,    61,   121,   126,  -169,  -169,  -169,
     103,  -169,  -169,  -169,    22,  -169,   103,    45,  -169,    51,
    -169,  -169,  -169,  -169,    72,    93,   121,  -169,  -169,   103,
     103,   103,   103,   103,  -169,  -169,    88,    99,  -169,  -169,
    -169,  -169,    99,   103,  -169,    72,  -169,  -169,   126,   126,
    -169,  -169,  -169,  -169,   109,    -3,  -169,   123,    -1,  -169,
       1,   103,  -169,    65,   128,  -169,    99,  -169,  -169,  -169,
     103,   125,  -169,   130,   111,  -169,  -169,  -169,   136,  -169,
      65,   136,  -169,  -169,    65,  -169,    65,   138,  -169,  -169,
     141,  -169,   142,  -169,   137,   140,  -169,  -169,  -169,   137,
      59,  -169,  -169,   134,   139,   144,  -169,  -169,  -169,  -169,
     145,   124,  -169,    89,  -169,  -169,  -169,   166,   167,  -169,
    -169,  -169,   103,  -169,   147,  -169,  -169,  -169,  -169,   148,
     103,   171,   103,   173,  -169,   150,  -169,   121,   160,   162,
     105,    69,  -169,  -169,    15,   103,   103,   103,   103,   103,
     103,   103,   103,    15,  -169,   162,   105,    69,    69,   121,
     121,   121,   121,  -169,   159,    15,  -169,  -169
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,     6,     7,     5,
      22,     0,    20,     0,    68,     1,     3,     0,    23,    25,
      36,     0,    19,     0,     0,     0,    11,     0,     0,     0,
      57,     0,     0,    54,     0,    18,    41,    44,    48,    55,
       0,    26,    37,    65,    22,    21,     0,     0,    10,     0,
      69,    51,    52,    49,     0,     0,    40,    53,    38,     0,
       0,     0,     0,     0,    24,    39,     0,    72,    13,    17,
      14,    12,    72,   121,    62,    59,    60,    56,    42,    43,
      45,    46,    47,    27,     0,     0,    73,     0,     0,   122,
       0,     0,    61,    29,    75,    66,     0,    15,    70,    50,
       0,     0,    34,     0,    30,    31,    33,    76,     0,    74,
      29,     0,   123,    63,    29,    28,     0,     0,    81,    67,
       0,    71,     0,    32,    80,    85,    16,    35,    78,    77,
       0,    96,   100,     0,     0,    89,    92,    86,     8,     9,
       0,    55,    93,     0,    83,    87,    79,     0,     0,   103,
     104,    88,     0,    91,     0,    82,    84,   107,   107,     0,
       0,     0,     0,     0,    90,     0,    97,   116,   108,   109,
     111,   113,   101,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,   110,   112,   114,   115,   117,
     119,   118,   120,   102,   105,     0,    99,   106
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -169,  -169,   184,  -169,  -169,  -103,  -169,   143,  -169,  -169,
    -169,   149,   -77,  -169,   168,  -169,  -169,  -169,   -93,  -169,
      73,  -169,   169,   -16,  -169,   -30,   -17,    76,   -24,  -169,
    -169,  -123,  -169,  -169,   115,  -169,  -169,  -169,  -169,  -169,
    -169,  -169,  -169,   122,   100,  -169,  -169,   -83,  -169,  -169,
      54,  -168,  -169,  -169,  -169,  -169,  -169,  -169,  -169,  -169,
      40,  -169,  -169,    24,    25,   -40,  -169
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,   137,     7,    25,    26,    87,   110,
      68,    34,     8,    11,    12,    40,    66,    93,   103,   104,
     105,   114,    19,    20,    64,   140,    56,    36,    37,    73,
      38,    39,    54,    75,    76,    91,     9,    21,    67,   108,
      27,    72,   111,    85,    86,   117,   129,   142,   125,   143,
     144,   145,   152,   154,   147,   174,   194,   148,   183,   196,
     161,   162,   168,   169,   170,   171,    90
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      35,    55,   141,    42,    51,    52,   184,    95,    57,    98,
      65,    99,    53,    17,    10,   193,   -58,   120,    28,    29,
     141,   122,   138,    30,    31,   119,   118,   197,   121,    35,
      17,    42,    96,    41,    96,    17,   100,    80,    81,    82,
     138,    32,   131,    89,   132,   133,   134,   135,   139,   136,
      46,   141,    33,   -64,    13,    18,    14,    17,    17,    24,
     141,   101,    43,   106,    22,    23,   139,    44,    28,    29,
     112,    50,   141,    30,    31,    58,   102,    18,    70,    15,
     106,     1,     2,     3,   106,    74,   106,   179,   180,   181,
     182,    32,    28,    29,     1,     2,     3,    30,    31,    83,
     118,   155,    33,    77,   130,     2,    28,    29,   128,    48,
      49,    30,    31,   146,    84,    32,   131,    94,   132,   133,
     134,   135,   159,   136,    59,    60,    33,   177,   178,    32,
     165,    61,    62,    63,    97,    78,    79,   187,   188,   113,
      33,   107,   115,    28,    29,   167,   116,   118,    30,    31,
      17,   118,   124,   126,   127,   130,     2,   -94,   167,   167,
     167,   167,   189,   190,   191,   192,    32,   131,   149,   132,
     133,   134,   135,   150,   136,   157,   158,    33,   151,   153,
     160,   166,   164,   172,   173,   175,   176,   195,    16,   123,
      92,    45,    71,    47,    88,    69,   109,   156,   163,   185,
       0,   186
};

static const yytype_int16 yycheck[] =
{
      17,    31,   125,    19,    28,    29,   174,    10,    32,    10,
      40,    10,     9,    13,     8,   183,    13,   110,     3,     4,
     143,   114,   125,     8,     9,   108,    11,   195,   111,    46,
      13,    47,    35,    33,    35,    13,    35,    61,    62,    63,
     143,    26,    27,    73,    29,    30,    31,    32,   125,    34,
      33,   174,    37,     9,    15,    33,     8,    13,    13,     8,
     183,    91,     9,    93,    34,    35,   143,     8,     3,     4,
     100,     9,   195,     8,     9,    14,    11,    33,    33,     0,
     110,    15,    16,    17,   114,    13,   116,    18,    19,    20,
      21,    26,     3,     4,    15,    16,    17,     8,     9,    11,
      11,    12,    37,    10,    15,    16,     3,     4,   124,    34,
      35,     8,     9,   129,    15,    26,    27,     8,    29,    30,
      31,    32,   152,    34,     3,     4,    37,    22,    23,    26,
     160,     5,     6,     7,    11,    59,    60,   177,   178,    14,
      37,    13,    12,     3,     4,   162,    35,    11,     8,     9,
      13,    11,    14,    12,    12,    15,    16,    33,   175,   176,
     177,   178,   179,   180,   181,   182,    26,    27,    34,    29,
      30,    31,    32,    34,    34,     9,     9,    37,    34,    34,
      33,    10,    34,    10,    34,    25,    24,    28,     4,   116,
      75,    23,    49,    24,    72,    46,    96,   143,   158,   175,
      -1,   176
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    15,    16,    17,    39,    40,    41,    43,    50,    74,
       8,    51,    52,    15,     8,     0,    40,    13,    33,    60,
      61,    75,    34,    35,     8,    44,    45,    78,     3,     4,
       8,     9,    26,    37,    49,    64,    65,    66,    68,    69,
      53,    33,    61,     9,     8,    52,    33,    60,    34,    35,
       9,    66,    66,     9,    70,    63,    64,    66,    14,     3,
       4,     5,     6,     7,    62,    63,    54,    76,    48,    49,
      33,    45,    79,    67,    13,    71,    72,    10,    65,    65,
      66,    66,    66,    11,    15,    81,    82,    46,    81,    63,
     104,    73,    72,    55,     8,    10,    35,    11,    10,    10,
      35,    63,    11,    56,    57,    58,    63,    13,    77,    82,
      47,    80,    63,    14,    59,    12,    35,    83,    11,    85,
      56,    85,    56,    58,    14,    86,    12,    12,    61,    84,
      15,    27,    29,    30,    31,    32,    34,    42,    43,    50,
      63,    69,    85,    87,    88,    89,    61,    92,    95,    34,
      34,    34,    90,    34,    91,    12,    88,     9,     9,    63,
      33,    98,    99,    98,    34,    63,    10,    64,   100,   101,
     102,   103,    10,    34,    93,    25,    24,    22,    23,    18,
      19,    20,    21,    96,    89,   101,   102,   103,   103,    64,
      64,    64,    64,    89,    94,    28,    97,    89
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    38,    39,    39,    40,    40,    41,    41,    42,    42,
      43,    44,    44,    45,    46,    47,    45,    48,    49,    50,
      51,    51,    52,    53,    52,    52,    54,    55,    52,    56,
      56,    57,    57,    58,    59,    58,    60,    60,    61,    62,
      63,    64,    64,    64,    65,    65,    65,    65,    66,    67,
      66,    66,    66,    66,    68,    68,    68,    69,    70,    69,
      71,    71,    73,    72,    75,    76,    77,    74,    78,    79,
      80,    74,    81,    81,    81,    82,    83,    82,    84,    84,
      84,    86,    85,    87,    87,    88,    88,    88,    89,    90,
      89,    89,    89,    89,    91,    89,    92,    93,    94,    89,
      95,    96,    89,    89,    89,    97,    97,    99,    98,   100,
     100,   101,   101,   102,   102,   102,   103,   103,   103,   103,
     103,   104,   104,   104
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       4,     1,     3,     3,     0,     0,     8,     1,     1,     3,
       1,     3,     1,     0,     4,     2,     0,     0,     8,     0,
       1,     1,     3,     1,     0,     4,     1,     2,     3,     1,
       1,     1,     3,     3,     1,     3,     3,     3,     1,     0,
       5,     2,     2,     2,     1,     1,     3,     1,     0,     3,
       1,     2,     0,     4,     0,     0,     0,     9,     0,     0,
       0,     9,     0,     1,     3,     2,     0,     6,     1,     2,
       0,     0,     4,     1,     2,     0,     1,     1,     2,     0,
       4,     2,     1,     1,     0,     5,     0,     0,     0,     9,
       0,     0,     7,     2,     2,     0,     2,     0,     2,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     0,     1,     3
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
        case 4:
#line 242 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        Out_Print();
    }
#line 1648 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 5:
#line 246 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        Out_Print();
    }
#line 1656 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 13:
#line 273 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        int num = *ToInt(yyvsp[0]);
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-2]), num, DEEP, 1);
        if(check_define(*ToStr(yyvsp[-2]))){       //如果在当前域中未被定义过
            Scope.push_back(tmp);
        }
        else{
            string err = "\"" +  *ToStr(yyvsp[-2]) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
#line 1672 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 14:
#line 285 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        R_Array_Flag = 1;
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
        // out << "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;
        def_out = IF_DEEP_DEF() + "var" + to_string(n*INTSIZE) + " T" + to_string(VAR_T_num);
        Func_VarDecl.push_back(def_out);

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
#line 1708 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 15:
#line 317 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            Array_deep = 0;    //将深度初始化为0
            Array_loc = 0;    //将下标初始化为0，path_length是整个数组的长度
            Array_dest = Array_loc + path_length;
        }
#line 1718 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 16:
#line 323 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            //没填满的元素用0填充
            for(; Array_loc < Array_dest; Array_loc++){
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                //out << ir_name << " = " << 0 << endl;
                other_out = IF_DEEP() + ir_name + " = 0";
                Func_Other.push_back(other_out);
            }
            VAR_T_num ++;     //定义结束后，把变量名数字 + 1
            Array_dim.clear();     //初始化数组维度
            R_Array_Flag = 0;
        }
#line 1737 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 18:
#line 345 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
        //out << "ADD EXP" << endl;
    }
#line 1746 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 22:
#line 364 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[0]), "0", DEEP, 0);
        tmp.IR_name = "T" + to_string(VAR_T_num);
        //tmp.Print_IDENT();
        if(check_define(*ToStr(yyvsp[0]))){       //如果在当前域中未被定义过
            Scope.push_back(tmp);
            //out << IF_DEEP() + "var T" << VAR_T_num << endl;
            def_out = IF_DEEP_DEF() + "var T" + to_string(VAR_T_num);
            //out << IF_DEEP() + "T" << VAR_T_num << " = " << 0 << endl;
            other_out = IF_DEEP() + "T" + to_string(VAR_T_num) + " = 0";
            Func_VarDecl.push_back(def_out);
            Func_Other.push_back(other_out);
            VAR_T_num ++ ;
        }
        else{
            string err = "\"" +  *ToStr(yyvsp[0]) + "\" already defined in this scope.";
            yyerror(err);
        }
    }
#line 1770 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 23:
#line 384 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        R_Array_Flag = 1;
    }
#line 1778 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 24:
#line 388 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            //out << "InitVal"<<endl;
            if(ToPtrnum(yyvsp[0])->IF_ptr_int == 1){       //传递的是常量
                int num = ToPtrnum(yyvsp[0])->ptr_int;
                IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-3]), to_string(num), DEEP, 0);  
                tmp.IR_name = "T" + to_string(VAR_T_num);   
                Scope.push_back(tmp);
                // out << IF_DEEP() + "var T" << VAR_T_num << endl;
                def_out = IF_DEEP_DEF() + "var T" + to_string(VAR_T_num);
                // out << IF_DEEP() + "T" << VAR_T_num << " = " << num << endl;
                other_out = IF_DEEP() + "T" + to_string(VAR_T_num) + " = " + to_string(num);
                Func_VarDecl.push_back(def_out);
                Func_Other.push_back(other_out);
                VAR_T_num ++ ;
            }
            else{              //传递的是变量
                string num = ToPtrnum(yyvsp[0])->ptr_str;

                IDENT_scope tmp = IDENT_scope(*ToStr(yyvsp[-3]), num, DEEP, 0);
                tmp.IR_name = "T" + to_string(VAR_T_num);   
                Scope.push_back(tmp);
                // out << IF_DEEP() + "var T" << VAR_T_num << endl;
                def_out = IF_DEEP_DEF() + "var T" + to_string(VAR_T_num);
                // out << IF_DEEP() + "T" << VAR_T_num << " = " << num << endl;

                // other_out = "t" + to_string(VAR_t_num) + " = " + num;
                // Func_Other.push_back(other_out);

                other_out = IF_DEEP() + "T" + to_string(VAR_T_num) + " = " + num;
                Func_VarDecl.push_back(def_out);
                Func_Other.push_back(other_out);
                VAR_T_num ++ ;
                // VAR_t_num ++ ;
            }
            R_Array_Flag = 0;
            //tmp.Print_IDENT();
        }
#line 1820 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 25:
#line 426 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
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
        // out << IF_DEEP() + "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;
        def_out = IF_DEEP_DEF() + "var " + to_string(n*INTSIZE) + " T" + to_string(VAR_T_num);
        Func_VarDecl.push_back(def_out);
        
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
        }
        VAR_T_num ++;

        Array_dim.clear();
    }
#line 1862 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 26:
#line 464 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        R_Array_Flag = 1;
        
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
        // out << IF_DEEP() + "var " << n*INTSIZE << " T" << to_string(VAR_T_num) <<endl;
        def_out = IF_DEEP_DEF() + "var " + to_string(n*INTSIZE) + " T" + to_string(VAR_T_num);
        Func_VarDecl.push_back(def_out);

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
#line 1902 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 27:
#line 500 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            Array_dest = Array_loc + path_length;
        }
#line 1910 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 28:
#line 504 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            //没填满的元素用0填充
            for(; Array_loc < Array_dest; Array_loc++){
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                //out << IF_DEEP() + ir_name << " = " << 0 << endl;
                other_out = IF_DEEP() + ir_name + " = 0";
                Func_Other.push_back(other_out);
            }
            VAR_T_num ++;     //定义结束后，把变量名数字 + 1
            Array_dim.clear();     //初始化数组维度

            R_Array_Flag = 0;
        }
#line 1930 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 29:
#line 522 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // 类似于{}的情况，推出空值
    }
#line 1938 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 526 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //类似于 {1},{1,2}
        yyval = yyvsp[0];
    }
#line 1947 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 539 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //为常量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum(yyvsp[0])->ptr_int);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            // out << IF_DEEP() + ir_name << " = " << ToPtrnum($1)->ptr_int << endl;
            other_out = IF_DEEP() + ir_name + " = " + to_string(ToPtrnum(yyvsp[0])->ptr_int);
            Func_Other.push_back(other_out);
        }       
        else{          //为变量，加入到数组中
            Ptr_num tmp_ptr = Ptr_num(ToPtrnum(yyvsp[0])->ptr_str);     //构造vector中的元素
            string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
            Scope.back().IDENT_array->push_back(tmp_ptr);
            // out << IF_DEEP() + ir_name << " = " << ToPtrnum($1)->ptr_str << endl;
            other_out = IF_DEEP() + ir_name + " = " + ToPtrnum(yyvsp[0])->ptr_str;
            Func_Other.push_back(other_out);
        }
        Array_loc ++;     //位置向前进1
    }
#line 1971 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 559 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "LCURLY" << endl;
        path_length = path_length / Array_dim[Array_deep];
        Array_deep ++;      //遇到左括号，深度+1
        old_Array_dest = Array_dest;
        Array_dest = Array_loc + path_length;
    }
#line 1983 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 35:
#line 567 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            //out << "RCURLY" << endl;
            //out << "------- Array_dest = "<<Array_dest<<endl;
            for(; Array_loc < Array_dest; Array_loc++){
                //out << "Array_loc = " << Array_loc << endl;
                Ptr_num tmp_ptr = Ptr_num("0");     //构造vector中的元素
                string ir_name = "T" + to_string(VAR_T_num) + "[" + to_string(4 * Array_loc) + "]";
                Scope.back().IDENT_array->push_back(tmp_ptr);
                // out << IF_DEEP() + ir_name << " = " << 0 << endl;
                other_out = IF_DEEP() + ir_name + " = 0";
                Func_Other.push_back(other_out);   
            }
            Array_dest = old_Array_dest;
            Array_deep --;      //遇到右括号，深度-1
            path_length = path_length * Array_dim[Array_deep];
        }
#line 2004 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 590 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
    }
#line 2012 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 594 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        ToPtrnum(yyvsp[-1])->ptr_int = ToPtrnum(yyvsp[-1])->ptr_int * ToPtrnum(yyvsp[0])->ptr_int;
        yyval = yyvsp[-1];
    }
#line 2021 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 38:
#line 602 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[-1];
        Array_dim.push_back(ToPtrnum(yyvsp[-1])->ptr_int);      //把数组数据放到Array_dim中，记录数组维度信息
        
    }
#line 2031 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 611 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // out << "Exp"<<endl;
        yyval = yyvsp[0];
    }
#line 2040 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 619 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // out << "ADDExp"<<endl;
        yyval = yyvsp[0];            
    }
#line 2049 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 41:
#line 627 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval =yyvsp[0];
        // if(!ToPtrnum($1)->IF_ptr_int){
        //     other_out = "ADD1-------------tmp_ptr_old = "  + ToPtrnum($1)->ptr_str ; 
        //     Func_Other.push_back(other_out);
        // }
        // ToPtrnum($1)->Print();
    }
#line 2062 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 636 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // if(!ToPtrnum($1)->IF_ptr_int){
        //     other_out = "tmp_ptr_old = "  + ToPtrnum($1)->ptr_str ; 
        //     Func_Other.push_back(other_out);
        // }
        // if(!ToPtrnum($3)->IF_ptr_int){
        //     other_out = "tmp_ptr_old = "  + ToPtrnum($3)->ptr_str ; 
        //     Func_Other.push_back(other_out);
        // }
        //out << "AddExp ADD MulExp" << endl;
        // ToPtrnum($1)->Print();
        // ToPtrnum($3)->Print();
        Ptr_num* add_1 = ToPtrnum(yyvsp[-2]);
        Ptr_num* mul_1 = ToPtrnum(yyvsp[0]);
        Ptr_num* tmp_ptr = new Ptr_num;
        if(add_1->IF_ptr_int && mul_1->IF_ptr_int){      //两个都是常量
            tmp_ptr->ptr_int = add_1->ptr_int + mul_1->ptr_int;
            tmp_ptr->IF_ptr_int = 1;
        }
        else{             // 至少有一个是变量
            //如果其中有常量，先强制把常量转成string类型
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
                //out << "str_exp1 = " << str_exp1 << endl;
            }
            else{
                str_exp1 = add_1->ptr_str;
                //out << "str_exp1 = " << str_exp1 << endl;
            }
            //ToPtrnum($1)->Print();
            if(mul_1->IF_ptr_int){
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }

            // out << "str_exp1 = " << str_exp1 << endl;
            // out << "str_exp2 = " << str_exp2 << endl;

            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            //out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str_exp1 << " + " << str_exp2 << endl;      // 输出类似于 t0 = T0 + 1
            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " + " + str_exp2;
            Func_Other.push_back(other_out);
        }
        yyval = tmp_ptr; 
    }
#line 2119 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 43:
#line 689 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
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
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
            }
            else{
                str_exp1 = add_1->ptr_str;
                //out <<  "str_exp1 = "<<str_exp1 << endl;
            }
            if(mul_1->IF_ptr_int){
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //out << "str_exp1 = "<<str_exp1 << "  str_exp2 = " << str_exp2 << endl;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            // out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str_exp1 << " - " << str_exp2 << endl;      // 输出类似于 t0 = T0 + 1
            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " - " + str_exp2;
            Func_Other.push_back(other_out);
        }
        yyval = tmp_ptr; 
    }
#line 2161 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 44:
#line 731 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
        // if(!ToPtrnum($1)->IF_ptr_int){
            
        //     other_out = "MulExp-----------tmp_ptr_old = "  + ToPtrnum($1)->ptr_str ; 
        //     Func_Other.push_back(other_out);
        // }
        //ToPtrnum($1)->Print();
        // ToPtrnum($1)->Print();
        // out << "MulExp" << endl;
    }
#line 2177 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 45:
#line 743 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
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
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
            }
            else{
                str_exp1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            // out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str_exp1 << " * " << str_exp2 << endl;      // 输出类似于 t0 = T0 + 1
            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " * " + str_exp2;
            Func_Other.push_back(other_out);
        }
        yyval = tmp_ptr; 
    }
#line 2216 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 778 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
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
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
            }
            else{
                str_exp1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                if(mul_1->ptr_int == 0){       //除0报错
                    yyerror("Integer division by zero.");
                }
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            // out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str_exp1 << " / " << str_exp2 << endl;      // 输出类似于 t0 = T0 + 1
            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " / " + str_exp2;
            Func_Other.push_back(other_out);
        }
        yyval = tmp_ptr; 
    }
#line 2261 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 47:
#line 819 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
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
            if(add_1->IF_ptr_int){
                str_exp1 = to_string(add_1->ptr_int);
            }
            else{
                str_exp1 = add_1->ptr_str;
            }
            if(mul_1->IF_ptr_int){
                if(mul_1->ptr_int == 0){       //除0报错
                    yyerror("Integer mod by zero.");
                }
                str_exp2 = to_string(mul_1->ptr_int);
            }
            else{
                str_exp2 = mul_1->ptr_str;
            }
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            tmp_ptr->ptr_str = "t" +  to_string(VAR_t_num);   // 生成中间变量
            VAR_t_num ++;
            //是字符型
            tmp_ptr->IF_ptr_int = 0;
            // out << IF_DEEP() + tmp_ptr->ptr_str << " = " << str_exp1 << " % " << str_exp2 << endl;      // 输出类似于 t0 = T0 + 1
            other_out = IF_DEEP() + tmp_ptr->ptr_str + " = " + str_exp1 + " % " + str_exp2;
            Func_Other.push_back(other_out);
        }
        yyval = tmp_ptr; 
    }
#line 2306 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 863 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
        //ToPtrnum($1)->Print();
        // out << "PrimaryExp" << endl;
  //      
    }
#line 2317 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 870 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        BRAC_Array_Flag.push(1);
    }
#line 2325 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 874 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "IDENT LPAREN FuncRParams RPAREN" << endl;
        IDENT_scope* tmp = find_define(*ToStr(yyvsp[-4]));

        if( tmp == nullptr){          //变量尚未定义
            string err = "\"" +  *ToStr(yyvsp[-4]) + "\" was not declared in this scope.";
            yyerror(err);
        }

        // out << IF_DEEP() + "t" + to_string(VAR_t_num) + " = call f_" + *(ToStr($1)) << endl;
        if(tmp->IDENT_if_ret_int == 1){        //返回值是INT的函数
            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
            Func_VarDecl.push_back(def_out);
            other_out = IF_DEEP() + "t" + to_string(VAR_t_num) + " = call f_" + *(ToStr(yyvsp[-4]));
            Func_Other.push_back(other_out);

            Ptr_num* tmp_ptr = new Ptr_num;
            tmp_ptr->ptr_str = "t" + to_string(VAR_t_num);
            tmp_ptr->IF_ptr_int = 0;

            VAR_t_num ++;
            yyval = tmp_ptr;
        }
        else{
            other_out = IF_DEEP() + "call f_" + *(ToStr(yyvsp[-4]));
            Func_Other.push_back(other_out);
        }
        BRAC_Array_Flag.pop();
    }
#line 2359 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 51:
#line 904 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[-1];
    }
#line 2367 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 52:
#line 908 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* tmp_ptr = ToPtrnum(yyvsp[0]);
        if(tmp_ptr->IF_ptr_int){
            tmp_ptr->ptr_int = - tmp_ptr->ptr_int;
        }
        else{
            tmp_ptr->ptr_str = "-" + tmp_ptr->ptr_str;
        }
        yyval = yyvsp[0];
    }
#line 2382 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 53:
#line 919 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* tmp_ptr = ToPtrnum(yyvsp[0]);
        if(tmp_ptr->IF_ptr_int){
            if(tmp_ptr->ptr_int == 0)
                tmp_ptr->ptr_int = 1;
            else    
                tmp_ptr->ptr_int = 0;
        }
        else{
            tmp_ptr->ptr_str = "!" + tmp_ptr->ptr_str;
        }
        yyval = yyvsp[0];
    }
#line 2400 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 54:
#line 938 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "NUMBER" << endl;
        Ptr_num* tmp_ptr = new Ptr_num;
        tmp_ptr->ptr_int = *ToInt(yyvsp[0]);
        tmp_ptr->IF_ptr_int = 1;
        yyval = tmp_ptr;
    }
#line 2412 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 55:
#line 946 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // out << "LVal" << endl;
        // ToPtrnum($1)->Print();
        yyval = yyvsp[0];
        
    }
#line 2423 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 56:
#line 953 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[-1];
    }
#line 2431 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 57:
#line 960 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "IDENT" << endl;
        IDENT_scope* tmp = find_define(*ToStr(yyvsp[0]));
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
#line 2454 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 58:
#line 979 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "IDENT  ArrayLVals" << endl;
        // other_out = "IDENT  ArrayLVals  IDENT name = " + (*ToStr($1));
        // Func_Other.push_back(other_out);
        IDENT_scope* tmp = find_define(*ToStr(yyvsp[0]));    //搜索这个数组的定义
        Array_name.push(tmp->IR_name);
        // other_out = "tmp->IR_name = " + tmp->IR_name;
        // Func_Other.push_back(other_out);
        // Array_dim.clear();
        // for(int i = 0;i < (*tmp).IDENT_dim_array->size();i++){
        //     Array_dim.push_back((*(*tmp).IDENT_dim_array)[i]);
        // }

        Array_LVal_length.push(0);
    }
#line 2474 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 59:
#line 995 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {        //a[2][3]     a[1][b]  

            IDENT_scope* tmp_lval = find_define(*ToStr(yyvsp[-2]));    //搜索这个数组的定义
            Array_dim.clear();
            for(int i = 0;i < (*tmp_lval).IDENT_dim_array->size();i++){
                Array_dim.push_back((*(*tmp_lval).IDENT_dim_array)[i]);    //原数组的维度
            }
            // Ptr_num tmp_ptr, tmp_ptr_new, tmp_ptr_old;
            Ptr_num* tmp_ptr = new Ptr_num;      //尝试写成指针
            Ptr_num* tmp_ptr_new = new Ptr_num;
            Ptr_num* tmp_ptr_old = new Ptr_num;
            // auto tmp_ptr_old = new Ptr_num;
            int ptr_size = INTSIZE;
            
            // other_out = "Array_dim.size = " + to_string(Array_LVal_dim.size());
            // Func_Other.push_back(other_out);
            //  Array_LVal_dim 存储了各个数组具体的维度，Array_dim存储了原始的维度
            int array_len = Array_LVal_length.top();
            for(int i = 0; i < array_len ;i ++){
                tmp_ptr = & Array_LVal_dim.back();
                Array_LVal_dim.pop_back();
                if(tmp_ptr->IF_ptr_int){     //是整数
                    tmp_ptr_new->IF_ptr_int = 1;
                    tmp_ptr_new->ptr_int = tmp_ptr->ptr_int * ptr_size;
                    if(i != 0){     //第一次，不用考虑和之前相加
                        if(tmp_ptr_old->IF_ptr_int){     //如果前面的也是INT
                            tmp_ptr_old->ptr_int += tmp_ptr_new->ptr_int;
                        }
                        else{
                            def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
                            Func_VarDecl.push_back(def_out);
                            // out << IF_DEEP() + "t" + to_string(VAR_t_num ) << " = "<< tmp_ptr_new.ptr_int << " + " << tmp_ptr_old.ptr_str << endl;
                            other_out =  IF_DEEP() + "t" + to_string(VAR_t_num ) + " = " + to_string(tmp_ptr_new->ptr_int) + " + " + tmp_ptr_old->ptr_str;
                            Func_Other.push_back(other_out);
                            tmp_ptr_old->ptr_str = "t" + to_string(VAR_t_num);
                            VAR_t_num ++;
                        }
                    }
                    else{
                        tmp_ptr_old->ptr_int = tmp_ptr_new->ptr_int;
                        tmp_ptr_old->IF_ptr_int = tmp_ptr_new->IF_ptr_int;
                    }
                }
                else{            //其中有一个是变量
                    // other_out = "not INT ---------";
                    // Func_Other.push_back(other_out);
                    
                    if(i != 0){
                        // other_out = "NOT_INT-start-------tmp_ptr_old->str = " + tmp_ptr_old->ptr_str;
                        // Func_Other.push_back(other_out);
                    }
                    
                    def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
                    Func_VarDecl.push_back(def_out);
                    tmp_ptr_new->IF_ptr_int = 0;
                    tmp_ptr_new->ptr_str = "t" + to_string(VAR_t_num);
                    
                    // if(i != 0){
                    //     other_out = "NOT_INT-start-------tmp_ptr_new->str = " + tmp_ptr_new->ptr_str;
                    //     Func_Other.push_back(other_out);
                    // }
                    if(i != 0 && tmp_ptr_old->IF_ptr_int){
                        tmp_ptr_old->ptr_str = to_string(tmp_ptr_old->ptr_int);    //强制转换为string类型
                        tmp_ptr_old->IF_ptr_int = 0;    
                    }
                    VAR_t_num ++;
                    // tmp_ptr_old->Print();
                    // if(i != Array_LVal_dim.size()-1){
                    //     other_out = "NOT_INT-start-------tmp_ptr_old->str = " + tmp_ptr_old->ptr_str;
                    //     Func_Other.push_back(other_out);
                    // }
                    other_out = IF_DEEP() + tmp_ptr_new->ptr_str + " = " + tmp_ptr->ptr_str + " * " + to_string(ptr_size);
                    Func_Other.push_back(other_out);
                    if(i != 0){     //第一次不用考虑和之前相加
                        // out <<IF_DEEP() + "t" + to_string(VAR_t_num ) << " = "<< tmp_ptr_new.ptr_str << " + " << tmp_ptr_old.ptr_str << endl;
                        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
                        Func_VarDecl.push_back(def_out);

                        // other_out = "tmp_ptr_new->str = " + tmp_ptr_new->ptr_str;
                        // Func_Other.push_back(other_out);
                        // other_out = "tmp_ptr_old->str = " + tmp_ptr_old->ptr_str;
                        // Func_Other.push_back(other_out);
                        other_out = IF_DEEP() + "t" + to_string(VAR_t_num) + " = " + tmp_ptr_new->ptr_str + " + " + tmp_ptr_old->ptr_str;
                        Func_Other.push_back(other_out);
                        tmp_ptr_old->ptr_str = "t" + to_string(VAR_t_num);
                        VAR_t_num ++;
                    }
                    else{     //注意这里需要深度复制
                        tmp_ptr_old->ptr_str = tmp_ptr_new->ptr_str;
                        tmp_ptr_old->IF_ptr_int = tmp_ptr_new->IF_ptr_int;
                    }
                    // other_out = "end-------tmp_ptr_old->str = " + tmp_ptr_old->ptr_str;
                    // Func_Other.push_back(other_out);
                }
                ptr_size *= Array_dim.back();     //更新ptr_size
                Array_dim.pop_back();
            }
            // Array_LVal_dim.clear();
            if(tmp_ptr_old->IF_ptr_int){
                tmp_ptr_old->ptr_str = Array_name.top() + "[" + to_string(tmp_ptr_old->ptr_int) + "]";
                tmp_ptr_old->IF_ptr_int = 0;     //最后的结果一定是一个字符串类型
            }
            else{
                tmp_ptr_old->ptr_str = Array_name.top() + "[" + tmp_ptr_old->ptr_str + "]";
                tmp_ptr_old->IF_ptr_int = 0;
            }    
            
            // other_out = "Cond_Array_Flag = " + to_string(Cond_Array_Flag);
            // Func_Other.push_back(other_out);
            // other_out = "R_Array_Flag = " + to_string(R_Array_Flag);
            // Func_Other.push_back(other_out);
            // other_out = "BRAC_Array_Flag = " + to_string(BRAC_Array_Flag);
            // Func_Other.push_back(other_out);

            if(Cond_Array_Flag == 1 || R_Array_Flag == 1 || (!BRAC_Array_Flag.empty())){      //表示位于右侧的数组

                def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
                Func_VarDecl.push_back(def_out);
                other_out = IF_DEEP() + "t" + to_string(VAR_t_num ) + " = " + tmp_ptr_old->ptr_str;
                Func_Other.push_back(other_out);

                tmp_ptr_old->ptr_str = "t" + to_string(VAR_t_num);
                VAR_t_num ++;
                yyval = tmp_ptr_old;
                // other_out = "tmp_ptr_old = "  + tmp_ptr_old.ptr_str ; 
                // Func_Other.push_back(other_out);
            }
            else{
                yyval = tmp_ptr_old;
            }
            // tmp_ptr_old->Print();
            Array_name.pop();
            Array_LVal_length.pop();
        }
#line 2613 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 62:
#line 1138 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        BRAC_Array_Flag.push(1);
        // other_out = "set BRAC_Array_Flag = 1";
        // Func_Other.push_back(other_out);
    }
#line 2623 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 63:
#line 1144 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // other_out = "LBRAC Exp PBRAC and EXP = " + ((ToPtrnum($3))->ptr_str);
        // Func_Other.push_back(other_out);
        Array_LVal_dim.push_back(*(ToPtrnum(yyvsp[-1])));    //存入进行引用的维度
        BRAC_Array_Flag.pop();
        Array_LVal_length.top() += 1;      //长度 + 1

        //ToPtrnum($3)->Print();
        // other_out = "set BRAC_Array_Flag = 0";
        // Func_Other.push_back(other_out);
    }
#line 2639 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 64:
#line 1160 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
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
#line 2657 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 65:
#line 1174 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            DEEP ++;
        }
#line 2665 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 66:
#line 1178 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            DEEP --;
            //IDENT_scope* tmp_ptr = find_define(*ToStr($2));    //找到函数变量的指针
            //tmp_ptr->IDENT_func_param_num = 
            //out << "f_" << *ToStr($2) << " [" << VAR_p_num << "]" << endl;  
            func_def_in =  "f_" + *ToStr(yyvsp[-5]) + " [" + to_string(VAR_p_num) + "]";
        }
#line 2677 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 67:
#line 1186 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            
            // out << "\treturn 0" << endl;
            // out << "end " << "f_" << *ToStr($2) << endl;
            other_out = "\treturn 0";
            Func_Other.push_back(other_out);
            other_out = "end f_" + *ToStr(yyvsp[-7]);
            Func_Other.push_back(other_out);
            //声明结束后，把记录参数数量的 VAR_p_num 初始化
            VAR_p_num = 0;
            
        }
#line 2694 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 68:
#line 1199 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
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
    }
#line 2711 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 69:
#line 1212 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            DEEP ++;
        }
#line 2719 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 70:
#line 1216 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            DEEP --;
            // out << "f_" << *ToStr($2) << " [" << VAR_p_num << "]" << endl;
            func_def_in = "f_" + (*ToStr(yyvsp[-5])) + " [" + to_string(VAR_p_num) + "]";
            //Func_Other.push_back(other_out);
        }
#line 2730 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 71:
#line 1223 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            
            // out << "\treturn" << endl;
            // out << "end " << "f_" << *ToStr($2) << endl;

            other_out = "\treturn";
            Func_Other.push_back(other_out);
            other_out = "end f_" + *ToStr(yyvsp[-7]);
            Func_Other.push_back(other_out);

            //声明结束后，把记录参数数量的 VAR_p_num 初始化
            VAR_p_num = 0;
        }
#line 2748 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 72:
#line 1240 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //表示没有参数的情况
    }
#line 2756 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 73:
#line 1244 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {

    }
#line 2764 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 75:
#line 1252 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    { 
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
#line 2783 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 76:
#line 1267 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        Array_dim.push_back(10);   //传入一个假参数
    }
#line 2791 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 77:
#line 1271 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {      //void d(int d[])
        // 检查是否出现过
        // other_out = "INT IDENT LBRAC RBRAC ArrayParamDef";
        // Func_Other.push_back(other_out);
        // out << "INT IDENT LBRAC RBRAC ArrayParamDef" << endl;

        if(!check_define(*ToStr(yyvsp[-4]))){
            string err = "\"" +  *ToStr(yyvsp[-5]) + "\" already defined in this scope.";
            yyerror(err);
        }

        IDENT_scope* tmp_ptr = new IDENT_scope(*ToStr(yyvsp[-4]), "0", DEEP, 0);    //是一个变量
        tmp_ptr->IDENT_if_array = 1;    //是数组
        tmp_ptr->IR_name = "p" + to_string(VAR_p_num);

        int n = ToPtrnum(yyvsp[0])->ptr_int;
        // out << "n = "<<n<<endl;

        vector<Ptr_num>* Ident_array = new vector<Ptr_num>;
        vector<int>* Ident_dim_array = new vector<int>;

        tmp_ptr->Array_size = n;
        for(int i = 0; i < Array_dim.size();i++){
            Ident_dim_array->push_back(Array_dim[i]);
            // out << "Array_dim[i] = " << Array_dim[i] << endl;
        }
        tmp_ptr->IDENT_dim_array = Ident_dim_array;

        //tmp.Print_IDENT();
        VAR_p_num ++;
        Scope.push_back(*tmp_ptr);

        Array_dim.clear();
    }
#line 2830 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 78:
#line 1309 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
    }
#line 2838 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 79:
#line 1313 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {   //   例如[3][4]
        ToPtrnum(yyvsp[-1])->ptr_int = ToPtrnum(yyvsp[-1])->ptr_int * ToPtrnum(yyvsp[0])->ptr_int;
        yyval = yyvsp[-1];
    }
#line 2847 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 80:
#line 1318 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //也有可能为空
        // ToPtrnum($$)->ptr_int = 10;
        Ptr_num* tmp_dim = new Ptr_num(1);     //传一个假的n上去 
        yyval = (void*)tmp_dim;
    }
#line 2858 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 81:
#line 1328 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        DEEP ++;     // 深度+1，保证这些新加入的元素属于当前域中
    }
#line 2866 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 82:
#line 1332 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
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
#line 2881 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 85:
#line 1349 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //暂时先考虑内容为空的情况
    }
#line 2889 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 86:
#line 1353 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
    }
#line 2896 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 87:
#line 1356 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
    }
#line 2903 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 88:
#line 1362 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
    }
#line 2910 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 89:
#line 1365 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        BRAC_Array_Flag.push(1);
    }
#line 2918 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 90:
#line 1369 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        if(ToPtrnum(yyvsp[-1])->IF_ptr_int){       //为常量
            // out << IF_DEEP() + "return " << ToPtrnum($2)->ptr_int << endl;
            other_out = IF_DEEP() + "return " + to_string(ToPtrnum(yyvsp[-1])->ptr_int);
            Func_Other.push_back(other_out);
        }
        else{
            // out << IF_DEEP() + "return " + ToPtrnum($2)->ptr_str << endl;
            other_out = IF_DEEP() + "return " + ToPtrnum(yyvsp[-1])->ptr_str;
            Func_Other.push_back(other_out);
        }
        BRAC_Array_Flag.pop();
    }
#line 2936 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 91:
#line 1383 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //类似于 直接调用void函数，如f(1,2);
    }
#line 2944 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 92:
#line 1387 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // 一个;的情况
    }
#line 2952 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 93:
#line 1391 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // 内部还是一个语句块
    }
#line 2960 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 94:
#line 1395 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // out << IF_DEEP() + ToPtrnum($1)->ptr_str;
        LVal_Assign_out = IF_DEEP() + ToPtrnum(yyvsp[0])->ptr_str;
        R_Array_Flag = 1;
    }
#line 2970 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 95:
#line 1401 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            //类似于 a = b   LVal 返回一个 Ptr_num类型的指针tmp_ptr，此时LVal传上来的必定是一个ptr_str
            
            if(ToPtrnum(yyvsp[-1])->IF_ptr_int == 1){       //传递的是常量
                int num = ToPtrnum(yyvsp[-1])->ptr_int;
                // out << " = " << num << endl;
                LVal_Assign_out += (" = " + to_string(num));
                Func_Other.push_back(LVal_Assign_out);
            }
            else{              //Exp传递的是变量
                string num = ToPtrnum(yyvsp[-1])->ptr_str;

                // 用一个临时变量来存右边的值，主要用于数组的情况
                // string other_out_tmp = IF_DEEP() + "t" + to_string(VAR_t_num) + " = " + num;
                // Func_Other.push_back(other_out_tmp);
                // out << " = " << num << endl;
                LVal_Assign_out += (" = " + num);
                Func_Other.push_back(LVal_Assign_out);
            }
            R_Array_Flag = 0;

        }
#line 2997 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 96:
#line 1424 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        LABEL_l_num_st = LABEL_l_num_end;
        Stk_IF_ELSE.push(LABEL_l_num_st);      //当前的label存入栈中
        // other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":";    //没什么用，但看起来更整齐
        // Func_Other.push_back(other_out);
    }
#line 3008 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 97:
#line 1431 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            
            LABEL_l_num_end += 3;      //因为一个IF语句一般会用到3个label
            other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+1) + ":";
            Func_Other.push_back(other_out);
        }
#line 3019 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 98:
#line 1438 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    { 
            LABEL_l_num_st = Stk_IF_ELSE.top();
            
            // out << IF_DEEP() + "\t" + "goto l" + to_string(LABEL_l_num_st+2) << endl;  //goto l2
            other_out = IF_DEEP() + "\t" + "goto l" + to_string(LABEL_l_num_st+2);
            Func_Other.push_back(other_out);
            // out << IF_DEEP() + "l" + to_string(LABEL_l_num_st) + ":" << endl;
            other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st) + ":";
            Func_Other.push_back(other_out);    
            //
        }
#line 3035 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 99:
#line 1450 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            Stk_IF_ELSE.pop();
        }
#line 3043 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 100:
#line 1454 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        LABEL_l_num_st = LABEL_l_num_end;
        Stk_IF_ELSE.push(LABEL_l_num_st);      //当前的label存入栈中
        Stk_Break.push(LABEL_l_num_st);
        other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":";
        Func_Other.push_back(other_out);
    }
#line 3055 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 101:
#line 1462 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            
            LABEL_l_num_end += 3;      //因为一个While语句一般会用到3个label
            other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+1) + ":";
            Func_Other.push_back(other_out);  
        }
#line 3066 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 102:
#line 1469 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
            LABEL_l_num_st = Stk_IF_ELSE.top();
            Stk_IF_ELSE.pop();
            Stk_Break.pop();
            // out << IF_DEEP() + "\t" + "goto l" + to_string(LABEL_l_num_st+2) << endl;
            other_out = IF_DEEP() + "\t" + "goto l" + to_string(LABEL_l_num_st+2);
            Func_Other.push_back(other_out);  
            // out << IF_DEEP() + "l" + to_string(LABEL_l_num_st) + ":" << endl;
            other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st) + ":";
            Func_Other.push_back(other_out);  
        }
#line 3082 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 103:
#line 1481 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        LABEL_l_num_st = Stk_Break.top();
        other_out = IF_DEEP() + "goto l" + to_string(LABEL_l_num_st);
        Func_Other.push_back(other_out);  
    }
#line 3092 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 104:
#line 1487 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        LABEL_l_num_st = Stk_Break.top();
        other_out = IF_DEEP() + "goto l" + to_string(LABEL_l_num_st+2);
        Func_Other.push_back(other_out);  
    }
#line 3102 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 105:
#line 1497 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // 不存在ELSE的情况
        LABEL_l_num_st = Stk_IF_ELSE.top();
        //Stk_IF_ELSE.pop();
        // out << IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":" << endl;
        other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":";
        Func_Other.push_back(other_out);  
    }
#line 3115 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 106:
#line 1506 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        LABEL_l_num_st = Stk_IF_ELSE.top();
        //Stk_IF_ELSE.pop();
        // out << IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":" << endl;
        other_out = IF_DEEP() + "l" + to_string(LABEL_l_num_st+2) + ":";
        Func_Other.push_back(other_out);  
    }
#line 3127 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 107:
#line 1516 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {Cond_Array_Flag = 1;}
#line 3133 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 108:
#line 1517 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        yyval = yyvsp[0];
        Cond_Array_Flag = 0;
        //out << "LOrExp"<<endl;
        // // out << IF_DEEP() + "t" + to_string(VAR_t_num) + " = ";
        // other_out = IF_DEEP() + "t" + to_string(VAR_t_num) + " = ";
        // if(ToPtrnum($1)->IF_ptr_int){    //如果是常量
        //     other_out += to_string(ToPtrnum($$)->ptr_int);
        // }
        // else{
        //     other_out += ToPtrnum($1)->ptr_str;
        // }
        // //VAR_t_num ++;
        // // ToPtrnum($$)->Print();
        // //out << endl;
        // Func_Other.push_back(other_out);  
        // $$ = $1;
    }
#line 3156 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 109:
#line 1539 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "LAndExp"<<endl;
        string str_2;
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            str_2 = to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            str_2 = ToPtrnum(yyvsp[0])->ptr_str;
        }

        LABEL_l_num_st = Stk_IF_ELSE.top();

        // other_out = IF_DEEP() + "\tif " + str_2 + " == 0 goto l" + to_string(LABEL_l_num_st);
        // Func_Other.push_back(other_out);
        other_out = IF_DEEP() + "\tgoto l" + to_string(LABEL_l_num_st+1);
        Func_Other.push_back(other_out);
        
    }
#line 3179 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 110:
#line 1558 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "LOrExp OR LAndExp"<<endl;
        string str_2;
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            str_2 = to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            str_2 = ToPtrnum(yyvsp[0])->ptr_str;
        }

        LABEL_l_num_st = Stk_IF_ELSE.top();
        // other_out = IF_DEEP() + "\tif " + str_2 + " == 0 goto l" + to_string(LABEL_l_num_st);
        // Func_Other.push_back(other_out);
        other_out = IF_DEEP() + "\tgoto l" + to_string(LABEL_l_num_st+1);
        Func_Other.push_back(other_out);
    }
#line 3200 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 111:
#line 1578 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "EqExp"<<endl;
        string str_2;
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            str_2 = to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            str_2 = ToPtrnum(yyvsp[0])->ptr_str;
        }

        LABEL_l_num_st = Stk_IF_ELSE.top();

        other_out = IF_DEEP() + "\tif " + str_2 + " == 0 goto l" + to_string(LABEL_l_num_st);
        Func_Other.push_back(other_out);
    }
#line 3220 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 112:
#line 1594 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "LAndExp AND EqExp"<<endl;
        string str_2;
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            str_2 = to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            str_2 = ToPtrnum(yyvsp[0])->ptr_str;
        }

        LABEL_l_num_st = Stk_IF_ELSE.top();
        other_out = IF_DEEP() + "\tif " + str_2 + " == 0 goto l" + to_string(LABEL_l_num_st);
        Func_Other.push_back(other_out);
        // other_out = IF_DEEP() + "\tgoto l" + to_string(LABEL_l_num_st+1);
        // Func_Other.push_back(other_out);
    }
#line 3241 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 113:
#line 1614 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //out << "RelExp" << endl;
        yyval = yyvsp[0];
    }
#line 3250 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 114:
#line 1619 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        // Func_VarDecl.push_back(def_out);
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum(yyvsp[-2])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[-2])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[-2])->ptr_str;
        }
        other_out += " == ";
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[0])->ptr_str;
        }
        // ToPtrnum($$)->Print();
        //out << endl;
        Func_Other.push_back(other_out); 
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out); 
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        yyval = tmp_ptr;
    }
#line 3281 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 115:
#line 1646 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        // Func_VarDecl.push_back(def_out);
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum(yyvsp[-2])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[-2])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[-2])->ptr_str;
        }
        other_out += " != ";
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[0])->ptr_str;
        }
        // ToPtrnum($$)->Print();
        //out << endl;
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        yyval = tmp_ptr;
    }
#line 3312 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 116:
#line 1676 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // out << "ADDExp" << endl;
        // out << "===";
        // ToPtrnum($1)->Print(); 
        // out<< "===" << endl;
        yyval = yyvsp[0];
    }
#line 3324 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 117:
#line 1684 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        // Func_VarDecl.push_back(def_out);
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum(yyvsp[-2])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[-2])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[-2])->ptr_str;
        }
        other_out += " < ";
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[0])->ptr_str;
        }
        // ToPtrnum($$)->Print();
        //out << endl;
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        yyval = tmp_ptr;
    }
#line 3355 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 118:
#line 1711 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        // Func_VarDecl.push_back(def_out);
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum(yyvsp[-2])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[-2])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[-2])->ptr_str;
        }
        other_out += " > ";
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[0])->ptr_str;
        }
        // ToPtrnum($$)->Print();
        //out << endl;
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        yyval = tmp_ptr;
    }
#line 3386 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 119:
#line 1738 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        // Func_VarDecl.push_back(def_out);
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum(yyvsp[-2])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[-2])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[-2])->ptr_str;
        }
        other_out += " <= ";
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[0])->ptr_str;
        }
        // ToPtrnum($$)->Print();
        //out << endl;
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        yyval = tmp_ptr;
    }
#line 3417 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 120:
#line 1765 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        // def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        // Func_VarDecl.push_back(def_out);
        other_out = IF_DEEP() + "\tt" + to_string(VAR_t_num) + " = ";
        if(ToPtrnum(yyvsp[-2])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[-2])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[-2])->ptr_str;
        }
        other_out += " >= ";
        if(ToPtrnum(yyvsp[0])->IF_ptr_int){    //如果是常量
            other_out += to_string(ToPtrnum(yyvsp[0])->ptr_int);
        }
        else{
            other_out += ToPtrnum(yyvsp[0])->ptr_str;
        }
        // ToPtrnum($$)->Print();
        //out << endl;
        Func_Other.push_back(other_out);  
        def_out = IF_DEEP_DEF() + "var t" + to_string(VAR_t_num);
        Func_VarDecl.push_back(def_out);
        Ptr_num* tmp_ptr = new Ptr_num("t" + to_string(VAR_t_num));
        VAR_t_num ++;
        yyval = tmp_ptr;
    }
#line 3448 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 121:
#line 1796 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        //表示函数调用中没有参数的情况
    }
#line 3456 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 122:
#line 1800 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* param_tmp = ToPtrnum(yyvsp[0]);
        if(param_tmp->IF_ptr_int){       //如果是常量
            // out << IF_DEEP() + "param " << param_tmp->ptr_int << endl;
            other_out = IF_DEEP() + "param " + to_string(param_tmp->ptr_int);
            Func_Other.push_back(other_out);  
        }
        else{
            // out << IF_DEEP() + "param " + param_tmp->ptr_str << endl;
            other_out = IF_DEEP() + "param " + param_tmp->ptr_str ;
            Func_Other.push_back(other_out);  
        }

    }
#line 3475 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;

  case 123:
#line 1815 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1652  */
    {
        Ptr_num* param_tmp = ToPtrnum(yyvsp[0]);
        if(param_tmp->IF_ptr_int){       //如果是常量
            // out << IF_DEEP() + "param " << param_tmp->ptr_int << endl;
            other_out = IF_DEEP() + "param " + to_string(param_tmp->ptr_int);
            Func_Other.push_back(other_out);
        }
        else{
            // out << IF_DEEP() + "param " + param_tmp->ptr_str << endl;
            other_out = IF_DEEP() + "param " + param_tmp->ptr_str ;
            Func_Other.push_back(other_out); 
        }
    }
#line 3493 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
    break;


#line 3497 "/home/xcw/xcw_Compiler/build/xcw_parser.tab.c" /* yacc.c:1652  */
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
#line 1839 "/home/xcw/xcw_Compiler/source/xcw_parser.y" /* yacc.c:1918  */


void yyerror(const char *s) {
    extern int yylineno, charNum;
    cout << "Line " << yylineno << "," << charNum << ": " << s << endl;
    exit(1);
}

void yyerror(const string &s) {
    yyerror(s.c_str());
}

int main(int argc, char **argv) {
    if (argc >= 4)
        if ((yyin = fopen(argv[3], "r")) == NULL)
            yyerror("Cannot open input file.");
    
    if (argc >= 6)
        if (freopen(argv[5], "w", stdout) == NULL)
            yyerror("Cannot open output file.");

    // nowScope->addToken(new FuncIdentToken(RetInt, "getint", 0));
    // nowScope->addToken(new FuncIdentToken(RetInt, "getch", 0));
    // nowScope->addToken(new FuncIdentToken(RetInt, "getarray", 1));
    // nowScope->addToken(new FuncIdentToken(RetVoid, "putint", 1));
    // nowScope->addToken(new FuncIdentToken(RetVoid, "putch", 1));
    // nowScope->addToken(new FuncIdentToken(RetVoid, "putarray", 2));

    IDENT_scope tmp_1 = IDENT_scope("getint", "0", DEEP, 0);    //不是Const
    tmp_1.IDENT_if_ret_int = 1;
    Scope.push_back(tmp_1);
    IDENT_scope tmp_2 = IDENT_scope("getch", "0", DEEP, 0);    //不是Const
    tmp_2.IDENT_if_ret_int = 1;
    Scope.push_back(tmp_2);
    IDENT_scope tmp_3 = IDENT_scope("getarray", "0", DEEP, 0);    //不是Const
    tmp_3.IDENT_if_ret_int = 1;
    Scope.push_back(tmp_3);
    IDENT_scope tmp_4 = IDENT_scope("putint", "0", DEEP, 0);    //不是Const
    tmp_4.IDENT_if_ret_int = 0;
    Scope.push_back(tmp_4);
    IDENT_scope tmp_5 = IDENT_scope("putch", "0", DEEP, 0);    //不是Const
    tmp_5.IDENT_if_ret_int = 0;
    Scope.push_back(tmp_5);
    IDENT_scope tmp_6 = IDENT_scope("putarray", "0", DEEP, 0);    //不是Const
    tmp_6.IDENT_if_ret_int = 0;
    Scope.push_back(tmp_6);

    yyparse();
    return 0;
}
