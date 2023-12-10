//��27�� 11020117 �L�l�q  11020134 �f�v��

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

// �u�@���O
class Job {
public:
    int OID;
    int Arrival;
    int Duration;
    int TimeOut;
};

// ��C���O
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

//Queue����l�� 
Queue::Queue(int size) {
    capacity = size;
    jobs = new Job[capacity];
    front = rear = -1;
}

//����Jobs�}�C�e�Ϊ��Ŷ� 
Queue::~Queue() {
    delete[] jobs;
}

//�ˬd��C�O�_���� 
bool Queue::isEmpty() {
    return front == -1;
}

//�ˬd��C�O�_�w�� 
bool Queue::isFull() {
    return (rear + 1) % capacity == front;
}

//�NJob�����F��[�J��C 
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

//���o��C���̫e�� 
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

//�ΨӨ��o��C�����w��m�� Job�C
Job& Queue::at(int index) {
    if (index >= 0 && index < capacity) {
        return jobs[index];
    } else {
        cout << "Error!" << endl;
        static Job emptyJob;
        return emptyJob;
    }
}

// �p��ثe��C���������ƶq
int Queue::getNumElements() {
    
    if (isEmpty()) {
        return 0;
    } else {
        return (rear - front + capacity) % capacity + 1;
    }
}

// Ū���ɮ׸�� 
void readData(Queue& queue, const char* filename) {
    ifstream infile(filename);

    if (!infile) {
        return;
    }

    Job job;
    
    // �������D 
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

        // ����� Arrival
		// �p�G Arrival �ۦP�A�N��� OID
        while (j >= 0 && (queue.at(j).Arrival > key.Arrival || (queue.at(j).Arrival == key.Arrival && queue.at(j).OID > key.OID))) {
            queue.at(j + 1) = queue.at(j);
            --j;
        }

        queue.at(j + 1) = key;
    }
}


// ��ܦ�C�������
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

//��Ƨǫ᪺��Ƽg�J�s���ɮפ�
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

    // �ŧi�ɶ��ܼ�
    clock_t readStart, readEnd, sortStart, sortEnd, writeStart, writeEnd;

	//�}�l���� 
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
            case 0:  //0�N�h�X���� 
                break; 
            case 1:  //Ū�� �Ƨ� �� �g�� �íp��ɶ� 

                cout << "Input a file number: ";
                cin >> filename;
                cout << "\n";
                input = "input" + filename + ".txt";
                output = "sorted" + filename + ".txt";

				// �p��Ū�ɶ}�l�ɶ�
                readStart = clock();
                
                // Ū���ɮ׸�� 
                readData(jobsQueue, input.c_str());
                
                if (jobsQueue.isEmpty()) {
                    cout << "### "<< input <<" does not exist! ###" << endl;
                    break;
                }

                // �p��Ū�ɵ����ɶ�
                readEnd = clock();
                
                // ��ܦ�C���
                displayQueue(jobsQueue);

                // �p��ƧǶ}�l�ɶ�
                sortStart = clock();

                // �ϥΧƺ��ƧǡA�ھ� Arrival�ɶ��i��Ƨ�
                insertionSort(jobsQueue);

                // �p��Ƨǵ����ɶ�
                sortEnd = clock();

                // �p��g�ɶ}�l�ɶ�
                writeStart = clock();

                // �N�Ƨǫ᪺��Ƽg�J�ɮ�
                writeSortedData(jobsQueue, output.c_str());

                // �p��g�ɵ����ɶ�
                writeEnd = clock();

                // ��ܰ���ɶ�
                cout << "Reading data: " << readEnd - readStart << " clocks (" << fixed << setprecision(2) << double(readEnd - readStart) * 1000.0 / CLOCKS_PER_SEC << " ms)." << endl;
                cout << "Sorting data: " << sortEnd - sortStart << " clocks (" << fixed << setprecision(2) << double(sortEnd - sortStart) * 1000.0 / CLOCKS_PER_SEC << " ms)." << endl;
                cout << "Writing data: " << writeEnd - writeStart << " clocks (" << fixed << setprecision(2) << double(writeEnd - writeStart) * 1000.0 / CLOCKS_PER_SEC << " ms)." << endl;
                cout << "\n";
                cout << "See " << output.c_str() << endl;
                break;
            case 2:
            	cout << "�S������";
				break;    
            default:  //�קK�X�{0�B1�B2�H�~�����O 
                cout << "Command does not exist!" << endl;
        }
    } while (choice != 0);

    return 0;
}


