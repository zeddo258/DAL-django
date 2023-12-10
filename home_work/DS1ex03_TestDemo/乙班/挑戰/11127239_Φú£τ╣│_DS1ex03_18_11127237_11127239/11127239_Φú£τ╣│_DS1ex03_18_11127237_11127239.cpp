// 第18組, 11127237, 11127237
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>


using namespace std;


#ifndef _jobList_HPP
#define _jobList_HPP
typedef struct jT {
    int OID;
    int arrival;
    int duration;
    int timeout;
    jT *next;
} jobType;

class jobList {
    vector<jobType> aList;
    string filelD;

    void reset() {
        aList.clear();
    }

    void sortByArrival() {
        int n = aList.size();

        // 選擇初始間隔（gap）為數量的一半，然後逐漸減少間隔
        for (int gap = n / 2; gap > 0; gap /= 2) {
            // 使用插入排序在每個間隔上進行排序
            for (int i = gap; i < n; i++) {
                // 從間隔的位置開始，進行比較並插入到正確的位置
                jobType temp = aList[i];
                int j;

                for (j = i; j >= gap && (aList[j - gap].arrival > temp.arrival || 
                                        (aList[j - gap].arrival == temp.arrival && aList[j - gap].OID > temp.OID));
                                        j -= gap) {
                    aList[j] = aList[j - gap];
                }
                aList[j] = temp;
            }
        }
    }

    void putAll() {
        int j = 0 ;
        ofstream outFile("sorted"+filelD+".txt");
        outFile << "OID\tarrival\tduration\ttimeout\n";
        while(j < aList.size()){
            outFile << aList[j].OID << "\t" << aList[j].arrival << "\t" <<
                    aList[j].duration << "\t" << aList[j].timeout << "\n";
            j++ ;
        }
    }

    void showjob( ) {
        int j = 0 ;
        cout << "\n\tOID\tarrival\tduration\ttimeout\n" ;
        while( j < aList.size() ){
            cout << "("<< j + 1 << ")" << "\t" <<aList[j].OID << "\t" << aList[j].arrival << "\t" <<
                                             aList[j].duration << "\t" << aList[j].timeout << "\n";
            j++ ;
        }       
    }

public:
    jobList() { reset(); }
    ~jobList() { reset(); }
    bool isEmpty() { 
       if ( aList.empty() ) return true;
       else return false;
    }

    void getFile_num( string &num ) {
        num = filelD;
    }

    bool getAll(string file_name, int i) {
        if (!aList.empty())
            aList.clear();
        string title;
        string file = file_name + filelD + ".txt";
        ifstream inputfile(file);
        if ( !inputfile.is_open() ) return false;
        inputfile >> title >> title >> title >> title;
        jobType h;
        while ( inputfile >> h.OID >> h.arrival >> h.duration >> h.timeout ) {
            aList.push_back(h);
        }
        cout << "The simulation is running..." << endl;
        if ( i == 2) cout << "See output" + filelD +".txt" << endl;
        else cout << "See double" + filelD +".txt" << endl;
        inputfile.close();
        return true;
    }

    bool getSorted3(int &n){
        if (!aList.empty())
            aList.clear();
        string title;
        cout << endl << "Input a file number:"; 
        cin >> filelD;
        cout << endl << "How many CPU:"; 
        cin >> n;
        string file = "sorted" + filelD + ".txt"; 
      
        ifstream inputfile(file);
        if ( !inputfile.is_open() ) return false; 
        inputfile >> title >> title >> title >> title;
        jobType h;
        while ( inputfile >> h.OID >> h.arrival >> h.duration >> h.timeout ) {
            aList.push_back(h);
        }
        inputfile.close();
        return true;

    }

    bool getSorted2(){
        if (!aList.empty())
            aList.clear();
        string title;
        cout << endl << "Input a file number:"; 
        cin >> filelD;
        string file = "sorted" + filelD + ".txt"; 
      
        ifstream inputfile(file);
        if ( !inputfile.is_open() ) return false; 
        inputfile >> title >> title >> title >> title;
        jobType h;
        while ( inputfile >> h.OID >> h.arrival >> h.duration >> h.timeout ) {
            aList.push_back(h);
        }
        inputfile.close();
        return true;

    }

