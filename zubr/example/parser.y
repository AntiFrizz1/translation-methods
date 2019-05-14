%{
	#include "header.h"
	#include <cstdio>
	#include <cstdlib>
	#define ZZTYPE expression_type*
%}

%token NUMBER PLUS MINUS TIMES DIVIDE LPAREN RPAREN ENDOF

%start main

%%
main
	: expression ENDOF {
		printf("%d\n", $1->value);
		exit(0);
	}
	| {
		printf("%d\n", 0);
		exit(0);
	}
	;

expression
	: expression1 expression2 {
		expression_type* first = $1;
		expression_type* second = $2;
		second->numbers.push_front($1->value);
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
		$$ = ans;
	}
	;

expression2
	: PLUS expression1 expression2 {
		$3->numbers.push_front($2->value);
		$3->operations.push_front('+');
		$$ = $3;
	}
	| MINUS expression1 expression2 {
		$3->numbers.push_front($2->value);
		$3->operations.push_front('-');
		$$ = $3;
	}
	| {
		$$ = new expression_type;
	}
	;

expression1
	: factor expression4 {
		expression_type* first = $1;
		expression_type* second = $2;
		second->numbers.push_front($1->value);
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
		$$ = ans;
	}
	;

expression4
	: TIMES factor expression4 {
		$3->numbers.push_front($2->value);
		$3->operations.push_front('*');
		$$ = $3;
	}
	| DIVIDE factor expression4 {
		$3->numbers.push_front($2->value);
		$3->operations.push_front('/');
		$$ = $3;
	}
	|{
		$$ = new expression_type();
	}
	;

factor
	: NUMBER {
		$$ = $1;
	}
	| MINUS factor {
		$$ = $2;
		$$->value = -$$->value;
	}
	| LPAREN expression RPAREN {
		$$ = $2;
	}
	| PLUS factor {
		$$ = $2;
	}
	;
