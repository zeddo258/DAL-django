/** Data Structures  * 
  * Exercise 3 Quiz  * 
  * Group 4          *
  * 11127107, ³¯¬f·ì * 
  * 11127110, ³\¦Ü©û **/

# include <iostream>
# include <fstream>
# include <ctime>
# include <iomanip>
# include <vector>

using namespace std ;

// ---------------- JobList ---------------------

class JobList {
	
	private :
		
		struct JobListNode { // the data struct of node of JobList
			int oid ;
			int arrival ;
			int duration ;
			int timeout ;
			JobListNode *next ;
		} ; // struct JobListNode
		
		int size ;          // the size of linked list
		JobListNode *head ; // a pointer that point to the head of linked list
		JobListNode *tail ; // a pointer that point to the tail of linked list
		
		JobListNode *find( int index ) ;                                                // find the item to the specify position of the linked list
		void alterNode( int indexA, int oid, int arrival, int duration, int timeout ) ; // alter the node at indexA's data
		
	public :
		
		JobList() ;  // constructor
		~JobList() ; // distructor 
		
		bool isEmpty() ;                                                                      // determine whether the linked list is empty
		int getLength() ;                                                                     // get the length of the linked list
		void add( int oid, int arrival, int duration, int timeout ) ;                         // add the item to the end of the linked list
		void remove( int index ) ;                                                            // delete the item to the specify position of the linked list
		void retrieve( int index, int & oid, int & arrival, int & duration, int & timeout ) ; // get the item of specify position of the linked list
		void printJobList() ;                                                                 // show all items in the linked list
		
		void shellSortOfJobList() ; // sort the file's data by Shell sort
		
} ; // class JobList

// ---------------- JobList End -----------------

// ---------------- Queue -----------------------

class Queue {
	
	private :
		
		struct QueueNode { // the data struct of node of Queue
			int oid ;
			int arrival ;
			int duration ;
			int timeout ;
			QueueNode *next ;
		} ; // struct QueueNode
		
		QueueNode *backPtr ;  // a pointer that point to the back of queue
		QueueNode *frontPtr ; // a pointer that point to the front of queue
		
	public :
		
		Queue() ;  // constructor
		~Queue() ; // distructor
		
		bool isEmpty() ;                                                           // determine whether the queue is empty
		void enqueue( int oid, int arrival, int duration, int timeout ) ;          // add the item to the end of the queue
		void dequeue() ;                                                           // remove the front item of the queue
		void dequeue( int & oid, int & arrival, int & duration, int & timeout ) ;  // get the front item of queue and remove it
		void getFront( int & oid, int & arrival, int & duration, int & timeout ) ; // get the front item of queue
		
}; // class Queue

// ---------------- Queue End -------------------

// ---------------- DoneOrAbortList -------------

class DoneOrAbortList {
	
	private :
		
		struct DoneOrAbortListNode { // the data struct of node of EventList
			int oid ;
			int cid ;
			int arrival ;
			int time ;
			int delay ;
			int indexOfJobList ;
			bool isDone ;
			DoneOrAbortListNode *next ;
		} ; // struct DoneOrAbortListNode
		
		DoneOrAbortListNode *find( int index ) ; // find the item to the specify position of the linked list
		
		int size ; 									// the size of linked list
		DoneOrAbortListNode *head ; // a pointer that point to the head of linked list
		DoneOrAbortListNode *tail ; // a pointer that point to the tail of linked list
		
	public :
		
		DoneOrAbortList() ;  // constructor
		~DoneOrAbortList() ; // destructor
		
		bool isEmpty() ; 	// determine whether the linked list is empty
		int getLength() ; // get the length of the linked list
		void add( int oid, int cid, int arrival, int time, int delay, int indexOfJobList, bool isDone ) ; // add the item to the end of the linked list
		void remove( int index ) ; // delete the item to the specify position of the linked list
		void retrieve( int index, int & oid, int & cid, int & arrival, int & time, int & delay, int & indexOfJobList, bool & isDone ) ; // get the item of specify position of the linked list		

		void getDoneList( DoneOrAbortList & aDoneOrAbortList ) ; // get the done list
		void getAbortList( DoneOrAbortList & aDoneOrAbortList ) ; // get the abort list
		
		void getDoneListOfDouble( DoneOrAbortList & aDoneOrAbortList, vector< int > & doneListRecord ) ; // get the done list
		void getAbortListOfDouble( DoneOrAbortList & aDoneOrAbortList ) ; // get the abort list
		
		void adjustDoneList( vector< int > doneListRecord ) ; // adjust

}; // class DoneOrAbortList

// ---------------- DoneOrAbortList End ---------

// ---------------- EventList -------------------

class EventList {
	
	private :
		
		struct EventListNode { // the data struct of node of EventList
			int oid ;
			int cid ;
			int arrival ;
			int actionTime ;
			int duration ;
			int timeout ;
			bool isArrival ;
			bool isTimeOut ;
			EventListNode *next ;
		} ; // struct EventListNode
		
		EventListNode *find( int index ) ; // find the item to the specify position of the linked list
		
		void alterNode( int indexA, int oid, int cid, int arrival, int actionTime, int duration, int timeout, bool isArrival, bool isTimeOut ) ; // alter the node at indexA's data
		
		int size ; 			  		// the size of linked list
		EventListNode *head ; // a pointer that point to the head of linked list
		EventListNode *tail ; // a pointer that point to the tail of linked list
		
		
	public :
		
		EventList() ;  // constructor
		~EventList() ; // destructor
		
		bool isEmpty() ;  // determine whether the linked list is empty
		int getLength() ; // get the length of the linked list
		void add( int oid, int cid, int arrival, int actionTime, int duration, int timeout, bool isArrival, bool isTimeOut ) ; // add the item to the end of the linked list
		void remove( int index ) ; // delete the item to the specify position of the linked list
		void retrieve( int index, int & oid, int & cid, int & arrival, int & actionTime, int & duration, int & timeout, bool & isArrival, bool & isTimeOut ) ; // get the item of specify position of the linked list
		
		void shellSortOfEventList() ; // sort the linked list's data by Shell sort
		
		void simulation( JobList & aJobList, DoneOrAbortList & aDoneOrAbortList ) ; // simualtion of the cpu's processing
		void processArrival( JobList & aJobList, Queue & aQueue, DoneOrAbortList & aDoneOrAbortList, int & currentTime, int & queueLength ) ; 	// process the arrival event
		void processDeparture( JobList & aJobList, Queue & aQueue, DoneOrAbortList & aDoneOrAbortList, int & currentTime, int & queueLength ) ; // process the departure event
		
		void simulationByDouble( JobList & aJobList, DoneOrAbortList & aDoneOrAbortList, vector< int > & doneListRecord ) ; // simualtion of the double cpu's processing
		void processArrivalOfDouble( JobList & aJobList, Queue & aQueue1, Queue & aQueue2, DoneOrAbortList & aDoneOrAbortList, int & currentTime, int & queueLength1, int & queueLength2 ) ; 	 // proccess the arrival event undex double cpu
		void processDepartureOfDouble( JobList & aJobList, Queue & aQueue1, Queue & aQueue2, DoneOrAbortList & aDoneOrAbortList, int & currentTime, int & queueLength1, int & queueLength2 ) ; // process the departure event undex double cpu
		
}; // class EventList

// ---------------- EventList End ---------------

// ---------------- Other -----------------------

bool inputFile( JobList & aJobList, string & fileNumber, string fileTitle, clock_t & readingDataStart, clock_t & readingDataFinish ) ; // input the specify file
void outputSortedFile( JobList & aJobList, string & fileNumber ) ;                                                                     // output the sorted file
void outputFile( DoneOrAbortList & aDoneList, DoneOrAbortList & aAbortList, string & fileNumber ) ;                                    // output the file
void outputDoubleFile( DoneOrAbortList & aDoneList, DoneOrAbortList & aAbortList, string & fileNumber ) ; 														 // output the double file
void printTime( clock_t readingDataStart, clock_t readingDataFinish, clock_t sortingDataStart, 
                clock_t sortingDataFinish, clock_t writingDataStart, clock_t writingDataFinish ) ;                                     // print the cpu's operating time
                
// ---------------- Other End -------------------

