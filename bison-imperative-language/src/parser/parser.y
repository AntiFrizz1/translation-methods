%{
    #include <iostream>
    #include <string>
    #include "../header.h"
    const int UNKNOWN = 100;
    int yylex(void);
    void yyerror(const char *);

    std::map<std::string, int> variables_map;
    std::map<int, std::string> types;
    std::map<int, std::pair<int, int>> cast_type;
    std::map<std::string, int> cast_id;
    std::map<std::string, int> read_type;

    int string_tmp_id = 0;
    int int_tmp_id = 0;
    int char_tmp_id = 0;
    int float_tmp_id = 0;
    int bool_tmp_id = 0;

    int expression_type = UNKNOWN;

    std::string header = "";

    std::string io_header = "#include <stdio.h>";
    bool is_io_header = false;

    std::string math_header = "#include <math.h>";
    bool is_math_header = false;

    std::string string_header = "#include <string.h>";
    bool is_string_header = false;

    std::string stdlib_header = "#include <stdlib.h>";
    bool is_stdlib_header = false;

    std::string bool_header = "#include <stdbool.h>";
    bool is_bool_header = false;

    std::string string_clean_function = "void _clean_string(char* _str, int _size) {\nfor (int _i = 0; _i < _size; _i++) {\n_str[_i] = '\0';\n}\n}\n";
    bool is_string_clean_function = false;

    std::string global = "";

    std::string main_function_begin = "int main() {\n";
    std::string main_function_end = "}\n";

    std::string main_function = "";
    std::string top_string = "";
    std::vector<std::string> top_out;
%}

%union
{
    int int_value;
    expression_struct* expression_value;
}
%token <expression_value> NUMBER
%token <expression_value> FNUMBER
%token <expression_value> CHAR
%token <expression_value> STRING
%token <expression_value> ID
%token <expression_value> CAST
%token <expression_value> PRINT
%token <expression_value> READ
%token <expression_value> BOOLEAN
%token <expression_value> COMPARE
%token ASSIGNMENT COMMA  COLON OBRACKET CBRACKET PLUS MINUS TIMES DIVIDE POWER OR AND NOT IF ELSE OBLOCKBRACKET CBLOCKBRACKET WHILE IN RANGE FOR MOD DO
%left PLUS MINUS
%left TIMES DIVIDE MOD
%left NEG
%left OR
%left AND
%right NOT
%left COMPARE

%type <expression_value> expression
%type <expression_value> lines
%type <expression_value> line
%type <expression_value> program
%type <expression_value> if_expression
%type <expression_value> while_expression
%type <expression_value> for_expression
%type <expression_value> for_variable
%start main
%%

main: program {
        main_function += $1->string_value;
    }
    ;

program:    {$$ = new expression_struct();}
    | program lines {
        expression_struct* ans = new expression_struct();
        ans->string_value += $1->string_value + top_string + $2->string_value;
        top_string = "";
        $$ = ans;
    }
    | program if_expression {
        expression_struct* ans = new expression_struct();
        ans->string_value += $1->string_value + top_string + $2->string_value;
        top_string = "";
        $$ = ans;
    }
    | program while_expression {
        expression_struct* ans = new expression_struct();
        ans->string_value += $1->string_value + top_string + $2->string_value;
        top_string = "";
        $$ = ans;
    }
    | program for_expression {
        expression_struct* ans = new expression_struct();
        ans->string_value += $1->string_value + top_string + $2->string_value;
        top_string = "";
        $$ = ans;
    }
    ;


