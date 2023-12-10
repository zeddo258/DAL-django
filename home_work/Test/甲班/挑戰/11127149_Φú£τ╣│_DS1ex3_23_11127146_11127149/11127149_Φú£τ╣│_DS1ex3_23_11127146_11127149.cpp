#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <time.h>
#include <iomanip>

using namespace std;

struct jobData{     // 工作資料
    int OID;            // 工作編號
    int Arrival;        // 抵達時間
    int Duration;       // 工作耗時
    int TimeOut;        // 逾時時刻
};

typedef vector<jobData> jobList;    // 工作資料List


struct List{        // 佇列資料
    jobData data;
    List * next;
};

typedef List * Listtype; // 佇列List


struct EndData{     // 結束資料
    int OID;            // 工作編號
    int CID;
    int EndTime;        // 結束時間
    int Delay;          // 延遲時間
};

typedef vector<EndData> EndList;  // 結束資料List


// 佇列
class Queue{

    private:
        Listtype backPtr;           // 尾指標
        int cid;
        int len, max;               // 佇列長度,佇列長度最大限制
        int time;                   // 時間

    public:
        Queue();                    // 建構
        ~Queue();                   // 解構
        bool isEmpty();             // 是否為空
        void enbackPtr(jobData);    // 將新資料放至尾端
        void dequeue();             // 刪除頭資料
        void dequeue(jobData&);     // 取出頭資料並刪除
        void getfront(jobData&);    // 取出頭資料

        int NowTime();              // 回傳時間
        void inputTime(int);        // 輸入時間
        int getLen();               // 回傳佇列長度
        int getMax();               // 回傳佇列長度最大限制
        void inputMax(int);
        int getCID();
        void inputCID(int);
        

};


// 讀取資料
class Company{

    private:
        jobList companyData;                    //  公司資料

    public:
        Company();                              // 建構
        ~Company();                             // 解構
        bool inputdata(string, string, int&);   // 讀入檔案input(filename).txt資料，存於動態陣列(公司資料)，並確認是否成功
        void printcompany();                    // 輸出公司資料
        void shellsort(int&);                   // 希爾排序
        bool isFront(jobData,jobData);          // 前者Data是否先於後者Data
        void creatSort_txt(string, int&);       // 將動態陣列資料存於新建檔案sort(filename).txt

        jobList getAll();                       // 回傳公司資料
};


// 結果資料
class AnsList{

    private:
        EndList AbortList, DoneList;    // 中止資料,完成資料
        float avgDelay, successRate;    // 平均延遲時間,完成率

    public:
        AnsList();                      // 建構
        ~AnsList();                     // 解構
        void inputAbort(EndData);       // 輸入中止資料
        void inputDone(EndData);        // 輸入完成資料
        void creatOutput_txt(string, int);   // 將結果資料存於新建檔案output(filename).txt
        
        
};


// 模擬
class Simulation{

    typedef vector<Queue> CPU;

    private:
        jobList companyData;            // 公司資料
        AnsList answer;                 // 結果資料
        CPU cpu;                        // 佇列
        int qNum;                       // 佇列數

    public:
        void simulation(jobList,int);   // 模擬
        void dealNextJob(jobData, int);      // 處理公司頭資料
        void dealQueueJob(int);            // 處理佇列頭資料
        int findStQ(jobData);
        bool getNextJob(jobData&);      // 取出公司頭資料並刪除
        void creatOutput_txt(string,int);   // 將結果資料存於新建檔案output(filename).txt

};



