#pragma once
#include "ast.hpp"
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include "dhtt.hpp"

struct ReturnException : std::exception
{
};

struct BreakException : std::exception
{
};

struct ContinueException : std::exception
{
};

template <typename T>
struct Stack
{
    std::vector<T> stack;

    void push(T item)
    {
        stack.push_back(std::move(item));
    }

    T pop()
    {
        T item = std::move(stack.back());
        stack.pop_back();
        return std::move(item);
    }
};

enum MyType
{
    MYINT,
    MYFLOAT,
    MYSTRING,
    MYBOOL,
    MYNONE,
    MYFUNCTION,
    MYARRAY,
};

struct Interpreter;
struct Value;
struct Callable
{
    std::unique_ptr<FnDecl> fn_decl;

    void call(Interpreter *interpreter, std::vector<Value> args);

    Callable(FnDecl *fn_decl) : fn_decl(fn_decl) {}
};

struct Array
{
    std::vector<Value> elements;

    Array(std::vector<Value> elements) : elements(elements) {}

    Value operator[](int index);

    Value operator[](Value index);
};

struct Value
{
    union
    {
        int int_value;
        float float_value;
        std::string string_value;
        bool bool_value;
        Callable *callable;
        Array *array;
    };

    MyType type;

    Value() : type(MyType::MYNONE) {}
    Value(int value) : int_value(value), type(MyType::MYINT) {}
    Value(float value) : float_value(value), type(MyType::MYFLOAT) {}
    Value(std::string value) : string_value(value), type(MyType::MYSTRING) {}
    Value(bool value) : bool_value(value), type(MyType::MYBOOL) {}
    Value(Callable *value) : callable(value), type(MyType::MYFUNCTION) {}
    Value(Array *value) : array(value), type(MyType::MYARRAY) {}

    Value(const Value &other) : type(other.type)
    {
        switch (type)
        {
        case MyType::MYINT:
            int_value = other.int_value;
            break;
        case MyType::MYFLOAT:
            float_value = other.float_value;
            break;
        case MyType::MYSTRING:
            new (&string_value) std::string(other.string_value);
            break;
        case MyType::MYBOOL:
            bool_value = other.bool_value;
            break;
        case MyType::MYFUNCTION:
            callable = other.callable;
            break;
        case MyType::MYARRAY:
            array = other.array;
            break;
        default:
            break;
        }
    }

    ~Value()
    {
        if (type == MyType::MYSTRING)
        {
            string_value.~basic_string();
        }
    }

    Value operator=(const Value &other)
    {
        if (this == &other)
        {
            return *this;
        }

        type = other.type;

        switch (type)
        {
        case MyType::MYINT:
            int_value = other.int_value;
            break;
        case MyType::MYFLOAT:
            float_value = other.float_value;
            break;
        case MyType::MYSTRING:
            new (&string_value) std::string(other.string_value);
            break;
        case MyType::MYBOOL:
            bool_value = other.bool_value;
            break;
        case MyType::MYFUNCTION:
            callable = other.callable;
            break;
        case MyType::MYARRAY:
            array = other.array;
            break;
        default:
            break;
        }

        return *this;
    }

