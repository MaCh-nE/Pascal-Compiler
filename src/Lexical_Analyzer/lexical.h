#ifndef LEXICAL_H
#define LEXICAL_H

#include <string>
#include <vector>
#include <unordered_map>

// Token Types Enum
enum TokenType {
    PROGRAM_TOKEN,           // 'program'
    CONST_TOKEN,             // 'const'
    VAR_TOKEN,               // 'var'
    BEGIN_TOKEN,             // 'begin'
    END_TOKEN,               // 'end'
    IF_TOKEN,                // 'if'
    THEN_TOKEN,              // 'then'
    WHILE_TOKEN,             // 'while'
    DO_TOKEN,                // 'do'
    READ_TOKEN,              // 'read'
    WRITE_TOKEN,             // 'write'
    PV_TOKEN,                // ';' (semicolon)
    PT_TOKEN,                // '.' (period)
    PLUS_TOKEN,              // '+' (addition operator)
    MOINS_TOKEN,             // '-' (subtraction operator)
    MULT_TOKEN,              // '*' (multiplication operator)
    DIV_TOKEN,               // '/' (division operator)
    VIR_TOKEN,               // ',' (comma)
    AFF_TOKEN,               // ':=' (assignment operator)
    INF_TOKEN,               // '<' (less than)
    INFEG_TOKEN,             // '<=' (less than or equal to)
    SUP_TOKEN,               // '>' (greater than)
    SUPEG_TOKEN,             // '>=' (greater than or equal to)
    DIFF_TOKEN,              // '<>' (not equal)
    PO_TOKEN,                // '(' (left parenthesis)
    PF_TOKEN,                // ')' (right parenthesis)
    FIN_TOKEN,               // 'end' (end of program)
    NUMBER_TOKEN,            // Numeric values (integer)
    REAL_NUMBER_TOKEN,       // Real numbers (floats)
    IDENTIFIER_TOKEN,        // Identifiers (variable names, function names)
    ERROR_TOKEN,             // Error token (invalid token)
    EOF_TOKEN,               // End of file (EOF)
    ELSE_TOKEN,              // 'else'
    CONST_ASSIGN_TOKEN,      // '=' in constant definitions (e.g., pi = 3.14159)
    ARRAY_TOKEN,             // 'array'
    RECORD_TOKEN,            // 'record'
    FOR_TOKEN,               // 'for'
    CASE_TOKEN,              // 'case'
    REPEAT_TOKEN             // 'repeat'
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