// 主程式
int main()
{
    string command;     // 指令
    string filecode = "";    // 檔案編號
    Company temtCompany;
    
    
    int rt, st, wt;     // 讀入檔案資料時間, 排序資料時間, 新建檔案時間
    do
    {
        Company temtCompany;        // 暫存資料動態陣列

        cout << endl << "**** Simulate FIFO Queues by SQF *****";
        cout << endl << "* 0. Quit                            *";
        cout << endl << "* 1. Sort a file                     *";
        cout << endl << "* 2. Simulate one FIFO queue         *";
        cout << endl << "* 3. Simulate two queues by SQF      *";
        cout << endl << "**************************************";
        cout << endl << "Input a command(0, 1, 2, 3): ";
        cin >> command;                                         // 讀入指令

        if ( strcmp( command.c_str(), "0" ) == 0 ) break;       // 確認指令是否為 "0". 是,break
        else if ( strcmp( command.c_str(), "1" ) == 0 )         // 否,確認指令是否為 "1"
        {
            cout << endl << "Input a file number: ";
            cin >> filecode;                                    // 讀入檔案編號

                                                  

            if ( temtCompany.inputdata(command, filecode, rt) ) {           // 確認讀入檔案資料是否成功

                temtCompany.printcompany();                                     // 輸出檔案資料
                system("pause");                                                // 暫停程式

                temtCompany.shellsort(st);                                      // 希爾排序
                temtCompany.creatSort_txt(filecode, wt);                        // 將整理後資料存於新建檔案

                printf("\nReading data: %d clocks (%d.00 ms).", rt, rt);        // 輸出讀入檔案資料時間
                printf("\nSorting data: %d clocks (%d.00 ms).", st, st);        // 輸出排序資料時間
                printf("\nWriting data: %d clocks (%d.00 ms).", wt, wt);        // 輸出新建檔案時間

                printf("\n\nSee sorted%s.txt\n", filecode.c_str() );
            } // if
        } // else if
        else if ( strcmp( command.c_str(), "2" ) == 0 )         // 否,確認指令是否為 "2"
        {

            if ( strcmp(filecode.c_str(), "" ) == 0 ){              // 當檔案編號為空
                cout << endl << "Input a file number: ";
                cin >> filecode;                                        // 讀入檔案編號
            } // if
                

            if ( temtCompany.inputdata(command, filecode, rt) ) {   // 確認讀入檔案資料是否成功
                Simulation system;                                      // 模擬類別
                system.simulation(temtCompany.getAll(), 1);             // 模擬
                system.creatOutput_txt(filecode, 1);                       // 將資料存於新建檔案 
                printf("\nSee output%s.txt\n", filecode.c_str() );
            } // if
        } // else if
        else if ( strcmp( command.c_str(), "3" ) == 0 )         // 否,確認指令是否為 "2"
        {

            if ( strcmp(filecode.c_str(), "" ) == 0 ){              // 當檔案編號為空
                cout << endl << "Input a file number: ";
                cin >> filecode;                                        // 讀入檔案編號
            } // if
                

            if ( temtCompany.inputdata(command, filecode, rt) ) {   // 確認讀入檔案資料是否成功

                Simulation system;                                      // 模擬類別
                system.simulation(temtCompany.getAll(), 2);             // 模擬
                system.creatOutput_txt(filecode, 2);                       // 將資料存於新建檔案 
                printf("\nSee double%s.txt\n", filecode.c_str() );
            } // if
        } // else if
        else cout << endl << "Command does not exist!";         // 否,指令不存在

    } while (true);
    
    system("pause");            // 暫停程式
    return 0;

} // int main()



// 佇列 ////////////////////////////////

// 建構
Queue::Queue(){
    backPtr = NULL;
    len = 0;
    max = 3;
    time = 0;
} // Queue::Queue()


// 解構
Queue::~Queue(){
	len = 0;
    max = 3;
    time = 0;
} // Queue::~Queue()


// 是否為空
bool Queue::isEmpty(){
    return ( backPtr == NULL );
} // bool Queue::isEmpty()


// 將新資料放至尾端
void Queue::enbackPtr( jobData newData ){
    Listtype newNode = new List;
    newNode->data = newData;
    newNode->next = NULL;
    len++;

    if ( isEmpty() ) newNode->next = newNode;
    else {
    	newNode->next = backPtr->next;
    	backPtr->next = newNode;
	} // else

    backPtr = newNode;
} // void Queue::enbackPtr( jobData newData )


// 刪除頭資料
void Queue::dequeue(){
    
    if ( !isEmpty() ){
    	Listtype tempPtr = backPtr->next;
    	len--;
        if ( backPtr->next == backPtr ) backPtr = NULL;    
        else backPtr->next = tempPtr->next;

        tempPtr->next = NULL;
        delete tempPtr;
    } // if
} // void Queue::dequeue()


// 取出頭資料並刪除
void Queue::dequeue(jobData& data){
    getfront(data);
    dequeue();
} // void Queue::dequeue(jobData& data)


