%{
#include "y.tab.h"  
#include <stdio.h>  
void yyerror(const char *s);  
%}
%%
[0-9]   { 
    printf("Syntax Error: Only double-digit numbers allowed\n"); 
    return 0; 
    } 
[0-9][0-9]+ { yylval = atoi(yytext); return NUMBER; }
[ \t] { /* Ignore whitespace */ }
\n { return '\n'; }
. { return yytext[0]; }
%%
int yywrap() {
 return 1;
}
