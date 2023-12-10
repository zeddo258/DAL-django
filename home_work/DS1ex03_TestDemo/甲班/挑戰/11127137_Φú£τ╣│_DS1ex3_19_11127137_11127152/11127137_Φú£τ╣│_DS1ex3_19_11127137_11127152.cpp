// 11127137, 黃乙家, 11127152 郭怡楨
/****************************************************/
/*  CPP Template for School                         */
/*  Author: CompileErr0r(YiJia)                     */
/*  Author ID: 11127137                             */
/*  Compile Environment: Linux 5.15.90.1 WSL x86_64 */
/*  Compiler: g++ 9.4                               */
/****************************************************/

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <new>
#include <vector>

// #include <bits/stdc++.h>
using namespace std;

#define endl '\n'

// #define CompileErr0rDEBUGGING

template <typename... rest>
void VERBOSE(rest... args) {
#ifdef CompileErr0rDEBUGGING
    cout << "\033[1;37m[\033[1;33mDEBUG\033[1;37m]\033[0m ";
    initializer_list<int>{(cout << args, 0)...};
#endif
}

const int queueMax = 3;
template <class T>
class Queue {
    struct Node { // circular doubly linked lists
        T data;
        Node *next;
        Node *prev;
        Node(const T &d) : data(d), next(nullptr), prev(nullptr) {}
    };
    Node *head;
    Node *tail;
    int size;

   public:
    Queue() : head(nullptr), tail(nullptr), size(0) {}
    void PrintAll() {
        Node *temp = head;
        for (int i = 0; i < size; ++i) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
    int length() const { return size; }

    bool isEmpty() const { return size == 0; }

    bool isFull() const { return length() >= queueMax; }

    void push(const T &input) {
        Node **h = &head;
        Node **t = &tail;
        Node *newNode = new Node(input);
        if (isEmpty()) {
            *h = newNode;
            *t = newNode;
            newNode->next = newNode;
            newNode->prev = newNode;
        } else {
            (*t)->next = newNode;
            newNode->prev = *t;
            newNode->next = *h;
            (*h)->prev = newNode;
            *t = newNode;
        }
        size++;
    }
    T &getFront() {
        if (head == nullptr) {
            cerr << "\033[1;31mQueue is empty!\033[0m" << endl;
        }
        return head->data;
    }
    void getFront(T &first) {
        if (head == nullptr) {
            cerr << "\033[1;31mQueue is empty!\033[0m" << endl;
        }
        first = head->data;
    }

    void pop() {
        Node **h = &head;
        Node **t = &tail;
        if (isEmpty()) {
            cerr << "\033[1;31mQueue is empty!\033[0m" << endl;
        } else if (length() == 1) {
            delete *h;
            *h = nullptr;
            *t = nullptr;
        } else {
            Node *temp = *h;
            *h = (*h)->next;
            (*h)->prev = *t;
            (*t)->next = *h;
            delete temp;
        }
        size--;
    }

    void clear() { // clean up
        while (!isEmpty()) {
            pop();
        }
    }
    ~Queue() { // destructor
        clear();
    }

}; // end Queue

class Data {
   public:
    struct Process {
        int OID, Arrival, Duration, Timeout;
        bool operator<(const Process &p) const {
            if (Arrival == p.Arrival) {
                return OID < p.OID;
            }
            return Arrival < p.Arrival;
        }
        bool operator==(const Process &p) const { return (OID == p.OID && Arrival == p.Arrival); }
        bool operator>(const Process &p) const {
            if (Arrival == p.Arrival) {
                return OID > p.OID;
            }
            return Arrival > p.Arrival;
        }
        friend ostream &operator<<(ostream &os, const Process &p) {
            os << p.OID;
            return os;
        }
    };
    struct Process_State {
        int OID;
        int CID;
        int Finish;
        int Delay;
        Process_State() : OID(0), CID(0), Finish(0), Delay(0) {}
        Process_State(int o, int f, int d) : OID(o), CID(1), Finish(f), Delay(d) {}
        Process_State(int o, int c, int f, int d) : OID(o), CID(c), Finish(f), Delay(d) {}
    };

