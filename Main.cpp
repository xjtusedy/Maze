//MAIN.CPP--the user can use the main function to test the class an it' functions
/*��������:�Թ�������������Թ�·������
  ����ԭ��:���ཻ���ļ�Ӧ��
  ���ܽ���:���ݲ��ཻ��ԭ����������Թ���������·��
           ����Թ������ּ�ͼ����Ϣ����·����ͼ����Ϣ(���ļ�����)

  ���ʱ��:2010.3.20
  ����ƽ̨:XP/VC++ 6.0������ BY ����*/

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
		cin>>File;                               //�����ļ�����"maze.txt"
	}
	cout<<endl<<"Now the computer will Random a Maze with the row and column you input"<<endl;
	for(i=0;i<30;i++)
		cout<<"* ";
	cout<<endl;

//**************����ı���Ϣ**************
	ClearFile.open(File.c_str());
	ClearFile.close();
//**************����ı���Ϣ**************

	Maze maze(Row,Column);                       //��ʼ���Թ�
	maze.DisPlay(File);                          //�����ʼ���Թ���Ϣ
	maze.CreateMaze();                           //��������Թ�
	maze.DisPlay(File);                          //����Թ���Ϣ
	maze.FindPath(MazePath);                     //�����Թ�·��
	maze.DisPlay(File,MazePath);                 //�����·�����Թ�
	maze.PathMsg(File,MazePath);                  //����Թ�·��
	return 0;
}