// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file /my/cminus-compiler/include/front/syntax_analyzer.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_MY_CMINUS_COMPILER_INCLUDE_FRONT_SYNTAX_ANALYZER_HPP_INCLUDED
# define YY_YY_MY_CMINUS_COMPILER_INCLUDE_FRONT_SYNTAX_ANALYZER_HPP_INCLUDED
// "%code requires" blocks.
#line 12 "syntax_analyzer.y"

#include "ast.hpp"
class driver;

#line 54 "/my/cminus-compiler/include/front/syntax_analyzer.hpp"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"
#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
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

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

namespace yy {
#line 194 "/my/cminus-compiler/include/front/syntax_analyzer.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // AddExp
      char dummy1[sizeof (AstAddExp*)];

      // ArrayConstExpList
      char dummy2[sizeof (AstArrayConstExpList*)];

      // ArrayExpList
      char dummy3[sizeof (AstArrayExpList*)];

      // AssignStmt
      char dummy4[sizeof (AstAssignStmt*)];

      // Block
      char dummy5[sizeof (AstBlock*)];

      // BlockItem
      char dummy6[sizeof (AstBlockItem*)];

      // BlockItemList
      char dummy7[sizeof (AstBlockItemList*)];

      // BreakStmt
      char dummy8[sizeof (AstBreakStmt*)];

      // Callee
      char dummy9[sizeof (AstCallee*)];

      // CompUnit
      char dummy10[sizeof (AstCompUnit*)];

      // Cond
      char dummy11[sizeof (AstCond*)];

      // ConstDecl
      char dummy12[sizeof (AstConstDecl*)];

      // ConstDef
      char dummy13[sizeof (AstConstDef*)];

      // ConstDefList
      char dummy14[sizeof (AstConstDefList*)];

      // ConstExp
      char dummy15[sizeof (AstConstExp*)];

      // ConstInitVal
      char dummy16[sizeof (AstConstInitVal*)];

      // ConstInitValList
      char dummy17[sizeof (AstConstInitValList*)];

      // ContinueStmt
      char dummy18[sizeof (AstContinueStmt*)];

      // DeclDef
      char dummy19[sizeof (AstDeclDef*)];

      // EqExp
      char dummy20[sizeof (AstEqExp*)];

      // Exp
      char dummy21[sizeof (AstExp*)];

      // ExpList
      char dummy22[sizeof (AstExpList*)];

      // Float
      char dummy23[sizeof (AstFloat*)];

      // FuncDef
      char dummy24[sizeof (AstFuncDef*)];

      // FuncFParam
      char dummy25[sizeof (AstFuncFParam*)];

      // FuncFParamList
      char dummy26[sizeof (AstFuncFParamList*)];

      // InitVal
      char dummy27[sizeof (AstInitVal*)];

      // InitValList
      char dummy28[sizeof (AstInitValList*)];

      // Integer
      char dummy29[sizeof (AstInteger*)];

      // IterationStmt
      char dummy30[sizeof (AstIterationStmt*)];

      // LAndExp
      char dummy31[sizeof (AstLAndExp*)];

      // LOrExp
      char dummy32[sizeof (AstLOrExp*)];

      // LVal
      char dummy33[sizeof (AstLVal*)];

      // MulExp
      char dummy34[sizeof (AstMulExp*)];

      // ParamArrayExpList
      char dummy35[sizeof (AstParamArrayExpList*)];

      // PrimaryExp
      char dummy36[sizeof (AstPrimaryExp*)];

      // RelExp
      char dummy37[sizeof (AstRelExp*)];

      // ReturnStmt
      char dummy38[sizeof (AstReturnStmt*)];

      // SelectStmt
      char dummy39[sizeof (AstSelectStmt*)];

      // Stmt
      char dummy40[sizeof (AstStmt*)];

      // UnaryExp
      char dummy41[sizeof (AstUnaryExp*)];

      // VarDecl
      char dummy42[sizeof (AstVarDecl*)];

      // VarDef
      char dummy43[sizeof (AstVarDef*)];

      // VarDefList
      char dummy44[sizeof (AstVarDefList*)];

      // BType
      char dummy45[sizeof (BType)];

      // UnaryOp
      char dummy46[sizeof (UnaryOp)];

      // FLOAT_CONST
      char dummy47[sizeof (float)];

      // INTEGER_CONST
      char dummy48[sizeof (int)];

