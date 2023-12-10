// ��14�� 11127206 �����| 11127227 ���a��

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

struct Job {
    int OID;
    int Arrival;
    int Duration;
    int TimeOut;
    int CancelTime;
    int Process;
    int Delay;
    int DepartureTime;
    Job() = default;
    Job(int oid, int arrival, int duration, int timeout, int canceltime, int process, int delay, int departureTime)
        : OID(oid), Arrival(arrival), Duration(duration), TimeOut(timeout), CancelTime(canceltime), Process(process), Delay(delay), DepartureTime(departureTime) {}
};

// �۩w�q�� Queue ���O
template <typename T>
class MyQueue {
private:
    vector<T> elements;

public:
    void push(const T& element) {
        elements.push_back(element);
    }

    void pop() {
        if (!empty()) {
            elements.erase(elements.begin());
        }
    }

    T front() const {
        if (!empty()) {
            return elements.front();
        }

        return T();
    }

    bool empty() const {
        return elements.empty();
    }

};


class JobScheduler {
public:
    vector<Job> jobs;

    // Ū��
    void read(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        jobs.clear();

        string header;
        getline(file, header);

        int oid, arrival, duration, timeout,canceltime,process, delay, departureTime;
        while (file >> oid >> arrival >> duration >> timeout) {
            jobs.push_back({oid, arrival, duration, timeout, canceltime, process, delay, departureTime});
        }

        file.close();
    }

    // �g��
    void write(string filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        file << "OID\tArrival\tDuration\tTimeOut" << endl;

        for (const auto &job : jobs) {
            file << job.OID << "\t" << job.Arrival << "\t" << job.Duration << "\t" << job.TimeOut << endl;
        }

        file.close();
    }

    // �Ƨ�
    void shellSort() {
        int n = jobs.size();
        for (int gap = n / 2; gap > 0; gap = gap / 2) {
            for (int i = gap; i < n; i++) {
                Job temp = jobs[i];
                int j;
                for (j = i; j >= gap && (jobs[j - gap].Arrival > temp.Arrival
                     || (jobs[j - gap].Arrival == temp.Arrival && jobs[j - gap].OID > temp.OID)); j = j - gap) {
                    jobs[j] = jobs[j - gap];
                }
                jobs[j] = temp;
            }
        }
    }

    // ���ȤG�G��@ CPU ��C����
    void executeQueueSimulation() {
        int fileNumber;
        int CancelTime = 0; // ��l��


        cout << "Input a file number: ";
        cin >> fileNumber;

        string sortedFilename = "sorted" + to_string(fileNumber) + ".txt";

        ifstream sortedFile(sortedFilename);
        if (!sortedFile.is_open()) {
            cerr << "Error: Sorted file does not exist. Please sort the file first." << endl;
            return;
        }
        sortedFile.close();
        string outputFilename = "output" + to_string(fileNumber) + ".txt";
        cout << "The simulation is running..." << endl;
        cout << "See " << outputFilename << endl;

        auto start_read = high_resolution_clock::now();

        read(sortedFilename);

        auto duration_read = duration_cast<milliseconds>(high_resolution_clock::now() - start_read);

        vector<Job> originalJobs = jobs;
        MyQueue<Job> queue;
        vector<Job> doneList;
        vector<Job> abortList;

        int currentTime = 0, a = 0, delay = 0;
        int process = 0, canceltime = 0;
        int i = 0, executeTime = 0;

        for (const auto &job : originalJobs) {
            if (job.Arrival > currentTime) {
                currentTime = job.Arrival;
            }

            queue.push(job);

            while (!queue.empty()) {

                Job currentJob = queue.front();
                queue.pop();

                currentTime += currentJob.Duration;

                if (currentTime <= currentJob.TimeOut) {
                    //���ȧ���
                    currentJob.Process = -1;

                    if (i == 0) executeTime = currentJob.Arrival;
                    else executeTime += currentJob.Duration;

                    currentJob.DepartureTime = executeTime + currentJob.Duration;
                    currentJob.Delay = executeTime - currentJob.Arrival;
                    doneList.push_back({currentJob.OID, currentJob.Arrival, currentJob.Duration, currentJob.TimeOut,
                                         currentJob.Process, currentJob.CancelTime, currentJob.Delay, currentJob.DepartureTime});
                }
                else {

                    a = a + 1;
                    if (a > 3) {
                        currentJob.Process = 0;
                        currentJob.Delay = 0;
                        // cout << "��C�w�� " << currentJob.OID << endl;//testing

                    }
                    else {
                        if (currentTime >= currentJob.Arrival + currentJob.Duration) {
                            // cout << "����L�{���~�o�͹O�� " << currentJob.OID << endl;//testing
                            currentJob.Process = 1;
                            currentJob.CancelTime = currentJob.TimeOut;
                            currentJob.Delay = currentJob.CancelTime - currentJob.Arrival;

                        } // CPU ����L�{���~�o�͹O��
                        else {
                            // cout << "�q��C���X�ɵo�{�w�g�O�� "<< currentJob.OID << endl;//testing
                            currentJob.Process = 2;
                            currentJob.CancelTime = currentTime;
                            currentJob.Delay = currentJob.CancelTime - currentJob.Arrival;
                        } // �q��C���X�ɵo�{�w�g�O��

                    }

                    abortList.push_back({currentJob.OID, currentJob.Arrival, currentJob.Duration, currentJob.TimeOut,
                                          currentJob.Process, currentJob.CancelTime, currentJob.Delay, currentJob.DepartureTime});
                }

                cout << "delay " << currentJob.OID << " : " << currentJob.Delay << endl;

            } // while

            i++;

        } // for


        outputResult(abortList, doneList, fileNumber, canceltime, delay, process);
    }

