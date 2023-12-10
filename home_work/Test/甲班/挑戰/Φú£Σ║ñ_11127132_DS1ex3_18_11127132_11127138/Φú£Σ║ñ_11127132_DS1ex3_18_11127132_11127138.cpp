//11127132, 羅海綺
//11127138, 林雨臻
#include <iostream> // cout, endl, fixed
#include <fstream> // open, is_open
#include <string> // string
#include <string.h>
#include <vector> // vector
#include <cstdlib> // atoi, system
#include <iomanip> // setw, setprecision
#include <ctime> // clock, CLOCKS_PER_SEC
using namespace std;

struct jobType{
    string OID; // order identifier
    int arrival; // arrival time
    int duration; // job duration
    int timeout; // expire time
};
struct abortType{
    string OID ;
    int CID = 0 ;
    int Abort ;
    int Delay ;
};
struct doneType{
    string OID;
    int CID = 0 ;
    int Departure ;
    int Delay ;
};
struct jobQ{
    jobType j;
    jobQ *next;
};

class JobList{
    string fileID; // file identifier
    vector<jobType> aList; // list of jobs with four columns
    vector<abortType> abortJobs; // list of aborted jobs with three columns
    vector<doneType> doneJobs;
    double avgDelay; // average delay
    double successRate; // percentage of done jobs

    void reset(){ //definition: initial set up
        vector<jobType>().swap(aList) ;
        vector<abortType>().swap(abortJobs) ;
        vector<doneType>().swap(doneJobs) ;
    } // end reset
    void showJob(){ // declaration: display a job on screen //
        int i = 1 ;
        cout << endl << "\tOID\tArrival\tDuration\tTimeOut" << endl;
        while( i-1<aList.size() ){
            string n = "(" + to_string(i) + ")" ;
            cout << n << "\t" << aList[i-1].OID << "\t" << aList[i-1].arrival << "\t" << aList[i-1].duration << "\t" << aList[i-1].timeout << endl;
            i++;
        }
    }
    void sortByArrival(){ // declaration: sort all by arrival time
        jobType swap;
        int n = aList.size(), tmp, id;
        sort = clock() ;
        for (int interval = n/2 ; interval>0 ; interval = interval/2) {// 直到間隔成為1，盡可能讓間隔成為1時的複雜度降低
            for (int i = interval ; i < n ; i++) {
                tmp = aList[i].arrival;
                id = stoi(aList[i].OID);
                for (int j = i; j >= interval && aList[j-interval].arrival >= tmp; j = j-interval) {
                    if(aList[j-interval].arrival > tmp){
                        swap = aList[j];
                        aList[j] = aList[j-interval];
                        aList[j-interval] = swap;
                    } // end if

                    if(aList[j-interval].arrival == tmp){
                        if(stoi(aList[j-interval].OID) > id ){
                            swap = aList[j];
                            aList[j] = aList[j-interval];
                            aList[j-interval] = swap;
                        } // end if
                    } // end if
                } // end for
            } // end for
        } // end for

        sort = clock() - sort;
    } // end sortByArrival
    void putAll(){ // declaration: write all as file
        string fileout = "sorted"+fileID+".txt" ;
        ofstream sortedout( fileout.c_str() ) ;
        if(!sortedout){
            cout << "### Cannot create sorted"<< fileID <<".txt ###\n" ;
            return ;
        }
        int i = 1 ;
        write = clock();
        sortedout << "OID\tArrival\tDuration\tTimeOut\n" ;
        while( i-1<aList.size() ){
            string n = "(" + to_string(i) + ")" ;
            sortedout << aList[i-1].OID << '\t' << aList[i-1].arrival << '\t' << aList[i-1].duration << '\t' << aList[i-1].timeout << '\n' ;
            //aList.erase(aList.begin());
            i++;
        }

        write = clock() - write;
        sortedout.close() ;
    }
    void showTime(){ // declaration: output time on screen
        cout << fixed << endl << "Reading data: " << read << " clocks " << "(" << setprecision(2) << double(read) << " ms)" << endl;
        cout << fixed << "Sorting data: " << sort << " clocks " << "(" << setprecision(2) << double(sort) << " ms)" << endl;
        cout << fixed << "Writing data: " << write << " clocks " << "(" << setprecision(2) << double(write) << " ms)" << endl;
    }

