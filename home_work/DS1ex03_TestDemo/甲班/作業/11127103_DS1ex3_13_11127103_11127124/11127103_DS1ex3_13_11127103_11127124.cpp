// 11127103 ¤ýË¢¿o 11127124 ¦ó«T¾^ 

# include <ctime>
# include <vector>
# include <iomanip>
# include <cstdlib>
# include <fstream>
# include <iostream>
# include <string>
using namespace std;

struct Job {
	int OID;
	int arrival;
	int duration;
	int timeout;
};

struct Job2 {
	int OID;
	int abortOrDeparture;
	int delay;
};

// queue
class Queue {
	public:
		void enqueue( struct Job );
		void dequeue();
		Job getFront();
		int getLineLimit();
		int getLineSize();

	private:
		int waitLineLimit = 3; // waiting line limit
		vector<Job> waitLine;  // storage queued data
	
};

// mission1 calss
class Mission1 {
	public:
		void read( ifstream &inputFile );
		void outputList( );
		void swap( int front, int back );
		void shellSort();
		void sort();
		void write( string fileName );
		void outputTime( string fileName );

	private:
		vector<Job> alist; // storage job list 
		clock_t readtime;  // storage read time 
		clock_t sorttime;  // storage sort time
		clock_t writetime; // storage write time
};

// mission2 class
class CPUQueueSimulation {
	public:
		void read( ifstream &inputFile );
		int findLineLocation( vector<Queue> queueList );
		void addToAbortList( int OID, int abortTime, int delay );
		void addToDoneList( int OID, int departureTime, int delay );
		void Implement();
		void calculate();
		void write( string fileName );

	private:
		int CPUNum = 1;
		vector<Job> alist;           // storage job list 
		vector<Job2> abortJobList;   // storage abort list
		vector<Job2> doneJobList;    // storage done list
		Queue queue;                 // create child objects of class Queue
		int CPUClock = 0;            // cpu's clock
		Job Executing ;              // now executing
		bool isExecuting = false;    // determine if someone is executing
		
		double AverageDelay;         // storage average delay time
		double SuccessRate;          // storage success rate
};

// add new node to queue
void Queue::enqueue( Job job ) {
	waitLine.push_back( job );
}

// delete node
void Queue::dequeue( ) {
	waitLine.erase( waitLine.begin() );
}

// get front node
Job Queue::getFront() {
	return waitLine.front();
}

// get limit of wait line
int Queue::getLineLimit() {
	return waitLineLimit;
}

// get size of wait line
int Queue::getLineSize() {
	return waitLine.size();
}

// read opened file
void Mission1::read( ifstream &inputFile ) {
	clock_t start_time = clock();    // start of the read time 
	string string1, string2, string3, string4;
	inputFile >> string1 >> string2 >> string3 >> string4;
	int tempNum = 0;
	while ( inputFile >> tempNum ) {
		Job job;
		job.OID = tempNum;
		inputFile >> job.arrival >> job.duration >> job.timeout;
		alist.push_back(job);
	} 

	readtime = clock() - start_time; // end of the read time and calculate read time
} // read()

// output unsorted list
void Mission1::outputList( ) {
	cout << "\n\tOID\tArrival\tDuration\tTimeOut\n";
	for ( int i = 0 ; i < alist.size() ; i++ ) {
		cout << "(" << i+1 << ")\t";
		cout << alist[i].OID << "\t" << alist[i].arrival << "\t" << alist[i].duration << "\t" << alist[i].timeout << endl;
	} 
} // outputList()

// swap two structure
void Mission1::swap( int back, int front ) {
	alist.at(back) = alist.at(front);
} // swap()

// shell sort
void Mission1::shellSort() {
	clock_t start_time = clock();    // start of the sort time
	for ( int h = alist.size() / 2 ; h > 0 ; h = h/2 ) {
		for ( int unsorted = h ; unsorted < alist.size() ; unsorted++ ) {
			int loc = unsorted;
			Job temjob = alist.at(unsorted);
			for (; ( loc >= h ) && alist.at(loc-h).arrival > temjob.arrival ; loc = loc - h ) swap( loc, loc-h );
			alist.at(loc) = temjob;
		} 
	} 
	
	sort();
	sorttime = clock() - start_time; // end of the sort time and calculate sort time
} // shellSort()

// sort same OID people
void Mission1::sort() {
	for ( int unsorted = 1 ; unsorted < alist.size() ; unsorted++ ) {
			int loc = unsorted;
			Job temjob = alist.at(unsorted);
			for (; ( loc > 0 ) && alist.at(loc-1).arrival == temjob.arrival && alist.at(loc-1).OID > temjob.OID ; loc-- ) {
				swap( loc, loc-1 );
			} 
			alist.at(loc) = temjob;	
	} 
} // sort()

