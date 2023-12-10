#include <iostream>     // cout, endl
#include <fstream>      // open, is open
#include <string>       // string
#include <vector>       // vector
#include <cstdlib>      // atoi, system
#include <iomanip>      // setw, setprecision
#include <ctime>        // clock, CLOCKS_PER_SEC

using namespace std;

struct jobType {
    int OID;           //order identifier
    int arrival;       // arrival time
    int duration;      // job duration
    int timeout;       // expire time
} ;

struct Event {
    bool in ;
    int dotime ;
    jobType job ;
};

// ************************************** //
// The above are data structures globally available
// ************************************** //

class JobList {
    vector<jobType> alist;// list of jobs with four columes
    double readTime, sortTime, writeTime;
    // string fileID ;             // file identifier

    // ************************************** //
    // The above are private data members
    // ************************************** //
    void reset() {                         // definition: initial set up
        while( !isEmpty() ) {
            alist.clear() ;
        } // while
    } // end reset

    void showJob(jobType &);               // declaration: display a job on screen
    void sortByArrival();                  // declaration: sort all by arrival time
    void showTime();                       // declaration output time on screen
    // ************************************** //
    // The above are private methods
    // ************************************** //

public:

    JobList() { reset(); }          // constructor for initialization

    JobList( JobList const &in ) {
        alist = in.alist ;
    }

    ~JobList() { reset(); } // destructor for initialization

    bool isEmpty() {
        if ( alist.size() == 0 )
            return true;
        return false ;
    }                              // check whether it is empty or not

    // ************************************** //
    // The above are primitive methods publicly available
    // ************************************** //
    void showall() ;               // declaration: show all on a screen
    void putAll( string );         // declaration: write all as a file
    bool getAll( int ,string );           // declaration: read all from a file
    bool getSorted();              // declaration: read all from a file and sort them
    void nextJob(jobType &);       // declaration: get & remove the next job
    void nextJobCheck(jobType &) ;         // declaration: take a look at the next job

}; // end JobList

template<typename T>
class JobQueue {
    struct qNode {
        T item;
        qNode *next = NULL;
    };
    qNode *qFront;
    qNode *qBack;         // head & tail of queue
    int qSize = 0 ;
    int qMax = 3;          // current size & max size of queue


    int gmax() const {
        return qMax + 1 ;
    } // getmax()

    // ************************************** //
    // The above are private methods
    // ************************************** //

public:
    // ************************************** //
    // The above are public data members
    // ************************************** //

    JobQueue() :  qFront(NULL), qBack(NULL) {
        qSize = 0 ;
        qMax = 3 ;
    }  // constructor of no-space queue

    JobQueue<T>(int maxS) : qFront(NULL), qBack(NULL) { // constructor of an empty queue
        qSize = 0 ;
        maxS = qMax ;
    }                      // end constructor

    int length() const {
        return qSize ;
    }                       // get the current queue length

    bool isEmpty() const {
        if ( length() == 0 )
            return true ;
        return false ;
    }                       // check whether it is empty

    bool isFull() const {
        if ( length() == gmax() )
            return true ;
        return false ;
    }                       // check whether it is full

    void enQueue(T &) ;     // append a new element
    void getFront(T &) ;    // get the first element
    void deQueue() ;        // drop the first element
    void deQueue(T &) ;     // get & drop the first element
    void clearQ() ;         // clean up

    ~JobQueue() {           // destructor
        clearQ() ;
    }                       // end destructor

}; //end JobQueue


class AnsList{
    typedef struct aT {
        int OID ;
        int abort ;
        int delay ;
    } abortType;

    struct doneType{
        int OID ;
        int departure ;
        int delay ;
    } ;

    vector<abortType> abortJobs ;   // list of aborted jobs with three columns
    vector<doneType> doneJobs;      // list of done jobs with three columns
    float avgDelay ;                // average delay
    float successRate;              // percentage of done jobs

    // ************************************** //
    // The above are private data members
    // ************************************** //

