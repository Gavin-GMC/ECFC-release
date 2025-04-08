//------------------------Description------------------------
// This file defines the feasible intervel and region of elements.
// The computation and reduction of intervals are also included.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"basicfunc.h"
#include"ecfc-constant.h"
#include"variable.h"
#include<algorithm>

namespace ECFC
{
	// ��������Ե���������ʽ������
	class Interval
	{
	protected:
		int _length;
		double _accuracy;
		double _begin;
		double _end;

	public:
		Interval* next;

		Interval(double begin, double end, double accuracy, bool left_open, bool right_open)
		{
			_accuracy = accuracy;

			_begin = begin;
			if (left_open)
				_begin += accuracy;

			// ����Ԥ�����ұ߽�����߽�ľ����Ǿ��ȵ��������������Ҫ���ұ߽���ж���
			_length = intelliTrunc((end - _begin) / accuracy);
			_end = _begin + _length * _accuracy;
			if (right_open && equal(_end, end))
			{
				_end -= accuracy;
			}
			else
			{
				_length += 1;
			}		

			next = nullptr;
		}

		Interval(Interval& source)
		{
			_length = source._length;
			_accuracy = source._accuracy;
			_begin = source._begin;
			_end = source._end;
			next = nullptr;
		}

		~Interval()
		{
		}

		double discretized(double value)
		{
			int length = intelliTrunc((value - _begin) / _accuracy);
			return _begin + length * _accuracy;
		}

		// �������е�λ��ת��Ϊʵ��ֵ
		double dereference(int distance)//�������ṩ�ӿڣ�Ԥ�����Ϸ�
		{
#if defined(DEBUGMODE) || defined(FEASIBLEDEBUG)
			sys_logger.debug("Interval:\tResult:\t" + std::to_string(_begin + distance * _accuracy) + "\tAccuracy:\t" + std::to_string(_accuracy) + "\tDistance:\t" + std::to_string(distance) + "\tBegin:\t" + std::to_string(_begin));
#endif
			return _begin + distance * _accuracy;
		}

		double getAccuracy()
		{
			return _accuracy;
		}

		double getLeftBoundary()
		{
			return _begin;
		}

		double getRightBoundary()
		{
			return _end;
		}

		// ������п���ֵ���б�(�������ṩ�ӿڣ�Ԥ�����鳤�����㹻��)
		void getFeasibleList(double* list_buffer)
		{
			list_buffer[0] = _begin;
			for (int i = 1; i < _length; i++)
				list_buffer[i] = list_buffer[i - 1] + _accuracy;
		}

		// ������п���ֵ���б�(�������ṩ�ӿڣ�Ԥ�����鳤�����㹻��)
		void getFeasibleList(double* list_buffer, int size)
		{
			list_buffer[0] = _begin;
			size = std::min(size, _length);
			for (int i = 1; i < size; i++)
				list_buffer[i] = list_buffer[i - 1] + _accuracy;
		}

		int getLength()
		{
			return _length;
		}

		// �����������ȥһ����
		int reductionLeft(double boundary, bool is_open)
		{
			if (boundary < _begin)
				return 0;

			int back = intelliTrunc((boundary - _begin) / _accuracy);//����ֵ����������

			_begin += (back * _accuracy);

			if (_begin < boundary || is_open)
			{
				back += 1;
				_begin += _accuracy;
			}

			_length -= back;

			return back;
		}

		// ��������Ҳ��ȥһ����
		int reductionRight(double boundary, bool is_open)
		{
			if (boundary > _end)
				return 0;

			int back = intelliTrunc((_end - boundary) / _accuracy);

			_end -= (back * _accuracy);

			if (_end > boundary || is_open)
			{
				back += 1;
				_end -= _accuracy;
			}

			_length -= back;

			return back;
		}

		// ������ָ�Ϊ����������
		void split(double cut_position, bool belong_front) // ��Ҫ���и���λ�ý��м�飬ȷ���Ǻ��ʵ�λ��
		{
			if (less(cut_position, _begin) || large(cut_position, _end))
				return;

			//�ָ����䣬�и��һ��һ�������ظ�ֵ
			Interval* buffer;
			if (belong_front)
			{
				if (equal(cut_position, _end))
					return;

				// ����и���Ҳ������λ
				cut_position = _begin + ceil((cut_position - EQACCURACY - _begin) / _accuracy) * _accuracy;

				// �ָ�
				buffer = new Interval(cut_position, _end, _accuracy, true, false);
				reductionRight(cut_position, false);
			}
			else
			{
				if (equal(cut_position, _begin))
					return;

				// ����и����������λ
				cut_position = _begin + floor((cut_position + EQACCURACY - _begin) / _accuracy) * _accuracy;

				// �ָ�
				buffer = new Interval(cut_position, _end, _accuracy, false, false);
				reductionRight(cut_position, true);
			}

			//��������
			buffer->next = next;
			next = buffer;
		}

		// �ж�Ŀ��ֵ�Ƿ��ڵ�ǰ������
		bool isIn(double value)
		{
			return notless(value, _begin) && notlarge(value, _end);
		}
	};
}
