#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

struct Job {
    int id;
    int arrivalTime;
    int duration;
    int timeOut;
};

class JobList {
private:
    vector< Job > jobList;
    string fileName;

public:
    JobList( string fileName = "" ) {
        this->fileName = fileName;
    } // JobList()
    ~JobList() {
        jobList.clear();
    } // ~JobList()

    bool readInput() {
        ifstream inFile( "input" + fileName + ".txt" );
        if ( !inFile.is_open() ) {
            cout << endl << "### input" << fileName << ".txt does not exist! ###" << endl;
            return false;
        } // if

        Job job;
        // abort the first line
        string line;
        getline( inFile, line );

        while ( inFile >> job.id >> job.arrivalTime >> job.duration >> job.timeOut ) {
            jobList.push_back( job );
        } // while

        inFile.close();
        return true;
    } // readFile()

    bool readSorted() {
        ifstream inFile( "sorted" + fileName + ".txt" );
        if ( !inFile.is_open() ) {
            cout << endl << "### sorted" << fileName << ".txt does not exist! ###" << endl;
            return false;
        } // if

        Job job;
        // abort the first line
        string line;
        getline( inFile, line );

        while ( inFile >> job.id >> job.arrivalTime >> job.duration >> job.timeOut ) {
            jobList.push_back( job );
        } // while

        inFile.close();
        return true;
    } // readSorted()

    void shellSort() {
        // sort by arrival time, if arrival time is the same, sort by id
        for ( int gap = jobList.size() / 2 ; gap > 0 ; gap /= 2 ) {
            for ( int i = gap ; i < jobList.size() ; i++ ) {
                Job temp = jobList[ i ];
                int j = i;
                // insertion sort
                for ( ; j >= gap && ( jobList[ j - gap ].arrivalTime > temp.arrivalTime ||
                        ( jobList[ j - gap ].arrivalTime == temp.arrivalTime && jobList[ j - gap ].id > temp.id ) ) ; j -= gap ) {
                    jobList[ j ] = jobList[ j - gap ];
                } // for
                jobList[ j ] = temp;
            } // for
        } // for
    } // shellSort()

    void printJobListOnScreen() {
        cout << endl << "\tOID\tArrival\tDuration\tTimeOut";
        for ( int i = 0 ; i < jobList.size() ; i++ ) {
            cout << endl << '(' << i + 1 << ')' << '\t';
            cout << jobList[ i ].id << '\t';
            cout << jobList[ i ].arrivalTime << '\t';
            cout << jobList[ i ].duration << '\t';
            cout << jobList[ i ].timeOut;
        } // for
        cout << endl;
    } // printJobListOnScreen()

    void writeJobListToFile() {
        ofstream outFile( "sorted" + fileName + ".txt" );
        if ( !outFile.is_open() ) {
            cout << endl << "output" << fileName << ".txt does not exist!" << endl;
            return;
        } // if

        outFile << "OID\tArrival\tDuration\tTimeOut" << endl;
        for ( int i = 0 ; i < jobList.size() ; i++ ) {
            outFile << jobList[ i ].id << '\t';
            outFile << jobList[ i ].arrivalTime << '\t';
            outFile << jobList[ i ].duration << '\t';
            outFile << jobList[ i ].timeOut << endl;
        } // for
        outFile.close();
    } // writeJobListToFile()

    Job getNextJob() {
        Job job = jobList[ 0 ];
        jobList.erase( jobList.begin() );
        return job;
    } // getNextJob()

    bool isEmpty() {
        return jobList.empty();
    } // isEmpty()

    int nextArrivalTime() {
        if ( isEmpty() ) return -1;
        return jobList[ 0 ].arrivalTime;
    } // nextArrivalTime()

    string getFileName() {
        return fileName;
    } // getFileName()

}; // class JobList

class AnswerList {
struct AbortedJob {
    int id;
    int abort;
    int delay;
};

struct DoneJob {
    int id;
    int departure;
    int delay;
};

private:
    vector< AbortedJob > abortedJobList;
    vector< DoneJob > doneJobList;

    float getAverageDelay() {
        float sum = 0.0;
        for ( int i = 0 ; i < doneJobList.size() ; i++ ) {
            sum += doneJobList[ i ].delay;
        } // for
        for ( int i = 0 ; i < abortedJobList.size() ; i++ ) {
            sum += abortedJobList[ i ].delay;
        } // for

        return sum / ( doneJobList.size() + abortedJobList.size() );
    } // calculateAverageDelay()

    float getSuccessRate() {
        return ( float ) doneJobList.size() / ( doneJobList.size() + abortedJobList.size() );
    } // calculateSuccessRate()

public:
    AnswerList() {
    } // AnswerList()
    ~AnswerList() {
        abortedJobList.clear();
        doneJobList.clear();
    } // ~AnswerList()

    void addAbortedJob( Job abortedJob, int currentTime ) {
        AbortedJob job;
        job.id = abortedJob.id;
        job.abort = currentTime;
        job.delay = currentTime - abortedJob.arrivalTime;
        abortedJobList.push_back( job );
    } // addAbortedJob()

