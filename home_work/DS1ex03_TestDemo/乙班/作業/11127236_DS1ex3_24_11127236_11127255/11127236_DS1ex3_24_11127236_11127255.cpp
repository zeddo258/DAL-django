// 11127236莊楷崴 11127255黃冠程 
#include <iostream>
#include <stdlib.h>
#include <new>
#include <string.h>  // string class 
#include <cstdlib>   // system, atoi
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <ctime>

using namespace std;

#define MAX_QUEUE 3

struct dataname {
	int OID;
	int Arrival;
	int Duration;
	int TimeOut;
};


struct AbortJob {
	int OID;
	int Abort;
	int Delay;
};


struct DoneJob {
	int OID;
	int Departure;
	int Delay;
};


class JobQueue{
private:
	vector<dataname> qu;
	int front;
	int back;
	
public:
	JobQueue():front(0), back(0){}
	
	~JobQueue(){
		qu.clear();
	}
	
	bool isEmpty() const {
    	return qu.empty();
    } // isEmpty()
    
    bool isFull() {
		return qu.size() == MAX_QUEUE ;
	} // isFull()
    
    void enqueue( const dataname &newItem ){
    	
    	if ( isFull() ){
    		return;
		} // if
		
		else { 
			qu.push_back( newItem );
		} // else 
		
		return;
		
	} // enqueue()
	
	void dequeue(){
		if (!isEmpty()) {
            qu.erase(qu.begin());
        }
		
	} // dequeue()
	
	void getFront( dataname &head ){
		if ( isEmpty() ){
			return;
		}
		
		head = qu[0];
		return;
		
	} // getFront()
	
	void clearQ(){	
		qu.clear();
	} // clearQ()
	
};


class AnsList{
private:
	vector<DoneJob> donelist;
	vector<AbortJob> abortlist;
	float avg;
	float successRate;

public:
	AnsList(): avg(0.0), successRate(0.0) 
	{}
	~AnsList()
	{
		clearDone();
		clearAbort();
	} 
	
	AbortJob getAbort( int i ) {
		return abortlist[i];
	}
	
	DoneJob getDone( int i ) {
		return donelist[i];
	}
	
	
	void addAbortJob( int OID, int Abort, int Delay ){
		
		AbortJob temp;
		temp.OID = OID;
		temp.Abort = Abort;
		temp.Delay = Delay;
		abortlist.push_back( { temp.OID, temp.Abort, temp.Delay } );
		return;
		
	}
	
	void addDoneJob( dataname cur, int time ){
		
		DoneJob temp;
		temp.OID = cur.OID;
		temp.Departure = time;
		temp.Delay = time - cur.Arrival - cur.Duration;
		donelist.push_back( { temp.OID, temp.Departure, temp.Delay } );
		return; 
	}
	
	void computeStat(){
		
		for( int i = 0; i < abortlist.size(); i++ )
			avg += getAbort(i).Delay;
		
		for( int i = 0; i < donelist.size(); i++ )
			avg += getAbort(i).Delay;
		
		avg /= ( abortlist.size() + donelist.size() );	// 計算平均Delay時間 
		avg = round( avg * 100.0) / 100.0;
		
		
		successRate = abortlist.size() / ( abortlist.size() + donelist.size() ) * 100;	// 計算成功率 
		successRate = round( successRate * 100.0) / 100.0;
		
		return;
	}
	
	void putall( string fileName ){
	
		computeStat();
		fileName = "output" + fileName + ".txt";
		ofstream outFile(fileName.c_str());
		
		if ( !outFile.is_open() ){
			cout << endl << "### " << fileName << " does not exist! ###" << endl ;
		} // if

		else { 
			computeStat();
			
			outFile << "\t[Aborts Jos]" << endl << "\tOID\tAbort\tDelay" << endl;	// 將取消清單存入output???.txt 
			for( int i = 0; i < abortlist.size(); i++ ){ 	
				outFile <<"["<<i+1<<"]"<< "\t"<< abortlist[i].OID << "\t" << abortlist[i].Abort << "\t" << abortlist[i].Delay << "\t" << endl;
			} // for
			 
			outFile << "\t[Jos Dones]" << endl << "\tOID\tDeparture\tDelay" << endl;	// 將完成清單存入output???.txt 
			for( int i = 0; i < donelist.size(); i++ ){ 
				outFile <<"["<<i+1<<"]"<< "\t"<< donelist[i].OID << "\t" << donelist[i].Departure << "\t" << donelist[i].Delay << "\t" << endl;
			} // for 
			
			outFile << "[Average Delay]\t" << avg << " ms" << endl; 	
			outFile << "[Success Rate]\t" <<  successRate << " %" << endl;
		} // else
	}
	
	void clearDone(){
		donelist.clear();
	}
	
