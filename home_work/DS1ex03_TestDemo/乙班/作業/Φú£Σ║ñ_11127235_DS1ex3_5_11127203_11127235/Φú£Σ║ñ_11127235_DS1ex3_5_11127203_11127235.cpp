// 11127203 郭力嫚 11127235 許倚菁 
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <iomanip> 

using namespace std;

typedef struct jT {
	int OID;
    int Arrival;
    int Duration;
    int TimeOut;
} jobType;

struct Queue {
    int OID;
    int Abort;
    int Delay;
};

struct Done {
    int OID;
    int Departure;
    int Delaytime;
};

// 自訂義佇列類別
template <typename T> //定義一個模板，其中的 T 是一個類型參數，表示這個模板可以應用於不同的數據類型
class MyQueue {
private:
    vector<T> elements;

public:
    // 將元素推送到佇列的末尾
    void enqueue(const T& element) {
        elements.push_back(element);
    }

    // 從佇列的頭部移除元素
    void dequeue() {
        if (!isEmpty()) {
            elements.erase(elements.begin());
        }
    }

    // 存取佇列的頭部元素
    T front() const {
        if (!isEmpty()) {
            return elements.front();
        }
    }


    // 檢查佇列是否為空
    bool isEmpty() const {
        return elements.empty();
    }

    // 取得佇列中的元素數量
    size_t size() const {
        return elements.size();
    }
};

class CPUQueueSimulation {
public:
    CPUQueueSimulation(const string& inputFile, const string& outputFile)
        : inputFile(inputFile), outputFile(outputFile) {}

    void simulate() {
        bool isSuccess = readData();
        if (isSuccess == true) {
          
          jT frontJob = inputQueue.front(); // 用 frontJob儲存 inputQueue的頭部元素 
          tofinish.enqueue(frontJob); // 將 frontJob複製到 tofinish
          int currentTime = frontJob.Arrival; // 現在時間(currentTime)等於 tofinish的到達時間 
          inputQueue.dequeue();  // 將頭部元素移除 
        
          currentTime = processFirst(currentTime, tofinish, inputQueue, cpuQueue, doneList, abortList); // 處裡第一筆資料與要放進Q的內容，
		                                                                                             //返回現在時間(currentTime)

        
          processLasts(currentTime, tofinish, inputQueue, cpuQueue, doneList, abortList); // 處裡Q裡的資料與剩下的資料 
          cout << endl << "The simulation is running...\n" ;
          cout << "See " << outputFile << "\n";
          writeOutput(doneList, abortList); // 印出 
          
        }
    }
    
private:
    string inputFile;
    string outputFile;
    
    MyQueue<jT> inputQueue; // 輸入的資料 
    MyQueue<jT> tofinish; // 準備完成的資料 
    MyQueue<jT> cpuQueue; // Q 
    vector<Done> doneList; // 已完成的資料列表 
    vector<Queue> abortList; // 已取消的資料列表  

    bool success = true;  
    bool readData() {
        ifstream inFile(inputFile.c_str());
        if (!inFile.is_open()) {
            cout << endl << "### " << inputFile << " does not exist!" << " ###" << endl;
            return false; 
        } else {
          
          jT tempJob;
          string header;
          getline(inFile, header);
          while (inFile >> tempJob.OID >> tempJob.Arrival >> tempJob.Duration >> tempJob.TimeOut) {
              // 將資料加入inputQueue
              inputQueue.enqueue(tempJob);
          }

          inFile.close();
          return true;
    	}
    	
	}

