#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN 100

const char *keywords[] = {
    "int", "float", "char", "if", "else", "while", "for", "return", "class",
    "public", "private", "protected", "void", "static", "new", "try", "catch",
    "import", "include", "SELECT", "FROM", "WHERE", "INSERT", "UPDATE", "DELETE"
};
int keywordCount = sizeof(keywords) / sizeof(keywords[0]);

int isKeyword(char *str) {
    for (int i = 0; i < keywordCount; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

void analyze(FILE *fp) {
    char ch, buffer[MAX_LEN];
    int i;

    while ((ch = fgetc(fp)) != EOF) {
        if (isalpha(ch) || ch == '_') {
            i = 0;
            buffer[i++] = ch;
            while (isalnum(ch = fgetc(fp)) || ch == '_') {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, fp);

            if (isKeyword(buffer))
                printf("[Keyword: %s]\n", buffer);
            else
                printf("[Identifier: %s]\n", buffer);
        }
        else if (isdigit(ch)) {
            i = 0;
            buffer[i++] = ch;
            while (isdigit(ch = fgetc(fp))) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, fp);
            printf("[Number: %s]\n", buffer);
        }
        else if (ch == '"' || ch == '\'') {
            char quote = ch;
            i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != quote && ch != EOF) {
                buffer[i++] = ch;
            }
            buffer[i++] = quote;
            buffer[i] = '\0';
            printf("[String Literal: %s]\n", buffer);
        }
        else if (ch == '/') {
            ch = fgetc(fp);
            if (ch == '/') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                printf("[Comment: Single-line]\n");
            } else if (ch == '*') {
                while ((ch = fgetc(fp)) != EOF) {
                    if (ch == '*' && fgetc(fp) == '/') break;
                }
                printf("[Comment: Multi-line]\n");
            } else {
                ungetc(ch, fp);
                printf("[Operator: /]\n");
            }
        }
        else if (ch == '-') {
            if ((ch = fgetc(fp)) == '-') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                printf("[Comment: SQL single-line]\n");
            } else {
                ungetc(ch, fp);
                printf("[Operator: -]\n");
            }
        }
        else if (strchr("=+*<>!", ch)) {
            printf("[Operator: %c]\n", ch);
        }
        else if (strchr("(){}[],.;", ch)) {
            printf("[Symbol: %c]\n", ch);
        }
    }
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    printf("Lexical Analysis Output:\n");
    analyze(fp);
    fclose(fp);
    return 0;
}
