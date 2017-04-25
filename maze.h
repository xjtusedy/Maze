//MAZE.H--the header file of the chass

#ifndef MAZE_H
#define MAZE_H
#include<iostream>
#include<vector>
#include<ctime>
#include<string>
#include<cstdlib>
#include<fstream>
using namespace std;

const int N=4;

struct misRM                                  //�洢������Ϣ
{
	int rmset;
	int rmpos;
	int rmstate[N];
};

class Maze
{
public:
	Maze(int m,int n);
	void CreateMaze();                         //�����Թ�
	void FindPath(vector<misRM>& v);           //Ѱ���Թ�·��
	void DisPlay(const string &s);             //����Թ�
	void PathMsg(const string &s,
		         vector<misRM>& v);            //����Թ�·����Ϣ
	void DisPlay(const string &s,
		         vector<misRM>& v);            //�����·�����Թ�

private:
	vector<misRM> MyMaze;                      //�洢�Թ�
	int row;                                   //�洢ÿ�з�����
	bool isInPath(vector<misRM>& v,misRM& m);  //��ѯ�Թ��з����Ƿ����Թ�·����
	bool isConnect(misRM& m,misRM& n);         //�鿴���������Ƿ���ͨ
	int  FindSet(misRM& m,int& n);             //���Ҽ��ϲ��������ڵ�λ��
	void UnionRM(misRM& m,misRM& n);           //�ϲ�����������·��
};

#endif