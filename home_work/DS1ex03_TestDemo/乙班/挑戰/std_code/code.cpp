//*********************************************************/
// Exercise 03: Simulate FIFO Queues by Wu, YH@CYCU, 2023/11
//*********************************************************/
//#define DEBUG
//#define REPLY
#define QUIZ

//#include "DS1Simulation.hpp"                                // class JobQueue
//*********************************************************/
// Header file for Simulation by YH in 2017/11-2023/11
//*********************************************************/
#ifndef _Simulation_HPP
#define _Simulation_HPP

//#include "DS1JobList.hpp"                                   // type jobType, class JobList
//*********************************************************/
// Header file for Job List by YH in 2017/11-2023/11
//*********************************************************/
#ifndef _JobList_HPP
#define _JobList_HPP

#include <iostream>                                         // cout, endl
#include <fstream>                                          // open, is_open
#include <string>                                           // string
#include <vector>                                           // vector
#include <cstdlib>                                          // atoi, system
#include <iomanip>                                          // setw, setprecision
#include <ctime>                                            // clock, CLOCKS_PER_SEC

using namespace std;                                        // standard naming space

typedef struct jT
{   int jobID;                                              // job identifier
    int arrival;                                            // arrival time
    int duration;                                           // job duration
    int timeOut;                                            // expire time
}	jobType;
//*********************************************************/
// The above are data structures globally available
//*********************************************************/

class JobList {
    #define firstLine   "OID\tArrival\tDuration\tTimeOut"
    vector<jobType> aList;                                  // list of jobs with four columns
    clock_t         timers[3];                              // reading, sorting, & writing time
    string          fileID;                                 // file identifier
    //*********************************************************/
    // The above are private data members
    //*********************************************************/

    void reset()                                            // definition: initial set up
    {   this->aList.clear();                                // clean up the job list
        for (int i = 0; i < 3; i++)
        this->timers[i] = 0;                                // zero the time clocks
        this->fileID = "";                                  // clean up the file identifier
    }   //end reset
    void    showJob(jobType &aJ) const                      // definition: display a job on screen
    {   cout << "\t" << aJ.jobID << "\t" << aJ.arrival << "\t" << aJ.duration << "\t" << aJ.timeOut << endl;
    }   //end showJob
    void    sortByArrival();                                // declaration: sort all by arrival time
    void    putAll();                                       // declaration: write all as a file
    void    showTime();                                     // declaration: output time on screen
    void    nextJobCheck(jobType &oneJ)                     // definition: take a look at the next job
    {   oneJ = this->aList.front(); }                       // get the next job without removal
    //*********************************************************/
    // The above are private methods
    //*********************************************************/

public:
    JobList()   {   reset();    }                           // constructor for initialization
    ~JobList()   {   reset();    }                          // destructor for initialization
    bool    isEmpty()  {   return (aList.size() == 0);    } // check whether it is empty or not
    string  &getFileID()  {   return (fileID);   }          // get the file identifier
    //*********************************************************/
    // The above are primitive methods publicly available
    //*********************************************************/

    bool    getAll(string);                                 // declaration: read all from a file
    void    showAll();                                      // declaration: output all on screen
    bool    getSorted();                                    // declaration: read all from a file and sort them
    void    nextJob(jobType &);                             // declaration: get & remove the next job
};  //end JobList
//*********************************************************/
// class JobList declaration is as the above
//*********************************************************/

