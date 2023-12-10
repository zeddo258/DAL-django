// 11127251 林柏安 11127245 薛宏毓 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <cmath>

using namespace std;

typedef struct jT {
	int OID;		// order identifier
	int arrival;	//arrival time
	int duration;	// job duration
	int timeout;	// expire time
} jobType;

class JobList {
	vector<jobType> aList;
	string fileID;
	double read, sort, write;  // record time 
	
	void reset() {
		aList.clear();
		fileID = "";
	} // end reset()
	
	// 印出jobType的資料 
	void showJob( jobType& curJob) {
		cout << curJob.OID << '\t';
		cout << curJob.arrival << '\t';
		cout << curJob.duration << '\t';
		cout << curJob.timeout << '\t' << endl;
	} // end showJob()
	
	// shell sort
	void sortByArrival() {
		int len = aList.size();
		
		// 紀錄時間(開始) 
		clock_t start = clock();
		// 將序列分成兩邊，從個別的最開始位置進行比較 
		for (int step = len; step > 0; step /= 2) {
	        for (int unsorted = step; unsorted < len; ++unsorted) {
	
	            int loc = unsorted;
	            jobType nextItem = aList[unsorted];
				
				// 如果資料的arrival小於前者 ，兩者交換位置，如果arrival相同則比較OID大小 
	            for (; (loc >= step) && (aList[loc - step].arrival > nextItem.arrival) ||
                       (aList[loc - step].arrival == nextItem.arrival && aList[loc - step].OID > nextItem.OID); loc -= step) {
	                	aList[loc] = aList[loc - step];
	        	} // for
	
	            aList[loc] = nextItem;
	        } // end for (unsorted)
    	} // end for (step)
    	
    	// 紀錄時間(停止) 
    	clock_t stop = clock();
		sort = (stop - start) / (double) CLOCKS_PER_SEC;
	} // end sortByArrival
	
	// 創建新檔案存入已排序後的資料 
	void putAll() {
        string str = "sorted"+fileID+".txt";		// 檔名 
        ofstream outputFile( str.c_str() );

        outputFile << "OID\tArrival\tDuration\tTimeOut" << endl;
        
        clock_t start = clock();	// 計時開始 
        for ( int i = 0 ; i < aList.size() ; i ++ ) {
            outputFile << aList[i].OID << '\t';
            outputFile << aList[i].arrival << '\t';
            outputFile << aList[i].duration << '\t';
            outputFile << aList[i].timeout << '\t' << endl;
        } // end for
        clock_t stop = clock();
		write = (stop - start) / (double) CLOCKS_PER_SEC;		//計時停止並存入變數write中 
			
        outputFile.close();
    } // end putAll()
	
	void nextJobCheck( jobType& curJob ) {
		curJob = aList[0];
	} // end nextJobCheck()
	
public:
	JobList() {
		reset();
	}
	
	~JobList() {
		reset();
	}
	
	bool isEmpty() {
		if (aList.size() == 0)
			return true;
		else 
			return false;
	} // end isEmpty()
	
	void setFileID(string fileName) {
		fileID = fileName;
	} // end setFileID()
	
	// 讀檔 
	bool getAll( string file ) {
		ifstream inputFile( file.c_str() );
		if (inputFile.is_open()) {
			string line;
			getline( inputFile, line );  // OID, arrival, duration, timeout
			
			clock_t start = clock();
			while ( getline( inputFile, line ) ) {
		        jobType temp;
		        stringstream ss;
		        ss << line;
		
		        int num;
		        int count = 0;
		        while ( ss >> num ) {
		            if ( count == 0 ) temp.OID = num;
		            else if ( count == 1 ) temp.arrival = num;
		            else if ( count == 2 ) temp.duration = num;
		            else if ( count == 3 ) temp.timeout = num;
		
		            count ++;
		        } // while() 
		        
		        aList.push_back( temp );
	        } // while(getline)
	        
			clock_t stop = clock();
			read = ( stop - start ) / (double) CLOCKS_PER_SEC;
			
	        inputFile.close();
	        return true;
		} // end if (file open)
		
		else {
			return false;
		} // end else
        
    } // end getAll()
	