    Value operator+(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value + other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value + other.float_value);
        }
        else if (type == MyType::MYSTRING && other.type == MyType::MYSTRING)
        {
            return Value(string_value + other.string_value);
        }
        else
        {
            std::cerr << "Invalid types for addition" << std::endl;
            exit(1);
        }
    }

    Value operator-(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value - other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value - other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for subtraction" << std::endl;
            exit(1);
        }
    }

    Value operator*(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value * other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value * other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for multiplication" << std::endl;
            exit(1);
        }
    }

    Value operator/(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value / other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value / other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for division" << std::endl;
            exit(1);
        }
    }

    Value operator%(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value % other.int_value);
        }
        else
        {
            std::cerr << "Invalid types for modulo" << std::endl;
            exit(1);
        }
    }

    Value operator==(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value == other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value == other.float_value);
        }
        else if (type == MyType::MYSTRING && other.type == MyType::MYSTRING)
        {
            return Value(string_value == other.string_value);
        }
        else if (type == MyType::MYBOOL && other.type == MyType::MYBOOL)
        {
            return Value(bool_value == other.bool_value);
        }
        else
        {
            std::cerr << "Invalid types for equality" << std::endl;
            exit(1);
        }
    }

    Value operator!=(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value != other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value != other.float_value);
        }
        else if (type == MyType::MYSTRING && other.type == MyType::MYSTRING)
        {
            return Value(string_value != other.string_value);
        }
        else if (type == MyType::MYBOOL && other.type == MyType::MYBOOL)
        {
            return Value(bool_value != other.bool_value);
        }
        else
        {
            std::cerr << "Invalid types for inequality" << std::endl;
            exit(1);
        }
    }

    Value operator<(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value < other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value < other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for less than" << std::endl;
            exit(1);
        }
    }

    Value operator<=(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value <= other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value <= other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for less than or equal" << std::endl;
            exit(1);
        }
    }

    Value operator>(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value > other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value > other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for greater than" << std::endl;
            exit(1);
        }
    }

    Value operator>=(const Value &other)
    {
        if (type == MyType::MYINT && other.type == MyType::MYINT)
        {
            return Value(int_value >= other.int_value);
        }
        else if (type == MyType::MYFLOAT && other.type == MyType::MYFLOAT)
        {
            return Value(float_value >= other.float_value);
        }
        else
        {
            std::cerr << "Invalid types for greater than or equal" << std::endl;
            exit(1);
        }
    }

    Value operator!()
    {
        if (type == MyType::MYBOOL)
        {
            return Value(!bool_value);
        }
        else
        {
            std::cerr << "Invalid type for logical not" << std::endl;
            exit(1);
        }
    }

    Value operator-()
    {
        if (type == MyType::MYINT)
        {
            return Value(-int_value);
        }
        else if (type == MyType::MYFLOAT)
        {
            return Value(-float_value);
        }
        else
        {
            std::cerr << "Invalid type for negation" << std::endl;
            exit(1);
        }
    }

    std::string to_string()
    {
        switch (type)
        {
        case MyType::MYINT:
            return std::to_string(int_value);
        case MyType::MYFLOAT:
            return std::to_string(float_value);
        case MyType::MYSTRING:
            return string_value;
        case MyType::MYBOOL:
            return bool_value ? "true" : "false";
        case MyType::MYNONE:
            return "None";
        case MyType::MYFUNCTION:
            return "Function";
        case MyType::MYARRAY:
            return "Array";
        default:
            return "Unknown";
        }
    }
};

template <typename T>
struct Environment
{
    std::vector<std::unordered_map<std::string, T>> env;

    void push_env()
    {
        env.push_back(std::unordered_map<std::string, T>());
    }

    void pop_env()
    {
        env.pop_back();
    }

    void set(std::string key, T value)
    {
        for (auto it = env.rbegin(); it != env.rend(); ++it)
        {
            if (it->find(key) != it->end())
            {
                it->at(key) = value;
                return;
            }
        }

        env.back()[key] = value;
    }

    bool contains(std::string key)
    {
        for (auto it = env.rbegin(); it != env.rend(); ++it)
        {
            if (it->find(key) != it->end())
            {
                return true;
            }
        }

        return false;
    }

    T get(std::string key)
    {
        for (auto it = env.rbegin(); it != env.rend(); ++it)
        {
            auto value = it->find(key);
            if (value != it->end())
            {
                return value->second;
            }
        }

        return T();
    }
};

struct Interpreter : Visitor
{

    Stack<Value> stack;
    Stack<std::shared_ptr<DHTT::Node>> node_stack;
    Environment<Value> env;
    std::shared_ptr<DHTT::Node> root;
    std::shared_ptr<DHTT::Node> current_root;

    Interpreter()
    {
        env.push_env();
        root = std::shared_ptr<DHTT::Node>(new DHTT::Then());
        current_root = root;
    }

    void evaluate(Program *program)
    {
        for (auto &input : program->inputs)
        {
            input->accept(this);
        }

        for (int i = 0; i < program->stmts.size(); i++)
        {
            auto &stmt = program->stmts[program->stmts.size() - 1 - i];
            stmt->accept(this);
        }

        program->treeNode->accept(this);

        for (int i = 0; i < node_stack.stack.size(); i++)
        {
            root->add(std::move(node_stack.pop()));
        }
    }

    void in_new_scope(std::function<void(void)> f)
    {
        env.push_env();
        f();
        env.pop_env();
    }

    virtual void visit(IfStmt *stmt) override
    {
        stmt->condition->accept(this);
        auto condition = stack.pop();

        if (condition.type != MyType::MYBOOL)
        {
            std::cerr << "Expected boolean value in if statement condition" << std::endl;
            exit(1);
        }

        if (condition.bool_value)
        {
            stmt->then_block->accept(this);
        }
    }

