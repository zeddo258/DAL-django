//第27組 11020117 林子皓  11020134 呂宗凱

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

// 工作類別
class Job {
public:
    int OID;
    int Arrival;
    int Duration;
    int TimeOut;
};

// 佇列類別
class Queue {
private:
    Job* jobs;
    int front;
    int rear;
    int capacity;

public:
    Queue(int size);
    ~Queue();

    bool isEmpty();
    bool isFull();
    void enqueue(Job job);
    Job dequeue();
    Job& at(int index);
    int getNumElements();
};

//Queue的初始化 
Queue::Queue(int size) {
    capacity = size;
    jobs = new Job[capacity];
    front = rear = -1;
}

//釋放Jobs陣列占用的空間 
Queue::~Queue() {
    delete[] jobs;
}

//檢查佇列是否為空 
bool Queue::isEmpty() {
    return front == -1;
}

//檢查佇列是否已滿 
bool Queue::isFull() {
    return (rear + 1) % capacity == front;
}

//將Job中的東西加入佇列 
void Queue::enqueue(Job job) {
    if (isFull()) {
        cout << "Queue is full. Cannot enqueue." << endl;
        return;
    }

    if (isEmpty()) {
        front = rear = 0;
    } else {
        rear = (rear + 1) % capacity;
    }

    jobs[rear] = job;
}

//取得佇列中最前項 
Job Queue::dequeue() {
    if (isEmpty()) {
        cout << "Queue is empty. Cannot dequeue." << endl;
        Job emptyJob;
        return emptyJob;
    }

    Job dequeuedJob = jobs[front];

    if (front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % capacity;
    }

    return dequeuedJob;
}

//用來取得佇列中指定位置的 Job。
Job& Queue::at(int index) {
    if (index >= 0 && index < capacity) {
        return jobs[index];
    } else {
        cout << "Error!" << endl;
        static Job emptyJob;
        return emptyJob;
    }
}

// 計算目前佇列中的元素數量
int Queue::getNumElements() {
    
    if (isEmpty()) {
        return 0;
    } else {
        return (rear - front + capacity) % capacity + 1;
    }
}

// 讀取檔案資料 
void readData(Queue& queue, const char* filename) {
    ifstream infile(filename);

    if (!infile) {
        return;
    }

    Job job;
    
    // 忽略標題 
    infile.ignore(256, '\n');
    
    while (infile >> job.OID >> job.Arrival >> job.Duration >> job.TimeOut) {
        queue.enqueue(job);
    }

    infile.close();
}

void insertionSort(Queue& queue) {
    int n = queue.getNumElements();

    for (int i = 1; i < n; ++i) {
        Job key = queue.at(i);
        int j = i - 1;

        // 先比較 Arrival
		// 如果 Arrival 相同，就比較 OID
        while (j >= 0 && (queue.at(j).Arrival > key.Arrival || (queue.at(j).Arrival == key.Arrival && queue.at(j).OID > key.OID))) {
            queue.at(j + 1) = queue.at(j);
            --j;
        }

        queue.at(j + 1) = key;
    }
}


// 顯示佇列中的資料
void displayQueue(Queue& queue) {
    cout << "        OID     Arrival Duration        TimeOut" << endl;
    int numElements = queue.getNumElements();
    for (int i = 0; i < numElements; ++i) {
        Job job = queue.at(i);
        if (i < 9) {
        	cout << "("<< std::setw(1) << i + 1 << ") "
             << std::setw(8) << job.OID 
             << std::setw(7) << job.Arrival
             << std::setw(7) << job.Duration
             << std::setw(9) << job.TimeOut << endl;
		}
		else {
			cout << "("<< std::setw(1) << i + 1 << ")"
             << std::setw(8) << job.OID 
             << std::setw(7) << job.Arrival
             << std::setw(7) << job.Duration
             << std::setw(9) << job.TimeOut << endl;
		}
    }
    cout << "\n"; 
}

//把排序後的資料寫入新的檔案中
void writeSortedData(Queue& queue, const char* filename) {
    ofstream outfile(filename);

    outfile << std::setw(3) << "OID" 
            << std::setw(12) << "Arrival" 
            << std::setw(9) << "Duration" 
            << std::setw(15) << "TimeOut" << endl;

    int numElements = queue.getNumElements();
    for (int i = 0; i < numElements; ++i) {
        Job job = queue.at(i);
        outfile << std::setw(3) << job.OID 
                << std::setw(6) << job.Arrival 
                << std::setw(8) << job.Duration 
                << std::setw(9) << job.TimeOut << endl;
    }

    outfile.close();
}

int main() {
    int choice;
    string expression;
    Queue jobsQueue(100);
    string input;
    string output;
    string filename;

    // 宣告時間變數
    clock_t readStart, readEnd, sortStart, sortEnd, writeStart, writeEnd;

	//開始執行 
    do {
    	cout << "\n" ;
        cout << "**** Simulate FIFO Queues by SQF *****" << endl;
        cout << "* 0. Quit                            *" << endl;
        cout << "* 1. Sort a file                     *" << endl;
        cout << "* 2. Simulate one FIFO queue         *" << endl;
        cout << "**************************************" << endl;
        cout << "Input a command (0, 1, 2): ";
        
        cin >> choice;
        cout << endl;

        cin.ignore();

        switch (choice) {
            case 0:  //0就退出執行 
                break; 
            case 1:  //讀檔 排序 跟 寫檔 並計算時間 

                cout << "Input a file number: ";
                cin >> filename;
                cout << "\n";
                input = "input" + filename + ".txt";
                output = "sorted" + filename + ".txt";

				// 計算讀檔開始時間
                readStart = clock();
                
                // 讀取檔案資料 
                readData(jobsQueue, input.c_str());
                
                if (jobsQueue.isEmpty()) {
                    cout << "### "<< input <<" does not exist! ###" << endl;
                    break;
                }

                // 計算讀檔結束時間
                readEnd = clock();
                
                // 顯示佇列資料
                displayQueue(jobsQueue);

                // 計算排序開始時間
                sortStart = clock();

                // 使用希爾排序，根據 Arrival時間進行排序
                insertionSort(jobsQueue);

                // 計算排序結束時間
                sortEnd = clock();

                // 計算寫檔開始時間
                writeStart = clock();

                // 將排序後的資料寫入檔案
                writeSortedData(jobsQueue, output.c_str());

                // 計算寫檔結束時間
                writeEnd = clock();

                // 顯示執行時間
                cout << "Reading data: " << readEnd - readStart << " clocks (" << fixed << setprecision(2) << double(readEnd - readStart) * 1000.0 / CLOCKS_PER_SEC << " ms)." << endl;
                cout << "Sorting data: " << sortEnd - sortStart << " clocks (" << fixed << setprecision(2) << double(sortEnd - sortStart) * 1000.0 / CLOCKS_PER_SEC << " ms)." << endl;
                cout << "Writing data: " << writeEnd - writeStart << " clocks (" << fixed << setprecision(2) << double(writeEnd - writeStart) * 1000.0 / CLOCKS_PER_SEC << " ms)." << endl;
                cout << "\n";
                cout << "See " << output.c_str() << endl;
                break;
            case 2:
            	cout << "沒有完成";
				break;    
            default:  //避免出現0、1、2以外的指令 
                cout << "Command does not exist!" << endl;
        }
    } while (choice != 0);

    return 0;
}