	// 呼叫 排序和寫入Sorted 
	bool getSorted() {
		cout << "\tOID\tArrival\tDuration\tTimeOut" << endl;
        for ( int i = 0 ; i < aList.size() ; i ++ ) {
        	cout << "(" << i+1 << ")\t";
            showJob( aList[i] );
        } // for
        
        sortByArrival();
        
        putAll();
        cout << endl;
        return true;
    } // end getSorted()
	
	void nextJobAndDel( jobType& job ) { // need to confirm whether it is empty first (for mission 2)
		nextJobCheck(job);               // get
		aList.erase(aList.begin());      // and delete
	} // end nextJob()
	
	void nextJob( jobType& job ) {  // need to confirm whether it is empty first (for mission 2)
		nextJobCheck(job);          // get
	} // end nextJob()
	
	int size() {
		return aList.size();
	}
	
	// 輸出時間 
	void showTime() {
		cout << fixed << setprecision(2);
		cout << "Reading data: " << (int)read << " clocks " << "(" << read << " ms" << ")." << endl;
		cout << "Sorting data: " << (int)sort << " clocks " << "(" << sort << " ms" << ")." << endl;
		cout << "Writing data: " << (int)write << " clocks " << "(" << write << " ms" << ")." << endl;
	} // end shoeTime()
};


template<typename T>
class JobQueue {
	T *cA;					// circular array
	int qFront, qBack;		// head & tail of queue
	int qSize, qMax;		// current size & max size of queue
	
	// move to the next position in queue
	int next(int idx) {
		return ( idx + 1 ) % qMax;
	} // end next() 
	
public:
	int avail;				// the time when CPU is available
	
	JobQueue() : cA(NULL), qFront(0), qBack(0), 
					qSize(0), qMax(0), avail(0) {
		
	} // end constructor()
	
	JobQueue<T> (int maxS): qFront(0), qSize(0), avail(0) {
		cA = new T[maxS];
		qMax = maxS;
	} // end constructor()
	
	// get the current queue length
	int length() const {
		return qSize;
	} // end length()
	
	// check whether it is empty
	bool isEmpty() const {
		if ( qSize == 0 ) {
			return true;
		}
		
		return false;
	} // end isEmpty()
	
	// check whether it is full
	bool isFull() const {
		if ( qSize == qMax ) {
			return true;
		}
		
		return false;
 	} // end isFull()
 	
 	// append a new element
 	void enQueue(T& newElement ) {
 		if ( isFull() ) {
 			cout << "full";
 			return;
		 }
		 
		 cA[qBack] = newElement;
		 qBack = next(qBack);
		 qSize ++;
		 
	} // end enQueue()
	
	// get the first element
	void getFront(T& firstElement ) {
		if ( isEmpty() ) {
			return;
		}
		
		firstElement = cA[qFront];
	} // end getFront()
	
	// drop the first element
	void deQueue() {
		if ( isEmpty() ) {
			return;
		}
		
		qFront = next( qFront );
		qSize --;
	} // end deQueue()
	
	// get & drop the first element
	void deQueue(T& firstElement ) {
		if ( isEmpty() ) {
			return;
		}
		
		firstElement = cA[qFront];
		qFront = next( qFront );
		qSize --;
	} // end deQueue()
	
	// clean up
	void clearQ() {
		delete[] cA;
		
		cA = NULL;
		qFront = 0;
		qBack = 0;
		qSize = 0;
		qMax = 0;
		avail = 0;
	} // end clearQ()
	
	// destructor
	~JobQueue() {
		clearQ();
	} // end destructor()
	
}; // class JobQueue


class AnsList {
	typedef struct aT {
		int OID;
		int abort;
		int delay;
	} abortType;
	
	typedef struct dT {
		int OID;
		int departure;
		int delay;
	} doneType;
	
	vector<abortType> abortJobs;	// list of aborted jobs with three columns
	vector<doneType> doneJobs;		// list of done jobs with three columns
	
