#include "parser.hpp"
#include <parse/reader.hpp>
#include <unordered_map>
#include <vector>
#include <stack>


namespace SON {


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

    TOKEN_DOUBLE_SLASH,

    TOKEN_EOF,
};


static const char* to_debug_string(kind_t k) {
    switch (k) {
        case TOKEN_UNDEFINED:     return "TOKEN_UNDEFINED";

        case TOKEN_EQUAL_SIGN:    return "TOKEN_EQUAL_SIGN";
        case TOKEN_SEMICOLON:     return "TOKEN_SEMICOLON";
        case TOKEN_COMMA:         return "TOKEN_COMMA";

        case TOKEN_BRACE_OPEN:    return "TOKEN_BRACE_OPEN";
        case TOKEN_BRACE_CLOSE:   return "TOKEN_BRACE_CLOSE";

        case TOKEN_PAREN_OPEN:    return "TOKEN_PAREN_OPEN";
        case TOKEN_PAREN_CLOSE:   return "TOKEN_PAREN_CLOSE";

        case TOKEN_BRACKET_OPEN:  return "TOKEN_BRACKET_OPEN";
        case TOKEN_BRACKET_CLOSE: return "TOKEN_BRACKET_CLOSE";

        case TOKEN_KW_NULL:       return "TOKEN_KW_NULL";
        case TOKEN_KW_TRUE:       return "TOKEN_KW_TRUE";
        case TOKEN_KW_FALSE:      return "TOKEN_KW_FALSE";

        case TOKEN_IDENTIFIER:    return "TOKEN_IDENTIFIER";
        case TOKEN_INTEGER:       return "TOKEN_INTEGER";
        case TOKEN_FLOATING:      return "TOKEN_FLOATING";
        case TOKEN_STRING:        return "TOKEN_STRING";

        case TOKEN_DOUBLE_SLASH:  return "TOKEN_DOUBLE_SLASH";

        case TOKEN_EOF:           return "TOKEN_EOF";
    }

    return "ERROR";
}