    void showAbortJobs(){ // declaration: display abort jobs on screen
        cout << endl << "\t[Abort List]" << endl;
        cout << "\tOID\tCID\tAbort\tDelay" << endl;
        int i = 1 ;
        while( i-1<abortJobs.size() ){
            string n = "[" + to_string(i) + "]" ;
            cout << n << "\t" << abortJobs[i-1].OID << "\t" << abortJobs[i-1].CID << "\t" << abortJobs[i-1].Abort << "\t" << abortJobs[i-1].Delay << endl;
            i++;
        }
    }
    void showDoneJobs(){ // declaration: display done jobs on screen
        cout << endl << "\t[Done List]" << endl;
        cout << "\tOID\tCID\tDeparture\tDelay" << endl;
        int i = 1 ;
        while( i-1<doneJobs.size() ){
            string n = "[" + to_string(i) + "]" ;
            cout << n << "\t" << doneJobs[i-1].OID << "\t" << doneJobs[i-1].CID << "\t" << doneJobs[i-1].Departure << "\t"
            << doneJobs[i-1].Delay << endl;
            i++;
        }
    }
    void computeStat(){ // declaration: compute the statistics
        avgDelay=0,successRate=0 ;
        for( int i = 0 ; i < abortJobs.size() ; i++){
            avgDelay = avgDelay + abortJobs[i].Delay ;
        }
        for( int i = 0 ; i < doneJobs.size() ; i++){
            avgDelay = doneJobs[i].Delay + avgDelay ;
        }

        if( abortJobs.size()+doneJobs.size()!=0 ){
            avgDelay = avgDelay / (abortJobs.size()+doneJobs.size()) ;
            successRate = 100*double(doneJobs.size()) / (abortJobs.size()+doneJobs.size()) ;
        }
    }
    void showStat(){ // declaration: display statistics on screen
        cout << fixed << "[Average Delay]\t" << setprecision(2) << avgDelay <<" ms" << endl ;
        cout << fixed << "[Success Rate]\t" << setprecision(2) << successRate <<" %" << endl ;
    }

public:
    int read ,sort ,write ;
    JobList(): avgDelay(0.0), successRate(0.0){ reset(); } // constructor for initialization
    ~JobList(){ reset(); } // destructor for initialization
    string getfileID (){
        return fileID ;
    }
    int getlengh(){
        return aList.size();
    }
    bool isEmpty() { // check whether it is empty or not
        return aList.empty();
    }
 
    bool getAll(string fileName){ // declaration: read all from a inputfile
        struct jobType job ;
        string tmp, filein = "input"+fileName+".txt" ;
        fileID = fileName ;
        ifstream input( filein.c_str() ); // c_str()將String物件轉換成C語言形式的字串常數。
        if( !input ) { // confirm existence or not
            cout << endl << "### " << filein << " does not exist! ###" << endl;
            return false ; // end
        }

        read = clock() ;
        input >> tmp >> tmp >> tmp >> tmp; // read the name of column
        while(!input.eof()){
            input >> job.OID >> job.arrival >> job.duration >> job.timeout ;
            if (!input.fail()) // ensue when .txt show up lots of \n will not read the last one into vector
                aList.push_back( job );
        } // end while

        read = clock() - read ;
        input.close() ;
        return true ;
    }
    bool opensortedfile(string fileName){// declaration: read all from a sortedfile
        fileID = fileName ;
        string filein = "sorted" + fileName + ".txt" ;
        string tmp ;
        struct jobType job ;

        ifstream input(filein.c_str()) ;
        if( !input ) { // confirm existence or not
            cout << endl << "### " << filein << " does not exist! ###" << endl;
            return false ;
        }
        input >> tmp >> tmp >> tmp >> tmp; // read the name of column
        while(!input.eof()){
            input >> job.OID >> job.arrival >> job.duration >> job.timeout ;
            if (!input.fail())
                aList.push_back( job );
        } // end while

        input.close() ;
        return true ;
    }

