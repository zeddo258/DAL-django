// 11127130 蔡淳全 11127134 林日彥 
# include <vector>                              // vector
# include <iostream>                            // cin, cout, ...
# include <iomanip>                             // setprecision
# include <string.h>                            // strcpy, strcat, ...
# include <time.h>                              // clock
# include <math.h>                              // round

using namespace std;                            // 使用標準命名空間 

typedef char Char100[100];                      // 定義100格的字元陣列為Char100 

struct Node {                                   // structure of process
    vector <int> data;                          // process data
    Node* next;                                 // next Node
};

class ProcessQueue {
	private:
		Node *qfront;                           // linked list of Queue (front)
		Node *qback;                            // linked list of Queue (back)
		int qsize, qmax;                        // current size & max size of queue
		
	public:
		int availTime;                          // the time when CPU is available
		
		ProcessQueue() {                        // constructor
			qfront = NULL;                      // linked list of Queue (front)
			qback = NULL;                       // linked list of Queue (back)
			qsize = 0, qmax = 3, availTime = 0; // current size & max size of queue & the time when CPU is available
		}
	
		bool isEmpty() {                        // check whether the queue is empty
			if (qfront == NULL) {
				return true;
			}
			
			return false;
		}
	
		int length() {                          // get the queue's current length
			return qsize;
		}
		
		int max() {                             // get the queue's max size
			return qmax;
		}
	
		void getFront(vector <int> &front) {    // get the first element
			front = qfront -> data;
		}
	
		void enQueue(vector <int> newProcess) { // append a new element at the end of the queue
			Node *newptr = new Node;
			newptr->data = newProcess;
			newptr->next = NULL;
			if (isEmpty()) {
				qfront = newptr;
			}
			else {
				qback->next = newptr;
			}
			
			qback = newptr;
			qsize++;
		}
	
		void deQueue() {                       // drop the first element
			if (isEmpty()) {
				return;
			}
			else {
				Node *qtemp = qfront;
				if (qfront == qback) {
					qfront = NULL;
					qback = NULL;
				}
				else {
					qfront = qfront->next;
				}
				
				delete qtemp;
			}
			
			qsize--;
		}
	
		void clearQ() {                       // 清除linked list
			if (qfront == NULL) return;
			Node *qtemp = qfront;
			while (qfront != NULL) {
				if (qtemp->next != NULL) {    // 兩個Node以上
					qtemp = qtemp->next;
					delete qfront;
					qfront = qtemp;
				}
				else {
					delete qfront;
					qfront = NULL;
					qtemp = NULL;
					qback = NULL;
				}
			}
		}
		
		~ProcessQueue() {                    // 解構子(釋放記憶體) clean up
			clearQ();
		}
		
}; // end ProcessQueue

bool load(string fileName, string fileNumber, vector <vector <int> > &process); // 讀檔 
void storeSorted(string fileNumber, vector <vector <int> > process);            // 建排序檔 
void storeOutput(string fileNumber, vector<vector <int> > abortProcess, vector<vector <int> > doneProcess,
				 double averageDelay, double successRate);                      // 建輸出檔 
void shellSort(vector <vector <int> > &process, int n);                         // 希爾排序法 
void clearVector(vector <vector <int> > &process);                              // 清除vector 
void doQueueFront(ProcessQueue &aQueue, vector<vector <int> > &abortProcess,
				  vector<vector <int> > &doneProcess);                          // 執行佇列第一項 

