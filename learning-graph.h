//------------------------Description------------------------
// This file defines the learning graph of individuals, which
// represents the learning object topology of individuals.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#include"solution.h"
#include"individual.h"

namespace ECFC
{
	// Ŀ���Ϊ��ָ���ʾ������ѧϰ����ֱ�ӽ����Ӵ�
	class LearningGraph
	{
	private:
		int _point_number;
		int _end_counter;
		int _end_point_size;
		int _graph_size;
		Individual** _start_point;
		Solution** _end_point;
	public:
		LearningGraph(size_t graph_size, size_t end_point_size)
		{
			_point_number = 0;
			_end_counter = 0;
			_graph_size = graph_size;
			_end_point_size = end_point_size;
			_start_point = new Individual * [graph_size];
			_end_point = new Solution * [graph_size * end_point_size];
		}

		~LearningGraph()
		{
			delete[] _start_point;
			delete[] _end_point;
		}

		void addEdge(Individual* start_point, Solution** end_points)
		{
			// Խ����
			//if (_point_number >= _graph_size)
			//	return;

			// ����
			_start_point[_point_number] = start_point;
			memcpy(_end_point + _point_number * _end_point_size, end_points, _end_point_size * sizeof(Solution*));

			// ��������
			_point_number++;
		}

		void addStart(Individual* start_point)
		{
			_start_point[_point_number] = start_point;
			_end_counter = _point_number * _end_point_size;
			_point_number++;
		}

		void addEnd(Solution* end_point)
		{
			_end_point[_end_counter] = end_point;
			_end_counter++;
		}

		size_t getSize()
		{
			return _point_number;
		}

		size_t getEndSize()
		{
			return _end_point_size;
		}

		Individual* getStartPoint(int id)
		{
			return _start_point[id];
		}

		Solution** getEndPoint(int id)
		{
			return _end_point + _end_point_size * id;
		}
	};
}
