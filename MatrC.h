// ���� "matr.h"
// � ������ ����� ������ ����� MatrC ��� ������ � ��������� ����������� �����


// ������ �� ���������� ��������� ������������� �����
#pragma once


// ����������� ����������� ���������
#include "comp.h"

#include <iostream>
#include <iomanip>
#include <initializer_list>

// ����� ����������
// � ������ ��������� ��������� ����������, �.�. ��������� �������� ��� ��������� ���������� ��� �� ��������������
// ����� ���������� ExceptionC �������� ������ � ����������� �� ������
class ExceptionC
{
	const char* info;

public:
	ExceptionC(const char* info) : info(info) {}
	~ExceptionC() = default;

	const char* what() const { return info; }
};


// ������������� ���������� � �������
int powi(int value, int power)
{
	int res = 1;

	while (power--)
	{
		res *= value;
	}

	return res;
}


// ����� �������
class MatrC
{
private:

	int h;				// ������ ������� (���������� �����)
	int w;				// ������ ������� (���������� ��������)
	int n;				// ������� ������� (������ ��� ���������� ������, ����� 0)
	Comp** buffer;		// ����� �������� ������

public:

	// ������������
	MatrC(std::initializer_list<std::initializer_list<Comp>> initList)
		: h(initList.size())				// ������ ������� ����� ������� ������ �������
		, w(initList.begin()->size())		// ������ ������� ����� ������� ���������
		, n(h == w ? h : 0)					// ������� ������� �����������, ������ �� �������� �������
	{
		auto it = initList.begin(); // �������� �������� �� ������ ������

		buffer = new Comp*[h];

		for (int i = 0; i < h; i++)
		{
			if (it[i].size() != w) throw ExceptionC("invalid size"); // ���� � ������� ������ ������ ������, �� ���������� ������

			auto jt = it[i].begin(); // �������� �������� �� ������ i-�� ���������

			buffer[i] = new Comp[w];

			for (int j = 0; j < w; j++)
			{
				buffer[i][j] = jt[j]; // �������� �������� ���������
			}
		}
	}

	MatrC(std::initializer_list<Comp> initList)
		: h(initList.size())			// ������ ������� ����� ������� ������
		, w(1)					// ������ ������� ����� �������
		, n(h == 1 ? 1 : 0)		// � �������-������� ����� ���� ������ ������ ������� ��� �������������
	{
		auto it = initList.begin();

		buffer = new Comp*[h];

		for (int i = 0; i < h; i++)
		{
			buffer[i] = new Comp(it[i]);
		}
	}


	MatrC(int h, int w, Comp value = COMP_0)
		: h(h)
		, w(w)
		, n(h == w ? h : 0)
	{
		buffer = new Comp*[h];

		for (int i = 0; i < h; i++)
		{
			buffer[i] = new Comp[w];

			for (int j = 0; j < w; j++)
			{
				buffer[i][j] = value;
			}
		}
	}

	MatrC(const MatrC& matr)
		: h(matr.h)
		, w(matr.w)
		, n(matr.n)
	{
		buffer = new Comp*[h];

		for (int i = 0; i < h; i++)
		{
			buffer[i] = new Comp[w];

			for (int j = 0; j < w; j++)
			{
				buffer[i][j] = matr.buffer[i][j];
			}
		}
	}

	MatrC()
		: h(0)
		, w(0)
		, n(0)
		, buffer(nullptr)
	{}

	// ����������
	~MatrC()
	{
		for (int i = 0; i < h; i++)
		{
			delete[] buffer[i];
			buffer[i] = nullptr;
		}
		delete[] buffer;
		buffer = nullptr;
	}

	// ����� ����������
	Comp& at(int i, int j)
	{
		if (i >= 0 && j >= 0 && i < h && j < w)
		{
			 return *(*(buffer + i) + j);
		}
		else
		{
			throw ExceptionC("at()");
		}
	}

