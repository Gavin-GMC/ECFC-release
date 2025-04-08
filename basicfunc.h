//------------------------Description------------------------
// This file is consisted of some commonly used basic funcions
// and function templete, which provide simple calculations and 
// operations for the optimizer.
//-------------------------Reference-------------------------
//-------------------------Copyright-------------------------
// Copyright (c) 2024 ���������ƣ���ȷ�ϣ�, All Rights Reserved.
// You are free to use the ECFC����ȷ�ϣ� for research purposes.
// All publications which use this library or any code in the library
// should acknowledge the use of "ECFC" and reference 
// "δȷ��"
//-----------------------------------------------------------

#pragma once
#if defined(_WIN32) || defined(_WIN64)
#define NOMINMAX
#include <direct.h>
#include <io.h>
#include<Windows.h>
#else
#include <sys/io.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif 

#include"ecfc-constant.h"
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<cmath>
#include<cstring>
#include<iomanip>
#include<algorithm>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include<random>
#include"./_rels/alglib/hungarian.h"

namespace ECFC
{
	//create the directory if not exist 
	//����һ���ļ���������ļ��в�����
	void dir_create(const std::string& dir)
	{
		int flag;
		
#if defined(WIN32) || defined(_WIN64)
		if (_access(dir.c_str(), 0) == -1) // directory is no exist
		{
			flag = _mkdir(dir.c_str());
			if (flag != 0)
			{
				throw std::runtime_error("Failed to create directory: " + dir);
			}
		}
#else
		if (access(dir.c_str(), 0) == -1) // directory is no exist
		{
			flag = mkdir(dir.c_str(), S_IRWXU);
			if (flag != 0)
			{
				throw std::runtime_error("Failed to create directory: " + dir);
			}
		}
#endif 
	}

	// ��ѯ���ļ��Ƿ����
	bool file_exist(const std::string& path)
	{
		std::ifstream f(path);

		return f.good();
	}

	//system environment initialization
	//ϵͳ˯��ʱ��
	void sys_sleep(int sec)
	{
#if defined(WIN32) || defined(_WIN64)
		Sleep(sec * 1000);
#else
		sleep(sec);
#endif 
	}

	std::string formatTime()
	{
		time_t now = time(NULL);
		tm tm_now;
		
#if defined(WIN32) || defined(_WIN64)
		localtime_s(&tm_now, &now);
#else
		localtime_r(&now, &tm_now);
#endif 
		
		std::stringstream ss;
		ss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");

		return ss.str();
	}

	// ��һ��
	void normalization(double* input, double* output, size_t length)
	{
		if (length == 0)
			return;

		// �ҵ����������е���Сֵ�����ֵ
		double min_value = *std::min_element(input, input + length);
		double max_value = *std::max_element(input, input + length);

		// �����Сֵ�����ֵ��ȣ�˵������ֵ����ͬ��ֱ�ӽ����������Ϊ0
		if (min_value == max_value) {
			std::fill(output, output + length, 0.0);
			return;
		}

		// �����һ�����ֵ���洢���������
		for (size_t i = 0; i < length; i++) {
			output[i] = (input[i] - min_value) / (max_value - min_value);
		}

	}

	//return the sigmoid value
	double sigmoid(double value)
	{
		return 1 / (1 + pow(E_CONST, value));
	}

    template<class T>
    void shuffle(T* arr, size_t n)
    {
        // ���������������
        std::random_device rd;  // ���������
        std::mt19937 g(rd());   // ʹ�� Mersenne Twister ������

        // ʹ�� std::shuffle ��������
        std::shuffle(arr, arr + n, g);
    }

	//return a value between 0-1
	double rand01()
	{
		return double(rand()) / RAND_MAX;
	}

	//return a value between 0-1 but not be 0 or 1
	double rand01_()
	{
		return double(rand() + 1) / (double(RAND_MAX) + 2);
	}

    int wide_rand()
    {
        std::random_device rd;
        std::mt19937 gen(rd());  // ʹ�� Mersenne Twister ������

        // ֱ�����������
        return gen() % 2147483648;  // ȡģ���Ʒ�Χ
    }

	//return the eula distance between two points
	double eu_distance(double a[], double b[], size_t size = 2)
	{
		double back = 0;
		for (size_t i = 0; i < size; i++)
		{
			back += pow(a[i] - b[i], 2);
		}

		return sqrt(back);
	}

	//return the manhattan distance between two points
	double man_distance(double a[], double b[], size_t size = 2)
	{
		double back = 0;
		for (size_t i = 0; i < size; i++)
		{
			back += fabs(a[i] - b[i]);
		}
		return back;
	}

	//return the chebyshec distance between two points
	double che_distance(double a[], double b[], size_t size = 2)
	{
		double back = 0;
		double bu;
		for (size_t i = 0; i < size; i++)
		{
			bu = fabs(a[i] - b[i]);
			if (bu > back)
				back = bu;
		}
		return back;
	}

