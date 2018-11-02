указатели
массивы

встроенные средства->низкоуровневые

STL - высокоуровневые и безопасные
smart pointers
контейнеры

T*->указатель на T

char c = 'a'; // &c - взятие адреса
char * ptr = &c;

*ptr - разыменование

char ch = *ptr;

void* -очень низкоуровневое средство

void * allocate(size_t n)

// int * <-> void *

int * ptr_1;
void * ptr_2;
ptr_1 = (int*)allocate(10); // ок

ptr_2 = ptr_1; // нельзя

nullptr

T[size]

float f[3]; // индексация с 0

f[0] f[1] f[2]

vector->используйте по умолчанию
array->фикс.размера

std::string s = "hello";

int * ptr = new int(10);

int global = 4;

int main()
{
	static int x;
}

void f()
{
	int y;
	int z;

	static int val = 5;
	val = val + 1;
	val = y;
}

динамическая память - undefined
стек - то же самое
глобальная - 0

int * ptr = new int(5);

delete ptr;

int n;
std::cin >> n;

float * arr = new float[n];

delete[] arr;

arr++;

x = x + 1; ->x++; ++x;

int v1[8] = { 1,2,3,4 };
char v2[] = { 'a', 'b', 'c', 0 };

"hello" '\0' -> 0

char v3[2] = { 'a', 0 };


char str[] = "hello\\";

Raw strings

std::string s = R"(abc\de\nf)"; // regular expr

int v[] = { 1,2,3,4 };
int * p = v;
int * p = &v[0];

int * p_1 = v - 1;
int * p_2 = v + 7;
int * p_3 = v + 2; // ок

size_t size = 10;



void compute(int * arg, size_t size)
{
	...
}


int v[] = { 1,2,3,4 };

compute(v, 4);

int mult[10][10][10]; // STL -> Matrix

int * ptr;

ptr++;

const int * ptr;

int * const ptr;

const int * const ptr;

ptr++ ++ptr

ptr-- --ptr

ptr += 8;

ptr -= 6;


int n = ptr_1 - ptr_2;

int main()
{
	int x;
	f(5); // передача по ссылке

	const int & ref = x;

	ref++;

	int & ref = x;
	const int & ref = 5;
}
	
// ссылка - это авт. разыменуемый при 
// использовании указаель T * const ptr

// ссылка - это псевдоним имени














