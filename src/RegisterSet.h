#pragma once
#include <stack>

using namespace std;

class RegisterSet {
public:
    RegisterSet();
    ~RegisterSet();

    void increaseProgramCounter();
    int getProgramCounter();
    stack<int> getGpRegister();
    void setProgramCounter(int addr);

    void pushGpRegister(int data);
    int popGpRegister();

private:
    stack<int> gp_register; // 범용 레지스터, 스택
    int program_counter; // 프로그램 카운터
};