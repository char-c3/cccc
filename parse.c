#include "cccc.h"

int pos = 0;

// エラーを報告するための関数
// printfと同じ引数をとる
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// エラー個所を報告するための関数
void error_at(char *loc, char *msg) {
    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ %s\n", msg);
    exit(1);
}

// user_inputが指している文字列を
// トークンに分割してtokensに保存する
void tokenize() {
    char *p = user_input;

    int i = 0;
    while (*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '=') {
            p++;
            if (*p == '=') {
                // tokens[i].ty = TK_EQ;
                // tokens[i].input = p;
                vec_push(tokens, new_token(TK_EQ, 0, p));
                i++;
                p++;
            } else {
                error_at(p, "トークナイズできません。=がありません。");
            }
            continue;
        }

        if (*p == '!') {
            p++;
            if (*p == '=') {
                // tokens[i].ty = TK_NE;
                // tokens[i].input = p;
                vec_push(tokens, new_token(TK_NE, 0, p));
                i++;
                p++;
            } else {
                error_at(p, "トークナイズできません。=がありません。");
            }
            continue;
        }

        if (*p == '<') {
            p++;
            if (*p == '=') {
                // tokens[i].ty = TK_LE;
                // tokens[i].input = p;
                vec_push(tokens, new_token(TK_LE, 0, p));
                i++;
                p++;
            } else {
                // tokens[i].ty = '<';
                // tokens[i].input = p - 1;
                vec_push(tokens, new_token('<', 0, p - 1));
                i++;
            }
            continue;
        }

        if (*p == '>') {
            p++;
            if (*p == '=') {
                // tokens[i].ty = TK_GE;
                // tokens[i].input = p;
                vec_push(tokens, new_token(TK_GE, 0, p));
                i++;
                p++;
            } else {
                // tokens[i].ty = '>';
                // tokens[i].input = p - 1;
                vec_push(tokens, new_token('>', 0, p - 1));
                i++;
            }
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/'
         || *p == '(' || *p == ')' || *p == '>' || *p == ';') {
            // tokens[i].ty = *p;
            // tokens[i].input = p;
            vec_push(tokens, new_token(*p, 0, p));
            i++;
            p++;
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            vec_push(tokens, new_token(TK_IDENT, 0, p));
            i++;
            p++;
            continue;
        }

        if (isdigit(*p)) {
            // tokens[i].ty = TK_NUM;
            // tokens[i].input = p;
            // tokens[i].val = strtol(p, &p, 10);
            vec_push(tokens, new_token(TK_NUM, strtol(p, &p, 10), p));
            i++;
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    // tokens[i].ty = TK_EOF;
    // tokens[i].input = p;
    vec_push(tokens, new_token(TK_EOF, 0, p));
}

Node *new_node(int ty, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->ty = ty;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = malloc(sizeof(Node));
    node->ty = ND_NUM;
    node->val = val;
    return node;
}

int consume(int ty) {
    // if (tokens[pos].ty != ty) {
    if (((Token *)tokens->data[pos])->ty != ty) {
        return 0;
    }
    pos++;
    return 1;
}

Node *term() {
    // 次のトークンが'('なら、"(" expr ")"のはず
    if (consume('(')) {
        Node *node = expr();
        if (!consume(')')) {
            error_at(((Token *)tokens->data[pos])->input, "開き括弧に対応する閉じ括弧がありません");
        }
        return node;
    }

    // そうでなければ数値のはず
    if (((Token *)tokens->data[pos])->ty == TK_NUM) {
        return new_node_num(((Token *)tokens->data[pos++])->val);
    }

    error_at(((Token *)tokens->data[pos])->input, "数値でも開きかっこでもないトークンです");
}

Node *unary() {
    if (consume('+')) {
        return term();
    } else if (consume('-')) {
        return new_node('-', new_node_num(0), term());
    }
    return term();
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume('*')) {
            node = new_node('*', node, unary());
        } else if (consume('/')) {
            node = new_node('/', node, unary());
        } else {
            return node;
        }
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume('+')) {
            node = new_node('+', node, mul());
        } else if (consume('-')) {
            node = new_node('-', node, mul());
        } else {
            return node;
        }
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume('<')) {
            node = new_node('<', node, add());
        } else if (consume('>')) {
            node = new_node('<', add(), node);
        } else if (consume(TK_LE)) {
            node = new_node(TK_LE, node, add());
        } else if (consume(TK_GE)) {
            node = new_node(TK_LE, add(), node);
        } else {
            return node;
        }
    }
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume(TK_EQ)) {
            node = new_node(TK_EQ, node, relational());
        } else if (consume(TK_NE)) {
            node = new_node(TK_NE, node, relational());
        } else {
            return node;
        }
    }
}

Node *assign() {
    return equality();
}

Node *expr() {
    return assign();
}

Node *stmt() {
    return expr();
}

Node *program() {
    return stmt();
}