// ---------------- JobList ---------------------

JobList::JobListNode *JobList::find(int index) { // find the item to the specify position of the linked list

	if ( ( index < 1 ) || ( index > getLength() ) ) // when the input index is out of range
		return NULL ; // return a NULL address
	else { // when the input index is in the range
		JobListNode * cur = head ;
		for ( int skip = 1 ; skip < index ; skip++ ) // find the specify location by loop
			cur = cur -> next ;
		
		return cur ; // return the specify location's address
	} // else

} // find()

void JobList::alterNode( int indexA, int oid, int arrival, int duration, int timeout ) { // alter the node at indexA's data

	JobListNode * tempPtrA = find( indexA ) ; // get a pointer that point to the node at position of indexA
	tempPtrA -> oid = oid ; 					// alter oid of the node at position of indexA
	tempPtrA -> arrival = arrival ; 	// alter arrival time of the node at position of indexA
	tempPtrA -> duration = duration ; // alter duration time of the node at position of indexA
	tempPtrA -> timeout = timeout ; 	// alter timeout time of the node at position of indexA
	
} // alterNode()

JobList::JobList() { // constructor

	head = NULL ; // initialize head pointer
	tail = NULL ; // initialize tail pointer
	size = 0 ;    // initialize the integer of size of linked list
	
} // JobList()

JobList::~JobList() { // distructor

	while ( !isEmpty() ) // clear all the node of linked list
		remove( 1 ) ; 		 // remove the first node of linked list

} // ~JobList()

bool JobList::isEmpty() { // determine whether the linked list is empty
	
	return size == 0 ; // when size = 0, return true; else, return false
	
} // isEmpty()

int JobList::getLength() { // get the length of the linked list

	return size ; // return integer of size

} // getLength()

void JobList::add( int oid, int arrival, int duration, int timeout ) { // add the item to the end of the linked list
	
	JobListNode *newPtr = new JobListNode ;	// allocate a new pointer
	newPtr -> oid = oid ;           // save the node's oid
	newPtr -> arrival = arrival ;   // save the node's arrival time
	newPtr -> duration = duration ; // save the node's duration time
	newPtr -> timeout = timeout ;   // save the node's timeout time
	newPtr -> next = NULL ;         // let new node point to NULL
		
	if ( head == NULL ) { // if the linked list has no node
		head = newPtr ; // let head point to new node
		tail = newPtr ; // let tail point to new node
	} // if
	else { // if the linked list has one node or more nodes
		tail -> next = newPtr ; // put new node to the tail of linked list 
		tail = tail -> next ;   // let the tail point to the end node
	} // else
	
	size++ ; // update the length of the linked list
	
} // addJobList()

void JobList::remove( int index ) { // delete the item to the specify position of the linked list
 
	if ( ( index < 1 ) || ( index > getLength() ) ) // whether the index is out of range
		return ;
	else {
		JobListNode * cur ; // allocate memory for node
		size-- ;            // update the size of linked list
		
		if ( index == 1 ) { // when remove the item at the head
			cur = head ;
			head = head -> next ;
		} // if
		else { // when remove the item at the position except the head
			JobListNode * prev = find( index - 1 ) ; // find the previous node of target node
			cur = prev -> next ;                     // set current pointer to point to previous pointer's next pointer
			prev -> next = cur -> next ;             // let current node break off the linked list
		} // else
		
		cur -> next = NULL ; // delete cur pointer
		delete cur ;
		cur = NULL ;
	} // else
	
	for ( JobListNode * cur = head ; cur != NULL ; cur = cur -> next ) // setup the tail pointer
		if ( cur -> next == NULL )
			tail = cur ;

} // remove()

void JobList::retrieve( int index, int & oid, int & arrival, int & duration, int & timeout ) { // get the item of specify position of the linked list

	if ( ( index < 1 ) || ( index > getLength() ) ) // whether the index is out of range
		return ;
	else {
		JobListNode * cur = find( index ) ; // allocate memory for node at the specify position and copy the data of node
		oid = cur -> oid ;
		arrival = cur -> arrival ;
		duration = cur -> duration ;
		timeout = cur -> timeout ;
	} // else

} // retrieve()

void JobList::printJobList() { // show all items in the linked list
		
	if ( !isEmpty() ) { // when the linked list is not empty
		JobListNode *cur = head ; // a pointer that point to current node
		
		cout << "\tOID\tArrival\tDuration\tTimeOut" << endl ; 	 // print out the four titles at first line of file
		for ( int index = 1 ; index <= getLength() ; index++ ) { // retrieve the linked list and print out each node's data
	    	cout << "(" << index << ")\t" << cur -> oid << "\t" << cur -> arrival << "\t" << cur -> duration << "\t" << cur -> timeout << endl ;
	    	cur = cur -> next ; // go to next node
		} // for
	} // if

} // printJobList()

void JobList::shellSortOfJobList() { // sort the file's data by Shell sort

	if ( !isEmpty() ) { // determine whether the linked list is empty
		for ( int h = ( getLength() ) / 2 ; h > 0 ; h = h / 2 ) { // the shell sort's loop
			for ( int unsorted = h + 1 ; unsorted <= getLength() ; unsorted++ ) {
				int location = unsorted ;                 // the integer that represent the index of unsorted node
				int oid1, arrival1, duration1, timeout1 ; // four integers to store data of target node
				int oid2, arrival2, duration2, timeout2 ; // four integers to store data of temporary node
								
				retrieve( location, oid1, arrival1, duration1, timeout1 ) ;     									 // get the data of target node at ( location )
				retrieve( location - h, oid2, arrival2, duration2, timeout2 ) ; 									 // get the data of temporary node at ( location - h )
							
				for ( ; ( location > h ) && ( arrival2 >= arrival1 ) ; location = location - h ) { // the insert sort's loop
					retrieve( location - h, oid2, arrival2, duration2, timeout2 ) ; 								 // get the data of node at ( location - h )
					if ( arrival2 > arrival1 ) {                                      							 // if the arrival time of ( location - h ) > ( location )'s
						alterNode( location, oid2, arrival2, duration2, timeout2 ) ;    							 // alter the data of node at ( location ) with temporary node's data
					} // if
					else if ( arrival2 == arrival1 ) {                                							 // if the arrival time of ( location - h ) = ( location )'s
						if ( oid2 > oid1 ) {                                            							 // if the oid of ( location - h ) > ( location )'s
							alterNode( location, oid2, arrival2, duration2, timeout2 ) ;  							 // alter the data of node at ( location ) with temporary node's data
						} // if
						else break ; // if the oid of ( location - h ) <= ( location )'s
						
					} // else if
					
					else break ; // if the arrival time of ( location - h ) < ( location )'s
				} // for
				
				if ( location > 0 ) { // when the location > zero
					alterNode( location, oid1, arrival1, duration1, timeout1 ) ; // alter the data of node at ( location ) with target node's data
				} // if
			} // for
		} // for
	} // if
	
} // shellSortOfJobList()

// ---------------- JobList End -----------------

// ---------------- Queue -----------------------

Queue::Queue() { // constructor

	backPtr = NULL ;  // initialize a pointer that point to the back of queue
	frontPtr = NULL ; // initialize a pointer that point to the front of queue

} // Queue()

Queue::~Queue() { // distructor

	while ( !isEmpty() ) // clear all the node of queue
		dequeue() ; 			 // clear the front node of queue

} // ~Queue()

bool Queue::isEmpty() { // determine whether the queue is empty

	return backPtr == NULL ; // when backPtr point to NULL, return true; else, return false

} // isEmpty()

void Queue::enqueue( int oid, int arrival, int duration, int timeout ) { // add the item to the end of the queue

	QueueNode *newPtr = new QueueNode ; // allocate a new pointer of queue's node and update the data of node
	newPtr -> oid = oid ;
	newPtr -> arrival = arrival ;
	newPtr -> duration = duration ;
	newPtr -> timeout = timeout ;
	newPtr -> next = NULL ;
	
	if ( isEmpty() ) // if the queue is empty
		frontPtr = newPtr ; // set the front pointer to point to new pointer
	else
		backPtr -> next = newPtr ; // set the back pointer's next pointer to point to new pointer
		
	backPtr = newPtr ; // set the back pointer to point to new pointer

} // enqueue()

