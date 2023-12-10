// 11127256 蕭天語, 11127257 胡加恩 
#include <iostream>		// cout, endl
#include <fstream> 		// open, is_open
#include <string> 		// string
#include <vector>		//vector
#include <cstdlib>		// atoi, system
#include <iomanip>		// setw, setprecision
#include <ctime>		// clock, CLOCKS_PER_SEC
#include <sstream>		// change to string
#include <math.h>       // decimal change 
using namespace std;

string its(int number) {  // change int to string
    stringstream s;
    s << number;
    return s.str();
}

string fts(float number) {  // change float to string
    stringstream s;
    s << number;
    return s.str();
}

typedef struct jT
{ 	int OID;		//order identifier
	int arrival;		// arrival time
	int duration;	// job duration
	int timeout;	// expire time
}	jobType;
// The above are data structures globally available
class JobList {
	vector<jobType> aList; 		// list of jobs with four columns
	string	fileID;				// file identifier
	// The above are private data members
	void reset()					// definition: initial set up
	{ 			
	    vector<jobType>().swap(aList);		
		fileID = "";
	} //end reset 
	
	void showJob(vector<jobType> & list) {		// declaration: display a job on screen
		cout<<"OID"<<'\t'<<"Arrival"<<'\t'<< "Duration"<<'\t'<<"TimeOut"<<endl;
		for(int j = 0;j<list.size();j++) 
			cout<<list[j].OID<<'\t'<<list[j].arrival<<'\t'<<list[j].duration<<'\t'<<list[j].timeout<<endl;
	}
			
	void sortByArrival(int n) {	// declaration: sort all by arrival time by using shell sort
		for(int h = n/2;h>0;h=h/2){ 
			for(int unsorted = h;unsorted<n;++unsorted)
			{	int loc = unsorted;
				int nextitem = aList[unsorted].arrival;
				int nextitemm = aList[unsorted].OID;
				int nextitemmm = aList[unsorted].duration;
				int nextitemmmm = aList[unsorted].timeout;
				for(;(loc >= h) && (aList[loc - h].arrival > nextitem)|| 
                    ((aList[loc - h].arrival == nextitem) && (aList[loc - h].OID > nextitemm));loc = loc - h){ 
					
					aList[loc].arrival = aList[loc - h].arrival;
					aList[loc].OID = aList[loc - h].OID;
					aList[loc].duration = aList[loc - h].duration;
					aList[loc].timeout = aList[loc - h].timeout;
				} 
				aList[loc].arrival = nextitem;
				aList[loc].OID = nextitemm;
				aList[loc].duration = nextitemmm;
				aList[loc].timeout = nextitemmmm;
			}			
		} 				
	} // sortByArrival()
	
	void putAll() {	// declaration: write all as a file
		string id = "sorted"+fileID+".txt";
		ofstream outfile(id.c_str());
		if(outfile.is_open()){
			outfile<<"OID"<<'\t'<<"Arrival"<<'\t'<< "Duration"<<'\t'<<"TimeOut"<<endl;
			for(int j = 0;j < aList.size();j++){
				outfile<<aList[j].OID<<'\t'<<aList[j].arrival<<'\t'<<aList[j].duration<<'\t'<<aList[j].timeout<<endl;
			}
			outfile.close();
		}
	} // putAll()
		
	void showTime(double r,double s,double w) {	// declaration: output time on screen
		cout<<endl;
		cout << "Reading data: " << fixed << setprecision(1) << r << " ms" << endl;
		cout << "Sorting data: " << fixed << setprecision(1) << s << " ms" << endl;
		cout << "Writing data: " << fixed << setprecision(1) << w << " ms" << endl;
	} // showTime()
	
	// The above are private methods

public:
	JobList() { reset(); }		// constructor for initialization
	~JobList() { reset(); }		// destructor for initialization
	bool isEmpty() { 			// check whether it is empty or not
		return aList.empty();
	} 
	// The above are primitive methods publicly available
	
