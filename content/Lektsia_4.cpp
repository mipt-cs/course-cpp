Лекция 4.
Функции.

1946, 

RT f(AL);
RT -> auto

Функция должна быть короткой.
Избегать функций Бога.
printf -- C

//=============================

void f(int); // forward-declaration

int main()
{
	// f
}

void f(int i)
{
	int result = ...;
	//
	return result; // return-statement
}

int fact(int n)
{
	return (n > 1) ? n * fact(n - 1) : 1;
}

// =======================================

int f(int i) // может но не обязан
{
	return ++i;
}

// где-то далее используем

int result = f(5);

// Функции-члены классов, которые определены
// в классе являются inline

constexpr int f(int i)
{
	retur i + 1;
}

void g(int n)
{ // compile-time (CT)
	int a = f(5); // может быть вычислено CT
	int b = f(n); // только run-time
	constexpr int c = f(6); // только CT
	constexpr int d = f(n); // error

	// use c
}

[[...]] -- атрибут

[[noreturn]] void f()
{
	/// ...
	return;
}

void f(int & a)
{
	static int value = 0; // уничтожение при зав. прог.
	std::cout << ++value << ' ' << ++a;
}

// используем
int a = 0;
f(a); f(a); f(a);

1 2
2 2
3 2

Повторить передачу:
по указателю
по ссылке
по значению
// const

// ========================================

Аргументы по умолчанию

f(my_arg, arg = 0); // python

f(x,y,z)

void f(int x, int y = 0, int z = 0)
{ // trailing arguments

}

f(1, 2, 3);
f(1, 2);
f(2);

void f(int, char * = nullptr);
// правило максимального куска

void f(int x, char * ptr = nullptr)
{
	//
}

a *= b; // a = a * b

a+++++1; // error
(++a) = 1;
a++ = 1;

// =================================

Перегрузка функций.
Полиморфизм.

void print(int);
void print(const char *); // для строк
void print(double);
void print(long);
void print(char);

void print(int)
{
	// 1
}

void print(double)
{
	// 2
}
// где-то используем
print(5.0f);
char c;
int i;
short s;
float f;
print(c, i, s, f);
print('a', 49, "a", nullptr)

void f(int a, int b);
void f(double a, double b);
void f(int a, double b);

f(2.0, 2);

// =========================================

Пре- и постусловия

int area(int a, int b)
{
	// предусловия a>0 && b>0
	// постусловия result > 0 && result == a*b
	return a * b;
}

area(INT_MAX, 1000); // это на совести пользователя

// ===========================================

Указатели на функции.

void error(std::string s) {...}

void(*ptr)(std::string); // int * ptr;

void f()
{
	ptr = &error; // ptr = error;
	(*ptr)("error"); // ptr("error");
}

STL algorithms

sort(..., ..., predikat)
{
	predikat(..., ...);
}

bool compare(..., ...) {...}

sort(..., ..., compare)

// Лямбды
// Указатели на ф-ии
// Функциональные объекты

int x;
MyType x; // если определен оператор ()
x(a,b,c);

// =========================================
Макросы -- не использовать

#define VALUE 1000
#define max(a,b) (a>b)?a:b;

int x = VALUE;

int i = 0;
std::cout << max(++i, ++i);
std::cout << (++i > ++i) ? ++i : ++i;

C -- макросы используются;

#include <...>

__DATE__
__TIME__
__FILE__
__LINE__
__FUNC__ / __FUNCTION__

void f()
{
	std::cout << __FUNCTION__;
}

#include <cassert>

void f(int i)
{
	assert(i > 0);
	// ...
}

// debug
// release

// ===========================================

void print() {};

int main()
{
	int print();
	// {}
	// = {}
	// = 
	// ()
	// print

	std::cout << print() << std::endl;
}

memcpy
strlem
strcmp
strncmp

class Data
{
public:
	Data(int v) : value(v) {}
	Data & operator=(const Data & d) = default;
	// оператор коп. присв.
	Data(const Data & d) = default;
	// коп. конструктор
private:
	int value;
};


Data d1(10), d2(20);

Data d3(d1);

// d3.value == d1.value

d2 = d1; // d2.value = d1.value;

std::vector < int > v;
std::vector < int > b;
std::copy(v.begin(), v.end(), b.begin());
std::sort
std::for_each
std::transform






