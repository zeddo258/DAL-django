// 11127155 11127119
#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<time.h>
#include <iomanip>

#define MAX_LEN 255

using namespace std;

struct JobType {
	int oid;
	int arrival;
	int duration;
	int timeout;	
};

struct JobTypePtr {
	int oid;
	int arrival;
	int duration;
	int timeout;
	JobTypePtr *next;
		
};

class Type{
	public:
	string OID;  
	string Arrival;  
	string Duration;  
	string TimeOut;

};

class Category {
	public:
	Type types;
};

class JobList {
	private:
		vector<JobType> jobList;

	public:
		JobList() {
			jobList.resize(0);
		}
	
		bool isEmpty() {
			if(jobList.size()==0)
				return true;
			else
				return false;
		}
		
		bool getAll(string fileNumber) {  //取得資料 
			ifstream file;
			string inputALine;
			JobType job;
			int i, index;
			
			string fileName = "sorted" + fileNumber + ".txt";
			const char *name = fileName.c_str();
			file.open(name);                               
			if(!file.is_open())
				return false;
			
			getline(file, inputALine);
			while (getline(file, inputALine)) {
				index = inputALine.size(), i = 0;
				job = {0,0,0,0}; 
				while(inputALine[i] != '\t'){
					job.oid = job.oid*10 + inputALine[i] - 48;
					i++;
				}
				
				i++;
				while(inputALine[i] != '\t'){
					job.arrival = job.arrival*10 + inputALine[i] - '0';
					i++;
				}

				i++;
				while(inputALine[i] != '\t'){
					job.duration = job.duration*10 + inputALine[i] - '0';
					i++;
				}
				
				i++;
				while(inputALine[i] - '0' >= 0 && inputALine[i] - '0' <= 9){
					job.timeout = job.timeout*10 + inputALine[i] - '0';
					i++;
				}
				
				jobList.push_back(job);				
			}
        		
			file.close();
			return true;
		}
		
		
		bool nextJob(JobType &next) {
			if(jobList.size() > 0) {
				next.oid = jobList[0].oid;
				next.arrival = jobList[0].arrival;
				next.duration = jobList[0].duration;
				next.timeout = jobList[0].timeout;
				return false;
			}
			
			else
				return true;
			
		}
		
		void deleteJob() {
			jobList.erase(jobList.begin());
		}
		
};

class JobQueue {
	private:
		JobTypePtr *qFront, *qBack;
		int size;
		
	public:
		JobQueue(){
			qFront = NULL, qBack = NULL;
			size = 0;
		}
		
		int length() {
			return size;
		}
		
		bool isEmpty() {
			if(size == 0)
				return true;
			else 
				return false;
				
			
		}
		
		bool isFull() {
			if(size == 3)
				return true;
			else
				return false;
		}
		
		void enQueue(JobType jobType) {
			JobTypePtr *job = new JobTypePtr;
			job->next = NULL;
			
			job->arrival = jobType.arrival;
			job->oid = jobType.oid;
			job->duration = jobType.duration;
			job->timeout = jobType.timeout;
			
			if(isEmpty()){
				qFront = job;
				qBack = job;
			}
			
			else{
				JobTypePtr *ptr = qBack;
				qBack = job;
				ptr->next = qBack;
			}
			size++;
		}
		
		void deQueue(JobType &jobType) {
			jobType.oid = qFront->oid;
			jobType.arrival = qFront->arrival;
			jobType.duration = qFront->duration;
			jobType.timeout = qFront->timeout;
			
			JobTypePtr *ptr = qFront;
			qFront = qFront->next;
			delete ptr;
			size--;
		}
		
}; 

class AnsList {
	struct AbortType{
		int oid;
		int abort;
		int delay;
	};
	
	struct DoneType{
		int oid;
		int departure;
		int delay;
	};
	
	private:
		vector<AbortType> abortList;
		vector<DoneType> doneList;
		int allDelayTime;
		
	public:
		AnsList() {
			allDelayTime = 0;
		}
		
		void addAbortJob(int oid, int abort, int delay){	//add one abort job
			AbortType abortType;
			abortType.oid = oid;
			abortType.abort = abort;
			abortType.delay = delay;
			abortList.push_back(abortType);
			allDelayTime = allDelayTime + delay;
		}
		
		void addDoneJob(int oid, int departure, int delay){	//add one done job
			DoneType doneType;
			doneType.oid = oid;
			doneType.departure = departure;
			doneType.delay = delay;
			doneList.push_back(doneType);
			allDelayTime = allDelayTime + delay;
		}
		