int main() {
	vector<vector <int> > process;                                              // 工作清單 
	string fileNumber;                                                          // 檔案編號 
    bool success, hasFileNumber = false;                                        // 判斷讀檔成功、有無讀入檔案編號 
    double startTime, endTime, readTime, sortTime, writeTime;
    int command;                                                                // 指令 
    
    while (true) {
	    cout << endl << "**** Simulate FIFO Queues by SQF *****";
	    cout << endl << "* 0. Quit                            *";
	    cout << endl << "* 1. Sort a file                     *";
	    cout << endl << "* 2. Simulate one FIFO queue         *";
	    cout << endl << "**************************************";
		cout << endl << "Input a command(0, 1, 2): ";
		cin >> command;                                                         // get command 
		
		if (command == 1) {
			cout << endl << "Input a file number: ";
			cin >> fileNumber;                                                  // get file number 
			hasFileNumber = true;
		    startTime = clock();
		    success = load("input", fileNumber, process);                       // 讀檔 
		    endTime = clock();
		    readTime = endTime - startTime;                                     // 讀檔時間 
		    
		    if (success) {
			    cout << endl << "\tOID\tArrival\tDuration\tTimeOut" << endl;
			    // 印出原始資料 
				for(int i = 0; i < process.size(); i++) {
					Char100 serialNumber = "(", temp;
					sprintf(temp, "%d", i + 1);
					strcat(serialNumber, temp);
					strcat(serialNumber, ")");					
					cout << serialNumber << "\t";
					
					for(int j = 0; j < 4; j++) {
						cout << process[i][j] << "\t";
					}
					
					cout << endl;
				}
			
				startTime = clock();
				shellSort(process, process.size());                             // 希爾排序 
			    endTime = clock();
			    sortTime = endTime - startTime;                                 // 排序時間 
			    
			    startTime = clock();
				storeSorted(fileNumber, process);                               // 建排序檔 
			    endTime = clock();
			    writeTime = endTime - startTime;                                // 建檔時間 
			    
			    // 印出讀檔、排序、建檔時間 
			    cout << endl << "Reading data: " << readTime << " clocks (" << fixed << setprecision(2) << round(readTime * 100) / 100 << " ms).";
			    cout.unsetf( ios::fixed );
			    cout << endl << "Sorting data: " << sortTime << " clocks (" << fixed << setprecision(2) << round(sortTime * 100) / 100 << " ms).";
			    cout.unsetf( ios::fixed );
			    cout << endl << "Writing data: " << writeTime << " clocks (" << fixed << setprecision(2) << round(writeTime * 100) / 100 << " ms)." << endl;
			    cout.unsetf( ios::fixed );
			    
				cout << endl << "See sorted" << fileNumber << ".txt" << endl;
				clearVector(process);                                           // 清空工作清單 
			}
		} 
		else if (command == 2) {
			if (!hasFileNumber) {
				cout << endl << "Input a file number: ";
				cin >> fileNumber;	                                            // get file number 
				hasFileNumber = true;			
			}
			
			success = load("sorted", fileNumber, process);                      // 讀入已排序檔 
			
			if (success){
				cout << endl << "The simulation is running...";				
				ProcessQueue aQueue;                                            // 佇列 
				vector<vector <int> > abortProcess;                             // 取消清單 
				vector<vector <int> > doneProcess;                              // 完成清單 
				bool processDone;
				
				// process[i][0]:工作編號, process[i][1]:進入時間, process[i][2]:持續時間, process[i][3]:逾時時間 
				for (int i = 0; i < process.size(); i++) {
					processDone = false;
					while (process[i][1] >= aQueue.availTime) {
						if (!aQueue.isEmpty()) {
							doQueueFront(aQueue, abortProcess, doneProcess);    // 執行佇列中第一項工作 
						}
						else {
							// 直接執行並完成該工作，加入完成清單 
							aQueue.availTime = process[i][1] + process[i][2];   // CPU空閒時間設為該工作完成時間 
							processDone = true;	
							vector <int> done;
							done.push_back(process[i][0]);
							done.push_back(aQueue.availTime);                   // 完成時間  
							done.push_back(0);                                  // 無延遲 
							doneProcess.push_back(done);						
						}						
					}
					
					if (!processDone) {                                         // 該工作尚未完成 
						if (aQueue.length() < aQueue.max()) {                   // 佇列未滿 
							aQueue.enQueue(process[i]);                         // 排進佇列 
						}
						else {                                                  // 佇列已滿 
							// 直接取消該工作，加入取消清單 
							vector <int> abort;
							abort.push_back(process[i][0]);
							abort.push_back(process[i][1]);                     // 取消時間為該工作的進入時間 
							abort.push_back(0);                                 // 無延遲  
							abortProcess.push_back(abort);
						}
					}					
				}
				
				// 清空佇列 
				while (!aQueue.isEmpty()) {
					doQueueFront(aQueue, abortProcess, doneProcess);					
				}				
					
				double averageDelay, totalDelay = 0, successRate;
				// 計算總延遲時間 
				for (int i = 0; i < abortProcess.size(); i++) {
					totalDelay = totalDelay + abortProcess[i][2];
				}								
				
				for (int i = 0; i < doneProcess.size(); i++) {
					totalDelay = totalDelay + doneProcess[i][2];
				}
				
				averageDelay = round(totalDelay / (abortProcess.size() + doneProcess.size()) * 100) / 100; // 平均延遲時間 
				successRate = round(100 * (double)doneProcess.size() / (abortProcess.size() + doneProcess.size()) * 100) / 100; // 成功率 
				
				storeOutput(fileNumber, abortProcess, doneProcess, averageDelay, successRate); // 建立output檔 
														
				cout << endl << "See output" << fileNumber << ".txt" << endl;
				clearVector(process);                                           // 清空工作清單  
			}
		}   
		else if (command == 0) {
			break;
		}
		else {
			cout << endl << "Command does not exist!" << endl;
		}
	}
		
	return 0;
}