    void s(){ // showJob()
        showJob();
    }
    void Test(){
        showJob();
        sortByArrival();
        putAll();
        showTime();
    }
    
    void nextJobCheck(jobType & nextjob){ // declaration: take a look at the next job
        nextjob = aList[0];
    }
    void nextJob(jobType & nextjob){ // declaration: get & remove the next job
        nextJobCheck(nextjob) ;
        aList.erase(aList.begin());
    }
    
    void showAll(){ // declaration: display all on screen
        showAbortJobs() ;
        showDoneJobs() ;
        computeStat() ;
        showStat() ;
    }

    void addAbortJob(jobType& job, int cur, bool isFull, bool proccess, int index ){ // declaration: add one aborted job
        abortType abort;
        abort.OID = job.OID ;

        if( isFull && !proccess ){
            abort.Abort = job.arrival ;
            abort.Delay = 0;
        }
        else if ( proccess ){ 
            abort.CID = index;
            abort.Abort = job.timeout ;
            abort.Delay = abort.Abort - job.arrival ;
        }
        else if ( !proccess ){
            abort.CID = index;
            abort.Abort = cur ;
            abort.Delay = abort.Abort - job.arrival ;
        }
        abortJobs.push_back( abort );
    }
    void addDoneJob(jobType& job, int cur, int index ){ // declaration: add one done job
        doneType done;
        done.CID = index;
        done.OID = job.OID ;
        done.Departure = cur + job.duration ;
        done.Delay = cur - job.arrival ;
        doneJobs.push_back( done ) ;
    }

    void putAll(int a){ // declaration: write all as a file
        string fileout = "output"+fileID+".txt" ;
        ofstream sortedout( fileout.c_str() ) ;
        computeStat() ;
        
        if(!sortedout){
            cout << "### Cannot create output"<< fileID <<".txt ###\n" ;
            return ;
        }
        sortedout << "\t[Abort Jobs]\n" ;
        sortedout << "\tOID\tAbort\tDelay" << endl;
        for( int i = 1 ; i-1<abortJobs.size() ; i++ ){
            string n = "[" + to_string(i) + "]" ;
            sortedout << n << "\t" << abortJobs[i-1].OID << "\t" << abortJobs[i-1].Abort << "\t" << abortJobs[i-1].Delay << endl ;
        }

        sortedout << "\t[Jobs Done]\n" ;
        sortedout << "\tOID\tDeparture\tDelay" << endl;
        for( int i = 1 ; i-1<doneJobs.size() ; i++ ){
            string n = "[" + to_string(i) + "]" ;
            sortedout << n << "\t" << doneJobs[i-1].OID << "\t" << doneJobs[i-1].Departure << "\t" << doneJobs[i-1].Delay << endl ;
        }

        sortedout << fixed << "[Average Delay]\t" << setprecision(2) << double(avgDelay) << " ms" << endl ;
        sortedout << fixed << "[Success Rate]\t" << setprecision(2) << double(successRate) << " %" << endl ;
        sortedout.close() ;

        //showStat() ;
    }
    void putAll(string a){ // declaration: write all as a file
        string fileout = "double"+fileID+".txt" ;
        ofstream sortedout( fileout.c_str() ) ;
        computeStat() ;
        
        if(!sortedout){
            cout << "### Cannot create double"<< fileID <<".txt ###\n" ;
            return ;
        }
        sortedout << "\t[Abort Jobs]\n" ;
        sortedout << "\tOID\tCID\tAbort\tDelay" << endl;
        for( int i = 1 ; i-1<abortJobs.size() ; i++ ){
            string n = "[" + to_string(i) + "]" ;
            //int cid = stoi(doneJobs[i-1].CID)+1 ;
            sortedout << n << "\t" << abortJobs[i-1].OID << "\t" << abortJobs[i-1].CID << "\t" << abortJobs[i-1].Abort << "\t" << abortJobs[i-1].Delay << endl ;
        }

        sortedout << "\t[Jobs Done]\n" ;
        sortedout << "\tOID\tCID\tDeparture\tDelay" << endl;
        for( int i = 1 ; i-1<doneJobs.size() ; i++ ){
            string n = "[" + to_string(i) + "]" ;
            //int cid = stoi(doneJobs[i-1].CID)+1 ;
            sortedout << n << "\t" << doneJobs[i-1].OID << "\t" << doneJobs[i-1].CID << "\t" << doneJobs[i-1].Departure << "\t" << doneJobs[i-1].Delay << endl ;
        }

        sortedout << fixed << "[Average Delay]\t" << setprecision(2) << double(avgDelay) << " ms" << endl ;
        sortedout << fixed << "[Success Rate]\t" << setprecision(2) << double(successRate) << " %" << endl ;
        sortedout.close() ;

        //showStat() ;
    }




}; // end JobList

