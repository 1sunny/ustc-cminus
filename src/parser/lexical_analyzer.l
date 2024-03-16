%option noyywrap
%{
#include "driver.hpp"

// Code run each time a pattern is matched.
# define YY_USER_ACTION  loc.columns (yyleng);

%}

%%
%{
  // Code run each time yylex is called.
  // loc是CminusFlexLexer.hpp中的
  // Method on location: void step () Move begin onto end.
  loc.step ();
%}

int         {return yy::parser::make_INT(loc);}
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
"/*"([^\*]|(\*)*[^\*/])*(\*)*"*/"          {}
"//".*$						               {}
[a-zA-Z_][a-zA-Z0-9_]* 					   {return yy::parser::make_IDENTIFIER(yytext, loc);}
([1-9][0-9]*|0[0-7]*|(0x|0X)[0-9a-fA-F]*)  {return yy::parser::make_NUMBER(std::stoi(yytext,nullptr,0),loc);}
<<EOF>>                                    {return yy::parser::make_END(loc);}
.			{std::cerr << "Unknown Character !!!" << yytext << '\n'; exit(1);}

%%

// undefined reference to `yyFlexLexer::yylex()'
int yyFlexLexer::yylex() {}