	float avgDelay;					// average delay
	float successRate;				// percentage of done jobs
	
public :
	AnsList(): avgDelay( 0.0 ), successRate( 0.0 ) {
		
	}
	
	~AnsList() {
		abortJobs.clear();
		doneJobs.clear();
	}
	
	// display all on screen
	void showAll() {
		cout << "\t[Abort Jobs]" << endl;
		cout << "\tOID\tAbort\tDelay" << endl;
		for (int n = 0; n < abortJobs.size(); n++) {
			cout << "[" << n+1 << "]\t";
			cout << abortJobs[n].OID << '\t';
			cout << abortJobs[n].abort << '\t';
			cout << abortJobs[n].delay << endl;
		} // for
			
		cout << "\t[Jobs Done]" << endl;
		cout << "\tOID\tDeparture\tDelay" << endl;
		for (int n = 0; n < doneJobs.size(); n++) {
			cout << "[" << n+1 << "]\t";
			cout << doneJobs[n].OID << '\t';
			cout << doneJobs[n].departure << '\t';
			cout << doneJobs[n].delay << endl;
		} // for
	}
	 
	
	// add one aborted job
	void addAbortJob( jobType job, int endTime ) {
		abortType temp;
		temp.OID = job.OID;
		temp.abort = endTime;
		temp.delay = endTime - job.arrival;
		abortJobs.push_back(temp);
	}
	
	
	// add one done job
	void addDoneJob( jobType job, int endTime ) {
		doneType temp;
		temp.OID = job.OID;
		temp.departure = endTime;
		temp.delay = endTime - job.arrival - job.duration;
		doneJobs.push_back(temp);
	}
	
	// write all as a file (only call once when the simulation is completed
	void putAll( string fileName ) {  // delete another string parameter because I don't kown what is its role
		fileName = "output"+fileName+".txt";
		ofstream outputFile( fileName.c_str() );
		
		if ( outputFile.is_open() ) {
			outputFile << "\t[Abort Jobs]" << endl;
			outputFile << "\tOID\tAbort\tDelay" << endl;
			for (int n = 0; n < abortJobs.size(); n++) {
				outputFile << "[" << n+1 << "]\t";
				outputFile << abortJobs[n].OID << '\t';
				outputFile << abortJobs[n].abort << '\t';
				outputFile << abortJobs[n].delay << endl;
				
				avgDelay += abortJobs[n].delay;
				
			} // for
			
			outputFile << "\t[Jobs Done]" << endl;
			outputFile << "\tOID\tDeparture\tDelay" << endl;
			for (int n = 0; n < doneJobs.size(); n++) {
				outputFile << "[" << n+1 << "]\t";
				outputFile << doneJobs[n].OID << '\t';
				outputFile << doneJobs[n].departure << '\t';
				outputFile << doneJobs[n].delay << endl;
				
				avgDelay += doneJobs[n].delay;
				
			} // for
			
			// 算出平均Delay時間 和 成功率 
			avgDelay /= (float) abortJobs.size() + doneJobs.size();
			successRate = (float) doneJobs.size() / ( doneJobs.size() + abortJobs.size() ) * 100;
			
			// 取小數後兩位 
			avgDelay = round( avgDelay * 100 ) / 100;
			successRate = round( successRate * 100 ) / 100;
			
			outputFile << "[Average Delay] " << avgDelay << " ms" << endl;
			outputFile << "[Success Rate]  "  << successRate<< " %" << endl;
			outputFile.close();
		}
		
	} // end putAll()
	
	// 回傳abortJobs的大小 
	int aSize() {
		return abortJobs.size();
	}
	
};


class Simulation {
	JobList jobs;				// a list of jobs 
	AnsList answers;			// a set of answers
	JobQueue<jobType> **allQ;	// a set of queues
	int qNum;					// number of queue
	
	
public:
	Simulation() {
		
	}
	
	Simulation( int N ) : qNum(N) {
		allQ = new JobQueue<jobType>*[N];
		for (int i = 0; i < N; i++) {
    		allQ[i] = new JobQueue<jobType>(3);
		} // for
	} // simulation
	
