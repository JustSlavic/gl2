#include "abstract_object_parser.hpp"
#include "reader.hpp"
#include <service/storage.hpp>
#include <unordered_map>
#include <vector>


namespace service {

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

    TOKEN_KW_NULL = 256,
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

struct token {
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
    printf("token { kind = %20s; value = ", to_string(t.kind));
    switch (t.kind) {
        case TOKEN_EOF:
            printf("EOF; }\n");
            break;
        case TOKEN_KW_NULL:
            printf("null; }\n");
            break;
        case TOKEN_KW_TRUE:
            printf("true; }\n");
            break;
        case TOKEN_KW_FALSE:
            printf("false; }\n");
            break;
        case TOKEN_INTEGER: 
            printf("%ld; }\n", t.value.integer); 
            break;
        case TOKEN_FLOATING:
            printf("%lf; }\n", t.value.floating);
            break;
        case TOKEN_IDENTIFIER:
        case TOKEN_STRING:
            printf("%.*s; }\n", i32(t.length), t.begin);
            break;
        case TOKEN_EQUAL_SIGN:
        case TOKEN_SEMICOLON:
        case TOKEN_COMMA:
        case TOKEN_BRACE_OPEN:
        case TOKEN_BRACE_CLOSE:
        case TOKEN_PAREN_OPEN:
        case TOKEN_PAREN_CLOSE:
        case TOKEN_BRACKET_OPEN:
        case TOKEN_BRACKET_CLOSE:
            printf("'%c'; }\n", char(t.kind));
            break;
        default:
            printf("??? }\n");
    }
}


static inline bool is_double_quote (char c) { return c == '"'; }
static inline bool is_valid_identifier_head (char c) { return is_alpha(c) || c == '_'; }
static inline bool is_valid_identifier_body (char c) { return is_digit(c) || is_alpha(c) || c == '_'; }


constexpr size_t BUFFER_LENGTH = 64;
struct bucket_t {
    bucket_t* previous = nullptr;
    bucket_t* next = nullptr;

    token* buffer = nullptr;
    size_t buffer_size = 0;
    size_t counter = 0;
};


struct iterator {
    bucket_t* bucket = nullptr;
    size_t idx = 0;

    operator bool () const {
        return bucket != nullptr && idx < bucket->counter;
    }

    token* get() const {
        if (bucket == nullptr) {
            return nullptr;
        }

        if (idx >= bucket->counter) {
            return nullptr;
        }

        return bucket->buffer + idx;
    }

    bool next() {
        if (bucket == nullptr) {
            // Iterator not valid.
            return false;
        }

        idx += 1;
        if (idx == bucket->counter && idx < BUFFER_LENGTH) {
            // Reached end of filled part of the bucket. Invalidate iterator and return.
            bucket = nullptr;
            return false;
        }

        if (idx == BUFFER_LENGTH) {
            // Reached end of the bucket, but it could be more in the next one.
            bucket = bucket->next;
            idx = 0;

            // If there's no next bucket => iterator is automatically invalidated.
            return bucket != nullptr;
        }

        // 'idx' is on valid memory
        return idx < bucket->counter;
    }

    bool prev() {
        if (bucket == nullptr) {
            return false;
        }

        // 'idx' is unsigned integer => should check before decreasing value
        if (idx == 0) {
            // We on the start of the bucket and should move to the previous one
            bucket = bucket->previous;
            idx = BUFFER_LENGTH - 1;

            // If there's no previous bucket => iterator is invalidated automatically
            return bucket != nullptr;
        }

        // idx > 0, can decrease safely
        idx -= 1;

        // Assume you cannot delete value from the bucket at all, => buckets always has previous values
        return true;
    }
};


bucket_t* make_new_bucket() {
    auto* bucket = new bucket_t();
    bucket->buffer = (token*) malloc(sizeof(token) * BUFFER_LENGTH);
    bucket->buffer_size = BUFFER_LENGTH;

    return bucket;
}


void bucket_push_token(bucket_t* bucket, token t) {
    ASSERT(bucket);

    size_t n = 0;

    while (bucket->next) {
        if (bucket->counter == BUFFER_LENGTH) {
            bucket = bucket->next;
            n += 1;
        }
    }

    if (bucket->counter == BUFFER_LENGTH) {
        bucket->next = make_new_bucket();
        bucket = bucket->next;

        n += 1;
    }

    // printf("Push token into it(bucket=%ld, it=%ld);\n", n, bucket->counter);

    new (bucket->buffer + bucket->counter++) token(t);

}

void free_all_buckets(bucket_t* bucket) {
    bucket_t* next_bucket = nullptr;

    size_t n = 0;
    while (bucket) {
        next_bucket = bucket->next;

        for (size_t i = 0; i < bucket->counter; i++) {
            bucket->buffer[i].~token();

            // printf("Destroy token in it(bucket=%ld, it=%ld);\n", n, i);
        }

        free(bucket->buffer);
        delete bucket;
        bucket = next_bucket;
        n += 1;
    }
}


struct parser {
    bucket_t* tokens = nullptr;
    iterator current;

