#include <iostream>
#include "hackAssembler.h"

bool filenameValidation(std::string inputFile) {
    int pos = inputFile.find_last_of('.');
    if (pos == -1) {
        puts("Error: invalid file");
        puts("Usage: hackAssembler <filename>.asm");
        return false;
    }

    std::string suf = inputFile.substr(pos);
    if (suf != ".asm") {
        puts("Error: invalid file");
        puts("Usage: hackAssembler <filename>.asm");
        return false;
    }

    return true;
}

hackAssembler::hackAssembler(std::string inputFile, std::string outputFile) {
    asmFile.open(inputFile);
    hackFile.open(outputFile);
}

hackAssembler::~hackAssembler() {
    asmFile.close();
    hackFile.close();
}

std::string lineRemoval(std::string line) {
    std::string retLine = "";

    for (int i = 0; i < line.length(); ++i) {
        if (line[i] == '/' && i + 1 < line.length() && line[i + 1] == '/') break;
        if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r') continue;
        retLine += line[i];
    }

    return retLine;
}

void hackAssembler::instrA(std::string line) {
    std::string hackLine = "0", reg = line.substr(1);
    int num;

    if (reg[0] >= '0' && reg[0] <= '9') {
        num = stoi(reg);
    } else if (symbolTable.find(reg) != symbolTable.end()) {
        num = symbolTable[reg];
    } else {
        symbolTable[reg] = addr;
        num = addr, addr++;
    }

    hackLine += std::bitset<15>(num).to_string();
    hackFile << hackLine << "\n";
}

void hackAssembler::instrL(std::string line, int index) {
    std::string label = line.substr(1, line.length() - 2);
    symbolTable[label] = index;
}

void hackAssembler::instrC(std::string line) {
    std::string hackLine = "111", compStr = "", destStr = "", jumpStr = "";
    int posEq = line.find('='), posJmp = line.find(';');
    std::string compNum, destNum, jumpNum;

    if (posEq != -1) destStr = line.substr(0, posEq);
    if (posJmp != -1) jumpStr = line.substr(posJmp + 1);
    if (posEq != -1 && posJmp != -1) {
        compStr = line.substr(posEq + 1, posJmp - posEq - 1);
    } else if (posJmp == -1) {
        compStr = line.substr(posEq + 1);
    } else if (posEq == -1) {
        compStr = line.substr(0, posJmp);
    };

    compNum = comp[compStr], destNum = dest[destStr], jumpNum = jmp[jumpStr];
    hackLine += compNum + destNum + jumpNum;
    hackFile << hackLine << "\n";
}

void hackAssembler::convertAsmToHack() {
    std::string originLine, line;
    int cnt = -1;

    while (getline(asmFile, originLine)) {          // Classify the type of each line and attribute the index of each labels
        line = lineRemoval(originLine);

        if (line[0] == '@') cnt++;
        else if (line[0] == '(') {
            instrL(line, cnt + 1);
        } else {
            for (int i = 0; i < line.length(); ++i) {
                if (line[i] == '=' || line[i] == ';') {
                    cnt++;
                    break;
                }
            }
        }
    }

    asmFile.clear();
    asmFile.seekg(0, std::ios::beg);

    while (getline(asmFile, originLine)) {
        line = lineRemoval(originLine);

        if (line[0] == '@') instrA(line);
        else if (line[0] == '(') continue;
        else {
            for (int i = 0; i < line.length(); ++i) {
                if (line[i] == '=' || line[i] == ';') {
                    instrC(line);
                    break;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {                                 // Verify the number of arguments
        puts("Error: invalid command");
        puts("Usage: hackAssembler <filename>.asm");
        return 1;
    }

    std::string inputFile(argv[1]);
    if (!filenameValidation(inputFile)) return 0;   // Verify the name of the file

    int pos = inputFile.find_last_of('.');          // Name the output file
    std::string outputFile = inputFile.substr(0, pos) + ".hack";

    hackAssembler hack(inputFile, outputFile);
    hack.convertAsmToHack();
    return 0;
}