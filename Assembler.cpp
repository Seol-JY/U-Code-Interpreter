#include <fstream>
#include <string>
#include "Assembler.h"

using namespace std;

Assembler::Assembler(Logger& logger) : logger_(logger), data_area_size(0), length(0) {};
Assembler::~Assembler() {};

void Assembler::assemble(ifstream& ucode_file) {
    int begin_flag = 0;
    int end_flag = 0;

    vector<int> line_for_postprocessing;
    vector<string> name_for_postprocessing;

    cout << " == Assembling ... ==" << '\n';
    if (ucode_file.fail()) {
        throw UcodeAssemblerException("파일을 찾을 수 없습니다", length);
    }

    while (getline(ucode_file, buffer)) {
        length++;
        UCode u_code; u_code.opcode = ""; u_code.operand1 = ""; u_code.operand2 = "";
        u_codes.push_back(u_code); // 문자열 형태의 U-Code 수집을 위한 구조체 생성

        if (buffer.length() < 13 || buffer.length() > 21) {
            throw UcodeAssemblerException("명령어 형식이 잘못 되었습니다 [잘못된 명령어 길이]", length);
        }

        // cout << buffer << endl;
        translateLabel(buffer);
        // cout << translateOpcode(buffer) << endl;

        Opcode opcode_ = (Opcode)translateOpcode(buffer);

        Instruction instruction;
        instruction.opcode = opcode_;
        instruction.operand1 = -1; // 구조체 초기화
        instruction.operand2 = -1;

        switch (opcode_) {
        case UJP:
        case TJP:
        case FJP:
        case CALL:
            name_for_postprocessing.push_back(translateLabelOperand(buffer));
            line_for_postprocessing.push_back(length);
            instructions.push_back(instruction);
            break;

        case BGN:
            start_addr = length - 1;
            begin_flag = 1;
            instructions.push_back(instruction);
            break;

        case END:
            if (begin_flag) {
                end_flag = 1;
            }
            instructions.push_back(instruction);
            break;

        case NOP:
        case LDI:
        case STI:
        case NOT:
        case NEG:
        case INC:
        case DEC:
        case DUP:
        case ADD:
        case SUB:
        case MULT:
        case DIV:
        case MOD:
        case GT:
        case LT:
        case GE:
        case LE:
        case EQ:
        case NE:
        case AND:
        case OR:
        case SWP:
        case RET:
        case RETV:
        case LDP:
            instructions.push_back(instruction);
            break;

        case SYM:
            instruction.operand1 = translateIntegerOperand(1, buffer);
            instruction.operand2 = translateIntegerOperand(2, buffer);

            data_area_size = data_area_size < (instruction.operand1 + instruction.operand2) ? (instruction.operand1 + instruction.operand2) : data_area_size;
            instructions.push_back(instruction);
            break;

        case LOD:
        case LDA:
        case LDC:
        case STR:
        case PROC_:
            instruction.operand1 = translateIntegerOperand(1, buffer);
            instructions.push_back(instruction);
            break;


        default:
            throw UcodeAssemblerException("명령어 형식이 잘못 되었습니다 [유효하지 않은 연산자]", length);
        }
    }

    for (int idx = 0; idx < line_for_postprocessing.size(); idx++) {
        int success = 0;
        for (Label label_item : labels) {
            if (label_item.name.compare(name_for_postprocessing[idx]) == 0) {
                instructions[line_for_postprocessing[idx] - 1].operand1 = label_item.index;
                success++;
                break;
            }
            else if (name_for_postprocessing[idx].compare("read") == 0) {  // 내장함수에 대한 주소 지정
                instructions[line_for_postprocessing[idx] - 1].operand1 = READ_ADDR;
                success++;
                break;
            }
            else if (name_for_postprocessing[idx].compare("write") == 0) {
                instructions[line_for_postprocessing[idx] - 1].operand1 = WRITE_ADDR;
                success++;
                break;
            }
            else if (name_for_postprocessing[idx].compare("lf") == 0) {
                instructions[line_for_postprocessing[idx] - 1].operand1 = LF_ADDER;
                success++;
                break;
            }
        }
        if (!success) {
            throw UcodeAssemblerException("명령어 형식이 잘못 되었습니다 [해당 레이블이나 내장 함수가 존재하지 않음]", line_for_postprocessing[idx]);
        }
    }
    if (!end_flag) {
        throw UcodeAssemblerException("프로그램 실행 코드가 존재하지 않습니다", length);
    }
    logger_.setLength(this->length);
    logger_.setStartAddr(start_addr);
    logger_.setDataAreaSize(data_area_size);
    logger_.setLabels(labels);
    logger_.setInstructions(instructions);
    logger_.setUCodes(u_codes);
}