    bool getSorted() {
        if (!aList.empty())
            aList.clear();
        string title;
        cout << endl << "Input a file number:"; 
        cin >> filelD;
        string file = "input" + filelD + ".txt"; 
      
        auto start_time = chrono::high_resolution_clock::now(); // 記錄開始時間
        ifstream inputfile(file);
        if ( !inputfile.is_open() ) return false; 
        inputfile >> title >> title >> title >> title;
        jobType h;
        while ( inputfile >> h.OID >> h.arrival >> h.duration >> h.timeout ) {
            aList.push_back(h);
        }
        auto end_time = chrono::high_resolution_clock::now(); // 記錄結束時間
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // 計算執行時間
        showjob();
        cout << "Reading data: " << duration.count() << " ms\n";


        auto Sorting_start_time = chrono::high_resolution_clock::now(); // 記錄開始時間
        sortByArrival();
        auto Sorting_end_time = chrono::high_resolution_clock::now(); // 記錄結束時間
        auto Sorting_duration = chrono::duration_cast<chrono::microseconds>(Sorting_end_time - Sorting_start_time); // 計算執行時間
        cout << "Sorting data: " << Sorting_duration.count() << " ms\n";;  

        auto Writing_start_time = chrono::high_resolution_clock::now(); // 記錄開始時間                 
        putAll();
        auto Writing_end_time = chrono::high_resolution_clock::now(); // 記錄結束時間
        auto Writing_duration = chrono::duration_cast<chrono::microseconds>(Writing_end_time - Writing_start_time); // 計算執行時間
        cout << "Writing data: " << Writing_duration.count() << " ms\n";

        cout << "\nSee.sorted" + filelD + ".txt\n";
        inputfile.close();
        return true;
    }

    void nextjob_del(jobType &job) {
        job.arrival = aList[0].arrival;
        job.duration = aList[0].duration ;
        job.OID = aList[0].OID;
        job.timeout = aList[0].timeout ;
        aList.erase(aList.begin() ); // 刪除第一位

    }
    void nextjob(jobType &job) {
        job.arrival = aList[0].arrival;
        job.duration = aList[0].duration ;
        job.OID = aList[0].OID;
        job.timeout = aList[0].timeout ;

    }
};

#endif

#ifndef _jobQueue_HPP
#define _jobQueue_HPP

template<typename T>
class jobQueue {
    T *cA;
    int qFront, qBack;
    int qsize, qMax;

    int next(int idx) { return 0 ;}

public:
    int avail;
    int CPU_number;

    jobQueue():cA(NULL), qFront(0), qBack(0),
                qsize(0), qMax(3), avail(0) {}

    int length() const {
        return qsize;
    }

    int getMax() {
        return qMax;
    }

    bool isEmpty() const {
        if ( cA == NULL ) return true;
        else return false;
    } 

    bool isFull() const {
        if ( qsize == qMax ) return true;
        else return false;
    }

    void enQueue( T & thing) {
        if ( cA == NULL ) {
            cA = new T;
            *cA = thing;     
            cA -> next = cA; 
            qsize = qsize + 1; 
        }

        else {
            T *temp = cA;
            cA = new T;
            *cA = thing;
            cA -> next = temp -> next;
            temp -> next = cA;
            qsize = qsize + 1; 
        }
    }

    void deQueue( T & thing ) {
        T *temp = cA -> next;
        thing = *temp;
        if ( cA == cA -> next ) {
            delete cA;
            cA = NULL;
            qsize = qsize - 1; 
        }
        else {
            cA -> next = temp -> next;
            temp -> next = NULL;
            delete temp;
            qsize = qsize - 1; 
        }
        temp = NULL;
    }

    void get(T & g) {
        g = *cA;
    }

    ~jobQueue() {
        delete cA;
        cA = NULL;
    }

};

#endif

# ifndef _AnsList_HPP
# define _AnsList_HPP
#include <vector>
class AnsList{

    typedef struct  aT{
        int Abort;
        int OID;
        int Deley;
        int CID;
    } abortType;

    typedef struct  dT{
        int OID;
        int Departure;
        int Deley;
        int CID;
    } doneType;

    vector<abortType> abortJobs ;
    vector<doneType> donejobs ;
    float avgDelay ;
    float successRate ;

public:
    AnsList() : avgDelay(0.0), successRate(0.0){ }
    ~AnsList(){
        vector<abortType>().swap(abortJobs);
        vector<doneType>().swap(donejobs);
    }