void JobList::sortByArrival()                               // definition: sort all by arrival time
{   int             idx = 1, cur;                           // the index of an element, starting at 0, 1, 2, ...
    clock_t         aTimer = 0;                             // number of clocks

    //*********************************************************/
    // Shell short is implemented as below
    //*********************************************************/
    aTimer = clock();                                       // start timer
    for (int h = this->aList.size()/2; h > 0; h = h/2)      // the gap starts at a half of the vector size
    {   idx = h;
        for (vector<jobType>::iterator unsorted = this->aList.begin() + h;
            unsorted != this->aList.end(); ++unsorted, idx++)
        {   vector<jobType>::iterator   sorted;             // locate the sorted area
            jobType                     nextItem;           // the first element in the unsorted area

            sorted = unsorted;                              // the current element to be sorted
            cur = idx;                                      // index of the current element to be sorted
            nextItem = *unsorted;
            while ((cur >= h) && ( ((sorted - h)->arrival > nextItem.arrival) ||
                (((sorted - h)->arrival == nextItem.arrival) && ((sorted - h)->jobID > nextItem.jobID)) ))
            {   cur -= h;                                   // sorted by the arrival time in ascending order
                sorted -= h;                                // sorted by the job identifier when it ties
            }   //end while
            this->aList.erase(unsorted);                    // remove the current element from the list
            this->aList.insert(sorted, nextItem);           // insert the current element into the right position
        }   // end inner-for
#ifdef DEBUG
        cout << endl << "h = " << h;
        this->showAll();
#endif
    }   // end outer-for
    aTimer = clock() - aTimer;                              // calculate the elapse time
    this->timers[1] = aTimer;                               // get the sorting time
}   // end sortByArrival

void JobList::putAll()                                      // definition: write all as a file
{   fstream     outFile;                                    // output file handle
    string      fname = "sorted" + this->fileID + ".txt";     // output file name "sorted401.txt"
    int         j = 0;

    outFile.open(fname.c_str(), fstream::out);              // create a new file to write
    if (!outFile.is_open())                                 // unable to create a file
        cout << endl << "### Cannot create " <<  fname << " ! ###" << endl;
    else
    {   clock_t aTimer = 0;                                 // number of clocks

        aTimer = clock();                                   // start timer
        outFile << firstLine << endl;
        for (vector<jobType>::iterator it = this->aList.begin(); it < this->aList.end(); ++it)
        {   outFile << it->jobID << "\t" << it->arrival;
            outFile << "\t" << it->duration << "\t" << it->timeOut << endl;
        }   // end for
        outFile.close();                                    // close output file
        aTimer = clock() - aTimer;                          // calculate the elapse time
        this->timers[2] = aTimer;                           // get the writing time
    }   //end else
}   // end putAll

void JobList::showTime()                                    // definition: output time on screen
{   cout << endl << "Reading data: " << this->timers[0] << " clocks (";
    cout << fixed << setprecision(2) << ((float)this->timers[0]) * 1000 / CLOCKS_PER_SEC << " ms).";
    cout << endl << "Sorting data: " << this->timers[1] << " clocks (";
    cout << ((float)this->timers[1]) * 1000 / CLOCKS_PER_SEC << " ms).";
    cout << endl << "Writing data: " << this->timers[2] << " clocks (";
    cout << ((float)this->timers[2]) * 1000 / CLOCKS_PER_SEC << " ms)." << endl;
}   // end showTime
//*********************************************************/
// The above are definitions of private methods
//*********************************************************/

bool JobList::getAll(string prefix)                         // definition: read all from a file
{   fstream     inFile;                                     // file handle
    string      fileName = this->fileID;                    // file name

    this->reset();                                          // call: initial set up
    if (prefix.compare("input") && fileName.compare(""))    // continue mission one
        this->fileID = fileName;
    else
    {   cout << endl << "Input a file number: ";
        cin >> this->fileID;                                // get the file identifier
    }   //end else
    fileName = prefix + this->fileID + ".txt";
    inFile.open(fileName.c_str(), fstream::in);             // open file to read
    if (!inFile.is_open())                                  // unable to open file
    {   cout << endl << "### " << fileName << " does not exist! ###" << endl;
        return false;
    }   //end if
    else
    {   char    cstr[255];                                  // input buffer of one line as c-string
        clock_t aTimer = 0;                                 // number of clocks
        int     fNo, pre, pos;

        aTimer = clock();                                   // start timer
        inFile.getline(cstr, 255, '\n');                    // skip the first line
        while (inFile.getline(cstr, 255, '\n'))             // get all records line by line
        {   jobType oneJ;                                   // one job per line
            string  buf, cut;                               // transform c-string into C++ string

            fNo = 0;                                        // number of fields read so far
            pre = 0;                                        // locate one field of input record
            buf.assign(cstr);                               // copy c-string into C++ string
            do                                              // scan the buffer from left to right
            {   pos = buf.find_first_of('\t', pre);         // locate the next field by '\t' (tab)
                cut = buf.substr(pre, pos - pre);           // retrieve a field
                switch (++fNo)
               	{   case 1: oneJ.jobID = atoi(cut.c_str()); // get the job identifier
                            break;
                    case 2: oneJ.arrival = atoi(cut.c_str());   // get the arrival time
                            break;
                    case 3: oneJ.duration = atoi(cut.c_str());  // get the job duration
                            break;
                    case 4: oneJ.timeOut = atoi(cut.c_str());   // get the expire time
                            break;
                    default: break;
               	}   //end switch
                pre = ++pos;                                // move to the next field
            } while (pos > 0);                              // continue if it is not the end of line
            this->aList.push_back(oneJ);                    // save one job into the job list
        }   //end outer-while
        inFile.close();                                     // close file
        aTimer = clock() - aTimer;                          // calculate the elapse time
        this->timers[0] = aTimer;                           // get the reading time
    }   // end else
    if (!this->aList.size())
    {   cout << endl << "### Get nothing from the file " << fileName << " ! ### " << endl;
        return false;
    }   // end if
    return true;
}   // end getAll

