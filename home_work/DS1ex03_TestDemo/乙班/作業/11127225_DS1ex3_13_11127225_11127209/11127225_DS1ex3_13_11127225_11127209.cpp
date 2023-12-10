// 中原大學 資訊二乙 Data Structure Team13 11127225 賴枋佑  11127209 郭安峻  Excerise03

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h> 
#include <iomanip> 
using namespace std;

class Input{ 

public:
	
	static int input_command() {  
		
		string input_str;		
		cin >> input_str;

		if (input_str.length() == 1)  
			if (input_str[0] == '0' or input_str[0] == '1' or input_str[0] == '2')
				return int(input_str[0] - '0');
			
		return -1; 
		
	} // input_command()
		
}; // class Input

class Ajob{

public:
	
	string OID;
	int Arr; // Arrival
	int Dur; // Duration
	int TO; // TimeOut
	
	Ajob(string tOID, int tArr, int tDur, int tTO){
		
		OID = tOID;
		Arr = tArr;
		Dur = tDur;
		TO = tTO;
			
	}
	
	Ajob(){
		
		OID = "-1";
		Arr = 0;
		Dur = 0;
		TO = 0;		
		
	}
	
	void init(){
		
		OID = "-1";
		Arr = 0;
		Dur = 0;
		TO = 0;	
		
	}
};

class Aabortjob{

public:
	
	string OID;
	int Abort; 
	int Delay; 
	
	Aabortjob(string tOID, int tAbort, int tDelay){
		
		OID = tOID;		
		Abort = tAbort;
		Delay = tDelay;

	}
	
	Aabortjob(){
		
		OID = "-1";			
		Abort = 0;
		Delay = 0;	
		
	}
};

class Adonejob{

public:
	
	string OID;
	int Departure;
	int Delay; 
	
	Adonejob(string tOID, int tDeparture, int tDelay) {

		OID = tOID;		
		Departure = tDeparture;
		Delay = tDelay;

			
	}
	
	Adonejob(){
		
		OID = "-1";		
		Departure = 0;
		Delay = 0;	
		
	}
};

class JobQueue{
	
private:
	struct Node { 
		Ajob job;
		Node *next;
	};

public:
	
	Node *front,*back;
	int size ;
	
	bool pop(Ajob &Rejob) {
		
		if(front == NULL)
			return 0;
	
		Node* temp = front;
		Rejob = front -> job;
		front = front -> next;
		
		delete temp;	
		if (front == NULL)
			back = NULL;
			
		size --;
		return 1;		
		
	}
	
	bool pop() {
		
		if(front == NULL)
			return 0;
	
		Node* temp = front;
		front = front -> next;
		
		delete temp;	
		if (front == NULL)
			back = NULL;
			
		size --;
		return 1;		
		
	}
	
	void push(Ajob Injob){

		Node* newNode = new Node;
		newNode -> job.OID = Injob.OID;
		newNode -> job.Arr = Injob.Arr;
		newNode -> job.Dur = Injob.Dur;
		newNode -> job.TO = Injob.TO;	
		newNode -> next = NULL;
		
		if (back == NULL) {		
			front = newNode;
			back = newNode;
		}
		else {
			back -> next = newNode;
			back = back -> next;
		}
		size ++; 

	}
	
	JobQueue (){
		front = NULL;
		back = NULL;
		size = 0;
	}
	
	~JobQueue(){
		while ( pop () );	
	}
	
};

class Jobs{

public:
	
	vector <Ajob> jobVec;
	
