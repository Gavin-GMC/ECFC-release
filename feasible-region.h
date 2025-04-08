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
#include<algorithm>
#include"ecfc-constant.h"
#include"variable.h"
#include"interval.h"

namespace ECFC
{
	// ÿ�������Ŀ����򹹳ɣ�
	// ��ʼ�������ɱ���ȡֵ��Χ����
	// ���ÿ�����������Ӧ��������ά�ȵ�Լ������һԼ���ȣ�
	// ά�ȿ������ȿ������ÿ�������Ӧ�����ά�ȵ�Լ��������Լ������С��Լ���ȣ�

	// �ɶ���������乹�ɵ�����������
	class FeasibleLine
	{
	private:
		Interval* _root; // ���ڵ㣬��ʵ�����������ָʾ
		int _length;

		// ����λ�÷��ؿ���ֵ
		double dereference(int position)
		{
			Interval* iter = _root->next;
			while (iter != nullptr)
			{
				if (position < iter->getLength())
					return iter->dereference(position);
				position -= iter->getLength();
				iter = iter->next;
			}

			return EMPTYVALUE;
		}

		void copy(Interval* intervals)
		{
			deleteSubsequentInterval(_root);

			if (intervals == nullptr)
				return;

			_root->next = new Interval(*intervals);

			Interval* iter = _root->next;
			_length = iter->getLength();

			while (intervals->next != nullptr)
			{
				intervals = intervals->next;
				iter->next = new Interval(*intervals);
				iter = iter->next;
				_length += iter->getLength();
			}
		}

		// ɾȥ�������һ������
		void deleteNextInterval(Interval* interval)
		{
			Interval* delete_buffer;
			delete_buffer = interval->next;
			interval->next = interval->next->next;
			_length -= delete_buffer->getLength();

			delete delete_buffer;
		}

		// ɾȥ����ĺ�������
		void deleteSubsequentInterval(Interval* root)
		{
			if (root == nullptr || root->next == nullptr)
				return;

			Interval* delete_iter = root->next;
			root->next = nullptr;
			root = delete_iter;

			while (delete_iter != nullptr)
			{
				root = root->next;
				_length -= delete_iter->getLength();
				delete delete_iter;
				delete_iter = root;
			}
		}

		// ��Ŀ��������������
		void insertInterval(Interval* interval, Interval* insert_interval)
		{
			insert_interval->next = interval->next;
			interval->next = insert_interval;
			_length += insert_interval->getLength();
		}

		// �Ƴ����䣨��ֹ���ұ߽磩
		Interval* regionReductionOut(Interval* root, double left, double right, bool left_open, bool right_open)
		{
			//���������Ը�Ϊ���䣬��Ϊ���漰������;��ȵ�����
			Interval* iter = root;

			// ��λ��߽�����ǰ�������������м���ߵ�ǰ������м䣨ͨ���и�ʵ�ֶ�λ��������֮�䣩
			if (left_open)
			{
				while (iter->next != nullptr && notless(left, iter->next->getLeftBoundary()))
				{
					iter = iter->next;
				}
				if (less(left, iter->getRightBoundary()))
				{
					// ����ǿ����䣬���и�㲻���Ƴ�����ǰ�����������
					iter->split(left, true);
				}
			}
			else
			{
				while (iter->next != nullptr && large(left, iter->next->getLeftBoundary()))
				{
					iter = iter->next;
				}
				if(notlarge(left, iter->getRightBoundary()))
				{
					// ����Ǳ����䣬���и�㱻�Ƴ����������
					iter->split(left, false);
				}
			}

			// ��λ�ұ߽磬���Ƴ���䲻��������
			if (right_open)
			{
				while (iter->next != nullptr && large(right, iter->next->getRightBoundary()))
				{
					deleteNextInterval(iter);
				}

				if (iter->next != nullptr && large(right, iter->next->getLeftBoundary()))
				{
					// ����ǿ����䣬���и�㲻���Ƴ�����������������
					iter->next->split(right, false);
					deleteNextInterval(iter);
				}
			}
			else
			{
				while (iter->next != nullptr && notless(right, iter->next->getRightBoundary()))
				{
					deleteNextInterval(iter);
				}

				if (iter->next != nullptr && notless(right, iter->next->getLeftBoundary()))
				{
					// ����Ǳ����䣬���и�㱻�Ƴ�����ǰ����
					iter->next->split(right, true);
					deleteNextInterval(iter);
				}
			}

			return iter;
		}