void JobList::showAll()                                     // definition: output all on screen
{   int j = 0;

#ifdef DEBUG
    cout << endl << "*** There are " << this->aList.size() << " jobs in total." << endl;
#endif
    cout << endl << "\t" << firstLine << endl;
    for (vector<jobType>::iterator it = this->aList.begin(); it < this->aList.end(); ++it)
    {   cout << "(" << ++j << ") ";
        showJob(*it);
    }   // end for
    //system("pause");
}   // end showAll

bool JobList::getSorted()                                   // definition: read all from a file and sort them
{   if (!getAll("input"))                                   // call: read all from a file
        return false;
    showAll();                                              // call: output all on screen
    sortByArrival();                                        // call: sort all by arrival time
    putAll();                                               // call: write all as a file
    showTime();                                             // call: output time on screen
    return true;
}   //end getSorted

void JobList::nextJob(jobType &oneJ)                        // definition: get & remove the next job
{   this->nextJobCheck(oneJ);                               // get the next job
    this->aList.erase(this->aList.begin());
#ifdef DEBUG
        cout << "The next job: ";
        showJob(oneJ);
        this->showAll();
#endif
}   //end nextJob
//*********************************************************/
// method definitions of class JobList are as the above
//*********************************************************/
#endif  //_JobList_HPP


//#include "DS1JobQueue.hpp"                                  // class JobQueue
//*********************************************************/
// Header file for Job Queue by YH in 2017/11-2023/11
//*********************************************************/
#ifndef _JobQueue_HPP
#define _JobQueue_HPP

template <typename T>
class JobQueue {
    T   *cA;                                                // circular array
    int qFront, qBack;                                      // head & tail of queue
    int qSize, qMax;                                        // current size & max size of queue
    //*********************************************************/
    // The above are private data members
    //*********************************************************/

    int next(int idx)   {   return ((idx + 1) % qMax);  }   // move to the next position in queue
    int prev(int idx)
    {   return ((idx + qMax - 1) % qMax);  }                // move to the previous position in queue
    //*********************************************************/
    // The above are private methods
    //*********************************************************/

public:
        int avail;                                          // the time when cook is available
    //*********************************************************/
    // The above are public data members
    //*********************************************************/
    JobQueue():cA(NULL), qFront(0), qBack(0),
                qSize(0), qMax(0), avail(0)  {}             // constructor of no-space queue
    JobQueue<T>(int maxS):qFront(0), qSize(0), avail(0)     // constructor of an empty queue
    {   cA = new T [maxS];                                  // create the array
        qMax = maxS;                                        // limit of array size
        qBack = maxS - 1;                                   // keep track of the last element
    }  //end constructor