    void addAbortJob( int OID, int CPU_Num ,int Abort , int Deley){
        abortType temp;

        temp.OID = OID;
        temp.Abort = Abort;
        temp.Deley = Deley;
        temp.CID = CPU_Num;

        abortJobs.push_back(temp);
    }
    void addDoneJob( int OID, int CPU_Num ,int Departure, int Deley ){
        doneType temp;

        temp.OID = OID;
        temp.Departure = Departure;
        temp.Deley = Deley;
        temp.CID = CPU_Num;

        donejobs.push_back(temp);
    }

    void putAll(string filelD_naber) {
        int j = 0 ;
        float math_delay = 0 ;
        ofstream outFile("output"+filelD_naber+".txt");
        outFile << "\t[Abort Jobs]\n" ;
        outFile << "\tOID\tAbort\tDelay\n";
        while(j < abortJobs.size()){
            outFile << "["<< j+1 << "]\t" <<abortJobs[j].OID << "\t" <<
                       abortJobs[j].Abort << "\t" << abortJobs[j].Deley << "\n" ;
            
            if ( abortJobs[j].Deley != 0 ){math_delay = math_delay + abortJobs[j].Deley ; }
            j ++ ;
        }
        outFile << "\t[Jobs Done]\n" ;
        outFile << "\tOID\tDeparture\tDelay\n";

        j = 0 ;
        while(j < donejobs.size()){
            outFile << "["<< j+1 << "]\t" <<donejobs[j].OID << "\t" <<
                       donejobs[j].Departure << "\t" << donejobs[j].Deley << "\n" ;
            if ( donejobs[j].Deley != 0 ){math_delay = math_delay + donejobs[j].Deley ; }
            j ++ ;
        }
        outFile << "[Average Delay]\t" << fixed << setprecision(2) << 
                (float)math_delay/(abortJobs.size()+donejobs.size()) << " ms\n" ;

        outFile << "[Success Rate]\t"<< fixed << setprecision(2) << 
                100*(float)donejobs.size()/(abortJobs.size()+donejobs.size()) <<" %\n" ;
    }

    void putAll3(string filelD_naber) {
        int j = 0 ;
        float math_delay = 0 ;
        ofstream outFile("double"+filelD_naber+".txt");
        outFile << "\t[Abort Jobs]\n" ;
        outFile << "\tOID\tCID\tAbort\tDelay\n";
        while(j < abortJobs.size()){
            outFile << "["<< j+1 << "]\t" <<abortJobs[j].OID << "\t" << abortJobs[j].CID << "\t" <<
                       abortJobs[j].Abort << "\t" << abortJobs[j].Deley << "\n" ;
            
            if ( abortJobs[j].Deley != 0 ){math_delay = math_delay + abortJobs[j].Deley ; }
            j ++ ;
        }
        outFile << "\t[Jobs Done]\n" ;
        outFile << "\tOID\tCID\tDeparture\tDelay\n";

        j = 0 ;
        while(j < donejobs.size()){
            outFile << "["<< j+1 << "]\t" <<donejobs[j].OID << "\t" << donejobs[j].CID << "\t" <<
                       donejobs[j].Departure << "\t" << donejobs[j].Deley << "\n" ;
            if ( donejobs[j].Deley != 0 ){math_delay = math_delay + donejobs[j].Deley ; }
            j ++ ;
        }
        outFile << "[Average Delay]\t" << fixed << setprecision(2) << 
                (float)math_delay/(abortJobs.size()+donejobs.size()) << " ms\n" ;

        outFile << "[Success Rate]\t"<< fixed << setprecision(2) << 
                100*(float)donejobs.size()/(abortJobs.size()+donejobs.size()) <<" %\n" ;
    }

};

# endif  // _AnsList_HPP

#ifndef _Simulation_HPP
#define _Simulation_HPP

using namespace std;

class Simulation {
    jobList jobs;
    AnsList answers;
    jobQueue<jobType> **allQ;
    int qNum;

