//------------------------Description------------------------
// This file defines the handle of problem module for optimizer,
//  which provides all the functions related to problem handling, 
// including individual evaluation, constrains propogation,
// feasiable region reduction, heuristic decision, etc.
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
#include"variable.h"
#include"solution.h"
#include"calculator.h"
#include"objective.h"
#include"comparer.h"
#include"feasible-region.h"
#include"constrain.h"
#include"inspiration.h"
#include"logger.h"


namespace ECFC
{
/// <summary>
/// �������Լ��������������ʽ
/// �������̣�
/// ��λ����λ��
/// ����ѡ��ֽ�Ϊ����
/// ���ڱ���������ط��������ؽ��
/// 
/// ��������Լ����Эͬ����
/// ��ЧԼ���Ŀ��Ʒ�ʽ��
/// Լ������Խ�ߵļ�������Խ���ӵ�Լ������ʼ�����¿��������Ϊ�ں��������룩
/// 
/// ����ӣ����Ʋ��������������
/// </summary>

	class Problem;

	class ProblemHandle final
	{
	private:
		std::string _name;

		// �����ֵ�ռ�
		int _problem_size; // ���������ܳ��ȣ���ѡ�ⳤ�ȣ�
		ECElement* variables; // ��������ĳ������������������
		int variable_number;
		// ����ֵ�Ŀ�����
		FeasibleLine* feasible_regions_ini; //ÿ�������ĳ�ʼ������
		FeasibleLine* feasible_regions_cur; //ÿ�������ĵ�ǰ������
		int feasible_regions_dem_index; // ά�ȿ�����ĵ�ǰ����
        bool* no_dem_reduction_region; // ����Ҫά�������Ŀ�����
        FeasibleLine* feasible_regions_dem; //ά�ȵĵ�ǰ������
		FeasibleLine* feasible_regions_dem_ptr; //ά�ȵĵ�ǰ�������ָ��
		// �����������Ϣ
		int decision_variable_number; // ����������Ŀ
		int* decision_variable_index; // �������ڱ����е�����
		int* decision_variable_offset; // �������ں�ѡ���е�ƫ��
		int* solution_belong_variable; // ÿ��������ά�������ĸ���
		// �����������Ϣ
		// # To be added
		int calculation_variable_number; // ����������Ŀ
		// int* calculation_variable_index; // �������ڱ����е���������������������˳������
		// int** calculation_require_variable; // ������������Ҫ������������������˳�����У�
		// Calculator* calculation_variable_calculator; // ����������ļ�������


		// ����Ŀ��ռ�
		Objective* _objective;
		int* objective_variable_number; // ����Ŀ����Ҫ������������Ŀ
		int** objective_variable_index; // ����Ŀ����Ҫ��������������
		int* objective_penalty_number; // ����Ŀ����Ҫ��Լ���ͷ�����Ŀ
		int** objective_penalty_index; // ����Ŀ����Ҫ��Լ���ͷ�������		
		double** evaluate_buffer; // ����Ŀ��ʱ�Ļ�������
		int _object_number;
		Comparer* _comparer;

		bool full_evaluate; // �Ƿ����������ǵ�Ŀ�꣨�����ȼ���

		// ����������
		SolutionDecoder* _decoder;

		// �����Լ���ռ�
		struct Con4ElePair
		{
			std::vector<Constrain*> constrains; // ����Լ�����������Ӧ��
			// int variable_offset; // ������ƫ�ã��漰ֻ�б�����һ���ֲ���Լ�������������Ҫ�𣿣�
			// int* constrain_offset; // Լ����ƫ�ã��漰�ж����������ͬһ��Լ���������Ҫ����ƫ�ã�
		};
		Con4ElePair* constrain_pairs; //�������Լ����(�����Ǳ�����)
		int constrain_number; // Լ����Ŀ
		Constrain** constrains; // ����е�����Լ��������Լ������ͬʱԼ��������������ڿ���������ʱ�������⣬�����Ҫͳһ����
		// int* constrain_variable_number; // ����Լ����Ҫ������������Ŀ
		// int** constrain_variable_index; // ����Լ����Ҫ��������������
		int* constrain_variable_index; // ����Լ����Ҫ������������Ŀ

