#include "lexical.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <cctype>

// Static file stream
static std::ifstream inputFile;

// Token Names Array Definition
const std::string TOKEN_NAMES[] = {
    "PROGRAM_TOKEN", "CONST_TOKEN", "VAR_TOKEN", "BEGIN_TOKEN", "END_TOKEN", "IF_TOKEN",
    "THEN_TOKEN", "WHILE_TOKEN", "DO_TOKEN", "READ_TOKEN", "WRITE_TOKEN",
    "PV_TOKEN", "PT_TOKEN", "PLUS_TOKEN", "MOINS_TOKEN", "MULT_TOKEN", "DIV_TOKEN",
    "VIR_TOKEN", "AFF_TOKEN", "INF_TOKEN", "INFEG_TOKEN", "SUP_TOKEN", "SUPEG_TOKEN",
    "DIFF_TOKEN", "PO_TOKEN", "PF_TOKEN", "FIN_TOKEN", "NUMBER_TOKEN", "REAL_NUMBER_TOKEN",
    "IDENTIFIER_TOKEN", "ERROR_TOKEN", "EOF_TOKEN", "ELSE_TOKEN", "ARRAY_TOKEN",
    "RECORD_TOKEN", "FOR_TOKEN", "CASE_TOKEN", "REPEAT_TOKEN"
};



// Keywords Map
static std::unordered_map<std::string, TokenType> keywords = {
    {"program", PROGRAM_TOKEN}, {"const", CONST_TOKEN}, {"var", VAR_TOKEN},
    {"begin", BEGIN_TOKEN}, {"end", END_TOKEN}, {"if", IF_TOKEN},
    {"then", THEN_TOKEN}, {"while", WHILE_TOKEN}, {"do", DO_TOKEN},
    {"read", READ_TOKEN}, {"write", WRITE_TOKEN}, {"else", ELSE_TOKEN},
    {"array", ARRAY_TOKEN}, {"record", RECORD_TOKEN}, {"for", FOR_TOKEN},
    {"case", CASE_TOKEN}, {"repeat", REPEAT_TOKEN}
};


// Special Symbols Map
static std::unordered_map<std::string, TokenType> specialSymbols = {
    {";", PV_TOKEN}, {".", PT_TOKEN}, {"+", PLUS_TOKEN}, {"-", MOINS_TOKEN},
    {"*", MULT_TOKEN}, {"/", DIV_TOKEN}, {",", VIR_TOKEN}, {":=", AFF_TOKEN},
    {"<", INF_TOKEN}, {"<=", INFEG_TOKEN}, {">", SUP_TOKEN}, {">=", SUPEG_TOKEN},
    {"<>", DIFF_TOKEN}, {"(", PO_TOKEN}, {")", PF_TOKEN}, {"EOF", FIN_TOKEN}
};

// Helper Functions
static char nextChar() {
    char c;
    if (inputFile.get(c)) {
        return c;
    }
    return EOF;
}

Token createToken(TokenType type, const std::string& value) {
    return { type, TOKEN_NAMES[type], value };
}

// Token Reading Functions
static Token readIdentifier(char initialChar) {
    std::string identifier(1, initialChar);
    char ch;

    while ((ch = nextChar()) != EOF && (isalnum(ch) || ch == '_')) {
        identifier += ch;
    }

    if (ch != EOF) {
        inputFile.putback(ch);
    }

    // Regex for identifiers
    std::regex identifierRegex("^[a-zA-Z][a-zA-Z0-9_]*$");
    if (!std::regex_match(identifier, identifierRegex)) {
        return createToken(ERROR_TOKEN, "Invalid identifier: " + identifier);
    }

    // Check if it's a keyword
    if (keywords.find(identifier) != keywords.end()) {
        return createToken(keywords[identifier], identifier);
    }

    // Otherwise, it's a valid identifier
    return createToken(IDENTIFIER_TOKEN, identifier);
}

static Token readNumber(char initialChar) {
    std::string number(1, initialChar);
    char ch;

    while ((ch = nextChar()) != EOF && isdigit(ch)) {
        number += ch;
    }

    // Check if the next character is alphabetic or part of an invalid identifier
    if (isalpha(ch)) {
        number += ch;

        // Continue reading until the invalid identifier ends
        while ((ch = nextChar()) != EOF && (isalnum(ch) || ch == '_')) {
            number += ch;
        }

        if (ch != EOF) {
            inputFile.putback(ch);
        }

        // Return an error token for invalid number
        return createToken(ERROR_TOKEN, "Invalid : " + number);
    }

    // Put back the non-digit character for further processing
    if (ch != EOF) {
        inputFile.putback(ch);
    }

    return createToken(NUMBER_TOKEN, number);
}


