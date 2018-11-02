С++ - компилируемый ЯП общего назначения

C++ низкоур.высокоур.

C++ мультипарадигменный

int x;
double y;

x=y+f(2)

x,y,f - имена

имена должны быть объявлены

объявление - тип и имя
определение - имя тип и  доп. инф

Фундаментальные типы:
1) boolean или bool
2) character - char
3) integer - int
4) floating point types
5) void - пустой тип
6) pointer
7) array
8) reference
9) class / struct
10) enumeration - для набора констант

1, 2, 3 - integral
1, 2, 3, 4 - arithmetic
9, 10 - UDT -- user - defined types

bool greater(int x)
{
	bool result;
	result = x > 5;
	return result;
}

int -> bool
0 -> false
1, 2, 3... -> true

bool x = 0;

|| && ^

bool res = x && y;

nullptr

int * ptr = nullptr;

bool x = ptr;

nullptr -> false
real ptr -> true

char c = 't';

ASCII

signed char =
unsigned char =

wchar_t
w - wide - Unicode

cp1251->Windows

UTF - 8, 16, 32

char16_t
char32_t

'a' - 97
'0' - 48

'\n' - enter
'\t' - tab
'\\' - \
'\'' - апостроф

'0' - 48 - '\60' - '\x30'

int->по умолчанию signed

signed int
unsigned int

short, long, long long

#include <cstdint> // cstdint.h

std::int32_t x = 5;
std::uint64_t y;

Проблема переносимости.

const int x = 5;

float, double, long double

1.2345E-5

xxx - yyyyzzzuuuuzzzyyyy - xxxx

const unsigned int res = 5L;

float x = 1.0F;

void f(int)
void *

1 = char <= short <= int <= long <= ll
1 <= bool <= long
char <= wchar_t <= long
float <= double <= long double

unsigned signed const constexpr
const - константа времени выполнения
constexpr - константа(не только) времени компиляции

#include <...>

size_t - unsigned int

double int_size = sizeof(int); 4 -> 4.0
typeid(int_size).name() -> "double" отдельно:'t' 'a' 'o'

int x = 4.5; // x == 4

В имени объекта можно исп.буквы цифры и _

имя не с цифры

alignment - выравнивание

alignof(int) -> 4
alignas(12)

auto x = 5; // int x = 5; не int32_t

int x = 5; --классика
int x(5); --для UDT

int x{ 5.5 }; -- хорошо

{ 5,6,7 } - initializer_list

auto x = 3;

{} - унифицированная фигурная инициализация


