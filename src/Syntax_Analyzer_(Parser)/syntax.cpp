#include "syntax.h"
#include <iostream>
#include <stdexcept>

Parser::Parser(const std::vector<Token>& t) : tokens(t), current(0) {}

Token Parser::peek() {
    if (current >= tokens.size()) return createToken(EOF_TOKEN, "EOF");
    return tokens[current];
}

Token Parser::advance() {
    if (current >= tokens.size()) return createToken(EOF_TOKEN, "EOF");
    return tokens[current++];
}

bool Parser::check(TokenType type) {
    if (current >= tokens.size()) return false;
    return tokens[current].type == type;
}

void Parser::expect(TokenType type) {
    Token token = advance();
    if (token.type != type) {
        throw std::runtime_error("Expected " + TOKEN_NAMES[type] +
            " but got " + token.tokenName +
            " with value '" + token.value + "'");
    }
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::parse() {
    try {
        parse_program();
        std::cout << "Analyse syntaxique réussie!" << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cout << "Erreur syntaxique: " << e.what() << std::endl;
        std::cout << "À la position du token: " << current << std::endl;
    }
}

void Parser::parse_program() {
    expect(PROGRAM_TOKEN);
    expect(IDENTIFIER_TOKEN);
    expect(PV_TOKEN);
    parse_block();
    expect(PT_TOKEN);
}

void Parser::parse_block() {
    parse_consts();
    parse_vars();
    parse_insts();
}

void Parser::parse_consts() {
    if (match(CONST_TOKEN)) {
        do {
            expect(IDENTIFIER_TOKEN);
            expect(AFF_TOKEN);
            expect(NUMBER_TOKEN);
            expect(PV_TOKEN);
        } while (check(IDENTIFIER_TOKEN));
    }
}

void Parser::parse_vars() {
    if (match(VAR_TOKEN)) {
        do {
            expect(IDENTIFIER_TOKEN);
            while (match(VIR_TOKEN)) {
                expect(IDENTIFIER_TOKEN);
            }
            expect(PV_TOKEN);
        } while (check(IDENTIFIER_TOKEN));
    }
}

void Parser::parse_insts() {
    expect(BEGIN_TOKEN);
    do {
        parse_inst();
        expect(PV_TOKEN);
    } while (!check(END_TOKEN));
    expect(END_TOKEN);
}

void Parser::parse_inst() {
    if (check(IDENTIFIER_TOKEN)) {
        parse_affectation();
    }
    else if (match(IF_TOKEN)) {
        parse_if();
    }
    else if (match(WHILE_TOKEN)) {
        parse_while();
    }
    else if (match(READ_TOKEN)) {
        parse_read();
    }
    else if (match(WRITE_TOKEN)) {
        parse_write();
    }
    else if (match(BEGIN_TOKEN)) {
        parse_insts();
    }
    else {
        throw std::runtime_error("Instruction invalide");
    }
}

void Parser::parse_affectation() {
    expect(IDENTIFIER_TOKEN);
    expect(AFF_TOKEN);
    parse_expr();
}

void Parser::parse_if() {
    parse_condition();
    expect(THEN_TOKEN);
    parse_inst();
}

void Parser::parse_while() {
    parse_condition();
    expect(DO_TOKEN);
    parse_inst();
}

void Parser::parse_read() {
    expect(PO_TOKEN);
    expect(IDENTIFIER_TOKEN);
    while (match(VIR_TOKEN)) {
        expect(IDENTIFIER_TOKEN);
    }
    expect(PF_TOKEN);
}

void Parser::parse_write() {
    expect(PO_TOKEN);
    parse_expr();
    while (match(VIR_TOKEN)) {
        parse_expr();
    }
    expect(PF_TOKEN);
}

void Parser::parse_condition() {
    parse_expr();
    if (!(match(INF_TOKEN) || match(INFEG_TOKEN) ||
        match(SUP_TOKEN) || match(SUPEG_TOKEN) ||
        match(DIFF_TOKEN) || match(AFF_TOKEN))) {
        throw std::runtime_error("Opérateur de comparaison attendu");
    }
    parse_expr();
}

void Parser::parse_expr() {
    parse_term();
    while (match(PLUS_TOKEN) || match(MOINS_TOKEN)) {
        parse_term();
    }
}

void Parser::parse_term() {
    parse_fact();
    while (match(MULT_TOKEN) || match(DIV_TOKEN)) {
        parse_fact();
    }
}

void Parser::parse_fact() {
    if (match(IDENTIFIER_TOKEN) || match(NUMBER_TOKEN)) {
        return;
    }
    else if (match(PO_TOKEN)) {
        parse_expr();
        expect(PF_TOKEN);
    }
    else {
        throw std::runtime_error("Facteur invalide");
    }
}