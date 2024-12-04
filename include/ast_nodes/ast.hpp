#pragma once

#include <string>
#include <vector>
#include "visitors/visitor.hpp"
#include <memory>

// expr nodes
struct Expr;
struct LambdaExpr;
struct AssignExpr;
struct TernaryExpr;
struct BinaryExpr;
struct UnaryExpr;
struct CallExpr;
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

// type nodes
struct Type;
struct PrimitiveType;
struct ArrayType;
struct FunctionType;

// helper structs
template <typename T>
struct List;
struct IdentifierType;

// others
struct Program;

// the root node of the AST
struct Program
{
    std::vector<std::unique_ptr<Input>> inputs;
    std::vector<std::unique_ptr<Stmt>> stmts;
    std::unique_ptr<TreeNode> treeNode;

    Program(std::vector<Input *> inputs, std::vector<Stmt *> stmts, TreeNode *treeNode) : treeNode(treeNode)
    {
        for (auto &input : inputs)
        {
            this->inputs.push_back(std::unique_ptr<Input>(input));
        }
        for (auto &stmt : stmts)
        {
            this->stmts.push_back(std::unique_ptr<Stmt>(stmt));
        }
    }
    Program(std::vector<std::unique_ptr<Input>> inputs, std::vector<std::unique_ptr<Stmt>> stmts, TreeNode *treeNode) : inputs(std::move(inputs)), stmts(std::move(stmts)), treeNode(std::move(treeNode)) {}
};

struct ASTNode
{
    virtual void accept(Visitor *) = 0;
};

// Input nodes
struct Input : ASTNode
{
    std::string identifier;
    std::unique_ptr<Type> type;