    virtual void visit(IfElseStmt *stmt) override
    {
        stmt->condition->accept(this);
        auto condition = stack.pop();

        if (condition.type != MyType::MYBOOL)
        {
            std::cerr << "Expected boolean value in if statement condition" << std::endl;
            exit(1);
        }

        if (condition.bool_value)
        {
            stmt->then_block->accept(this);
        }
        else
        {
            stmt->else_block->accept(this);
        }
    }

    virtual void visit(WhileStmt *stmt) override
    {
        stmt->condition->accept(this);
        auto condition = stack.pop();

        if (condition.type != MyType::MYBOOL)
        {
            std::cerr << "Expected boolean value in while statement condition" << std::endl;
            exit(1);
        }

        auto condition_bool = condition.bool_value;

        while (condition.bool_value)
        {
            try
            {
                stmt->block->accept(this);
            }
            catch (BreakException e)
            {
                break;
            }
            catch (ContinueException e)
            {
                continue;
            }
            stmt->condition->accept(this);
            condition_bool = stack.pop().bool_value;
        }
    }

    virtual void visit(ForInStmt *stmt) override
    {
        stmt->iterable->accept(this);
        auto iterable = stack.pop();

        if (iterable.type != MyType::MYSTRING && iterable.type != MyType::MYARRAY)
        {
            std::cerr << "Expected string value in for statement iterable" << std::endl;
            exit(1);
        }

        if (iterable.type == MyType::MYARRAY)
        {
            for (auto &element : iterable.array->elements)
            {
                try
                {
                    in_new_scope([&]()
                                 {
                                     env.set(stmt->identifier, element);
                                     stmt->block->accept(this); });
                }
                catch (BreakException e)
                {
                    break;
                }
                catch (ContinueException e)
                {
                    continue;
                }
            }
        }
        else
        {
            for (auto &c : iterable.string_value)
            {
                try
                {
                    in_new_scope([&]()
                                 { 
                                    env.set(stmt->identifier, Value(std::string(1, c)));
                                    stmt->block->accept(this); });
                }
                catch (BreakException e)
                {
                    break;
                }
                catch (ContinueException e)
                {
                    continue;
                }
            }
        }
    }

    virtual void visit(ReturnStmt *stmt) override
    {
        if (stmt->is_void)
        {
            // do nothing
        }
        else
        {
            stmt->expr->accept(this);
        }

        throw ReturnException();
    }

    virtual void visit(BreakStmt *stmt) override
    {
        throw BreakException();
    }

    virtual void visit(ContinueStmt *stmt) override
    {
        throw ContinueException();
    }

    virtual void visit(FnDecl *stmt) override
    {
        auto callable = new Callable(stmt);
        env.set(stmt->identifier, callable);
    }

    virtual void visit(VarDecl *stmt) override
    {
        stmt->value->accept(this);
        env.set(stmt->identifier, stack.pop());
    }

    virtual void visit(ExprStmt *stmt) override
    {
        stmt->expr->accept(this);
    }

    virtual void visit(BlockStmt *stmt) override
    {
        in_new_scope([&]()
                     {
                    for (int i = 0; i < stmt->stmts.size(); i++)
                    {
                        auto &s = stmt->stmts[stmt->stmts.size() - i - 1];
                        s->accept(this);
                    } });
    }

    virtual void visit(AssignExpr *expr) override
    {
        if (!env.contains(expr->identifier))
        {
            std::cerr << "Variable " << expr->identifier << " not defined" << std::endl;
            exit(1);
        }

        expr->value->accept(this);
        env.set(expr->identifier, stack.pop());
    }

    virtual void visit(TernaryExpr *expr) override
    {
        expr->condition->accept(this);
        auto condition = stack.pop();

        if (condition.type != MyType::MYBOOL)
        {
            std::cerr << "Expected boolean value in ternary expression condition" << std::endl;
            exit(1);
        }

        if (condition.bool_value)
        {
            expr->then_expr->accept(this);
        }
        else
        {
            expr->else_expr->accept(this);
        }
    }

