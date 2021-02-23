#include "abstract_object_parser.hpp"
#include "lexer.hpp"
#include <service/storage.hpp>


namespace service {


struct token {
    enum kind_t {
        UNDEFINED,
        TOKEN_EOF,

        TOKEN_NULL,
        TOKEN_INTEGER,
        TOKEN_FLOATING,
        TOKEN_BOOLEAN,
        TOKEN_STRING,

        TOKEN_EQUAL_SIGN,
        TOKEN_SEMICOLON,

        TOKEN_BRACE_OPEN,
        TOKEN_BRACE_CLOSE,

        TOKEN_PAREN_OPEN,
        TOKEN_PAREN_CLOSE,

        TOKEN_BRACKET_OPEN,
        TOKEN_BRACKET_CLOSE,
    };

    union value_t {
        i64 integer;
        f64 floating;
        bool boolean;
    };

    const char* begin;
    const char* end;
    kind_t kind;
    value_t value;
};


struct abstract_object_parser_impl {
    const char* text;
    size_t size;
    lexer lex;

    storage<token> tokens;


    void initialize (const char* text_, size_t size_) {
        text = text_;
        size = size_;

        lex.initialize(text, size);
    }


    void terminate () {}


    bool parse_null () {
        const char null_string[] = "null";
        size_t size = sizeof(null_string);

        const char* start = lex.eat_string(null_string, size);

        if (start == nullptr) { return false; }

        token t;
        t.begin = start;
        t.end = lex.current;
        t.kind = token::TOKEN_NULL;

        tokens.push(t);
        return true;
    }


    bool parse_boolean () {
        const char true_string[] = "true";
        const char false_string[] = "false";

        {
            const char* start = lex.eat_string(true_string, sizeof(true_string));

            if (start != nullptr) {
                token t;
                t.begin = start;
                t.end = lex.current;
                t.kind = token::TOKEN_BOOLEAN;
                t.value.boolean = true;

                tokens.push(t);
                return true;
            }
        }

        {
            const char* start = lex.eat_string(false_string, sizeof(false_string));

            if (start != nullptr) {
                token t;
                t.begin = start;
                t.end = lex.current;
                t.kind = token::TOKEN_BOOLEAN;
                t.value.boolean = false;

                tokens.push(t);
                return true;
            }            
        }

        return false;
    }


    bool parse_one_character_token (token::kind_t kind, char ch) {
        char c = lex.get_char();

        if (c != ch) { return false; }

        token t;
        t.begin = lex.current;
        t.end = lex.current + 1;
        t.kind = kind;

        tokens.push(t);

        lex.skip_char();
        return true;
    }

    bool parse_equal_sign    () { return parse_one_character_token(token::TOKEN_EQUAL_SIGN, '='); }
    bool parse_semicolon     () { return parse_one_character_token(token::TOKEN_SEMICOLON, ';'); }
    bool parse_brace_open    () { return parse_one_character_token(token::TOKEN_BRACE_OPEN, '{'); }
    bool parse_brace_close   () { return parse_one_character_token(token::TOKEN_BRACE_CLOSE, '}'); }
    bool parse_paren_open    () { return parse_one_character_token(token::TOKEN_PAREN_OPEN, '('); }
    bool parse_paren_close   () { return parse_one_character_token(token::TOKEN_PAREN_CLOSE, ')'); }
    bool parse_bracket_open  () { return parse_one_character_token(token::TOKEN_BRACKET_OPEN, '['); }
    bool parse_bracket_close () { return parse_one_character_token(token::TOKEN_BRACKET_CLOSE, ']'); }


    bool parse_string () {
        char c = lex.get_char();
        if (c != '\"') {
            return false;
        }

        lex.skip_char();

        while (c = lex.eat_char()) {
            if (c == '\"') {
                
            }
        }
    }

    value_object* parse_object () {
        if (!parse_brace_open()) {
            return nullptr;
        }

        auto* object = new value_object();



        return nullptr;
    }

    abstract_object parse () {
        return abstract_object();
    }
};


void abstract_object_parser::initialize (const char* text, size_t size) {
    impl = new abstract_object_parser_impl();
    impl->initialize(text, size);
}


void abstract_object_parser::terminate () {
    impl->terminate();
    delete impl;
}


abstract_object abstract_object_parser::parse () {
    return impl->parse();
}


}
