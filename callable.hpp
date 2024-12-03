#pragma once

#include <memory>
#include "ast.hpp"

struct Interpreter;
struct Value;
struct Callable
{
    std::vector<std::unique_ptr<IdentifierType>> params;
    std::unique_ptr<BlockStmt> block;

    void call(Interpreter *interpreter, std::vector<Value> args);

    Callable(FnDecl *fn_decl) : block(std::move(fn_decl->block)), params(std::move(fn_decl->params)) {}
    Callable(LambdaExpr *lambda_expr) : params(std::move(lambda_expr->params))
    {
        auto expr = std::move(lambda_expr->expr);

        auto stmts = std::vector<std::unique_ptr<Stmt>>();

        stmts.push_back(std::unique_ptr<ReturnStmt>(new ReturnStmt(std::move(expr))));
        block = std::unique_ptr<BlockStmt>(new BlockStmt(std::move(stmts)));
    }
};
