// 10920151 張又晟, 11127128 蔡維庭

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <cmath>

using namespace std;

typedef struct jT {
    int OID = 0;
    int arrival = 0;
    int duration = 0;
    int timeout = 0;
} jobType ;

class JobList {
    vector<jobType> aList ;
    string fileID ;
    clock_t timeReading ;
    clock_t timeSorting ;
    clock_t timeWriting ;

    void reset() {  // definition: initial set up
        fileID = '\0';
    }

    void showJob(jobType& a) { // declaration: display a job on screen
        cout << a.OID << '\t' << a.arrival << '\t' << a.duration << "\t" << a.timeout << endl;
    }
    
    void putAll() { // declaration: write all as a file
        clock_t time = clock();
        ofstream outFile;
        outFile.open("sorted" + fileID + ".txt");

        outFile << "OID\tArrival\tDuration\tTimeOut" << endl;
        for (int i = 0 ; i < aList.size() ; i++) {
            outFile << aList[i].OID << '\t' << aList[i].arrival << '\t' << aList[i].duration << "\t" << aList[i].timeout << endl;
        }

        outFile.close();
        timeWriting = clock() - time;
    }

    void showTime() {    // declaration: output time on screen
        cout << "Reading data: " << timeReading << " clocks (" << timeReading << " ms)." << endl;
        cout << "Sorting data: " << timeSorting << " clocks (" << timeSorting << " ms)." << endl;
        cout << "Writing data: " << timeWriting << " clocks (" << timeWriting << " ms)." << endl;
    }

    void nextJobCheck(jobType& nextJob) {  // declaration: take a look at the next job
        nextJob = aList[0];
    }

    bool shell_compare(jobType& a, jobType& b) {
        if (a.arrival > b.arrival) return true;
        if (a.arrival == b.arrival && a.OID > b.OID) return true;
        return false;
    }

public:
    JobList() { reset(); }
    ~JobList() { reset(); }
    bool isEmpty() {
        if (aList.empty()) return true;
        else return false;
    }

    bool getAll(string name, bool isSorted) {    // declaration: read all from a file
        clock_t time = clock();
        aList.clear();
        fileID = name;
        ifstream infile;
        if (!isSorted) name = "input" + fileID + ".txt";
        else name = "sorted" + fileID + ".txt";
        infile.open(name);
        if (!infile.is_open()) {
            cout << endl << "### " << name << " does not exist! ###" << endl;
            return false;
        }

        string buffer;
        getline(infile, buffer);
        while (getline(infile, buffer)) {
            jobType jb;
            stringstream ss(buffer);
            ss >> jb.OID >> jb.arrival >> jb.duration >> jb.timeout ;
            aList.push_back(jb);
        }

        infile.close();
        timeReading = clock() - time ;
        return true;
    }

    void print(bool isSorted) { // print the sorted data
        int length = aList.size();
        cout << endl;
        if (!isSorted) cout << "Original Data:" << endl;
        else {
            showTime();
            cout << endl;
            cout << "Sorted Data:" << endl;
        }
        cout << "\tOID" << "\tArrival" << "\tDuration" << "\tTimeOut" << endl;

        for (int i = 0; i < length; i++) {
            cout << '(' << i+1 << ')' << '\t';
            showJob(aList[i]);
        }

        cout << endl;
        if (isSorted) {
            cout << "The sorted data has been included in \"sorted" << fileID << ".txt\"." << endl;
        }
    }

    void getSorted() {  // sort the datas and add a new file
        clock_t time = clock();
        int length = aList.size();
        int gap = length / 2;
        jobType temp;

        while (gap>=1) {
            for (int i = gap; i < length; i++) {
                int j = i;
                while (j >= gap && shell_compare(aList[j - gap], aList[j])) {
                    temp = aList[j - gap];
                    aList[j - gap] = aList[j];
                    aList[j] = temp;
                    j -= gap;
                }
            }
            gap /= 2;
        }

        timeSorting = clock() - time;
        putAll();
    }

    void nextJob(jobType &jobToGet) {   // declaration: get & remove the next job
        nextJobCheck(jobToGet);
        aList.erase(aList.begin());
    }

    string getFileID() {
        return fileID;
    }
};

template <typename T>
class JobQueue {
    vector<T> cA ;
    int qMax ;

