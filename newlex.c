#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN 100

// List of keywords for different languages
const char *keywords[] = {
    "int", "float", "char", "if", "else", "while", "for", "return", "class",
    "public", "private", "protected", "void", "static", "new", "try", "catch",
    "import", "include", "SELECT", "FROM", "WHERE", "INSERT", "UPDATE", "DELETE"
};
int keywordCount = sizeof(keywords) / sizeof(keywords[0]);

// Function to check if a string is a keyword
int isKeyword(char *str) {
    for (int i = 0; i < keywordCount; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;  // It's a keyword
    }
    return 0;  // Not a keyword
}

// Function to print token in a table format
void printToken(int line, const char *lexeme, const char *type, const char *value) {
    if (value == NULL) value = "null"; // If there's no value, print "null"
    printf("%-8d | %-15s | %-12s | %-10s\n", line, lexeme, type, value);
}

// Function to perform lexical analysis
void analyze(FILE *fp) {
    char ch, buffer[MAX_LEN];
    int i, line = 1;

    // Print table header
    printf("Line No | Lexeme          | Type         | Value\n");
    printf("--------|-----------------|--------------|------------\n");

    // Read each character from the file
    while ((ch = fgetc(fp)) != EOF) {

        // Count newlines and update the line number
        if (ch == '\n') {
            line++;
            continue;
        }

        // Skip whitespace characters
        if (isspace(ch))
            continue;

        // Identifiers or keywords
        if (isalpha(ch) || ch == '_') {
            i = 0;
            buffer[i++] = ch;
            while (isalnum(ch = fgetc(fp)) || ch == '_') {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, fp);

            if (isKeyword(buffer))
                printToken(line, buffer, "Keyword", NULL);
            else
                printToken(line, buffer, "Identifier", NULL);
        }

        // Numbers (including integers and floats)
        else if (isdigit(ch)) {
            i = 0;
            buffer[i++] = ch;
            int hasDot = 0;

            while (isdigit(ch = fgetc(fp)) || (ch == '.' && !hasDot)) {
                if (ch == '.') hasDot = 1;
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, fp);

            // For floating-point numbers, print the value of the number
            printToken(line, buffer, hasDot ? "Float" : "Integer", buffer);
        }

        // String literals
        else if (ch == '"' || ch == '\'') {
            char quote = ch;
            i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != quote && ch != EOF) {
                buffer[i++] = ch;
            }
            buffer[i++] = quote;
            buffer[i] = '\0';
            printToken(line, buffer, "String", buffer);
        }

        // Single-line or multi-line comments
        else if (ch == '/') {
            ch = fgetc(fp);
            if (ch == '/') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                line++;
                printToken(line, "//", "Comment", NULL);
            } else if (ch == '*') {
                while ((ch = fgetc(fp)) != EOF) {
                    if (ch == '\n') line++;
                    if (ch == '*' && fgetc(fp) == '/') break;
                }
                printToken(line, "/*...*/", "Comment", NULL);
            } else {
                ungetc(ch, fp);
                buffer[0] = '/'; buffer[1] = '\0';
                printToken(line, buffer, "Operator", NULL);
            }
        }

        // SQL single-line comment (starts with --)
        else if (ch == '-') {
            if ((ch = fgetc(fp)) == '-') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                line++;
                printToken(line, "--", "SQL Comment", NULL);
            } else {
                ungetc(ch, fp);
                buffer[0] = '-'; buffer[1] = '\0';
                printToken(line, buffer, "Operator", NULL);
            }
        }

        // Operators
        else if (strchr("=+*<>!%", ch)) {
            buffer[0] = ch; buffer[1] = '\0';
            printToken(line, buffer, "Operator", NULL);
        }

        // Symbols
        else if (strchr("(){}[],.;", ch)) {
            buffer[0] = ch; buffer[1] = '\0';
            printToken(line, buffer, "Symbol", NULL);
        }

        // Unknown characters
        else {
            buffer[0] = ch; buffer[1] = '\0';
            printToken(line, buffer, "Unknown", NULL);
        }
    }
}

// Main function
int main() {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    // Perform lexical analysis
    analyze(fp);
    fclose(fp);

    return 0;
}
