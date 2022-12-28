#pragma once
#include <vector>
#include <stack>
#include "Assembler.h"
#include "UcodeCollection.h"
#include "Logger.h"

class Memory {
public:
    Memory();
    ~Memory();
    stack<int> getStackArea();

    void load(Assembler& assembler);
    Instruction fetchInstruction(int addr);

    int loadData(int addr);
    void storeData(int addr, int data);

    void pushStackArea(int data);
    int popStackArea();
    bool isStackAreaEmpty();

private:
    stack<int> stack_area;
    int* data_area;
    int data_area_size;
    vector<Instruction> code_area;
};
