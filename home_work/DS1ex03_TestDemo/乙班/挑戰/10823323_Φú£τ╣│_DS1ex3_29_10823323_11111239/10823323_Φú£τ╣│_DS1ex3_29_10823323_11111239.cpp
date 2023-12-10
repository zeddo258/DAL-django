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
		int CID;
	} AbortType;
	typedef struct dT
	{
		int OID;
		int Departure;
		int Delay;
		int CID;
	} DoneType;

	vector<AbortType> abortJobs;
	vector<DoneType> doneJobs;
	float avgDelay;
	float successRate;

	void showAbortJobs(string no_use)
	{
		cout << "\t\t[Abort Jobs]" << endl;
		for (int i = 0; i < abortJobs.size(); i++)
		{
			cout << abortJobs[i].OID << " " << abortJobs[i].Abort << "  " << abortJobs[i].Delay << endl;
		}
	}
	void showDoneJobs(string no_use)
	{
		cout << "\t\t[Done Jobs]" << endl;
		for (int i = 0; i < doneJobs.size(); i++)
		{
			cout << doneJobs[i].OID << " " << doneJobs[i].Departure << "  " << doneJobs[i].Delay << endl;
		}
	}
	void computeStat(void)
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
	void showStat()
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

	void showAll(string no_use)
	{

		showAbortJobs(no_use);
		showDoneJobs(no_use);
		showStat();
	}
	void addAbortJob(int inOID, int CID , int inAbort, int inDelay)
	{
		AbortType tmp;
		tmp.OID = inOID;
		tmp.Abort = inAbort;
		tmp.Delay = inDelay;
		tmp.CID = CID;
		abortJobs.push_back(tmp);
	}
	void addDoneJob(int inOID, int CID ,  int inDeparture, int inDelay)
	{
		DoneType tmp;
		tmp.OID = inOID;
		tmp.Departure = inDeparture;
		tmp.Delay = inDelay;
		tmp.CID = CID;
		doneJobs.push_back(tmp);
	}
	void putAll(string file_name, int qnum)
	{
		computeStat();
		string writetxt = "";
		if (qnum == 1)
			writetxt = "output" + file_name + ".txt";
		else if (qnum == 2)
			writetxt = "double" + file_name + ".txt";
		ofstream file(writetxt); // ���}�ɮץH�i��g�J

		if (file.is_open())
		{
			if (qnum == 1)
			{
				file << "\t[Abort Jobs]\n"; // �g�J���Ѫ�
				file << "\tOID\tAbort\tDelay\n"; // �g�J���Ѫ�

				int i = 1;
				for (const auto& process : abortJobs)
				{
					file << "[" << i << "]" << "\t" << process.OID << "\t" << process.Abort << "\t\t" << process.Delay << "\n";
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
				file << "[Average Delay] " << avgDelay << "ms" << "\n"; // [Average Delay]
				file << "[Success Rate] " << successRate << "%" << "\n"; // [Success Rate]

				file.close(); // �����ɮ�
			}
			else if (qnum == 2)
			{
				file << "\t[Abort Jobs]\n"; // �g�J���Ѫ�
				file << "\tOID\tCID\tAbort\tDelay\n"; // �g�J���Ѫ�

				int i = 1;
				for (const auto& process : abortJobs)
				{
					file << "[" << i << "]" << "\t" << process.OID << "\t" <<process.CID <<"\t" << process.Abort << "\t\t" << process.Delay << "\n";
					i++;
				}

				file << "\t[Jobs Done]\n"; // �g�J���\��
				file << "\tOID\tCID\tAbort\tDelay\n"; //�g�J���\��
				i = 1;
				for (const auto& process : doneJobs)
				{
					file << "[" << i << "]" << "\t" << process.OID << "\t"<<process.CID<<"\t" << process.Departure << "\t\t" << process.Delay << "\n";
					i++;
				}
				file << "[Average Delay] " << avgDelay << "ms" << "\n"; // [Average Delay]
				file << "[Success Rate] " << successRate << "%" << "\n"; // [Success Rate]

				file.close(); // �����ɮ�
			}



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
			cout << "(" << i++ << ") \t" << process.OID << "\t" << process.arrival << "\t\t" << process.duration << "\t\t" << process.timeout << "\n";
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
	int state[2]; //computer state
	int real_time;

	typedef struct job_time_information
	{
		int OID;
		int arrival_time;
		int start_time;
		int end_time;
		int timeout;
	}job_time_informationT;

	job_time_informationT job_information[2];

	void delQ(int);
	void updateQ(int, int);
	void finishQ();
	jobType inputJob, workingJob;

	void check_CPU_Job(JobQueue<jobType> *cpu_queue, int cpu_number)
	{
		while (true)	//�@���ˬd ����queue�S���ȩάO�w�g�b�u�@��
		{
			if (state[cpu_number] == 0)	//�ثe���m�A�n��queue���L����
			{
				if (!cpu_queue->IsEmpty())
				{
					cpu_queue->deQueue(workingJob);	//���X����
					job_information[cpu_number].OID = workingJob.OID;
					job_information[cpu_number].start_time = real_time; //����O���}�l�u�@�A�ӥB�����|�OQueue���Ĥ@�Ӥu�@
					job_information[cpu_number].end_time = job_information[cpu_number].start_time + workingJob.duration;
					job_information[cpu_number].timeout = workingJob.timeout;
					job_information[cpu_number].arrival_time = workingJob.arrival;
					state[cpu_number] = 1;
					//�q��C���X �N�i�H���ˬd �O�_time out
					if (job_information[cpu_number].timeout <= real_time)	//�@���X�ӴNtime_out
					{
						//cout << "���X�N����" << "  OID = " << job_information[cpu_number].OID << "  CID =  " << cpu_number + 1 << "Abort = " << real_time << " Delay = " << real_time - job_information[cpu_number].arrival_time << endl;
						answers.addAbortJob(job_information[cpu_number].OID, cpu_number + 1, real_time, real_time - job_information[cpu_number].arrival_time);
						state[cpu_number] = 0;
					}
					continue;
				}
				else
					break;
			}
			else
			{
				if (job_information[cpu_number].end_time <= real_time)	//���\����
				{
					//�����N�����ȥ�즨�\��
					//cout << "���\ " << "  OID = " << job_information[cpu_number].OID << "  CID =  " << cpu_number + 1 << "Departure  " << job_information[cpu_number].end_time << "Delay = " << job_information[cpu_number].start_time - job_information[cpu_number].arrival_time << endl;
					answers.addDoneJob(job_information[cpu_number].OID, cpu_number + 1, job_information[cpu_number].end_time, job_information[cpu_number].start_time - job_information[cpu_number].arrival_time);
					state[cpu_number] = 0;
				}
				else if (job_information[cpu_number].timeout <= real_time)	//��ܮɶ���F ���楢��
				{
					//cout << "�ɶ���F���� " << "  OID = " << job_information[cpu_number].OID << "  CID =  " << cpu_number + 1 << "Abort = " << job_information[cpu_number].timeout << " Delay = " << real_time - job_information[cpu_number].arrival_time << endl;
					answers.addAbortJob(job_information[cpu_number].OID, cpu_number + 1, job_information[cpu_number].timeout, real_time - job_information[cpu_number].arrival_time);
					state[cpu_number] = 0;
				}
				else
					break;
			}

		}

		return;
	}
public:
	Simulation(JobList aList, int N) : jobs(aList), qNum(N), real_time(0)
	{
		state[0] = 0;
		state[1] = 0;
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
		int check_nextflag = 1;
		int last_job = 0;

		while (!jobs.IsEmpty() || last_job) //�٦��u�@
		{
			if (check_nextflag == 1)
			{
				check_nextflag = 0;
				jobs.nextJob(inputJob);	//list���X�u�@
			}

			//---------------���P�_�u�@�������p---------------//
		check:
			check_CPU_Job(allQ[0], 0);
			if (qNum == 2)check_CPU_Job(allQ[1], 1);

			//------------------------------------------------//
			if (real_time == inputJob.arrival)
			{
				last_job = 0;
				while (true)
				{
					//�ݨM�w����@��queue
					if (qNum == 1)
					{
						if (!allQ[0]->IsFull())	//Queue�٦���m ,��J����
						{
							allQ[0]->enQueue(inputJob);
						}
						else
						{
							//cout << "�񤣶i�h���� " << inputJob.OID << " " << inputJob.arrival << " " << "ans = " << 0 << endl;
							answers.addAbortJob(inputJob.OID, 1 , inputJob.arrival, 0);

						}
					}
					else if (qNum == 2)
					{
						int work_count = state[0] + state[1];
						if (work_count == 1 && (state[0] == 0 && allQ[0]->IsEmpty() || state[1] == 0 && allQ[1]->IsEmpty()))	//�u���@�ӪŶ� �B���Ӧ�C����   case1
						{
							if (state[0] == 0 && allQ[0]->IsEmpty())	//0�O�Ŷ��B��
							{
								allQ[0]->enQueue(inputJob);
							}
							else  // 1�O�Ŷ��B��
							{
								allQ[1]->enQueue(inputJob);
							}
						}
						else if (work_count == 0)	//��ӳ��Ŷ�  �D�p����	//case2
						{
							if (allQ[0]->IsEmpty())	// ���`�ӻ��Ŷ��@�w�O��
							{
								allQ[0]->enQueue(inputJob);
							}
						}
						else if (work_count == 2 && (!allQ[0]->IsFull() || !allQ[1]->IsFull()))	//���ܦ�  �����H��C�S��	case3
						{
							if (allQ[0]->length() == allQ[1]->length())	//�@�˪� �ˤp��
							{
								allQ[0]->enQueue(inputJob);
							}
							else
							{
								if (allQ[0]->length() < allQ[1]->length())	allQ[0]->enQueue(inputJob);
								else	allQ[1]->enQueue(inputJob);
							}
						}
						else   //case4  �S�H���
						{
							//cout << "�񤣶i�h���� " << "OID = " << inputJob.OID << " " << inputJob.arrival << " " << allQ[0]->avail << "ans = " << 0 << endl;
							answers.addAbortJob(inputJob.OID, 0 , real_time, real_time - inputJob.arrival);
						}
					}

					//�˧����ȭn���W�P�_
					check_CPU_Job(allQ[0], 0);
					if (qNum == 2)check_CPU_Job(allQ[1], 1);

					if (!jobs.IsEmpty())
					{
						jobs.nextJob(inputJob);	//list���X�u�@
					}

					else break;

					if (real_time != inputJob.arrival)
					{
						if (jobs.IsEmpty())
							last_job = 1;
						break;
					}

				}

			}
			real_time++;


		}
		//--�u�@�C��S�u�@�F�A�n�ˬd��C�̪�
		{
			while (true)
			{
				if (qNum == 1)
				{
					if (allQ[0]->IsEmpty())
					{
						if (state[0] == 0) break;
					}
					check_CPU_Job(allQ[0], 0);
					real_time++;
				}
				else if (qNum == 2)
				{
					if (allQ[0]->IsEmpty() && allQ[1]->IsEmpty())
					{
						if (state[0] == 0 && state[1] == 0) break;
					}
					check_CPU_Job(allQ[0], 0);
					check_CPU_Job(allQ[1], 1);
					real_time++;
				}

			}

		}
		answers.putAll(file_name, qNum);

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
		cout << endl << "3. Simulate Two FIFO queue";
		cout << endl << "*********************************";
		cout << endl << "Input a choice (0 , 1 , 2 , 3):";

		cin >> command;
		cin.ignore(255, '\n');
		if (command == 0)
			break;
		else if (command == 1)
		{
			cout << endl << "Input a file number : ";
			cin >> file_name;
			bool result = job.getAll(file_name);
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
		else if (command == 2 || command == 3)
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

			Simulation computer(job, command - 1);
			computer.SQF();
			cout << "The simulation is running..." << endl;
			if (command == 2)
				cout << "see output" << file_name << ".txt" << endl;
			if (command == 3)
				cout << "see double" << file_name << ".txt" << endl;
		}
		else
		{
			cout << "comman does not exist" << endl;
		}
	}


	return 0;
}
