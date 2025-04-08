//------------------------Description------------------------
// This file defines the interface of problem module for user,
//  which provides all the functions related to problem definition, 
// including variable and constant addition, goal definition, 
// constraint addition, heuristic addition, etc.
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
#include"problem-handle.h"

namespace ECFC
{
	// ���ⶨ���߼�
	// �û�ͨ���ӿڽ��������ⶨ�壬����������ʽ�洢
	// �û������ֶ����ñ��뷽�����������������Ż��������Ż������б���
	// ����ʱ���Ƚ�����ʽ��飬ͨ����Ż���б���
	// ��ʽ�������Ƿ��������ؼ��֣����ظ���������������ȱʧ������
	// 

	class Problem
	{
	private:
		std::string name; // ��������
		
		std::vector<ECElement*> elements;
		std::vector<bool> decision_variable;
		std::vector<bool> calculate_variable;

		std::vector<Objective*> objectives;
		std::vector<std::vector<std::string>> related_variables_o;

		enum class ConstrainType { user, range, compatibility, capacity, unique, distributed, mindistance };
		std::vector<ConstrainType> constrains;
		std::vector<long long int> addresses_c;
		std::vector<double*> parameters_c;
		std::vector<int> penaltys;
		std::vector<std::vector<std::string>> related_variables_c;
		std::vector<std::vector<std::string>> related_objects_c;
		// std::vector<std::string> related_variables_c;
		// std::vector<int> related_vnumber_c;
		bool not_check_constrain = false;

		// enum class HeuristicType { user_d, user_s, user_m, random, boundary };
		// std::vector<HeuristicType> inspirations;
		// std::vector<long long int> addresses_i;
		std::vector<Inspiration*> heuristics;
		std::vector<std::string> related_variable_i;
		std::vector<std::vector<std::string>> input_variables_i;
		std::vector<bool> stable_heuristic;

		bool compil_check;

		int _findVid(std::string vname)
		{
			int vid = 0;
			while (vid < elements.size())
			{
				if (vname == elements[vid]->getName())
				{
					return vid;
				}
				vid++;
			}
			return -1;
		}

		// ��ͻ�������
		bool _duplicationCheck(std::string name, std::string type)
		{
			// �����ռ���
			for (int i = 0; i < elements.size(); i++)
			{
				if (name == elements[i]->getName())
				{
					// ϵͳ����־�������
					sys_logger.error("The type name duplicate with existing variable!");
					return false;
				}
			}
			// Ŀ��ռ���
			for (int i = 0; i < objectives.size(); i++)
			{
				if (name == objectives[i]->getName())
				{
					// ϵͳ����־�������
					sys_logger.error("The type name duplicate with existing object!");
					return false;
				}
			}

			return true;
		}