		bool constraint_check; // �Ƿ����ʵʱԼ�����
		ConstrianLevel check_level; // ֻ�е��ڼ����Լ���Ż����Լ��������������

		// ��������ʽ
		Inspiration** inspirations; //�������Ӧ������������ʽ����Ϊ��ָ��
		int* inspiration_variable_number; // ����Ŀ����Ҫ������������Ŀ
		int** inspiration_variable_index; // ����Ŀ����Ҫ��������������
		double** inspirate_buffer; // �������Ļ�������

		// ˽�У�ֻ�б�������Ϊ��Ԫ���Է��ʲ����й���
		ProblemHandle()
		{
			_name = "";

			// �����ֵ�ռ�
			_problem_size = 0;
			variables = nullptr; 
			variable_number = 0;
			// ����ֵ�Ŀ�����
			feasible_regions_ini = nullptr;
			feasible_regions_cur = nullptr;
            feasible_regions_dem_index = EMPTYVALUE;
            no_dem_reduction_region = nullptr;
			feasible_regions_dem = nullptr;
            feasible_regions_dem_ptr = nullptr;
			// �����������Ϣ
			decision_variable_number = 0;
			decision_variable_index = nullptr;
			decision_variable_offset = nullptr;
			solution_belong_variable = nullptr;
			// �����������Ϣ
			// # To be added
			calculation_variable_number = 0;
			// calculation_variable_index; // �������ڱ����е���������������������˳������
			// calculation_require_variable; // ������������Ҫ������������������˳�����У�
			// calculation_variable_calculator; // ����������ļ�������


			// ����Ŀ��ռ�
			_objective = nullptr;
			objective_variable_number = nullptr;
			objective_variable_index = nullptr;
			objective_penalty_number = nullptr;
			objective_penalty_index = nullptr;
			evaluate_buffer = nullptr;
			_object_number = 0;
			_comparer = nullptr;

			full_evaluate = true; // �Ƿ����������ǵ�Ŀ�꣨�����ȼ���

			// ����������
			_decoder = nullptr;

			// �����Լ���ռ�
			constrain_pairs = nullptr;
			constrain_number = 0;
			constrains = nullptr;
			// constrain_variable_number; // ����Լ����Ҫ������������Ŀ
			// constrain_variable_index; // ����Լ����Ҫ��������������
			constrain_variable_index = nullptr;

			constraint_check = true;
			check_level = constraints_customization;

			// ��������ʽ
			inspirations = nullptr;
			inspiration_variable_number = nullptr;
			inspiration_variable_index = nullptr;
			inspirate_buffer = nullptr;
		}

		void regionReduction(int demensionId)
		{
			int vid = solution_belong_variable[demensionId]; // ����id
			int did = demensionId - decision_variable_offset[vid]; // ά���ڱ����ڵ�����
            
            if (no_dem_reduction_region[vid])// ���ٲ���Ҫ�Ŀ������������Ч��
            {
                feasible_regions_dem_ptr = feasible_regions_cur + vid;
                return;
            }

            feasible_regions_dem->copy(feasible_regions_cur + vid);
            feasible_regions_dem_ptr = feasible_regions_dem;
			for (int i = 0; i < constrain_pairs[vid].constrains.size(); i++)
			{
				if (constrain_pairs[vid].constrains[i]->getConstrainLevel() == constraints_discrete)
				// ֻ��Ҫ������ɢԼ������Ϊ1����ʱû������Լ����2������Լ���Ѿ������ڱ�������;3���Զ���Լ����û�кܺõĴ���ʽ
				{
					constrain_pairs[vid].constrains[i]->regionReduction(did, feasible_regions_dem);
				}
			}

			feasible_regions_dem_index = demensionId; // �������������������
		}

		void clearResult()
		{
			for (int i = 0; i < decision_variable_number; i++)
			{
				variables[decision_variable_index[i]].variable.address = nullptr;
			}
		}

		void constructFeasibleRegion()
		{
			delete[] feasible_regions_ini;
			delete[] feasible_regions_cur;
			delete feasible_regions_dem;
            feasible_regions_dem_ptr = nullptr;

			feasible_regions_ini = new FeasibleLine[decision_variable_number];
			feasible_regions_cur = new FeasibleLine[decision_variable_number];
			feasible_regions_dem = new FeasibleLine();

			for (int i = 0; i < decision_variable_number; i++)
			{
				FeasibleLine buffer(&variables[decision_variable_index[i]]._note);
				// sys_logger.debug(variables[decision_variable_index[i]]._note._name);
				feasible_regions_ini[i].copy(&buffer);
				// sys_logger.debug(std::to_string(feasible_regions_ini[i].getLength()));
			}
            
			feasible_regions_dem_index = EMPTYVALUE;
		}

