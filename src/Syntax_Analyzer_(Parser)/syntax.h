#ifndef SYNTAX_H
#define SYNTAX_H

#include "../Lexical_Analyzer/lexical.h"
#include <vector>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    void parse();

private:
    std::vector<Token> tokens;
    size_t current;

    // Helper methods
    Token peek();
    Token advance();
    bool check(TokenType type);
    void expect(TokenType type);
    bool match(TokenType type);

    // Parsing methods
    void parse_program();
    void parse_block();
    void parse_consts();
    void parse_vars();
    void parse_insts();
    void parse_inst();
    void parse_affectation();
    void parse_if();
    void parse_while();
    void parse_read();
    void parse_write();
    void parse_condition();
    void parse_expr();
    void parse_term();
    void parse_fact();
};

#endif