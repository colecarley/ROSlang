#pragma once
#include <iostream>

// expr nodes
struct Expr;
struct LambdaExpr;
struct AssignExpr;
struct ArrayAssignExpr;
struct TernaryExpr;
struct BinaryExpr;
struct UnaryExpr;
struct CallExpr;
struct ArrayAccessExpr;
struct LiteralExpr;
struct IdentifierExpr;
struct ArrayLiteral;
struct IntLiteral;
struct FloatLiteral;
struct StringLiteral;
struct NoneLiteral;
struct BoolLiteral;

// stmt nodes
struct Stmt;
struct IfStmt;
struct IfElseStmt;
struct WhileStmt;
struct ForInStmt;
struct ReturnStmt;
struct BreakStmt;
struct ContinueStmt;
struct FnDecl;
struct VarDecl;
struct ExprStmt;
struct BlockStmt;

// tree nodes
struct TreeNode;
struct AndNode;
struct OrNode;
struct ThenNode;
struct BehaviorNode;
struct PseudoNode;
struct AtIfNode;
struct AtIfElseNode;
struct AtForNode;

// input nodes
struct InputDefault;
struct Input;

// others
struct Program;

struct Visitor
{
    virtual void visit(IfStmt *) = 0;
    virtual void visit(IfElseStmt *) = 0;
    virtual void visit(WhileStmt *) = 0;
    virtual void visit(ForInStmt *) = 0;
    virtual void visit(ReturnStmt *) = 0;
    virtual void visit(BreakStmt *) = 0;
    virtual void visit(ContinueStmt *) = 0;
    virtual void visit(FnDecl *) = 0;
    virtual void visit(VarDecl *) = 0;
    virtual void visit(ExprStmt *) = 0;
    virtual void visit(BlockStmt *) = 0;
    virtual void visit(LambdaExpr *) = 0;
    virtual void visit(ArrayAssignExpr *) = 0;
    virtual void visit(AssignExpr *) = 0;
    virtual void visit(TernaryExpr *) = 0;
    virtual void visit(BinaryExpr *) = 0;
    virtual void visit(UnaryExpr *) = 0;
    virtual void visit(CallExpr *) = 0;
    virtual void visit(ArrayAccessExpr *) = 0;
    virtual void visit(IntLiteral *) = 0;
    virtual void visit(FloatLiteral *) = 0;
    virtual void visit(StringLiteral *) = 0;
    virtual void visit(NoneLiteral *) = 0;
    virtual void visit(BoolLiteral *) = 0;
    virtual void visit(IdentifierExpr *) = 0;
    virtual void visit(ArrayLiteral *) = 0;
    virtual void visit(AndNode *) = 0;
    virtual void visit(OrNode *) = 0;
    virtual void visit(ThenNode *) = 0;
    virtual void visit(BehaviorNode *) = 0;
    virtual void visit(InputDefault *) = 0;
    virtual void visit(Input *) = 0;
    virtual void visit(AtIfNode *) = 0;
    virtual void visit(AtIfElseNode *) = 0;
    virtual void visit(AtForNode *) = 0;
};
