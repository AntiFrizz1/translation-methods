%{
    #include "parser.h"
%}

digit [0-9]
number {digit}+
%%
{number}    {expression_type* ans = new expression_type(); ans->value = atoi(zztext.c_str()); zzlval = ans; return NUMBER;};
\+          {return PLUS;};
-           {return MINUS;};
\*          {return TIMES;};
/           {return DIVIDE;};
\(          {return LPAREN;};
\)          {return RPAREN;};
<<EOF>>     {return ENDOF;};
[ \t\n\r]   {};
.           {throw std::runtime_error("unexpected symbol" + zztext);};
%%
