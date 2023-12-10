// �Ĥ@�� 11127212 ���դ� 11120406 �®f�� 
# include <iostream>
# include <vector>
# include <string.h>
# include <fstream>
# include <sstream>
# include <iomanip>
# include <stdlib.h>
# include <time.h>

using namespace std; 

typedef struct jT {
	int OID ;		// order identifier
	int arrival ;   // arrival time
	int duration ;  // job duration
	int timeout ;   // expire time
} jobType;

class JobList {
	vector<jobType> aList ;		// list of with four columns
	string fileID ;				// file identifier 
	
	void reset() {				// defintion : initial set up
		vector<jobType>(aList).swap(aList) ;
		fileID = "null" ;
	} // end reset
	
	void sortByarrival() {		// declaration: sort all by arrival time
		jobType temp ; // �ΨӫO�s��e���ƭ�
    	for (int gap = aList.size() / 2; gap > 0; gap /= 2) { // �ƺ��ƧǪk�}�l 
       		// �ϥδ��J�Ƨǹ�l�Ʋնi��Ƨ�
        	for (int i = gap; i < aList.size(); i++) {
            temp = aList[i];
            int j;
            	for (j = i; j >= gap && ( aList[j - gap].arrival > temp.arrival || 
            		( aList[j - gap].arrival == temp.arrival &&  aList[j - gap].OID > temp.OID )); j -= gap) {
               		aList[j] = aList[j - gap];
            	}
            aList[j] = temp;
        	}
    	} // for
       	
        
	} // sortByarrival 
	
	void showJobList() {		// desplay all job on screen
		cout << "\tOID\tArrival\tDuration\tTimeOut" << endl ;
		for ( int i = 0 ; i < aList.size() ; i ++ ) {
			cout << "(" << i+1 << ")\t" ;
			cout << aList[i].OID << "\t" ;
			cout << aList[i].arrival << "\t" ;
			cout << aList[i].duration << "\t" ;
			cout << aList[i].timeout << endl ;
		} // for
	}
	
	public:
		JobList(){reset();}		// constructor for initialization
		~JobList(){reset();}	// destructor for initialization
		jobType returnjob(int i) {
			return aList[i] ;
		}
		vector<jobType> returnaList() {	// return aList information
			return aList ;
		} // returnaList
		bool isEmpty(string filename) { // check whether it is empty or not
			fileID = filename ;
			filename = "input" + filename + ".txt" ;
			ifstream file(filename.c_str()) ;
			if ( !file.is_open()) {
				cout << "��󥴶}����" << endl ;
				return false ; 
			}
			
			file.seekg(0, ios::end) ;  // �P�_��󤺮e�O�_���� 
			if (!file.tellg()){
				cout << "\n### Get nothing from the file " << fileID <<" ! ###" << endl << endl ;
				return false ; 
			} // if
			else {
				return true ; 
			}

    		file.close();
		} // isEmpty
		bool getSorted() {				// ���ȤG�Ψ�AŪ��sorted.txt�ɮ� 
			string fileInfor ;
			string filename = "sorted" + fileID + ".txt" ;
			ifstream file(filename.c_str()) ;
			getline(file, fileInfor) ;
			
			if ( file.eof() ) {			// �P�_���S�����u�@�A�S���������There is no jobs in the file 
				cout << "\n### There is no jobs in the file sorted" << fileID <<".txt ! ###" << endl << endl ;
				return false ;				
			}
			while(true) {				// �}�lŪ�����R�u�@ 
				getline(file, fileInfor) ;
				if (file.eof()) break ;
				getSort(fileInfor) ; 
			
			} // while
		} // getSorted()
		bool isNull(string fileInfor) {
			
			if (fileInfor[0] < '0' || fileInfor[0] > '9') return true ;
			else return false ;
		}
		bool getAllSort() {				// ���Ȥ@�ΨӤ������ 
			double readtime, sorttime ;
			double readclock, sortclock ;
			clock_t start, end ;
			string fileInfor ;
			start = clock() ;
			string filename = "input" + fileID + ".txt" ;
			ifstream file(filename.c_str()) ;
			getline(file, fileInfor) ;			// Ū���Ĥ@����D 
			if ( file.eof() ) {
				cout << "\n### There is no jobs in the file input" << fileID <<".txt ! ###" << endl << endl ;
				return false ;
			}
			while(!file.eof()) {
				getline(file, fileInfor) ;
				if ( isNull(fileInfor) ) break ;
				getSort(fileInfor) ;
				
			}  // while
			showJobList() ;
			end = clock() ;
			readclock = double(end-start) ;
			readtime = (readclock/CLOCKS_PER_SEC) * 1000 ;
			start = clock() ;
			sortByarrival() ;
			end = clock() ;
			
			sortclock = double(end-start) ;
			sorttime = (sortclock/CLOCKS_PER_SEC) * 1000 ;
			
			cout << endl << "Reading data: " << readclock << " clocks (" << fixed << setprecision(2) << readtime << " ms)." << fixed << setprecision(0) << endl ;
			cout << "Sorting data: " << sortclock << " clocks (" << fixed << setprecision(2) << sorttime << " ms)." << fixed << setprecision(0) ;
			
			 			
			Filesave() ;
			
		} // getAllSort
		
		
		bool getSort(string fileInfor) {	// �N�ǤJ���r����R�A�ä�����C�����O 
			jobType temp ;
			int cindex = 0 ;
			
			temp.OID = getNum(cindex,fileInfor) ;
			
			temp.arrival = getNum(cindex,fileInfor) ;
			temp.duration = getNum(cindex,fileInfor) ;
			temp.timeout = getNum(cindex,fileInfor) ;
			
			aList.push_back(temp) ;
			
		} // getSort()
		
