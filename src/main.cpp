#include "Lexical_Analyzer/lexical.h"
#include "Syntax_Analyzer_(Parser)/syntax.h"
#include <iostream>

int main() {
    std::string filename;
    std::cout << "Fichier Pascal Source: ";
    std::getline(std::cin, filename);

    // Lexical Analysis
    std::cout << "Analyse Lexicale: " << filename << std::endl;
    std::vector<Token> tokens = getTokensFromFile(filename);

    // Print tokens
    std::cout << "\nTokens:\n";
    std::cout << "----------------------------------------\n";
    for (const auto& token : tokens) {
        std::cout << "Token(Code: " << token.type
            << ", Nom: " << token.tokenName
            << ", Valeur: '" << token.value << "')\n";
    }
    std::cout << "----------------------------------------\n";

    // Syntactic Analysis
    if (!tokens.empty()) {
        std::cout << "\nAnalyse Syntaxique:\n";
        Parser parser(tokens);
        parser.parse();
    }
    
    return 0;
}