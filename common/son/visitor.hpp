#pragma once

#include <defines.h>
#include <cstring>
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

    Object* scheme = nullptr;
    List* values = nullptr;

    bool in_list = false;

methods:
    virtual void visit (IValue* value) override {
        Super::visit(value);
    }

    inline void visit_value (IValue* value) {
        if (not value) return;

        Object* entry = new Object();
        entry->emplace("key", new Null());
        entry->emplace("type", new String(to_string(value->get_kind())));

        values->emplace(entry);
    }

    virtual void visit (Null* value)     override { visit_value(value); }
    virtual void visit (Boolean* value)  override { visit_value(value); }
    virtual void visit (Integer* value)  override { visit_value(value); }
    virtual void visit (Floating* value) override { visit_value(value); }
    virtual void visit (String* value)   override { visit_value(value); }

    virtual void visit (const std::string& key, IValue* value) override {
        if (not value) return;
        if (not values) return;

        IValue::Kind kind = value->get_kind();

        Object* entry = new Object();
        entry->emplace("key", new String(key));
        entry->emplace("type", new String(to_string(kind)));

        if (kind == IValue::VALUE_OBJECT or kind == IValue::VALUE_LIST) {
            Object* upper_scheme = scheme;
            List* upper_values = values;
            values = new List();

            in_list = true;
            value->visit(this);

            entry->emplace("values", values);
            values = upper_values;
            scheme = upper_scheme;
        }

        values->emplace(entry);
    }

    virtual void visit (Object* value) override {
        if (not value) return;

        Object* entry = new Object();
        entry->emplace("key", new Null());
        entry->emplace("type", new String(to_string(value->get_kind())));

        bool upper_in_list = in_list;
        in_list = false;
        List* upper_values = values;
        values = new List();

        value->visit(this);

        entry->emplace("values", values);
        values = upper_values;
        in_list = upper_in_list;

        if (in_list) {
            values->emplace(entry);
        } else {
            scheme = entry;
        }
    }

    virtual void visit (List* value) override {
        if (not value) return;

        Object* entry = new Object();
        entry->emplace("key", new Null());
        entry->emplace("type", new String(to_string(value->get_kind())));

        bool upper_in_list = in_list;
        in_list = true;

        List* upper_values = values;
        values = new List();

        value->visit(this);

        entry->emplace("values", values);

        values = upper_values;
        in_list = upper_in_list;

        if (in_list) {
            values->emplace(entry);
        } else {
            scheme = entry;
        }
    }
};


struct VisitorIntoCpp : public IVisitor {
    using Super = IVisitor;

    int indent_size = 4;
    int depth = 0;

    FILE* output = stdout;
    const char* spaces = "                                                  "; // 50 spaces
    const int max_spaces = 50;

    bool top_level = true;
    bool new_lined = false;

methods:
#define print_(...) \
    { indent(); fprintf(output, __VA_ARGS__); new_line(); } void(0)

    inline void new_line () {
        fprintf(output, "\n");
        new_lined = true;
    }

    inline void indent () {
        if (new_lined) {
            fprintf(output, "%.*s", depth * indent_size > max_spaces ? max_spaces : depth * indent_size, spaces);
        }
    }

    void visit (IValue* value) override {
        Super::visit(value);
    }

    void print_struct (List* members) {
        if (not members) return;

        if (top_level) {
            print_("struct config {");
        } else {
            print_("struct {");
        }
        top_level = false;

        depth += 1;
        members->visit(this);
        depth -= 1;

        indent();
        fprintf(output, "}");
    }

    void print_struct_member (Object* value) {
        String* type = value->get("type")->as_string();
        if (not type) {
            fprintf(output, "Object value have no \"type\" member!\n");
            return;
        }

        String* key = value->get("key")->as_string();
        if (type->value == "object") {
            print_struct(value->get("values")->as_list());
        } else {
            indent();
            fprintf(output, "%s", son_to_cpp_type(type->value.c_str()));
        }
        if (key) {            
            fprintf(output, " %s;", key->value.c_str());
        } else {
            fprintf(output, ";");
        }
        new_line();
    }

    const char* son_to_cpp_type(const char* s) {
        if (strcmp(s, "boolean") == 0)  return "bool";
        if (strcmp(s, "integer") == 0)  return "int";
        if (strcmp(s, "floating") == 0) return "float";
        if (strcmp(s, "string") == 0)   return "const char*";

        return "unknown";
    }

    void visit (Null* value)     override {}
    void visit (Boolean* value)  override {}
    void visit (Integer* value)  override {}
    void visit (Floating* value) override {}
    void visit (String* value)   override {}

    void visit (const std::string& key, IValue* value) override {}

    void visit (Object* value) override {
        String* type = value->get("type")->as_string();
        if (not type) {
            printf("Object value have no \"type\" member!\n");
            return;
        }

        bool top = top_level;

        // HPP
        if (top) {
            fprintf(output, "#pragma once\n\n");
        }

        print_struct_member(value);

        if (top) {
            fprintf(output, "\nconst config& get_config();\n");
        }

        // CPP
        if (top) {
            fprintf(output, 
                "\n"
                "static config* instance;\n"
                "\n"
                "const config& get_config {\n"
                "    if (instance) return *instance;\n"
                "\n"
                "    instance = new config();\n"
                "}\n"
                "\n"
                ""
                );
        }
    }

    void visit (List* value) override {
        print_("We do not support lists yet.");
    }
};


} // SON