	// destructor
	~Simulation() {
	    for (int i = 0; i < qNum; i++) {
	        delete allQ[i];
	    }
	    
	    delete[] allQ;
	}
	
	// shortest queue first
	void SQF() {
		
	}
	
	// handle queuing problem and assign to doneJob or abortJob list
	void CPU( string fileName ) {
		
		jobType data, temp;
		int endTime;
		
		// take the first item at jobList vector if (not empty) and add to the doneJob list
		if ( !jobs.isEmpty() ) {
			jobs.nextJobAndDel(data);
			endTime = data.arrival + data.duration;
			answers.addDoneJob(data, endTime);
		} // end if (joblist not empty)
		
		// allocate remaining data of jobList
		while( !jobs.isEmpty() ) {
			
			bool action = false;       // check have change for answers object or queue
			jobs.nextJobAndDel(data);  // get and delete the first item of jobList vector 
			
			// no need to queue
			if ( allQ[0]->isEmpty() && data.arrival >= endTime ) {
				endTime = data.arrival + data.duration;  // end = arrival + processing time
				answers.addDoneJob(data, endTime);       // success, add to done
			} // if
			
			// there is room to queue up
			else if ( !allQ[0]->isFull() ) {
				allQ[0]->enQueue(data);  // add to the queue
				action = true;			 // queue change
			} // else if
			
			// the queue is full
			else {
				bool timeOut = false;    // check is timeout exceeded of data
				
				// arrive the queue is still full
				if ( data.arrival < endTime ) {
					answers.addAbortJob(data, data.arrival);  // add to the abort
					action = true;  // queue change
				} // if
				
				// just finished on arrive
				else if ( data.arrival == endTime ) {  // there may will have CPU time out
					allQ[0]->deQueue(temp); // the first one to enter processing
					
					// previous data causes current data to expire.
					if ( temp.timeout < endTime ) {
						answers.addAbortJob(temp, endTime); // add to the abort
					} // if
					
					// can be processed normally
					else {
						// timeout before processing current data.
						if ( temp.timeout < endTime + temp.duration ) {
							endTime = temp.timeout;  // only counts until processing timeout
							answers.addAbortJob(temp, endTime); // add to the abort
						} // if
						
						// end of processing
						else {
							// the current data arrives later than the end of the previous data
							if ( temp.arrival > endTime )
								endTime = temp.arrival + temp.duration; // the end time of current data processing
							// The current data arrived before or just before the end of the previous data
							else 
								endTime = endTime + temp.duration;		// the end time of current data processing
								
							answers.addDoneJob(temp, endTime); // add to the done
						} // end else
					} // end else
					
					allQ[0]->enQueue(data); // add the data by the jobList to the queue
					action = true;			// answers object or queue change
				} // else if
				
				// will arrive after finishing
				else {  
					// data.arrival > endTime
					// the queue is full, but also mean the first item of queue is completed
					// so it will be add to the queue at if(!action) function
				} // end else
				
			} // end else
			
			// when you have not any action to add list or enqueue, you will deal with the queue first item and set new end time
			if (!action) { // there may have CPU time out
				allQ[0]->deQueue(temp); // the first one to enter processing
				
				// previous data causes current data to expire.
				if ( temp.timeout < endTime ) {
					answers.addAbortJob(temp, endTime); // add to the abort
				} // if
					
				// can be processed normally
				else {
					// timeout before processing current data.
					if ( temp.timeout < endTime + temp.duration ) {
						endTime = temp.timeout;  // only counts until processing timeout
						answers.addAbortJob(temp, endTime); // add to the abort
					} // if
						
					// end of processing
					else {
						// the current data arrives later than the end of the previous data
						if ( temp.arrival > endTime )
							endTime = temp.arrival + temp.duration; // the end time of current data processing
						// the current data arrived before or just before the end of the previous data
						else 
							endTime = endTime + temp.duration;		// the end time of current data processing
							
						answers.addDoneJob(temp, endTime); // add to the done
						} // end else
				} // end else
				
				allQ[0]->enQueue(data); // add the data by the jobList to the queue
			} // if (no action)
			
			// there has been action before
			else { 
				// do nothing
			} // end else
			
		} // while(joblist not empty))
		
		// process data in the queue when the jobList is empty
		while ( !allQ[0]->isEmpty() ) {
			
			allQ[0]->deQueue(temp); // get the first data in the queue
			int tempTime = 0; 		// temp completion time
			
			// the current data arrives later than the end of the previous data
			if ( temp.arrival > endTime )
				tempTime = temp.arrival + temp.duration; // set temp completion time
				
			// the current data arrived before or just before the end of the previous data
			else 
				tempTime = endTime + temp.duration;		 // set temp completion time
			
			// timeout before processing current data
			if ( temp.timeout < tempTime ) {
				// the timeout exceeded the completion time of the previous data
				if ( temp.timeout > endTime )
					endTime = temp.timeout;
					
				answers.addAbortJob(temp, endTime);  // add to the abort
			} // if
			
			// normal
			else {
				endTime = tempTime;
				answers.addDoneJob(temp, endTime);   // add to the done
			} // end else
		} // while (queue no empty)
		
		answers.putAll( fileName );  // input into file
	} // end CPU() 
	
