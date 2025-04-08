//------------------------Description------------------------
// This file defines the calculation process object in ECFC,
// which is used to encapsulate and provide a uniform interface
// for different forms of computing processes
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------
#pragma once
#include"ecfc-functor.h"

namespace ECFC
{
	// �������ļ���ʽ��֧�ּ������ͺ���ָ��������ʽ��
	class Calculator
	{
	protected:
		int _parameter_number;

	public:
		Calculator(int parameter_number)
		{
			_parameter_number = parameter_number;
		}

		virtual ~Calculator()
		{

		}

		// ��������ֵ���㣬���������¼
		virtual void run(double** input, double* output) = 0;

		int getParameterNumber()
		{
			return _parameter_number;
		}

		virtual Calculator* copy() = 0;
	};

	// ���ں���ָ��ļ�����
	class FuncCalculator : public Calculator
	{
	private:
		double (*_function)(double** input);

	public:
		FuncCalculator(double (*function)(double** input), int parameter_number)
			:Calculator(parameter_number)
		{
			_function = function;
		}

		~FuncCalculator()
		{

		}

		void run(double** input, double* output)
		{
			*output = _function(input);
		}

		Calculator* copy()
		{
			return new FuncCalculator(_function, _parameter_number);
		}
	};

	// ���ڷº����ļ�����
	class FunctorCalculator : public Calculator
	{
	private:
		eccalcul_functor* _function;

	public:
		FunctorCalculator(eccalcul_functor* function, int parameter_number)
			:Calculator(parameter_number)
		{
			_function = function->copy();
		}

		~FunctorCalculator()
		{
			delete _function;
		}

		void run(double** input, double* output)
		{
			*output = (*_function)(input);
		}

		Calculator* copy()
		{
			return new FunctorCalculator(_function, _parameter_number);
		}
	};

	//���ڼ������ļ�����
	class TreeCalculator
	{
	private:
		// ������ָ��
	public:
		TreeCalculator()
		{
			// # To be added
		}

		~TreeCalculator()
		{
			// # To be added
		}

		void run(double** input, double* output)
		{
			// # To be added
		}

		Calculator* copy()
		{
			// # To be added
			return nullptr;
		}
	};
}