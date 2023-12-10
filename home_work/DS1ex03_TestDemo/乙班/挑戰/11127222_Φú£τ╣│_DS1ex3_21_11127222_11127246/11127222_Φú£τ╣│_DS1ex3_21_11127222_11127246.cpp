// 11127222 ªô§±ã¸ 
// 11127246 ³¯¬fÊo 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <map>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace std::chrono;

typedef struct jT
{
    int OID;
    int arrival;
    int duration;
    int timeout;

    // Define the equality operator so I can use == after my code
    bool operator==(const jT& other) const {
        // Customize the comparison logic based on your needs
        return OID == other.OID && arrival == other.arrival &&
               duration == other.duration && timeout == other.timeout;
    }

} jobType;


map<string, int> inputFirstLine(  ifstream &inputFile );

class JobList{
    vector<jobType> aList;
    string fileID;
    int num = 0;
    void reset(){
        aList.clear();
        fileID.clear();
    }

    void showJob( jobType & );
    void putAll();

public:
    JobList(){
        reset();
    }
    ~JobList(){
        reset();
    }
    bool isEmpty(){
        return aList.empty();
    };

    int getLength(){
        return (int)aList.size();

    }

    void sortByArrival();
    void setNum( string id ){
        fileID = id;
    }

    bool getAll(const string& ); // read all from a file
    bool getStored();
    void nextJob(jobType & );
    void nextJobCheck( jobType &);
    bool outputFile();
    void sortByOID();
    void getCurrentJob( jobType & );
    void showTime();
};

JobList sortingData( string file_name );

void JobList::showJob(jobType & jt ) {
    cout << jt.OID << '\t' << jt.arrival << '\t' << jt.duration << '\t' << jt.timeout << endl;
}


bool JobList::getAll(const string& file_name ) {
    ifstream inputFile( file_name );
    if ( !inputFile.is_open() ) {
        inputFile.close();
        return false; // Check file is open or not
    }

    string t;
    char inputC;
    map<string, int >  inputOrder = inputFirstLine( inputFile );

    int i = 0;

    jT j;
    while ( inputFile >> t ){
        // Process the word as needed
        switch (i % 4) {
            case  0:
                j.OID = stoi(t);
                i++;
                break;
            case 1:
                j.arrival = stoi( t);
                i++;
                break;
            case 2:
                j.duration = stoi( t );
                i++;
                break;
            case 3:
                j.timeout = stoi(t);
                aList.push_back(j);
                i++;
                break;
        }
    }
    inputFile.close();
    return true;
}

void JobList::showTime() {
    int i = 0;
    cout << "OLD\t" << " Arrival\t" << "Duration\t" << " TimeOut\t" << endl;
    for ( jT j : aList ) {
        cout << "(" << i << ")  " << j.OID ;
        cout << '\t' << j.arrival;
        cout << '\t' << j.duration;
        cout << '\t' << j.timeout<< endl;
        i++;
    }
}

bool JobList::getStored() {

    if (!getAll("C:\\Users\\imagi\\CLionProjects\\hw\\input304.txt")){
        return false;
    }
    sortByArrival();
    return true;
}

void JobList::sortByArrival() {

    for (int gap = aList.size()/2; gap > 0; gap/=2  ) {

        for ( int i = gap; i < aList.size() ; i+=1 ) {
            jobType temp = aList[i];
            int j;
            for ( j = i; j >= gap && aList[j - gap].arrival > temp.arrival ;j -= gap ) {


                aList[j] = aList[j - gap];
            }
            aList[j] = temp;
        }
    }

}


bool JobList::outputFile() {
    ofstream outputFile;
    outputFile.open( "sorted" + fileID + ".txt" );
    if ( ! outputFile.is_open() ){
        cout << "error" << endl;
        return false;
    }
    outputFile << "OLD " << " Arrival  " << "Duration " << " Time  " << endl;
    for ( jT j : aList ) {
        outputFile << j.OID ;
        outputFile << '\t' << j.arrival;
        outputFile << '\t' << j.duration;
        outputFile << '\t' << j.timeout << endl;
    }
    outputFile.close();
    return true;
}