// write sorted file
void Mission1::write( string fileName ) {
	clock_t start_time = clock();     // start of the write time
	string str = "sorted" + fileName + ".txt";
	ofstream out( str.c_str() );
	
	if ( out.is_open() ) {
		out << "OID\tArrival\tDuration\tTimeOut\n";
		for ( int i = 0 ; i < alist.size() ; i++ ) {
			out << alist[i].OID << "\t" << alist[i].arrival << "\t" << alist[i].duration << "\t" << alist[i].timeout << "\n";
		}
    	out.close();
	} 
	
	writetime = clock() - start_time; // end of the write time and calculate write time
} // write()

// output time data
void Mission1::outputTime( string fileName ) {
	cout << "\nReading data: " << readtime << " clocks (" << fixed << setprecision(2) << double(readtime) << " ms).";
	cout << "\nSorting data: " << sorttime << " clocks (" << fixed << setprecision(2) << double(sorttime) << " ms).";
	cout << "\nWriting data: " << writetime << " clocks (" << fixed << setprecision(2) << double(writetime) << " ms).\n";
	
	string str = "sorted" + fileName + ".txt";
	cout << endl << "See " << str << endl;
} // outputTime()


// read the sortes data into CPUQueueSimulation class
void CPUQueueSimulation::read( ifstream &inputFile ) {
	
	string string1, string2, string3, string4;
	inputFile >> string1 >> string2 >> string3 >> string4;
	int tempNum = 0;
	while ( inputFile >> tempNum ) {
		Job job;
		job.OID = tempNum;
		inputFile >> job.arrival >> job.duration >> job.timeout;
		alist.push_back(job);
	} 
} // read()


// find best line to wait
int CPUQueueSimulation::findLineLocation( vector<Queue> queueList ) {
	int bestLine = 0;
	bool isfind = false;
	for ( int i = 0 ; i < queueList.size() ; i++ ) {
		if ( queueList[i].getLineSize() < 3 && queueList[i].getLineSize() < queueList[bestLine].getLineSize() ) {
			isfind = true;
			bestLine = i;
		}
	}
	if ( !isfind ) return -1;
	return bestLine;
} // findLineLocation()


// add to abort job list
void CPUQueueSimulation::addToAbortList( int OID, int abortTime, int delay ) {
	Job2 tempjob;
	tempjob.OID = OID;
	tempjob.abortOrDeparture = abortTime;
	tempjob.delay = delay;
	abortJobList.push_back(tempjob);
} // addToAbortList()

// add to done job list
void CPUQueueSimulation::addToDoneList( int OID, int departureTime, int delay ) {
	Job2 tempjob;
	tempjob.OID = OID;
	tempjob.abortOrDeparture = departureTime;
	tempjob.delay = delay;
	doneJobList.push_back(tempjob);
} // addToDoneList()

// implement CPU simulation
void CPUQueueSimulation::Implement( ) {
	bool haveDetermine = false; // determine whether have determine completed
	
	while ( alist.size() != 0 || queue.getLineSize() != 0 || isExecuting ) {
		// is not executing, and queue is empty, and list is not empty
		if ( !isExecuting && queue.getLineSize() == 0 && alist.size() != 0 ) {
			Executing = alist[0];
			alist.erase( alist.begin() );
			haveDetermine = true;
			
			// is executing, with completed
			if ( Executing.arrival + Executing.duration <= Executing.timeout ) {
				CPUClock = Executing.arrival + Executing.duration;
				addToDoneList( Executing.OID, CPUClock, CPUClock - Executing.arrival - Executing.duration );
			}
			// is executing, not yet completed
			else {
				CPUClock = Executing.timeout;
				addToAbortList( Executing.OID, CPUClock, CPUClock - Executing.arrival );
			}
		} // if()
		
		// is not executing, and queue is not empty, and have not determine completed
		if ( !isExecuting && queue.getLineSize() != 0 && !haveDetermine ) {
			// taken out from the queue, the timeout has pass
			while ( queue.getLineSize() != 0 && queue.getFront().timeout <= CPUClock ) {
				addToAbortList( queue.getFront().OID, CPUClock, CPUClock - queue.getFront().arrival );
				queue.dequeue();
			}
			// queue is not empty
			if ( queue.getLineSize() != 0 ) {
				Executing = queue.getFront();
				queue.dequeue();
				haveDetermine = true;
				
				// is executing, with completed
				if ( CPUClock + Executing.duration <= Executing.timeout ) {
					CPUClock = CPUClock + Executing.duration;
					addToDoneList( Executing.OID, CPUClock, CPUClock - Executing.arrival - Executing.duration );
				}
				// is executing, not yet completed
				else {
					CPUClock = Executing.timeout;
					addToAbortList( Executing.OID, CPUClock, CPUClock - Executing.arrival );
				}
			}
		} // if()
		
		// have determine completed
		if ( haveDetermine ) {
			// the first in the list has arrived, and push into queue until line size equal to line limit
			while ( alist.size() != 0 && ( queue.getLineSize() < queue.getLineLimit() ) && alist[0].arrival < CPUClock ) {
				queue.enqueue( alist[0] );
				alist.erase( alist.begin() );
			}
			// the first in the list has arrived, but queue has already full, and abort
			while ( alist.size() != 0 && alist[0].arrival < CPUClock ) {
				addToAbortList( alist[0].OID, alist[0].arrival, 0 );
				alist.erase( alist.begin() );
			}
		}
		
		haveDetermine = false;
	} // while()
} // Implement()