class JobQueue{
    jobQ *qFront, *qBack, *tmp; // head & tail of queue
    int qSize, qMax; // current size & max size of queue
    
public:
    int avail; // the time when CPU is available
    int curtime;
    JobQueue():qFront(nullptr), qBack(nullptr), qSize(0), qMax(0), avail(3), curtime(0){} // construtor of no-space queue
    JobQueue(int maxS):qFront(nullptr), qBack(nullptr), qSize(0), qMax(maxS), avail(maxS), curtime(0){} // construtor of an empty queue
    
    int length() const{ // get the current queue length
        return qSize;
    }
    bool isEmpty() const{ // check whether it is empty
        return qFront == nullptr ;
    }
    bool isFull() const{ // check whether it is full
        return qSize == qMax ;
    }
    void enQueue(jobType job){ // append a new element
        try{
            if ( isEmpty() ) {
                qFront = new jobQ();
                qFront->j = job ;
                qFront->next = qFront;
                qBack = qFront;
            } // end if 
            else{
                qBack->next = new jobQ();
                qBack = qBack->next;
                qBack->next = qFront;
                qBack->j = job ;
            } // end else
            qSize++ ;
        } // end try
        catch(std::bad_alloc& ba){ // unable to allocate space
            std::cerr << endl << "bad_alloc on queue caught: " << ba.what() << endl;
        } // end catch
    }
    void getFront(jobType &job){ // get the first element
        job = qFront->j;
    }
    void deQueue(){ // drop the first element
        if( !isEmpty() && qFront==qBack ){
            delete qFront ;
            qFront = nullptr;
            qBack = nullptr ;
            tmp = nullptr ;
            qSize--;
        }
        else if( !isEmpty() ){
            tmp = qFront;
            qFront = qFront->next;
            qBack->next = qFront;
            tmp->next = nullptr;
            delete tmp;
            tmp = nullptr;
            qSize--;
        } // end if
    }
    void deQueue(jobType &job){ // get & drop the first element
        getFront(job) ;
        deQueue() ;
    }
    void show(){
        tmp = qFront;
        cout << "curtime:" << curtime << endl ;
        if (!isEmpty()){
            do{
                cout<<"O:"<<tmp->j.OID<<" A:"<<tmp->j.arrival<<" D:"<<tmp->j.duration<<" T:"<<tmp->j.timeout<<" "<<endl ;
                tmp = tmp->next;
            }while ( tmp!=qBack );
            if( tmp==qBack ){
                cout<<"O:"<<tmp->j.OID<<" A:"<<tmp->j.arrival<<" D:"<<tmp->j.duration<<" T:"<<tmp->j.timeout<<" "<<endl ;
            }
        }
    }
    void clearQ(){ // clean up
        while (!isEmpty())
            deQueue();
    }
    ~JobQueue(){ // destrutor
        clearQ() ;
    } // end destrutor
    
}; // end JobQueue

