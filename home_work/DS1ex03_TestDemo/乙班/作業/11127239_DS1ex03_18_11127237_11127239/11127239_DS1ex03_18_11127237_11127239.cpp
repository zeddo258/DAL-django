/*第18組, 學號：11127237_11127239*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std ;

#ifndef _jobList_HPP
#define _jobList_HPP
typedef struct jT {
    int OID;
    int arrival;
    int duration;
    int timeout;
} jobType;

class jobList {
    vector<jobType> aList;
    string filelD;
    
    void reset() {
        vector<jobType>().swap(aList);
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

public:
    jobList() { reset(); }
    ~jobList() { reset(); }
    bool isEmpty() { 
       if ( aList.empty() ) return true;
       else return false;
    }

    bool getAll(string file_number) {
        string title;
        filelD = file_number;
        file_number = "input" + file_number + ".txt";
        ifstream inputfile(file_number);
        if ( !inputfile.is_open() ) return false;

        auto start_time = chrono::high_resolution_clock::now(); // 記錄開始時間
        inputfile >> title >> title >> title >> title;
        jobType h;
        while ( inputfile >> h.OID >> h.arrival >> h.duration >> h.timeout ) {
            aList.push_back(h);
        }
        auto end_time = chrono::high_resolution_clock::now(); // 記錄結束時間
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // 計算執行時間

        showjob();
        cout << "Reading data: " << duration.count() << " ms\n";;
        inputfile.close();
        return true;
    }

    bool getSortAll(string file_number) {
        
        string title;
        filelD = file_number;
        file_number = "sorted" + file_number + ".txt";
        ifstream inputfile(file_number);
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
        auto start_time = chrono::high_resolution_clock::now(); // 記錄開始時間
        sortByArrival();
        auto end_time = chrono::high_resolution_clock::now(); // 記錄結束時間
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // 計算執行時間
        cout << "Sorting data: " << duration.count() << " ms\n";;  

        auto Writing_start_time = chrono::high_resolution_clock::now(); // 記錄開始時間                 
        putAll();
        auto Writing_end_time = chrono::high_resolution_clock::now(); // 記錄結束時間
        auto Writing_duration = chrono::duration_cast<chrono::microseconds>(Writing_end_time - Writing_start_time); // 計算執行時間
        cout << "Writing data: " << Writing_duration.count() << " ms\n";

        cout << "\nSee.sorted" + filelD + ".txt\n";
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

#endif //_jobList_HPP


#ifndef _jobQueue_HPP
#define _jobQueue_HPP
typedef struct jQ {
    int arrival;
    int OID;
    int duration;
    int timeout;
    jQ *next;
} jobQ;


class jobQueue {

    jobQ *qFront = NULL;
    jobQ *qBack = NULL;
    int next(int idx) {return 0;}

public:
    int avail = 0;
    int qSize = 0, qMax = 3;
    bool isEmpty() const { 
        if ( qFront == NULL ) return true;
        else return false;
    }

    void enQueue( jobType job)  {

        if ( qFront == NULL ) {
            qFront = new jobQ;
            qFront -> next = NULL; 
            qFront -> arrival = job.arrival;
            qFront -> duration = job.duration;
            qFront -> OID = job.OID;
            qFront -> timeout = job.timeout;
            qBack = qFront;
        }

        else {
            qBack -> next = new jobQ;
            qBack = qBack -> next;
            qBack -> next = NULL;
            qBack -> arrival = job.arrival;
            qBack -> duration = job.duration;
            qBack -> OID = job.OID;
            qBack -> timeout = job.timeout;
        }   
         
        qSize++;
    }

    void getFront( jobType &j_t) {
        j_t.OID = qFront -> OID;
        j_t.arrival = qFront -> arrival ;
        j_t.duration = qFront -> duration ;
        j_t.timeout = qFront -> timeout ;
    }
    void deQueue() {
        jobQ *newhead = qFront ;
        qFront = qFront -> next ;
        qSize--;
        delete newhead ;
    }

    void clearQ() {
        jobQ *newhead = qFront ;
        while( newhead != NULL ) {
            qFront = qFront -> next ; 
            newhead -> next = NULL;
            delete newhead ;
            newhead = qFront ;
        }

        delete newhead ;
        delete qFront ;
        avail = 0;
        qSize = 0;
    }

    ~jobQueue()
    { clearQ(); }                
};

#endif //_joQueue_HPP


# ifndef _AnsList_HPP
# define _AnsList_HPP
class AnsList{

    typedef struct  aT{
        int Abort;
        int OID;
        int Deley;
    } abortType;

    typedef struct  dT{
        int OID;
        int Departure;
        int Deley;
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

    void addAbortJob( int OID, int Abort , int Deley){
        abortType temp;

        temp.OID = OID;
        temp.Abort = Abort;
        temp.Deley = Deley;

        abortJobs.push_back(temp);
    }
    void addDoneJob( int OID, int Departure, int Deley ){
        doneType temp;

        temp.OID = OID;
        temp.Departure = Departure;
        temp.Deley = Deley;

        donejobs.push_back(temp);
    }

    // 這裡還要排序!!!!!!

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

};

# endif  // _AnsList_HPP


int main() {
    jobList JL; 
    jobQueue JQ;
    AnsList AL;
    string filelD_naber; // 檔案名稱(數字部分)
    bool onetf = false ; // 是否有做過任務一
    int command = 0;
    

    do {
        int i = 0;
        cout << "\n**** Simulate FIFO Queues by SQF *****\n";
        cout << "* 0. Quit                            *\n";
        cout << "* 1. Sort a file                     *\n";
        cout << "* 2. Simulate one FIFO queue         *\n";
        cout << "**************************************\n";
        cout << "Input a command(0, 1, 2):";
        cin >> command;

        if ((command == 1) || (command == 2)) {

            if ( command == 1 ) {
                cout << endl << "Input a file number:";
                cin >> filelD_naber;
                if (JL.getAll(filelD_naber) == true) { // 讀資料
                    JL.getSorted(); // 排序 
                    onetf = true ;
                    JL.~jobList(); 
                }
                else{
                    cout << "### input" + filelD_naber + ".txt does not exist! ###\n" ;
                }
            }

            else {

                bool CPU_isDone = true; // 判斷是否處理資料
                jobType jb; // 抓取新資料
                jobType jq_vilue; // 抓取佇列第一筆資料
                int Departure = 0;
                int Abort = 0;
                int deley = 0;

                // 有先做完任務一
                if ( onetf == true ){
                    JL.getSortAll(filelD_naber);
                }
                else{
                    cout << endl << "Input a file number:";
                    cin >> filelD_naber;
                    if (JL.getSortAll(filelD_naber) != true){
                        cout << "### sorted" + filelD_naber + ".txt does not exist! ###\n" ;
                        continue;
                    }
                }

                while(JL.isEmpty() == false){

                    while( CPU_isDone == true && JL.isEmpty() == false ){
                        // 佇列裡面無人排隊
                        if (JQ.isEmpty() == true){
                            JL.nextjob_del(jb);
                            JQ.enQueue(jb);
                        }
                        // 拿出佇列裡第一筆資料
                        JQ.getFront(jq_vilue) ;

                        // 處理第一筆資料的時間
                        if ( JQ.avail < jq_vilue.arrival ) 
                            JQ.avail = jq_vilue.arrival;

                        // 判斷此資料須abort || done
                        // 要記時間(JQ.avail)
                        if (JQ.avail >= jq_vilue.timeout){

                            // JQ.avail = jq_vilue.timeout ;
                            deley = JQ.avail - jq_vilue.arrival ;
                            AL.addAbortJob(jq_vilue.OID, JQ.avail, deley );

                        }
                        else if (JQ.avail + jq_vilue.duration > jq_vilue.timeout){

                            JQ.avail = jq_vilue.timeout ;
                            deley = JQ.avail - jq_vilue.arrival ;
                            AL.addAbortJob(jq_vilue.OID, JQ.avail, deley );

                        }
                        else{
                            deley = JQ.avail - jq_vilue.arrival ;
                            JQ.avail = JQ.avail + jq_vilue.duration;
                            AL.addDoneJob(jq_vilue.OID, JQ.avail, deley );
                        }

                        JQ.deQueue();

                        // 先看下一筆資料是否抵達
                        if (JL.isEmpty() == false){
                            JL.nextjob(jb); // 先偷看，不刪除
                            if( jb.arrival < JQ.avail ) {
                                CPU_isDone = false ; // 處理佇列
                            }
                        }

                    } // while

                    while( CPU_isDone == false && JL.isEmpty() == false ){
                        JL.nextjob_del(jb);
                        // 佇列未滿
                        if( JQ.qSize < JQ.qMax ){
                            JQ.enQueue(jb);
                        }
                        //佇列滿了
                        else{AL.addAbortJob(jb.OID, jb.arrival, 0 );}

                        // 先看下一筆資料是否抵達
                        if (JL.isEmpty() == false){
                            JL.nextjob(jb); // 先偷看，不刪除
                            if( jb.arrival >= JQ.avail ){
                                CPU_isDone = true ; // 處理CPU
                            }
                        }
                    } // while

                } // while
                
                // 將剩下的佇列處理完
                while( JQ.isEmpty() != true ){
                    // 拿出佇列裡第一筆資料
                    JQ.getFront(jq_vilue) ;

                    if ( JQ.avail < jq_vilue.arrival ) 
                        JQ.avail = jq_vilue.arrival;

                    // 判斷此資料須abort || done
                    // 要記時間(JQ.avail)
                    if (JQ.avail >= jq_vilue.timeout){

                        // JQ.avail = jq_vilue.timeout ;
                        deley = JQ.avail - jq_vilue.arrival ;
                        AL.addAbortJob(jq_vilue.OID, JQ.avail, deley );

                    }
                    else if (JQ.avail + jq_vilue.duration > jq_vilue.timeout) {

                        JQ.avail = jq_vilue.timeout ;
                        deley = JQ.avail - jq_vilue.arrival ;
                        AL.addAbortJob(jq_vilue.OID, JQ.avail, deley );

                    }
                    else{
                        deley = JQ.avail - jq_vilue.arrival ;
                        JQ.avail = JQ.avail + jq_vilue.duration;
                        AL.addDoneJob(jq_vilue.OID, JQ.avail, deley );
                    }

                    JQ.deQueue();

                } // while

                onetf = true ;
                AL.putAll(filelD_naber) ;
                JL.~jobList();
                JQ.~jobQueue();
                AL.~AnsList();

                cout << "\nThe simulation is running...\n" ;
                cout << "See output" + filelD_naber + ".txt\n" ;

            }  // else
            
        } // if

        else if ( command == 0 ) break ;

        else{ cout << "Command does not exist!\n" ; }

        // 多個CPU使用vector裡面每個型態皆為Queue-class
     
    }while (true);

    return 0 ;

}