void Queue::dequeue() { // remove the front item of the queue

	if ( isEmpty() ) // if queue has no node
		return ;
	else {
		QueueNode *tempPtr = frontPtr ; // a temporary pointer that points as the front pointer
		
		if ( frontPtr == backPtr ) { // if queue only has one node
			frontPtr = NULL ;
			backPtr = NULL ;
		} // if
		else
			frontPtr = frontPtr -> next ; // set the front pointer to point to its next pointer
		
		tempPtr -> next = NULL ; // let temp pointer's next pointer to point to NULL
		delete tempPtr ; // delete the temp pointer
	} // else

} // dequeue()

void Queue::dequeue( int & oid, int & arrival, int & duration, int & timeout ) { // get the front item of queue and remove it

	if ( isEmpty() ) // if queue has no node
		return ;
	else { // copy the item of the front node and remove it
		getFront( oid, arrival, duration, timeout ) ; // get the front item of queue
		dequeue() ; // remove the front item of the queue
	} // else

} // dequeue()

void Queue::getFront( int & oid, int & arrival, int & duration, int & timeout ) { // get the front item of queue

	if ( isEmpty() ) // if queue has no node
		return ;
	else { // copy the items of the front node
		oid = frontPtr -> oid ;
		arrival = frontPtr -> arrival ;
		duration = frontPtr -> duration ;
		timeout = frontPtr -> timeout ;
	} // else

} // getFront()

// ---------------- Queue End -------------------

// ---------------- DoneOrAbortList -------------

DoneOrAbortList::DoneOrAbortListNode *DoneOrAbortList::find( int index ) { // find the item to the specify position of the linked list

	if ( ( index < 1 ) || ( index > getLength() ) ) // when the input index is out of range
		return NULL ; // return a NULL address
	else { // when the input index is in the range
		DoneOrAbortListNode * cur = head ;
		for ( int skip = 1 ; skip < index ; skip++ ) // find the specify location by loop
			cur = cur -> next ;
			
		return cur ; // return the specify location's address
	} // else

} // find()

DoneOrAbortList::DoneOrAbortList() {  // constructor

	head = NULL ; // initialize head pointer
	tail = NULL ; // initialize tail pointer
	size = 0 ;    // initialize the integer of size of linked list

} // EventList()

DoneOrAbortList::~DoneOrAbortList() { // destructor

	while ( !isEmpty() ) // clear all the node of linked list
		remove( 1 ) ; 		 // remove the first node of linked list

} // ~EventList()

int DoneOrAbortList::getLength() { // get the length of the linked list

	return size ; // return integer of size

} // getLength()

bool DoneOrAbortList::isEmpty() { // determine whether the linked list is empty

	return size == 0 ; // when size = 0, return true; else, return false

} // isEmpty()

void DoneOrAbortList::add( int oid, int cid, int arrival, int time, int delay, int indexOfJobList, bool isDone ) { // add the item to the end of the linked list

	DoneOrAbortListNode *newPtr = new DoneOrAbortListNode ;	// allocate a new pointer
	newPtr -> oid = oid ;         							// save the node's oid
	newPtr -> cid = cid ;												// save the node's cid
	newPtr -> arrival = arrival ; 							// save the node's arrival
	newPtr -> time = time ;       							// save the node's done or abort time
	newPtr -> delay = delay ; 	  							// save the node's delay time
	newPtr -> indexOfJobList = indexOfJobList ; // save the node's index of JobList
	newPtr -> isDone = isDone ;   							// save the node's boolean of isDone
	newPtr -> next = NULL ;       							// let new node point to NULL
		
	if ( head == NULL ) { // if the linked list has no node
		head = newPtr ; // let head point to new node
		tail = newPtr ; // let tail point to new node
	} // if
	else { // if the linked list has one node or more nodes
		tail -> next = newPtr ; // put new node to the tail of linked list 
		tail = tail -> next ;   // let the tail point to the end node
	} // else
	
	size++ ; // update the length of the linked list

} // add()

void DoneOrAbortList::remove( int index ) { // delete the item to the specify position of the linked list

	if ( ( index < 1 ) || ( index > getLength() ) ) // whether the index is out of range
		return ;
	else {
		DoneOrAbortListNode * cur ; // allocate memory for node
		size-- ;                    // update the size of linked list
		if ( index == 1 ) { // when remove the item at the head
			cur = head ;
			head = head -> next ;
		} // if
		else { // when remove the item at the position except the head
			DoneOrAbortListNode * prev = find( index - 1 ) ; // find the previous node of target node
			cur = prev -> next ;                             // set current pointer to point to previous pointer's next pointer
			prev -> next = cur -> next ;                     // let current node break off the linked list
		} // else
		
		cur -> next = NULL ; // delete cur pointer
		delete cur ;
		cur = NULL ;
	} // else
	
	for ( DoneOrAbortListNode * cur = head ; cur != NULL ; cur = cur -> next ) // setup the tail pointer
		if ( cur -> next == NULL )
			tail = cur ;

} // remove()

void DoneOrAbortList::retrieve( int index, int & oid, int & cid, int & arrival, int & time, int & delay, int & indexOfJobList, bool & isDone ) { // get the item of specify position of the linked list

	if ( ( index < 1 ) || ( index > getLength() ) ) // whether the index is out of range
		return ;
	else {
		DoneOrAbortListNode * cur = find( index ) ; // allocate memory for node at the specify position and copy the data of node
		oid = cur -> oid ;
		cid = cur -> cid ;
		arrival = cur -> arrival ;
		time = cur -> time ;
		delay = cur -> delay ;
		indexOfJobList = cur -> indexOfJobList ;
		isDone = cur -> isDone ;
	} // else

} // retrieve()

