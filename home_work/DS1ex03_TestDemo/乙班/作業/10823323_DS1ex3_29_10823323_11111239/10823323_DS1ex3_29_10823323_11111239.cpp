//��29�� 10823323���� 11111239���_�� 
#include <iostream> 
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
using namespace std;

string file_name = "";
// �w�q��ص��c��A�Ω�s�x���G
typedef struct jT
{
	int OID;
	int arrival;
	int duration;
	int timeout;
} jobType;



class AnsList
{
	typedef struct aT
	{
		int OID;
		int Abort;
		int Delay;
	} AbortType;
	typedef struct dT
	{
		int OID;
		int Departure;
		int Delay;
	} DoneType;
// �s�x���ѡ]Abort�^�M���\�]Done�^���u�@�A�H�ά����έp�ƾ�
	vector<AbortType> abortJobs;
	vector<DoneType> doneJobs;
	float avgDelay;
	float successRate;

	void showAbortJobs(string no_use)// ��ܥ��Ѫ��u�@
	{
		cout << "\t\t[Abort Jobs]" << endl;
		for (int i = 0; i < abortJobs.size(); i++)
		{
			cout << abortJobs[i].OID << " " << abortJobs[i].Abort << "  " << abortJobs[i].Delay << endl;
		}
	}
	void showDoneJobs(string no_use) // ��ܦ��\���u�@
	{
		cout << "\t\t[Done Jobs]" << endl;
		for (int i = 0; i < doneJobs.size(); i++)
		{
			cout << doneJobs[i].OID << " " << doneJobs[i].Departure << "  " << doneJobs[i].Delay << endl;
		}
	}
	void computeStat(void)// �p��έp�ƾ�
	{
		successRate = (float)doneJobs.size() / (doneJobs.size() + abortJobs.size()) * 100;
		int delay_total = 0;
		for (int i = 0; i < doneJobs.size(); i++)
		{
			delay_total += doneJobs[i].Delay;
		}
		for (int i = 0; i < abortJobs.size(); i++)
		{
			delay_total += abortJobs[i].Delay;
		}
		avgDelay = (float)delay_total / (doneJobs.size() + abortJobs.size());
	}
	void showStat()// ��ܲέp�ƾ�
	{
		computeStat();
		cout << "rate " << successRate << "%" << endl;
		cout << "delay " << avgDelay << endl;
	}
public:
	AnsList() : avgDelay(0.0), successRate(0.0)
	{

	}
	~AnsList()
	{}

	void showAll(string no_use)// ��ܩҦ����G
	{

		showAbortJobs(no_use);
		showDoneJobs(no_use);
		showStat();
	}
	void addAbortJob(int inOID, int inAbort, int inDelay)// �K�[�@�ӥ��Ѫ��u�@
	{
		AbortType tmp;
		tmp.OID = inOID;
		tmp.Abort = inAbort;
		tmp.Delay = inDelay;
		abortJobs.push_back(tmp);
	}
	void addDoneJob(int inOID, int inDeparture, int inDelay)// �K�[�@�Ӧ��\���u�@
	{
		DoneType tmp;
		tmp.OID = inOID;
		tmp.Departure = inDeparture;
		tmp.Delay = inDelay;
		doneJobs.push_back(tmp);
	}
	void putAll(string file_name, string no_use)// �N�Ҧ����G�g�J�ɮ�
	{
		computeStat();
		string writetxt = "output" + file_name + ".txt";
		ofstream file(writetxt); // ���}�ɮץH�i��g�J

		if (file.is_open())
		{
			file << "\t[Abort Jobs]\n"; // �g�J���Ѫ�
			file << "\tOID\tAbort\tDelay\n"; // �g�J���Ѫ�

			int i = 1;
			for (const auto& process : abortJobs)
			{
				file <<"["<<i<<"]"<<"\t"<<process.OID << "\t" << process.Abort << "\t\t" << process.Delay << "\n";
				i++;
			}

			file << "\t[Jobs Done]\n"; // �g�J���\��
			file << "\tOID\tAbort\tDelay\n"; //�g�J���\��
			i = 1;
			for (const auto& process : doneJobs)
			{
				file << "[" << i << "]" << "\t" << process.OID << "\t" << process.Departure << "\t\t" << process.Delay << "\n";
				i++;
			}
			file << "[Average Delay] "<< avgDelay <<"ms"<< "\n"; // [Average Delay]
			file << "[Success Rate] " << successRate <<"%"<<"\n"; // [Success Rate]

			file.close(); // �����ɮ�


		}

	}
};
class JobList
{
	vector<jobType> aList; //list of jobs with four columns
	string  fileID;	//file ID

