%{
#include <string>
#include "../header.h"
#include "parser.tab.h"

void yyerror(const char*);
%}

%option outfile="lex.c" header-file="lex.h"
%option nounput
alpha [a-zA-z]
digit [0-9]
id  ({alpha})({alpha}|{digit})*
number ({digit})+
fnumber ({digit})+\.({digit})*
cast (intToString|intToFloat|intToChar|floatToString|floatToInt|charToInt|charToString|stringToInt|stringToFloat|stringToChar)
print (print|printLine)
read (readInt|readFloat|readChar|readString|readLine)
boolean (true|false)
compare (<=|>=|==|<|>|!=)
%%
{cast}                  {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return CAST;};
{print}                 {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return PRINT;};
{read}                  {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return READ;};
{boolean}               {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return BOOLEAN;};
and                     {return AND;};
or                      {return OR;};
not                     {return NOT;};
if                      {return IF;};
else                    {return ELSE;};
\{                      {return OBLOCKBRACKET;};
\}                      {return CBLOCKBRACKET;};
while                   {return WHILE;};
for                     {return FOR;};
in                      {return IN;};
range                   {return RANGE;};
do                      {return DO;};
{id}                    {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return ID;}
{number}                {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return NUMBER;}
{fnumber}               {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return FNUMBER;}
{compare}               {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return COMPARE;}
=                       {return ASSIGNMENT;}
,                       {return COMMA;}
\(                      {return OBRACKET;}
\)                      {return CBRACKET;}
\+                      {return PLUS;}
-                       {return MINUS;}
\%                      {return MOD;}
\*                      {return TIMES;}
\/                      {return DIVIDE;}
\".*\"                  {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return STRING;}
\'.?\'                  {yylval.expression_value = new expression_struct(); yylval.expression_value->string_value = strdup(yytext); return CHAR;}
[ \r\t\n]+                ;
.                       {char error[256]; sprintf(error, "unexpected symbol '%s'", yytext); yyerror(error); exit(1);}
%%