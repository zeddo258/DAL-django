//第二組 11120411 陳瀚傑 11127241 謝沐龍
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <ctime>
#include <cmath>    //round()
#include <iomanip>

using namespace std;
struct Sortedjobs
{
    int OID = 0 ;
    int Arrival = 0 ;
    int Duration = 0 ;
    int TimeOut = 0 ;
};
void shellSort(vector<vector<int>>&);


//開檔程式
class ReadFile      
{
    private:

    ifstream infile;
    ofstream outfile;

    public:
    
    int choice;

    //呼叫使用者輸入字串
    int ChooseFile(int &choice) {      
        choice = 0;
        cout << "Input a file number: ";
        cin >> choice;
        cout << endl;

        return choice;
    }

    //將檔案內容存入vector中
    void ReadVector(vector<vector<int>>& data){
        string firstline;
        string line;

         // 讀取並忽略第一行，因為它是標題
        getline(infile, firstline);

        while (getline(infile, line)) {
            vector<int> row;
            istringstream iss(line);
            string cell;

            // 將每個數字讀取到 vector 中
            while (getline(iss, cell, '\t')) {
                row.push_back(stoi(cell));
            }
            // 將這行的 vector 存入 data
            data.push_back(row);
        }
    }

    //讀檔，並return成一個vector<vector<int>>
    vector<vector<int>> OpenInputFile() {
 
        ChooseFile(choice);

        string filename = "input" + to_string(choice) + ".txt";    //把使用者的輸入結合成完整的檔案

        infile.open(filename);       //開啟使用者所選擇的檔案

        if (!infile.is_open()) {     //如果檔案開不了就回傳空的vector
            cout << filename << " does not exist!" << endl;
            cout << endl;
            vector<vector<int>> emptyVector;
            return emptyVector;
        }

        vector<vector<int>> data;
        ReadVector(data);

        infile.close();  //關閉檔案line

        return data;   //回傳所擷取到的地圖
    }

    //讀sorted檔，並return成一個vector<vector<int>>
    vector<vector<int>> OpenSortedFile() {

        ChooseFile(choice);

        string filename = "sorted" + to_string(choice) + ".txt";    //把使用者的輸入結合成完整的檔案

        infile.open(filename);       //開啟使用者所選擇的檔案

        if (!infile.is_open()) {     //如果檔案開不了就回傳空的vector
            cout << filename << " does not exist!" << endl;
            cout << endl;
            vector<vector<int>> emptyVector;
            return emptyVector;
        }

        vector<vector<int>> data;
        ReadVector(data);
       
        infile.close();  //關閉檔案line

        return data;   //回傳所擷取到的地圖
    }
    
    //多載 已知檔案名稱 讀sorted檔，並return成一個vector<vector<int>>
    vector<vector<int>> OpenSortedFile(int file) {

        string filename = "sorted" + to_string(file) + ".txt";    //把使用者的輸入結合成完整的檔案

        infile.open(filename);       //開啟使用者所選擇的檔案

        if (!infile.is_open()) {     //如果檔案開不了就回傳空的vector
            cout << filename << " does not exist!" << endl;
            cout << endl;
            vector<vector<int>> emptyVector;
            return emptyVector;
        }

        vector<vector<int>> data;
        ReadVector(data);
       
        infile.close();  //關閉檔案line

        return data;   //回傳所擷取到的地圖
    }

    //關閉檔案並輸出.txt檔
    void CloseFileMisson_1(vector<vector<int>> data){

        string filename = "sorted" + to_string(choice) + ".txt";
        outfile.open(filename);
        
        // 檢查文件是否成功打開
        if (outfile.is_open()) {
            //將vector的內容輸出到文件中
            outfile << "OID\tArrival\tDuration\tTimeOut" << endl;
            for (const auto& row : data) {
                for (const auto& col : row) {
                    outfile << col << "\t";
                }
                outfile << "\n"; 
            }
            outfile.close();    // 關閉 sorted.txt 文件
        }
    }

