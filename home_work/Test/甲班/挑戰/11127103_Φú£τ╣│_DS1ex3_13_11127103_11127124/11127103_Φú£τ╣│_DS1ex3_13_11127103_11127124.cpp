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
	int CID;
	int abortOrDeparture;
	int delay;
};

// queue
class Queue {
	public:
		void enqueue( Job job );
		void dequeue();
		Job getFront();
		int getLineLimit();
		int getLineSize();
		void setWaitLineLimit( int num );
	private:
		int waitLineLimit = 3; // waiting line limit
		vector<Job> waitLine;  // storage queued data
};

// cpu 
class CPU {
	public:
		Queue getQueue();
		void setExecutingPosition( Job job );
		Job getExecutingPosition();
		void setStartExecutingTime( int num );
		int getStartExecutingTime();
		void setPreEndTime( int num );
		int getPreEndTime();
		void CPUDequeue();
		void CPUEnqueue( Job job );
	private:
		Job ExecutingPosition;       // now executing
		Queue queue;                 // create child objects of class Queue
		int startExecutingTime = 0;  // start executing time
		int preEndTime = 0;          // previous executing complete time ( single CPU time )
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
		void addToAbortList( int OID, int abortTime, int delay );
		void addToDoneList( int OID, int departureTime, int delay );
		void Implement();
		void calculate();
		void write( string fileName );
	private:
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

// mission3 class
class mutipleCPUSimulation : public CPUQueueSimulation  {
	public:
		mutipleCPUSimulation( int num );
		void read( ifstream &inputFile );
		void addToAbortList( int OID, int abortTime, int delay, int CID);
		void addToDoneList( int OID, int departureTime, int delay, int CID );
		void calculate();
		void write( string fileName );
		bool isAllLineFull();
		int findShortestLine();
		bool isFinishSimulation();
		int judgeExecuting( int CPUPosition );
		int distribute( int CPUPosition );
		int findEarliestLine();
		void handleWaitLine();
		void CleanWaitLine();
		int FindExecutingPosition();
		void Implement();
	private:
		int numOfCPU = 1;            // number of CPU
		vector<CPU> CPUList;         // CPU list
		vector<Job> alist;           // storage job list 
		vector<Job2> abortJobList;   // storage abort list
		vector<Job2> doneJobList;    // storage done list
		int CPUClock = 0;            // cpu's clock
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

// set wait line limit
void Queue::setWaitLineLimit( int num ) {
	waitLineLimit = num;
}

// get queue
Queue CPU::getQueue() {
	return queue;
}

// set now executing
void CPU::setExecutingPosition( Job job ) {
	ExecutingPosition = job;
}

// get now executing
Job CPU::getExecutingPosition() { 
	return ExecutingPosition;
}

// set start executing time
void CPU::setStartExecutingTime( int num ) {
	startExecutingTime = num;
}

// get start executing time
int CPU::getStartExecutingTime() {
	return startExecutingTime;
}

// set previous executing complete time
void CPU::setPreEndTime( int num ) {
	preEndTime = num;
}

// get previous executing complete time
int CPU::getPreEndTime() {
	return preEndTime;
}

// dequeue
void CPU::CPUDequeue() {
	queue.dequeue();
}

// enqueue
void CPU::CPUEnqueue( Job job ) {
	queue.enqueue( job );
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
		for ( int i = 0 ; i < alist.size() ; i++ ) 
			out << alist[i].OID << "\t" << alist[i].arrival << "\t" << alist[i].duration << "\t" << alist[i].timeout << "\n";
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

// constructor
mutipleCPUSimulation::mutipleCPUSimulation( int num ) {
	numOfCPU = num;
}

// read the sortes data into CPUQueueSimulation class
void mutipleCPUSimulation::read( ifstream &inputFile ) {
	
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

// add to abort job list
void mutipleCPUSimulation::addToAbortList( int OID, int abortTime, int delay, int CID ) {
	Job2 tempjob;
	tempjob.OID = OID;
	tempjob.CID = CID;
	tempjob.abortOrDeparture = abortTime;
	tempjob.delay = delay;
	abortJobList.push_back(tempjob);
} // addToAbortList()

// add to done job list
void mutipleCPUSimulation::addToDoneList( int OID, int departureTime, int delay, int CID ) {
	Job2 tempjob;
	tempjob.OID = OID;
	tempjob.CID = CID;
	tempjob.abortOrDeparture = departureTime;
	tempjob.delay = delay;
	doneJobList.push_back(tempjob);
} // addToDoneList()

// calculate average delay and success rate
void mutipleCPUSimulation::calculate() {
	int num = 0;
	for ( int i = 0 ; i < abortJobList.size() ; i++ ) num = num + abortJobList[i].delay;
	for ( int i = 0 ; i < doneJobList.size() ; i++ ) num = num + doneJobList[i].delay;
	AverageDelay = static_cast<double> (num)  / ( abortJobList.size() + doneJobList.size() );
	SuccessRate = ( static_cast<double> (doneJobList.size()) / double( abortJobList.size() + doneJobList.size() ) ) * 100;
} // calculate()

// write execution completed file
void mutipleCPUSimulation::write( string fileName ) {
	calculate();
	string str = "double" + fileName + ".txt";
	ofstream out( str.c_str() );
	if ( out ) {
		out << "\t[Abort Jobs]\n\tOID\tCID\tAbort\tDelay\n";
		for ( int i = 0 ; i < abortJobList.size() ; i++ ) {
			out << "[" << i+1 << "]\t";
			out << abortJobList[i].OID << "\t" << abortJobList[i].CID << "\t" << abortJobList[i].abortOrDeparture << "\t" << abortJobList[i].delay << "\n";
		} 
		out << "\t[Jobs Done]\n\tOID\tCID\tDeparture\tDelay\n";
		for ( int i = 0 ; i < doneJobList.size() ; i++ ) {
			out << "[" << i+1 << "]\t";
			out << doneJobList[i].OID << "\t" << doneJobList[i].CID << "\t" << doneJobList[i].abortOrDeparture << "\t" << doneJobList[i].delay << "\n";
		} 
		out << "[Average Delay]\t" << fixed << setprecision(2) << AverageDelay << " ms\n" << "[Success Rate]\t" << fixed << setprecision(2) << SuccessRate << " %\n";
		cout << "\nSee " << str << endl;
    	out.close();
	} 
} // write()

// determine whether queue is full
bool mutipleCPUSimulation::isAllLineFull( ) {
	for ( int i = 0 ; i < CPUList.size() ; i++ )
		if ( CPUList[i].getQueue().getLineSize() < CPUList[i].getQueue().getLineLimit() ) return false;
	return true;
}

// find best line to wait
int mutipleCPUSimulation::findShortestLine( ) {
	int lineNum = 0;
	for ( int i = 0 ; i < CPUList.size() ; i++ ) 
		if ( CPUList[i].getQueue().getLineSize() < CPUList[lineNum].getQueue().getLineSize() ) lineNum = i;
	return lineNum;
}


// determine whether simulation is finish
bool mutipleCPUSimulation::isFinishSimulation( ) {
	if ( alist.size() != 0 ) return false;
	for ( int i = 0 ; i < CPUList.size() ; i++ ) {
		if ( CPUList[i].getQueue().getLineSize() != 0 ) return false;
	}
	return true;
}

// determine executing type and set
int mutipleCPUSimulation::judgeExecuting( int CPUPosition ) {
	int endTime = CPUList[CPUPosition].getStartExecutingTime() + CPUList[CPUPosition].getExecutingPosition().duration;
	int id = CPUList[CPUPosition].getExecutingPosition().OID;
	int timeOut = CPUList[CPUPosition].getExecutingPosition().timeout;
	int arrivalTime = CPUList[CPUPosition].getExecutingPosition().arrival;
	int startExecutingTime = CPUList[CPUPosition].getStartExecutingTime();
	if ( endTime <= timeOut ) {
		addToDoneList( id, endTime, startExecutingTime - arrivalTime, CPUPosition+1);
		return 1; // case 1 : success done
	}
	else if ( endTime > timeOut && startExecutingTime < timeOut ) {
		addToAbortList( id, timeOut, timeOut - arrivalTime, CPUPosition+1);
		return 2; // case 2 : fail half way
	}
	else if ( startExecutingTime >= timeOut ) {
		addToAbortList( id, startExecutingTime, startExecutingTime - arrivalTime, CPUPosition+1);
		return 3; // case 3 : failed from the beginning
	}
	return -1;
}

// add to done list or abort list
int  mutipleCPUSimulation::distribute( int CPUPosition ) {
	Queue waitLine = CPUList[CPUPosition].getQueue();
	int preTime = CPUList[CPUPosition].getPreEndTime();
	int endTime = waitLine.getFront().duration + preTime;
	int timeOut = waitLine.getFront().timeout;
	int arrivalTime = waitLine.getFront().arrival;
	int id = waitLine.getFront().OID;

	if ( endTime <= timeOut ) {
		addToDoneList( id, endTime, preTime - arrivalTime, CPUPosition+1);
		CPUList[CPUPosition].setPreEndTime( endTime );
		return 1; // case 1 : success done
	}
	else if ( endTime > timeOut && preTime < timeOut ) {
		addToAbortList( id, timeOut, timeOut - arrivalTime, CPUPosition+1);
		CPUList[CPUPosition].setPreEndTime( timeOut );
		return 2; // case 2 : fail half way
	}
	else if ( preTime >= timeOut ) {
		addToAbortList( id, preTime, preTime - arrivalTime, CPUPosition+1);
		return 3; // case 3 : failed from the beginning
	}
}

// find earliest finish
int mutipleCPUSimulation::findEarliestLine() {
	int num = 0;
	// find first is executing cpu
	for ( int i = 0 ; i < CPUList.size() ; i++ ) {
		if ( CPUList[i].getPreEndTime() < CPUList[num].getPreEndTime() ) num = i;
	}
	return num;
}

// find and implement waitline
void mutipleCPUSimulation::handleWaitLine( ) {
	int nextArrivalTime = alist[0].arrival;
	for ( int i = 0 ; i < CPUList.size() ; i++ ) {
		while ( CPUList[i].getQueue().getLineSize() != 0 && CPUList[i].getPreEndTime() <= nextArrivalTime ) {
			int type = distribute( i );
			CPUList[i].CPUDequeue();
		}
	}
}

// clean all waitline
void mutipleCPUSimulation::CleanWaitLine( ) {
	for ( int i = 0 ; i < CPUList.size() ; i++ ) {
		while ( CPUList[i].getQueue().getLineSize() != 0  ) {
			int type = distribute( i );
			CPUList[i].CPUDequeue();
		}
	}
}

// find position where can executing
int mutipleCPUSimulation::FindExecutingPosition() {
	for ( int i = 0 ; i < CPUList.size() ; i++ ) {
		// no one is waiting and person can implement
		if ( CPUList[i].getQueue().getLineSize() == 0 && alist.size() != 0 && alist[0].arrival >= CPUList[i].getPreEndTime() ) {
			return i;
		}
	} // finish find executing position
	return -1;
}

// implement mutiple CPU simulation
void mutipleCPUSimulation::Implement( ) {
	// construct num of cpu
	for ( int i = 0 ; i < numOfCPU ; i++ ) {
		CPU cpu;
		CPUList.push_back(cpu);
	}

	// find all position where can executing
	while ( !isFinishSimulation() ) {
		// find position people
		int exectingCPUPosition = FindExecutingPosition(); // set executing position
		while( FindExecutingPosition() != -1 ) {
			CPUList[exectingCPUPosition].setExecutingPosition( alist[0] );
			CPUList[exectingCPUPosition].setStartExecutingTime( alist[0].arrival );
			alist.erase( alist.begin() );
			judgeExecuting( exectingCPUPosition ); // judge executing position and set executingType
			CPUList[exectingCPUPosition].setPreEndTime( CPUList[exectingCPUPosition].getStartExecutingTime() + CPUList[exectingCPUPosition].getExecutingPosition().duration );
			exectingCPUPosition = FindExecutingPosition();
		}

		// find earliest time and set
		int earlisetPosition = findEarliestLine();
		CPUClock = CPUList[earlisetPosition].getPreEndTime();
		// find People to line
		while ( alist.size() != 0 && alist[0].arrival < CPUClock ) {
			if ( isAllLineFull() ) addToAbortList( alist[0].OID, alist[0].arrival, 0, 0 );
			else {
				int distributeLine = findShortestLine();
				CPUList[distributeLine].CPUEnqueue( alist[0] );
			}
			alist.erase( alist.begin() );
		} // end find queue people

		// find alist first person
		if ( alist.size() != 0 ) handleWaitLine();
		// alist size = 0
		else CleanWaitLine();
	}

} // Implement()

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
		cout << endl << "* 3. Simulate two queues by SQF      *";
		cout << endl << "**************************************";
		cout << endl << "Input a command(0, 1, 2, 3): ";
	} 
	if ( mode == 1 ) cout << endl << "Input a file name: ";
	if ( mode == 2 ) cout << endl << "### " << fileName << " does not exist! ###" << endl;
	if ( mode == 3 ) cout << endl << "Command does not exist!" << endl;
	if ( mode == 4 ) cout << "\nThe simulation is running...";
} // output()


int main () {
	string prefile = "";                                                   // save sorted files
	do {
		int command = 0;                                        
	  	string fileName, fullfileName;
		UI::output( 0, "");                                     	       // ask command
		cin >> command;                                         	       // input command
	
		if ( command == 0 ) break;                             		       // Exit
		else if ( command == 1 ) {                              	       // implement mission1
			UI::output(1, "");                                  	       // ask file name
			cin >> fileName;                                   	 	       // input file name
			fullfileName = "input" + fileName + ".txt";        	 	       // full file name
			ifstream inputFile( fullfileName.c_str() );         	       // open file
			prefile = fileName;                                 	       // save file name to prefile
			
			if ( inputFile ) {                                  		   // success open file 
				Mission1 mission1;                              		   // create child objects
				mission1.read( inputFile );                     		   // read the opened file of class Mission1
				mission1.outputList( );                         		   // output unsorted list
				mission1.shellSort();                           		   // sort list
				mission1.write( fileName );                     		   // export sorted file
				mission1.outputTime( fileName );                		   // output spend time
			}
			else UI::output( 2, "input" + fileName + ".txt" );             // unsuccess oped file
			
		} // command = 1
		else if ( command == 2 ) {                              		   // implement mission2
			if ( prefile == "" ) {                              		   // does not exist prefile
				UI::output(1, "");                              		   // ask file name
				cin >> fileName;                                		   // input file name
				fullfileName = "sorted" + fileName + ".txt";    		   // full file name
				prefile = fileName;                             		   // save file name to prefile
			}
			else fullfileName = "sorted" + prefile + ".txt";    		   // exist prefile
			
			ifstream inputFile( fullfileName.c_str() );         		   // open file
			if ( inputFile ) {                                  		   // success oped file
				CPUQueueSimulation CPUQueueSimulationObj;       		   // create child objects of class CPUQueueSimulation
				UI::output(4, "");                              		   // message : the simulation is running
				CPUQueueSimulationObj.read( inputFile );        		   // read opened file
				CPUQueueSimulationObj.Implement();              		   // implement mission2
				CPUQueueSimulationObj.write( prefile );         		   // export execution completed file
			}
			else UI::output( 2, "sorted" + prefile + ".txt" );  		   // does not find file
		} // command = 2

		else if ( command == 3 ) {
			int numOfCPU = 2;											   // number of CPU

			if ( prefile == "" ) {                              		   // does not exist prefile
				UI::output(1, "");                              		   // ask file name
				cin >> fileName;                               			   // input file name
				fullfileName = "sorted" + fileName + ".txt";    		   // full file name
				prefile = fileName;                             		   // save file name to prefile
			}
			else fullfileName = "sorted" + prefile + ".txt";    		   // exist prefile

			ifstream inputFile( fullfileName.c_str() );         		   // open file
			if ( inputFile ) {                                  		   // success oped file
				mutipleCPUSimulation mutipleCPUSimulationObj( numOfCPU );  // create child objects of class mutipleCPUQueueSimulation
				UI::output(4, "");                              		   // message : the mutiplesimulation is running
				mutipleCPUSimulationObj.read( inputFile );      		   // read opened file
				mutipleCPUSimulationObj.Implement();            		   // implement mission3
				mutipleCPUSimulationObj.write( prefile );       		   // export execution completed file
			}
			else UI::output( 2, "sorted" + prefile + ".txt" );  		   // does not find file
		} // command = 3

		else UI::output( 3, "" );                               		   // command does not exist
	} while( true );
	
	system("pause");
	return 0;
} // main()
