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
struct IntLiteral;
struct FloatLiteral;
struct StringLiteral;
struct BoolLiteral;
struct NoneLiteral;
struct IdentifierExpr;
struct ArrayLiteral;
struct TreeNode;
struct AndNode;
struct OrNode;
struct ThenNode;
struct BehaviorNode;
struct DefaultInput;
struct Input;

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
    virtual void visit(IntLiteral *) = 0;
    virtual void visit(FloatLiteral *) = 0;
    virtual void visit(StringLiteral *) = 0;
    virtual void visit(NoneLiteral *) = 0;
    virtual void visit(BoolLiteral *) = 0;
    virtual void visit(IdentifierExpr *) = 0;
    virtual void visit(ArrayLiteral *) = 0;
    virtual void visit(TreeNode *) = 0;
    virtual void visit(AndNode *) = 0;
    virtual void visit(OrNode *) = 0;
    virtual void visit(ThenNode *) = 0;
    virtual void visit(BehaviorNode *) = 0;
    virtual void visit(DefaultInput *) = 0;
    virtual void visit(Input *) = 0;
};