    void push_token (token t) {
        bucket_push_token(tokens, t);
    }
};



struct lexer {
    reader* r = nullptr;
    parser* p = nullptr;
    std::unordered_map<std::string, kind_t> keywords;

    void initialize () {
        keywords.emplace("null", TOKEN_KW_NULL);
        keywords.emplace("true", TOKEN_KW_TRUE);
        keywords.emplace("false", TOKEN_KW_FALSE);
    }

    void terminate () {}

    // @make that escaped newlines do not show up in resulted string
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
        t.kind = TOKEN_STRING;
        t.value.integer = 0;

        p->push_token(t);
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
            t.kind = TOKEN_IDENTIFIER;
            t.value.integer = 0;

            p->push_token(t);
            return true;
        }

        token t;
        t.begin = result.start;
        t.length = result.length;
        t.kind = token_kind->second;
        t.value.integer = 0;

        p->push_token(t);
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
            t.kind = TOKEN_FLOATING;
            t.value.floating = sign*(integral + fractional);

            p->push_token(t);
            return true;
        }

        token t;
        t.begin = start;
        t.length = len;
        t.kind = TOKEN_INTEGER;
        t.value.integer = sign*integral;

        p->push_token(t);
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
                t.kind = kind_t(c);
                t.value.integer = 0;

                r->skip_char();

                p->push_token(t);
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
            else if (is_valid_identifier_head(c)) {
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
            t.kind = TOKEN_EOF;
            t.value.integer = 0;

            p->push_token(t);
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
        l.initialize();

        l.run();

        l.terminate();
        r.terminate();

        for (token& t : tokens) {
            print(t);

            if (t.kind == TOKEN_BRACE_OPEN) {
                parse_object(&tokens);
            }
        }
    }

    bool parse_object (std::vector<token>* tokens) {
        return false;
    }
};


void parser_report_error(iterator* it, kind_t expected) {
    ASSERT(it);
    token* t = it->get();
    if (t == nullptr) {
        printf("Unexpectedly reached end of token stream!\n");
        return;
    }

    printf("Parser error: expected %s, found %s\n", to_string(expected), to_string(t->kind));
}


bool parser_parse_object(iterator*, value_object*);
bool parser_parse_list_of_things (iterator*, value_list*);
bool parser_parse_key_value_pair (iterator* it, value_object* object) {
    ASSERT(it);

    iterator checkpoint = *it;

    //
    // Key-value pair is consists of the following tokens:
    //    1. identifier
    //    2. equal sign
    //    3. value (could be keywords, strings, numbers, other objects)
    //    4. semicolon
    //
    token* key = nullptr;
    value_base* value = nullptr;

    {
        // 1. Identifier
        token* t = it->get();
        if (t == nullptr || t->kind != TOKEN_IDENTIFIER) {
            *it = checkpoint;
            return false;
        }

        key = t;
        it->next();
    }

    {
        // 2. Equal sign
        token* t = it->get();
        if (t == nullptr || t->kind != TOKEN_EQUAL_SIGN) {
            *it = checkpoint;
            return false;
        }
        it->next();
    }

    {
        // 3. Value
        token* t = it->get();
        if (t == nullptr || (
            t->kind != TOKEN_KW_NULL &&
            t->kind != TOKEN_KW_TRUE &&
            t->kind != TOKEN_KW_FALSE &&
            t->kind != TOKEN_INTEGER &&
            t->kind != TOKEN_FLOATING &&
            t->kind != TOKEN_STRING &&
            t->kind != TOKEN_BRACE_OPEN &&
            t->kind != TOKEN_BRACKET_OPEN))
        {
            *it = checkpoint;
            return false;
        }

        // In case if value is an object
        if (t->kind == TOKEN_BRACE_OPEN) {
            auto* object = new value_object();            
            bool successful = parser_parse_object(it, object);
            if (not successful) {
                *it = checkpoint;
                delete object;
                return false;
            }

            value = object;
        } else if (t->kind == TOKEN_BRACKET_OPEN) {
            auto* list = new value_list();
            bool successful = parser_parse_list_of_things(it, list);
            if (not successful) {
                *it = checkpoint;
                delete list;
                return false;
            }

            value = list;
        } else {
            switch (t->kind) {
                case TOKEN_IDENTIFIER:
                case TOKEN_STRING:
                    value = new value_string(std::string(t->begin + 1, t->length - 2));
                    break;
                case TOKEN_INTEGER:
                    value = new value_integer(t->value.integer);
                    break;
                case TOKEN_FLOATING:
                    value = new value_float(t->value.floating);
                    break;
                case TOKEN_KW_NULL:
                    value = new value_null();
                    break;
                case TOKEN_KW_TRUE:
                    value = new value_boolean(true);
                    break;
                case TOKEN_KW_FALSE:
                    value = new value_boolean(false);
                    break;
                default:
                    ASSERT(false);
            }

            it->next();
        }
    }

    {
        // 4. Semicolon
        token* t = it->get();
        if (t == nullptr || t->kind != TOKEN_SEMICOLON) {
            *it = checkpoint;
            return false;
        }
        it->next();
    }

    // We reached the end => it's all good
    // printf("PARSED: { key: %.*s", key->length, key->begin);
    // printf(", value: ");
    // value->print();
    // printf(" }\n");
    object->add(std::string(key->begin, key->length), value);
    return true;
}