    void showAbortJobs(string);     // declaration: display abort jobs on screen
    void showDoneJobs(string);      // declaration: display done jobs on screen
    void showStat();                // declaration: display statistics on screen

    // ************************************** //
    // The above are private methods
    // ************************************** //

public:

    AnsList(): avgDelay(0.0), successRate(0.0) {
    }                                       // constructor for initialization

    ~AnsList() {
        abortJobs.clear() ;
        doneJobs.clear() ;
        avgDelay = 0 ;
        successRate = 0 ;
    }                                       // destructor for initialization

    void computeStat();             // declaration: compute the statistics
    void showAll(string);                   // declaration: display all on screen
    void addAbortJob(int, int, int, int);   // declaration: add one aborted job
    void addDoneJob(int, int, int, int);    // declaration: add one done job

    // ************************************** //
    // The above are primitive methods
    // ************************************** //

    void putAll( string );            // declaration write all as a file

}; // end AnsList



class Simulation {
    JobList     Jobs ;                    // list of jobs
    vector<JobQueue<jobType> >   allQ ;    // a set of queues
    int         qNum ;                    // number of queues
    int         avail;                 // the time when CPU is available
// ************************************** //
// The above are private data members
// ************************************** //

    void delQ(int);                      // declaration delete an old job from a queue
    void updateQ(int);                   // declaration update each queue
    void finishQ();                      // declaration: finish the remaining jobs in queues
    void addEvent( vector<Event> &, bool, jobType, int) ;

// ************************************** //
// The above are private methods
// ************************************** //

public:
    AnsList     answers ;  // a set of answers
    Simulation(JobList alist, int N): Jobs(alist), qNum(N), avail(0) { // copy constructor

    }                               // end constructor
    ~Simulation() {                 // destructor
        // *隢銵???蝢?*
    }       // end Destructor

    void SQF();                     // declaration: shortest queue first
}; // end Simulation

int main() {

    int cmd = 1;
    string fileName;
    while ( cmd != 0 ) {
        JobList aJobList; // job list
        // UI
        cout << endl << "**** Simulate FIFO Queues by SQF *****";
        cout << endl << "* 0. Quit                            *";
        cout << endl << "* 1. Sort a file                     *";
        cout << endl << "* 2. Simulate one FIFO queue         *";
        cout << endl << "**************************************";
        cout << endl << "Input a command(0, 1, 2):";
        cin >> cmd;

        // sort the file
        if ( cmd == 1 ) {
            cout << endl << "input file name : " ;
            cin >> fileName;
            if ( aJobList.getAll( cmd,fileName) ) {
                aJobList.showall() ;
                if( aJobList.getSorted() )
                    aJobList.putAll( fileName ) ;

            }
        } // if
        else if( cmd == 2 ){
            // activate the CPU
            if ( fileName.empty() ) {
                cout << endl << "input file name : " ;
                cin >> fileName;
            }
            if ( aJobList.getAll( cmd, fileName) ) {
                Simulation aSim = Simulation(aJobList, 1);
                aSim.SQF();
                aSim.answers.putAll(fileName);
            } // if
        }
        else if ( cmd == 0 ) {
            break ; // end the program
        }
        else {
            cout << endl << "command does not exist" << endl; // wrong comment
        } // else

    } // while

    cout << "pause" ;
    return 0;
} // main()

// job list--------------------------------------------------------------------------------------------

void JobList::showJob(jobType & data) {
    nextJob( data ) ;
    cout << data.OID << "\t" << data.arrival << "\t" << data.duration << "\t" << data.timeout << endl ;
} // JobList::showJob()