		// �걸�Լ��
		bool _completenessCheck()
		{
			// ���������������������
			// To be add

			// Ŀ�꺯�����������������
			int object_number = int(objectives.size());
			for (int i = 0; i < object_number; i++)
			{
				int v_number = int(related_variables_o[i].size());

				// ���ұ�����������������
				int vid;
				for (int vcounter = 0; vcounter < v_number; vcounter++)
				{
					vid = 0;
					while (vid < elements.size())
					{
						if (related_variables_o[i][vcounter] == elements[vid]->getName()) // find
						{
							break;
						}
						vid++;
					}
					if (vid == elements.size()) // ʧ�ܴ���
					{
						// ϵͳ����־�������
						sys_logger.error("The variable '" + related_variables_o[i][vcounter] + "' involved in objective '" + objectives[i]->getName() + "' is undefined!");
						return false;
					}
				}
			}

			// Լ������������Ŀ���������
			int c_number = constrains.size();
			for (int i = 0; i < c_number; i++)
			{
				int v_number = related_variables_c[i].size();

				if (v_number != 1)// ʧ�ܴ���
				{
					// ϵͳ����־�������
					sys_logger.error("Found constrain related to more than one variable!");
					return false;
				}

				// ���ұ�����
				int vid;
				for (int vcounter = 0; vcounter < v_number; vcounter++)
				{
					vid = 0;
					while (vid < elements.size())
					{
						if (related_variables_c[i][vcounter] == elements[vid]->getName()) // find
						{
							break;
						}
						vid++;
					}
					if (vid == elements.size()) // ʧ�ܴ���
					{
						// ϵͳ����־�������
						sys_logger.error("The variable '" + related_variables_c[i][vcounter] + "' involved in constrains '" + "' is undefined!");
						return false;
					}
				}

				// ����Ŀ����
				int o_number = related_objects_c[i].size();
				int oid;
				for (int ocounter = 0; ocounter < o_number; ocounter++)
				{
					oid = 0;
					while (oid < objectives.size())
					{
						if (related_objects_c[i][ocounter] == objectives[oid]->getName()) // find
						{
							break;
						}
						oid++;
					}
					if (oid == objectives.size()) // ʧ�ܴ���
					{
						// ϵͳ����־�������
						sys_logger.error("The objective '" + related_objects_c[i][ocounter] + "' involved in constrains '" + "' is undefined!");
						return false;
					}
				}
			}



			// ����ʽ���������������
			int h_number = heuristics.size();
			for (int i = 0; i < h_number; i++)
			{
				int v_number = input_variables_i[i].size();

				// ���ұ�����
				int vid;
				for (int vcounter = 0; vcounter < v_number; vcounter++)
				{
					vid = 0;
					while (vid < elements.size())
					{
						if (input_variables_i[i][vcounter] == elements[vid]->getName()) // find
						{
							break;
						}
						vid++;
					}
					if (vid == elements.size()) // ʧ�ܴ���
					{
						// ϵͳ����־�������
						sys_logger.error("The variable '" + input_variables_i[i][vcounter] + "' involved in heuristic information for '" + related_variable_i[i] + "' is undefined!");
						return false;
					}
				}
			}

			// ��ɼ��
			return true;
		}
		
		// ����Ϸ��Լ��
		bool _computationCheck()
		{
			// ������״���

			// ������

			return true;
		}

		// ������
		bool _compilationCheck()
		{
			if (compil_check)
			{
				if (!_completenessCheck())
				{
					sys_logger.error("The completeness check fails!");
					return false;
				}

				if (!_computationCheck())
				{
					sys_logger.error("The computation check fails!");
					return false;
				}
			}
			else
			{
				sys_logger.warning("Compilation checks are disabled!");
			}

			return true;
		}

		// ��������
		void _compileVariable(ProblemHandle* target)
		{
			// �����ռ��ʼ��
			target->variable_number = elements.size();
			target->variables = new ECElement[target->variable_number];

			// ���ñ���
			target->_problem_size = 0;
			target->decision_variable_number = 0;
			target->calculation_variable_number = 0;
			for (int i = 0; i < elements.size(); i++)
			{
				target->variables[i].copy(elements[i]);
				if (decision_variable[i])
				{
					target->decision_variable_number++;
					target->_problem_size += target->variables[i].getLength();
				}
				if (calculate_variable[i])
				{
					target->calculation_variable_number++;
				}
					
			}

			// ���þ��߱���
			target->decision_variable_index = new int[target->decision_variable_number];
			target->decision_variable_offset = new int[target->decision_variable_number];
			target->solution_belong_variable = new int[target->_problem_size];
			int offset_ptr = 0;
			int decison_ptr = 0;
			int v_length;
			for (int i = 0; i < elements.size(); i++)
			{
				if (decision_variable[i])
				{
					target->decision_variable_index[decison_ptr] = i;
					target->decision_variable_offset[decison_ptr] = offset_ptr;
					v_length = elements[i]->getLength();
					for (int j = 0; j < v_length; j++)
					{
						target->solution_belong_variable[offset_ptr++] = i;
					}
					decison_ptr++;
				}
			}
			// ��ʼ��������ռ�
			target->constructFeasibleRegion();

			// ���ü�����
			// To be added!
		}

		// Ŀ�����
		void _compileObjective(ProblemHandle* target)
		{
			// Ŀ��ռ��ʼ��
			target->_object_number = objectives.size();
			target->_objective = new Objective[objectives.size()];
			target->evaluate_buffer = new double* [target->variable_number];

			// �����Ż�Ŀ��
			for (int i = 0; i < objectives.size(); i++)
			{
				target->_objective[i].copy(objectives[i]);
			}

			// ����Ŀ�꺯��������
			target->objective_variable_number = new int[target->_object_number];
			target->objective_variable_index = new int* [target->_object_number];
			
			for (int i = 0; i < target->_object_number; i++)
			{
				int v_number = related_variables_o[i].size();
				target->objective_variable_index[i] = new int[v_number];
				target->objective_variable_number[i] = v_number;

				// ���ұ�����������������
				int vid;
				for (int vcounter = 0; vcounter < v_number; vcounter++)
				{
					vid = 0;
					while (vid < elements.size())
					{
						if (related_variables_o[i][vcounter] == elements[vid]->getName())
						{
							target->objective_variable_index[i][vcounter] = vid;
							break;
						}
						vid++;
					}
				}
			}
		}

