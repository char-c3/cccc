#include "cccc.h"

char *user_input;
Vector *tokens;
Vector *code;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    if (strcmp("--test", argv[1]) == 0) {
        runtest();
        return 0;
    }

    tokens = new_vector();
    code = new_vector();

    // トークナイズしてパースする
    user_input = argv[1];
    tokenize();
    program();

    // アセンブリの前半部を出力
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // 抽象構文木を下りながらコード生成
    for (int i = 0; i < code->len; i++) {
        gen((Node *)code->data[i]);
    }

    // スタックトップに式全体の値が残っているはずなので
    // それをRAXにロードして関数からの返り値とする
    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}
