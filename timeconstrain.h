//------------------------Description------------------------
// This file defines the time variable, which supports the 
// definition of cumulative functions and constraints for
// time-dependent problems.
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
#include<unordered_set>
#include"sort-helper.h"
#include"feasible-region.h"
#include"basicfunc.h"
#include"variable.h"
#include"constrain.h"

namespace ECFC
{
	// ʱ���������һ������ļ�����������ݾ���������м������
	// ʱ��������Զ����ṩ���ɼ�⺯������ʱ���ȣ�ƽ��ռ��ʱ�䣬�����ʣ���Դʹ�ñ�׼��Ⱥ���������Ŀ�궨��
	// ͬʱ������Լ��ֻ������ʱ�������˳��Լ������ʼʱ��Լ��������ʱ��Լ����
	// ����ʱ������������ھ��߱���

	// ���⣺
	// �����ʱ������;��߱����乹�����-���ӵ���Ӧ�ľ�����������������ϵ�Լ��Ҳ���Բο���һ�㣩
	//

	// ��ɢԼ����ʱ����Լ������
	// ����˳��Լ������ֹ��Լ��������ʱ�����ֹʱ�䣩��
	// ˳��Լ���У���������Ӧֻ��֮ǰ�����Լ�������ܺ��������Լ��
	
	// ����дһ���м���������������˼·
	// �м������������������������õ����������Ƕ���ʽҲ������ʱ����
	// ����ھ�������Լ��Ҳ�����ھ���������˹���һ��˫����Ǳ�Ҫ��
	// ��ȫ�����ǵ�����Լ��������������Լ���Ժ��ٿ���
	// ��������Լ���Ĵ�����ô�����أ�
	// ������̣�
	// 1.�������仯
	// 2.�����������仯
	// 3.���õ�������֮��
	// 
	// 
	// ʱ������ʲô����ʲô��
	// ʱ����������һ����Ŀ��ϵͳʱ����̵ķ���ͼ�¼
	// ��Ӧ��������ʱ���ʱ��㼰����ʱ����ϸ��������Ķ�����״̬
	// ���ᱻʩ��һЩԼ�����紦��˳��Լ������ֹʱ��Լ��
	// Ҳ���ܽ�ϵ�����Լ����������Լ�������ṩ�����Ĵ���
	// 
	// �����һ��������ǵ�Ŀǰ���ܽ�ϵ�ʱ������ֻ������Լ����
	// ��ֻ���ʱ�����ٶ���һ������Լ������������ά��
	//

	class ConstrainTimeInterval final : public Constrain
	{
	private:
		int _container_number;
		int _task_number;

		double* _occupy_time; // �����ڸ��ڵ�ռ��ʱ�䣬��taskid����nodeid��
		double* _task_preperation_time; // ����׼��ʱ�䣬��taskid����nodeid��
		double** _task_output_time; // ������ʱ�䣬��taskid����destination����location��
		bool* _task_precedence; // ����������ϵ����taskid����taskid��

		struct Event
		{
		public:
			int taskid;
			int action;
			double timestamp;

			Event(int tskid, int actn, double tm)
			{
				taskid = tskid;
				action = actn;
				timestamp = tm;
			}

			bool operator<(const Event& e) const
			{
				return timestamp < e.timestamp;
			}
		};
		std::vector<Event>* _scheme; // �ڵ��������nodeid����taskid��		
		double* ends; // �������ʱ�䣬��taskid��
		int* allocates; // ������λ�ã���taskid��

		// Լ�������
		double* _task_deadline; // �����ֹ���� ��taskid��
		int _resource_types; // ��Դ������Ŀ
		double* _capacitys; // ��Դ������nodeid����resourceid��
		double* _requirements; //��Դ����taskid����resourceid��

		double getEarliestAvailable(int task_id, int container_id)
		{
			if (_resource_types == 0)
				return 0;

			double* resource_buffer = new double[_resource_types];
			double* require_pointer = _requirements + task_id * _resource_types;
			memcpy(resource_buffer, _capacitys + container_id * _resource_types, _resource_types * sizeof(double));
			
			// ��Ҫ��ָ��ʱ�������ھ����������Դ����
			double start = 0;
			double hold = _occupy_time[task_id * _container_number + container_id];
			double end = start + hold;
			bool has_meet = true;

			// ����������
			for (int r = 0; r < _resource_types; r++)
			{
				if (resource_buffer[r] < require_pointer[r])
				{
					has_meet = false;
					break;
				}
			}
			if (!has_meet)
			{
				return ECFC_MAX;
			}

			for (int i = 0; i < _scheme[container_id].size(); i++)
			{
				if (has_meet && _scheme[container_id][i].timestamp >= end)
					return start;
				for (int j = 0; j < _resource_types; j++)
				{
					resource_buffer[j] -= _scheme[container_id][i].action * _requirements[_scheme[container_id][i].taskid * _resource_types + j];
				}
				if (has_meet)
				{
					if (_scheme[container_id][i].action < 0)
						continue;
					else
					{
						for (int r = 0; r < _resource_types; r++)
						{
							if (resource_buffer[r] < require_pointer[r])
							{
								has_meet = false;
								break;
							}
						}
					}
				}
				else
				{
					if (_scheme[container_id][i].action > 0)
						continue;
					else
					{
						bool success = true;
						for (int r = 0; r < _resource_types; r++)
						{
							if (resource_buffer[r] < require_pointer[r])
							{
								success = false;
								break;
							}
						}
						if (success)
						{
							has_meet = true;
							start = _scheme[container_id][i].timestamp;
							end = start + hold;
						}
					}
				}
			}

			return start;
		}

		double getEarliestStart(int task_id, int container_id)
		{
			double back = 0;
			if (_task_preperation_time != nullptr)
				back = _task_preperation_time[task_id * _container_number + container_id];

			if (_task_precedence != nullptr)
			{
				bool* precedence = _task_precedence + task_id * _task_number;
				double* trans_time = _task_output_time[task_id] + container_id * _container_number;
				double getdata;
				for (int i = 0; i < task_id; i++)
				{
					if (precedence[i])
					{
						getdata = ends[i] + trans_time[allocates[i]];
						if (getdata > back)
						{
							back = getdata;
						}
					}
				}
			}

			return back;
		}

	public:
		ConstrainTimeInterval(double penalty_w, int task_number, int container_number,
			double* occupy_time = nullptr, double* task_output_time = nullptr,
			double* task_preperation_time = nullptr, int* task_precedence = nullptr,
			double* task_deadline = nullptr, int resource_types = 0, double* capacitys = nullptr)
			:Constrain(penalty_w)
		{

		}

		~ConstrainTimeInterval()
		{

		}

		double systemSpan(double** input)
		{

		}

		double systemBalance(double** input)
		{

		}

		void ini()
		{
			for (int i = 0; i < _container_number; i++)
			{
				_scheme[i].clear();
			}
		}

		bool meet(int demension, double value)
		{
			if (_task_deadline == nullptr)
				return true;

			double start = getEarliestStart(demension, value);
			return (start + _occupy_time[demension * _container_number + int(value)]) < _task_deadline[demension];
		}

		void update(int demension, double value)
		{
			double start = getEarliestStart(demension, value);

			std::vector<Event>* _scheme; // �ڵ��������nodeid����taskid��		
			ends[demension] = start + _occupy_time[demension * _container_number + int(value)];
			allocates[demension] = value;

			std::vector<Event>* scheme = _scheme + allocates[demension];
			bool inserted = false;
			for(std::vector<Event>::iterator it = (*scheme).begin(); it!= (*scheme).end(); it++)
			{
				if (it->timestamp > start) //��ʼִ��
				{
					(*scheme).insert(it, Event(demension, 1, start));
					inserted = true;
					break;
				}
			}
			if (!inserted) // ���𵽷���ĩβ
			{
				(*scheme).push_back(Event(demension, 1, start));
				(*scheme).push_back(Event(demension, -1, ends[demension]));
			}
			else
			{
				// ������ֹ�¼�
				inserted = false;
				for (std::vector<Event>::iterator it = (*scheme).begin(); it != (*scheme).end(); it++)
				{
					if (it->timestamp > ends[demension])
					{
						(*scheme).push_back(Event(demension, -1, ends[demension]));
						inserted = true;
						break;
					}
				}
				if (!inserted)
				{
					(*scheme).push_back(Event(demension, -1, ends[demension]));
				}
			}
		}

		double violation(double* solution, int size)
		{
			if (_task_deadline == nullptr)
				return 0;

			ini();
			for (int i = 0; i < size; i++)
			{
				update(i, solution[i]);
			}
			
			double back = 0;
			double over;
			for (int i = 0; i < size; i++)
			{
				over = ends[i] - _task_deadline[i];
				if (over > 0)
					back += over;
			}

			return over;
		}

		void regionReduction(int demensionId, FeasibleLine* region)
		{
			if (_task_deadline == nullptr)
				return;

			for (int cid = 0; cid < _container_number; cid++)
			{
				if (!meet(demensionId, cid))
				{
					region->differ(cid, cid, false, false);
				}
			}
		}
	};
}