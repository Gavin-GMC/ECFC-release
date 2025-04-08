//------------------------Description------------------------
// This file defines the constrain in ECFC. They support 
// constraint propagation, candidate solution checking, 
// constraint violation degree calculation and feasible
// region reduction.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include<map>
#include"sort-helper.h"
#include<unordered_set>
#include"feasible-region.h"
#include"basicfunc.h"

namespace ECFC
{
	// Լ����������ȷ���Ƿ��Լ���ʱ��Լ��ʩ�Ӵ�����������
	enum ConstrianLevel
	{
		constrains_non,
		constraints_range,
		constrains_variable,
		constraints_discrete,
		constraints_continue,
		constraints_customization
	};

	// Լ����ģ�����
	class Constrain
	{
	protected:
		double _w; // �ͷ�ϵ��
	public:
		Constrain(double penalty_w) 
		{
			_w = penalty_w;
		}

		virtual ~Constrain() {}

		// Լ��״̬����
		virtual void ini() = 0;

		// ���Ŀ��ֵ�Ƿ�����Լ��
		virtual bool meet(int demensionId, double value) = 0;

		// ������ֵ����Լ��״̬��Լ��������
		virtual void update(int demensionId, double value) = 0;

		// ���������Լ��Υ���̶�
		virtual double violation(double* variables, int size) = 0;

		// ����Լ��״̬�Կ��������������
		virtual void regionReduction(int demensionId, FeasibleLine* region) = 0;

		// ��ӹ������������ڶ��������Լ������Լ����ʵ��
		virtual void addCorresConstrain(Constrain* pointer)
		{

		}

		// ����Լ������
		virtual ConstrianLevel getConstrainLevel() = 0;

		// Լ����¡
		virtual Constrain* clone() = 0;

		// ����Լ��Ȩ��
		double getWeight()
		{
			return _w;
		}
	};

	/*
	// ��ֵԼ��
	class ConstrainArithmetic final : public Constrain
	{
	private:

	public:
		ConstrainArithmetic();

		~ConstrainArithmetic();

		void ini();

		bool meet(int demensionId, double value);

		void update(int demensionId, double value);

		double violation(double* solution, int size);

		void regionReduction(FeasibleRegion* region);
	};

	// �߼�Լ��
	class ConstrainLogical final : public Constrain
	{
	private:

	public:
		ConstrainLogical();

		~ConstrainLogical();

		void ini();

		bool meet(int demensionId, double value);

		void update(int demensionId, double value);

		double violation(double* solution, int size);

		void regionReduction(FeasibleRegion* region);
	};
	*/

	// ȡֵ��ΧԼ������Ĭ�����ã���Ҫ�ֶ���ӣ�
	class ConstrainRange final : public Constrain
	{
	private:
		double _left;
		double _right;

	public:
		ConstrainRange(double penalty_w, const double left, const double right)
			:Constrain(penalty_w)
		{
			_left = left;
			_right = right;
		}

		~ConstrainRange()
		{
			
		}

		void ini()
		{

		}

		bool meet(int demensionId, double value)
		{
			if (value < _left || value > _right)
				return false;
			return true;
		}

		void update(int demensionId, double value)
		{

		}

		// ���ص�������������پ���
		double violation(double* variables, int size)
		{
			double back = 0;
			for (int j = 0; j < size; j++)
			{
				if (variables[j] < _left)
				{
					back += (_left - variables[j]);
				}
				else if (variables[j] > _right)
				{
					back += (variables[j] - _right);
				}
			}

			return back;
		}

		// ֻӦ�ڱ�����ʼ���׶α�����һ�ι��������ĳ�ʼ������
		void regionReduction(int demensionId, FeasibleLine* region)
		{
			region->unite(_left, _right, false, false);
		}

		ConstrianLevel getConstrainLevel()
		{
			return constraints_range;
		}

		Constrain* clone()
		{
			return new ConstrainRange(_w, _left, _right);
		}
	};

	// ���Ǽ�һ��ά�����ȵ�����Լ��
	// ����Լ����ָ����ر��������п���ֵ
	class ConstrainCompatibility final : public Constrain
	{
	private:
		double* _values;
		int _length;

	public:
		ConstrainCompatibility(double penalty_w, const double* value, const int length)
			:Constrain(penalty_w)
		{
			_length = length;
			_values = new double[_length];

			memcpy(_values, value, _length * sizeof(double));
		}

		~ConstrainCompatibility()
		{
			delete[] _values;
		}

