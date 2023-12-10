//group11,11127122≠J®K‡‡,11127148ºB±Í∏©
#include <iostream>
#include <cstdio>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <time.h>
#include <iomanip>

using namespace std;

typedef struct jT{
	int OID;
	int arrival;
	int duration;
	int timeout;
}jobType;

typedef struct aT{
	int OID;
	int Abort;
	int Delay;
}abortType;

typedef struct dT{
	int OID;
	int Departure;
	int Delay;
}doneType;

class JobList {
	vector< jobType > listJ ;  // list of jobs with four columns

public:
	void jobListShellSort( ) {
    int num = listJ.size() ; // vector size
    for ( int h = num / 2 ; h > 0 ; h = h / 2 ) {
      for ( int unsorted = h ; unsorted < num ; ++unsorted ) {
        int loc = unsorted ;
        jobType nextItem ;
        nextItem = listJ.at(unsorted) ;
        for( ; ( loc >= h ) && ( listJ.at(loc-h).arrival >= nextItem.arrival )  ; loc = loc - h ) {
          if ( listJ.at(loc-h).arrival == nextItem.arrival ) {
            if ( listJ.at(loc-h).OID > nextItem.OID ) {
              listJ.at(loc) = listJ.at(loc-h) ;
            } // if( OID equal or not )
            else
              break ;
          } // if( arrival equal or not )
          else
            listJ.at(loc) = listJ.at(loc-h) ;

        } // for( insert one item to property place )

        listJ.at(loc) = nextItem ;
      } // for ( insert by interval h )
    } // for ( change h )

  } // jobListShellSort

  void get( vector< jobType >& alistJ ) {
      alistJ = listJ;
  } // get()

  void show() {
    int sizeJ = listJ.size() ;
    cout<< "\n" << "\tOID\tArrival\tDuration\tTimeout" << "\n";
    for( int i = 0 ; i < sizeJ ; i++ ) {
			cout<< "(" << i+1 << ")" << "\t" << listJ.at(i).OID << "\t" << listJ.at(i).arrival << "\t" << listJ.at(i).duration << "\t" << listJ.at(i).timeout << endl;
    } // for()
  } // show()

  bool load( string file, string fileName ) {
	  bool success = false;

    ifstream ifs( file , std::ios::in );
    if (!ifs.is_open()) {
      cout << endl << "### " << file << " does not exist! ###.\n";
    } // if()
	  else {
		  int i = 0;
		  jobType job;
      string s;
      ifs >> s >> s >> s >> s;  // read OID,Arrival,Duration,Timeout
      while ( ifs >> job.OID >> job.arrival >> job.duration >> job.timeout ) {
		    listJ.push_back(job);
        i++;
      } // while()

    success = true;
	  } // else()

    ifs.close();
	return success;
  } // load( Read the file )

  int loadIn ( string fileName ) {
    std::ofstream ofs ;
    ofs.open( "sorted" + fileName + ".txt" ) ;
    if ( !ofs.is_open() ) {
      cout << "Failed to open file.\n";
      return 1 ;
    } // if()

    int sizeJ = listJ.size() ;
    ofs << "OID\tArrival\tDuration\tTimeOut\n" ;
    for ( int i = 0; i < sizeJ ; i++ ){
      ofs << listJ.at(i).OID << "\t" << listJ.at(i).arrival << "\t" << listJ.at(i).duration << "\t" << listJ.at(i).timeout << "\n" ;
    } // for()

    ofs.close() ;
    return 0 ;
  } // loadIn( load the answer to the file )

  void clear() {
  	listJ.clear();
	} // clear( clear listJ )
};

class JobQueue {

	typedef struct qN {
	  jobType list;
		struct qN *next;  // next element
	} qNode;        // only one data memberwe

	qNode *qFront, *qBack;  // head and tail of queue

public:
	JobQueue(): qFront( NULL ), qBack( NULL ) {}

	int length () {  // Get the length of the queue
		int i = 0;
		for ( qNode *temp = qFront ; temp != NULL ; temp = temp -> next ) {
			i++;
		} // for()

		return i;
	} // length()

	bool isEmpty() const {  // check whether queue is empty
		return qFront == NULL;
	} // isEmpty()

	bool isFull( int len ) {  // Check whether the length of the queue is full
		if ( len >= 3 ) {
			return true;
		} // if()

		return false;
	} // isFull()