    // return the hamming distance between two points
    double hamming_distance(double a[], double b[], size_t size = 2, double accuracy = EQACCURACY)
    {
        double back = 0;
        for (size_t i = 0; i < size; i++)
        {
            if (std::abs(a[i] - b[i]) > accuracy)
               back++;
        }
        return back;
    }

    // return the cosine similarity between two points
    double cos_similarity(double a[], double b[], size_t size = 2)
    {
        double dot_product = 0.0;  // ���
        double norm_a = 0.0;       // ���� a ��ģ��ƽ��
        double norm_b = 0.0;       // ���� b ��ģ��ƽ��

        for (size_t i = 0; i < size; ++i) {
            dot_product += a[i] * b[i];
            norm_a += a[i] * a[i];
            norm_b += b[i] * b[i];
        }

        if (norm_a == 0.0 || norm_b == 0.0) {
            throw std::invalid_argument("Vectors must have non-zero norms");
        }

        return dot_product / (std::sqrt(norm_a) * std::sqrt(norm_b));
    }

    // return the partition distance between set
    double partition_distance(int a[], int b[], size_t size = 2)
    {
        // ȷ���ص���Ŀ
        std::unordered_set<int> uniqueValues(a, a + size);
        size_t a_partition = uniqueValues.size();
        std::unordered_map<int, int> a_id;
        int id = 0;
        for (int value : uniqueValues) {
            a_id[value] = id++;
        }

        uniqueValues.clear();
        uniqueValues.insert(b, b + size);
        size_t b_partition = uniqueValues.size();
        std::unordered_map<int, int> b_id;
        id = 0;
        for (int value : uniqueValues) {
            b_id[value] = id++;
        }
        // �����������
        int* allo_matrix = new int[a_partition * b_partition + 1];
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                allo_matrix[i * size + j] = 0;

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                allo_matrix[a_id[a[i]] * size + b_id[b[j]]] ++;

        // ����������
        int** cost_matrix = new int* [size];
        for (int i = 0; i < size; i++)
            cost_matrix[i] = allo_matrix + i * size;
        int best_match = alglib::kuhnmunkres(cost_matrix, size);
        delete[] cost_matrix;
        delete[] allo_matrix;

        // ���ط�������
        return double(size - best_match);
    }

    // return the partition distance between set
    double partition_distance(double a[], double b[], size_t size = 2, double accuracy = EQACCURACY)
    {
        int* _a = new int[size];
        int* _b = new int[size];
        for(int i=0;i<size;i++)
        {
            _a[i] = a[i] / accuracy;
            _b[i] = b[i] / accuracy;
        }
        int back = partition_distance(_a, _b, size);

        delete[] _a;
        delete[] _b;
        return back;
    }

	// ��������ֵ�ھ������Ƿ����
	bool equal(double a, double b, double accuracy = EQACCURACY)
	{
		return abs(a - b) <= accuracy;
	}

	// ��������ֵ�ھ�����ǰֵ�Ƿ���ں�ֵ
	bool large(double a, double b, double accuracy = EQACCURACY)
	{
        return a - b > accuracy;
	}

	// ��������ֵ�ھ�����ǰֵ�Ƿ�С�ں�ֵ
	bool less(double a, double b, double accuracy = EQACCURACY)
	{
        return b - a > accuracy;
	}

	// ��������ֵ�ھ�����ǰֵ�Ƿ񲻴��ں�ֵ
	bool notlarge(double a, double b, double accuracy = EQACCURACY)
	{
        return a - b <= accuracy;
	}

	// ��������ֵ�ھ�����ǰֵ�Ƿ�С�ں�ֵ
	bool notless(double a, double b, double accuracy = EQACCURACY)
	{
        return b - a <= accuracy;
	}

	void stringSplit(const std::string& str, const char split, std::vector<std::string>& res)
	{
		if (str == "")
			return;
		//���ַ���ĩβҲ����ָ����������ȡ���һ��
		std::string strs = str + split;
		size_t pos = strs.find(split);

		// ���Ҳ����������ַ��������������� npos
		while (pos != strs.npos)
		{
			std::string temp = strs.substr(0, pos);
			res.push_back(temp);
			//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
			strs = strs.substr(pos + 1, strs.size());
			pos = strs.find(split);
		}
	}

	// ���ַ���ת��ΪΨһint
	size_t stringHash(const std::string& str)
	{
		std::hash<std::string> hash_fn;
		size_t hash_value = hash_fn(str);
		return hash_value;
	}

	// ����double���ȵ���ȡ�����(�����⣡����)
	int intelliTrunc(double value)
	{
		return int(value + EQACCURACY);
	}
}