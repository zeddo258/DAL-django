// 10727160 ªô¥@øÊ 11127171¶À§»¸Î 
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <queue>
#include <string.h>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>

using namespace std;

#define MAX_QUEUE 3

typedef struct jobL {
    int OID ;
    int Arrival ;
    int Duration ;
    int TimeOut ;
    bool isTimeOut = false ;
    
    bool compare ( jobL pre ) {
    	if ( Arrival < pre.Arrival ) return true ;
    	else if ( Arrival == pre.Arrival && OID < pre.OID ) return true ;
		return false ;		
	} // compare()

	bool isEmpty() {
		if ( OID == -1 ) return true ;
		return false ;
	} // isEmpty()

	void clear() {
		OID = Arrival = Duration = TimeOut = -1 ;
		isTimeOut = false ;
	} // clear()
	
} jobList;

typedef struct jobD {
	int OID ;
	int Departure ;
	int Delay ;
} jobDone;

typedef struct jobA {
	int OID ;
	int Abort ;
	int Delay ;
} jobAbort;

class Queue {
private:
	vector<jobList> joblist ;
	
public:
	void addJob( jobL jobs ) { joblist.push_back( jobs ) ; } 
    void clear() { joblist.clear(); } // clear()
    vector<jobList>& getJobs() { return joblist ; }

	void printJob() {
		cout << "\n\tOID\tArrival\tDuration\tTimeOut" << endl ;
		for ( int i = 0 ; i < joblist.size() ; i++ ) {
			cout << "(" << i+1 << ")\t" << joblist[i].OID << "\t" << joblist[i].Arrival << "\t" 
				 << joblist[i].Duration << "\t" << joblist[i].TimeOut << endl ;
		} // for()
	} // printJob()

	void shellSort() {
		for ( int gap = joblist.size() / 2 ; gap > 0 ; gap = gap / 2 ) {
			for ( int traverse = gap ; traverse < joblist.size() ; traverse++ ) {
				jobL jobTemp = joblist[traverse] ;
				int cur ;
				for ( cur = traverse ; cur >= gap && jobTemp.compare( joblist[cur - gap] ) ; cur = cur - gap ) {
					joblist[cur] = joblist[cur-gap] ;
				} // for()
				joblist[cur] = jobTemp ;
			} // for()
		} // for()
	} // shellSort() 

};

class CPU {

	vector<jobDone> listDone ;		// Done  List : OID / Depature / Delay
	vector<jobAbort> listAbort ;	// Abort List : OID / Abort    / Delay 

public:
	void clear() { 
		listDone.clear() ; 
		listAbort.clear() ;
	} // clear()
	
	void abort( jobL abortJob, int abortTime, int delayTime ) {
		jobA curAbort ;
		curAbort.OID = abortJob.OID ;
		curAbort.Abort = abortTime ;
		curAbort.Delay = delayTime ; 
		listAbort.push_back( curAbort ) ;
	} // abort()
	
	void done( jobL doneJob, int clock ) {
		jobD curDone ;
		curDone.OID = doneJob.OID ;
		curDone.Departure = clock ;
		curDone.Delay = clock - doneJob.Duration - doneJob.Arrival ;
		listDone.push_back( curDone ) ;		
	} // done()
	
	vector<jobAbort>& getAbort() { return listAbort ; }
	vector<jobDone>& getDone() { return listDone ; }
	
	void printList() { // show on screen
		cout << "----------Done List----------\n" ;
		for( int i = 0 ; i < listDone.size() ; i++ ) 
			cout << "OID: " << listDone[i].OID << "/ Depature: " << listDone[i].Departure << " / Delay: " << listDone[i].Delay << endl ;
		cout << "\n----------Abort List----------\n" ;
		for( int j = 0 ; j < listAbort.size() ; j++ ) 
			cout << "OID: " << listAbort[j].OID << "/ Abort: " << listAbort[j].Abort << " / Delay: " << listAbort[j].Delay << endl ;
	} // printList()
	