	chrono::steady_clock::time_point time;
	chrono::milliseconds read_time;
	chrono::milliseconds sort_time;
	chrono::milliseconds write_time;

	void reset()
	{
		vector<jobType>().swap(aList);	//�M��vector and recycle memory
		fileID = "";
	}
	void showWholeJob()
	{
		cout << "\tOID\tArrivalTime\tDurationTime\tTimeout\n";
		int i = 1;
		for (const auto& process : aList)
		{
			cout <<"("<<i++<<") \t"<< process.OID << "\t" << process.arrival << "\t\t" << process.duration << "\t\t" << process.timeout << "\n";
		}
	}
	void showJob()
	{
		cout << aList[0].OID << " " << aList[0].arrival << " " << aList[0].duration << " " << aList[0].timeout << endl;
	}
	void sortByArrival()
	{
		time = chrono::steady_clock::now();
		int n = aList.size();
		for (int gap = n / 2; gap > 0; gap /= 2)
		{
			for (int i = gap; i < n; ++i)
			{
				jobType temp = aList[i];
				int j;
				for (j = i; j >= gap && (aList[j - gap].arrival > temp.arrival || (aList[j - gap].arrival == temp.arrival && aList[j - gap].OID > temp.OID)); j -= gap)
				{
					aList[j] = aList[j - gap];
				}
				aList[j] = temp;
			}
		}
		sort_time = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - time);
	}
	bool putAll()
	{
		time = chrono::steady_clock::now();
		string writetxt = "sorted" + fileID + ".txt";
		ofstream file(writetxt); // ���}�ɮץH�i��g�J

		if (file.is_open())
		{

			file << "OID\tArrivalTime\tDurationTime\tTimeout\n"; // �g�J���D

			for (const auto& process : aList) {
				file << process.OID << "\t" << process.arrival << "\t\t" << process.duration << "\t\t" << process.timeout << "\n";
			}

			file.close(); // �����ɮ�
			write_time = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - time);
			return true;
		}
		else
		{
			return false;
		}
	}
	void showTime()
	{
		cout << "Reading data: " << chrono::duration <double, std::milli>(read_time).count() << " ms\n";
		cout << "Sorting data: " << chrono::duration <double, std::milli>(sort_time).count() << " ms\n";
		cout << "Writing data: " << chrono::duration <double, std::milli>(write_time).count() << " ms\n";

	}
	void nextJobCheck(jobType& job)
	{
		job = aList[0];
	}

public:

	JobList() { reset(); }
	~JobList() { reset(); }

	bool IsEmpty()
	{
		return aList.empty();
	}
	bool getAll(string filename)
	{
		time = chrono::steady_clock::now();
		fileID = filename;
		string readtxt = "input" + fileID + ".txt";
		ifstream file(readtxt);
		string line;
		if (!file.is_open())
		{
			return false;
		}
		while (getline(file, line)) {
			istringstream iss(line);
			jobType process;
			if (iss >> process.OID >> process.arrival >> process.duration >> process.timeout) {
				aList.push_back(process);
			}
		}

		file.close();
		read_time = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - time);
		return true;
	}
	bool getSorted()
	{
		sortByArrival();
		showWholeJob();
		showTime();
		bool result = putAll();
		return result;
	}
	bool getSorted(string file_name)
	{
		//---�T�{�ɮצs�b

		string readtxt = "sorted" + file_name + ".txt";
		ifstream file(readtxt);
		if (!file.is_open())
		{
			return false;
		}
		else
		{
			vector<jobType> processes;
			string line;
			while (getline(file, line)) {
				istringstream iss(line);
				jobType process;
				if (iss >> process.OID >> process.arrival >> process.duration >> process.timeout) {
					aList.push_back(process);
				}
			}
			file.close();
		}

		return true;
	}
	void nextJob(jobType& job)
	{
		nextJobCheck(job);
		aList.erase(aList.begin());
	}

};