	bool getAll(string id) {		// declaration: read all from a file
		FILE	*infile = NULL;
		fileID = "input"+id+".txt"; 
		infile = fopen(fileID.c_str(),"r");
		if (infile == NULL) {
			cout << endl << fileID << " does not exist!" << endl;	
			return false;	
		}
		else
		{	jobType tmp;
			int num;
			fscanf(infile,"%*c%*[^\n]%*c");					//read the first line out
			while(fscanf(infile, "%d", &num) != EOF)
			{	
				tmp.OID = num;
				fscanf(infile, "%d", &num);
				tmp.arrival = num;
				fscanf(infile, "%d", &num);
				tmp.duration = num;
				fscanf(infile, "%d", &num);
				tmp.timeout = num;
				if(tmp.duration <= 0 || (tmp.arrival + tmp.duration) > tmp.timeout)
					return false;
				aList.push_back(tmp);
				
			}
			fclose(infile);
			return true;
		}
	} // getAll()
	
	bool getAllsorted(string &id ){// declaration: read all sorted data from a file
		cout << endl << "Input a file number: ";
		cin >> id;		
		FILE	*infile = NULL;
		fileID = "sorted"+id+".txt";
		infile = fopen(fileID.c_str(),"r");
		if (infile == NULL) {
			cout << endl << fileID << " does not exist!" << endl;	
			return false;	
		}
		else
		{	fileID=id;
			jobType tmp;
			int num;
			fscanf(infile,"%*c%*[^\n]%*c");						//read the first line out
			while(fscanf(infile, "%d",&num) != EOF)
			{	
				tmp.OID = num;
				fscanf(infile, "%d", &num);
				tmp.arrival = num;
				fscanf(infile, "%d", &num);
				tmp.duration = num;
				fscanf(infile, "%d", &num);
				tmp.timeout = num;
				if(tmp.duration <= 0 || (tmp.arrival + tmp.duration) > tmp.timeout)
					return false;
				aList.push_back(tmp);
				
			}
			fclose(infile);
			return true;
		}
	} // getAllsorted()
	
	bool getSorted() {			// declaration: read all from a file and sort them
		string id;
		cout<<endl<<"Input a file number: ";
	    cin>>id;
		clock_t start = clock();
		if(getAll(id)){
			clock_t end = clock();
			fileID = id;
			double tmp = end - start;
			start = clock();
			sortByArrival(aList.size());
			end = clock();
			double stmp = end - start;
			start = clock();
			putAll();
			end = clock();
			double wtmp = end - start;
			showTime(tmp, stmp, wtmp);
			showJob(aList);
			return true;
		}
		else
			return false;
	} // getSorted()
	
	void nextJob(jobType & nextjob) {	// declaration: get & remove the next job
		if (aList.size() > 0) {
        	nextjob = aList[0];
    	}
		aList.erase(aList.begin());
	} // nextJob()
	
	void nextJobCheck(jobType &nextJob) {	// declaration: take a look at the next job
	 	if (aList.size() > 0) {
        	nextJob = aList[0];
    	}			
	} // nextJobCheck()
	
}; //end JobList


template<typename T>
class JobQueue {
	T *cA;						// circular array
	int qFront, qBack;			// head & tail of queue
	int qSize, qMax;			// current size & max size of queue
	// The above are private data members
	int next(int idx) { return (idx + 1) % qMax; } // move to the next position in queue
	// The above are private methods
public:
	int avail;			// the time when CPU is available 	
	// The above are public data members
	
	JobQueue():cA(NULL), qFront(0), qBack(0),
				qSize(0), qMax(0), avail(0) {}		// constructor of no-space queue
	JobQueue<T>(int maxS):qFront(0), qSize(0), avail(0)	// constructor of an empty queue
	{ 	qMax = maxS;
		cA = new T[qMax];	
	} // end constructor
	
	int length() const  { // get the current queue length
		return qSize;	
	} // length()	
	
	bool isEmpty() const { // check whether it is empty
		if (qSize == 0)
			return true;
			
		return false;
	} // isEmpty()
	
	bool isFull() const { // check whether it is full
		return qSize == qMax; 
	} // isFull()
	
	void enQueue(T &job) {		// append a new element
		if(isEmpty()) {
			cA[qBack] = job;
			qSize++;
		}
		else if(!isFull()) {
			qBack = next(qBack);
            cA[qBack] = job;
            qSize++;
        } 
    } // enQueue()
    
	void getFront(T &job) {		// get the first element
		if (!isEmpty()) {
			job = cA[qFront];
		}
	} // getFront()
	
