//------------------------Description------------------------
// This file defines some optional environment selection components,
// which are basic and commonly used.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"environment-select.h"

namespace ECFC
{
	//������Ⱥ���и��£�����ʹ�ã��������Ѿ����Ӵ���������ɸ���ʱ�����������������ԣ���
	class NoUpdater final : public EnvirSelect
	{
	public:
		NoUpdater()
			:EnvirSelect(false)
		{

		}

		~NoUpdater()
		{

		}

		static void preAssert(AssertList& list, double* paras)
		{
			list.add(new Assert(ModuleType::T_offspringgenerator, "updated", 1, MatchType::anyButNotice));
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		void update_subswarm(Population& parent, Population& offspring)
		{
			return;
		}
	};

	//�������������滻
	class IndexUpdater final : public EnvirSelect
	{
	public:
		IndexUpdater(bool better_replace)
			:EnvirSelect(better_replace)
		{

		}

		~IndexUpdater()
		{

		}

		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		void update_subswarm(Population& parent, Population& offspring)
		{
			//�滻�ķ�ΧΪ����Ⱥ���С����Сֵ
			int population_size = parent.getSize();
			if (offspring.getSize() < population_size)
			{
				population_size = offspring.getSize();
			}

			//ִ���滻��swapָ���Լ��ٿ���
			for (int i = 0; i < population_size; i++)
			{
				replace(offspring[i], parent[i]);
			}
		}
	};

	//���״����Ӵ���ѡȡ���ŵĲ��ֽ�����һ���������ڸ���Ƚ����Ѷ������걸ƫ���ϵ�������
	class RankUpdater final : public EnvirSelect
	{
	public:
		RankUpdater()
			:EnvirSelect(true)
		{

		}

		~RankUpdater()
		{

		}

		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		void update_subswarm(Population& parent, Population& offspring)
		{
			//������Ⱥ��ֱ�����
			parent.sort();
			offspring.sort();

			//������ͳ�ƺ�ϲ��ķ�ʽ����ֱ�Ӳ�����������˴����Ĳ������
			//ͳ����Ⱥ��Ĺ��ɱ���
			int parent_number = 0;
			int offspring_number = 0;
			int counter = 0;
			while (counter < parent.getSize())
			{
				if (offspring[offspring_number] < parent[parent_number])
				{
					offspring_number++;
				}
				else
				{
					parent_number++;
				}
                counter++;
			}

			//�ϲ�����Ⱥ
			offspring_number--;
			parent_number--;
			counter--;
			while (parent_number != -1 && offspring_number != -1)
			{
				if (offspring[offspring_number] < parent[parent_number])
				{
					parent[counter].swap(offspring[offspring_number]);
					offspring_number--;
					counter--;
				}
				else
				{
					parent[counter].swap(parent[parent_number]);
					parent_number--;
					counter--;
				}
			}
			while (offspring_number != -1)
			{
				parent[counter].swap(offspring[offspring_number]);
				offspring_number--;
				counter--;
			}
		}
	};

	//ʹ���Ӵ�������Ӵ����������Ƶĸ���
	class CloseUpdater final : public EnvirSelect
	{
	public:
		CloseUpdater(bool better_replace)
			:EnvirSelect(better_replace)
		{

		}

		~CloseUpdater()
		{

		}

		static void preAssert(AssertList& list, double* paras)
		{
			// none
		}

		static void postAssert(AssertList& list, double* paras)
		{
			// none
		}

		void update_subswarm(Population& parent, Population& offspring)
		{
			//�滻�ķ�ΧΪ����Ⱥ���С����Сֵ
			int population_size = offspring.getSize();

			//ִ���滻
			double distance_min;
			double distance_current;
			int distance_min_id;
			for (int offspring_id = 0; offspring_id < population_size; offspring_id++)
			{
				//��ʼ��
				distance_min = eu_distance(offspring[offspring_id].solution.result, parent[0].solution.result);
				distance_min_id = 0;

				//���������Ƹ���
				for (int parent_id = 1; parent_id < parent.getSize(); parent_id++)
				{
					distance_current = eu_distance(offspring[offspring_id].solution.result, parent[parent_id].solution.result);
					if (distance_current < distance_min)
					{
						distance_min = distance_current;
						distance_min_id = parent_id;
					}
				}

				//�滻
				replace(offspring[offspring_id], parent[distance_min_id]);
			}
		}
	};
}
