#include <bits/stdc++.h>
#include "header.h"
#include "parser/parser.tab.h"
#include "parser/language.lexer.h"

using namespace std;

extern map<string, int> values;
extern map<int, string> types;
extern string header;

extern string io_header;
extern bool is_io_header;

extern string math_header;
extern bool is_math_header;

extern string string_header;
extern bool is_string_header;

extern string string_clean_function;
extern bool is_string_clean_function;

extern string global;

extern string main_function_begin;
extern string main_function_end;

extern string main_function;

extern map<int, pair<int, int>> cast_type;
extern map<string, int> cast_id;

extern map<string, int> read_type;

void yyerror(const char* str)
{
    fprintf(stderr,"ошибка: %s\n",str);
}

int yywrap()
{
    return 1;
}


int main() {
    types[0] = "float";
    types[1] = "int";
    types[2] = "char";
    types[3] = "char*";
    types[4] = "bool";

    cast_id["intToString"] = 0;
    cast_id["intToFloat"] = 1;
    cast_id["intToChar"] = 2;

    cast_id["floatToString"] = 3;
    cast_id["floatToInt"] = 4;

    cast_id["charToString"] = 5;
    cast_id["charToInt"] = 6;

    cast_id["stringToFloat"] = 7;
    cast_id["stringToInt"] = 8;

    cast_id["boolToInt"] = 9;

    cast_id["intToBool"] = 10;

    cast_type[cast_id["intToString"]] = make_pair(1, 3);
    cast_type[cast_id["intToFloat"]] = make_pair(1, 0);
    cast_type[cast_id["intToChar"]] = make_pair(1, 2);

    cast_type[cast_id["floatToString"]] = make_pair(0, 3);
    cast_type[cast_id["floatToInt"]] = make_pair(0, 1);

    cast_type[cast_id["charToString"]] = make_pair(2, 3);
    cast_type[cast_id["charToInt"]] = make_pair(2, 1);

    cast_type[cast_id["stringToFloat"]] = make_pair(3, 0);
    cast_type[cast_id["stringToInt"]] = make_pair(3, 1);

    cast_type[cast_id["boolToInt"]] = make_pair(4, 1);

    cast_type[cast_id["intToBool"]] = make_pair(1, 4);

    read_type["readInt"] = 1;
    read_type["readFloat"] = 0;
    read_type["readChar"] = 2;
    read_type["readString"] = 3;
    read_type["readLine"] = 4;

    yyparse();

    ofstream fout("main.c");
    fout << header << global << main_function_begin << main_function << main_function_end << endl;
    fout.close();

}
