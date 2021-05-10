#ifndef SON_VALUE_HPP
#define SON_VALUE_HPP

#include <stdint.h>
#include <initializer_list>
#include <tuple>


namespace SON {


enum class type_t : uint8_t {
    null,
    boolean,
    integer,
    floating,
    string,
    object,
    array,
    // @todo: consider making user-defined value types,
    //        along with providing means to accept functions to parse them.
    // custom,
};


class Value {
private:
    union value_holder_t {
        bool     boolean;
        uint64_t integer;
        double   floating;
        void*    value_storage; // Pointer to the string, object or array.
    } value_;

    type_t type_;

public:
    Value (); // Makes NULL value.
    Value (type_t); // Default value of that type.
    Value (std::nullptr_t);
    Value (bool);
    Value (int32_t);
    Value (uint64_t);
    Value (double);
    Value (const char*);

    Value (std::initializer_list<Value> init);
    Value (std::initializer_list<std::pair<const char*, Value>> init);

    Value (const Value&);
    Value (Value&&) noexcept;
    ~Value ();

    Value& operator = (const Value&);
    Value& operator = (Value&&) noexcept;

    type_t get_type () const;

    bool is_null () const;
    bool is_boolean () const;
    bool is_integer () const;
    bool is_floating () const;
    bool is_number () const;
    bool is_string () const;
    bool is_object () const;
    bool is_array () const;

    uint64_t get_integer () const;
    bool get_boolean () const;
    double get_floating () const;

    void swap (Value&) noexcept;

    bool operator == (const Value&) const;
    bool operator != (const Value&) const;

    Value& operator [] (const char* key);
    Value& operator [] (int32_t index);

    Value get (const char* key, const Value& default_);
    Value get (int32_t index, const Value& default_);

    void push (const char* key, const Value& value);
    void push (const Value&);

    class iterator {
        friend class Value;

        Value* p_value = nullptr;
        uint64_t index = 0;
        uint64_t size = 0;

        iterator (Value* p_value_, uint64_t index_, uint64_t size_);

    public:
        void operator ++ ();
        void operator -- ();
        bool operator == (const iterator& other) const;
        bool operator != (const iterator& other) const;

        Value& operator * () const;
    };

    class const_iterator {
       friend class Value;

       const Value* p_value = nullptr;
       uint64_t index = 0;
       uint64_t size = 0;

       const_iterator (const Value* p_value_, uint64_t index_, uint64_t size_);

    public:
       void operator ++ ();
       void operator -- ();
       bool operator == (const const_iterator& other) const;
       bool operator != (const const_iterator& other) const;

       const Value& operator * () const;
    };

    iterator begin ();
    iterator end ();

    const_iterator begin () const;
    const_iterator end () const;

    void* get_storage () const;
};


Value make_object ();
Value make_array ();


} // SON


#endif // SON_VALUE_HPP