void JobList::putAll( string fileName ) {
    clock_t start = clock(); // start reading
    jobType data ;
    int num = 1 ;
    fileName = "sorted" + fileName + ".txt";
    fstream newFile;
    newFile.open(fileName.c_str(), std::ios::out | std::ios::trunc);
    newFile << "OID\tArrival\tDuration\tTimeOut" << endl ;

    while ( !isEmpty() ) {
        nextJob( data );
        newFile << data.OID << "\t" << data.arrival << "\t" << data.duration << "\t" << data.timeout << endl ;
        num ++ ;
    } // while

    newFile.close() ;
    clock_t stop = clock(); // start reading
    writeTime = static_cast<double>(stop - start) / (CLOCKS_PER_SEC / 1000.0);
    showTime() ;
    cout << endl << "See " << fileName << endl ;

} // JobList::putAll( string fileName )

void JobList::showall() {
    cout << "\tOID\tArrival\tDuration\tTimeOut" << endl ;
    int i ;
    i = alist.size() ;
    for (int x = 0; x < i ; x ++ ) {
        cout << "(" << x+1 << ")\t";
        cout << alist[x].OID << "\t" << alist[x].arrival << "\t" << alist[x].duration << "\t" << alist[x].timeout << endl ;
    } // for

} // JobList::showa()

void JobList::showTime() {
    cout << endl << "Reading data: " << fixed << setprecision(0) << readTime << " clocks " ;
    cout << "(" << fixed << setprecision(2) << readTime << " ms)." << endl ;
    cout << "Sorting data: " << fixed <<setprecision(0) << sortTime << " clocks " ;
    cout << "(" << fixed <<setprecision(2) << sortTime << " ms)." << endl ;
    cout << "Writing data: " << fixed <<setprecision(0) << writeTime << " clocks " ;
    cout << "(" << fixed << setprecision(2) << writeTime << " ms)." << endl ;
} // JobList::showTime()

void JobList::nextJobCheck(jobType & data) {          // declaration: take a look at the next job
    data = alist[0] ;
} // JobList::nextJobCheck(jobType & data)

void JobList::nextJob( jobType & data ) {
    nextJobCheck( data ) ;
    if ( !isEmpty() )
        alist.erase( alist.begin() ) ;
} // void JobList::nextJob( jobType & data )


void JobList:: sortByArrival() {
    clock_t start = clock(); // start reading
    for (int h = alist.size() / 2; h > 0; h /= 2) {
        for (int unsorted = h; unsorted < alist.size(); unsorted++) {
            int loc = unsorted;
            jobType temp;
            temp = alist[unsorted];

            int nextArrival = alist[unsorted].arrival;
            int nextID = alist[unsorted].OID;

            while (loc >= h && (alist[loc - h].arrival > nextArrival || (alist[loc - h].arrival == nextArrival && alist[loc - h].OID > nextID))) {
                // Swap elements
                alist[loc] = alist[loc - h];
                loc = loc - h;
            }

            alist[loc] = temp;
            // You need to copy other attributes as well if necessary
        }
    }
    clock_t stop = clock(); // stop reading
    sortTime = static_cast<double>(stop - start) / (CLOCKS_PER_SEC / 1000.0);
}

bool JobList:: getAll( int command, string fileName ){
    if ( command == 1 )
        fileName = "input" + fileName + ".txt";
    else
        fileName = "sorted" + fileName + ".txt";
    std::ifstream file;
    file.open( fileName.c_str() , std::ios::in); // 打開資料夾
    if (!file.is_open()) {
        cout << endl << "###" << fileName << " does not exist! ###";
        return false;
    }

    clock_t start = clock(); // start reading
    string line;
    getline(file,line); // read the first line
    jobType newJob;
    while ( file >> newJob.OID ) {
        file >> newJob.arrival >> newJob.duration >> newJob.timeout;
        alist.push_back(newJob);
    }
    file.close();
    clock_t stop = clock(); // stop reading
    readTime = static_cast<double>(stop - start) / (CLOCKS_PER_SEC / 1000.0);
    return true;

}


bool JobList :: getSorted() {
    sortByArrival();
    return true;
}

// Queue list------------------------------------------------------------------------------------------


