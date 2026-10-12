package lox;

import java.util.List;
import java.util.Map;

class LoxClass extends LoxInstance implements LoxCallable {
  final String name;
  final LoxClass superclass;
  private final Map<String, LoxFunction> methods;
  private final Map<String, LoxFunction> classMethods;

  LoxClass(String name, LoxClass superclass, Map<String, LoxFunction> methods, 
          Map<String, LoxFunction> classMethods) {
    super(null);
    this.superclass = superclass;
    this.name = name;
    this.methods = methods;
    this.classMethods = classMethods;

    for (Map.Entry<String, LoxFunction> entry : methods.entrySet()) {
      entry.setValue(entry.getValue().withOwner(this));
    }
  }

  @Override 
  Object get(Token name, Interpreter interpreter) {
    if (classMethods.containsKey(name.lexeme)) {
      return classMethods.get(name.lexeme).bind(this);
    }
    return super.get(name, interpreter);
  }

  LoxFunction findMethod(String name) {
    if (superclass != null) {
      LoxFunction method = superclass.findMethod(name);

      if (method != null) {
        return method;
      }
    }

    if (methods.containsKey(name)) {
      return methods.get(name);
    }

    return null;
  }

  @Override
  public String toString() {
    return name;
  }

  LoxFunction findInnerMethod(LoxClass owner, String name) {
    if (this == owner) {
        return null;
    }

    if (superclass != null) {
        LoxFunction method = superclass.findInnerMethod(owner, name);

        if (method != null) {
            return method;
        }
    }

    if (methods.containsKey(name)) {
        return methods.get(name);
    }

    return null;
  }

   @Override
  public Object call(Interpreter interpreter,
                     List<Object> arguments) {
    LoxInstance instance = new LoxInstance(this);
    LoxFunction initializer = findMethod("init");
    if (initializer != null) {
      initializer.bind(instance).call(interpreter, arguments);
    }
    return instance;
  }

  @Override
  public int arity() {
    LoxFunction initializer = findMethod("init");
    if (initializer == null) return 0;
    return initializer.arity();
  }
  
}