    void shorTestQ( int &shortQ, int k ) {
        int smallQ = k;
        for ( int i = k + 1; i < qNum; i++) {
            if ( !allQ[i]->isFull() ) {
                if ( allQ[i]->length() < allQ[smallQ]->length()  ) {
                    smallQ = i;
                }

                else if ( allQ[i]->length() == allQ[smallQ]->length()  ){
                    if ( allQ[i]->CPU_number < allQ[smallQ]->CPU_number) {
                        smallQ = i;
                    }
                }
            }
        }

        shortQ = smallQ;
    }


public:
    Simulation(jobList aList, int N): jobs(aList), qNum(N) {
        allQ = new jobQueue<jobType>*[qNum];
        for ( int i = 0; i < qNum; ++i ) {
            allQ[i] = new jobQueue<jobType>();
            allQ[i] -> CPU_number = i + 1;
        }
    }

    ~Simulation() {
        for (int i = 0; i < qNum; i++) {
            delete allQ[i];
            allQ[i] = nullptr; 
        }
        delete[] allQ;
        allQ = nullptr;

    }

    void SQF() {
        int Abort = 0;
        int OID = 0;
        int Deley = 0;
        int Departure = 0;
        jobType job;
        jobType jqJob;
        bool isDone = false;
        bool TRUE = false;

        while ( !jobs.isEmpty() ) {
            // 取出工作
            jobs.nextjob_del(job);

            // 判斷第一個閒置的CPU
            int j = 0;
            for ( int i = 0; i < qNum; i++ ) {
                if ( job.arrival >= allQ[i] -> avail ) {
                    TRUE = true;
                    j = i;
                    break;
                } // if(job.arrival >= allQ[i] -> avail)
            } // for()

            /**/
            // 往後判斷是否也有閒置並且時間小於第一個
            for (int i = j + 1; i < qNum; i++ ) {
                if ( allQ[i] -> avail != 0  && job.arrival >= allQ[i] ->avail ) {
                    if ( allQ[i]->avail < allQ[j]->avail) {
                        j = i;
                    }
                }
            }
            

            if ( TRUE ) {
                int i = j;
                // 佇列為空
                if (allQ[i] -> isEmpty()) {
                    allQ[i] -> avail = job.arrival + job.duration;
                    Departure = allQ[i] -> avail;
                    answers.addDoneJob(job.OID, allQ[i]->CPU_number ,Departure,0);
                    isDone = true;
                }
                // 佇列不為空
                else {
                    while ( !allQ[i]->isEmpty() ) {
                        allQ[i] -> deQueue(jqJob);
                        if ( allQ[i]->avail + jqJob.duration <= jqJob.timeout ) {
                            Deley = allQ[i]->avail - jqJob.arrival;
                            allQ[i]->avail = allQ[i]->avail + jqJob.duration;
                            Departure = allQ[i]->avail;
                            answers.addDoneJob(jqJob.OID,allQ[i]->CPU_number,Departure,Deley);
                            if ( job.arrival < allQ[i]->avail  )
                                break;
                        }

                        else {
                            if ( jqJob.timeout <= allQ[i]->avail ) {
                                Abort = allQ[i]->avail;
                                Deley = Abort - jqJob.arrival;
                                answers.addAbortJob(jqJob.OID,allQ[i]->CPU_number,Abort,Deley);                                       
                            }

                            else {
                                Abort = jqJob.timeout;
                                allQ[i]->avail = Abort;
                                Deley = Abort - jqJob.arrival;
                                answers.addAbortJob(jqJob.OID,allQ[i]->CPU_number,Abort,Deley);
                            } // else
                        } // else                          
                    } // while(!allQ[i]->isempty())

                    if ( allQ[i]->isEmpty() ) {

                        if ( allQ[i]->avail + job.duration <= job.timeout ) {
                            if ( job.arrival < allQ[i]->avail ) 
                                Deley = allQ[i]->avail - job.arrival;
                            else Deley = 0;     

                            allQ[i] -> avail = job.arrival + job.duration;
                            Departure = allQ[i] -> avail;
                            answers.addDoneJob(job.OID, allQ[i]->CPU_number ,Departure,Deley);
                            isDone = true; 
                        }

                        else {
                            
                            if(job.arrival < allQ[i]->avail && job.timeout < allQ[i]->avail ){
                                Deley = allQ[i]->avail - job.arrival;
                                Abort = allQ[i]->avail;
                                answers.addAbortJob(job.OID,allQ[i]->CPU_number,Abort,Deley);
                                isDone = true;
                            }
                            else{
                                allQ[i]->avail = job.timeout;
                                Deley = allQ[i]->avail - job.arrival;
                                Abort = allQ[i]->avail;
                                answers.addAbortJob(job.OID,allQ[i]->CPU_number,Abort,Deley);
                                isDone = true;
                            }
                        }
                        
                    }
                } // else
            } // if( TRUE)
            // 工作未處理

            if ( !isDone ) {
                bool QisExit = false;
                int smallQ = 0;
                int k = 0;
                // 判斷是否至少有一個未滿佇列存在
                for ( int j = 0; j < qNum; j++ ) {
                    if ( !allQ[j] -> isFull() ) {
                        QisExit = true;
                        break;
                    }
                }

                // 至少有一條佇列未滿
                if (QisExit) {
                    // 一個CPU不用判斷
                    if ( qNum != 1 ) 
                        shorTestQ(smallQ,k ); // 最短Q且如果重複為最小編號者    
                    allQ[smallQ]->enQueue(job);
                }

                // 佇列全滿
                else {
                    Abort = job.arrival;
                    answers.addAbortJob(job.OID,0,Abort,0);
                }
                
            }

            isDone = false;
            TRUE = false;
        } // while(!jobs.isEmpty())

        for ( int k = 0; k < qNum; k++ ) {
            while ( !allQ[k]->isEmpty() ) {
                allQ[k] -> deQueue(jqJob);
                if ( allQ[k]->avail + jqJob.duration <= jqJob.timeout ) {
                    Deley = allQ[k]->avail - jqJob.arrival;
                    allQ[k]->avail = allQ[k]->avail + jqJob.duration;
                    Departure = allQ[k]->avail;
                    answers.addDoneJob(jqJob.OID,allQ[k]->CPU_number,Departure,Deley);
                }

                else {
                    if ( jqJob.timeout <= allQ[k]->avail ) {
                        Abort = allQ[k]->avail;
                        Deley = Abort - jqJob.arrival;
                        answers.addAbortJob(jqJob.OID,allQ[k]->CPU_number,Abort,Deley);                                       
                    }

                    else {
                        Abort = jqJob.timeout;
                        allQ[k]->avail = Abort;
                        Deley = Abort - jqJob.arrival;
                        answers.addAbortJob(jqJob.OID,allQ[k]->CPU_number,Abort,Deley);
                    } // else
                } // else                          
            } // while(!allQ[i]->isempty())
        }

        if ( qNum == 1 ) {
            string num;
            jobs.getFile_num(num);
            answers.putAll(num);           
        }

        else {
            string num;
            jobs.getFile_num(num);
            answers.putAll3(num);            
        }

    }
};


