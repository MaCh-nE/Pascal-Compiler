#ifndef LEXICAL_H
#define LEXICAL_H

#include <string>
#include <vector>
#include <unordered_map>

// Token Types Enum
enum TokenType {
    PROGRAM_TOKEN, CONST_TOKEN, VAR_TOKEN, BEGIN_TOKEN, END_TOKEN, IF_TOKEN,
    THEN_TOKEN, WHILE_TOKEN, DO_TOKEN, READ_TOKEN, WRITE_TOKEN,
    PV_TOKEN, PT_TOKEN, PLUS_TOKEN, MOINS_TOKEN, MULT_TOKEN, DIV_TOKEN,
    VIR_TOKEN, AFF_TOKEN, INF_TOKEN, INFEG_TOKEN, SUP_TOKEN, SUPEG_TOKEN,
    DIFF_TOKEN, PO_TOKEN, PF_TOKEN, FIN_TOKEN, NUMBER_TOKEN, IDENTIFIER_TOKEN,
    ERROR_TOKEN, EOF_TOKEN
};

// Token Names Array Declaration
extern const std::string TOKEN_NAMES[];

// Token Structure
struct Token {
    TokenType type;
    std::string tokenName;
    std::string value;
};

// Function Declarations
Token createToken(TokenType type, const std::string& value);
Token getNextToken();
std::vector<Token> getTokensFromFile(const std::string& filename);

#endif