    int processFirst(int currentTime, MyQueue<jT>& tofinish, MyQueue<jT>& inputQueue, MyQueue<jT>& cpuQueue,
                 vector<Done>& doneList, vector<Queue>& abortList) {
    	jT frontJob = tofinish.front();
    	bool hasonedone = false;
    	int finishtime = currentTime + frontJob.Duration;

    	while (!inputQueue.isEmpty() && !hasonedone) { //處理第一筆要加入doneList的資料 
        	if (finishtime <= frontJob.TimeOut) {
          		doneList.push_back({frontJob.OID, finishtime, 0});
           		currentTime = finishtime;
            	tofinish.dequeue();
            	hasonedone = true;
        	} 
			
			else {
            	tofinish.dequeue();
            	if (!inputQueue.isEmpty()) {
                	frontJob = inputQueue.front();
               		finishtime = frontJob.Arrival + frontJob.Duration;
                	inputQueue.dequeue();
            	} 
				
				else {
                	break;
            	}
        	}
    	}

    	frontJob = inputQueue.front();
    	while (!inputQueue.isEmpty() && (cpuQueue.size() < 3 || currentTime > frontJob.Arrival)) { // 處裡先要加入Q的三筆資料 
        	if (cpuQueue.size() < 3) {
            	cpuQueue.enqueue(frontJob);
            	inputQueue.dequeue();
        	} 
			
			else {
            	abortList.push_back({frontJob.OID, frontJob.Arrival, 0});
            	inputQueue.dequeue();
        	}

        	frontJob = inputQueue.front();
    	}

    	return currentTime;
	}

   
	void processLasts(int currentTime, MyQueue<jT>& tofinish, MyQueue<jT>& inputQueue, MyQueue<jT>& cpuQueue,
                  vector<Done>& doneList, vector<Queue>& abortList) { // 處裡剩下的資料 
        
    	while (!cpuQueue.isEmpty()) { 
        	jT frontJob = cpuQueue.front(); //複製Q 
        	tofinish.enqueue(frontJob); //將Q的第一筆資料加入待完成 
        	cpuQueue.dequeue(); //刪掉Q的頭 
        	if ( !inputQueue.isEmpty() ) { // 將剩下的資料加入Q 
        		jT InputfrontJob = inputQueue.front();
        		while (currentTime <= InputfrontJob.Arrival && cpuQueue.size() < 3) {  
                	cpuQueue.enqueue(InputfrontJob);
                	inputQueue.dequeue();

                	if (!inputQueue.isEmpty()) {
                    	InputfrontJob = inputQueue.front();
                	} 
					
					else {
                    	break;
                	}
                	
            	}
			}
			
			frontJob = tofinish.front();
        	int finishtime = currentTime + frontJob.Duration; 

        	if (finishtime <= frontJob.TimeOut) { // 處裡待完成資料，判斷要將frontJob放到Abort List還是Done List 
        		if ( currentTime < frontJob.Arrival ) {
        			currentTime = frontJob.Arrival ;
        			finishtime = currentTime + frontJob.Duration;
        			doneList.push_back({frontJob.OID, finishtime, currentTime - frontJob.Arrival});
            		currentTime = finishtime;
				}
				
				else {
					doneList.push_back({frontJob.OID, finishtime, currentTime - frontJob.Arrival});
            		currentTime = finishtime;
				}
        	} 
			
			else {
				int abortTime = 0 ;
            	if (currentTime > frontJob.TimeOut) abortTime = currentTime ;
				else abortTime = frontJob.TimeOut;		
                abortList.push_back({frontJob.OID, abortTime , abortTime - frontJob.Arrival});
                currentTime = abortTime ;
        	}
        	
        	tofinish.dequeue();
        	jT CheckfrontJob = inputQueue.front(); // 檢查加完Q並更新currentTime後剩下的資料有無逾時 
        	if ( currentTime > CheckfrontJob.Arrival ) {
        		
        		while ( currentTime < CheckfrontJob.Arrival || !inputQueue.isEmpty() ){
        			abortList.push_back({CheckfrontJob.OID, CheckfrontJob.Arrival , 0});
            		inputQueue.dequeue();
            		CheckfrontJob = inputQueue.front();
            		if ( inputQueue.isEmpty() || currentTime == CheckfrontJob.Arrival ) {
            			break ;
					}
            		
				}
				
			}
    	}

	}





