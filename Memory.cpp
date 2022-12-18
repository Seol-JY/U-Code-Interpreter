#include "Memory.h"

Memory::Memory() {}

Memory::~Memory() {
    delete[] data_area;
}

stack<int> Memory::getStackArea() {
    return this->stack_area;
}

void Memory::load(Assembler& assembler) {
    this->code_area = assembler.getInstructions();
    this->data_area_size = assembler.getDataAreaSize();
    this->data_area = new int[this->data_area_size];
}

Instruction Memory::fetchInstruction(int addr) {
    if (addr < 0 || addr >= code_area.size()) {
        throw UcodeRuntimeException("명령어 인출 과정에서 오류가 발생했습니다", addr + 1);
    }
    return code_area[addr];
}

int Memory::loadData(int addr) {
    if (addr < 0 || addr >= this->data_area_size) {
        throw UcodeRuntimeException("데이터를 불러오는 과정에서 오류가 발생했습니다", addr + 1);
    }
    return data_area[addr];
}

void Memory::storeData(int addr, int data) {
    if (addr < 0 || addr >= this->data_area_size) {
        throw UcodeRuntimeException("데이터를 불러오는 과정에서 오류가 발생했습니다", addr + 1);
    }
    data_area[addr] = data;
}

void Memory::pushStackArea(int data) {
    stack_area.push(data);
}

int Memory::popStackArea() {
    if (stack_area.empty()) {
        throw UcodeRuntimeException("스텍 영역에서 오류가 발생했습니다", -1);
    }
    int data = stack_area.top();
    stack_area.pop();
    return data;
}

bool Memory::isStackAreaEmpty() {
    return stack_area.empty();
}