    virtual void visit(BinaryExpr *expr) override
    {
        expr->left->accept(this);
        auto left = stack.pop();

        expr->right->accept(this);
        auto right = stack.pop();

        if (left.type != right.type)
        {
            std::cerr << "Binary operation between different types" << std::endl;
            exit(1);
        }

        auto op = expr->op;

        if (op == "+")
        {
            stack.push(Value(left + right));
        }
        else if (op == "-")
        {
            stack.push(Value(left - right));
        }
        else if (op == "*")
        {
            stack.push(Value(left * right));
        }
        else if (op == "/")
        {
            stack.push(Value(left / right));
        }
        else if (op == "%")
        {
            stack.push(Value(left % right));
        }
        else if (op == "==")
        {
            stack.push(Value(left == right));
        }
        else if (op == "!=")
        {
            stack.push(Value(left != right));
        }
        else if (op == "<")
        {
            stack.push(Value(left < right));
        }
        else if (op == "<=")
        {
            stack.push(Value(left <= right));
        }
        else if (op == ">")
        {
            stack.push(Value(left > right));
        }
        else if (op == ">=")
        {
            stack.push(Value(left >= right));
        }
        else
        {
            std::cerr << "Unknown binary operator" << std::endl;
            exit(1);
        }
    }

    virtual void visit(UnaryExpr *expr) override
    {
        expr->expr->accept(this);
        auto value = stack.pop();

        if (expr->op == "-")
        {
            stack.push(-value);
        }
        else if (expr->op == "not")
        {
            stack.push(!value);
        }
        else
        {
            std::cerr << "Unknown unary operator" << std::endl;
            exit(1);
        }
    }

    virtual void visit(CallExpr *expr) override
    {
        if (!env.contains(expr->identifier))
        {
            std::cerr << "Function " << expr->identifier << " not defined" << std::endl;
            exit(1);
        }

        std::vector<Value> args;
        for (auto &arg : expr->args)
        {
            arg->accept(this);
            args.push_back(stack.pop());
        }

        auto callable = env.get(expr->identifier).callable;
        callable->call(this, args);
    }

    virtual void visit(IntLiteral *expr) override
    {
        stack.push(Value(expr->value));
    }

    virtual void visit(FloatLiteral *expr) override
    {
        stack.push(Value(expr->value));
    }

    virtual void visit(StringLiteral *expr) override
    {
        stack.push(Value(expr->value));
    }

    virtual void visit(NoneLiteral *expr) override
    {
        stack.push(Value());
    }

    virtual void visit(BoolLiteral *expr) override
    {
        stack.push(Value(expr->value));
    }

    virtual void visit(IdentifierExpr *expr) override
    {
        if (!env.contains(expr->identifier))
        {
            std::cerr << "Variable " << expr->identifier << " not defined" << std::endl;
            exit(1);
        }

        stack.push(env.get(expr->identifier));
    }

    virtual void visit(ArrayLiteral *expr) override
    {
        std::vector<Value> elements;
        for (auto &element : expr->elements)
        {
            element->accept(this);
            elements.push_back(std::move(stack.pop()));
        }

        auto array = new Array(elements);
        stack.push(Value(array));
    }

    void unwrap_pseudo_or_add(std::shared_ptr<DHTT::Node> dest, std::shared_ptr<DHTT::Node> result)
    {
        if (auto pseudo = dynamic_cast<DHTT::Pseudo *>(result.get()))
        {
            for (auto &child : pseudo->children)
            {
                dest->add(std::move(child));
            }
        }
        else
        {
            dest->add(std::move(result));
        }
    }

    virtual void visit(AndNode *node) override
    {
        auto and_node = std::shared_ptr<DHTT::Node>(new DHTT::And());
        current_root = and_node;
        for (int i = 0; i < node->children.size(); i++)
        {
            node->children[node->children.size() - i - 1]->accept(this);
            auto result = std::move(node_stack.pop());
            unwrap_pseudo_or_add(and_node, result);
        }

        node_stack.push(std::move(and_node));
    }

    virtual void visit(OrNode *node) override
    {
        auto or_node = std::shared_ptr<DHTT::Node>(new DHTT::Or());
        current_root = or_node;
        for (int i = 0; i < node->children.size(); i++)
        {
            node->children[node->children.size() - i - 1]->accept(this);
            auto result = std::move(node_stack.pop());
            unwrap_pseudo_or_add(or_node, result);
        }
        node_stack.push(std::move(or_node));
    }

    virtual void visit(ThenNode *node) override
    {

        auto then_node = std::shared_ptr<DHTT::Node>(new DHTT::Then());
        current_root = then_node;
        for (int i = 0; i < node->children.size(); i++)
        {
            node->children[node->children.size() - i - 1]->accept(this);
            auto result = std::move(node_stack.pop());
            unwrap_pseudo_or_add(then_node, result);
        }

        node_stack.push(std::move(then_node));
    }