    void writeOutput(const vector<Done>& doneList, const vector<Queue>& abortList) {
    	if (success == true) { 
          ofstream outputFileOut(outputFile.c_str()); // 打開檔案以寫入處理後的數據  


          // 將處理後的數據寫入檔案
          outputFileOut << "\t[Abort Jobs]\n";
          outputFileOut << "\tOID\tAbort\tDelay\n";

		  int i = 1 ;
          for (vector<Queue>::const_iterator it = abortList.begin(); it != abortList.end(); ++it) {
              const Queue& queue = *it;
              outputFileOut << "[" << i << "]\t" << queue.OID << '\t'<< queue.Abort << '\t' << queue.Delay << "\n";
              i++ ;
          }

		  // 將處理後的數據寫入檔案
          outputFileOut << "\t[Jobs Done]\n";
          outputFileOut << "\tOID\tDeparture\tDelay\n";

		  i = 1 ;
          for (vector<Done>::const_iterator it = doneList.begin(); it != doneList.end(); ++it) {
              const Done& done = *it;
              outputFileOut << "[" << i << "]\t"  << done.OID << '\t' << done.Departure << '\t' << done.Delaytime << "\n";
              i++ ;
          }

          // 輸入平均延遲和成功比例 
          double totalDelay = 0;
          int successCount = doneList.size();

          // 遍歷資料進行計算延遲
          for (vector<Done>::const_iterator it = doneList.begin(); it != doneList.end(); ++it) {
              const Done& done = *it;
              totalDelay += done.Delaytime;
          }

          // 遍歷資料進行計算延遲 
          for (vector<Queue>::const_iterator it = abortList.begin(); it != abortList.end(); ++it) {
              const Queue& queue = *it;
              totalDelay += queue.Delay;
          }

		   // 計算平均延遲(毫秒：ms)和成功比例(單位：%) 
          double averageDelay = totalDelay / (doneList.size() + abortList.size());
          double successRate = (static_cast<double>(successCount) / (inputQueue.size() + doneList.size() + abortList.size())) * 100.0;

		  // 輸出格式，使用 setprecision(2) 來設定小數點第2位數
          outputFileOut << "[Average Delay]\t" << fixed << setprecision(2) << averageDelay << " ms\n";
          outputFileOut << "[Success Rate]\t" << fixed << setprecision(2) << successRate << " %\n";

          outputFileOut.close(); // 關閉檔案 
        
        }
        
    }    
};

class JobList {
public:
    JobList(const string& inputFile, const string& outputFile)
     : inputFile(inputFile), outputFile(outputFile) {}  // 正確初始化成員變數 

    bool success = true;
    void readData() {
      // 計算讀檔開始時間
      readStart = clock();

      // 打開檔案
      ifstream inFile(inputFile.c_str()); // 使用 inputFile

      if (!inFile) {
          cout << endl << "### " << inputFile << " does not exist!" << " ###" << endl;
          success = false;
          return;
      } else {
      	success = true;
      	// 讀取檔案中的數據到動態陣列中
        jobType record;
        string header;
        getline(inFile, header); // 讀取並忽略第一行（表頭）
        while (inFile >> record.OID >> record.Arrival >> record.Duration >> record.TimeOut) {
          dynamicArray.push_back(record);
        }
        
        cout << endl << "        OID\tArrival\tDuration\tTimeOut\n";
        int x = 1 ;
        for (size_t i = 0; i < dynamicArray.size(); ++i) {
      	  cout <<"(" << x << ")\t" << dynamicArray[i].OID << '\t' << dynamicArray[i].Arrival << '\t' << dynamicArray[i].Duration << '\t' << dynamicArray[i].TimeOut << '\n';
          x ++;
        }
        // 關閉檔案
        inFile.close();
  
        // 計算讀檔結束時間
        readEnd = clock();
	  }
    }

    void sortData() {

        // 使用希爾排序對動態陣列進行排序
        if (success == true) {
          // 計算排序開始時間
          sortStart = clock();
          shellSort(dynamicArray);
          // 計算排序結束時間
          sortEnd = clock();
        }

    }

    void writeData() {

      if (success == true) {
      	// 計算寫檔開始時間
        writeStart = clock();
        // 打開檔案以寫入排序後的數據
	    ofstream file(outputFile.c_str());  // 使用類別成員變數 outputFile
	    // 檢查檔案是否成功打開
        if (!file.is_open()) {
          cout << endl << "### " << outputFile << " doesn't exist!" << " ###" << endl;
          return;
        }
        file << "OID\tArrival\tDuration\tTimeOut\n";
        for (size_t i = 0; i < dynamicArray.size(); ++i) {
          file << dynamicArray[i].OID << '\t' << dynamicArray[i].Arrival << '\t' << dynamicArray[i].Duration << '\t' << dynamicArray[i].TimeOut << '\n';
        }
        // 關閉檔案
        file.close();

        // 計算寫檔結束時間
        writeEnd = clock();
	  } 
      
      

    }