    int length()   const   {   return qSize; }              // get the current queue length
    bool isEmpty()  const    {  return (qSize == 0);    }   // check whether it is empty
    bool isFull()   const   {   return (qSize == qMax); }   // check whether it is full
    void enQueue(T &newItem)                                // append a new element
    {   qSize++;                                            // increase the number of elements
        qBack = next(qBack);                                // locate this last element
        cA[qBack] = newItem;                                // call by reference
     //   cout << endl << qFront << ", " << qBack << ", " << qMax << ", " << qSize << endl;
    }   //end enQueue

    void getFront(T &oldItem)  const   {  oldItem = cA[qFront]; }   // get the first element
    void deQueue()                                          // drop the first element
    {   qSize--;                                            // decrease the number of elements
        qFront = next(qFront);                              // locate the first element
    }   //end deQueue
    void deQueue(T &oldItem)                                // get & drop the first element
    {   getFront(oldItem);                                  // get the first element
        deQueue();                                          // drop the first element
    }   //end deQueue

    void clearQ()                                           // clean up
    {   while (!isEmpty())
            deQueue();
        delete [] cA;
        cA = NULL;
    }   //end clearQ
    ~JobQueue()                                             // destructor
    {   clearQ();
    }   //end destructor
};  //end JobQueue
//*********************************************************/
// class JobQueue declaration is as the above
//*********************************************************/
#endif  //_JobQueue_HPP


//#include "DS1AnsList.hpp"                                   // type abortType, type doneType, class AnsList
//*********************************************************/
// Header file for Answer List by YH in 2017/11-2023/11
//*********************************************************/
#ifndef _AnsList_HPP
#define _AnsList_HPP

#include <iostream>	                                        // cout, endl
#include <fstream>                                          // open, is_open
#include <string>                                           // string
#include <vector>                                           // vector
#include <cstdlib>                                          // atoi, system
#include <iomanip>                                          // setw, setprecision

class AnsList {
    #define     abortLine   "\tOID\tCID\tAbort\tDelay"
    #define     abortLineS  "\tOID\tAbort\tDelay"
    #define     doneLine    "\tOID\tCID\tDeparture\tDelay"
    #define     doneLineS   "\tOID\tDeparture\tDelay"

    typedef struct aT
    {   int jobID;                                          // abort job identifier
        int qID;                                            // cook to assign
        int delay;                                          // job waiting time
        int abort;                                          // time to abort
    }	abortType;
    typedef struct dT
    {   int jobID;                                          // done job identifier
        int qID;                                            // cook to assign
        int delay;                                          // job waiting time
        int depart;                                         // departure time
    }	doneType;
    vector<doneType>    doneJobs;                           // list of finished jobs with three columns
    vector<abortType>   abortJobs;                          // list of aborted jobs with three columns
    float               avgDelay;                           // total delay of failed jobs
    float               successRate;                        // percentage of failed jobs
    //*********************************************************/
    // The above are private data members
    //*********************************************************/

    void showAbortJobs(string);                             // declaration: display abort jobs on screen
    void showDoneJobs(string);                              // declaration: display done jobs on screen
    void computeStat();                                     // declaration: compute the statistics
    void showStat();                                        // declaration: display statistics on screen
    //*********************************************************/
    // The above are private methods
    //*********************************************************/

public:
    AnsList(): avgDelay(0.0), successRate(0.0)
    {   abortJobs.clear();  doneJobs.clear();    }          // constructor for initialization
    ~AnsList()
    {   abortJobs.clear();  doneJobs.clear();    }          // destructor for initialization
    void showAll(string type)                               // definition: display all on screen
    {   showAbortJobs(type);
        showDoneJobs(type);
        showStat();
    }   //end showAll
    void addAbortJob(int jID, int qID, int aTime, int dTime) // definition: add one aborted job
    {   abortType   aJob;

        aJob.jobID = jID;
        aJob.qID = qID;
        aJob.abort = aTime;
        aJob.delay = dTime;
        abortJobs.push_back(aJob);
    }   //end addAbortJob
    void addDoneJob(int jID, int qID, int fTime, int dTime) // definition: add one done job
    {   doneType    dJob;

        dJob.jobID = jID;
        dJob.qID = qID;
        dJob.depart = fTime;
        dJob.delay = dTime;
        doneJobs.push_back(dJob);
    }   //end addDoneJob
    //*********************************************************/
    // The above are primitive methods
    //*********************************************************/

