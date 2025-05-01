%{
#include <stdio.h>
#include <stdlib.h>

/* Function Declarations */
void yyerror(const char *s);
int yylex();
%}

/* Token Declaration */
%token NUMBER

/* Define precedence and associativity of operators */
%left '+' '-'

%%
/* Grammar rules */
input:
    /* empty */
  | input line
  ;

line:
    '\n'
  | exp '\n' { printf("Result: %d\n", $1); }
  ;

exp:
    NUMBER { 
        if ($1 < 10)  // Accept numbers 0-9
            $$ = $1; 
        else { 
            yyerror("Syntax Error"); 
            YYABORT; 
        } 
    }
  | exp '+' exp { $$ = $1 + $3; }
  | exp '-' exp { $$ = $1 - $3; }
  ;
%%

/* Error handling */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter expressions to evaluate (Ctrl+D to exit):\n");
    yyparse();
    return 0;
}
