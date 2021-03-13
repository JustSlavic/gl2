#pragma once

#include <defines.h>
#include <string>
#include <vector>


namespace service {


struct value_base {
    enum value_type {
        UNDEFINED,
        NULL_VALUE,
        INTEGER,
        FLOAT,
        STRING,
        BOOLEAN,
        OBJECT,
        LIST
    };

    value_type type = UNDEFINED;

    virtual ~value_base () = default;

    bool is_null    () const { return type == NULL_VALUE; }
    bool is_integer () const { return type == INTEGER; }
    bool is_float   () const { return type == FLOAT; }
    bool is_string  () const { return type == STRING; }
    bool is_boolean () const { return type == BOOLEAN; }
    bool is_object  () const { return type == OBJECT; }
    bool is_list    () const { return type == LIST; }

    virtual void print () const = 0;
};

struct value_null : public value_base {
    value_null () = default;

    virtual void print () const override;
};

struct value_integer : public value_base {
    i64 value = 0;

    value_integer (i64 val) : value(val) {}

    virtual void print () const override;
};

struct value_float : public value_base {
    f64 value = 0.;

    value_float (f64 val) : value(val) {}

    virtual void print () const override;
};


struct value_string : public value_base {
    std::string value;

    value_string (std::string val) : value(std::move(val)) {}

    virtual void print () const override;
};

struct value_boolean : public value_base {
    bool value = false;

    value_boolean (bool val) : value(val) {}

    virtual void print () const override;
};

struct value_object : public value_base {
    std::vector<std::string> keys;
    std::vector<value_base*> values;

    value_object () = default;

    ~value_object () {
        for (value_base* p : values) {
            if (p) delete p;
        }
    }

    void add (const std::string& key, value_base* value) {
        keys.push_back(key);
        values.push_back(value);
    }

    virtual void print () const override;
};

struct value_list : public value_base {
    std::vector<value_base*> values;

    value_list () = default;

    ~value_list () {
        for (value_base* p : values) {
            if (p) delete p;
        }
    }

    void add (value_base* value) {
        values.push_back(value);
    }

    virtual void print () const override;
};


struct abstract_object {
    value_base* value = nullptr;

    abstract_object () = default;
    ~abstract_object () { if (value) delete value; }

    bool is_null () const { return value == nullptr; }
    void set (value_base* val) { value = val; }

    void print () const;
};


} // service