	void running( vector<jobList>& allJobs ) {
		int clock = -1 ;
		vector<jobList> queueJobs ;
		jobL cur ; // current job
		cur.clear() ;

		for ( int i = 0 ; i < allJobs.size() ; i++ ) {
			if ( allJobs[i].Arrival < clock ) { // the previous job hasn't yet been completed
				if ( queueJobs.size() < MAX_QUEUE ) queueJobs.push_back( allJobs[i] ) ;
				else abort( allJobs[i], allJobs[i].Arrival, 0 ) ;
			} // if()
			else {	// the job will be entered after the CPU has finished running
				if ( queueJobs.size() < 1 ) {	// queue is empty, then execute this job
					cur = allJobs[i] ;
					clock = cur.Arrival + cur.Duration ;
				} // if()
				else {	// queue has another jobs, execute them first 
					while ( allJobs[i].Arrival >= clock && !cur.isEmpty() ) {
						if ( !cur.isEmpty() ) {
							if ( cur.isTimeOut ) abort( cur, clock, ( clock - cur.Arrival ) ) ;
							else done( cur, clock ) ;
							cur.clear() ;
						} // if()
						
						if ( !queueJobs.empty() ) {
							cur = queueJobs.front() ;
							queueJobs.erase( queueJobs.begin() ) ;
							if ( clock > cur.TimeOut ) cur.isTimeOut = true ;
							else if ( clock + cur.Duration > cur.TimeOut ) {
								cur.isTimeOut = true ;
								clock = cur.TimeOut ;
							} // else if()
							else clock = clock + cur.Duration ;						
						} // if()
					
					} // while()
					
					
					if ( cur.isEmpty() ) {
						cur = allJobs[i] ;
						clock = cur.Arrival + cur.Duration ;
					} // if()
					else if ( !cur.isEmpty() && queueJobs.size() < MAX_QUEUE ) queueJobs.push_back( allJobs[i] ) ;
					else abort( allJobs[i], clock, 0 ) ;
				} // else()

			} // else()

		} // for()

		// finish queuelist
		while ( !cur.isEmpty() || !queueJobs.empty() ) {
			if ( !cur.isEmpty() ) {
				if ( clock <= cur.TimeOut ) done( cur, clock ) ;
				else {
					abort( cur, cur.TimeOut, ( cur.TimeOut - cur.Arrival ) ) ;
					clock = cur.TimeOut ;
				} // else()
				cur.clear() ;				
			} // if()
			
 			if ( !queueJobs.empty() ) {
				cur = queueJobs.front() ;
				queueJobs.erase( queueJobs.begin() ) ;
				if ( clock >= cur.TimeOut ) {
					abort( cur, clock, ( clock - cur.Arrival ) ) ;
					cur.clear() ;
				} // if()
				else clock = clock + cur.Duration ;
			} // if()

		} // while()
		
	} // running()
};

int GetNum() {
    int num;
    cout << "\n**** Simulate FIFO Queues by SQF *****" << endl ;
    cout << "* 0. Quit                            *" << endl ;
    cout << "* 1. Sort a file                     *" << endl ;
    cout << "* 2. Simulate one FIFO queue         *" << endl ;
    cout << "**************************************" << endl ;
    cout << "Input a command(0, 1, 2): " ;
    cin >> num ;
    return num ;
} // GetNum()

bool readFile( string fileID, Queue &queueJob ) {
	string fileName = "input" + fileID + ".txt" ;
    ifstream inFile( fileName ) ;

    if ( !inFile ) {
        cout << "\n### " << fileName << " does not exist! ###" << endl ;
        return false ;
    } // if()

	// Drop first line
	string str ;
	getline( inFile, str ) ;

	// Read content & build dataset
	while ( getline( inFile, str ) ) {
		stringstream ss( str ) ;
		jobL jobs ;
		ss >> jobs.OID >> jobs.Arrival >> jobs.Duration >> jobs.TimeOut ;
		queueJob.addJob( jobs ) ;
	} // while()

	inFile.close();
    return true ;
} // readFile()

bool readSortedFile( string fileID ) {
	string fileName = "sorted" + fileID + ".txt" ;
    ifstream inFile( fileName ) ;

    if ( !inFile ) {
        cout << "\n### " << fileName << " does not exist! ###" << endl ;
        return false ;
    } // if()

	inFile.close();
    return true ;
} // readSortedFile()


void writeFile( string fileID, Queue &queueJob ) {
	vector<jobList> writeJobs = queueJob.getJobs() ;
    fstream outFile;
    string fileName ="sorted" + fileID + ".txt" ;
    outFile.open( fileName.c_str(), fstream::out) ;
    if( outFile.is_open() ) {
        outFile << "OID\tArrival\tDuration\tTimeOut" << endl ;
        for( int i = 0 ; i < writeJobs.size() ; i++ ) {
            outFile << writeJobs[i].OID << "\t" << writeJobs[i].Arrival << "\t" 
				 	<< writeJobs[i].Duration << "\t" << writeJobs[i].TimeOut << endl ;
        } // for
    } // if

    outFile.close() ;

} // writeFile()

