#pragma once

#include <defines.h>
#include <string>
#include <vector>


namespace SON {


namespace ugly {
    using string = std::string;

    template <typename T>
    using dynamic_array = std::vector<T>;
}


struct IValue;
struct Null;
struct Boolean;
struct Integer;
struct Floating;
struct String;
struct Object;
struct List;
struct IVisitor;

struct IValue {
    enum Kind {
        VALUE_UNDEFINED = 0,
        VALUE_NULL,
        VALUE_BOOLEAN,
        VALUE_INTEGER,
        VALUE_FLOATING,
        VALUE_STRING,
        VALUE_OBJECT,
        VALUE_LIST,
        VALUE_ENUM_SIZE,
    };

    virtual ~IValue () = default;

    virtual Kind get_kind () const = 0;
    virtual void visit (IVisitor* visitor) = 0;
    virtual bool equal_to(const IValue*) const = 0;
};


struct IVisitor {
    virtual ~IVisitor() = default;

    virtual void visit (IValue* value) {
        switch (value->get_kind()) {
            case IValue::VALUE_NULL:     this->visit((Null*)value); break;
            case IValue::VALUE_BOOLEAN:  this->visit((Boolean*)value); break;
            case IValue::VALUE_INTEGER:  this->visit((Integer*)value); break;
            case IValue::VALUE_FLOATING: this->visit((Floating*)value); break;
            case IValue::VALUE_STRING:   this->visit((String*)value); break;
            case IValue::VALUE_OBJECT:   this->visit((Object*)value); break;
            case IValue::VALUE_LIST:     this->visit((List*)value); break;
            default: return;
        }
    }

    virtual void visit (const std::string&, IValue* value) = 0;

    virtual void visit (Null*) = 0;
    virtual void visit (Boolean*) = 0;
    virtual void visit (Integer*) = 0;
    virtual void visit (Floating*) = 0;
    virtual void visit (String*) = 0;
    virtual void visit (Object*) = 0;
    virtual void visit (List*) = 0;
};


const char* to_string (IValue::Kind k);


struct Null : public IValue {
    virtual Kind get_kind () const override { return VALUE_NULL; }
    virtual void visit (IVisitor* visitor) override { visitor->visit(this); }
    virtual bool equal_to(const IValue* other) const override { return other->get_kind() == this->get_kind(); }
};


struct Boolean : public IValue {
    bool value;

    Boolean (bool v) : value(v) {}

    virtual Kind get_kind () const override { return VALUE_BOOLEAN; }
    virtual void visit (IVisitor* visitor) override { visitor->visit(this); }
    virtual bool equal_to(const IValue* other) const override {
        if (other->get_kind() != this->get_kind()) return false;

        const Boolean* v = (const Boolean*)other;
        return value == v->value;
    }
};


struct Integer : public IValue {
    i64 value;

    Integer (i64 v) : value(v) {}

    virtual Kind get_kind () const override { return VALUE_INTEGER; }
    virtual void visit (IVisitor* visitor) override { visitor->visit(this); }
    virtual bool equal_to(const IValue* other) const override {
        if (other->get_kind() != this->get_kind()) return false;

        const Integer* v = (const Integer*)other;
        return value == v->value;
    }
};


struct Floating : public IValue {
    f64 value;

    Floating (f64 v) : value(v) {}

    virtual Kind get_kind () const override { return VALUE_FLOATING; }
    virtual void visit (IVisitor* visitor) override { visitor->visit(this); }
    virtual bool equal_to(const IValue* other) const override {
        if (other->get_kind() != this->get_kind()) return false;

        const Floating* v = (const Floating*)other;
        return value == v->value;
    }
};


struct String : public IValue {
    ugly::string value;

    String (ugly::string v) : value(std::move(v)) {}

    virtual Kind get_kind () const override { return VALUE_STRING; }
    virtual void visit (IVisitor* visitor) override { visitor->visit(this); }
    virtual bool equal_to(const IValue* other) const override {
        if (other->get_kind() != this->get_kind()) return false;

        const String* v = (const String*)other;
        return value == v->value;
    }
};


struct Object : public IValue {
    ugly::dynamic_array<ugly::string> keys;
    ugly::dynamic_array<IValue*> values;

    virtual Kind get_kind () const override { return VALUE_OBJECT; }
    virtual bool equal_to(const IValue* other) const override {
        if (other->get_kind() != this->get_kind()) return false;

        const Object* v = (const Object*)other;

        if (v->keys.size() != keys.size()) return false;

        for (size_t i = 0; i < keys.size(); i++) {
            if (keys[i] != v->keys[i]) return false;
            if (not (values[i]->equal_to(v->values[i]))) return false;
        }

        return true;
    }

    void visit (IVisitor* visitor) override {
        for (size_t i = 0; i < keys.size(); i++) {
            ugly::string& k = keys[i];
            IValue* v = values[i];

            visitor->visit(k, v);
        }
    }

    void emplace (ugly::string key, IValue* value) {
        keys.push_back(key);
        values.push_back(value);
    }
};


struct List : public IValue {
    ugly::dynamic_array<IValue*> values;

    virtual Kind get_kind () const override { return VALUE_LIST; }
    virtual bool equal_to(const IValue* other) const override {
        if (other->get_kind() != this->get_kind()) return false;

        const List* v = (const List*)other;

        if (values.size() != v->values.size()) return false;

        for (size_t i = 0; i < values.size(); i++) {
            if (not (values[i]->equal_to(v->values[i]))) return false;
        }

        return true;
    }

    void visit (IVisitor* visitor) override {
        for (auto& v : values) {
            visitor->visit(v);
        }
    }

    void emplace (IValue* value) {
        values.push_back(value);
    }
};


struct Value : public IValue {
    IValue* data = nullptr;

    virtual Kind get_kind () const override { return data->get_kind(); }
    virtual bool equal_to(const IValue* other) const override { return false; }
};


} // SON

