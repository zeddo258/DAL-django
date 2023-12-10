//group 17, 11127131 梁嘉諾 & 11127135 李廷宇
#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
using namespace std;
 
class Queue {
 
	public:
 
		struct Q {
		    int oid = 0;
		    int arrival = 0;
		    int duration = 0;
		    int timeOut = 0;
            int process = 0;
            int delay = 0;
		    Q * next;
		};
 
		static Q * front, * back, * abortList, * doneList;
 
		static Q * createNewPtr( int oid, int arrival, int duration, int timeOut, int process, int delay ) {
		    Q * newPtr = new Q;
		    newPtr -> next = NULL;
		    newPtr -> oid = oid;
		    newPtr -> arrival = arrival;
		    newPtr -> duration = duration;
		    newPtr -> timeOut = timeOut;
		    newPtr -> process = process;
		    newPtr -> delay = delay;
		    return newPtr;
		} // end createNewPtr
 
		static void enqueue( Q * & head, Q * & tail, int oid, int arrival, int duration, int timeOut, int process, int delay ) {
		    Q * newPtr = createNewPtr( oid, arrival, duration, timeOut, process, delay );
 
		    if( head == NULL && head == tail ) {
		    	head = newPtr;
		    	tail = newPtr;
			} // end if
		    else {
		    	tail -> next = newPtr;
		    	tail = tail -> next;
			} // end else
		} // end enqueue
		
		static void enqueue( Q * & head, Q * & tail, Q * temp ) {
		    Q * newPtr = createNewPtr( temp -> oid, temp -> arrival, temp -> duration, temp -> timeOut, temp -> process, temp -> delay );
 
		    if( head == NULL && head == tail ) {
		    	head = newPtr;
		    	tail = newPtr;
			} // end if
		    else {
		    	tail -> next = newPtr;
		    	tail = tail -> next;
			} // end else
		} // end enqueue
 
		static void dequeue( Q * & head, Q * & tail ) {
			if ( head == NULL && head == tail ) cout << "This queue is empty, it cannot dequeue.";
			else {
				Q * temp = head;
				if ( head == tail ) {
					head = NULL;
					tail = NULL;
				} // end if
				else head = head -> next;
 
				temp -> next = NULL;
				delete temp; 
				temp = NULL;
			} // end else
		} // end dequeue
 
		static Q * getPtr( int num, Q * head ) { // num starts from 0
		    Q * temp = head;
		    for( int i = 0 ; i < num && temp != NULL ; i++ ) temp = temp -> next;
 
		    if ( num > getLen( head ) ) {
		    	cout << "There is not enough node to retrieve";
		    	return NULL;
			} // end if
		    else return temp;
		} // end getPtr
 
		static int getLen( Q * head ) { // 計算工作的數量 
			int len = 0;
			Q * temp = head;
 
			for ( ; temp != NULL ; temp = temp -> next ) len++;
 
			if ( head == NULL ) return 0;
			else return len;
		} // end getLen 
};

Queue::Q * Queue::front = NULL;
Queue::Q * Queue::back = NULL;
Queue::Q * Queue::abortList = NULL;
Queue::Q * Queue::doneList = NULL;

bool setFileNumber( char command[], string & fileNumber, bool & legalCmd ) {	
	string fileName = "\0";
 
	if ( strlen( command ) != 1 ) {
		legalCmd = false;
		return false;
	} // end if
	else if ( command[0] == 49 ) {
		cout << endl << "Input a file number: ";
		cin >> fileNumber;
		fileName = "input" + fileNumber + ".txt";
 
		ifstream inputfile( fileName.c_str() );
		if ( !inputfile.is_open() ) {
			cout << endl << "### " << fileName << " does not exist! ###" << endl;
			inputfile.close();
			legalCmd = true;
			return false;
		} // end if
		else {
			inputfile.seekg( 0, ios::end );
			
    		if ( inputfile.tellg() == 0 ) {
				cout << endl << "### Get nothing from the file " << fileName << " ! ###" << endl;
				inputfile.close();
				legalCmd = true;
				return false;
			} // end if
			else {
				inputfile.close();
				legalCmd = true;
				return true;
			} // end else
		} // end else
	} // end else if
	else if ( command[0] == 50 ) {
		if ( fileNumber == "\0" ) {
			cout << endl << "Input a file number: ";
			cin >> fileNumber;
		} // end if
 
		fileName = "sorted" + fileNumber + ".txt";	
 
		ifstream inputfile( fileName.c_str() );
		if ( !inputfile.is_open() ) {
			cout << endl << "### " << fileName << " does not exist! ###" << endl;
			inputfile.close();
			legalCmd = true;
			return false;
		} // end if
		else {
			fileName = "output" + fileNumber + ".txt";
			cout << endl << "The simulation is running..." << endl;
			cout << "See " << fileName << endl;
			inputfile.close();
			legalCmd = true;
			return true;
		} // end else
	} // end else if
} // end setFileNumber
 
