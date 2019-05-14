#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <fstream>

using namespace std;

string input;

void read_whole_file() {
    while(true) {
        char tmp = static_cast<char>(std::cin.get());
        input += tmp;
        if (tmp == EOF) {
            break;
        }
    }
}

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

string header;

string union_string;

void parse_header() {
    skip_whitespaces(input);

    if (input.size() < 4 || !(input[0] == '%' && input[1] == '{')) {
        exit(1);
    }
    int index = -1;
    for (int i = 0; i < input.size() - 1; i++) {
        if (input[i] == '%' && input[i + 1] == '}') {
            index = i;
            break;
        }
    }
    if (index == -1) {
        exit(1);
    }
    header = input.substr(2, static_cast<unsigned long>(index - 2));
    input = input.substr(static_cast<unsigned long>(index + 2), input.size() - index - 2);
}

map<string, string> tokens_type;
map<string, string> nonterminals_type;
string start;

void parse_lines() {
    bool f = true;
    while (true) {
        f = false;

        smatch match;
        skip_whitespaces(input);

        regex union_regex("^%union");
        regex token_regex("^%token");
        regex type_regex("^%type");
        regex start_regex("^%start");

        if (regex_search(input, match, union_regex)) {
            input = regex_replace(input, union_regex, "");
            skip_whitespaces(input);
            if (input.size() < 2 || input[0] != '{') {
                exit(1);
            }

            int index = -1;
            int balance = 0;

            for (int i = 1; i < input.size(); i++) {
                if (input[i] == '{') {
                    balance++;
                } else if (input[i] == '}') {
                    if (balance == 0) {
                        index = i;
                        break;
                    } else {
                        balance--;
                    }
                }
            }

            if (index == -1) {
                exit(1);
            }
            if (!union_string.empty()) {
                exit(1);
            }

            union_string = input.substr(1, static_cast<unsigned long>(index - 1));
            input = input.substr(static_cast<unsigned long>(index + 1), input.size() - index - 1);

            f = true;
        } else if (regex_search(input, match, token_regex)) {
            input = regex_replace(input, token_regex, "");
            skip_whitespaces(input);

            if (input.empty()) {
                exit(1);
            }
            string type;
            if (input[0] == '<') {
                int index = -1;
                for (int i = 0; i < input.size(); i++) {
                    if (input[i] == '>') {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    exit(1);
                }

                type = input.substr(1, static_cast<unsigned long>(index - 1));
                input = input.substr(static_cast<unsigned long>(index + 1), input.size() - index - 1);
                skip_whitespaces(input);
            }

            if (input.empty() || input[0] == '%') {
                exit(1);
            }

            vector<string> tokens;

            while (!input.empty() && input[0] != '%') {
                skip_whitespaces(input);
                int index = -1;
                for (int i = 0 ; i < input.size(); i++) {
                    if (is_line(input[i]) || is_space(input[i])) {
                        index = i;
                        break;
                    }
                }
                if (index == -1) {
                    exit(1);
                }
                string token = input.substr(0, static_cast<unsigned long>(index));
                trim_string(token);
                tokens.push_back(token);
                input = input.substr(static_cast<unsigned long>(index + 1), input.size() - index - 1);
                skip_whitespaces(input);
            }

            if (tokens.empty()) {
                exit(1);
            }

            for (auto &token : tokens) {
                if (tokens_type.count(token) > 0) {
                    exit(1);
                } else {
                    tokens_type[token] = type;
                }
            }
            f = true;
        } else if (regex_search(input, match, type_regex)) {
            input = regex_replace(input, type_regex, "");
            skip_whitespaces(input);

            if (input.empty()) {
                exit(1);
            }
            string type;
            if (input[0] == '<') {
                int index = -1;
                for (int i = 0; i < input.size(); i++) {
                    if (input[i] == '>') {
                        index = i;
                        break;
                    }
                }

                if (index == -1) {
                    exit(1);
                }

                type = input.substr(1, static_cast<unsigned long>(index - 1));
                input = input.substr(static_cast<unsigned long>(index + 1), input.size() - index - 1);
                skip_whitespaces(input);
            } else {
                exit(1);
            }

            if (input.empty() || input[0] == '%') {
                exit(1);
            }

            vector<string> nonterminals;

            while (!input.empty() && input[0] != '%') {
                skip_whitespaces(input);
                int index = -1;
                for (int i = 0 ; i < input.size(); i++) {
                    if (is_line(input[i]) || is_space(input[i])) {
                        index = i;
                        break;
                    }
                }
                if (index == -1) {
                    exit(1);
                }
                string nonterminal = input.substr(0, static_cast<unsigned long>(index));
                trim_string(nonterminal);
                nonterminals.push_back(nonterminal);
                input = input.substr(static_cast<unsigned long>(index + 1), input.size() - index - 1);
                skip_whitespaces(input);
            }

            if (nonterminals.empty()) {
                exit(1);
            }

            for (auto &nonterminal : nonterminals) {
                if (nonterminals_type.count(nonterminal) > 0) {
                    exit(1);
                } else {
                    nonterminals_type[nonterminal] = type;
                }
            }

            f = true;
        } else if (regex_search(input, match, start_regex)) {
            input = regex_replace(input, start_regex, "");
            skip_whitespaces(input);
            if (input.empty() || input[0] == '%') {
                exit(1);
            }
            int index = -1;
            for (int i = 0; i < input.size(); i++) {
                if (is_line(input[i]) || is_space(input[i])) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                exit(1);
            }



            start = input.substr(0, static_cast<unsigned long>(index));
            trim_string(start);
            if (start.empty()) {
                exit(1);
            }
            input = input.substr(static_cast<unsigned long>(index + 1), input.size() - index - 1);
            f = true;
        } else {
            break;
        }
    }
}

map<string, vector<pair<deque<string>, string>>> rules;

void parse_rules() {
    skip_whitespaces(input);

    if (input.size() < 2 || (input[0] != '%' && input[1] != '%')) {
        exit(1);
    }

    input = input.substr(2, input.size() - 2);

    skip_whitespaces(input);
    string eof;
    eof += EOF;
    while (!input.empty() && input != eof) {
        int index = -1;
        for (int i = 0; i < input.size(); i++) {
            if (input[i] == ':') {
                index = i;
                break;
            }
        }

        if (index == -1) {
            exit(1);
        }

        string nonterminal = input.substr(0, static_cast<unsigned long>(index));
        trim_string(nonterminal);
        if (nonterminals_type.count(nonterminal) == 0) {
            nonterminals_type[nonterminal] = "";
        }
        if (nonterminal.empty()) {
            exit(1);
        }

        input = input.substr(static_cast<unsigned long>(index + 1), input.size() - index - 1);
        skip_whitespaces(input);
        index = -1;
        int balance = 0;
        for (int i = 0; i < input.size(); i++) {
            if (input[i] == ';' && balance == 0) {
                index = i;
                break;
            }
            if (input[i] == '{') {
                balance++;
            } else if (input[i] == '}') {
                balance--;
            }
        }

        if (index == -1) {
            exit(1);
        }

        string rules_line = input.substr(0, static_cast<unsigned long>(index));
        input = input.substr(static_cast<unsigned long>(index + 1), input.size() - index - 1);
        skip_whitespaces(input);
        bool f1 = true;
        balance = 0;
        while (f1) {
            int i1 = -1;
            for (int i = 0; i < rules_line.size(); i++) {
                if (rules_line[i] == '|' && balance == 0) {
                    i1 = i;
                    break;
                }
                if (rules_line[i] == '{') {
                    balance++;
                } else if (rules_line[i] == '}') {
                    balance--;
                }

            }
            string tmp;
            if (i1 == -1) {
                tmp = rules_line;
                trim_string(tmp);
                rules_line = "";
                f1 = false;
            } else {
                tmp = rules_line.substr(0, static_cast<unsigned long>(i1));
                trim_string(tmp);
                rules_line = rules_line.substr(static_cast<unsigned long>(i1 + 1), rules_line.size() - i1 - 1);
                skip_whitespaces(rules_line);
            }

            deque<string> line;

            while (!tmp.empty() && tmp[0] != '{') {
                int index = -1;
                balance = 0;
                for (int i = 0; i < tmp.size(); i++) {
                    if (is_space(tmp[i]) || is_line(tmp[i])) {
                        index = i;
                        break;
                    }
                    if (tmp[i] == '{') {
                        index = i;
                        break;
                    }
                }
                string element;

                if (index == -1) {
                    element = tmp;
                    tmp = "";
                } else {
                    element = tmp.substr(0, static_cast<unsigned long>(index));
                    trim_string(element);
                    tmp = tmp.substr(static_cast<unsigned long>(index), tmp.size() - index);
                }
                line.push_back(element);
                skip_whitespaces(tmp);
            }
            trim_string(tmp);
            rules[nonterminal].push_back(make_pair(line, tmp));
            skip_whitespaces(rules_line);
        }
        skip_whitespaces(input);
    }
}


void process_functions() {
    for (auto &item: rules) {
        string nonterminal = item.first;
        for (auto &line: item.second) {
            deque<string> rule_line = line.first;
            string work = line.second;
            regex dollars("\\$[0-9]+");
            smatch match;
            while (regex_search(work, match, dollars)) {
                string tmp = match.str();
                int index = atoi(tmp.substr(1, tmp.size() - 1).c_str()) - 1;
                string name = rule_line[index];
                string string_type;
                if (tokens_type.count(name) > 0) {
                    string_type = tokens_type[name];
                } else if (nonterminals_type.count(name) > 0) {
                    string_type = nonterminals_type[name];
                } else {
                    exit(1);
                }
                regex regex1("\\$" + to_string(index + 1));
                if (string_type.empty()) {
                    work = regex_replace(work, regex1, "outs[" + to_string(index) + "]");
                } else {
                    work = regex_replace(work, regex1, "outs[" + to_string(index) + "]." + string_type);
                }
            }
            regex double_dollars("\\$\\$");
            if (regex_search(work, match, double_dollars)) {
                string string_type;
                if (nonterminals_type.count(nonterminal) > 0) {
                    string_type = nonterminals_type[nonterminal];
                } else {
                    exit(1);
                }
                if (string_type.empty()) {
                    work = regex_replace(work, double_dollars, "zzans");
                } else {
                    work = regex_replace(work, double_dollars, "zzans." + string_type);
                }
            }

            string func = "[](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;" + work + "return zzans;}";
            line.second = func;
        }
    }
}

string header_name;
void make_header(string file) {
    header_name = file;
    ofstream fout(file);
    string out;
    out.append("#include <string>\n"
               "#include <map>\n"
               "#include <set>\n"
               "#include <unordered_set>\n"
               "#include <vector>\n"
               "#include <deque>\n"
               "#include <exception>\n"
               "#include <functional>\n");

    out.append(header);

    out.append("#ifndef ZZTOKENTYPE\n")
    .append("#define ZZTOKENTYPE\n")
    .append("enum zztokentype {\n");

    int count = 256;
    int size = static_cast<int>(tokens_type.size());
    for (auto &j : tokens_type) {
        if (size != 1) {
            out.append("\t").append(j.first).append(" = ").append(to_string(count)).append(",\n");
        } else {
            out.append("\t").append(j.first).append(" = ").append(to_string(count)).append("\n");
        }
        size--;
        count++;
    }

    out.append("};\n")
    .append("#endif\n\n").append("#ifndef ZZTYPE\n").append("union ZZTYPE {\n").append(union_string).append("\n")
            .append("};\n").append("#endif\n");
    string program;

    process_functions();
    program
            .append("std::set<std::string> tokens;\n"
                    "std::set<std::string> nonterminals;\n"
                    "\n"
                    "std::map<int, std::string> token_table;\n"
                    "\n"
                    "std::map<std::string, std::set<std::string>> first_table, follow_table;\n"
                    "\n"
                    "std::map<std::string, std::vector<std::pair<std::deque<std::string>, std::function<ZZTYPE(std::vector<ZZTYPE>)>>>> rules;")
            .append("std::string start = \"").append(start).append("\";\n\n")
            .append("std::string ENDOFLINE = \"$\";\n"
                    "\n"
                    "extern int zzlex();\n"
                    "\n"
                    "extern std::string zztext;\n"
                    "\n"
                    "extern std::string input;\n"
                    "\n"
                    "\n"
                    "ZZTYPE zzlval;\n"
                    "\n"
                    "void make_first() {\n"
                    "    bool changed = true;\n"
                    "    while(changed) {\n"
                    "        changed = false;\n"
                    "        for (auto &nonterminal : rules) {\n"
                    "            for (auto &rule : nonterminal.second) {\n"
                    "                std::deque<std::string> tmp = rule.first;\n"
                    "                unsigned long size1 = first_table[nonterminal.first].size();\n"
                    "                if (!tmp.empty()) {\n"
                    "                    if (tokens.count(tmp[0]) > 0) {\n"
                    "                        first_table[nonterminal.first].insert(first_table[tmp[0]].begin(), first_table[tmp[0]].end());\n"
                    "                    } else {\n"
                    "                        int i = 0;\n"
                    "                        while (i < tmp.size() && first_table[tmp[i]].count(\"\") > 0) {\n"
                    "                            i++;\n"
                    "                        }\n"
                    "                        if (i != tmp.size()) {\n"
                    "                            first_table[nonterminal.first].insert(first_table[tmp[i]].begin(), first_table[tmp[i]].end());\n"
                    "                        } else {\n"
                    "                            first_table[nonterminal.first].insert(\"\");\n"
                    "                        }\n"
                    "                    }\n"
                    "                } else {\n"
                    "                    first_table[nonterminal.first].insert(\"\");\n"
                    "                }\n"
                    "                if (size1 != first_table[nonterminal.first].size()) {\n"
                    "                    changed = true;\n"
                    "                }\n"
                    "            }\n"
                    "        }\n"
                    "    }\n"
                    "}\n"
                    "\n"
                    "void make_follow() {\n"
                    "    follow_table[start].insert(ENDOFLINE);\n"
                    "    bool changed = true;\n"
                    "    while (changed) {\n"
                    "        changed = false;\n"
                    "        for (auto &nontermainal : rules) {\n"
                    "            for (auto &rule : nontermainal.second) {\n"
                    "                if (rule.first.empty()) {\n"
                    "                    continue;\n"
                    "                }\n"
                    "\n"
                    "                std::string last = rule.first[rule.first.size() - 1];\n"
                    "                if (nonterminals.count(last) > 0) {\n"
                    "                    int size = static_cast<int>(follow_table[last].size());\n"
                    "                    follow_table[last].insert(follow_table[nontermainal.first].begin(),\n"
                    "                                              follow_table[nontermainal.first].end());\n"
                    "                    if (follow_table[last].size() > size) {\n"
                    "                        changed = true;\n"
                    "                    }\n"
                    "                }\n"
                    "                bool f = true;\n"
                    "                for (int i = static_cast<int>(rule.first.size() - 2); i >= 0; i--) {\n"
                    "                    if (nonterminals.count(rule.first[i]) > 0) {\n"
                    "                        int size = static_cast<int>(follow_table[rule.first[i]].size());\n"
                    "                        std::set<std::string> tmp1 = first_table[rule.first[i + 1]];\n"
                    "                        tmp1.erase(\"\");\n"
                    "                        follow_table[rule.first[i]].insert(tmp1.begin(), tmp1.end());\n"
                    "                        if (first_table[rule.first[i + 1]].count(\"\") > 0 && f) {\n"
                    "                            follow_table[rule.first[i]].insert(follow_table[nontermainal.first].begin(), follow_table[nontermainal.first].end());\n"
                    "                        } else {\n"
                    "                            f = false;\n"
                    "                        }\n"
                    "                        if (follow_table[rule.first[i]].size() > size) {\n"
                    "                            changed = true;\n"
                    "                        }\n"
                    "                    }\n"
                    "                }\n"
                    "            }\n"
                    "        }\n"
                    "    }\n"
                    "}"
                    "\n"
                    "int cur_token = -1;\n"
                    "\n"
                    "void next_token() {\n"
                    "\tcur_token = zzlex();\n"
                    "}\n"
                    "\n"
                    "\n")
            .append(
                    "ZZTYPE simple_parser(const std::string &nonterminal) {\n"
                    "\tbool f = false;\n"
                    "\tZZTYPE ans;\n"
                    "\tfor (auto &item : rules[nonterminal]) {\n"
                    "\t\tif (item.first.empty()) {\n"
                    "\t\t\tif (follow_table[nonterminal].count(token_table[cur_token]) > 0) {\n"
                    "\t\t\t\treturn item.second(std::vector<ZZTYPE>());\n"
                    "\t\t\t}\n"
                    "\t\t} else if (first_table[item.first[0]].count(token_table[cur_token]) > 0  || (first_table[item.first[0]].count(\"\") > 0 && follow_table[item.first[0]].count(token_table[cur_token]) > 0)) {\n"
                    "\t\t\tstd::vector<ZZTYPE> outs(item.first.size());\n"
                    "\t\t\tf = true;\n"
                    "\t\t\tfor (int i = 0; i < item.first.size(); i++) {\n"
                    "\t\t\t\tif (tokens.count(item.first[i]) > 0 && token_table[cur_token] == item.first[i]) {\n"
                    "\t\t\t\t\touts[i] = zzlval;\n"
                    "\t\t\t\t\tnext_token();\n"
                    "\t\t\t\t} else if (nonterminals.count(item.first[i]) > 0) {\n"
                    "\t\t\t\t\touts[i] = simple_parser(item.first[i]);\n"
                    "\t\t\t\t} else {\n"
                    "\t\t\t\t\tthrow std::runtime_error(\"Unexpected token \" + token_table[cur_token] + \"(\" + zztext + \")\");\n"
                    "\t\t\t\t}\n"
                    "\t\t\t}\n"
                    "\t\t\tans = item.second(outs);\n"
                    "\t\t\tbreak;\n"
                    "\t\t}\n"
                    "\t}\n"
                    "\tif (f) {\n"
                    "\t\treturn ans;\n"
                    "\t} else {\n"
                    "\t\tthrow std::runtime_error(\"Parse error\");\n"
                    "\t}\n"
                    "}\n\n")
            .append("int zzparse() {\n"
                    "\ttokens = {");
    size = tokens_type.size();
    for (auto &item: tokens_type) {
        if (size != 1) {
            program.append("\"" + item.first + "\", ");
        } else {
            program.append("\"" + item.first + "\"");
        }
        size--;
    }

    program.append("};\n").append("\tnonterminals = {");

    size = nonterminals_type.size();
    for (auto &item: nonterminals_type) {
        if (size != 1) {
            program.append("\"" + item.first + "\", ");
        } else {
            program.append("\"" + item.first + "\"");
        }
        size--;
    }
    program.append("};\n");

    for (auto &item: tokens_type) {
        program.append("\ttoken_table[").append(item.first).append("] = \"").append(item.first).append("\";\n");
    }

    for (auto &item: rules) {
        program.append("\trules[\"").append(item.first).append("\"] = {");
        int size1 = item.second.size();
        for (auto &deques: item.second) {
            program.append("{std::deque<std::string>{");
            int size2 = deques.first.size();
            for (auto &element: deques.first) {
                if (size2 != 1) {
                    program.append("\"" + element + "\", ");
                } else {
                    program.append("\"" + element + "\"");
                }
                size2--;

            }
            program.append("}, ");
            program.append(deques.second).append("}");
            if (size1 != 1) {
                program.append(", ");
            }
            size1--;
        }
        program.append("};\n");
    }

    program.append("\tfor (auto &i: tokens) {\n"
                   "\t\tfirst_table[i].insert(i);\n"
                   "\t}");

    program.append("\tmake_first();\n"
                   "\tmake_follow();\n"
                   "\n"
                   "\tnext_token();\n"
                   "\tsimple_parser(start);\n"
                   "\treturn 0;\n}\n");


    fout << out << "\n" << program;
    fout.close();
}



string ENDOFLINE = "$";

map<std::string, std::set<std::string>> first_table, follow_table;


void make_first() {
    bool changed = true;
    while(changed) {
        changed = false;
        for (auto &nonterminal : rules) {
            for (auto &rule : nonterminal.second) {
                std::deque<std::string> tmp = rule.first;
                unsigned long size1 = first_table[nonterminal.first].size();
                if (!tmp.empty()) {
                    if (tokens_type.count(tmp[0]) > 0) {
                        first_table[nonterminal.first].insert(first_table[tmp[0]].begin(), first_table[tmp[0]].end());
                    } else {
                        int i = 0;
                        while (i < tmp.size() && first_table[tmp[i]].count("") > 0) {
                            i++;
                        }
                        if (i != tmp.size()) {
                            first_table[nonterminal.first].insert(first_table[tmp[i]].begin(), first_table[tmp[i]].end());
                        } else {
                            first_table[nonterminal.first].insert("");
                        }
                    }
                } else {
                    first_table[nonterminal.first].insert("");
                }
                if (size1 != first_table[nonterminal.first].size()) {
                    changed = true;
                }
            }
        }
    }
}

void make_follow() {
    follow_table[start].insert(ENDOFLINE);
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &nontermainal : rules) {
            for (auto &rule : nontermainal.second) {
                if (rule.first.empty()) {
                    continue;
                }

                std::string last = rule.first[rule.first.size() - 1];
                if (nonterminals_type.count(last) > 0) {
                    int size = static_cast<int>(follow_table[last].size());
                    follow_table[last].insert(follow_table[nontermainal.first].begin(),
                                              follow_table[nontermainal.first].end());
                    if (follow_table[last].size() > size) {
                        changed = true;
                    }
                }
                bool f = true;
                for (int i = static_cast<int>(rule.first.size() - 2); i >= 0; i--) {
                    if (nonterminals_type.count(rule.first[i]) > 0) {
                        int size = static_cast<int>(follow_table[rule.first[i]].size());
                        std::set<std::string> tmp1 = first_table[rule.first[i + 1]];
                        tmp1.erase("");
                        follow_table[rule.first[i]].insert(tmp1.begin(), tmp1.end());
                        if (first_table[rule.first[i + 1]].count("") > 0 && f) {
                            follow_table[rule.first[i]].insert(follow_table[nontermainal.first].begin(), follow_table[nontermainal.first].end());
                        } else {
                            f = false;
                        }
                        if (follow_table[rule.first[i]].size() > size) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}

void check_ll1() {
    for (auto &i: tokens_type) {
        first_table[i.first].insert(i.first);
    }
    make_first();
    make_follow();
    for (auto &item: rules) {
        string nonterminal = item.first;
        for (int i = 0; i < item.second.size(); i++) {
            for (int j = 0; j < item.second.size(); j++) {
                if (i != j) {
                    if (item.second[i].first.empty() && item.second[j].first.empty()) {
                        throw std::runtime_error("not ll1 grammar");
                    } else if (!item.second[i].first.empty() && item.second[j].first.empty()) {
                        set<string> fl = follow_table[nonterminal];
                        set<string> ft = first_table[item.second[i].first[0]];
                        set<string> tmp;
                        tmp.insert(fl.begin(), fl.end());
                        tmp.insert(ft.begin(), ft.end());
                        if (tmp.size() != fl.size() + ft.size()) {
                            throw std::runtime_error("not ll1 grammar");
                        }
                    } else if (item.second[i].first.empty() && !item.second[j].first.empty()) {
                        set<string> fl = follow_table[nonterminal];
                        set<string> ft = first_table[item.second[j].first[0]];
                        set<string> tmp;
                        tmp.insert(fl.begin(), fl.end());
                        tmp.insert(ft.begin(), ft.end());
                        if (tmp.size() != fl.size() + ft.size()) {
                            throw std::runtime_error("not ll1 grammar");
                        }
                    } else {
                        set<string> ft1 = first_table[item.second[i].first[0]];
                        set<string> ft2 = first_table[item.second[j].first[0]];
                        set<string> tmp;
                        tmp.insert(ft1.begin(), ft1.end());
                        tmp.insert(ft2.begin(), ft2.end());
                        if (tmp.size() != ft1.size() + ft2.size()) {
                            throw std::runtime_error("not ll1 grammar");
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    string input_file;
    string output_file;

    if (argc == 1) {
        input_file = "parser.y";
        output_file = "parser.h";
    } else if (argc == 2) {
        input_file = argv[1];
        output_file = input_file + ".h";
    } else if (argc == 3) {
        input_file = argv[1];
        output_file = argv[2];
    }
    freopen(input_file.c_str(), "r", stdin);
    read_whole_file();
    parse_header();
    parse_lines();
    parse_rules();
    check_ll1();
    make_header(output_file);
    return 0;
}