        void buildNoReductionTable()
        {
            no_dem_reduction_region = new bool[decision_variable_number];
            for (int vid = 0; vid < decision_variable_number; vid++)
            {
                no_dem_reduction_region[vid] = true;
                for (int i = 0; i < constrain_pairs[vid].constrains.size(); i++)
                {
                    if (constrain_pairs[vid].constrains[i]->getConstrainLevel() == constraints_discrete)
                        // ֻ��Ҫ������ɢԼ������Ϊ1����ʱû������Լ����2������Լ���Ѿ������ڱ�������;3���Զ���Լ����û�кܺõĴ���ʽ
                    {
                        no_dem_reduction_region[vid] = false;
                        break;
                    }
                }
            }
        }

		double getPenalty4Object(int oid)
		{
			double back = 0;

			int cid, vid;
			// ����ǰ�Ѿ����ӹ��˺�ѡ�⣬�����ظ�����
			for (int i = 0; i < objective_penalty_number[oid]; i++)
			{
				cid = objective_penalty_index[oid][i];
				vid = constrain_variable_index[cid];
				back += constrains[cid]->violation(variables[vid].variable.address, variables[vid].getLength()) * constrains[cid]->getWeight();
			}

			return back;
		}

		void selfBuild()
		{
			// �����Ƚ���
			delete _comparer;
			_comparer = new Comparer(_objective, _object_number);

			// ����������
			delete _decoder;
			_decoder = new SolutionDecoder(decision_variable_number, _object_number);
			for (int i = 0; i < decision_variable_number; i++)
			{
				_decoder->notes[i] = variables[decision_variable_index[i]]._note;
				_decoder->variable_sizes[i] = variables[decision_variable_index[i]].getLength();
			}
			for (int i = 0; i < _object_number; i++)
			{
				_decoder->object_name[i] = _objective[i].getName();
			}
		}

		friend class Problem;

