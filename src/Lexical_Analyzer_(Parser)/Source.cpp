#include <iostream>
#include <string>
// for the Keyword <--> Token Association
#include <unordered_map>
// Character checking methods
#include <cctype>
// Vector to return all Tokens
#include <vector>
// File Stream and Initialization
#include <fstream>
std::ifstream inputFile;

// Enum for the TokenTypes
enum TokenType {
    PROGRAM_TOKEN, CONST_TOKEN, VAR_TOKEN, BEGIN_TOKEN, END_TOKEN, IF_TOKEN,
    THEN_TOKEN, WHILE_TOKEN, DO_TOKEN, READ_TOKEN, WRITE_TOKEN,
    PV_TOKEN, PT_TOKEN, PLUS_TOKEN, MOINS_TOKEN, MULT_TOKEN, DIV_TOKEN,
    VIR_TOKEN, AFF_TOKEN, INF_TOKEN, INFEG_TOKEN, SUP_TOKEN, SUPEG_TOKEN,
    DIFF_TOKEN, PO_TOKEN, PF_TOKEN, FIN_TOKEN, NUMBER_TOKEN, IDENTIFIER_TOKEN,
    ERROR_TOKEN, EOF_TOKEN
};

const std::string TOKEN_NAMES[] = {
    "PROGRAM_TOKEN", "CONST_TOKEN", "VAR_TOKEN", "BEGIN_TOKEN", "END_TOKEN", "IF_TOKEN",
    "THEN_TOKEN", "WHILE_TOKEN", "DO_TOKEN", "READ_TOKEN", "WRITE_TOKEN",
    "PV_TOKEN", "PT_TOKEN", "PLUS_TOKEN", "MOINS_TOKEN", "MULT_TOKEN", "DIV_TOKEN",
    "VIR_TOKEN", "AFF_TOKEN", "INF_TOKEN", "INFEG_TOKEN", "SUP_TOKEN", "SUPEG_TOKEN",
    "DIFF_TOKEN", "PO_TOKEN", "PF_TOKEN", "FIN_TOKEN", "NUMBER_TOKEN", "IDENTIFIER_TOKEN",
    "ERROR_TOKEN", "EOF_TOKEN"
};

// Token + Value
struct Token {
    TokenType type;
    std::string tokenName;
    std::string value;
};

// Keywords <-> Token
std::unordered_map<std::string, TokenType> keywords = {
    {"program", PROGRAM_TOKEN}, {"const", CONST_TOKEN}, {"var", VAR_TOKEN},
    {"begin", BEGIN_TOKEN}, {"end", END_TOKEN}, {"if", IF_TOKEN},
    {"then", THEN_TOKEN}, {"while", WHILE_TOKEN}, {"do", DO_TOKEN},
    {"read", READ_TOKEN}, {"write", WRITE_TOKEN}
};

// Operators + Separators
std::unordered_map<std::string, TokenType> specialSymbols = {
    {";", PV_TOKEN}, {".", PT_TOKEN}, {"+", PLUS_TOKEN}, {"-", MOINS_TOKEN},
    {"*", MULT_TOKEN}, {"/", DIV_TOKEN}, {",", VIR_TOKEN}, {":=", AFF_TOKEN},
    {"<", INF_TOKEN}, {"<=", INFEG_TOKEN}, {">", SUP_TOKEN}, {">=", SUPEG_TOKEN},
    {"<>", DIFF_TOKEN}, {"(", PO_TOKEN}, {")", PF_TOKEN}, {"EOF", FIN_TOKEN}
};

// Next Char from Input
char nextChar() {
    char c;
    if (inputFile.get(c)) {
        return c;
    }
    return EOF;
}

// Helper function to create token with all info
Token createToken(TokenType type, const std::string& value) {
    return { type, TOKEN_NAMES[type], value };
}

// 1 - Var IDs / Keywords Identifier
Token readIdentifier(char initialChar) {
    std::string identifier(1, initialChar);
    char ch;

    // Alphanumeric Next => Continue
    while ((ch = nextChar()) != EOF && isalnum(ch)) {
        identifier += ch;
    }

    // Last character put back
    if (ch != EOF) {
        inputFile.putback(ch);
    }

    // Check if the identifier is a reserved keyword
    if (keywords.find(identifier) != keywords.end()) {
        TokenType type = keywords[identifier];
        return createToken(type, identifier);
    }
    else {
        // Regular VAR ID then
        return createToken(IDENTIFIER_TOKEN, identifier);
    }
}

// 2 - Numbers Identifier
Token readNumber(char initialChar) {
    std::string number(1, initialChar);
    char ch;

    // Digit Next => Continue
    while ((ch = nextChar()) != EOF && isdigit(ch)) {
        number += ch;
    }

    // Put back the last character that broke the loop (not part of the number)
    if (ch != EOF) {
        inputFile.putback(ch);
    }

    return createToken(NUMBER_TOKEN, number);
}

///////////////////// Main Lexical Analyzer Function /////////////////////
Token getNextToken() {
    char ch;

    // Skip any whitespace characters
    do {
        ch = nextChar();
        if (ch == EOF) {
            return createToken(EOF_TOKEN, "EOF");
        }
    } while (isspace(ch));

    // START-I - Alphetic Character 1st -> readIdentifier
    if (isalpha(ch)) {
        return readIdentifier(ch);
    }

    // START-II - Digit -> readNumber
    if (isdigit(ch)) {
        return readNumber(ch);
    }

    // Operators / SpclChar
    std::string symbol(1, ch);
    char next = nextChar();

    // Switch statment for the cases
    switch (ch) {
    case ':':
        // ":="
        if (next == '=') {
            symbol += next;
        }
        else if (next != EOF) {
            inputFile.putback(next);
        }
        break;
    case '<':
        // "<=" or "<>"
        if (next == '=' || next == '>') {
            symbol += next;
        }
        else if (next != EOF) {
            inputFile.putback(next);
        }
        break;
    case '>':
        // ">="
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

    // Spcl Symbl check
    if (specialSymbols.find(symbol) != specialSymbols.end()) {
        TokenType type = specialSymbols[symbol];
        return createToken(type, symbol);
    }

    // Symbol Unrecognized -> Error Token
    return createToken(ERROR_TOKEN, std::string(1, ch));
}

std::vector<Token> getTokensFromFile(const std::string& filename) {
    inputFile.open(filename);
    
    // Check if file exists and can be opened
    if (!inputFile.is_open()) {
        std::cout << "Le Fichier '" << filename << "' ne s'ouvre pas ou n'existe pas." << std::endl;
        return std::vector<Token>();
    }
    
    std::cout << "Fichier ouvert: " << filename << std::endl;
    
    std::vector<Token> tokens;
    Token token;

    // Read until EOF
    do {
        token = getNextToken();
        tokens.push_back(token);
    } while (token.type != EOF_TOKEN);

    inputFile.close();
    return tokens;  
}

int main() {
    std::string filename;
    std::cout << "Fichier Pascal Source: ";
    std::getline(std::cin, filename);
    std::cout << "Analyse Lexicale: " << filename << std::endl;
    std::vector<Token> tokens = getTokensFromFile(filename);
    std::cout << "\nTokens:\n";
    std::cout << "----------------------------------------\n";
    for (const auto& token : tokens) {
        std::cout << "Token(Code: " << token.type 
                 << ", Nom: " << token.tokenName 
                 << ", Valeur: '" << token.value << "')\n";
    }
    std::cout << "----------------------------------------\n";

    return 0;
}
