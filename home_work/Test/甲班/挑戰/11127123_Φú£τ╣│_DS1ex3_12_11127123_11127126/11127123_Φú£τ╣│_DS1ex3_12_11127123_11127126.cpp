// 11127123 林靖庭, 11127126 黃柏寧
#include<iostream>
#include<fstream>
#include<vector>
#include<ctime>
#include<cstdio>

using namespace std;

struct Data {
    int oid, arrival, duration, timeout;
    Data* next;
};

struct JobState{
    int oid, stopTime, delay, cpuId;
    bool success;
    JobState* next;
};

//------------------>Queue
class Queue{
    public:
        ~Queue();
        void enQueue(int oid, int arrival, int duration, int timeout); //把資料放進Queue 尾
        void enQueue(Data *data);
        Data* deQueue(); //拿取第一筆並從Queue中刪除
        Data* getFirst(); //拿取第一筆資料
        int getSize(); //獲取當前大小
    private:
        int size = 0;
        Data* front = NULL, *last = NULL;
        Data* createData(int oid, int arrival, int duration, int timeout);
        void releaseMemory();

};

Queue::~Queue() {
    releaseMemory();
}

void Queue::enQueue(int oid, int arrival, int duration, int timeout){
    Data *newData = createData(oid,arrival,duration,timeout);

    //如果是空的
    if(front == NULL) front = newData;
    else last->next = newData;
    last = newData;

    size++;
    
}

void Queue::enQueue(Data *data){
    //如果是空的
    if(front == NULL) front = data;
    else last->next = data;

    data->next = NULL;
    last = data;
    size++;

}


Data* Queue::deQueue(){
    //如果裡面是空的
    if(front == NULL) return NULL;
    Data *res = front;
    
    //如果剩下一個
    if(front == last) {
        last = NULL;
    }

    front = front->next;
    size--;
    return res;
}

Data* Queue::getFirst(){
    return front;
}

int Queue::getSize(){
    return size;
}

Data* Queue::createData(int oid, int arrival, int duration, int timeout){
    return new Data{oid,arrival,duration,timeout,NULL};
}

void Queue::releaseMemory(){
    while(front != NULL){
        Data* next = front->next;
        delete front;
        front = next;
    }
}

//Queue<------------------


//------------------>Queue2
class Queue2{
    public:
        ~Queue2();
        void enQueue(JobState *state);
        JobState* deQueue(); //拿取第一筆並從Queue中刪除
        JobState* getFirst(); //拿取第一筆資料
        int getSize(); //獲取當前大小
    private:
        int size = 0;
        JobState* front = NULL, *last = NULL;
        void releaseMemory();

};

Queue2::~Queue2() {
    releaseMemory();
}


void Queue2::enQueue(JobState *state){
    //如果是空的
    if(front == NULL) front = state;
    else last->next = state;

    state->next = NULL;
    last = state;
    size++;

}


JobState* Queue2::deQueue(){
    //如果裡面是空的
    if(front == NULL) return NULL;
    JobState *res = front;
    
    //如果剩下一個
    if(front == last) {
        last = NULL;
    }

    front = front->next;
    size--;
    return res;
}

JobState* Queue2::getFirst(){
    return front;
}

int Queue2::getSize(){
    return size;
}


void Queue2::releaseMemory(){
    while(front != NULL){
        JobState* next = front->next;
        delete front;
        front = next;
    }
}

//Queue2<------------------

//------------------>CPU2
class CPU2{
    public:
        CPU2(int cpuId);
        ~CPU2();
        bool load(Data *data); //把新資料載入到CPU 並回傳是否直接變成正在執行的資料
        bool process(int newTime); //依照時間將資料往前挪移並回傳是否有改動
        bool flush(); //直接向前挪移並回傳是否有改動
        void printALL(); //Debug
        
        bool isQueueFull(); // 等待數量是否超過三個
        int queueSize();
        bool isProcessing(); //確認有沒有正在跑的工作
        JobState* getCurrentProcessing();
    private:
        int id, fullSize = 3, time = 0;
        bool success;
        Queue2 cpuQueue;
        JobState* currentProcessing = NULL;

        JobState* createState(Data *data);
        void feedStateToProcessing(); //從Queue中餵資料到正在執行序列
};

CPU2::CPU2(int cpuId){
    this->id = cpuId;
}