template<typename T>
class JobQueue
{
	T *cA; //circular array
	int qFront, qBack; //head & tail of queue
	int qSize, qMax; //current size & max size of queue

	int next(int idx)	//move to the next position in queue
	{
		//TODO
	}


public:
	int avail; //the time when CPU is available
	JobQueue<T>() : qFront(0), qSize(0), avail(0), qBack(0), qMax(0)
	{

	}
	JobQueue<T>(int maxS) : qFront(0), qSize(0), avail(0), qBack(0)
	{
		cA = new T[maxS];
		qMax = maxS;
	}
	int length() const { return qSize; }	//get the current queue length
	bool IsEmpty() const { return qSize == 0 ? 1 : 0; }
	bool IsFull() const { return qSize == qMax ? 1 : 0; }

	void enQueue(T& NewJob)	//append a new element
	{
		if (qSize != qMax)
		{
			cA[qBack] = NewJob;
			qBack = (qBack + 1) % qMax;
			qSize++;
		}
	}
	void getFront(T& job)	//get the first element
	{
		job = cA[qFront];
	}
	void deQueue()	//drop the first element
	{
		if (qSize > 0)
		{
			qFront = (qFront + 1) % qMax;
			qSize--;
		}

	}
	void deQueue(T& job)	//get and drop the first element
	{
		if (qSize > 0)	//queue is not empty
		{
			job = cA[qFront];
			qFront = (qFront + 1) % qMax;
			qSize--;
		}

	}
	void clearQ()	//clean up
	{
		qFront = 0;
		qBack = 0;
		qSize = 0;
	}
	void show_queue()
	{
		for (int i = 0; i < qSize; i++)
		{
			cout << i << cA[qFront + i] << "    ";
		}
		cout << endl;
	}

	~JobQueue()
	{
		delete[] cA;
	}
};

class Simulation
{
	JobList jobs;
	AnsList answers;
	JobQueue<jobType> **allQ;
	int qNum;
	int state; //computer state

	void delQ(int);
	void updateQ(int, int);
	void finishQ();
public:
	Simulation(JobList aList, int N) : jobs(aList), qNum(N), state(0)
	{
		jobs = aList;
		allQ = new JobQueue<jobType>*[N];	//���ЦC�@�ӳ��O�˫��Ъ��x�}
		for (int i = 0; i < N; i++)
		{
			allQ[i] = new JobQueue<jobType>(3); //�N�C�@�Ӥ�����new�@�ӰO����
		}
	}
	~Simulation()
	{
		for (int i = 0; i < qNum; i++)
		{
			delete allQ[i]; //�N�C�@�Ӥ������O����
		}
		delete allQ;
	}