#endif

int main() {
    int command = 0;
    int n = -1 ; // 有多少個CPU
    bool oneTF = false ;
    jobList job;
    do {
        cout << "\n**** Simulate FIFO Queues by SQF *****\n";
        cout << "* 0. Quit                            *\n";
        cout << "* 1. Sort a file                     *\n";
        cout << "* 2. Simulate one FIFO queue         *\n";
        cout << "* 3. Simulate two queues by SQF      *\n";
        cout << "* 4. more queues                     *\n";
        cout << "**************************************\n";
        cout << "Input a command(0, 1, 2, 3, 4):";
        cin >> command;

        switch (command){
            case 0 : break;

            case 1 : 
                    oneTF = false ;
                    if ( !job.getSorted() ) {
                        string st;
                        job.getFile_num(st);
                        cout <<"### input"+ st + ".txt does not exist! ###\n";
                    }
                    else{
                        oneTF = true ;
                    }
                    
                    break;

            case 2 : 
                    if( oneTF == false ){
                        job.getSorted2();
                    }

                    if ( job.getAll("sorted", 2)) {
                        Simulation SIM(job, 1) ;
                        SIM.SQF();
                    }
                    // oneTF = true ;
                    break;

            case 3 :
                    if( oneTF == false ){
                        job.getSorted2();
                    }

                    if ( job.getAll("sorted", 3)) {
                        Simulation SIM(job, 2) ;
                        SIM.SQF();
                    }
                    // oneTF = true ;
                    break;    
            case 4 :
                    
                    if( oneTF == false ){
                        job.getSorted3(n);
                    }

                    if ( job.getAll("sorted", 3)) {
                        Simulation SIM(job, n) ;
                        SIM.SQF();
                    }
                    // oneTF = true ;
                    break;                  

            default : cout << endl << "Command does not exist!" << endl;
        }
         
    }while (command != 0);

    return 0 ;

}