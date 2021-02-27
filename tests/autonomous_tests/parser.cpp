#include <service/parse/reader.hpp>
#include <service/parse/abstract_object_parser.hpp>

#include <stdio.h>


void Test_Parser_Lexer () {
    using namespace service;

    const char text[] = 
"{\n"
"  screen = {\n"
"    title  = \"Title\";\n"
"    width  = 1980;\n"
"    height = 1080;\n"
"  };\n"
"  visible = false;\n"
"  something = null;\n"
"  Geteroheneous_LIST = [ \"string\", -42, 42.24, true, null, { object = { int = 199; n = null; }; }];\n"
"  object = {\n"
"    int = 199;\n"
"    null = null;\n"
"  };\n"
"  Empty_list = [];\n"
"};\n";

    abstract_object_parser p;
    
    p.initialize(text, sizeof(text));
    p.parse();
    p.terminate();
}


int main () {
    Test_Parser_Lexer();
    return 0;
}