// 取出頭資料
void Queue::getfront(jobData& data){
    if ( !isEmpty() ){
        data = backPtr->next->data;
    }
} // void Queue::getfront(jobData& data)


// 回傳時間
int Queue::NowTime(){
    return time;
} // int Queue::NowTime() 


 // 輸入時間
void Queue::inputTime(int t){
    time = t;
} // void Queue::inputTime(int t)


// 回傳佇列長度
int Queue::getLen(){
    return len;
} // int Queue::getLen()


// 回傳佇列長度最大限制
int Queue::getMax(){
    return max;
} // int Queue::getMax()


void Queue::inputMax(int m){
    max = m;
} // void Queue::inputTime(int t)

int Queue::getCID(){
    return cid;
} // int Queue::getMax()


void Queue::inputCID(int c){
    cid = c;
} // void Queue::inputTime(int t)



// 讀取資料 //////////////////////

// 建構
Company::Company(){}


// 解構
Company::~Company(){
    companyData.clear();
} // Company::~Company()


// 讀入檔案input(filename).txt資料，存於動態陣列(公司資料)，並確認是否成功
bool Company::inputdata( string command, string filename, int& time ){

    int t_start, t_end;
    t_start = clock();                      // 紀錄開始時間

    ifstream input;
    if ( strcmp( command.c_str() , "1" ) == 0 )
        filename = "input" + filename + ".txt";
    else filename = "sorted" + filename + ".txt";

    input.open( filename.c_str() );                 // 開啟檔案          

    if ( input.fail() ){                    // 確認檔案是否開啟失敗
        cout << endl << "### " << filename << " does not exist! ###" << endl;
        input.close();
        return false;
    } // if

    
    string line;
    getline(input, line);                       // 跳過第一行             

    jobData newData;                               // 新Data
    string tempData[4] = {"0","0","0","0"};     // 暫存資料,[0]OID,[1]Arrival,[2]Duration,[3]TimeOut

    while ( getline(input, line) && !input.fail() ) {                   // 當尚未讀完資料

        int i = 0, j = 0;
        while ( strlen(line.c_str()) != 0 ) {                           // 當line不為空字串
            while ( line[i] != '\t' && i != strlen(line.c_str()) ) i++;     // 當尚未讀到'\t'且line尚未讀完

            if ( line[i] == '\t' && j < 3 ){    // 確認是否讀到'\t'且還未讀到TimeOut資料
                tempData[j] = line.substr(0,i);     // 存取資料
                line = line.substr(i + 1);          // 去掉已存資料
                i = 0;
                j++;
            } // if
            else {
                tempData[j] = line;                 // 存取資料
                line = "";                          // 去掉已存資料
            } // else
        } // while

        newData.OID = atoi(tempData[0].c_str());        // 將暫存資料轉成int存入新資料
        newData.Arrival = atoi(tempData[1].c_str());    //
        newData.Duration = atoi(tempData[2].c_str());   //
        newData.TimeOut = atoi(tempData[3].c_str());    //

        companyData.push_back( newData );               // 將新資料存入暫存資料動態陣列尾端
    }
    input.close();                                  // 關閉檔案

    t_end = clock();                    // 紀錄結束時間
    time = t_end - t_start;             // 儲存執行時間
    return true;
} // bool Company::inputdata( string command, string filename, int& time )


//  輸出公司資料
void Company::printcompany(){
    printf("\n\tOID\tArrival\tDuration\tTimeOut\n");    // 輸出標籤
    for ( int i = 0; i < companyData.size(); i++)       // 當尚未讀完資料
        printf("(%d)\t%d\t%d\t%d\t%d\n", i + 1, companyData[i].OID, companyData[i].Arrival, companyData[i].Duration, companyData[i].TimeOut);   // 輸出資料
} // void Company::printcompany()


