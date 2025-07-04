#pragma once
#include<cmath>
#include<vector>
#include<string>
#include<fstream>
#include"_rels/metriclib/metriclib.h"
#include"logger.h"
#include"out-txt.h"
#include"out-excel.h"
#include"out-latex.h"

// 指标计算流程
	// 明确需要参与计算的算法、问题、重复实验次数、指标（主指标，统计指标，是否需要统计获胜次数）
	// 读取数据
	// 计算结果
	// 输出表格 
	// 
	// 对于一组数据，其分析统计过程包含了三种数据
	// 原始数据：实验获得的最优解的目标值，每个实验次有一组数据
	// 指标数据：基于主指标计算来的指标数据，例如HV，IGD，某一目标的最优值等，每个实验次有一个数据
	// 统计数据：基于指标数据获得的统计结果，例如均值，方差，非参p值等，每个优化器每问题有一个数据
	// 
	// 对于一次统计来说，主指标应有一个，统计指标可以有多个
	// 
	// 同时应当标注是否需要加粗，是否需要统计胜负
	// 
	// 结果文件格式标准
	// 首行<文件格式（ver=）,解数目，目标数目>
	//

namespace ECFC
{
	class ECAnalyzer
	{
	public:
        enum class MetricType { mean, variance, std, median, mode, smallest, largest, hyper_volume, GD, IGD, peark_r, end };

	private:
		struct PartiInfo
		{
			std::string optimizer;
			std::string tag;
			int repeats;

			PartiInfo(std::string optimizer, std::string tag, int repeats)
			{
				this->optimizer = optimizer;
				this->tag = tag;
				this->repeats = repeats;
			}
		};

		MetricType _metric_type; // 需要计算的指标
		std::vector<double*> _metric_paras; // 计算指标需要的参数，每个问题的参数相对独立 

		std::vector<MetricType> _analysis;
		bool _significance_test;
		MetricType _metric_significance;
		bool _average_rank_statistic;
		MetricType _metric_rank;
		bool _min_is_better_rank;
		bool _win_lose_statistic;
		MetricType _metric_wl;
		bool _min_is_better_wl;
		bool _best_statistic;
		MetricType _metric_best;
		bool _min_is_better_best;

		std::vector<std::string> _problems;
		std::vector<PartiInfo> _infos;

		std::vector<double*>** _data_buffer; //算法、每次实验的结果[问题][算法][实验次][数据项]; 数据项第一位为解数目，第二位为目标数目
        bool position_loaded;
        bool process_loaded;

		double*** _result_buffer; // 指标计算的结果[问题][算法][实验次]
		double*** _analysis_buffer; // 指标统计的结果[问题][算法][指标项]
		int _data_buffer_size[2]; // 实验次长度由vector长度标明，数据项的size由其前两维的数据标明
		int _result_buffer_size[2];
		int _analysis_buffer_size[2];

		// 清除载入数据内容
		void clearData()
		{
			for (int i1 = 0; i1 < _data_buffer_size[0]; i1++)
			{
				for (int i2 = 0; i2 < _data_buffer_size[1]; i2++)
				{
					for (size_t i3 = 0; i3 < _data_buffer[i1][i2].size(); i3++)
					{
						delete[] _data_buffer[i1][i2][i3];
					}
					_data_buffer[i1][i2].clear();
				}
				delete[] _data_buffer[i1];
			}
			delete[] _data_buffer;
			
			for (int i = 0; i < 2; i++)
			{
				_data_buffer_size[i] = 0;
			}
			_data_buffer = nullptr;
		}

		// 清除结果内存
		void clearResult()
		{
			for (int i1 = 0; i1 < _result_buffer_size[0]; i1++)
			{
				for (int i2 = 0; i2 < _result_buffer_size[1]; i2++)
				{
					delete[] _result_buffer[i1][i2];
				}
				delete[] _result_buffer[i1];
			}
			delete[] _result_buffer;

			for (int i = 0; i < 2; i++)
			{
				_result_buffer_size[i] = 0;
			}
			_result_buffer = nullptr;

			for (int i1 = 0; i1 < _analysis_buffer_size[0]; i1++)
			{
				for (int i2 = 0; i2 < _analysis_buffer_size[1]; i2++)
				{
					delete[] _analysis_buffer[i1][i2];
				}
				delete[] _analysis_buffer[i1];
			}
			delete[] _analysis_buffer;

			for (int i = 0; i < 2; i++)
			{
				_analysis_buffer_size[i] = 0;
			}
			_analysis_buffer = nullptr;
		}

