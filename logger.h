//------------------------Description------------------------
// This file defines the logger of ECFC, which provides the log
// recording and output for system and each optimizer.
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
#include<iomanip>
#include<fstream>
#include"basicfunc.h"
#include"predefine.h"


// ��־�ļ�����֯������
// һ���ļ��У��Ż�������������log��err�ļ���
// �����ļ��У���������������rslt��prcs�ļ���
// 
// ��ͬ��������Ĵ������������ϸ�̶ȼ��������
// ���������֯��logger����
// ��ϸ�̶���logger�ṩ���������������п���
// 
// ��־�����ʽ
// ʱ�䣨���嵽�룩���������ݣ�����λ��(�Ⱥ������)
//


namespace ECFC {
	class Logger
	{
	private:
		std::string _optimzer_name;
		std::string _problem_name;
		
		std::ofstream _exe_file;     // ������־�ļ������������־���м�¼
		std::ofstream _err_file;     // ������־�ļ���ֻ��¼warning��error����ļ�¼
		std::ofstream _result_file;  // �����־�ļ��������Ż���������
		std::ofstream _process_file; // ������־�ļ��������Ż����̵ļ�¼
		
		std::string _tag; // ����ʱ��ʱ���or�û�ָ����ǩ

		bool _full_result = false; // ����ļ��Ƿ��ӡȫ��������
		bool _process_log = false; // �Ƿ��ӡ�Ż�����
		bool _full_process = false; // �����ļ��Ƿ��ӡȫ��������
		bool _swarm_log = false; // �����ļ��Ƿ��ӡ��Ⱥȫ������

		// ���·���ļ��������
		void _directoryDetection() const
		{
			if (_optimzer_name == "system")
				return;

			std::string log_path = "_log/" + _optimzer_name;
			// һ��Ŀ¼���
			dir_create(log_path);

			// ����Ŀ¼���
			if (_problem_name != "")
			{
				log_path += "/" + _problem_name;
				dir_create(log_path);
			}
		}

		// �����־��·��������
		std::string _get_first_prefix() const
		{
			if (_optimzer_name == "system")
			{
				return "_log/" + _optimzer_name + "(" + _tag + ")";
			}

			return "_log/" + _optimzer_name + "/" 
				+ _optimzer_name + "(" + _tag + ")";
		}

		std::string _get_second_prefix() const
		{
			return "_log/" + _optimzer_name + "/" + _problem_name + "/" 
				+ _optimzer_name + "_" + _problem_name + "(" + _tag + ")";
		}

	public:
		Logger(std::string name, std::string tag, bool full_print = false, bool process_print = false, bool full_process_print = false)
		{
			_optimzer_name = name;
			_problem_name = "";
			_tag = tag;
			_exe_file = std::ofstream(_get_first_prefix() + ".log");
			_err_file = std::ofstream(_get_first_prefix() + ".err");
			
			_full_result = full_print;
			_process_log = process_print;
			_full_process = full_process_print;

			_directoryDetection();
		}

		~Logger()
		{
			_exe_file.close();
			_err_file.close();
			_result_file.close();
			_process_file.close();
		}

		void setProblem(std::string problem_name)
		{
			_problem_name = problem_name;
			_result_file.close();
			if (_process_log)
			{
				_process_file.close();
			}
			
			_directoryDetection();
		}

		void newOptimization(int exe_counter)
		{
			_result_file.close();
			_result_file = std::ofstream(_get_second_prefix() + "_" + std::to_string(exe_counter) + ".rslt");
			
			if (_process_log)
			{
				_process_file.close();
				_process_file = std::ofstream(_get_second_prefix() + "_" + std::to_string(exe_counter) + ".prcs");
			}
		}

		bool full_result() const
		{
			return _full_result;
		}

		bool swarm_print() const
		{
			return _swarm_log;
		}

		bool process_print() const
		{
			return _process_log;
		}

		bool full_process_print() const
		{
			return _full_process;
		}

		void logprocess(std::string message)
		{
			_process_file << message << std::endl;
		}

		void logresult(std::string message)
		{
			_result_file << message << std::endl;
		}

		void info(std::string message)
		{
			_exe_file << std::left << std::setw(24) << formatTime() << std::setw(12) << "[INFO]" << message << std::endl;
		}

		void debug(std::string message)
		{
			_err_file << std::left << std::setw(24) << formatTime() << std::setw(12) << "[DEBUG]" << message << std::endl;
		}

		void warning(std::string message)
		{
			_err_file << std::left << std::setw(24) << formatTime() << std::setw(12) << "[WARN]" << message << std::endl;
			_exe_file << std::left << std::setw(24) << formatTime() << std::setw(12) << "[WARN]" << message << std::endl;
		}

		void error(std::string message)
		{
			_err_file << std::left << std::setw(24) << formatTime() << std::setw(12) << "[ERROR]" << message << std::endl;
			_exe_file << std::left << std::setw(24) << formatTime() << std::setw(12) << "[ERROR]" << message << std::endl;
		}
	};

	Logger sys_logger("system", std::to_string(time(NULL)));
}