	void enQueue( jobType element ) {  // append a new element
    if ( isEmpty() ) {
    	qFront = new qNode;
    	qFront->list = element;
    	qFront -> next = NULL;
    	qBack = qFront;
		} // if()
		else {
			qBack -> next = new qNode;
			qBack = qBack -> next;
			qBack -> list = element;
			qBack -> next = NULL;
		} // else()
	} // enQueue()

	void getFront( jobType& outElement ) {  // get the first element of queue
		if( !isEmpty() ) {
			outElement = qFront -> list;
		} // if()
	} // getFront()

	void deQueue() {  // drop the first element of queue
		if( !isEmpty() ) {
			qNode *temp = qFront;
			qFront = qFront -> next;
			temp -> next = NULL;
			delete temp;
			temp = NULL;
		} // if()
	} // deQueue()

	void deQueue( jobType& outElement ) {  // get and drop the first element of queue
		if ( !isEmpty() ) {
			outElement = qFront -> list;
		  qNode *temp = qFront;
			qFront = qFront -> next;
			temp -> next = NULL;
			delete temp;
			temp = NULL;
		} // if()
	} // deQueue()

  void clearQ () {  // clear up dueue
  	while ( !isEmpty() ) {
  		deQueue();
		} // while()
	} // clearQ()

	~JobQueue() {  // destructor of queue
		while ( !isEmpty() ) {
			deQueue();
		} // while()
	} // ~JobQueue()

};

class AnsList{
	vector<abortType> abortJobs;  // list of aborted jobs with three column
	vector<doneType> doneJobs;    // list of done jobs with three column
	float avgDelay;               // average delay
	float successRate;            // percentage of done jobs

public:
	AnsList():avgDelay(0.0),successRate(0.0) {}  // constructor for initialization

	~AnsList(){  // destructor for initialization
		clear();
	} // ~AnsList()

	void addAbortJob( int oid, int abort, int delay ) {  // append abort job in list
		abortType temp;
		temp.OID = oid;
		temp.Abort = abort;
		temp.Delay = delay;
		abortJobs.push_back(temp);
	} // addAbortJob()

	void addDoneJob( int oid, int departure, int delay ) {  // append done job in list
		doneType temp;
		temp.OID = oid;
		temp.Departure = departure;
		temp.Delay = delay;
		doneJobs.push_back(temp);
	} // addDoneJob()

	void compute() {  // compute avgDelay and successRate
		float quantity = (float)abortJobs.size() + (float)doneJobs.size();  // The number of all abort jobs and done jobs

		for ( int i = 0; i < abortJobs.size(); i++ ) {  // The delay time of all abort jobs
			avgDelay = avgDelay + (float)abortJobs.at(i).Delay;
		} // for()

		for ( int i = 0; i < doneJobs.size(); i++ ) {  // The delay time of all done jobs
			avgDelay = avgDelay + (float)doneJobs.at(i).Delay;
		} // for()

		avgDelay = avgDelay / quantity;
		successRate = (float)doneJobs.size() / quantity;
	} // compute()

	bool putAll( string fileName ) {  // load the answer in file
	  compute();  // compute avgDelay and successRate
		std::ofstream ofs;
    ofs.open( "output" + fileName + ".txt" );
    if ( !ofs.is_open() ) {
      cout << "Failed to open file.\n";
      return false;
    } // if()

    int abortSize = abortJobs.size();
    ofs << "\t[Abort Jobs]\n" << "\tOID\tAbort\tDelay\n";
    for ( int i = 0; i < abortSize ; i++ ){
      ofs << "[" << i+1 << "]\t"<< abortJobs.at(i).OID << "\t" << abortJobs.at(i).Abort << "\t" << abortJobs.at(i).Delay << "\n";
    } // for()

    int doneSize = doneJobs.size();
    ofs << "\t[Jobs Done]\n" << "\tOID\tDeparture\tDelay\n";
    for ( int i = 0; i < doneSize ; i++ ){
      ofs << "[" << i+1 << "]\t"<< doneJobs.at(i).OID << "\t" << doneJobs.at(i).Departure << "\t" << doneJobs.at(i).Delay << "\n";
    } // for()

    ofs << "[Average Delay]\t" << fixed << setprecision(2) << avgDelay << " ms\n";
    ofs << "[Success Rate]\t" << fixed << setprecision(2) << ( successRate * 100 ) << " %\n";

    ofs.close();
    return true;
  } // putAll()

