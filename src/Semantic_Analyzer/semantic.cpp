#include "semantic.h"
#include <iostream>
#include <unordered_map>
#include <stdexcept>

std::unordered_map<std::string, Symbol> symbolTable;
std::string programID;
bool inInstructions = false;

void declareConst(const std::string& id, int value) {
    if (symbolTable.find(id) != symbolTable.end()) {
        throw std::runtime_error("Erreur: Double déclaration de constante '" + id + "'.");
    }
    symbolTable[id] = { true, value };
}

void declareVar(const std::string& id) {
    if (symbolTable.find(id) != symbolTable.end()) {
        throw std::runtime_error("Erreur: Double déclaration de variable '" + id + "'.");
    }
    symbolTable[id] = { false, 0 };
}

void startInstructions() {
    inInstructions = true;
}

void checkIdentifier(const std::string& id) {
    if (symbolTable.find(id) == symbolTable.end()) {
        throw std::runtime_error("Erreur: Identifiant '" + id + "' non déclaré.");
    }
}

void modifyConst(const std::string& id) {
    if (symbolTable.find(id) != symbolTable.end() && symbolTable[id].isConst) {
        throw std::runtime_error("Erreur: La constante '" + id + "' ne peut pas être modifiée.");
    }
}
