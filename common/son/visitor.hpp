#pragma once

#include <defines.h>
#include "object.hpp"


namespace SON {


struct VisitorCounter : public IVisitor {
    using Super = IVisitor;

    int n = 0;

methods:
    void visit (IValue* value) override {
        Super::visit(value);
    }

    void visit (Null* value)     override { n += 1; }
    void visit (Boolean* value)  override { n += 1; }
    void visit (Integer* value)  override { n += 1; }
    void visit (Floating* value) override { n += 1; }
    void visit (String* value)   override { n += 1; }

    void visit (const std::string& key, IValue* value) override { n += 1; this->visit(value); }

    void visit (Object* value) override {
        value->visit(this);
    }

    void visit (List* value) override {
        value->visit(this);
    }
};



struct VisitorPrint : public IVisitor {
    using Super = IVisitor;

    enum Status {
        STATUS_NONE = 0,
        STATUS_IN_OBJECT,
        STATUS_IN_LIST,
    };

    FILE* output = stdout;
    const char* spaces = "                                                  "; // 50 spaces
    const int max_spaces = 50;
    
    Status status = STATUS_NONE;

    int indent_size = 2;
    int depth = 0;
    int n = 0; // number of already printed pairs or list elements

    bool multiline_auto = true;
    bool multiline = true;
    bool use_separator = true;
    bool new_lined = true;
    bool in_kv_pair = false;

methods:
#define print_(...) \
    { fprintf(output, __VA_ARGS__); new_lined = false; } void(0)

    inline void new_line () {
        if (multiline) {
            fprintf(output, "\n");
            new_lined = true;
        }
    }

    inline void indent () {
        if (multiline and new_lined) {
            print_("%.*s", depth * indent_size > max_spaces ? max_spaces : depth * indent_size, spaces);
        }
    }

    inline const char* get_separator () {
        switch (status) {
            case STATUS_IN_LIST:   return n > 0 ? multiline ? "," : ", " : ""; // if there's no elements before (n == 0), nothing to separate
            case STATUS_IN_OBJECT: return n > 0 ? multiline ? ";" : "; " : " ";
            default: return "";
        }
    }

    inline void separator () {
        if (use_separator) {
            print_("%s", get_separator());
        } else if (n > 0 or status == STATUS_IN_OBJECT) {
            print_(" ");
        }
    }

    void visit (IValue* value) override {
        if (not value) return;
        
        Super::visit(value);
    }

