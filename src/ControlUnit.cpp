#include <string>
#include "ControlUnit.h"
#include "UcodeCollection.h"

ControlUnit::ControlUnit(int start_addr, Memory& memory, RegisterSet& register_set, Logger& logger) : start_addr_(start_addr), memory_(memory), register_set_(register_set), logger_(logger) {}

ControlUnit::~ControlUnit() {}

void ControlUnit::execute(int debug) {
    cout << " == Executing ... ==" << endl;

    register_set_.setProgramCounter(start_addr_);
    logger_.initWindowBuffer();

    while (true) {
        Instruction instruction = this->memory_.fetchInstruction(register_set_.getProgramCounter());
        int before_pc = register_set_.getProgramCounter();
        register_set_.increaseProgramCounter();

        int tmp1, tmp2;
        logger_.increaseOpcodeCount(instruction.opcode);
        switch ((int)instruction.opcode) {
        case NOP:
            break;
        case BGN:
            break;
        case SYM:
            break;
        case LOD:
            register_set_.pushGpRegister(memory_.loadData(instruction.operand1));
            break;
        case LDA:
            register_set_.pushGpRegister(instruction.operand1);
            break;
        case LDC:
            register_set_.pushGpRegister(instruction.operand1);
            break;
        case STR:
            memory_.storeData(instruction.operand1, register_set_.popGpRegister());
            break;
        case LDI:
            register_set_.pushGpRegister(memory_.loadData(register_set_.popGpRegister()));
            break;
        case STI:
            tmp1 = register_set_.popGpRegister();
            memory_.storeData(register_set_.popGpRegister(), tmp1);
            break;
        case NOT:
            register_set_.pushGpRegister(!(register_set_.popGpRegister()));
            break;
        case NEG:
            register_set_.pushGpRegister(-(register_set_.popGpRegister()));
            break;
        case INC:
            register_set_.pushGpRegister(1 + (register_set_.popGpRegister()));
            break;
        case DEC:
            register_set_.pushGpRegister(-1 + (register_set_.popGpRegister()));
            break;
        case DUP:
            tmp1 = register_set_.popGpRegister();
            register_set_.pushGpRegister(tmp1);
            register_set_.pushGpRegister(tmp1);
            break;
        case ADD:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister(tmp2 + tmp1);
            break;
        case SUB:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister(tmp2 - tmp1);
            break;
        case MULT:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister(tmp2 * tmp1);
            break;
        case DIV:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister((int)(tmp2 / tmp1));
            break;
        case MOD:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister(tmp2 % tmp1);
            break;
        case GT:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister((int)(tmp2 > tmp1));
            break;
        case LT:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister((int)(tmp2 < tmp1));
            break;
        case GE:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister((int)(tmp2 >= tmp1));
            break;
        case LE:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister((int)(tmp2 <= tmp1));
            break;
        case EQ:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister((int)(tmp2 == tmp1));
            break;
        case NE:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister((int)(tmp2 != tmp1));
            break;
        case AND:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister((int)(tmp2 && tmp1));
            break;
        case OR:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister((int)(tmp2 || tmp1));
            break;
        case SWP:
            tmp1 = register_set_.popGpRegister();
            tmp2 = register_set_.popGpRegister();
            register_set_.pushGpRegister(tmp1);
            register_set_.pushGpRegister(tmp2);
            break;
        case UJP:
            register_set_.setProgramCounter(instruction.operand1);
            break;
        case TJP:
            if (register_set_.popGpRegister()) {
                register_set_.setProgramCounter(instruction.operand1);
            }
            break;
        case FJP:
            if (!register_set_.popGpRegister()) {
                register_set_.setProgramCounter(instruction.operand1);
            }
            break;
        case CALL:
            if (instruction.operand1 == WRITE_ADDR) {
                logger_.updateOutBuffer(to_string(register_set_.popGpRegister()));
            }
            else if (instruction.operand1 == LF_ADDER) {
                logger_.updateOutBuffer("\n");
            }
            else if (instruction.operand1 == READ_ADDR) {
                
            }
            else {
                memory_.pushStackArea(register_set_.getProgramCounter());
                register_set_.setProgramCounter(instruction.operand1);
            }
            break;
        case RET:
        case RETV:
        case LDP:
        case PROC_:
            break;
        case END:
            if (memory_.isStackAreaEmpty()) {
                logger_.updateWindow(-1, 0, memory_.getStackArea(), register_set_.getGpRegister());
                return;
            }
            register_set_.setProgramCounter(memory_.popStackArea());
            break;
        default:
            throw UcodeRuntimeException("제어장치에서 오류가 발생했습니다", register_set_.getProgramCounter());
            break;
        }
        if (debug) { logger_.updateWindow(before_pc, register_set_.getProgramCounter()+1, memory_.getStackArea(), register_set_.getGpRegister()); }
    }
}