void writeCpu( string fileID, CPU &cpu, double aveDelay, double sucRate ) {
	vector<jobDone> writeDone = cpu.getDone() ;
	vector<jobAbort> writeAbort = cpu.getAbort() ;

    fstream outFile ;
    string fileName ="output" + fileID + ".txt" ;
    outFile.open( fileName.c_str(), fstream::out ) ;
    if( outFile.is_open() ) {
    	outFile << "\t[Abort Jobs]" << endl ;
        outFile << "\tOID\tAbort\tDelay" << endl ;
        for( int i = 0 ; i < writeAbort.size() ; i++ ) {
            outFile << "[" << i+1 << "]\t" << writeAbort[i].OID << "\t" 
					<< writeAbort[i].Abort << "\t" << writeAbort[i].Delay << endl ;
        } // for
        
    	outFile << "\t[Jobs Done]" << endl ;
        outFile << "\tOID\tDeparture\tDelay" << endl ;
        for( int j = 0 ; j < writeDone.size() ; j++ ) {
            outFile << "[" << j+1 << "]\t" << writeDone[j].OID << "\t" 
					<< writeDone[j].Departure << "\t" << writeDone[j].Delay << endl ;
        } // for
    } // if

	outFile << "[Average Delay]\t" << fixed  <<  setprecision(2) << aveDelay << " ms" << endl ;
	outFile << "[Success Rate]\t" << fixed  <<  setprecision(2) << sucRate << " %" << endl ;
    outFile.close() ;
} // writeCpu()

double averageDelay( CPU &cpu ) {
	vector<jobDone> delayDone = cpu.getDone() ;
	vector<jobAbort> delayAbort = cpu.getAbort() ;
	double sizeTotal = delayDone.size() + delayAbort.size() ;
	double delayTotal = 0 ;

    for ( int i = 0 ; i < delayDone.size() ; i++ ) 
    	delayTotal = delayTotal + delayDone[i].Delay ;
    for ( int j = 0 ; j < delayAbort.size() ; j++ ) 
    	delayTotal = delayTotal + delayAbort[j].Delay ;
    	
    // cout << "delaytotal: " << delayTotal << " / sizeTotal: " << sizeTotal << endl ;
    return delayTotal / sizeTotal ;
} // averageDelay()

double successRate( CPU &cpu ) {
	vector<jobDone> success = cpu.getDone() ;
	vector<jobAbort> unsuccess = cpu.getAbort() ;
	double sizeTotal = success.size() + unsuccess.size() ;
	// cout << "sucess.size: " << success.size() << " / size: " << sizeTotal << endl ;
	return success.size() / sizeTotal * 100 ;
} // successRate()

showTime( double readTime, double sortTime, double writeTime ) {
	cout << "\nReading data: " << (int)readTime << " clocks (" ;
    cout << fixed << setprecision(2) << readTime << " ms)." << endl ;
	cout << "Sorting data: " << (int)sortTime << " clocks (" ;
    cout << fixed << setprecision(2) << sortTime << " ms)." << endl ;
	cout << "Writing data: " << (int)writeTime << " clocks (" ;
    cout << fixed << setprecision(2) << writeTime << " ms)." << endl ;
} // showTime()

resetVariables( double readTime, double writeTime, double sortTime, double start, double end, double aveDelay, double sucRate ) {
	readTime = sortTime = writeTime = 0.0 ;
	start = end = 0.0 ;
	aveDelay = sucRate = 0.0 ;
} // resetVariables()

int main() {
    int command ;
    double readTime, writeTime, sortTime ;
    double start, end ;
    double aveDelay, sucRate ;
    string fileID ;
    vector<jobList> allJobs ;
    Queue queueJob ;
    CPU cpu ;
    
	int num = GetNum() ;
	while ( num != 0 ) {
		if ( num == 1 ) {
			cout << "\nInput a file number: " ;
    		cin >> fileID ;

    		start = clock() ;
    		if ( readFile( fileID, queueJob ) ) {
    			end = clock() ;
                readTime = ( end - start ) / static_cast<double>(CLOCKS_PER_SEC) ;

                start = clock() ;
    			queueJob.shellSort() ;			// shell sort
    			end = clock() ;
    			sortTime = ( end - start ) / static_cast<double>(CLOCKS_PER_SEC) ;
    			
				start = clock() ;
				writeFile( fileID, queueJob ) ;	// output if file
				end = clock() ;
				writeTime = ( end - start ) / static_cast<double>(CLOCKS_PER_SEC) ;
				
				queueJob.printJob() ;			// output on screen
				showTime( readTime, sortTime, writeTime ) ;
				
				cout << "\nSee sorted" << fileID << ".txt" << endl ;
			} // if()
			
			allJobs = queueJob.getJobs() ;

			// initial
			resetVariables( readTime, writeTime, sortTime, start, end, aveDelay, sucRate ) ;
			queueJob.clear() ;
			cpu.clear() ;
		} // if()
		else if ( num == 2 ) {
			if ( readSortedFile( fileID ) ) {
				cout << "\nThe simulation is running..." << endl ;
				cout << "See output" << fileID << ".txt" << endl ;
				cpu.running( allJobs ) ;
				aveDelay = averageDelay( cpu ) ;
				sucRate = successRate( cpu ) ;
				writeCpu( fileID, cpu, aveDelay, sucRate ) ;
				// cpu.printList() ;				
			} // if()

		} // else if()
		num = GetNum() ;
	} // while()

} // main()