    void CloseFileMisson_2(vector<vector<int>> abortjobsdata, vector<vector<int>> donejobsdata, double successrate_value, double averagedelay_value, int resent_file ){
        
        string filename = "output" + to_string(resent_file) + ".txt";
        outfile.open(filename);

        // 檢查文件是否成功打開
        if (outfile.is_open()) {
            int i = 1 ;
            //將vector的內容輸出到文件中
            //先做abort jobs
            outfile << "\t[Abort Jobs]" << endl;
            outfile << "\tOID\tAbort\tDelay" << endl;
            //下面做資料的讀入
            for (const auto& row : abortjobsdata) {
                outfile << "["  << i << "]" << "\t";
                for (const auto& col : row) {
                    outfile << col << "\t";
                }
                outfile << "\n"; 
                i++ ;
            }
            i = 1 ;
            //換成done jobs
            outfile << "\t[Jobs Done]" << endl;
            outfile << "\tOID\tDeparture\tDelay" << endl;
            for (const auto& row : donejobsdata) {
                outfile << "["  << i << "]" << "\t";
                for (const auto& col : row) {
                    outfile << col << "\t";
                }
                outfile << "\n"; 
                i++ ;
            }
            

            //寫出average delay跟success rate 
            outfile << "[Average Delay]" << "\t" << fixed << setprecision(2) << averagedelay_value << " ms" << endl;
            outfile << "[Success Rate]" << "\t" << fixed << setprecision(2) << successrate_value << " %" << endl;
            outfile.close();    // 關閉 output.txt 文件
        }
    }

};


//queue的class
class queue{
    private:

    vector<Sortedjobs> array;

    public:
    
    bool isEmpty() const {  //  確認是否存在
        return array.empty();
    }

    bool isFull() const {  //  確認此queue有沒有滿
        int size = array.size();
        if ( size == 3 ) {
            return true ;
        }
        
        return false ;
    }

    void enQueue( Sortedjobs job ) {  //  加進去queue裡面
        array.push_back(job);
    }

    void removeAt(int position) {  //在queue刪除指定資料
        if (position >= 0 && position < array.size()) {
            array.erase(array.begin() + position);
        } else {
            std::cout << "Invalid position. Cannot remove element." << std::endl;
        }
    }

    void deQueue() {  //  在queue裡面刪掉第一個的資料
        if (!isEmpty()) {
            array.erase(array.begin());
        }
        else {
            cout << "this queue doesn`t have data" << endl ;
        }
    }

    Sortedjobs getFront() {  //  在queue裡面取得最前面的
        if (!isEmpty()) {
            return array.front();
        } 
        else {
            cout << "this queue doesn`t have data" << endl ;
            return Sortedjobs();
        }
    }


    Sortedjobs geti( int i ) {  //  在queue裡面取得指定位置
        if (!isEmpty()) {
            return array.at(i);
        } 
        else {
            cout << "this queue doesn`t have data" << endl ;
            return Sortedjobs();
        }
    }


    void clearQ() {  //  把queue清除乾淨
        array.clear();
    }

    int size() const {
        return array.size();
    }


};

//用shellsort排序
void shellSort(vector<vector<int>> &data) {
    int n = data.size();

    // 選擇一個合適的間隔序列（這裡使用 n/2, n/4, n/8, ..., 1）
    for (int gap = n / 2; gap > 0; gap /= 2) {
        // 對每個子列進行插入排序
        for (int i = gap; i < n; ++i) {
            // 從第 gap 個元素開始，對每個元素進行插入排序
            vector<int> temp = data[i];
            int j;
            for (j = i; j >= gap; j -= gap) {

                if(data[j - gap][1] > temp[1]){ // 如果前面的元素比當前元素大，就將前面的元素往後移
                    temp = data[j];
                    data[j] = data[j - gap];
                    data[j - gap] = temp;
                }else if(data[j - gap][1] == temp[1]){
                    if(data[j - gap][0] > temp[0]){ // 如果前面的元素比當前元素大，就將前面的元素往後移
                        temp = data[j];
                        data[j] = data[j - gap];
                        data[j - gap] = temp;
                    }
                }
                // 將當前元素插入到合適的位置
                //data[j] = temp;
            }
        }
    }
}

class Missions{
    private:
        int resent_file = 0;
        
    public:

    //任務1
    void mission_1(){    
        clock_t start_time = clock();   // 記錄執行開始時間

        ReadFile list;
        vector<vector<int>> ToDoList = list.OpenInputFile();

        if (ToDoList.empty()) { //如果是空的代表檔案沒正確開啟，因此回到主選單
            return;
        }

        resent_file = list.choice; //紀錄目前所開啟的是哪一個檔案(ex. 301,302,303...)

        //印出原本的檔案
        cout << "\tOID\tArrival\tDuration\tTimeOut" << endl;
        int i = 1;
        for (const auto& row : ToDoList) {
            cout << "(" << i << ")\t";
            for (const auto& element : row) {
                cout << element << "\t";
            }
            cout << "\n";
            i++;
        }

        clock_t end_openfile_time = clock();    // 記錄 OpenFile 的執行結束時間
        double duration_openfile = double(end_openfile_time - start_time);

        shellSort(ToDoList); //用 shell sort 進行排序

        clock_t end_shellsort_time = clock();   // 記錄 shellSort 的執行結束時間
        double duration_shellsort = double(end_shellsort_time - end_openfile_time);

        list.CloseFileMisson_1(ToDoList); //關閉並寫入檔案

        clock_t end_closefile_time = clock();   // 記錄 closefile 的執行結束時間
        double duration_closefile = double(end_closefile_time - end_shellsort_time);

        cout << "\nReading data: " << duration_openfile << " ms" <<endl;
        cout << "Sorting data: " << duration_shellsort << " ms" <<endl;
        cout << "Writing data: " << duration_closefile << " ms" <<endl;
        cout << "\nSee sorted" << resent_file <<".txt" << endl;
    }

    //任務2
    void mission_2(){      
        ReadFile list;
        vector<vector<int>> ToDoList;

        if(resent_file/100 != 3){   //看是否有做過任務1了
            ToDoList = list.OpenSortedFile();
            if (ToDoList.empty()) { //如果是空的代表檔案沒正確開啟，因此回到主選單
                return;
            }
            resent_file = list.choice; //紀錄目前所開啟的是哪一個檔案(ex. 301,302,303...)
        }else{
            ToDoList = list.OpenSortedFile(resent_file);
            if (ToDoList.empty()) {
                return;
            }
        }

        
        vector<vector<int>> abortjobsdata ;  //  abort jobs的資料
        vector<vector<int>> donejobsdata ;  //  done jobs的資料
        abortjobsdata.resize(0, vector<int>(3, 0));  //  先設定行數是3
        donejobsdata.resize(0, vector<int>(3, 0));  //  先設定行數是3
        process_2( abortjobsdata, donejobsdata, ToDoList ) ;
        list.CloseFileMisson_2( abortjobsdata, donejobsdata, successrate( abortjobsdata, donejobsdata ), averagedelay( abortjobsdata, donejobsdata ), resent_file ) ;
        cout << "The simulation is running..." << endl ;
        cout << "See output" << resent_file << ".txt" << endl ;
    }

