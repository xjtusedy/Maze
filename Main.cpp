
#include"maze.h"

int main()
{
	int i;
	int Row,Column;
	string File;
	char Default;
	vector<misRM> MazePath;
	ofstream ClearFile;

	for(i=0;i<30;i++)
		cout<<"* ";
	cout<<endl;
	cout<<"Input Y or y to Random a Maze with Default Msg that is given!!!"<<endl<<endl;
	cout<<"Default:";
	cin>>Default;
	if(Default=='Y'||Default=='y')
	{
		Row=30;
		Column=50;
		File="maze.txt";
	}
	else
	{
		cout<<"Input the two Numbers:(row|column)!!"<<endl<<endl;;
		cout<<"ROW:";
		cin>>Row;
		cout<<"COLUMN:";
		cin>>Column;
		cout<<"Input a File's Name so that you can keep your maze with the file!!"<<endl<<endl;
		cout<<"FileName:";
		cin>>File;                               //比如文件名称"maze.txt"
	}
	cout<<endl<<"Now the computer will Random a Maze with the row and column you input"<<endl;
	for(i=0;i<30;i++)
		cout<<"* ";
	cout<<endl;

//**************清空文本信息**************
	ClearFile.open(File.c_str());
	ClearFile.close();
//**************清空文本信息**************

	Maze maze(Row,Column);                       //初始化迷宫
	maze.DisPlay(File);                          //输出初始化迷宫信息
	maze.CreateMaze();                           //随机生成迷宫
	maze.DisPlay(File);                          //输出迷宫信息
	maze.FindPath(MazePath);                     //查找迷宫路径
	maze.DisPlay(File,MazePath);                 //输出带路径的迷宫
	maze.PathMsg(File,MazePath);                  //输出迷宫路径
	return 0;
}