	bool readJobsFromFile( string filenum , int &ReadingTime ) {
		
		clock_t start = clock();
		string filename = "input" + filenum + ".txt";
		ifstream infile(filename);
		if (!infile.is_open()) { // 如果檔案無法開啟，輸出錯誤訊息後回傳 0 
            cout << filename  <<" does not exist!"<< "\n";
            return 0;
        }
        string nameCheck;
		infile >> nameCheck;
		if ( nameCheck != "OID" ) {
            cout << "An error occurred while inputing from file: " << filename << "\n";
            infile.close();
            return 0;
		}
		infile >> nameCheck;
		if ( nameCheck != "Arrival" ) {
            cout << "An error occurred while inputing from file: " << filename << "\n";
            infile.close();
            return 0;
		}
		infile >> nameCheck;
		if ( nameCheck != "Duration" ) {
            cout << "An error occurred while inputing from file: " << filename << "\n";
            infile.close();
            return 0;
		}
		infile >> nameCheck;
		if ( nameCheck != "TimeOut" ) {
            cout << "An error occurred while inputing from file: " << filename << "\n";
            infile.close();
            return 0;
		}
		string tOID;
		int tArr, tDur, tTO;
		
		while (infile >> tOID) {
			
			if(infile >> tArr >> tDur >> tTO )
				jobVec.push_back( Ajob( tOID, tArr, tDur, tTO ) );
			else {
            	cout << "An error occurred while inputing from file: " << filename << "\n";
            	infile.close();
            	return 0;
			}
		}
		
		
		infile.close();
		clock_t end = clock();
		ReadingTime = end - start;
		
		cout << "OID\tArrival\tDuration\tTimeOut\n";
		
		for( auto oneJob: jobVec )
			cout << oneJob.OID <<"\t" << oneJob.Arr << "\t" << oneJob.Dur << "\t" << oneJob.TO <<"\n";
		
		return 1;
		
	}
	
	bool ShellSortJobsAndSaveFile( string filenum , int &SortingTime, int &WritingTime) {
		
		string sorted_filename = "sorted" + filenum + ".txt";
		ofstream outfile(sorted_filename);

		if (!outfile.is_open()) {
			cout << "Can't write file: '" << sorted_filename  <<" !!!\n";
			return 0;
		}

		clock_t start = clock();
        // Shell sort
        for ( int n = jobVec.size() / 2 ; n > 0 ; n = n / 2 ) {

    		for ( int i = n; i < jobVec.size() ; i++ ) {

      			int loc = i ;
      			Ajob temp = jobVec[i] ;
      			
      			while ( loc >= n && (jobVec[ loc-n ].Arr > temp.Arr || ( jobVec[ loc-n ].Arr == temp.Arr && jobVec[ loc-n ].OID > temp.OID ) ) ) {
					
          			jobVec[ loc ] = jobVec[ loc-n ] ;
          			loc = loc - n;
    
      			}

      			jobVec[loc] = temp ;

    		}
  		}
        
		
		clock_t end = clock();    

		SortingTime = end - start;

        start = clock(); 
		
		outfile << "OID\tArrival\tDuration\tTimeOut\n";
        for( auto oneJob: jobVec )
			outfile << oneJob.OID <<"\t" << oneJob.Arr << "\t" << oneJob.Dur << "\t" << oneJob.TO <<"\n";
			
		
		outfile.close();
		end = clock();
		
		WritingTime = end - start;
		
		return 1;
	}
	
	bool readSortedJobs(string filenum){
		
		string filename = "sorted" + filenum + ".txt";
		ifstream infile(filename);
		if (!infile.is_open()) { // 如果檔案無法開啟，輸出錯誤訊息後回傳 0 
            cout << filename  <<" does not exist!"<< "\n";
            return 0;
        }
        string nameCheck;
		infile >> nameCheck;
		if ( nameCheck != "OID" ) {
            cout << "An error occurred while inputing from file: " << filename << "\n";
            infile.close();
            return 0;
		}
		infile >> nameCheck;
		if ( nameCheck != "Arrival" ) {
            cout << "An error occurred while inputing from file: " << filename << "\n";
            infile.close();
            return 0;
		}
		infile >> nameCheck;
		if ( nameCheck != "Duration" ) {
            cout << "An error occurred while inputing from file: " << filename << "\n";
            infile.close();
            return 0;
		}
		infile >> nameCheck;
		if ( nameCheck != "TimeOut" ) {
            cout << "An error occurred while inputing from file: " << filename << "\n";
            infile.close();
            return 0;
		}
		string tOID;
		int tArr, tDur, tTO;
	
		while (infile >> tOID) {
			
			if(infile >> tArr >> tDur >> tTO )
				jobVec.push_back( Ajob( tOID, tArr, tDur, tTO ) );
			else {
            	cout << "An error occurred while inputing from file: " << filename << "\n";
            	infile.close();
            	return 0;
			}
		}
		
		
		infile.close();

		return 1;
		
	}
	