    Input(std::string identifier, Type *type) : identifier(identifier), type(type) {}
    Input(std::string identifier, std::unique_ptr<Type> type) : identifier(identifier), type(std::move(type)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct InputDefault : Input
{
    std::unique_ptr<Expr> value;

    InputDefault(std::string identifier, Type *type, Expr *value) : Input(identifier, type), value(value) {}
    InputDefault(std::string identifier, std::unique_ptr<Type> type, std::unique_ptr<Expr> value) : Input(identifier, type.get()), value(std::move(value)) {}
};

// Expr nodes
struct Expr : ASTNode
{
    virtual void accept(Visitor *v) = 0;
    virtual ~Expr() {}
};

struct LambdaExpr : Expr
{
    std::vector<std::unique_ptr<IdentifierType>> params;
    std::unique_ptr<Type> return_type;
    std::unique_ptr<Expr> expr;

    LambdaExpr(std::vector<IdentifierType *> params, Type *return_type, Expr *expr) : return_type(return_type), expr(expr)
    {
        for (auto &param : params)
        {
            this->params.push_back(std::unique_ptr<IdentifierType>(param));
        }
    }
    LambdaExpr(std::vector<std::unique_ptr<IdentifierType>> params, std::unique_ptr<Type> return_type, std::unique_ptr<Expr> expr) : params(std::move(params)), return_type(std::move(return_type)), expr(std::move(expr)) {}
    LambdaExpr(std::vector<std::unique_ptr<IdentifierType>> params, Type *return_type, Expr *expr) : return_type(return_type), expr(expr), params(std::move(params)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct AssignExpr : Expr
{
    std::string identifier;
    std::unique_ptr<Expr> value;

    AssignExpr(std::string identifier, Expr *value) : identifier(identifier), value(value) {}
    AssignExpr(std::string identifier, std::unique_ptr<Expr> value) : identifier(identifier), value(std::move(value)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct ArrayAssignExpr : Expr
{
    std::string identifier;
    std::unique_ptr<Expr> index;
    std::unique_ptr<Expr> value;

    ArrayAssignExpr(std::string identifier, Expr *index, Expr *value) : identifier(identifier), index(index), value(value) {}
    ArrayAssignExpr(std::string identifier, std::unique_ptr<Expr> index, std::unique_ptr<Expr> value) : identifier(identifier), index(std::move(index)), value(std::move(value)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct TernaryExpr : Expr
{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> then_expr;
    std::unique_ptr<Expr> else_expr;

    TernaryExpr(Expr *condition, Expr *then_expr, Expr *else_expr) : condition(condition), then_expr(then_expr), else_expr(else_expr) {}
    TernaryExpr(std::unique_ptr<Expr> condition, std::unique_ptr<Expr> then_expr, std::unique_ptr<Expr> else_expr) : condition(std::move(condition)), then_expr(std::move(then_expr)), else_expr(std::move(else_expr)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct BinaryExpr : Expr
{
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    std::string op;

    BinaryExpr(Expr *left, Expr *right, std::string op) : left(left), right(right), op(op) {}
    BinaryExpr(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right, std::string op) : left(std::move(left)), right(std::move(right)), op(op) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct UnaryExpr : Expr
{
    std::unique_ptr<Expr> expr;
    std::string op;

    UnaryExpr(Expr *expr, std::string op) : expr(expr), op(op) {}
    UnaryExpr(std::unique_ptr<Expr> expr, std::string op) : expr(std::move(expr)), op(op) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct CallExpr : Expr
{
    std::string identifier;
    std::vector<std::unique_ptr<Expr>> args;

    CallExpr(std::string identifier, std::vector<Expr *> args) : identifier(identifier)
    {
        for (auto &arg : args)
        {
            this->args.push_back(std::unique_ptr<Expr>(arg));
        }
    }
    CallExpr(std::string identifier, std::vector<std::unique_ptr<Expr>> args) : identifier(identifier), args(std::move(args)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct ArrayAccessExpr : Expr
{
    std::string identifier;
    std::unique_ptr<Expr> index;

    ArrayAccessExpr(std::string identifier, Expr *index) : identifier(identifier), index(index) {}
    ArrayAccessExpr(std::string identifier, std::unique_ptr<Expr> index) : identifier(identifier), index(std::move(index)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct ArrayLiteral : Expr
{
    std::vector<std::unique_ptr<Expr>> elements;

    ArrayLiteral(std::vector<Expr *> elements)
    {
        for (auto &element : elements)
        {
            this->elements.push_back(std::unique_ptr<Expr>(element));
        }
    }
    ArrayLiteral(std::vector<std::unique_ptr<Expr>> elements) : elements(std::move(elements)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct IntLiteral : Expr
{
    int value;

    IntLiteral(int value) : value(value) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct FloatLiteral : Expr
{
    float value;

    FloatLiteral(float value) : value(value) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct StringLiteral : Expr
{
    std::string value;

    StringLiteral(std::string value) : value(value) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct NoneLiteral : Expr
{
    NoneLiteral() {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct BoolLiteral : Expr
{
    bool value;

    BoolLiteral(bool value) : value(value) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct IdentifierExpr : Expr
{
    std::string identifier;

    IdentifierExpr(std::string identifier) : identifier(identifier) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

// Stmt nodes
struct Stmt : ASTNode
{
    virtual void accept(Visitor *v) = 0;
    virtual ~Stmt() {}
};

struct IfStmt : Stmt
{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<BlockStmt> then_block;

    IfStmt(Expr *condition, Stmt *then_block) : condition(condition), then_block((BlockStmt *)then_block) {}
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<BlockStmt> then_block) : condition(std::move(condition)), then_block(std::move(then_block)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct IfElseStmt : Stmt
{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<BlockStmt> then_block;
    std::unique_ptr<BlockStmt> else_block;

    // TODO: should be a block stmt
    IfElseStmt(Expr *condition, Stmt *then_block, Stmt *else_block) : condition(condition), then_block((BlockStmt *)then_block), else_block((BlockStmt *)else_block) {}
    IfElseStmt(std::unique_ptr<Expr> condition, std::unique_ptr<BlockStmt> then_block, std::unique_ptr<BlockStmt> else_block) : condition(std::move(condition)), then_block(std::move(then_block)), else_block(std::move(else_block)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct WhileStmt : Stmt
{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<BlockStmt> block;

    // TODO: should be a block stmt
    WhileStmt(Expr *condition, Stmt *block) : condition(condition), block((BlockStmt *)block) {}
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<BlockStmt> block) : condition(std::move(condition)), block(std::move(block)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct ForInStmt : Stmt
{
    std::string identifier;
    std::unique_ptr<Expr> iterable;
    std::unique_ptr<BlockStmt> block;

    // TODO: should be a block stmt
    ForInStmt(std::string identifier, Expr *iterable, Stmt *block) : identifier(identifier), iterable(iterable), block((BlockStmt *)block) {}
    ForInStmt(std::string idntifier, std::unique_ptr<Expr> iterable, std::unique_ptr<BlockStmt> block) : identifier(idntifier), iterable(std::move(iterable)), block(std::move(block)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct ReturnStmt : Stmt
{
    std::unique_ptr<Expr> expr;
    bool is_void = false;

    ReturnStmt(Expr *expr) : expr(expr) {}
    ReturnStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}
    ReturnStmt() : is_void(true) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct BreakStmt : Stmt
{
    BreakStmt() {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct ContinueStmt : Stmt
{
    ContinueStmt() {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct FnDecl : Stmt
{
    std::string identifier;
    std::vector<std::unique_ptr<IdentifierType>> params;
    std::unique_ptr<Type> return_type;
    std::unique_ptr<BlockStmt> block;

    FnDecl(std::string identifier, std::vector<IdentifierType *> params, Type *return_type, BlockStmt *block) : identifier(identifier), return_type(return_type), block(block)
    {
        for (auto &param : params)
        {
            this->params.push_back(std::unique_ptr<IdentifierType>(param));
        }
    }
    FnDecl(std::string identifier, std::vector<std::unique_ptr<IdentifierType>> params, std::unique_ptr<Type> return_type, std::unique_ptr<BlockStmt> block) : identifier(identifier), params(std::move(params)), return_type(std::move(return_type)), block(std::move(block)) {}
    FnDecl(std::string identifier, std::vector<std::unique_ptr<IdentifierType>> params, Type *return_type, BlockStmt *block) : identifier(identifier), return_type(return_type), block(block), params(std::move(params)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct VarDecl : Stmt
{
    std::string identifier;
    std::unique_ptr<Type> type;
    std::unique_ptr<Expr> value;

    VarDecl(std::string identifier, Type *type, Expr *value) : identifier(identifier), type(type), value(value) {}
    VarDecl(std::string identifier, std::unique_ptr<Type> type, std::unique_ptr<Expr> value) : identifier(identifier), type(std::move(type)), value(std::move(value)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct ExprStmt : Stmt
{
    std::unique_ptr<Expr> expr;

    ExprStmt(Expr *expr) : expr(expr) {}
    ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct BlockStmt : Stmt
{
    std::vector<std::unique_ptr<Stmt>> stmts;

    BlockStmt(std::vector<Stmt *> stmts)
    {
        for (auto &stmt : stmts)
        {
            this->stmts.push_back(std::unique_ptr<Stmt>(stmt));
        }
    }
    BlockStmt(std::vector<std::unique_ptr<Stmt>> stmts) : stmts(std::move(stmts)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

// type nodes
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
    std::unique_ptr<Type> type;

    ArrayType(Type *type) : type(type) {}
    ArrayType(std::unique_ptr<Type> type) : type(std::move(type)) {}
};

struct FunctionType : Type
{
    std::vector<std::unique_ptr<Type>> params;
    std::unique_ptr<Type> return_type;

    FunctionType(std::vector<Type *> params, Type *return_type)
    {
        for (auto &param : params)
        {
            this->params.push_back(std::unique_ptr<Type>(param));
        }
        this->return_type = std::unique_ptr<Type>(return_type);
    }
    FunctionType(std::vector<std::unique_ptr<Type>> params, Type *return_type) : params(std::move(params))
    {
        this->return_type = std::unique_ptr<Type>(return_type);
    }
};

// tree nodes
struct TreeNode : ASTNode
{
    std::vector<std::unique_ptr<TreeNode>> children;
    virtual void accept(Visitor *v) = 0;
    virtual ~TreeNode() {}
};

struct AndNode : TreeNode
{
    std::vector<std::unique_ptr<TreeNode>> children = {};
    AndNode(std::vector<TreeNode *> children)
    {
        for (auto &child : children)
        {
            this->children.push_back(std::unique_ptr<TreeNode>(child));
        }
    }
    AndNode(std::vector<std::unique_ptr<TreeNode>> children) : children(std::move(children)) {}
    AndNode() {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct OrNode : TreeNode
{
    std::vector<std::unique_ptr<TreeNode>> children = {};
    OrNode(std::vector<TreeNode *> children)
    {
        for (auto &child : children)
        {
            this->children.push_back(std::unique_ptr<TreeNode>(child));
        }
    }
    OrNode(std::vector<std::unique_ptr<TreeNode>> children) : children(std::move(children)) {}
    OrNode() {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct ThenNode : TreeNode
{
    std::vector<std::unique_ptr<TreeNode>> children = {};

    ThenNode(std::vector<TreeNode *> children)
    {
        for (auto &child : children)
        {
            this->children.push_back(std::unique_ptr<TreeNode>(child));
        }
    }
    ThenNode(std::vector<std::unique_ptr<TreeNode>> children) : children(std::move(children)) {}
    ThenNode() {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct BehaviorNode : TreeNode
{
    std::string identifier;
    std::vector<std::unique_ptr<Expr>> args;

    BehaviorNode(std::string identifier, std::vector<Expr *> args) : identifier(identifier)
    {
        for (auto &arg : args)
        {
            this->args.push_back(std::unique_ptr<Expr>(arg));
        }
    }
    BehaviorNode(std::string identifier, std::vector<std::unique_ptr<Expr>> args) : identifier(identifier), args(std::move(args)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct PseudoNode : TreeNode
{
    virtual void accept(Visitor *v) = 0;
    virtual ~PseudoNode() {}
};

struct AtLoadNode : PseudoNode
{
    std::vector<std::unique_ptr<Expr>> args;

    AtLoadNode(std::vector<std::unique_ptr<Expr>> args) : args(std::move(args)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct AtIfNode : PseudoNode
{
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<TreeNode>> children;

    AtIfNode(Expr *condition, std::vector<TreeNode *> children) : condition(condition)
    {
        for (auto &child : children)
        {
            this->children.push_back(std::unique_ptr<TreeNode>(child));
        }
    }
    AtIfNode(std::unique_ptr<Expr> condition, std::vector<std::unique_ptr<TreeNode>> children) : condition(std::move(condition)), children(std::move(children)) {}

    AtIfNode(Expr *condition, std::vector<std::unique_ptr<TreeNode>> children) : condition(condition), children(std::move(children)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct AtIfElseNode : PseudoNode
{
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<TreeNode>> then_children;
    std::vector<std::unique_ptr<TreeNode>> else_children;

    AtIfElseNode(Expr *condition, std::vector<TreeNode *> then_children, std::vector<TreeNode *> else_children) : condition(condition)
    {
        for (auto &child : then_children)
        {
            this->then_children.push_back(std::unique_ptr<TreeNode>(child));
        }
        for (auto &child : else_children)
        {
            this->else_children.push_back(std::unique_ptr<TreeNode>(child));
        }
    }
    AtIfElseNode(std::unique_ptr<Expr> condition, std::vector<std::unique_ptr<TreeNode>> then_children, std::vector<std::unique_ptr<TreeNode>> else_children) : condition(std::move(condition)), then_children(std::move(then_children)), else_children(std::move(else_children)) {}
    AtIfElseNode(Expr *condition, std::vector<std::unique_ptr<TreeNode>> then_children, std::vector<std::unique_ptr<TreeNode>> else_children) : condition(condition), then_children(std::move(then_children)), else_children(std::move(else_children)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

struct AtForNode : PseudoNode
{
    std::string identifier;
    std::unique_ptr<Expr> iterable;
    std::vector<std::unique_ptr<TreeNode>> children;

    AtForNode(std::string identifier, Expr *iterable, std::vector<TreeNode *> children) : identifier(identifier), iterable(iterable)
    {
        for (auto &child : children)
        {
            this->children.push_back(std::unique_ptr<TreeNode>(child));
        }
    }
    AtForNode(std::string identifier, std::unique_ptr<Expr> iterable, std::vector<std::unique_ptr<TreeNode>> children) : identifier(identifier), iterable(std::move(iterable)), children(std::move(children)) {}
    AtForNode(std::string identifier, Expr *iterable, std::vector<std::unique_ptr<TreeNode>> children) : identifier(identifier), iterable(iterable), children(std::move(children)) {}

    void accept(Visitor *v) override
    {
        v->visit(this);
    }
};

// helper structs

template <typename T>
struct List
{
    std::vector<std::unique_ptr<T>> items = {};

    List(std::vector<T *> items)
    {
        for (auto &item : items)
        {
            this->items.push_back(std::unique_ptr<T>(item));
        }
    }

    List(std::vector<std::unique_ptr<T>> items) : items(std::move(items)) {}

    List() {}

    void add(T *item)
    {
        items.push_back(std::unique_ptr<T>(item));
    }
};

struct IdentifierType
{
    std::string identifier;
    std::unique_ptr<Type> type;

    IdentifierType(std::string identifier, Type *type) : identifier(identifier), type(type) {}
    IdentifierType(std::string identifier, std::unique_ptr<Type> type) : identifier(identifier), type(std::move(type)) {}
};