		// Լ������
		void _compileConstrain(ProblemHandle* target)
		{
			// Լ���ռ��ʼ��
			target->constraint_check = !not_check_constrain;
			target->constrain_pairs = new ProblemHandle::Con4ElePair[elements.size()];
			target->constrain_number = constrains.size();
			target->constrains = new Constrain * [constrains.size()];
			target->constrain_variable_index = new int [constrains.size()];

			target->objective_penalty_number = new int[objectives.size()];
			target->objective_penalty_index = new int* [objectives.size()];

			// ����Լ��
			int address_ptr = 0;
			int parameter_ptr = 0;
			Constrain* constrain_buffer = nullptr;
			for (int cid = 0; cid < constrains.size(); cid++)
			{
				// ����Լ��
				switch (constrains[cid])
				{
				case ConstrainType::user:
					constrain_buffer = new ConstrainUserDefined(penaltys[cid], (void(*)())(addresses_c[address_ptr++]),
						(double(*)(int, double))(addresses_c[address_ptr++]),
						(void(*)(int, double))(addresses_c[address_ptr++]));
					break;
				case ConstrainType::range:
				{
					int vid = -1;
					if (parameters_c[parameter_ptr][0] == EMPTYVALUE)
					{
						vid = _findVid(related_variables_c[cid][0]);
						parameters_c[parameter_ptr][0] = elements[vid]->_note._lowbound;
					}
					if (parameters_c[parameter_ptr][1] == EMPTYVALUE)
					{
						if (vid < 0)
							vid = _findVid(related_variables_c[cid][0]);
						parameters_c[parameter_ptr][1] = elements[vid]->_note._upbound;
					}
					constrain_buffer = new ConstrainRange(penaltys[cid], parameters_c[parameter_ptr][0], parameters_c[parameter_ptr][1]);
					parameter_ptr++;
					break;
				}					
				case ConstrainType::compatibility:
					constrain_buffer = new ConstrainCompatibility(penaltys[cid], parameters_c[parameter_ptr] + 1, parameters_c[parameter_ptr][0]);
					parameter_ptr++;
					break;
				case ConstrainType::capacity:
					constrain_buffer = new ConstrainCapacity(penaltys[cid], parameters_c[parameter_ptr] + 1, parameters_c[parameter_ptr][0],
						parameters_c[parameter_ptr + 1] + 1, parameters_c[parameter_ptr + 1][0]);
					parameter_ptr += 2;
					break;
				case ConstrainType::unique:
				{
					// �жϹ������ĳ���
					int vid;
					int vlength = 0;
					for (int vcounter = 0; vcounter < related_variables_c[cid].size(); vcounter++)
					{
						vid = 0;
						while (vid < elements.size())
						{
							if (related_variables_c[cid][vcounter] == elements[vid]->getName())
							{
								vlength += target->feasible_regions_ini[vid].getLength();
								break;
							}
							vid++;
						}
					}
					if (vlength < 1000)
					{
						int len = 0;
						double* v_buffer = target->feasible_regions_ini[vid].getFeasibleList(len);
						constrain_buffer = new ConstrainUnique(penaltys[cid], v_buffer, target->feasible_regions_ini->getLength());
						delete[] v_buffer;
					}
					else
					{
						constrain_buffer = new ConstrainUniqueLarge(penaltys[cid]);
					}
					break;
				}
				case ConstrainType::distributed:
					constrain_buffer = new ConstrainDistributed(penaltys[cid], parameters_c[parameter_ptr] + 1, parameters_c[parameter_ptr][0],
						parameters_c[parameter_ptr + 1]);
					parameter_ptr += 2;
					break;
				case ConstrainType::mindistance:
				{
					// ���������
					int vid;
					for (int vcounter = 0; vcounter < related_variables_c[cid].size(); vcounter++)
					{
						vid = 0;
						while (vid < elements.size())
						{
							if (related_variables_c[cid][vcounter] == elements[vid]->getName())
							{
								if (parameters_c[parameter_ptr][0]) // ָ�붨��
								{
									constrain_buffer = new ConstrainMinDistance(penaltys[cid],
										target->variables[vid].getLowbound(), target->variables[vid].getUpbound(),
										(double*)(addresses_c[address_ptr++]), target->variables[vid].getLength());
									parameter_ptr++;
								}
								else // ֵ����
								{
									constrain_buffer = new ConstrainMinDistance(penaltys[cid],
										target->variables[vid].getLowbound(), target->variables[vid].getUpbound(),
										parameters_c[parameter_ptr][1], target->variables[vid].getLength());
									parameter_ptr++;
								}
								break;
							}
							vid++;
						}
					}
				}
				break;
				default:
					constrain_buffer = nullptr;
					break;
				}
				// ���Լ��
				target->constrains[cid] = constrain_buffer;

				// ����Լ��
				// ���ұ�����������������
				int vid;
				for (int vcounter = 0; vcounter < related_variables_c[cid].size(); vcounter++)
				{
					vid = 0;
					while (vid < elements.size())
					{
						if (related_variables_c[cid][vcounter] == elements[vid]->getName())
						{
							// ���Լ��
							target->constrain_pairs[vid].constrains.push_back(constrain_buffer);

							target->constrain_variable_index[cid] = vid; // ���ж������������ʱ��ͻ���������������汾��Ҫע��

							// Ӧ����Լ��
							if (constrain_buffer->getConstrainLevel() == constraints_range)
							{
								constrain_buffer->regionReduction(0, target->feasible_regions_ini + vid);
							}
							break;
						}
						vid++;
					}
				}
			}
            // ������������
            target->buildNoReductionTable();

			// ����Ŀ�������������ͷ���
			std::vector<int> penalty_list;
			for (int oid = 0; oid < objectives.size(); oid++)
			{
				penalty_list.clear();

				// �������й���Լ��
				for (int cid = 0; cid < constrains.size(); cid++)
				{
					for (std::vector<std::string>::iterator it = related_objects_c[cid].begin(); it != related_objects_c[cid].end(); it++)
					{
						if (objectives[oid]->getName() == *it)
						{
							penalty_list.push_back(cid);
						}
					}
				}

				target->objective_penalty_number[oid] = penalty_list.size();
				target->objective_penalty_index[oid] = new int[penalty_list.size()];
				for (int i = 0; i < penalty_list.size(); i++)
				{
					target->objective_penalty_index[oid][i] = penalty_list[i];
				}
			}
			
		}

