//
// Created by antifrizz on 03.12.18.
//

#ifndef BISON_EXPRESSION_H
#define BISON_EXPRESSION_H


#include <string>
#include <iostream>
#include <set>
#include <map>
#include <utility>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>

const int MAX = 100;

class my_int{
private:
    int value;
public:
    void set(int value) {
        this->value = value;
        printf("set value %d\n", value);
    }
};

struct expression_struct {
    std::string string_value;
    int type = -1;
};

#endif //BISON_EXPRESSION_H
