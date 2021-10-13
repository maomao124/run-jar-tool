#include<iostream>
#include<stdio.h>
#include<string.h>
#include <Windows.h>
#include<time.h>
#include <graphics.h>
#include <conio.h>
#include<algorithm>
#include<stdlib.h>
#include <random>
#include <string>
#include <fstream>
#include <ddraw.h>
#include<iomanip>
#include <Psapi.h>
#include <direct.h>
#include<io.h>
#include<vector>
#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include<vector>
#include <audioclient.h>
#include <sapi.h>                                             // 语音头文件
#include <sphelper.h>                                     // 语音识别头文件
#include <thread>                                           //多线程
#include"resource1.h"
//vs2019自定义控制台代码模板 by mao
 //作者QQ1296193245
using namespace std;
#pragma warning(disable : 4996)
#pragma comment (lib,"ddraw.lib")
#pragma comment(lib,"sapi.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"Winmm.lib")                     // 引用 Windows Multimedia API
#pragma intrinsic(__rdtsc)
unsigned __int64 t1, t2, t3, t4;
double run_time;                                                   //微秒级算法时间计时器
LARGE_INTEGER time_start;	                                //开始时间
LARGE_INTEGER time_over;	                                //结束时间
double dqFreq;		                                               //计时器频率
LARGE_INTEGER f;	                                               //计时器频率
double tt;
long double totaltime;