void DoneOrAbortList::getDoneList( DoneOrAbortList & aDoneOrAbortList ) { // get the done list
	
	int oid, cid, arrival, time, delay, indexOfJobList ; // five integers to temporary store data of linked list's node
	bool isDone ; // boolean of linked list's node
	for ( int index = 1 ; index <= aDoneOrAbortList.getLength() ; index++ ) { // output the donelist
		aDoneOrAbortList.retrieve( index, oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // get the data of each node of linked list
		if ( isDone ) { // when the current job is donejob
    	add( oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // add the data into the done list
    } // if
	} // for

} // getDoneList()

void DoneOrAbortList::getAbortList( DoneOrAbortList & aDoneOrAbortList ) { // get the abort list

	int oid, cid, arrival, time, delay, indexOfJobList ; // five integers to temporary store data of linked list's node
	bool isDone ; // boolean of linked list's node
	for ( int index = 1 ; index <= aDoneOrAbortList.getLength() ; index++ ) { // output the abortlist
		aDoneOrAbortList.retrieve( index, oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // get the data of each node of linked list
		if ( !isDone ) { // when the current job is abortjob
    	add( oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // add the data into the abort list
    } // if
	} // for

} // getAbortList()

void DoneOrAbortList::getDoneListOfDouble( DoneOrAbortList & aDoneOrAbortList, vector< int > & doneListRecord ) { // get the done list
	
	int oid, cid, arrival, time, delay, indexOfJobList ; // five integers to temporary store data of linked list's node
	bool isDone ; // boolean of linked list's node
	for ( int index1 = 0 ; index1 < doneListRecord.size() ; index1++ ) {
		for ( int index2 = 1 ; index2 <= aDoneOrAbortList.getLength() ; index2++ ) { // output the donelist
			aDoneOrAbortList.retrieve( index2, oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // get the data of each node of linked list
			if ( isDone && indexOfJobList == doneListRecord[index1] ) { // when the current job is donejob and the index in the vector is exist
	    	add( oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // add the data into the done list
	    } // if
		} // for
	} // for

} // getDoneListOfDouble()

void DoneOrAbortList::getAbortListOfDouble( DoneOrAbortList & aDoneOrAbortList ) { // get the abort list

	int oid, cid, arrival, time, delay, indexOfJobList ; // five integers to temporary store data of linked list's node
	bool isDone ; // boolean of linked list's node
	for ( int index = 1 ; index <= aDoneOrAbortList.getLength() ; index++ ) { // output the abortlist
		aDoneOrAbortList.retrieve( index, oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // get the data of each node of linked list
		if ( !isDone ) { // when the current job is abortjob
    	add( oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // add the data into the abort list
    } // if
	} // for

} // getAbortListOfDouble()

// ---------------- DoneOrAbortList End ---------

// ---------------- EventList -------------------

EventList::EventListNode *EventList::find( int index ) { // find the item to the specify position of the linked list

	if ( ( index < 1 ) || ( index > getLength() ) ) // when the input index is out of range
		return NULL ; // return a NULL address
	else { // when the input index is in the range
		EventListNode * cur = head ;
		for ( int skip = 1 ; skip < index ; skip++ ) // find the specify location by loop
			cur = cur -> next ;
			
		return cur ; // return the specify location's address
	} // else

} // find()

void EventList::alterNode( int indexA, int oid, int cid, int arrival, int actionTime, int duration, int timeout, bool isArrival, bool isTimeOut ) { // alter the node at indexA's data

	EventListNode * tempPtrA = find( indexA ) ; // get a pointer that point to the node at position of indexA
		
	tempPtrA -> oid = oid ; 	    	  		// alter oid of the node at position of indexA
	tempPtrA -> cid = cid ;								// alter cid of the node at position of indexA
	tempPtrA -> arrival = arrival ;       // alter arrival time of the node at position of indexA
	tempPtrA -> actionTime = actionTime ; // alter action time of the node at position of indexA
	tempPtrA -> duration = duration ;     // alter duration time of the node at position of indexA
	tempPtrA -> timeout = timeout ;       // alter timeout time of the node at position of indexA
	tempPtrA -> isArrival = isArrival ;   // alter boolean of isArrival of the node at position of indexA
	tempPtrA -> isTimeOut = isTimeOut ;		// alter boolean of isTimeOut of the node at position of indexA

} // alterNode()

EventList::EventList() {  // constructor

	head = NULL ; // initialize head pointer
	tail = NULL ; // initialize tail pointer
	size = 0 ;    // initialize the integer of size of linked list

} // EventList()

EventList::~EventList() { // destructor

	while ( !isEmpty() ) // clear all the node of linked list
		remove( 1 ) ; 		 // remove the first node of linked list

} // ~EventList()

bool EventList::isEmpty() { // determine whether the linked list is empty

	return size == 0 ; // when size = 0, return true; else, return false

} // isEmpty()

int EventList::getLength() { // get the length of the linked list
	
	return size ; // return integer of size
	
} // getLength()

void EventList::add( int oid, int cid, int arrival, int actionTime, int duration, int timeout, bool isArrival, bool isTimeOut ) { // add the item to the end of the linked list

	EventListNode *newPtr = new EventListNode ;	// allocate a new pointer
	newPtr -> oid = oid ;          			// save the node's oid
	newPtr -> cid = cid ;								// save the node's cid
	newPtr -> arrival = arrival ;       // save the node's arrival time
	newPtr -> actionTime = actionTime ; // save the node's action time
	newPtr -> duration = duration ;     // save the node's delay time
	newPtr -> timeout = timeout ; 			// save the node's timeout time
	newPtr -> isArrival = isArrival ;  	// save the node's boolean of isArrival
	newPtr -> isTimeOut = isTimeOut ;   // save the node's boolean of isTimeOut
	newPtr -> next = NULL ;         		// let new node point to NULL
		
	if ( head == NULL ) { // if the linked list has no node
		head = newPtr ; // let head point to new node
		tail = newPtr ; // let tail point to new node
	} // if
	else { // if the linked list has one node or more nodes
		tail -> next = newPtr ; // put new node to the tail of linked list 
		tail = tail -> next ;   // let the tail point to the end node
	} // else
	
	size++ ; // update the length of the linked list

} // add()

void EventList::remove( int index ) { // delete the item to the specify position of the linked list

	if ( ( index < 1 ) || ( index > getLength() ) ) // whether the index is out of range
		return ;
	else {
		EventListNode * cur ; // allocate memory for node
		size-- ; // update the size of linked list
		if ( index == 1 ) { // when remove the item at the head
			cur = head ;
			head = head -> next ;
		} // if
		else { // when remove the item at the position except the head
			EventListNode * prev = find( index - 1 ) ; // find the previous node of target node
			cur = prev -> next ;                       // set current pointer to point to previous pointer's next pointer
			prev -> next = cur -> next ;               // let current node break off the linked list
		} // else
		
		cur -> next = NULL ; // delete cur pointer
		delete cur ;
		cur = NULL ;
	} // else

	for ( EventListNode * cur = head ; cur != NULL ; cur = cur -> next ) // setup the tail pointer
		if ( cur -> next == NULL )
			tail = cur ;

} // remove()

void EventList::retrieve( int index, int & oid, int & cid, int & arrival, int & actionTime, int & duration, int & timeout, bool & isArrival, bool & isTimeOut ) { // get the item of specify position of the linked list

	if ( ( index < 1 ) || ( index > getLength() ) ) // whether the index is out of range
		return ;
	else {
		EventListNode * cur = find( index ) ; // allocate memory for node at the specify position and copy the data of node
		oid = cur -> oid ;
		cid = cur -> cid ;
		arrival = cur -> arrival ;
		actionTime = cur -> actionTime ;
		duration = cur -> duration ;
		timeout = cur -> timeout ;
		isArrival = cur -> isArrival ;
		isTimeOut = cur -> isTimeOut ;
	} // else

} // retrieve()

void EventList::shellSortOfEventList() { // sort the linked list's data by Shell sort

	if ( !isEmpty() ) { // determine whether the linked list is empty
		for ( int h = ( getLength() ) / 2 ; h > 0 ; h = h / 2 ) { // the shell sort's loop
			for ( int unsorted = h + 1 ; unsorted <= getLength() ; unsorted++ ) {
				int location = unsorted ;                              			 // the integer that represent the index of unsorted node
				int oid1, cid1, arrival1, actionTime1, duration1, timeout1 ; // six integers to store data of target node
				int oid2, cid2, arrival2, actionTime2, duration2, timeout2 ; // six integers to store data of temporary node
				bool isArrival1, isArrival2, isTimeOut1, isTimeOut2 ;  			 // booleans of eventlist of target node and temporary node
								
				retrieve( location, oid1, cid1, arrival1, actionTime1, duration1, timeout1, isArrival1, isTimeOut1 ) ;     		// get the data of target node at ( location )
				retrieve( location - h, oid2, cid2, arrival2, actionTime2, duration2, timeout2, isArrival2, isTimeOut2 ) ; 		// get the data of temporary node at ( location - h )
							
				for ( ; ( location > h ) && ( actionTime2 >= actionTime1 ) ; location = location - h ) { 	  									// the insert sort's loop
					retrieve( location - h, oid2, cid2, arrival2, actionTime2, duration2, timeout2, isArrival2, isTimeOut2 ) ;  // get the data of node at ( location - h )
					if ( actionTime2 > actionTime1 ) {                                                        									// if the action time time of ( location - h ) > ( location )'s
						alterNode( location, oid2, cid2, arrival2, actionTime2, duration2, timeout2, isArrival2, isTimeOut2 ) ;   // alter the data of node at ( location ) with temporary node's data
					} // if
					else if ( actionTime2 == actionTime1 ) {                                                  									// if the action time of ( location ) = ( location - h )'s
						if ( !isArrival1 && isArrival2 ) {                                                      									// if the event of ( location ) is departure and ( location - h ) is arrival
							alterNode( location, oid2, cid2, arrival2, actionTime2, duration2, timeout2, isArrival2, isTimeOut2 ) ; // alter the data of node at ( location ) with temporary node's data
						} // if
						else if ( oid2 > oid1 && isArrival1 && isArrival2 ) {																											// if the event of ( location ) and ( location - h ) both arrival and the oid of ( location - h ) > ( location )
							alterNode( location, oid2, cid2, arrival2, actionTime2, duration2, timeout2, isArrival2, isTimeOut2 ) ; // alter the data of node at ( location ) with temporary node's data
						} // else if
						else if ( cid2 > cid1 && !isArrival1 && !isArrival2 ) { 																									// if the event of ( location ) and ( location - h ) both departure and the cid of ( location - h ) > ( location )
							alterNode( location, oid2, cid2, arrival2, actionTime2, duration2, timeout2, isArrival2, isTimeOut2 ) ; // alter the data of node at ( location ) with temporary node's data
						} // else if
						else break ; // if the oid of ( location ) <= ( location - h )'s or the event of ( location ) and ( location - h ) both departure or the event of ( location ) is arrival and ( location - h ) is departure
						
					} // else if
					else break ; // if the arrival time of ( location ) < ( location - h )'s

				} // for
				
				if ( location > 0 ) { // when the location > zero
					alterNode( location, oid1, cid1, arrival1, actionTime1, duration1, timeout1, isArrival1, isTimeOut1 ) ; // alter the data of node at ( location ) with target node's data
				} // if
			} // for
		} // for
	} // if

} // shellSortOfEventList()

void EventList::simulation( JobList & aJobList, DoneOrAbortList & aDoneOrAbortList ) { // simualtion of the cpu's processing
	
	if ( !aJobList.isEmpty() ) { // when the linked list of job is not empty
		Queue aQueue ; 				// create a queue
		int queueLength = 0 ; // the queue's size
		int currentTime = 0 ; // the initialize time
		
		int oidJL, arrivalJL, durationJL, timeoutJL ; 											// four integers to store data of joblist
		int oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL ; 	// six integers to store data of eventlist
		int oidQ, arrivalQ, durationQ, timeoutQ ; 													// four integers to store data of queue
		bool isArrival = false, isTimeOut = false, isDone = false ; 				// booleans of eventlist and doneorabortlist
		
		vector< int > departureRecord ;  // record whether oid had built departure event
		bool isDepartureRecord = false ; // boolean that determine whether the front item of queue create a departure event
		
		for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) { // retrieve each node of jobList
			aJobList.retrieve( index, oidJL, arrivalJL, durationJL, timeoutJL ) ; 	 		// get the data of the each node of joblist
			add( oidJL, 0, arrivalJL, arrivalJL, durationJL, timeoutJL, true, false ) ; // add arrival event into eventList
		} // for
		
		while ( !isEmpty() ) { // when the event is not completed yet, keep doing loop
			
			retrieve( 1, oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL, isArrival, isTimeOut ) ; // get the data of the first node of eventlist
			
			if ( currentTime <= actionTimeEL ) // when the current time before or equal the next event's action time
				currentTime = actionTimeEL ; 		 // update the current time
			
			if ( isArrival ) { // process the arrival event
				processArrival( aJobList, aQueue, aDoneOrAbortList, currentTime, queueLength ) ; 	 // proccess the arrival event
			} // if
			else { // process the departure event
				processDeparture( aJobList, aQueue, aDoneOrAbortList, currentTime, queueLength ) ; // process the departure event
			} // else
				
			aQueue.getFront( oidQ, arrivalQ, durationQ, timeoutQ ) ; // get the data of the front node of queue
			int indexOfOid ; // the integer for target oid's index in jobList
			for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) { // retrieve each node of jobList
				aJobList.retrieve( index, oidJL, arrivalJL, durationJL, timeoutJL ) ; // get the data of the each node of joblist
				if ( oidQ == oidJL && arrivalQ == arrivalJL && durationQ == durationJL && timeoutQ == timeoutJL ) { // when find the same data
					indexOfOid = index ; // record the index
					break ;
				} // if
			} // for
			
			for ( int index = 0 ; index < departureRecord.size() ; index++ ) { // retrieve the vector of record oid's index
				if ( indexOfOid == departureRecord[index] ) { // when find the same index between queue's front and vector
					isDepartureRecord = true ; 		 // find
					break ;
				} // if
				else isDepartureRecord = false ; // not find
			} // for

			if ( !isDepartureRecord ) { // when the front of queue is not create departure event
				aQueue.getFront( oidQ, arrivalQ, durationQ, timeoutQ ) ; // get the data of the front node of queue
				if ( currentTime + durationQ <= timeoutQ ) { // not timeout
					add( oidQ, 0, arrivalQ, currentTime + durationQ, durationQ, timeoutQ, false, false ) ; // add departure event into eventList
				} // if
				else { // timeout
					add( oidQ, 0, arrivalQ, timeoutQ, durationQ, timeoutQ, false, true ) ; // add departure event into eventList
					
					if ( timeoutQ < currentTime ) // current time > front of queue's timeout time
						aDoneOrAbortList.add( oidQ, 0, arrivalQ, currentTime, currentTime - arrivalQ, indexOfOid, false ) ; // add the data into the abortlist
					else // current time <= front of queue's timeout time
						aDoneOrAbortList.add( oidQ, 0, arrivalQ, timeoutQ, timeoutQ - arrivalQ, indexOfOid, false ) ; // add the data into the abortlist
				} // else
				
				departureRecord.push_back( indexOfOid ) ; // push the index into the vector
				shellSortOfEventList() ; // sort the eventlist by ShellSort
			} // if
		} // while
		
		departureRecord.clear() ; // clear the vector's data
	} // if

} // simulation()

void EventList::processArrival( JobList & aJobList, Queue & aQueue, DoneOrAbortList & aDoneOrAbortList, int & currentTime, int & queueLength ) { // process the arrival event
	
	int oidJL, arrivalJL, durationJL, timeoutJL ; // four integers to store data of joblist
	int oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL ; // six integers to store data of eventlist
	int oidQ, arrivalQ, durationQ, timeoutQ ; // four integers to store data of queue
	bool isArrival, isTimeOut ; // booleans of eventlist
	
	retrieve( 1, oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL, isArrival, isTimeOut ) ; // get the data of the first node of eventlist
	
	int indexOfOid ; // the integer of target oid's index in jobList
	for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) { // retrieve each node of jobList
		aJobList.retrieve( index, oidJL, arrivalJL, durationJL, timeoutJL ) ; // get the data of the each node of joblist
		if ( oidEL == oidJL && arrivalEL == arrivalJL && durationEL == durationJL && timeoutEL == timeoutJL ) { // when the data same
			indexOfOid = index ; // record the index
			break ;
		} // if
	} // for
	
	if ( queueLength < 4 ) { // queue is not full
		aQueue.enqueue( oidEL, actionTimeEL, durationEL, timeoutEL ) ; // add the new node into queue
		queueLength++ ; // the size of queue increase
	} // if
	else { // queue is full
		aDoneOrAbortList.add( oidEL, 0, arrivalEL, currentTime, 0, indexOfOid, false ) ; // add the data into the abortlist
	} // else
	
	remove( 1 ) ; // remove the first event from eventlist
	
} // processArrival()

void EventList::processDeparture( JobList & aJobList, Queue & aQueue, DoneOrAbortList & aDoneOrAbortList, int & currentTime, int & queueLength ) { // process the departure event

	int oidJL, arrivalJL, durationJL, timeoutJL ; // four integers to store data of joblist
	int oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL ; // six integers to store data of eventlist
	int oidQ, arrivalQ, durationQ, timeoutQ ; // four integers to store data of queue
	bool isArrival, isTimeOut ; // booleans of eventlist
	
	retrieve( 1, oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL, isArrival, isTimeOut ) ;	// get the data of the first node of eventlist
	
	int indexOfOid ; // the integer of target oid's index in jobList
	for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) { // retrieve each node of jobList
		aJobList.retrieve( index, oidJL, arrivalJL, durationJL, timeoutJL ) ; // get the data of the each node of joblist
		if ( oidEL == oidJL && arrivalEL == arrivalJL && durationEL == durationJL && timeoutEL == timeoutJL ) { // when the data same
			indexOfOid = index ; // record the index
			break ;
		} // if
	} // for
	
	aQueue.dequeue( oidQ, arrivalQ, durationQ, timeoutQ ) ; // remove the front node from queue
	queueLength-- ; // the size of queue decrease

	if ( !isTimeOut ) { // not timeout
		aDoneOrAbortList.add( oidEL, 0, arrivalEL, currentTime, currentTime - arrivalEL - durationEL, indexOfOid, true ) ; // add the data into the donelist
	} // if
	
	remove( 1 ) ; // remove the first event from eventlist
	
} // processDeparture()

void EventList::simulationByDouble( JobList & aJobList, DoneOrAbortList & aDoneOrAbortList, vector< int > & doneListRecord ) { // simualtion of the double cpu's processing
	
	if ( !aJobList.isEmpty() ) { // when the linked list of job is not empty
		Queue aQueue1, aQueue2 ; 								 // create a queue
		int queueLength1 = 0, queueLength2 = 0 ; // the queues' size
		int currentTime = 0 ; 									 // the initialize time
		
		int oidJL, arrivalJL, durationJL, timeoutJL ; 										 // four integers to store data of joblist
		int oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL ; // six integers to store data of eventlist
		int oidQ, arrivalQ, durationQ, timeoutQ ; 												 // four integers to store data of queue
		bool isArrival = false, isTimeOut = false, isDone = false ; 			 // booleans of eventlist and doneorabortlist
		
		vector< int > departureRecord ;  // record whether oid had built departure event
		bool isDepartureRecord = false ; // boolean that determine whether the front item of queue create a departure event
		
		for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) { // retrieve each node of jobList
			aJobList.retrieve( index, oidJL, arrivalJL, durationJL, timeoutJL ) ; 	 		// get the data of the each node of joblist
			add( oidJL, 0, arrivalJL, arrivalJL, durationJL, timeoutJL, true, false ) ; // add arrival event into eventList
		} // for
		
		while ( !isEmpty() ) { // when the event is not completed yet, keep doing loop
			
			retrieve( 1, oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL, isArrival, isTimeOut ) ; // get the data of the first node of eventlist
			
			if ( currentTime <= actionTimeEL ) // when the current time before or equal the next event's action time
				currentTime = actionTimeEL ; // update the current time
							
			if ( isArrival ) { // process the arrival event
				processArrivalOfDouble( aJobList, aQueue1, aQueue2, aDoneOrAbortList, currentTime, queueLength1, queueLength2 ) ; 	// proccess the arrival event undex double cpu
			} // if
			else { // process the departure event
				processDepartureOfDouble( aJobList, aQueue1, aQueue2, aDoneOrAbortList, currentTime, queueLength1, queueLength2 ) ; // process the departure event undex double cpu
			} // else
				
			aQueue1.getFront( oidQ, arrivalQ, durationQ, timeoutQ ) ; // get the data of the front node of queue
			int indexOfOid ; // the integer of target oid's index in jobList
			for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) { // retrieve each node of jobList
				aJobList.retrieve( index, oidJL, arrivalJL, durationJL, timeoutJL ) ; // get the data of the each node of joblist
				if ( oidQ == oidJL && arrivalQ == arrivalJL && durationQ == durationJL && timeoutQ == timeoutJL ) { // when the data same
					indexOfOid = index ; // record the index
					break ;
				} // if
			} // for
			
			for ( int index = 0 ; index < departureRecord.size() ; index++ ) { // retrieve the vector of record index
				if ( indexOfOid == departureRecord[index] ) { // when find the same index between queue's front and vector
					isDepartureRecord = true ; 		 // find
					break ;
				} // if
				else isDepartureRecord = false ; // not find
			} // for

			if ( !isDepartureRecord ) { // when the front of queue is not create departure event
				aQueue1.getFront( oidQ, arrivalQ, durationQ, timeoutQ ) ; // get the data of the front node of queue
				if ( currentTime + durationQ <= timeoutQ ) { // not timeout
					add( oidQ, 1, arrivalQ, currentTime + durationQ, durationQ, timeoutQ, false, false ) ; 		// add departure event into eventList
					
					doneListRecord.push_back( indexOfOid ) ;
				} // if
				else { // timeout
					add( oidQ, 1, arrivalQ, timeoutQ, durationQ, timeoutQ, false, true ) ; 								 		// add departure event into eventList
					
					if ( timeoutQ < currentTime ) // current time > front of queue's timeout time
						aDoneOrAbortList.add( oidQ, 1, arrivalQ, currentTime, currentTime - arrivalQ, indexOfOid, false ) ; // add the data into the abortlist
					else // current time <= front of queue's timeout time
						aDoneOrAbortList.add( oidQ, 1, arrivalQ, timeoutQ, timeoutQ - arrivalQ, indexOfOid, false ) ; 			// add the data into the abortlist
				} // else
				
				departureRecord.push_back( indexOfOid ) ; // push the oid into the vector
				shellSortOfEventList() ; // sort the eventlist by ShellSort
			} // if
			
			aQueue2.getFront( oidQ, arrivalQ, durationQ, timeoutQ ) ; // get the data of the front node of queue
			for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) { // retrieve each node of jobList
				aJobList.retrieve( index, oidJL, arrivalJL, durationJL, timeoutJL ) ; // get the data of the each node of joblist
				if ( oidQ == oidJL && arrivalQ == arrivalJL && durationQ == durationJL && timeoutQ == timeoutJL ) { // when the data same
					indexOfOid = index ; // record the index
					break ;
				} // if
			} // for
			
			for ( int index = 0 ; index < departureRecord.size() ; index++ ) { // retrieve the vector of record index
				if ( indexOfOid == departureRecord[index] ) { // when find the same oid between queue's front and vector
					isDepartureRecord = true ; 		 // find
					break ;
				} // if
				else isDepartureRecord = false ; // not find
			} // for

			if ( !isDepartureRecord ) { // when the front of queue is not create departure event
				aQueue2.getFront( oidQ, arrivalQ, durationQ, timeoutQ ) ; // get the data of the front node of queue
				if ( currentTime + durationQ <= timeoutQ ) { // not timeout
					add( oidQ, 2, arrivalQ, currentTime + durationQ, durationQ, timeoutQ, false, false ) ; // add departure event into eventList
					
					doneListRecord.push_back( indexOfOid ) ;
				} // if
				else { // timeout
					add( oidQ, 2, arrivalQ, timeoutQ, durationQ, timeoutQ, false, true ) ; // add departure event into eventList
					
					if ( timeoutQ < currentTime ) // current time > front of queue's timeout time
						aDoneOrAbortList.add( oidQ, 2, arrivalQ, currentTime, currentTime - arrivalQ, indexOfOid, false ) ; // add the data into the abortlist
					else // current time <= front of queue's timeout time
						aDoneOrAbortList.add( oidQ, 2, arrivalQ, timeoutQ, timeoutQ - arrivalQ, indexOfOid, false ) ; // add the data into the abortlist
				} // else
				
				departureRecord.push_back( indexOfOid ) ; // push the oid into the vector
				shellSortOfEventList() ; // sort the eventlist by ShellSort
			} // if
			
		} // while
		
		departureRecord.clear() ; // clear the vector's data
	} // if

} // simulationByDouble()

void EventList::processArrivalOfDouble( JobList & aJobList, Queue & aQueue1, Queue & aQueue2, DoneOrAbortList & aDoneOrAbortList, int & currentTime, int & queueLength1, int & queueLength2 ) { // proccess the arrival event undex double cpu
	
	int oidJL, arrivalJL, durationJL, timeoutJL ; // four integers to store data of joblist
	int oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL ; // six integers to store data of eventlist
	bool isArrival, isTimeOut ; // booleans of eventlist
	
	retrieve( 1, oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL, isArrival, isTimeOut ) ; // get the data of the first node of eventlist
	
	int indexOfOid ; // the integer of target oid's index in jobList
	for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) { // retrieve each node of jobList
		aJobList.retrieve( index, oidJL, arrivalJL, durationJL, timeoutJL ) ; // get the data of the each node of joblist
		if ( oidEL == oidJL && arrivalEL == arrivalJL && durationEL == durationJL && timeoutEL == timeoutJL ) { // when the data same
			indexOfOid = index ; // record the index
			break ;
		} // if
	} // for
	
	if ( queueLength1 < 4 || queueLength2 < 4 ) { // queue1 or queue2 not full
		if ( queueLength1 == 0 ) { 																				// queue1 is empty
			aQueue1.enqueue( oidEL, actionTimeEL, durationEL, timeoutEL ) ; // add the new node into queue
			queueLength1++ ; 																								// the size of queue1 increase
		} // if
		else if ( queueLength2 == 0 ) { 																	// queue2 is empty
			aQueue2.enqueue( oidEL, actionTimeEL, durationEL, timeoutEL ) ; // add the new node into queue
			queueLength2++ ; 																								// the size of queue2 increase
		} // else if
		else if ( queueLength1 < queueLength2 && queueLength1 < 4 ) { 		// queue1's data amount < queue2's and queue1 not full
			aQueue1.enqueue( oidEL, actionTimeEL, durationEL, timeoutEL ) ; // add the new node into queue
			queueLength1++ ; 																								// the size of queue1 increase
		} // else if
		else if ( queueLength1 > queueLength2 && queueLength2 < 4 ) { 		// queue1's data amount > queue2's and queue2 not full
			aQueue2.enqueue( oidEL, actionTimeEL, durationEL, timeoutEL ) ; // add the new node into queue
			queueLength2++ ; 																								// the size of queue2 increase
		} // else if
		else if ( queueLength1 == queueLength2 && queueLength1 < 4 ) { 		// queue1's data amount = queue2's and queue1 not full
			aQueue1.enqueue( oidEL, actionTimeEL, durationEL, timeoutEL ) ; // add the new node into queue
			queueLength1++ ; 																								// the size of queue1 increase
		} // else if
		else { // two queues are both full
			aDoneOrAbortList.add( oidEL, 0, arrivalEL, currentTime, 0, indexOfOid, false ) ; // add the data into the abortlist
		} // else
	} // if
	else { // two queues are both full
		aDoneOrAbortList.add( oidEL, 0, arrivalEL, currentTime, 0, indexOfOid, false ) ; // add the data into the abortlist
	} // else
	
	remove( 1 ) ; // remove the first event from eventlist
	
} // processArrivalOfDouble()