//  希爾排序
void Company::shellsort(int& time){

    int t_start, t_end;
    t_start = clock();                  // 紀錄開始時間

    for ( int h = companyData.size()/2; h > 0; h = h/2 ) {                          // 當間隔大於0
        for ( int unsorted = h; unsorted < companyData.size(); ++unsorted ) {           // 當未排序位置小於動態陣列資料大小
            int loc = unsorted;                                                         // 插入位置
            jobData nextData = companyData[loc];                                           // 要插入的資料
            for (; loc >= h && isFront(nextData,companyData[loc-h]); loc = loc - h )    // 當插入位置>=間隔且要插入的資料先於動態陣列資料loc - h位置資料
                companyData[loc] = companyData[loc - h];                                    // 將插入位置資料設為loc - h位置資料，插入位置設為loc - h
            companyData[loc] = nextData;                                                // 將要插入的資料置入插入位置
        } // for
    } // for

    t_end = clock();                    // 紀錄結束時間
    time = t_end - t_start;             // 儲存執行時間
} // void Company::shellsort(int& time)


//  前者Data是否先於後者Data
bool Company::isFront( jobData aData, jobData bData){
    if ( aData.Arrival < bData.Arrival )            // 當aData抵達時間 < bData抵達時間
        return true;
    else if ( aData.Arrival == bData.Arrival ) {    // 當aData抵達時間 == bData抵達時間
        if ( aData.OID < bData.OID )                    // 當aData工作編號 < bData工作編號
            return true;
    } // else if

    return false;
} // bool Company::isFront( jobData aData, jobData bData)


//  將公司資料存於新建檔案sort(filename).txt
void Company::creatSort_txt( string filename, int& time ){

    int t_start, t_end;
    t_start = clock();                  // 紀錄開始時間

    filename = "sorted" + filename + ".txt";

    ofstream sort(filename.c_str());        // 建構檔案

    sort <<  "OID" << "\t" << "Arrival" << "\t" << "Duration" << "\t" << "TimeOut";                                                                 // 寫入資料
    for ( int i = 0; i < companyData.size(); ++i )                                                                                                  //
        sort << endl << companyData[i].OID << "\t" << companyData[i].Arrival << "\t" << companyData[i].Duration << "\t" << companyData[i].TimeOut;  // 

    t_end = clock();                    // 紀錄結束時間
    time = t_end - t_start;             // 儲存執行時間
} // void Company::creatSort_txt( string filename, int& time )


// 回傳公司資料
jobList Company::getAll(){
    return companyData;
} // jobList Company::getAll()



// 結果資料 ///////////////////////////////

// 建構
AnsList::AnsList(){
    avgDelay = 0;
    successRate = 0;
} // AnsList::AnsList()


// 解構
AnsList::~AnsList(){
    AbortList.clear();
    DoneList.clear();
    avgDelay = 0;
    successRate = 0;
} // AnsList::~AnsList()


// 輸入中止資料
void AnsList::inputAbort(EndData newData){
	AbortList.push_back(newData);
} // void AnsList::inputAbort(EndData newData)


// 輸入完成資料
void AnsList::inputDone(EndData newData){
	DoneList.push_back(newData);
} // void AnsList::inputDone(EndData newData)