      // IDENTIFIER
      char dummy49[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    YYEOF = 0,                     // "end of file"
    YYerror = 256,                 // error
    YYUNDEF = 300,                 // "invalid token"
    END = 301,                     // END
    ERROR = 258,                   // ERROR
    ADD = 259,                     // ADD
    SUB = 260,                     // SUB
    MUL = 261,                     // MUL
    DIV = 262,                     // DIV
    LT = 263,                      // LT
    LTE = 264,                     // LTE
    GT = 265,                      // GT
    GTE = 266,                     // GTE
    EQ = 267,                      // EQ
    NEQ = 268,                     // NEQ
    ASSIN = 269,                   // ASSIN
    SEMICOLON = 270,               // SEMICOLON
    COMMA = 271,                   // COMMA
    LPARENTHESE = 272,             // LPARENTHESE
    RPARENTHESE = 273,             // RPARENTHESE
    LBRACKET = 274,                // LBRACKET
    RBRACKET = 275,                // RBRACKET
    LBRACE = 276,                  // LBRACE
    RBRACE = 277,                  // RBRACE
    ELSE = 278,                    // ELSE
    IF = 279,                      // IF
    INT = 280,                     // INT
    FLOAT = 281,                   // FLOAT
    RETURN = 282,                  // RETURN
    VOID = 283,                    // VOID
    WHILE = 284,                   // WHILE
    IDENTIFIER = 285,              // IDENTIFIER
    INTEGER_CONST = 286,           // INTEGER_CONST
    FLOAT_CONST = 287,             // FLOAT_CONST
    ARRAY = 288,                   // ARRAY
    LETTER = 289,                  // LETTER
    EOL = 290,                     // EOL
    COMMENT = 291,                 // COMMENT
    BLANK = 292,                   // BLANK
    CONST = 293,                   // CONST
    BREAK = 294,                   // BREAK
    CONTINUE = 295,                // CONTINUE
    NOT = 296,                     // NOT
    AND = 297,                     // AND
    OR = 298,                      // OR
    MOD = 299                      // MOD
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 46, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_END = 3,                               // END
        S_ERROR = 4,                             // ERROR
        S_ADD = 5,                               // ADD
        S_SUB = 6,                               // SUB
        S_MUL = 7,                               // MUL
        S_DIV = 8,                               // DIV
        S_LT = 9,                                // LT
        S_LTE = 10,                              // LTE
        S_GT = 11,                               // GT
        S_GTE = 12,                              // GTE
        S_EQ = 13,                               // EQ
        S_NEQ = 14,                              // NEQ
        S_ASSIN = 15,                            // ASSIN
        S_SEMICOLON = 16,                        // SEMICOLON
        S_COMMA = 17,                            // COMMA
        S_LPARENTHESE = 18,                      // LPARENTHESE
        S_RPARENTHESE = 19,                      // RPARENTHESE
        S_LBRACKET = 20,                         // LBRACKET
        S_RBRACKET = 21,                         // RBRACKET
        S_LBRACE = 22,                           // LBRACE
        S_RBRACE = 23,                           // RBRACE
        S_ELSE = 24,                             // ELSE
        S_IF = 25,                               // IF
        S_INT = 26,                              // INT
        S_FLOAT = 27,                            // FLOAT
        S_RETURN = 28,                           // RETURN
        S_VOID = 29,                             // VOID
        S_WHILE = 30,                            // WHILE
        S_IDENTIFIER = 31,                       // IDENTIFIER
        S_INTEGER_CONST = 32,                    // INTEGER_CONST
        S_FLOAT_CONST = 33,                      // FLOAT_CONST
        S_ARRAY = 34,                            // ARRAY
        S_LETTER = 35,                           // LETTER
        S_EOL = 36,                              // EOL
        S_COMMENT = 37,                          // COMMENT
        S_BLANK = 38,                            // BLANK
        S_CONST = 39,                            // CONST
        S_BREAK = 40,                            // BREAK
        S_CONTINUE = 41,                         // CONTINUE
        S_NOT = 42,                              // NOT
        S_AND = 43,                              // AND
        S_OR = 44,                               // OR
        S_MOD = 45,                              // MOD
        S_YYACCEPT = 46,                         // $accept
        S_Begin = 47,                            // Begin
        S_CompUnit = 48,                         // CompUnit
        S_DeclDef = 49,                          // DeclDef
        S_ConstDecl = 50,                        // ConstDecl
        S_ConstDefList = 51,                     // ConstDefList
        S_ConstDef = 52,                         // ConstDef
        S_ArrayConstExpList = 53,                // ArrayConstExpList
        S_ConstInitVal = 54,                     // ConstInitVal
        S_ConstInitValList = 55,                 // ConstInitValList
        S_VarDecl = 56,                          // VarDecl
        S_VarDefList = 57,                       // VarDefList
        S_VarDef = 58,                           // VarDef
        S_InitVal = 59,                          // InitVal
        S_InitValList = 60,                      // InitValList
        S_FuncDef = 61,                          // FuncDef
        S_BType = 62,                            // BType
        S_FuncFParamList = 63,                   // FuncFParamList
        S_FuncFParam = 64,                       // FuncFParam
        S_ParamArrayExpList = 65,                // ParamArrayExpList
        S_Block = 66,                            // Block
        S_BlockItemList = 67,                    // BlockItemList
        S_BlockItem = 68,                        // BlockItem
        S_Stmt = 69,                             // Stmt
        S_BreakStmt = 70,                        // BreakStmt
        S_ContinueStmt = 71,                     // ContinueStmt
        S_AssignStmt = 72,                       // AssignStmt
        S_SelectStmt = 73,                       // SelectStmt
        S_IterationStmt = 74,                    // IterationStmt
        S_ReturnStmt = 75,                       // ReturnStmt
        S_Exp = 76,                              // Exp
        S_Cond = 77,                             // Cond
        S_LVal = 78,                             // LVal
        S_ArrayExpList = 79,                     // ArrayExpList
        S_PrimaryExp = 80,                       // PrimaryExp
        S_Integer = 81,                          // Integer
        S_Float = 82,                            // Float
        S_UnaryExp = 83,                         // UnaryExp
        S_Callee = 84,                           // Callee
        S_UnaryOp = 85,                          // UnaryOp
        S_ExpList = 86,                          // ExpList
        S_MulExp = 87,                           // MulExp
        S_AddExp = 88,                           // AddExp
        S_RelExp = 89,                           // RelExp
        S_EqExp = 90,                            // EqExp
        S_LAndExp = 91,                          // LAndExp
        S_LOrExp = 92,                           // LOrExp
        S_ConstExp = 93                          // ConstExp
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_AddExp: // AddExp
        value.move< AstAddExp* > (std::move (that.value));
        break;

      case symbol_kind::S_ArrayConstExpList: // ArrayConstExpList
        value.move< AstArrayConstExpList* > (std::move (that.value));
        break;

      case symbol_kind::S_ArrayExpList: // ArrayExpList
        value.move< AstArrayExpList* > (std::move (that.value));
        break;

      case symbol_kind::S_AssignStmt: // AssignStmt
        value.move< AstAssignStmt* > (std::move (that.value));
        break;

      case symbol_kind::S_Block: // Block
        value.move< AstBlock* > (std::move (that.value));
        break;

      case symbol_kind::S_BlockItem: // BlockItem
        value.move< AstBlockItem* > (std::move (that.value));
        break;

      case symbol_kind::S_BlockItemList: // BlockItemList
        value.move< AstBlockItemList* > (std::move (that.value));
        break;

      case symbol_kind::S_BreakStmt: // BreakStmt
        value.move< AstBreakStmt* > (std::move (that.value));
        break;

      case symbol_kind::S_Callee: // Callee
        value.move< AstCallee* > (std::move (that.value));
        break;

      case symbol_kind::S_CompUnit: // CompUnit
        value.move< AstCompUnit* > (std::move (that.value));
        break;

      case symbol_kind::S_Cond: // Cond
        value.move< AstCond* > (std::move (that.value));
        break;

      case symbol_kind::S_ConstDecl: // ConstDecl
        value.move< AstConstDecl* > (std::move (that.value));
        break;

      case symbol_kind::S_ConstDef: // ConstDef
        value.move< AstConstDef* > (std::move (that.value));
        break;

      case symbol_kind::S_ConstDefList: // ConstDefList
        value.move< AstConstDefList* > (std::move (that.value));
        break;

      case symbol_kind::S_ConstExp: // ConstExp
        value.move< AstConstExp* > (std::move (that.value));
        break;

      case symbol_kind::S_ConstInitVal: // ConstInitVal
        value.move< AstConstInitVal* > (std::move (that.value));
        break;

      case symbol_kind::S_ConstInitValList: // ConstInitValList
        value.move< AstConstInitValList* > (std::move (that.value));
        break;

      case symbol_kind::S_ContinueStmt: // ContinueStmt
        value.move< AstContinueStmt* > (std::move (that.value));
        break;

      case symbol_kind::S_DeclDef: // DeclDef
        value.move< AstDeclDef* > (std::move (that.value));
        break;

      case symbol_kind::S_EqExp: // EqExp
        value.move< AstEqExp* > (std::move (that.value));
        break;

      case symbol_kind::S_Exp: // Exp
        value.move< AstExp* > (std::move (that.value));
        break;

      case symbol_kind::S_ExpList: // ExpList
        value.move< AstExpList* > (std::move (that.value));
        break;

      case symbol_kind::S_Float: // Float
        value.move< AstFloat* > (std::move (that.value));
        break;

      case symbol_kind::S_FuncDef: // FuncDef
        value.move< AstFuncDef* > (std::move (that.value));
        break;

      case symbol_kind::S_FuncFParam: // FuncFParam
        value.move< AstFuncFParam* > (std::move (that.value));
        break;

      case symbol_kind::S_FuncFParamList: // FuncFParamList
        value.move< AstFuncFParamList* > (std::move (that.value));
        break;

      case symbol_kind::S_InitVal: // InitVal
        value.move< AstInitVal* > (std::move (that.value));
        break;

      case symbol_kind::S_InitValList: // InitValList
        value.move< AstInitValList* > (std::move (that.value));
        break;

      case symbol_kind::S_Integer: // Integer
        value.move< AstInteger* > (std::move (that.value));
        break;

      case symbol_kind::S_IterationStmt: // IterationStmt
        value.move< AstIterationStmt* > (std::move (that.value));
        break;

      case symbol_kind::S_LAndExp: // LAndExp
        value.move< AstLAndExp* > (std::move (that.value));
        break;

      case symbol_kind::S_LOrExp: // LOrExp
        value.move< AstLOrExp* > (std::move (that.value));
        break;

      case symbol_kind::S_LVal: // LVal
        value.move< AstLVal* > (std::move (that.value));
        break;

      case symbol_kind::S_MulExp: // MulExp
        value.move< AstMulExp* > (std::move (that.value));
        break;

      case symbol_kind::S_ParamArrayExpList: // ParamArrayExpList
        value.move< AstParamArrayExpList* > (std::move (that.value));
        break;

      case symbol_kind::S_PrimaryExp: // PrimaryExp
        value.move< AstPrimaryExp* > (std::move (that.value));
        break;

      case symbol_kind::S_RelExp: // RelExp
        value.move< AstRelExp* > (std::move (that.value));
        break;

      case symbol_kind::S_ReturnStmt: // ReturnStmt
        value.move< AstReturnStmt* > (std::move (that.value));
        break;

      case symbol_kind::S_SelectStmt: // SelectStmt
        value.move< AstSelectStmt* > (std::move (that.value));
        break;

      case symbol_kind::S_Stmt: // Stmt
        value.move< AstStmt* > (std::move (that.value));
        break;

      case symbol_kind::S_UnaryExp: // UnaryExp
        value.move< AstUnaryExp* > (std::move (that.value));
        break;

      case symbol_kind::S_VarDecl: // VarDecl
        value.move< AstVarDecl* > (std::move (that.value));
        break;

      case symbol_kind::S_VarDef: // VarDef
        value.move< AstVarDef* > (std::move (that.value));
        break;

      case symbol_kind::S_VarDefList: // VarDefList
        value.move< AstVarDefList* > (std::move (that.value));
        break;

      case symbol_kind::S_BType: // BType
        value.move< BType > (std::move (that.value));
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
        value.move< UnaryOp > (std::move (that.value));
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.move< float > (std::move (that.value));
        break;

      case symbol_kind::S_INTEGER_CONST: // INTEGER_CONST
        value.move< int > (std::move (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstAddExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstAddExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstArrayConstExpList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstArrayConstExpList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstArrayExpList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstArrayExpList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstAssignStmt*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstAssignStmt*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstBlock*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstBlock*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstBlockItem*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstBlockItem*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstBlockItemList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstBlockItemList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstBreakStmt*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstBreakStmt*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstCallee*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstCallee*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstCompUnit*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstCompUnit*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstCond*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstCond*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstConstDecl*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstConstDecl*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstConstDef*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstConstDef*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstConstDefList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstConstDefList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstConstExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstConstExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstConstInitVal*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstConstInitVal*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstConstInitValList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstConstInitValList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstContinueStmt*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstContinueStmt*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstDeclDef*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstDeclDef*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstEqExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstEqExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstExpList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstExpList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstFloat*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstFloat*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstFuncDef*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstFuncDef*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstFuncFParam*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstFuncFParam*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstFuncFParamList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstFuncFParamList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstInitVal*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstInitVal*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstInitValList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstInitValList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstInteger*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstInteger*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstIterationStmt*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstIterationStmt*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstLAndExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstLAndExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstLOrExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstLOrExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstLVal*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstLVal*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstMulExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstMulExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstParamArrayExpList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstParamArrayExpList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstPrimaryExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstPrimaryExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstRelExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstRelExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstReturnStmt*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstReturnStmt*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstSelectStmt*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstSelectStmt*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstStmt*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstStmt*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstUnaryExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstUnaryExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstVarDecl*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstVarDecl*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstVarDef*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstVarDef*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AstVarDefList*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AstVarDefList*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, BType&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const BType& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, UnaryOp&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const UnaryOp& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, float&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const float& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, int&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const int& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_AddExp: // AddExp
        value.template destroy< AstAddExp* > ();
        break;

      case symbol_kind::S_ArrayConstExpList: // ArrayConstExpList
        value.template destroy< AstArrayConstExpList* > ();
        break;

      case symbol_kind::S_ArrayExpList: // ArrayExpList
        value.template destroy< AstArrayExpList* > ();
        break;

      case symbol_kind::S_AssignStmt: // AssignStmt
        value.template destroy< AstAssignStmt* > ();
        break;

      case symbol_kind::S_Block: // Block
        value.template destroy< AstBlock* > ();
        break;

      case symbol_kind::S_BlockItem: // BlockItem
        value.template destroy< AstBlockItem* > ();
        break;

      case symbol_kind::S_BlockItemList: // BlockItemList
        value.template destroy< AstBlockItemList* > ();
        break;

      case symbol_kind::S_BreakStmt: // BreakStmt
        value.template destroy< AstBreakStmt* > ();
        break;

      case symbol_kind::S_Callee: // Callee
        value.template destroy< AstCallee* > ();
        break;

      case symbol_kind::S_CompUnit: // CompUnit
        value.template destroy< AstCompUnit* > ();
        break;

      case symbol_kind::S_Cond: // Cond
        value.template destroy< AstCond* > ();
        break;

      case symbol_kind::S_ConstDecl: // ConstDecl
        value.template destroy< AstConstDecl* > ();
        break;

      case symbol_kind::S_ConstDef: // ConstDef
        value.template destroy< AstConstDef* > ();
        break;

      case symbol_kind::S_ConstDefList: // ConstDefList
        value.template destroy< AstConstDefList* > ();
        break;

      case symbol_kind::S_ConstExp: // ConstExp
        value.template destroy< AstConstExp* > ();
        break;

      case symbol_kind::S_ConstInitVal: // ConstInitVal
        value.template destroy< AstConstInitVal* > ();
        break;

      case symbol_kind::S_ConstInitValList: // ConstInitValList
        value.template destroy< AstConstInitValList* > ();
        break;

      case symbol_kind::S_ContinueStmt: // ContinueStmt
        value.template destroy< AstContinueStmt* > ();
        break;

      case symbol_kind::S_DeclDef: // DeclDef
        value.template destroy< AstDeclDef* > ();
        break;

      case symbol_kind::S_EqExp: // EqExp
        value.template destroy< AstEqExp* > ();
        break;

      case symbol_kind::S_Exp: // Exp
        value.template destroy< AstExp* > ();
        break;

      case symbol_kind::S_ExpList: // ExpList
        value.template destroy< AstExpList* > ();
        break;

      case symbol_kind::S_Float: // Float
        value.template destroy< AstFloat* > ();
        break;

      case symbol_kind::S_FuncDef: // FuncDef
        value.template destroy< AstFuncDef* > ();
        break;

      case symbol_kind::S_FuncFParam: // FuncFParam
        value.template destroy< AstFuncFParam* > ();
        break;

      case symbol_kind::S_FuncFParamList: // FuncFParamList
        value.template destroy< AstFuncFParamList* > ();
        break;

      case symbol_kind::S_InitVal: // InitVal
        value.template destroy< AstInitVal* > ();
        break;

      case symbol_kind::S_InitValList: // InitValList
        value.template destroy< AstInitValList* > ();
        break;

      case symbol_kind::S_Integer: // Integer
        value.template destroy< AstInteger* > ();
        break;

      case symbol_kind::S_IterationStmt: // IterationStmt
        value.template destroy< AstIterationStmt* > ();
        break;

      case symbol_kind::S_LAndExp: // LAndExp
        value.template destroy< AstLAndExp* > ();
        break;

      case symbol_kind::S_LOrExp: // LOrExp
        value.template destroy< AstLOrExp* > ();
        break;

      case symbol_kind::S_LVal: // LVal
        value.template destroy< AstLVal* > ();
        break;

      case symbol_kind::S_MulExp: // MulExp
        value.template destroy< AstMulExp* > ();
        break;

      case symbol_kind::S_ParamArrayExpList: // ParamArrayExpList
        value.template destroy< AstParamArrayExpList* > ();
        break;

      case symbol_kind::S_PrimaryExp: // PrimaryExp
        value.template destroy< AstPrimaryExp* > ();
        break;

      case symbol_kind::S_RelExp: // RelExp
        value.template destroy< AstRelExp* > ();
        break;

      case symbol_kind::S_ReturnStmt: // ReturnStmt
        value.template destroy< AstReturnStmt* > ();
        break;

      case symbol_kind::S_SelectStmt: // SelectStmt
        value.template destroy< AstSelectStmt* > ();
        break;

      case symbol_kind::S_Stmt: // Stmt
        value.template destroy< AstStmt* > ();
        break;

      case symbol_kind::S_UnaryExp: // UnaryExp
        value.template destroy< AstUnaryExp* > ();
        break;

      case symbol_kind::S_VarDecl: // VarDecl
        value.template destroy< AstVarDecl* > ();
        break;

      case symbol_kind::S_VarDef: // VarDef
        value.template destroy< AstVarDef* > ();
        break;

      case symbol_kind::S_VarDefList: // VarDefList
        value.template destroy< AstVarDefList* > ();
        break;

      case symbol_kind::S_BType: // BType
        value.template destroy< BType > ();
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
        value.template destroy< UnaryOp > ();
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.template destroy< float > ();
        break;

      case symbol_kind::S_INTEGER_CONST: // INTEGER_CONST
        value.template destroy< int > ();
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::YYEOF
                   || tok == token::YYerror
                   || (token::YYUNDEF <= tok && tok <= token::END)
                   || (token::ERROR <= tok && tok <= token::WHILE)
                   || (token::ARRAY <= tok && tok <= token::MOD));
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, float v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const float& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::FLOAT_CONST);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, int v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const int& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::INTEGER_CONST);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::IDENTIFIER);
#endif
      }
    };

    /// Build a parser object.
    parser (driver& drv_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);

    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOF (location_type l)
      {
        return symbol_type (token::YYEOF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYEOF (const location_type& l)
      {
        return symbol_type (token::YYEOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror (location_type l)
      {
        return symbol_type (token::YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::YYUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END (location_type l)
      {
        return symbol_type (token::END, std::move (l));
      }
#else
      static
      symbol_type
      make_END (const location_type& l)
      {
        return symbol_type (token::END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ERROR (location_type l)
      {
        return symbol_type (token::ERROR, std::move (l));
      }
#else
      static
      symbol_type
      make_ERROR (const location_type& l)
      {
        return symbol_type (token::ERROR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ADD (location_type l)
      {
        return symbol_type (token::ADD, std::move (l));
      }
#else
      static
      symbol_type
      make_ADD (const location_type& l)
      {
        return symbol_type (token::ADD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SUB (location_type l)
      {
        return symbol_type (token::SUB, std::move (l));
      }
#else
      static
      symbol_type
      make_SUB (const location_type& l)
      {
        return symbol_type (token::SUB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MUL (location_type l)
      {
        return symbol_type (token::MUL, std::move (l));
      }
#else
      static
      symbol_type
      make_MUL (const location_type& l)
      {
        return symbol_type (token::MUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIV (location_type l)
      {
        return symbol_type (token::DIV, std::move (l));
      }
#else
      static
      symbol_type
      make_DIV (const location_type& l)
      {
        return symbol_type (token::DIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LT (location_type l)
      {
        return symbol_type (token::LT, std::move (l));
      }
#else
      static
      symbol_type
      make_LT (const location_type& l)
      {
        return symbol_type (token::LT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LTE (location_type l)
      {
        return symbol_type (token::LTE, std::move (l));
      }
#else
      static
      symbol_type
      make_LTE (const location_type& l)
      {
        return symbol_type (token::LTE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GT (location_type l)
      {
        return symbol_type (token::GT, std::move (l));
      }
#else
      static
      symbol_type
      make_GT (const location_type& l)
      {
        return symbol_type (token::GT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GTE (location_type l)
      {
        return symbol_type (token::GTE, std::move (l));
      }
#else
      static
      symbol_type
      make_GTE (const location_type& l)
      {
        return symbol_type (token::GTE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQ (location_type l)
      {
        return symbol_type (token::EQ, std::move (l));
      }
#else
      static
      symbol_type
      make_EQ (const location_type& l)
      {
        return symbol_type (token::EQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NEQ (location_type l)
      {
        return symbol_type (token::NEQ, std::move (l));
      }
#else
      static
      symbol_type
      make_NEQ (const location_type& l)
      {
        return symbol_type (token::NEQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASSIN (location_type l)
      {
        return symbol_type (token::ASSIN, std::move (l));
      }
#else
      static
      symbol_type
      make_ASSIN (const location_type& l)
      {
        return symbol_type (token::ASSIN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEMICOLON (location_type l)
      {
        return symbol_type (token::SEMICOLON, std::move (l));
      }
#else
      static
      symbol_type
      make_SEMICOLON (const location_type& l)
      {
        return symbol_type (token::SEMICOLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LPARENTHESE (location_type l)
      {
        return symbol_type (token::LPARENTHESE, std::move (l));
      }
#else
      static
      symbol_type
      make_LPARENTHESE (const location_type& l)
      {
        return symbol_type (token::LPARENTHESE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RPARENTHESE (location_type l)
      {
        return symbol_type (token::RPARENTHESE, std::move (l));
      }
#else
      static
      symbol_type
      make_RPARENTHESE (const location_type& l)
      {
        return symbol_type (token::RPARENTHESE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LBRACKET (location_type l)
      {
        return symbol_type (token::LBRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_LBRACKET (const location_type& l)
      {
        return symbol_type (token::LBRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RBRACKET (location_type l)
      {
        return symbol_type (token::RBRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_RBRACKET (const location_type& l)
      {
        return symbol_type (token::RBRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LBRACE (location_type l)
      {
        return symbol_type (token::LBRACE, std::move (l));
      }
#else
      static
      symbol_type
      make_LBRACE (const location_type& l)
      {
        return symbol_type (token::LBRACE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RBRACE (location_type l)
      {
        return symbol_type (token::RBRACE, std::move (l));
      }
#else
      static
      symbol_type
      make_RBRACE (const location_type& l)
      {
        return symbol_type (token::RBRACE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ELSE (location_type l)
      {
        return symbol_type (token::ELSE, std::move (l));
      }
#else
      static
      symbol_type
      make_ELSE (const location_type& l)
      {
        return symbol_type (token::ELSE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IF (location_type l)
      {
        return symbol_type (token::IF, std::move (l));
      }
#else
      static
      symbol_type
      make_IF (const location_type& l)
      {
        return symbol_type (token::IF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT (location_type l)
      {
        return symbol_type (token::INT, std::move (l));
      }
#else
      static
      symbol_type
      make_INT (const location_type& l)
      {
        return symbol_type (token::INT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT (location_type l)
      {
        return symbol_type (token::FLOAT, std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT (const location_type& l)
      {
        return symbol_type (token::FLOAT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RETURN (location_type l)
      {
        return symbol_type (token::RETURN, std::move (l));
      }
#else
      static
      symbol_type
      make_RETURN (const location_type& l)
      {
        return symbol_type (token::RETURN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VOID (location_type l)
      {
        return symbol_type (token::VOID, std::move (l));
      }
#else
      static
      symbol_type
      make_VOID (const location_type& l)
      {
        return symbol_type (token::VOID, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WHILE (location_type l)
      {
        return symbol_type (token::WHILE, std::move (l));
      }
#else
      static
      symbol_type
      make_WHILE (const location_type& l)
      {
        return symbol_type (token::WHILE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENTIFIER (std::string v, location_type l)
      {
        return symbol_type (token::IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IDENTIFIER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTEGER_CONST (int v, location_type l)
      {
        return symbol_type (token::INTEGER_CONST, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INTEGER_CONST (const int& v, const location_type& l)
      {
        return symbol_type (token::INTEGER_CONST, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT_CONST (float v, location_type l)
      {
        return symbol_type (token::FLOAT_CONST, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT_CONST (const float& v, const location_type& l)
      {
        return symbol_type (token::FLOAT_CONST, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ARRAY (location_type l)
      {
        return symbol_type (token::ARRAY, std::move (l));
      }
#else
      static
      symbol_type
      make_ARRAY (const location_type& l)
      {
        return symbol_type (token::ARRAY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LETTER (location_type l)
      {
        return symbol_type (token::LETTER, std::move (l));
      }
#else
      static
      symbol_type
      make_LETTER (const location_type& l)
      {
        return symbol_type (token::LETTER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EOL (location_type l)
      {
        return symbol_type (token::EOL, std::move (l));
      }
#else
      static
      symbol_type
      make_EOL (const location_type& l)
      {
        return symbol_type (token::EOL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMENT (location_type l)
      {
        return symbol_type (token::COMMENT, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMENT (const location_type& l)
      {
        return symbol_type (token::COMMENT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BLANK (location_type l)
      {
        return symbol_type (token::BLANK, std::move (l));
      }
#else
      static
      symbol_type
      make_BLANK (const location_type& l)
      {
        return symbol_type (token::BLANK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONST (location_type l)
      {
        return symbol_type (token::CONST, std::move (l));
      }
#else
      static
      symbol_type
      make_CONST (const location_type& l)
      {
        return symbol_type (token::CONST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BREAK (location_type l)
      {
        return symbol_type (token::BREAK, std::move (l));
      }
#else
      static
      symbol_type
      make_BREAK (const location_type& l)
      {
        return symbol_type (token::BREAK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONTINUE (location_type l)
      {
        return symbol_type (token::CONTINUE, std::move (l));
      }
#else
      static
      symbol_type
      make_CONTINUE (const location_type& l)
      {
        return symbol_type (token::CONTINUE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NOT (location_type l)
      {
        return symbol_type (token::NOT, std::move (l));
      }
#else
      static
      symbol_type
      make_NOT (const location_type& l)
      {
        return symbol_type (token::NOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AND (location_type l)
      {
        return symbol_type (token::AND, std::move (l));
      }
#else
      static
      symbol_type
      make_AND (const location_type& l)
      {
        return symbol_type (token::AND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OR (location_type l)
      {
        return symbol_type (token::OR, std::move (l));
      }
#else
      static
      symbol_type
      make_OR (const location_type& l)
      {
        return symbol_type (token::OR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MOD (location_type l)
      {
        return symbol_type (token::MOD, std::move (l));
      }
#else
      static
      symbol_type
      make_MOD (const location_type& l)
      {
        return symbol_type (token::MOD, l);
      }
#endif


    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      const location_type& location () const YY_NOEXCEPT { return yyla_.location; }

      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const parser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif

    /// Check the lookahead yytoken.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_check_ (symbol_kind_type yytoken) const;
    /// Establish the initial context if no initial context currently exists.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_establish_ (symbol_kind_type yytoken);
    /// Discard any previous initial lookahead context because of event.
    /// \param event  the event which caused the lookahead to be discarded.
    ///               Only used for debbuging output.
    void yy_lac_discard_ (const char* event);

    /// Stored state numbers (used for stacks).
    typedef unsigned char state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;



    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const unsigned char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const unsigned char yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;
    /// The stack for LAC.
    /// Logically, the yy_lac_stack's lifetime is confined to the function
    /// yy_lac_check_. We just store it as a member of this class to hold
    /// on to the memory and to avoid frequent reallocations.
    /// Since yy_lac_check_ is const, this member must be mutable.
    mutable std::vector<state_type> yylac_stack_;
    /// Whether an initial LAC context was established.
    bool yy_lac_established_;


    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 244,     ///< Last index in yytable_.
      yynnts_ = 48,  ///< Number of nonterminal symbols.
      yyfinal_ = 13 ///< Termination state number.
    };


    // User arguments.
    driver& drv;

  };

  inline
  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
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
       2,     2,     2,     2,     2,     2,     1,     2,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       2,     3
    };
    // Last valid token kind.
    const int code_max = 301;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_AddExp: // AddExp
        value.copy< AstAddExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ArrayConstExpList: // ArrayConstExpList
        value.copy< AstArrayConstExpList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ArrayExpList: // ArrayExpList
        value.copy< AstArrayExpList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_AssignStmt: // AssignStmt
        value.copy< AstAssignStmt* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Block: // Block
        value.copy< AstBlock* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BlockItem: // BlockItem
        value.copy< AstBlockItem* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BlockItemList: // BlockItemList
        value.copy< AstBlockItemList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BreakStmt: // BreakStmt
        value.copy< AstBreakStmt* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Callee: // Callee
        value.copy< AstCallee* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CompUnit: // CompUnit
        value.copy< AstCompUnit* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Cond: // Cond
        value.copy< AstCond* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstDecl: // ConstDecl
        value.copy< AstConstDecl* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstDef: // ConstDef
        value.copy< AstConstDef* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstDefList: // ConstDefList
        value.copy< AstConstDefList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstExp: // ConstExp
        value.copy< AstConstExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstInitVal: // ConstInitVal
        value.copy< AstConstInitVal* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstInitValList: // ConstInitValList
        value.copy< AstConstInitValList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ContinueStmt: // ContinueStmt
        value.copy< AstContinueStmt* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DeclDef: // DeclDef
        value.copy< AstDeclDef* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_EqExp: // EqExp
        value.copy< AstEqExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Exp: // Exp
        value.copy< AstExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ExpList: // ExpList
        value.copy< AstExpList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Float: // Float
        value.copy< AstFloat* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncDef: // FuncDef
        value.copy< AstFuncDef* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncFParam: // FuncFParam
        value.copy< AstFuncFParam* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncFParamList: // FuncFParamList
        value.copy< AstFuncFParamList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_InitVal: // InitVal
        value.copy< AstInitVal* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_InitValList: // InitValList
        value.copy< AstInitValList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Integer: // Integer
        value.copy< AstInteger* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IterationStmt: // IterationStmt
        value.copy< AstIterationStmt* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LAndExp: // LAndExp
        value.copy< AstLAndExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LOrExp: // LOrExp
        value.copy< AstLOrExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LVal: // LVal
        value.copy< AstLVal* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MulExp: // MulExp
        value.copy< AstMulExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ParamArrayExpList: // ParamArrayExpList
        value.copy< AstParamArrayExpList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PrimaryExp: // PrimaryExp
        value.copy< AstPrimaryExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RelExp: // RelExp
        value.copy< AstRelExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ReturnStmt: // ReturnStmt
        value.copy< AstReturnStmt* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SelectStmt: // SelectStmt
        value.copy< AstSelectStmt* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Stmt: // Stmt
        value.copy< AstStmt* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UnaryExp: // UnaryExp
        value.copy< AstUnaryExp* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_VarDecl: // VarDecl
        value.copy< AstVarDecl* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_VarDef: // VarDef
        value.copy< AstVarDef* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_VarDefList: // VarDefList
        value.copy< AstVarDefList* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BType: // BType
        value.copy< BType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
        value.copy< UnaryOp > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.copy< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER_CONST: // INTEGER_CONST
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_AddExp: // AddExp
        value.move< AstAddExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ArrayConstExpList: // ArrayConstExpList
        value.move< AstArrayConstExpList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ArrayExpList: // ArrayExpList
        value.move< AstArrayExpList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_AssignStmt: // AssignStmt
        value.move< AstAssignStmt* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Block: // Block
        value.move< AstBlock* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_BlockItem: // BlockItem
        value.move< AstBlockItem* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_BlockItemList: // BlockItemList
        value.move< AstBlockItemList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_BreakStmt: // BreakStmt
        value.move< AstBreakStmt* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Callee: // Callee
        value.move< AstCallee* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_CompUnit: // CompUnit
        value.move< AstCompUnit* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Cond: // Cond
        value.move< AstCond* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ConstDecl: // ConstDecl
        value.move< AstConstDecl* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ConstDef: // ConstDef
        value.move< AstConstDef* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ConstDefList: // ConstDefList
        value.move< AstConstDefList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ConstExp: // ConstExp
        value.move< AstConstExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ConstInitVal: // ConstInitVal
        value.move< AstConstInitVal* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ConstInitValList: // ConstInitValList
        value.move< AstConstInitValList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ContinueStmt: // ContinueStmt
        value.move< AstContinueStmt* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DeclDef: // DeclDef
        value.move< AstDeclDef* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_EqExp: // EqExp
        value.move< AstEqExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Exp: // Exp
        value.move< AstExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ExpList: // ExpList
        value.move< AstExpList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Float: // Float
        value.move< AstFloat* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FuncDef: // FuncDef
        value.move< AstFuncDef* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FuncFParam: // FuncFParam
        value.move< AstFuncFParam* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FuncFParamList: // FuncFParamList
        value.move< AstFuncFParamList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_InitVal: // InitVal
        value.move< AstInitVal* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_InitValList: // InitValList
        value.move< AstInitValList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Integer: // Integer
        value.move< AstInteger* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_IterationStmt: // IterationStmt
        value.move< AstIterationStmt* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_LAndExp: // LAndExp
        value.move< AstLAndExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_LOrExp: // LOrExp
        value.move< AstLOrExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_LVal: // LVal
        value.move< AstLVal* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_MulExp: // MulExp
        value.move< AstMulExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ParamArrayExpList: // ParamArrayExpList
        value.move< AstParamArrayExpList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_PrimaryExp: // PrimaryExp
        value.move< AstPrimaryExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_RelExp: // RelExp
        value.move< AstRelExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ReturnStmt: // ReturnStmt
        value.move< AstReturnStmt* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_SelectStmt: // SelectStmt
        value.move< AstSelectStmt* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_Stmt: // Stmt
        value.move< AstStmt* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_UnaryExp: // UnaryExp
        value.move< AstUnaryExp* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_VarDecl: // VarDecl
        value.move< AstVarDecl* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_VarDef: // VarDef
        value.move< AstVarDef* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_VarDefList: // VarDefList
        value.move< AstVarDefList* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_BType: // BType
        value.move< BType > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_UnaryOp: // UnaryOp
        value.move< UnaryOp > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.move< float > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER_CONST: // INTEGER_CONST
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  inline
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  inline
  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  inline
  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  inline
  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  inline
  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


} // yy
#line 3711 "/my/cminus-compiler/include/front/syntax_analyzer.hpp"




#endif // !YY_YY_MY_CMINUS_COMPILER_INCLUDE_FRONT_SYNTAX_ANALYZER_HPP_INCLUDED