	public:
		ProblemHandle(ProblemHandle& source_handle)
		{
			// ����Ӳ����
			_name = source_handle._name;

			// �����ռ��ʼ��
			variable_number = source_handle.variable_number;
			variables = new ECElement[variable_number];

			// ���Ʊ���
			_problem_size = source_handle._problem_size;
			decision_variable_number = source_handle.decision_variable_number;
			calculation_variable_number = source_handle.calculation_variable_number;
			for (int i = 0; i < variable_number; i++)
			{
				variables[i].copy(source_handle.variables + i);
			}

			// ���ƾ��߱���
			decision_variable_index = new int[decision_variable_number];
			decision_variable_offset = new int[decision_variable_number];
			solution_belong_variable = new int[_problem_size];
			
			memcpy(decision_variable_index, source_handle.decision_variable_index, decision_variable_number * sizeof(int));
			memcpy(decision_variable_offset, source_handle.decision_variable_offset, decision_variable_number * sizeof(int));
			memcpy(solution_belong_variable, source_handle.solution_belong_variable, _problem_size * sizeof(int));
			
			// ��ʼ��������ռ�
			// ������Կ���ֱ�Ӹ��ƿ�����ͬʱ���������Ŀ���������
			feasible_regions_ini = nullptr;
			feasible_regions_cur = nullptr;
			feasible_regions_dem_index = EMPTYVALUE;
            no_dem_reduction_region = nullptr;
            feasible_regions_dem = nullptr;
			feasible_regions_dem_ptr = nullptr;
            constructFeasibleRegion();

			// ���Ƽ�����
			// To be added!
			calculation_variable_number = 0;

			// Ŀ��ռ��ʼ��
			_object_number = source_handle._object_number;
			_objective = new Objective[_object_number];
			evaluate_buffer = new double* [variable_number];
			objective_penalty_number = new int [_object_number]; // ����Ŀ����Ҫ��Լ���ͷ�����Ŀ
			objective_penalty_index = new int* [_object_number]; // ����Ŀ����Ҫ��Լ���ͷ�������	

			full_evaluate = source_handle.full_evaluate;
			_comparer = nullptr;
			_decoder = nullptr;

			// �����Ż�Ŀ��
			for (int i = 0; i < _object_number; i++)
			{
				_objective[i].copy(source_handle._objective + i);
			}

			// ����Ŀ�꺯��������
			objective_variable_number = new int[_object_number];
			objective_variable_index = new int* [_object_number];
			memcpy(objective_variable_number, source_handle.objective_variable_number, _object_number * sizeof(int));
			for (int i = 0; i < _object_number; i++)
			{
				objective_variable_index[i] = new int[objective_variable_number[i]];
				memcpy(objective_variable_index[i], source_handle.objective_variable_index[i], objective_variable_number[i] * sizeof(int));
			}

			// ����Լ���ͷ���
			memcpy(objective_penalty_number, source_handle.objective_penalty_number, _object_number * sizeof(int));
			for (int i = 0; i < _object_number; i++)
			{
				objective_penalty_index[i] = new int[objective_penalty_number[i]];
				memcpy(objective_penalty_index[i], source_handle.objective_penalty_index[i], objective_penalty_number[i] * sizeof(int));
			}

			// Լ���ռ��ʼ��
			constraint_check = source_handle.constraint_check;
			check_level = source_handle.check_level;
			constrain_pairs = new ProblemHandle::Con4ElePair[variable_number];
			constrain_number = source_handle.constrain_number;
			constrains = new Constrain * [constrain_number];
			constrain_variable_index = new int[constrain_number];

			// ����Լ��
			for (int cid = 0; cid < constrain_number; cid++)
			{
				constrains[cid] = source_handle.constrains[cid]->clone();

				// ����Լ��
				for (int vid = 0; vid < variable_number; vid++)
				{
                    if (source_handle.constrain_pairs[vid].constrains.size() == constrain_pairs[vid].constrains.size()) // �ñ���������Լ���Ѿ��������
                        continue;

					if (source_handle.constrain_pairs[vid].constrains[constrain_pairs[vid].constrains.size()]
						== source_handle.constrains[cid]) // Դ�����Լ������cid��Լ����vid�е���һ��Լ������ƥ�䣬��Ӧ����������
					{
						constrain_pairs[vid].constrains.push_back(constrains[cid]);
					}

                    // Ӧ����Լ��
                    if (constrains[cid]->getConstrainLevel() == constraints_range)
                    {
                        constrains[cid]->regionReduction(0, feasible_regions_ini + vid);
                    }

					break;
				}
			
				constrain_variable_index[cid] = source_handle.constrain_variable_index[cid];
			}
            buildNoReductionTable();
            
			// ����ʽ�ռ��ʼ��
			inspirations = new Inspiration * [variable_number];
			inspiration_variable_number = new int[variable_number];
			inspiration_variable_index = new int* [variable_number];
			inspirate_buffer = new double* [variable_number + 2];
			// ��������ʽ
			for (int i = 0; i < variable_number; i++)
			{
				if (source_handle.inspirations[i] != nullptr)
				{
					inspirations[i] = source_handle.inspirations[i]->clone();
				}
				else
				{
					inspirations[i] = nullptr;
				}
			}

			memcpy(inspiration_variable_number, source_handle.inspiration_variable_number, sizeof(int)* variable_number);
			for (int i = 0; i < variable_number; i++)
			{
				inspiration_variable_index[i] = new int[inspiration_variable_number[i]];
				memcpy(inspiration_variable_index[i], source_handle.inspiration_variable_index[i], sizeof(int) * inspiration_variable_number[i]);
			}			

			// ����������Թ���
			selfBuild();
		}