	void SimulateCPU( vector <Aabortjob> &AbortJobVec, vector <Adonejob> &DoneJobVec ) {
		
			JobQueue queue = JobQueue();
			int nowTime = 1;
			Ajob nowJob = Ajob();
			int nowJobStartTime = 0;
			
			while ( jobVec.size() > 0 ) {
						
				if ( nowJob.OID != "-1" ) {
					
					if ( nowJobStartTime + nowJob.Dur <= nowTime  ) { // 如果目前任務完成 
						
						// 加到完成清單後清空目前任務
						Adonejob nowDonejob = Adonejob(nowJob.OID,nowTime,nowJobStartTime - nowJob.Arr);
						DoneJobVec.push_back(nowDonejob);
						nowJob.init();
						
						if ( queue.size > 0 ) { // 取出queue 

							while ( queue.size > 0 ) {
								
								queue.pop(nowJob);
								nowJobStartTime = nowTime;
								if ( nowTime >= nowJob.TO ) {
									Aabortjob nowAbortjob = Aabortjob( nowJob.OID, nowTime , nowTime - nowJob.Arr  );
									AbortJobVec.push_back(nowAbortjob);	
									nowJob.init();
								}
								else 
									break;
							}	
							
						}			

							
							
					}
					
					else if ( nowTime >= nowJob.TO ) { // 如果逾時 
						
						// 加到取消清單後清空目前任務
						Aabortjob nowAbortjob = Aabortjob( nowJob.OID, nowJob.TO , nowJob.TO - nowJob.Arr  );
						AbortJobVec.push_back(nowAbortjob);							
						nowJob.init();
						
						if ( queue.size > 0 ) { // 取出queue  

							while ( queue.size > 0 ) {
								
								queue.pop(nowJob);
								nowJobStartTime = nowTime;
								if ( nowTime >= nowJob.TO ) {
									Aabortjob nowAbortjob = Aabortjob( nowJob.OID, nowTime , nowTime - nowJob.Arr  );
									AbortJobVec.push_back(nowAbortjob);	
									nowJob.init();
								}
								else 
									break;
							}	
							
						}			

						
					}
				
				}
				

				
				while ( jobVec.size() > 0 and nowTime == jobVec.front().Arr ) { // 時間到的話讀入Job 
					
					if ( nowJob.OID == "-1" ) {
						nowJob = jobVec.front();
						nowJobStartTime = nowTime;
					} 
					
					else if (queue.size < 3) {
						queue.push( jobVec.front() );
					}
					
					else {
						
						Aabortjob nowAbortjob = Aabortjob( jobVec.front().OID, nowTime,0 );
						AbortJobVec.push_back(nowAbortjob);
						
					}
					
					jobVec.erase( jobVec.begin() );
					
				}
				
				nowTime ++ ;

			}
			
				
			
			while ( queue.size > 0 or nowJob.OID != "-1" ) { // 清空queue與清空nowJob 
						
				if ( nowJob.OID != "-1" ) {
					
					if ( nowJobStartTime + nowJob.Dur <= nowTime  ) { // 如果目前任務完成 
						
						// 加到完成清單後清空目前任務
						Adonejob nowDonejob = Adonejob(nowJob.OID,nowTime,nowJobStartTime - nowJob.Arr);
						DoneJobVec.push_back(nowDonejob);
						nowJob.init();
						
						if ( queue.size > 0 ) { // 取出queue 

							while ( queue.size > 0 ) {
								
								queue.pop(nowJob);
								nowJobStartTime = nowTime;
								if ( nowTime >= nowJob.TO ) {
									Aabortjob nowAbortjob = Aabortjob( nowJob.OID, nowTime , nowTime - nowJob.Arr  );
									AbortJobVec.push_back(nowAbortjob);	
									nowJob.init();
								}
								else 
									break;
							}	
							
						}			

							
							
					}
					
					else if ( nowTime >= nowJob.TO ) { // 如果逾時 
						
						// 加到取消清單後清空目前任務
						Aabortjob nowAbortjob = Aabortjob( nowJob.OID, nowJob.TO , nowJob.TO - nowJob.Arr  );
						AbortJobVec.push_back(nowAbortjob);							
						nowJob.init();
						
						if ( queue.size > 0 ) { // 取出queue  

							while ( queue.size > 0 ) {
								
								queue.pop(nowJob);
								nowJobStartTime = nowTime;
								if ( nowTime >= nowJob.TO ) {
									Aabortjob nowAbortjob = Aabortjob( nowJob.OID, nowTime , nowTime - nowJob.Arr  );
									AbortJobVec.push_back(nowAbortjob);	
									nowJob.init();
								}
								else 
									break;
							}	
							
						}			

						
					}
				
				}						
					
				nowTime ++ ;
			}			
	
		
	}
	
