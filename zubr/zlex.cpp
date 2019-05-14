#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <fstream>
#include <stack>
using namespace std;

string program;

string standart_header = "#include <regex>\n"
                         "#include <iostream>\n"
                         "#include <vector>\n"
                         "#include <fstream>\n"
                         "#include <map>\n"
                         "\n"
                         "std::string input;\n"
                         "std::string zztext;\n";
string header;
string defined = "std::vector<std::pair<std::string, char>> defined = {std::make_pair(\"<<EOF>>\", EOF)};\n";

string new_char = "void next_char() {\n"
                  "\tchar tmp = static_cast<char>(std::cin.get());\n"
                  "\tinput += tmp;\n"
                  "}\n";

string read_whole_file = "bool is_read_whole_file = false;\n"
                         "\n"
                         "void read_whole_file() {\n"
                         "\twhile(true) {\n"
                         "\t\tchar tmp = static_cast<char>(std::cin.get());\n"
                         "\t\tinput += tmp;\n"
                         "\t\tif (tmp == EOF) {\n"
                         "\t\t\tis_read_whole_file = true;\n"
                         "\t\t\tbreak;\n"
                         "\t\t}\n"
                         "\t}\n"
                         "}\n";

string is_defined = "std::pair<bool, int> is_defined(std::string str) {\n"
                    "\tint i = 0;\n"
                    "\tbool f = false;\n"
                    "\tint answer = 0;\n"
                    "\tfor (; i < defined.size(); i++) {\n"
                    "\t\tif (defined[i].first == str) {\n"
                    "\t\t\tf = true;\n"
                    "\t\t\tanswer = defined[i].second;\n"
                    "\t\t\tbreak;\n"
                    "\t\t}\n"
                    "\t}\n"
                    "\treturn std::make_pair(f, answer);\n"
                    "}\n";

string main_option = "int main() {\n"
                     "\twhile (zzlex() != -1) {\n"
                     "\t\tstd::cout << zztext << \"\\n\";\n"
                     "\t}\n"
                     "\treturn 0;\n"
                     "}\n";

map<string, string> variables;

map<string, int> defined_options;

vector<pair<string, string>> regex_and_operation;

void add_options(int num) {

}

int regex_count = 0;

string input;

bool is_space(char c) {
    return (c == ' ') || (c == '\t') || (c == '\r');
}

bool is_line(char c) {
    return (c == '\n');
}

void skip_spaces(string & string1) {
    regex re("^[ \t\r]+");
    string1 = regex_replace(string1, re, "");
}

void skip_line(string & string1) {
    regex re("^[\n]+");
    string1 = regex_replace(string1, re, "");
}

void skip_whitespaces(string & string1) {
    regex re("^[ \n\t\r]+");
    string1 = regex_replace(string1, re, "");
}

void trim_string(string &string1) {
    regex re1("^[ \n\r\t]+");
    regex re2("[ \n\r\t]+$");
    string1 = regex_replace(string1, re1, "");
    string1 = regex_replace(string1, re2, "");
}



void parse_variable(string & first, string & second) {
    if (variables.count(first) > 0) {
        exit(1);
    }

    smatch match;
    regex re("\\{[a-zA-Z]+\\}");
    while (regex_search(second, match, re)) {
        string var = match.str();
        string help = var.substr(1, var.size() - 2);
        string tmp;
        if (variables.count(help) > 0) {
            tmp = variables[help];
        } else {
            exit(1);
        }
        regex regex2("(\\{|\\})");
        var = regex_replace(var, regex2, "\\$1");
        regex regex1(var);
        second = regex_replace(second, regex1, tmp);
    }
    variables[first] = second;
}

void parse_header() {
    skip_whitespaces(input);

    if (!(input[0] == '%' && input[1] == '{')) {
        exit(1);
    }
    int index = -1;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '%' && input[i + 1] == '}') {
            index = i;
            break;
        }
    }
    header = input.substr(2, static_cast<unsigned long>(index - 2));
    input = input.substr(static_cast<unsigned long>(index + 2), input.size() - index - 2);
}