bool load(string fileName, string fileNumber, vector <vector <int> > &process) { // 讀檔 
	string completeFileName = fileName + fileNumber + ".txt";
	Char100 trash;
	FILE * fp;
	fp = fopen(completeFileName.c_str(), "r");
	
	if (fp == NULL) {                          // 讀檔失敗 
		cout << endl << "### " << completeFileName << " does not exist! ###" << endl;
		return false;
	}
	
	fgets(trash, 100, fp);                     // 把第一行讀完 
	int temp[4], data[4];
	bool first = true;
	while(!feof(fp)) {                         // 讀到EOF為止 
		fscanf(fp, "%d	%d	%d	%d", &data[0], &data[1], &data[2], &data[3]);
		fgets(trash, 100, fp);	
		
	
		if (first || !(temp[0] == data[0] && temp[1] == data[1] && temp[2] == data[2] && temp[3] == data[3])) {
			vector<int> row;
			first = false;
			for (int i = 0; i < 4; i++) {
				row.push_back(data[i]);
				temp[i] = data[i];
			}

			process.push_back(row);
		}
	}
	
	fclose(fp);
	return true;
}

void storeSorted(string fileNumber, vector <vector <int> > process) { // 建排序檔 
	string newFileName = "sorted" + fileNumber + ".txt";
	string firstLine = "OID\tArrival\tDuration\tTimeOut\n";
	FILE * fp;
	fp = fopen(newFileName.c_str(), "w+"); 
	fwrite(firstLine.c_str(), sizeof(char), 29, fp);
	
	for(int i = 0; i < process.size(); i++) {
		for(int j = 0; j < 4; j++) {
			Char100 buff;
			sprintf(buff, "%d", process[i][j]);
			fwrite(buff, sizeof(char), strlen(buff), fp);
			if (j < 3) fwrite("\t", sizeof(char), 1, fp);
		}
		
		fwrite("\n", sizeof(char), 1, fp);
	}
	
	fclose(fp);   
}