	bool SaveOutput( vector <Aabortjob> AbortJobVec, vector <Adonejob> DoneJobVec ,int jobsCount, string filenum ) {
		
		string out_filename = "output" + filenum + ".txt";
		ofstream outfile(out_filename);
		
		if (!outfile.is_open() ) {
			cout << "Can't write file: '" << out_filename  <<" !!!\n";
			return 0;
		}		
		
		int delaySum = 0;
		int successCount = 0;
		
		int count = 1;
		outfile << "\t[Abort Jobs]\n";
		outfile << "\tOID\tAbort\tDelay\n";
		for( auto oneabortJob: AbortJobVec ){
	
			outfile << "[" << count << "]\t"<< oneabortJob.OID <<"\t" << oneabortJob.Abort << "\t" << oneabortJob.Delay << "\n";
			delaySum += oneabortJob.Delay;
			count ++;
		}
		
		count = 1;
		outfile << "\t[Jobs Done]\n";
		outfile << "\tOID\tDeparture\tDelay\n";
		for( auto onedoneJob: DoneJobVec ){
	
			outfile << "[" << count << "]\t" << onedoneJob.OID <<"\t" << onedoneJob.Departure << "\t" << onedoneJob.Delay << "\n";
			successCount ++ ;
			delaySum += onedoneJob.Delay;
			count ++;
		}
		outfile << "[Average Delay]\t"<< fixed << setprecision(2) << delaySum/(jobsCount*1.0) << " ms\n";
		outfile << "[Success Rate]\t" << fixed << setprecision(2) << (successCount*100.0)/jobsCount << " %\n";
				
		outfile.close();
		
		return 1;
		
	}
	
	Jobs(){
		jobVec = vector <Ajob>();
	}
	
	void init(){
		jobVec.clear();
	}
	
	~Jobs(){
    	jobVec.clear(); 
	}
	
};

int main() {
	
	int command;
	string filenum = "";
	Jobs AllJobs = Jobs(); 
	
	do{
		cout <<"\n***  Simulate FIFO Queues  ***";
		cout <<"\n* 0. Quit Program            *";
		cout <<"\n* 1. Sort a file             *";
		cout <<"\n* 2. Simulate one FIFO queue *";
		cout <<"\n******************************"; 
		cout <<"\nPlease input a command(0, 1, 2): ";
		
		command = Input::input_command(); // will get 0, 1, 2 or -1(無效指令)
		
		if ( command == 1 ) { 
		
			AllJobs.init(); 
			cout <<"\nInput a file number: ";
			cin >> filenum;
			cout << "\n";
			int ReadingTime, SortingTime, WritingTime;
			
			if ( AllJobs.readJobsFromFile(filenum, ReadingTime) ) {
				
				if ( AllJobs.ShellSortJobsAndSaveFile(filenum, SortingTime, WritingTime) ) {
					cout << "\n";
					cout << "Reading data: " << ReadingTime << " ms\n";
					cout << "Sorting data: " << SortingTime << " ms\n";
					cout << "Writing data: " << WritingTime << " ms\n";
					cout << "\n";
					cout << "See sorted" + filenum + ".txt\n";
				}
				
			}
			
		}

		else if ( command == 2 ) { 
		 
			AllJobs.init(); 
	
			if ( filenum == "" ) {
				cout <<"\nInput a file number: ";
				cin >> filenum;
			}
			
			cout << "\n";
			
			if ( AllJobs.readSortedJobs(filenum) ) {
				
				vector <Aabortjob> AbortJobVec = vector<Aabortjob>(); 
				vector <Adonejob> DoneJobVec  = vector<Adonejob>();
				int jobsCount = AllJobs.jobVec.size();

				AllJobs.SimulateCPU( AbortJobVec, DoneJobVec );
				
				if ( AllJobs.SaveOutput(AbortJobVec, DoneJobVec, jobsCount, filenum) )
					cout <<"See output" << filenum <<  ".txt\n";
					
			}
			else 
				filenum = "";
		}
		
		else if ( command == 0 )  // 0 means stop the program
			break;
		else  
			cout << "\nThere is no such command!\n";
			
	} while(true);
	
	cout << "\n";
	system("pause");

} // main() 