    void putAll(string, string);                            // declaration: write all as a file
};  //end AnsList
//*********************************************************/
// class AnsList declaration is as the above
//*********************************************************/

void AnsList::showAbortJobs(string type)                    // definition: display abort jobs on screen
{   int j = 0;

    if (type.compare("output"))                                // more than one queues
    {   cout << endl << "\t[Abort List]" << endl << abortLine << endl;
        for (vector<abortType>::iterator it = this->abortJobs.begin(); it < this->abortJobs.end(); ++it)
            cout << "[" << ++j << "]" << "\t" << it->jobID << "\t" << it->qID <<
                     "\t" << it->abort << "\t" << it->delay << endl;
    }   //end if
    else                                                    // only one queue: NO queue ID
    {   cout << endl << "\t[Abort List]" << endl << abortLineS << endl;
        for (vector<abortType>::iterator it = this->abortJobs.begin(); it < this->abortJobs.end(); ++it)
            cout << "[" << ++j << "]" << "\t" << it->jobID << "\t" << it->abort << "\t" << it->delay << endl;
    }   //end else
}   // end showAbortJobs

void AnsList::showDoneJobs(string type)                     // definition: display done jobs on screen
{   int j = 0;

    if (type.compare("output"))                             // more than one queues
    {   cout << endl << "\t[Done List]" << endl << doneLine << endl;
        for (vector<doneType>::iterator it = this->doneJobs.begin(); it < this->doneJobs.end(); ++it)
            cout << "[" << ++j << "]" << "\t" << it->jobID << "\t" << it->qID <<
                     "\t" << it->depart << "\t" << it->delay << endl;
    }   //end if
    else                                                    // only one queue: NO queue ID
    {   cout << endl << "\t[Done List]" << endl << doneLineS << endl;
        for (vector<doneType>::iterator it = this->doneJobs.begin(); it < this->doneJobs.end(); ++it)
            cout << "[" << ++j << "]" << "\t" << it->jobID << "\t" << it->depart << "\t" << it->delay << endl;
    }   //end else
}   // end showDoneJobs

void AnsList::computeStat()                                 // definition: compute the statistics
{   int success = this->doneJobs.size();                    // number of done jobs
    int total = success + this->abortJobs.size();           // total number of jobs

    if (total > 0)
    {   float delaySum = 0.0;                               // sum of all delays

        for (vector<abortType>::iterator it = this->abortJobs.begin(); it < this->abortJobs.end(); ++it)
            delaySum += it->delay;
        for (vector<doneType>::iterator it = this->doneJobs.begin(); it < this->doneJobs.end(); ++it)
            delaySum += it->delay;
        this->avgDelay = delaySum / total;
        this->successRate = (float)success * 100 / total;
    }   //end if
}   // end computeStat

void AnsList::showStat()                                    // definition: display statistics on screen
{
    computeStat();                                          // call: compute the statistics
    cout << endl << "[[Average Delay]" << "\t" << fixed << setprecision(2) << this->avgDelay << " ms" << endl;
    cout << endl << "[Success Rate]" << "\t" << fixed << setprecision(2) << this->successRate << " %" << endl;
    //system("pause");
}   // end showStat
//*********************************************************/
// The above are definitions of private methods
//*********************************************************/