CPU2::~CPU2(){
    if(isProcessing()){
        delete currentProcessing;
    }
}

bool CPU2::load(Data *data){
    if(isQueueFull()) return false;

    JobState* state = createState(data);
    cpuQueue.enQueue(state);

    if(!isProcessing())  {
        feedStateToProcessing();
        return true;
    }

    return false;
}

bool CPU2::process(int newTime){
    time = time > newTime ? time : newTime;

    if(!isProcessing()) return false;
    if(currentProcessing->stopTime > newTime) return false;

    feedStateToProcessing();
    return isProcessing();
}

bool CPU2::flush(){
    if(!isProcessing()) return false;
    feedStateToProcessing();
    return isProcessing();
}


void CPU2::printALL(){
    cout << "CPU: " << id << endl;
    JobState * it = cpuQueue.getFirst();
    while(it !=  NULL){
        cout << "OID: " << it->oid << endl;
        it = it->next;
    }

    int curID = isProcessing()? currentProcessing->oid : -1;
    cout << "Current OID: " << curID << endl;
    cout << "END print\n";
}


bool CPU2::isQueueFull(){
    return cpuQueue.getSize() >= fullSize;
}

int CPU2::queueSize(){
    return cpuQueue.getSize() + (isProcessing() ? 1 : 0);
}

bool CPU2::isProcessing(){
    return currentProcessing !=NULL;
}

JobState* CPU2::getCurrentProcessing(){
    return currentProcessing;
}


JobState *CPU2::createState(Data *data){
    JobState* res = new JobState;

    res->oid = data->oid;
    res->cpuId = id;

    //如果CPU時間比較小 就同步到到達的時間
    this->time = time > data->arrival ? time : data->arrival;
    int finishTime = time + data->duration;

    //如果資料可以成功執行
    if(finishTime <= data->timeout){
        res->stopTime = finishTime;
        res->delay = finishTime - (data->arrival+data->duration);
        res->success = true;
    }
    else{
        res->stopTime = time > data->timeout ? time : data->timeout;
        res->delay = res->stopTime - data->arrival;
        res->success = false;
        finishTime = res->stopTime;
    }

    //更新最後的時間
    this->time = finishTime;


    delete data;

    return res;
}


void CPU2::feedStateToProcessing(){
    if(isProcessing()) delete currentProcessing;
    JobState* state = cpuQueue.deQueue();
    currentProcessing = state;
}

//CPU2<------------------

//------------------>FileReader
class FileReader{
    public:
        clock_t read(string something, vector<Data> &storage); // 讀檔
        clock_t writeFile1(string some, vector<Data> &storage); // 任務一的寫檔
        void writeFile2(string some, vector<JobState> &abort, vector<JobState> &success); // 任務二的寫檔
        void writeFile3(string some, vector<JobState> &abort, vector<JobState> &success); //任務三寫檔
        void setFileName(); // 取得檔案的數字，ex:301、302、303... 
        bool isFileNameSet(); // 看filename有無內容，有就return true，沒有就return false 
        string getFileName(); // 從reader裡取得檔案名稱
    private:
        string fileName;
        bool nameSet = false; // 看filename裡有無內容
};

clock_t FileReader::read(string something, vector<Data> &storage){
    int oid, arrival, duration, timeout;
    string read = something + fileName + ".txt";
    clock_t start = clock();  // 開始測量讀檔時間
    ifstream inputFile(read);
    if(inputFile.is_open()){
        string buf;
        inputFile >> buf >> buf >> buf >> buf; // 讀取字串"OID"、"Arrival"、"Duartion"、"TimeOut" 
        inputFile>>oid;
        while(!inputFile.eof()){
            inputFile>>arrival>>duration>>timeout;
            storage.push_back({oid, arrival, duration, timeout, NULL});

            inputFile>>oid;
        }
        inputFile.close();
        clock_t end = clock();  // 結束測量讀檔時間
        clock_t duration = end - start; // 計算時間差
        return duration; // 回傳讀檔時間 
    }
    else cerr << "### "<<read<<" does not exist! ###\n"; 
    return -1.0; // 如果沒有成功讀檔就回傳-1.0 
}
clock_t FileReader::writeFile1(string some, vector<Data> &storage){
    string write = some + fileName + ".txt";
    clock_t start = clock();  // 開始測量寫檔時間
    ofstream outFile(write);
    outFile<<"OID\tArrival\tDuration\tTimeOut\n";
    for(size_t i = 0 ; i < storage.size() ; i++) {
        outFile<<storage[i].oid<<"\t"<<storage[i].arrival<<"\t"<<storage[i].duration<<"\t"<<storage[i].timeout << "\n";
    }
    outFile.close(); // 關閉檔案 
    clock_t end = clock();  // 結束測量寫檔時間
    clock_t duration = end - start;
    return duration; // 回傳寫檔時間 
}

