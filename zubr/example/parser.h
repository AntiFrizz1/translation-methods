#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <deque>
#include <exception>
#include <functional>

	#include "header.h"
	#include <cstdio>
	#include <cstdlib>
	#define ZZTYPE expression_type*
#ifndef ZZTOKENTYPE
#define ZZTOKENTYPE
enum zztokentype {
	DIVIDE = 256,
	ENDOF = 257,
	LPAREN = 258,
	MINUS = 259,
	NUMBER = 260,
	PLUS = 261,
	RPAREN = 262,
	TIMES = 263
};
#endif

#ifndef ZZTYPE
union ZZTYPE {

};
#endif

std::set<std::string> tokens;
std::set<std::string> nonterminals;

std::map<int, std::string> token_table;

std::map<std::string, std::set<std::string>> first_table, follow_table;

std::map<std::string, std::vector<std::pair<std::deque<std::string>, std::function<ZZTYPE(std::vector<ZZTYPE>)>>>> rules;std::string start = "main";

std::string ENDOFLINE = "$";

extern int zzlex();

extern std::string zztext;

extern std::string input;


ZZTYPE zzlval;

void make_first() {
    bool changed = true;
    while(changed) {
        changed = false;
        for (auto &nonterminal : rules) {
            for (auto &rule : nonterminal.second) {
                std::deque<std::string> tmp = rule.first;
                unsigned long size1 = first_table[nonterminal.first].size();
                if (!tmp.empty()) {
                    if (tokens.count(tmp[0]) > 0) {
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
                if (nonterminals.count(last) > 0) {
                    int size = static_cast<int>(follow_table[last].size());
                    follow_table[last].insert(follow_table[nontermainal.first].begin(),
                                              follow_table[nontermainal.first].end());
                    if (follow_table[last].size() > size) {
                        changed = true;
                    }
                }
                bool f = true;
                for (int i = static_cast<int>(rule.first.size() - 2); i >= 0; i--) {
                    if (nonterminals.count(rule.first[i]) > 0) {
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
int cur_token = -1;

void next_token() {
	cur_token = zzlex();
}


ZZTYPE simple_parser(const std::string &nonterminal) {
	bool f = false;
	ZZTYPE ans;
	for (auto &item : rules[nonterminal]) {
		if (item.first.empty()) {
			if (follow_table[nonterminal].count(token_table[cur_token]) > 0) {
				return item.second(std::vector<ZZTYPE>());
			}
		} else if (first_table[item.first[0]].count(token_table[cur_token]) > 0  || (first_table[item.first[0]].count("") > 0 && follow_table[item.first[0]].count(token_table[cur_token]) > 0)) {
			std::vector<ZZTYPE> outs(item.first.size());
			f = true;
			for (int i = 0; i < item.first.size(); i++) {
				if (tokens.count(item.first[i]) > 0 && token_table[cur_token] == item.first[i]) {
					outs[i] = zzlval;
					next_token();
				} else if (nonterminals.count(item.first[i]) > 0) {
					outs[i] = simple_parser(item.first[i]);
				} else {
					throw std::runtime_error("Unexpected token " + token_table[cur_token] + "(" + zztext + ")");
				}
			}
			ans = item.second(outs);
			break;
		}
	}
	if (f) {
		return ans;
	} else {
		throw std::runtime_error("Parse error");
	}
}

int zzparse() {
	tokens = {"DIVIDE", "ENDOF", "LPAREN", "MINUS", "NUMBER", "PLUS", "RPAREN", "TIMES"};
	nonterminals = {"eps", "expression", "expression1", "expression2", "expression4", "factor", "main"};
	token_table[DIVIDE] = "DIVIDE";
	token_table[ENDOF] = "ENDOF";
	token_table[LPAREN] = "LPAREN";
	token_table[MINUS] = "MINUS";
	token_table[NUMBER] = "NUMBER";
	token_table[PLUS] = "PLUS";
	token_table[RPAREN] = "RPAREN";
	token_table[TIMES] = "TIMES";
	rules["eps"] = {{std::deque<std::string>{}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;return zzans;}}};
	rules["expression"] = {{std::deque<std::string>{"expression1", "expression2"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		expression_type* first = outs[0];
		expression_type* second = outs[1];
		second->numbers.push_front(outs[0]->value);
		while(!second->operations.empty()) {
			int a = second->numbers[0];
			int b = second->numbers[1];

			char operation = second->operations[0];

			second->numbers.pop_front();
			second->numbers.pop_front();
			second->operations.pop_front();

			if (operation == '+') {
				a = a + b;
			} else {
				a = a - b;
			}

			second->numbers.push_front(a);
		}
		expression_type* ans = new expression_type();
		ans->value = second->numbers[0];
		zzans = ans;
	}return zzans;}}};
	rules["expression1"] = {{std::deque<std::string>{"factor", "expression4"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		expression_type* first = outs[0];
		expression_type* second = outs[1];
		second->numbers.push_front(outs[0]->value);
		while(!second->operations.empty()) {
			int a = second->numbers[0];
			int b = second->numbers[1];

			char operation = second->operations[0];

			second->numbers.pop_front();
			second->numbers.pop_front();
			second->operations.pop_front();

			if (operation == '*') {
				a = a * b;
			} else {
				a = a / b;
			}

			second->numbers.push_front(a);
		}
		expression_type* ans = new expression_type();
		ans->value = second->numbers[0];
		zzans = ans;
	}return zzans;}}};
	rules["expression2"] = {{std::deque<std::string>{"PLUS", "expression1", "expression2"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		outs[2]->numbers.push_front(outs[1]->value);
		outs[2]->operations.push_front('+');
		zzans = outs[2];
	}return zzans;}}, {std::deque<std::string>{"MINUS", "expression1", "expression2"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		outs[2]->numbers.push_front(outs[1]->value);
		outs[2]->operations.push_front('-');
		zzans = outs[2];
	}return zzans;}}, {std::deque<std::string>{"eps"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		zzans = new expression_type;
	}return zzans;}}};
	rules["expression4"] = {{std::deque<std::string>{"TIMES", "factor", "expression4"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		outs[2]->numbers.push_front(outs[1]->value);
		outs[2]->operations.push_front('*');
		zzans = outs[2];
	}return zzans;}}, {std::deque<std::string>{"DIVIDE", "factor", "expression4"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		outs[2]->numbers.push_front(outs[1]->value);
		outs[2]->operations.push_front('/');
		zzans = outs[2];
	}return zzans;}}, {std::deque<std::string>{"eps"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		zzans = new expression_type();
	}return zzans;}}};
	rules["factor"] = {{std::deque<std::string>{"NUMBER"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		zzans = outs[0];
	}return zzans;}}, {std::deque<std::string>{"MINUS", "expression"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		zzans = outs[1];
		zzans->value = -zzans->value;
	}return zzans;}}, {std::deque<std::string>{"LPAREN", "expression", "RPAREN"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		zzans = outs[1];
	}return zzans;}}, {std::deque<std::string>{"PLUS", "expression"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		zzans = outs[1];
	}return zzans;}}};
	rules["main"] = {{std::deque<std::string>{"expression", "ENDOF"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		printf("%d\n", outs[0]->value);
		exit(0);
	}return zzans;}}, {std::deque<std::string>{"eps"}, [](std::vector<ZZTYPE> outs) -> ZZTYPE {ZZTYPE zzans;{
		printf("%d\n", 0);
		exit(0);
	}return zzans;}}};
	for (auto &i: tokens) {
		first_table[i].insert(i);
	}	make_first();
	make_follow();

	next_token();
	simple_parser(start);
	return 0;
}