	bool getSortedFile( string fileName ) {
		string file = "sorted"+fileName+".txt";
		if ( jobs.getAll( file ) )
			return true;
		else 
			return false;
		
	} // end getSortedFile()
	
	void printJobList() {  // delete and print list
		jobType temp;
		while(!jobs.isEmpty()) {
			jobs.nextJobAndDel(temp);
			cout << temp.OID << '\t';
			cout << temp.arrival << '\t';
			cout << temp.duration << '\t';
			cout << temp.timeout << '\t' << endl;
		}
	} // test the data
	
	void printAns() {
		answers.showAll();
	}
	
};


int main(void) {
	int command = 0;
	string preName = "";
	
	do {
		cout << "**** Simulate FIFO Queues by SQF *****" << endl;
		cout << "* 0. Quit                            *" << endl;
		cout << "* 1. Sort a file                     *" << endl;
		cout << "* 2. Simulate one FIFO queue         *" << endl;
		cout << "**************************************" << endl;	
		cout << "Input a command(0, 1, 2): ";
		cin >> command;
		cout << endl;
		string fileName = "";
		
		if ( command == 0 ) {
			break;
		} // end if(0)
		
		else if ( command == 1 ) {
            cout << "Input a file number: ";
            cin >> fileName;
            cout << endl;
            
            JobList myList;
            myList.setFileID(fileName);
			string file = "input"+fileName+".txt";
            if (myList.getAll( file )) {//  get the file name and store in the list
            	if ( myList.getSorted() ) {        // Sort the List by arrival
            		preName = fileName;
            		myList.showTime();
            		cout << endl << "See sorted" << fileName << ".txt" << endl;
            	}
        	}
        	else {
        		preName = fileName;
        		cout << "### input" << preName << ".txt does not exist! ###" << endl;
			}
        	
        } // end else if(1)
			
		else if ( command == 2 ) {
			
			Simulation simulation(1);
			
			if ( preName != "" ) {
				// execution 
				if ( simulation.getSortedFile(preName) ) {
					simulation.CPU( preName );
					cout << "The simulation is running..." << endl;
					cout << "See output"+preName+".txt" << endl;
				}
				else 
					cout << "### sorted" << preName << ".txt does not exist! ###" << endl;
			} // end if
			
			else {
				cout << "Input a file number: ";
				cin >> preName;
				cout << endl;
				// execution
				if ( simulation.getSortedFile(preName)) {
					simulation.CPU( preName );
					cout << "The simulation is running..." << endl;
					cout << "See output"+preName+".txt" << endl;
				}
				else 
					cout << "### sorted" << preName << ".txt does not exist! ###" << endl;
			} // end else
			
		} // end else if(2)
			
		else {
			if ( command != 1 && command != 2 ) cout << "command does not exist!" << endl;
		} // end else(other)
		
		cout << endl;
		
	} while ( command != 0 ) ;
		
	
	// ifstream inputFile("")
	// ofstream outputFile("output.txt")
	
	return 0;
} // end main()


