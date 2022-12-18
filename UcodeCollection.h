#pragma once
#include <iostream>

/* 내장 함수 주소 */
#define READ_ADDR 1000
#define WRITE_ADDR 1001
#define LF_ADDER 1002

using namespace std;

const string OPCODE[37] = { "nop", "bgn", "sym",
                        "lod", "lda", "ldc", "str", "ldi", "sti",
                        "not", "neg", "inc", "dec", "dup",
                        "add", "sub", "mult", "div", "mod", "gt", "lt", "ge", "le", "eq", "ne", "and", "or", "swp",
                        "ujp", "tjp", "fjp",
                        "call", "ret", "retv", "ldp", "proc", "end",
};

typedef enum {
    NOP, BGN, SYM,
    LOD, LDA, LDC, STR, LDI, STI,
    NOT, NEG, INC, DEC, DUP,
    ADD, SUB, MULT, DIV, MOD, GT, LT, GE, LE, EQ, NE, AND, OR, SWP,
    UJP, TJP, FJP,
    CALL, RET, RETV, LDP, PROC_, END,
} Opcode;

typedef struct {
    Opcode opcode;
    int operand1;
    int operand2;
} Instruction;

typedef struct {
    string label;
    string opcode;
    string operand1;
    string operand2;
} UCode;

typedef struct {
    string name;
    int index;
} Label;

class UcodeException {
public:
    virtual void What() = 0;
};

class UcodeRuntimeException : public UcodeException {
private:
    string msg_;
    int line_num_;
public:
    UcodeRuntimeException(string msg, int line_num);
    void What();
};

class UcodeAssemblerException : public UcodeException {
private:
    string msg_;
    int line_num_;
public:
    UcodeAssemblerException(string msg, int line_num);
    void What();
};
