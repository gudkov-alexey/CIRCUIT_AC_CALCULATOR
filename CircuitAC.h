// файл "circuit.h"
// в данном файле описан класс CircuitAC для работы с графом электрической цепи


// защита от повторного включения заголовочного файла
#pragma once


// подключение необходимых библиотек
#include "MatrC.h"
#include "Comp.h"

#include <iostream>
#include <fstream>
#include <vector>


// класс электрическая цепь переменного тока
class CircuitAC
{
	// структура ветви ориентированный графа
	// определяет направление ветви графа индексами начального и конечного узлов 
	typedef struct _GraphBranch
	{
		int begin;
		int end;
	}GraphBranch;

	int branchesCount;							// количество ветвей цепи
	int nodesCount;								// количество узлов цепи
	std::vector<GraphBranch> circuitBranches;	// ориентированный граф ветвей
	std::vector<Comp> ZValues;					// массив значений сопротивлений
	std::vector<Comp> EValues;					// массив значений напряжений
	std::vector<Comp> JValues;					// массив значений токов

public:
	// конструктор
	// считывает исходные данные цепи из файла и на их основе формирует граф электрической цепи
	explicit CircuitAC(const std::string& path)
		: branchesCount(0)
		, nodesCount(0)
	{
		std::ifstream ifl(path);

		if (!ifl) throw std::exception("invalid file name");

		int branchBeginEnd, branchBegin, branchEnd;
		double value, value1, value2;
		char skipLine[256];

		// пропускаем первые две заголовочные строки
		ifl.getline(skipLine, 256);
		ifl.getline(skipLine, 256);

		while (!ifl.eof())
		{
			ifl >> branchesCount;

			ifl >> branchBeginEnd;

			branchBegin = branchBeginEnd / 10;
			branchEnd = branchBeginEnd % 10;

			if (branchBegin > nodesCount) nodesCount = branchBegin;
			if (branchEnd > nodesCount) nodesCount = branchEnd;

			circuitBranches.push_back({ 
				branchBegin - 1,
				branchEnd - 1 
			});

			ifl >> value >> value1 >> value2;
			ZValues.push_back(value + COMP_J * value1 - COMP_J * value2);

			ifl >> value1 >> value2;
			EValues.push_back(Comp(value1, value2, EXP_FORM));

			ifl >> value1 >> value2;
			JValues.push_back(Comp(value1, value2, EXP_FORM));
		}

		ifl.close();
	}

	// создает топологическую узловую матрицу по графу исходной цепи
	MatrC getNodesMatrix() const
	{
		MatrC nodesMartix(nodesCount - 1, branchesCount);

		for (int i = 0; i < nodesMartix.getH(); i++)
		{
			for (int j = 0; j < nodesMartix.getW(); j++)
			{
				if (circuitBranches[j].begin == i) nodesMartix.at(i, j) = COMP_1;
				else if (circuitBranches[j].end == i) nodesMartix.at(i, j) = -COMP_1;
				else nodesMartix.at(i, j) = COMP_0;
			}
		}

		return nodesMartix;
	}

	// создает матрицу источников тока для исходной цепи 
	MatrC getCurrentSourcesMatrix() const
	{
		MatrC J(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			J.at(i, 0) = JValues[i];
		}

		return J;
	}

	// создает матрицу источников напряжения для исходной цепи
	MatrC getVoltageSourcesMatrix() const
	{
		MatrC E(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			E.at(i, 0) = EValues[i];
		}

		return E;
	}

	// создает матрицу сопротивлений для исходной цепи
	MatrC getResistorsMatrix() const
	{
		MatrC Z(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			Z.at(i, 0) = ZValues[i];
		}

		return Z;
	}

	// функция расчета электрической цепи методом узловых потенциалов
	friend MatrC calculateCircuit(CircuitAC& circuit)
	{
		// задаем матрицы с исходными данными
		MatrC Z = circuit.getResistorsMatrix();
		MatrC E = circuit.getVoltageSourcesMatrix();
		MatrC J = circuit.getCurrentSourcesMatrix();

		// формируем диагональную матрицу RD из матрицы R
		MatrC ZD = Z.diag();

		// формируем матрицу соединений A для графа цепи
		MatrC NodesMatrix = circuit.getNodesMatrix();

		// формируем матрицу проводимости G из матрицы RD
		MatrC G = ZD.rev();

		// вычисляем потенциалы всех узлов цепи по отношению к базисному узлу
		MatrC F = (NodesMatrix * G * NodesMatrix.transp()).rev() * (-NodesMatrix * G * E - NodesMatrix * J);

		// вычисляем напряжение на всех ветвях цепи
		MatrC U = NodesMatrix.transp() * F;

		// вычисляем токи в сопротивлениях ветвей
		MatrC IR = G * (U + E);

		return IR;
	}
};