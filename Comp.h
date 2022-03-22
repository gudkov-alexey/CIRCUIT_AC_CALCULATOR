#pragma once

// Comp.h
// � ������ ����� ���������� ����� Comp ��� ������ � ������������ �������


#include <iostream>
#include <cmath>


// ����������� ����� ��
#define MATH_PI 3.14159265358979 


// ��������� ������� ��� ������� �������������
#define COMP_0 Comp(0.0, 0.0)
#define COMP_1 Comp(1.0, 0.0)
#define COMP_J Comp(0.0, 1.0)


// ��������� ������������������ ������� �� ��������, ����� � ���������� �� �������� � ���������
// � ������ ������ �������� ������������ inline, ����� ���������� ������� ��� ���������� � �� �������� �� ������ ���
inline double atan_deg(double value)
{ 
	return atan(value) / MATH_PI * 180.0;
}

inline double cos_deg(double angleDeg)
{
	return cos(angleDeg / 180.0 * MATH_PI);
}

inline double sin_deg(double angleDeg)
{
	return sin(angleDeg / 180.0 * MATH_PI);
}


// ������������ ���� ����������, ����� �������� ������������ ����� �� ������� �� ������� (-180�; 180�]
inline void norm_deg(double& angleDeg)
{
	while (angleDeg > 180.0)
	{
		angleDeg -= 360.0;
	}
	
	while (angleDeg <= -180.0)
	{
		angleDeg += 360.0;
	}
}


// ������� ��������� ��������� ������������ ����� �� ��� �������������� ���������� 
// ����� ������� atan_deg() �� ����������, �.�. ��� ���������� �������� [-90; 90], � �������� ������������ ����� ��������� (-180�; 180�]
inline double arg_deg(double x, double y)
{
	if (x < 0.0)
	{
		if (y > 0.0) return atan_deg(y / x) + 180.0;
		else return atan_deg(y / x) - 180.0;
	}
	else if (fabs(x) < 1.0e-15)
	{
		if (y > 0.0) return 90.0;
		else return -90.0;
	}
	return atan_deg(y / x);
}


// ���� ������������ ���������������� �����
enum ExpForm
{
	EXP_FORM
};


// ����� ����������� ����� (Comp ���������� �� Complex)
class Comp
{
private:
	double x;		// �������������� �����
	double y;		// ������ �����
	double r;		// ������
	double f;		// ��������

	// ��������� ����������� ���������� � ���� ����� ������� ������������ �����
	static int coutForm;

public:
	
	// ������������ �������������� �����
	Comp(double x, double y)
		: x(x)
		, y(y)
	{
		r = sqrt(x * x + y * y);
		f = arg_deg(x, y);
	}

	// ����������� ���������������� �����
	// ������ �������� ���� �������� ��������� ������������ ���������������� �����
	Comp(double r, double f, int)
		: r(r)
		, f(f)
	{
		x = r * cos_deg(f);
		y = r * sin_deg(f);
	}

	// ����������� �� ������ ��������� (����������� ��������������) � ����������� ����� �� ����� �������������� �����
	// ����� ��� ���������� ������� �������������� ���� � Comp 
	Comp(double x) 
		: x(x)
		, y(0.0)
	{
		r = sqrt(x * x + y * y);
		f = arg_deg(x, y);
	}

	// ����������� �� ���������
	Comp()
		: x(0.0)
		, y(0.0)
		, r(0.0)
		, f(0.0)
	{}

	// ��������� ������������� �������� �������� ������ �������������� �������� ��� ������������ �������
	// �� ���������� �������� �� ������������ � ��������������� ���� � ����� �������� ���������
	// �������� �������� ����������� ����� friend ������, ��� �������� ������ ��������� ����� �� ���������� �������� � ������������ �����

	// �������� ���������
	Comp operator-()
	{
		return Comp(-x, -y);
	}

	// �������� ����������� ������������
	Comp operator*()
	{
		return Comp(x, -y);
	}