    //任務二的過程
    void process_2(vector<vector<int>> &abortjobsdata, vector<vector<int>> &donejobsdata, vector<vector<int>> &sorteddata) {
        int num = 0 ;  //  計數器
        int time = 1 ;  //現在時間
        int starttime = 0 ; //cpu開始時間
        bool firsttimecpu = true ;  //是否為第一次跑cpu
        queue Queue ; //  把queue帶進來
        vector<Sortedjobs> cpuArray;
        while ( num != sorteddata.size() || !cpuisEmpty(cpuArray) || !Queue.isEmpty() ) {  //  結束條件確保跑完sort和cpu和queue
            bool queueisok = true ;
            if ( !cpuisEmpty(cpuArray) ) {  //cpu裡面有東西
                if ( time == cpuArray[0].Duration + starttime )  { //cpu跑完了
                    cpufinish( cpuArray, donejobsdata, time ) ;
                    cpuArray.clear() ;
                    if ( !Queue.isEmpty() ) {  //如果有queue先跑queue裡面的    
                        while ( !Queue.isEmpty() && queueisok != false ) {
                            Sortedjobs currentQueueJob = Queue.getFront();
                            if ( time < currentQueueJob.TimeOut ) {  //當前時間在此逾時之前
                                cpuReplenish_2( currentQueueJob, cpuArray ) ;
                                Queue.deQueue() ;
                                starttime = time ;  //紀錄cpu開始跑的時間
                                queueisok = false ;  
                            }
                            else {  //當前時間在此逾時之後
                                situation_42( currentQueueJob, abortjobsdata, time ) ;
                                Queue.deQueue() ;  //此為4.2的情況
                            }
                        } 
                        
                    }
                    
                }

                if ( !cpuisEmpty(cpuArray) ) {
                    if ( time >= cpuArray[0].TimeOut ) { //此為4.3的情況
                        situation_43( cpuArray, abortjobsdata, time ) ;
                        cpuArray.clear() ;
                        
                        if ( !Queue.isEmpty() ) {  //如果有queue先跑queue裡面的     
                            while ( !Queue.isEmpty() && queueisok != false ) {
                                Sortedjobs currentQueueJob = Queue.getFront();
                                if ( time < currentQueueJob.TimeOut ) {  //當前時間在此逾時之前
                                    cpuReplenish_2( currentQueueJob, cpuArray ) ;
                                    Queue.deQueue() ;
                                    starttime = time ;  //紀錄cpu開始跑的時間
                                    queueisok = false ;
                                }
                                else {  //當前時間在此逾時之後
                                    situation_42( currentQueueJob, abortjobsdata, time ) ; //  此為4.2的情況
                                    Queue.deQueue() ;  
                                }
                                
                            } 
                        }
                    }
                }
            }

            while ( num != sorteddata.size() && time == sorteddata[num][1] ) {  //跑到arrival的時間
                if ( cpuisEmpty(cpuArray) ) { //  cpu是否為空
                    if ( firsttimecpu == true ) {  //第一次跑cpu
                        cpuReplenish_1( sorteddata, cpuArray, num ) ;
                        starttime = time ;  //紀錄cpu開始跑的時間
                        firsttimecpu = false ;
                    }
                    else {  //不是第一次跑
                        if ( !Queue.isEmpty() ) {  //如果有queue先跑queue裡面的
                            
                            while ( !Queue.isEmpty() && queueisok != false ) {
                                Sortedjobs currentQueueJob = Queue.getFront();
                                if ( time < currentQueueJob.TimeOut ) {  //當前時間在此逾時之前
                                    cpuReplenish_2( currentQueueJob, cpuArray ) ;
                                    Queue.deQueue() ;
                                    starttime = time ;  //紀錄cpu開始跑的時間
                                    queueisok = false ;
                                }
                                else {  //當前時間在此逾時之後
                                    situation_42( currentQueueJob, abortjobsdata, time ) ; //  此為4.2的情況
                                    Queue.deQueue() ;  
                                }
                                
                            } 
                            
                        }
                        else {  //沒有queue跑當下num的
                            cpuReplenish_1( sorteddata, cpuArray, num ) ;
                            starttime = time ;  //紀錄cpu開始跑的時間
                        }
                    }
                }
                else {
                    if ( !Queue.isFull() ) {  //沒滿把下一個加進queue裡面
                        queueReplenish ( sorteddata, Queue, num ) ;
                    }
                    else {  //queue如果滿了
                        situation_41(sorteddata, Queue, abortjobsdata, num );  //此為4.1的情況
                    }
                }

                num++ ;
            }

            time++ ;
        }
    
    }

    //計算success rate
    double successrate ( vector<vector<int>> &abortjobsdata, vector<vector<int>> &donejobsdata) {
        int totalsize = abortjobsdata.size() + donejobsdata.size() ;
        double total = donejobsdata.size() * 100;
        total = total / totalsize ;
        total = round(total * 100) / 100;
        return total ;
    };

    //計算average delay
    double averagedelay ( vector<vector<int>> &abortjobsdata, vector<vector<int>> &donejobsdata) {
        double total = 0 ;
        for (int i = 0; i < donejobsdata.size(); i++)
        {
            total = total + donejobsdata[i][2] ;
        }
        
        for (int i = 0; i < abortjobsdata.size(); i++)
        {
            total = total + abortjobsdata[i][2] ;
        }

        int totalsize = donejobsdata.size() + abortjobsdata.size() ;
        total = total / totalsize ;
        total = round(total * 100) / 100;
        return total ;
    };

    //cpu完成要做的事
    void cpufinish ( vector<Sortedjobs> cpuArray, vector<vector<int>> &donejobsdata, int time) {
        vector<int> doneJobRow = { cpuArray[0].OID, time, time - cpuArray[0].Arrival - cpuArray[0].Duration} ;
        donejobsdata.push_back(doneJobRow);
    }

    //找出最大的timeout
    int biggesttime ( vector<vector<int>> &sorteddata) {
        int n = 0 ;
        int thebiggest = 0 ;
        while ( n != sorteddata.size() ) {
            if ( thebiggest < sorteddata[n][3] ) {
                thebiggest = sorteddata[n][3] ;
            }

            n++ ;
        }

        return thebiggest ;

    }

    //把cpu刪除
    void clearCpuArray(vector<Sortedjobs>& cpuArray) {
        cpuArray.clear();
    }

    //cpu是否為空
    bool cpuisEmpty ( vector<Sortedjobs> array) {
        return array.empty();
    }