void JobList::sortByOID() {
    for (int gap = aList.size()/2; gap > 0; gap/=2  ) {

        for ( int i = gap; i < aList.size() ; i+=1 ) {
            jobType temp = aList[i];
            int j;
            for ( j = i; j >= gap && (aList[j - gap].arrival == temp.arrival && aList[j - gap].OID > temp.OID) ||
                         (aList[j - gap].arrival > temp.arrival) ; j -= gap ) {
                aList[j] = aList[j - gap];
            }
            aList[j] = temp;
        }
    }


}

void JobList::nextJob(jobType & jt ) {
    aList.erase( aList.begin() );
    jt = aList.front();
}

void JobList::getCurrentJob(jobType & jb )  {
    /// leave for default constructor if alist is empty
    if ( aList.empty() ) {
        jb = {};
        return;
    }

    jb = aList.front();
    aList.erase( aList.begin() );
}

void JobList::nextJobCheck( jobType &job ) {
    // index for element in this aList iterator<jT*, vector<jT>>
    auto it  = find(aList.begin(), aList.end(), job);

    int index = 0;
    if ( it != aList.end() ){
        // get the index form begin
        index = distance( aList.begin(), it );
    }
    job = aList[index];
}

template <typename T>
class JobQueue{
    T *cA;  // circular array
    int qFront, qBack;
    int qSize, qMax;

    int next( int idx ); // move to the next position in queue


public:
    int avaT;    // avalible time in CPU
    JobQueue():cA( NULL ), qFront(0), qBack(0), qSize(0), qMax(0), avaT(0){
        // constructor for JobQueue with no element
    }
    JobQueue<T>( int maxS ):cA(new T[maxS]), qFront(0), qSize(0), avaT(0), qMax(maxS), qBack(0){
        ////
    }

    int length() const{
        return qSize;
    };


    bool isEmpty() const{
        if ( qSize == 0 )
            return true;
        return false;

    };




    bool isFull() const{
        if ( qSize == qMax )
            return true;
        return false;
    }

    void  enQueue( T& );
    void getFront( T& );
    void deQueue(); // drop the first  element
    void deQueue( T& ); // drop
    void showQ(){
    }

    void clearQ(); // clean up
    ~JobQueue(){
        delete[] cA;
    }

};

template<typename T>
void JobQueue<T>::clearQ() {
    qFront = qBack = qSize = 0;
}

template<typename T>
void JobQueue<T>::deQueue(T & job ) {
    if ( qSize == 0 ) {
        job = {};
        return;
    }
    job = cA[qFront];
    qFront = next(qFront);
    --qSize;

}

template<typename T>
void JobQueue<T>::deQueue() {
    if (isEmpty()) {
        cout << "Queue is empty. Cannot dequeue." << endl;
        return;
    }

    qFront = next(qFront);
    qSize--;

}

template<typename T>
void JobQueue<T>::getFront(T & job ) {
    if ( isEmpty() ){
        cout << "there is no element to get";
        return;
    }
    job = cA[qFront];
}

template<typename T>
int JobQueue<T>::next(int idx ) {
    return (idx + 1) % qMax;
}

template<typename T>
void JobQueue<T>::enQueue(T & j ) {
    if ( isFull() ){
        cout << "error _ enqueue" << endl;
        return;
    }

    cA[qBack] = j;
    qBack = next(qBack);
    qSize++;
}


class AnsList{
    typedef struct aT{
        int OID;
        int CID;
        int Abort;
        int Delay;

    } abortType;




    typedef struct dT{
        int OID;
        int CID;
        int Departure;
        int Delay;

    } doneType;

    vector<abortType> abortJobs;
    vector<doneType> doneJobs;
    float avgDelay;
    float successfulRate;
    int showAbortJobs( string,  ostream& stream );
    int showDoneJobs( string, ostream& stream );
    void ComputeState();
    void showState();

public:
    AnsList(): avgDelay(0.0), successfulRate(0){
    }

    ~AnsList(){

    };


    void showALL( string );
    void showALL( );
    void addAbortJob( int, int, int, int );
    void addDoneJob( int, int, int, int );
    void putAll( string, string );

};



int AnsList::showDoneJobs( string file_num, ostream& outputFile ) {


    outputFile << '\t' << "[Done Jobs]" << endl;
    outputFile << '\t' << "OID\t" <<  "CID\t"  << "Departure" << '\t' << "Delay" << endl;
    int i = 0;

    for ( doneType j : doneJobs ) {
        outputFile << "[" << i+1 << "]" << '\t';
        outputFile << '\t' << j.OID ;
        outputFile << '\t' << j.CID;
        outputFile << '\t' << j.Departure;
        outputFile << '\t' << j.Delay << endl;
        avgDelay = avgDelay + j.Delay;
        i++;
    }
    return i;

}