void readFile( Queue::Q * & front, Queue::Q * & back, string & fileNumber, Queue & queue ) {
	string text = "\0", fileName = "input" + fileNumber + ".txt";
	int count = 1, oid, arrival, duration, timeOut, process, delay;
	Queue::Q * temp = NULL;
	clock_t start, end;
 
	start = clock();
 
	ifstream inputfile( fileName.c_str() );
	if ( inputfile.is_open() ) {
		for ( int i = 0 ; i < 4 ; i++ ) // 把OID, Arrival, Duration, TimeOut四個單字讀掉 
			inputfile >> text;
	
		while ( front != NULL ) // 清除前一個檔案的front
			queue.dequeue( front, back );
	 
		while ( inputfile >> oid >> arrival >> duration >> timeOut ) // 資料用linked list紀錄 
			queue.enqueue( front, back, oid, arrival, duration, timeOut, 0, 0 );
	} // end if
 
	inputfile.close();
 
	end = clock();
 
	cout << "\n\tOID\tArrival\tDuration\tTimeOut\n";
 
	temp = front;
	while ( temp != NULL ) {
		cout << "(" << count << ")" << "\t" << temp -> oid << "\t" << temp -> arrival << "\t" << temp -> duration << "\t" << temp -> timeOut << endl;
		count++;
		temp = temp -> next;
	} // end while
 
	cout << "\nReading data: " << end - start << " clocks (";
	cout << fixed << setprecision( 2 ) << double( end - start ) << " ms).\n";
} // end readFile
 
void shellSort( Queue::Q * & front, Queue::Q * & back, Queue & queue ) {
	int oid, arrival, duration, timeOut;
	int len = queue.getLen( front );
	Queue::Q * ptr1 = NULL, * ptr2 = NULL, * ptrI = NULL, * ptrJ = NULL;
	clock_t start, end;
 
	start = clock();
 
    if ( front != NULL && front -> next != NULL ) { // No sorting needed for empty or single-node list
	    for ( int gap = len / 2 ; gap > 0 ; gap = gap / 2 ) {
	    	for ( int i = gap ; i < len ; i++ ) {
	    		int j = i;
				ptrI = queue.getPtr( i, front );
				oid = ptrI -> oid;
	            arrival = ptrI -> arrival;
	            duration = ptrI -> duration;
	            timeOut = ptrI -> timeOut;
 
	    		while ( j >= gap && ( queue.getPtr( j - gap, front ) -> arrival > arrival ||
					   ( queue.getPtr( j - gap, front ) -> arrival == arrival && queue.getPtr( j - gap, front ) -> oid > oid ) ) ) {
	    			ptr1 = queue.getPtr( j, front ), ptr2 = queue.getPtr( j - gap, front );
	                ptr1 -> oid = ptr2 -> oid;
	                ptr1 -> arrival = ptr2 -> arrival;
	                ptr1 -> duration = ptr2 -> duration;
	                ptr1 -> timeOut = ptr2 -> timeOut;
 
	                j = j - gap;
				} // end while
 
				ptrJ = queue.getPtr( j, front );		
				ptrJ -> oid = oid;
	            ptrJ -> arrival = arrival;
	            ptrJ -> duration = duration;
	            ptrJ -> timeOut = timeOut;
			} // end for
		} // end for
	} // end if
 
	end = clock();
 
	cout << "Sorting data: " << end - start << " clocks (";
	cout << fixed << setprecision( 2 ) << double( end - start ) << " ms).\n";
} // end shellSort

void writeSortedFile( Queue::Q * front, Queue::Q * back, string fileNumber, Queue & queue ) {
	string fileName = "sorted" + fileNumber + ".txt";
	int len = queue.getLen( front );
	Queue::Q * temp = NULL;
	clock_t start, end;
 
	start = clock();
 
	ofstream outputfile( fileName.c_str() );
 
	if ( outputfile.is_open() ) {
		outputfile << "OID\tArrival\tDuration\tTimeOut\n";
 
		for ( int i = 0 ; i < len ; i++ ) {
			temp = queue.getPtr( i, front );
			outputfile << temp -> oid << "\t" << temp -> arrival << "\t" << temp -> duration << "\t" << temp -> timeOut << "\n";
		} // end for
	} // end if
 
	outputfile.close();
 
	end = clock();
 
	cout << "Writing data: " << end - start << " clocks (";
	cout << fixed << setprecision( 2 ) << double( end - start ) << " ms).\n";
	cout << "\nSee " << fileName << endl;
} // end writeSortedFile