		~ProblemHandle()
		{
			clearResult();
			delete[] variables;
			// ����ֵ�Ŀ�����
			delete[] feasible_regions_ini;
			delete[] feasible_regions_cur;
			delete feasible_regions_dem;
            delete[] no_dem_reduction_region;
            feasible_regions_dem_ptr = nullptr;
			// �����������Ϣ
			delete[] decision_variable_index;
			delete[] decision_variable_offset;
			delete[] solution_belong_variable;
			// �����������Ϣ
			// delete[] calculation_variable_index; // �������ڱ����е���������������������˳������
			// for (int i = 0; i < calculation_variable_number; i++)
			//     delete[] calculation_require_variable[i];
			// delete[] calculation_require_variable;
			// delete[] calculation_variable_calculator; // ����������ļ�������

			// ����Ŀ��ռ�
			delete[] _objective;
			delete[] objective_variable_number;
			for (int i = 0; i < _object_number; i++)
				delete[] objective_variable_index[i];
			delete[] objective_variable_index;

			delete[] objective_penalty_number;
			for (int i = 0; i < _object_number; i++)
				delete[] objective_penalty_index[i];
			delete[] objective_penalty_index;

			delete[] evaluate_buffer;


			delete _comparer;
			delete _decoder;

			// �����Լ���ռ�
			delete[] constrain_pairs; //�������Լ����(�����Ǳ�����)
			for (int i = 0; i < constrain_number; i++)
				delete constrains[i];
			delete[] constrains;
			delete[] constrain_variable_index;

			for (int i = 0; i < variable_number; i++)
				delete inspirations[i];
			delete[] inspirations;
			delete[] inspiration_variable_number;
			for (int i = 0; i < variable_number; i++)
				delete[] inspiration_variable_index[i];
			delete[] inspiration_variable_index;
			delete[] inspirate_buffer;
		}

		std::string getName()
		{
			return _name;
		}

		int getProblemSize()
		{
			return _problem_size;
		}

		int getObjectNumber()
		{
			return _object_number;
		}

		int getVariableNumber()
		{
			return variable_number;
		}

		int getBelongVariableId(int demensionId)
		{
			return solution_belong_variable[demensionId];
		}

		int getWithinVariableId(int demensionId)
		{
			int vid = solution_belong_variable[demensionId];
			return demensionId - decision_variable_offset[vid];
		}

		int getVariableOffset(int vid)
		{
			return decision_variable_offset[vid];
		}

		int getVariableLength(int vid)
		{
			return variables[vid].getLength();
		}

		SolutionDecoder* getSolutionDecoder()
		{
			return _decoder;
		}

		Comparer* getSolutionComparer()
		{
			return _comparer;
		}

		// ����ѡ���еľ�����ͬ�������⻷��֮��
		void setResult(const Solution& solution)
		{
			for (int i = 0; i < decision_variable_number; i++)
			{
				variables[decision_variable_index[i]].variable.address = solution.result + decision_variable_offset[i];
			}
		}

		// ����ѡ���еľ�����ͬ�������⻷��֮��
		void setResult(double* solution)
		{
			for (int i = 0; i < decision_variable_number; i++)
			{
				variables[decision_variable_index[i]].variable.address = solution + decision_variable_offset[i];
			}
		}

		void constrainReset()
		{
			for (int vid = 0; vid < variable_number; vid++)
			{
				for (size_t i = 0; i < constrain_pairs[vid].constrains.size(); i++)
				{
					constrain_pairs[vid].constrains[i]->ini();
				}
			}

			for (int i = 0; i < decision_variable_number; i++)
			{
				feasible_regions_cur[i].copy(feasible_regions_ini + i);
			}

			// Լ��״̬���ã���ǰ�������Ѿ���ʱ����������
			feasible_regions_dem_index = EMPTYVALUE;
		}

		bool constrainCheck(const int demensionId, const double value)
		{
			if (value == EMPTYVALUE) // ���������
				return true;

			// ����ά������Ѱ�Ҷ�Ӧ��Լ��
			int vid = solution_belong_variable[demensionId]; // ����id
			int did = demensionId - decision_variable_offset[vid]; // ά���ڱ����ڵ�����

			// ���Լ��
			for (size_t i = 0; i < constrain_pairs[vid].constrains.size(); i++)
			{
				// ���ڻ��߼�����һ�������ϼ���鲻ͨ��
				if (!constrain_pairs[vid].constrains[i]->meet(did, value))
				{
					return false;
				}
			}

			// ͨ��Լ�����
			return true;
		}

