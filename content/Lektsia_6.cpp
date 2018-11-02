Лекция 6
(basic facilities, исключения -- на потом)

Пространства имен, раздельная компиляция.
Модульность -- это важно и хорошо.
Модули -- функция, класс, ...

Приложени
- граф.либ.
- работа с текстом
class Text; // Graph_lib::Text ...
std::vector

Text_box
gl_text, gl_shape

namespace MyNS
{
	int f();
	double g(int);
	class X;
}

int x = MyNS::f();

// ===================================

int f() {...};

int g()
{
	int f;
	f(); // error
	::f();
}

// =====================================

using-declaration
std::string
void g()
{
	using std::string;
	using std::cout;

	cout << ...

	// string вместо std::string
}

// ==========================================

namespace NS0
{
	void f(int);
	void f(char);
	void f(double);
}

NS0::f(1.0); // double

using NS0::f;

f(3), f('a'), f(1.0);

int global = 0;

void g()
{
	// есть что-то

	{
		int x;
		using std::string;
		// code
		// x уничтожается
	}

	// code
}

// =====================================

using namespace std;
using namespace NS0;

// =====================================

ADL (argument dependent lookup)
-- поиск зависящий от аргумента, поиск Кёнига

namespace NS1
{
	struct S;
	void f(S);
}

NS1::S s;
f(s); // поиск Кенига

// ===================================

namespace NS2
{
	// определим этот другой тип
	void f(другой тип); // 
}

void f(используем похожий на другой тип);

int main()
{
	f(объект );
}

// ==============================================

namespace A
{
	void f();
}

// code -- только A::f

namespace A
{
	void g();
}

// code -- и A::f и A::g

using std::string;
using MYNS::string;
string str;

// ===============================

namespace Very_Long_Name_Namespace
{

}

namespace VRNN = Very_Long_Name_Namespace;
// псевдоним -- alias

namespace A
{
	namespace B
	{
		void f()
	}
}

A::B::f();

// =========================================
Раздельная компиляция.
#include <vector> // vector.hpp#

#define

#ifdef 
...
#else
...
#endif

#pragma

юнит трансляции(translation unit)

.cpp, выполняется препроцессинг->юнит трансляции

#include <vector>
#include "MyFile.hpp"

1) Во - первых -- сторонние библиотеки
2) Трудно работать с большим текстом
3) Разбиение на кокретные подзадачи
4) Повторное использование
5) Разделение интрефейса и реализации

Модуль 1
что - то, например, ф - я
-интерфейс
-реализация
Модуль 2
std::sort(..., ..., ...);
использовать именно интерфейс
Модуль 3
использовать именно интерфейс

.hpp < - интрефейс; // file.hpp
.cpp < - реализация; // file.cpp

// file.cpp
class MyClass
{
public:
	void some_func();
};
void MyClass::some_func()
{

}

.cpp main
#include "file.hpp"

// ===========================================

ODR -- one definition rule

file1.cpp
struct S { int a, int b; };

file2.cpp
struct S { int a, int bb; };

// =============================================

file.hpp
// #pragma once // MS VS
#ifndef FILE_HPP
#define FILE_HPP
//code
#endif

#ifdef FILE_HPP
// code
#elif !defined(MY_FILE)
//code
#endif