    int next(int idx) {

    }

public:
    int avail ;

    JobQueue(): qMax(3), avail(0) {}

    int length() {
        return cA.size();
    }

    bool isEmpty() const {
        if (cA.size() == 0) return true;
        else return false;
    }

    bool isFull() {
        if (cA.size() >= qMax) return true;
        else return false;
    }

    void enQueue(T job) {
        if (!isFull()) {
            cA.push_back(job);
        }
    }

    void getFront(T& job) {
        if (!isEmpty())
            job = cA[0];
    }

    void deQueue() {
        if (!isEmpty()) {
            cA.erase(cA.begin());
        }
    }

    void deQueue(T& job) {
        getFront(job);
        deQueue();
    }

    void clearQ() {
        while (cA.size() != 0) {
            cA.clear();
        }
    }

    ~JobQueue() {
        clearQ();
    }
};

class AnsList {
    typedef struct aT {
        int OID;
        int abort;
        int delay;
    } abortType ;

    typedef struct dT {
        int OID;
        int departure;
        int delay;
    } doneType ;

    vector<abortType> abortJobs ;
    vector<doneType> doneJobs ;

    float avgDelay ;
    float successRate ;

    void showAbortJobs(string &screen) {    // convert abort jobs to string
        screen += "\t[Abort Jobs]\n";
        screen += "\tOID\tAbort\tDelay\n";

        for (int i = 0; i < abortJobs.size(); ++i) {
            screen += "[" + to_string(i + 1) + "]\t" + to_string(abortJobs[i].OID) + "\t" + to_string(abortJobs[i].abort) + "\t" + to_string(abortJobs[i].delay) + "\n";
        }
    }
    void showDoneJobs(string &screen) {     // convert done jobs to string
        screen += "\t[Jobs Done]\n";
        screen += "\tOID\tDeparture\tDelay\n";

        for (int i = 0; i < doneJobs.size(); ++i) {
            screen += "[" + to_string(i + 1) + "]\t" + to_string(doneJobs[i].OID) + "\t" + to_string(doneJobs[i].departure) + "\t" + to_string(doneJobs[i].delay) + "\n";
        }
    }
    void computeStat() {
        float sumDelay = 0.0;
        int count = abortJobs.size() + doneJobs.size();

        for (auto job : abortJobs) {
            sumDelay += job.delay;
        }

        for (auto job : doneJobs) {
            sumDelay += job.delay;
        }

        avgDelay = round(sumDelay / count * 100) / 100 ;
        successRate = round(float(doneJobs.size()) / count * 100 * 100) / 100;
    }

    void showStat(string &screen) {     // convert stats to string
        computeStat();
        screen += "[Average Delay]	" + to_string(avgDelay).erase(to_string(avgDelay).size()-4) + " ms\n";
        screen += "[Success Rate]	" + to_string(successRate).erase(to_string(successRate).size() - 4) + " %\n";
    }

public:
    AnsList(): avgDelay(0.0), successRate(0.0) {}

    void showAll(string &screen) {      // combine all strings then print on screen
        string aborts;
        string dones;
        string stats;
        showAbortJobs(aborts);
        showDoneJobs(dones);
        showStat(stats);

        screen = aborts + dones + stats;
        cout << endl << screen;
    }

    void addAbortJob(int OID, int abort, int delay) {
        aT job;
        job.OID = OID;
        job.abort = abort;
        job.delay = delay;
        abortJobs.push_back(job);
    }

    void addDoneJob(int OID, int departure, int delay) {
        dT job;
        job.OID = OID;
        job.departure = departure;
        job.delay = delay;
        doneJobs.push_back(job);
    }

    void putAll(string fileID) {   // show all and write into a file
        string result;
        showAll(result);

        ofstream outFile;
        outFile.open("output" + fileID + ".txt");
        outFile << result;
        outFile.close();
        cout << endl << "The result has been saved as \"output" << fileID << ".txt\"." << endl;
    }
};

struct event {
    jobType job;
    char mode = ' ';  // 'A' = arrive, 'D' = depart, 't' = timeout
    int time = 0;
};

class Simulation {
    JobList jobs ;
    AnsList answers ;
    jobType jobProcessing ;
    bool isWorking = false;
    JobQueue<jobType> allQ ;
    vector<event> events ;