	void clearAbort(){
		abortlist.clear();
	}
	
};


class JobList{
	vector<dataname> data;
	string fileID; 
	double durationRead;
 	double durationSort;
 	double durationWrite;
	
public:
	JobList():fileID( "NULL" ), durationRead(0.0), durationSort(0.0), durationWrite(0.0)
	{}
	~JobList()
	{
		vector<dataname> ().swap(data);
	} 
	
	vector<dataname> getData() const {
        return data;
    }
    
    dataname getData( int i ) const	{
		return data[i];
	}
    
    int datasize() const {
    	return data.size();
	}
	
	void shellSort() {
		
		clock_t startSort = clock();
    	int n = data.size();

    	for (int gap = n / 2; gap > 0; gap /= 2) {
        	for (int i = gap; i < n; i++) {
            	dataname temp = data[i];
            	int j;

            	// 在比較時同時考慮進入時刻和工作編號
            	for (j = i; j >= gap && (data[j - gap].Arrival > temp.Arrival || (data[j - gap].Arrival == temp.Arrival && data[j - gap].OID > temp.OID)); j -= gap) {
              	  	data[j] = data[j - gap];
            	} //for
            	data[j] = temp;
        	} // for
    	} // for
    
    	clock_t endSort = clock();
    	durationSort = double(endSort - startSort) / CLOCKS_PER_SEC * 1000;
    
	} // shellSort()

	bool Load( string fileName ) {
	
		clock_t startRead = clock();
		fileID = fileName;
	
		string line;
    	fileName = "input" + fileID + ".txt"; 
    	ifstream file(fileName.c_str());
   
    	if( !file.is_open()){
			cout << endl << "### " << fileName << " does not exist! ###" << endl ;
			return false; 
		} // if
    	else {
    	
    		getline( file , line ); // 先讀標題 
    		while( getline( file , line ) ) {	// 讀內容 
    			stringstream ss(line);
   	         	dataname entry;
   	         	ss >> entry.OID >> entry.Arrival >> entry.Duration >> entry.TimeOut;
    			data.push_back(entry);
			} // while

    		file.close();
	    	clock_t endRead = clock();
			durationRead = double(endRead - startRead) / CLOCKS_PER_SEC * 1000;
  	  		return true;
		} //else 
    	
	} // Load()

	void write(){
	
		clock_t startWrite = clock();
		string fileName = "sorted" + fileID + ".txt";
		ofstream outFile(fileName.c_str());

    	if (!outFile.is_open()){
			cout << endl << "### " << fileName << " does not exist! ###" << endl ;
		} // if

   		// 寫入標題
    	outFile << "\tOID\tArrival\tDuration\tTimeOut" << endl;

    	// 寫入已排序的數據
    	for (int no = 0 ; no < data.size() ; no++) {
        	outFile <<"("<<no+1<<")"<< "\t"<< data[no].OID << "\t" << data[no].Arrival << "\t" << data[no].Duration << "\t" << data[no].TimeOut << endl;
    	} // for

    	outFile.close();
    	clock_t endWrite = clock();
    	durationWrite = double(endWrite - startWrite) / CLOCKS_PER_SEC * 1000;
    
	} // write()

	void printdata(){
	
		int i = 1;
		string line, fileName;
    	fileName = "input" + fileID + ".txt"; 
    	ifstream file(fileName.c_str());
    
    
    	getline(file, line);	// 印出OID Arrival等等 
    	cout << "\t" << line << endl;
   		while(getline(file, line)) { 
   			cout << "(" << i++ << ")\t" ;
   			cout << line << endl;
		} // while 

		file.close();
	
		int read = durationRead;
		int sort = durationSort;
		int write = durationWrite;
		cout << endl;
		cout << "Reading data: " << read << " clocls " << "(" << durationRead << ".00ms)." << endl;
  	 	cout << "Sorting data: " << sort << " clocls " << "(" << durationSort << ".00ms)." << endl;
  	  	cout << "Writing data: " << write << " clocls " << "(" << durationWrite << ".00ms)." << endl;
		cout << endl << "See sorted" + fileID + ".txt" << endl;;
	} // printdata()

};


class Simulation{
private:
	JobList jobs; 
	AnsList answers;
	JobQueue allQ;
	string fileID;
	
/*	void delQ( int time ){	// 刪掉queue裡的數據 
		
	}*/
	
/*	void updateQ( int ){
		
	}
	
/*	void finishQ(){
		
	}*/
	
	
	int biggest() const{
		int max = jobs.getData(0).TimeOut;
		for( int i = 1; i < jobs.datasize(); i++ ){
			if( max < jobs.getData(i).TimeOut ) {
				max = jobs.getData(i).TimeOut;
			}
		}
		
		return max;
	}
	
