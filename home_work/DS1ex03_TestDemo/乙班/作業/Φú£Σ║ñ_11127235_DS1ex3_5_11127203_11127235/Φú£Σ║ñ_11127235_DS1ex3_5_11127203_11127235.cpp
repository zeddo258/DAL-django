// 11127203 ���O�p 11127235 �\�ʵ� 
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

// �ۭq�q��C���O
template <typename T> //�w�q�@�ӼҪO�A�䤤�� T �O�@�������ѼơA��ܳo�ӼҪO�i�H���Ω󤣦P���ƾ�����
class MyQueue {
private:
    vector<T> elements;

public:
    // �N�������e���C������
    void enqueue(const T& element) {
        elements.push_back(element);
    }

    // �q��C���Y����������
    void dequeue() {
        if (!isEmpty()) {
            elements.erase(elements.begin());
        }
    }

    // �s����C���Y������
    T front() const {
        if (!isEmpty()) {
            return elements.front();
        }
    }


    // �ˬd��C�O�_����
    bool isEmpty() const {
        return elements.empty();
    }

    // ���o��C���������ƶq
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
          
          jT frontJob = inputQueue.front(); // �� frontJob�x�s inputQueue���Y������ 
          tofinish.enqueue(frontJob); // �N frontJob�ƻs�� tofinish
          int currentTime = frontJob.Arrival; // �{�b�ɶ�(currentTime)���� tofinish����F�ɶ� 
          inputQueue.dequeue();  // �N�Y���������� 
        
          currentTime = processFirst(currentTime, tofinish, inputQueue, cpuQueue, doneList, abortList); // �B�̲Ĥ@����ƻP�n��iQ�����e�A
		                                                                                             //��^�{�b�ɶ�(currentTime)

        
          processLasts(currentTime, tofinish, inputQueue, cpuQueue, doneList, abortList); // �B��Q�̪���ƻP�ѤU����� 
          cout << endl << "The simulation is running...\n" ;
          cout << "See " << outputFile << "\n";
          writeOutput(doneList, abortList); // �L�X 
          
        }
    }
    