    void visit (Null* value) override {
        if (not value) return;
        
        if (not in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        in_kv_pair = false;
        print_("%s", to_string(value->get_kind()));
        n += 1;
    }

    void visit (Boolean* value) override {
        if (not value) return;
        
        if (not in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        in_kv_pair = false;
        print_("%s", value->value ? "true" : "false");
        n += 1;
    }

    void visit (Integer* value) override {
        if (not value) return;
        
        if (not in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        in_kv_pair = false;
        print_("%ld", value->value);
        n += 1;
    }

    void visit (Floating* value) override {
        if (not value) return;

        if (not in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        in_kv_pair = false;
        print_("%lf", value->value);
        n += 1;
    }

    void visit (String* value) override {
        if (not value) return;

        if (not in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        in_kv_pair = false;
        print_("\"%s\"", value->value.c_str());
        n += 1;
    }

    void visit (const std::string& key, IValue* value) override {
        if (not value) return;

        separator();
        new_line();
        indent();

        print_("%s = ", key.c_str());
        in_kv_pair = true;
        this->visit(value);
        n += 1;
    }

    void visit (Object* value) override {
        if (not value) return;

        if (not in_kv_pair) {
            separator();
            if (depth > 0) new_line();
            indent();
        }

        Status previous_status = status;
        bool previous_multiline_setting = multiline;
        int m = n;

        { // Count elements in the list to decide print it in one line or multiline
            VisitorCounter counter;
            counter.visit(value);

            if (multiline_auto) {
                if (counter.n > 6) {
                    multiline = true;
                } else {
                    multiline = false;
                }
            }
        }

        { // Entering the object
            print_("{");
            n = 0;
            status = STATUS_IN_OBJECT;
            in_kv_pair = false;
            depth += 1;
        }

        value->visit(this);

        { // Leaving the object
            depth -= 1; // The reason for depth being decreased BEFORE printing ']' is that I want it to be on same indent as '['
            if (n > 0) {
                separator();
                new_line();
                indent();
            }
            print_("}");
            n = m + 1;
            status = previous_status;
            multiline = previous_multiline_setting;
        }

        if (depth == 0) new_line();
    }

    void visit (List* value) override {
        if (not value) return;

        if (not in_kv_pair) {
            separator();
            if (depth > 0) new_line();
            indent();
        }

        Status previous_status = status;
        bool previous_multiline_setting = multiline;
        int m = n;

        { // Count elements in the list to decide print it in one line or multiline
            VisitorCounter counter;
            counter.visit(value);

            if (multiline_auto) {
                if (counter.n > 5) {
                    multiline = true;
                } else {
                    multiline = false;
                }
            }
        }

        { // Entering the list
            print_("[");
            n = 0;
            status = STATUS_IN_LIST;
            in_kv_pair = false;
            depth += 1;
        }

        value->visit(this);

        { // Leaving the list
            depth -= 1; // The reason for depth being decreased BEFORE printing ']' is that I want it to be on same indent as '['
            if (n > 0) {
                new_line();
                indent();
            }
            print_("]");
            n = m + 1;
            status = previous_status;
            multiline = previous_multiline_setting;
        }

        if (depth == 0) new_line();
    }
#undef print_
};


struct VisitorIntoScheme : public IVisitor {
    using Super = IVisitor;

    List* scheme = nullptr;

methods:
    // VisitorIntoScheme () : scheme(new List()) {} 

    virtual void visit (IValue* value) override {
        Super::visit(value);
    }

    inline void visit_value (IValue* value) {
        if (not value) return;

        String* kind = new String(to_string(value->get_kind()));

        if (not scheme) {
            scheme = new List();
        }

        scheme->emplace(kind);
    }

    virtual void visit (Null* value)     override { visit_value(value); }
    virtual void visit (Boolean* value)  override { visit_value(value); }
    virtual void visit (Integer* value)  override { visit_value(value); }
    virtual void visit (Floating* value) override { visit_value(value); }
    virtual void visit (String* value)   override { visit_value(value); }

    virtual void visit (const std::string& key, IValue* value) override {
        if (not value) return;

        IValue::Kind kind = value->get_kind();

        Object* entry = new Object();
        entry->emplace("key", new String(key));
        entry->emplace("type", new String(to_string(kind)));

        if (kind == IValue::VALUE_OBJECT || kind == IValue::VALUE_LIST) {
            List* upper = scheme;
            scheme = new List();

            value->visit(this);

            entry->emplace("values", scheme);
            scheme = upper;
        }

        scheme->emplace(entry);
    }

    virtual void visit (Object* value) override {
        if (not value) return;

        Object* entry = new Object();
        entry->emplace("key", new Null());
        entry->emplace("type", new String(to_string(value->get_kind())));

        List* upper = scheme;
        scheme = new List();

        value->visit(this);

        entry->emplace("values", scheme);
        scheme = upper;

        if (not scheme) {
            scheme = new List();
        }

        scheme->emplace(entry);
    }

    virtual void visit (List* value) override {
        if (not value) return;

        Object* entry = new Object();
        entry->emplace("key", new Null());
        entry->emplace("type", new String(to_string(value->get_kind())));

        List* upper = scheme;
        scheme = new List();

        value->visit(this);

        entry->emplace("values", scheme);
        scheme = upper;

        if (not scheme) {
            scheme = new List();
        }

        scheme->emplace(entry);
    }
};


} // SON
