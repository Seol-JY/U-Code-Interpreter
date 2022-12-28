#include "UcodeCollection.h"

UcodeAssemblerException::UcodeAssemblerException(string msg, int line_num) :msg_(msg), line_num_(line_num) {};
void UcodeAssemblerException::What() {
    cout << "AssemblerError: " << msg_ << ", line " << line_num_ << endl;
}

UcodeRuntimeException::UcodeRuntimeException(string msg, int line_num) :msg_(msg), line_num_(line_num) {};
void UcodeRuntimeException::What() {
    cout << "RuntimeError: " << msg_ << ", addr " << line_num_ << endl;
}