		int getNum(int & cindex, string fileInfor) {		// ��r�ꤤ���Ʀr���X�A�ç���cindex 
			for ( ; fileInfor[cindex] == ' ' || fileInfor[cindex]=='\t' ; cindex ++ ) {} // for
			string snum ;
		
			while(fileInfor[cindex]!=' '&& fileInfor[cindex]!='\t' && cindex < fileInfor.size()){
				snum += fileInfor[cindex] ;
				cindex ++ ;
				
			}
			
			return atoi(snum.c_str()) ;
			
		} // skipspace
	bool Filesave() {		// ���Ƽg�Jsorted.txt�ɮ׸� 
		string filename = "sorted" + fileID + ".txt" ;
		clock_t start, end ;
		double writetime, writeclock ;
		start = clock() ;
		ofstream outFile(filename.c_str()) ;
		if ( outFile.is_open()) {
			outFile << "OID\tArrival\tDuration\tTimeOut" << endl ;
			for ( int i = 0 ; i < aList.size() ; ++i) {
				outFile << aList[i].OID << "\t" ;
				outFile << aList[i].arrival << "\t" ;
				outFile << aList[i].duration << "\t" ;
				outFile << aList[i].timeout << endl ;	
			} // for
			outFile.close() ;
			end = clock() ;
			writeclock = double(end-start) ;
			writetime = (writeclock/CLOCKS_PER_SEC) * 1000 ;
			cout << endl << "Writing data: " << writeclock << " clocks (" << fixed << setprecision(2) << writetime << " ms)." << endl << endl ;			
			cout << "see " << filename << endl << endl ;
		} // if
		else {
			cout << "�L�k�}���ɮ�" << endl ; 
		} // else
		
	} // FileExist() 

		
};



class AnsList{
	typedef struct aT {
		int OID ;			// �u�@�s�� 
		int about ;			// �������ɶ� 
		int delay ;			// ����ɶ� 
	} aboutType;
	
	typedef struct dT {
		int OID ;			// �u�@�s�� 
		int departure ;		// �u�@�����ɶ� 
		int delay ;			// ����ɶ� 
	} doneType;
	
	vector<aboutType> aboutJobs ;	// list of abouted jobs with three columns
	vector<doneType> doneJobs ;		// list of done jobs with three columns
	float avgDelay ;				// average delay
	double successRate = 0.00 ;		// percentage of done jobs
	string fileID;					// fileNum 
	void showAboutJobs() {			// declaration: display abort jobs on screen
		cout << "\t[About Jobs]" << endl ;
		cout << "\tOID\tabort\tDelay" << endl ;
		for ( int i = 0 ; i < aboutJobs.size() ; i ++ ) {
			cout << "[" << i+1 << "]\t" ;
			cout << aboutJobs[i].OID << "\t" ;
			cout << aboutJobs[i].about << "\t" ;
			cout << aboutJobs[i].delay << "\t" << endl ;
		} // for
	} // showAboutJobs()
	
