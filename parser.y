%{
    #include <iostream>
    #include <string>
    #include "ast.hpp"

    void yyerror(ASTNode** root, const char* s);
    void ros_parse(ASTNode** root, const char* code);

    extern int yylex();
    extern void scanner_destroy();
    extern void scanner_init(const char* code);
%}

%parse-param { ASTNode** root }

%union {
    int intval;
    float floatval;
    bool boolval;
    char* strval;
    char* id;
}

%token <intval> INT_LITERAL
%token <floatval> FLOAT_LITERAL
%token <boolval> BOOL_LITERAL
%token <strval> STRING_LITERAL
%token <id> IDENTIFIER

%token INPUT
%token LET
%token FUN
%token RETURN
%token IF
%token ELSE
%token FOR
%token IN
%token WHILE
%token BREAK
%token CONTINUE
%token AT_IF
%token AT_ELSE
%token AT_FOR

%token AND
%token THEN
%token OR

%token STAR
%token SLASH
%token PLUS
%token MINUS
%token RBRACKET
%token LBRACKET
%token LPAREN
%token RPAREN
%token COLON
%token EQUAL
%token BANG
%token COMMA
%token GREATER
%token LESS
%token QUESTION_MARK
%token MOD
%token INDENT
%token DEDENT
%token OUTDENT
%token NEW_LINE

%token STAR_STAR
%token TYPE_ARROW
%token EQUAL_EQUAL
%token BANG_EQUAL
%token GREATER_EQUAL
%token LESS_EQUAL

%token INT;
%token FLOAT;
%token STRING;
%token BOOL;
%token VOID;
%token NONE;

%%

program:
    input_list stmt_list tree
    ;

input_list:
    input_list input
    | input
    | 
    ;

input:
    INPUT IDENTIFIER COLON type EQUAL expr NEW_LINE
    | INPUT IDENTIFIER COLON type NEW_LINE
    ;

tree:
    and_node
    | or_node
    | then_node
    | behavior_node
    | pseudo_node
    ;

and_node:
    AND COLON NEW_LINE children
    ;

or_node:
    OR COLON NEW_LINE children
    ;

then_node:
    THEN COLON NEW_LINE children
    ;

behavior_node:
    IDENTIFIER LPAREN arg_list RPAREN NEW_LINE

pseudo_node:
    at_if_stmt
    | at_if_else_stmt
    | at_for_stmt
    ;

at_if_stmt:
    AT_IF expr COLON NEW_LINE children
    ;

at_if_else_stmt:
    AT_IF expr COLON NEW_LINE children AT_ELSE COLON NEW_LINE children
    ;

at_for_stmt:
    AT_FOR IDENTIFIER IN IDENTIFIER COLON NEW_LINE children
    ;

children:
    INDENT node_list DEDENT
    | INDENT node_list OUTDENT
    ;

node_list:
    tree
    | tree node_list
    ; 

stmt_list:
    stmt
    | stmt stmt_list
    ;

stmt:
    block 
    | expr NEW_LINE 
    | var_decl NEW_LINE 
    | fn_decl 
    | return_stmt NEW_LINE 
    | break_stmt NEW_LINE 
    | continue_stmt NEW_LINE 
    | while_stmt 
    | if_stmt 
    | for_in_stmt 
    | NEW_LINE
    ;

for_in_stmt:
    FOR IDENTIFIER IN expr COLON NEW_LINE block 
    ;

if_stmt:
    IF expr COLON NEW_LINE block 
    | IF expr COLON NEW_LINE block ELSE COLON NEW_LINE block 
    ;

while_stmt:
    WHILE expr COLON NEW_LINE block 
    ;

break_stmt:
    BREAK
    ;

continue_stmt:
    CONTINUE
    ;

fn_decl:
    FUN IDENTIFIER LPAREN param_list RPAREN TYPE_ARROW type COLON NEW_LINE block 
    ;

param_list:
    IDENTIFIER COLON type 
    | IDENTIFIER COMMA type param_list 
    |
    ;

return_stmt:
    RETURN expr
    | RETURN
    ;

type: 
    type_identifier 
    | type_identifier LBRACKET RBRACKET 
    ;

type_identifier:
    INT
    | FLOAT
    | STRING
    | BOOL
    | VOID
    | NONE
    ;

var_decl:
    LET IDENTIFIER COLON type EQUAL expr
    ;

block:
    INDENT stmt_list DEDENT 
    | INDENT stmt_list OUTDENT 
    ; 

expr:
    assignment
    ;

assignment:
    IDENTIFIER EQUAL expr
    | ternary
    ;

ternary:
    expr QUESTION_MARK expr COLON expr 
    | or
    ;

or: 
    or OR and
    | and
    ;

and: 
    and AND equality
    | equality
    ;

equality:
    equality EQUAL_EQUAL comparison
    | equality BANG_EQUAL comparison
    | comparison
    ;

comparison:
    comparison GREATER comparison
    | comparison LESS comparison
    | comparison GREATER_EQUAL comparison
    | comparison LESS_EQUAL comparison
    | term
    ;

term:
    factor PLUS term
    | factor MINUS term
    | factor
    ;

factor:
    exponent STAR factor
    | exponent SLASH factor
    | exponent MOD factor
    | exponent
    ;

exponent:
    unary STAR_STAR exponent
    | unary
    ;

unary:
    MINUS unary
    | BANG unary 
    | call
    ;

call:
    primary LPAREN arg_list RPAREN
    | primary
    ;

arg_list:
    expr
    | expr COMMA arg_list
    |
    ;

primary:
    INT_LITERAL 
    | FLOAT_LITERAL 
    | STRING_LITERAL 
    | BOOL_LITERAL 
    | IDENTIFIER 
    | array
    ;

array:
    LBRACKET arg_list RBRACKET
    ;


%%

void yyerror(ASTNode** root, const char *s) {
    std::cerr << "Error: " << s << std::endl;
}

void ros_parse(ASTNode** root, const char* code) {
    scanner_init(code);
    yyparse(root);
    scanner_destroy();
}