    virtual void visit(BehaviorNode *node) override
    {
        std::vector<std::string> args;
        for (auto &arg : node->args)
        {
            arg->accept(this);
            args.push_back(stack.pop().to_string());
        }

        auto behavior_node = std::shared_ptr<DHTT::Node>(new DHTT::Behavior(node->identifier, args));
        node_stack.push(std::move(behavior_node));
    }

    virtual void visit(AtIfNode *at_if) override
    {
        at_if->condition->accept(this);
        auto condition = stack.pop();

        if (condition.type != MyType::MYBOOL)
        {
            std::cerr << "Expected boolean value in if statement condition" << std::endl;
            exit(1);
        }

        auto pseudo_node = std::shared_ptr<DHTT::Node>(new DHTT::Pseudo());
        if (condition.bool_value)
        {
            for (int i = 0; i < at_if->children.size(); i++)
            {
                at_if->children[at_if->children.size() - i - 1]->accept(this);
                auto result = std::move(node_stack.pop());
                unwrap_pseudo_or_add(pseudo_node, result);
            }
        }

        node_stack.push(std::move(pseudo_node));
    }

    virtual void visit(AtIfElseNode *at_if_else) override
    {
        at_if_else->condition->accept(this);
        auto condition = stack.pop();

        if (condition.type != MyType::MYBOOL)
        {
            std::cerr << "Expected boolean value in if statement condition" << std::endl;
            exit(1);
        }

        auto pseudo_node = std::shared_ptr<DHTT::Node>(new DHTT::Pseudo());
        if (condition.bool_value)
        {
            for (int i = 0; i < at_if_else->then_children.size(); i++)
            {
                at_if_else->then_children[at_if_else->then_children.size() - 1 - i]->accept(this);
                auto result = std::move(node_stack.pop());
                unwrap_pseudo_or_add(pseudo_node, result);
            }
        }
        else
        {
            for (int i = 0; i < at_if_else->else_children.size(); i++)
            {
                at_if_else->else_children[at_if_else->else_children.size() - 1 - i]->accept(this);
                auto result = std::move(node_stack.pop());
                unwrap_pseudo_or_add(pseudo_node, result);
            }
        }

        node_stack.push(std::move(pseudo_node));
    }

    virtual void visit(AtForNode *at_for) override
    {
        at_for->iterable->accept(this);
        auto iterable = stack.pop();

        if (iterable.type != MyType::MYSTRING && iterable.type != MyType::MYARRAY)
        {
            std::cerr << "Expected string or array value in for statement iterable" << std::endl;
            exit(1);
        }

        auto pseudo_node = std::shared_ptr<DHTT::Node>(new DHTT::Pseudo());
        if (iterable.type == MyType::MYARRAY)
        {
            for (auto &element : iterable.array->elements)
            {
                in_new_scope([&]()
                             {
                env.set(at_for->identifier, element);
                for (auto &child : at_for->children)
                {
                    child->accept(this);
                    auto result = std::move(node_stack.pop());
                    unwrap_pseudo_or_add(pseudo_node, result);
                } });
            }
        }
        else
        {
            for (auto &c : iterable.string_value)
            {
                in_new_scope([&]()
                             {
                env.set(at_for->identifier, Value(std::string(1, c)));
                for (auto &child : at_for->children)
                {
                    child->accept(this);
                    auto result = std::move(node_stack.pop());
                    if (auto pseudo = dynamic_cast<DHTT::Pseudo *>(result.get()))
                    {
                        for (auto &child : pseudo->children)
                        {
                            pseudo_node->add(std::move(child));
                        }
                    }
                    else
                    {
                        pseudo_node->add(std::move(result));
                    }
                } });
            }
        }

        node_stack.push(std::move(pseudo_node));
    }

    virtual void visit(InputDefault *input) override
    {
        input->value->accept(this);
        env.set(input->identifier, Value(stack.pop()));
    }

    virtual void visit(Input *input) override
    {
        env.set(input->identifier, Value());
    }
};

void Callable::call(Interpreter *interpreter, std::vector<Value> args)
{
    interpreter->in_new_scope([&]()
                              {
    for (int i = 0; i < args.size(); i++)
    {
        interpreter->env.set(fn_decl->params[i]->identifier, args[i]);
    }

    try
    {
        fn_decl->block->accept(interpreter);
    }
    catch (ReturnException e)
    {
        interpreter->env.pop_env();
        return;
    } });
}

Value Array::operator[](int index)
{
    return elements[index];
}

Value Array::operator[](Value index)
{
    if (index.type != MyType::MYINT)
    {
        std::cerr << "Array index must be an integer" << std::endl;
        exit(1);
    }

    return elements[index.int_value];
}