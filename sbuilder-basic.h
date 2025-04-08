//------------------------Description------------------------
// This file defines some optional swarm builder components,
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
#include"subswarm-constructer.h"

namespace ECFC
{
	class FixSubswarm final : public SubswarmConstructer
	{
	public:
		FixSubswarm()
			:SubswarmConstructer()
		{

		}

		~FixSubswarm()
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

		void build(Subswarm** subswarms, int& swarm_number)
		{
			return;
		}
	};

	class DistanceBuilder final : public SubswarmConstructer
	{
	public:
		DistanceBuilder()
			:SubswarmConstructer()
		{

		}

		~DistanceBuilder()
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

		void build(Subswarm** subswarms, int& swarm_number)
		{
			Individual* i_buffer1, * i_buffer2; // ���建��
			double* distance; // ���������Ÿ���ľ��뻺��
			int* belong_subswarm;
			int* index_subswaarm;
			double distance_buffer;

			// �����СΪ�����Ⱥ��С
			int buffer_size = 0;
			for (int i = 0; i < swarm_number; i++)
			{
				if (subswarms[i]->getSize() > buffer_size)
					buffer_size = subswarms[i]->getSize();
			}
			distance = new double[buffer_size + 1];
			belong_subswarm = new int[buffer_size + 1];
			index_subswaarm = new int[buffer_size + 1];

			for (int i = 0; i < swarm_number; i++)
			{
				// �ҵ�ʣ������е����Ž�
				i_buffer1 = subswarms[i]->getBestIndividualInSwarm();
				for (int j = i + 1; j < swarm_number; j++)
				{
					i_buffer2 = subswarms[j]->getBestIndividualInSwarm();
					if (*i_buffer2 < *i_buffer1)
						i_buffer1 = i_buffer2;
				}

				// �ҵ����������n������
				int subswarm_size = subswarms[i]->getSize();
				// ��ʼ��
				for (int j = 0; j < subswarm_size; j++)
				{
					distance[j] = ECFC_MAX;
				}
				// ��ʼѰ��
				int insert_position;
				for (int j = i; j < swarm_number; j++)
				{
					for (int k = 0; k < subswarms[j]->getSize(); k++)
					{
						distance_buffer = eu_distance(i_buffer1->solution.result, subswarms[j][0][k].solution.result);

						if (distance_buffer < distance[subswarm_size - 1])
						{
							//���ڲ�������Ѱ�Ҳ���λ��
							for (insert_position = subswarm_size - 1; insert_position > 0; insert_position--)
							{
								if (distance[insert_position - 1] > distance_buffer)
								{
									distance[insert_position] = distance[insert_position - 1];
									belong_subswarm[insert_position] = belong_subswarm[insert_position - 1];
									index_subswaarm[insert_position] = index_subswaarm[insert_position - 1];
								}
								else
								{
									break;
								}
							}
							distance[insert_position] = distance_buffer;
							belong_subswarm[insert_position] = j;
							index_subswaarm[insert_position] = k;
						}
					}
				}

				// Ǩ�����Ӧ����Ⱥ
				for (int j = 0; j < subswarm_size; j++)
				{
					// ���ڽ�������ʵ�ָ���Ļ���
					subswarms[i][0][j].swap(subswarms[belong_subswarm[j]][0][index_subswaarm[j]]);
				}
			}

			// ������
			delete[] distance;
			delete[] belong_subswarm;
			delete[] index_subswaarm;
		}
	};

	class FitnessBuilder final : public SubswarmConstructer
	{
	public:
		FitnessBuilder()
			:SubswarmConstructer()
		{

		}

		~FitnessBuilder()
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

