%option noyywrap
%x StateNestedComment
%x StateLineComment

%{
#include "driver.hpp"

// Code run each time a pattern is matched.
# define YY_USER_ACTION  loc.columns (yyleng);

// "."(点)的作用是匹配除了"\n"（换行符）以外的任何单个字符
%}

IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
INTEGER ([1-9][0-9]*|0[0-7]*|(0x|0X)[0-9a-fA-F]*)
DEC_FP ((([0-9]*[.][0-9]*([eE][+-]?[0-9]+)?)|([0-9]+[eE][+-]?[0-9]+))[fLlL]?)
HEX_FP (0[xX](([0-9A-Fa-f]*[.][0-9A-Fa-f]*([pP][+-]?[0-9]+)?)|([0-9A-Fa-f]+[pP][+-]?[0-9]+))[fLlL]?)

%%
%{
  // Code run each time yylex is called.
  // loc是CminusFlexLexer.hpp中的
  // Method on location: void step () Move begin onto end.
  loc.step ();
%}

int         {return yy::parser::make_INT(loc);}
float       {return yy::parser::make_FLOAT(loc);}
else        {return yy::parser::make_ELSE(loc);}
if          {return yy::parser::make_IF(loc);}
return      {return yy::parser::make_RETURN(loc);}
void        {return yy::parser::make_VOID(loc);}
while       {return yy::parser::make_WHILE(loc);}
const       {return yy::parser::make_CONST(loc);}
break       {return yy::parser::make_BREAK(loc);}
continue    {return yy::parser::make_CONTINUE(loc);}

"+"         {return yy::parser::make_ADD(loc);}
"-"         {return yy::parser::make_SUB(loc);}
"*"         {return yy::parser::make_MUL(loc);}
"/"         {return yy::parser::make_DIV(loc);}
"<"         {return yy::parser::make_LT(loc);}
"<="        {return yy::parser::make_LTE(loc);}
">"         {return yy::parser::make_GT(loc);}
">="        {return yy::parser::make_GTE(loc);}
"=="        {return yy::parser::make_EQ(loc);}
"!="        {return yy::parser::make_NEQ(loc);}
"!"         {return yy::parser::make_NOT(loc);}
"&&"        {return yy::parser::make_AND(loc);}
"\|\|"      {return yy::parser::make_OR(loc);}
"="         {return yy::parser::make_ASSIN(loc);}
";"         {return yy::parser::make_SEMICOLON(loc);}
","         {return yy::parser::make_COMMA(loc);}
"("         {return yy::parser::make_LPARENTHESE(loc);}
")"         {return yy::parser::make_RPARENTHESE(loc);}
"["         {return yy::parser::make_LBRACKET(loc);}
"]"         {return yy::parser::make_RBRACKET(loc);}
"{"         {return yy::parser::make_LBRACE(loc);}
"}"         {return yy::parser::make_RBRACE(loc);}
"%"         {return yy::parser::make_MOD(loc);}
"\n"        {loc.lines(yyleng);}
[ \t\r\f\v] {}
"[]" 		{return yy::parser::make_ARRAY(loc);}

"//" { BEGIN(StateLineComment); }
<StateLineComment>.*
<StateLineComment>\n {
  loc.lines(yyleng);
  BEGIN(INITIAL);
}

"/*" {
  BEGIN(StateNestedComment);
}
<StateNestedComment>"*/" {
  BEGIN(INITIAL);
}
<StateNestedComment>.
<StateNestedComment>\n { loc.lines(yyleng); }
<StateNestedComment><<EOF>> {
  BEGIN(INITIAL);
  exit(999);
}

{IDENTIFIER}            {return yy::parser::make_IDENTIFIER(yytext, loc);}
{INTEGER}               {return yy::parser::make_INTEGER_CONST(std::stoi(yytext,nullptr,0),loc);}
({DEC_FP}|{HEX_FP})     {return yy::parser::make_FLOAT_CONST(std::stof(yytext,nullptr),loc);}
<<EOF>>                 {return yy::parser::make_END(loc);}
.			{std::cerr << "Unknown Character: " << yytext << '\n'; exit(1);}

%%

// undefined reference to `yyFlexLexer::yylex()'
int yyFlexLexer::yylex() {}