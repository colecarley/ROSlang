#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "ast.hpp"

void ros_parse(ASTNode **root, const char *source);

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

    std::cout << source << std::endl;

    ASTNode *root = nullptr;
    ros_parse(&root, source.c_str());

    return 0;
}