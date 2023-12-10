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

//------------------>CPU
class CPU{
    public:
        CPU(int cpuId);
        bool process(int cpuTime, JobState &jobState);
        void load(Data *data, int cpuTime);
        bool isQueueFull(); // 等待數量是否超過三個
        bool queueSize();
        bool isSuccess(); //察看結果是abort還是 success
        bool isProcessing(); //確認有沒有正在跑的工作
    private:
        int id, fullSize = 3, startProcessTime;
        bool success;
        Queue cpuQueue;
        Data* currentProcessing = NULL;

        void startCPUTransation(int cpuTime, JobState &jobState, bool jobSuccess); //把正在跑的程式結束並產生結果
        void feedJobToCPU(int cpuTime); //從Queue feed 到currentProcessing
        void createState(int cpuTime, JobState &jobState, bool jobSuccess); //產生執行結果
};

CPU::CPU(int cpuId){
    this->id = cpuId;
}

bool CPU::process(int cpuTime, JobState &jobState){
    if(currentProcessing == NULL) return false;

    //如果任務達成
    if(startProcessTime+currentProcessing->duration <= cpuTime ){
        startCPUTransation(cpuTime,jobState,true);
        return true;
    }
    //如果timeout
    else if(cpuTime >= currentProcessing->timeout){
        startCPUTransation(cpuTime,jobState,false);
        return true;
    }

    return false;
    
}

void CPU::load(Data *data, int cpuTime){
    if(isQueueFull()) return;
    cpuQueue.enQueue(data);

    if(currentProcessing == NULL) feedJobToCPU(cpuTime);
    
}

bool CPU::isQueueFull(){
    return cpuQueue.getSize() >= fullSize;
}

bool CPU::queueSize(){
    return cpuQueue.getSize();
}

bool CPU::isSuccess(){
    return success;
}

bool CPU::isProcessing(){
    return currentProcessing !=NULL;
}

void CPU::startCPUTransation(int cpuTime, JobState &jobState, bool jobSuccess){
    createState(cpuTime,jobState,jobSuccess);
    feedJobToCPU(cpuTime);
    this->success = jobSuccess;
}

void CPU::feedJobToCPU(int cpuTime){
    if(currentProcessing != NULL){
        delete currentProcessing;
    }

    //把下一個任務丟到正在處理核心
    Data* data = cpuQueue.deQueue();
    currentProcessing = data;
    //如果Queue 是空的就結束
    if(data == NULL) return;

    startProcessTime = cpuTime;
}

void CPU::createState(int cpuTime, JobState &jobState, bool jobSuccess){
    jobState.cpuId = id;
    jobState.delay = cpuTime - currentProcessing->arrival - (jobSuccess?currentProcessing->duration:0);
    jobState.oid = currentProcessing->oid;
    jobState.stopTime = cpuTime;
}


//CPU<------------------

//------------------>FileReader
class FileReader{
    public:
        clock_t read(string something, vector<Data> &storage); // 讀檔
        clock_t writeFile1(string some, vector<Data> &storage); // 任務一的寫檔
        void writeFile2(string some, vector<JobState> &abort, vector<JobState> &success); // 任務二的寫檔
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
    sprintf(str,"[Success Rate]\t%.2f %\n",(float)(success.size()*100)/(float)totalSize);
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
class Mission2{
    public:
        Mission2(FileReader &newReader);
        void process();
    private:
        FileReader *reader;
        Queue jobList;
        CPU cpu;
        string fileName;

        void simulate(); //開始模擬
        bool fetchData(); //從 file reader 讀資料並轉換成 Queue
        void convertJobList(vector<Data> sortedJobs); //vector to Queue
        bool isEnd(); // 如果cpu沒有正在執行或是 joblist 是空的就是結束
};

Mission2::Mission2(FileReader &newReader) : cpu(1), reader(&newReader){
    
}

void Mission2::process(){
    if(!fetchData()) return;
    cout << "The simulation is running...\n";
    simulate();
}

void Mission2::simulate(){
    vector<JobState> success, abort;
    for(int time = 0; !isEnd() ; time++){

        //CPU 先處理手上的工作
        bool cpuWorking = true;
        while(cpuWorking){
            JobState jobState;
            cpuWorking = cpu.process(time,jobState);

            //如果有結果輸出
            if(cpuWorking){
                cpu.isSuccess() ? success.push_back(jobState) : abort.push_back(jobState);
            }
        }

        //看jobList可不可以丟資料
        while(jobList.getSize() != 0 && jobList.getFirst()->arrival == time){
            Data *data = jobList.deQueue();

            //如果已經Full 就 abort
            if(cpu.isQueueFull()){
                abort.push_back({data->oid,time,0,0});
                delete data;
            }
            else{
                cpu.load(data,time);
            }
        }
    }

    reader->writeFile2("output",abort,success);
    
}

bool Mission2::fetchData(){
    if(!reader->isFileNameSet()){
        reader->setFileName();
    }
    
    vector<Data> sortedJob;
    if(reader->read("sorted",sortedJob) == -1.0) return false;

    convertJobList(sortedJob);
    
    return true;
}

void Mission2::convertJobList(vector<Data> sortedJobs){
    for(size_t i = 0 ; i < sortedJobs.size() ; i++){
        Data data = sortedJobs[i];
        jobList.enQueue(data.oid,data.arrival,data.duration,data.timeout);
    }
}

bool Mission2::isEnd(){
    return !cpu.isProcessing() && jobList.getSize() == 0 ;
}

//Mission2<------------------



void info(){
    cout<<
"\n**** Simulate FIFO Queues by SQF *****\n\
* 0. Quit                            *\n\
* 1. Sort a file                     *\n\
* 2. Simulate one FIFO queue         *\n\
**************************************\n";
}

int main(){
    int command;
    FileReader reader;

    do{
        info();

        cout << "Input a command(0, 1, 2): ";
        cin>>command;
        cout << "\n";

        if(command == 1){
            Mission1 task1(reader);
            task1.process();
        }
        else if(command == 2){
            Mission2 task2(reader);
            task2.process();
        }
        else if(command != 1 && command != 2 && command != 0) cout<<"Command does not exist!\n";
    } while(command != 0);
}