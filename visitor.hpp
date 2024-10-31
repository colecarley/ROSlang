#pragma once

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

struct Visitor
{
    virtual void visit(Expr *) = 0;
    virtual void visit(Stmt *) = 0;
    virtual void visit(IfStmt *) = 0;
    virtual void visit(WhileStmt *) = 0;
    virtual void visit(ForInStmt *) = 0;
    virtual void visit(ReturnStmt *) = 0;
    virtual void visit(BreakStmt *) = 0;
    virtual void visit(ContinueStmt *) = 0;
    virtual void visit(FnDecl *) = 0;
    virtual void visit(VarDecl *) = 0;
    virtual void visit(ExprStmt *) = 0;
    virtual void visit(BlockStmt *) = 0;
    virtual void visit(AssignExpr *) = 0;
    virtual void visit(TernaryExpr *) = 0;
    virtual void visit(BinaryExpr *) = 0;
    virtual void visit(UnaryExpr *) = 0;
    virtual void visit(CallExpr *) = 0;
    virtual void visit(LiteralExpr *) = 0;
    virtual void visit(IdentifierExpr *) = 0;
};
