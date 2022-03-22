#pragma once

// Comp.h
// в данном файле реализован класс Comp для работы с комплексными числами


#include <iostream>
#include <cmath>


// определение числа пи
#define MATH_PI 3.14159265358979 


// определим макросы для большей читабельности
#define COMP_0 Comp(0.0, 0.0)
#define COMP_1 Comp(1.0, 0.0)
#define COMP_J Comp(0.0, 1.0)


// определим тригонометрические функции от градусов, чтобы в дальнейшем не работать с радианами
// в данном случае актуален спецификатор inline, чтобы встраивать функции при компиляции и не вызывать их каждый раз
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


// нормализация угла необходима, чтобы аргумент комплексного числа не выходил за пределы (-180°; 180°]
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


// функция получение аргумента комплексного числа по его алгебраическим параметрам 
// одной функции atan_deg() не достаточно, т.к. она возвращает значения [-90; 90], а аргумент комплексного числа принимает (-180°; 180°]
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


// флаг конструктора экспоненциальной формы
enum ExpForm
{
	EXP_FORM
};


// класс комплексное число (Comp сокращенно от Complex)
class Comp
{
private:
	double x;		// действительная часть
	double y;		// мнимая часть
	double r;		// модуль
	double f;		// аргумент

	// объявляем статическую переменную – флаг формы ввывода комплексного числа
	static int coutForm;

public:
	
	// уонструкторо алгебраической формы
	Comp(double x, double y)
		: x(x)
		, y(y)
	{
		r = sqrt(x * x + y * y);
		f = arg_deg(x, y);
	}

	// конструктор экспоненциальной формы
	// третий параметр лишь является указанием конструктора экспоненциальной формы
	Comp(double r, double f, int)
		: r(r)
		, f(f)
	{
		x = r * cos_deg(f);
		y = r * sin_deg(f);
	}

	// конструктор от одного параметра (конструктор преобразования) – комплексное число из одной действительной части
	// нужен для выполнения неявных преобразований типа к Comp 
	Comp(double x) 
		: x(x)
		, y(0.0)
	{
		r = sqrt(x * x + y * y);
		f = arg_deg(x, y);
	}

	// конструкотр по умолчанию
	Comp()
		: x(0.0)
		, y(0.0)
		, r(0.0)
		, f(0.0)
	{}

	// следующие перегруженные операции отражают логику математических операций над комплексными числами
	// их реализация позволит не задумываться о преобразованиях форм и сразу получать результат
	// бинарные операции реализованы через friend методы, что позволит неявно приводить любой из параметров операции к комплексному числу

	// операция отрицания
	Comp operator-()
	{
		return Comp(-x, -y);
	}

	// операция комплексной сопряжённости
	Comp operator*()
	{
		return Comp(x, -y);
	}

	// операция суммы
	friend Comp operator+(const Comp& comp1, const Comp& comp2)
	{
		Comp res = comp1; // присваиваем результату первый параметр и к нему прибавляем второй

		res.x += comp2.x;
		res.y += comp2.y;
		res.r = sqrt(res.x * res.x + res.y * res.y);
		res.f = arg_deg(res.x, res.y);

		return res;
	}

	// операция суммы с присваиванием
	Comp& operator+=(const Comp& comp)
	{
		x += comp.x;
		y += comp.y;
		r = sqrt(x * x + y * y);
		f = arg_deg(x, y);

		return *this;
	}

	// операция разности
	friend Comp operator-(const Comp& comp1, const Comp& comp2)
	{
		Comp res = comp1;

		res.x -= comp2.x;
		res.y -= comp2.y;
		res.r = sqrt(res.x * res.x + res.y * res.y);
		res.f = arg_deg(res.x, res.y);

		return res;
	}

	// операция суммы с присваиванием
	Comp& operator-=(const Comp& comp)
	{
		x -= comp.x;
		y -= comp.y;
		r = sqrt(x * x + y * y);
		f = arg_deg(x, y);

		return *this;
	}

	// операция умножения
	friend Comp operator*(const Comp& comp1, const Comp& comp2)
	{
		Comp res = comp1;

		res.r *= comp2.r;
		res.f += comp2.f;
		norm_deg(res.f); // нормализируем угол
		res.x = res.r * cos_deg(res.f);
		res.y = res.r * sin_deg(res.f);

		return res;
	}


	// операция суммы с присваиванием
	Comp& operator*=(const Comp& comp)
	{
		r *= comp.r;
		f += comp.f;
		norm_deg(f); // нормализируем угол
		x = r * cos_deg(f);
		y = r * sin_deg(f);

		return *this;
	}


	// операция деления
	friend Comp operator/(const Comp& comp1, const Comp& comp2)
	{
		Comp res = comp1;

		res.r /= comp2.r;
		res.f -= comp2.f;
		norm_deg(res.f); // нормализируем угол
		res.x = res.r * cos_deg(res.f);
		res.y = res.r * sin_deg(res.f);

		return res;
	}

	// операция суммы с присваиванием
	Comp& operator/=(const Comp& comp)
	{
		r /= comp.r;
		f -= comp.f;
		norm_deg(f); // нормализируем угол
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

	// методы получения значений параметров комплексного числа
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

	// операция помещения в поток вывода
	// вывод происходит в экспоненциальной форме
	friend std::ostream& operator<<(std::ostream& os, const Comp& comp)
	{
		// выбор формы вывода
		if (Comp::coutForm)
		{
			// проверяем параметры на отличие от нуля, чтобы не выводить нулевые значения
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

int Comp::coutForm = 1; // определение статической переменной