		void putAll(string fileNumber) {		// write all as a file
      		string outputFileName = "output" + fileNumber + ".txt";
      		const char *name = outputFileName.c_str();
      		ofstream outFile(name);
      
         	outFile << "\t[Abort Jobs]" << endl;
         	outFile << "\tOID\tAbort\tDelay" << endl;
        	for (int i = 0; i < abortList.size(); ++i) 
             	outFile << "[" << i + 1 << "]\t" << abortList[i].oid << "\t" << abortList[i].abort << "\t" << abortList[i].delay << endl;
        
      		outFile << "\t[Jobs Done]" << endl;
      		outFile << "\tOID\tDeparture\tDelay" << endl;
      		for (int i = 0; i < doneList.size(); ++i) 
          		outFile << "[" << i + 1 << "]\t" << doneList[i].oid << "\t" << doneList[i].departure << "\t" << doneList[i].delay << endl;
        
      		float totalDelay = 0.0;
      		for (int i = 0; i < abortList.size(); ++i) 
          		totalDelay += abortList[i].delay;          
        	for (int i = 0; i < doneList.size(); ++i) 
             	totalDelay += doneList[i].delay;
        
         	float averageDelay = totalDelay / (abortList.size() + doneList.size());

        	outFile << "[Average Delay]\t" << fixed << setprecision(2) << averageDelay << " ms" << endl;

         	float successRate = (float)doneList.size() / (abortList.size() + doneList.size()) * 100.0;
         	outFile << "[Success Rate]\t" << fixed << setprecision(2) << successRate << " %" << endl;

         	outFile.close();
         	cout << "The simulation is running..." << endl;
         	cout << "See " <<  outputFileName << endl;
         	
         	abortList.clear();   
         	doneList.clear();
         	allDelayTime = 0;
    	}
};  

class SQFAnsList {
	struct AbortType{
		int oid;
		int cid;
		int abort;
		int delay;
	};
	
	struct DoneType{
		int oid;
		int cid;
		int departure;
		int delay;
	};
	
	private:
		vector<AbortType> abortList;
		vector<DoneType> doneList;
		int allDelayTime;
		
	public:
		SQFAnsList() {
			allDelayTime = 0;
		}
		
		void addAbortJob(int oid, int cid, int abort, int delay){	//add one abort job
			AbortType abortType;
			abortType.oid = oid;
			abortType.cid = cid;
			abortType.abort = abort;
			abortType.delay = delay;
			abortList.push_back(abortType);
			allDelayTime = allDelayTime + delay;
		}
		
		void addDoneJob(int oid, int cid, int departure, int delay){	//add one done job
			DoneType doneType;
			doneType.oid = oid;
			doneType.cid = cid;
			doneType.departure = departure;
			doneType.delay = delay;
			doneList.push_back(doneType);
			allDelayTime = allDelayTime + delay;
		}
		
		void putAll(string fileNumber) {		// write all as a file
      		string outputFileName = "double" + fileNumber + ".txt";
      		const char *name = outputFileName.c_str();
      		ofstream outFile(name);
      
         	outFile << "\t[Abort Jobs]" << endl;
         	outFile << "\tOID\tCID\tAbort\tDelay" << endl;
        	for (int i = 0; i < abortList.size(); ++i) 
             	outFile << "[" << i + 1 << "]\t" << abortList[i].oid << "\t" << abortList[i].cid << "\t" << abortList[i].abort << "\t" << abortList[i].delay << endl;
        
      		outFile << "\t[Jobs Done]" << endl;
      		outFile << "\tOID\tCID\tDeparture\tDelay" << endl;
      		for (int i = 0; i < doneList.size(); ++i) 
          		outFile << "[" << i + 1 << "]\t" << doneList[i].oid << "\t" << doneList[i].cid << "\t" << doneList[i].departure << "\t" << doneList[i].delay << endl;
        
      		float totalDelay = 0.0;
      		for (int i = 0; i < abortList.size(); ++i) 
          		totalDelay += abortList[i].delay;          
        	for (int i = 0; i < doneList.size(); ++i) 
             	totalDelay += doneList[i].delay;
        
         	float averageDelay = totalDelay / (abortList.size() + doneList.size());

        	outFile << "[Average Delay]\t" << fixed << setprecision(2) << averageDelay << " ms" << endl;

         	float successRate = (float)doneList.size() / (abortList.size() + doneList.size()) * 100.0;
         	outFile << "[Success Rate]\t" << fixed << setprecision(2) << successRate << " %" << endl;

         	outFile.close();
         	cout << "The simulation is running..." << endl;
         	cout << "See " <<  outputFileName << endl;
         	
         	abortList.clear();   
         	doneList.clear();
         	allDelayTime = 0;
    	}
};  