void parse_lines() {
    while (true) {
        skip_whitespaces(input);
        if (input[0] == '%' && input[1] == '%') {
            input = input.substr(2, input.size() - 2);
            break;
        }
        int index = -1;

        for (int i = 0; i < input.size(); i++) {
            if (is_space(input[i])) {
                index = i;
                break;
            }
            if (is_line(input[i])) {
                index = -2;
            }
        }
        if (index == -2 || index == -1) {
            exit(1);
        }
        string first = input.substr(0, static_cast<unsigned long>(index));
        input = input.substr(static_cast<unsigned long>(index), input.size() - index);

        skip_whitespaces(input);
        index = -1;
        for (int i = 0; i < input.size(); i++) {
            if (is_line(input[i])) {
                index = i;
                break;
            }
        }
        if (index == -2 || index == -1) {
            exit(1);
        }
        string second = input.substr(0, static_cast<unsigned long>(index));
        input = input.substr(static_cast<unsigned long>(index), input.size() - index);

        trim_string(second);

        if (first == "%option") {
            if (defined_options.count(second)) {
                add_options(defined_options[second]);
            } else {
                exit(1);
            }

        } else {
            parse_variable(first, second);
        }
    }
}

bool is_open_bracket(char c) {
    return (c == '(')|| (c == '{')|| (c == '[');
}

bool is_closed_bracket(char c) {
    return (c == ')') ||(c == '}') ||(c == ']');
}

bool is_equal_bracket(char c1, char c2) {
    return ((c1 == '(' && c2 == ')') || (c1 == '{' && c2 == '}') || (c1 == '[' && c2 == ']'));
}

void parse_main_regexps() {
    regex end("^%%");
    while (true) {
        skip_whitespaces(input);
        smatch match;
        if (regex_search(input, match, end)) {
            break;
        }
        /*smatch match;
        if (!regex_search(input, match, re)) {
            break;
        }

        string tmp = match.str();
        string help = regex_replace(tmp, brackets, "\\$1");
        regex regex1(help);
        input = regex_replace(input, regex1, "");*/
        string tmp = input;

        stack<char> stack1;
        int index = -1;
        for (int i = 0; i < tmp.size(); i++) {
            if (tmp[i] == '\\') {
                i++;
            } else if (is_open_bracket(tmp[i])) {
                stack1.push(tmp[i]);
            } else if (is_closed_bracket(tmp[i])) {
                if (is_open_bracket(stack1.top()) && is_equal_bracket(stack1.top(), tmp[i])) {
                    stack1.pop();
                } else {
                    exit(1);
                }
            } else if (is_space(tmp[i]) || is_line(tmp[i])) {
                if (stack1.empty()) {
                    index = i;
                    break;
                }
            }
        }
        string first = tmp.substr(0, static_cast<unsigned long>(index));

        while (is_space(tmp[index]) || is_line(tmp[index])) {
            index++;
        }

        int from = index;
        for (int i = from; i < tmp.size(); i++) {
            if (is_open_bracket(tmp[i])) {
                stack1.push(tmp[i]);
            } else if (is_closed_bracket(tmp[i])) {
                if (is_open_bracket(stack1.top()) && is_equal_bracket(stack1.top(), tmp[i])) {
                    stack1.pop();
                } else {
                    exit(1);
                }
            } else if (tmp[i] == ';') {
                if (stack1.empty()) {
                    index = i + 1;
                    break;
                }
            }
        }
        string second = tmp.substr(static_cast<unsigned long>(from), static_cast<unsigned long>(index - from));
        trim_string(second);

        input = input.substr(static_cast<unsigned long>(index), input.size() - index);
        regex def("(\\\\\\{|\\\\\\}|\\\\\\.|\\\\\\*|\\\\\\(|\\\\\\)|\\\\\\[|\\\\\\]|\\\\\\+|\\\\\\?|\\\\\\^)");
        regex regex3("\\{[a-zA-Z]+\\}");
        while (regex_search(first, match, regex3)) {
            string var1 = match.str();
            string help1 = var1.substr(1, var1.size() - 2);
            string tmp1;
            if (variables.count(help1) > 0) {
                tmp1 = variables[help1];
            } else {
                exit(1);
            }
            regex regex4("(\\{|\\})");
            var1 = regex_replace(var1, regex4, "\\$1");
            regex regex5(var1);
            first = regex_replace(first, regex5, tmp1);
        }
        first = regex_replace(first, def, "\\$1");
        regex_and_operation.emplace_back(first, second);

    }
}