private:
    string inputFile;
    string outputFile;
    
    MyQueue<jT> inputQueue; // ��J����� 
    MyQueue<jT> tofinish; // �ǳƧ�������� 
    MyQueue<jT> cpuQueue; // Q 
    vector<Done> doneList; // �w��������ƦC�� 
    vector<Queue> abortList; // �w��������ƦC��  

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
              // �N��ƥ[�JinputQueue
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

    	while (!inputQueue.isEmpty() && !hasonedone) { //�B�z�Ĥ@���n�[�JdoneList����� 
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
    	while (!inputQueue.isEmpty() && (cpuQueue.size() < 3 || currentTime > frontJob.Arrival)) { // �B�̥��n�[�JQ���T����� 
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
                  vector<Done>& doneList, vector<Queue>& abortList) { // �B�̳ѤU����� 
        
    	while (!cpuQueue.isEmpty()) { 
        	jT frontJob = cpuQueue.front(); //�ƻsQ 
        	tofinish.enqueue(frontJob); //�NQ���Ĥ@����ƥ[�J�ݧ��� 
        	cpuQueue.dequeue(); //�R��Q���Y 
        	if ( !inputQueue.isEmpty() ) { // �N�ѤU����ƥ[�JQ 
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

        	if (finishtime <= frontJob.TimeOut) { // �B�̫ݧ�����ơA�P�_�n�NfrontJob���Abort List�٬ODone List 
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
        	jT CheckfrontJob = inputQueue.front(); // �ˬd�[��Q�ç�scurrentTime��ѤU����Ʀ��L�O�� 
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
          ofstream outputFileOut(outputFile.c_str()); // ���}�ɮץH�g�J�B�z�᪺�ƾ�  


          // �N�B�z�᪺�ƾڼg�J�ɮ�
          outputFileOut << "\t[Abort Jobs]\n";
          outputFileOut << "\tOID\tAbort\tDelay\n";

		  int i = 1 ;
          for (vector<Queue>::const_iterator it = abortList.begin(); it != abortList.end(); ++it) {
              const Queue& queue = *it;
              outputFileOut << "[" << i << "]\t" << queue.OID << '\t'<< queue.Abort << '\t' << queue.Delay << "\n";
              i++ ;
          }

		  // �N�B�z�᪺�ƾڼg�J�ɮ�
          outputFileOut << "\t[Jobs Done]\n";
          outputFileOut << "\tOID\tDeparture\tDelay\n";

		  i = 1 ;
          for (vector<Done>::const_iterator it = doneList.begin(); it != doneList.end(); ++it) {
              const Done& done = *it;
              outputFileOut << "[" << i << "]\t"  << done.OID << '\t' << done.Departure << '\t' << done.Delaytime << "\n";
              i++ ;
          }

          // ��J��������M���\��� 
          double totalDelay = 0;
          int successCount = doneList.size();

          // �M����ƶi��p�⩵��
          for (vector<Done>::const_iterator it = doneList.begin(); it != doneList.end(); ++it) {
              const Done& done = *it;
              totalDelay += done.Delaytime;
          }

          // �M����ƶi��p�⩵�� 
          for (vector<Queue>::const_iterator it = abortList.begin(); it != abortList.end(); ++it) {
              const Queue& queue = *it;
              totalDelay += queue.Delay;
          }

		   // �p�⥭������(�@��Gms)�M���\���(���G%) 
          double averageDelay = totalDelay / (doneList.size() + abortList.size());
          double successRate = (static_cast<double>(successCount) / (inputQueue.size() + doneList.size() + abortList.size())) * 100.0;

		  // ��X�榡�A�ϥ� setprecision(2) �ӳ]�w�p���I��2���
          outputFileOut << "[Average Delay]\t" << fixed << setprecision(2) << averageDelay << " ms\n";
          outputFileOut << "[Success Rate]\t" << fixed << setprecision(2) << successRate << " %\n";

          outputFileOut.close(); // �����ɮ� 
        
        }
        
    }    
};

class JobList {
public:
    JobList(const string& inputFile, const string& outputFile)
     : inputFile(inputFile), outputFile(outputFile) {}  // ���T��l�Ʀ����ܼ� 

    bool success = true;
    void readData() {
      // �p��Ū�ɶ}�l�ɶ�
      readStart = clock();

      // ���}�ɮ�
      ifstream inFile(inputFile.c_str()); // �ϥ� inputFile

      if (!inFile) {
          cout << endl << "### " << inputFile << " does not exist!" << " ###" << endl;
          success = false;
          return;
      } else {
      	success = true;
      	// Ū���ɮפ����ƾڨ�ʺA�}�C��
        jobType record;
        string header;
        getline(inFile, header); // Ū���é����Ĥ@��]���Y�^
        while (inFile >> record.OID >> record.Arrival >> record.Duration >> record.TimeOut) {
          dynamicArray.push_back(record);
        }
        
        cout << endl << "        OID\tArrival\tDuration\tTimeOut\n";
        int x = 1 ;
        for (size_t i = 0; i < dynamicArray.size(); ++i) {
      	  cout <<"(" << x << ")\t" << dynamicArray[i].OID << '\t' << dynamicArray[i].Arrival << '\t' << dynamicArray[i].Duration << '\t' << dynamicArray[i].TimeOut << '\n';
          x ++;
        }
        // �����ɮ�
        inFile.close();
  
        // �p��Ū�ɵ����ɶ�
        readEnd = clock();
	  }
    }

    void sortData() {

        // �ϥΧƺ��Ƨǹ�ʺA�}�C�i��Ƨ�
        if (success == true) {
          // �p��ƧǶ}�l�ɶ�
          sortStart = clock();
          shellSort(dynamicArray);
          // �p��Ƨǵ����ɶ�
          sortEnd = clock();
        }

    }

    void writeData() {

      if (success == true) {
      	// �p��g�ɶ}�l�ɶ�
        writeStart = clock();
        // ���}�ɮץH�g�J�Ƨǫ᪺�ƾ�
	    ofstream file(outputFile.c_str());  // �ϥ����O�����ܼ� outputFile
	    // �ˬd�ɮ׬O�_���\���}
        if (!file.is_open()) {
          cout << endl << "### " << outputFile << " doesn't exist!" << " ###" << endl;
          return;
        }
        file << "OID\tArrival\tDuration\tTimeOut\n";
        for (size_t i = 0; i < dynamicArray.size(); ++i) {
          file << dynamicArray[i].OID << '\t' << dynamicArray[i].Arrival << '\t' << dynamicArray[i].Duration << '\t' << dynamicArray[i].TimeOut << '\n';
        }
        // �����ɮ�
        file.close();

        // �p��g�ɵ����ɶ�
        writeEnd = clock();
	  } 
      
      

    }

    void displayExecutionTimes() {
      
	  // �p��clock 
      clock_t readClock = readEnd - readStart;  
      clock_t sortClock = sortEnd - sortStart;
      clock_t writeClock = writeEnd - writeStart;

      // �p�����ܰ���ɶ��A�H�@�����
      double readMilliseconds = static_cast<double>(readEnd - readStart) / (CLOCKS_PER_SEC/1000 );
      double sortMilliseconds = static_cast<double>(sortEnd - sortStart) / (CLOCKS_PER_SEC/1000 );
      double writeMilliseconds = static_cast<double>(writeEnd - writeStart) / (CLOCKS_PER_SEC/1000 );

      //'fixed << setprecision(2)'��ܨ���p���I���� 
      if ( success == true ) {
        cout << endl << "Reading data: " << readClock << " clocks (" << fixed << setprecision(2) << readMilliseconds << " ms)\n";
        cout << "Sorting data: " << sortClock << " clocks (" << fixed << setprecision(2)<< sortMilliseconds << " ms)\n";
        cout << "Writing data: " << writeClock << " clocks (" << fixed << setprecision(2)<< writeMilliseconds  << " ms)\n";
        cout << endl << "See " << outputFile << "\n";
      }
    }

private:
    string inputFile; //�x�s��J�ɮשM��X�ɮת��ɮצW��
    string outputFile; //�x�s��J�ɮשM��X�ɮת��ɮצW�� 
    vector<jT> dynamicArray; //�x�s'jobType'���c���O��
    clock_t readStart, readEnd, sortStart, sortEnd, writeStart, writeEnd; //�x�s���P���q���ɶ�

    // �ƺ��Ƨ�
   void shellSort(vector<jT>& arr) {
     int n = arr.size();

     for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            jT temp = arr[i];
            int j;

            // �p�GArrival�ۦP�A�h���OID
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
    string command ; // �Ω���O��J 
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
              stringstream ss; //�Ыؤ@�Ӧr�Ŧ�y 
              ss << "input" << fileNumber << ".txt"; //�N��J�ɮ׸��X�������ɮצW 
              fileName = ss.str();  //�ե�'ss.str()�N�ɮצW�٦s�b'fileName'��
              stringstream ss2;
              ss2 << "sorted" << fileNumber << ".txt"; 
              sorted = ss2.str(); // �ץ����B
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
                sorted = ss2.str(); // �ץ����B
                stringstream ss3;
                ss3 << "output" << fileNumber << ".txt";
                output = ss3.str(); // �ץ����B
                first = false;
              } else {
                stringstream ss3;
                ss3 << "output" << fileNumber << ".txt";
                output = ss3.str(); // �ץ����B
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

