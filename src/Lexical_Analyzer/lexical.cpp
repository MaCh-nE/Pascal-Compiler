#include "lexical.h"
#include <iostream>
#include <fstream>
#include <cctype>

// Static file stream
static std::ifstream inputFile;

// Token Names Array Definition
const std::string TOKEN_NAMES[] = {
    "PROGRAM_TOKEN", "CONST_TOKEN", "VAR_TOKEN", "BEGIN_TOKEN", "END_TOKEN", "IF_TOKEN",
    "THEN_TOKEN", "WHILE_TOKEN", "DO_TOKEN", "READ_TOKEN", "WRITE_TOKEN",
    "PV_TOKEN", "PT_TOKEN", "PLUS_TOKEN", "MOINS_TOKEN", "MULT_TOKEN", "DIV_TOKEN",
    "VIR_TOKEN", "AFF_TOKEN", "INF_TOKEN", "INFEG_TOKEN", "SUP_TOKEN", "SUPEG_TOKEN",
    "DIFF_TOKEN", "PO_TOKEN", "PF_TOKEN", "FIN_TOKEN", "NUMBER_TOKEN", "IDENTIFIER_TOKEN",
    "ERROR_TOKEN", "EOF_TOKEN"
};

// Keywords Map
static std::unordered_map<std::string, TokenType> keywords = {
    {"program", PROGRAM_TOKEN}, {"const", CONST_TOKEN}, {"var", VAR_TOKEN},
    {"begin", BEGIN_TOKEN}, {"end", END_TOKEN}, {"if", IF_TOKEN},
    {"then", THEN_TOKEN}, {"while", WHILE_TOKEN}, {"do", DO_TOKEN},
    {"read", READ_TOKEN}, {"write", WRITE_TOKEN}
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

    while ((ch = nextChar()) != EOF && isalnum(ch)) {
        identifier += ch;
    }

    if (ch != EOF) {
        inputFile.putback(ch);
    }

    if (keywords.find(identifier) != keywords.end()) {
        return createToken(keywords[identifier], identifier);
    }
    return createToken(IDENTIFIER_TOKEN, identifier);
}

static Token readNumber(char initialChar) {
    std::string number(1, initialChar);
    char ch;

    while ((ch = nextChar()) != EOF && isdigit(ch)) {
        number += ch;
    }

    if (ch != EOF) {
        inputFile.putback(ch);
    }

    return createToken(NUMBER_TOKEN, number);
}

// Main Lexical Analysis Function
Token getNextToken() {
    char ch;

    do {
        ch = nextChar();
        if (ch == EOF) {
            return createToken(EOF_TOKEN, "EOF");
        }
    } while (isspace(ch));

    if (isalpha(ch)) {
        return readIdentifier(ch);
    }

    if (isdigit(ch)) {
        return readNumber(ch);
    }

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
    } while (token.type != EOF_TOKEN);

    inputFile.close();
    return tokens;
}