	void deQueue() {		// drop the first element
		if (!isEmpty()) {
			qFront = next(qFront);
			qSize--;
		}
	} // deQueue()
	
	void deQueue(T &job) {		// get & drop the first element
		if (!isEmpty()) {
			job = cA[qFront];
			deQueue();
		}
	} // deQueue(T &job)
	
	void clearQ() {			// clean up
		qFront = qBack = qSize = 0;
	    delete[] cA; 
	    cA = new T[qMax]; 
	    avail = 0;
	} // clearQ()
	
	~JobQueue()				// destructor
	{ delete[] cA; }  //end destructor
	
}; //end JobQueue


class AnsList {
	string id;		// file id
	
	typedef struct aT
	{	int OID;    // 工作編號
        int abortTime; // 取消時刻
        int delay;  // 延遲時間
	} abortType;
	
	typedef struct dT
	{ 	int OID;    // 工作編號
        int departureTime;  // 完成時刻
        int delay;  // 延遲時間/
	} doneType;
	
	vector<abortType> abortJobs;		// list of aborted jobs with three columns
	vector<doneType> doneJobs;			// list of done jobs with three columns
	float avgDelay;					    // average delay
	float successRate;					// percentage of done jobs
	// The above are private data members
	
	void showAbortJobs(string &OAD) {	// declaration: display abort jobs on screen
		OAD += "\t[Abort Jobs]\n";
    	OAD += "\tOID\tAbort\tDelay\n";
        for (int i = 0; i < abortJobs.size(); ++i) {
            abortType job = abortJobs[i];
            OAD +="["+its(i+1)+"]\t"+its(job.OID)+"\t"+its(job.abortTime)+"\t"+its(job.delay)+"\n";
    	}
    	cout<<OAD;
    } // showAbortJobs()
		
	void showDoneJobs(string &ODD) {			// declaration: display done jobs on screen
		ODD += "\t[Jobs Done]\n";
    	ODD += "\tOID\tDeparture\tDelay\n";
        for (int i = 0; i < doneJobs.size(); ++i) {
            doneType job = doneJobs[i];
            ODD +="["+its(i+1)+"]\t"+its(job.OID)+"\t"+its(job.departureTime)+"\t"+its(job.delay)+"\n";
    	}
    	cout<<ODD;
    } // showDoneJobs()
    
	void computeStat() {			// declaration: compute the statistics
		int totalDelay = 0;
        for (int i = 0; i < doneJobs.size(); ++i) {
        	doneType job = doneJobs[i];
            totalDelay = job.delay + totalDelay;
        }
        
        for (int i = 0; i < abortJobs.size(); ++i) {
        	abortType job = abortJobs[i];
            totalDelay = job.delay + totalDelay;
        }
        
        if (doneJobs.size() > 0) 	
		    avgDelay = float(totalDelay) / (doneJobs.size() + abortJobs.size());
		else 
		    avgDelay = 0;
		
		if (doneJobs.size() + abortJobs.size() > 0)
		    successRate = (float(doneJobs.size()) * 100) / float(doneJobs.size() + abortJobs.size());
		else
		    successRate = 0;
		    
    } // computeStat()
		
	void showStat(string &output) {                    // declaration: display statistics on screen
		float roundedavgDelay = round(avgDelay * pow(10, 2)) / pow(10, 2);
		float roundedsuccessRate = round(successRate * pow(10, 2)) / pow(10, 2);
		output+= "[Average Delay]\t"+fts(roundedavgDelay) +" ms\n[Success Rate]\t"+fts(roundedsuccessRate)+"%\n";
        cout << output;
    } // showStat()
	// The above are private methods
	
public:
	AnsList(): avgDelay(0.0), successRate(0.0)
	{};
	AnsList(string fileid): avgDelay(0.0), successRate(0.0)
	{ id  = fileid;}					// constructor for initialization
	
	~AnsList()								
	{ 	vector<abortType>().swap(abortJobs);
		vector<doneType>().swap(doneJobs);
	}					// destructor for initialization
	
	void showAll() {					// declaration: display all on screen
		string out;
		
		showAbortJobs(out);
		out.clear();
		
        showDoneJobs(out);
        out.clear();
        
        computeStat();
        showStat(out);
        out.clear();
	} // showAll()
	