void AnsList::ComputeState() {

}

void AnsList::showState() {

}

void AnsList::showALL(string file_num ) {
    ofstream outputFile(file_num );
    int i = showAbortJobs( file_num, outputFile  );
    int j = showDoneJobs( file_num, outputFile );

    if ( ! outputFile.is_open( ) ){
        cout << "error" << endl;
        return ;
    }
    outputFile << "[Average Delay]" << round( (avgDelay / (float )(i+j) )*100 )/100 << " ms" << endl;
    outputFile <<  "[Success Rate]" <<  round (( (float) (j)/ (float )(i+j)) * 10000 ) / 100  << " %" << endl;

}

void AnsList::addAbortJob(int OID, int Abort, int CID, int Delay ) {

    abortType abort;
    abort.OID = OID;
    abort.Delay = Delay;
    abort.Abort = Abort;
    abort.CID = CID;
    abortJobs.push_back( abort );
}

void AnsList::addDoneJob(int OID, int Departure, int CID, int Delay) {
    doneType done;
    done.OID = OID;
    done.Departure = Departure;
    done.Delay = Delay;
    done.CID = CID;
    doneJobs.push_back( done );
}

void AnsList::showALL() {
    cout << "done Jobs" << endl;
    for (doneType d: doneJobs ) {
        cout << d.OID << '\t' <<  d.Departure << '\t' << d.Delay << endl;
    }
    cout << "abort Jobs" << endl;
    for (abortType a : abortJobs ) {
        cout << a.OID << '\t' <<  a.Abort << '\t' << a.Delay << endl;
    }
}

int AnsList::showAbortJobs( string file_num, ostream& outputFile ) {


    outputFile << '\t' << "[Abort Jobs]" << endl;
    outputFile << '\t' << "OID" << '\t' << "CID" << '\t' << "Abort" << '\t' << "CID" << '\t' << "Delay" << endl;
    int i = 0;



        for ( abortType j : abortJobs ) {
            outputFile << "[" << i+1 << "]" << '\t';
            outputFile << '\t' << j.OID ;
            outputFile << '\t' << j.Abort;
            outputFile << '\t' << j.CID;
            outputFile << '\t' << j.Delay << endl;
            i++;
            avgDelay = avgDelay + (float )j.Delay;
        }

    return i;
}


class Simulation{
    JobList  jobs;
    AnsList answers;
    int *cpuT;
    // for mission 2 default num is 0
        JobQueue<jobType>** allQ;
    int QNum;
    int QuesesN;
    void delQ( int  );
    void updateQ( int , int );
    void finishQ();

    template <typename T>
    JobQueue<T>* findShortestQueue( ) {
        int shortestIndex = 0;
        for (int i = 1; i < QuesesN; ++i) {
            if (allQ[i]->length() < allQ[shortestIndex]->length()) {
                shortestIndex = i;
            }
        }

        return allQ[shortestIndex];
    }


    template<typename T>
    int getIndexfromshortestQ( JobQueue<T>* q ){
        for (int i = 0; i < QuesesN; ++i) {
            if (allQ[i] == q) {
                return i;
            }
        }

        return -1;
        
    }


public:
    Simulation( JobList aList, int QuesesN, int N ): jobs(aList), QNum( N ), QuesesN(QuesesN){
        allQ = new JobQueue<jobType> * [QuesesN];
        for (int i = 0; i < QuesesN; i++ ) {
            // size 3 is for mission 2
            allQ[i] = new JobQueue<jobType>(N ); // init all queue for simulation
        }
    }

    ~Simulation(){
        for (int i = 0; i <QuesesN; ++i) {
            delete allQ[i];
        }
        delete[] allQ;

    }

    void output( string file_name );
    void SQF();
    int slowestCpu();
    int shortestAndslowest();
    int cpuIsIdle( int j );
    JobQueue<jT> *longestQ();
};

void Simulation::delQ(int) {
}

