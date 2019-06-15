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

    // プロローグ
    // 変数26個分の領域を確保する
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n"); // 208 = 26 * 8

    // 抽象構文木を下りながらコード生成
    for (int i = 0; i < code->len; i++) {
        gen((Node *)code->data[i]);

        // 式の評価結果としてスタックに一つの値が残っている
        // はずなので、スタックが溢れないようにポップしておく
        printf("  pop rax\n");
    }

    // エピローグ
    // 最後の式の結果がRAXに残っているので、それを返り値とする
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;
}