enum State { DONE, ABORT, PROCESSING };

class Cpu {
	private:
		int completeTime;
		bool process;
		JobType cpu;
		JobQueue queue;
		
		
	public:
		Cpu(){
			completeTime = 0;
			process = false;
		}
		
		bool processing() {  // CPU處理狀態 
			return process;
		}
		
		int queueLength() {  // queue長度 
			return queue.length();
		}
		
		void enCpu(JobType job, int time) {  // 把job放入CPU
			cpu = job;
			process = true;
			completeTime = time + cpu.duration;
		}
		
		void enQueue(JobType job) {  // 把job放入queue 
			queue.enQueue(job);
		}
		
		void deQueue(int time) {  // 把job從queue移入CPU 
			queue.deQueue(cpu);
			process = true;
			completeTime = time + cpu.duration;
		}
		
		State operation(JobType &job,int time) {  // 處理CPU內的job 
			if(completeTime == time) {
				job = cpu;
				process = false;
				return DONE;
			}
			
			else if(cpu.timeout <= time) {
				job = cpu;
				process = false;
				return ABORT; 
			}
			
			else
				return PROCESSING;
		}
};

class Simulation {
	private:
		JobList jobList;
		AnsList answers;
		SQFAnsList sqfAnswers;
		JobQueue queue;
		Cpu cpu1,cpu2;
		int time;
		
		 
	public:
		Simulation(){
			time = 0;
		}
		
		bool openfile(string fileNumber) {  // 開啟檔案 
			return jobList.getAll(fileNumber);
		}
		void FIFOProcessing(){  // 單核 
			bool processing = false;
			JobType cpu, nextJob;
			int completeTime;
			bool jobListIsEmpty = false;
			
			while(!jobList.isEmpty() || !queue.isEmpty() || processing) {	// 當jobList或queue內還有job 
				while(!processing){					// 當沒有正在處理的job 
					if(!queue.isEmpty()){				// 如果queue內還有job ->沒有正在處理的job 且佇列內有job 
						queue.deQueue(cpu);						// 將queue deQueue 並放入CPU處理 
						completeTime = time + cpu.duration;
						if(cpu.timeout <= time)					// 檢查從佇列取出是否TimeOut 
							answers.addAbortJob(cpu.oid, time, time-cpu.arrival);
						
						else{
							processing = true;
						}
		
					}
					
					else if(!jobList.isEmpty())	{		// 如果queue內無job且jobList內還有job ->沒有正在處理的JOB 且佇列為空 
						jobList.nextJob(cpu);					// 將下個job放入CPU處理 
						jobList.deleteJob();
						time = cpu.arrival;						// 把時間直接改為下個job的時間 
						completeTime = time + cpu.duration;
						processing = true;
					}
					
					else	// 無正在處理的job、jobList和queue內皆無job 
						return; 
				}
				
				if(time == completeTime) {		// 當job處理完成 
					answers.addDoneJob(cpu.oid, time, time - cpu.duration - cpu.arrival);
					processing = false;
				}
				
				else if(time == cpu.timeout) { 	// 執行中途發生逾時 
					answers.addAbortJob(cpu.oid, time, time - cpu.arrival);
					processing = false;
				}
				
				else if(!jobList.isEmpty()) { // 有正在執行的job 檢查下個job的抵達時間 
					jobList.nextJob(nextJob);
					while(nextJob.arrival == time && !jobListIsEmpty ) {	// 當nextJob的arrival為time 
						if(queue.isFull())										// 如果佇列滿了 
							answers.addAbortJob(nextJob.oid, time, 0);
						else													// 佇列還有空間則enQueue 
							queue.enQueue(nextJob);
						jobList.deleteJob();
						jobListIsEmpty = jobList.nextJob(nextJob);
					}
					
					time++;
				}
				
				else		// 有job正在處理 且jobList為空 
					time++;
			}

			time = 0;
		}
	
		void FIFOWriteAllAsAFile(string fileNumber) {	// 單核
			answers.putAll(fileNumber);
		}
		