		std::string buildPath(std::string problem, std::string optimizer, std::string tag, int repeat_id)
		{
			return "_log/" + optimizer + "/" + problem + "/" + optimizer + "_" + problem + "(" + tag + ")_" + std::to_string(repeat_id) + ".rslt";
		}

		bool load(std::vector<double*> &buffer, const PartiInfo& info, const std::string problem)
		{
			std::string path;

			std::ifstream data_file;
			std::string read_buffer;

			int object_number;
			int solution_number;
			bool include_solution;
			std::string version_tag;
			double* datas;

			for (int i = 0; i < info.repeats; i++)
			{
				path = buildPath(problem, info.optimizer, info.tag, i);
				data_file.open(path);

				if (data_file.is_open())
				{
					// 头部信息处理
					data_file >> read_buffer >> version_tag >> read_buffer; // 版本
					if (version_tag == "1.0")
					{
						include_solution = false;
					}
					else if (version_tag == "1.1")
					{
						include_solution = true;
					}
					std::getline(data_file, read_buffer);
					std::getline(data_file, read_buffer); // 运行开销
					std::getline(data_file, read_buffer); // 分割线

					// 结果信息处理
					data_file >> read_buffer >> object_number >> read_buffer >> solution_number;

					datas = new double[2 + object_number * solution_number + 1];
					datas[0] = solution_number; 
					datas[1] = object_number;

					// 结果数据读取
					if (include_solution)
					{
						int data_index = 2;
						for (int i = 0; i < solution_number; i++)
						{
							data_file >> read_buffer; // 结果序号
							for (int j = 0; j < object_number; j++)
							{
								data_file >> read_buffer >> datas[data_index]; // 目标名称，目标值
								data_index++;

								// 变量数据过滤
								while (true)
								{
									data_file >> read_buffer;
									if (read_buffer == "EOS") // 结束
										break;
									std::getline(data_file, read_buffer); // 变量数据
								}
							}
						}
					}
					else
					{
						int data_index = 2;
						for (int i = 0; i < solution_number; i++)
						{
							data_file >> read_buffer; // 结果序号
							for (int j = 0; j < object_number; j++)
							{
								data_file >> read_buffer >> datas[data_index]; // 目标名称，目标值
								data_index++;
							}
						}
					}

					buffer.push_back(datas);
					data_file.close();
				}
				else
				{
					sys_logger.error("The result data is missing or damaged, when processing " + path);
					return false;
				}
			}

			return true;
		}

		bool loadData()
		{
			clearData();

			// 构建空间
			_data_buffer = new std::vector<double*>*[_problems.size()];
			for (size_t i = 0; i < _problems.size(); i++)
			{
				_data_buffer[i] = new std::vector<double*>[_infos.size()];
			}
			_data_buffer_size[0] = _problems.size();
			_data_buffer_size[1] = _infos.size();

			// 读取数据
			bool state;
			for (size_t i = 0; i < _problems.size(); i++)
			{
				for (size_t j = 0; j < _infos.size(); j++)
				{
					state = load(_data_buffer[i][j], _infos[j], _problems[i]);
					
					if (!state)
						return false;
				}
			}
			
			return true;
		}

		std::string getMetricName(MetricType metric_type)
		{
			switch (metric_type)
			{
			case ECFC::ECAnalyzer::MetricType::mean:
				return "mean";
			case ECFC::ECAnalyzer::MetricType::variance:
				return "variance";
			case ECFC::ECAnalyzer::MetricType::std:
				return "std";
			case ECFC::ECAnalyzer::MetricType::median:
				return "median";
			case ECFC::ECAnalyzer::MetricType::mode:
				return "mode";
			case ECFC::ECAnalyzer::MetricType::smallest:
				return "smallest";
			case ECFC::ECAnalyzer::MetricType::largest:
				return "largest";
			case ECFC::ECAnalyzer::MetricType::hyper_volume:
				return "HV";
			case ECFC::ECAnalyzer::MetricType::GD:
				return "GD";
			case ECFC::ECAnalyzer::MetricType::IGD:
				return "IGD";
            case ECFC::ECAnalyzer::MetricType::peark_r:
                return "peak_ratio";
			case ECFC::ECAnalyzer::MetricType::end:
			default:
				return "";
			}
		}