void make_zzlex() {
    program += "int zzlex() {\n"
               "\tif (!is_read_whole_file) {\n"
               "\t\tread_whole_file();\n"
               "\t}\n"
               "\n"
               "\tstd::smatch match;\n"
               "\twhile (!input.empty()) {\n";
    for (int i = 0; i < regex_and_operation.size(); i++) {
        string number = to_string(i);
        program.append("\tstd::string regex_string").append(number).append(" = \"^\";\n")
        .append("\tstd::pair<bool, int> pair").append(number).append(" = is_defined(").append("\"")
        .append(regex_and_operation[i].first).append("\");\n")
        .append("\tif (pair").append(number).append(".first) {\n")
        .append("\t\tregex_string").append(number).append(" += (char) pair").append(number).append(".second;\n")
        .append("\t} else {\n")
        .append("\t\tregex_string").append(number).append(" += \"").append(regex_and_operation[i].first).append("\";\n")
        .append("\t}\n")
        .append("\n")
        .append("\tregex_string").append(number).append(" += \"$\";\n")
        .append("\tstd::regex regex").append(number).append("(regex_string").append(number).append(");\n")
        .append("\tbool flag").append(number).append(" = false;\n")
        .append("\tfor (int i = 0; i < input.length(); i++) {\n")
        .append("\t\tstd::string tmp = input.substr(0, input.length() - i);\n")
        .append("\t\tif (std::regex_search(tmp, match, regex").append(number).append(") && match.position() == 0) {\n")
        .append("\t\t\tzztext = match.str();\n")
        .append("\t\t\tinput = input.substr(zztext.size(), input.size() - zztext.size());\n")
        .append("\t\t\t").append(regex_and_operation[i].second).append("\n")
        .append("\t\t\tflag").append(number).append(" = true;\n")
        .append("\t\t\tbreak;\n")
        .append("\t\t}\n")
        .append("\t}\n\n")
        .append("\tif (flag").append(number).append(") {\n")
        .append("\t\tcontinue;\n")
        .append("\t}\n");
    }

    program.append("\t}\n"
                   "\treturn -1;\n}\n");

}


int main(int argc, char *argv[]) {
    string input_file, output_file;
    if (argc == 1) {
        input_file = "lexer.lex";
        output_file = "lexer.cpp";
    } else if (argc == 2) {
        input_file = argv[1];
        output_file = argv[1];
        output_file += ".cpp";
    } else if (argc == 3) {
        input_file = argv[1];
        output_file = argv[2];
    }

    freopen(input_file.c_str(), "r", stdin);
    freopen(output_file.c_str(), "w", stdout);

    while (true) {
        char tmp = static_cast<char>(cin.get());
        if (tmp == EOF) {
            break;
        }
        input += tmp;
    }

    parse_header();
    parse_lines();
    parse_main_regexps();


    program += standart_header + "\n" + defined + "\n" + header  + "\n" + new_char + "\n" + read_whole_file + "\n" + is_defined + "\n";
    make_zzlex();
    cout << program;
    return 0;
}