		void SQFProcessing() {	// 多核 
			JobType nextJob, cpu1Job, cpu2Job;
			State cpu1State, cpu2State;

			while(!jobList.isEmpty() || cpu1.processing() || cpu2.processing()) {	// 當CPU還有job在處理或jobList內還有job
				if(cpu1.processing()) {  // (1)CPU處理 
					cpu1State = cpu1.operation(cpu1Job, time);
					if(cpu1State == DONE)	// CPU1處理完成 
						sqfAnswers.addDoneJob(cpu1Job.oid, 1, time, time - cpu1Job.duration - cpu1Job.arrival);
					
					else if(cpu1State == ABORT)  // CPU1處理中逾時 
						sqfAnswers.addAbortJob(cpu1Job.oid, 1, time, time - cpu1Job.arrival); 
				}
				
				if(cpu2.processing()) {	   
					cpu2State = cpu2.operation(cpu2Job, time);
					if(cpu2State == DONE)	// CPU2處理完成 
						sqfAnswers.addDoneJob(cpu2Job.oid, 2, time, time - cpu2Job.duration - cpu2Job.arrival);
					
					else if(cpu2State == ABORT) // CPU2處理中逾時 
						sqfAnswers.addAbortJob(cpu2Job.oid, 2, time, time - cpu2Job.arrival);
				}
			
				while(!cpu1.processing() && cpu1.queueLength() != 0 ) {  // (2)從佇列取出 
					cpu1.deQueue(time);
					if(cpu1.operation(cpu1Job, time) == ABORT)	// 從佇列中取出放入CPU1時逾時 
						sqfAnswers.addAbortJob(cpu1Job.oid, 1, time, time - cpu1Job.arrival);
				}
				
				while(!cpu2.processing() && cpu2.queueLength() != 0 ) { 
					cpu2.deQueue(time);
					if(cpu2.operation(cpu2Job, time) == ABORT)	// 從佇列中取出放入CPU2時逾時 
						sqfAnswers.addAbortJob(cpu2Job.oid, 2, time, time - cpu2Job.arrival);
				}
				
				if(!jobList.isEmpty()){  // (3)jobList處理 
					jobList.nextJob(nextJob);
					while(nextJob.arrival == time && !jobList.isEmpty()) {
						if(!cpu1.processing()) {  // CPU1沒有job在處理 
							cpu1.enCpu(nextJob, time);
						}
						
						else if(!cpu2.processing()) {  // CPU1處理中、CPU2沒有job在處理 
							cpu2.enCpu(nextJob, time);
						}
						
						else if(cpu1.queueLength() < 3 || cpu2.queueLength() < 3) {  // CPU1、CPU2的佇列仍有空間 
							if(cpu1.queueLength() <= cpu2.queueLength())	// CPU1的佇列空間相等或小於CPU2 
								cpu1.enQueue(nextJob);
							
							else 	// CPU2的佇列空間小於CPU1 
								cpu2.enQueue(nextJob);
						}
						
						else	// CPU1、CPU2佇列皆滿 ->取消 
							sqfAnswers.addAbortJob(nextJob.oid, 0, time, 0);
						 
						
						jobList.deleteJob();
						jobList.nextJob(nextJob);
					}
				}
				
				time++;
			}
				
		}
		
		void SQFWriteAllAsAFile(string fileNumber) {	// 單核
			sqfAnswers.putAll(fileNumber);
		}
};

void FIFOProcessing(string fileNumber);

void SQFProcessing(string fileNumber);

string sorting();

int main() {
    int command;
	string fileNumber;
    bool check = false;
    do{
        cout << "**** Simulate FIFO Queues by SQF *****" << endl;
        cout << "* 0. Quit                            *" << endl;
        cout << "* 1. Sort a file                     *" << endl;
        cout << "* 2. Simulate one FIFO queue         *" << endl;
        cout << "* 3. Simulate two queues by SQF      *" << endl;
        cout << "**************************************" << endl;
        cout << "Input a command(0, 1, 2, 3): ";

        cin >> command;
        cin.ignore(MAX_LEN, '\n');

        switch(command){
            case 0: break;
            case 1:	fileNumber = sorting(); 
            	check = true;
				break;
            case 2:
            	if (check) 
            		FIFOProcessing(fileNumber);
				else {
					cout << "Input a file number: ";
					getline(cin, fileNumber);
                	FIFOProcessing(fileNumber);
                	check = true;
				}
                break;
            case 3:
            	if(check)
            		SQFProcessing(fileNumber);
            	else {
            		cout << "Input a file number: ";
					getline(cin, fileNumber);
                	SQFProcessing(fileNumber);
                	check = true;
				}
				break;
            default:cout << endl << "Command does not exist!" << endl;
        }
    } while(command != 0);
    system("pause");
    return 0; 
}

