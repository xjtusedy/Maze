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

struct misRM                                  //存储房间信息
{
	int rmset;
	int rmpos;
	int rmstate[N];
};

class Maze
{
public:
	Maze(int m,int n);
	void CreateMaze();                         //构建迷宫
	void FindPath(vector<misRM>& v);           //寻找迷宫路径
	void DisPlay(const string &s);             //输出迷宫
	void PathMsg(const string &s,
		         vector<misRM>& v);            //输出迷宫路径信息
	void DisPlay(const string &s,
		         vector<misRM>& v);            //输出带路径的迷宫

private:
	vector<misRM> MyMaze;                      //存储迷宫
	int row;                                   //存储每行房间数
	bool isInPath(vector<misRM>& v,misRM& m);  //查询迷宫中房间是否在迷宫路径中
	bool isConnect(misRM& m,misRM& n);         //查看两个房间是否连通
	int  FindSet(misRM& m,int& n);             //查找集合并保留祖宗的位置
	void UnionRM(misRM& m,misRM& n);           //合并房间以生成路径
};

#endif