		std::string _number2string(double input)
		{
			std::stringstream ss;

			if (std::abs(input) >= 1000.0 || std::abs(input) <= 0.0001) {
				// 使用科学计数法，保留最多 4 位有效数字
				ss << std::scientific << std::setprecision(4) << input;
				return ss.str();
			}
			else {
				// 整数部分不大于 1000
				int integerPart = static_cast<int>(std::abs(input));
				// 计算整数位数
				int int_size;
				if (input == 0)
					int_size = 1;
				else
					int_size = static_cast<int>(std::log10(integerPart)) + 1;
				// 调整小数位数，总有效数字不超过 5 位
				ss << std::fixed << std::setprecision(5 - int_size) << input;
				return ss.str();
			}
		}

		void _significant_statistic()
		{
			int _s_id = _analysis.size();
			int problem_number = _problems.size();
			int optimizer_number = _infos.size();

			for (int p = 0; p < problem_number; p++)
			{
				for (int o = 1; o < optimizer_number; o++)
				{
					_analysis_buffer[p][o][_s_id] = MetricLib::MannWhitneyUTest(_result_buffer[p][0], _infos[0].repeats, _result_buffer[p][o], _infos[o].repeats);
				}
			}
		}

		void _wl_statistic()
		{
			// 记录每种算法在每个问题中排名的指标索引
			int _wl_id = _analysis.size() + 1;

			// 计算需要统计的指标
			int competition_id = 0;
			for (size_t i = 0; i < _analysis.size(); i++)
			{
				if (_analysis[i] == _metric_wl)
				{
					competition_id = i;
					break;
				}
			}

			// 数据统计
			bool is_larger;
			if (_significance_test)
			{
				int win, lose, eq;

				for (size_t o = 1; o < _infos.size(); o++)
				{
					win = 0;
					lose = 0;
					eq = 0;

					for (size_t p = 0; p < _problems.size(); p++)
					{
						if (std::abs(_analysis_buffer[p][o][_analysis.size()]) > 5e-4)
						{
							eq++;
						}
						else
						{
							is_larger = _analysis_buffer[p][0][competition_id] > _analysis_buffer[p][o][competition_id];
							if (_min_is_better_wl ^ is_larger)
								win++;
							else
								lose++;
						}
					}

					_analysis_buffer[0][o][_wl_id] = win;
					_analysis_buffer[0][o][_wl_id + 1] = lose;
					_analysis_buffer[0][o][_wl_id + 2] = eq;
				}
			}
			else
			{
				int win, lose;

				for (size_t o = 1; o < _infos.size(); o++)
				{
					win = 0;
					lose = 0;

					for (size_t p = 0; p < _problems.size(); p++)
					{
						is_larger = _analysis_buffer[p][0][competition_id] > _analysis_buffer[p][o][competition_id];
						if (_min_is_better_wl ^ is_larger)
							win++;
						else
							lose++;
					}

					_analysis_buffer[0][o][_wl_id] = win;
					_analysis_buffer[0][o][_wl_id + 1] = lose;
				}
			}
		}

		void _best_solution_statistic()
		{
			// 记录每种算法在每个问题中排名的指标索引
			int _wl_id = _analysis.size() + 4;

			// 计算需要统计的指标
			int competition_id = 0;
			for (int i = 0; i < _analysis.size(); i++)
			{
				if (_analysis[i] == _metric_best)
				{
					competition_id = i;
					break;
				}
			}

			int best_result_id;
			double best_fitness;
			bool is_larger;
			for (size_t p = 0; p < _problems.size(); p++)
			{
				best_result_id = 0;
				best_fitness = _analysis_buffer[p][0][competition_id];
				for (size_t o = 1; o < _infos.size(); o++)
				{
					is_larger = best_fitness > _analysis_buffer[p][o][competition_id];
					if (!(_min_is_better_wl ^ is_larger))
					{
						best_result_id = o;
						best_fitness = _analysis_buffer[p][o][competition_id];
					}
				}
				
				for (size_t o = 0; o < _infos.size(); o++)
				{
					if (o == best_result_id)
						_analysis_buffer[p][o][_wl_id] = 1;
					else
						_analysis_buffer[p][o][_wl_id] = 0;
				}
			}
		}

