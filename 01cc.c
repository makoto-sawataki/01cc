#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// token 
typedef enum{
    TK_RESERVED,                    // symbol
    TK_NUM,                         // token of numeral
    TK_EOF                          // token of its end of input
}TOKEN_KIND;

typedef struct st_token_t token_t;  // "typedef type tag name" style is normal but this style makes me confused.

struct st_token_t{
    TOKEN_KIND tk;                  // type of Token
    token_t *next;                  // next token
    int val;                        // if tk == TK_NUM, this is its value.
    char *str;                      // string of token
};

// current token
token_t *token;


// error messaging function
void error_01cc(char *fmt, ...){        // variable argument
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// if next token is the sign which is expected, return true. if else, return false.   
bool consume_01cc(char op){
    if(token->tk != TK_RESERVED || token->str[0] != op){ return false;}

    token = token->next;
    return true;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect_01cc(char op){
    if(token->tk != TK_RESERVED || token->str[0] != op) {
        error_01cc("'%c'ではありません", op);
    }
    token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number_01cc(){
    if(token->tk != TK_NUM)
    {
        error_01cc("is not number.");
    }

    int val =token->val;
    token = token->next;
    return val;
}

// 
bool at_eof_01cc(){
    return token->tk == TK_EOF;
}

//　新しいトークンを作成してcurにつなげる。
token_t *new_token_01cc(TOKEN_KIND tk, token_t *cur, char *str){
    token_t *tok = calloc(1, sizeof(token_t));                          // calloc(1, sizeof(data_type)); 
    tok->tk = tk;
    tok->str = str;
    cur->next = tok;
    return tok;
}

//
token_t *tokenize_01cc(char *p){
    token_t head;
    head.next = NULL;
    token_t *cur = &head;

    while(*p){
        // skip blank character
        if(isspace(*p))
        {
            p++;
            continue;
        }

        if(*p == '+' || *p == '-')
        {
            cur = new_token_01cc(TK_RESERVED, cur, p++);
            continue;
        }

        if(isdigit(*p)){
            cur = new_token_01cc(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error_01cc("cannnot toknize");
    }

    new_token_01cc(TK_EOF, cur, p);
    
    return head.next;
}


int main (int argc, char **argv){
    if(argc != 2){
        error_01cc("fault argument.");
        return 1;
    }

    // do toknization
    token = tokenize_01cc(argv[1]);

    // show part of former words of assembly
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // the exation must start number. Check it and show command of assembly 'mov' 
    printf("    mov rax, %d\n", expect_number_01cc());
    
    // 
    while(!at_eof_01cc()){
        if( consume_01cc('+')){
            printf("    add rax, %d\n", expect_number_01cc());
            continue;
        }
        
        expect_01cc('-');
            printf("    sub rax, %d\n", expect_number_01cc());
    }

    printf("    ret\n");
    
    return 0;
}