		void ini()
		{

		}

		bool meet(int demensionId, double value)
		{
			for (int i = 0; i < _length; i++)
			{
				if (equal(value, _values[i]))
					return true;
			}
			return false;
		}

		void update(int demensionId, double value)
		{

		}

		// ���ص�������ĺ�������
		double violation(double* variables, int size)
		{
			double back = 0;
			for (int j = 0; j < size; j++)
			{
				if (!meet(j, variables[j]))
					back++;
			}

			return back;
		}

		// ֻӦ�ڱ�����ʼ���׶α�����һ�ι��������ĳ�ʼ������
		void regionReduction(int demensionId, FeasibleLine* region)
		{
			FeasibleLine buffer(_values, _length);

			region->unite(&buffer);
		}

		ConstrianLevel getConstrainLevel()
		{
			return constraints_range;
		}

		Constrain* clone()
		{
			return new ConstrainCompatibility(_w, _values, _length);
		}
	};

	// ��ɢԼ������һԼ����������������ֵֻ�ܳ���һ��
	class ConstrainUnique final : public Constrain
	{
	private:
		bool* _not_had;
		double* _values; // ���ܳ��ֵ�ֵ
		int _size;

		double prevalue; // �ող��õ�ֵ

		// ���ַ�����id
		int findIndex(double value)
		{
			int left = 0;
			int right = _size - 1;
			int mid;

			while (left <= right)
			{
				mid = (left + right) / 2;
				if (equal(_values[mid], value))
					return mid;
				if (_values[mid] < value)
					left = mid + 1;
				else
					right = mid - 1;
			}
			return -1;
		}

	public:
		ConstrainUnique(double penalty_w, double* element_list, int size)
			:Constrain(penalty_w)
		{
			_not_had = new bool[size];
			_size = size;
			_values = new double[_size];

			memcpy(_values, element_list, _size * sizeof(double));
			std::sort(_values, _values + _size);

			prevalue = EMPTYVALUE;
		}

		~ConstrainUnique()
		{
			delete[] _not_had;
			delete[] _values;
		}

		void ini()
		{
			for (int i = 0; i < _size; i++)
				_not_had[i] = true;
		}

		bool meet(int demensionId, double value)
		{
			int id = findIndex(value);
			if (id < 0) // ֵ���ڿ���ֵ��
				return false;
			return _not_had[id];
		}

		void update(int demensionId, double value)
		{
			int id = findIndex(value);
			if (id < 0) // ֵ������
				return;

			prevalue = value;
			_not_had[id] = false;
		}

		// ���ص�������ĺ�����
		double violation(double* variables, int size)
		{
			int back = 0;
			int id;

			ini();

			for (int did = 0; did < size; did++)
			{
				id = findIndex(variables[did]);
				if (id < 0 || !_not_had[id])
				{
					back++;
				}
				else
				{
					_not_had[id] = false;
				}
			}

			return back;
		}

		// ����������Ч����Ա���������ִ��һ������
		void regionReduction(int demensionId, FeasibleLine* region)
		{
			region->differ(prevalue, prevalue, false, false);
		}

		ConstrianLevel getConstrainLevel()
		{
			return constrains_variable;
		}

		Constrain* clone()
		{
			return new ConstrainUnique(_w, _values, _size);
		}
	};

	// ��ɢԼ������һԼ�������ģ�汾��������������ֵֻ�ܳ���һ��
	class ConstrainUniqueLarge final : public Constrain
	{
	private:
		std::unordered_set<double> _exist_element;

		double prevalue;

	public:
		ConstrainUniqueLarge(double penalty_w)
			:Constrain(penalty_w)
		{
			_exist_element.clear();
			prevalue = EMPTYVALUE;
		}

		~ConstrainUniqueLarge()
		{

		}

		void ini()
		{
			_exist_element.clear();
		}

		bool meet(int demensionId, double value)
		{
			for (std::unordered_set<double>::iterator it = _exist_element.begin(); it != _exist_element.end(); it++)
			{
				if (equal(*it, value))
					return false;
			}
			return true;
		}

		void update(int demensionId, double value)
		{
			_exist_element.insert(value);
			prevalue = value;
		}

		// ���ص�������ĺ�����
		double violation(double* variables, int size)
		{
			int back = 0;

			ini();

			for (int did = 0; did < size; did++)
			{
				if (meet(did, variables[did]))
				{
					update(did, variables[did]);
				}
				else
				{
					back++;
				}
			}

			return back;
		}

