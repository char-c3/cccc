#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの型を表す値
enum {
    TK_NUM = 256,   // 整数トークン
    TK_IDENT,       // 識別子
    TK_EQ,          // ==
    TK_NE,          // !=
    TK_LE,          // <=
    TK_GE,          // >=
    TK_EOF,         // 入力の終わりを表すトークン
};

enum {
    ND_NUM = 256,   // 整数のノードの型
};

// トークンの型
typedef struct {
    int ty;      // トークンの型
    int val;     // tyがTK_NUMの場合、その数値
    char *input; // トークン文字列 (エラーメッセージ用)
} Token;

typedef struct Node {
    int ty;             // 演算子かND_NUM
    struct Node *lhs;   // 左辺
    struct Node *rhs;   // 右辺
    int val;            // tyがND_NUMの場合のみ使う
} Node;

typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Token *new_token(int ty, int val, char* p);
Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void expect(int line, int expected, int actual);
void runtest();

void error(char *fmt, ...);
void error_at(char *loc, char *msg);
void tokenize();
Node *new_node(int ty, Node *lhs, Node *rhs);
Node *new_node_num(int val);
int consume(int ty);
Node *term();
Node *unary();
Node *mul();
Node *add();
Node *relational();
Node *equality();
Node *assign();
Node *expr();
Node *stmt();
Node *program();

void gen(Node *node);

// 入力プログラム
extern char *user_input;

// トークナイズした結果のトークン列はこの配列に保存する
// 100個以上のトークンは来ないものとする
// Token tokens[100];
extern Vector *tokens;

extern int pos;