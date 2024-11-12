#include "value.hpp"
#include "interpreter.hpp"

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