   public:
    static void shellsort(vector<Process> &p) {
        int n = p.size();
        for (uint32_t gap = n >> 1; gap > 0; gap >>= 1) {
            for (int i = gap; i < n; i++) {
                Process temp = p[i];
                uint32_t j;
                for (j = i; j >= gap; j -= gap) {
                    if (p[j - gap] > temp) {
                        p[j] = p[j - gap];
                    } else {
                        break;
                    }
                }
                p[j] = temp;
            }
        }
    }
};

class ProcessSimulator {
    typedef Data::Process Process;
    typedef Data::Process_State Process_State;
    vector<Process> ProcessList;
    vector<Process_State> Success;
    vector<Process_State> Fail;
    vector<Queue<Process>> CPU_Queue_arr;

   public:
    double avg_delay;
    double success_rate;
    /*
        constructor
        @param p: the process list
    */
    ProcessSimulator(vector<Process> &p, int cpunum)
        : ProcessList(p), CPU_Queue_arr(cpunum, Queue<Process>()) {}
    /*
        Get the statistic of the simulation
        @side effect: update the avg_delay and success_rate
    */
    void GetStatistic() {
        avg_delay = 0;
        for (auto o : Success) {
            avg_delay += o.Delay;
        }
        for (auto o : Fail) {
            avg_delay += o.Delay;
        }
        if (Success.size() + Fail.size() == 0) {
            avg_delay = 0;
            success_rate = 0;
        } else {
            avg_delay /= (Success.size() + Fail.size());
            success_rate = (double)Success.size() / (Success.size() + Fail.size()) * 100;
        }
    }
    /*
        process an order
        @param now: the current time
        @param o: the order
        @side effect: update the now, and push the result to Success or Fail list
    */
    void ProcessOrder(int &now, Process o, int cid) {
        if (now < o.Arrival) now = o.Arrival;
        if (now > o.Timeout) {
            VERBOSE("\033[1;31mOrder ", o.OID, " is cancelled, CPU is ", cid, ", cpu clock = ", now,
                    "\033[0m", endl);
            Fail.push_back(Process_State(o.OID, cid, now, now - o.Arrival));
            return;
        }
        int finish = now + o.Duration;
        if (finish > o.Timeout) {
            // cancel
            now = o.Timeout;
            VERBOSE("\033[1;31mTimeout when processing, Order ", o.OID, " is cancelled, CPU is ", cid,
                    ", cpu clock = ", now, "\033[0m", endl);
            Fail.push_back(Process_State(o.OID, cid, o.Timeout, o.Timeout - o.Arrival));
        } else {
            // process

            Success.push_back(Process_State(o.OID, cid, finish, now - o.Arrival));
            now = finish;
            VERBOSE("\033[1;32mOrder ", o.OID, " in Queue is processed, CPU is ", cid,
                    ", cpu clock = ", finish, "\033[0m", endl);
            //  success
        }
    }
    /*
        process the orders in queue
        @param now: the current time
        @param orders: the queue
        @side effect: update the now, and push the result to Success or Fail list
    */
    void ProcessQueue(int &now, Queue<Process> &orders, int cid) {
        if (!orders.isEmpty()) {
            Process o;
            orders.getFront(o);
            orders.pop();
            ProcessOrder(now, o, cid);
        }
    }
    /*
        CPU simulation
        @param CPU_Queue_arr: the array of queues of CPUs
        @side effect: Push the result to Success or Fail list and update the statistic
    */
    void Simulate() {
        int cpu_num = CPU_Queue_arr.size();
        vector<int> cpu_curtime_arr(cpu_num, 0);
        for (auto o : ProcessList) {
            VERBOSE("\033[1;34mOrder ", o.OID, " is coming.\033[0m", endl);
            for (int i = 0; i < cpu_num; ++i) { // if there is some idle cpu, process queue till now
                while (!CPU_Queue_arr[i].isEmpty() && cpu_curtime_arr[i] <= o.Arrival) {
                    ProcessQueue(cpu_curtime_arr[i], CPU_Queue_arr[i], i + 1);
                }
            }

            // choose which cpu is idle
            int idle_cpu = -1;
            for (int i = 0; i < cpu_num; ++i) {
                if (cpu_curtime_arr[i] <= o.Arrival) {
                    idle_cpu = i;
                    // cpu_curtime_arr[i] = o.Arrival;
                    break;
                }
            }

            // if there is an idle cpu, process the order
            if (idle_cpu != -1) {
                if (cpu_curtime_arr[idle_cpu] < o.Arrival) { // there is no any order to process
                    cpu_curtime_arr[idle_cpu] = o.Arrival;   // move the time to the next order's arrival time
                    VERBOSE("\033[1;35m<!>\033[0m CPU ", idle_cpu + 1, " is idle, process order ", o.OID,
                            endl);
                    ProcessOrder(cpu_curtime_arr[idle_cpu], o, idle_cpu + 1);
                } else {
                    VERBOSE("\033[1;35m<!>\033[0m CPU ", idle_cpu + 1, " is idle, enqueue order ", o.OID,
                            endl);
                    CPU_Queue_arr[idle_cpu].push(o);
                }

                continue;
            }

            // if there is no idle cpu, choose the cpu with the shortest queue
            int shortest_queue = 0;
            for (int i = 1; i < cpu_num; ++i) {
                if (CPU_Queue_arr[i].length() < CPU_Queue_arr[shortest_queue].length()) {
                    shortest_queue = i;
                }
            }
            VERBOSE("\033[1;35m<!>\033[0m CPU ", shortest_queue + 1, " is chosen, enqueue order ", o.OID,
                    endl);
            if (!CPU_Queue_arr[shortest_queue].isFull()) {
                CPU_Queue_arr[shortest_queue].push(o);
            } else { // queue is full
                VERBOSE("\033[1;31mQueue is full, Order ", o.OID,
                        " is rejected, now=", cpu_curtime_arr[shortest_queue], "\033[0m", endl);
                Fail.push_back(Process_State(o.OID, 0, o.Arrival, 0));
            }
        }

        int idx = 0;
        while (idx >= 0) {
            idx = -1;
            for (int i = 0, minval = -1; i < cpu_num; ++i) { // process closest order
                if (!CPU_Queue_arr[i].isEmpty() && (minval == -1 || minval > cpu_curtime_arr[i])) {
                    idx = i;
                    minval = cpu_curtime_arr[i];
                }
            }
            if (idx < 0) break;
            ProcessQueue(cpu_curtime_arr[idx], CPU_Queue_arr[idx], idx + 1);
        }

        GetStatistic();
    }

