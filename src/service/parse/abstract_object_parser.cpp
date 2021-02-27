#include "abstract_object_parser.hpp"
#include "reader.hpp"
#include <service/storage.hpp>
#include <unordered_map>
#include <vector>


namespace service {


struct token {
    enum kind_t {
        TOKEN_UNDEFINED = 0,

        TOKEN_EQUAL_SIGN = '=',
        TOKEN_SEMICOLON = ';',
        TOKEN_COMMA = ',',

        TOKEN_BRACE_OPEN = '{',
        TOKEN_BRACE_CLOSE = '}',

        TOKEN_PAREN_OPEN = '(',
        TOKEN_PAREN_CLOSE = ')',

        TOKEN_BRACKET_OPEN = '[',
        TOKEN_BRACKET_CLOSE = ']',

        TOKEN_KW_NULL = 300,
        TOKEN_KW_TRUE,
        TOKEN_KW_FALSE,

        TOKEN_IDENTIFIER,
        TOKEN_INTEGER,
        TOKEN_FLOATING,
        TOKEN_STRING,

        TOKEN_EOF,
    };

    static const char* to_string(kind_t k) {
        switch (k) {
            case TOKEN_UNDEFINED: return "TOKEN_UNDEFINED";

            case TOKEN_EQUAL_SIGN: return "TOKEN_EQUAL_SIGN";
            case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
            case TOKEN_COMMA: return "TOKEN_COMMA";

            case TOKEN_BRACE_OPEN: return "TOKEN_BRACE_OPEN";
            case TOKEN_BRACE_CLOSE: return "TOKEN_BRACE_CLOSE";

            case TOKEN_PAREN_OPEN: return "TOKEN_PAREN_OPEN";
            case TOKEN_PAREN_CLOSE: return "TOKEN_PAREN_CLOSE";

            case TOKEN_BRACKET_OPEN: return "TOKEN_BRACKET_OPEN";
            case TOKEN_BRACKET_CLOSE: return "TOKEN_BRACKET_CLOSE";

            case TOKEN_KW_NULL: return "TOKEN_KW_NULL";
            case TOKEN_KW_TRUE: return "TOKEN_KW_TRUE";
            case TOKEN_KW_FALSE: return "TOKEN_KW_FALSE";

            case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
            case TOKEN_INTEGER: return "TOKEN_INTEGER";
            case TOKEN_FLOATING: return "TOKEN_FLOATING";
            case TOKEN_STRING: return "TOKEN_STRING";

            case TOKEN_EOF: return "TOKEN_EOF";
        }

        return "ERROR";
    }

    union value_t {
        i64 integer;
        f64 floating;
    };

