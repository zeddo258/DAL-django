// 11127130 ���E�� 11127134 �L��� 
# include <vector>                              // vector
# include <iostream>                            // cin, cout, ...
# include <iomanip>                             // setprecision
# include <string.h>                            // strcpy, strcat, ...
# include <time.h>                              // clock
# include <math.h>                              // round

using namespace std;                            // �ϥμзǩR�W�Ŷ� 

typedef char Char100[100];                      // �w�q100�檺�r���}�C��Char100 

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
		
		void get(int index, vector <int> &info) { // get the element at index 
			Node * cur = qfront;
			for (int i = 0; i < index; i++) {
				cur = cur -> next;
			}
			
			info = cur -> data;
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
	
		void clearQ() {                       // �M��linked list
			if (qfront == NULL) return;
			Node *qtemp = qfront;
			while (qfront != NULL) {
				if (qtemp->next != NULL) {    // ���Node�H�W
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
		
		~ProcessQueue() {                    // �Ѻc�l(����O����) clean up
			clearQ();
		}
		
}; // end ProcessQueue

bool load(string fileName, string fileNumber, vector <vector <int> > &process); // Ū�� 
void storeSorted(string fileNumber, vector <vector <int> > process);            // �رƧ��� 
void storeOutput(string fileNumber, vector<vector <int> > abortProcess, vector<vector <int> > doneProcess,
				 double averageDelay, double successRate);                      // �ؿ�X�� 
void storeDouble(string fileNumber, vector<vector <int> > abortProcess, vector<vector <int> > doneProcess,
				 double averageDelay, double successRate);                      // ��double��  
void shellSort(vector <vector <int> > &process, int n);                         // �ƺ��ƧǪk 
void clearVector(vector <vector <int> > &process);                              // �M��vector 
void doQueueFront(ProcessQueue &aQueue, vector<vector <int> > &abortProcess,
				  vector<vector <int> > &doneProcess);                          // �����C�Ĥ@�� 
void doQueuesFront(ProcessQueue aQueue[], vector<vector <int> > &abortProcess,
				   vector<vector <int> > &doneProcess, int index);              // �����C�Ĥ@��(�h��C) 

int main() {
	vector<vector <int> > process;                                              // �u�@�M�� 
	string fileNumber;                                                          // �ɮ׽s�� 
    bool success, hasFileNumber = false;                                        // �P�_Ū�ɦ��\�B���LŪ�J�ɮ׽s�� 
    double startTime, endTime, readTime, sortTime, writeTime;
    int command;                                                                // ���O 
    
    while (true) {
	    cout << endl << "**** Simulate FIFO Queues by SQF *****";
	    cout << endl << "* 0. Quit                            *";
	    cout << endl << "* 1. Sort a file                     *";
	    cout << endl << "* 2. Simulate one FIFO queue         *";
	    cout << endl << "* 3. Simulate two queues by SQF      *";
	    cout << endl << "**************************************";
		cout << endl << "Input a command(0, 1, 2, 3): ";
		cin >> command;                                                         // get command 
		
		if (command == 1) {
			cout << endl << "Input a file number: ";
			cin >> fileNumber;                                                  // get file number 
			hasFileNumber = true;
		    startTime = clock();
		    success = load("input", fileNumber, process);                       // Ū�� 
		    endTime = clock();
		    readTime = endTime - startTime;                                     // Ū�ɮɶ� 
		    
		    if (success) {
			    cout << endl << "\tOID\tArrival\tDuration\tTimeOut" << endl;
			    // �L�X��l��� 
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
				shellSort(process, process.size());                             // �ƺ��Ƨ� 
			    endTime = clock();
			    sortTime = endTime - startTime;                                 // �ƧǮɶ� 
			    
			    startTime = clock();
				storeSorted(fileNumber, process);                               // �رƧ��� 
			    endTime = clock();
			    writeTime = endTime - startTime;                                // ���ɮɶ� 
			    
			    // �L�XŪ�ɡB�ƧǡB���ɮɶ� 
			    cout << endl << "Reading data: " << readTime << " clocks (" << fixed << setprecision(2) << round(readTime * 100) / 100 << " ms).";
			    cout.unsetf( ios::fixed );
			    cout << endl << "Sorting data: " << sortTime << " clocks (" << fixed << setprecision(2) << round(sortTime * 100) / 100 << " ms).";
			    cout.unsetf( ios::fixed );
			    cout << endl << "Writing data: " << writeTime << " clocks (" << fixed << setprecision(2) << round(writeTime * 100) / 100 << " ms)." << endl;
			    cout.unsetf( ios::fixed );
			    
				cout << endl << "See sorted" << fileNumber << ".txt" << endl;
				clearVector(process);                                           // �M�Ťu�@�M�� 
			}
		} 
		else if (command == 2) {
			if (!hasFileNumber) {
				cout << endl << "Input a file number: ";
				cin >> fileNumber;	                                            // get file number 
				hasFileNumber = true;			
			}
			
			success = load("sorted", fileNumber, process);                      // Ū�J�w�Ƨ��� 
			
			if (success){
				cout << endl << "The simulation is running...";				
				ProcessQueue aQueue;                                            // ��C 
				vector<vector <int> > abortProcess;                             // �����M�� 
				vector<vector <int> > doneProcess;                              // �����M�� 
				bool processDone;
				
				// process[i][0]:�u�@�s��, process[i][1]:�i�J�ɶ�, process[i][2]:����ɶ�, process[i][3]:�O�ɮɶ� 
				for (int i = 0; i < process.size(); i++) {
					processDone = false;
					while (process[i][1] >= aQueue.availTime) {
						if (!aQueue.isEmpty()) {
							doQueueFront(aQueue, abortProcess, doneProcess);    // �����C���Ĥ@���u�@ 
						}
						else {
							// ��������ç����Ӥu�@�A�[�J�����M�� 
							aQueue.availTime = process[i][1] + process[i][2];   // CPU�Ŷ��ɶ��]���Ӥu�@�����ɶ� 
							processDone = true;	
							vector <int> done;
							done.push_back(process[i][0]);
							done.push_back(aQueue.availTime);                   // �����ɶ�  
							done.push_back(0);                                  // �L���� 
							doneProcess.push_back(done);						
						}						
					}
					
					if (!processDone) {                                         // �Ӥu�@�|������ 
						if (aQueue.length() < aQueue.max()) {                   // ��C���� 
							aQueue.enQueue(process[i]);                         // �ƶi��C 
						}
						else {                                                  // ��C�w�� 
							// ���������Ӥu�@�A�[�J�����M�� 
							vector <int> abort;
							abort.push_back(process[i][0]);
							abort.push_back(process[i][1]);                     // �����ɶ����Ӥu�@���i�J�ɶ� 
							abort.push_back(0);                                 // �L����  
							abortProcess.push_back(abort);
						}
					}					
				}
				
				// �M�Ŧ�C 
				while (!aQueue.isEmpty()) {
					doQueueFront(aQueue, abortProcess, doneProcess);					
				}				
					
				double averageDelay, totalDelay = 0, successRate;
				// �p���`����ɶ� 
				for (int i = 0; i < abortProcess.size(); i++) {
					totalDelay = totalDelay + abortProcess[i][2];
				}								
				
				for (int i = 0; i < doneProcess.size(); i++) {
					totalDelay = totalDelay + doneProcess[i][2];
				}
				
				averageDelay = round(totalDelay / (abortProcess.size() + doneProcess.size()) * 100) / 100; // ��������ɶ� 
				successRate = round(100 * (double)doneProcess.size() / (abortProcess.size() + doneProcess.size()) * 100) / 100; // ���\�v 
				
				storeOutput(fileNumber, abortProcess, doneProcess, averageDelay, successRate); // �إ�output�� 
														
				cout << endl << "See output" << fileNumber << ".txt" << endl;
				clearVector(process);                                           // �M�Ťu�@�M��  
			}
		}
		else if (command == 3) {
			if (!hasFileNumber) {
				cout << endl << "Input a file number: ";
				cin >> fileNumber;	                                            // get file number 
				hasFileNumber = true;			
			}
			
			success = load("sorted", fileNumber, process);                      // Ū�J�w�Ƨ��� 
			
			if (success){
				cout << endl << "The simulation is running...";				
				ProcessQueue aQueue[2];                                         // ��C 
				vector<vector <int> > abortProcess;                             // �����M�� 
				vector<vector <int> > doneProcess;                              // �����M�� 
				bool processDone;
				bool avail[2];                                                  // cpu�O�_�Ŷ� 
				int cpuNum;                                                     // cpu�s��-1(��C�}�C��index) 
				
				// process[i][0]:�u�@�s��, process[i][1]:�i�J�ɶ�, process[i][2]:����ɶ�, process[i][3]:�O�ɮɶ� 
				for (int i = 0; i < process.size(); i++) {
					processDone = false;
					avail[0] = false;
					avail[1] = false;
					
					int newAvailTime[2], queIndex[2];                           // �w��cpu�Ŷ��ɶ�, ��Cindex 
					// �P�_����cpu�O�Ŷ��� 
					for (int j = 0; j < 2; j++) { 
						queIndex[j] = 0;
						newAvailTime[j] = aQueue[j].availTime;
						while (process[i][1] >= newAvailTime[j] && queIndex[j] < aQueue[j].length()) {
							vector <int> temp;
							aQueue[j].get(queIndex[j], temp);                   // ���o��queIndex[j] + 1���u�@����T 
							// temp[0]:�u�@�s��, temp[1]:�i�J�ɶ�, temp[2]:����ɶ�, temp[3]:�O�ɮɶ� 
							if (newAvailTime[j] + temp[2] > temp[3]) {          // �O�� 
								if (newAvailTime[j] < temp[3]) {                // �u�@����~���|�O�� 
									newAvailTime[j] = temp[3];                  // �w��cpu�Ŷ��ɶ��]���Ӥu�@���O�ɮɶ� 
								}								
							}
							else {                                              // ���|�O��  
								newAvailTime[j] = newAvailTime[j] + temp[2];    // �w��cpu�Ŷ��ɶ��]���Ӥu�@�����ɶ�  
							}
							
							queIndex[j]++;	                                    // �ˬd��C���U�@���u�@  
						}
					
						if (newAvailTime[j] <= process[i][1]) {                 // �u�@�i�Ӯ�cpu�O�Ŷ��� 
							avail[j] = true;
						}
					}
					
					if (avail[0]) {                                             // �@��cpu�Ŷ��Ψ��cpu���Ŷ� 
						cpuNum = 0;
					}
					else if (avail[1]) {                                        // �u���G��cpu�Ŷ� 
						cpuNum = 1;
					}
					else {                                                      // ���S���m 
						if (aQueue[0].length() - queIndex[0] == aQueue[0].max() && aQueue[1].length() - queIndex[1] == aQueue[1].max()) { // ��C���� 
							cpuNum = -1;                                        // �������� 
						}
						else if (aQueue[0].length() - queIndex[0] <= aQueue[1].length() - queIndex[1]) { // �@����C���u 
							cpuNum = 0;
						}
						else {                                                  // �G����C���u 
							cpuNum = 1; 
						}
					}
					
					if (cpuNum != -1) {                                         // �S���Q�������� 
						// ������C���סA����s��������C�A�A��s���u����C�F�Y���פ@�ˡA�h����s�@����C 
						if (aQueue[1 - cpuNum].length() - queIndex[1 - cpuNum] > aQueue[cpuNum].length() - queIndex[cpuNum]) { 
							// ��s��1 - cpuNum + 1�Ӧ�C 
							while (process[i][1] >= aQueue[1 - cpuNum].availTime && !aQueue[1 - cpuNum].isEmpty()) {							
								doQueuesFront(aQueue, abortProcess, doneProcess, 1 - cpuNum);
							}						
							
							// �A��s��cpuNum + 1�Ӧ�C 
							while (process[i][1] >= aQueue[cpuNum].availTime) {
								if (!aQueue[cpuNum].isEmpty()) {							
									doQueuesFront(aQueue, abortProcess, doneProcess, cpuNum);
								}
								else {
									// ��������ç����Ӥu�@�A�[�J�����M�� 
									aQueue[cpuNum].availTime = process[i][1] + process[i][2]; // CPU�Ŷ��ɶ��]���Ӥu�@�����ɶ� 
									processDone = true;	
									vector <int> done;
									done.push_back(process[i][0]);
									done.push_back(cpuNum + 1); 
									done.push_back(aQueue[cpuNum].availTime);   // �����ɶ�  
									done.push_back(0);                          // �L���� 
									doneProcess.push_back(done);						
								}
							}
	
							if (!processDone) {                                            
								aQueue[cpuNum].enQueue(process[i]);             // �ƶi��C 
							}
						}
						else if (aQueue[1 - cpuNum].length() - queIndex[1 - cpuNum] == aQueue[cpuNum].length() - queIndex[cpuNum]) {
							bool queueIsEmpty = false; // �Q��쪺��C�O�Ū� 
							// ��s��1�Ӧ�C 
							while (process[i][1] >= aQueue[0].availTime) {
								if (!aQueue[0].isEmpty()) {							
									doQueuesFront(aQueue, abortProcess, doneProcess, 0); 
								}
								else {
									if (cpuNum == 0) { // �Q��쪺cpu�O�Ĥ@��cpu 
										queueIsEmpty = true;	
									}
									
									break;								
								}
							}
	
							if (!queueIsEmpty && cpuNum == 0) {
								aQueue[0].enQueue(process[i]); // �ƶi��C 
							}
							
							// ��s��2�Ӧ�C 
							while (process[i][1] >= aQueue[1].availTime) {
								if (!aQueue[1].isEmpty()) {							
									doQueuesFront(aQueue, abortProcess, doneProcess, 1);
								}
								else {
									if (cpuNum == 1) { // �Q��쪺cpu�O�ĤG��cpu 
										queueIsEmpty = true;	
									}

									break;
								}
							}
	
							if (!queueIsEmpty && cpuNum == 1) {                                            
								aQueue[1].enQueue(process[i]); // �ƶi��C 
							}
							
							// �Y��쪺��C�ŤF�A�h�Ӥu�@�[�J�����M�� 
							if (queueIsEmpty && cpuNum == 0) { // ���Ĥ@�Ӧ�C�����p 
								aQueue[0].availTime = process[i][1] + process[i][2]; // CPU�Ŷ��ɶ��]���Ӥu�@�����ɶ� 	
								vector <int> done;
								done.push_back(process[i][0]);
								done.push_back(1);
								done.push_back(aQueue[0].availTime);                 // �����ɶ�  
								done.push_back(0);                                   // �L���� 
								doneProcess.push_back(done);								
							}	
							
							// �Y��쪺��C�ŤF�A�h�Ӥu�@�[�J�����M�� 
							if (queueIsEmpty && cpuNum == 1) { // ���ĤG�Ӧ�C�����p 
								aQueue[1].availTime = process[i][1] + process[i][2]; // CPU�Ŷ��ɶ��]���Ӥu�@�����ɶ� 
								vector <int> done;
								done.push_back(process[i][0]);
								done.push_back(2);
								done.push_back(aQueue[1].availTime);                 // �����ɶ�  
								done.push_back(0);                                   // �L���� 
								doneProcess.push_back(done);								
							}						 
						}
						else { // aQueue[1 - cpuNum].length() - queIndex[1 - cpuNum] < aQueue[cpuNum].length() - queIndex[cpuNum]
							// ��s��cpuNum + 1�Ӧ�C 
							while (process[i][1] >= aQueue[cpuNum].availTime) {
								if (!aQueue[cpuNum].isEmpty()) {							
									doQueuesFront(aQueue, abortProcess, doneProcess, cpuNum);
								}
								else {
									// ��������ç����Ӥu�@�A�[�J�����M�� 
									aQueue[cpuNum].availTime = process[i][1] + process[i][2]; // CPU�Ŷ��ɶ��]���Ӥu�@�����ɶ� 
									processDone = true;	
									vector <int> done;
									done.push_back(process[i][0]);
									done.push_back(cpuNum + 1);
									done.push_back(aQueue[cpuNum].availTime);                 // �����ɶ�  
									done.push_back(0);                                        // �L���� 
									doneProcess.push_back(done);						
								}
							}
	
							if (!processDone) {                                            
								aQueue[cpuNum].enQueue(process[i]);                           // �ƶi��C 
							}
							
							// �A��s��1 - cpuNum + 1�Ӧ�C 
							while (process[i][1] >= aQueue[1 - cpuNum].availTime && !aQueue[1 - cpuNum].isEmpty()) {							
								doQueuesFront(aQueue, abortProcess, doneProcess, 1 - cpuNum);
							}							
						}
					}
					else { // cpuNum == -1
						vector <int> abort;
						abort.push_back(process[i][0]);
						abort.push_back(cpuNum + 1);
						abort.push_back(process[i][1]);                     // �����ɶ����Ӥu�@���i�J�ɶ� 
						abort.push_back(0);                                 // �L����  
						abortProcess.push_back(abort);					
					}
				}
				
				// �M�Ŧ�C�A��������C���M�� 
				if (aQueue[0].length() >= aQueue[1].length()) {
					cpuNum = 0;
				}
				else {
					cpuNum = 1;
				}				
				
				while (!aQueue[0].isEmpty() || !aQueue[1].isEmpty()) {
					if (!aQueue[0].isEmpty() && aQueue[1].isEmpty()) {
						cpuNum = 0;
					}
					else if (aQueue[0].isEmpty() && !aQueue[1].isEmpty()){
						cpuNum = 1;
					}
					
					doQueuesFront(aQueue, abortProcess, doneProcess, cpuNum);					
				}				
					
				double averageDelay, totalDelay = 0, successRate;
				// �p���`����ɶ� 
				for (int i = 0; i < abortProcess.size(); i++) {
					totalDelay = totalDelay + abortProcess[i][3];
				}								
				
				for (int i = 0; i < doneProcess.size(); i++) {
					totalDelay = totalDelay + doneProcess[i][3];
				}
				
				averageDelay = round(totalDelay / (abortProcess.size() + doneProcess.size()) * 100) / 100; // ��������ɶ� 
				successRate = round(100 * (double)doneProcess.size() / (abortProcess.size() + doneProcess.size()) * 100) / 100; // ���\�v 
				
				storeDouble(fileNumber, abortProcess, doneProcess, averageDelay, successRate); // �إ�double�� 
														
				cout << endl << "See double" << fileNumber << ".txt" << endl;
				clearVector(process); // �M�Ťu�@�M��  
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

bool load(string fileName, string fileNumber, vector <vector <int> > &process) { // Ū�� 
	string completeFileName = fileName + fileNumber + ".txt";
	Char100 trash;
	FILE * fp;
	fp = fopen(completeFileName.c_str(), "r");
	
	if (fp == NULL) {                          // Ū�ɥ��� 
		cout << endl << "### " << completeFileName << " does not exist! ###" << endl;
		return false;
	}
	
	fgets(trash, 100, fp);                     // ��Ĥ@��Ū�� 
	int temp[4], data[4];
	bool first = true;
	while(!feof(fp)) {                         // Ū��EOF���� 
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

void storeSorted(string fileNumber, vector <vector <int> > process) { // �رƧ��� 
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
				 double averageDelay, double successRate) { // �ؿ�X�� 
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

void storeDouble(string fileNumber, vector<vector <int> > abortProcess, vector<vector <int> > doneProcess,
				 double averageDelay, double successRate) { // ��double�� 
	string newFileName = "double" + fileNumber + ".txt";
	string abortFirstLine = "\t[Abort Jobs]\n";
	string abortSecondLine = "\tOID\tCID\tAbort\tDelay\n";
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
		for(int j = 0; j < 4; j++) {		
			sprintf(buff, "%d", abortProcess[i][j]);
			fwrite(buff, sizeof(char), strlen(buff), fp);
			if (j < 3) fwrite("\t", sizeof(char), 1, fp);
		}
		
		fwrite("\n", sizeof(char), 1, fp);
	}	
	
	string doneFirstLine = "\t[Jobs Done]\n";
	string doneSecondLine = "\tOID\tCID\tDeparture\tDelay\n";
	fwrite(doneFirstLine.c_str(), sizeof(char), doneFirstLine.length(), fp);
	fwrite(doneSecondLine.c_str(), sizeof(char), doneSecondLine.length(), fp);
	
	for(int i = 0; i < doneProcess.size(); i++) {
		Char100 buff = "[", temp;
		sprintf(temp, "%d", i + 1);
		strcat(buff, temp);
		strcat(buff, "]");	
		fwrite(buff, sizeof(char), strlen(buff), fp);
		fwrite("\t", sizeof(char), 1, fp);
		for(int j = 0; j < 4; j++) {		
			sprintf(buff, "%d", doneProcess[i][j]);
			fwrite(buff, sizeof(char), strlen(buff), fp);
			if (j < 3) fwrite("\t", sizeof(char), 1, fp);
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

void shellSort(vector <vector <int> > &process, int n) { // �ƺ��ƧǪk 
	for (int h = n / 2; h > 0; h = h / 2) {
		for (int unsorted = h; unsorted < n; unsorted++) {
			int loc = unsorted;
			vector <int> nextItem = process[unsorted];
			// arrival time�j���u�@�ƫ᭱�Farrival time�ۦP���ⵧ�u�@�A�s���j���ƫ᭱ 
			for(; (loc >= h) && ((process[loc - h][1] > nextItem[1]) || 
				((process[loc - h][1] == nextItem[1]) && process[loc - h][0] > nextItem[0])); loc = loc - h) {
				process[loc] = process[loc - h];
			}
			
			process[loc] = nextItem;
		}
	}
}

void clearVector(vector <vector <int> > &process) { // �M��vector 
	for (int i = 0; i < process.size(); i++) {
		process[i].clear();
	}
	
	process.clear();
}

void doQueueFront(ProcessQueue &aQueue, vector<vector <int> > &abortProcess, vector<vector <int> > &doneProcess) { // �����C�Ĥ@���u�@ 
	vector <int> temp;
	aQueue.getFront(temp);                                // ���o�Ĥ@���u�@����T 
	// temp[0]:�u�@�s��, temp[1]:�i�J�ɶ�, temp[2]:����ɶ�, temp[3]:�O�ɮɶ� 
	if (aQueue.availTime + temp[2] > temp[3]) {           // �O�� 
		if (aQueue.availTime < temp[3]) {                 // ����~���o�{�O�� 
			// �[�J�����M�� 
			aQueue.availTime = temp[3];                   // CPU�Ŷ��ɶ��]���Ӥu�@���O�ɮɶ� 
			vector <int> abort;
			abort.push_back(temp[0]);
			abort.push_back(temp[3]);                     // �����ɶ����O�ɮɶ� 
			abort.push_back(temp[3] - temp[1]);           // ���𬰨����ɶ���i�J�ɶ� 
			abortProcess.push_back(abort);
		}			
		else {                                            // �٨S�}�l����N�O�� 
			// �[�J�����M�� 
			vector <int> abort;
			abort.push_back(temp[0]);
			abort.push_back(aQueue.availTime);            // �����ɶ� 
			abort.push_back(aQueue.availTime - temp[1]);  // ���𬰨����ɶ���i�J�ɶ�  
			abortProcess.push_back(abort);							
		}					
	}
	else {                                                // �S���O��  
		// �[�J�����M�� 
		vector <int> done;
		done.push_back(temp[0]);
		done.push_back(aQueue.availTime + temp[2]);       // �����ɶ���CPU�Ŷ��ɶ��[�u�@����ɶ� 
		done.push_back(aQueue.availTime - temp[1]);       // ���𬰶}�l����Ӥu�@���ɶ���i�J�ɶ� 
		doneProcess.push_back(done);
		aQueue.availTime = aQueue.availTime + temp[2];    // CPU�Ŷ��ɶ��]���Ӥu�@�����ɶ�  
	}
	
	aQueue.deQueue();	                                  // �N�u�@�q��C������ 
}

void doQueuesFront(ProcessQueue aQueue[], vector<vector <int> > &abortProcess,
				   vector<vector <int> > &doneProcess, int index) { // �����C�Ĥ@���u�@(�h��C) 
	vector <int> temp;
	aQueue[index].getFront(temp);                         // ���o�Ĥ@���u�@����T 
	// temp[0]:�u�@�s��, temp[1]:�i�J�ɶ�, temp[2]:����ɶ�, temp[3]:�O�ɮɶ� 
	if (aQueue[index].availTime + temp[2] > temp[3]) {    // �O�� 
		if (aQueue[index].availTime < temp[3]) {          // ����~���o�{�O�� 
			// �[�J�����M�� 
			aQueue[index].availTime = temp[3];            // CPU�Ŷ��ɶ��]���Ӥu�@���O�ɮɶ� 
			vector <int> abort;
			abort.push_back(temp[0]);
			abort.push_back(index + 1);
			abort.push_back(temp[3]);                     // �����ɶ����O�ɮɶ� 
			abort.push_back(temp[3] - temp[1]);           // ���𬰨����ɶ���i�J�ɶ� 
			abortProcess.push_back(abort);
		}			
		else {                                            // �٨S�}�l����N�O�� 
			// �[�J�����M�� 
			vector <int> abort;
			abort.push_back(temp[0]);
			abort.push_back(index + 1);
			abort.push_back(aQueue[index].availTime);     // �����ɶ� 
			abort.push_back(aQueue[index].availTime - temp[1]);  // ���𬰨����ɶ���i�J�ɶ�  
			abortProcess.push_back(abort);							
		}					
	}
	else {                                                // �S���O��  
		// �[�J�����M�� 
		vector <int> done;
		done.push_back(temp[0]);
		done.push_back(index + 1);									
		done.push_back(aQueue[index].availTime + temp[2]); // �����ɶ���CPU�Ŷ��ɶ��[�u�@����ɶ� 
		done.push_back(aQueue[index].availTime - temp[1]); // ���𬰶}�l����Ӥu�@���ɶ���i�J�ɶ� 
		aQueue[index].availTime = aQueue[index].availTime + temp[2]; // CPU�Ŷ��ɶ��]���Ӥu�@�����ɶ� 									
		doneProcess.push_back(done); 
	}
	
	aQueue[index].deQueue();	                          // �N�u�@�q��C������ 
}