void FileReader::writeFile2(string some, vector<JobState> &abort, vector<JobState> &success){
    string write = some + fileName + ".txt";
    ofstream outFile(write);
    outFile<<"\t[Abort Jobs]\n\tOID\tAbort\tDelay\n";
    int averageDelay = 0;
    for(size_t i = 0 ; i < abort.size() ; i++){
        JobState state = abort[i];
        averageDelay += state.delay; //把各個工作的Delay時間加總 
        outFile<<"[" << i+1 << "]" << "\t" << state.oid << "\t" << state.stopTime << "\t" << state.delay << endl;
    }
    outFile<<"\t[Jobs Done]\n\tOID\tDeparture\tDelay\n";
    for(size_t i = 0 ; i < success.size() ; i++){
        JobState state = success[i];
        averageDelay += state.delay; //把各個工作的Delay時間加總
        outFile<<"[" << i+1 << "]" << "\t" << state.oid << "\t" << state.stopTime << "\t" << state.delay << endl;
    }
    size_t totalSize = success.size() + abort.size();
    char str[256];
    sprintf(str,"[Average Delay]\t%.2f ms\n",(float)averageDelay/(float)totalSize);
    outFile << str;
    sprintf(str,"[Success Rate]\t%.2f %%\n",(float)(success.size()*100)/(float)totalSize);
    outFile << str;
    cout << "\nSee " << write <<endl;
    outFile.close();
}

void FileReader::writeFile3(string some, vector<JobState> &abort, vector<JobState> &success){
    string write = some + fileName + ".txt";
    ofstream outFile(write);
    outFile<<"\t[Abort Jobs]\n\tOID\tCID\tAbort\tDelay\n";
    int averageDelay = 0;

    for(size_t i = 0 ; i < abort.size() ; i++){
        JobState state = abort[i];
        averageDelay += state.delay; //把各個工作的Delay時間加總 
        outFile<<"[" << i+1 << "]" << "\t" << state.oid << "\t" << state.cpuId << "\t" << state.stopTime << "\t" << state.delay << endl;
        
    }
    outFile<<"\t[Jobs Done]\n\tOID\tCID\tDeparture\tDelay\n";
    for(size_t i = 0 ; i < success.size() ; i++){
        JobState state = success[i];
        averageDelay += state.delay; //把各個工作的Delay時間加總
        outFile<<"[" << i+1 << "]" << "\t" << state.oid << "\t" << state.cpuId << "\t" << state.stopTime << "\t" << state.delay << endl;
    }
    
    size_t totalSize = success.size() + abort.size();
    char str[256];
    sprintf(str,"[Average Delay]\t%.2f ms\n",(float)averageDelay/(float)totalSize);
    outFile << str;
    sprintf(str,"[Success Rate]\t%.2f %%\n",(float)(success.size()*100)/(float)totalSize);
    outFile << str;
    cout << "\nSee " << write <<endl;
    outFile.close();
}


void FileReader::setFileName(){
	// 取得檔案的數字，ex:301、302、303...
    cout<<"Input a file number: ";
    cin>>fileName;
    nameSet = true;
}

bool FileReader::isFileNameSet(){
	// 回傳nameSet的布林值 
    return nameSet;
}

string FileReader::getFileName(){
	// 取得檔案名稱 
    return fileName;
}


//FileReader<------------------



//------------------>Mission1
class Mission1{
    public:
        Mission1(FileReader &newReader); // constructer
        void process(); // 執行任務一
        void print(); // 印出未整理的內容
        void caculation(clock_t duration_read, clock_t duration_sort, clock_t duration_write); // 印出讀檔、排序以及寫檔的時間
    private:
        FileReader *reader;
        vector<Data>storage; // 儲存文字檔的內容
        string filename;
		clock_t duration_read, duration_sort, duration_write; //分別代表讀檔、整理以及寫檔的時間
};