void EventList::processDepartureOfDouble( JobList & aJobList, Queue & aQueue1, Queue & aQueue2, DoneOrAbortList & aDoneOrAbortList, int & currentTime, int & queueLength1, int & queueLength2 ) { // process the departure event undex double cpu

	int oidJL, arrivalJL, durationJL, timeoutJL ; // four integers to store data of joblist
	int oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL ; // six integers to store data of eventlist
	bool isArrival, isTimeOut ; // boolean of eventlist
	
	retrieve( 1, oidEL, cidEL, arrivalEL, actionTimeEL, durationEL, timeoutEL, isArrival, isTimeOut ) ;	// get the data of the first node of eventlist
	
	int indexOfOid ; // the integer of target oid's index in jobList
	for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) { // retrieve each node of jobList
		aJobList.retrieve( index, oidJL, arrivalJL, durationJL, timeoutJL ) ; // get the data of the each node of joblist
		if ( oidEL == oidJL && arrivalEL == arrivalJL && durationEL == durationJL && timeoutEL == timeoutJL ) { // when the data same
			indexOfOid = index ; // record the index
			break ;
		} // if
	} // for
	
	if ( cidEL == 1 ) { // cpu1's departure event
		aQueue1.dequeue() ; // remove the front node from queue1
		queueLength1-- ; 																				 // the size of queue1 decrease
	} // if
	else if ( cidEL == 2 ) { // cpu2's departure event
		aQueue2.dequeue() ; // remove the front node from queue2
		queueLength2-- ; 																				 // the size of queue2 decrease
	} // else if
	
	if ( !isTimeOut ) { // not timeout
		aDoneOrAbortList.add( oidEL, cidEL, arrivalEL, currentTime, currentTime - arrivalEL - durationEL, indexOfOid, true ) ; // add the data into the donelist
	} // if
	
	remove( 1 ) ; // remove the first event from eventlist
	
} // processDepartureOfDouble()

