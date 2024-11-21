#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <unordered_map>

struct Symbol {
    bool isConst;
    int value;  // Consts
};

void declareConst(const std::string& id, int value);
void declareVar(const std::string& id);
void startInstructions();
void checkIdentifier(const std::string& id);
void modifyConst(const std::string& id);

#endif 
