# Pascal-Compiler

## Lexical Analysis: 
The Lexical Analyzer (Lexer) tokenizes the Pascal source code, scanning for lexical units such as keywords, operators, identifiers, literals, and symbols. This phase outputs a sequence of tokens used for further parsing.

## Syntax Analysis: 
The Parser performs Context-Free Grammar parsing, using either recursive descent or a table-driven parsing approach to construct an Abstract Syntax Tree (AST). The AST represents the hierarchical structure of the source code according to Pascal’s syntax rules.

## Semantic Analysis: 
The compiler performs semantic checks by building a Symbol Table to manage variable declarations, types, and scopes. The Type Checker ensures that operations adhere to the Pascal type system, and the Scope Resolution ensures variables are correctly defined and used within their valid scopes.

## Intermediate Code Generation: 
After syntax and semantic analysis, the compiler generates an Intermediate Representation (IR) of the code. This IR serves as a platform-independent abstraction, facilitating optimizations and further code generation.

## Optimization: 
The Optimizer performs transformations on the IR to improve performance and reduce resource usage, including constant folding, dead code elimination, and loop unrolling. These optimizations enhance the efficiency of the generated code.
