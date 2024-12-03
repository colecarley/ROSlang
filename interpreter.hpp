#pragma once

#include "ast.hpp"
#include <unistd.h>
#include <limits.h> // For PATH_MAX

#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include "dhtt.hpp"
#include "exceptions/index.hpp"
#include "stack.hpp"
#include "environment.hpp"
#include "array.hpp"
#include "standard_lib.hpp"
#include "value.hpp"
#include "callable.hpp"
#include "parser.hpp"

void ros_parse(Program **root, const char *source);

struct Interpreter : Visitor
{

    Stack<Value> stack;
    Stack<std::shared_ptr<DHTT::Node>> node_stack;
    Environment<Value> env;
    std::vector<std::shared_ptr<DHTT::Node>> roots;
    std::shared_ptr<DHTT::Node> current_root;

    Interpreter()
    {
        env.push_env();
        current_root = nullptr;
    }

    void evaluate(Program *program)
    {
        for (auto &input : program->inputs)
        {
            if (dynamic_cast<InputDefault *>(input.get()))
            {
                auto default_input = dynamic_cast<InputDefault *>(input.get());
                default_input->value->accept(this);
                env.set(input->identifier, stack.pop());
            }
            else
            {
                input->accept(this);
            }
        }

        for (int i = 0; i < program->stmts.size(); i++)
        {
            auto &stmt = program->stmts[program->stmts.size() - 1 - i];
            stmt->accept(this);
        }

        program->treeNode->accept(this);

        for (int i = 0; i < node_stack.stack.size(); i++)
        {
            this->roots.push_back(std::move(node_stack.pop()));
        }
    }

    void evaluate(Program *program, std::vector<Value> inputs)
    {
        for (auto &input : program->inputs)
        {
            input->accept(this);
        }

        for (int i = 0; i < inputs.size(); i++)
        {
            env.set(program->inputs[i]->identifier, inputs[i]);
        }

        for (int i = 0; i < program->stmts.size(); i++)
        {
            auto &stmt = program->stmts[program->stmts.size() - 1 - i];
            stmt->accept(this);
        }

        program->treeNode->accept(this);

        for (int i = 0; i < node_stack.stack.size(); i++)
        {
            this->roots.push_back(std::move(node_stack.pop()));
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

    virtual void visit(LambdaExpr *expr) override
    {
        auto callable = new Callable(expr);
        stack.push(Value(callable));
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

    virtual void visit(ArrayAssignExpr *expr) override
    {
        if (!env.contains(expr->identifier))
        {
            std::cerr << "Variable " << expr->identifier << " not defined" << std::endl;
            exit(1);
        }

        expr->index->accept(this);
        auto index = stack.pop();

        expr->value->accept(this);
        auto value = stack.pop();

        auto array_value = env.get(expr->identifier);
        auto array = array_value.array;

        if (index.type == MyType::MYINT)
        {
            array->set(index, value);
        }
        else
        {
            std::cerr << "Array index must be an integer" << std::endl;
            exit(1);
        }
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
            if (expr->identifier == "print")
            {

                auto values = std::vector<Value>();
                for (auto &arg : expr->args)
                {
                    arg->accept(this);
                    values.push_back(stack.pop());
                }

                print(values);
                return;
            }
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

    virtual void visit(ArrayAccessExpr *expr) override
    {
        if (!env.contains(expr->identifier))
        {
            std::cerr << "Variable " << expr->identifier << " not defined" << std::endl;
            exit(1);
        }
        expr->index->accept(this);
        auto index = stack.pop();

        auto array_value = env.get(expr->identifier);
        auto array = array_value.array;

        if (index.type == MyType::MYINT)
        {
            stack.push((*array)[index]);
        }
        else
        {
            std::cerr << "Array index must be an integer" << std::endl;
            exit(1);
        }
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
        for (auto it = expr->elements.rbegin(); it != expr->elements.rend(); ++it)
        {
            it->get()->accept(this);
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

    virtual void visit(AtLoadNode *at_load) override
    {
        std::vector<Value> args;
        for (auto it = at_load->args.rbegin(); it != at_load->args.rend(); ++it)
        {
            it->get()->accept(this);
            args.push_back(stack.pop());
        }

        if (args.size() < 1 || args[0].type != MyType::MYSTRING)
        {
            std::cerr << "Expected string value as first argument to load" << std::endl;
            exit(1);
        }

        std::string filename = args[0].string_value.substr(1, args[0].string_value.length() - 2);

        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Could not open file: " << filename << std::endl;
            exit(1);
        }

        std::string source;
        std::string line;
        while (getline(file, line))
        {
            source += line + '\n';
        }

        file.close();

        Program *root = nullptr;
        ros_parse(&root, source.c_str());

        Interpreter interpreter;
        interpreter.evaluate(root, std::vector<Value>(args.begin() + 1, args.end()));

        auto tree = interpreter.roots;

        for (auto &child : tree)
        {
            node_stack.push(std::move(child));
        }
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
