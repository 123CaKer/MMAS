#include "common.h"
#include<cstdlib>
double g_Pheromone[N_CITY_COUNT][N_CITY_COUNT]; //�������м���Ϣ��
double g_Distance[N_CITY_COUNT][N_CITY_COUNT]; //�������м����

double g_Prob[N_CITY_COUNT][N_CITY_COUNT];
double g_DistanceBeta[N_CITY_COUNT][N_CITY_COUNT];



//////////////////////////////////////////////////////////////////////////////
//
//  �� ;������ָ����Χ�ڵ��������
//
//  ˵ ������
//
//  �� �ڣ�int nLow �����������
//                     int nUpper ������������
//
//  �� �أ�int ,��Χ��[nLow,nUpper)
//
/////////////
int rnd(int nLow,int nUpper)
{
	
	return nLow+(nUpper-nLow)*rand()/(RAND_MAX+1);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  �� ;������ָ����Χ�ڵ����������
//
//  ˵ ������
//
//  �� �ڣ�double dbLow �����������
//                     double dbUpper �����������
//
//  �� �أ�double  ,��Χ��[dbLow,dbUpper)
//
/////////////
double rnd(double dbLow,double dbUpper)
{
	double dbTemp=rand()/((double)RAND_MAX+1.0);
	return dbLow+dbTemp*(dbUpper-dbLow);
}

//���ظ�������������ȡ����ĸ�����
double ROUND(double dbA)
{
	return (double)((int)(dbA+0.5));
}