if_expression: IF OBRACKET expression CBRACKET OBLOCKBRACKET  program CBLOCKBRACKET  {
        expression_struct* tmp1 = $3;
        expression_struct* tmp2 = $6;
        if (tmp1->type != 4) {
            {
                char error_str[256];
                sprintf(error_str,"expected bool expression but was %s", types[tmp1->type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
        std::string local_out;
        local_out += "if (" + tmp1->string_value + ") {\n" + tmp2->string_value + "}\n";
        expression_struct* ans = new expression_struct();
        ans->string_value = local_out;
        $$ = ans;
    }
    | IF OBRACKET expression CBRACKET OBLOCKBRACKET  program CBLOCKBRACKET ELSE OBLOCKBRACKET  program CBLOCKBRACKET  {
          expression_struct* tmp1 = $3;
          expression_struct* tmp2 = $6;
          expression_struct* tmp3 = $10;
          if (tmp1->type != 4) {
              {
                  char error_str[256];
                  sprintf(error_str,"expected bool expression but was %s", types[tmp1->type].c_str());
                  yyerror(error_str);
                  exit(1);
              }
          }
          std::string local_out;
          local_out += "if (" + tmp1->string_value + ") {\n" + tmp2->string_value + "} else {\n" + tmp3->string_value + "}\n";
          expression_struct* ans = new expression_struct();
          ans->string_value = local_out;
          $$ = ans;
      }
    ;

while_expression: WHILE OBRACKET expression CBRACKET OBLOCKBRACKET  program CBLOCKBRACKET  {
        expression_struct* tmp1 = $3;
        expression_struct* tmp2 = $6;
        if (tmp1->type != 4) {
            {
                char error_str[256];
                sprintf(error_str,"expected bool expression but was %s", types[tmp1->type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
        std::string local_out = "while (" + tmp1->string_value + ") {\n" + tmp2->string_value + "}\n";
        expression_struct* ans = new expression_struct();
        ans->string_value = local_out;
        $$ = ans;
    }
    | DO OBLOCKBRACKET program CBLOCKBRACKET WHILE OBRACKET expression CBRACKET {
        expression_struct* tmp1 = $7;
        expression_struct* tmp2 = $3;
        if (tmp1->type != 4) {
            {
                char error_str[256];
                sprintf(error_str,"expected bool expression but was %s", types[tmp1->type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
        std::string local_out = "do {\n" + tmp2->string_value + "} while (" + tmp1->string_value + ");\n";
        expression_struct* ans = new expression_struct();
        ans->string_value = local_out;
        $$ = ans;
    }
    ;

for_expression: FOR for_variable IN RANGE OBRACKET expression COMMA expression COMMA expression CBRACKET OBLOCKBRACKET  program CBLOCKBRACKET  {
        expression_struct* var = $2;
        expression_struct* from = $6;
        expression_struct* to = $8;
        expression_struct* step = $10;

        if (from->type != 1) {
            {
                char error_str[256];
                sprintf(error_str,"expected int variable but was %s", types[from->type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }

        if (to->type != 1) {
            {
                char error_str[256];
                sprintf(error_str,"expected int variable but was %s", types[to->type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }

        if (step->type != 1) {
            {
                char error_str[256];
                sprintf(error_str,"expected int variable but was %s", types[step->type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }

        std::string local_out;

        std::string from_var;
        from_var = "_int" + std::to_string(int_tmp_id);
        int_tmp_id++;

        std::string to_var;
        to_var = "_int" + std::to_string(int_tmp_id);
        int_tmp_id++;

        std::string step_var;
        step_var = "_int" + std::to_string(int_tmp_id);
        int_tmp_id++;
        if (!is_stdlib_header) {
            is_stdlib_header = true;
            header += stdlib_header + "\n";
        }

        expression_struct* expr = $13;

        local_out += "int " + from_var + " = " + from->string_value + ";\nint " + to_var + " = " + to->string_value + ";\nint " + step_var + " = " + step->string_value + ";\n";
        local_out += "if ((" + step_var + " >= 0)) {\n";
        local_out += "for (" + var->string_value + " = " + from_var + "; " + var->string_value + " < " + to_var + "; " + var->string_value + " = " + var->string_value + " + " + step_var + ") {\n";
        local_out += expr->string_value + "}\n} else {\n";
        local_out += "if ((" + step_var + " <= 0)) {\n";
        local_out += "for (" + var->string_value + " = " + from_var + "; " + var->string_value + " > " + to_var + "; " + var->string_value + " = " + var->string_value + " + " + step_var + ") {\n";
        local_out += expr->string_value + "}\n} else {\n}\n}\n";
        expression_struct* ans = new expression_struct();
        ans->string_value = local_out;
        $$ = ans;
    }
    ;

for_variable: ID {
        expression_struct* var = $1;
        if (variables_map.count(var->string_value) != 0 && variables_map[var->string_value] != 1) {
            {
                char error_str[256];
                sprintf(error_str,"expected int variable but was %s", types[variables_map[var->string_value]].c_str());
                yyerror(error_str);
                exit(1);
            }
        } else if (variables_map.count(var->string_value) == 0) {
            variables_map[var->string_value] = 1;
            global += types[1] + " " + var->string_value + ";\n";
        }
        $$ = var;
}

lines: line  {
        expression_struct* ans = new expression_struct();
        expression_struct* tmp1 = $1;
        ans->string_value = tmp1->string_value + "\n";
        //printf("%s", ans->string_value.c_str());
        $$ = ans;
    }
    ;



line: ID ASSIGNMENT expression {
        expression_struct* ans = new expression_struct();
        expression_struct* tmp1 = $3;
        expression_struct* tmp2 = $1;

        std::string expr = tmp1->string_value;
        std::string id = tmp2->string_value;
        std::string local_out;

        if (variables_map.count(id) == 0) {
            variables_map[id] = tmp1->type;
            global += types[tmp1->type] + " " + id + ";\n";
        }
        if (tmp1->type != variables_map[id]) {
            char error_str[256];
            sprintf(error_str,"can't cast %s to %s", types[variables_map[id]].c_str(), types[tmp1->type].c_str());
            yyerror(error_str);
            exit(1);
        }
        local_out += id + " = " + expr + ";";
        ans->string_value = local_out;
        $$ = ans;
    }
    | PRINT OBRACKET expression CBRACKET {
        expression_struct* tmp1 = $1;
        expression_struct* tmp2 = $3;

        std::string tmp_new_line;
        std::string tmp_out_format;

        if (tmp1->string_value == "printLine") {
            tmp_new_line = "\\n";
        }

        if (!is_io_header) {
            is_io_header = true;
            header += io_header + "\n";
        }

        switch(tmp2->type) {
            case 0:
                tmp_out_format = "%f";
                break;
            case 1:
                tmp_out_format = "%d";
                break;
            case 2:
                tmp_out_format = "%c";
                break;
            case 3:
                tmp_out_format = "%s";
                break;
            case 4:
                tmp_out_format = "%d";
        }
        expression_struct* ans = new expression_struct();
        std::string local_out;
        local_out += "printf(\"" + tmp_out_format + tmp_new_line + "\", " + tmp2->string_value + ");";
        ans->string_value = local_out;
        $$ = ans;
    }
    | PRINT OBRACKET CBRACKET {
        expression_struct* tmp1 = $1;
        std::string tmp_new_line;
        std::string tmp_out_format;
        if (tmp1->string_value == "printLine") {
          tmp_new_line = "\\n";
        }
        if (!is_io_header) {
          is_io_header = true;
          header += io_header + "\n";
        }
        expression_struct* ans = new expression_struct();
        std::string local_out;
        local_out += "printf(\"" + tmp_new_line + "\");";
        ans->string_value = local_out;
        $$ = ans;
    }
    ;

expression: NUMBER {
        expression_type = 1;
        expression_struct* tmp1 = $1;
        tmp1->type = 1;
        //printf("d: %s %d\n", tmp1->string_value.c_str(), tmp1->type);
        $$ = tmp1;
    }
    | FNUMBER {
        expression_type = 0;
        expression_struct* tmp1 = $1;
        tmp1->type = 0;
        //printf("d: %s %d\n", tmp1->string_value.c_str(), tmp1->type);
        $$ = tmp1;

    }
    | CHAR {
        expression_type = 2;
        expression_struct* tmp1 = $1;
        tmp1->type = 2;
        //printf("d: %s %d\n", tmp1->string_value.c_str(), tmp1->type);
        $$ = tmp1;
    }
    | STRING {
        if (!is_string_header) {
            is_string_header = true;
            header += string_header + "\n";
        }
        expression_type = 3;
        expression_struct* tmp1 = $1;
        tmp1->type = 3;
        //printf("d: %s %d\n", tmp1->string_value.c_str(), tmp1->type);
        $$ = tmp1;
    }
    | BOOLEAN {
        if (!is_bool_header) {
            is_bool_header = true;
            header += bool_header + "\n";
        }
        expression_type = 4;
        expression_struct* tmp1 = $1;
        tmp1->type = 4;
        //printf("d: %s %d\n", tmp1->string_value.c_str(), tmp1->type);
        $$ = tmp1;
    }
    | ID {
        expression_struct* tmp1 = $1;
        std::string help = tmp1->string_value;
        if (variables_map.count(help) == 0) {
            {
                char error_str[256];
                sprintf(error_str,"undefined variable %s", help.c_str());
                yyerror(error_str);
                exit(1);
            }
        }
        expression_type = variables_map[help];
        tmp1->type = variables_map[help];
        //printf("d :%s %d\n", tmp1->string_value.c_str(), tmp1->type);
        $$ = $1;
    }
    | expression PLUS expression {
        expression_type = UNKNOWN;
        expression_struct* left = $1;
        int first_type = left->type;

        expression_type = UNKNOWN;
        expression_struct* right = $3;
        int second_type = right->type;

        if (first_type == second_type && (first_type == 0 || first_type == 1)) {
            left->string_value += " + " + right->string_value;
            left->type = first_type;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do '%s' '+' '%s'", types[first_type].c_str(), types[second_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    | expression MOD expression {
        expression_type = UNKNOWN;
        expression_struct* left = $1;
        int first_type = left->type;

        expression_type = UNKNOWN;
        expression_struct* right = $3;
        int second_type = right->type;

        if (first_type == second_type && (first_type == 1)) {
            left->string_value += " % " + right->string_value;
            left->type = first_type;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do '%s' 'mod' '%s'", types[first_type].c_str(), types[second_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    | expression MINUS expression {
        expression_type = UNKNOWN;

        expression_struct* left = $1;
        int first_type = left->type;

        expression_type = UNKNOWN;
        expression_struct* right = $3;
        int second_type = right->type;

        if (first_type == second_type && (first_type == 0 || first_type == 1)) {
            left->string_value += " - " + right->string_value;
            left->type = first_type;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do '%s' '-' '%s'", types[first_type].c_str(), types[second_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    | expression TIMES expression {
        expression_type = UNKNOWN;

        expression_struct* left = $1;
        int first_type = left->type;

        expression_type = UNKNOWN;
        expression_struct* right = $3;
        int second_type = right->type;

        if (first_type == second_type && (first_type == 0 || first_type == 1)) {
            left->string_value += " * " + right->string_value;
            left->type = first_type;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do '%s' '*' '%s'", types[first_type].c_str(), types[second_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    | expression DIVIDE expression {
        expression_type = UNKNOWN;

        expression_struct* left = $1;
        int first_type = left->type;

        expression_type = UNKNOWN;
        expression_struct* right = $3;
        int second_type = right->type;

        if (first_type == second_type && (first_type == 0 || first_type == 1)) {
            left->string_value += " / " + right->string_value;
            left->type = first_type;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do '%s' '/' '%s'", types[first_type].c_str(), types[second_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    | MINUS expression %prec NEG {
        expression_type = UNKNOWN;

        expression_struct* left = $2;
        int first_type = left->type;

        if ((first_type == 0 || first_type == 1)) {
            left->string_value = "-" + left->string_value;
            left->type = first_type;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do ''-' '%s'", types[first_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    | OBRACKET expression CBRACKET {
        expression_struct* expr = $2;
        expr->string_value = "(" + expr->string_value + ")";
        $$ = expr;
    }
    | CAST OBRACKET expression CBRACKET {
        expression_type = UNKNOWN;
        expression_struct* operation = $1;
        expression_struct* expr = $3;
        std::pair<int, int> help = cast_type[cast_id[operation->string_value]];
        printf("d: %s %d\n", expr->string_value.c_str(), expr->type);
        printf("d: %s %d %d\n", operation->string_value.c_str(), help.first, help.second);
        if (expr->type != help.first) {
            {
                char error_str[256];
                sprintf(error_str,"can't use %s %s(%s) with type %s", types[help.second].c_str(), operation->string_value.c_str(), types[help.first].c_str(), types[expr->type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
        expression_struct* answer = new expression_struct();
        answer->type = help.second;
        switch (cast_id[operation->string_value]) {
            //intToString
            case 0:
                if (!is_string_header) {
                    is_string_header = true;
                    header += string_header + "\n";
                }
                if (!is_io_header) {
                    is_io_header = true;
                    header += io_header + "\n";
                }
                {
                    std::string local_out = "char _str";
                    local_out += std::to_string(string_tmp_id) + "[11];\n";
                    local_out += "sprintf(_str" + std::to_string(string_tmp_id) + ", \"%d\", " + expr->string_value + ");\n";
                    top_string += local_out;
                    answer->string_value = "_str" + std::to_string(string_tmp_id);
                    string_tmp_id++;
                }
                break;
            //intToFloat
            case 1:
                {
                    std::string local_out = "((float)(";
                    local_out += expr->string_value + "))";
                    answer->string_value = local_out;
                }
                break;
            //intToChar
            case 2:
                {
                    std::string local_out = "((char)(";
                    local_out += expr->string_value + "))";
                    answer->string_value += local_out;
                }
                break;
            //floatToString
            case 3:
                if (!is_string_header) {
                    is_string_header = true;
                    header += string_header + "\n";
                }
                if (!is_io_header) {
                    is_io_header = true;
                    header += io_header + "\n";
                }
                {
                    std::string local_out = "char _str";
                    local_out += std::to_string(string_tmp_id) + "[11];\n";
                    local_out += "sprintf(_str" + std::to_string(string_tmp_id) + ", \"%f\", " + expr->string_value + ");\n";
                    top_string += local_out;
                    answer->string_value += "_str" + std::to_string(string_tmp_id);
                    string_tmp_id++;
                }
                break;
            //floatToInt
            case 4:
                {
                    std::string local_out = "((int)(";
                    local_out += expr->string_value + "))";
                    answer->string_value += local_out;
                }
                break;
            //charToString
            case 5:
                if (!is_string_header) {
                    is_string_header = true;
                    header += string_header + "\n";
                }
                {
                    std::string local_out = "char _str";
                    local_out += std::to_string(string_tmp_id) + "[1];\n";
                    local_out += "_str" + std::to_string(string_tmp_id) + "[0] = " + expr->string_value + ";\n";
                    top_string += local_out;
                    answer->string_value = "_str" + std::to_string(string_tmp_id);
                    string_tmp_id++;
                }
                break;
            //charToInt
            case 6:
                {
                    std::string local_out = "((int)(";
                    local_out += expr->string_value + "))";
                    answer->string_value += local_out;
                }
                break;
            //stringToFloat
            case 7:
                if (!is_string_header) {
                    is_string_header = true;
                    header += string_header + "\n";
                }
                if (!is_stdlib_header) {
                    is_stdlib_header = true;
                    header += stdlib_header + "\n";
                }
                {
                    std::string local_out = "atof(";
                    local_out += expr->string_value + ")";
                    answer->string_value += local_out;
                }
                break;
            //stringToInt
            case 8:
                if (!is_string_header) {
                    is_string_header = true;
                    header += string_header + "\n";
                }
                if (!is_stdlib_header) {
                    is_stdlib_header = true;
                    header += stdlib_header + "\n";
                }
                {
                    std::string local_out = "atoi(";
                    local_out += expr->string_value + ")";
                    answer->string_value += local_out;
                }
                break;
        }
        //printf("d: %s\n", answer->string_value.c_str());
        $$ = answer;
    }
    | READ OBRACKET CBRACKET {
        expression_struct* ans = new expression_struct();
        expression_struct* tmp1 = $1;
        if (!is_io_header) {
            is_io_header = true;
            header += io_header + "\n";
        }
        switch (read_type[tmp1->string_value]) {
            case 0:
                {
                    std::string local_out = "float _float";
                    local_out += std::to_string(float_tmp_id) + ";\n";
                    local_out += "scanf(\"%f\", &_float" + std::to_string(float_tmp_id) + ");\n";
                    top_string += local_out;
                    ans->string_value = "_float" + std::to_string(float_tmp_id);
                    ans->type = 0;
                    float_tmp_id++;

                }
                break;
            case 1:
                {
                    std::string local_out = "int _int";
                    local_out += std::to_string(int_tmp_id) + ";\n";
                    local_out += "scanf(\"%d\", &_int" + std::to_string(int_tmp_id) + ");\n";
                    top_string += local_out;
                    ans->string_value = "_int" + std::to_string(int_tmp_id);
                    ans->type = 1;
                    int_tmp_id++;

                }
                break;
            case 2:
                {
                    std::string local_out = "char _char";
                    local_out += std::to_string(char_tmp_id) + ";\n";
                    local_out += "scanf(\"%c\", &_char" + std::to_string(char_tmp_id) + ");\n";
                    top_string += local_out;
                    ans->string_value = "_char" + std::to_string(char_tmp_id);
                    ans->type = 2;
                    char_tmp_id++;
                }
                break;
            case 3:
                {
                    std::string local_out = "char _str";
                    local_out += std::to_string(string_tmp_id) + "[256];\n";
                    local_out += "scanf(\"%s\", _str" + std::to_string(string_tmp_id) + ");\n";
                    top_string += local_out;
                    ans->string_value = "_str" + std::to_string(string_tmp_id);
                    ans->type = 3;
                    string_tmp_id++;
                }
                break;
            case 4:
                {
                    std::string local_out = "char _str";
                    local_out += std::to_string(string_tmp_id) + "[256];\n";
                    local_out += "fgets(_str" + std::to_string(string_tmp_id) + ", sizeof(_str" + std::to_string(string_tmp_id) + "), stdin);\n";
                    top_string += local_out;
                    ans->string_value = "_str" + std::to_string(string_tmp_id);
                    ans->type = 3;
                    string_tmp_id++;
                }
                break;
        }
        $$ = ans;
    }
    | expression AND expression {
        if (!is_bool_header) {
            is_bool_header = true;
            header += bool_header + "\n";
        }
        expression_struct* left = $1;
        int first_type = left->type;

        expression_struct* right = $3;
        int second_type = right->type;

        if (first_type == second_type && first_type == 4) {
            left->string_value += " && " + right->string_value;
            left->type = first_type;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do '%s' 'and' '%s'", types[first_type].c_str(), types[second_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    | expression OR expression {
        if (!is_bool_header) {
            is_bool_header = true;
            header += bool_header + "\n";
        }
        expression_struct* left = $1;
        int first_type = left->type;

        expression_struct* right = $3;
        int second_type = right->type;

        if (first_type == second_type && first_type == 4) {
            left->string_value += " || " + right->string_value;
            left->type = first_type;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do '%s' 'or' '%s'", types[first_type].c_str(), types[second_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    | NOT expression {
        if (!is_bool_header) {
            is_bool_header = true;
            header += bool_header + "\n";
        }
        expression_struct* left = $2;
        int first_type = left->type;
        if (first_type == 4) {
            left->string_value = "!" + left->string_value;
            left->type = first_type;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do 'not' '%s'", types[first_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    | expression COMPARE expression {
        if (!is_bool_header) {
            is_bool_header = true;
            header += bool_header + "\n";
        }
        expression_struct* left = $1;
        int first_type = left->type;

        expression_struct* right = $3;
        int second_type = right->type;

        expression_struct* operation = $2;

        if (first_type == second_type && (first_type != 3)) {
            left->string_value += " " + operation->string_value + " " + right->string_value;
            left->type = 4;
            //printf("d: %s %d\n", left->string_value.c_str(), left->type);
            $$ = left;
        } else {
            {
                char error_str[256];
                sprintf(error_str,"can't do '%s' 'or' '%s'", types[first_type].c_str(), types[second_type].c_str());
                yyerror(error_str);
                exit(1);
            }
        }
    }
    ;