void AnsList::putAll(string type, string fname)             // definition: write all as a file
{   fstream outFile;                                        // output file handle

    fname = type + fname;
    outFile.open(fname.c_str(), fstream::out);              // create a new file to write
    if (!outFile.is_open())                                 // unable to create a file
        cout << endl << "### Cannot create " <<  fname << " ! ###" << endl;
    else
    {   int j = 0;

        if (type.compare("output"))                            // more than one queues
        {   outFile << "\t[Abort Jobs]" << endl << abortLine << endl;
            for (vector<abortType>::iterator it = this->abortJobs.begin(); it < this->abortJobs.end(); ++it)
                outFile << "[" << ++j << "]" << "\t" << it->jobID << "\t" << it->qID <<
                             "\t" << it->abort << "\t" << it->delay << endl;
            j = 0;
            outFile << "\t[Jobs Done]" << endl << doneLine << endl;
            for (vector<doneType>::iterator it = this->doneJobs.begin(); it < this->doneJobs.end(); ++it)
                outFile << "[" << ++j << "]" << "\t" << it->jobID << "\t" << it->qID <<
                             "\t" << it->depart << "\t" << it->delay << endl;
        }   //end if
        else                                                // only one queue: NO queue ID
        {   outFile << "\t[Abort Jobs]" << endl << abortLineS << endl;
            for (vector<abortType>::iterator it = this->abortJobs.begin(); it < this->abortJobs.end(); ++it)
                outFile << "[" << ++j << "]" << "\t" << it->jobID << "\t" << it->abort << "\t" << it->delay << endl;
            j = 0;
            outFile << "\t[Jobs Done]" << endl << doneLineS << endl;
            for (vector<doneType>::iterator it = this->doneJobs.begin(); it < this->doneJobs.end(); ++it)
                outFile << "[" << ++j << "]" << "\t" << it->jobID <<
                            "\t" << it->depart << "\t" << it->delay << endl;
        }   //end else
        computeStat();                                      // call: compute the statistics
        outFile << "[Average Delay]" << "\t" << fixed << setprecision(2) << this->avgDelay << " ms" << endl <<
                "[Success Rate]" << "\t" << fixed << setprecision(2) << this->successRate << " %" << endl;
    }   //end else
}   // end putAll
//*********************************************************/
// method definitions of class AnsList are as the above
//*********************************************************/
#endif  //_AnsList_HPP

class Simulation {
    #define Q_MAX   3                                       // upper bound of queue size
    JobList             jobs;                               // a list of jobs
    AnsList             answers;                            // a set of answers
    JobQueue<jobType>   **allQ;                             // a set of queues
    int                 qNum;                               // number of queues

    //*********************************************************/
    // The above are private data members
    //*********************************************************/

    void delQ(int);                                         // declaration: delete an old job from a queue
    void updateQ(int, int);                                 // declaration: update each queue
    void finishQ();                                         // declaration: finish the remaining jobs in queues
#ifdef DEBUG
    void showQ(JobQueue<jobType> *&, int);                  // declaration: display the queue content in sequence
    void showJob(jobType &aJ)                               // definition: display a job on screen
    {   cout << aJ.jobID << "\t" << aJ.arrival << "\t" << aJ.duration << "\t" << aJ.timeOut << endl;
    }   //end showJob
#endif // DEBUG
    //*********************************************************/
    // The above are private methods
    //*********************************************************/

public:
    Simulation(JobList aList, int N): jobs(aList), qNum(N)  // copy constructor
    {   if (qNum >= 1)
        {   allQ = new JobQueue<jobType> * [qNum];          // create a set of queues
            for (int i = 0; i < qNum; ++i)                  // allocate space for each queue
                allQ[i] = new JobQueue<jobType>(Q_MAX);
        }   // end if
    }   //end constructor
    ~Simulation()                                           // destructor
    {   for (int i = 0; i < qNum; ++i)
        {   allQ[i]->clearQ();                              // release the space of each queue
            delete allQ[i];
        }   //end for
        delete [] allQ;                                     // release the space of queue set
    }   //end Destructor
    void SQF();                                             // declaration: shortest queue first
};  //end Simulation
//*********************************************************/
// class Simulation declaration is as the above
//*********************************************************/

