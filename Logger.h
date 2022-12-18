#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include "UcodeCollection.h"

class Logger {
public:
    Logger(string path, int* breakpoints, int breakpoint_num);
    void setLength(int length);
    void setStartAddr(int start_addr);
    void setDataAreaSize(int data_area_size);
    void setLabels(vector<Label> labels);
    void setInstructions(vector<Instruction> instrcutions);
    void setUCodes(vector<UCode> u_codes);
    void increaseOpcodeCount(Opcode opcode);
    void storeLog(ofstream& log_file);
    void initWindowBuffer();
    void updateWindow(int idx, int program_counter, stack<int> stack_area, stack<int> gp_register);
    void updateOutBuffer(string out);

private:
    string name;
    string execution_date;
    vector<string> window_buffer;
    string out_buffer;
    int branch_count;
    int* opcode_count;
    int cycle_count;
    int length_;
    int start_addr_;
    int data_area_size_;
    int* breakpoints_;
    int breakpoint_num_;

    vector<Instruction> instructions_;
    vector<UCode> u_codes_;
    vector<Label> labels_;
};
