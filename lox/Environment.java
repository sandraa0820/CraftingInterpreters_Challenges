package lox;

import java.util.HashMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.List;

class Environment {
    static final Object UNINITIALIZED = new Object();
    final Environment enclosing;
    private final Map<String, Object> values = new HashMap<>();
    private final List<Object> localValues = new ArrayList<>();

    Environment() {
        enclosing = null;
    }

    Environment(Environment enclosing) {
        this.enclosing = enclosing;
    }

    Object get(Token name) {
        if (values.containsKey(name.lexeme)) {
        Object value = values.get(name.lexeme);

        if (value == UNINITIALIZED) {
            throw new RuntimeError(name, "variable '" + name.lexeme + "' has not been initialize");
        }

        return value;
    }

        if (enclosing != null) return enclosing.get(name);

        throw new RuntimeError(name,
            "Undefined variable '" + name.lexeme + "'.");
        }

    void assign(Token name, Object value) {
        if (values.containsKey(name.lexeme)) {
            values.put(name.lexeme, value);
            return;
        }

        if (enclosing != null) {
            enclosing.assign(name, value);
            return;
        }

        throw new RuntimeError(name,
            "Undefined variable '" + name.lexeme + "'.");
    }

    Environment ancestor(int distance) {
        Environment environment = this;
        for (int i = 0; i < distance; i++) {
            environment = environment.enclosing; 
        }

        return environment;
    }

    void define(String name, Object value) {
        values.put(name, value);
    }

    // this will work better for challenge ch11 #4
    void define(int index, Object value) {
        while (localValues.size() <= index) {
            localValues.add(null);
        }
        localValues.set(index, value);
    }

    // name-based version
    Object getAt(int distance, String name) {
        return ancestor(distance).values.get(name);
    }

    // Overload version for ch.11 #4
    Object getAt(int distance, int index) {
        return ancestor(distance).localValues.get(index);
    }
    
    void assignAt(int distance, Token name, Object value) {
        ancestor(distance).values.put(name.lexeme, value);
    }

    //overload version
    void assignAt(int distance, int index, Object value) {
        ancestor(distance).localValues.set(index, value);
    }
}