	dataname supcpu( int time ) {

    	dataname temp;

    	while (!allQ.isEmpty()) {
        	allQ.getFront( temp );

        	if ( temp.TimeOut >= time ) {
            	allQ.dequeue();
            	return temp;
        	} // if
        	
			else if ( temp.TimeOut < time ) {	// queue取出逾時 
            	answers.addAbortJob(temp.OID, time, time - temp.Arrival);
            	allQ.dequeue();
        	}	// else
        	
    	} // while
        	
    	dataname cur;
    	cur.OID = 0;
    	cur.Arrival = 0;
    	cur.Duration = 0;
    	cur.TimeOut = 0;
    	return cur;
}
	
public:
	Simulation( JobList data ): jobs( data ){
	} 
	~Simulation(){
		allQ.clearQ();
	}
	
	
	void setfile( string filename ){
		fileID = filename;
	}

	void SQF(){
		
		dataname cur = jobs.getData(0); 
		int i = 1, time = 0, pretime = cur.Arrival;
		bool check = 0;
		
		while( time <= biggest() ){
			
			if( pretime < cur.Arrival ){
				pretime = cur.Arrival;
			}
			
			while( time == pretime + cur.Duration ){	// 目前處理的資料時間到了要取出
				if( cur.OID == 0 ){
					break;
				}
			
				if( pretime < cur.Arrival )
					pretime = cur.Arrival;
			
				if ( time <= cur.TimeOut ) {	// 從cpu執行時沒有逾時 
					answers.addDoneJob( cur, time );	// 將cpu的資料丟進done
					pretime = time;
					cur = supcpu( time );
				} // if
					
				else if ( time > cur.TimeOut ) {	// cpu執行逾時 
					answers.addAbortJob( cur.OID, cur.TimeOut, cur.TimeOut - cur.Arrival );	// cpu執行逾時，將cpu的資料丟進abort 
					pretime = time;
					cur = supcpu( time );
				} // else if
				
				if( cur.OID == 0 )
					break;
					
			} // while
			
			if( pretime + cur.Duration > cur.TimeOut ){
				answers.addAbortJob( cur.OID, cur.TimeOut, cur.TimeOut-cur.Arrival);
				pretime = cur.TimeOut;
				cur = supcpu( time );
				if( cur.OID == 0 )
					check = 1;
			}


			while( i < jobs.datasize() ){	// Arrival到了存進queue 
				
				if ( time != jobs.getData(i).Arrival ) {
					break;
				}
				
				if( cur.OID == 0 ) {
					cur.Arrival = jobs.getData(i).Arrival;
					cur.Duration = jobs.getData(i).Duration;
					cur.OID = jobs.getData(i).OID;
					cur.TimeOut = jobs.getData(i).TimeOut;
					check = 0;
				}
				
				 
				if ( !allQ.isFull() ) {	// 如果queue還有位子就丟進queue
					allQ.enqueue( jobs.getData(i) );
				}
				else if ( allQ.isFull() ){	//queue滿了就丟進abort
					answers.addAbortJob( jobs.getData(i).OID, jobs.getData(i).Arrival, 0 );	
				}
				 
				 
				i++;
			} // while
			
			time++;
		} // while
		
		answers.putall( fileID );
	} // SQF()
};


int main() {
 	AnsList ans;
 	JobList jobList;
 	int command = 0 ;
 	string filename ;
 	
 	do {
		cout << "\n" ;
    	cout << "**** Simulate FIFO Queues by SQF *****" << "\n" ;
  		cout << "* 0. Quit                            *" << "\n" ;
 		cout << "* 1. Sort a file                     *" << "\n" ;
 		cout << "* 2. Simulate one FIFO queue         *" << "\n" ;
  		cout << "**************************************" << "\n" ;
  		cout << "Input a command(0, 1, 2): "  ;
  		cin >> command ;          
 		cout << "\n" ;
	
  
  
  		if ( command == 0 )
   			break ;
   
  		else if ( command == 1 ) {
  			
  			cout << "Input a file number(ex: 301, 302...) :";
   			cin >> filename ;
   			cout << endl;
   			
   			if ( jobList.Load( filename ) ) {
			    
    			jobList.shellSort() ; 
    			jobList.write () ;
				jobList.printdata() ; 
				
     		} // if
     		continue;
	
  		} // else if
  		
  		else if ( command == 2 ) {
  			
  				Simulation simulation( jobList );
  				cout << "Input a file number: " << endl;
  				simulation.setfile( filename );
				simulation.SQF();
				cout << "The simulation is running..." << endl;
				cout << "See output" + filename + ".txt" << endl; 
				
  		} // else if 
  		
  		else {
    		cout << endl << "Command does not exist!" << "\n" ;   
   			break ;
  		} // else


 	} while( true ) ;
 
} // main()