Mission1::Mission1(FileReader &newReader) : reader(&newReader){

}

void Mission1::print(){
	cout<<"\n\tOID\tArrival\tDuration\tTimeOut\n";
	for(size_t i = 0 ; i < storage.size() ; i++) {
    	cout<<"("<<i + 1<<")\t"<<storage[i].oid<<"\t"<<storage[i].arrival<<"\t"<<storage[i].duration<<"\t"<<storage[i].timeout<<endl;
	}
	
	cout<<"\n";
} 

void Mission1::caculation(clock_t duration_read, clock_t duration_sort, clock_t duration_write){
    printf("Reading data: %ld clocks (%.2f ms).\n", duration_read, (float) duration_read);
    printf("Sorting data: %ld clocks (%.2f ms).\n", duration_sort, (float) duration_sort);
    printf("Writing data: %ld clocks (%.2f ms).\n", duration_write, (float) duration_write);
}

void Mission1::process(){
    reader->setFileName();
    duration_read = reader -> read("input", storage); // 取得讀檔時間
    if(duration_read >= 0.0) {
    	print();
    	clock_t sortstart = clock(); // 排序時間開始 
    	int n = storage.size();
        for (int gap = n / 2 ; gap > 0 ; gap = gap / 2) {
            for (int i = gap ; i < n ; i++) {
                Data temp = storage[i];
                int j = i;
                // 如果 Arrival大於temp或者Arrival相等但OID大於temp，進行交換
                for (; j >= gap && ((storage[j - gap].arrival > temp.arrival) || (storage[j - gap].arrival == temp.arrival && storage[j - gap].oid > temp.oid)) ; j = j - gap) {
                    storage[j] = storage[j - gap];
                }
                storage[j] = temp;
            }
        }
        clock_t sortend = clock(); // 排序時間結束 
        duration_sort = sortend - sortstart; // 計算排序時間
        duration_write = reader -> writeFile1("sorted", storage); // 取得寫檔時間
        caculation(duration_read, duration_sort, duration_write);
        filename = reader -> getFileName(); // 取得filename 
    	cout<<"\nSee sorted"<<filename<<".txt\n";
    }
}
    
//Mission1<------------------



//------------------>Mission2
class Simulator{
    public:
        Simulator(FileReader &newReader);
        ~Simulator();
        void setCPUSize(int amount);
        void process();
    private:
        FileReader *reader;
        Queue jobList;
        vector<CPU2*> cpus;
        int cpuCount = 1;

        void simulate(); //開始模擬
        bool fetchData(); //從 file reader 讀資料並轉換成 Queue
        void convertJobList(vector<Data> sortedJobs); //vector to Queue
        void setupAllCPU(); //按照cpuCount 設置所有CPU
        void resetAllCPU(); //清除所有CPU
        void processAllCPU(vector<JobState> &success, vector<JobState> &abort, int time); //處理所有處理器
        bool feedDataToCPU(Data *data, vector<JobState> &success, vector<JobState> &abort); //把Data 嘗試丟給CPU
        CPU2* getShortestQueueCPU(); //找到目前隊伍最短的CPU
        JobState getStateFromPtr(JobState* state); //轉換Jobstate
        void addCurrentProcessingToResult(CPU2 *cpu, vector<JobState> &success, vector<JobState> &abort); //把CPU最新的任務直接丟到結果
        void flushAllData(vector<JobState> &success, vector<JobState> &abort); //所剩餘資料全部丟出來
};

Simulator::Simulator(FileReader &newReader) {
    reader = &newReader;
}

Simulator::~Simulator(){
    resetAllCPU();
}

void Simulator::process(){
    if(!fetchData()) return;
    cout << "The simulation is running...\n";
    simulate();
}

void Simulator::setCPUSize(int amount){
    this->cpuCount = amount;
}


