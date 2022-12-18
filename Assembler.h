#pragma once
#include <iostream>
#include <vector>
#include "UcodeCollection.h"
#include "Logger.h"

using namespace std;

class Assembler {
public:
    Assembler(Logger& logger);
    ~Assembler();
    void assemble(ifstream& ucode_file);

    int getLength();
    int getDataAreaSize();
    int getStartAddr();
    vector<Instruction> getInstructions();
    vector<UCode> getUCodes();

private:
    int translateLabel(string buffer);
    int translateOpcode(string buffer);
    string translateLabelOperand(string buffer);
    int translateIntegerOperand(int operand_num, string buffer);

private:
    Logger& logger_;
    vector<Instruction> instructions;
    vector<UCode> u_codes;
    vector<Label> labels;
    int length;  // 명령어 전체 길이`
    int data_area_size; // 데이터 영역의 크기
    int start_addr;  // 명령어 시작 주소
    string buffer;
};