  void clear() {  // clear up
  	abortJobs.clear();
  	doneJobs.clear();
  	avgDelay = 0.0;
		successRate = 0.0;
	} // clear()

};

class Simulation {
private:
  int t, jobSerialIndex ; // t is tribute the current time, and  jobSerialIndex is for recording the index of sortedJobList I have read or current using
  vector< jobType > sortedJobList ; // the job list that is sorted
  typedef vector< jobType > jTvector ; // a type that is a vector element is jobType
  AnsList answer ; // a AnsList class
  JobQueue job ; // a JobQueue class

  struct eList {          // the event list struct
    jobType work ;        // the basic information, like arrival time, OID, timeout, and duration
    bool arrival ;        // this boolean item is to record this item is arrival event or not
    bool departure ;      // this boolean item is to record this item is departure event or not
    int departureTime ;   // if this item is belong to departure, then it must have departure time
  };

  vector< eList > eventList ;  // event list

public :

  Simulation( jTvector sorted ) {   // constructor
    t = 0 ;
    jobSerialIndex = 0 ;
    sortedJobList = sorted ;
  } // job constructor

  void abort( jobType item ) {  // deal abort item
    int delay ;
    delay = t - item.arrival ;     // delay time is current time minus arrival time
    answer.addAbortJob( item.OID, t, delay ) ; // add abort item in the answer list
  } // abort()

  void done( jobType item, int InTime ) {  // deal done item
    int delay = InTime - item.arrival ;    // delay time is the time that item become work, minus the item arrival time
    answer.addDoneJob( item.OID, t, delay ) ;  // add done item in the answer list
  } // done()


  void simulate ( string fileName ) {  // the simulate main function
    int inTime = 0 ;  // record the time that current work item begin working

    if ( jobSerialIndex < sortedJobList.size() ) { // if there is any job in sortedJobList
      eList temp ;                    // this part is going to add one arrival item to event list
      eventList.push_back( temp ) ;   // then the while loop under this part can work fluently
      eventList.at(0).work = sortedJobList.at(jobSerialIndex) ;
      eventList.at(0).arrival = true ;
      inTime = sortedJobList.at(jobSerialIndex).arrival ;
      jobSerialIndex++ ;
    } // if()

    while ( !eventList.empty() ) { // the while loop continue to  deal the event until there is not any event can disposal
      if ( eventList.at(0).arrival )  // if this item is arrival event
        processArrival( inTime ) ;
      else
        processDeparture( inTime ) ;  // if this item is departure event
    } // whjle()

    answer.putAll( fileName ) ;       // load the answer to the file
  } // simulate ()

  void sortEvent() {         // when there is two item in the event list, they should be sort by the time they will be disposaled
    if ( eventList.size() > 1 ) {    // if there is more than one event
      if ( eventList.at(0).arrival ) {  // if the first item is arrival
        if ( eventList.at(0).work.arrival >= eventList.at(1).departureTime ) {  // if the departure item will be disposaled earlier, or equal
            eList temp = eventList.at(0) ;                                      // even their times equal, the departure item must be deal before the arrival item
            eventList.at(0) = eventList.at(1) ;                                 // because if departure item be erased, there will be more item add into event list and queue
            eventList.at(1) = temp ;
        } // if()
      } // if()
      else {       // if the first item is departure
        if ( eventList.at(0).departureTime > eventList.at(1).work.arrival ) { // if the departure item will be disposaled later, then exchange the position with arrival one
            eList temp = eventList.at(0) ;
            eventList.at(0) = eventList.at(1) ;
            eventList.at(1) = temp ;
        } // if()
      } // else
    } // if()
  } // sortEvent()

