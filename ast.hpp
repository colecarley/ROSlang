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

    IfStmt(Expr *condition, Stmt *then_block);
};

struct IfElseStmt : Stmt
{
    Expr *condition;
    BlockStmt *then_block;
    BlockStmt *else_block;

    IfElseStmt(Expr *condition, Stmt *then_block, Stmt *else_block);
};

struct WhileStmt : Stmt
{
    Expr *condition;
    BlockStmt *block;

    WhileStmt(Expr *condition, Stmt *block);
};

struct ForInStmt : Stmt
{
    std::string identifier;
    Expr *iterable;
    BlockStmt *block;

    ForInStmt(std::string identifier, Expr *iterable, Stmt *block);
};

struct ReturnStmt : Stmt
{
    Expr *expr;

    ReturnStmt(Expr *expr);
};

struct BreakStmt : Stmt
{
    BreakStmt();
};

struct ContinueStmt : Stmt
{
    ContinueStmt();
};

struct Type
{
};

struct PrimitiveType : Type
{
    std::string primitive;
};

struct ArrayType : Type
{
    Type type;
};

struct IdentifierType
{
    std::string identifier;
    Type type;
};

struct FnDecl : Stmt
{
    std::string identifier;
    std::vector<IdentifierType> params;
    Type return_type;
    BlockStmt *block;

    FnDecl(std::string identifier, std::vector<IdentifierType> params, Type return_type, Stmt *block);
};

struct VarDecl : Stmt
{
    std::string identifier;
    Type type;
    Expr *value;

    VarDecl(std::string identifier, Type type);
};

struct ExprStmt : Stmt
{
    Expr *expr;

    ExprStmt(Expr *expr);
};

struct BlockStmt : Stmt
{
    std::vector<Stmt *> stmts;

    BlockStmt(std::vector<Stmt *> stmts);
};

struct AssignExpr : Expr
{
    std::string identifier;
    Expr *value;

    AssignExpr(std::string identifier, Expr *value);
};

struct TernaryExpr : Expr
{
    Expr *condition;
    Expr *then_expr;
    Expr *else_expr;

    TernaryExpr(Expr *condition, Expr *then_expr, Expr *else_expr);
};

struct BinaryExpr : Expr
{
    Expr *left;
    Expr *right;
    std::string op;

    BinaryExpr(Expr *left, Expr *right, std::string op);
};

struct UnaryExpr : Expr
{
    Expr *expr;
    std::string op;

    UnaryExpr(Expr *expr, std::string op);
};

struct CallExpr : Expr
{
    std::string identifier;
    std::vector<Expr *> args;

    CallExpr(std::string identifier, std::vector<Expr *> args);
};

struct ArrayExpr : Expr
{
    std::vector<Expr *> elements;

    ArrayExpr(std::vector<Expr *> elements);
};

struct LiteralExpr : Expr
{
    std::string literal;

    LiteralExpr(std::string literal);
};

struct IdentifierExpr : Expr
{
    std::string identifier;

    IdentifierExpr(std::string identifier);
};
