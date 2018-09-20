#include <fstream>
#include <iostream>
#include <string>
#include <sstream> 
#include "opencv\cv.h"
#include "opencv\highgui.h"
using namespace cv;
using namespace std;
typedef pair<string, float> PAIR;

bool cmp_by_value(const PAIR& lhs, const PAIR& rhs) 
{
	return lhs.second < rhs.second;
}

struct CmpByValue 
{
	bool operator()(const PAIR& lhs, const PAIR& rhs) 
	{
		return lhs.second < rhs.second;
	}
};

int test22(int argc, char *argv[])
{
	
	string  labelpathone = "I:/mtcnn-train/train_cla-all.txt";
	string  ratiopathone = "I:/双个车牌放在一起/左右-plate_left_right_las17/0_all/0_ratio.txt";

	map<string, float> msf;
	fstream finRead;
	finRead.open(labelpathone, ios::in);
	if (!finRead.is_open())
	{
		cout << "finRead 文件数据打开错误！" << endl;
		system("pause");
		return false;
	}

	fstream finWrite;
	finWrite.open(ratiopathone, ios::out);
	if (!finWrite.is_open())
	{
		cout << "finRead 文件数据打开错误！" << endl;
		system("pause");
		return false;
	}

	float minratio = 100; float maxratio = 0;
	string minname; string maxname;
	string line;
	while (getline(finRead, line))
	{
		if (line.empty())
		{
			break;
		}
		
		
			string name; int label; int x1, y1, x2, y2;
			stringstream str_s(line);
			str_s >> name >> label >> x1 >> y1 >> x2 >> y2;

			

			int pos = name.find_last_of('/');
			int pos2 = name.find_first_of('.');
			string temp = name.substr(pos + 1, pos2 - pos - 1);

			int width = x2 - x1 + 1;
			int height = y2 - y1 + 1;
			float fl = width*1.0 / height;
			//
			msf[temp] = fl;
			if (minratio>fl)
			{
				minratio = fl;
				minname = name;
			}
			if (maxratio<fl)
			{
				maxratio = fl;
				maxname = name;
			}
		
	}

	finRead.close();
	
	printf("maxratio=%0.2f,minratio=%0.2f\n",maxratio,minratio);
	cout <<"minname:"<<minname<< endl;
	cout << "maxname:" << maxname << endl;

	vector<PAIR> name_score_vec(msf.begin(), msf.end());
	sort(name_score_vec.begin(), name_score_vec.end(), CmpByValue());

	/*map<string, float>::iterator it;
	for (it = msf.begin(); it != msf.end();it++)
	{
		finWrite << it->first << ";"<<it->second << endl;
	}*/

	for (int i = 0; i != name_score_vec.size(); ++i) 
	{
		finWrite << name_score_vec[i].first << ";" << name_score_vec[i].second << endl;
	}
	
	finWrite.close();
	return 0;
}