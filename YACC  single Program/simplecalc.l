%{
#include "y.tab.h"   // Include Bison-generated header
#include <stdio.h>   // For printf()
void yyerror(const char *s);  // Declare yyerror()
%}

%%

[0-9]   { yylval = atoi(yytext); return NUMBER; } // Accept only single-digit numbers
[0-9][0-9]+ { 
    printf("Syntax Error: Only single-digit numbers allowed\n"); 
    return 0;  // Stops further parsing
}
[ \t]   { /* Ignore spaces and tabs */ }
\n      { return '\n'; }
.       { return yytext[0]; }  /* Return operators and other characters */

%%

int yywrap() {
    return 1;
}