// Main Lexical Analysis Function
Token getNextToken() {
    char ch;

    // Loop to ignore whitespaces and comments
    while (true) {
        ch = nextChar();

        // Handle end of file
        if (ch == EOF) {
            return createToken(EOF_TOKEN, "EOF");
        }

        // Skip whitespaces
        if (isspace(ch)) {
            continue;
        }

        // Skip Pascal comments: { ... }
        if (ch == '{') {
            while ((ch = nextChar()) != EOF && ch != '}') {
                // Continue
            }
            if (ch == EOF) {
                return createToken(ERROR_TOKEN, "Unclosed comment starting with '{'");
            }
            continue; // Restart after the comment
        }

        // Skip Pascal comments: (* ... *)
        if (ch == '(') {
            char next = nextChar();
            if (next == '*') {
                while ((ch = nextChar()) != EOF) {
                    if (ch == '*') {
                        char closing = nextChar();
                        if (closing == ')') {
                            break; // Comment ended
                        }
                        inputFile.putback(closing);
                    }
                }
                if (ch == EOF) {
                    return createToken(ERROR_TOKEN, "Unclosed comment starting with '(*'");
                }
                continue; // After comment, restart the loop
            }
            inputFile.putback(next); // Put back non-comment character
        }

        // Non-whitespace and non-comment character found
        break;
    }

    // Handle identifiers, numbers, and symbols
    if (isalpha(ch)) {
        return readIdentifier(ch);
    }

    if (isdigit(ch)) {
        return readNumber(ch);
    }

    // Handle special symbols and multi-character operators
    std::string symbol(1, ch);
    char next = nextChar();

    switch (ch) {
    case ':':
        if (next == '=') {
            symbol += next;
        }
        else if (next != EOF) {
            inputFile.putback(next);
        }
        break;
    case '<':
        if (next == '=' || next == '>') {
            symbol += next;
        }
        else if (next != EOF) {
            inputFile.putback(next);
        }
        break;
    case '>':
        if (next == '=') {
            symbol += next;
        }
        else if (next != EOF) {
            inputFile.putback(next);
        }
        break;
    default:
        if (next != EOF) {
            inputFile.putback(next);
        }
    }

    if (specialSymbols.find(symbol) != specialSymbols.end()) {
        return createToken(specialSymbols[symbol], symbol);
    }

    return createToken(ERROR_TOKEN, symbol);
}


//std::vector<Token> getTokensFromFile(const std::string& filename) {
//    inputFile.open(filename);
//
//    if (!inputFile.is_open()) {
//        std::cout << "Le Fichier '" << filename << "' ne s'ouvre pas ou n'existe pas." << std::endl;
//        return std::vector<Token>();
//    }
//
//    std::cout << "Fichier ouvert: " << filename << std::endl;
//
//    std::vector<Token> tokens;
//    Token token;
//
//    do {
//        token = getNextToken();
//        tokens.push_back(token);
//    } while (token.type != EOF_TOKEN);
//
//    inputFile.close();
//    return tokens;
//}

std::vector<Token> getTokensFromFile(const std::string& filename) {
    inputFile.open(filename);

    if (!inputFile.is_open()) {
        std::cout << "Le Fichier '" << filename << "' ne s'ouvre pas ou n'existe pas." << std::endl;
        return std::vector<Token>();
    }

    std::cout << "Fichier ouvert: " << filename << std::endl;

    std::vector<Token> tokens;
    Token token;

    do {
        token = getNextToken();
        tokens.push_back(token);

        // Log the token details to the terminal
        std::cout << "Token Type: " << TOKEN_NAMES[token.type]
            << ", Value: '" << token.value << "'" << std::endl;

    } while (token.type != EOF_TOKEN);

    inputFile.close();
    return tokens;
}

int main() {
    std::string filePath = "C:\\Users\\Lenovo\\Desktop\\Pascal-Compiler\\src\\test_syntx.pas";
    std::vector<Token> tokens = getTokensFromFile(filePath);

    // Number of tokens for confirmation
    std::cout << "Total tokens found: " << tokens.size() << std::endl;

    return 0;
}