// 將結果資料存於新建檔案output(filename).txt
void AnsList::creatOutput_txt(string filename, int n){
    for ( int i = 0; i < AbortList.size(); i++ )            // 加總所有延遲時間
        avgDelay = avgDelay + float(AbortList[i].Delay);    //
    for ( int i = 0; i < DoneList.size(); i++ )             //
        avgDelay = avgDelay + float(DoneList[i].Delay);     //

    avgDelay = avgDelay / float( AbortList.size() + DoneList.size());   // 平均延遲時間 = 加總所有延遲時間 / 總資料數

    successRate = float(DoneList.size()) / float( AbortList.size() + DoneList.size()) * 100; // 完成率 = 完成資料數 / 總資料數

    if ( n == 1) filename = "output" + filename + ".txt";
    else if ( n == 2) filename = "double" + filename + ".txt";

    ofstream output(filename.c_str());        // 建構檔案

    if ( n == 1 ) {
        output << "\t" << "[Abort Jobs]";                                                                                               // 寫入資料
        output << endl << "\t" << "OID" << "\t" << "Abort" << "\t" << "Delay";                                                          //
        for ( int i = 0; i < AbortList.size(); ++i )                                                                                    //
            output << endl << "[" << i + 1 << "]\t"  << AbortList[i].OID << "\t" << AbortList[i].EndTime << "\t" << AbortList[i].Delay;  //
                                                                                                                                        //
        output << "\n\t" << "[Jobs Done]";                                                                                              //
        output << endl << "\t" << "OID" << "\t" << "Departure" << "\t" << "Delay";                                                      //
        for ( int i = 0; i < DoneList.size(); ++i )                                                                                     //
            output << endl << "[" << i + 1 << "]\t" << DoneList[i].OID << "\t" << DoneList[i].EndTime << "\t" << DoneList[i].Delay;     //
                                                                                                                                        //
        output << endl << "[Average Delay]" << "\t" << fixed << setprecision(2) << avgDelay << " ms";                                   //
        output << endl << "[Success Rate]" << "\t" << fixed << setprecision(2) << successRate << " %";
    }                                  //
    else if ( n == 2){
        output << "\t" << "[Abort Jobs]";                                                                                               // 寫入資料
        output << endl << "\t" << "OID" << "\t" << "CID" << "\t" << "Abort" << "\t" << "Delay";                                                          //
        for ( int i = 0; i < AbortList.size(); ++i )                                                                                    //
            output << endl << "[" << i + 1 << "]\t"  << AbortList[i].OID <<  "\t" << AbortList[i].CID << "\t" << AbortList[i].EndTime << "\t" << AbortList[i].Delay;  //
                                                                                                                                        //
        output << "\n\t" << "[Jobs Done]";                                                                                              //
        output << endl << "\t" << "OID" << "\t" << "CID" << "\t" << "Departure" << "\t" << "Delay";                                                      //
        for ( int i = 0; i < DoneList.size(); ++i )                                                                                     //
            output << endl << "[" << i + 1 << "]\t" << DoneList[i].OID << "\t" << DoneList[i].CID << "\t" << DoneList[i].EndTime << "\t" << DoneList[i].Delay;     //
                                                                                                                                        //
        output << endl << "[Average Delay]" << "\t" << fixed << setprecision(2) << avgDelay << " ms";                                   //
        output << endl << "[Success Rate]" << "\t" << fixed << setprecision(2) << successRate << " %";
    }
} // void AnsList::creatOutput_txt(string filename)



// 模擬

// 模擬
void Simulation::simulation(jobList job,int N){

    companyData = job;
    qNum = N;
    jobData nextjob;
	
    Queue newQueue;
    if ( qNum == 0 ){
    	qNum = 1;
    	cpu.push_back(newQueue);
    	cpu[0].inputMax(0);	
	}
	else{
		for( int i = 0; i < qNum; i++){                  // 建佇列
            cpu.push_back(newQueue);                    //
            cpu[i].inputCID(i+1);
        }
	}
    

    cout << endl << "The simulation is running...";

    while ( !companyData.empty() ){                 // 當公司資料非空
        getNextJob(nextjob);                            // 取出公司頭資料並刪除存為目前處理資料

        int i = 0;
        while( i < qNum ){
            if( nextjob.Arrival >= cpu[i].NowTime() && !cpu[i].isEmpty() )   // 當目前處理資料抵達時間是否大於佇列時間且佇列非空
                dealQueueJob(i);                                                     // 處理佇列頭資料
            else i++;
            
        }
        dealNextJob(nextjob, findStQ(nextjob));                                               // 處理目前處理資料 
    } // while

	int i = 0;
    while( i < qNum ){
        if ( !cpu[i].isEmpty() )
            dealQueueJob(i);
        else i++;
	}

} // void Simulation::simulation(jobList job,int N)


// 處理公司頭資料
void Simulation::dealNextJob(jobData nextjob, int i){

    if ( nextjob.Arrival >= cpu[i].NowTime() && cpu[i].isEmpty()){  // 公司頭資料抵達時間是否大於佇列時間且佇列為空
        EndData newEndData;
                                                                        // 3-1
        newEndData.OID = nextjob.OID;                                   // 工作編號
        newEndData.CID = cpu[i].getCID();
        newEndData.EndTime = nextjob.Arrival + nextjob.Duration;        // 完成時刻 = 抵達時間 + 工作耗時
        newEndData.Delay = 0;                                           // 延遲時間 = 抵達時間 - 抵達時間 = 0
        answer.inputDone(newEndData);                                   // 存入完成資料
        cpu[i].inputTime(nextjob.Arrival + nextjob.Duration);           // 佇列時間 = 抵達時間 + 工作耗時
    } // if
    else if ( cpu[i].getLen() == cpu[i].getMax() ){                 // 佇列為滿
        EndData newEndData;
                                                                        // 4-1
        newEndData.OID = nextjob.OID;;                                             // 工作編號
        newEndData.CID = 0;
        newEndData.EndTime = nextjob.Arrival;                           // 中止時刻 = 抵達時間
        newEndData.Delay = 0;                                           // 延遲時間 = 0
        answer.inputAbort(newEndData);                                  // 存入中止資料
    } // else if
    else{
        cpu[i].enbackPtr(nextjob);                                      // 存入佇列尾
    } // else
} // void Simulation::dealNextJob(jobData nextjob)