// calculate average delay and success rate
void CPUQueueSimulation::calculate() {
	int num = 0;
	for ( int i = 0 ; i < abortJobList.size() ; i++ ) num = num + abortJobList[i].delay;
	for ( int i = 0 ; i < doneJobList.size() ; i++ ) num = num + doneJobList[i].delay;
	AverageDelay = static_cast<double> (num)  / ( abortJobList.size() + doneJobList.size() );
	SuccessRate = ( static_cast<double> (doneJobList.size()) / double( abortJobList.size() + doneJobList.size() ) ) * 100;
} // calculate()

// write execution completed file
void CPUQueueSimulation::write( string fileName ) {
	calculate();
	string str = "output" + fileName + ".txt";
	ofstream out( str.c_str() );
	if ( out ) {
		out << "\t[Abort Jobs]\n\tOID\tAbort\tDelay\n";
		for ( int i = 0 ; i < abortJobList.size() ; i++ ) {
			out << "[" << i+1 << "]\t";
			out << abortJobList[i].OID << "\t" << abortJobList[i].abortOrDeparture << "\t" << abortJobList[i].delay << "\n";
		} 
		out << "\t[Jobs Done]\n\tOID\tDeparture\tDelay\n";
		for ( int i = 0 ; i < doneJobList.size() ; i++ ) {
			out << "[" << i+1 << "]\t";
			out << doneJobList[i].OID << "\t" << doneJobList[i].abortOrDeparture << "\t" << doneJobList[i].delay << "\n";
		} 
		out << "[Average Delay]\t" << fixed << setprecision(2) << AverageDelay << " ms\n" << "[Success Rate]\t" << fixed << setprecision(2) << SuccessRate << " %\n";
		cout << "\nSee " << str << endl;
    	out.close();
	} 
} // write()

// class of UI
class UI{
	public:
		static void output( int mode, string fileName );
};

// message output
void UI::output( int mode , string fileName ) {
	if ( mode == 0 ) {
		cout << endl << "**** Simulate FIFO Queues by SQF *****";
		cout << endl << "* 0. Quit                            *";
		cout << endl << "* 1. Sort a file                     *";
		cout << endl << "* 2. Simulate one FIFO queue         *";
		cout << endl << "**************************************";
		cout << endl << "Input a command(0, 1, 2): ";
	} 
	if ( mode == 1 ) cout << endl << "Input a file name: ";
	if ( mode == 2 ) cout << endl << "### " << fileName << " does not exist! ###" << endl;
	if ( mode == 3 ) cout << endl << "Command does not exist!" << endl;
	if ( mode == 4 ) cout << "\nThe simulation is running...";
} // output()


int main () {
	string prefile = "";                                        // save sorted files
	do {
		int command = 0;                                        
	  	string fileName, fullfileName;
		UI::output( 0, "");                                     // ask command
		cin >> command;                                         // input command
	
		if ( command == 0 ) break;                              // Exit
		else if ( command == 1 ) {                              // implement mission1
			UI::output(1, "");                                  // ask file name
			cin >> fileName;                                    // input file name
			fullfileName = "input" + fileName + ".txt";         // full file name
			ifstream inputFile( fullfileName.c_str() );         // open file
			prefile = fileName;                                 // save file name to prefile
			
			if ( inputFile ) {                                  // success open file 
				Mission1 mission1;                              // create child objects
				mission1.read( inputFile );                     // read the opened file of class Mission1
				mission1.outputList( );                         // output unsorted list
				mission1.shellSort();                           // sort list
				mission1.write( fileName );                     // export sorted file
				mission1.outputTime( fileName );                // output spend time
			}
			else UI::output( 2, "input" + fileName + ".txt" );  // unsuccess oped file
			
		} // else if()
		else if ( command == 2 ) {                              // implement mission2
			if ( prefile == "" ) {                              // does not exist prefile
				UI::output(1, "");                              // ask file name
				cin >> fileName;                                // input file name
				fullfileName = "sorted" + fileName + ".txt";    // full file name
				prefile = fileName;                             // save file name to prefile
			}
			else fullfileName = "sorted" + prefile + ".txt";    // exist prefile
			
			ifstream inputFile( fullfileName.c_str() );         // open file
			if ( inputFile ) {                                  // success oped file
				CPUQueueSimulation CPUQueueSimulationObj;       // create child objects of class CPUQueueSimulation
				UI::output(4, "");                              // message : the simulation is running
				CPUQueueSimulationObj.read( inputFile );        // read opened file
				CPUQueueSimulationObj.Implement();              // implement mission2
				CPUQueueSimulationObj.write( prefile );         // export execution completed file
			}
			else UI::output( 2, "sorted" + prefile + ".txt" );  // does not find file
		} // else if()
		else UI::output( 3, "" );                               // command does not exist
	} while( true );
	
	system("pause");
	return 0;
} // main()