	void SQF()
	{

		jobType inputJob, workingJob;
		int start_time = 0, end_time = 0, timeout = 0;
		int check_nextflag = 1 , the_last_job = 0;

		while (!jobs.IsEmpty() || the_last_job) //�٦��u�@
		{
			if (check_nextflag == 1 && the_last_job == 0)
			{
				check_nextflag = 0;
				jobs.nextJob(inputJob);	//list���X�u�@
			}

			//---------------���P�_�u�@�������p---------------//
		check:
			if (state == 1)
			{
				if (end_time <= allQ[0]->avail)	//��ܦ��\����
				{
					//�����N�����ȥ�즨�\��
					//cout << "���\ " << workingJob.OID << "  " << end_time << "  " << allQ[0]->avail - workingJob.arrival - workingJob.duration <<endl;
					answers.addDoneJob(workingJob.OID, end_time, allQ[0]->avail - workingJob.arrival - workingJob.duration);

					state = 0;
				}
				else if (timeout <= allQ[0]->avail)	//��ܮɶ���F ���楢��
				{
					//�N�����ȥ�쥢�Ѱ�
					//cout << "�O�ɥ��� " << workingJob.OID <<"  " << timeout <<"  "<< allQ[0]->avail - workingJob.arrival << endl;
					answers.addAbortJob(workingJob.OID, timeout, allQ[0]->avail - workingJob.arrival);

					state = 0;
				}
			}
			if (state == 0)	//�i��o�������Ŷ� �ҥH�n�A�P�_�@��
			{
				//�Nqueue�̪����Ȯ��X�ö}�l�u�@
				if (!allQ[0]->IsEmpty())
				{
					allQ[0]->deQueue(workingJob);	//���X����
					start_time = allQ[0]->avail; //����O���}�l�u�@�A�ӥB�����|�OQueue���Ĥ@�Ӥu�@
					end_time = start_time + workingJob.duration;
					timeout = workingJob.timeout;
					state = 1;
					if (allQ[0]->avail > timeout) //�@���X�ӴN�d�@
					{
						//cout << "�O�ɥ��� " << workingJob.OID << " " << allQ[0]->avail << "  " << allQ[0]->avail - workingJob.arrival << endl;
						answers.addAbortJob(workingJob.OID, allQ[0]->avail, allQ[0]->avail - workingJob.arrival);
						state = 0;
					}
					goto check;
				}
			}
			//------------------------------------------------//

			if (allQ[0]->avail == inputJob.arrival)
			{
				if (the_last_job == 1) the_last_job = 0;
				//--���ȨӤF  �P�_�ण��˶i�h
				while (true)	//����U�ӥ��Ȥ��O�o�ɶ�����
				{
					if (!allQ[0]->IsFull())	//Queue�٦���m ,��J����
					{
						allQ[0]->enQueue(inputJob);
					}
					else
					{
						//cout << "�񤣶i�h���� " << inputJob.OID << " " << inputJob.arrival << " " << "ans = " << 0 << endl;
						answers.addAbortJob(inputJob.OID, inputJob.arrival, 0);

					}
					if (state == 0)	//�i��o�������Ŷ� �ҥH�n�A�P�_�@��
					{
						//�Nqueue�̪����Ȯ��X�ö}�l�u�@
						if (!allQ[0]->IsEmpty())
						{
							allQ[0]->deQueue(workingJob);	//���X����
							start_time = allQ[0]->avail; //����O���}�l�u�@�A�ӥB�����|�OQueue���Ĥ@�Ӥu�@
							end_time = start_time + workingJob.duration;
							timeout = workingJob.timeout;
							state = 1;

							//--���W�P�_
							if (end_time <= allQ[0]->avail)	//��ܦ��\����
							{
								//�����N�����ȥ�즨�\��
								//cout << "���\ " << workingJob.OID << "  " << end_time << "  " << allQ[0]->avail - workingJob.arrival - workingJob.duration <<endl;
								answers.addDoneJob(workingJob.OID, end_time, allQ[0]->avail - workingJob.arrival - workingJob.duration);
								state = 0;
							}
							else if (timeout <= allQ[0]->avail)	//��ܮɶ���F ���楢��
							{
								//�N�����ȥ�쥢�Ѱ�
								//cout << "�ɶ���F���� " << workingJob.OID << " " << allQ[0]->avail << "ans " << allQ[0]->avail - workingJob.arrival << endl;
								answers.addAbortJob(workingJob.OID, allQ[0]->avail, allQ[0]->avail - workingJob.arrival);
								state = 0;
							}
						}
					}
					if (!jobs.IsEmpty())
					{
						jobs.nextJob(inputJob);	//list���X�u�@
					}
					else break;

					if (allQ[0]->avail != inputJob.arrival)
					{
						if(jobs.IsEmpty()) the_last_job = 1;
						break;
					}

				}

			}
			allQ[0]->avail++;

		}

		//--list�̪��u�@�]����A�n�h����ݭ쥻��C�̪��u�@
		{

			//--�Ĥ@�� ���ݷ�e���u�@
			if (state == 1)	//�u�@��
			{
				if (timeout < end_time)	//��S�ϤF ������쥢�Ѱ�
				{
					//cout<< "�O�ɥ��� " << workingJob.OID << " "<< workingJob.timeout << "  " << allQ[0]->avail - workingJob.arrival << endl;
					answers.addAbortJob(workingJob.OID, workingJob.timeout, allQ[0]->avail - workingJob.arrival);
					allQ[0]->avail = timeout;
					state = 0;
				}
				else
				{
					allQ[0]->avail = end_time;
					//cout << "���\ " << workingJob.OID << "  " << end_time << "  " << allQ[0]->avail - workingJob.arrival - workingJob.duration <<endl;
					answers.addDoneJob(workingJob.OID, end_time, allQ[0]->avail - workingJob.arrival - workingJob.duration);
					state = 0;
				}


			}
			//---�ݦ�C�̪�

			while (!allQ[0]->IsEmpty())
			{
				allQ[0]->deQueue(workingJob);	//���X����
				start_time = allQ[0]->avail; //����O���}�l�u�@�A�ӥB�����|�OQueue���Ĥ@�Ӥu�@
				end_time = start_time + workingJob.duration;
				timeout = workingJob.timeout;

				if (allQ[0]->avail > timeout) //�@���X�ӴN�d�@
				{
					//cout << "�ɶ���F���� " << workingJob.OID << " " << allQ[0]->avail << "  " << allQ[0]->avail - workingJob.arrival << endl;
					answers.addAbortJob(workingJob.OID, allQ[0]->avail, allQ[0]->avail - workingJob.arrival);
				}
				else if (timeout < end_time) //��S�ϤF ������쥢�Ѱ�
				{
					allQ[0]->avail = timeout;
					//cout << "�O�ɥ��� " << workingJob.OID << " "<< workingJob.timeout << "  " << timeout - workingJob.arrival << endl;
					answers.addAbortJob(workingJob.OID, workingJob.timeout, timeout - workingJob.arrival);
				}
				else
				{
					//cout << "���\ " << workingJob.OID << "  " << end_time << "  " << allQ[0]->avail - workingJob.arrival - workingJob.duration<<endl;
					answers.addDoneJob(workingJob.OID, end_time, allQ[0]->avail - workingJob.arrival - workingJob.duration);
					allQ[0]->avail = end_time;
					state = 0;
				}
			}
		}

		//cout << "---------" << endl;
		//answers.showAll("a");
		answers.putAll(file_name, "a");

	}

};