void Simulation ( JobList job_list, int cpu, int max ){
    jobType tJob, qJob ; // temp
    //int max = 3 ;
    vector<JobQueue> queue(max, JobQueue(3)) ;
    //int curtime = 0 ;
    cout << "\nThe simulation is running...\n" ;
    for( int i=1 ; i<cpu ; i++ ){ // create not only one queue
        JobQueue q(max);
        queue.push_back(q);
    }

    job_list.nextJob(tJob) ;
    queue[0].curtime = tJob.arrival ;
    job_list.addDoneJob(tJob, queue[0].curtime, 0) ; // tJob.arrival+tJob.duration <= tJob.timeout
    queue[0].curtime = tJob.arrival + tJob.duration ; // OID
    
    while( !job_list.isEmpty() || !queue[0].isEmpty() ){
        job_list.nextJobCheck(tJob) ;

        while( (!job_list.isEmpty() && tJob.arrival < queue[0].curtime) ){
            job_list.nextJob(tJob) ;
            if( !queue[0].isFull() )
                queue[0].enQueue(tJob) ;
            else if( queue[0].isFull() ) // when quene isfull but a task is arrive
                job_list.addAbortJob(tJob, queue[0].curtime, queue[0].isFull(), false, 0 ) ;
            
            if( !job_list.isEmpty() )
                job_list.nextJobCheck(tJob) ;
        }
        
        bool updateTime = false ;// if done or processing, update current time to curtime+qJob.duration or qJob.overtime
        while( !updateTime && !queue[0].isEmpty() ){
            queue[0].getFront(qJob) ;
            if( qJob.timeout < queue[0].curtime ){ // preparing to deal it but find it overtime
                queue[0].deQueue() ;
                job_list.addAbortJob( qJob, queue[0].curtime, queue[0].isFull(), false, 0 ) ;
            }
            else if( queue[0].curtime+qJob.duration <= qJob.timeout ){ // current work done
                queue[0].deQueue() ;
                job_list.addDoneJob(qJob, queue[0].curtime, 0 ) ;
                queue[0].curtime= queue[0].curtime + qJob.duration ;
                updateTime = true ;
            }
            else{ // processsing and find it overtime
                  // curtime+qJob.duration > qJob.timeout
                queue[0].deQueue() ;
                queue[0].curtime = qJob.timeout ;
                job_list.addAbortJob( qJob, queue[0].curtime, queue[0].isFull(), true, 0 ) ;
                updateTime = true ;
            }
            if( queue[0].isEmpty() && !job_list.isEmpty() ){
                job_list.nextJobCheck(tJob) ;
                if( tJob.arrival >= queue[0].curtime && tJob.arrival+tJob.duration <= tJob.timeout ){
                    job_list.nextJob(tJob) ;
                    queue[0].enQueue(tJob) ;
                    queue[0].curtime = tJob.arrival ;
                    updateTime = false ;
                }
                
            }
        
        }
        
        if( updateTime == false && queue[0].isEmpty() && !job_list.isEmpty() ){
            job_list.nextJob(tJob) ;
            queue[0].curtime = tJob.arrival ;
            queue[0].enQueue(tJob) ;

            if( !job_list.isEmpty() )
                job_list.nextJobCheck(tJob);
            while( !job_list.isEmpty() && tJob.arrival==queue[0].curtime ){
                job_list.nextJob(tJob) ;
                if( !queue[0].isFull() )
                    queue[0].enQueue(tJob) ;
                else
                    job_list.addAbortJob( tJob, queue[0].curtime, queue[0].isFull(), false, 0 ) ;
            }
            queue[0].getFront(qJob) ;
        }
    }

    job_list.putAll(1);
}

void SQF( JobList &job_list, vector<JobQueue> &queue, jobType &tJob ){ // shortest queue first
    if( queue[0].isFull() && queue[1].isFull() )
        job_list.addAbortJob( tJob, queue[0].curtime, queue[0].isFull(), false, 0 ) ;
    else{ 
        if( queue[0].length() <= queue[1].length() ){
            queue[0].enQueue(tJob) ;// joblist add to queue[0] ;
            if( queue[0].curtime < tJob.arrival )
                queue[0].curtime = tJob.arrival ;
        }
        else{
            queue[1].enQueue(tJob) ;// joblist add to queue[1] ;
            if( queue[1].curtime < tJob.arrival )
                queue[1].curtime = tJob.arrival ;
        }
    }
}

void updateQ(JobList &job_list, vector<JobQueue> &q, jobType tJob, int index){
    if( !q[index].isFull() )
        q[index].enQueue( tJob ) ;
    else
        job_list.addAbortJob( tJob, q[index].curtime, q[index].isFull(), false, 0 ) ;
}