void Simulation::delQ(int Qid)                              // definition: delete an old job from a queue
{   jobType oldJob;                                         // old job in a queue

    allQ[Qid]->deQueue(oldJob);                             // call: get & drop a job from queue
    if (allQ[Qid]->avail < oldJob.arrival)                  // queued job arrives later than the departed job
        allQ[Qid]->avail = oldJob.arrival;                  // move forward the available time
    if (oldJob.timeOut <= allQ[Qid]->avail)                 // already timeout when CPU is available, so abort it
        answers.addAbortJob(oldJob.jobID, Qid+1, allQ[Qid]->avail,
                        allQ[Qid]->avail - oldJob.arrival); // call: add one aborted job 4.2
    else                                                    // execute the job in queue
    {   int availPrev = allQ[Qid]->avail;                   // available time before the execution

        allQ[Qid]->avail += oldJob.duration;                // move to the next available time
        if (oldJob.timeOut < allQ[Qid]->avail)              // timeout in the middle of execution
        {   allQ[Qid]->avail = oldJob.timeOut;              // adjusted earlier to timeout
            answers.addAbortJob(oldJob.jobID, Qid+1, allQ[Qid]->avail,
                            allQ[Qid]->avail - oldJob.arrival); // call: add one aborted job 4.3
        }   //end if
        else
            answers.addDoneJob(oldJob.jobID, Qid+1, allQ[Qid]->avail,
                            availPrev - oldJob.arrival);    // call: add one done job 3.1
    }   //end else
}   //end delQ

void Simulation::updateQ(int idx, int newT)                 // definition: update each queue
{   while (allQ[idx]->avail <= newT)                        // CPU is available when new job arrives
        if (allQ[idx]->isEmpty())                           // Queue is empty: no more update
            break;
        else                                                // Queue is not empty: try to execute an old job
            delQ(idx);                                      // call: delete an old job from a queue
}   //end updateQ

void Simulation::finishQ()                                  // definition: finish the remaining jobs in queues
{   while (true)
    {   int Qid, minT;                                      // available queue, earliest available time

        Qid = -1;                                           // assume: all queues are empty
        minT = -1;                                          // set the upper bound of queue length
        for (int i = 0; i < qNum; ++i)
            if (!allQ[i]->isEmpty())                        // one unfinished queue
            {   if ((minT == -1) ||
                    (allQ[i]->avail < minT))                // first choose the queue with earliest available time
                {   Qid = i;
                    minT = allQ[i]->avail;
                }   // end inner-if
            }   // end outer-if
        if (Qid > -1)                                       // one job from a queue is executed
            delQ(Qid);                                      // call: delete an old job from a queue
        else
            break;
#ifdef DEBUG
        for (int i = 0; i < qNum; ++i)
            showQ(allQ[i], i);
        if (qNum > 1)
            answers.showAll("two");
        else
            answers.showAll("one");
#endif
    } // end while
}   //end finishQ

#ifdef DEBUG
void Simulation::showQ(JobQueue<jobType> *&aQ, int idx)     // definition: display the queue content in sequence
{   JobQueue<jobType>   bQ(Q_MAX);
    jobType             oneJ;

    cout << endl << "Queue " << idx+1 << " (" << allQ[idx]->avail << "): " << endl;
    while (!aQ->isEmpty())
    {   aQ->deQueue(oneJ);
        showJob(oneJ);                                      // call: display a job on screen
        bQ.enQueue(oneJ);
    }   //end while
    while (!bQ.isEmpty())
    {   bQ.deQueue(oneJ);
        aQ->enQueue(oneJ);
    }   //end while
}   //end showQ
#endif
//*********************************************************/
// private method definitions of class Simulation are as the above
//*********************************************************/

