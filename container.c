#include "cccc.h"

// Tokenを生成するための関数
Token* new_token(int ty, int val, char* p) {
    Token *token = (Token *)malloc(sizeof(Token));
    token->ty = ty;
    token->val = val;
    token->input = p;
    return token;
}


Vector *new_vector() {
    Vector *vec = malloc(sizeof(Vector));
    vec->capacity = 16;
    vec->data = malloc(sizeof(void *) * vec->capacity);
    vec->len = 0;
    return vec;
}

void vec_push(Vector *vec, void *elem) {
    if (vec->capacity == vec->len) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
    }
    vec->data[vec->len++] = elem;
}

void expect(int line, int expected, int actual) {
    if (expected == actual) {
        return;
    }
    
    fprintf(stderr, "%d: %d expected, but got %d\n", line, expected, actual);
    exit(1);
}

void runtest() {
    Vector *vec = new_vector();
    expect(__LINE__, 0, vec->len);

    for (int i = 0; i < 100; i++) {
        vec_push(vec, (void *)i);
    }

    expect(__LINE__, 100, vec->len);
    expect(__LINE__, 0, (long)vec->data[0]);
    expect(__LINE__, 50, (long)vec->data[50]);
    expect(__LINE__, 99, (long)vec->data[99]);

    printf("OK\n");
}
