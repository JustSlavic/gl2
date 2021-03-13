#pragma once

#include <defines.h>
#include <string>
#include <vector>


namespace object {


struct value_t {
    enum kind_t {
        UNDEFINED,
        NULL_VALUE,
        INTEGER,
        FLOATING,
        STRING,
        BOOLEAN,
        OBJECT,
        LIST,
    };

    kind_t kind = UNDEFINED;

    virtual ~value_t () = default;

    bool is_null     () const { return kind == NULL_VALUE; }
    bool is_integer  () const { return kind == INTEGER; }
    bool is_floating () const { return kind == FLOATING; }
    bool is_string   () const { return kind == STRING; }
    bool is_boolean  () const { return kind == BOOLEAN; }
    bool is_object   () const { return kind == OBJECT; }
    bool is_list     () const { return kind == LIST; }

    virtual void print () const = 0;
};


struct null_t : public value_t {
    null_t () = default;

    virtual void print () const override;
};


struct integer_t : public value_t {
    i64 value = 0;

    integer_t (i64 val) : value(val) {}

    virtual void print () const override;
};


struct floating_t : public value_t {
    f64 value = 0.;

    floating_t (f64 val) : value(val) {}

    virtual void print () const override;
};


struct string_t : public value_t {
    std::string value;

    string_t (std::string val) : value(std::move(val)) {}

    virtual void print () const override;
};


struct boolean_t : public value_t {
    bool value = false;

    boolean_t (bool val) : value(val) {}

    virtual void print () const override;
};

struct object_t : public value_t {
    std::vector<std::string> keys;
    std::vector<value_t*> values;

    object_t () = default;

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

    list_t () = default;

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

