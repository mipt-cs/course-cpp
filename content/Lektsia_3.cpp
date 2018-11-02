Лекция #3

Утверждения, выражения, операции.
(statements, expressions, operations)

declaration -- утверждение
= () int x; int x = 0;

a = b + c; -- утв

if(a == b) -- if-statement

statement:
1) declaration;
2) expression;
3) {st-list} // compound st
4) break;
5) continue;
6) return expr;
7) selection
8) iteration
9) case const_expression :
10) default :
	11) goto identifier;
	12) id - er: st;

	selection
		if (condition) st
			if (c) st else st

				Тернарный оператор
				(condition) ? (st) : (st);

	if (c) st else st;

	&& ||
		A && B == true
		A || B || C || D

		int x;
	std::cin >> x;

switch (x)
{
case 10: // const (integral), один раз повтор
	...;
	break;
case 33:
	...break;
default:
	...break;
}

Перемаенную надо объявлять и использовать
в наим. обл. видимости.

if (double d = f(...))
{
	do_smth(d);
}
else
{
	do_smth_else(d);
}

d -- error

while ()
do - while
for (2 шт.)

Range - based for - цикл for основ.на диапазоне


std::vector<int > v;

int arr[10];

for (const int & x: v)
{
	std::cout << x;
	x++;
}

for (int i = 0; i < n; i++);

	for (std::size_t i = 0; i < v.size(); ++i);

	int i = 0;
	int x = ++i; // --

for(int i = 0, j = MAX; i < N && j >= 0; i++, j-=2)

do-while

do
{
	...
}
while(...)

while (...)
{
	...
}

	while (true)
		for (;;)

			for (char c; std::cin >> ch;);

	break и continue;

	void f()
	{
		for (;;)
		{
			for (; i < n; i++)
			{
				...
					if (condition) { goto id; }
			}
		}
	id:;
	}


	{
		...
			goto abc;
		...
			abc:
		...
	}

	const int * ptr;
	int * const ptr;

	const int & ref; // int & const ref;

	int main(int argc, char ** argv)
	{
		return 0;
	}

	./a.out 88 kkkk 90

		argc >= 1

		char ** argv -> char * argv[]

		argv[0] == "a.out"
		argv[1] == "77\0"
		argv[3] == "hello"
		argv[argc] = 0

		std::vector < std::string >

Лямбда-функции.

-- анонимный вызываемый функциональный объект

void f() {}

	f()

		T t;
	t();

	int x;
	x!;


	[]() {}


		[] --область захвата
		() --область параметров
	{} --тело

#include <algorithm>
		;
	std::sort(..., ..., [](int x, int y) {return x >= y; });

	std::for_each(..., ..., [](const int & x) {std::cout << x; })

	{
		int x = 0; int y = 0; int z = 0;

		auto lambda = [=](...){... x + 1 ...};

		std::sort(, , lambda);
	}

	{
		bool sensitive = true;

		sort(...,...,[](int x, int y)
		{return sensitive ? x < y : abs(x) < abs(y); })
	}

	[] {} // в мин л-фии можно опустить ()

	// тип лямбд -- closure type
	// lambda_expr346487654378503475374537856
#include <functional>
	int main()
	{
		int x, y, z;
		std::function<void(int, int)> lambda = [](int x, int y) {};
	
		f(lambda);
	}

	void f(std::function ...)

	void f(int i, double x);

	void(int, double) -- тип f

		RT(AL...)

		callback функции
		