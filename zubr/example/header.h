#include <deque>
#include <string>

struct expression_type {
    std::string string_value;
    int value;
    std::deque<int> numbers;
    std::deque<char> operations;
};