    void addDoneJob( Job doneJob, int currentTime ) {
        DoneJob job;
        job.id = doneJob.id;
        job.departure = currentTime;
        job.delay = currentTime - doneJob.arrivalTime - doneJob.duration;
        doneJobList.push_back( job );
    } // addDoneJob()

    void writeAnswerListToFile( string fileName ) {
        ofstream outFile( "output" + fileName + ".txt" );
        if ( !outFile.is_open() ) {
            cout << endl << "output" << fileName << ".txt does not exist!" << endl;
            return;
        } // if

        outFile << "\t[Abort Jobs]" << endl;
        outFile << "\tOID\tAbort\tDelay" << endl;
        for ( int i = 0 ; i < abortedJobList.size() ; i++ ) {
            outFile << '[' << i + 1 << ']' << '\t';
            outFile << abortedJobList[ i ].id << '\t';
            outFile << abortedJobList[ i ].abort << '\t';
            outFile << abortedJobList[ i ].delay << endl;
        } // for
        outFile << "\t[Jobs Done]" << endl;
        outFile << "\tOID\tDeparture\tDelay" << endl;
        for ( int i = 0 ; i < doneJobList.size() ; i++ ) {
            outFile << '[' << i + 1 << ']' << '\t';
            outFile << doneJobList[ i ].id << '\t';
            outFile << doneJobList[ i ].departure << '\t';
            outFile << doneJobList[ i ].delay << endl;
        } // for

        outFile << "[Average Delay]\t" << fixed << setprecision(2) << getAverageDelay() << " ms" << endl;
        outFile << "[Success Rate]\t" << fixed << setprecision(2) << getSuccessRate() * 100 << " %" << endl;
    } // writeAnswerListToFile()

}; // class AnswerList

class Queue {
private:
    vector< Job > jobList;
    int MAX_SIZE;

public:
    Queue( int MAX_SIZE = 3 ) {
        this->MAX_SIZE = MAX_SIZE; // default MAX_SIZE = 3
    } // Queue()
    ~Queue() {
        jobList.clear();
    } // ~Queue()

    bool isEmpty() {
        return jobList.empty();
    } // isEmpty()

    bool isFull() {
        return jobList.size() == MAX_SIZE;
    } // isFull()

    Job getFront() {
        if ( isEmpty() ) {
            Job job;
            job.id = -1;
            return job;
        } // if
        return jobList[ 0 ];
    } // getFront()

    void enqueue( Job job ) {
        jobList.push_back( job );
    } // enqueue()

    Job dequeue() {
        if ( isEmpty() ) {
            Job job;
            job.id = -1;
            return job;
        } // if
        Job job = jobList[ 0 ];
        jobList.erase( jobList.begin() );
        return job;
    } // dequeue()

}; // class Queue

class Sumulation {
protected:
    Job currentJob;
    int departureTimeOfCurrentJob;
    int currentTime;
    JobList jobList;
    AnswerList answerList;
    Queue queue;

    bool currentJobIsDone() {
        return currentTime >= departureTimeOfCurrentJob;
    } // currentJobIsDone()

    bool currentJobIsAborted() {
        return currentTime >= currentJob.timeOut;
    } // currentJobIsAborted()

    bool noCurrentJob() {
        return currentJob.id == -1;
    } // noCurrentJob()

    void putInDoneJobList() {
        answerList.addDoneJob( currentJob, currentTime );
        currentJob.id = -1; // no current job
        departureTimeOfCurrentJob = -1; // no current job
    } // putInDoneJobList()

    void putInAbortedJobList() {
        answerList.addAbortedJob( currentJob, currentTime );
        currentJob.id = -1; // no current job
        departureTimeOfCurrentJob = -1; // no current job
    } // putInAbortedJobList()

    void getJobFromQueue() {
        currentJob = queue.dequeue();
        while ( currentJobIsAborted() ) {
            putInAbortedJobList();
            if ( queue.isEmpty() ) break;
            currentJob = queue.dequeue();
        } // while
        if ( !noCurrentJob() ) departureTimeOfCurrentJob = currentTime + currentJob.duration;
    } // getJobFromQueue()

    void dealWithCurrentJob() {
        if ( currentJobIsDone() ) putInDoneJobList();
        else if ( currentJobIsAborted() ) putInAbortedJobList();
    } // dealWithCurrentJob()

public:
    Sumulation( string fileName = "" ) {
        currentJob.id = -1; // no current job
        currentJob.timeOut = -1; // no current job
        currentJob.duration = -1; // no current job
        currentJob.arrivalTime = -1; // no current job
        departureTimeOfCurrentJob = -1; // no current job
        currentTime = -1;
        jobList = JobList( fileName );
        answerList = AnswerList();
        queue = Queue();
    } // Sumulation()
    ~Sumulation() {
    } // ~Sumulation()

