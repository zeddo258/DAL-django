// 11127150 張睿恩 , 11127151 楊少杰 
#include <iostream>		// cout, endl
#include <fstream> 		// open, is_open() 
#include <string> 		// string
#include <vector>		// vector
#include <cstring>		// strcpy

#include <cstdlib>		// atoi, system
#include <iomanip>		// setw, setprecision
#include <ctime>		// clock
#include <sstream>		// str
using namespace std;


typedef struct jT
{ 	int OID;		// 工作編號
	int arrival;	// 到達時間
	int duration;	// 持續時間 
	int timeout;	// 超時時間
}	jobType;
// The above are data structures globally available

class JobList {
	vector<jobType> aList; 		// 工作清單
	string	fileID;				// 檔案識別碼
	FILE	*infile = NULL;		// 檔案指標 
	// The above are private data members

	// 初始設定
	void reset()
	{
    	aList.clear();  // 清空 aList
    	fileID = "";
	}


	// 以到達時間來進行排序
	void sortByArrival(int number)
	{
    	number = aList.size(); // 陣列的大小 

    	// 使用 Shell Sort 
    	for (int gap = number / 2; gap > 0; gap /= 2) // gap,子序列的間隔 
    												  // 初始化為陣列大小的一半，每次迭代將間隔減半 
    	{
    		// 對每個子序列進行插入排列 
        	for (int i = gap; i < number; i += 1)
        	{
            	jobType temp = aList[i]; // 暫存當前選取的元素

            	int j;
            	
            	// 在子序列中進行插入排序，比較 arrival和 OID 
            	for (j = i; j >= gap && (aList[j - gap].arrival > temp.arrival || (aList[j - gap].arrival == temp.arrival && aList[j - gap].OID > temp.OID)); j -= gap)
            	{
                	aList[j] = aList[j - gap]; // 移動元素，為當前元素騰出插入的位置
            	}

            	aList[j] = temp; // 將暫存的元素插入到正確的位置 
        	}
    	}
	}


	// 將排序後的工作清單寫回檔案
	void putAll(string fileName)
	{
		fileName = "sorted" + fileID + ".txt"; 
	    ofstream outfile(fileName.c_str()); // 開啟新檔案供寫入

		outfile << "OID" << '\t' << "Arrival" << '\t' << "Duration" << '\t'  << "TimeOut" << endl;


		// 寫入每個工作的 OID、arrival、duration、timeout(以 tab分隔 ) 
    	for (int i = 0; i < aList.size(); i++)
    	{
        	outfile << aList[i].OID << "\t" << aList[i].arrival << "\t" << aList[i].duration << "\t" << aList[i].timeout << endl;
    	}

        outfile.close(); // 關閉檔案 
	}


	// 顯示讀取、排序、寫入時間
	void showTime(double read,double sort,double write)
	{
		cout << "Reading data: " << fixed  <<  setprecision(1)  <<  read  <<  " ms"  <<  endl;
		cout << "Sorting data: " << fixed  <<  setprecision(1)  <<  sort  <<  " ms"  <<  endl;
		cout << "Writing data: " << fixed  <<  setprecision(1)  <<  write  <<  " ms"  <<  endl;

	}


	// 查看下一個工作
	void nextJobCheck(jobType &)
	{
		if (aList.size() > 0)
		{
			jobType nextJob = aList.front(); // 獲取第一個元素 ( = 陣列[0] )
		}
	}


public:
	JobList() { reset(); }		// constructor for initialization
	~JobList() { reset(); }		// destructor for initialization

	// 檢查工作清單是否為空
	bool isEmpty()
	{
		return aList.empty();
	}


	// 顯示整個工作清單的資訊
	void showJob(vector<jobType> &list)
	{
    	cout << "OID" << '\t' << "Arrival" << '\t' << "Duration" << '\t'  << "TimeOut" << endl;

		// 輸出每個工作的信息
    	for(int j = 0; j < list.size(); j++)
        	cout << list[j].OID << '\t' << list[j].arrival << '\t' << list[j].duration << '\t' << list[j].timeout << endl;
	}


	// 從檔案中讀取工作清單
	bool getAll(string fileName)
	{
    	fileID = "input" + fileName + ".txt";
    	ifstream infile(fileID.c_str()); // 以讀取模式打開檔案 

    	if (!infile.is_open()) // 檔案不存在 
    	{
        	cout << endl << fileID << " does not exist!" << endl;
        	return false;
    	}
    	
    	else
    	{
        	jobType job;

        	// 跳過第一行
        	infile.ignore(1000, '\n');  // 使用足夠大的數字確保跳過整行 

        	while (infile >> job.OID >> job.arrival >> job.duration >> job.timeout)
        	{
        		// 持續時間為 0或負數(不合理) 
				// 到達時間 + 持續時間 > 超時時間 (表示工作在規定時間內未完成 ?!  不合理) 
            	if (job.duration <= 0 || (job.arrival + job.duration) > job.timeout)
                	return false;

            	aList.push_back(job); // 向 aList 中添加 
        	}

        	infile.close();
        	return true;
    	}
	}