	// �������������� ��������
	MatrC operator-()
	{
		MatrC matrNeg(h, w);

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				matrNeg.buffer[i][j] = -buffer[i][j];
			}
		}

		return matrNeg;
	}

	MatrC& operator=(const MatrC& matr)
	{
		// ������������� ������� ����� ���� ������� �������, ������� ������ ������ ������� ���������� � �������� �����
		this->clear();

		h = matr.h;
		w = matr.w;
		n = matr.n;
		buffer = new Comp*[h];

		for (int i = 0; i < h; i++)
		{
			buffer[i] = new Comp[w];

			for (int j = 0; j < w; j++)
			{
				buffer[i][j] = matr.buffer[i][j];
			}
		}

		return *this;
	}

	friend MatrC operator+(const MatrC& matr1, const MatrC& matr2)
	{
		// ���� ������� �� ������ �������, �� ���������� ������
		if (matr1.h != matr2.h || matr1.w != matr2.w) throw ExceptionC("operator+");

		MatrC matrSum(matr1.h, matr1.w);

		for (int i = 0; i < matr1.h; i++)
		{
			for (int j = 0; j < matr1.w; j++)
			{
				matrSum.buffer[i][j] = matr1.buffer[i][j] + matr2.buffer[i][j];
			}
		}

		return matrSum;
	}

	friend MatrC operator-(const MatrC& matr1, const MatrC& matr2)
	{
		// ���� ������� �� ������ �������, �� ���������� ������
		if (matr1.h != matr2.h || matr1.w != matr2.w) throw ExceptionC("operator-");

		MatrC matrSub(matr1.h, matr1.w);

		for (int i = 0; i < matr1.h; i++)
		{
			for (int j = 0; j < matr1.w; j++)
			{
				matrSub.buffer[i][j] = matr1.buffer[i][j] - matr2.buffer[i][j];
			}
		}

		return matrSub;
	}

	friend MatrC operator*(const MatrC& matr, const Comp& value)
	{
		MatrC res(matr);

		for (int i = 0; i < res.h; i++)
		{
			for (int j = 0; j < res.w; j++) res.buffer[i][j] *= value;
		}

		return res;
	}

	friend MatrC operator*(const MatrC& matr1, const MatrC& matr2)
	{
		// ���� ������� �� ������������, �� ���������� ������
		if (matr1.w != matr2.h) throw ExceptionC("operator*");

		MatrC matrMul(matr1.h, matr2.w);
		int d = matr1.w;

		for (int i = 0; i < matrMul.h; i++)
		{
			for (int j = 0; j < matrMul.w; j++)
			{
				matrMul.buffer[i][j] = COMP_0;

				for (int k = 0; k < d; k++)
				{
					matrMul.buffer[i][j] += matr1.buffer[i][k] * matr2.buffer[k][j];
				}
			}
		}

		return matrMul;
	}


	friend std::ostream& operator<<(std::ostream& os, const MatrC& matr)
	{
		for (int i = 0; i < matr.h; i++)
		{
			for (int j = 0; j < matr.w; j++)
			{
				os << std::setw(10) << matr.buffer[i][j];
			}
			os << std::endl;
		}

		return os;
	}

	// ����������� ������� �������� ������ �������� ������� 
	friend Comp minor_r(const MatrC& matr, int im, int jm)
	{
		if (matr.n == 0) throw ExceptionC("minor_r()");	// ���� ������� �� ����������, �� ���������� ������

		MatrC matrTemp(matr.n - 1, matr.n - 1);
		int it = 0, jt = 0;

		for (int i = 0; i < matr.n; i++)
		{
			for (int j = 0; j < matr.n; j++)
			{
				if (i != im && j != jm) // ��������� �� ������� mi ������ � mj �������
				{
					matrTemp.buffer[it][jt] = matr.buffer[i][j];
					jt++;

					if (jt == matr.n - 1)
					{
						jt = 0;
						it++;
					}
				}
			}
		}

		// ������� ������������ ���������� ������� �� ������� �������
		if (matrTemp.n > 1)
		{
			Comp det(COMP_0);

			for (int i = 0; i < matr.n - 1; i++)
			{
				det += matrTemp.buffer[i][0] * powi(-1, i) * minor_r(matrTemp, i, 0);
			}

			return det;
		}
		// ������������ ������� �� ������ �������� ����� �� ��������
		else
		{
			return matrTemp.buffer[0][0];
		}
	}

	Comp det()
	{
		if (n == 0) throw ExceptionC("det()");	// ���� ������� �� ����������, �� ���������� ������

		if (n == 1) return buffer[0][0];	// ������������ ������� �� ������ �������� ����� �� ��������

		// ������� ������������ ������� �� ������� �������
		Comp det(COMP_0);

		for (int i = 0; i < n; i++)
		{
			det += buffer[i][0] * powi(-1, i) * minor_r(*this, i, 0);
		}

		return det;
	}

	MatrC rev()
	{
		if (n == 0) throw ExceptionC("rev()"); // ���� ������� �� ����������, �� ���������� ������

		if (n == 1) return MatrC(1, 1, COMP_1 / buffer[0][0]); // �������� ������� �� ������ �������� ����� �� ��������� ��������

		MatrC matrRev(n, n);
		Comp det(this->det());

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				matrRev.buffer[i][j] = powi(-1, i + j) * minor_r(*this, j, i) / det;
			}
		}

		return matrRev;
	}

	MatrC diag()
	{
		if (w != 1) throw ExceptionC("diag()"); // ���� ������� �� �������, �� ���������� ������

		MatrC matrDiag(h, h);

		for (int i = 0; i < h; i++)
		{
			matrDiag.buffer[i][i] = buffer[i][0];
		}

		return matrDiag;
	}

	MatrC transp()
	{
		MatrC matrTransp(w, h);

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				matrTransp.buffer[j][i] = buffer[i][j];
			}
		}

		return matrTransp;
	}

	// ������� �������
	void clear()
	{
		for (int i = 0; i < h; i++)
		{
			delete[] buffer[i];
			buffer[i] = nullptr;
		}
		delete[] buffer;
		buffer = nullptr;
	}

	// ������ ��������� ������� �������
	inline int getH() const
	{
		return h;
	}

	inline int getW() const
	{
		return w;
	}
};