    // ��X���G
    void outputResult(const vector<Job> &abortList, const vector<Job> &doneList, int fileNumber, int canceltime, int delay,  int process) {
        string outputFilename = "output" + to_string(fileNumber) + ".txt";

        ofstream outFile(outputFilename);
        if (!outFile.is_open()) {
            cerr << "Error opening " << outputFilename << endl;
            exit(EXIT_FAILURE);
        }

        outFile << "[Abort Jobs]" << endl;
        outFile << "OID\tAbort\tDelay" << endl;
        int counter = 1;


        for (const auto &job : abortList) {
            if (job.Process == 0 ) { // ��C�w��
                int delay2 = 0;
                outFile << "[" << counter++ << "] " << job.OID << "\t" << job.Arrival << "\t" << delay2 << endl;
            }
            else if (job.Process == 1 || job.Process == 2 ) { // ����L�{���~�o�͹O��, �q��C���X�ɵo�{�w�g�O��
                outFile << "[" << counter++ << "] " << job.OID << "\t" << job.CancelTime << "\t" << job.Delay << endl;
            }

        }


        outFile << endl << "[Jobs Done]" << endl;
        outFile << "OID\tDeparture\tDelay" << endl;
        counter = 1;
        for (const auto &job : doneList) {
            outFile << "[" << counter++ << "] " << job.OID << "\t" << job.DepartureTime << "\t" << job.Delay << endl;
        }

        double totalDelay = 0;
        for (const auto &job : abortList) {
            if (job.Process == 0 || job.Process == 1 || job.Process == 2) {
                totalDelay += job.Delay;
            }


        }


        double averageDelay = 0;
        if (!doneList.empty()){
            for (const auto &job : doneList) {
                totalDelay += job.Delay;
            }
            averageDelay = totalDelay / (abortList.size() + doneList.size());
        }

        double successRate = 0;
        if (!doneList.empty() || (!abortList.empty() && abortList.front().Process != 0)){
            successRate = (static_cast<double>(doneList.size()) / (doneList.size() + abortList.size())) * 100;
        }


        outFile << endl << "[Average Delay]" << endl;
        outFile << averageDelay << " ms" << endl;
        outFile << fixed << setprecision(2);
        outFile << "[Success Rate]" << endl;
        outFile << successRate << "%" << endl;

        outFile.close();
    }


    // ��J���O���
    int inputCommand() {
        int command;
        cout << "**** Simulate FIFO Queues by SQF *****" << endl;
        cout << "* 0. Quit                            *" << endl;
        cout << "* 1. Sort a file                     *" << endl;
        cout << "* 2. Simulate one FIFO queue         *" << endl;
        cout << "**************************************" << endl;
        cout << "Input a command (0, 1, 2): ";
        cin >> command;
        return command;
    }

    // ����Ƨ�
    void execute() {
        auto start_read = high_resolution_clock::now(); // �����e�ɶ��ӭp��Ū���ɮשһݪ��ɶ�
        int fileNumber;
        cout << "Input a file number: ";
        cin >> fileNumber;

        string inputFilename = "input" + to_string(fileNumber) + ".txt";
        string outputFilename = "sorted" + to_string(fileNumber) + ".txt";

        auto duration_read = duration_cast<milliseconds>(high_resolution_clock::now() - start_read); // �p��Ū���ɮשһݪ��ɶ�

        cout << "\tOID\tArrival\tDuration\tTimeOut" << '\n';

        // ��ܭ�l���
        read(inputFilename);
        int counter = 1;
        for (const auto &job : jobs) {
            cout <<  "(" << counter++ << ")" << "\t" << job.OID << "\t" << job.Arrival << "\t" << job.Duration << "\t" << job.TimeOut << endl;
        }

        // �Ƨ�
        auto start_sort = high_resolution_clock::now(); // �����e�ɶ��ӭp��Ƨǩһݪ��ɶ�
        shellSort();
        auto stop_sort = high_resolution_clock::now(); // ����Ƨǧ�������e�ɶ�
        auto duration_sort = duration_cast<milliseconds>(stop_sort - start_sort); // �p��Ƨǩһݪ��ɶ�

        // �g�J�ɮ�
        auto start_write = high_resolution_clock::now(); // �����e�ɶ��ӭp��g�J�ɮשһݪ��ɶ�
        write(outputFilename);
        auto stop_write = high_resolution_clock::now(); //  ����g�J�ɮק�������e�ɶ�
        auto duration_write = duration_cast<milliseconds>(stop_write - start_write); // �p��g�J�ɮשһݪ��ɶ�

        // �L�X����ɶ�
        cout << endl;
        cout << "Reading data: " << duration_read.count() << " clocks (" << duration_read.count() << ".00"<< "ms)." << '\n';
        cout << "Sorting data: " << duration_sort.count() << " clocks (" << duration_sort.count() << ".00"<< "ms)." << '\n';
        cout << "Writing data: " << duration_write.count() << " clocks (" << duration_write.count() << ".00"<< "ms)." << '\n' << endl;
        cout << "See " << outputFilename << endl;
        cout << endl;


    }




    // �D�{��������
    void run() {
        int command;
        while (true) {
            command = inputCommand();
            if (command == 1) {
                execute();
            }
            else if (command == 2) {
                executeQueueSimulation();
            }
            else if (command == 0) {
                cout << "Exiting program." << endl;
                break;  // ���}�j��
            }
            else {
                cout << "Invalid command. Please try again." << endl;
            }
        }
    }

};

int main() {
    JobScheduler scheduler;
    scheduler.run();

    return 0;
}