inline void runstart()
{
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	t1 = __rdtsc();
	QueryPerformanceCounter(&time_start);	       //计时开始
}
inline void runend()
{
	QueryPerformanceCounter(&time_over);	        //计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	t2 = __rdtsc();
	t3 = t2 - t1;
}
void ShowProcessMemoryUsageInfo()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	float memoryUsage_M = pmc.WorkingSetSize / (1024.0 * 1024.0);
	float memoryUsage_K = pmc.WorkingSetSize / 1024.0;
	std::cout << std::fixed << setprecision(0) << "程序内存使用率：" << memoryUsage_K << " KB = " << setprecision(2) << memoryUsage_M << " MB" << std::endl;
}
__int64 CompareFileTime(FILETIME time1, FILETIME time2)//转换为整数时间
{
	__int64 a = static_cast<long long>(time1.dwHighDateTime) << 32 | time1.dwLowDateTime;
	__int64 b = static_cast<long long>(time2.dwHighDateTime) << 32 | time2.dwLowDateTime;
	return   (b - a);
}
void rundisplay()
{
	cout << endl;
	//cout << "-------------------------------------------------------" << endl;
	if (t3 < 10000)
	{
		cout << "CPU脉冲数：" << (t3) << endl;
	}
	else if (t3 < 100000000 && t3 >= 10000)
	{
		tt = t3;
		cout << "CPU脉冲数：" << (tt) / 10000 << "万" << endl;
	}
	else if (t3 > 100000000000)
	{
		t4 = t3;
		t4 = t3 / 10000000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10 << "亿" << endl;
	}
	else
	{
		t4 = t3;
		t4 = t3 / 10000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10000 << "亿" << endl;
	}
	if (run_time > 10000)
	{
		cout << "算法运行时间：" << run_time / 1000 << "秒" << endl;
	}
	else if (run_time < 1)
	{
		cout << "算法运行时间：" << run_time * 1000 << "微秒" << endl;
	}
	else
	{
		cout << "算法运行时间：" << run_time << "毫秒" << endl;
	}
	totaltime = clock();
	cout << "程序运行时间：" << totaltime / 1000 << "秒" << endl;
	if (run_time > 0.4)
	{
		double frequency = t3 / run_time / 1000;
		cout << "CPU频率：" << frequency << "MHZ" << endl;
	}
	ShowProcessMemoryUsageInfo();
	long lRet;
	HKEY hKey;
	TCHAR tchData[64];
	DWORD dwSize;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)tchData, &dwSize);
		if (lRet == ERROR_SUCCESS)
		{
			cout << "CPU 名字：";
			wcout << tchData << endl;
		}
		else
		{
			cout << "CPU 名字：";
			wcout << L"未知" << endl;
		}
	}
	else
	{
		cout << "CPU 名字：";
		wcout << L"未知" << endl;
	}
	RegCloseKey(hKey);
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	double totalphy = statex.ullTotalPhys / 1024 / 1024;
	totalphy = totalphy / 1024;
	cout << "总物理内存：    " << setprecision(4) << left << setw(7) << totalphy << " GB" << endl;
	double totalvir = statex.ullTotalPageFile / 1024 / 1024;
	totalvir = totalvir / 1024;
	cout << "总虚拟内存：    " << setw(7) << totalvir - totalphy << " GB" << endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 }; //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		//printf("光标坐标:(%d,%d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	HANDLE   hStdout;
	//   光标位置   
	COORD     cursorPos;
	//   标准输出句柄   
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
	double freephy = statex.ullAvailPhys / 1024 / 1024;
	freephy = freephy / 1024;
	double usephy = (totalphy - freephy) * 100.0 / totalphy;
	cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
	double freetotal = statex.ullAvailPageFile / 1024 / 1024;
	freetotal = freetotal / 1024;
	cout << "总空闲内存：    " << setprecision(3) << freetotal << " GB" << endl;
	double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
	cout << "虚拟内存使用量：" << setprecision(3) << freevir << " GB" << endl;
	double usevir = freevir * 100 / (totalvir - totalphy);
	cout << "虚拟内存使用率：";
	printf("%.2f%%", usevir);
	cout << endl;
	cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << 0.00 << "%" << endl;
	cout << "CPU空闲率：    " << setw(6) << 100.0 << "%" << endl;
	//cout << "-------------------------------------------------------" << endl;
	cout << endl;
	cout << "按任意键退出程序" << endl;
	while (!_kbhit())
	{
		GlobalMemoryStatusEx(&statex);
		WaitForSingleObject(hEvent, 1000);
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		int idle = CompareFileTime(preidleTime, idleTime);
		int kernel = CompareFileTime(prekernelTime, kernelTime);
		int user = CompareFileTime(preuserTime, userTime);
		float cpu = (kernel + user - idle) * 100.0 / (kernel + user);
		float cpuidle = (idle) * 100.0 / (kernel + user);
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 0;
		SetConsoleCursorPosition(hStdout, cursorPos);
		//cout << "内存使用率：" << statex.dwMemoryLoad << "%" << endl;
		double freephy = statex.ullAvailPhys / 1024 / 1024;
		freephy = freephy / 1024;
		double usephy = (totalphy - freephy) * 100.0 / totalphy;
		cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 1;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freetotal = statex.ullAvailPageFile / 1024 / 1024;
		freetotal = freetotal / 1024;
		cout << "总空闲内存：    " << setprecision(3) << freetotal << " GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 2;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
		cout << "虚拟内存使用量：" << setprecision(3) << freevir << " GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 3;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double usevir = freevir * 100 / (totalvir - totalphy);
		cout << "虚拟内存使用率：";
		printf("%.2f%%", usevir);
		cout << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 4;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << cpu << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 5;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU空闲率：    " << setw(6) << cpuidle << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 6;
		SetConsoleCursorPosition(hStdout, cursorPos);
		//cout << "-------------------------------------------------------" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 7;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "按任意键退出程序" << endl;
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}
	cout << endl;

}


//**************************************************************//************************非主函数

// 提取指定模块中的资源文件
// 参数：
//     strDstFile:     目标文件名。提取的资源将保存在这里；
//     strResType:     资源类型；
//     strResName:     资源名称；
// 返回值：
//     true: 执行成功；
//     false: 执行失败。

bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName)
{
	// 创建文件
	HANDLE hFile = ::CreateFile(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// 查找资源文件中、加载资源到内存、得到资源大小
	HRSRC    hRes = ::FindResource(NULL, strResName, strResType);
	HGLOBAL    hMem = ::LoadResource(NULL, hRes);
	DWORD    dwSize = ::SizeofResource(NULL, hRes);

	// 写入文件
	DWORD dwWrite = 0; // 返回写入字节
	::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
	::CloseHandle(hFile);

	return true;
}

char* Wchar2char(const wchar_t* szStr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
	return pResult;
}

string GetExePath()
{
	char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	/*
	strrchr:函数功能：查找一个字符c在另一个字符串str中末次出现的位置（也就是从str的右侧开始查找字符c首次出现的位置），
	并返回这个位置的地址。如果未能找到指定字符，那么函数将返回NULL。
	使用这个地址返回从最后一个字符c到str末尾的字符串。
	*/
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串//
	string path = szFilePath;
	return path;
}

void getFiles(string path, vector<string>& files)
{
	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void getFilesAll(string path, vector<string>& files)
{
	//文件句柄
	long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					getFilesAll(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

string solve(string& s1, string& s2)
{
	if (s2.size() == 0)
		return s1;
	for (int i = 0;i < (int)(s1.size() - s2.size() + 1);)
	{
		if (s1.substr(i, s2.size()) == s2)
		{
			s1.erase(i, s2.size());
		}
		else
			i++;
	}
	return s1;
}

/*
path: 指定目录
files: 保存结果
fileType: 指定的文件格式，如 .jpg
*/
void getAllFiles(string path, vector<string>& files, string fileType)
{
	// 文件句柄
	long hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
		do {
			// 保存文件的全路径
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1

		_findclose(hFile);
	}
}

BOOL SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	return SetConsoleTextAttribute(hConsole, wAttributes);
}

#define FOREGROUND_BLUE      0x0001 // text color contains blue.文本颜色包含蓝色。
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.文本颜色增强。
#define BACKGROUND_BLUE      0x0010 // background color contains blue.//背景色包含蓝色。
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.
#define COMMON_LVB_LEADING_BYTE    0x0100 // Leading Byte of DBCS
#define COMMON_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
#define COMMON_LVB_GRID_HORIZONTAL 0x0400 // DBCS: Grid attribute: top horizontal.
#define COMMON_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
#define COMMON_LVB_GRID_RVERTICAL  0x1000 // DBCS: Grid attribute: right vertical.
#define COMMON_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
#define COMMON_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.
#define COMMON_LVB_SBCSDBCS        0x0300 // SBCS or DBCS flag.

//**************************************************************//************************

int main(int args, char* argv[])
{

	//*************************************************************************



	//*************************************************************************

	//*************************计时开始***************************************


	//始终用此程序打开jar文件
	//HWND hwnd;
	//if (hwnd = ::FindWindow(L"ConsoleWindowClass", NULL)) //找到控制台句柄
	//{
	//	::ShowWindow(hwnd, SW_HIDE); //隐藏控制台窗口
	//}

	/*
	ifstream in("run jar.bat", ios::in);
	if (!in)
	{

		// 产生临时文件的文件名
		TCHAR tmpmp3[_MAX_PATH] = L"";
		//::GetTempPath(_MAX_PATH, tmpmp3);                                                                                // L"C:\\Users\\用户名\\AppData\\Local\\Temp\\"
		_tcscat(tmpmp3, _T("run jar.bat"));     // VC2008、VC2010 请用 _tcscat_s 函数                            //临时文件路径，更改项

		// 将 MP3 资源提取为临时文件
		//ExtractResource(tmpmp3, _T("bin"), _T("background.mp3"));
		// 注：如果 MP3 资源的 ID 为宏 IDR_BACKGROUND，这样做：
		ExtractResource(tmpmp3, _T("bin"), MAKEINTRESOURCE(IDR_BIN1));                                 //资源类型和资源名称，更改项

	   // 打开音乐
		//TCHAR mcicmd[300];
		//_stprintf(mcicmd, _T("open \"%s\" alias mymusic"), tmpmp3);     // VC2008、VC2010 请用 _stprintf_s 函数
		//wcout << mcicmd << endl;
		//mciSendString(mcicmd, NULL, 0, NULL);

		// 播放音乐
		//mciSendString(_T("play mymusic"), NULL, 0, NULL);
		//循环播放：
		//mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);

		// 输出提示信息
		//outtextxy(0, 0, _T("按任意键停止播放"));
		//_getch();

		// 停止播放并关闭音乐
		//mciSendString(_T("stop mymusic"), NULL, 0, NULL);
		//mciSendString(_T("close mymusic"), NULL, 0, NULL);

		// 删除临时文件
		//DeleteFile(tmpmp3);
		string s;
		char path[_MAX_PATH];
		getcwd(path, MAX_PATH);
		cout << path;
		string s1 = path;
		s = "start \"\" \"" + s1 + "\\run jar.bat\"";
		system(s.c_str());
	}
	else
	{
		//WCHAR runpath[_MAX_PATH];
		//::GetTempPath(_MAX_PATH,runpath);
		string s;

		char path[_MAX_PATH];
		getcwd(path, MAX_PATH);
		cout << path;
		string s1 = path;
		s = "start \"\" \"" + s1 + "\\run jar.bat\"";
		system(s.c_str());
	}
	in.close();
	//string str = GetExePath();
	//cout << endl;
	//cout << str << endl;
	wchar_t ExeFile[200];
	//得到当前文件路径名
	GetModuleFileName(NULL, ExeFile, 200);
	printf("当前文件路径为：\n");
	printf("%s\n", ExeFile);
	*/

	bool ishide = false;

	SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << setw(40) << "jar文件启动器2.1   by mao" << endl;
	SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	//vector<string> files;
	char buffer[MAX_PATH];
	getcwd(buffer, MAX_PATH);
	cout << left << setw(30) << "  当前工作目录为：" << buffer << endl;
	//char* filePath = buffer;
	string path = buffer;
	//getAllFiles(filePath, files,".jar");
	//char str[30];
	//int size = files.size();
	if (args == 2)
	{
		path = path + "\\";
		string pathfile = argv[1];
		//solve(files[0], path);
		solve(pathfile, path);
		if (pathfile.find(".jar") >= pathfile.length())
		{
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "文件后缀名不正确\a！！！    正确的后缀名应为.jar" << endl;
			cout << "按任意键退出程序......." << endl;
			_getch();
			exit(0);
		}
		if (pathfile.find("_hide") < pathfile.length() - 3)
		{
			ishide = true;
			HWND hwnd;
			if (hwnd = ::FindWindow(L"ConsoleWindowClass", NULL)) //找到控制台句柄
			{
				::ShowWindow(hwnd, SW_HIDE); //隐藏控制台窗口
			}
		}

		cout << setw(30) << "  即将执行的jar文件的名称：";
		//cout << files[0].c_str() << endl;
		cout << pathfile << endl;
		string strcmd = "java -jar ";
		strcmd = strcmd + pathfile;
		cout << setw(2) << "" << "开始执行！   执行内容如下：" << endl;
		cout << "------------------------------------------------" << endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //默认情况下控制台颜色是白色，所以用“RGB”组合
		if (ishide == 0)
		{
			runstart();
		}
		system(strcmd.c_str());
		cout << endl;
		SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "------------------------------------------------" << endl;
		cout << setw(20) << "jar文件执行完毕！以下内容不属于jar文件" << endl;
		ofstream out("C:\\tools\\jar文件启动器\\run jar.log", ios::app);
		SYSTEMTIME sys;                                    //获取系统时间
		GetLocalTime(&sys);
		out << "日期：";
		out << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "\t";
		out << "时间：";
		out << setfill('0') << setw(2) << sys.wHour << ":" << setfill('0') << setw(2) << sys.wMinute << ":" << setfill('0') << setw(2) << sys.wSecond << "\t";
		out << "启动的jar文件的路径：";
		out << argv[1] << endl;
		out.close();
	}

	else if (args > 2)
	{
		path = path + "\\";
		cout << "  调用了多个jar文件，请选择要运行的文件" << endl;
		cout << "-------------------------------" << endl;
		string s2;
		for (int i = 0;i < args - 1;i++)
		{
			s2 = argv[i + 1];
			solve(s2, path);
			cout << right << setw(4) << i + 1 << ". " << s2 << endl;
		}
		cout << "-------------------------------" << endl;
		cout << "  请输入序号：";
		int a;
	loop:
		cin >> a;
		if (!cin)                                                          //解决循环内整形或者浮点类型数据输入字符型就会陷入死循环的问题
		{                                                                   //只有输入操作失败，才会跳转到这里
			if (cin.bad())
			{                                                              //流发生严重故障，只能退出函数
				cout << "cin is bad!";
				exit(0);
			}
			if (cin.eof())
			{                                                             //检测是否读取结束

			}
			if (cin.fail())
			{                                                              //流遇到了一些意外情况（int型输入字母）
				cin.clear();                                          //清除/恢复流状态
				string se;
				cin >> se;
				//getchar();                                        //方案二
				cout << endl << "\a请不要胡乱输入！！！" << endl;
				goto loop;
			}
		}
		if (a <= 0 || a > (args - 1))
		{
			cout << "输入的数据不在范围内，请重新输入！！！" << endl;
			goto loop;
		}
		string strcmd = "java -jar ";
		//strcmd = strcmd + files[a-1];
		string s3 = argv[a];
		solve(s3, path);
		if (s3.find(".jar") >= s3.length())
		{
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "文件后缀名不正确\a！！！    正确的后缀名应为.jar" << endl;
			cout << "按任意键退出程序......." << endl;
			_getch();
			exit(0);
		}

		if (s3.find("_hide") < s3.length() - 3)
		{
			ishide = true;
			HWND hwnd;
			if (hwnd = ::FindWindow(L"ConsoleWindowClass", NULL)) //找到控制台句柄
			{
				::ShowWindow(hwnd, SW_HIDE); //隐藏控制台窗口
			}
		}

		strcmd = strcmd + s3;
		cout << setw(2) << "" << "开始执行！   执行内容如下：" << endl;
		cout << "------------------------------------------------" << endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //默认情况下控制台颜色是白色，所以用“RGB”组合
		if (ishide == 0)
		{
			runstart();
		}
		system(strcmd.c_str());
		cout << endl;
		SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "------------------------------------------------" << endl;
		cout << setw(20) << "jar文件执行完毕！以下内容不属于jar文件" << endl;
		ofstream out("C:\\tools\\jar文件启动器\\run jar.log", ios::app);
		SYSTEMTIME sys;                                    //获取系统时间
		GetLocalTime(&sys);
		out << "日期：";
		out << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "\t";
		out << "时间：";
		out << setfill('0') << setw(2) << sys.wHour << ":" << setfill('0') << setw(2) << sys.wMinute << ":" << setfill('0') << setw(2) << sys.wSecond << "\t";
		out << "启动的jar文件的路径：";
		out << argv[a] << endl;
		out.close();
	}

	else if (args == 1)
	{
		runstart();
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "  异常，未找到jar文件！！！   请通过jar文件调用此程序\a" << endl;
		cout << "  选中要运行的jar文件，右键，打开方式，选择始终使用此程序打开jar文件" << endl;
		cout << "  使用此程序前需要配置java环境变量" << endl;
		cout << "  提示：当jar的文件名称包含 \"_hide\" 关键字时，用此程序启动jar文件时将会自动隐藏控制台" << endl;
		SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "  按tab键查看日志，或者按其它键退出" << endl;
		char ch;
		ch = _getch();
		string strlog;
		long totallog = 0;
		if (ch==9)
		{
			string pass;
			cout << "请输入密码：";
			cin >> pass;
			if (pass!="123")
			{
				SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout<<"密码错误！！！"<<endl;
				SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			else
			{ 
			ifstream log("C:\\tools\\jar文件启动器\\run jar.log", ios::in);
			{
				if (!log)
				{
					SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
					cout << "没有找到日志文件！！！" << endl;
					SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				}
				else
				{

					while (!log.eof())
					{
						getline(log, strlog);
						cout << strlog << endl;
						totallog++;
					}
					cout << "一共" << totallog << "项" << endl;
				}
			}
			}
			
		}
	}

	//*************************计时结束***************************************

	

	if (ishide == 0)
	{
		runend();
		system("pause");
		rundisplay();                                                                   //计时结果显示
		_getch();
	}
	


	//if (ishide == 1)
	//{
	//	Beep(900, 300);
	//}
	
	//closegraph();
	// system("pause");
	//Sleep(10000);
	return 0;
}