bool parser_parse_object (iterator* it, value_object* result) {
    ASSERT(it);
    iterator checkpoint = *it;

    {
        token* t = it->get();
        if (t == nullptr || t->kind != TOKEN_BRACE_OPEN) {
            *it = checkpoint;
            return false;
        }
    }

    it->next();

    {
        bool successful;
        do {
            successful = parser_parse_key_value_pair(it, result);
        } while (successful);
    }

    {
        token* t = it->get();
        if (t == nullptr || t->kind != TOKEN_BRACE_CLOSE) {
            *it = checkpoint;
            return false;
        }
    }

    it->next();
    return true;
}


bool parser_parse_list_of_things (iterator* it, value_list* result) {
    ASSERT(it);

    iterator checkpoint = *it;

    {
        token* t = it->get();
        if (t == nullptr || t->kind != TOKEN_BRACKET_OPEN) {
            *it = checkpoint;
            return false;
        }

        it->next();
    }

    do {
        {
            token* t = it->get();
            ASSERT(t);

            if (t->kind == TOKEN_BRACKET_CLOSE) {
                break;
            }

            switch (t->kind) {
                case TOKEN_BRACKET_CLOSE: break;
                case TOKEN_BRACE_OPEN: {
                    // This is an object
                    value_object* obj = new value_object();

                    bool successful = parser_parse_object(it, obj);
                    if (not successful) {
                        *it = checkpoint;
                        delete obj;
                        return false;
                    }

                    // add obj to list
                    result->add(obj);
                    break;
                }
                case TOKEN_KW_NULL:
                    result->add(new value_null());
                    it->next();
                    break;
                case TOKEN_KW_TRUE:
                    result->add(new value_boolean(true));
                    it->next();
                    break;
                case TOKEN_KW_FALSE:
                    result->add(new value_boolean(false));
                    it->next();
                    break;
                case TOKEN_INTEGER:
                    result->add(new value_integer(t->value.integer));
                    it->next();
                    break;
                case TOKEN_FLOATING:
                    result->add(new value_float(t->value.floating));
                    it->next();
                    break;
                case TOKEN_STRING:
                    result->add(new value_string(std::string(t->begin + 1, t->length - 2)));
                    it->next();
                    break;
                default:
                    *it = checkpoint;
                    return false;
            }            
        }

        {
            // Comma or end of the list.
            token* t = it->get();
            if (t == nullptr || (
                t->kind != TOKEN_COMMA &&
                t->kind != TOKEN_BRACKET_CLOSE))
            {
                *it = checkpoint;
                return false;
            }

            if (t->kind == TOKEN_BRACKET_CLOSE) {
                // List ended, exit the loop and let final read consume closed bracket.
                break;
            }            

            it->next();
        }
    } while (true);

    {
        // Consume closing bracket.
        token* t = it->get();
        if (t == nullptr || t->kind != TOKEN_BRACKET_CLOSE) {
            *it = checkpoint;
            return false;
        }
        it->next();
    }

    return true;
}


bool parser_initialize (parser* p, const char* text, size_t length) {
    p->tokens = make_new_bucket();
    
    reader r;
    lexer  l;

    r.initialize(text, length);

    l.initialize();
    l.r = &r;
    l.p = p;

    l.run();


    iterator it;
    it.bucket = p->tokens;
    it.idx = 0;

    while (it) {
        token* t = it.get();
        print(*t);
        it.next();
    }

    it.bucket = p->tokens;
    it.idx = 0;

    while (it) {
        token* t = it.get();

        if (t->kind == TOKEN_EOF) {
            printf("Reached EOF!\n");
            break;
        }

        auto* result = new value_object();
        bool successful = parser_parse_object(&it, result);
        if (not successful) {
            printf("Could not parse object!\n");
            delete result;
            break;
        }

        result->print();
        printf("\n");
        delete result;
    }

    printf("Parsing ends.\n");

    l.terminate();
    r.terminate();

    return false;
}


void abstract_object_parser::initialize (const char* text, size_t size) {
    auto* p_parser = new parser();

    parser_initialize(p_parser, text, size);

    free_all_buckets(p_parser->tokens);

    // p_parser->initialize(text, size);

    impl = (void*)p_parser;
}


void abstract_object_parser::terminate () {
    auto* p_parser = (parser*) impl;
    // p_parser->terminate();
    delete p_parser;
}


abstract_object abstract_object_parser::parse () {
    // auto* p_parser = (parser*) impl;

    // p_parser->parse();
    return abstract_object();
}


}

