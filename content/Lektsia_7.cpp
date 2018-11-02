������ 7.
- ����������� ������
- ��������� + move
- ������������ � ��������
1. inline namespace
2. namespace { ... } -��������
3. ���� �������� -- external � internal linkage
extern int x; -- external
const int x; -- internal
4. ���������� �������� ����� ��������!
������� ������������� ����. ����. �������� �� ���������.

������ -- UDT
float, +, = ...

����������� ����������. 

���������: ������ ���� ����� ������ �������� �����������.

1. UDT
2. members (������ � �������)
3. ����� - ��� namespace
4. creation, destruction, copy, move
5. ., ->
6. operator overloading
7. public|private, ������������
������������, �����������, ������������
8. struct VS class

// ================================================
7-8

class X
{
private:
	int m;
public:
	X(int i) : m{ i } {}

	void f()
	{
		m++;
	}
private:
	int m_2;
};

// protected

X x;
x.m;
x.f();

// =============================================
4 - copy

X x1(6);
X x2(7);
x1 = x2;
X x3(x2);

����������� �-�� �����:
1) �-� �� ���������
1.1) ���� ��������� �-�, �� ����. �� �����. �� �����.
2) �������� ����������� ������������
3) ����������� �����������
4) �������� ������������� ������������ - move
5) ����������� ����������� - move
6) ����������

class X
{
public:
	int x;
};

// ================================================

1. �������� �-�� � �-� �� ���������
2. ���������� -- ������ RAII

class X // ������ RAII
{
	int * ptr;
	int m;
	X() : m(0)
	{
		ptr = new int[10];
	}
	~X()
	{
		delete[] ptr;
	}
};

{
	X x;
	x.ptr ...

		int * ptr = new (p) int[10];

		x.~X(); // placement new
}

// =================================================
������� ���������� � - ��(� - ��)

X x;

1. -- ���������� �-�� ������� �������
2. ������������� ������ ������
3. ����������� ���� �-��

�����������:

1. ����������� ���� �-��
2. ����������� ������ ������
3. -- ���������� �-�� ������� �������

// ===================================================

class X
{
	const int c;

	int x;
	int y {1000}; // �������� �� ���������
	X(int a, int b) : x(a), c(100) { c = 100; }
	X(...) : ... {}
};

X x(1, 2);
X x { 1, 2 };
X x {}; // �-� �� ���������

! �-� � initializer_list ����� ��������� !

// ===================================================

initializer_list

auto v = { 1 }; // std::initializer_list<int>

{} --�������� ������������� ������

X x{}; // X x(); -- ���������� �-��

std::vector < int > v(10); // ����� �-�� ������. int
std::vector < int > v{10}; // ����� �-�� ������. list

vector(std::initializer_list < T > list)
{
	for (auto x : list)
	{
		std::cout << x;
	}
}

// ===================================================

class X
{
	void f() // �� ��������� inline
	{
		return m;
	}

	void g();
};

void X::g()
{
	...
}

// ===================================================

explicit �����������

class X
{
	static int s;
	int x;
	int y;
	explicit X(int a, int b) : x(a), y(b) 
	{
		s = 100;
	}
};

X x(10);
X y(20);
X x = { 10,10 }; // ������� ����� �-��

std::vector < int > v = 10;

x.s = 100;
y.s == 100;

X::s = 100;

Comlex c = 10; // �����������

// ===================================================

��������� this

class X
{
	int m;

	void f([�������� ������])
	{
		[���� ���������� ������].m++;

		this // ��� ��������� �� ������ ������

			(this->m)++;

	}

	X & operator=(const X & x)
	{
		// ��������� �����������
		m = x.m;

		return *this;
	}
};

a = b; // (a.operator=(b)).f()
(a=b)++;

X x;
x.f(); // ���� ������� 1-�� �������� � f