	void showDoneJobs() {			// declaration: display done jobs on screen
		cout << "\t[Jobs Done]" << endl ;
		cout << "\tOID\tDeparture\tDelay" << endl ;
		for ( int i = 0 ; i < doneJobs.size() ; i ++ ) {
			cout << "[" << i+1 << "]\t" ;
			cout << doneJobs[i].OID << "\t" ;
			cout << doneJobs[i].departure << "\t" ;
			cout << doneJobs[i].delay << "\t" << endl ;
		} // for
	} // showDoneJobs
	
	void computeStat(){				// declaration: compute the statistics
		for ( int i = 0 ; i < aboutJobs.size() ; i ++ ) {
			avgDelay = avgDelay + aboutJobs[i].delay;
		}
		for ( int i = 0 ; i < doneJobs.size() ; i ++ ) {
			avgDelay = avgDelay + doneJobs[i].delay;
		}	
		avgDelay = avgDelay/(aboutJobs.size() + doneJobs.size());
		successRate = 100 * float(doneJobs.size() ) / float(( aboutJobs.size() + doneJobs.size()));
	}
	void showStat(){	// declaration: display statistics on screen
		cout << "[Average Delay]" << fixed << setprecision(2) << avgDelay << "ms" << endl;
		cout << "[Success Rate]" << successRate << "%" << endl;
	}
public :
	AnsList(): avgDelay(0.0), successRate(0.0){

	}	 // constructor for initialization
	~AnsList(){
		vector<aboutType> ().swap(aboutJobs);
		vector<doneType> ().swap(doneJobs) ;
	}	// destructor for initialization
	void setFilename( string filename ) {	// copy fileID in simulation class
		fileID = filename ;
	}
	void showAll(){				// show all information on screen
		showAboutJobs();
		showDoneJobs();
		computeStat() ;
		showStat();
	}
	void addAboutJob(int OID, int about, int delay) {	// add abort job to vector
		aboutType temp ;
		temp.OID = OID ;
		temp.about = about ;
		temp.delay = delay ;
		aboutJobs.push_back(temp) ;
	} // addAboutJob ;
	void addDoneJob(int OID, int departure, int delay) {	// add done job to vector
		doneType temp ;
		temp.OID = OID ;
		temp.departure = departure ;
		temp.delay = delay ;
		doneJobs.push_back(temp) ;
	} // addDoneJob
	
	void putAll(){										// save all information to ouput.txt 
		string filename = "output" + fileID + ".txt" ;
		ofstream outfile(filename.c_str());
		if( outfile.is_open() ){
			outfile << "\t[About Jobs]" << endl ;
			outfile << "\tOID\tabort\tDelay" << endl ;
				for ( int i = 0 ; i < aboutJobs.size() ; i ++ ) {
					outfile << "[" << i+1 << "]\t" ;
					outfile << aboutJobs[i].OID << "\t" ;
					outfile << aboutJobs[i].about << "\t" ;
					outfile << aboutJobs[i].delay << "\t" << endl ;
				} 
			outfile << "\t[Jobs Done]" << endl ;
			outfile << "\tOID\tDeparture\tDelay" << endl ;
				for ( int i = 0 ; i < doneJobs.size() ; i ++ ) {
					outfile << "[" << i+1 << "]\t" ;
					outfile << doneJobs[i].OID << "\t" ;
					outfile << doneJobs[i].departure << "\t" ;
					outfile << doneJobs[i].delay << "\t" << endl ;
				} 
			computeStat() ;
			outfile << "[Average Delay]\t" << fixed << setprecision(2) << avgDelay << "ms" << endl;
			outfile << "[Success Rate]\t" << successRate << "%" << endl;
			outfile.close();
		} 
		else{
			cout << "�L�k�}���ɮ�" << endl ; 
		}
	}
};

class Ansdouble{
	typedef struct aT {
		int OID ;			// �u�@�s�� 
		int CID ;			// CPU �s�� 
		int about ;			// �������ɶ� 
		int delay ;			// ����ɶ� 
	} aboutType;
	
	typedef struct dT {
		int OID ;			// �u�@�s�� 
		int CID ;			// CPU �s�� 
		int departure ;		// �u�@�����ɶ� 
		int delay ;			// ����ɶ� 
	} doneType;
	