void simulation( Queue::Q * & front, Queue::Q * & back, Queue::Q * & abortList, Queue::Q * & doneList, string fileNumber, Queue & queue ) {
    Queue::Q * lineUp = NULL;
    Queue::Q * lineUpb = NULL, * abortListb = NULL, * doneListb = NULL; // 'b' means the last pointer of the linked list
    Queue::Q * cpu = NULL, * cpub = NULL; 
    Queue::Q * temp;
    int time;
    bool cpuIsNull = true;
    
    if ( front == NULL && front == back ) { // 執行的第一次command是2
    	string text = "\0", fileName = "sorted" + fileNumber + ".txt";
		int oid, arrival, duration, timeOut, process, delay;
		
		ifstream inputfile( fileName.c_str() );
		if ( inputfile.is_open() ) {
			for ( int i = 0 ; i < 4 ; i++ ) inputfile >> text;
			
			while ( inputfile >> oid >> arrival >> duration >> timeOut ) 
				queue.enqueue( front, back, oid, arrival, duration, timeOut, 0, 0 );
		} // end if
		
		inputfile.close();
	} // end if

	while ( abortList != NULL ) queue.dequeue( abortList, abortListb ); // 清除前一個檔案的abortList
	while ( doneList != NULL ) queue.dequeue( doneList, doneListb ); // 清除前一個檔案的doneList
	
    time = front -> arrival;
    temp = front;
    while( temp != NULL ) {
    	if ( time + temp -> duration <= temp -> timeOut && cpuIsNull ) { // cpu是空的且完成還不逾時 
    		cpuIsNull = false;
    		temp -> process = time + temp -> duration;
    		temp -> delay = time - temp -> arrival;
			time = temp -> process; // time為此筆工作的完成時刻 
    		queue.enqueue( doneList, doneListb, temp );
		} // end if
		else {
			if ( temp -> arrival >= time ) {
				while ( lineUp != NULL ) {
					if ( lineUp -> timeOut <= time ) { // 4.2
						lineUp -> process = time;
						lineUp -> delay = time - lineUp -> arrival;
						queue.enqueue( abortList, abortListb, lineUp );
					} // end if 
					else { // 進cpu處理 
						if ( time + lineUp -> duration <= lineUp -> timeOut ) { // 可完成且不逾時  
							lineUp -> process = time + lineUp -> duration;
							lineUp -> delay = time - lineUp -> arrival;
							time = lineUp -> process; // time為此筆工作的完成時刻 
							queue.enqueue( doneList, doneListb, lineUp );
						} // end if
						else { // 4.3
							lineUp -> process = lineUp -> timeOut;
							lineUp -> delay = lineUp -> timeOut - lineUp -> arrival;
							time = lineUp -> timeOut; // time為此筆工作的取消時刻 
							queue.enqueue( abortList, abortListb, lineUp ); 
						} // end else
					} // end else
						
					queue.dequeue( lineUp, lineUpb ); 
					
					if ( temp -> arrival < time ) break;
				} // end while
				
				if ( time <= temp -> arrival && temp -> arrival + temp -> duration <= temp -> timeOut ) { // 可完成且不逾時 
					temp -> process = temp -> arrival + temp -> duration;
					temp -> delay = 0;
					time = temp -> process; // time為此筆工作的完成時刻 
					queue.enqueue( doneList, doneListb, temp );
				} // end if
				else { 
					if ( temp -> arrival + temp -> duration > temp -> timeOut ) {
						if ( time > temp -> timeOut ) { // 4.2
							temp -> process = time;
							temp -> delay = time - temp -> arrival;
							queue.enqueue( abortList, abortListb, temp );
						} // end if
						else { // 4.3
							temp -> process = temp -> timeOut;
							temp -> delay = temp -> timeOut - temp -> arrival;
							time = temp -> timeOut; // time為此筆工作的取消時刻 
							queue.enqueue( abortList, abortListb, temp );
						} // end else
					} // end if
					else { // time > temp -> arrival 
						temp -> process = 0;
						temp -> delay = 0;
						queue.enqueue( lineUp, lineUpb, temp );
					} // end else
				} // end else
			} // end if
			else if ( temp -> arrival < time && queue.getLen( lineUp ) < 3 ) { 
				temp -> process = 0;
				temp -> delay = 0;
				queue.enqueue( lineUp, lineUpb, temp );
			} // end else if
			else if ( queue.getLen( lineUp ) == 3 ) { // 4.1
				temp -> process = temp -> arrival;
				temp -> delay = 0;
				queue.enqueue( abortList, abortListb, temp );
			} // end else if
		} // end else
		
		temp = temp -> next;
	} // end while
	
	while ( lineUp != NULL ) { // 處理佇列的工作 
		if ( lineUp -> timeOut <= time ) { // 4.2
			lineUp -> process = time;
			lineUp -> delay = time - lineUp -> arrival;
			queue.enqueue( abortList, abortListb, lineUp );
		} // end if 
		else { // 進cpu處理 
			if ( time + lineUp -> duration <= lineUp -> timeOut ) { // 可完成且不逾時 
				lineUp -> process = time + lineUp -> duration;
				lineUp -> delay = time - lineUp -> arrival;
				time = lineUp -> process; // time為此筆工作的完成時刻 
				queue.enqueue( doneList, doneListb, lineUp );
			} // end if
			else { // 4.3
				lineUp -> process = lineUp -> timeOut;
				lineUp -> delay = lineUp -> timeOut - lineUp -> arrival;
				time = lineUp -> timeOut; // time為此筆工作的取消時刻 
				queue.enqueue( abortList, abortListb, lineUp );
			} // end else
		} // end else
						
		queue.dequeue( lineUp, lineUpb ); 
	} // end while
} // end simulation