    void displayExecutionTimes() {
      
	  // 計算clock 
      clock_t readClock = readEnd - readStart;  
      clock_t sortClock = sortEnd - sortStart;
      clock_t writeClock = writeEnd - writeStart;

      // 計算並顯示執行時間，以毫秒為單位
      double readMilliseconds = static_cast<double>(readEnd - readStart) / (CLOCKS_PER_SEC/1000 );
      double sortMilliseconds = static_cast<double>(sortEnd - sortStart) / (CLOCKS_PER_SEC/1000 );
      double writeMilliseconds = static_cast<double>(writeEnd - writeStart) / (CLOCKS_PER_SEC/1000 );

      //'fixed << setprecision(2)'表示取到小數點後兩位 
      if ( success == true ) {
        cout << endl << "Reading data: " << readClock << " clocks (" << fixed << setprecision(2) << readMilliseconds << " ms)\n";
        cout << "Sorting data: " << sortClock << " clocks (" << fixed << setprecision(2)<< sortMilliseconds << " ms)\n";
        cout << "Writing data: " << writeClock << " clocks (" << fixed << setprecision(2)<< writeMilliseconds  << " ms)\n";
        cout << endl << "See " << outputFile << "\n";
      }
    }

private:
    string inputFile; //儲存輸入檔案和輸出檔案的檔案名稱
    string outputFile; //儲存輸入檔案和輸出檔案的檔案名稱 
    vector<jT> dynamicArray; //儲存'jobType'結構的記錄
    clock_t readStart, readEnd, sortStart, sortEnd, writeStart, writeEnd; //儲存不同階段的時間

    // 希爾排序
   void shellSort(vector<jT>& arr) {
     int n = arr.size();

     for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            jT temp = arr[i];
            int j;

            // 如果Arrival相同，則比較OID
            for (j = i; j >= gap && (arr[j - gap].Arrival > temp.Arrival || 
                                     (arr[j - gap].Arrival == temp.Arrival && arr[j - gap].OID > temp.OID)); 
                 j -= gap) {
                arr[j] = arr[j - gap];
            }

            arr[j] = temp;
        }
      }
    }


};
int main() {
    string command ; // 用於指令輸入 
    bool first = true;
    string fileName; 
    string sorted ;
    string output ;
    string fileNumber ;
    do  {  
        cout << endl ;
        cout << "**** Simulate FIFO Queues by SQF *****\n";
        cout << "* 0.Quit                             *\n";
        cout << "* 1.Sort a file                      *\n";
        cout << "* 2.Simulate one FIFO queue          *\n";
        cout << "**************************************\n";
        cout << "Input a command(0,1,2):";
        cin >> command;
		 
        if (command == "1" || command == "2") {
            if ( command == "1") {
              cout << endl << "Input a file number:";
              cin >> fileNumber; 
              stringstream ss; //創建一個字符串流 
              ss << "input" << fileNumber << ".txt"; //將輸入檔案號碼拼接成檔案名 
              fileName = ss.str();  //調用'ss.str()將檔案名稱存在'fileName'中
              stringstream ss2;
              ss2 << "sorted" << fileNumber << ".txt"; 
              sorted = ss2.str(); // 修正此處
              JobList jobList(fileName, sorted);
              jobList.readData();
              jobList.sortData();
              jobList.writeData();
              jobList.displayExecutionTimes();   
              first = false;
            } else  if ( command == "2") {
              if ( first == true ) {
              	cout << endl << "Input a file number:";
                cin >> fileNumber;
                stringstream ss2;
                ss2 << "sorted" << fileNumber << ".txt"; 
                sorted = ss2.str(); // 修正此處
                stringstream ss3;
                ss3 << "output" << fileNumber << ".txt";
                output = ss3.str(); // 修正此處
                first = false;
              } else {
                stringstream ss3;
                ss3 << "output" << fileNumber << ".txt";
                output = ss3.str(); // 修正此處
              }
              
              CPUQueueSimulation simulation(sorted, output);
              simulation.simulate(); 
            }
        } else if (command == "0") {
            break;
        } else {
            cout << endl << "Command does not exist!" << endl;
        }
    } while (true);
    system("pause");
    return 0;
}