		void build(Subswarm** subswarms, int& swarm_number)
		{
			Individual** best_list; // ���建��

			// �����СΪ�����Ⱥ��С
			int buffer_size = 0;
			for (int i = 0; i < swarm_number; i++)
			{
				if (subswarms[i]->getSize() > buffer_size)
					buffer_size = subswarms[i]->getSize();
			}
			best_list = new Individual * [buffer_size + 1];

			int insert_position;
			int subswarm_size;
			for (int i = 0; i < swarm_number; i++)
			{
				subswarm_size = subswarms[i]->getSize();
				// �ҵ�ʣ������е����ŵ�n����
				for (int k = 0; k < subswarm_size; k++)
				{
					//���ڲ�������Ѱ�Ҳ���λ��
					for (insert_position = k - 1; insert_position > 0; insert_position--)
					{
						if ((*subswarms[i])[k] < *best_list[insert_position - 1])
						{
							best_list[insert_position] = best_list[insert_position - 1];
						}
						else
						{
							break;
						}
					}
					best_list[insert_position] = &(*subswarms[i])[k];
				}
				for (int j = i + 1; j < swarm_number; j++)
				{
					for (int k = 0; k < subswarms[j]->getSize(); k++)
					{
						if ((*subswarms[j])[k] < *best_list[subswarm_size - 1])
						{
							//���ڲ�������Ѱ�Ҳ���λ��
							for (insert_position = subswarm_size - 1; insert_position > 0; insert_position--)
							{
								if ((*subswarms[j])[k] < *best_list[insert_position - 1])
								{
									best_list[insert_position] = best_list[insert_position - 1];
								}
								else
								{
									break;
								}
							}
							best_list[insert_position] = &(*subswarms[j])[k];
						}
					}
				}

				// Ǩ�����Ӧ����Ⱥ
				int subswarm_size = subswarms[i]->getSize();
				for (int j = 0; j < subswarm_size; j++)
				{
					// ���ڽ�������ʵ�ָ���Ļ���
					(*subswarms[i])[j].swap(*best_list[j]);
				}
			}
		}

		void build(Subswarm* subswarms, int& swarm_number)
		{
			Individual* i_buffer1, * i_buffer2; // ���建��
			double* distance; // ���������Ÿ���ľ��뻺��
			int* belong_subswarm;
			int* index_subswaarm;
			double distance_buffer;

			// �����СΪ�����Ⱥ��С
			int buffer_size = 0;
			for (int i = 0; i < swarm_number; i++)
			{
				if (subswarms[i].getSize() > buffer_size)
					buffer_size = subswarms[i].getSize();
			}
			distance = new double[buffer_size + 1];
			belong_subswarm = new int[buffer_size + 1];
			index_subswaarm = new int[buffer_size + 1];

			for (int i = 0; i < swarm_number; i++)
			{
				// �ҵ�ʣ������е����Ž�
				i_buffer1 = subswarms[i].getBestIndividualInSwarm();
				for (int j = i + 1; j < swarm_number; j++)
				{
					i_buffer2 = subswarms[j].getBestIndividualInSwarm();
					if (*i_buffer2 < *i_buffer1)
						i_buffer1 = i_buffer2;
				}

				// �ҵ����������n������
				int subswarm_size = subswarms[i].getSize();
				// ��ʼ��
				for (int j = 0; j < subswarm_size; j++)
				{
					distance[j] = ECFC_MAX;
				}
				// ��ʼѰ��
				int insert_position;
				for (int j = i; j < swarm_number; j++)
				{
					for (int k = 0; k < subswarms[j].getSize(); k++)
					{
						distance_buffer = eu_distance(i_buffer1->solution.result, subswarms[j][k].solution.result);

						if (distance_buffer < distance[subswarm_size - 1])
						{
							//���ڲ�������Ѱ�Ҳ���λ��
							for (insert_position = subswarm_size - 1; insert_position > 0; insert_position--)
							{
								if (distance[insert_position - 1] > distance_buffer)
								{
									distance[insert_position] = distance[insert_position - 1];
									belong_subswarm[insert_position] = belong_subswarm[insert_position - 1];
									index_subswaarm[insert_position] = index_subswaarm[insert_position - 1];
								}
								else
								{
									break;
								}
							}
							distance[insert_position] = distance_buffer;
							belong_subswarm[insert_position] = j;
							index_subswaarm[insert_position] = k;
						}
					}
				}

				// Ǩ�����Ӧ����Ⱥ
				for (int j = 0; j < subswarm_size; j++)
				{
					// ���ڽ�������ʵ�ָ���Ļ���
					subswarms[i][j].swap(subswarms[belong_subswarm[j]][index_subswaarm[j]]);
				}
			}

			// ������
			delete[] distance;
			delete[] belong_subswarm;
			delete[] index_subswaarm;
		}
	};

}
