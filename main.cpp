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

    root->sayName();

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

    for (auto &root : interpreter.roots)
    {
        print_tree(root.get());
    }

    return 0;
}
