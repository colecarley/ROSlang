
struct ASTNode;

struct Visitor
{
    virtual void visit(ASTNode *node) = 0;
};

struct ASTNode
{
    virtual void accept(Visitor) = 0;
};