void Simulator::simulate(){
    setupAllCPU();
    vector<JobState> success, abort;
    while(jobList.getSize() != 0){
        int time = jobList.getFirst()->arrival;

        processAllCPU(success,abort,time);
        
        //看jobList可不可以丟資料
        while(jobList.getSize() != 0 && jobList.getFirst()->arrival == time){
            Data *data = jobList.deQueue();

            //如果都已經Full 就 abort
            if(!feedDataToCPU(data,success,abort)){
                abort.push_back({data->oid,time,0,0,false,NULL});
                delete data;
            }
            
        }
    }

    flushAllData(success,abort);

    if(cpuCount == 1){
        reader->writeFile2("output",abort,success);
    }
    else{
        reader->writeFile3("double",abort,success);
    }
    
}

bool Simulator::fetchData(){
    if(!reader->isFileNameSet()){
        reader->setFileName();
    }
    
    vector<Data> sortedJob;
    if(reader->read("sorted",sortedJob) == -1.0) return false;
    convertJobList(sortedJob);
    
    return true;
}

void Simulator::convertJobList(vector<Data> sortedJobs){
    for(size_t i = 0 ; i < sortedJobs.size() ; i++){
        Data data = sortedJobs[i];
        jobList.enQueue(data.oid,data.arrival,data.duration,data.timeout);
    }
}


void Simulator::setupAllCPU(){
    resetAllCPU();

    for(int i = 1 ; i <= cpuCount ; i++ ){
        cpus.push_back(new CPU2(i)); 
    }
}
void Simulator::resetAllCPU(){
    while (cpus.size() != 0) {
        CPU2* cpu = cpus.back();
        cpus.pop_back();
        
        delete cpu;
    }
    
}

void Simulator::processAllCPU(vector<JobState> &success, vector<JobState> &abort, int time){
    //CPU 先處理手上的工作
    for(size_t i = 0 ; i < cpus.size() ; i++){
        CPU2 *cpu = cpus[i];

        bool cpuWorking = true;
        while(cpuWorking){
            cpuWorking = cpu->process(time);

            //如果有結果輸出
            if(cpuWorking) addCurrentProcessingToResult(cpu,success,abort);
        }
    }
}

bool Simulator::feedDataToCPU(Data *data, vector<JobState> &success, vector<JobState> &abort){
    CPU2 *shortestCPU = getShortestQueueCPU();
    if(shortestCPU == NULL) return false;

    bool change =  shortestCPU->load(data);
    //如果一進去就直接 process
    if(change) addCurrentProcessingToResult(shortestCPU,success,abort);

    return true;
}

CPU2 *Simulator::getShortestQueueCPU(){
    CPU2* result = NULL;
     
    //直找到第一個遇到最短的
    for(size_t i = 0 ; i < cpus.size() ; i++){
        CPU2* cpu = cpus[i];
        
        if(cpu->isQueueFull()) continue;

        if(result == NULL || cpu->queueSize() < result->queueSize()) {
            result = cpu;
        }
    }

    return result;
}

JobState Simulator::getStateFromPtr(JobState *state){
    return {state->oid,state->stopTime,state->delay,state->cpuId,state->success,NULL};
}

void Simulator::addCurrentProcessingToResult(CPU2 *cpu, vector<JobState> &success, vector<JobState> &abort){
    JobState state = getStateFromPtr(cpu->getCurrentProcessing());
    state.success? success.push_back(state) : abort.push_back(state);
}

void Simulator::flushAllData(vector<JobState> &success, vector<JobState> &abort){
    for(size_t i = 0 ; i < cpus.size() ; i++){
        CPU2* cpu = cpus[i];
        
        //用到CPU全空為止
        while(cpu->flush()){
            addCurrentProcessingToResult(cpu,success,abort);
        }
    }
}



//Mission2<------------------



void info(){
    cout<<
"\n**** Simulate FIFO Queues by SQF *****\n\
* 0. Quit                            *\n\
* 1. Sort a file                     *\n\
* 2. Simulate one FIFO queue         *\n\
* 3. Simulate two queues by SQF      *\n\
**************************************\n";
}

int main(){
    int command;
    FileReader reader;

    do{
        info();

        cout << "Input a command(0, 1, 2, 3): ";
        cin>>command;
        cout << "\n";

        if(command == 0) break;
        else if(command == 1){
            Mission1 task1(reader);
            task1.process();
        }
        else if(command == 2){
            Simulator task2(reader);
            task2.process();
        }
        else if(command == 3){
            Simulator task3(reader);

            task3.setCPUSize(2);
            task3.process();
        }
        else cout<<"Command does not exist!\n";
    } while(command != 0);
}