// 處理佇列頭資料
void Simulation::dealQueueJob(int i){

    jobData dojob;
    cpu[i].dequeue(dojob);                  // 取出佇列頭資料並刪除存為處理資料

    if ( cpu[i].NowTime() + dojob.Duration <= dojob.TimeOut ) { // 佇列時間 + 處理資料工作耗時 是否 <= 處理資料逾時時�
        EndData newEndData;
                                                                    // 3-1
        newEndData.OID = dojob.OID;                                 // 工作編號
        newEndData.CID = cpu[i].getCID();
        newEndData.EndTime = cpu[i].NowTime() + dojob.Duration;     // 完成時刻 = 佇列時間 + 工作耗時
        newEndData.Delay = cpu[i].NowTime() - dojob.Arrival;        // 延遲時間 = 佇列時間 - 抵達時間
        answer.inputDone(newEndData);                               // 存入完成資料
        cpu[i].inputTime(cpu[i].NowTime() + dojob.Duration);        // 佇列時間 = 佇列時間 + 工作耗時
    } // if
    else {
        EndData newEndData;
        newEndData.OID = dojob.OID;                                 // 工作編號
        newEndData.CID = cpu[i].getCID();
        if ( cpu[i].NowTime() > dojob.TimeOut ){                    // 佇列時間是否 > 處理資料逾時時刻
                                                                        // 4-2
            newEndData.EndTime = cpu[i].NowTime();                      // 中止時刻 = 佇列時間
            newEndData.Delay = cpu[i].NowTime() - dojob.Arrival;        // 延遲時間 = 佇列時間 - 抵達時間
        } // if
        else {
                                                                        // 4-3
            newEndData.EndTime = dojob.TimeOut;                         // 中止時刻 = 逾時時刻
            newEndData.Delay = dojob.TimeOut - dojob.Arrival;           // 延遲時間 = 逾時時刻 - 抵達時間
            cpu[i].inputTime(dojob.TimeOut);                            // 佇列時間 = 逾時時刻
        } // else
        answer.inputAbort(newEndData);                              // 存入中止資料
    } // else

} // void Simulation::dealQueueJob()


// 取出公司頭資料
bool Simulation::getNextJob(jobData& data){
    if ( companyData.empty() )                  // 確認公司資料為空
        return false;
    else{
        data = companyData[0];                  // 取出第一筆資料
        companyData.erase(companyData.begin()); // 刪除第一筆資料
    } // else
    return true;
} // bool Simulation::getNextJob(jobData& data)


// 將結果資料存於新建檔案output(filename).txt
void Simulation::creatOutput_txt(string filename,int n){
    answer.creatOutput_txt(filename,n);               // 呼叫將結果資料存於新建檔案output(filename).txt
} // void Simulation::creatOutput_txt(string filename)


int Simulation::findStQ(jobData nextjob){
    int n = 0;
    for ( int i = 1; i < qNum; i++ ){
        if ( ((cpu[i].isEmpty() && cpu[n].isEmpty()) &&  (cpu[i].NowTime() < nextjob.Arrival && cpu[n].NowTime() < nextjob.Arrival)) ){
            if ( cpu[i].getCID() < cpu[n].getCID() )
                n = i;
        }
        else if (!(cpu[n].isEmpty() && cpu[n].NowTime() < nextjob.Arrival )){
            if (cpu[i].isEmpty() && cpu[i].NowTime() < nextjob.Arrival ){
                n = i;
            }
            else if ( cpu[i].getLen() < cpu[n].getLen() || ((cpu[i].getLen() == cpu[n].getLen() ) && cpu[i].getCID() < cpu[n].getCID()) )
                n = i;
        }
    }
    return n;
}
