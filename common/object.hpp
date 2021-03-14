#pragma once

#include <defines.h>
#include <string>
#include <vector>


namespace object {


struct value_t {
    enum kind_t {
        VALUE_UNDEFINED,
        VALUE_NULL,
        VALUE_INTEGER,
        VALUE_FLOATING,
        VALUE_STRING,
        VALUE_BOOLEAN,
        VALUE_OBJECT,
        VALUE_LIST,
    };

    kind_t kind = VALUE_UNDEFINED;

    virtual ~value_t () = default;

    bool is_null     () const { return kind == VALUE_NULL; }
    bool is_integer  () const { return kind == VALUE_INTEGER; }
    bool is_floating () const { return kind == VALUE_FLOATING; }
    bool is_string   () const { return kind == VALUE_STRING; }
    bool is_boolean  () const { return kind == VALUE_BOOLEAN; }
    bool is_object   () const { return kind == VALUE_OBJECT; }
    bool is_list     () const { return kind == VALUE_LIST; }

    virtual void print () const = 0;
};


struct null_t : public value_t {
    null_t () { kind = VALUE_NULL; }

    virtual void print () const override;
};


struct integer_t : public value_t {
    i64 value = 0;

    integer_t (i64 val) : value(val) { kind = VALUE_INTEGER; }

    virtual void print () const override;
};


struct floating_t : public value_t {
    f64 value = 0.;

    floating_t (f64 val) : value(val) { kind = VALUE_FLOATING; }

    virtual void print () const override;
};


struct string_t : public value_t {
    std::string value;

    string_t (std::string val) : value(std::move(val)) { kind = VALUE_STRING; }

    virtual void print () const override;
};


struct boolean_t : public value_t {
    bool value = false;

    boolean_t (bool val) : value(val) { kind = VALUE_BOOLEAN; }

    virtual void print () const override;
};


struct object_t : public value_t {
    std::vector<std::string> keys;
    std::vector<value_t*> values;

    object_t () { kind = VALUE_OBJECT; }

    ~object_t () {
        for (value_t* p : values) {
            if (p) delete p;
        }
    }

    void add (const std::string& key, value_t* value) {
        keys.push_back(key);
        values.push_back(value);
    }

    virtual void print () const override;
};


struct list_t : public value_t {
    std::vector<value_t*> values;

    list_t () { kind = VALUE_LIST; }

    ~list_t () {
        for (value_t* p : values) {
            if (p) delete p;
        }
    }

    void add (value_t* value) {
        values.push_back(value);
    }

    virtual void print () const override;
};


} // object