	vector<aboutType> aboutJobs ;	// list of abouted jobs with three columns
	vector<doneType> doneJobs ;		// list of done jobs with three columns
	float avgDelay ;				// average delay
	double successRate = 0.00 ;		// percentage of done jobs
	string fileID;					// fileNum 
	
	void computeStat(){				// declaration: compute the statistics
		for ( int i = 0 ; i < aboutJobs.size() ; i ++ ) {
			avgDelay = avgDelay + aboutJobs[i].delay;
		}
		for ( int i = 0 ; i < doneJobs.size() ; i ++ ) {
			avgDelay = avgDelay + doneJobs[i].delay;
		}	
		avgDelay = avgDelay/(aboutJobs.size() + doneJobs.size());
		successRate = 100 * float(doneJobs.size() ) / float(( aboutJobs.size() + doneJobs.size()));
	}
	void showStat(){	// declaration: display statistics on screen
		cout << "[Average Delay]" << fixed << setprecision(2) << avgDelay << "ms" << endl;
		cout << "[Success Rate]" << successRate << "%" << endl;
	}
public :
	Ansdouble(): avgDelay(0.0), successRate(0.0){

	}	 // constructor for initialization
	~Ansdouble(){
		vector<aboutType> ().swap(aboutJobs);
		vector<doneType> ().swap(doneJobs) ;
	}	// destructor for initialization
	void setFilename( string filename ) {	// copy fileID in simulation class
		fileID = filename ;
	}

	void addAboutJob(int OID, int CID, int about, int delay) {	// add abort job to vector
		aboutType temp ;
		temp.OID = OID ;
		temp.about = about ;
		temp.delay = delay ;
		temp.CID = CID ;
		aboutJobs.push_back(temp) ;
	} // addAboutJob ;
	void addDoneJob(int OID, int CID,int departure, int delay) {	// add done job to vector
		doneType temp ;
		temp.OID = OID ;
		temp.CID = CID ;
		temp.departure = departure ;
		temp.delay = delay ;
		doneJobs.push_back(temp) ;
	} // addDoneJob
	
	void putAll(){										// save all information to ouput.txt 
		string filename = "double" + fileID + ".txt" ;
		ofstream outfile(filename.c_str());
		if( outfile.is_open() ){
			outfile << "\t[About Jobs]" << endl ;
			outfile << "\tOID\tCID\tabort\tDelay" << endl ;
				for ( int i = 0 ; i < aboutJobs.size() ; i ++ ) {
					outfile << "[" << i+1 << "]\t" ;
					outfile << aboutJobs[i].OID << "\t" ;
					outfile << aboutJobs[i].CID << "\t" ;
					outfile << aboutJobs[i].about << "\t" ;
					outfile << aboutJobs[i].delay << "\t" << endl ;
				} 
			outfile << "\t[Jobs Done]" << endl ;
			outfile << "\tOID\tCID\tDeparture\tDelay" << endl ;
				for ( int i = 0 ; i < doneJobs.size() ; i ++ ) {
					outfile << "[" << i+1 << "]\t" ;
					outfile << doneJobs[i].OID << "\t" ;
					outfile << doneJobs[i].CID << "\t" ;
					outfile << doneJobs[i].departure << "\t" ;
					outfile << doneJobs[i].delay << "\t" << endl ;
				} 
			computeStat() ;
			outfile << "[Average Delay]\t" << fixed << setprecision(2) << avgDelay << "ms" << endl;
			outfile << "[Success Rate]\t" << successRate << "%" << endl;
			outfile.close();
		} 
		else{
			cout << "�L�k�}���ɮ�" << endl ; 
		}
	}
};


 
class JobQueue{
	private :
		struct QueueNode{		// �Ы� QueueNode ��Ƶ��c�h�x�s�ƶ����u�@ 
			jobType item;		// �u�@����� 
			QueueNode *next;	// ���Ы��V�U�@����� 
		};
		
		QueueNode *frontPtr = NULL;	// ���V��C���e�� 
		QueueNode *tailPtr = NULL ;	// ���V��C����� 
	
	public:
		int avail ;							// the time when CPU available
		int qSize = 0, qMax = 3 ;			// current size & max size of queue
		/*JobQueue() ;
		~JobQueue();*/
		
		int length() const {				// return queue size ;
			return qSize ;
		} // length()
		bool isFull() {						// check the queue is full, or not 
			if (qSize >= qMax) return true ;
			else return false ;
		} // isFull()
		