	void addAbortJob(int OID, int abortTime, int delay){	 // declaration: add one aborted job
		abortType job = {OID, abortTime, delay};
        abortJobs.push_back(job);
    } // addAbortJob()
    
	void addDoneJob(int OID, int departureTime, int delay){ 	// declaration: add one done job 
		doneType job = {OID, departureTime, delay};
        doneJobs.push_back(job);
    }
	// The above are primitive methods
	
	void putAll() {          // declaration: write all as a file
        string output;
		string allout;
	    showAbortJobs(output);
	    allout += output;
	    output.clear();
	    showDoneJobs(output);
	    allout += output;
	    output.clear();
	    computeStat();
	    showStat(output);
		output.clear();
		output = "output"+id+".txt";
	    ofstream outfile(output.c_str());
	    if (outfile.is_open()) {
	        outfile << allout;
	        outfile.close();
	    }
	    
	    cout << endl << "See " << output << endl;
    }
    
    void setid( string name ){							// set file id
    	id = name;
    }
    	
}; //end AnsList

class Simulation {
	JobList	jobs;	//a list of jobs
	AnsList	answers;	//a set of answers
	JobQueue<jobType> **allQ;	// a set of queues
	string id;	// file id
	int	qNum;	// number of queues
	// The above are private data members
	
	void delQ(int idx){	// declaration: delete an old job from a queue
		allQ[idx]->deQueue();
	}
	
	void updateQ(int currentTime, int queueIdx) {	// declaration: update each queue
        jobType currentJob;
        allQ[queueIdx]->getFront(currentJob);

        if (currentTime >= currentJob.timeout) {
            answers.addAbortJob(currentJob.OID, currentJob.timeout, currentTime - currentJob.arrival);
            delQ(queueIdx);
        } 
		else {
            allQ[queueIdx]->deQueue(currentJob);
            allQ[queueIdx]->enQueue(currentJob);
        }
    }	
    
	void finishQ(){	         // declaration: finish the remaining jobs in queues
		for (int i = 0; i < qNum; ++i) {
            while (!allQ[i]->isEmpty()) {
                jobType currentJob;
                allQ[i]->deQueue(currentJob);
                answers.addDoneJob(currentJob.OID, currentJob.arrival + currentJob.duration, 0);
            }
        }
    }
	// The above are private methods
	
public:
	Simulation(JobList aList, int N): jobs(aList), qNum(N) // copy constructor
	{	string id;
		allQ = new JobQueue<jobType> *[qNum];		
        for (int i = 0; i < qNum; ++i) {
            allQ[i] = new JobQueue<jobType>(3);
        }
		if ( jobs.getAllsorted( id ) ) {
			answers.setid(id);
	        SQF();
		}
	} //end constructor
	
	~Simulation()	// destructor
	{ 	for (int i = 0; i < qNum; ++i) {
	        delete allQ[i];
	    }
    	delete[] allQ; 
	}	//end Destructor

