//------------------------Description------------------------
// This file defines subswarm object. It is the fundamental unit
// that governed all individual's activities, including the 
// production of offspring, environmental selection, etc.
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
#include"population.h"
#include"best-archive.h"
#include"terminator.h"
#include"environment-select.h"
#include"offspring-generator.h"
#include"problem-handle.h"

namespace ECFC
{
	class Subswarm
	{
	private:
		std::string _id;
		Population* _parent; //�û���population��ֱ���޸Ŀ��ܵ���best�Ĳ�׼ȷ�쳣
		Population* _offspring;

		BestArchive* _best_archive;
		Terminator* _terminator;
		EnvirSelect* _swarm_updater;
		SolutionInitializer* _solution_initializer;
		OffspringGenerator* _offspring_generator;
		// LocalSearcher* _local_seacher;

		Comparer* _comparer_pointer;

		Population** _parent_buffer;
		BestArchive** _archives_buffer;
		int _buffers_size;

		Logger* _logger_point;

	public:
		Subswarm(std::string id, Population* parent, Population* offspring, Terminator* terminator, BestArchive* best_archive, EnvirSelect* swarm_updater,
			SolutionInitializer* solution_initializer, OffspringGenerator* offspring_generator, Logger* logger)
		{
			_id = id;
			_parent = parent;
			_offspring = offspring;
			_best_archive = best_archive;
			_swarm_updater = swarm_updater;
			_solution_initializer = solution_initializer;
			_offspring_generator = offspring_generator;

			_terminator = terminator;
			_offspring_generator->setTerminator(terminator);

			_comparer_pointer = nullptr;

			_parent_buffer = nullptr;
			_archives_buffer = nullptr;
			_buffers_size = 0;

			_logger_point = logger;
		}

		~Subswarm()
		{
			delete _parent;
			delete _offspring;
			delete _best_archive;
			delete _swarm_updater;
			delete _solution_initializer;
			delete _offspring_generator;
			delete _terminator;
			delete[] _parent_buffer;
			delete[] _archives_buffer;
		}

		void setProblem(ProblemHandle* problem_hanlde)
		{
			_parent->setProblem(problem_hanlde);
			_offspring->setProblem(problem_hanlde);
			_solution_initializer->setProblem(problem_hanlde);
			_best_archive->setProblem(problem_hanlde);
			_offspring_generator->setProblem(problem_hanlde);
			_comparer_pointer = problem_hanlde->getSolutionComparer();
		}

		/*
		void setTerminator(Terminator* terminator)
		{
			_terminator = terminator;
			_offspring_generator->setTerminator(terminator);
		} 
		*/
		
		Terminator* getTerminator()
		{
			return _terminator;
		}

		void resetTerminator()
		{
			_terminator->reset();
		}

		bool getBest(Solution*& best_pointer, int& best_size)
		{
			best_pointer = _best_archive->getBest();
			best_size = _best_archive->getBestSize();

			return true;
		}

		int getSize()
		{
			return _parent->getSize();
		}

		void ini()
		{
			_best_archive->clear();

			_parent->ini();
			for (int i = 0; i < _parent->getSize(); i++)
			{
				_best_archive->updateBest((*_parent)[i].solution);
				_terminator->update(true);
			}
			_offspring_generator->ini();
		}

		void nextIteration()
		{
			_offspring_generator->getOffspring(*_offspring, _parent_buffer, _archives_buffer);
			_swarm_updater->update_subswarm(*_parent, *_offspring);
			// _local_seacher->localSearch(_parent, _best_archive);
			_offspring_generator->update(*_parent, *_offspring);
		}

		void setNeibors(Subswarm** neighbors, const int swarm_number)
		{
			if (_buffers_size != swarm_number+1)
			{
				delete[] _parent_buffer;
				delete[] _archives_buffer;
				_buffers_size = swarm_number + 1;
				_parent_buffer = new Population * [_buffers_size + 3];
				_archives_buffer = new BestArchive * [_buffers_size + 3];
				_parent_buffer[0] = _parent;
				_archives_buffer[0] = _best_archive;
			}

			for (int i = 1; i <= swarm_number; i++)
			{
				_parent_buffer[i] = neighbors[i]->_parent;
				_archives_buffer[i] = neighbors[i]->_best_archive;
			}

			_parent_buffer[swarm_number + 1] = nullptr;
			_archives_buffer[swarm_number + 1] = nullptr;
		}

		void run()
		{
			while (!_terminator->termination())
			{
				nextIteration();
				if (_logger_point->process_print())
				{
					int size = 0;
					Solution* bests = nullptr;

					getBest(bests, size);

					for (int i = 0; i < size; i++)
					{
						_logger_point->logprocess(bests[i].decoder_pointer->toString(
							bests[i].result, bests[i].fitness, _logger_point->full_process_print()));
					}

					if (_logger_point->swarm_print())
					{
						size = _parent->getSize();
						for (int i = 0; i < size; i++)
						{
							_logger_point->logprocess((*_parent)[i].solution.decoder_pointer->toString(
								bests[i].result, bests[i].fitness, _logger_point->full_process_print()));
						}
					}
				}
			}
		}

		// �滻�漴����
		void replaceIndividualR(Individual* individual)
		{
			int id = rand() % _parent->getSize();
			_parent[0][id].copy(*individual);
		}

		// �滻���Ÿ���
		void replaceIndividualB(Individual* individual)
		{
			Individual* s = &(_parent[0][0]);
			for (int i = 1; i < _parent->getSize(); i++)
			{
				if (_parent[0][i] < *s)
					s = &(_parent[0][i]);
			}

			s->copy(*individual);
		}

		// �滻������
		void replaceIndividualW(Individual* individual)
		{
			Individual* s = &(_parent[0][0]);
			for (int i = 1; i < _parent->getSize(); i++)
			{
				if (*s < _parent[0][i])
					s = &(_parent[0][i]);
			}

			s->copy(*individual);
		}

		// �滻�������
		void replaceIndividualR(Solution* solution)
		{
			int id = rand() % _parent->getSize();
			_parent[0][id].solution.copy(*solution);
		}

		// �滻���Ÿ���
		void replaceIndividualB(Solution* solution)
		{
			Individual* s = &(_parent[0][0]);
			for (int i = 1; i < _parent->getSize(); i++)
			{
				if (_parent[0][i] < *s)
					s = &(_parent[0][i]);
			}

			s->solution.copy(*solution);
		}

		// �滻������
		void replaceIndividualW(Solution* solution)
		{
			Individual* s = &(_parent[0][0]);
			for (int i = 1; i < _parent->getSize(); i++)
			{
				if (*s < _parent[0][i])
					s = &(_parent[0][i]);
			}

			s->solution.copy(*solution);
		}

		// ��õ�ǰ��Ⱥ�е����Ÿ���
		Individual* getBestIndividualInSwarm()
		{
			Individual* s = &(_parent[0][0]);
			for (int i = 1; i < _parent->getSize(); i++)
			{
				if (_parent[0][i] < *s)
					s = &(_parent[0][i]);
			}

			return s;
		}

		void addIndividual(Individual* individuals, const int number)
		{
			_parent->append(individuals, number);
		}

		void removeIndividual(Individual* individuals, const int number)
		{
			_parent->remove(individuals, number);
		}

		void removeIndividual(int* individual_index, const int number)
		{
			_parent->remove(individual_index, number);
		}

		Individual& operator[](int i)
		{
			return (*_parent)[i];
		}
	};
}