		void regionReduction(int demensionId, FeasibleLine* region)
		{
			region->differ(prevalue, prevalue, false, false);
		}

		ConstrianLevel getConstrainLevel()
		{
			return constrains_variable;
		}

		Constrain* clone()
		{
			return new ConstrainUniqueLarge(_w);
		}
	};

	// ��ɢԼ������С��Լ������ÿ�������ľ�������ڸ���ֵ
	class ConstrainMinDistance final : public Constrain
	{
	private:
		double* _gap_width;
		int _size;
		double _start;
		double* _position_min;
		double* _position_max;
	public:
		ConstrainMinDistance(double penalty_w, double start, double end, double gap_width, int size)
			:Constrain(penalty_w)
		{
			_start = start;
			_size = size;
			_gap_width = new double[size];
			for (int i = 0; i < size; i++)
			{
				_gap_width[i] = gap_width;
			}

			_position_min = new double[size];
			_position_max = new double[size];
			_position_max[size - 1] = end;
			for (int i = size - 2; i > -1; i--)
			{
				_position_max[i] = _position_max[i + 1] - _gap_width[i + 1];
			}
		}

		ConstrainMinDistance(double penalty_w, double start, double end, double* gap_width, int size)
			:Constrain(penalty_w)
		{
			_size = size;
			_start = start;
			_gap_width = new double[size];
			for (int i = 0; i < size; i++)
			{
				_gap_width[i] = gap_width[i];
			}

			_position_min = new double[size];
			_position_max = new double[size];
			_position_max[size - 1] = end;
			for (int i = size - 2; i > -1; i--)
			{
				_position_max[i] = _position_max[i + 1] - _gap_width[i + 1];
			}
		}

		~ConstrainMinDistance()
		{
			delete[] _gap_width;
			delete[] _position_min;
			delete[] _position_max;
		}

		void ini()
		{
			_position_min[0] = _start;
			for (int i = 0; i < _size; i++)
			{
				_position_min[i] = _position_min[i - 1] + _gap_width[i];
			}
		}

		bool meet(int demensionId, double value)
		{
			return value >= _position_min[demensionId] && value <= _position_max[demensionId];
		}

		void update(int demensionId, double value)
		{
			_position_min[demensionId] = value;
			for (int i = demensionId + 1; i < _size; i++)
			{
				_position_min[i] = _position_min[i - 1] + _gap_width[i];
			}
		}

		double violation(double* variables, int size)
		{
			double back = 0;
			double distance_deficiency, pre_value;
			//int counter = 0;

			pre_value = variables[0];
			for (int did = 1; did < size; did++)
			{
				distance_deficiency = _gap_width[did-1] - (variables[did] - pre_value);
				if (distance_deficiency > 0)
					back += distance_deficiency;
				pre_value = variables[did];
			}

			return back;
		}

		void regionReduction(int demensionId, FeasibleLine* region)
		{
			region->unite(_position_min[demensionId], _position_max[demensionId], false, false);
		}

		ConstrianLevel getConstrainLevel()
		{
			return constraints_discrete;
		}

		Constrain* clone()
		{
			return new ConstrainMinDistance(_w, _start, _position_max[_size - 1], _gap_width, _size);
		}
	};

	// ��ɢԼ��������Լ������ÿ�������ĸ��ز��ܳ���������
	class ConstrainCapacity final : public Constrain
	{
	private:
		double* _capacity_ini;
		double* _capacity_left;
		double* _capacity_require;
		int _size;
		int _item_number;

	public:

		const static int level = 1;

		ConstrainCapacity(double penalty_w, double* capacitys, int container_number, double* volumes, int item_number)
			:Constrain(penalty_w)
		{
			_capacity_ini = new double[container_number];
			_capacity_left = new double[container_number];
			_size = container_number;
			for (int i = 0; i < container_number; i++)
			{
				_capacity_ini[i] = capacitys[i];
			}

			_capacity_require = new double[item_number];
			_item_number = item_number;
			for (int i = 0; i < item_number; i++)
			{
				_capacity_require[i] = volumes[i];
			}
		}

		~ConstrainCapacity()
		{
			delete[] _capacity_ini;
			delete[] _capacity_left;
			delete[] _capacity_require;
		}

		void ini()
		{
			for (int i = 0; i < _size; i++)
			{
				_capacity_left[i] = _capacity_ini[i];
			}
		}

		bool meet(int demensionId, double value)
		{
			return _capacity_left[int(value)] >= _capacity_require[demensionId];
		}