		// �����򣨽�ֹ���ұ߽磩
		Interval* regionReductionTo(Interval* root, double left, double right, bool left_open, bool right_open)
		{
			Interval* iter = root;

			// ��λ��߽�
			if (left_open)
			{
				while (iter->next != nullptr && notless(left, iter->next->getRightBoundary()))
				{
					deleteNextInterval(iter);
				}

				if (iter->next == nullptr)
				{
					return iter;
				}

				if (notless(left, iter->getLeftBoundary()))
				{
					// ����ǿ����䣬���и�㱻�Ƴ�����ǰ����
					iter->next->split(left, true);

					deleteNextInterval(iter);
				}
			}
			else
			{
				while (iter->next != nullptr && large(left, iter->next->getRightBoundary()))
				{
					deleteNextInterval(iter);
				}

				if (iter->next == nullptr)
				{
					return iter;
				}
				if (large(left, iter->next->getLeftBoundary()))
				{
					// ����Ǳ����䣬���и�㱻�������������
					iter->next->split(left, false);

					deleteNextInterval(iter);
				}
			}

			// ��λ�ұ߽�
			if (right_open)
			{
				while (iter->next != nullptr && less(iter->next->getLeftBoundary(), right))
				{
					iter = iter->next;
				}
				// if (right <= iter->getRightBoundary())
				if (notless(right, iter->getRightBoundary()))
				{
					// ����ǿ��������и�������䣬�Ӷ����Ƴ�
					iter->split(right, false);
				}
			}
			else
			{
				while (iter->next != nullptr && notlarge(iter->next->getLeftBoundary(), right))
				{
					iter = iter->next;
				}
				// if (right < iter->getRightBoundary())
				if (less(right, iter->getRightBoundary()))
				{
					// ����Ǳ��������и���ǰ���䣬�Ӷ�������
					iter->split(right, true);
				}
				deleteSubsequentInterval(iter->next);
			}

			return iter;
		}

		//�ϲ��򣨽�ֹ���ұ߽磩
		Interval* regionConnectWith(Interval* root, double left, double right, bool left_open, bool right_open, double accuary)
		{
			Interval* iter = root;

			// ��λ��߽�
			if (left_open)
			{
				while (iter->next != nullptr && notless(left, iter->next->getLeftBoundary()))
				{
					iter = iter->next;
				}
				if (less(left, iter->getRightBoundary()))
				{
					left = iter->getRightBoundary();
				}
				else
				{
					// ����߽���������ʵ�λ�ã�ֻ�����ţ�
					double left_buffer;
					if (iter->getRightBoundary() != ECFC_MIN) // ����λ�ò�Ϊ���ڵ��
					{
						left_buffer = iter->getRightBoundary() + ceil((left - iter->getRightBoundary()) / accuary) * accuary;

						if (left_buffer != left) // ����߽��Ѿ���С�ˣ���Ҫ��Ϊ������
						{
							left = left_buffer;
							left_open = false;
						}
					}
					else // ����λ��Ϊ���ڵ����Ҫ�ο���������
					{
						if (iter->next != nullptr) // �к�������
						{
							left_buffer = iter->next->getLeftBoundary() - floor((iter->next->getLeftBoundary() - left) / accuary) * accuary;

							if (left_buffer != left) // ����߽��Ѿ���С�ˣ���Ҫ��Ϊ������
							{
								left = left_buffer;
								left_open = false;
							}
						}
					}
				}
			}
			else
			{
				// while (iter->next != nullptr && left > iter->next->getLeftBoundary())
				while (iter->next != nullptr && large(left, iter->next->getLeftBoundary()))
				{
					iter = iter->next;
				}
				// if (left <= iter->getRightBoundary())
				if (notlarge(left, iter->getRightBoundary()))
				{
					// ����߽���������ʵ�λ�ã�ֻ�����ţ�
					left = iter->getRightBoundary();

					// ��Ҫ�����������䣬��Ȼ�����ظ�
					left_open = true;
				}
				else
				{
					// ����߽���������ʵ�λ�ã�ֻ�����ţ�
					if (iter->getRightBoundary() != ECFC_MIN) // ����λ�ò�Ϊ���ڵ��
					{
						left = iter->getRightBoundary() + ceil((left - iter->getRightBoundary()) / accuary) * accuary;
					}
					else
					{
						if (iter->next != nullptr) // �к�������
						{
							left = iter->next->getLeftBoundary() - floor((iter->next->getLeftBoundary() - left) / accuary) * accuary;
						}
					}
				}
			}

			if (left > right || (left == right && (left_open || right_open))) // �������ѱ����������
			{
				return iter;
			}

			// ��������
			insertInterval(iter, new Interval(left, right, accuary, left_open, right_open));
			iter = iter->next;

			// ��λ�ұ߽磬ֻ�бյ���������Ƴ�����ظ�����
			// while (iter->next != nullptr && iter->getRightBoundary() >= iter->next->getRightBoundary())
			while (iter->next != nullptr && notless(iter->getRightBoundary(), iter->next->getRightBoundary()))
			{
				deleteNextInterval(iter);
			}

			// if (iter->next != nullptr && iter->getRightBoundary() >= iter->next->getLeftBoundary())
			if (iter->next != nullptr && notless(iter->getRightBoundary(), iter->next->getLeftBoundary()))
			{
				// ����Ǳ����䣬���и���Ƴ�����ǰ����
				iter->next->split(iter->getRightBoundary(), true);
				deleteNextInterval(iter);
			}

			return iter;
		}

