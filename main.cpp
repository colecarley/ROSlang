#include <iostream>
#include <fstream>
#include <algorithm>
#include "ast.hpp"
#include "parser.hpp"
#include "visitor.hpp"
#include "printer.hpp"
#include "interpreter.hpp"
#include "dhtt.hpp"

void ros_parse(Program **root, const char *source);

void print_tree(DHTT::Node *root, int indent = 0)
{
    if (root == nullptr)
    {
        return;
    }

    for (int i = 0; i < indent; i++)
    {
        std::cout << "  ";
    }

    if (auto andNode = dynamic_cast<DHTT::And *>(root))
    {
        std::cout << "AndNode\n";
    }
    else if (auto orNode = dynamic_cast<DHTT::Or *>(root))
    {
        std::cout << "OrNode\n";
    }
    else if (auto thenNode = dynamic_cast<DHTT::Then *>(root))
    {
        std::cout << "thenNode\n";
    }
    else if (auto behaviorNode = dynamic_cast<DHTT::Behavior *>(root))
    {
        std::cout << "behaviorNode\n";
    }
    else
    {
        std::cerr << "Unknown node type" << std::endl;
        exit(1);
    }

    for (auto &child : root->children)
    {
        print_tree(child.get(), indent + 1);
    }
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
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

    Printer printer;

    printer.print(root);

    Interpreter interpreter;
    interpreter.evaluate(root);

    print_tree(interpreter.root.get());

    return 0;
}
