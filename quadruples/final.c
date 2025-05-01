#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    char op[5];
    char arg1[20];
    char arg2[20];
    char result[20];
} Quadruple;

typedef struct {
    char key[60];
    char result[20];
} ExprTableEntry;

int isCommutative(char *op) {
    return strcmp(op, "+") == 0 || strcmp(op, "*") == 0;
}

void getKey(char *key, char *op, char *arg1, char *arg2) {
    char temp1[20], temp2[20];
    strcpy(temp1, arg1);
    strcpy(temp2, arg2);

    if (isCommutative(op) && strcmp(temp1, temp2) > 0) {
        // Swap for commutative operations
        char tmp[20];
        strcpy(tmp, temp1);
        strcpy(temp1, temp2);
        strcpy(temp2, tmp);
    }

    sprintf(key, "%s|%s|%s", op, temp1, temp2);
}

int findInExprTable(ExprTableEntry *table, int size, char *key) {
    for (int i = 0; i < size; i++) {
        if (strcmp(table[i].key, key) == 0)
            return i;
    }
    return -1;
}

int main() {
    Quadruple input[MAX];
    Quadruple optimized[MAX];
    ExprTableEntry exprTable[MAX];

    int inputSize = 0, optimizedSize = 0, exprTableSize = 0;

    printf("Enter number of quadruples: ");
    scanf("%d", &inputSize);
    getchar(); // consume newline

    for (int i = 0; i < inputSize; i++) {
        printf("Enter Quadruple %d (op arg1 arg2 result): ", i + 1);
        scanf("%s %s %s %s", input[i].op, input[i].arg1, input[i].arg2, input[i].result);
    }

    printf("\nOriginal Quadruples:\n");
    for (int i = 0; i < inputSize; i++) {
        printf("(%s, %s, %s, %s)\n", input[i].op, input[i].arg1, input[i].arg2, input[i].result);
    }

    for (int i = 0; i < inputSize; i++) {
        char key[60];
        getKey(key, input[i].op, input[i].arg1, input[i].arg2);

        int foundIndex = findInExprTable(exprTable, exprTableSize, key);
        if (foundIndex != -1) {
            strcpy(optimized[optimizedSize].op, "=");
            strcpy(optimized[optimizedSize].arg1, exprTable[foundIndex].result);
            strcpy(optimized[optimizedSize].arg2, "");
            strcpy(optimized[optimizedSize].result, input[i].result);
            optimizedSize++;
        } else {
            optimized[optimizedSize] = input[i];
            strcpy(exprTable[exprTableSize].key, key);
            strcpy(exprTable[exprTableSize].result, input[i].result);
            optimizedSize++;
            exprTableSize++;
        }
    }

    printf("\nAfter Common Subexpression Elimination:\n");
    for (int i = 0; i < optimizedSize; i++) {
        printf("(%s, %s, %s, %s)\n", optimized[i].op, optimized[i].arg1, optimized[i].arg2, optimized[i].result);
    }

    return 0;
}