		void update(int demensionId, double value)
		{
			_capacity_left[int(value)] -= _capacity_require[demensionId];
		}

		double violation(double* variables, int size)
		{
			int counter = 0;
			ini();

			for (int did = 0; did < size; did++)
			{
				update(counter, variables[did]);
				counter++;
			}

			// ͳ��Υ����
			double back = 0;
			for (int i = 0; i < _size; i++)
			{
				if (_capacity_left[i] < 0)
				{
					back -= _capacity_left[i];
				}
			}

			return back;
		}

		void regionReduction(int demensionId, FeasibleLine* region)
		{
			// ���Կ��ǹ���һ����������������ܿ������Ч��
			for (int i = 0; i < _size; i++)
			{
				if (_capacity_left[i] < _capacity_require[demensionId])
				{
					region->differ(i, i, false, false);
				}
			}
		}

		ConstrianLevel getConstrainLevel()
		{
			return constraints_discrete;
		}

		Constrain* clone()
		{
			return new ConstrainCapacity(_w, _capacity_ini, _size, _capacity_require, _item_number);
		}
	};

	/*
	// ��ɢԼ������תԼ����������Ӧ���㷴ת��
	class ConstrainInverse final : public Constrain
	{
	private:
		int _size;
		int* _determined;
		ConstrainInverse* _correspondence;

	public:
		ConstrainInverse(int size)
		{
			_size = size;
			_determined = new int[size];
		}

		~ConstrainInverse()
		{
			delete[] _determined;
		}

		void addCorresConstrain(Constrain* pointer)
		{
			_correspondence = static_cast<ConstrainInverse*>(pointer);
		}

		void ini()
		{
			for (int i = 0; i < _size; i++)
			{
				_determined[i] = EMPTYVALUE;
			}
		}

		bool meet(int demensionId, double value)
		{
			if (_determined[demensionId] == EMPTYVALUE || _determined[demensionId] == value) // �����ƻ�����Լ��
			{
				return true;
			}
			else
				return false;
		}

		void update(int demensionId, double value)
		{
			_determined[demensionId] = value;
			_correspondence->_determined[int(value)] = demensionId;
		}

		double violation(double* variables, int size)
		{
			double back = 0;
			for (int i = 0; i < _size; i++)
			{
				if (_determined[int(_correspondence->_determined[i])] != i)
				{
					back += 1;
				}
			}

			return back;
		}

		void regionReduction(int demensionId, FeasibleLine* region)
		{
			if (_determined[demensionId] == EMPTYVALUE)
				return;

			region->unite(_determined[demensionId], _determined[demensionId], false, false);
		}

		ConstrianLevel getConstrainLevel()
		{
			return constraints_discrete;
		}

		Constrain* clone()
		{
			ConstrainInverse* back = new ConstrainInverse(_size);
			back->addCorresConstrain(_correspondence);

			return back;
		}
	};
	*/
	/*
	// ��ɢԼ�����ֵ���Լ���������������ıȽ�Ӧ�����ֵ���
	class ConstrainDictionaryOrder final : public Constrain
	{
	private:
		bool _met;


	public:
		ConstrainDictionaryOrder(int less_size, int larger_size)
		{

		}

		~ConstrainDictionaryOrder();

		void ini();

		bool meet(int demension, double value);

		void update(int demension, double value);

		double violation(double* solution, int size);

		void regionReduction(int demension_d, FeasibleRegion* region);
	};
	*/

	// ��ɢԼ�����ֲ�Լ������ֵ���ִ������ܳ���ָ������
	class ConstrainDistributed final : public Constrain
	{
	private:
		int* _appear_max;
		int* _appear_number;
		int _size;
		double* _values;

		double prevalue;
		// ���ַ�����id
		int findIndex(double value)
		{
			int left = 0;
			int right = _size - 1;
			int mid;

			while (left <= right)
			{
				mid = (left + right) / 2;
				if (equal(_values[mid], value))
					return mid;
				if (_values[mid] < value)
					right = mid - 1;
				else
					left = mid + 1;
			}
			return -1;
		}
	public:
		ConstrainDistributed(double penalty_w, double* feasible_values, int size, int* appear_numbers)
			:Constrain(penalty_w)
		{
			_appear_max = new int[size];
			_appear_number = new int[size];
			_size = size;

			memcpy(_values, feasible_values, _size * sizeof(double));
			std::sort(_values, _values + _size);

			sortHelper<int, double>* buffer = new sortHelper<int, double>[_size];
			for (int i = 0; i < size; i++)
			{
				buffer[i].id = appear_numbers[i];
				buffer[i].value = feasible_values[i];
			}
			std::sort(buffer, buffer + _size);

			for (int i = 0; i < size; i++)
			{
				_values[i] = buffer[i].value;
				_appear_max[i] = buffer[i].id;
			}
			prevalue = EMPTYVALUE;
		}