void deal(JobList &job_list, vector<JobQueue> &q, jobType tJob){
    bool avil = false ; // anyone is avalible?
    jobType qtemp ;

    int index = -1;

    if( q[0].curtime <= tJob.arrival ) // setting the processing cpu
        index = 0;
    else if( q[1].curtime <= tJob.arrival )
        index = 1;

    if(index!=-1){
        if(q[index].curtime < tJob.arrival){ 
            q[index].curtime = tJob.arrival;
            if( tJob.timeout < q[index].curtime ){ // preparing to deal it but find it overtime
                job_list.addAbortJob( tJob, q[index].curtime, q[index].isFull(), false, index+1 ) ;
            }
            else if( q[index].curtime+tJob.duration <= tJob.timeout ){ // current work done
                job_list.addDoneJob(tJob, q[index].curtime, index+1 ) ;
                q[index].curtime= q[index].curtime + tJob.duration ;
            }
            else{ // processsing and find it overtime
                  // curtime+qJob.duration > qJob.timeout
                q[index].curtime = tJob.timeout ;
                job_list.addAbortJob( tJob, q[index].curtime, q[index].isFull(), true, index+1 ) ;
            }
        }
        else
            q[index].enQueue(tJob);

        return;
    }
    

    index = -1;
    int len = -1; // find shortest queue
    if( q[0].length() <= q[1].length() ){
        len = q[0].length() ;
        index = 0 ; // setting the processing cpu
    }
    else{
        len = q[1].length() ;
        index = 1 ;
    }

    updateQ(job_list, q, tJob, index);
}

void doQ(JobList &job_list, vector<JobQueue> &q, jobType tJob){
    int n;
    jobType qJob;
    while( tJob.arrival >= q[0].curtime && !q[0].isEmpty() ){
        q[0].getFront(qJob) ;q[0].deQueue();
        if( q[0].curtime > qJob.timeout ){ // preparing to deal it but find it overtime
            job_list.addAbortJob(qJob, q[0].curtime, q[0].isFull(), false, 1);
        }
        else if( q[0].curtime+qJob.duration <= qJob.timeout ){ // current work done
            job_list.addDoneJob(qJob, q[0].curtime, 1) ;
            q[0].curtime = q[0].curtime + qJob.duration ;
        }
        else{   // processsing and find it overtime
                // curtime+qJob.duration > qJob.timeout
            q[0].curtime = qJob.timeout ;
            job_list.addAbortJob( qJob, q[0].curtime, q[0].isFull(), true, 1);
        }
    }

    while( tJob.arrival >= q[1].curtime && !q[1].isEmpty() ){
        q[1].getFront(qJob) ;

        if( q[1].curtime > qJob.timeout ){ // preparing to deal it but find it overtime
            q[1].deQueue();
            job_list.addAbortJob(qJob, q[1].curtime, q[1].isFull(), false, 2);
        }
        else if( q[1].curtime+qJob.duration <= qJob.timeout ){ // current work done
            q[1].deQueue();
            job_list.addDoneJob(qJob, q[1].curtime, 2) ;
            q[1].curtime = q[1].curtime + qJob.duration ;

        }
        else{   // processsing and find it overtime
                // curtime+qJob.duration > qJob.timeout
            q[1].deQueue();
            q[1].curtime = qJob.timeout ;
            job_list.addAbortJob( qJob, q[1].curtime, q[1].isFull(), true, 2);
        }
    }
}

