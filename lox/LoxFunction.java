package lox;

import java.util.List;

class LoxFunction implements LoxCallable {
    private final Stmt.Function declaration;
    private final Expr.Function expression;
    private final Environment closure;

    LoxFunction(Stmt.Function declaration, Environment closure) {
        this.closure = closure;
        this.declaration = declaration;
        this.expression = null;
    }

    LoxFunction(Expr.Function expression, Environment closure) {
        this.declaration = null;
        this.expression = expression;
        this.closure = closure;
    }

    @Override
    public int arity() {
        if (declaration != null) {
            return declaration.params.size();
        }
        return expression.params.size();
    }

    @Override
    public String toString() {
        if (declaration != null) {
            return "<fn " + declaration.name.lexeme + ">";
        }
        return "<fn anonymous>";
    }

    @Override
    public Object call(Interpreter interpreter,
                     List<Object> arguments) {

        List<Token> params;
        List<Stmt> body;

        if (declaration != null) {
            params = declaration.params;
            body = declaration.body;
        } else {
            params = expression.params;
            body = expression.body;
        }

        Environment environment = new Environment(closure);
        for (int i = 0; i < params.size(); i++) {
            environment.define(i,
                arguments.get(i));
        }

        try { 
            interpreter.executeBlock(body, environment);
        } catch (Return returnValue) {
            return returnValue.value;
        }
        return null;
    }
}