int Assembler::translateLabel(string buffer) {
    string tmp = buffer.substr(0, 10);
    tmp = tmp.erase(tmp.find_last_not_of(' ') + 1);
    if (tmp.length()) {
        u_codes[length - 1].label = tmp; // 로그에 기록
        Label label;
        label.name = tmp;
        label.index = this->length - 1;
        labels.push_back(label);
    }
    return 1;
}

int Assembler::translateOpcode(string buffer) {
    string tmp = buffer.substr(11);
    if (tmp.find(' ') != string::npos) {
        tmp.erase(tmp.find(' '));
    }
    for (int i = 0; i < size(OPCODE); i++) {
        if (!tmp.compare(OPCODE[i])) {
            u_codes[length - 1].opcode = tmp; // 로그에 기록
            return i;
        }
    }
    return -1;
}

string Assembler::translateLabelOperand(string buffer) {
    string tmp = buffer.substr(11);
    if (tmp.find(' ') != string::npos) {
        u_codes[length - 1].operand1 = tmp.erase(0, tmp.find(' ') + 1); // 로그에 기록
        return tmp.erase(0, tmp.find(' ') + 1);
    }
    throw UcodeAssemblerException("명령어 형식이 잘못 되었습니다 [유효하지 않은 피연산자]", this->length);
    return "";
}

int Assembler::translateIntegerOperand(int operand_num, string buffer) {
    int number_of_operand;
    string operand1_tmp = "";
    string operand2_tmp = "";

    string tmp = buffer.substr(11);

    if (tmp.find(' ') == string::npos) {
        throw UcodeAssemblerException("명령어 형식이 잘못 되었습니다 [유효하지 않은 피연산자]", this->length);
    }
    tmp.erase(0, tmp.find(' ') + 1); // 연산자 제거

    if (tmp.find(' ') == string::npos) {
        operand1_tmp = tmp;
        number_of_operand = 1;
    }
    else {
        operand1_tmp = tmp.substr(0, tmp.find(" "));
        operand2_tmp = tmp.substr(tmp.find(" ") + 1);
        number_of_operand = 2;
    }

    if (operand_num == 1) {
        if (!operand1_tmp.length()) {
            throw UcodeAssemblerException("명령어 형식이 잘못 되었습니다 [피연산자 부족]", this->length);
        }
        else {
            for (int i = 0; i < operand1_tmp.length(); i++) {
                if (!isdigit(operand1_tmp[i])) {
                    throw UcodeAssemblerException("명령어 형식이 잘못 되었습니다 [유효하지 않은 피연산자]", this->length);
                }
            }
            u_codes[length - 1].operand1 = operand1_tmp; // 로그에 기록
            return stoi(operand1_tmp);
        }

    }
    else if (operand_num == 2) {
        if (number_of_operand != 2) {
            throw UcodeAssemblerException("함수 오류 [translateIntegerOperand]", this->length);
        }
        else {
            if (!operand2_tmp.length()) {
                throw UcodeAssemblerException("명령어 형식이 잘못 되었습니다 [피연산자 부족]", this->length);
            }
            else {
                for (int i = 0; i < operand2_tmp.length(); i++) {
                    if (!isdigit(operand2_tmp[i])) {
                        throw UcodeAssemblerException("명령어 형식이 잘못 되었습니다 [유효하지 않은 피연산자]", this->length);
                    }
                }
                u_codes[length - 1].operand2 = operand2_tmp; // 로그에 기록
                return stoi(operand2_tmp);
            }
        }
    }
    throw UcodeAssemblerException("함수 오류 [translateIntegerOperand]", this->length);
    return -1;
}


int Assembler::getLength() {
    return this->length;
}

int Assembler::getDataAreaSize() {
    return this->data_area_size;
}

int Assembler::getStartAddr() {
    return this->start_addr;
}

vector<Instruction> Assembler::getInstructions() {
    return this->instructions;
}

vector<UCode> Assembler::getUCodes() {
    return this->u_codes;
}

