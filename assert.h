//------------------------Description------------------------
// This file is consist of class "Assert", which define the basic
// data format of assert system, and class "AssertList", which 
// provide a container for the assertion object
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
#include<vector>
#include"module-type.h"

namespace ECFC
{
	//
	// ���Լ����
	// ÿ�������һ�����Ի�ȡ�ķ�����������������Ķ���
	// ������ڵĶ����Լ̳е���ʽ��֯����һ���������棬�����ఴ�ռ̳н�����ӣ�
	// �������ñ������Ծ����������������
	// 1.���ñ���ָ�������𣺸����������Ŀ���������ƥ��
	// 2.���ñ�δָ�������𣺽�����Ԥƥ��Ķ���������������У�Ȼ���򹤳����л�ȡ��Сƥ�����
	// 
	// ���Լ��ֻ������ñ����
	// �����������ñ�����Ż�������
	// 
	// ÿ���������ṩ
	// 1.������𷵻ض���ָ��
	// 2.������𷵻ض������
	// 
	// ����ƥ�����ṩ
	// 1.�������÷��ض���ƥ�����
	// 2.��������Ӧ���û��ڶ��Է�����Сƥ�����ͺͲ���
	//
	// ����ƥ�乲�������Σ�
	// 1.�Ż������ñ��ڵĶ���ƥ��
	// 2.�Ż���������Ķ���ƥ�䣨һ�㲻���ܾ�orʧ�ܣ�ֻ����ʾ��
	//

	enum class MatchType
	{
		notLess,
		notLessButNotice,
		equal,
		anyButNotice,
		postAssert
	};

	class AssertList;

	class Assert
	{
	public:
		enum class MatchResult {
			unsatisfied,
			satisfied,
			notfullysatisfied,
			failsatisfied
		};

		friend AssertList;

	private:
		int _number;
		std::string _item;
		ModuleType _module_type;
		MatchResult(*match_func)(const Assert& target, const Assert& source);

		static MatchResult matchNotLess(const Assert& target, const Assert& source)
		{
			if(target._item != source._item) //��Ŀ��ƥ��
				return MatchResult::unsatisfied;
			if (source._number < target._number)
				return MatchResult::failsatisfied; // ƥ��ʧ��
			return MatchResult::satisfied;
		}
		static MatchResult matchNotLessButNotice(const Assert& target, const Assert& source)
		{
			if (target._item != source._item) //��Ŀ��ƥ��
				return MatchResult::unsatisfied;
			if (source._number < target._number) // ƥ��ʧ��
				return MatchResult::failsatisfied; 
			if (source._number > target._number) // ��׼ȷƥ��
				return MatchResult::notfullysatisfied;
			return MatchResult::satisfied;
		}
		static MatchResult matchEqual(const Assert& target, const Assert& source)
		{
			if (target._item != source._item) //��Ŀ��ƥ��
				return MatchResult::unsatisfied;
			if (source._number != target._number) // ƥ��ʧ��
				return MatchResult::failsatisfied;
			return MatchResult::satisfied;
		}
		static MatchResult matchAnyButNotice(const Assert& target, const Assert& source)
		{
			if (target._item != source._item) //��Ŀ��ƥ��
				return MatchResult::unsatisfied;
			if (source._number < target._number) // ��׼ȷƥ��
				return MatchResult::notfullysatisfied;
			if (source._number > target._number) // ��׼ȷƥ��
				return MatchResult::notfullysatisfied;
			return MatchResult::satisfied;
		}

	public:
		Assert(ModuleType module_type, std::string item, int number = -1, MatchType match_type = MatchType::notLessButNotice)
		{
			_module_type = module_type;
			_item = item;
			_number = number;
			switch (match_type)
			{
			case ECFC::MatchType::notLess:
				match_func = matchNotLess;
				break;
			case ECFC::MatchType::notLessButNotice:
				match_func = matchNotLessButNotice;
				break;
			case ECFC::MatchType::equal:
				match_func = matchEqual;
				break;
			case ECFC::MatchType::anyButNotice:
				match_func = matchAnyButNotice;
				break;
			case ECFC::MatchType::postAssert:
				match_func = nullptr;
				break;
			default:
				match_func = matchNotLessButNotice;
				break;
			}
		}

		Assert(const Assert& target)
		{
			_module_type = target._module_type;
			_item = target._item;
			_number = target._number;
			match_func = target.match_func;
		}

		~Assert()
		{

		}

		bool itemMatch(const Assert& target)
		{
			return _item == target._item;
		}

		ModuleType getModuleType()
		{
			return _module_type;
		}

		std::string getitem()
		{
			return _item;
		}

		int getNumber()
		{
			return _number;
		}

		Assert* copy()
		{
			return new Assert(*this);
		}

		MatchResult match(const Assert& target)
		{
			return match_func(target, *this);
		}
	};

	class AssertList
	{
	private:
		std::vector<Assert*> _list;
	public:
		AssertList()
		{

		}

		~AssertList()
		{
			for (int i = 0; i < _list.size(); i++)
			{
				delete _list[i];
			}
			_list.clear();
		}

		size_t getSize()
		{
			return _list.size();
		}

		void add(Assert* item, bool fusion_merge = false)
		{
			if (fusion_merge)
			{
				bool added = false;
				for (int i = 0; i < _list.size(); i++)
				{
					if (_list[i]->itemMatch(*item))
					{
						_list[i]->_number = std::max(_list[i]->_number, item->_number);
						added = true;
						delete item;
						break;
					}
				}

				if (!added)
				{
					_list.push_back(item);
				}
			}
			else
			{
				_list.push_back(item);
			}
		}

		Assert& operator[](const int index)
		{
			return *_list[index];
		}
	};
}