	// �������� �����
	friend Comp operator+(const Comp& comp1, const Comp& comp2)
	{
		Comp res = comp1; // ����������� ���������� ������ �������� � � ���� ���������� ������

		res.x += comp2.x;
		res.y += comp2.y;
		res.r = sqrt(res.x * res.x + res.y * res.y);
		res.f = arg_deg(res.x, res.y);

		return res;
	}

	// �������� ����� � �������������
	Comp& operator+=(const Comp& comp)
	{
		x += comp.x;
		y += comp.y;
		r = sqrt(x * x + y * y);
		f = arg_deg(x, y);

		return *this;
	}

	// �������� ��������
	friend Comp operator-(const Comp& comp1, const Comp& comp2)
	{
		Comp res = comp1;

		res.x -= comp2.x;
		res.y -= comp2.y;
		res.r = sqrt(res.x * res.x + res.y * res.y);
		res.f = arg_deg(res.x, res.y);

		return res;
	}

	// �������� ����� � �������������
	Comp& operator-=(const Comp& comp)
	{
		x -= comp.x;
		y -= comp.y;
		r = sqrt(x * x + y * y);
		f = arg_deg(x, y);

		return *this;
	}

	// �������� ���������
	friend Comp operator*(const Comp& comp1, const Comp& comp2)
	{
		Comp res = comp1;

		res.r *= comp2.r;
		res.f += comp2.f;
		norm_deg(res.f); // ������������� ����
		res.x = res.r * cos_deg(res.f);
		res.y = res.r * sin_deg(res.f);

		return res;
	}


	// �������� ����� � �������������
	Comp& operator*=(const Comp& comp)
	{
		r *= comp.r;
		f += comp.f;
		norm_deg(f); // ������������� ����
		x = r * cos_deg(f);
		y = r * sin_deg(f);

		return *this;
	}


	// �������� �������
	friend Comp operator/(const Comp& comp1, const Comp& comp2)
	{
		Comp res = comp1;

		res.r /= comp2.r;
		res.f -= comp2.f;
		norm_deg(res.f); // ������������� ����
		res.x = res.r * cos_deg(res.f);
		res.y = res.r * sin_deg(res.f);

		return res;
	}

	// �������� ����� � �������������
	Comp& operator/=(const Comp& comp)
	{
		r /= comp.r;
		f -= comp.f;
		norm_deg(f); // ������������� ����
		x = r * cos_deg(f);
		y = r * sin_deg(f);

		return *this;
	}

	Comp& operator=(const Comp& comp)
	{
		x = comp.x;
		y = comp.y;
		r = comp.r;
		f = comp.f;
		
		return *this;
	}

	// ������ ��������� �������� ���������� ������������ �����
	inline double getX() const
	{ 
		return x;
	}
	inline double getY() const
	{ 
		return y;
	}
	inline double getR() const
	{ 
		return r;
	}
	inline double getF() const
	{ 
		return f;
	}

	friend std::ostream& alg(std::ostream& os)
	{
		Comp::coutForm = 1;

		return os;
	}

	friend std::ostream& exp(std::ostream& os)
	{
		Comp::coutForm = 0;

		return os;
	}

	// �������� ��������� � ����� ������
	// ����� ���������� � ���������������� �����
	friend std::ostream& operator<<(std::ostream& os, const Comp& comp)
	{
		// ����� ����� ������
		if (Comp::coutForm)
		{
			// ��������� ��������� �� ������� �� ����, ����� �� �������� ������� ��������
			if (fabs(comp.r) > 1.0e-5)
			{
				os << comp.r;

				if (fabs(comp.f) > 1.0e-5)
				{
					os << "e^";
					os << comp.f << 'j';
				}
			}
			else os << "0";
		}
		else
		{
			if (fabs(comp.x) > 1.0e-5) os << comp.x;
			else os << "0";

			if (fabs(comp.y) > 1.0e-5)
			{
				os.setf(std::ios::showpos);
				os << comp.y << 'j';
				os.unsetf(std::ios::showpos);
			}
			else os << "+0j";
		}

		return os;
	}
};

int Comp::coutForm = 1; // ����������� ����������� ����������