		bool isEmpty() const{				// check the queue is empty, or not
			return frontPtr == NULL;
		}
		
		void enqueue(jobType& newItem){		// put the job in queue
			QueueNode *newPtr = new QueueNode;
			newPtr->item = newItem;
			newPtr->next = NULL ;
			qSize ++ ;
			
			if(isEmpty()){
				frontPtr = newPtr;
			} // if
			else{
				tailPtr->next = newPtr; 
			} 
			tailPtr = newPtr;
		}
		
		void dequeue(jobType & queueFront){	// take out the job from queue
			QueueNode *deletePtr ;
			queueFront = frontPtr->item ;
			deletePtr = frontPtr ;
			frontPtr = frontPtr->next ;
			qSize -- ;
			delete deletePtr ;

		} // dequeue
		
		void getFront(jobType & queueFront){// get front information of queue
			queueFront = frontPtr->item ;
		}
		
		void clearQ() {						// clear queue's all information
			jobType temp ; 
			if ( !isEmpty() ) {
				dequeue(temp) ;
			} // if
		}
	

};
class AutoSimulate {
	typedef struct pT {
		bool isprocess = false ;		// order identifier
		int processtime = 0 ;	// job �w�g���檺�ɶ� 
		jobType currentJob ;			// ���b���檺job 
		JobQueue allQ ;			// a set of queues
	} processType;
	
	JobList jobs ;			// a list of jobs
	string fileID ;			// �{�b���b���檺�ɮת�fileID 
	Ansdouble answers ;		// a set of answers
	int CPU ;
	int qNum ;				// number of queues
	int nowtime = 0 ;		// ��U���ɶ� 
	int nowinfor = 0 ;		// ��U�b�B�z����� 
	int jobsize ;			// ���h�֭�job
	bool needstop = false ; 
	
	jobType headJob ;		// ��C�e��job
	
	vector<processType> queue ;	

	//bool is
	bool isAllempty() {
		for (int i = 0 ; i < CPU ; i ++) {
			if (!queue[i].allQ.isEmpty()) return false ;
		} // for
		return true ;		
	} // isAllempty
	bool isAllprocess() {
		
		for (int i = 0 ; i < CPU ; i ++) {
			
			if (queue[i].isprocess == false) {return false ;}
		} // for
		
		return true ;
	}
	bool isfinish() {		// �P�_�᭱�٦��S��job�n�i�J��C 
		if (nowinfor >= jobsize ) return true ;
		else return false ;
	} // isfinish()
	bool isAllFull() {		// �P�_�O�_�����F 
		for ( int i = 0 ; i < CPU ; i ++ ) {
			if (queue[i].allQ.length() < 3 ) return false ; 
		} // for
		
		return true ;
	} // isAllFull
	void cancel() {			// ��C���F�ӭn�����u�@����� 
		answers.addAboutJob(headJob.OID, 0 , nowtime, 0 ) ;
		nowinfor ++ ;
		headJob = jobs.returnjob(nowinfor) ;
	}			
	void delQ(jobType canceljob, int CID) {	// �]���O�ɦӭn���������								// declaration : delete an old job from a queue
		int delay = nowtime - canceljob.arrival ;
		answers.addAboutJob(canceljob.OID, CID, nowtime, delay ) ;
	} // delQ()
	void inQ() {					// �i�H�i�Jqueue����� 
		//cout << headJob.arrival ;
		int minq = queue[0].allQ.length() ;
		int putN = 0 ;
		for ( int i = 0 ; i < CPU ; i ++ ) {
			if (queue[i].allQ.length() < minq) {
			putN = i ;			
			minq = queue[i].allQ.length() ;}
		} // for
		
		queue[putN].allQ.enqueue(headJob) ;

		nowinfor ++ ;
		headJob = jobs.returnjob(nowinfor) ;
			
	} // inQ
	