template<typename T>
void JobQueue<T>::enQueue(T& job) {
    qNode* newNode = new qNode;
    newNode->item = job;
    newNode->next = NULL;

    if (isEmpty()) {
        qFront = newNode;
        qBack = newNode;
    }
    else {
        qBack->next = newNode;
        qBack = newNode;
    }

    qBack->next = qFront;
    qSize = qSize+1;
} // end of enQueue

template<typename T>
void JobQueue<T> :: getFront (T & job){
    job = qFront->item;
}    // get the first element

template<typename T>
void JobQueue<T> :: deQueue() {
    if (isEmpty()) {
        cout << endl << "Queue is empty. Cannot dequeue.";
        return;
    }

    qNode *temp = qFront;

    if ( qFront == qBack ) {
        qFront = NULL;
        qBack = NULL;
    }
    else {
        qFront = qFront->next;
        qBack->next = qFront;
    }

    delete temp;
    qSize = qSize-1;
}    // get the first element

template<typename T>
void JobQueue<T> :: deQueue(T & job) {
    getFront(job ) ;
    deQueue() ;
}

template<typename T>
void JobQueue<T> ::clearQ(){
    while ( !isEmpty() ) {
        deQueue();
    }
}

// ans list--------------------------------------------------------------------------------------------
void AnsList::computeStat() {
    int alldelay = 0 ;
    int cancel = abortJobs.size() ;
    int done = doneJobs.size() ;
    int total = done + cancel ;

    for ( int i = 0 ; i < done ; i ++ ) {
        alldelay = doneJobs[i].delay + alldelay ;
    } // for

    for ( int i = 0 ; i < cancel ; i ++ ) {
        alldelay = abortJobs[i].delay + alldelay ;
    } // for

    avgDelay = static_cast<double>(alldelay) / total ;
    successRate = static_cast<double>(done) / total*100 ;
} // AnsList::computeStat()


void AnsList::addAbortJob( int OID, int arrival, int timeout, int nowtime ) {
    int delay = 0 ;
    abortType cancel ;
    cancel.OID = OID ;
    if ( timeout == nowtime ) {
        cancel.abort = timeout ;
        cancel.delay = timeout - arrival ;
    } // if

    else if ( timeout < nowtime ) {
        cancel.abort = nowtime;
        cancel.delay = nowtime - arrival;
    }

    else {
        cancel.abort = arrival;
        cancel.delay = 0;
    } // else

    abortJobs.push_back( cancel ) ;
} // AnsList::addAbortJob( int OID, int arrival, int timeout, int nowtime ) {

void AnsList::addDoneJob(int OID, int arrival, int duration, int nowtime ) {
    doneType done ;
    done.OID = OID ;
    done.departure = nowtime ;
    done.delay = nowtime - ( duration + arrival ) ;
    doneJobs.push_back( done ) ;
} // AnsList::addDoneJob(int OID, int arrival, int duration, int nowtime )

void AnsList::putAll( string fileName ) {
    int cancel = abortJobs.size() ;
    int done = doneJobs.size() ;
    fileName = "output" + fileName + ".txt";
    cout << endl << "The simulation is running..." << endl ;
    cout << "See " << fileName << endl ;
    fstream newFile;
    newFile.open(fileName.c_str(), ios::out | ios::trunc);
    newFile << "\t[Abort Jobs]" << endl << "\tOID\tAbort\tDelay" << endl ;

    for ( int i = 0 ; i < cancel ; i ++ ) {
        newFile << "[" << i+1 << "]\t" ;
        newFile << abortJobs[i].OID << "\t" << abortJobs[i].abort << "\t" << abortJobs[i].delay << endl ;
    } // while

    newFile << "\t[Jobs Done]" << endl << "\tOID\tDeparture\tDelay" << endl ;

    for ( int i = 0 ; i < done ; i ++ ) {
        newFile << "[" << i+1 << "]\t" ;
        newFile << doneJobs[i].OID << "\t" << doneJobs[i].departure << "\t" << doneJobs[i].delay << endl ;
    } // while

    newFile << "[Average Delay]" << "\t" << fixed << setprecision(2) << avgDelay << " ms" << endl ;
    newFile << "[Success Rate]" << "\t" << fixed << setprecision(2) << successRate << " %" << endl ;

    newFile.close() ;

} // AnsList::putAll( string )