    const char* begin;
    size_t length;
    kind_t kind;
    value_t value;
};


void print(token t) {
    printf("token { kind = %20s; value = ", token::to_string(t.kind));
    switch (t.kind) {
        case token::TOKEN_EOF:
            printf("EOF; }\n");
            break;
        case token::TOKEN_KW_NULL:
            printf("null; }\n");
            break;
        case token::TOKEN_KW_TRUE:
            printf("true; }\n");
            break;
        case token::TOKEN_KW_FALSE:
            printf("false; }\n");
            break;
        case token::TOKEN_INTEGER: 
            printf("%ld; }\n", t.value.integer); 
            break;
        case token::TOKEN_FLOATING:
            printf("%lf; }\n", t.value.floating);
            break;
        case token::TOKEN_IDENTIFIER:
        case token::TOKEN_STRING:
            printf("%.*s; }\n", i32(t.length), t.begin);
            break;
        case token::TOKEN_EQUAL_SIGN:
        case token::TOKEN_SEMICOLON:
        case token::TOKEN_COMMA:
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


static inline bool is_double_quote (char c) { return c == '"'; }
static inline bool is_valid_identifier_head (char c) { return is_alpha(c) || c == '_'; }
static inline bool is_valid_identifier_body (char c) { return is_digit(c) || is_alpha(c) || c == '_'; }


struct lexer {
    reader* r;
    std::unordered_map<std::string, token::kind_t> keywords;
    std::vector<token>* tokens;

    void initialize (reader* rdr) {
        r = rdr;

        keywords.emplace("null", token::TOKEN_KW_NULL);
        keywords.emplace("true", token::TOKEN_KW_TRUE);
        keywords.emplace("false", token::TOKEN_KW_FALSE);
    }

    void terminate () {}

    bool eat_quoted_string () {
        auto* checkpoint = r->current;
        u64 length = 0;

        char c = r->get_char();
        if (c != '"') {
            // No characters are eated yet. Return as normal.
            return false;
        }

        r->skip_char(); // Skip double quote.
        length += 1;
        
        while ((c = r->get_char()) != '"') {
            // Check for escape sequence.
            if (c == '\\') {
                // Skip two symbols, backslash and following escaped symbol.
                r->skip_char();
                r->skip_char();
                length += 2;
                continue;
            }

            // Newlines are not allowed to intercept string
            if (c == '\0' || c == '\n') {
                r->current = checkpoint;
                printf("ERROR: Unclosed double quote!\n");
                return false;
            }

            // If it is not an escape symbol, just skip is once;
            r->skip_char();
            length += 1;
        }

        if (c != '"') {
            r->current = checkpoint;
            printf("ERROR: Unclosed double quote!\n");
            return false;
        }

        r->skip_char(); // Skip double quote.
        length += 1;

        token t;
        t.begin = checkpoint;
        t.length = length;
        t.kind = token::TOKEN_STRING;
        t.value.integer = 0;

        print(t);
        return true;
    }

    bool eat_keyword_or_identifier () {
        char c = r->get_char();
        
        if (!is_valid_identifier_head(c)) {
            return false;
        }

        // This have to eat at least one symbol.
        auto result = r->eat_while(is_valid_identifier_body);

        auto token_kind = keywords.find(std::string(result.start, result.length));
        if (token_kind == keywords.end()) {
            // This is an identifier

            token t;
            t.begin = result.start;
            t.length = result.length;
            t.kind = token::TOKEN_IDENTIFIER;
            t.value.integer = 0;

            print(t);
            return true;
        }

        token t;
        t.begin = result.start;
        t.length = result.length;
        t.kind = token_kind->second;
        t.value.integer = 0;

        print(t);
        return true;
    }

    bool eat_number () {
        const char* start = r->current;
        u64 len = 0;

        char c = r->get_char();
        if (!is_digit(c) && c != '.' && c != '-' && c != '+') {
            return false;
        }

        i64 sign = 1;
        i64 integral = 0;
        f64 fractional = 0.;
        f64 multiplier = 1.;

        if (c == '-' || c == '+') {
            sign = (c == '-' ? -1 : 1);
            r->skip_char();
            len += 1;
        }

        while (is_digit(c = r->get_char())) {
            r->skip_char();
            len += 1;

            integral *= 10;
            integral += (c - '0');
        }

        if (c == '.') {
            r->skip_char();
            len += 1;

            while(is_digit(c = r->get_char())) {
                r->skip_char();
                len += 1;

                multiplier *= 0.1;
                fractional += (c - '0') * multiplier;
            }

            token t;
            t.begin = start;
            t.length = len;
            t.kind = token::TOKEN_FLOATING;
            t.value.floating = sign*(integral + fractional);

            print(t);
            return true;
        }

        token t;
        t.begin = start;
        t.length = len;
        t.kind = token::TOKEN_INTEGER;
        t.value.integer = sign*integral;

        print(t);
        return true;
    }

    void run () {
        char c;
        while (r->skip_spaces(), (c = r->get_char()) != 0) {
            if (c == '{' ||
                c == '}' ||
                c == '(' ||
                c == ')' ||
                c == '[' ||
                c == ']' ||
                c == '=' ||
                c == ';' ||
                c == ',')
            {
                token t;
                t.begin = r->current;
                t.length = 1;
                t.kind = token::kind_t(c);
                t.value.integer = 0;

                r->skip_char();

                print(t);
                continue;
            }
            else if (c == '\"') {
                bool successful = eat_quoted_string();
                if (successful) continue;
            }
            else if (is_digit(c) || (c == '.') || (c == '+') || (c == '-')) { // Read number, integer or float is unknown.
                bool successful = eat_number();
                if (successful) continue;
            }
            else if (is_alpha(c)) {
                bool successful = eat_keyword_or_identifier();
                if (successful) continue;
            }

            auto result = r->eat_until(is_space);
            printf("Parse Error! Unknown lexeme: %.*s\n", i32(result.length), result.start);
            break;
        }

        if (c == '\0') {
            token t;
            t.begin = r->current;
            t.length = 1;
            t.kind = token::TOKEN_EOF;
            t.value.integer = 0;

            print(t);
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
        reader r;
        lexer l;

        // @todo: remove std away
        std::vector<token> tokens;

        r.initialize(text, size);
        l.initialize(&r);

        l.run();

        l.terminate();
        r.terminate();
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