// ---------------- EventList End ---------------

// ---------------- Other -----------------------

bool inputFile( JobList & aJobList, string & fileNumber, string fileTitle, clock_t & readingDataStart, clock_t & readingDataFinish  ) { // input the specify file

	string fileName ; // a string for file's total name
	
	if ( fileTitle == "input" || fileNumber == "NULL" ) { // the special case of input file number
		cout << endl << "Input a file number: " ;
		cin >> fileNumber ;
	} // if
	
	fileName =  fileTitle + fileNumber + ".txt" ; // combine the "input", number and ".txt"
	
	ifstream file( fileName.c_str() ) ; // change fileName from string to char and open it
	
	if( !file.is_open() ) { // if the file not exist
		cout << endl << "### " << fileName + " does not exist! ###" << endl ;
		return false ;
	} // if
	else { // if the file success opened
		readingDataStart = clock() ; // start measuring the time spent by reading data from the file
		
		string line ;           // a string for get first line of file
		getline( file, line ) ; // get first line
		
		int oid, arrival, duration, timeout ; // four integers to get each data from file
		while ( file >> oid ) {                               // a loop that input the data from file until end of file
			file >> arrival >> duration >> timeout ;          	// input the data from file
    		aJobList.add( oid, arrival, duration, timeout ) ; // add the item to the end of the linked list
		} // while
		
		file.close() ; // close the file
		readingDataFinish = clock() ; // stop measuring the time spent by reading data from the file
		
		if ( aJobList.isEmpty() && fileTitle == "input" ) { // if the input file is not empty, input not success
			cout << endl << "### Get nothing from the file input" << fileNumber << ".txt ! ###" << endl ;
			return false ;
		} // if
		
		return true ;
	} // else
} // inputFile()