void finishQ( JobList &job_list, vector<JobQueue> &q ){
    jobType tJob;
    int index = 0;
    bool finish = false ;
    while( !finish ){ 
        index = -1;
        int tmpTime = 0;
        if( !q[0].isEmpty() && !q[1].isEmpty() ){ // setting the processing cpu
            if( q[0].curtime <= q[1].curtime )
                index = 0 ;
            else
                index = 1 ;
        }
        if( !q[0].isEmpty() && q[1].isEmpty() ) // setting the processing cpu
                index = 0 ;
        
        if( q[0].isEmpty() && !q[1].isEmpty() ) // setting the processing cpu
                index = 1 ;
        
        if( index == -1 ) 
            finish = true ;
        
        if( !finish ){
            q[index].getFront(tJob);
        
            q[index].deQueue();
            if(q[index].curtime < tJob.arrival){
                q[index].curtime = tJob.arrival;
            }
            if( tJob.timeout < q[index].curtime ){ // preparing to deal it but find it overtime
               
                job_list.addAbortJob( tJob, q[index].curtime, q[index].isFull(), false, index+1 ) ;
            }
            else if( q[index].curtime+tJob.duration <= tJob.timeout ){ // current work done
                
                job_list.addDoneJob(tJob, q[index].curtime, index+1 ) ;
                q[index].curtime= q[index].curtime + tJob.duration ;
                //updateTime = true ;
            }
            else{ // processsing and find it overtime
                  // curtime+qJob.duration > qJob.timeout
                q[index].curtime = tJob.timeout ;
                job_list.addAbortJob( tJob, q[index].curtime, q[index].isFull(), true, index+1 ) ;
            }
        }
    }

}

void SSSimulation ( JobList &job_list, int cpu, int max ){
    jobType tJob, qJob, next ; // temp
    vector<JobQueue> queue(cpu, JobQueue(3)) ; // create not only one queue
    int index = 0, count1, count2 ; // index of cpu
    cout << "\nThe simulation is running...\n" ;
    queue[index].curtime = 0 ;
    queue[index+1].curtime = 0 ;

    while( !job_list.isEmpty() && index < cpu ){ // initial the time of each cpu
        job_list.nextJob(tJob) ;
        queue[index].curtime = tJob.arrival ;
        job_list.addDoneJob(tJob, queue[index].curtime, index+1) ; // tJob.arrival+tJob.duration <= tJob.timeout
        queue[index].curtime = tJob.arrival + tJob.duration ; // OID
        //job_list.nextJob(tJob) ;
        index++;
    }

    index = 0 ; 
    while( !job_list.isEmpty() ){
        job_list.nextJob(tJob);
        doQ(job_list, queue, tJob);
        deal(job_list, queue, tJob) ;
    }

    finishQ( job_list, queue ) ;
    string f ;
    job_list.putAll(f);
}

int main(){
    string command = "0" ;
    string fileName ;
    while( true ){
        JobList job_list ; // task 1
		cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
		cout << endl << "* 0. Quit                            *" ;
		cout << endl << "* 1. Sort a file                     *" ;
		cout << endl << "* 2. Simulate one FIFO queue         *" ;
        cout << endl << "* 3. Simulate one FIFO SQF         *            " ;
		cout << endl << "**************************************" ;
		cout << endl << "Input a command(0, 1, 2, 3): " ;
        cin >> command ;
		if ( command.compare( "0" ) == 0 )
			break ;
        else if( command.compare( "1" ) == 0 || command.compare( "2" ) == 0 ) {
            if( command.compare( "1" ) == 0 ){
                cout << endl << "Input a file number: " ;
                cin >> fileName ;
                if ( job_list.getAll( fileName ) ){
                    job_list.Test();
                    cout << endl << "See sorted" << fileName <<".txt" << endl ;
                }
            }
            else{
                if ( fileName.size() == 0 ){
                    cout << endl << "Input a file number: " ;//### sorted304.txt does not exist! ###
                    cin >> fileName ;
                }
                if( job_list.opensortedfile(fileName) ){
                    Simulation ( job_list, 1, 3 ) ;
                    cout << "See output" << fileName <<".txt" << endl ;
                }
            }
        }
        else if( command.compare( "3" ) == 0 ) { //*******************************************************************************************************
            if ( fileName.size() == 0 ){
                cout << endl << "Input a file number: " ;
                cin >> fileName ;
            }
            if( job_list.opensortedfile(fileName) ){
                SSSimulation ( job_list, 2, 3 ) ;
                cout << "See double" << fileName <<".txt" << endl ;
            }
        }
        else
			cout<< endl << "Command does not exist!" << endl ;
    }

} // main()