		// ����ʽ����
		void _compileHeuristic(ProblemHandle* target)
		{
			// ����ʽ�ռ��ʼ��
			target->inspirations = new Inspiration * [elements.size()];
			target->inspiration_variable_number = new int[elements.size()];
			target->inspiration_variable_index = new int* [elements.size()];
			target->inspirate_buffer = new double* [elements.size() + 2]; // +2����Ϊ����ʽ��Ĭ������������
			
			// ��������ʽ
			for (int i = 0; i < elements.size(); i++)
			{
				// ֻ�о�������Ҫ����ʽ
				if (!decision_variable[i])
				{
					target->inspirations[i] = nullptr;
					target->inspiration_variable_number[i] = 0;
					target->inspiration_variable_index[i] = nullptr;
					continue;
				}

				// ���ұ���
				bool connected = false;
				for (int j = 0; j < heuristics.size(); j++)
				{
					if (elements[i]->getName() == related_variable_i[j]) // �ҵ���Ӧ����ʽ
					{
						// ��������ʽ�����ռ�
						int v_number = input_variables_i[j].size();
						target->inspiration_variable_number[i] = v_number;
						target->inspiration_variable_index[i] = new int[v_number];
						// ���ұ�����������������
						int vid;
						for (int vcounter = 0; vcounter < v_number; vcounter++)
						{
							vid = 0;
							while (vid < elements.size())
							{
								if (input_variables_i[j][vcounter] == elements[vid]->getName())
								{
									target->inspiration_variable_index[i][vcounter] = vid;
									break;
								}
								vid++;
							}
						}

						// ��������ʽ
						if (stable_heuristic[j])
						{
							// ���ò���
							for (int pid = 0; pid < target->inspiration_variable_number[i]; pid++)
							{
								target->inspirate_buffer[pid] = target->variables[target->inspiration_variable_index[i][pid]].variable.address;
							}

							target->inspirations[i] = static_cast<NormalInspiration*>(heuristics[j])->toStable(
								target->feasible_regions_ini + i, target->variables[i].getLength(), target->inspirate_buffer);
						}
						else
						{
							target->inspirations[i] = heuristics[j]->clone();
						}
						connected = true;

						break; //�������
					}
				}
				if (!connected) // ȱʡ����Ϊ�������ʽ
				{
					target->inspirations[i] = new RandomInspiration();
					target->inspiration_variable_number[i] = 0;
					target->inspiration_variable_index[i] = nullptr;
				}
			}
		}

