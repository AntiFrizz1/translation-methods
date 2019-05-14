#include <regex>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>

std::string input;
std::string zztext;

std::vector<std::pair<std::string, char>> defined = {std::make_pair("<<EOF>>", EOF)};


    #include "parser.h"

void next_char() {
	char tmp = static_cast<char>(std::cin.get());
	input += tmp;
}

bool is_read_whole_file = false;

void read_whole_file() {
	while(true) {
		char tmp = static_cast<char>(std::cin.get());
		input += tmp;
		if (tmp == EOF) {
			is_read_whole_file = true;
			break;
		}
	}
}

std::pair<bool, int> is_defined(std::string str) {
	int i = 0;
	bool f = false;
	int answer = 0;
	for (; i < defined.size(); i++) {
		if (defined[i].first == str) {
			f = true;
			answer = defined[i].second;
			break;
		}
	}
	return std::make_pair(f, answer);
}

int zzlex() {
	if (!is_read_whole_file) {
		read_whole_file();
	}

	std::smatch match;
	while (!input.empty()) {
	std::string regex_string0 = "^";
	std::pair<bool, int> pair0 = is_defined("[0-9]+");
	if (pair0.first) {
		regex_string0 += (char) pair0.second;
	} else {
		regex_string0 += "[0-9]+";
	}

	regex_string0 += "$";
	std::regex regex0(regex_string0);
	bool flag0 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex0) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{expression_type* ans = new expression_type(); ans->value = atoi(zztext.c_str()); zzlval = ans; return NUMBER;};
			flag0 = true;
			break;
		}
	}

	if (flag0) {
		continue;
	}
	std::string regex_string1 = "^";
	std::pair<bool, int> pair1 = is_defined("\\+");
	if (pair1.first) {
		regex_string1 += (char) pair1.second;
	} else {
		regex_string1 += "\\+";
	}

	regex_string1 += "$";
	std::regex regex1(regex_string1);
	bool flag1 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex1) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{return PLUS;};
			flag1 = true;
			break;
		}
	}

	if (flag1) {
		continue;
	}
	std::string regex_string2 = "^";
	std::pair<bool, int> pair2 = is_defined("-");
	if (pair2.first) {
		regex_string2 += (char) pair2.second;
	} else {
		regex_string2 += "-";
	}

	regex_string2 += "$";
	std::regex regex2(regex_string2);
	bool flag2 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex2) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{return MINUS;};
			flag2 = true;
			break;
		}
	}

	if (flag2) {
		continue;
	}
	std::string regex_string3 = "^";
	std::pair<bool, int> pair3 = is_defined("\\*");
	if (pair3.first) {
		regex_string3 += (char) pair3.second;
	} else {
		regex_string3 += "\\*";
	}

	regex_string3 += "$";
	std::regex regex3(regex_string3);
	bool flag3 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex3) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{return TIMES;};
			flag3 = true;
			break;
		}
	}

	if (flag3) {
		continue;
	}
	std::string regex_string4 = "^";
	std::pair<bool, int> pair4 = is_defined("/");
	if (pair4.first) {
		regex_string4 += (char) pair4.second;
	} else {
		regex_string4 += "/";
	}

	regex_string4 += "$";
	std::regex regex4(regex_string4);
	bool flag4 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex4) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{return DIVIDE;};
			flag4 = true;
			break;
		}
	}

	if (flag4) {
		continue;
	}
	std::string regex_string5 = "^";
	std::pair<bool, int> pair5 = is_defined("\\(");
	if (pair5.first) {
		regex_string5 += (char) pair5.second;
	} else {
		regex_string5 += "\\(";
	}

	regex_string5 += "$";
	std::regex regex5(regex_string5);
	bool flag5 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex5) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{return LPAREN;};
			flag5 = true;
			break;
		}
	}

	if (flag5) {
		continue;
	}
	std::string regex_string6 = "^";
	std::pair<bool, int> pair6 = is_defined("\\)");
	if (pair6.first) {
		regex_string6 += (char) pair6.second;
	} else {
		regex_string6 += "\\)";
	}

	regex_string6 += "$";
	std::regex regex6(regex_string6);
	bool flag6 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex6) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{return RPAREN;};
			flag6 = true;
			break;
		}
	}

	if (flag6) {
		continue;
	}
	std::string regex_string7 = "^";
	std::pair<bool, int> pair7 = is_defined("<<EOF>>");
	if (pair7.first) {
		regex_string7 += (char) pair7.second;
	} else {
		regex_string7 += "<<EOF>>";
	}

	regex_string7 += "$";
	std::regex regex7(regex_string7);
	bool flag7 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex7) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{return ENDOF;};
			flag7 = true;
			break;
		}
	}

	if (flag7) {
		continue;
	}
	std::string regex_string8 = "^";
	std::pair<bool, int> pair8 = is_defined("[ \t\n\r]");
	if (pair8.first) {
		regex_string8 += (char) pair8.second;
	} else {
		regex_string8 += "[ \t\n\r]";
	}

	regex_string8 += "$";
	std::regex regex8(regex_string8);
	bool flag8 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex8) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{};
			flag8 = true;
			break;
		}
	}

	if (flag8) {
		continue;
	}
	std::string regex_string9 = "^";
	std::pair<bool, int> pair9 = is_defined(".");
	if (pair9.first) {
		regex_string9 += (char) pair9.second;
	} else {
		regex_string9 += ".";
	}

	regex_string9 += "$";
	std::regex regex9(regex_string9);
	bool flag9 = false;
	for (int i = 0; i < input.length(); i++) {
		std::string tmp = input.substr(0, input.length() - i);
		if (std::regex_search(tmp, match, regex9) && match.position() == 0) {
			zztext = match.str();
			input = input.substr(zztext.size(), input.size() - zztext.size());
			{throw std::runtime_error("unexpected symbol" + zztext);};
			flag9 = true;
			break;
		}
	}

	if (flag9) {
		continue;
	}
	}
	return -1;
}
