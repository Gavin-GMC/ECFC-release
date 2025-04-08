//------------------------Description------------------------
// This file defines the inspiration in ECFC. They provide 
// the optimizer with recommendations for the best possible 
// decision based on specific rules.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include<algorithm>
#include <random>
#include"calculator.h"
#include"sort-helper.h"
#include"feasible-region.h"

namespace ECFC
{
	// �����������˼·��
	// ���ڿ����������ʽ������������
	// �䲻����������ά�������Կ�������Ϊ����������Ը��õؽ�����϶�
	// ���ڴˣ����������ӦΪ��ǰ�����򣬾�����ά�ȣ������������
	// ��������������ά�ȣ����а�
	// 
	// ����ʽ�������
	// ��̬ - �������漰����������������ݺ�ѡ������״̬�������ı�
	// ��̬ - ֻ�뻷���еĳ������
	// 
	// ����ʽ����ʽ���壺
	// ��һά����ǰ����ά��
	// �ڶ�ά������������
	// ����ά�����������ݣ��漰����������
	//


	// ��������������ά�Ȱ�
	class Inspiration
	{
	public:
		Inspiration()
		{

		}

		virtual ~Inspiration() {}

		// ��ȡ���ܵ����ž���
		virtual double getPrioriDecision(FeasibleLine* feasible_region, int demension, double** inputs) = 0;

		// ������п��о��ߵ�������
		virtual double* getPrioriOrder(FeasibleLine* feasible_region, int demension, double** inputs, int& order_size) = 0;

		// �������ʽ�ļ���ֵ
		virtual double getHeuristic(FeasibleLine* feasible_region, int demension, double choice, double** inputs) = 0;

		virtual Inspiration* clone() = 0;
	};

	// �����������Ĭ�Ϸ��ؿ������е����ֵ��ȱʡ��������
	class RandomInspiration final : public Inspiration
	{
	public:
		RandomInspiration()
			:Inspiration()
		{

		}

		~RandomInspiration() {}

		double getPrioriDecision(FeasibleLine* feasible_region, int demension, double** inputs)
		{
			return feasible_region->getRandomDecision();
		}

		double* getPrioriOrder(FeasibleLine* feasible_region, int demension, double** inputs, int& order_size)
		{
			double* back = feasible_region->getFeasibleList(order_size);
			std::shuffle(back, back + order_size, std::random_device());
			return back;
		}

		double getHeuristic(FeasibleLine* feasible_region, int demension, double choice, double** inputs)
		{
			return rand01();
		}

		Inspiration* clone()
		{
			return new RandomInspiration();
		}
	};

	// �߽���������Ĭ�Ϸ��ؿ�����ı߽�ֵ
	class BoundaryInspiration final : public Inspiration
	{
	public:
		BoundaryInspiration()
			:Inspiration()
		{

		}

		~BoundaryInspiration() {}

		double getPrioriDecision(FeasibleLine* feasible_region, int demension, double** inputs)
		{
			return feasible_region->getBoundaryDecision(true);
		}

		double* getPrioriOrder(FeasibleLine* feasible_region, int demension, double** inputs, int& order_size)
		{
			double* back = feasible_region->getFeasibleList(order_size);
			return back;
		}

		double getHeuristic(FeasibleLine* feasible_region, int demension, double choice, double** inputs)
		{
			if (choice == feasible_region->getBoundaryDecision(true) || choice == feasible_region->getBoundaryDecision(false))
				return 1;
			return 0;
		}

		Inspiration* clone()
		{
			return new BoundaryInspiration();
		}
	};

	// ����Ԥ���������ʽ��Ϣ���������Ⱦ��ߣ��������ʽ��Ϣ��̬����������ʱ�临�Ӷȵ�
	class StableInspiration : public Inspiration
	{
	private:
		double* _saved_order;
		int _order_size[2];

		double& orderTable(int demension, int rank)
		{
			return _saved_order[demension * _order_size[0] + rank];
		}

		void ini(FeasibleLine* region_pointer, Calculator* calculator, double** inputs)
		{
			double decision_pair[2];// 0 is demension, 1 is decision 
			int paras = calculator->getParameterNumber();
			double** calculator_buffer = new double* [paras];
			double* feasible_list;
			int feasible_list_size;
			sortHelper<double, double>* sort_buffer = new sortHelper<double, double>[_order_size[1]];

			// ��ȡ���п��н�
			feasible_list = region_pointer->getFeasibleList(feasible_list_size);
			// ����������
			calculator_buffer[0] = decision_pair;
			calculator_buffer[1] = decision_pair + 1;
			memcpy(calculator_buffer + 2, inputs, (paras - 2) * sizeof(double*));

			for (int demensionId = 0; demensionId < _order_size[0]; demensionId++)
			{
				// ������н�����ʽֵ
				decision_pair[0] = demensionId;
				for (int i = 0; i < feasible_list_size; i++)
				{
					sort_buffer[i].id = feasible_list[i];
					decision_pair[1] = feasible_list[i];
					calculator->run(calculator_buffer, &sort_buffer[i].value);
				}

				// ���нⰴ����ʽֵ����
				std::sort(sort_buffer, sort_buffer + feasible_list_size);

				// �����¼����
				for (int i = 0; i < feasible_list_size; i++)
				{
					orderTable(demensionId, i) = sort_buffer[i].value;
				}

				// �����ڴ�
				delete[] feasible_list;
			}
			delete[] sort_buffer;
		}