		void constrainChange(const int demensionId, const double value)
		{
			// �ȼ������е�Լ�����ж���ά������Լ�������ڲ���ɶ����漰����ڲ���

			// ����ά������Ѱ�Ҷ�Ӧ��Լ��
			int vid = solution_belong_variable[demensionId]; // ����id
			int did = demensionId - decision_variable_offset[vid]; // ά���ڱ����ڵ�����

			// Լ������
			for (int i = 0; i < constrain_pairs[vid].constrains.size(); i++)
			{
				constrain_pairs[vid].constrains[i]->update(did, value);

				// ��������Ŀ���������
				if (constrain_pairs[vid].constrains[i]->getConstrainLevel() == constrains_variable)
				{
					constrain_pairs[vid].constrains[i]->regionReduction(did, feasible_regions_cur + vid);
				}
			}

			// Լ���޸Ŀ��ܵ��µ�ǰά�ȿ������ʱ����˽��ж���
			feasible_regions_dem_index = EMPTYVALUE;
		}

		double constraintViolation(const Solution& solution)
		{
			double back = 0;
			// �ֽ��ѡ��
			setResult(solution);
			// ���Լ��
			for (int vid = 0; vid < variable_number; vid++)
			{
				for (int i = 0; i < constrain_pairs[vid].constrains.size(); i++)
				{
					back += constrain_pairs[vid].constrains[i]->violation(variables[vid].variable.address, variables[vid].getLength());
				}
			}
			return back;
		}

		bool solutionLegality(const Solution& solution)
		{
			// �ֽ��ѡ��
			setResult(solution);
			double violation;
			// ���Լ��
			for (int vid = 0; vid < variable_number; vid++)
			{
				for (int i = 0; i < constrain_pairs[vid].constrains.size(); i++)
				{
					// ����Υ���ȣ�0��ʾ��Υ����
					violation = constrain_pairs[vid].constrains[i]->violation(variables[vid].variable.address, variables[vid].getLength());
					// ���ڻ��߼�����һ�������ϼ���鲻ͨ��
					if (!equal(violation, 0))
						return false;
				}
			}
			// ͨ���Ϸ��Լ��
			return true;
		}

		void solutionEvaluate(Solution& solution)
		{
			// ���ú�ѡ��
			setResult(solution);
			for (int oid = 0; oid < _object_number; oid++)
			{
				// ��������
				for (int i = 0; i < objective_variable_number[oid]; i++)
				{
					evaluate_buffer[i] = variables[objective_variable_index[oid][i]].variable.address;
				}
				// ����Ŀ��
				solution.fitness[oid] = _objective[oid].getFitness(evaluate_buffer);

				// ʩ�ӳͷ�
				double penalty = getPenalty4Object(oid);
								
				if (!_objective[oid].IsMin())
					penalty *= -1;

				solution.fitness[oid] += penalty;
			}
		}

		int getChoiceNumber(const int demensionId)
		{
			if (feasible_regions_dem_index != demensionId)
				regionReduction(demensionId);
			return feasible_regions_dem_ptr->getLength();
		}

		double* getFeasibleList(const int demensionId)
		{
			int size;
			if (feasible_regions_dem_index != demensionId)
				regionReduction(demensionId);
			return feasible_regions_dem_ptr->getFeasibleList(size);
		}

		void getFeasibleList(const int demensionId, double* &list_buffer, int& size)
		{
			if (feasible_regions_dem_index != demensionId)
				regionReduction(demensionId);
			feasible_regions_dem_ptr->getFeasibleList(list_buffer, size);
		}

		double choiceDiscretized(int demension, double value)
		{
			int vid = solution_belong_variable[demension];
			double begin = variables[vid].getLowbound();
			double accuracy = variables[vid].getAccuracy();

			int length = intelliTrunc((value - begin) / accuracy);
			return begin + length * accuracy;

		}

		double getChoiceHeuristic(const int demensionId, const double choice)
		{
			// ����ά������Ѱ�Ҷ�Ӧ��Լ��
			int vid = solution_belong_variable[demensionId]; // ����id
			int did = demensionId - decision_variable_offset[vid]; // ά���ڱ����ڵ�����

			// ���¿�����
			if (feasible_regions_dem_index != demensionId)
				regionReduction(demensionId);
			
			// ���ò���
			for (int i = 0; i < inspiration_variable_number[vid]; i++)
			{
				inspirate_buffer[i] = variables[inspiration_variable_index[vid][i]].variable.address;
			}

			// ��������ʽֵ
			return inspirations[vid]->getHeuristic(feasible_regions_dem_ptr, did, choice, inspirate_buffer);
		}