void storeOutput(string fileNumber, vector<vector <int> > abortProcess, vector<vector <int> > doneProcess,
				 double averageDelay, double successRate) { // 建輸出檔 
	string newFileName = "output" + fileNumber + ".txt";
	string abortFirstLine = "\t[Abort Jobs]\n";
	string abortSecondLine = "\tOID\tAbort\tDelay\n";
	FILE * fp;
	fp = fopen(newFileName.c_str(), "w+");	
	fwrite(abortFirstLine.c_str(), sizeof(char), abortFirstLine.length(), fp);
	fwrite(abortSecondLine.c_str(), sizeof(char), abortSecondLine.length(), fp);
	
	for(int i = 0; i < abortProcess.size(); i++) {
		Char100 buff = "[", temp;
		sprintf(temp, "%d", i + 1);
		strcat(buff, temp);
		strcat(buff, "]");	
		fwrite(buff, sizeof(char), strlen(buff), fp);
		fwrite("\t", sizeof(char), 1, fp);
		for(int j = 0; j < 3; j++) {		
			sprintf(buff, "%d", abortProcess[i][j]);
			fwrite(buff, sizeof(char), strlen(buff), fp);
			if (j < 2) fwrite("\t", sizeof(char), 1, fp);
		}
		
		fwrite("\n", sizeof(char), 1, fp);
	}	
	
	string doneFirstLine = "\t[Jobs Done]\n";
	string doneSecondLine = "\tOID\tDeparture\tDelay\n";
	fwrite(doneFirstLine.c_str(), sizeof(char), doneFirstLine.length(), fp);
	fwrite(doneSecondLine.c_str(), sizeof(char), doneSecondLine.length(), fp);
	
	for(int i = 0; i < doneProcess.size(); i++) {
		Char100 buff = "[", temp;
		sprintf(temp, "%d", i + 1);
		strcat(buff, temp);
		strcat(buff, "]");	
		fwrite(buff, sizeof(char), strlen(buff), fp);
		fwrite("\t", sizeof(char), 1, fp);
		for(int j = 0; j < 3; j++) {		
			sprintf(buff, "%d", doneProcess[i][j]);
			fwrite(buff, sizeof(char), strlen(buff), fp);
			if (j < 2) fwrite("\t", sizeof(char), 1, fp);
		}
		
		fwrite("\n", sizeof(char), 1, fp);
	}
	
	Char100 buff = "[Average Delay]\t", temp;
	sprintf(temp, "%.2lf", averageDelay);
	strcat(buff, temp);
	strcat(buff, " ms\n");
	fwrite(buff, sizeof(char), strlen(buff), fp);
	
	strcpy(buff, "[Success Rate]\t");
	sprintf(temp, "%.2lf", successRate);
	strcat(buff, temp);
	strcat(buff, " %\n");
	fwrite(buff, sizeof(char), strlen(buff), fp);
	
	fclose(fp);
}

void shellSort(vector <vector <int> > &process, int n) { // 希爾排序法 
	for (int h = n / 2; h > 0; h = h / 2) {
		for (int unsorted = h; unsorted < n; unsorted++) {
			int loc = unsorted;
			vector <int> nextItem = process[unsorted];
			// arrival time大的工作排後面；arrival time相同的兩筆工作，編號大的排後面 
			for(; (loc >= h) && ((process[loc - h][1] > nextItem[1]) || 
				((process[loc - h][1] == nextItem[1]) && process[loc - h][0] > nextItem[0])); loc = loc - h) {
				process[loc] = process[loc - h];
			}
			
			process[loc] = nextItem;
		}
	}
}

void clearVector(vector <vector <int> > &process) { // 清除vector 
	for (int i = 0; i < process.size(); i++) {
		process[i].clear();
	}
	
	process.clear();
}

void doQueueFront(ProcessQueue &aQueue, vector<vector <int> > &abortProcess, vector<vector <int> > &doneProcess) { // 執行佇列第一項工作 
	vector <int> temp;
	aQueue.getFront(temp);                                // 取得第一項工作的資訊 
	// temp[0]:工作編號, temp[1]:進入時間, temp[2]:持續時間, temp[3]:逾時時間 
	if (aQueue.availTime + temp[2] > temp[3]) {           // 逾時 
		if (aQueue.availTime < temp[3]) {                 // 執行途中發現逾時 
			// 加入取消清單 
			aQueue.availTime = temp[3];                   // CPU空閒時間設為該工作的逾時時間 
			vector <int> abort;
			abort.push_back(temp[0]);
			abort.push_back(temp[3]);                     // 取消時間為逾時時間 
			abort.push_back(temp[3] - temp[1]);           // 延遲為取消時間減進入時間 
			abortProcess.push_back(abort);
		}			
		else {                                            // 還沒開始執行就逾時 
			// 加入取消清單 
			vector <int> abort;
			abort.push_back(temp[0]);
			abort.push_back(aQueue.availTime);            // 取消時間 
			abort.push_back(aQueue.availTime - temp[1]);  // 延遲為取消時間減進入時間  
			abortProcess.push_back(abort);							
		}					
	}
	else {                                                // 沒有逾時  
		// 加入完成清單 
		vector <int> done;
		done.push_back(temp[0]);
		done.push_back(aQueue.availTime + temp[2]);       // 完成時間為CPU空閒時間加工作持續時間 
		done.push_back(aQueue.availTime - temp[1]);       // 延遲為開始執行該工作的時間減進入時間 
		doneProcess.push_back(done);
		aQueue.availTime = aQueue.availTime + temp[2];    // CPU空閒時間設為該工作完成時間  
	}
	
	aQueue.deQueue();	                                  // 將工作從佇列中移除 
}