	void Qtoprocess(int N) {				// �q��C���X�ö}�l���� 
		jobType getjob ;
		queue[N].isprocess = false ;		
		while (!queue[N].allQ.isEmpty()) {	// ��C�����Ť~�|�q��C���X���� 
			queue[N].allQ.dequeue(getjob) ;
			
			if ( nowtime >= getjob.timeout ) {	// �P�_�O�_���O�� 
				//cout << getjob.OID << " cancel" << endl ;
				delQ(getjob,N+1) ;		// cancel the job
			} // if
			else {						// �u�@�S���O�ɥi���� 
				//cout << getjob.OID << " next" << endl ;
				queue[N].currentJob = getjob ;
				queue[N].processtime = 0 ;
				queue[N].isprocess = true ;
				break ;
			} // else
		} 
		
		if (queue[N].allQ.isEmpty() && !queue[N].isprocess) { // �p�G�S���u�@�i����A��C�]�O�Ū��A��h1���ɶ��t 

		needstop = true ;}
	}
	void success(jobType job,int CID) {				// ���榨�\��job��J Done �� List �x�s 
		int delay ;
		delay = nowtime - job.duration - job.arrival ;
		answers.addDoneJob(job.OID,CID, nowtime, delay) ;
	} // execute()
	void timeout(jobType timeout, int CID) {	// �B�z�����@�b�ӹO�ɪ�job
	 
		int delay ;
		delay = nowtime - timeout.arrival ;
		answers.addAboutJob(timeout.OID,CID, nowtime, delay) ;
		
	} // timeout()

	
	int giveJob() {
		for ( int i = 0 ; i < CPU ; i ++ ) {
			if(queue[i].isprocess == false) {
				queue[i].isprocess = true;
				queue[i].processtime = 0 ;	
				return i ;
			}
		}
		return 0 ;
	} // giveJob()
	bool haveJob() {
		for ( int i = 0 ; i < CPU ; i ++ ) {
			if(queue[i].isprocess == true) {
				return true ;
			} // if		
		} // for
		return false ;
	}
	
	void time_1 (int time) {
		for ( int i = 0 ; i < CPU ; i ++ ) {
			if(queue[i].isprocess == true) {
				queue[i].processtime += time  ;
			} // if		
		} // for
	} // time_1

	public :
		AutoSimulate(JobList aList, int N, int C, string filename) : jobs(aList), qNum(N), CPU(C) {	// copy constructor
			jobsize = jobs.returnaList().size() ;
			fileID = filename ;
			processType pro ;
			for ( int i = 0 ; i < CPU ; i ++ ) {
				queue.push_back(pro) ;
			} // for
		} // end constructor
		