void Simulation::SQF() {
    // declare
    cpuT = new int[2];
    for (int i = 0; i < 2; i++) {
        cpuT[i] = 0;
    }
    JobQueue<jT> *shortestQueue;
    // init shortest q
    shortestQueue = findShortestQueue<jobType>();
    jT j;
    jobs.getCurrentJob(j);
    jT pin = {};
    // init time init n is zero when everything is 0
    int n = getIndexfromshortestQ(shortestQueue);
    // init stop condition
    int stop = jobs.getLength();

    while (!(j == pin)) {
        // if Q is empty and job can execute
        if (longestQ()->isEmpty() && (cpuT[slowestCpu()] + j.duration) <= j.timeout &&
            j.arrival <= cpuT[n] + j.duration) {
            n = slowestCpu();
            if (cpuT[n] < j.arrival)
                cpuT[n] = j.arrival;
            answers.addDoneJob(j.OID, cpuT[n] + j.duration, n + 1, 0);
            cpuT[n] = cpuT[n] + j.duration;
            n = getIndexfromshortestQ(shortestQueue);

            // if
        } else {
            // if no need to pop up
            if (!shortestQueue->isFull() && j.arrival < cpuT[shortestAndslowest()]) {
                shortestQueue->enQueue(j);
                // get shortest Q
            }
                // cancel job

                // pop
            else if (cpuIsIdle(j.arrival ) != -1 ) {
                jT d;
                n = cpuIsIdle(j.arrival );
                shortestQueue = allQ[n];
                shortestQueue->deQueue(d);
                shortestQueue->enQueue(j );
                if ( d == pin )
                    break;
                // if
                while (cpuT[n] + d.duration >= d.timeout) {
                    if (cpuT[n] > d.timeout) {
                        answers.addAbortJob(d.OID, cpuT[n], n + 1, cpuT[n] - d.arrival);

                    } else {
                        answers.addAbortJob(d.OID, d.timeout, n + 1, d.timeout - d.arrival);
                        cpuT[n] = d.timeout;
                    }
                    if (shortestQueue->isEmpty())
                        break;
                    shortestQueue->deQueue(d);
                }

                if (cpuT[n] + d.duration <= d.timeout) {

                    if (cpuT[n] < d.arrival) {
                        cpuT[n] = d.arrival;
                    }
                    answers.addDoneJob(d.OID, cpuT[n] + d.duration, n + 1, cpuT[n] - d.arrival);
                    cpuT[n] = cpuT[n] + d.duration;
                    //n = slowestCpu();

                }


            }   // dump when q is full and slowest cpu is not available
            else if (j.arrival <= cpuT[slowestCpu()] && shortestQueue->isFull()) {
                answers.addAbortJob(j.OID, j.arrival, 0, 0);
                jobs.getCurrentJob(j);
                shortestQueue = findShortestQueue<jobType>();
                n = getIndexfromshortestQ(shortestQueue);
                continue;
            }
        }

        jobs.getCurrentJob(j);
        shortestQueue = findShortestQueue<jobType>();
    }


    // for Q is not fully clear
    for (int k = 0; k < QuesesN; k++) {
        int length = allQ[k]->length();
        for (int l = 0; l < length; l++ ) {
            allQ[k]->deQueue(j );
            if (cpuT[k] + j.duration > j.timeout) {
                if (cpuT[k] > j.timeout) {
                    answers.addAbortJob(j.OID, cpuT[k], k+1, cpuT[k] - j.arrival);
                } else {
                    answers.addAbortJob(j.OID, j.timeout, k+1, j.timeout - j.arrival);
                    cpuT[k] = j.timeout;
                }
            }
            if ( cpuT[k] + j.duration <= j.timeout ){


                if ( cpuT[k] < j.arrival ){
                    cpuT[k] = j.arrival;
                }
                answers.addDoneJob( j.OID,  cpuT[k] + j.duration, k+1, cpuT[k] - j.arrival  );
                cpuT[k] = cpuT[k] + j.duration;

            }
        }

    }
}

void Simulation::output(string file_num ) {
    answers.showALL( file_num );
}

int Simulation::slowestCpu() {
    if (QuesesN <= 0) {
        // Handle the case where the array is empty or invalid
        return -1; // or another suitable sentinel value
    }

    int slowestCPU = 0;

    for (int i = 0; i < QuesesN; ++i) {
        if ( cpuT[slowestCPU] > cpuT[i] ) {
            slowestCPU = i;
        }
    }
    return slowestCPU;
}

