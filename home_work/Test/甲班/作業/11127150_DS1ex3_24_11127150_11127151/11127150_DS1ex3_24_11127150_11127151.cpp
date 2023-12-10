// 11127150 �i�ͮ� , 11127151 ���֪N 
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
{ 	int OID;		// �u�@�s��
	int arrival;	// ��F�ɶ�
	int duration;	// ����ɶ� 
	int timeout;	// �W�ɮɶ�
}	jobType;
// The above are data structures globally available

class JobList {
	vector<jobType> aList; 		// �u�@�M��
	string	fileID;				// �ɮ��ѧO�X
	FILE	*infile = NULL;		// �ɮ׫��� 
	// The above are private data members

	// ��l�]�w
	void reset()
	{
    	aList.clear();  // �M�� aList
    	fileID = "";
	}


	// �H��F�ɶ��Ӷi��Ƨ�
	void sortByArrival(int number)
	{
    	number = aList.size(); // �}�C���j�p 

    	// �ϥ� Shell Sort 
    	for (int gap = number / 2; gap > 0; gap /= 2) // gap,�l�ǦC�����j 
    												  // ��l�Ƭ��}�C�j�p���@�b�A�C�����N�N���j��b 
    	{
    		// ��C�Ӥl�ǦC�i�洡�J�ƦC 
        	for (int i = gap; i < number; i += 1)
        	{
            	jobType temp = aList[i]; // �Ȧs��e���������

            	int j;
            	
            	// �b�l�ǦC���i�洡�J�ƧǡA��� arrival�M OID 
            	for (j = i; j >= gap && (aList[j - gap].arrival > temp.arrival || (aList[j - gap].arrival == temp.arrival && aList[j - gap].OID > temp.OID)); j -= gap)
            	{
                	aList[j] = aList[j - gap]; // ���ʤ����A����e�����˥X���J����m
            	}

            	aList[j] = temp; // �N�Ȧs���������J�쥿�T����m 
        	}
    	}
	}


	// �N�Ƨǫ᪺�u�@�M��g�^�ɮ�
	void putAll(string fileName)
	{
		fileName = "sorted" + fileID + ".txt"; 
	    ofstream outfile(fileName.c_str()); // �}�ҷs�ɮרѼg�J

		outfile << "OID" << '\t' << "Arrival" << '\t' << "Duration" << '\t'  << "TimeOut" << endl;


		// �g�J�C�Ӥu�@�� OID�Barrival�Bduration�Btimeout(�H tab���j ) 
    	for (int i = 0; i < aList.size(); i++)
    	{
        	outfile << aList[i].OID << "\t" << aList[i].arrival << "\t" << aList[i].duration << "\t" << aList[i].timeout << endl;
    	}

        outfile.close(); // �����ɮ� 
	}


	// ���Ū���B�ƧǡB�g�J�ɶ�
	void showTime(double read,double sort,double write)
	{
		cout << "Reading data: " << fixed  <<  setprecision(1)  <<  read  <<  " ms"  <<  endl;
		cout << "Sorting data: " << fixed  <<  setprecision(1)  <<  sort  <<  " ms"  <<  endl;
		cout << "Writing data: " << fixed  <<  setprecision(1)  <<  write  <<  " ms"  <<  endl;

	}


	// �d�ݤU�@�Ӥu�@
	void nextJobCheck(jobType &)
	{
		if (aList.size() > 0)
		{
			jobType nextJob = aList.front(); // ����Ĥ@�Ӥ��� ( = �}�C[0] )
		}
	}


public:
	JobList() { reset(); }		// constructor for initialization
	~JobList() { reset(); }		// destructor for initialization

	// �ˬd�u�@�M��O�_����
	bool isEmpty()
	{
		return aList.empty();
	}


	// ��ܾ�Ӥu�@�M�檺��T
	void showJob(vector<jobType> &list)
	{
    	cout << "OID" << '\t' << "Arrival" << '\t' << "Duration" << '\t'  << "TimeOut" << endl;

		// ��X�C�Ӥu�@���H��
    	for(int j = 0; j < list.size(); j++)
        	cout << list[j].OID << '\t' << list[j].arrival << '\t' << list[j].duration << '\t' << list[j].timeout << endl;
	}