void FIFOProcessing(string fileNumber) {  // 單核處理 
	Simulation simulation;
    if(!simulation.openfile(fileNumber)) {
    	cout << "### sorted" << fileNumber << ".txt does not exist! ###" << endl;
    	return;
	}
    simulation.FIFOProcessing();
    simulation.FIFOWriteAllAsAFile(fileNumber);
}

void SQFProcessing(string fileNumber) {	 // 多核處理 
	Simulation simulation;
    if(!simulation.openfile(fileNumber)) {
    	cout << "### sorted" << fileNumber << ".txt does not exist! ###" << endl;
    	return;
	}
	simulation.SQFProcessing();
	simulation.SQFWriteAllAsAFile(fileNumber);
}

string sorting() {
    int check = 0;
	double readbegin,readstop; 
	double sortbegin,sortstop; 
	double writebegin,writestop; 
	string filename;
    fstream file;
    string input = "\0" ;
	Category data;
    vector<Category> a;


   	while ( check == 0 ) {
		cout << "\n" << "Input a file number :";
    	cin >> filename;
    	input = "\0" ;
		input = input + "input";
		input = input + filename;
		input = input + ".txt";
		
		const char *name = input.c_str();
		file.open( name );
		
		if( file.is_open() == true ) {  
			string line;
    		string sort = "\0" ;
			sort = sort + "sorted";
			sort = sort + filename;
			sort = sort + ".txt";
			ofstream newFile;
			newFile.open( sort.c_str() );
			bool firstline = false;
			readbegin = clock();
			while ( getline( file, line ) ) {  // getline��
				int space = 0;
				if ( firstline == false ) {  // 第一行
					cout << "     " << line << "\n"; 
					newFile << line << "\n";
					firstline = true;
				}
				else {
					for( int i = 0 ; i < line.size() ; i++ ) {
						if( line[i] == '\t' )  space++;
						if( space == 0 && line[i] != '\t' )  data.types.OID = data.types.OID + line[i];
						if( space == 1 && line[i] != '\t' )  data.types.Arrival = data.types.Arrival + line[i];
						if( space == 2 && line[i] != '\t' )  data.types.Duration = data.types.Duration + line[i];
						if( space == 3 && line[i] != '\t' )  data.types.TimeOut = data.types.TimeOut + line[i];
					}
					a.push_back( data );
					data = {"\0"}; 
				} 
			} 
			readstop = clock();
			sortbegin = clock();
			int h = a.size()/2;
			for( int i = (a.size()/2) ; i > 0 ; i = i/2 ) {
				for ( int j = i ; j < a.size() ; j++ ) {
					
					string oo = a[j].types.OID;
					string aa = a[j].types.Arrival;
					string dd = a[j].types.Duration;
					string tt = a[j].types.TimeOut;
					int a2 = atoi(aa.c_str());
					int o2 = atoi(oo.c_str());
					int k = j; 
			
					while ( k >= i && (((atoi((a[k-i].types.Arrival).c_str())) > a2) || 
							(((atoi((a[k-i].types.Arrival).c_str())) == a2) && atoi((a[k-i].types.OID).c_str()) > o2)) ) {
						a[k] = a[k-i];
						k = k - i;
					}  // while
					a[k].types.OID = oo;
					a[k].types.Arrival = aa;
					a[k].types.Duration = dd;
					a[k].types.TimeOut = tt;
				}  // for
				
			}  // for
			sortstop = clock();
			
			for ( int i = 0 ; i < a.size() ; i++ ) {
				newFile << a[i].types.OID << "\t";
				newFile << a[i].types.Arrival << "\t";
				newFile << a[i].types.Duration << "\t";
				newFile << a[i].types.TimeOut << "\n";
			}  // for
			writebegin = clock();
			for ( int i = 0 ; i < a.size() ; i++ ) {
				cout << "("<< i+1 << ")  " << a[i].types.OID << "\t" << a[i].types.Arrival 
				     << "\t"<< a[i].types.Duration << "\t" << a[i].types.TimeOut << "\n";
			}  // for
			writestop = clock();
			cout << "reading data: " << readstop-readbegin<< "ms\n";
			cout << "sorting data: " << sortstop-sortbegin << "ms\n";
			cout << "writing data: " << writestop-writebegin << "ms\n";
			a.clear();
			newFile.close();
			file.close();
			check++;
		} 
		else {
			check = 1;
			cout << "\n" << "###" << input << " does not exist! ###" << "\n";	
		} 
		
		return filename;
	}
}  // Sorting()
