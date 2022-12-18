#pragma once
#include "Memory.h"
#include "RegisterSet.h"
#include "Logger.h"

class ControlUnit {
public:
    ControlUnit(int start_addr, Memory& memory, RegisterSet& register_set, Logger& logger);
    ~ControlUnit();
    void execute(int debug=0);

private:
    int start_addr_;
    Memory& memory_;
    RegisterSet& register_set_;
    Logger& logger_;
};
