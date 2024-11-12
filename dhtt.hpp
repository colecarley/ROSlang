#pragma once
#include <memory>
#include <vector>

namespace DHTT
{
    struct Node
    {
        virtual ~Node() = default;
        std::vector<std::shared_ptr<Node>> children;

        void add(std::shared_ptr<Node> child)
        {
            children.push_back(std::move(child));
        }

        virtual void sayName() = 0;
    };

    struct And : Node
    {
        void sayName() override
        {
            std::cout << "AND" << std::endl;
        }
    };

    struct Or : Node
    {
        void sayName() override
        {
            std::cout << "OR" << std::endl;
        }
    };

    struct Then : Node
    {
        void sayName() override
        {
            std::cout << "THEN" << std::endl;
        }
    };

    struct Behavior : Node // always a leaf node
    {
        std::string identifier;
        std::vector<std::string> args;

        Behavior(std::string identifier, std::vector<std::string> args) : identifier(identifier), args(args) {}

        void sayName() override
        {
            std::cout << "BEHAVIOR: " << identifier << std::endl;
        }
    };

    struct Pseudo : Node // for use in tree building
    {
        void sayName() override
        {
            std::cout << "PSEUDO" << std::endl;
        }
    };
};