  void processDeparture( int &inTime ) {  // deal departure item
    jobType item = eventList.at(0).work ;  // this part is to record some information in variables that are easy to identify
    int duration = item.duration ;
    int timeout = item.timeout ;
    int arrival = item.arrival ;

    if ( eventList.at(0).departureTime > t )  // if the item's departureTime is bigger than previous current time
      t = eventList.at(0).departureTime ;     // then change t to item departureTime
                                              // limit this is because of some item can't done, and their departureTime is timeout

    eventList.erase( eventList.begin() ) ;  // erase the first departure item in event
    job.deQueue() ;                          // erase the first item in queue
    if ( timeout < duration + inTime )       // if this item is already time out and can not done. then abort
      abort( item ) ;
    else                                    // else this item is done
      done( item, inTime ) ;

    if ( !job.isEmpty() ) {          // if there still have thing in queue after the first one has been erased
      eList temp ;                   // then add current first item in queue, to event list
      eventList.push_back(temp) ;
      job.getFront( eventList.at( eventList.size() - 1 ).work ) ;
      eventList.at( eventList.size() - 1 ).arrival = false ;
      eventList.at( eventList.size() - 1 ).departure = true ;      // and record it in departure item


      if ( t <  eventList.at( eventList.size() - 1 ).work.arrival )    // if current time is bigger than item arrival
        inTime = eventList.at( eventList.size() - 1 ).work.arrival ;   // then record item begin working time as current time
      else                                                             // else record it as item arrival time
        inTime = t ;


      if ( inTime + eventList.at( eventList.size() - 1 ).work.duration < eventList.at( eventList.size() - 1 ).work.timeout )    // if begin work time + duration is smaller than timeout
        eventList.at( eventList.size() - 1 ).departureTime = inTime + eventList.at( eventList.size() - 1 ).work.duration ;      // then record departure time as begin work time + duration
      else                                                                                                                       // or   record departure time as timeout
        eventList.at( eventList.size() - 1 ).departureTime = eventList.at( eventList.size() - 1 ).work.timeout ;
    } // if()


    if ( eventList.empty() ) {      // if the event list is empty
      if ( jobSerialIndex < sortedJobList.size() ) {         // if there is still have job after departure
        eList temp ;                                         // then add it to event list as arrival one
        eventList.push_back(temp) ;
        eventList.at( eventList.size() - 1 ).work = sortedJobList.at( jobSerialIndex ) ;
        eventList.at( eventList.size() - 1 ).arrival = true ;
        eventList.at( eventList.size() - 1 ).departure = false ;
        jobSerialIndex ++ ;
      } // if()
    } // if()


    sortEvent() ;        // sort event list

    // this part is to set the current time
    if ( !eventList.empty() ) {  // if there is still have event in event list
      if ( eventList.at(0).arrival ) {           // if the first item is arrival
        if ( eventList.at(0).work.arrival > t )  // and arrival time bigger than previous current time
          t = eventList.at(0).work.arrival ;     // then change t to arrival time
      } // if()
      else if ( eventList.at(0).departureTime > t )  // if the first item is departure and departure time bigger than previous current time
        t = eventList.at(0).departureTime ;          // then change t to departure time
    } // if()
  } // processDeparture()

  void processArrival( int &inTime ) {
    jobType item = eventList.at(0).work ;  // this part is to record some information in variables that are easy to identify
    int duration = item.duration ;
    int timeout = item.timeout ;
    int arrival = item.arrival ;

    eventList.erase( eventList.begin() ) ;  // erase the first departure item in event

    if ( ( ( arrival + duration > timeout ) && ( duration < 0 ) ) ||  // if this new item is legal or the queue is full
           ( job.isFull( job.length () -1 ) ) )
      abort( item ) ;                                                 // then abort it
    else {                                                     // if item is legal and there is still have space to queue

      if ( job.isEmpty() ) {                                   // if there is nothing in queue
        job.enQueue( item ) ;                                  // add item in queue
        eList temp ;                                           // and create a departure item in event list as this item
        eventList.push_back(temp) ;
        eventList.at( eventList.size() - 1 ).work = item ;
        eventList.at( eventList.size() - 1 ).arrival = false ;
        eventList.at( eventList.size() - 1 ).departure = true ;


        if ( t <  eventList.at( eventList.size() - 1 ).work.arrival )    // if current time is bigger than item arrival
          inTime = eventList.at( eventList.size() - 1 ).work.arrival ;   // then record item begin working time as current time
        else                                                             // else record it as item arrival time
          inTime = t ;

        if ( inTime + eventList.at( eventList.size() - 1 ).work.duration < eventList.at( eventList.size() - 1 ).work.timeout )    // if begin work time + duration is smaller than timeout
          eventList.at( eventList.size() - 1 ).departureTime = inTime + eventList.at( eventList.size() - 1 ).work.duration ;      // then record departure time as begin work time + duration
        else                                                                                                                      // or   record departure time as timeout
          eventList.at( eventList.size() - 1 ).departureTime = eventList.at( eventList.size() - 1 ).work.timeout ;

      } // if()
      else
        job.enQueue( item ) ;        // add item in queue

    } // else ( )

    if ( jobSerialIndex < sortedJobList.size() ) {       // if there is still have job after previous arrival item
       eList temp ;                                       // then add it to event list as arrival one
       eventList.push_back(temp) ;
       eventList.at( eventList.size() - 1 ).work = sortedJobList.at( jobSerialIndex ) ;
       eventList.at( eventList.size() - 1 ).arrival = true ;
       eventList.at( eventList.size() - 1 ).departure = false ;
       jobSerialIndex ++ ;
    } // if()

    sortEvent() ;        // sort event list

    // this part is to set the current time

    if ( !eventList.empty() ) {  // if there is still have event in event list
      if ( eventList.at(0).arrival ) {           // if the first item is arrival
        if ( eventList.at(0).work.arrival > t )  // and arrival time bigger than previous current time
          t = eventList.at(0).work.arrival ;     // then change t to arrival time
      } // if()
      else if ( eventList.at(0).departureTime > t )  // if the first item is departure and departure time bigger than previous current time
        t = eventList.at(0).departureTime ;          // then change t to departure time
    } // if()
  } // processArrival()

} ; // // Simulation ()


