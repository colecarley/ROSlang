#pragma once

#include <string>
#include <vector>
#include "visitor.hpp"

struct Expr;
struct Stmt;
struct IfStmt;
struct WhileStmt;
struct ForInStmt;
struct ReturnStmt;
struct BreakStmt;
struct ContinueStmt;
struct FnDecl;
struct VarDecl;
struct ExprStmt;
struct BlockStmt;
struct AssignExpr;
struct TernaryExpr;
struct BinaryExpr;
struct UnaryExpr;
struct CallExpr;
struct LiteralExpr;
struct IdentifierExpr;

struct ASTNode
{
    virtual void accept(Visitor *) = 0;
};

struct Expr : ASTNode
{
    virtual void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct Stmt : ASTNode
{
    virtual void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct IfStmt : Stmt
{
    Expr *condition;
    BlockStmt *then_block;

    IfStmt(Expr *condition, Stmt *then_block) : condition(condition), then_block((BlockStmt *)then_block) {}
};

struct IfElseStmt : Stmt
{
    Expr *condition;
    BlockStmt *then_block;
    BlockStmt *else_block;

    IfElseStmt(Expr *condition, Stmt *then_block, Stmt *else_block) : condition(condition), then_block((BlockStmt *)then_block), else_block((BlockStmt *)else_block) {}
};

struct WhileStmt : Stmt
{
    Expr *condition;
    BlockStmt *block;

    WhileStmt(Expr *condition, Stmt *block) : condition(condition), block((BlockStmt *)block) {}
};

struct ForInStmt : Stmt
{
    std::string identifier;
    Expr *iterable;
    BlockStmt *block;

    ForInStmt(std::string identifier, Expr *iterable, Stmt *block) : identifier(identifier), iterable(iterable), block((BlockStmt *)block) {}
};

struct ReturnStmt : Stmt
{
    Expr *expr;

    ReturnStmt(Expr *expr) : expr(expr) {}
};

struct BreakStmt : Stmt
{
    BreakStmt() {}
};

struct ContinueStmt : Stmt
{
    ContinueStmt() {}
};

struct Type
{
};

struct PrimitiveType : Type
{
    std::string primitive;

    PrimitiveType(std::string primitive) : primitive(primitive) {}
};

struct ArrayType : Type
{
    Type type;

    ArrayType(Type type) : type(type) {}
};

struct IdentifierType
{
    std::string identifier;
    Type type;

    IdentifierType(std::string identifier, Type type) : identifier(identifier), type(type) {}
};

struct FnDecl : Stmt
{
    std::string identifier;
    std::vector<IdentifierType> params;
    Type return_type;
    BlockStmt *block;

    FnDecl(std::string identifier, std::vector<IdentifierType> params, Type return_type, Stmt *block) : identifier(identifier), params(params), return_type(return_type), block((BlockStmt *)block) {}
};

struct VarDecl : Stmt
{
    std::string identifier;
    Type type;
    Expr *value;

    VarDecl(std::string identifier, Type type) : identifier(identifier), type(type) {}
};

struct ExprStmt : Stmt
{
    Expr *expr;

    ExprStmt(Expr *expr) : expr(expr) {}
};

struct BlockStmt : Stmt
{
    std::vector<Stmt *> stmts;

    BlockStmt(std::vector<Stmt *> stmts) : stmts(stmts) {}
};

struct AssignExpr : Expr
{
    std::string identifier;
    Expr *value;

    AssignExpr(std::string identifier, Expr *value) : identifier(identifier), value(value) {}
};

struct TernaryExpr : Expr
{
    Expr *condition;
    Expr *then_expr;
    Expr *else_expr;

    TernaryExpr(Expr *condition, Expr *then_expr, Expr *else_expr) : condition(condition), then_expr(then_expr), else_expr(else_expr) {}
};

struct BinaryExpr : Expr
{
    Expr *left;
    Expr *right;
    std::string op;

    BinaryExpr(Expr *left, Expr *right, std::string op) : left(left), right(right), op(op) {}
};

struct UnaryExpr : Expr
{
    Expr *expr;
    std::string op;

    UnaryExpr(Expr *expr, std::string op) : expr(expr), op(op) {}
};

struct CallExpr : Expr
{
    std::string identifier;
    std::vector<Expr *> args;

    CallExpr(std::string identifier, std::vector<Expr *> args) : identifier(identifier), args(args) {}
};

struct ArrayExpr : Expr
{
    std::vector<Expr *> elements;

    ArrayExpr(std::vector<Expr *> elements) : elements(elements) {}
};

struct LiteralExpr : Expr
{
    std::string literal;

    LiteralExpr(std::string literal) : literal(literal) {}
};

struct IdentifierExpr : Expr
{
    std::string identifier;

    IdentifierExpr(std::string identifier) : identifier(identifier) {}
};
