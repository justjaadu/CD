%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int line_count = 1;        // Counter for lines
int char_count = 0;        // Counter for characters
%}

%%

The|the|a|A|an|An {printf(" Line %d: Lexeme: %s Token: ARTICLE\n",line_count, yytext); }
is {printf("Line %d: Lexeme: %s Token: VERB\n", line_count,yytext); }
[a-z]+[ing] {printf("Line %d: Lexeme: %s Token: VERB\n", line_count,yytext); }
[a-z]+[ed] {printf("Line %d: Lexeme: %s Token: VERB\n", line_count,yytext); }
dance|see|run|walk|jump|eat|drink|sleep|talk|listen|read|write|say|tell|speak|shout|whisper|ask|answer|explain|argue|complain|think|know|understand|believe|remember|forget|want|like|love|hate|go|come|leave|arrive|turn|move|stop|sit|stand|lie|work|study|learn|teach|build|make|create|develop|solve|fix {printf("Line %d: Lexeme: %s Token: VERB\n", line_count,yytext); }
[a-zA-Z]+ {printf("Line %d: Lexeme: %s Token: NOUN\n", line_count,yytext); }

\n                        { 
                            line_count++; 
                          } 

.  {}
%%

int main() {
    printf("Enter your sentence (type 'exit' on a new line to finish):\n");

    char input[10240] = "";  // Large buffer for multiline input
    char line[1024];         // Buffer for each line of input

    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            break;  // Exit if input fails
        }
        if (strncmp(line, "exit", 4) == 0 && strlen(line) == 5) {
            break;  // Exit if user types "exit" followed by a newline
        }
        strcat(input, line);  // Append the current line to the buffer
    }

    YY_BUFFER_STATE buffer = yy_scan_string(input);
    yylex();
    yy_delete_buffer(buffer);
    
   
    
    return 0;
}
int yywrap()
{
    return 1; // Indicates end of input to flex
}