void outputSortedFile( JobList & aJobList, string & fileNumber ) { // output the sorted file
	
	if ( !aJobList.isEmpty() ) { // determine whether the linked list is empty
		ofstream newFile ;                                 // output file stream
		int oid, arrival, duration, timeout ;              // four integers to temporary store data of linked list's node
		string fileName = "sorted" + fileNumber + ".txt" ; // combine the file name with "sorted", fileNumber and ".txt"
		
		newFile.open( fileName.c_str() ) ; // create the new file named sortedNUM.txt and open it
		
		newFile << "OID\tArrival\tDuration\tTimeOut" << endl ;                                 // output the title
		for ( int index = 1 ; index <= aJobList.getLength() ; index++ ) {                      // retrieve the linked list
			aJobList.retrieve( index, oid, arrival, duration, timeout ) ;                      	 // get each data of linked list's node
			newFile << oid << "\t" << arrival << "\t" << duration << "\t" << timeout << endl ; 	 // write each data into output file
		} // for
		
		newFile.close() ; // close the file
	} // if
	
} // outputSortedFile()

void outputFile( DoneOrAbortList & aDoneList, DoneOrAbortList & aAbortList, string & fileNumber ) { // output the file

	ofstream newFile ;                                 // output file stream	
	string fileName = "output" + fileNumber + ".txt" ; // combine the file name with "sorted", fileNumber and ".txt"
	
	newFile.open( fileName.c_str() ) ; // create the new file named sortedNUM.txt and open it
	
	double delayTotal = 0 ;
	int oid, cid, arrival, time, delay, indexOfJobList ; // five integers to temporary store data of linked list's node
	bool isDone ; // boolean of linked list's node
	
	newFile << "\t[Abort Jobs]" << endl ; // output the name of list
	newFile << "\tOID\tAbort\tDelay" << endl ; // output the data's title
	
	for ( int index = 1 ; index <= aAbortList.getLength() ; index++ ) { // output the abortlist
		aAbortList.retrieve( index, oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // get the data of each node of abort list
    newFile << "[" << index << "]\t" << oid << "\t" << time << "\t" << delay << endl ; // output the data
    delayTotal = delayTotal + delay ;
	} // for
		
	newFile << "\t[Jobs Done]" << endl ; // output the name of list
	newFile << "\tOID\tDeparture\tDelay" << endl ; // output the data's title
	
	for ( int index = 1 ; index <= aDoneList.getLength() ; index++ ) { // output the donelist
		aDoneList.retrieve( index, oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // get the data of each node of done list
    newFile << "[" << index << "]\t" << oid << "\t" << time << "\t" << delay << endl ; // output the data
    delayTotal = delayTotal + delay ;
	} // for
	
	double averageDelay = ( double )( delayTotal ) / ( aAbortList.getLength() + aDoneList.getLength() ) ; 		 						// calculate the average delay
	double successRate = ( double )( aDoneList.getLength() * 100 ) / ( aAbortList.getLength() + aDoneList.getLength() ) ; // calculate the success rate
	
	newFile << "[Average Delay]\t" << fixed << setprecision(2) << averageDelay << " ms" << endl ; // output the average delay
	newFile << "[Success Rate]\t" << fixed << setprecision(2) << successRate << " %" << endl ; // output the success rate
	
	newFile.close() ; // close the file

} // outputFile()

void outputDoubleFile( DoneOrAbortList & aDoneList, DoneOrAbortList & aAbortList, string & fileNumber ) { // output the double file

	ofstream newFile ;                                 // output file stream	
	string fileName = "double" + fileNumber + ".txt" ; // combine the file name with "sorted", fileNumber and ".txt"
	
	newFile.open( fileName.c_str() ) ; // create the new file named sortedNUM.txt and open it
	
	double delayTotal = 0 ;
	int oid, cid, arrival, time, delay, indexOfJobList ; // four integers to temporary store data of linked list's node
	bool isDone ; 
	
	newFile << "\t[Abort Jobs]" << endl ; // output the name of list
	newFile << "\tOID\tCID\tAbort\tDelay" << endl ; // output the data's title
	
	for ( int index = 1 ; index <= aAbortList.getLength() ; index++ ) { // output the abortlist
		aAbortList.retrieve( index, oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // get the data of each node of abort list
    newFile << "[" << index << "]\t" << oid << "\t" << cid << "\t" << time << "\t" << delay << endl ; // output the data
    delayTotal = delayTotal + delay ;
	} // for
		
	newFile << "\t[Jobs Done]" << endl ; // output the name of list
	newFile << "\tOID\tCID\tDeparture\tDelay" << endl ; // output the data's title
	
	for ( int index = 1 ; index <= aDoneList.getLength() ; index++ ) { // output the donelist
		aDoneList.retrieve( index, oid, cid, arrival, time, delay, indexOfJobList, isDone ) ; // get the data of each node of done list
    newFile << "[" << index << "]\t" << oid << "\t" << cid << "\t" << time << "\t" << delay << endl ; // output the data
    delayTotal = delayTotal + delay ;
	} // for
	
	double averageDelay = ( double )( delayTotal ) / ( aAbortList.getLength() + aDoneList.getLength() ) ; 		 						// calculate the average delay
	double successRate = ( double )( aDoneList.getLength() * 100 ) / ( aAbortList.getLength() + aDoneList.getLength() ) ; // calculate the success rate
	
	newFile << "[Average Delay]\t" << fixed << setprecision(2) << averageDelay << " ms" << endl ; // output the average delay
	newFile << "[Success Rate]\t" << fixed << setprecision(2) << successRate << " %" << endl ; // output the success rate
	
	newFile.close() ; // close the file

} // outputDoubleFile()

void printTime( clock_t readingDataStart, clock_t readingDataFinish, clock_t sortingDataStart, 
                clock_t sortingDataFinish, clock_t writingDataStart, clock_t writingDataFinish ) { // print the cpu's operating time
                
	cout << "Reading data: " << readingDataFinish - readingDataStart << " clocks (" <<
	        fixed << setprecision(2) << double( readingDataFinish - readingDataStart ) << " ms)." << endl ; // print the time of reading's cost
	cout << "Sorting data: " << sortingDataFinish - sortingDataStart << " clocks (" <<
	        fixed << setprecision(2) << double( sortingDataFinish - sortingDataStart ) << " ms)." << endl ; // print the time of sorting's cost
	cout << "Writing data: " << writingDataFinish - writingDataStart << " clocks (" <<
	    fixed << setprecision(2) << double( writingDataFinish - writingDataStart ) << " ms)." << endl ;     // print the time of writing's cost
	
} // printTime()

// ---------------- Other End -------------------

// ---------------- main() ----------------------

int main() {
	
	int command = 0 ;
	string fileNumber = "NULL" ; // a string of file's number
	clock_t readingDataStart, readingDataFinish ;

	do {
		JobList aJobList ;                 			// a linked list of job list
		EventList aEventList ;             			// a linked list of event list
		DoneOrAbortList aDoneOrAbortList ; 			// a linked list of done or abort list
		DoneOrAbortList aDoneList, aAbortList ; // the linked list of done and abort list
		
		cout << endl ;
		cout << "**** Simulate FIFO Queues by SQF *****" << endl ;
		cout << "* 0. Quit                            *" << endl ;
		cout << "* 1. Sort a file                     *" << endl ;
		cout << "* 2. Simulate one FIFO queue         *" << endl ;
		cout << "* 3. Simulate two queues by SQF      *" << endl ;
		cout << "**************************************" << endl ;
		cout << "Input a command(0, 1, 2, 3): " ;	
		cin >> command ;
		cin.ignore( 255 , '\n' ) ;
		
		if ( command == 0 ) { // command of quit
			break ;
		} // if
		else if ( command == 1 ) { // command of sort the input file and output the sorted file
			
			bool isInputFile = inputFile( aJobList, fileNumber, "input", readingDataStart, readingDataFinish ) ; // input the specify file
			
			if ( isInputFile ) { // when input of the file is success
				cout << endl ;
				aJobList.printJobList() ; // show all items in the linked list ( unsorted )
				
				clock_t sortingDataStart = clock() ;  // start measuring the time spent by sort data from the linked list
				aJobList.shellSortOfJobList() ;       // sorted the input file by Shell sort
				clock_t sortingDataFinish = clock() ; // stop measuring the time spent by sort data from the linked list
				
				clock_t writingDataStart = clock() ; 	   // start measuring the time spent by output data into the file
				outputSortedFile( aJobList, fileNumber ) ; // output the sorted file
				clock_t writingDataFinish = clock() ;      // stop measuring the time spent by output data into the file
				
				cout << endl ;
				printTime( readingDataStart, readingDataFinish, sortingDataStart, sortingDataFinish, writingDataStart, writingDataFinish ) ; // print the cpu's operating time
				cout << endl << "See sorted" << fileNumber << ".txt" << endl ; // print the file's name of sorted file
			} // if			
		} // else if
		else if ( command == 2 ) { // command of simulate a FIFO queue with the data of sorted file
			bool isInputFile = inputFile( aJobList, fileNumber, "sorted", readingDataStart, readingDataFinish ) ; // input the specify file
			
			if ( isInputFile ) { // if read file success
				cout << endl << "The simulation is running..." << endl ;
				aEventList.simulation( aJobList, aDoneOrAbortList ) ;  // simualtion of the cpu's processing
				
				aDoneList.getDoneList( aDoneOrAbortList ) ;
				aAbortList.getAbortList( aDoneOrAbortList ) ;
				
				cout << "See output" << fileNumber << ".txt" << endl ; // print the file's name of sorted file
				outputFile( aDoneList, aAbortList, fileNumber ) ; 					 // output the file
			} // if		
		} // else if
		else if ( command == 3 ) { // command of simulate two queues by SQF with the data of sorted file
			bool isInputFile = inputFile( aJobList, fileNumber, "sorted", readingDataStart, readingDataFinish ) ; // input the specify file
			
			if ( isInputFile ) { // if read file success
				vector< int > doneListRecord ; // record the order of done event
				cout << endl << "The simulation is running..." << endl ;
				aEventList.simulationByDouble( aJobList, aDoneOrAbortList, doneListRecord ) ;  // simualtion of the cpu's processing
				
				aDoneList.getDoneListOfDouble( aDoneOrAbortList, doneListRecord ) ;
				aAbortList.getAbortList( aDoneOrAbortList ) ;
				
				cout << "See double" << fileNumber << ".txt" << endl ; 				 // print the file's name of sorted file
				outputDoubleFile( aDoneList, aAbortList, fileNumber ) ; 		 				 // output the file				
			} // if		
		} // else if
		else // unknown command
			cout << endl << "Command does not exist!" << endl ;
		
		aJobList.~JobList() ; 								// clean the data of JobList
		aDoneOrAbortList.~DoneOrAbortList() ; // clean the data of DoneOrAbortList
		aDoneList.~DoneOrAbortList() ; 				// clean the data of DoneList
		aAbortList.~DoneOrAbortList() ; 			// clean the data of AbortList
	} while ( command != 0 ) ;

	system( "pause" ) ;
	return 0 ;
	
} // main()

// ---------------- main() End ------------------