	// 從檔案中讀取工作清單並進行排序
	bool getSorted(string fileName)
	{
		// 開始計時
		clock_t start = clock();

    	// 如果成功讀取工作清單
		if(getAll(fileName)){
			
			// 停止計時，計算讀取所花時間
			clock_t end = clock();
			fileID = fileName ;
			double read = end - start;
				
        	// 開始計時
			start = clock();
			// 對工作清單進行排序
			sortByArrival(aList.size());
			// 停止計時，計算排序所花時間
			end = clock();
			double sort = end - start;

        	// 開始計時
			start = clock();
			// 將排序後的工作清單寫回檔案
			putAll(fileID) ;
			// 停止計時，計算排序所花時間
			end = clock();
			double write = end - start;

			// 顯示時間
			showTime(read,sort,write);

			return true;
		}

		else
			return false;
	}


	// 取得下一個工作並將其從清單中移除
	void nextJob(jobType &nextJob)
	{
        nextJobCheck(nextJob) ;
        aList.erase(aList.begin()); // 移除第一個工作
	}


	// 用於int main 執行操作 
	void dothing(string fileName)
	{
		getSorted(fileName);
		showJob(aList);
	}

}; //end JobList



template<typename T>
class JobQueue {
	T *cA;						// circular array
	int qFront, qBack;			// head & tail of queue
	int qSize, qMax;			// current size & max size of queue
	// The above are private data members
	int next(int idx) { /* 請自行完成定義 */ } //move to the next position in queue
	// The above are private methods
public:
	int avail;			// the time when CPU is available 	
	// The above are public data members
	
	JobQueue():cA(NULL), qFront(0), qBack(0),
				qSize(0), qMax(0), avail(0) {}		// constructor of no-space queue
	JobQueue<T>(int maxS):qFront(0), qSize(0), avail(0)	// constructor of an empty queue
	{ /*請自行完成定義 */	}//end constructor
	int length() const  { /*請自行完成定義 */	}		// get the current queue length
	bool isEmpty() const { /*請自行完成定義 */}			// check whether it is empty
	bool isFull() const { /*請自行完成定義 */ } 		// check whether it is full
	void enQueue(T &);		// append a new element
	void getFront(T &);		// get the first element
	void deQueue();			// drop the first element
	void deQueue(T &);		// get & drop the first element
	void clearQ();			// clean up
	~JobQueue()				// destructor
	{/*請自行完成定義 */ }  //end destructor
}; //end JobQueue


class AnsList {
	typedef struct aT
	{ /*請自行完成定義 */} abortType;
	typedef struct dT
	{ /*請自行完成定義 */} doneType;
	vector<abortType> abortJobs;		// list of aborted jobs with three columns
	vector<doneType> doneJobs;			// list of done jobs with three columns
	float avgDelay;					// average delay
	float successRate;					// percentage of done jobs
	// The above are private data members
	void showAbortJobs(string);			// declaration: display abort jobs on screen
	void showDoneJobs(string);			// declaration: display done jobs on screen
	void computeStat();					// declaration: compute the statistics
	void showStat();                    // declaration: display statistics on screen 
	// The above are private methods
public:
	AnsList(): avgDelay(0.0), successRate(0.0)
	{ /*請自行完成定義 */ }					// constructor for initialization
	~AnsList()								
	{ /*請自行完成定義 */ }					// destructor for initialization
	void showAll(string);					// declaration: display all on screen
	void addAbortJob(int, int, int, int);	 // declaration: add one aborted job
	void addDoneJob(int, int, int, int); 	// declaration: add one done job 
	// The above are primitive methods
	void putAll(string, string);			// declaration: write all as a file
}; //end AnsList

class Simulation {
	JobList	jobs;	//a list of jobs
	AnsList	answers;	//a set of answers
	JobQueue<jobType> **allQ;	// a set of queues
	int	qNum;	// number of queues
	// The above are private data members
	void delQ(int);	// declaration: delete an old job from a queue
	void updateQ(int, int);	// declaration: update each queue
	void finishQ();	// declaration: finish the remaining jobs in queues
	// The above are private methods
public:
	Simulation(JobList aList, int N): jobs(aList), qNum(N) // copy constructor
	{ /* 請自行完成定義 */} //end constructor
	~Simulation()	// destructor
	{ /*請自行完成定義 */}	//end Destructor
	void SQF();	// declaration: shortest queue first
}; //end Simulation


int main()
{
    JobList jobList;
	int command = 0;

	do
	{
    	cout << "**** Simulate FIFO Queues by SQF *****\n";
    	cout << "* 0. Quit                            *\n";
    	cout << "* 1. Sort a file                     *\n";
    	cout << "* 2. Simulate one FIFO queue         *\n";
    	cout << "**************************************\n";
    	cout << "Input a command (0, 1, 2): ";
    	cin >> command;
		switch (command)
		{
			case 0: break;
			case 1:
			{
				string fileName;
        		cout << "Input a file number: ";
        		cin >> fileName;
        		jobList.dothing(fileName);

        		break;
			}

			default: cout << endl << "Command does not exist!" << endl;
		} // end switch
	} while (command != 0);

	system("pause");
	return 0;

} // end main