		void excute() {		// the main excute of simulation
			 
			headJob = jobs.returnjob(nowinfor) ;
			answers.setFilename(fileID) ;

			while(true) {	// run simulation

				
				
				if ( nowinfor == jobsize && !haveJob() && isAllempty() ) break ;	// �p�G�����]���A����simulation ���B�� 
				if (!isAllprocess() && !isfinish() && nowtime <= headJob.arrival) {	// ��C���ũM�S��job�b�B��ɡA��������U�@�Ӧ��u�@�i�Ӫ�arrival 
					while (!isAllprocess() && !isfinish() && nowtime == headJob.arrival) {
					
						
						int time_p = headJob.arrival - nowtime ;
						time_1(time_p);
						//nowtime = headJob.arrival ;
						queue[giveJob()].currentJob = headJob ;			
						nowinfor ++ ;
						headJob = jobs.returnjob(nowinfor) ;
					} // while
						
				} 
						// �w�g��job�b����ɪ��B�z 
				
					for ( int i = 0 ; i < CPU ; i ++ ) {
						if ( !queue[i].isprocess ) continue ;
						
						if ( nowtime >= queue[i].currentJob.timeout && queue[i].processtime < queue[i].currentJob.duration ) {
						// �p�G����ɶ��w�g�W�L�Ӥu�@��timeout�A�����Ӥu�@ 
						
							timeout(queue[i].currentJob, i+1) ;
							queue[i].processtime = 0 ;
							Qtoprocess(i) ;
						}
						else if (queue[i].processtime == queue[i].currentJob.duration) {
						 // �p�G����ɶ����job�һݪ��ɶ��A�h�⧹���Ӥu�@ 
						 
							success(queue[i].currentJob,i+1) ;
							Qtoprocess(i) ;
						} // if
						
						queue[i].processtime ++;	// job���p�� + 1
						
					} // for
				
				
				
				while ( !isfinish() && headJob.arrival == nowtime && isAllprocess() ){
					// ���u�@�b����A�P�_�O�_�i�i��C 
				
					if ( isAllFull() ) {	// ��C�w�g���A�����Ӥu�@ 
					
						cancel() ;
					}
					else {
						
						// ��C�����A�u�@��J��C�ƶ� 
						inQ() ;
						//cout << "5656" ;
					}
				} // while
				if ( needstop ) {
					nowtime -- ;
					time_1(-1) ;
				}
				nowtime ++ ;
				
				needstop = false ;
			} // while
			
			cout << endl << "The simulation is running..." << endl ;
			cout << "See double" << fileID << ".txt" << endl << endl ;
			answers.putAll() ;
			
		}	
};
class Simulation {
	JobList jobs ;			// a list of jobs
	JobQueue allQ ;			// a set of queues
	string fileID ;			// �{�b���b���檺�ɮת�fileID 
	AnsList answers ;		// a set of answers
	int qNum ;				// number of queues
	int nowtime = 0 ;		// ��U���ɶ� 
	int nowinfor = 0 ;		// ��U�b�B�z����� 
	int jobsize ;			// ���h�֭�job 
	int processtime = 0 ;	// job �w�g���檺�ɶ� 
	jobType headJob ;		// ��C�e��job
	jobType currentJob ;	// ���b���檺job 
	bool isProcess = false ;// �O�_��job�b���� 
	//bool is
	bool isfinish() {		// �P�_�᭱�٦��S��job�n�i�J��C 
		if (nowinfor >= jobsize ) return true ;
		else return false ;
	} // isfinish()
	void cancel() {			// ��C���F�ӭn�����u�@����� 
		answers.addAboutJob(headJob.OID, nowtime, 0 ) ;
		nowinfor ++ ;
		headJob = jobs.returnjob(nowinfor) ;
	}			
	void delQ(jobType canceljob) {	// �]���O�ɦӭn���������								// declaration : delete an old job from a queue
		int delay = nowtime - canceljob.arrival ;
		answers.addAboutJob(canceljob.OID, nowtime, delay ) ;
	} // delQ()
	void inQ() {					// �i�H�i�Jqueue����� 
		//cout << headJob.arrival ;
		allQ.enqueue(headJob) ;
		nowinfor ++ ;
		headJob = jobs.returnjob(nowinfor) ;
			
	} // inQ
	void Qtoprocess() {				// �q��C���X�ö}�l���� 
		jobType getjob ;

		isProcess = false ;			
		
		while (!allQ.isEmpty()) {	// ��C�����Ť~�|�q��C���X���� 
			allQ.dequeue(getjob) ;
			
			if ( nowtime >= getjob.timeout ) {	// �P�_�O�_���O�� 

				delQ(getjob) ;		// cancel the job
			} // if
			else {						// �u�@�S���O�ɥi���� 

				currentJob = getjob ;
				processtime = 0 ;
				isProcess = true ;
				break ;
			} // else
		} 
		
		if (allQ.isEmpty() && !isProcess) { // �p�G�S���u�@�i����A��C�]�O�Ū��A��h1���ɶ��t 

		nowtime -- ;}
	}
	void success(jobType job) {				// ���榨�\��job��J Done �� List �x�s 
		int delay ;
		delay = nowtime - job.duration - job.arrival ;
		answers.addDoneJob(job.OID, nowtime, delay) ;
	} // execute()
	void timeout(jobType timeout) {	// �B�z�����@�b�ӹO�ɪ�job
	 
		int delay ;
		delay = nowtime - timeout.arrival ;
		answers.addAboutJob(timeout.OID, nowtime, delay) ;
		
	} // timeout()
	public :
		Simulation(JobList aList, int N, string filename) : jobs(aList), qNum(N) {	// copy constructor
			jobsize = jobs.returnaList().size() ;
			fileID = filename ;
		} // end constructor
		