		void _rank_statistic()
		{
			// 记录每种算法在每个问题中排名的指标索引
			int ranked_id = _analysis.size() + 5;
			int average_rank_id = _analysis.size() + 6;

			// 计算需要统计的指标
			int competition_id = 0;
			for (size_t i = 0; i < _analysis.size(); i++)
			{
				if (_analysis[i] == _metric_rank)
				{
					competition_id = i;
					break;
				}
			}

            sortHelper<int, double>* sortbuffer = new sortHelper<int, double>[_infos.size() + 1];
			int factor = (_min_is_better_rank ? 1 : -1); // 归约到最小化的系数
			for (size_t p = 0; p < _problems.size(); p++)
			{
				for (size_t o = 0; o < _infos.size(); o++)
				{
					sortbuffer[o].id = o;
					sortbuffer[o].value = _analysis_buffer[p][o][competition_id] * factor;
				}

				std::sort(sortbuffer, sortbuffer + _infos.size());

				// 记录排名
				for (size_t o = 0; o < _infos.size(); o++)
				{
					_analysis_buffer[p][sortbuffer[o].id][ranked_id] = o + 1;
				}
			}
			delete[] sortbuffer;

			// 计算平均排名
			double total_rank;
			for (size_t o = 0; o < _infos.size(); o++)
			{
				total_rank = 0;
				for (int p = 0; p < _problems.size(); p++)
					total_rank += _analysis_buffer[p][o][ranked_id];
				_analysis_buffer[0][o][average_rank_id] = total_rank / _problems.size();
			}
		}

