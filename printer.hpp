#pragma once
#include "ast.hpp"

struct Printer : Visitor
{
    void print(Program *program)
    {

        std::cout << "Inputs\n";
        for (auto &input : program->inputs)
        {
            input->accept(this);
        }

        std::cout << "StmtList\n";
        for (auto &stmt : program->stmts)
        {
            stmt->accept(this);
        }

        std::cout << "TreeNode\n";
        program->treeNode->accept(this);
    }

    virtual void visit(IfStmt *stmt) override
    {
        std::cout << "IfStmt\n";

        stmt->condition->accept(this);
        stmt->then_block->accept(this);
    }

    virtual void visit(IfElseStmt *stmt) override
    {
        std::cout << "IfElseStmt\n";

        stmt->condition->accept(this);
        stmt->then_block->accept(this);
        stmt->else_block->accept(this);
    }

    virtual void visit(WhileStmt *stmt) override
    {
        std::cout << "WhileStmt\n";

        stmt->condition->accept(this);
        stmt->block->accept(this);
    }

    virtual void visit(ForInStmt *stmt) override
    {
        std::cout << "ForInStmt\n";

        stmt->iterable->accept(this);
        stmt->block->accept(this);
    }

    virtual void visit(ReturnStmt *stmt) override
    {
        std::cout << "ReturnStmt\n";

        if (stmt->expr)
        {
            stmt->expr->accept(this);
        }
    }

    virtual void visit(BreakStmt *stmt) override
    {
        std::cout << "BreakStmt\n";
    }

    virtual void visit(ContinueStmt *stmt) override
    {
        std::cout << "ContinueStmt\n";
    }

    virtual void visit(FnDecl *stmt) override
    {
        std::cout << "FnDecl\n";

        stmt->block->accept(this);
    }

    virtual void visit(VarDecl *stmt) override
    {
        std::cout << "VarDecl\n";

        stmt->value->accept(this);
    }

    virtual void visit(ExprStmt *stmt) override
    {
        std::cout << "ExprStmt\n";

        stmt->expr->accept(this);
    }

    virtual void visit(BlockStmt *stmt) override
    {
        std::cout << "BlockStmt\n";

        for (auto &stmt : stmt->stmts)
        {
            stmt->accept(this);
        }
    }

    virtual void visit(LambdaExpr *expr) override
    {
        std::cout << "LambdaExpr\n";
        expr->expr->accept(this);
    }

    virtual void visit(AssignExpr *expr) override
    {
        std::cout << "AssignExpr\n";

        expr->value->accept(this);
    }

    virtual void visit(ArrayAssignExpr *expr) override
    {
        std::cout << "ArrayAssignExpr\n";

        expr->index->accept(this);
        expr->value->accept(this);
    }

    virtual void visit(TernaryExpr *expr) override
    {
        std::cout << "TernaryExpr\n";

        expr->condition->accept(this);
        expr->then_expr->accept(this);
        expr->else_expr->accept(this);
    }

    virtual void visit(BinaryExpr *expr) override
    {
        std::cout << "BinaryExpr\n";

        expr->left->accept(this);
        expr->right->accept(this);
    }

    virtual void visit(UnaryExpr *expr) override
    {
        std::cout << "UnaryExpr\n";

        expr->expr->accept(this);
    }

    virtual void visit(CallExpr *expr) override
    {
        std::cout << "CallExpr\n";

        for (auto &arg : expr->args)
        {
            arg->accept(this);
        }
    }

    virtual void visit(ArrayAccessExpr *expr) override
    {
        std::cout << "ArrayAccessExpr\n";

        expr->index->accept(this);
    }

    virtual void visit(IntLiteral *expr) override
    {
        std::cout << "IntLiteral\n";
    }

    virtual void visit(FloatLiteral *expr) override
    {
        std::cout << "FloatLiteral\n";
    }

    virtual void visit(StringLiteral *expr) override
    {
        std::cout << "StringLiteral\n";
    }

    virtual void visit(NoneLiteral *expr) override
    {
        std::cout << "NoneLiteral\n";
    }

    virtual void visit(BoolLiteral *expr) override
    {
        std::cout << "BoolLiteral\n";
    }

    virtual void visit(IdentifierExpr *expr) override
    {
        std::cout << "IdentifierExpr\n";
    }

    virtual void visit(ArrayLiteral *expr) override
    {
        std::cout << "ArrayLiteral\n";

        for (auto &element : expr->elements)
        {
            element->accept(this);
        }
    }

    virtual void visit(AndNode *node) override
    {
        std::cout << "AndNode\n";

        for (auto &child : node->children)
        {
            child->accept(this);
        }
    }

    virtual void visit(OrNode *node) override
    {
        std::cout << "OrNode\n";

        for (auto &child : node->children)
        {
            child->accept(this);
        }
    }

    virtual void visit(ThenNode *node) override
    {
        std::cout << "ThenNode\n";

        for (auto &child : node->children)
        {
            child->accept(this);
        }
    }

    virtual void visit(BehaviorNode *node) override
    {
        std::cout << "BehaviorNode\n";

        for (auto &arg : node->args)
        {
            arg->accept(this);
        }
    }

    virtual void visit(AtLoadNode *at_load) override
    {
        std::cout << "AtLoadNode\n";

        for (auto &arg : at_load->args)
        {
            arg->accept(this);
        }
    }

    virtual void visit(AtIfNode *at_if) override
    {
        std::cout << "AtIfNode\n";

        at_if->condition->accept(this);
        for (auto &child : at_if->children)
        {
            child->accept(this);
        }
    }

    virtual void visit(AtIfElseNode *at_if_else) override
    {
        std::cout << "AtIfElseNode\n";

        at_if_else->condition->accept(this);
        for (auto &child : at_if_else->then_children)
        {
            child->accept(this);
        }
        for (auto &child : at_if_else->else_children)
        {
            child->accept(this);
        }
    }

    virtual void visit(AtForNode *at_for) override
    {
        std::cout << "AtForNode\n";

        at_for->iterable->accept(this);
        for (auto &child : at_for->children)
        {
            child->accept(this);
        }
    }

    virtual void visit(InputDefault *input) override
    {
        std::cout << "DefaultInput\n";

        input->value->accept(this);
    }

    virtual void visit(Input *input) override
    {
        std::cout << "Input\n";
    }
};