// Simulation list----------------------------------------------------------------------------------------

void Simulation::delQ(int qnum) {
    jobType temp ;
    allQ[qnum].deQueue( temp ) ;

    if ( temp.timeout >= temp.duration + avail ) {
        avail += temp.duration ;
        answers.addDoneJob( temp.OID, temp.arrival, temp.duration, avail ) ;
    } // if

    else {
        if ( avail < temp.timeout ) {
            avail = temp.timeout ;
            return ;
        } // if

        answers.addAbortJob( temp.OID, temp.arrival, temp.timeout, avail ) ;
    } // else


} // Simulation::delQ(int)

void Simulation::updateQ( int qnum ) {
    jobType temp ;
    Jobs.nextJob( temp ) ;

    if ( !allQ[qnum].isFull() ) {
        allQ[qnum].enQueue(temp);
    }
    else {
        answers.addAbortJob(temp.OID, temp.arrival, temp.timeout, avail);
    }
} // void Simulation::updateQ( int qnum, jobType job )

void Simulation::finishQ() {
    for ( int i = 0 ; i < qNum - 1 ; i ++ ) {
        while ( !allQ[i].isEmpty() ) {
            delQ(i);
        } // while
    } // for

} // void Simulation::updateQ( int qnum, jobType job )

void Simulation::addEvent( vector<Event> &eventList, bool in, jobType job, int nowtime ) {
    int num ;
    Event temp ;
    temp.job = job ;
    temp.in = in ;
    if ( in )
        temp.dotime = job.arrival ;
    else {
        if ( nowtime + job.duration <= job.timeout )
            temp.dotime = nowtime + job.duration ;
        else {
            if ( job.timeout > nowtime )
                answers.addAbortJob( job.OID, job.arrival, job.timeout, job.timeout ) ;
            temp.dotime = job.timeout;
        } // else
    } // else

    num = eventList.size() ;

    for ( int i = 0 ; i < num ; i ++ ) {
        if ( eventList[i].dotime > temp.dotime ) {
            eventList.insert( eventList.begin() + i, temp ) ;
            return ;
        } // if
    } // for
    eventList.push_back( temp ) ;
} // Simulation::sortdotime( vector<Event> eventList )

void Simulation :: SQF() {
    jobType temp ;
    vector<Event> eventList ;
    for ( int i = 0; i < qNum; i++ ) {
        JobQueue<jobType> newQueue ;
        allQ.push_back(newQueue) ;
    }
    Jobs.nextJobCheck( temp ) ;
    addEvent( eventList, true, temp, avail );
    while ( !Jobs.isEmpty() || !eventList.empty() ) {

        int numQ = 0;
        for ( int i = 1; i < qNum; i ++ ) { // which one is shortest
            if ( allQ[numQ].length() > allQ[i].length() )
                numQ = i;
        } // for

        if ( eventList[0].in ) { // remember to
            if ( allQ[numQ].isEmpty() && !Jobs.isEmpty() ) {
                avail = temp.arrival ;
                addEvent( eventList, false, temp, avail ) ;
            } // if
            updateQ( numQ );
            eventList.erase(eventList.begin());
            if ( !Jobs.isEmpty() ) {
                Jobs.nextJobCheck( temp ) ;
                addEvent( eventList, true, temp, avail );
            }

        } // if
        else if ( !eventList[0].in ) {
            delQ(numQ);
            eventList.erase(eventList.begin());
            jobType t;
            if ( !allQ[numQ].isEmpty() ) {
                allQ[numQ].getFront(t);
                addEvent( eventList, false, t, avail );
            }
        } // else if

    }
    finishQ();
    answers.computeStat() ;

}