		StableInspiration()
		{

		}

	public:
		StableInspiration(FeasibleLine* region_pointer, Calculator* calculator, int demension_number, double** inputs)
			:Inspiration()
		{
			_order_size[0] = demension_number;
			_order_size[1] = region_pointer->getLength();
			_saved_order = new double[_order_size[0] * _order_size[1]];
			ini(region_pointer, calculator, inputs);
		}

		StableInspiration(double* priori_matrix, int size, int demension_number)
			:Inspiration()
		{
			_order_size[0] = demension_number;
			_order_size[1] = size;
			_saved_order = new double[_order_size[0] * _order_size[1]];
			memcpy(_saved_order, priori_matrix, size * demension_number * sizeof(double));
		}

		~StableInspiration()
		{
			delete[] _saved_order;
		}

		double* getPrioriOrder(FeasibleLine* feasible_region, int demension, double** inputs, int& order_size)
		{
			order_size = feasible_region->getLength();
			double* back = new double[order_size];

			//�����нⰴ�洢��˳������
			int counter = 0;
			for (int i = 0; i < _order_size[1]; i++)
			{
				if (feasible_region->isin(orderTable(demension, i)))
				{
					back[counter] = orderTable(demension, i);
					counter++;
				}
			}

			return back;
		}

		// Ԥ����нⲻΪ�գ�Ӧ���ڵ���֮ǰ���м��
		double getPrioriDecision(FeasibleLine* feasible_region, int demension, double** inputs)
		{
			for (int i = 0; i < _order_size[1]; i++)
			{
				if (feasible_region->isin(orderTable(demension, i)))
				{
					return orderTable(demension, i);
				}
			}

			return EMPTYVALUE;
		}

		double getHeuristic(FeasibleLine* feasible_region, int demension, double choice, double** inputs)
		{
			int index = demension * _order_size[0];
			for (int i = 0; i < _order_size[1]; i++)
			{
				if (_saved_order[index] == choice)
					return i;
				index++;
			}
			return _order_size[1] + 1;
		}

		Inspiration* clone()
		{
			StableInspiration* back = new StableInspiration();

			memcpy(back->_saved_order, _saved_order, sizeof(double) * _order_size[0] * _order_size[1]);
			memcpy(back->_order_size, _order_size, 2 * sizeof(double));

			return back;
		}
	};

	// ����Ԥ���������ʽ��Ϣ���������Ⱦ��ߣ��������ʽ��Ϣ��̬���ɵ������ʱ�临�ӶȽϸ�
	class NormalInspiration : public Inspiration
	{
	private:
		Calculator* _calculator;
		double _decision_pair[2];
		int _parameter_number;
		double** _calculate_buffer; // ǰ��ά�ֱ�Ϊ����ά�Ⱥ;���ֵ

	public:
		NormalInspiration(Calculator* calculator)
			:Inspiration()
		{
			_calculator = calculator->copy();
			_parameter_number = calculator->getParameterNumber();
			if (_parameter_number < 2) // ����
				return;
			_calculate_buffer = new double* [_parameter_number];
			_calculate_buffer[0] = _decision_pair;
			_calculate_buffer[1] = _decision_pair + 1;
		}

		~NormalInspiration()
		{
			delete _calculator;
			delete[] _calculate_buffer;
		}

		double* getPrioriOrder(FeasibleLine* feasible_region, int demension, double** inputs, int& order_size)
		{
			double* back = feasible_region->getFeasibleList(order_size);
			sortHelper<double, double>* sort_buffer = new sortHelper<double, double>[order_size];
			memcpy(_calculate_buffer + 2, inputs, sizeof(double*) * (_parameter_number - 2));

			//������н�����ʽֵ
			_decision_pair[0] = demension;
			for (int i = 0; i < order_size; i++)
			{
				sort_buffer[i].id = back[i];
				_decision_pair[1] = back[i];
				_calculator->run(_calculate_buffer, &sort_buffer[i].value);
			}

			//���нⰴ����ʽֵ����
			std::sort(sort_buffer, sort_buffer + order_size, std::greater<sortHelper<double, double>>());

			//���뷵������
			for (int i = 0; i < order_size; i++)
			{
				back[i] = sort_buffer[i].id;
			}

			delete[] sort_buffer;
			return back;
		}

		double getPrioriDecision(FeasibleLine* feasible_region, int demension, double** inputs)
		{
			int order_size;
			double* priori_order = getPrioriOrder(feasible_region, demension, inputs, order_size);

			double back = priori_order[0];
			delete[] priori_order;

			return back;
		}

		double getHeuristic(FeasibleLine* feasible_region, int demension, double choice, double** inputs)
		{
			double back;
			_decision_pair[0] = demension;
			_decision_pair[1] = choice;
			memcpy(_calculate_buffer + 2, inputs, sizeof(double*) * (_parameter_number - 2));
			_calculator->run(_calculate_buffer, &back);

			return back;
		}

		Inspiration* clone()
		{
			return new NormalInspiration(_calculator);
		}

		StableInspiration* toStable(FeasibleLine* region_pointer, int demension_number, double** inputs)
		{
			return new StableInspiration(region_pointer, _calculator, demension_number, inputs);
		}
	};
}