	public:
		FeasibleLine()
		{
			_root = new Interval(ECFC_MIN, ECFC_MIN, 1, false, true);

			_length = 0;
		}

		FeasibleLine(const double* source, const int size)
		{
			_root = new Interval(ECFC_MIN, ECFC_MIN, 1, false, true);
			Interval* iter = _root;

			double* buffer = new double[size];
			memcpy(buffer, source, size * sizeof(double));
			std::sort(buffer, buffer + size);// ��������

			// ����ֵ
			for (int i = 0; i < size; i++)
			{
				// ȥ�أ�������
				iter->next = new Interval(buffer[i], buffer[i], 1, false, false);
				iter = iter->next;
			}
			_length = size;

			delete[] buffer;
		}

		FeasibleLine(FeasibleLine& source)
		{
			copy(source._root);
		}

		FeasibleLine(ElementNote* variable_note)
		{
			_root = new Interval(ECFC_MIN, ECFC_MIN, 1, false, true);
			_root->next = new Interval(variable_note->_lowbound, variable_note->_upbound, variable_note->_accuracy, false, false);
			_length = _root->next->getLength();
		}

		~FeasibleLine()
		{
			deleteSubsequentInterval(_root);
			delete _root;
		}

		double getDiscretized(double value)
		{
			Interval* iter = _root->next;
			while (iter != nullptr)
			{
				if (iter->isIn(value))
					return iter->discretized(value);
				iter = iter->next;
			}
			return EMPTYVALUE;
		}

		// ��ȡ�������ֵ
		double getRandomDecision()
		{
			if (_length == 0)
				return ECFC::EMPTYVALUE;
			return dereference(wide_rand() % _length);
		}

		// ��ȡ�߽�ֵ
		double getBoundaryDecision(bool get_left = true)
		{
			// ��Ϊ��
			if (_length == 0)
			{
				return EMPTYVALUE;
			}

			if (get_left)
				return _root->next->getLeftBoundary();

			Interval* iter = _root->next;
			while (iter->next != nullptr)
				iter = iter->next;
			return iter->getRightBoundary();
		}

		// ��ȡĿ��ֵ�����ڽ�����ֵ
		double getClosestFeasibleDecision(double value)
		{
			//��λ�����������������
			Interval* iter = _root;
			while (iter->next != nullptr && value >= iter->next->getLeftBoundary())
			{
				iter = iter->next;
			}

			//�����ǿ��н�
			if (value <= iter->getRightBoundary())
				return value;

			//�޺�������
			if (iter->next == nullptr)
				return iter->getRightBoundary();

			//�������
			double distance = value - iter->getRightBoundary();
			if (distance <= (iter->next->getLeftBoundary() - value))
				return iter->getRightBoundary();
			else
				return iter->next->getLeftBoundary();
		}

