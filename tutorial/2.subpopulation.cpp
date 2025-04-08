#include"ecfc.h"

long long int buffer;

double f1(double** a)
{
	double back = 0;
	for (int i = 0; i < 5; i++)
	{
		back += a[0][i] * a[0][i];
	}
	return back;
}

double heuristic(double** input)
{
	return -1 * abs(input[1][0]);
}

int main()
{
	// ���ⶨ��
	ECFC::Problem problem("test_problem");
	// ��ӱ���
	problem.addVariable("x", -5, 5, 0.001, 5);
	// ���Ŀ�꺯����f1��С����f2���
	problem.addObjective("f1", 1, true, "x", f1);
	// �������������x����ֵ�ĸ�ֵ,y�������ʽ��Ĭ�ϣ�����ȱʡ���ã�
	problem.addInspirationFunc("x", "x", heuristic);
	// ���Լ��
	problem.addConstrainUnique("x", 1);
	problem.addConstrainRange("x", ECFC::EMPTYVALUE, ECFC::EMPTYVALUE, 1);

	// �������
	ECFC::ProblemHandle* problem_handle = problem.compile();

	// �Ż�������
	ECFC::OptimizerBuilder builder;
	builder.setName("test_optimizer");
	builder.setIndividual(ECFC::IndividualType::F_individual);
	builder.setArchive(ECFC::BestArchiveType::F_normal);
	builder.setTerminateMAXFES(1e3);
	// builder.setTerminateMAXTime(1);
	// builder.setTerminateMAXStop(100);

	// ��ǰ���Խ����ڵ���Ⱥ������
	builder.setSwarmManager(ECFC::SubswarmManagerType::F_single);
	// builder.setSwarmConstruct(ECFC::SubswarmConstructerType::F_fix);
	// builder.setSwarmTopology(ECFC::SubswarmTopologyType::F_connected);

	// �����Ⱥ
	ECFC::SubpopulationSetter* sp_setter = builder.addSubpopulation("1");
	sp_setter->setSwarmSize(20);
	sp_setter->setSolutionIni(ECFC::InitializerType::F_random);
	sp_setter->lstrategy.GA(sp_setter->lstrategy.crossover.point(), sp_setter->lstrategy.mutation.bit());
	sp_setter->ltopology.championship();
	sp_setter->setLFramework(ECFC::OffspringGeneratorType::F_generation);
	sp_setter->setRepairMethod(ECFC::RepairType::F_random);
	sp_setter->setSelector(ECFC::SelectorType::F_index, false);
	sp_setter->setTerminateMAXFES(1e3);
	// sp_setter->setTerminateMAXTime(1);
	// sp_setter->setTerminateMAXStop(100);
	sp_setter->setArchive(ECFC::BestArchiveType::F_normal);

	// ��־����
	builder.setLoggerFull(false);
	builder.setLoggerProcess(false);
	builder.saveConfigure();

	return 0;
}