	public:
		Problem(std::string problem_name)
		{
			name = problem_name;
			compil_check = true;
		}

		~Problem()
		{
			clear();
		}

		void clear()
		{
			for (int i = 0; i < elements.size(); i++)
				delete elements[i];
			elements.clear();
			decision_variable.clear();
			calculate_variable.clear();

			for (int i = 0; i < objectives.size(); i++)
				delete objectives[i];
			objectives.clear();
			for (int i = 0; i < related_variables_o.size(); i++)
				related_variables_o[i].clear();
			related_variables_o.clear();

			constrains.clear();
			addresses_c.clear();
			for (int i = 0; i < parameters_c.size(); i++)
				delete parameters_c[i];
			parameters_c.clear();
			penaltys.clear();
			for (int i = 0; i < related_variables_c.size(); i++)
				related_variables_c[i].clear();
			related_variables_c.clear();
			for (int i = 0; i < related_objects_c.size(); i++)
				related_objects_c[i].clear();
			related_objects_c.clear();
			not_check_constrain = false;

			for (int i = 0; i < heuristics.size(); i++)
				delete heuristics[i];
			heuristics.clear();
			related_variable_i.clear();
			for (int i = 0; i < input_variables_i.size(); i++)
				input_variables_i[i].clear();
			input_variables_i.clear();
			stable_heuristic.clear();
		}

		void addConstant(std::string name, double* values, int length, int height = 1)
		{
			if (!_duplicationCheck(name, "constant"))
			{
				return;
			}

			ECElement* buffer = new ECElement(name, length, height, 0, 0, 0);
			int size = length * height;
			double* v_buffer = new double[size];
			memcpy(v_buffer, values, sizeof(double) * size);
			buffer->variable.setAddress(v_buffer);
			elements.push_back(buffer);
			decision_variable.push_back(false);
			calculate_variable.push_back(false);
		}

		void addVariable(std::string name, double low_bound, double up_bound, double accuracy, int length, int height = 1, VariableType type = VariableType::normal)
		{
			if (!_duplicationCheck(name, "variable"))
			{
				return;
			}

			ECElement* buffer = new ECElement(name, length, height, up_bound, low_bound, accuracy, type);
			elements.push_back(buffer);
			decision_variable.push_back(true);
			calculate_variable.push_back(false);
		}

		void addObjective(std::string name, int priority, bool min_is_better, std::string input_elements, double (*evaluate_func)(double** inputs))
		{
			if (!_duplicationCheck(name, "objective"))
			{
				return;
			}

			Objective* buffer = new Objective(name, priority, min_is_better);

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_o.push_back(vname);//��������õ��ı�����
			
			// ��������
			buffer->setCalculator(new FuncCalculator(evaluate_func, related_variables_o.size()));
			objectives.push_back(buffer);
		}

		void addObjective(std::string name, int priority, bool min_is_better, std::string input_elements, eccalcul_functor* evaluate_func)
		{
			if (!_duplicationCheck(name, "objective"))
			{
				return;
			}

			Objective* buffer = new Objective(name, priority, min_is_better);

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_o.push_back(vname);//��������õ��ı�����

			// ��������
			buffer->setCalculator(new FunctorCalculator(evaluate_func, related_variables_o.size()));
			objectives.push_back(buffer);
		}

		void changeConstrainDeal(bool no_check)
		{
			not_check_constrain = no_check;
		}

		// �ְ汾Լ�������Ƶ�����
		// ������Ӻ����й����������Ϊ��ȷ�������ı����Ƿ����������
		