void writeOutputFile( Queue::Q * front, Queue::Q * abortList, Queue::Q * doneList, string fileNumber, Queue & queue ) {
	string fileName = "output" + fileNumber + ".txt";
	float totalLen = queue.getLen( front );
	float abortLen = queue.getLen( abortList );
	float doneLen = queue.getLen( doneList );
	float totalDelay = 0;
	Queue::Q * temp = NULL;
	
	ofstream outputfile( fileName.c_str() );
	
	if ( outputfile.is_open() ) {
		outputfile << "\t[Abort Jobs]\n";
		outputfile << "\tOID\tAbort\tDelay\n";
		
		for ( int i = 0 ; i < abortLen ; i++ ) {
			temp = queue.getPtr( i, abortList );
			outputfile << "[" << i + 1 << "]\t" << temp -> oid << "\t" << temp -> process << "\t" << temp -> delay << "\n";
			totalDelay = totalDelay + temp -> delay;
		} // end for
		
		outputfile << "\t[Jobs Done]\n";
		outputfile << "\tOID\tDeparture\tDelay\n";
		
		for ( int i = 0 ; i < doneLen ; i++ ) {
			temp = queue.getPtr( i, doneList );
			outputfile << "[" << i + 1 << "]\t" << temp -> oid << "\t" << temp -> process << "\t" << temp -> delay << "\n";
			totalDelay = totalDelay + temp -> delay;
		} // end for
		
		outputfile << "[Average Delay]\t" << fixed << setprecision( 2 ) << totalDelay / totalLen << " ms\n";
		outputfile << "[Success Rate]\t" << fixed << setprecision( 2 ) <<  100 * doneLen / totalLen << " %\n";
	} // end if
	
	outputfile.close();
} // end writeOutputFile
 
int main() { 
	char command[200];
	bool legalCmd = false; // whether the command is legal
	bool setSucc = false; // whether the fileNumber is legal
	string fileNumber = "\0";
    Queue queue;
    
	for ( int i = 0 ; i < strlen( command ) ; i++ ) command[i] = '\0';
 
	while ( true ) { 
		cout << endl << "**** Simulate FIFO Queues by SQF *****";
		cout << endl << "* 0. Quit                            *";
		cout << endl << "* 1. Sort a file                     *";
		cout << endl << "* 2. Simulate one FIFO queue         *";
		cout << endl << "**************************************";
		cout << endl << "Input a command(0, 1, 2): ";
		cin >> command;
 		
 		setSucc = setFileNumber( command, fileNumber, legalCmd );
 		
		if ( command[0] == 48 && strlen( command ) == 1 ) break;
		else if ( command[0] == 49 && setSucc )  { 
			readFile( queue.front, queue.back, fileNumber, queue );
			shellSort( queue.front, queue.back, queue );
			writeSortedFile( queue.front, queue.back, fileNumber, queue );
		} // end else if 
		else if ( command[0] == 50 && setSucc ) {
			simulation( queue.front, queue.back, queue.abortList, queue.doneList, fileNumber, queue );
			writeOutputFile( queue.front, queue.abortList, queue.doneList, fileNumber, queue );
		} // end else if
		else if ( !legalCmd ) cout << endl << "Command does not exist!" << endl;
	} // end while
 
	return 0;
} // end main
