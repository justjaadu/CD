%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex();
%}

%token WHILE LPAREN RPAREN LBRACE RBRACE NUMBER IDENTIFIER RELOP ASSIGN SEMICOLON

%%

program:
    WHILE LPAREN condition RPAREN LBRACE statement_list RBRACE
    {
        printf("Valid while loop syntax!\n");
    }
    ;

condition:
    IDENTIFIER RELOP NUMBER
    | NUMBER RELOP IDENTIFIER
    | IDENTIFIER RELOP IDENTIFIER
    ;

statement_list:
    statement_list statement
    | statement
    ;

statement:
    IDENTIFIER ASSIGN NUMBER SEMICOLON
    | IDENTIFIER ASSIGN IDENTIFIER SEMICOLON
    ;

%%

void yyerror(const char *s) {
    printf("Syntax error: %s\n", s);
}

int main() {
    printf("Enter a while loop statement:\n");
    yyparse();
    return 0;
}
