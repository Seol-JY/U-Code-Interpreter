#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <fstream>
#include <string>
#include "Assembler.h"
#include "Memory.h"
#include "RegisterSet.h"
#include "ControlUnit.h"
#include "Logger.h"

using namespace std;

int main(int argc, const char* argv[]) {
    try {
        string ucode_path;
        string log_path;
        string opt;
        cout << "                           ┌────────────────────────┐" << endl;
        cout << "                           │   U-Code Interpreter   │" << endl;
        cout << "                           └────────────────────────┘" << endl;
        cout << left;
        cout << "┌────────────────────────────────────────────────────────────────────────────────┐" << endl;
        cout << "│                      U-Code 파일을 실행하는 프로그램 입니다.                   │"<< endl;
        cout << "│                                                                                │" << endl;
        cout << "│           디버깅 모드 사용시 지정된 라인 번호에서 프로그램이 중지되며,         │" << endl;
        cout << "│                  Enter 입력을 통해 한 줄씩 실행할 수 있습니다.                 │" << endl;
        cout << "│              통계 파일은 동일 폴더 내에 (파일명.lst)로 생성됩니다.             │" << endl;
        cout << "└────────────────────────────────────────────────────────────────────────────────┘";
        cout << "\n\nU-Code 파일 경로 입력: ";
        cin >> ucode_path;
        cin.clear();
        cin.ignore(100, '\n');

        ifstream ucode_file;
        ofstream log_file;
        ucode_file.open(ucode_path, ifstream::in);


        int ext = ucode_path.rfind("uco");
        int name = ucode_path.rfind("\\") + 1;

        log_path = ucode_path.substr(0, name);
        log_path += ucode_path.substr(name, ext - name);
        log_path += "lst";

        log_file.open(log_path);
        Memory memory;
        RegisterSet register_set;

        cout << "\n디버깅 진행 여부(Y/n): ";
        cin >> opt;
        cin.clear();
        cin.ignore(100, '\n');

        char str[100];
        int num[100], i, cnt = 0;

        if (!opt.compare("Y")) {
            cout << "\n\nBreakPoint 지정할 라인 번호 입력(공백으로 구분): ";
            scanf("%[^\n]", str);  // gets_s(str) 와 동일
            for (i = 2; i > 1; cnt += i > 0) {
                i = sscanf(str, "%d%*[ ]%[^\n]", &num[cnt], str);
            }
        }

        Logger logger(ucode_path, num, cnt);

        Assembler assembler(logger);
        assembler.assemble(ucode_file);
        memory.load(assembler);

        ControlUnit control_unit(assembler.getStartAddr(), memory, register_set, logger);
        control_unit.execute(!opt.compare("Y"));
        logger.storeLog(log_file);
    }
    catch (UcodeException& e) {
        e.What();
        system("pause");
    }

}