    vector<Process> GetProcessList() { return ProcessList; }
    vector<Process_State> GetSuccessList() { return Success; }
    vector<Process_State> GetFailList() { return Fail; }
    void clear() {
        ProcessList.clear();
        Success.clear();
        Fail.clear();
    }
    void operator()(vector<Process> &p) {
        ProcessList = p;
        CPU_Queue_arr.push_back(Queue<Process>());
    }
    void operator()(vector<Process> &p, int cpunum) {
        ProcessList = p;
        CPU_Queue_arr = vector<Queue<Process>>(cpunum, Queue<Process>());
    }

    ~ProcessSimulator() { clear(); }
};

class Solution {
    typedef Data::Process Process;
    typedef Data::Process_State Process_State;
    vector<Process> ProcessList;
    int64_t Read_Time;
    int64_t Sort_Time;
    int64_t Write_Time;
    ProcessSimulator process_simulator;

   public:
    Solution() : Read_Time(0), Sort_Time(0), Write_Time(0), process_simulator(ProcessList, 1) {}

    /*
        load a file from user's input
        @param fp: the file stream
        @param fout: the output file stream
        @param filename: the file name
        @param caseNum: 1 for case 1, 2 for case 2
        @side effect: initialize the file stream
    */
    void GetFile(ifstream &fp, ofstream &fout, string &filename, int caseNum) {
        // do {
        string output_filename;
        string input_filename;
        cout << "\nInput a file number: ";
        cin >> filename;
        /*
        if (filename == "*") {
            return;
        }
        if (filename.size() == 3) {
            if (caseNum == 1)
                filename = "input" + filename + ".txt";
            else
                filename = "sorted" + filename + ".txt";
        }*/
        if (caseNum == 1) {
            input_filename = "input" + filename + ".txt";
            output_filename = "sorted" + filename + ".txt";
        } else if (caseNum == 2) {
            input_filename = "sorted" + filename + ".txt";
            output_filename = "output" + filename + ".txt";
        } else {
            input_filename = "sorted" + filename + ".txt";

            output_filename = "double" + filename + ".txt";
        }
        fp.open(input_filename.c_str());
        if (!fp.is_open()) {
            cout << "\n### " << input_filename << " does not exist! ###" << endl;
            filename = "*";
            return;
        }
        //} while (!fp.is_open());
        fout.open(output_filename);
        filename = output_filename;
        return;
    }
    /*
        load a file from user's input
        @param fp: the file stream
        @param filename: the file name
        @param caseNum: 1 for case 1, 2 for case 2
        @side effect: initialize the file stream
    */
    void GetFile(ifstream &fp, string &filename, int caseNum) {
        // do {
        string input_filename;
        cout << "\nInput a file number: ";
        cin >> filename;
        /*if (filename == "*") {
            return;
        }
        if (filename.size() == 3) {
            if (caseNum == 1)
                filename = "input" + filename + ".txt";
            else
                filename = "sorted" + filename + ".txt";
        }*/
        if (caseNum == 1) {
            input_filename = "input" + filename + ".txt";
        } else {
            input_filename = "sorted" + filename + ".txt";
        }
        fp.open(input_filename.c_str());
        if (!fp.is_open()) {
            cout << "\n### " << input_filename << " does not exist! ###" << endl;
            filename = "*";
            return;
        }
        //} while (!fp.is_open());
        filename = input_filename;
        return;
    }
    /*
        load process list from file
        @param fp: the file stream
        @return: the process list
    */
    vector<Process> getProcessList(ifstream &fp) {
        vector<Process> ProcessList;
        chrono::steady_clock::time_point start = chrono::steady_clock::now();
        string sch;
        for (int i = 0; i < 4; ++i) {
            fp >> sch; // read the first row
        }
        int n = 0;
        while (fp >> sch) {
            Process o;
            o.OID = stoi(sch);
            fp >> o.Arrival >> o.Duration >> o.Timeout;
            ProcessList.push_back(o);
            n++;
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        Read_Time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        return ProcessList;
    }
    /*
        output process list
    */
    void WriteProcessList() {
        cout << "\n\tOID\tArrival\tDuration\tTimeOut" << endl;
        int n = ProcessList.size();
        for (int i = 0; i < n; ++i) {
            cout << "(" << i + 1 << ")"
                 << "\t" << ProcessList[i].OID << "\t" << ProcessList[i].Arrival << "\t"
                 << ProcessList[i].Duration << "\t" << ProcessList[i].Timeout << endl;
        }
    }
    /*
        output process list to ofstream
        @param fout: the output file stream
    */
    void WriteProcessList(ofstream &fout) {
        chrono::steady_clock::time_point start = chrono::steady_clock::now();
        fout << "OID\tArrival\tDuration\tTimeOut" << endl;
        int n = ProcessList.size();
        for (int i = 0; i < n; ++i) {
            fout << ProcessList[i].OID << '\t' << ProcessList[i].Arrival << '\t' << ProcessList[i].Duration
                 << '\t' << ProcessList[i].Timeout << endl;
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        Write_Time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    /*
        Sort process list
        @side effect: update the ProcessList and write the result to file
    */
    void Case1() {
        string filename;
        ifstream fp;
        ofstream fout;
        GetFile(fp, fout, filename, 1);
        if (filename == "*") {
            return;
        }

        ProcessList = getProcessList(fp);
        WriteProcessList();

        chrono::steady_clock::time_point start = chrono::steady_clock::now();
        Data::shellsort(ProcessList);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        Sort_Time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        WriteProcessList(fout);

        cout << "\nReading data: " << Read_Time << " ms." << endl;
        cout << "Sorting data: " << Sort_Time << " ms." << endl;
        cout << "Writing data: " << Write_Time << " ms." << endl;

        cout << "\nSee " << filename << endl;
        fp.close();
        fout.close();
    }

    /*
        output the result
    */
    void WriteResult() {
        cout << "\t[Abort Jobs]" << endl;
        cout << "\tOID\tAbort\tDelay" << endl;
        int i = 1;
        vector<Process_State> Fail = process_simulator.GetFailList();
        vector<Process_State> Success = process_simulator.GetSuccessList();
        for (auto o : Fail) {
            cout << "[" << i << "]\t" << o.OID << "\t" << o.Finish << "\t" << o.Delay << endl;
            i++;
        }

        cout << "\t[Jobs Done]" << endl;
        cout << "\tOID\tDeparture\tDelay" << endl;
        i = 1;
        for (auto o : Success) {
            cout << "[" << i << "]\t" << o.OID << "\t" << o.Finish << "\t" << o.Delay << endl;
            i++;
        }

        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "[Average Delay]\t" << process_simulator.avg_delay << " ms" << endl;
        cout << "[Success Rate]\t" << process_simulator.success_rate << " %" << endl;
    }
    /*
        output the result to ofstream
        @param fout: the output file stream
    */
    void WriteResult(ofstream &fout) {
        fout << "\t[Abort Jobs]" << endl;
        fout << "\tOID\tAbort\tDelay" << endl;
        int i = 1;
        vector<Process_State> Fail = process_simulator.GetFailList();
        vector<Process_State> Success = process_simulator.GetSuccessList();
        for (auto o : Fail) {
            fout << "[" << i << "]\t" << o.OID << "\t" << o.Finish << "\t" << o.Delay << endl;
            i++;
        }

        fout << "\t[Jobs Done]" << endl;
        fout << "\tOID\tDeparture\tDelay" << endl;
        i = 1;
        for (auto o : Success) {
            fout << "[" << i << "]\t" << o.OID << "\t" << o.Finish << "\t" << o.Delay << endl;
            i++;
        }

        fout.setf(ios::fixed);
        fout.precision(2);
        fout << "[Average Delay]\t" << process_simulator.avg_delay << " ms" << endl;
        fout << "[Success Rate]\t" << process_simulator.success_rate << " %" << endl;
    }
    /*
        output the result to ofstream with CPU id
        @param fout: the output file stream
    */
    void WriteResultCID(ofstream &fout) {
        fout << "\t[Abort Jobs]" << endl;
        fout << "\tOID\tCID\tAbort\tDelay" << endl;
        int i = 1;
        vector<Process_State> Fail = process_simulator.GetFailList();
        vector<Process_State> Success = process_simulator.GetSuccessList();
        for (auto o : Fail) {
            fout << "[" << i << "]\t" << o.OID << "\t" << o.CID << "\t" << o.Finish << "\t" << o.Delay
                 << endl;
            i++;
        }

        fout << "\t[Jobs Done]" << endl;
        fout << "\tOID\tCID\tDeparture\tDelay" << endl;
        i = 1;
        for (auto o : Success) {
            fout << "[" << i << "]\t" << o.OID << "\t" << o.CID << "\t" << o.Finish << "\t" << o.Delay
                 << endl;
            i++;
        }

        fout.setf(ios::fixed);
        fout.precision(2);
        fout << "[Average Delay]\t" << process_simulator.avg_delay << " ms" << endl;
        fout << "[Success Rate]\t" << process_simulator.success_rate << " %" << endl;
    }

    /*
        simulataion of process orders and output the result
    */
    void Case2() {
        string filename;
        ifstream fp;
        ofstream fout;
        GetFile(fp, fout, filename, 2);
        if (filename == "*") {
            return;
        }

        cout << "\nThe simulation is running..." << endl;
        ProcessList = getProcessList(fp);
        process_simulator(ProcessList, 1);
        process_simulator.Simulate();
        WriteResult(fout);
        cout << "See " << filename << endl;
        fp.close();
    }
    /*
        simulataion of process orders in multi-CPU and output the result
    */
    void Case3() {
        string filename;
        ifstream fp;
        ofstream fout;
        GetFile(fp, fout, filename, 3);
        if (filename == "*") {
            return;
        }

        cout << "\nThe simulation is running..." << endl;
        ProcessList = getProcessList(fp);
        process_simulator(ProcessList, 2);
        process_simulator.Simulate();
        WriteResultCID(fout);
        cout << "See " << filename << endl;
        fp.close();
    }
};
void WriteMenu() {
    cout << "\n**** Simulate FIFO Queues by SQF *****\n"
            "* 0. Quit                            *\n"
            "* 1. Sort a file                     *\n"
            "* 2. Simulate one FIFO queue         *\n"
            "* 3. Simulate two queues by SQF      *\n"
            "**************************************\n";
    cout << "Input a command(0, 1, 2, 3): ";
}
int main() {
    string command;

    // use string to avoid malicious input
    WriteMenu();
    while (cin >> command && command != "0") {
        cin.ignore();
        Solution s;
        if (command == "1") {
            s.Case1();
            // cerr << "\033[1;32mTask Passed\033[0m" << endl;
        } else if (command == "2") {
            s.Case2();
        } else if (command == "3") {
            s.Case3();
        } else
            cout << "\nCommand does not Exist!!!" << endl;
        WriteMenu();
    }
    return 0;
}
