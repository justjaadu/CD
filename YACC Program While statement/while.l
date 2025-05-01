%{
#include "y.tab.h"
%}

%%
"while"         { return WHILE; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
[0-9]+          { return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*  { return IDENTIFIER; }
"==" { return RELOP; }
"!=" { return RELOP; }
"<=" { return RELOP; }
">=" { return RELOP; }
"<"  { return RELOP; }
">"  { return RELOP; }
"="             { return ASSIGN; }
";"             { return SEMICOLON; }
[ \t\n]         { /* Ignore Whitespace */ }
.               { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