	void SQF() {
	    int currentTime = 0;
	    int currentJobBegin;
	    int finishTimes = 0;
	    jobType nextJob;
	    jobType currentJob;
	    
	    if(!jobs.isEmpty()) {
	    	jobs.nextJob(currentJob);                  // 剛開始先從工作列表取出第一個工作，設為正在執行的工作 
	    	if (currentTime < currentJob.arrival)      // 將目前時間設為執行中工作的進入時刻 
	    		currentTime = currentJob.arrival;
	    		currentJobBegin = currentTime;         //  將目前執行中的工作之開始時間記錄下來 
		}
	
	    while (!jobs.isEmpty()) {
	        bool allQueuesEmpty = true;
	
	        for (int i = 0; i < qNum; i++) {
	            if (!allQ[i]->isEmpty()) {
	                allQueuesEmpty = false;
	                break;
	            }
	        }
	
	        if (jobs.isEmpty() && allQueuesEmpty) {
	            // All queues are empty, simulation ends
	            break;
	        }
	
	        if (!jobs.isEmpty()) {
	            jobs.nextJob(nextJob);     // 從工作列中取出工作 
	        
	            
	            if (nextJob.arrival == currentJob.arrival + currentJob.duration) {      // 舊工作的完成時刻與新工作的進入時刻相同 
	            	answers.addDoneJob(currentJob.OID, currentJobBegin + currentJob.duration, currentJobBegin - currentJob.arrival); // 先完成舊工作 
					allQ[0]->deQueue(currentJob);        // 從佇列中取出最優先的工作 
					currentTime = nextJob.arrival;       // 將目前時間設為新工作的進入時刻 
					currentJobBegin = currentTime;       // 將目前執行中的工作之開始時間記錄下來 
				}
				
				
				if (finishTimes != 0) {  // 原工作列中的第一個工作已經執行完畢 
					if (currentTime > currentJob.timeout) { // 取出時已經逾時，符合第二種取消條件，將工作加入失敗清單
	    				answers.addAbortJob(currentJob.OID, currentTime, currentTime - currentJob.arrival);
	    				if (!allQ[0]->isEmpty())  // 若佇列仍有工作，則取出工作 
							allQ[0]->deQueue(currentJob);
						}
					else if (currentTime + currentJob.duration > currentJob.timeout) { // 執行途中發現逾時，符合第三種取消條件，將工作加入失敗清單 
						answers.addAbortJob(currentJob.OID, currentJob.timeout, currentJob.timeout - currentJob.arrival);
						currentTime = currentJob.timeout;  // 目前時間設為此工作的逾時時刻 
						if (!allQ[0]->isEmpty())  // 若佇列仍有工作，則取出工作 
							allQ[0]->deQueue(currentJob);  
						else   // 若佇列為空，則將目前工作清空 
							currentJob.arrival = currentJob.duration = currentJob.OID = currentJob.timeout = 0;
					}
				}
	
	            if (nextJob.arrival < currentTime + currentJob.duration) {  // 新工作進入時刻比目前工作完成的時間還早 
	                if (!allQ[0]->isFull()) {       // 佇列仍有空位 
	                	allQ[0]->enQueue(nextJob);  // 將新工作加入佇列 
	                	if (finishTimes == 0)     // 尚未有執行完成的工作  
	                		currentTime = nextJob.arrival;      // 將目前時間設為新工作的進入時刻 
	            	}
	                else if (allQ[0]->isFull()) {      //佇列已滿 
	                	answers.addAbortJob(nextJob.OID, nextJob.arrival, 0);   // 符合第一種取消條件，將工作加入失敗清單 
	                	if (finishTimes == 0)     // 尚未有執行完成的工作 
	                		currentTime = nextJob.arrival;      // 將目前時間設為新工作的進入時刻
	            	}
	            }

				if (currentJob.OID == 0) {  // 若是目前無工作但佇列仍有工作等待中，取出佇列中最優先工作 
					if (!allQ[0]->isEmpty())
						allQ[0]->deQueue(currentJob);
				}
				
				while (currentTime >= currentJob.arrival + currentJob.duration && !allQ[0]->isEmpty()) {
					finishTimes++;   // 原工作列中的第一個工作執行完畢
					if (currentTime > currentJob.timeout) {  // 取出時已經逾時，符合第二種取消條件，將工作加入失敗清單 
						answers.addAbortJob(currentJob.OID, currentTime, currentTime - currentJob.arrival);
						if (!allQ[0]->isEmpty())  // 若佇列仍有工作，則取出工作 
							allQ[0]->deQueue(currentJob);
					}
					else if (currentTime + currentJob.duration > currentJob.timeout) {  // 執行途中發現逾時，符合第三種取消條件，將工作加入失敗清單 
						answers.addAbortJob(currentJob.OID, currentJob.timeout, currentJob.timeout - currentJob.arrival);
						currentTime = currentJob.timeout;  // 目前時間設為此工作的逾時時刻 
						if (!allQ[0]->isEmpty())  // 若佇列仍有工作，則取出工作 
							allQ[0]->deQueue(currentJob);
					}
					else if (currentTime >= currentJob.arrival + currentJob.duration) {  // 可成功執行，將工作加入完成清單  
						answers.addDoneJob(currentJob.OID, currentJobBegin + currentJob.duration, currentJobBegin - currentJob.arrival);
						currentTime += currentJob.duration;  // 目前時間設為此工作的完成時刻 
						if (!allQ[0]->isEmpty())  // 若佇列仍有工作，則取出工作 
							allQ[0]->deQueue(currentJob);
						currentJobBegin = currentTime;  // 將目前執行中的工作之開始時間記錄下來
					}
				} // while
				
				if (finishTimes != 0) {  // 原工作列中的第一個工作已經執行完畢  
					if (currentTime + currentJob.duration <= currentJob.timeout && allQ[0]->isEmpty()) {  // 當佇列為空，且目前工作不會逾時 
						answers.addDoneJob(currentJob.OID, currentTime + currentJob.duration, currentTime - currentJob.arrival);  // 可成功執行，將工作加入完成清單 
						currentTime += currentJob.duration;  // 目前時間設為此工作的完成時刻 
						jobs.nextJob(nextJob);  // 取出工作列中的下一個工作，並設為目前工作 
						currentJob = nextJob;
						if (currentTime < currentJob.arrival)
							currentTime = currentJob.arrival;  // 將目前時間設為目前工作的的進入時刻 
						currentJobBegin = currentTime;  // 將目前執行中的工作之開始時間記錄下來
						finishTimes == 0;  // 因目前工作未執行且佇列為空，將狀況設為原工作列中的第一個工作尚未執行
					}
				}
				
				if (allQ[0]->isEmpty())  // 若是佇列中的工作被取完了 
					allQ[0]->clearQ();   // 將佇列重設一次 
	        }
	        
	    } // while
	    
	    if (currentTime > currentJob.timeout)   // 取出時已經逾時，符合第二種取消條件，將工作加入失敗清單
	    	answers.addAbortJob(currentJob.OID, currentTime, currentTime - currentJob.arrival);
	    else {
	    	currentTime = currentJob.arrival + currentJob.duration;  // 工作列表已清空，將目前的工作執行完畢 
	    	if (currentTime <= currentJob.timeout) {  // 目前工作無逾時，將工作加入完成清單 
				answers.addDoneJob(currentJob.OID, currentJobBegin + currentJob.duration, currentJobBegin - currentJob.arrival);
			}
			else if (currentTime > currentJob.timeout) { // 執行途中發現逾時，符合第三種取消條件，將工作加入失敗清單
				answers.addAbortJob(currentJob.OID, currentTime, currentTime - currentJob.arrival);
				currentTime = currentJob.timeout;  // 目前時間設為此工作的逾時時刻 
			}
		}
	    
	    while (!allQ[0]->isEmpty()) {   // 若佇列中仍有工作等待 
	    	allQ[0]->deQueue(currentJob);   // 依序取出並執行 
	    	
	    	if (currentTime > currentJob.timeout)   // 取出時已經逾時，符合第二種取消條件，將工作加入失敗清單
	    		answers.addAbortJob(currentJob.OID, currentTime, currentTime - currentJob.arrival);
	    	else {
	    		if (currentTime + currentJob.duration > currentJob.timeout) { // 執行途中發現逾時，符合第三種取消條件，將工作加入失敗清單 
	    			answers.addAbortJob(currentJob.OID, currentJob.timeout, currentJob.timeout - currentJob.arrival);
	    			currentTime = currentJob.timeout;  // 目前時間設為此工作的逾時時刻 
	    		}
	    		else { // 可成功執行，將工作加入完成清單 
	    			answers.addDoneJob(currentJob.OID, currentTime + currentJob.duration, currentTime - currentJob.arrival);
	    			currentTime = currentTime + currentJob.duration; // 目前時間設為此工作的完成時刻 
	    		}
			}
		} // while
	    		
	    // Output results
	    answers.putAll();
	} // SQF(
	
}; //end Simulation


int main(void)
{	int command = 0;
	string id;
	do
	{	
		cout<<endl<<"**** Simulate FIFO Queues by SQF *****";
		cout<<endl<<"* 0. Quit                            *";
		cout<<endl<<"* 1. Sort a file                     *";
		cout<<endl<<"* 2. Simulate one FIFO queue         *";
		cout<<endl<<"**************************************";
		cout<<endl<<"Input a command(0, 1, 2): ";
		cin>>command;
		if((command == 1)||(command == 2))
		{	if(command==1){
				JobList jobList;
				jobList.getSorted();
			}
			else if	(command==2){
				
				JobList jobList;
				Simulation sim (jobList,1);
			}
		}
		else if(command == 0)//stop the program
			break;
		else
			cout<<endl<<"Command does not exist!"<<endl;
	
	} while(true);
	
	system("pause");
	return 0;
} // main()