int Simulation::shortestAndslowest() {
    bool same = true;
    for (int i = 0; i < QuesesN ; i++ ) {
        if ( allQ[i]->length() !=  allQ[i]->length()  ){
            same = false;
        }
    }
    if ( !same ){
        return getIndexfromshortestQ( findShortestQueue<jT>() );
    }
    return slowestCpu();
}

int Simulation::cpuIsIdle( int j  ) {
    vector< int > idleCPU;
    for (int i = 0; i < QuesesN ; i++ ) {
        if ( cpuT[i] <= j  ){
            idleCPU.push_back(i);
        }

    }
    if ( !idleCPU.empty() ){
        return idleCPU[0];
    }

    return -1;
}

JobQueue<jT> *Simulation::longestQ() {
    JobQueue<jT>* current = allQ[0];
    for (int i = 0; i < QuesesN ; i++ ) {
        if ( allQ[i]->length() > current ->length() )
        current = allQ[i];
    }
    return current;

}


int main(){

    int command = 0 ;
    string file_num;
    do {
        cout <<"**** Simulate FIFO Queues by SQF *****";
        cout << endl <<"* 0. Quit                            *";
        cout << endl <<"* 1. Sort a file                     *";
        cout << endl <<"* 2. Simulate one FIFO queue         *";
        cout << endl <<"* 3. Simulate two queues by SQF      *";
        cout << endl <<"**************************************" << endl;
        cout <<"Input a command(0, 1, 2, 3):";
        cin >> command;
        switch( command )
        {
            case 0: {
                break;
            }

            case 1: {
                JobList jobList;
                cout << endl << "Input a file number:";
                cin >> file_num;

                jobList.setNum( file_num );
                // jobList.getAll( file_num );
                // file_name = "input" + file_num + ".txt";
                sortingData( file_num );
                cout << "See sorted" << file_num << ".txt"<< endl << endl;
                break;
            }


            case 2: {
                if ( file_num.empty() ){
                    cout << endl << "Input a file number:";
                    cin >> file_num;
                }
                JobList jobList;
                jobList.setNum( file_num );
                string file_name = "output" + file_num + ".txt";
                jobList.getAll( "sorted" + file_num + ".txt" );
                Simulation* simulation = new Simulation( jobList, 1,3 );
                //jobList.showTime();
                simulation -> SQF();
                simulation -> output( file_name );

                cout << endl << "The simulation is running..." << endl << "See " +  file_name << endl << endl;

                break;
            }
            case 3:{
                if ( file_num.empty() ){
                    cout << endl << "Input a file number:";
                    cin >> file_num;
                }
                JobList jobList;
                jobList.setNum( file_num );
                string file_name = "double" + file_num + ".txt";
                jobList.getAll( "sorted" + file_num + ".txt" );
                Simulation* simulation = new Simulation( jobList, 2,3 );
                //jobList.showTime();
                simulation -> SQF();
                simulation -> output( file_name );

                cout << endl << "The simulation is running..." << endl << "See " +  file_name << endl << endl;

                break;




            }




            default: cout << endl << "Command does not exist!" << endl;
        }
    }
    while ( command != 0);
    system("pause");
    return 0;



}



JobList sortingData( string file_name ) {

    string file_num = file_name;
    JobList jl;
    jl.setNum( file_num );
    clock_t start = clock();
    file_name = "input" + file_name + ".txt";
    jl.getAll( file_name ) ;
    /// time after read
    clock_t read = clock();
    //// time after show
    jl.showTime();
    clock_t show = clock();
    jl.sortByOID();

    //// time after sorting
    clock_t sort = clock();
    if ( !jl.outputFile() ) {
        cout << "file generate error" << endl;
    }
    /// time after writing
    cout << "Reading data: " << read - start << " clocks (" << (read - start) << ".00 ms)" << endl;


    cout << "sorting data: " << sort - show << " clocks (" << sort - show << ".00 ms)" << endl;


    cout << "writing data: " << clock() - sort << " clocks (" << (clock() - sort) << ".00 ms)" << endl;

    return jl;
}






map<string, int > inputFirstLine(ifstream & inputFile ) { // process the first line order also input it
    map<string, int> order;
    string item;
    char inputC;
    int i = 0;


    while (inputFile.get(inputC) && inputC != '\n') {
        if (inputC != ('\t' | '\0'))
            item += inputC;
        if (inputC == '\t') {
            order.insert(pair<string, int>(item, i));
            i++;
            item = "";
        }


    }


    order.insert(pair<string, int>(item, i));


    return order;
}