static const char* to_string(kind_t k) {
    switch (k) {
        case TOKEN_UNDEFINED:     return "? undefined";

        case TOKEN_EQUAL_SIGN:    return "=";
        case TOKEN_SEMICOLON:     return ";";
        case TOKEN_COMMA:         return ",";

        case TOKEN_BRACE_OPEN:    return "{";
        case TOKEN_BRACE_CLOSE:   return "}";

        case TOKEN_PAREN_OPEN:    return "(";
        case TOKEN_PAREN_CLOSE:   return ")";

        case TOKEN_BRACKET_OPEN:  return "[";
        case TOKEN_BRACKET_CLOSE: return "]";

        case TOKEN_KW_NULL:       return "null";
        case TOKEN_KW_TRUE:       return "true";
        case TOKEN_KW_FALSE:      return "false";

        case TOKEN_IDENTIFIER:    return "identifier";
        case TOKEN_INTEGER:       return "integer";
        case TOKEN_FLOATING:      return "floating";
        case TOKEN_STRING:        return "string";

        case TOKEN_DOUBLE_SLASH:  return "//";

        case TOKEN_EOF:           return "EOF";
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

    size_t line_number;
    size_t char_number;
    parse::reader::result_t line;

    kind_t kind;
    value_t value;
};


void print (token t) {
    printf("%lu:%lu token { kind = %20s; value = ", t.line_number, t.char_number, to_debug_string(t.kind));

    switch (t.kind) {
        case TOKEN_UNDEFINED: printf("ERROR! }\n"); break;
        case TOKEN_EOF:       printf("EOF; }\n"); break;
        case TOKEN_KW_NULL:   printf("null; }\n"); break;
        case TOKEN_KW_TRUE:   printf("true; }\n"); break;
        case TOKEN_KW_FALSE:  printf("false; }\n"); break;
        case TOKEN_INTEGER:   printf("%ld; }\n", t.value.integer); break;
        case TOKEN_FLOATING:  printf("%lf; }\n", t.value.floating); break;

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

        case TOKEN_DOUBLE_SLASH:
            printf("//; }\n");
            break;
        default:
            printf("??? }\n");
    }
}


static inline bool is_double_quote (char c) { return c == '"'; }
static inline bool is_valid_identifier_head (char c) { return parse::is_alpha(c) || c == '_'; }
static inline bool is_valid_identifier_body (char c) { return parse::is_digit(c) || parse::is_alpha(c) || c == '_'; }

static const char* spaces = "                                                                         ";
static const char* carets = "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";

static inline i32 digits_in_number (i32 n) {
    i32 result = 0;

    while (n != 0) {
        n /= 10;
        result += 1;
    }

    return result;
}


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

    token* get () const {
        if (bucket == nullptr) {
            return nullptr;
        }

        if (idx >= bucket->counter) {
            return nullptr;
        }

        return bucket->buffer + idx;
    }

    bool next () {
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

    bool prev () {
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


bucket_t* make_new_bucket () {
    auto* bucket = new bucket_t();
    bucket->buffer = (token*) malloc(sizeof(token) * BUFFER_LENGTH);
    bucket->buffer_size = BUFFER_LENGTH;

    return bucket;
}


iterator iterator_make (bucket_t* bucket) {
    iterator it;
    it.bucket = bucket;
    it.idx = 0;

    return it;
}


void bucket_push_token (bucket_t* bucket, token t) {
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


void free_all_buckets (bucket_t* bucket) {
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


// void lexer_report_error (
//     const char* message,
//     const char* filename,
//     reader::result_t line,
//     reader::checkpoint_t checkpoint,
//     size_t error_length)
// {
//     printf("%s:%lu:%lu: error: %s\n", filename, checkpoint.line_counter, checkpoint.char_counter, message);

//     printf("   %lu |%.*s\n", checkpoint.line_counter, (int)line.length, line.start);
//     printf("   %.*s |%.*s%.*s\n", 
//         (int)(digits_in_number(checkpoint.line_counter)), spaces,
//         (int)(checkpoint.current - line.start), spaces,
//         (int)(line.length - (checkpoint.current - line.start)), carets);
// }


struct lexer {
    const char* filename = nullptr;
    parse::reader* r = nullptr;
    bucket_t* storage = nullptr;
    std::unordered_map<std::string, kind_t> keywords;

    void initialize (parse::reader* r_, bucket_t* storage_, const char* filename_) {
        r = r_;
        storage = storage_;
        filename = filename_;

        keywords.emplace("null", TOKEN_KW_NULL);
        keywords.emplace("true", TOKEN_KW_TRUE);
        keywords.emplace("false", TOKEN_KW_FALSE);
    }

    void terminate () {}

    // @make that escaped newlines do not show up in resulted string
    bool eat_quoted_string () {
        auto checkpoint = r->get_checkpoint();
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

            // Newlines are not allowed to intercept string.
            if (c == '\0' || parse::is_newline(c)) {
                r->restore_checkpoint(checkpoint);

                auto line = r->get_line();

                printf("%s:%lu:%lu: error: unclosed double quote\n", filename, checkpoint.line_counter, checkpoint.char_counter);
                printf("   %lu |%.*s\n", checkpoint.line_counter, (int)line.length, line.start);
                printf("   %.*s |%.*s%.*s\n", 
                    (int)(digits_in_number(checkpoint.line_counter)), spaces,
                    (int)(checkpoint.current - line.start), spaces,
                    (int)(line.length - (checkpoint.current - line.start)), carets);
                return false;
            }

            // If it is not an escape symbol, just skip it once.
            r->skip_char();
            length += 1;
        }

        r->skip_char(); // Skip double quote.
        length += 1;

        token t;
        t.begin = checkpoint.current;
        t.length = length;
        t.line_number = r->line_counter;
        t.char_number = r->char_counter;
        t.line = r->get_line();
        t.kind = TOKEN_STRING;
        t.value.integer = 0;

        bucket_push_token(storage, t);
        // p->push_token(t);
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
            t.line_number = r->line_counter;
            t.char_number = r->char_counter;
            t.line = r->get_line();
            t.kind = TOKEN_IDENTIFIER;
            t.value.integer = 0;

            bucket_push_token(storage, t);
            // p->push_token(t);
            return true;
        }

        token t;
        t.begin = result.start;
        t.length = result.length;
        t.line_number = r->line_counter;
        t.char_number = r->char_counter;
        t.line = r->get_line();
        t.kind = token_kind->second;
        t.value.integer = 0;

        bucket_push_token(storage, t);
        // p->push_token(t);
        return true;
    }

    bool eat_number () {
        const char* start = r->current;
        u64 len = 0;

        char c = r->get_char();
        if (!parse::is_digit(c) && c != '.' && c != '-' && c != '+') {
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

        while (parse::is_digit(c = r->get_char())) {
            r->skip_char();
            len += 1;

            integral *= 10;
            integral += (c - '0');
        }

        if (c == '.') {
            r->skip_char();
            len += 1;

            while(parse::is_digit(c = r->get_char())) {
                r->skip_char();
                len += 1;

                multiplier *= 0.1;
                fractional += (c - '0') * multiplier;
            }

            token t;
            t.begin = start;
            t.length = len;
            t.line_number = r->line_counter;
            t.char_number = r->char_counter;
            t.line = r->get_line();
            t.kind = TOKEN_FLOATING;
            t.value.floating = sign*(integral + fractional);

            bucket_push_token(storage, t);
            return true;
        }

        token t;
        t.begin = start;
        t.length = len;
        t.line_number = r->line_counter;
        t.char_number = r->char_counter;
        t.line = r->get_line();
        t.kind = TOKEN_INTEGER;
        t.value.integer = sign*integral;

        bucket_push_token(storage, t);
        return true;
    }

    bool run () {
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
                t.line_number = r->line_counter;
                t.char_number = r->char_counter;
                t.line = r->get_line();
                t.kind = kind_t(c);
                t.value.integer = 0;

                r->skip_char();

                bucket_push_token(storage, t);
                continue;
            }
            else if (c == '/') {
                parse::reader::result_t result = r->eat_string("//");
                if (result) {
                    r->eat_until(parse::is_newline);
                    r->eat_while(parse::is_newline);
                    continue;
                }
            }
            else if (c == '\"') {
                bool successful = eat_quoted_string();
                if (not successful) return false;
            }
            else if (parse::is_digit(c) || (c == '.') || (c == '+') || (c == '-')) { // Read number, integer or float is unknown.
                bool successful = eat_number();
                if (not successful) return false;
            }
            else if (is_valid_identifier_head(c)) {
                bool successful = eat_keyword_or_identifier();
                if (not successful) return false;
            }
            else {
                auto checkpoint = r->get_checkpoint();
                auto result = r->eat_until(parse::is_space);

                printf("%s:%lu:%lu: error: unknown lexeme ’%.*s’\n", filename, checkpoint.line_counter, checkpoint.char_counter, (int)result.length, result.start);

                auto line = r->get_line();
                printf("   %lu | %.*s\n", checkpoint.line_counter, (int)line.length, line.start);
                printf("   %.*s | %.*s%.*s\n", 
                    (int)(digits_in_number(checkpoint.line_counter)), spaces,
                    (int)(checkpoint.current - line.start), spaces,
                    (int)result.length, carets);

                // printf("Parse Error! Unknown lexeme: %.*s\n", i32(result.length), result.start);
                return false;
            }
        }

        if (c == '\0') {
            token t;
            t.begin = r->current;
            t.length = 1;
            t.line_number = r->line_counter;
            t.char_number = r->char_counter;
            t.line = r->get_line();
            t.kind = TOKEN_EOF;
            t.value.integer = 0;

            bucket_push_token(storage, t);
            return true;
        }

        return false;
    }
};


// ======================================================== //



struct parser_t {
    bool verbose = false;
    const char* filename = nullptr;
    const char* text = nullptr;
    size_t length;

    bucket_t* tokens = nullptr;
    iterator it;

    struct error_t {
        char message[256] = {0};
        size_t length = 0;
        bool critical = true;
    };

#define eprintf(ERROR, ...) \
    if ((ERROR).length > 0) \
        (ERROR).length += sprintf((ERROR).message + (ERROR).length, __VA_ARGS__); \
    else \
        (ERROR).length = sprintf((ERROR).message, __VA_ARGS__); \
    (void)0

    void highlight_token (token* t) {
        printf("   %lu |%.*s\n", t->line_number, (int)t->line.length, t->line.start);
        printf("   %.*s |%.*s%.*s\n", 
            (int)(digits_in_number(t->line_number)), spaces,
            (int)(t->begin - t->line.start), spaces,
            (int)(t->length), carets);
    }


    void highlight_token (token* t, char* buffer) {
        int n = 0;
        n += sprintf(buffer + n, "   %lu |%.*s\n", t->line_number, (int)t->line.length, t->line.start);
        n += sprintf(buffer + n, "   %.*s |%.*s%.*s\n", 
            (int)(digits_in_number(t->line_number)), spaces,
            (int)(t->begin - t->line.start), spaces,
            (int)(t->length), carets);
    }

    void highlight_token (token* t, error_t* e) {
        eprintf(*e, "   %lu | %.*s\n", t->line_number, (int)t->line.length, t->line.start);
        eprintf(*e, "   %.*s | %.*s%.*s\n", 
            (int)(digits_in_number(t->line_number)), spaces,
            (int)(t->begin - t->line.start), spaces,
            (int)(t->length), carets);
    }


    std::stack<error_t> error_stack;

    void initialize (const char* text_, size_t length_, const char* filename_) {
        text = text_;
        length = length_;
        filename = filename_;

        tokens = make_new_bucket();
    }

    void terminate () {
        free_all_buckets(tokens);
    }

    SON::IValue* parse () {
        parse::reader r;
        r.initialize(text, length);

        lexer l;
        l.initialize(&r, tokens, filename);

        bool successful = l.run();
        if (not successful) {
            l.terminate();
            r.terminate();
            return nullptr;
        }

        it = iterator_make(tokens);
        SON::IValue* result = parse_something(it);

        if (result and verbose) {
            printf("Parsing successful\n");
        }

        l.terminate();
        r.terminate();

        return result;
    }

    SON::IValue* parse_something (iterator it) {
        if (not it) {
            return nullptr;
        }

        SON::IValue* result = nullptr;

        result = parse_object(true);
        if (result) {
            return result;
        }

        if (not error_stack.top().critical) {
            error_stack = std::stack<error_t>();

            result = parse_list_of_things(true);

            if (result) {
                return result;
            }
        }

        while (not error_stack.empty()) {
            error_t error = error_stack.top();
            error_stack.pop();
            
            printf("%.*s\n", (int)error.length, error.message);
        }

        return nullptr;
    }

    SON::Object* parse_object (bool top_level = false) {
        iterator checkpoint = it;
        bool have_open_brace = false;
        SON::Object* result = new SON::Object();

        {
            token* t = it.get();
            ASSERT(t);

            if (not (t->kind == TOKEN_BRACE_OPEN or
                    (t->kind == TOKEN_IDENTIFIER and top_level)))
            {
                error_t error;
                error.critical = false;
                eprintf(error, "%s:%lu:%lu: error: '{' expected, found %s ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                highlight_token(t, &error);
                error_stack.push(error);

                delete result;
                return nullptr; // This is definetely not an object!
            }

            have_open_brace = t->kind == TOKEN_BRACE_OPEN;

            if (t->kind == TOKEN_BRACE_OPEN) {
                it.next();
            }
        }

        {    
            do {
                token* t = it.get();
                ASSERT(t);

                if (t->kind != TOKEN_IDENTIFIER) break;
                if (not parse_key_value_pair(result, top_level)) break;
            } while (true);
        }

        {
            token* t = it.get();
            ASSERT(t);

            if (top_level and have_open_brace and t->kind != TOKEN_BRACE_CLOSE) {
                error_t error;
                eprintf(error, "%s:%lu:%lu: error: '}' expected, found %s ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                highlight_token(t, &error);
                error_stack.push(error);

                it = checkpoint;
                delete result;
                return nullptr;
            }

            if (top_level and not have_open_brace and t->kind != TOKEN_EOF) {
                error_t error;
                eprintf(error, "%s:%lu:%lu: error: expected EOF (end of naked top level object), but found %s ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                highlight_token(t, &error);
                error_stack.push(error);

                it = checkpoint;
                delete result;
                return nullptr;
            }

            if (not top_level and not have_open_brace) {
                ASSERT(false); // I don't know is this even possible
            }

            if (have_open_brace and t->kind != TOKEN_BRACE_CLOSE) {
                error_t error;
                eprintf(error, "%s:%lu:%lu: error: '}' expected, found %s ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                highlight_token(t, &error);
                error_stack.push(error);

                it = checkpoint;
                delete result;
                return nullptr;
            }

            if (t->kind == TOKEN_BRACE_CLOSE) {
                it.next(); // Consume '}'
            }
        }

        return result;
    }

    bool parse_key_value_pair (SON::Object* result, bool top_level = false) {
        iterator checkpoint = it;

        token* key = nullptr;
        SON::IValue* value = nullptr;

        {
            token* t = it.get();
            ASSERT(t);

            if (top_level and t->kind == TOKEN_EOF) {
                // Return without errors
                return false;
            }

            if (not top_level and t->kind == TOKEN_BRACE_CLOSE) {
                // Return without errors
                return false;
            }

            if (t->kind != TOKEN_IDENTIFIER) {
                error_t error;
                error.critical = false;
                eprintf(error, "%s:%lu:%lu: error: expected identifier, but found %s ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                highlight_token(t, &error);
                error_stack.push(error);

                return false;
            }

            key = t;
            it.next();
        }

        {
            token* t = it.get();
            ASSERT(t);

            if (t->kind != TOKEN_EQUAL_SIGN) {
                error_t error;
                eprintf(error, "%s:%lu:%lu: error: expected ’=’, but found %s ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                highlight_token(t, &error);
                error_stack.push(error);

                it = checkpoint;
                return false;
            }

            it.next();
        }

        {
            token* t = it.get();
            ASSERT(t);

            if (t->kind != TOKEN_KW_NULL &&
                t->kind != TOKEN_KW_TRUE &&
                t->kind != TOKEN_KW_FALSE &&
                t->kind != TOKEN_INTEGER &&
                t->kind != TOKEN_FLOATING &&
                t->kind != TOKEN_STRING &&
                t->kind != TOKEN_BRACE_OPEN && // List
                t->kind != TOKEN_BRACKET_OPEN) // Object
            {
                error_t error;
                eprintf(error, "%s:%lu:%lu: error: value is expected, found ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, (int)t->length, t->begin);
                highlight_token(t, &error);
                error_stack.push(error);

                it = checkpoint;
                return false;
            }

            switch (t->kind) {
                case TOKEN_KW_NULL: value = new SON::Null(); it.next(); break;
                case TOKEN_KW_TRUE: value = new SON::Boolean(true); it.next(); break;
                case TOKEN_KW_FALSE: value = new SON::Boolean(false); it.next(); break;
                case TOKEN_INTEGER: value = new SON::Integer(t->value.integer); it.next(); break;
                case TOKEN_FLOATING: value = new SON::Floating(t->value.floating); it.next(); break;
                case TOKEN_STRING: value = new SON::String(std::string(t->begin + 1, t->length - 2)); it.next(); break;
                case TOKEN_BRACE_OPEN: {
                    auto* object = parse_object();

                    if (object == nullptr) {
                        error_t error;
                        eprintf(error, "%s:%lu:%lu: error: value is expected, found %s ’%.*s’\n",
                            filename,
                            t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                        highlight_token(t, &error);
                        error_stack.push(error);

                        it = checkpoint;
                        return false;
                    }

                    value = object;
                    break;
                }
                case TOKEN_BRACKET_OPEN: {
                    auto* list = new SON::List();

                    bool successful = parse_list_of_things();

                    if (not successful) {
                        error_t error;
                        eprintf(error, "%s:%lu:%lu: error: value is expected, found %s ’%.*s’\n",
                            filename,
                            t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                        highlight_token(t, &error);
                        error_stack.push(error);

                        delete list;

                        it = checkpoint;
                        return false;
                    }

                    value = list;
                    break;
                }
                default: ASSERT(false);
            }
        }

        {
            token* t = it.get();
            ASSERT(t);

            if (t->kind == TOKEN_SEMICOLON) {
                it.next(); // Skip semicolon.
            } else {
                // Semicolon is optional.
            }
        }

        // We reached the end, it's all good.
        result->emplace(std::string(key->begin, key->length), value);
        return true;
    }

    SON::List* parse_list_of_things (bool top_level = false) {
        iterator checkpoint = it;

        SON::List* result = new SON::List();
        token* t_bracket_open = nullptr;
        bool have_open_bracket = false;

        {
            token* t = it.get();
            ASSERT(t);

            if (t->kind != TOKEN_BRACKET_OPEN and not top_level) {
                error_t error;
                eprintf(error, "%s:%lu:%lu: error: ’[’ is expected, found %s ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                highlight_token(t, &error);
                error_stack.push(error);

                delete result;
                return nullptr;
            }

            have_open_bracket = t->kind == TOKEN_BRACKET_OPEN;
            if (have_open_bracket) {
                t_bracket_open = t;
                it.next();
            }
        }
middle:
        do {
            {
                token* t = it.get();
                ASSERT(t);

                switch (t->kind) {
                    case TOKEN_KW_NULL: result->emplace(new SON::Null()); it.next(); break;
                    case TOKEN_KW_TRUE: result->emplace(new SON::Boolean(true)); it.next(); break;
                    case TOKEN_KW_FALSE: result->emplace(new SON::Boolean(false)); it.next(); break;
                    case TOKEN_INTEGER: result->emplace(new SON::Integer(t->value.integer)); it.next(); break;
                    case TOKEN_FLOATING: result->emplace(new SON::Floating(t->value.floating)); it.next(); break;
                    case TOKEN_STRING: result->emplace(new SON::String(std::string(t->begin + 1, t->length - 2))); it.next(); break;
                    case TOKEN_BRACE_OPEN: {
                        // This is an object
                        auto* object = parse_object();

                        if (object == nullptr) {
                            it = checkpoint;
                            delete result;
                            return nullptr;
                        }

                        result->emplace(object);
                        break;
                    }
                    case TOKEN_BRACKET_OPEN: {
                        // This is a nested list
                        SON::List* p_list = parse_list_of_things();

                        if (p_list == nullptr) {
                            it = checkpoint;
                            delete result;
                            return nullptr;
                        }

                        result->emplace(p_list);
                        break;
                    }
                    default:
                        error_t error;
                        eprintf(error, "%s:%lu:%lu: error: ’]’ is expected, found %s ’%.*s’\n",
                            filename,
                            t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);
                        if (have_open_bracket) {
                            highlight_token(t_bracket_open, &error);
                            eprintf(error, "   %.*s | %.*s%s\n", 
                                (int)(digits_in_number(t_bracket_open->line_number)), spaces,
                                (int)(t_bracket_open->begin - t_bracket_open->line.start), spaces,
                                "open bracket is here");
                        }
                        highlight_token(t, &error);
                        eprintf(error, "   %.*s | %.*s%s\n", 
                            (int)(digits_in_number(t->line_number)), spaces,
                            (int)(t->begin - t->line.start), spaces,
                            to_string(t->kind));
                        error_stack.push(error);

                        it = checkpoint;
                        delete result;
                        return nullptr;
                }
            }

            {
                token* t = it.get();
                ASSERT(t);

                if (t->kind == TOKEN_COMMA) {
                    it.next();
                }
                
                if (t->kind == TOKEN_BRACKET_CLOSE or t->kind == TOKEN_EOF) {
                    break;
                }
            }
        } while (true);

        {
            token* t = it.get();
            ASSERT(t);

            if (have_open_bracket and t->kind == TOKEN_BRACKET_CLOSE) {
                // Consume ']'
                it.next();
            }
            else if (not have_open_bracket and t->kind == TOKEN_BRACKET_CLOSE) {
                error_t error;
                eprintf(error, "%s:%lu:%lu: error: expected EOF (end of naked top level list), found %s ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);

                highlight_token(t, &error);
                eprintf(error, "   %.*s | %.*s%s is here\n", 
                    (int)(digits_in_number(t->line_number)), spaces,
                    (int)(t->begin - t->line.start), spaces,
                    to_string(t->kind));

                error_stack.push(error);

                it = checkpoint;
                delete result;
                return nullptr;
            }
            else if (have_open_bracket and t->kind != TOKEN_BRACKET_CLOSE) {
                error_t error;
                eprintf(error, "%s:%lu:%lu: error: ’]’ is expected, found %s ’%.*s’\n",
                    filename,
                    t->line_number, t->char_number, to_string(t->kind), (int)t->length, t->begin);

                highlight_token(t_bracket_open, &error);
                eprintf(error, "   %.*s | %.*s%s\n", 
                    (int)(digits_in_number(t_bracket_open->line_number)), spaces,
                    (int)(t_bracket_open->begin - t_bracket_open->line.start), spaces,
                    "open bracket is here");

                highlight_token(t, &error);
                eprintf(error, "   %.*s | %.*s%s is here\n", 
                    (int)(digits_in_number(t->line_number)), spaces,
                    (int)(t->begin - t->line.start), spaces,
                    to_string(t->kind));

                error_stack.push(error);

                it = checkpoint;
                delete result;
                return nullptr;
            }
        }

        {
            token* t = it.get();
            if (t->kind != TOKEN_EOF and top_level) {
                auto* top_level_list = new SON::List();
                top_level_list->emplace(result);
                result = top_level_list;
                have_open_bracket = false;
                goto middle; // @FIX BAD BAD BAD!!!
            }
        }

        return result;
    }
};


// ======================================================== //


void Parser::initialize (const char* text, size_t size, const char* filename) {
    auto* parser = new parser_t();
    parser->initialize(text, size, filename);
    parser->verbose = verbose;
    impl = (void*)parser;
}


void Parser::terminate () {
    auto* parser = (parser_t*) impl;
    delete parser;
}


SON::IValue* Parser::parse () {
    auto* parser = (parser_t*) impl;

    return parser->parse();
}

} // SON