    bool simulate() {
        if ( !jobList.readSorted() ) return false; // read sorted file
        cout << endl << "The simulation is running...";

        while ( !jobList.isEmpty() ) {
            int nextArrivalTime = jobList.nextArrivalTime();
            if ( currentTime == -1 ) currentTime = nextArrivalTime; // initialize currentTime

            // check if the current job is done or aborted, if there is no current job, it will not enter the if statement
            // if the current job is done or aborted, there is no current job
            if ( !noCurrentJob() ) dealWithCurrentJob();

            // if there is no current job, get a new job
            // get a new job from the queue first, if the queue is empty, get a new job from the jobList
            if ( noCurrentJob() && !queue.isEmpty() ) getJobFromQueue();
            // if queue is empty or all jobs in the queue are aborted, get a new job from the jobList
            if ( noCurrentJob() && currentTime == nextArrivalTime ) {
                currentJob = jobList.getNextJob();
                departureTimeOfCurrentJob = currentTime + currentJob.duration;
                nextArrivalTime = jobList.nextArrivalTime();
            } // if

            // put all jobs that arrive into the queue or abort them
            while ( currentTime >= nextArrivalTime && !jobList.isEmpty() ) {
                Job job = jobList.getNextJob();
                if ( queue.isFull() ) {
                    answerList.addAbortedJob( job, currentTime );
                } // if
                else {
                    queue.enqueue( job );
                } // else
                nextArrivalTime = jobList.nextArrivalTime();
            } // while

            // update currentTime to the smallest value of nextArrivalTime, departureTimeOfCurrentJob and timeOut
            currentTime = nextArrivalTime;
            // if the jobList is empty, nextArrivalTime will be -1
            if ( currentTime == -1 ) {
                currentTime = departureTimeOfCurrentJob;
                if ( currentJob.timeOut < currentTime ) currentTime = currentJob.timeOut;
            } // if
            else if ( !noCurrentJob() ) {
                if ( departureTimeOfCurrentJob < currentTime ) currentTime = departureTimeOfCurrentJob;
                if ( currentJob.timeOut < currentTime ) currentTime = currentJob.timeOut;
            } // else if
        } // while

        // finish the rest of the jobs
        while ( !noCurrentJob() || !queue.isEmpty() ) {
            dealWithCurrentJob();
            // if there is no current job, get a new job from the queue
            if ( !queue.isEmpty() ) getJobFromQueue();

            if ( !noCurrentJob() ) {
                currentTime = departureTimeOfCurrentJob;
                if ( currentJob.timeOut < currentTime ) currentTime = currentJob.timeOut;
            } // if
        } // while

        answerList.writeAnswerListToFile( jobList.getFileName() );
        return true;
    } // simulate()

}; // class Sumulation

int getCommand() {
    string input;
    getline( cin, input );
    if ( input.length() != 1 ) return -1;
    else if ( input[ 0 ] < '0' || input[ 0 ] > '3' ) return -1;
    else return input[ 0 ] - '0';
} // getCommand()

string getFileName() {
    cout << endl << "Input a file number: ";
    string fileName;
    getline( cin, fileName );

    return fileName;
} // getFileName()

int main() {
    string fileName = "";

    do {
        cout << endl << "**** Simulate FIFO Queues by SQF *****";
        cout << endl << "* 0. Quit                            *";
        cout << endl << "* 1. Sort a file                     *";
        cout << endl << "* 2. Simulate one FIFO queue         *";
        cout << endl << "**************************************";
        cout << endl << "Input a command(0, 1, 2): ";

        int command = getCommand();
        if ( command == 1 ) {
            fileName = getFileName();
            JobList jobList( fileName );
            // clock_t is a type for storing processor clock, not elapsed real time
            clock_t readStart, readEnd, sortStart, sortEnd, writeStart, writeEnd;

            readStart = clock();
            if ( !jobList.readInput() ) continue;
            readEnd = clock();

            sortStart = clock();
            jobList.shellSort();
            sortEnd = clock();

            writeStart = clock();
            jobList.writeJobListToFile();
            writeEnd = clock();

            jobList.printJobListOnScreen();
            cout << endl << "Reading data: " << readEnd - readStart << " clocks";
            cout << " (" << fixed << setprecision(2) << ( readEnd - readStart ) / ( double ) CLOCKS_PER_SEC * 1000 << " ms)";
            cout << endl << "Sorting data: " << sortEnd - sortStart << " clocks";
            cout << " (" << fixed << setprecision(2) << ( sortEnd - sortStart ) / ( double ) CLOCKS_PER_SEC * 1000 << " ms)";
            cout << endl << "Writing data: " << writeEnd - writeStart << " clocks";
            cout << " (" << fixed << setprecision(2) << ( writeEnd - writeStart ) / ( double ) CLOCKS_PER_SEC * 1000 << " ms)";
            cout << endl << endl << "See sorted" << fileName << ".txt" << endl;
        } // if
        else if ( command == 2 ) {
            if ( fileName == "" ) {
                fileName = getFileName();
            } // if
            Sumulation sumulation( fileName );
            if ( !sumulation.simulate() ) continue;

            cout << endl << "See output" << fileName << ".txt" << endl;
        } // else if
        else if ( command == 0 ) {
            break;
        } // else if
        else {
            cout << endl << "Command does not exist!" << endl;
        } // else
    } while ( true );
    
    system( "pause" ); // for windows
    return 0;
} // main()