		void excute() {		// the main excute of simulation
			headJob = jobs.returnjob(nowinfor) ;
			answers.setFilename(fileID) ;

			while(true) {	// run simulation


				if ( nowinfor == jobsize && !isProcess && allQ.isEmpty() ) break ;	// �p�G�����]���A����simulation ���B�� 
				if (!isProcess && !isfinish() && nowtime <= headJob.arrival) {	// ��C���ũM�S��job�b�B��ɡA��������U�@�Ӧ��u�@�i�Ӫ�arrival 

					nowtime = headJob.arrival ;
					currentJob = headJob ;			
					nowinfor ++ ;
					headJob = jobs.returnjob(nowinfor) ;
					isProcess = true ;				// �}�l����job 
					processtime = 0 ;	
				} // if
				else {		// �w�g��job�b����ɪ��B�z 

					processtime ++ ;	// job���p��+1 
					if ( nowtime >= currentJob.timeout && processtime < currentJob.duration ) {
						// �p�G����ɶ��w�g�W�L�Ӥu�@��timeout�A�����Ӥu�@ 
						timeout(currentJob) ;
						processtime = 0 ;
						Qtoprocess() ;
					}
					else if (processtime == currentJob.duration) {
						 // �p�G����ɶ����job�һݪ��ɶ��A�h�⧹���Ӥu�@ 
						success(currentJob) ;
						Qtoprocess() ;
					} // if
					
				} // else
				
				
				while ( !isfinish() && headJob.arrival == nowtime && isProcess ){
					// ���u�@�b����A�P�_�O�_�i�i��C 
					if ( allQ.isFull() ) {	// ��C�w�g���A�����Ӥu�@ 
				
						cancel() ;
					}
					else {
						// ��C�����A�u�@��J��C�ƶ� 
						inQ() ;
					}
				} // if
				
				nowtime ++ ;
				
			} // while
			
			cout << endl << "The simulation is running..." << endl ;
			cout << "See output" << fileID << ".txt" << endl << endl ;
			answers.putAll() ;
			
		}

};
bool FileExist(string filename, string command) ;
bool SortFile(string filename) ;
bool Simulate(string filename) ;
bool SimAuto(string filename) ;

int main () { 
	string command = "0"; // �R�O 
	
	do {
		cout << "**** Simulate FIFO Queue by SQF *****" << endl ;
		cout << "* 0. Quit                           *" << endl ;
		cout << "* 1. Sort a file                    *" << endl ;
		cout << "* 2. Simulate one FIFO queue        *" << endl ;
		cout << "* 3. Simulate muto FIFO queue       *" << endl ;
		cout << "*************************************" << endl ;
		cout << "Input a command(0, 1, 2, 3):" ;
		cin >> command ;
		if ( command == "0" ) { cout << endl << "�{������" << endl ; break ; }
		if ( command == "1" || command == "2" || command == "3" ) {
			string filename ;
			cout << "\nInput a file number:" ;
			cin >> filename ;
			
			if ( ! FileExist(filename,command)){
				continue ;
			} // if
			if ( command == "1" ) SortFile(filename) ;
			if ( command == "2" ) Simulate(filename) ;
			if ( command == "3" ) SimAuto(filename) ; 
		} // end if
		
		else 
			cout << endl << "command error!!" << endl << endl ;
		
	} while(command != "0") ;
	
	
} // main()
bool SimAuto(string filename) {
	JobList joblist ;
	int CPU ;
	if ( ! joblist.isEmpty(filename) ) {	// �P�_�ɮ׬O�_���� 
		return false ;
	} // if
	if ( ! joblist.getSorted() ) {		
		return false ;	
	} // if	
	//cout << "How many CPU :" ;
	//cin >> CPU ;
	AutoSimulate autosim(joblist, 3, 2, filename) ;
	autosim.excute() ;
} // SimAuto

bool Simulate(string filename) {	// ���ȤG������ 
	JobList joblist ;
	if ( ! joblist.isEmpty(filename) ) {	// �P�_�ɮ׬O�_���� 
		return false ;
	} // if
	if ( ! joblist.getSorted() ) {		
		return false ;	
	} // if

	Simulation simulation(joblist, 3, filename) ;	// simulation ��l�� 
	simulation.excute() ;		// simulation �}�l���� 
	
	
} // Simulate

bool SortFile(string filename){
	JobList joblist ;
	if ( ! joblist.isEmpty(filename) ) {	// ���Ȥ@������ 
		return false ;
	}
	if ( ! joblist.getAllSort() ) {		 
		return false ;	
	}
	
}

bool FileExist(string filename, string command) {
	if (command == "1") {
		filename = "input" + filename + ".txt" ;
	}
	else if ( command == "2" || command == "3" ) {
		filename = "sorted" + filename + ".txt" ;
	}
	fstream file(filename.c_str()) ;
	if ( ! file.good()) {     // if the file can open
		cout << "\n### "<< filename << " does not exist! ###" << endl << endl ;
		return false ;
	} // if
	else {
		file.close() ;
		return true ; 
	} // else
} // FileExist()

