#include "abstract_object_parser.hpp"
#include "reader.hpp"
#include <service/storage.hpp>


namespace service {


struct token {
    enum kind_t {
        TOKEN_UNDEFINED = 0,

        TOKEN_EQUAL_SIGN = '=',
        TOKEN_SEMICOLON = ';',

        TOKEN_BRACE_OPEN = '{',
        TOKEN_BRACE_CLOSE = '}',

        TOKEN_PAREN_OPEN = '(',
        TOKEN_PAREN_CLOSE = ')',

        TOKEN_BRACKET_OPEN = '[',
        TOKEN_BRACKET_CLOSE = ']',

        TOKEN_NULL = 300,
        TOKEN_INTEGER,
        TOKEN_FLOATING,
        TOKEN_BOOLEAN,
        TOKEN_STRING,

        TOKEN_EOF,
    };

    static const char* to_string(kind_t k) {
        switch (k) {
            case TOKEN_UNDEFINED: return "TOKEN_UNDEFINED";

            case TOKEN_EQUAL_SIGN: return "TOKEN_EQUAL_SIGN";
            case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";

            case TOKEN_BRACE_OPEN: return "TOKEN_BRACE_OPEN";
            case TOKEN_BRACE_CLOSE: return "TOKEN_BRACE_CLOSE";

            case TOKEN_PAREN_OPEN: return "TOKEN_PAREN_OPEN";
            case TOKEN_PAREN_CLOSE: return "TOKEN_PAREN_CLOSE";

            case TOKEN_BRACKET_OPEN: return "TOKEN_BRACKET_OPEN";
            case TOKEN_BRACKET_CLOSE: return "TOKEN_BRACKET_CLOSE";

            case TOKEN_NULL: return "TOKEN_NULL";
            case TOKEN_INTEGER: return "TOKEN_INTEGER";
            case TOKEN_FLOATING: return "TOKEN_FLOATING";
            case TOKEN_BOOLEAN: return "TOKEN_BOOLEAN";
            case TOKEN_STRING: return "TOKEN_STRING";

            case TOKEN_EOF: return "TOKEN_EOF";
        }

        return "ERROR";
    }

    union value_t {
        i64 integer;
        f64 floating;
        bool boolean;
    };

    const char* begin;
    size_t length;
    kind_t kind;
    value_t value;
};


void print(token t) {
    printf("token { kind = %20s; value = ", token::to_string(t.kind));
    switch (t.kind) {
        case token::TOKEN_INTEGER: 
            printf("%ld; }\n", t.value.integer); 
            break;
        case token::TOKEN_FLOATING:
            printf("%lf; }\n", t.value.floating);
            break;
        case token::TOKEN_BOOLEAN:
            printf("%s; }\n", t.value.boolean ? "true" : "false");
            break;
        case token::TOKEN_NULL:
            printf("null; }\n");
            break;
        case token::TOKEN_STRING:
            printf("\"%.*s\"; }\n", i32(t.length), t.begin);
            break;
        case token::TOKEN_EQUAL_SIGN:
        case token::TOKEN_SEMICOLON:
        case token::TOKEN_BRACE_OPEN:
        case token::TOKEN_BRACE_CLOSE:
        case token::TOKEN_PAREN_OPEN:
        case token::TOKEN_PAREN_CLOSE:
        case token::TOKEN_BRACKET_OPEN:
        case token::TOKEN_BRACKET_CLOSE:
            printf("'%c'; }\n", char(t.kind));
            break;
        default:
            printf("??? }\n");
    }
}


bool is_double_quote (char c) { return c == '"'; }


struct lexer {
    reader r;

    void initialize (const char* text, size_t size) {
        r.initialize(text, size);
    }

    void terminate () {
        r.terminate();
    }

    void run () {
        char c;
        while (r.skip_spaces(), (c = r.get_char()) != 0) {
            if (c == '{' ||
                c == '}' ||
                c == '(' ||
                c == ')' ||
                c == '[' ||
                c == ']' ||
                c == '=' ||
                c == ';')
            {
                token t;
                t.begin = r.current;
                t.length = 1;
                t.kind = token::kind_t(c);
                t.value.integer = 0;

                r.skip_char();

                print(t);
                continue;
            } else if (c == '\"') {
                // read string
                
                r.skip_char(); // eat double quote
                reader::result_t result = r.eat_until(is_double_quote);
                if (result) {
                    r.skip_char(); // eat double quote

                    token t;
                    t.begin = result.start;
                    t.length = result.length;
                    t.kind = token::TOKEN_STRING;
                    t.value.integer = 0;

                    print(t);
                    continue;
                }
            } else if (is_digit(c)) {
                // read number, integer or float is unknown
                printf("Number\n");
                continue;
            } else if (is_alpha(c)) {
                //
                // !!! IMPORTANT:
                // 
                // I have to fix this next time, now it's reversed:
                // Lexer reads strings equal to keywords, but have to read identifiers,
                // and THEN compare them to the predefined keywords. Hashmap<string->token::kind_t> could be used.
                //
                
                reader::result_t result;
                result = r.eat_string("true");

                if (result) {
                    token t;
                    t.begin = result.start;
                    t.length = result.length;
                    t.kind = token::TOKEN_BOOLEAN;
                    t.value.boolean = true;

                    print(t);
                    continue;
                }

                result = r.eat_string("false");

                if (result) {
                    token t;
                    t.begin = result.start;
                    t.length = result.length;
                    t.kind = token::TOKEN_BOOLEAN;
                    t.value.boolean = false;

                    print(t);
                    continue;
                }

                result = r.eat_string("null");

                if (result) {
                    token t;
                    t.begin = result.start;
                    t.length = result.length;
                    t.kind = token::TOKEN_NULL;
                    t.value.integer = 0;

                    print(t);
                    continue;
                }
            }

            auto result = r.eat_until(is_space);
            printf("Parse Error! Unknown lexeme: %.*s\n", i32(result.length), result.start);
            break;
        }
    }
};


struct abstract_object_parser_impl {
    const char* text;
    size_t size;

    void initialize (const char* text_, size_t n) {
        text = text_;
        size = n;
    }

    void terminate () {}

    void parse () {
        lexer l;
        l.initialize(text, size);

        l.run();

        l.terminate();
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
    impl->parse();
    return abstract_object();
}


}