void Simulation::SQF()                                                      // definition: shortest queue first
{   jobType newJob;                                                         // the new job
    int     Qid, minQ;                                                      // available queue, shortest queue length

    while (!jobs.isEmpty())
    {
#ifdef DEBUG
        for (int i = 0; i < qNum; ++i)
            showQ(allQ[i], i);
        if (qNum > 1)
            answers.showAll("double");
        else
            answers.showAll("output");
#endif
           jobs.nextJob(newJob);                                            // get & remove the next job
        if ((newJob.duration == 0) ||
            (newJob.arrival + newJob.duration > newJob.timeOut))
            continue;                                                       // skip the job with a wrong setup
        for (int i = 0; i < qNum; ++i)                                      // update queue content for each CPU
            updateQ(i, newJob.arrival);                                     // call: update each queue
        Qid = -1;                                                           // assume: no CPU is available
        for (int i = 0; i < qNum; ++i)
            if (allQ[i]->avail <= newJob.arrival)
            {   Qid = i;                                                    // available CPU
                break;                                                      // smaller CPU ID is preferred
            }   //end for-if
        if (Qid > -1)                                                       // Cases 1, 2: new job is executed
        {   allQ[Qid]->avail = newJob.arrival + newJob.duration;            // time to finish new job, the next available time
            answers.addDoneJob(newJob.jobID, Qid+1, allQ[Qid]->avail, 0);   // call: add one finished job with no delay
            continue;                                                       // continue the next job
        }   //end if
        minQ = Q_MAX + 1;                                                   // set the upper bound of queue length
        for (int i = 0; i < qNum; ++i)
            if (allQ[i]->length() < minQ)
            {   Qid = i;                                                    // the CPU with a shorter queue length
                minQ = allQ[i]->length();
            }   //end if-for-if
        if (minQ == Q_MAX)                                                  // Case 4: all queues are full
            answers.addAbortJob(newJob.jobID, 0, newJob.arrival, 0);        // call: add one aborted job with no delay 4.1
        else                                                                // Case 3: at least one queue is not full
            allQ[Qid]->enQueue(newJob);                                     // call: append a job into one queue
    }   //end while
    finishQ();                                                              // call: finish the remaining jobs in queues
    switch (qNum)                                                           // call: write all results as a file
    {   case 1: answers.putAll("output", jobs.getFileID() + ".txt");
                break;
        case 2: answers.putAll("double", jobs.getFileID() + ".txt");
                break;
        default:
                answers.putAll("more", jobs.getFileID() + ".txt");
    }   // end switch
}   //end SQF
#endif  // _Simulation_HPP

int getNO(int, int);                                        // get a number from user

//*********************************************************/
// MAIN function is as below
//*********************************************************/
int main(void)
{   int     command = 0;                                    // user command
    JobList jobs;                                           // a list of jobs

    do
    {   string  temp;

        cout << endl << "**** Simulate FIFO Queues by SQF *****";
        cout << endl << "* 0. Quit                            *";
        cout << endl << "* 1. Sort a file                     *";
        cout << endl << "* 2. Simulate one FIFO queue         *";
#ifdef QUIZ
        cout << endl << "* 3. Simulate two queues by SQF      *";
#endif
        cout << endl << "**************************************";
#ifndef QUIZ
        cout << endl << "Input a command(0, 1, 2): ";
#else
        cout << endl << "Input a command(0, 1, 2, 3): ";
#endif
        cin >> temp;                                        // get the command
        command = -1;
        if ((temp.size() == 1) &&
            ((temp[0] >= '0') && (temp[0] <= '3')))
             command = atoi(temp.c_str());
        switch (command)
        {	case 0: break;
            case 1: if (jobs.getSorted())                   // call: read all from a file and sort them
                    {   // jobs.showAll();                     // call: output all on screen
                        cout << endl << "See " << "sorted" << jobs.getFileID() << ".txt" << endl;
                    }   //end if
                    break;
            case 2: if (jobs.getAll("sorted"))              // call: read all from a sorted file
                    {   Simulation  sim(jobs,1);            // create a simulation of one queue

                        // jobs.showAll();                     // call: output all on screen
                        cout << endl << "The simulation is running...";
                        sim.SQF();                          // call: simulate FIFO queues by SQF
                        cout << endl << "See " << "output" << jobs.getFileID() << ".txt" << endl;
                    }   //end if
                    break;
#ifdef QUIZ
            case 3: if (jobs.getAll("sorted"))              // call: read all from a sorted file
                    {   Simulation  sim(jobs,2);            // create a simulation of two queues

                        // jobs.showAll();                     // call: output all on screen
                        cout << endl << "The simulation is running...";
                        sim.SQF();                          // call: simulate FIFO queues by SQF
                        cout << endl << "See " << "double" << jobs.getFileID() << ".txt" << endl;
                    }   //end if
                    break;
#endif
            default: cout << endl << "Command does not exist!" << endl;
        }	// end switch
    } while (command != 0);                                 // 0: stop the program
    return 0;
}	// end of main
