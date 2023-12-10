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
		
		bool getAll(string fileNumber) {  //¨ú±o¸ê®Æ 
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

        	outFile << "[Average Delay]\t" << setprecision(4) << averageDelay << " ms" << endl;

         	float successRate = (float)doneList.size() / (abortList.size() + doneList.size()) * 100.0;
         	outFile << "[Success Rate]\t" << setprecision(4) << successRate << " %" << endl;

         	outFile.close();
         	cout << "The simulation is running..." << endl;
         	cout << "See " <<  outputFileName << endl;
         	
         	abortList.clear();   
         	doneList.clear();
         	allDelayTime = 0;
    	}
};  


class Simulation {
	private:
		JobList jobList;
		AnsList answers;
		JobQueue queue;
		int time;
		
		 
	public:
		Simulation(){
			time = 0;
		}
		
		bool openfile(string fileNumber) {
			return jobList.getAll(fileNumber);
		}
		void processing(){
			bool processing = false;
			JobType cpu, nextJob;
			int completeTime;
			bool jobListIsEmpty = false;
			
			
			while(!jobList.isEmpty() || !queue.isEmpty() || processing) {	// ·íjobList©Îqueue¤ºÁÙ¦³job 
				while(!processing){					// ·í¨S¦³¥¿¦b³B²zªºjob 
					if(!queue.isEmpty()){				// ¦pªGqueue¤ºÁÙ¦³job ->¨S¦³¥¿¦b³B²zªºjob ¥B¦î¦C¤º¦³job 
						queue.deQueue(cpu);						// ±Nqueue deQueue ¨Ã©ñ¤JCPU³B²z 
						completeTime = time + cpu.duration;
						if(cpu.timeout <= time)					// ÀË¬d±q¦î¦C¨ú¥X¬O§_TimeOut 
							answers.addAbortJob(cpu.oid, time, time-cpu.arrival);
						
						else{
							processing = true;
						}
		
					}
					
					else if(!jobList.isEmpty())	{		// ¦pªGqueue¤ºµLjob¥BjobList¤ºÁÙ¦³job ->¨S¦³¥¿¦b³B²zªºJOB ¥B¦î¦C¬°ªÅ 
						jobList.nextJob(cpu);					// ±N¤U­Ójob©ñ¤JCPU³B²z 
						jobList.deleteJob();
						time = cpu.arrival;						// §â®É¶¡ª½±µ§ï¬°¤U­Ójobªº®É¶¡ 
						completeTime = time + cpu.duration;
						processing = true;
					}
					
					else	// µL¥¿¦b³B²zªºjob¡BjobList©Mqueue¤º¬ÒµLjob 
						return; 
				}
				
				if(time == completeTime) {		// ·íjob³B²z§¹¦¨ 
					answers.addDoneJob(cpu.oid, time, time - cpu.duration - cpu.arrival);
					processing = false;
				}
				
				else if(time == cpu.timeout) { 	// °õ¦æ¤¤³~µo¥Í¹O®É 
					answers.addAbortJob(cpu.oid, time, time - cpu.arrival);
					processing = false;
				}
				
				else if(!jobList.isEmpty()) { // ¦³¥¿¦b°õ¦æªºjob ÀË¬d¤U­Ójobªº©è¹F®É¶¡ 
					jobList.nextJob(nextJob);
					while(nextJob.arrival == time && !jobListIsEmpty ) {	// ·ínextJobªºarrival¬°time 
						if(queue.isFull())										// ¦pªG¦î¦Cº¡¤F 
							answers.addAbortJob(nextJob.oid, time, 0);
						else													// ¦î¦CÁÙ¦³ªÅ¶¡«henQueue 
							queue.enQueue(nextJob);
						jobList.deleteJob();
						jobListIsEmpty = jobList.nextJob(nextJob);
					}
					
					time++;
				}
				
				else		// ¦³job¥¿¦b³B²z ¥BjobList¬°ªÅ 
					time++;
			}


		}
	
		void writeAllAsAFile(string fileNumber) {
			answers.putAll(fileNumber);
		}
};

void processing(string fileName);

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
        cout << "**************************************" << endl;
        cout << "Input a command(0, 1, 2): ";

        cin >> command;
        cin.ignore(MAX_LEN, '\n');

        switch(command){
            case 0: break;
            case 1:	fileNumber = sorting(); 
            	check = true;
				break;
            case 2:
            	if (check) 
            		processing(fileNumber);
				else {
					cout << "Input a file number: ";
					getline(cin, fileNumber);
                	processing(fileNumber);
				}
                break;
                
            default:cout << endl << "Command does not exist!" << endl;
        }
    } while(command != 0);
    system("pause");
    return 0; 
}

void processing(string fileNumber) {
	Simulation simulation;
    if(!simulation.openfile(fileNumber)) {
    	cout << "### sorted" << fileNumber << ".txt does not exist! ###" << endl;
    	return;
	}
	
    simulation.processing();
    simulation.writeAllAsAFile(fileNumber);
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
			while ( getline( file, line ) ) {  // getline•¸
				int space = 0;
				if ( firstline == false ) {  // ²Ä¤@¦æ
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