		double getPrioriChoice(const int demensionId)
		{
			// ����ά������Ѱ�Ҷ�Ӧ��Լ��
			int vid = solution_belong_variable[demensionId]; // ����id
			int did = demensionId - decision_variable_offset[vid]; // ά���ڱ����ڵ�����

			// ���¿�����
			if (feasible_regions_dem_index != demensionId)
				regionReduction(demensionId);

			// ���ò���
			for (int i = 0; i < inspiration_variable_number[vid]; i++)
			{
				inspirate_buffer[i] = variables[inspiration_variable_index[vid][i]].variable.address;
			}

			// ��������ѡ��
			return inspirations[vid]->getPrioriDecision(feasible_regions_dem_ptr, did, inspirate_buffer);
		}

		void getPrioriChoice(const int demensionId, int size, double* output)
		{
			// ����ά������Ѱ�Ҷ�Ӧ��Լ��
			int vid = solution_belong_variable[demensionId]; // ����id
			int did = demensionId - decision_variable_offset[vid]; // ά���ڱ����ڵ�����

			// ���¿�����
			if (feasible_regions_dem_index != demensionId)
				regionReduction(demensionId);

			// ���ò���
			for (int i = 0; i < inspiration_variable_number[vid]; i++)
			{
				inspirate_buffer[i] = variables[inspiration_variable_index[vid][i]].variable.address;
			}

			// ��ȡ����ѡ��
			int buffersize;
			double* prioribuffer = inspirations[vid]->getPrioriOrder(feasible_regions_dem_ptr, did, inspirate_buffer, buffersize);

			// ��ȡ�����sizeΪ����������
			if (buffersize < size)
				size = buffersize;
			memcpy(output, prioribuffer, size * sizeof(double));

			// ����ڴ�
			delete[] prioribuffer;
		}

		double getCloseChoice(const int demensionId, const double value)
		{
			if (feasible_regions_dem_index != demensionId)
				regionReduction(demensionId);
			return feasible_regions_dem_ptr->getClosestFeasibleDecision(value);
		}

		double getBoundaryChoice(const int demensionId, bool left = true)
		{
			if (feasible_regions_dem_index != demensionId)
				regionReduction(demensionId);
			return feasible_regions_dem_ptr->getBoundaryDecision(left);
		}

		double getVariableUpbound(const int demensionId)
		{
			int vid = solution_belong_variable[demensionId];
			return variables[vid].getUpbound();
		}

		double getVariableLowbound(const int demensionId)
		{
			int vid = solution_belong_variable[demensionId];
			return variables[vid].getLowbound();
		}

		double getRandomChoice(const int demensionId)
		{
			int vid = solution_belong_variable[demensionId];
			
			return feasible_regions_ini[vid].getRandomDecision();
		}

		double getRandomChoiceInspace(const int demensionId)
		{
			if (feasible_regions_dem_index != demensionId)
				regionReduction(demensionId);
			return feasible_regions_dem_ptr->getRandomDecision();
		}

		void getGreedyResult(Solution& solution) 
		{
			setResult(solution);
			
			constrainReset();
			for (int i = 0; i < _problem_size; i++)
			{
				solution[i] = getPrioriChoice(i);
				constrainChange(i, solution[i]);
			}
		}

		// �ı价����
		void changeEnv(const double* values, int variableId)
		{
			memcpy(variables[variableId].variable.address, values, variables[variableId].getLength() * sizeof(double));
		}

		// ����ǰ����ת��Ϊһ��ֻ�Ż����־��߱�����������
		void getSubproblemForVariable(int* remove_variableId, int variable_number)
		{
			// To be added

			// ���¾���������
			decision_variable_index;
			// ���¾�����ƫ��
			decision_variable_offset;
			// ����ά�ȹ���
			solution_belong_variable;
			// ���¾���������Ŀ
			decision_variable_number;
		}

		// ����ǰ����ת��Ϊһ��ֻ�Ż�����Ŀ���������
		void getSubproblemForObject(int* remove_objectId, int object_number)
		{
			// ���������Ż�Ŀ������ȶ���Ϊ-1�������ǣ�
			for (int i = 0; i < object_number; i++)
			{
				_objective[remove_objectId[i]].setPriority(-1);
			}
		}
	};
}

