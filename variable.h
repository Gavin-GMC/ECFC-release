//------------------------Description------------------------
// This file defines the element object in ECFC,
//  which consists of two parts: value and note 
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include<string>
#include"ecfc-constant.h"
#include"logger.h"

namespace ECFC
{

	// ������ɢ�Ż����⣬�ɴ��·�Ϊ���࣬�����Ż����⣬�����Ż�������Ӽ��Ż�����
	// ���ڷ����Ż����⣬��ѡ��ȡ���ڵ�ǰ��ά��
	// ���������Ż����⣬��ѡ��ȡ����֮ǰѡ��
	// �����Ӽ��Ż����⣬��������������п��ܳ���
	//
	enum class VariableType
	{
		normal,
		allocation,
		sequence_direction,
		sequence_bidiagraph,
		sub_module
	};

	// Ԫ��ֵ���󣬰��������ַ����״��Ϣ
	class ECVariable
	{
	private:
		int _length;
		int size_h;
		int size_b;
	public:
		double* address;

		ECVariable()
		{
			address = nullptr;
			size_h = 0;
			size_b = 0;
			_length = 0;
		}

		ECVariable(int length, int height)
		{
			address = nullptr;
			size_h = height;
			size_b = length;
			_length = size_b * size_h;
		}

		~ECVariable()
		{

		}

		void setSize(int length, int height)
		{
			size_h = height;
			size_b = length;
			_length = size_b * size_h;
		}

		int getLength()
		{
			return _length;
		}

		int getHeight()
		{
			return size_h;
		}

		int getWidth()
		{
			return size_b;
		}

		void setAddress(double* pointer)
		{
			address = pointer;
		}

		void copy(const ECVariable& source)
		{
			address = source.address;
			size_h = source.size_h;
			size_b = source.size_b;
			_length = source._length;
		}

		double& operator[](const int index)
		{
			return address[index];
		}
	};

	// Ԫ��ע�Ͷ��󣬰������½磬��ȷ�ȵ���Ϣ
	struct ElementNote
	{
	public:
		std::string _name = "none";
		double _upbound = EMPTYVALUE;
		double _lowbound = EMPTYVALUE;
		double _accuracy = 0;
		VariableType _type = VariableType::normal;
	};

	// ECFC��Ԫ�ض������ڴ��沢��¼������Ż��������漰�ĸ�����
	class ECElement
	{
	private:
		//ElementNote _note;

	public:
		ElementNote _note;

		ECVariable variable;

		ECElement()
		{

		}

		ECElement(std::string name, int length, int height, double upbound, double lowbound, double accuracy, VariableType type = VariableType::normal)
		{
			_note._name = name;
			_note._upbound = upbound;
			_note._lowbound = lowbound;
			_note._accuracy = accuracy;
			_note._type = type;
			variable.setSize(length, height);
		}

		~ECElement()
		{

		}

		std::string getName()
		{
			return _note._name;
		}

		int getLength()
		{
			return variable.getLength();
		}

		double getUpbound()
		{
			return _note._upbound;
		}

		double getLowbound()
		{
			return _note._lowbound;
		}

		double getAccuracy()
		{
			return _note._accuracy;
		}

		VariableType getType()
		{
			return _note._type;
		}

		void copy(ECElement* source)
		{
			_note._name = source->_note._name;
			_note._upbound = source->_note._upbound;
			_note._lowbound = source->_note._lowbound;
			_note._accuracy = source->_note._accuracy;
			_note._type = source->_note._type;
			variable.copy(source->variable);
		}
	};
}