#pragma once
#include<functional>

namespace ECFC
{
// ��Ҫ����ʲô��
// ������ģ���У���Ϊ��������ĺ���������ȫ�֡���̬��
// �������ᵼ��ȫ�ֻ�����ֻ�ܴ���һ��ͬ������
// ������ֻ��һ�����ظ��Ķ���ᵼ�´���
// ����ܲ���ͨ������ʹͬһ������ģ��Ĳ�ͬʾ����ʵ�����ݡ���������������ʽ�����ȵķ���
// 
// ʵ��˼·��
// ���ڰ�������ָ��ķº��������еģ��º���֮��ֻʵ�������ݵķ��룬������Ҫ���뺯��ָ�룿
// ��������ָ�룬ֱ���ڷº�����д��
// �������º������Ͳ�ͬ����ô�����أ�
// �ڻ���ģ������һ���º����Ļ��࣬�����Ķ�������̳�
// 
// ����ϸ�ڣ�
// ������ģ�廹��һ����ͨ�����أ�ģ��ɣ����������ͳһ
// ���ˣ�ģ���м��
// 
// ���Ծ��к������������
// �������û��������Ϊֱ����STL��ģ������
// ���������������ΪSTL��ģ��̫������
// 
//
	/*
	// һԪ�º���
	template <class _Arg, class _Result>
	struct unary_ecfunctor : std::unary_function<_Arg, _Result>
	{
		unary_ecfunctor() {}

		virtual _Result operator()(const _Arg& a) const = 0;

		virtual unary_ecfunctor* copy() = 0;
	};
	*/

	// ��calculatorʹ�õķº���
	struct eccalcul_functor : std::unary_function<double, double>
	// struct eccalcul_functor
	{
		eccalcul_functor() {}

		virtual ~eccalcul_functor()
		{

		}

		virtual double operator()(double** a) const = 0;

		virtual eccalcul_functor* copy() = 0;
	};
	
}