		// ����û�����Լ��
		void addConstrain(std::string input_elements, void (*model_ini)(void), double (*check_func)(int, double), void (*model_change)(int, double), double penalty_w = 1, std::string related_objects = "")
		{
			penaltys.push_back(penalty_w);
			constrains.push_back(ConstrainType::user);
			addresses_c.push_back(intptr_t(model_ini));
			addresses_c.push_back(intptr_t(check_func));
			addresses_c.push_back(intptr_t(model_change));

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_c.push_back(vname);

			// ����Ŀ����
			std::vector<std::string> oname;
			if (related_objects != "")
				stringSplit(related_objects, ',', oname);
			related_objects_c.push_back(oname);
		}

		//void addConstrain(std::string input_elements, std::string formula);

		// ��ӷ�ΧԼ��
		void addConstrainRange(std::string input_elements, const double _left = EMPTYVALUE, const int _right = EMPTYVALUE, double penalty_w = 1, std::string related_objects = "")
		{
			penaltys.push_back(penalty_w);
			constrains.push_back(ConstrainType::range);
			double* buffer;
			buffer = new double[2];
			buffer[0] = _left;
			buffer[1] = _right;
			parameters_c.push_back(buffer);

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_c.push_back(vname);

			// ����Ŀ����
			std::vector<std::string> oname;
			if (related_objects != "")
				stringSplit(related_objects, ',', oname);
			related_objects_c.push_back(oname);
		}

		// �������Լ��
		void addConstrainCompatibility(std::string input_elements, const double* value, const int length, double penalty_w = 1, std::string related_objects = "")
		{
			penaltys.push_back(penalty_w);
			constrains.push_back(ConstrainType::compatibility);
			double* buffer;
			buffer  = new double[length + 1];
			buffer[0] = length;
			memcpy(buffer + 1, value, length * sizeof(double));
			parameters_c.push_back(buffer);

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_c.push_back(vname);

			// ����Ŀ����
			std::vector<std::string> oname;
			if (related_objects != "")
				stringSplit(related_objects, ',', oname);
			related_objects_c.push_back(oname);
		}

		// ��Ӷ�һԼ��
		void addConstrainUnique(std::string input_elements, double penalty_w = 1, std::string related_objects = "")
		{
			penaltys.push_back(penalty_w);
			constrains.push_back(ConstrainType::unique);

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_c.push_back(vname);

			// ����Ŀ����
			std::vector<std::string> oname;
			if (related_objects != "")
				stringSplit(related_objects, ',', oname);
			related_objects_c.push_back(oname);
		}

		// �����С��Լ��
		void addConstrainMinDistance(std::string input_elements, double gap_width, double penalty_w = 1, std::string related_objects = "")
		{
			penaltys.push_back(penalty_w);
			constrains.push_back(ConstrainType::mindistance);
			double* buffer;
			buffer = new double[2];
			buffer[0] = 0;
			buffer[1] = gap_width;
			parameters_c.push_back(buffer);

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_c.push_back(vname);

			// ����Ŀ����
			std::vector<std::string> oname;
			if (related_objects != "")
				stringSplit(related_objects, ',', oname);
			related_objects_c.push_back(oname);
		}

		// �����С��Լ��
		void addConstrainMinDistance(std::string input_elements, double* gap_width, double penalty_w = 1, std::string related_objects = "")
		{
			penaltys.push_back(penalty_w);
			constrains.push_back(ConstrainType::compatibility);
			double* buffer;
			buffer = new double(1);
			parameters_c.push_back(buffer);
			addresses_c.push_back(intptr_t(gap_width)); // gap_width δ���������ڷ���

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_c.push_back(vname);

			// ����Ŀ����
			std::vector<std::string> oname;
			if (related_objects != "")
				stringSplit(related_objects, ',', oname);
			related_objects_c.push_back(oname);
		}

