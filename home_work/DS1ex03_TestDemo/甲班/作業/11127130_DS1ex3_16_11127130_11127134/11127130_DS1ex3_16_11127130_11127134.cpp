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
void shellSort(vector <vector <int> > &process, int n);                         // �ƺ��ƧǪk 
void clearVector(vector <vector <int> > &process);                              // �M��vector 
void doQueueFront(ProcessQueue &aQueue, vector<vector <int> > &abortProcess,
				  vector<vector <int> > &doneProcess);                          // �����C�Ĥ@�� 

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
	    cout << endl << "**************************************";
		cout << endl << "Input a command(0, 1, 2): ";
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
