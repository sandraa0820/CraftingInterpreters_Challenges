package lox;

import java.util.List;

import lox.Expr.Function;

class LoxFunction implements LoxCallable {
    private final Stmt.Function declaration;
    private final Expr.Function expression;
    private final Environment closure;
    private final LoxInstance receiver;

    private final boolean isInitializer;
    private final LoxClass owner;

    LoxFunction(Stmt.Function declaration, Environment closure,
                boolean isInitializer) {
            this.isInitializer = isInitializer;
            this.declaration = declaration;
            this.closure = closure;
            this.expression = null;
            this.owner = null;
            this.receiver = null;

        }

    LoxFunction(Stmt.Function declaration, Environment closure,
                boolean isInitializer, LoxClass owner, LoxInstance receiver) {
            this.isInitializer = isInitializer;
            this.declaration = declaration;
            this.closure = closure;
            this.expression = null;
            this.owner = owner;
            this.receiver = receiver;

    }

    LoxFunction(Expr.Function expression, Environment closure) {
        this.declaration = null;
        this.expression = expression;
        this.closure = closure;
        this.isInitializer = false;
        this.owner = null;
        this.receiver = null;
    }

    LoxFunction bind(LoxInstance instance) {
        Environment environment = new Environment(closure);
        environment.define(0, instance);

        return new LoxFunction(declaration, environment, isInitializer, owner, instance);
    }

    LoxFunction withOwner(LoxClass owner) {
        return new LoxFunction(declaration, closure, isInitializer, owner, receiver);
    }

    boolean isGetter() {
        return declaration != null && declaration.isGetter;
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

       boolean isMethod = owner != null;

    if (isMethod) {
        interpreter.pushMethod(this);
    }

    try {
        try {
            interpreter.executeBlock(body, environment);
        } catch (Return returnValue) {
            if (isInitializer) return closure.getAt(0, 0);
            return returnValue.value;
        }
    } finally {
        if (isMethod) {
            interpreter.popMethod();
        }
    }

        if (isInitializer) return closure.getAt(0, 0);
        return null;
    }

    LoxClass getOwner() {
        return owner;
    }

    LoxInstance getReceiver() {
        return receiver;
    }

    String getName() {
        return declaration.name.lexeme;
    }
}
