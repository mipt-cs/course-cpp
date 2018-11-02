Лекция #5.
Структуры, объединения, перечисления.

см.лекцию 1 - классификация типов
9 - 10 --UDT(сегодня раздел 9)

struct --набор элементов(членов);
struct --упрощенный класс;

union, enum (enumeration);

struct Address
{
	const char * street; // поля структуры
	int number; // данные-члены
	char state[2]; // = {'N', 'J'}
}; // стоит ;

Структура отличается от массива[]
int array1[5];

Address v; // экземпляром Address
v.number = 5;
std::cout << v.number << std::endl;

std::string str = "street";
const char * c_str = "street"; // C-style

Address v = { "street", 13, "NJ" };

Address * ptr = &v;

(*ptr).number = 10;
ptr->number = 10;

Передавать в функции (через указатель, ссылку)
Возвращать в return-st

alignment -- выравнивание

struct V
{
	char a; // по 4 байтам
	int b; // по 4 байтам
	char c; // по 4 байтам
};

sizeof(V) -> 12

struct V alignas(6)
{
	char a; // по 4 байтам
	char c;
	int b; // по 4 байтам
};

alighof // низкоуровневые ср-ва
alignas // низкоуровневые ср-ва

std::vector

struct Node
{
	Node * parent;
	Node * right_child;
	Node * left_child;
	int data;
};

struct Node; // forward declaration

struct Tree
{
	Node * root;
};

struct Node
{
	Node * parent;
};

class B;

class A
{
	B * b;
	A() : b(new B(1,2,3))
	{
	}
};

class B
{
	A a;
};


int x = 10; // 1 вариант к-ра (со списком иниц.)
int x;
x = 10; // 2 вариант к-ра
struct S
{
	int m_x; // 1 здесь
	int m_y; // 2 здесь
	S(int x, int y) : m_y(y), m_x(x) // список иниц.
	{
		// m_x m_y уже сущ. и проинициализированы
	}
	S()
	{
		std::cout << "created" << std::endl;
	}
};
S point = {3,4}; // проверить правильность
int x = { 5 };
S point(3, 4);
S point;
// S point(); -- плохо
int x;

// ==========================================

Структуры и массивы.
S points[] = { {1,2},{3,4},{5,6} };
int x2 = points[2].m_x;

std::array <Point, 3> arr;
arr[0] = point;

// ==============================================

struct Point // POD
{
	int x;
	int y;
	int z;
};

void f(Point * p)
{
	point->x = ..
}

// ===============================================

POD -- Plain Old Data(старые добрые данные)

#include <type_traits>
std::cout << std::is_pod_v < Point > << std::endl;

// ===============================================

Поля -- к структурам

struct S // R6000 Phys. Page Number
{
	unsigned int PFN : 22;
	int : 3; // заглушка
	unsigned int CCA : 3;
	bool x : 1;
	bool y : 1; // битовое поле
	bool z : 1;
	bool a : 1;
}; // низкоуровневое средство

// ==============================================

union some_u
{
	char c;
	int i;
	long l;
};

sizeof(some_u) == 8

some_u u;
u.c = 'a'; // 97

//...

u.l == 97;

// нужны дополнительные проверки

// ================================================

enum class Color {red,green,blue};
// red == 0
// green == 1
// blue == 2
Color a1 = 7; // error
int a2 = green; // error
int a3 = Color::green; // error
Color a4 = Color::green; // ok

//enum Color { red, green, blue };
//
//Color a0 = 1; // ok
//Color a1 = 7; // error
//int a2 = green;
//int a3 = Color::green;
//Color a4 = Color::green;


Color var = Color::green;

// ... 

switch (s.mark)
{
case Color::red:
	// s
	break;
case Color::green;
	break;
}

struct S
{
	// ...
	Color mark;
};

S s;
s.mark = Color::green;

#include <cstdint>

enum Color // : std::uint8_t
{
	red, // 0 - int
	green, // 1
	blue // 2
};

enum class Traffic
{
	red,
	yellow,
	green
};

Traffic a = Traffic::yellow;

int x = ...(Traffic::yellow);

// =================================================