int main(void) {
	int command = 0;
	string file1, file2;  // file1 is the file name of the unsorted data
	                      // file2 is the file name of the sorted data
	JobList aList;        // a JobList class
	clock_t START_read,END_read;
	clock_t START_sort,END_sort;
	clock_t START_write,END_write;
	string fileName;
	bool check = true;

	do {

		cout<<endl<<"**** Simulate FIFO Queues by SQF *****";
		cout<<endl<<"* 0. Quit                            *";
		cout<<endl<<"* 1. Sort a file                     *";
		cout<<endl<<"* 2. Simulate one FIFO queue         *";
		cout<<endl<<"**************************************";
		cout<<endl<<"Input a command(0,1,2): ";
		cin>>command;

		if(command==1) {
			file1.clear();  // clear up file1
			cout<<endl<<"Input a file number: ";
			cin>>fileName;
			file1 = "input" +  fileName + ".txt";
			START_read = clock();
			if ( aList.load( file1, fileName ) ) {  // read unsorted data and store in aList
			  aList.show();         // show aList on screen
			  END_read = clock();
			  START_sort = clock();
			  aList.jobListShellSort();  // shell sort
			  END_sort = clock();
			  START_write = clock();
			  aList.loadIn( fileName );  // load the answer in file
			  END_write = clock();

			  cout << endl;
			  cout <<  "Reading Data: " << (END_read - START_read) / CLOCKS_PER_SEC << " clocks (" << fixed << setprecision(2) << double((END_read - START_read) / CLOCKS_PER_SEC)<< " ms)." << endl;
			  cout <<  "Sorting Data: " << (END_sort - START_sort) / CLOCKS_PER_SEC << " clocks (" << fixed << setprecision(2) << double((END_sort - START_sort) / CLOCKS_PER_SEC)<< " ms)." << endl;
			  cout <<  "Writing Data: " << (END_write - START_write) / CLOCKS_PER_SEC << " clocks (" << fixed << setprecision(2) << double((END_write - START_write) / CLOCKS_PER_SEC)<< " ms)." << endl;

			  cout << endl << "See sorted" << fileName << ".txt" << endl;
		  } // if()
			aList.clear();  // clear up aList
    } // if()
		else if ( command == 2 ) {
			if ( file1.length() == 0 && check ) {
				cout<<endl<<"Input a file number: ";
			  cin>>fileName;
			} // if()

			file2 = "sorted" + fileName + ".txt";
			if ( aList.load( file2, fileName ) ) {  // read sorted data and store in aList
			  vector< jobType > j ;
			  aList.get(j) ;  // get sorted data in aList
			  Simulation simulate( j ) ;
			  simulate.simulate( fileName ) ;  // start simulation
			  aList.clear();  // clear up aList
			  cout << endl << "The simulation is running..." << endl;
			  cout << "See output" << fileName << ".txt" << endl;
			  check = true;
			} // if()
			else {
				check = false;
			} // else()

			file2.clear();  // clear up file2
		} // else if()
		else if (!command)
			break;
		else
			cout<<endl<<"Command does not exist!"<<endl;
  } while ( command != 0 );

  return 0;
  //system ("pause");
} // main()