		void _metric_cal()
		{
			int solution_number;
			int object_number;
			double* calculation_buffer;
			double* datas_pointer;

			int problem_number = _problems.size();
			int optimizer_number = _infos.size();

			// 指标内存申请
			_result_buffer = new double** [problem_number];
			for (int p = 0; p < problem_number; p++)
			{
				_result_buffer[p] = new double* [optimizer_number];
				for (int o = 0; o < optimizer_number; o++)
					_result_buffer[p][o] = new double[_infos[o].repeats + 1];
			}

			// 查找要计算的指标并执行
			switch (_metric_type)
			{
			case ECFC::ECAnalyzer::MetricType::mean:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							calculation_buffer = new double[solution_number];
							for (int i = 0; i < solution_number; i++)
							{
								calculation_buffer[i] = datas_pointer[int(i * object_number + _metric_paras[p][0])];
							}
							_result_buffer[p][o][re] = MetricLib::mean(calculation_buffer, object_number);
							delete[] calculation_buffer;
						}
					}
				}
				break;
			case ECFC::ECAnalyzer::MetricType::variance:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							calculation_buffer = new double[solution_number];
							for (int i = 0; i < solution_number; i++)
							{
								calculation_buffer[i] = datas_pointer[int(2 + i * object_number + _metric_paras[p][0])];
							}
							_result_buffer[p][o][re] = MetricLib::variance(calculation_buffer, object_number);
							delete[] calculation_buffer;
						}
					}
				}
				break;
			case ECFC::ECAnalyzer::MetricType::std:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							calculation_buffer = new double[solution_number];
							for (int i = 0; i < solution_number; i++)
							{
								calculation_buffer[i] = datas_pointer[int(2 + i * object_number + _metric_paras[p][0])];
							}
							_result_buffer[p][o][re] = MetricLib::standardDeviation(calculation_buffer, object_number);
							delete[] calculation_buffer;
						}
					}
				}
				break;
			case ECFC::ECAnalyzer::MetricType::median:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							calculation_buffer = new double[solution_number];
							for (int i = 0; i < solution_number; i++)
							{
								calculation_buffer[i] = datas_pointer[int(2 + i * object_number + _metric_paras[p][0])];
							}
							_result_buffer[p][o][re] = MetricLib::median(calculation_buffer, object_number);
							delete[] calculation_buffer;
						}
					}
				}
				break;
			case ECFC::ECAnalyzer::MetricType::mode:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							calculation_buffer = new double[solution_number];
							for (int i = 0; i < solution_number; i++)
							{
								calculation_buffer[i] = datas_pointer[int(2 + i * object_number + _metric_paras[p][0])];
							}
							_result_buffer[p][o][re] = MetricLib::mode(calculation_buffer, object_number);
							delete[] calculation_buffer;
						}
					}
				}
				break;
			case ECFC::ECAnalyzer::MetricType::smallest:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							calculation_buffer = new double[solution_number];
							for (int i = 0; i < solution_number; i++)
							{
								calculation_buffer[i] = datas_pointer[int(2 + i * object_number + _metric_paras[p][0])];
							}
							_result_buffer[p][o][re] = MetricLib::smallest(calculation_buffer, object_number);
							delete[] calculation_buffer;
						}
					}
				}
				break;
			case ECFC::ECAnalyzer::MetricType::largest:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							calculation_buffer = new double[solution_number];
							for (int i = 0; i < solution_number; i++)
							{
								calculation_buffer[i] = datas_pointer[int(2 + i * object_number + _metric_paras[p][0])];
							}
							_result_buffer[p][o][re] = MetricLib::largest(calculation_buffer, object_number);
							delete[] calculation_buffer;
						}
					}
				}
				break;
			case ECFC::ECAnalyzer::MetricType::hyper_volume:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							if (equal(_metric_paras[p][0], 0))
								_result_buffer[p][o][re] = MetricLib::hv_montecarlo(datas_pointer, _metric_paras[p], solution_number, object_number);
							else
								_result_buffer[p][o][re] = MetricLib::hv_math(datas_pointer, _metric_paras[p], solution_number, object_number);
						}
					}
				}
				break;
			case ECFC::ECAnalyzer::MetricType::GD:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							_result_buffer[p][o][re] = MetricLib::gd(datas_pointer, _metric_paras[p] + 1, _metric_paras[p][0], solution_number, object_number);
						}
					}
				}
				break;
			case ECFC::ECAnalyzer::MetricType::IGD:
				for (int p = 0; p < problem_number; p++)
				{
					for (int o = 0; o < optimizer_number; o++)
					{
						for (size_t re = 0; re < _data_buffer[p][o].size(); re++)
						{
							solution_number = _data_buffer[p][o][re][0];
							object_number = _data_buffer[p][o][re][1];
							datas_pointer = _data_buffer[p][o][re] + 2;
							_result_buffer[p][o][re] = MetricLib::igd(datas_pointer, _metric_paras[p] + 1, _metric_paras[p][0], solution_number, object_number);
						}
					}
				}
				break;
            case ECFC::ECAnalyzer::MetricType::peark_r:
                break;
			case ECFC::ECAnalyzer::MetricType::end:
			default:
				break;
			}
		}

		void _metric_analysis()
		{
			int problem_number = _problems.size();
			int optimizer_number = _infos.size();

			// 统计内存申请
			_analysis_buffer = new double** [problem_number];
			for (int p = 0; p < problem_number; p++)
			{
				_analysis_buffer[p] = new double* [optimizer_number];
				for (int o = 0; o < optimizer_number; o++)
					_analysis_buffer[p][o] = new double[_analysis.size() + 15];
			}

			// 统计分析数据计算
			for (size_t metricid = 0; metricid < _analysis.size(); metricid++)
			{
				// 判断指标
				switch (_analysis[metricid])
				{
				case ECFC::ECAnalyzer::MetricType::mean:
					for (int p = 0; p < problem_number; p++)
					{
						for (int o = 0; o < optimizer_number; o++)
						{
							_analysis_buffer[p][o][metricid] = MetricLib::mean(_result_buffer[p][o], _data_buffer[p][o].size());
						}
					}
					break;
				case ECFC::ECAnalyzer::MetricType::variance:
					for (int p = 0; p < problem_number; p++)
					{
						for (int o = 0; o < optimizer_number; o++)
						{
							_analysis_buffer[p][o][metricid] = MetricLib::variance(_result_buffer[p][o], _data_buffer[p][o].size());
						}
					}
					break;
				case ECFC::ECAnalyzer::MetricType::std:
					for (int p = 0; p < problem_number; p++)
					{
						for (int o = 0; o < optimizer_number; o++)
						{
							_analysis_buffer[p][o][metricid] = MetricLib::standardDeviation(_result_buffer[p][o], _data_buffer[p][o].size());
						}
					}
					break;
				case ECFC::ECAnalyzer::MetricType::median:
					for (int p = 0; p < problem_number; p++)
					{
						for (int o = 0; o < optimizer_number; o++)
						{
							_analysis_buffer[p][o][metricid] = MetricLib::median(_result_buffer[p][o], _data_buffer[p][o].size());
						}
					}
					break;
				case ECFC::ECAnalyzer::MetricType::mode:
					for (int p = 0; p < problem_number; p++)
					{
						for (int o = 0; o < optimizer_number; o++)
						{
							_analysis_buffer[p][o][metricid] = MetricLib::mode(_result_buffer[p][o], _data_buffer[p][o].size());
						}
					}
					break;
				case ECFC::ECAnalyzer::MetricType::smallest:
					for (int p = 0; p < problem_number; p++)
					{
						for (int o = 0; o < optimizer_number; o++)
						{
							_analysis_buffer[p][o][metricid] = MetricLib::smallest(_result_buffer[p][o], _data_buffer[p][o].size());
						}
					}
					break;
				case ECFC::ECAnalyzer::MetricType::largest:
					for (int p = 0; p < problem_number; p++)
					{
						for (int o = 0; o < optimizer_number; o++)
						{
							_analysis_buffer[p][o][metricid] = MetricLib::largest(_result_buffer[p][o], _data_buffer[p][o].size());
						}
					}
					break;
				case ECFC::ECAnalyzer::MetricType::hyper_volume:
				case ECFC::ECAnalyzer::MetricType::GD:
				case ECFC::ECAnalyzer::MetricType::IGD:
                case ECFC::ECAnalyzer::MetricType::peark_r:
				case ECFC::ECAnalyzer::MetricType::end:
				default:
					break;
				}
			}
		}

		void _analysis_table(std::vector<std::string> &header, std::vector<std::string> &subheader, std::vector<std::string> &rows, std::vector<std::vector<std::string>> &datastrings, std::vector<std::vector<int>> &bolds)
		{
			header.clear();
			for (size_t i = 0; i < _infos.size(); i++)
			{
				header.push_back(_infos[i].optimizer);
			}

			subheader.clear();
			for (size_t i = 0; i < _analysis.size(); i++)
			{
				subheader.push_back(getMetricName(_analysis[i]));
			}
			if (_significance_test)
				subheader.push_back("p-value");

			rows.clear();
			for (size_t i = 0; i < _problems.size(); i++)
				rows.push_back(_problems[i]);

			datastrings.clear();
			datastrings.resize(_problems.size());
			for (size_t p = 0; p < _problems.size(); p++)
			{
				for (size_t o = 0; o < _infos.size(); o++)
				{
					for (size_t m = 0; m < _analysis.size(); m++)
					{
						datastrings[p].push_back(_number2string(_analysis_buffer[p][o][m]));
					}
					if (_significance_test)
					{
						if (o == 0)
						{
							datastrings[p].push_back("-");
						}
						else
						{
							datastrings[p].push_back(_number2string(_analysis_buffer[p][o][_analysis.size()]));
						}
					}
				}
			}

			if (_average_rank_statistic)
			{
				datastrings.push_back(std::vector<std::string>());
				int index = datastrings.size() - 1;

				rows.push_back("Rank");
				int ar_id = _analysis.size() + 6;
				for (int o = 0; o < _infos.size(); o++)
				{
					datastrings[index].push_back(std::to_string(_analysis_buffer[0][o][ar_id]));
				}
			}
			if (_win_lose_statistic)
			{
				datastrings.push_back(std::vector<std::string>());
				int index = datastrings.size() - 1;
				datastrings[index].push_back("-");

				int wl_id = _analysis.size() + 1;
				if (_significance_test)
				{
					rows.push_back("+/=/-");

					for (size_t o = 1; o < _infos.size(); o++)
					{
						std::stringstream ss;
						ss << int(_analysis_buffer[0][o][wl_id]) << "/" << int(_analysis_buffer[0][o][wl_id + 2]) << "/" << int(_analysis_buffer[0][o][wl_id + 1]);

						datastrings[index].push_back(ss.str());
					}
				}
				else
				{
					rows.push_back("+/-");

					for (size_t o = 1; o < _infos.size(); o++)
					{
						std::stringstream ss;
						ss << int(_analysis_buffer[0][o][wl_id]) << "/" << int(_analysis_buffer[0][o][wl_id + 1]);

						datastrings[index].push_back(ss.str());
					}
				}
			}
			if (_best_statistic)
			{
				bolds.clear();
				bolds.resize(_problems.size());

				int competition_id = 0;
				for (int i = 0; i < _analysis.size(); i++)
				{
					if (_analysis[i] == _metric_best)
					{
						competition_id = i;
						break;
					}
				}

				int be_id = _analysis.size() + 4;
				for (size_t p = 0; p < _problems.size(); p++)
				{
					for (size_t o = 0; o < _infos.size(); o++)
					{
						if (_analysis_buffer[p][o][be_id])
						{
							bolds[p].push_back(o * _analysis.size() + competition_id);
						}
					}
				}
			}	
			
		}

	public:
		ECAnalyzer()
		{
			_metric_type = MetricType::end;

			_significance_test = false;
			_metric_significance = MetricType::end;
			_average_rank_statistic = false;
			_metric_rank = MetricType::end;
			_min_is_better_rank = true;
			_win_lose_statistic = false;
			_metric_wl = MetricType::end;
			_min_is_better_wl = true;
			_best_statistic = false;
			_metric_best = MetricType::end;
			_min_is_better_best = true;

			_data_buffer = nullptr;
			_result_buffer = nullptr;
			_analysis_buffer = nullptr;
			_data_buffer_size[0] = 0;
			_data_buffer_size[1] = 0;
			_result_buffer_size[0] = 0;
			_result_buffer_size[1] = 0;
			_analysis_buffer_size[0] = 0;
			_analysis_buffer_size[1] = 0;
		}

		~ECAnalyzer()
		{
			clearAll();
		}

		void addOptimizer(std::string optimzer, std::string tag, int repeats)
		{
			_infos.push_back(PartiInfo(optimzer, tag, repeats));
		}

		void addProblem(std::string problem_name)
		{
			_problems.push_back(problem_name);
		}

		void addProblem(std::string problem_names[], int size)
		{
			for (int i = 0; i < size; i++)
			{
				_problems.push_back(problem_names[i]);
			}
		}

		void setMetric(MetricType metrics, double** paras = nullptr)
		{
			for (size_t i = 0; i < _metric_paras.size(); i++)
			{
				delete[] _metric_paras[i];
			}
			_metric_paras.clear();

			_metric_type = metrics;

			if (paras == nullptr)
			{
				for (size_t i = 0; i < _problems.size(); i++)
					_metric_paras.push_back(nullptr);

				return;
			}	

			double* buffer;
			int size;
			for (size_t i = 0; i < _problems.size(); i++)
			{
				size = int(paras[i][0]);
				buffer = new double[size];
				memcpy(buffer, paras[i] + 1, size * sizeof(double));
				_metric_paras.push_back(buffer);
			}
		}

		void addStatisticalTool(MetricType metrics)
		{
			switch (metrics)
			{
			case ECFC::ECAnalyzer::MetricType::mean:
			case ECFC::ECAnalyzer::MetricType::variance:
			case ECFC::ECAnalyzer::MetricType::std:
			case ECFC::ECAnalyzer::MetricType::median:
			case ECFC::ECAnalyzer::MetricType::mode:
			case ECFC::ECAnalyzer::MetricType::smallest:
			case ECFC::ECAnalyzer::MetricType::largest:
				_analysis.push_back(metrics);
				break;
			case ECFC::ECAnalyzer::MetricType::hyper_volume:
			case ECFC::ECAnalyzer::MetricType::GD:
			case ECFC::ECAnalyzer::MetricType::IGD:
            case ECFC::ECAnalyzer::MetricType::peark_r:
			case ECFC::ECAnalyzer::MetricType::end:
			default:
				break;
			}
		}

		void addSignificanceTest(MetricType metric = MetricType::end)
		{
			_significance_test = true;
			_metric_significance = metric;
		}

		void addWinLoseStatistic(bool min_is_better = true, MetricType metric = MetricType::end)
		{
			_win_lose_statistic = true;
			_min_is_better_wl = min_is_better;
			_metric_wl = metric;
		}

		void addRankStatistic(bool min_is_better = true, MetricType metric = MetricType::end)
		{
			_average_rank_statistic = true;
			_min_is_better_rank = min_is_better;
			_metric_rank = metric;
		}

		void addBestStatistic(bool min_is_better = true, MetricType metric = MetricType::end)
		{
			_best_statistic = true;
			_min_is_better_best = min_is_better;
			_metric_best = metric;
		}

		void clearMetirc()
		{
			_metric_type = MetricType::end;
			for (size_t i = 0; i < _metric_paras.size(); i++)
			{
				delete[] _metric_paras[i];
			}
			_metric_paras.clear();
			_analysis.clear();
			_metric_significance = MetricType::end;
			_average_rank_statistic = false;
			_metric_rank = MetricType::end;
			_min_is_better_rank = true;
			_win_lose_statistic = false;
			_metric_wl = MetricType::end;
			_min_is_better_wl = true;
			_best_statistic = false;
			_metric_best = MetricType::end;
			_min_is_better_best = true;
			clearResult();
		}

		void clearAll()
		{
			_problems.clear();
			_infos.clear();
			clearData();
			clearMetirc();
		}

		void cal()
		{
			// 载入数据
			loadData();

			// 指标计算
			_metric_cal();
			_metric_analysis();
			
			if (_significance_test)
				_significant_statistic();
			if (_average_rank_statistic)
				_rank_statistic();
			if (_win_lose_statistic)
				_wl_statistic();
			if(_best_statistic)
				_best_solution_statistic();
		}

		void printToTxt(std::string path = "")
		{
			if (path == "")
			{
				path = "_analysis/" + getMetricName(_metric_type) + "(" + std::to_string(time(NULL)) + ").txt";
			}

			if ((path.find('/') == std::string::npos) && (path.find('\\') == std::string::npos))
			{
				path = "_analysis/" + path + ".txt";
			}

			std::vector<std::string> header;
			std::vector<std::string> subheader;
			std::vector<std::string> rows;
			std::vector<std::vector<std::string>> datastrings;
			std::vector<std::vector<int>> bolds;
			_analysis_table(header, subheader, rows, datastrings, bolds);
			
			FileOut::writeTableToTxt(header, subheader, rows, datastrings, path);
		}

		void printToCsv(std::string path = "")
		{
			if (path == "")
			{
				path = "_analysis/" + getMetricName(_metric_type) + "(" + std::to_string(time(NULL)) + ").csv";
			}

			if ((path.find('/') == std::string::npos) && (path.find('\\') == std::string::npos))
			{
				path = "_analysis/" + path + ".csv";
			}

			std::vector<std::string> header;
			std::vector<std::string> subheader;
			std::vector<std::string> rows;
			std::vector<std::vector<std::string>> datastrings;
			std::vector<std::vector<int>> bolds;
			_analysis_table(header, subheader, rows, datastrings, bolds);

			FileOut::writeTableToCSV(header, subheader, rows, datastrings, path);
		}

		void printToLatex(std::string path = "", bool three_line_formate = true)
		{
			if (path == "")
			{
				path = "_analysis/" + getMetricName(_metric_type) + "(" + std::to_string(time(NULL)) + ").tex";
			}

			if ((path.find('/') == std::string::npos) && (path.find('\\') == std::string::npos))
			{
				path = "_analysis/" + path + ".tex";
			}

			std::vector<std::string> header;
			std::vector<std::string> subheader;
			std::vector<std::string> rows;
			std::vector<std::vector<std::string>> datastrings;
			std::vector<std::vector<int>> bolds;
			_analysis_table(header, subheader, rows, datastrings, bolds);

			FileOut::writeTableToLaTeX(header, subheader, rows, datastrings, path, bolds, three_line_formate);
		}

		void printToPng(std::string path = "", bool three_line_formate = true)
		{

		}

		void printToWord(std::string path = "", MetricType best_bold = MetricType::end, MetricType key_competition = MetricType::end)
		{

		}

		void printToExcel(std::string path = "", MetricType best_bold = MetricType::end, MetricType key_competition = MetricType::end)
		{

		}
	};

}