		// ��ȡ���п���ֵ���ɵ��б�
		double* getFeasibleList(int& list_size)
		{
			list_size = _length;

			double* back = new double[_length];
			Interval* iter = _root;

			int offset = 0;
			while (iter != nullptr)
			{
				// ��ȡ����εĿ���ֵ
				iter->getFeasibleList(back + offset);
				// �������հ�ָ�����ƫ��
				offset += iter->getLength();
				iter = iter->next;
			}

			return back;
		}

		void getFeasibleList(double*& list_buffer, int& list_size)
		{
			if (list_size == EMPTYVALUE)
			{
				list_buffer = getFeasibleList(list_size);
			}
			else
			{
				Interval* iter = _root;

				int offset = 0;
				int left_size = list_size;
				while (iter != nullptr && left_size > 0)
				{
					// ��ȡ����εĿ���ֵ
					iter->getFeasibleList(list_buffer + offset, left_size);
					// �������հ�ָ�����ƫ��
					offset += iter->getLength();
					left_size -= iter->getLength();
					iter = iter->next;
				}

				if (left_size < 0)
					left_size = 0;
				list_size -= left_size;
			}
		}

		int getLength()
		{
			return _length;
		}

		// ����
		void merge(double left, double right, bool left_open, bool right_open, double accuary)
		{
			regionConnectWith(_root, left, right, left_open, right_open, accuary);
		}

		// ����
		void merge(Interval* target)
		{
			regionConnectWith(_root, target->getLeftBoundary(), target->getRightBoundary(), false, false, target->getAccuracy());
		}

		// ����
		void merge(FeasibleLine* target)
		{
			Interval* iter = target->_root->next;
			Interval* root = _root;
			while (iter != nullptr)
			{
				root = regionConnectWith(root, iter->getLeftBoundary(), iter->getRightBoundary(), false, false, iter->getAccuracy());
				iter = iter->next;
			}
		}

		// ����
		void unite(double left, double right, bool left_open, bool right_open)
		{
			regionReductionTo(_root, left, right, left_open, right_open);
			// Interval* root = regionReductionTo(_root, left, right, left_open, right_open);
			// deleteSubsequentInterval(root);
		}

		// ����
		void unite(Interval* target)
		{
			Interval* root = regionReductionTo(_root, target->getLeftBoundary(), target->getRightBoundary(), false, false);
			deleteSubsequentInterval(root);
		}

		// ����
		void unite(FeasibleLine* target)
		{
			Interval* iter = target->_root->next;
			Interval* root = _root;
			while (iter != nullptr)
			{
				root = regionReductionTo(root, iter->getLeftBoundary(), iter->getRightBoundary(), false, false);
				iter = iter->next;
			}

			deleteSubsequentInterval(root);
		}

		// �
		void differ(double left, double right, bool left_open, bool right_open)
		{
			regionReductionOut(_root, left, right, left_open, right_open);
		}

		// �
		void differ(Interval* target)
		{
			regionReductionOut(_root, target->getLeftBoundary(), target->getRightBoundary(), false, false);
		}

		// �
		void differ(FeasibleLine* target)
		{
			Interval* iter = target->_root->next;
			Interval* root = _root;
			while (iter != nullptr)
			{
				root = regionReductionOut(root, iter->getLeftBoundary(), iter->getRightBoundary(), false, false);
				iter = iter->next;
			}
		}

		void copy(FeasibleLine* source)
		{
			copy(source->_root->next);
		}

		bool isEmpty()
		{
			return _length == 0;
		}

		bool isin(double value)
		{
			Interval* iter = _root->next;
			while (iter != nullptr)
			{
				if (iter->isIn(value))
					return true;

				iter = iter->next;
			}

			return false;
		}

		void clear()
		{
			deleteSubsequentInterval(_root);
		}
	};
}