		// �������Լ��
		void addConstrainCapacity(std::string input_elements, double* capacitys, int container_number, double* volumes, int item_number, double penalty_w = 1, std::string related_objects = "")
		{
			penaltys.push_back(penalty_w);
			constrains.push_back(ConstrainType::capacity);
			double* buffer;
			buffer = new double[container_number + 1];
			buffer[0] = container_number;
			memcpy(buffer + 1, capacitys, container_number * sizeof(double));
			parameters_c.push_back(buffer);
			buffer = new double[item_number + 1];
			buffer[0] = item_number;
			memcpy(buffer + 1, volumes, item_number * sizeof(double));
			parameters_c.push_back(buffer);

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_c.push_back(vname);

			// ����Ŀ����
			std::vector<std::string> oname;
			if (related_objects != "")
				stringSplit(related_objects, ',', oname);
			related_objects_c.push_back(oname);
		}

		// ��ӷֲ�Լ��
		void addConstrainDistributed(std::string input_elements, double* feasible_values, int size, int* appear_numbers, double penalty_w = 1, std::string related_objects = "")
		{
			penaltys.push_back(penalty_w);
			constrains.push_back(ConstrainType::distributed);
			double* buffer;
			buffer = new double[size + 1];
			buffer[0] = size;
			memcpy(buffer + 1, feasible_values, size * sizeof(double));
			parameters_c.push_back(buffer);
			buffer = new double[size];
			for (int i = 0; i < size; i++)
			{
				buffer[i] = appear_numbers[i];
			}
			parameters_c.push_back(buffer);

			// ���������
			std::vector<std::string> vname;
			if (input_elements != "")
				stringSplit(input_elements, ',', vname);
			related_variables_c.push_back(vname);

			// ����Ŀ����
			std::vector<std::string> oname;
			if (related_objects != "")
				stringSplit(related_objects, ',', oname);
			related_objects_c.push_back(oname);
		}

		void addInspirationRandom(std::string variable_name)
		{
			input_variables_i.push_back(std::vector<std::string> ());
			related_variable_i.push_back(variable_name);
			heuristics.push_back(new RandomInspiration());
			stable_heuristic.push_back(false);			
		}

		void addInspirationBoundary(std::string variable_name)
		{
			input_variables_i.push_back(std::vector<std::string>());
			related_variable_i.push_back(variable_name);
			heuristics.push_back(new BoundaryInspiration());
			stable_heuristic.push_back(false);
		}

		void addInspirationFunc(std::string variable_name, std::string input_elements, double (*inspiration_func)(double** inputs), bool stable = false)
		{
			// ���������
			std::vector<std::string> vname;
			if (variable_name != "") // ���������Ϊ��
			{
				stringSplit(input_elements, ',', vname);
			}
			input_variables_i.push_back(vname);
			related_variable_i.push_back(variable_name);
			heuristics.push_back(new NormalInspiration(new FuncCalculator(inspiration_func, int(vname.size()) + 2))); // +2������ʽ�Դ����������� 

			if (stable)
			{
				stable_heuristic.push_back(true);
			}
			else {
				stable_heuristic.push_back(false);
			}

		}

		void addInspirationFunc(std::string variable_name, std::string input_elements, eccalcul_functor* inspiration_func, bool stable = false)
		{
			// ���������
			std::vector<std::string> vname;
			if (variable_name != "") // ���������Ϊ��
			{
				stringSplit(input_elements, ',', vname);
			}
			input_variables_i.push_back(vname);
			related_variable_i.push_back(variable_name);

			FunctorCalculator* functor = new FunctorCalculator(inspiration_func, int(vname.size()) + 2); // +2������ʽ�Դ����������� 
			heuristics.push_back(new NormalInspiration(functor));
			delete functor;

			if (stable)
			{
				stable_heuristic.push_back(true);
			}
			else {
				stable_heuristic.push_back(false);
			}

		}

		// void addInspirationFunc(std::string variable_name, std::string input_elements, std::string formula, bool stable = false);

		void addInspirationMatrix(std::string variable_name, double* matrix, int decision_number, int demenion_number)
		{
			input_variables_i.push_back(std::vector<std::string>());
			related_variable_i.push_back(variable_name);
			heuristics.push_back(new StableInspiration(matrix, decision_number, demenion_number));
			stable_heuristic.push_back(false);
		}	

		ProblemHandle* compile()
		{
			if (!_compilationCheck()) // ������
				return nullptr;

			ProblemHandle* back = new ProblemHandle();

			back->_name = name;

			// ��ģ�����
			_compileVariable(back);
			_compileObjective(back);
			_compileConstrain(back);
			_compileHeuristic(back);

			// ����������Թ���
			back->selfBuild();

			return back;
		}
	};
}
