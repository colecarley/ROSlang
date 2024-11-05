%{
    #include "ast.hpp"
    #include <iostream>
    #include <string>
    #include <memory>

    void yyerror(Program** program, const char* s);
    void ros_parse(Program** program, const char* code);

    extern int yylex();
    extern void scanner_destroy();
    extern void scanner_init(const char* code);
%}

%parse-param { Program** program }

%union {
    int intval;
    float floatval;
    bool boolval;
    char* strval;
    char* id;
    ExprList* expr_list;
    Expr* expr;
    StmtList* stmt_list;
    Stmt* stmt;
    IdentifierType* identifier_type;
    IdentifierTypeList* identifier_type_list;
    Type* type;
    BlockStmt* block_stmt;
    Program* program;
    TreeNode* tree_node;
    TreeNodeList* tree_node_list;
    InputList* input_list;
    Input* input;
    InputDefault* input_default;
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
%token NOT
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

%type <tree_node> tree and_node or_node then_node behavior_node pseudo_node at_if_stmt at_if_else_stmt at_for_stmt
%type <tree_node_list> children node_list
%type <expr_list> arg_list
%type <expr> expr or and equality comparison term factor exponent unary call primary array assignment ternary
%type <stmt_list> stmt_list 
%type <identifier_type_list> param_list
%type <type> type type_identifier
%type <stmt> stmt var_decl fn_decl return_stmt if_stmt while_stmt for_in_stmt break_stmt continue_stmt 
%type <block_stmt> block
%type <input_list> input_list 
%type <input> input

%%

program:
    input_list stmt_list tree  { *program = new Program(std::move($1->inputs), std::move($2->stmts), $3); }
    ;

input_list:
    input_list input { $1->add($2); $$ = $1; }
    | input { $$ = new InputList({$1}); }
    | 
    ;

input:
    INPUT IDENTIFIER COLON type EQUAL expr NEW_LINE { $$ = new InputDefault($2, $4, $6); }
    | INPUT IDENTIFIER COLON type NEW_LINE { $$ = new Input($2, $4); }
    ;

tree:
    and_node 
    | or_node
    | then_node
    | behavior_node NEW_LINE 
    | pseudo_node
    ;

and_node:
    AND COLON NEW_LINE children {  $$ = new AndNode(std::move($4->children)); }
    ;

or_node:
    OR COLON NEW_LINE children { $$ = new OrNode(std::move($4->children)); }
    ;

then_node:
    THEN COLON NEW_LINE children { $$ = new ThenNode(std::move($4->children)); }
    ;

behavior_node:
    IDENTIFIER LPAREN arg_list RPAREN  {  $$ = new BehaviorNode($1, std::move($3->exprs)); }

pseudo_node:
    at_if_stmt
    | at_if_else_stmt
    | at_for_stmt
    ;

at_if_stmt:
    AT_IF expr COLON NEW_LINE children { $$ = new AtIfNode($2, std::move($5->children)); }
    ;

at_if_else_stmt:
    AT_IF expr COLON NEW_LINE children AT_ELSE COLON NEW_LINE children { $$ = new AtIfElseNode($2, std::move($5->children), std::move($9->children)); }
    ;

at_for_stmt:
    AT_FOR IDENTIFIER IN expr COLON NEW_LINE children { $$ = new AtForNode($2, $4, std::move($7->children)); }
    ;

children:
    INDENT node_list DEDENT {  $$ = $2; }
    | INDENT node_list OUTDENT {  $$ = $2; }
    ;

node_list:
    tree {  $$ = new TreeNodeList({$1}); }
    | tree node_list { $2->add($1); $$ = $2; }
    |
    ; 

stmt_list:
    stmt { $$ = new StmtList({$1}); }
    | stmt stmt_list { $2->add($1); $$ = $2; }
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
    ;

for_in_stmt:
    FOR IDENTIFIER IN expr COLON NEW_LINE block  { $$ = new ForInStmt($2, $4, $7); }
    ;

if_stmt:
    IF expr COLON NEW_LINE block  { $$ = new IfStmt($2, $5); }
    | IF expr COLON NEW_LINE block ELSE COLON NEW_LINE block  { $$ = new IfElseStmt($2, $5, $9); }
    ;

while_stmt:
    WHILE expr COLON NEW_LINE block { $$ = new WhileStmt($2, $5); }
    ;

break_stmt:
    BREAK { $$ = new BreakStmt(); }
    ;

continue_stmt:
    CONTINUE { $$ = new ContinueStmt(); }
    ;

fn_decl:
    FUN IDENTIFIER LPAREN param_list RPAREN TYPE_ARROW type COLON NEW_LINE block { $$ = new FnDecl($2, std::move($4->pairs), $7, $10); }
    ;

param_list:
    IDENTIFIER COLON type { $$ = new IdentifierTypeList({new IdentifierType($1, $3)}); }
    | IDENTIFIER COMMA type param_list { $4->add(new IdentifierType($1, $3)); $$ = $4; }
    | { $$ = new IdentifierTypeList(); }
    ;

return_stmt:
    RETURN expr { $$ = new ReturnStmt($2); }
    | RETURN { $$ = new ReturnStmt(); }
    ;

type: 
    type_identifier 
    | type_identifier LBRACKET RBRACKET { $$ = new ArrayType($1); }
    ;

type_identifier:
    INT { $$ = new PrimitiveType("int"); }
    | FLOAT { $$ = new PrimitiveType("float"); }
    | STRING { $$ = new PrimitiveType("string"); }
    | BOOL { $$ = new PrimitiveType("bool"); }
    | VOID { $$ = new PrimitiveType("void"); }
    | NONE { $$ = new PrimitiveType("none"); }
    ;

var_decl:
    LET IDENTIFIER COLON type EQUAL expr { $$ = new VarDecl($2, $4, $6); }
    ;

block:
    INDENT stmt_list DEDENT { $$ = new BlockStmt(std::move($2->stmts)); }
    | INDENT stmt_list OUTDENT  { $$ = new BlockStmt(std::move($2->stmts)); }
    ; 

expr:
    assignment
    ;

assignment:
    IDENTIFIER EQUAL expr { $$ = new AssignExpr($1, $3); }
    | ternary
    ;

ternary:
    expr QUESTION_MARK expr COLON expr { $$ = new TernaryExpr($1, $3, $5); }
    | or
    ;

or: 
    or OR and { $$ = new BinaryExpr($1, $3, "or"); }
    | and
    ;

and: 
    and AND equality { $$ = new BinaryExpr($1, $3, "and"); }
    | equality
    ;

equality:
    equality EQUAL_EQUAL comparison { $$ = new BinaryExpr($1, $3, "=="); }
    | equality BANG_EQUAL comparison { $$ = new BinaryExpr($1, $3, "!="); }
    | comparison
    ;

comparison:
    comparison GREATER comparison { $$ = new BinaryExpr($1, $3, ">"); }
    | comparison LESS comparison { $$ = new BinaryExpr($1, $3, "<"); }
    | comparison GREATER_EQUAL comparison { $$ = new BinaryExpr($1, $3, ">="); }
    | comparison LESS_EQUAL comparison { $$ = new BinaryExpr($1, $3, "<="); }
    | term
    ;

term:
    factor PLUS term { $$ = new BinaryExpr($1, $3, "+"); }
    | factor MINUS term { $$ = new BinaryExpr($1, $3, "-"); }
    | factor
    ;

factor:
    exponent STAR factor { $$ = new BinaryExpr($1, $3, "*"); }
    | exponent SLASH factor { $$ = new BinaryExpr($1, $3, "/"); }
    | exponent MOD factor { $$ = new BinaryExpr($1, $3, "%"); }
    | exponent
    ;

exponent:
    unary STAR_STAR exponent { $$ = new BinaryExpr($1, $3, "**"); }
    | unary
    ;

unary:
    MINUS unary { $$ = new UnaryExpr($2, "-"); }
    | NOT unary { $$ = new UnaryExpr($2, "!"); }
    | call
    ;

call:
    IDENTIFIER LPAREN arg_list RPAREN { $$ = new CallExpr($1, std::move($3->exprs)); }
    | primary
    ;

arg_list:
    expr { $$ = new ExprList({$1}); }
    | expr COMMA arg_list  { $3->add($1); $$ = $3; }
    | { $$ = new ExprList(); }
    ;

primary:
    INT_LITERAL { $$ = new IntLiteral($1); }
    | FLOAT_LITERAL  { $$ = new FloatLiteral($1); }
    | STRING_LITERAL  { $$ = new StringLiteral($1); }
    | BOOL_LITERAL { $$ = new BoolLiteral($1); }
    | IDENTIFIER  { $$ = new IdentifierExpr($1); }
    | array
    ;

array:
    LBRACKET arg_list RBRACKET { $$ = new ArrayLiteral(std::move($2->exprs)); }
    ;

%%

void yyerror(Program** program, const char *s) {
    std::cerr << "Error: " << s << std::endl;
}

void ros_parse(Program** program, const char* code) {
    scanner_init(code);
    yyparse(program);
    scanner_destroy();
}