int main()
{

	while (true)
	{
		JobList job;
		int command = -1;
		cout << endl << "****Simulate FIFO Queues by SQF*****";
		cout << endl << "0. Quit    ";
		cout << endl << "1. Sort a file";
		cout << endl << "2. Simulate one FIFO queue";
		cout << endl << "*********************************";
		cout << endl << "Input a choice (0 , 1 , 2):";

		cin >> command;
		cin.ignore(255, '\n');
		if (command == 0)
			break;
		else if (command == 1)
		{
			cout << endl << "Input a file number : ";
			cin >> file_name;
			bool result =  job.getAll(file_name);
			if (result == false)
			{
				cout << "###input" << file_name << ".txt does not exist!!" << endl;
				continue;
			}
			else
			{
				job.getSorted();
				cout << "see sorted" << file_name << ".txt" << endl;
			}
		}
		else if (command == 2)
		{
			if (file_name == "")
			{
				cout << endl << "Input a file number : ";
				cin >> file_name;
			}
			//---�T�{�ɮצs�b
			bool result = job.getSorted(file_name);
			if (result == false)
			{
				cout << "### sorted" << file_name << ".txt does not exist!###" << endl;
				continue;
			}

			Simulation computer(job, 1);
			computer.SQF();
			cout << "The simulation is running..." << endl;
			cout << "see output" << file_name << ".txt" << endl;
		}
		else
		{
			cout << "comman does not exist" << endl;
		}
	}


	JobList job;
	job.getAll("303");
	job.getSorted();


	Simulation computer(job, 1);
	computer.SQF();

	system("pause");
	return 0;
}