    //cpu的補充狀況1:queue裡面沒東西時
    void cpuReplenish_1 ( vector<vector<int>> &sorteddata, vector<Sortedjobs> &cpuArray, int num) {
        if (!sorteddata.empty()) {
            // 創一個struct
            Sortedjobs cpuJobs;

            // 把值輸入進去struct裡
            cpuJobs.OID = sorteddata[num][0];
            cpuJobs.Arrival = sorteddata[num][1];
            cpuJobs.Duration = sorteddata[num][2];
            cpuJobs.TimeOut = sorteddata[num][3];

            // 將新的工作加入到 cpuArray 中
            cpuArray.push_back(cpuJobs);
        }
    }

    //cpu的補充狀況2:queue裡面有東西時
    void cpuReplenish_2 ( Sortedjobs queuedata, vector<Sortedjobs> &cpuArray) {
        
        // 創一個struct
        Sortedjobs cpuJobs;

        // 把值輸入進去struct裡
        cpuJobs.OID = queuedata.OID ;
        cpuJobs.Arrival = queuedata.Arrival ;
        cpuJobs.Duration = queuedata.Duration ;
        cpuJobs.TimeOut = queuedata.TimeOut ;

        // 將新的工作加入到 cpuArray 中
        cpuArray.push_back(cpuJobs);
        
    }

    //queue的補充
    void queueReplenish ( vector<vector<int>> &sorteddata, queue& Queue, int num) {
        if (!sorteddata.empty()) {
        // 創一個struct
        Sortedjobs queueJobs;

        // 把值輸入進去struct裡
        queueJobs.OID = sorteddata[num][0];
        queueJobs.Arrival = sorteddata[num][1];
        queueJobs.Duration = sorteddata[num][2];
        queueJobs.TimeOut = sorteddata[num][3];

        // 將新的工作加入到 Queue 中
        Queue.enQueue(queueJobs);
        
        }
    }

    //4.1 佇列已滿的情況
    void situation_41 ( vector<vector<int>> &sorteddata, queue& Queue, vector<vector<int>> &abortjobsdata, int num ) { 
        vector<int> abortJobRow = {sorteddata[num][0], sorteddata[num][1], 0};
        abortjobsdata.push_back(abortJobRow);
    }

    //4.2 佇列取出發現逾時
    void situation_42 ( Sortedjobs currentQueueJob, vector<vector<int>> &abortjobsdata, int time) {
        vector<int> abortJobRow = { currentQueueJob.OID, time, time - currentQueueJob.Arrival };  //{OID, 取出時刻, abort - arrival}
        abortjobsdata.push_back(abortJobRow);
    }

    //4.3 cpu執行時逾時
    void situation_43 ( vector<Sortedjobs> cpuArray, vector<vector<int>> &abortjobsdata, int time) {
        //{OID, timeout, abort - arrival}
        vector<int> abortJobRow = { cpuArray[0].OID, time , time - cpuArray[0].Arrival };
        abortjobsdata.push_back(abortJobRow);
    }

    //超過biggesttime要清空queue並把他放到abort
    void situation_overtime_queue ( Sortedjobs currentQueueJob, vector<vector<int>> &abortjobsdata, int time) {
        vector<int> abortJobRow = { currentQueueJob.OID, time, time - currentQueueJob.Arrival };  //{OID, 取出時刻, abort - arrival}
        abortjobsdata.push_back(abortJobRow);
    }

    //超過biggesttime要清空cpu並把他放到abort
    void situation_overtime_cpu ( vector<Sortedjobs> cpuArray, vector<vector<int>> &abortjobsdata, int time) {
        //{OID, timeout, abort - arrival}
        vector<int> abortJobRow = { cpuArray[0].OID, time , time - cpuArray[0].Arrival };
        abortjobsdata.push_back(abortJobRow);
    }
};


int main()
{
    char startnum = 0;
    Missions go;
    while (1) {
        cout <<"**** Simulate FIFO Queues by SQF *****" << endl;
        cout <<"* 0. Quit                            *" << endl;
        cout <<"* 1. Sort a file                     *" << endl;
        cout <<"* 2. Simulate one FIFO queue         *" << endl;
        cout <<"**************************************" << endl;
        cout <<"Input a command(0, 1, 2):  ";
        startnum = getchar();   //擷取字串
        if(startnum == '\n'){   //以防緩存區存到enter
            startnum = getchar();
        }
        cout << endl;
        switch (startnum)
        {
        case '0':     //0的話直接結束
            exit(0);
        case '1':     //1進入任務1
            go.mission_1();
            break;
        case '2':     //2進入任務2
            go.mission_2();
            break;
        default:    //其他顯示錯誤
            cout << "Command does not exist!" << endl << endl;
            break;
        }
    }
    return 0;
}