    void readJob() {    // read job list, get new job and put into event list
        if (jobs.isEmpty()) return;

        event newEvent;
        jobs.nextJob(newEvent.job);
        newEvent.mode = 'A';
        newEvent.time = newEvent.job.arrival;
        addEvents(newEvent);
    }

    void update() {     // update event list and handle the events
        allQ.avail = events[0].time;

        do {
            if (events[0].mode == 'D') {
                answers.addDoneJob(jobProcessing.OID, events[0].time, events[0].time - jobProcessing.duration - jobProcessing.arrival);
                isWorking = false;
                events.erase(events.begin());
            }
            else if (events[0].mode == 'T') {   // Situation 4.3 reached
                answers.addAbortJob(jobProcessing.OID, events[0].time, jobProcessing.timeout - jobProcessing.arrival);
                isWorking = false;
                events.erase(events.begin());
            }
            else if (events[0].mode == 'A') {
                if (allQ.isFull()) {    // Situation 4.1 reached
                    answers.addAbortJob(events[0].job.OID, events[0].time, 0);
                }
                allQ.enQueue(events[0].job);
                events.erase(events.begin());
            }

            while (!isWorking && !allQ.isEmpty()) {
                jobType temp;
                allQ.getFront(temp);
                if (temp.timeout < allQ.avail) {    // Situation 4.2 reached
                    allQ.deQueue();
                    answers.addAbortJob(temp.OID, allQ.avail, allQ.avail - temp.arrival);
                }
                else {
                    allQ.deQueue(jobProcessing);
                    isWorking = true;

                    if (allQ.avail + jobProcessing.duration > jobProcessing.timeout) {  // Situation 4.3, add timeout event
                        event newEvent;
                        newEvent.job = jobProcessing;
                        newEvent.mode = 'T';
                        newEvent.time = jobProcessing.timeout;
                        addEvents(newEvent);
                    }
                    else {
                        event newEvent;
                        newEvent.job = jobProcessing;
                        newEvent.mode = 'D';
                        newEvent.time = allQ.avail + jobProcessing.duration;
                        addEvents(newEvent);
                    }
                }
            }
        } while (events.size() != 0 && allQ.avail == events[0].time) ;
    }

    void addEvents(event newEvent) {
        events.push_back(newEvent);
        if (events.size() >= 2) {
            for (int i = events.size() - 1; i > 0; --i) {
                if (!compareEvent(events[i - 1], events[i]))
                    swap(events[i - 1], events[i]);
            }
        }
    }

    bool compareEvent(event a, event b) {   // sort the event list
        if (a.time < b.time) return true;
        else if (a.time == b.time && (b.mode == 'D' || b.mode == 'T') && (a.mode != 'D' && a.mode != 'T')) return true;
        else if (a.time == b.time) return true;
        else return false;
    }

public:
    Simulation(JobList aList): jobs(aList) {}

    void SQF() {

        if (jobs.isEmpty()) {
            cout << "There is no job waiting for processing." << endl << "The simulation ends." << endl;
            return;
        }

        cout << "Simulating..." << endl;

        while (!jobs.isEmpty() || !allQ.isEmpty() || isWorking) {
            readJob();
            update();
        }

        answers.putAll(jobs.getFileID());
        cout << "The simulation ends." << endl;
    }
};

int main() {
    int command = 0; // user command
    JobList job;
    AnsList ans;
    string num;

    do {
        cout << endl << "**** Simulate FIFO Queues by SQF *****";
        cout << endl << "* 0. Quit                            *";
        cout << endl << "* 1. Sort a file                     *";
        cout << endl << "* 2. Simulate one FIFO queue         *";
        cout << endl << "**************************************";
        cout << endl << "Input a command(0, 1, 2): ";
        cin >> command; // get the command

        switch (command) {
        case 0:
            break;
        case 1:
            cout << endl << "Input a file number: ";
            cin >> num;
            if (job.getAll(num, false)) {
                job.print(false);
                job.getSorted();
                job.print(true);
            }

            break;
        case 2:
            cout << endl << "Input a file number: ";
            cin >> num;
            if (job.getAll(num, true)) {
                Simulation s(job);
                s.SQF();
            }

            break;
        default:
            cout << endl << "Command does not exist!" << endl;
            break;
        }

    } while (command != 0);
}