		ConstrainDistributed(double penalty_w, double* feasible_values, int size, double* appear_numbers)
			:Constrain(penalty_w)
		{
			_appear_max = new int[size];
			_appear_number = new int[size];
			_size = size;

			memcpy(_values, feasible_values, _size * sizeof(double));
			std::sort(_values, _values + _size);

			sortHelper<int, double>* buffer = new sortHelper<int, double>[_size];
			for (int i = 0; i < size; i++)
			{
				buffer[i].id = int(appear_numbers[i]);
				buffer[i].value = feasible_values[i];
			}
			std::sort(buffer, buffer + _size);

			for (int i = 0; i < size; i++)
			{
				_values[i] = buffer[i].value;
				_appear_max[i] = buffer[i].id;
			}
			prevalue = EMPTYVALUE;
		}

		~ConstrainDistributed()
		{
			delete[] _appear_max;
			delete[] _appear_number;
			delete[] _values;
		}

		void ini()
		{
			for (int i = 0; i < _size; i++)
			{
				_appear_number[i] = 0;
			}
			prevalue = EMPTYVALUE;
		}

		bool meet(int demension, double value)
		{
			int id = findIndex(value);
			return id > -1 && _appear_number[id] < _appear_max[id];
		}

		void update(int demension, double value)
		{
			int id = findIndex(value);
			if (id < 0)
				return;
			_appear_number[id]++;
			if (_appear_number[id] == _appear_max[id])
				prevalue = value;
		}

		double violation(double* solution, int size)
		{
			double back = 0;

			ini();
			for (int i = 0; i < size; i++)
			{
				update(i, solution[i]);
			}

			for (int i = 0; i < _size; i++)
			{
				if (_appear_number[i] > _appear_max[i])
				{
					back += (_appear_number[i] - _appear_max[i]);
				}
			}

			return back;
		}

		void regionReduction(int demensionId, FeasibleLine* region)
		{
			if (prevalue != EMPTYVALUE)
			{
				region->differ(prevalue, prevalue, false, false);
				prevalue = EMPTYVALUE;
			}
		}

		ConstrianLevel getConstrainLevel()
		{
			return constrains_variable;
		}

		Constrain* clone()
		{
			ConstrainDistributed* back = new ConstrainDistributed(_w, _values, _size, _appear_max);
			return back;
		}
	};

	// �û��Զ���Լ�����������á���顢ģ�͸�����������ʵ��Լ��������������������
	// ע�⣺�ö��󲻻�Ժ������漰��ȫ�ֱ���������������˵�ʹ�ò����Ӵ������ܵȲ��зֲ�ʽ����ʱ���������Ԥ����Ľ���������
	class ConstrainUserDefined final : public Constrain
	{
	private:
		void (*_model_ini)(void);
		double (*_check_func)(int demensionId, double value);
		void (*_model_change)(int demensionId, double value);

	public:
		ConstrainUserDefined(double penalty_w, void (*model_ini)(void), double (*check_func)(int, double), void (*model_change)(int, double))
			:Constrain(penalty_w)
		{
			_model_ini = model_ini;
			_check_func = check_func;
			_model_change = model_change;
		}

		~ConstrainUserDefined()
		{

		}

		void ini()
		{
			_model_ini();
		}

		bool meet(int demensionId, double value)
		{
			return equal(_check_func(demensionId, value), 0);
		}

		void update(int demensionId, double value)
		{
			_model_change(demensionId, value);
		}

		double violation(double* variables, int size)
		{
			double back = 0;

			_model_ini();
			for (int did = 0; did < size; did++)
			{
				back += _check_func(did, variables[did]);
				_model_change(did, variables[did]);
			}

			return back;
		}

		void regionReduction(int demensionId, FeasibleLine* region)
		{

		}

		ConstrianLevel getConstrainLevel()
		{
			return constraints_customization;
		}

		Constrain* clone()
		{
			return new ConstrainUserDefined(_w, _model_ini, _check_func, _model_change);
		}
	};
}