	// �q�ɮפ�Ū���u�@�M��
	bool getAll(string fileName)
	{
    	fileID = "input" + fileName + ".txt";
    	ifstream infile(fileID.c_str()); // �HŪ���Ҧ����}�ɮ� 

    	if (!infile.is_open()) // �ɮפ��s�b 
    	{
        	cout << endl << fileID << " does not exist!" << endl;
        	return false;
    	}
    	
    	else
    	{
        	jobType job;

        	// ���L�Ĥ@��
        	infile.ignore(1000, '\n');  // �ϥΨ����j���Ʀr�T�O���L��� 

        	while (infile >> job.OID >> job.arrival >> job.duration >> job.timeout)
        	{
        		// ����ɶ��� 0�έt��(���X�z) 
				// ��F�ɶ� + ����ɶ� > �W�ɮɶ� (��ܤu�@�b�W�w�ɶ��������� ?!  ���X�z) 
            	if (job.duration <= 0 || (job.arrival + job.duration) > job.timeout)
                	return false;

            	aList.push_back(job); // �V aList ���K�[ 
        	}

        	infile.close();
        	return true;
    	}
	}


	// �q�ɮפ�Ū���u�@�M��öi��Ƨ�
	bool getSorted(string fileName)
	{
		// �}�l�p��
		clock_t start = clock();

    	// �p�G���\Ū���u�@�M��
		if(getAll(fileName)){
			
			// ����p�ɡA�p��Ū���Ҫ�ɶ�
			clock_t end = clock();
			fileID = fileName ;
			double read = end - start;
				
        	// �}�l�p��
			start = clock();
			// ��u�@�M��i��Ƨ�
			sortByArrival(aList.size());
			// ����p�ɡA�p��ƧǩҪ�ɶ�
			end = clock();
			double sort = end - start;

        	// �}�l�p��
			start = clock();
			// �N�Ƨǫ᪺�u�@�M��g�^�ɮ�
			putAll(fileID) ;
			// ����p�ɡA�p��ƧǩҪ�ɶ�
			end = clock();
			double write = end - start;

			// ��ܮɶ�
			showTime(read,sort,write);

			return true;
		}

		else
			return false;
	}


	// ���o�U�@�Ӥu�@�ñN��q�M�椤����
	void nextJob(jobType &nextJob)
	{
        nextJobCheck(nextJob) ;
        aList.erase(aList.begin()); // �����Ĥ@�Ӥu�@
	}


	// �Ω�int main ����ާ@ 
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
	int next(int idx) { /* �Цۦ槹���w�q */ } //move to the next position in queue
	// The above are private methods
public:
	int avail;			// the time when CPU is available 	
	// The above are public data members
	
	JobQueue():cA(NULL), qFront(0), qBack(0),
				qSize(0), qMax(0), avail(0) {}		// constructor of no-space queue
	JobQueue<T>(int maxS):qFront(0), qSize(0), avail(0)	// constructor of an empty queue
	{ /*�Цۦ槹���w�q */	}//end constructor
	int length() const  { /*�Цۦ槹���w�q */	}		// get the current queue length
	bool isEmpty() const { /*�Цۦ槹���w�q */}			// check whether it is empty
	bool isFull() const { /*�Цۦ槹���w�q */ } 		// check whether it is full
	void enQueue(T &);		// append a new element
	void getFront(T &);		// get the first element
	void deQueue();			// drop the first element
	void deQueue(T &);		// get & drop the first element
	void clearQ();			// clean up
	~JobQueue()				// destructor
	{/*�Цۦ槹���w�q */ }  //end destructor
}; //end JobQueue


class AnsList {
	typedef struct aT
	{ /*�Цۦ槹���w�q */} abortType;
	typedef struct dT
	{ /*�Цۦ槹���w�q */} doneType;
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
	{ /*�Цۦ槹���w�q */ }					// constructor for initialization
	~AnsList()								
	{ /*�Цۦ槹���w�q */ }					// destructor for initialization
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
	{ /* �Цۦ槹���w�q */} //end constructor
	~Simulation()	// destructor
	{ /*�Цۦ槹���w�q */}	//end Destructor
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
