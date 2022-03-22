// ���� "circuit.h"
// � ������ ����� ������ ����� CircuitAC ��� ������ � ������ ������������� ����


// ������ �� ���������� ��������� ������������� �����
#pragma once


// ����������� ����������� ���������
#include "MatrC.h"
#include "Comp.h"

#include <iostream>
#include <fstream>
#include <vector>


// ����� ������������� ���� ����������� ����
class CircuitAC
{
	// ��������� ����� ��������������� �����
	// ���������� ����������� ����� ����� ��������� ���������� � ��������� ����� 
	typedef struct _GraphBranch
	{
		int begin;
		int end;
	}GraphBranch;

	int branchesCount;							// ���������� ������ ����
	int nodesCount;								// ���������� ����� ����
	std::vector<GraphBranch> circuitBranches;	// ��������������� ���� ������
	std::vector<Comp> ZValues;					// ������ �������� �������������
	std::vector<Comp> EValues;					// ������ �������� ����������
	std::vector<Comp> JValues;					// ������ �������� �����

public:
	// �����������
	// ��������� �������� ������ ���� �� ����� � �� �� ������ ��������� ���� ������������� ����
	explicit CircuitAC(const std::string& path)
		: branchesCount(0)
		, nodesCount(0)
	{
		std::ifstream ifl(path);

		if (!ifl) throw std::exception("invalid file name");

		int branchBeginEnd, branchBegin, branchEnd;
		double value, value1, value2;
		char skipLine[256];

		// ���������� ������ ��� ������������ ������
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

	// ������� �������������� ������� ������� �� ����� �������� ����
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

	// ������� ������� ���������� ���� ��� �������� ���� 
	MatrC getCurrentSourcesMatrix() const
	{
		MatrC J(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			J.at(i, 0) = JValues[i];
		}

		return J;
	}

	// ������� ������� ���������� ���������� ��� �������� ����
	MatrC getVoltageSourcesMatrix() const
	{
		MatrC E(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			E.at(i, 0) = EValues[i];
		}

		return E;
	}

	// ������� ������� ������������� ��� �������� ����
	MatrC getResistorsMatrix() const
	{
		MatrC Z(branchesCount, 1);

		for (int i = 0; i < branchesCount; i++)
		{
			Z.at(i, 0) = ZValues[i];
		}

		return Z;
	}

	// ������� ������� ������������� ���� ������� ������� �����������
	friend MatrC calculateCircuit(CircuitAC& circuit)
	{
		// ������ ������� � ��������� �������
		MatrC Z = circuit.getResistorsMatrix();
		MatrC E = circuit.getVoltageSourcesMatrix();
		MatrC J = circuit.getCurrentSourcesMatrix();

		// ��������� ������������ ������� RD �� ������� R
		MatrC ZD = Z.diag();

		// ��������� ������� ���������� A ��� ����� ����
		MatrC NodesMatrix = circuit.getNodesMatrix();

		// ��������� ������� ������������ G �� ������� RD
		MatrC G = ZD.rev();

		// ��������� ���������� ���� ����� ���� �� ��������� � ��������� ����
		MatrC F = (NodesMatrix * G * NodesMatrix.transp()).rev() * (-NodesMatrix * G * E - NodesMatrix * J);

		// ��������� ���������� �� ���� ������ ����
		MatrC U = NodesMatrix.transp() * F;

		// ��������� ���� � �������������� ������
		MatrC IR = G * (U + E);

		return IR;
	}
};