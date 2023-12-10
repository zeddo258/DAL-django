//*********************************************************************//
// 10827128 10827142
//********************************************************************************/
// #include "DS1Simulation.hpp" // class JobQueue
#include <stdio.h>
#include <string.h> // string class
#include <time.h>
#include <windows.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string> // string class
#include <vector>
#include <cmath>

using namespace std;

struct CPU
{
    int OID = 0;
    int Arrival = 0;
    int Duration = 0;
    int TimeOut = 0;
};

class Interface
{
public:
    void printInfo()
    {
        cout << endl
             << "**** Simulate FIFO Queues by SQF ****";
        cout << endl
             << "* 0. Quit                       *";
        cout << endl
             << "* 1. Sort a file                *";
        cout << endl
             << "* 2. Simulate one FIFO queue    *";
        cout << endl
             << "* 3. Simulate two queues by SQF *";
        cout << endl
             << "*******************************";
        cout << endl
             << "Input a command(0, 1, 2, 3): ";
    }
};

class Queue
{
public:
    vector<CPU> process;

    bool isEmpty()
    {
        return process.empty();
    } // isEmpty

    void enQueue(CPU CPUlist)
    {
        process.push_back(CPUlist);
    }

    void deQueue()
    {
        if (!process.empty())
        {
            process.erase(process.begin());
        }
    }

    CPU getFront()
    {
        CPU temp;
        if (process.empty() == true)
            return temp;
        else
            temp = process[0];
        return temp;
    }

    int size()
    {
        return process.size();
    }
};

struct Task
{
    int OID = 0;
    int CID = 0;
    int Delay = 0;
    int AbortorDeparture = 0;
};

string InttoString(int num)
{
    stringstream ss;
    ss << num;
    string out_string = ss.str();
    return out_string;
} // ItoS()

class Readfile
{
public:
    bool openFile(const string filename)
    {
        file.open(filename);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << filename << endl;
            return false;
        }
        return true;
    }

    vector<CPU> readData()
    {
        vector<CPU> cpuVector;
        CPU cpu;
        string firstline;
        getline(file, firstline);

        while (file >> cpu.OID >> cpu.Arrival >> cpu.Duration >> cpu.TimeOut)
        {
            cpuVector.push_back(cpu);
        }

        return cpuVector;
    }

private:
    ifstream file;
};

void shellSort(vector<CPU> &cpuVector)
{
    int n = cpuVector.size();
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            CPU temp = cpuVector[i];
            int j;
            for (j = i; j >= gap && (cpuVector[j - gap].Arrival > temp.Arrival || (cpuVector[j - gap].Arrival == temp.Arrival && cpuVector[j - gap].OID > temp.OID)); j -= gap)
            {
                cpuVector[j] = cpuVector[j - gap];
            }
            cpuVector[j] = temp;
        }
    }
}

// Function to measure time
double measureTime(clock_t start, clock_t end)
{
    return double(end - start) / CLOCKS_PER_SEC * 1000.0; // Convert to milliseconds
}

class Simulation
{
private:
    vector<CPU> cpuVector;
    vector<Queue> cpuQueue;
    vector<Task> doneList;
    vector<Task> abortList;
    int currentTime[2] = {0, 0};
    int preTime = -1;

public:
    Simulation(int numCPUs)
    {
        cpuQueue.resize(numCPUs); // 根據 CPU 數量調整佇列數目
    }

    bool loadTasks(const string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << filename << endl;
            return false;
        }

        CPU cpu;
        string s;
        getline(file, s);
        while (file >> cpu.OID >> cpu.Arrival >> cpu.Duration >> cpu.TimeOut)
        {
            cpuVector.push_back(cpu);
        }

        file.close();
        return true;
    }

    void simulate()
    {
        int index = 0;
        while (index < cpuVector.size() || anyQueueNotEmpty(cpuQueue))
        {
            cout << index;
            while (index < cpuVector.size() && (cpuVector[index].Arrival <= currentTime[0] || cpuVector[index].Arrival <= currentTime[1]))
            {
                // 選擇要加入的佇列
                int selectedQueueIndex = selectQueueIndex();
                if (selectedQueueIndex != -1 && cpuQueue[selectedQueueIndex].size() < 3 && selectedQueueIndex != 2)
                {
                    cpuQueue[selectedQueueIndex].enQueue(cpuVector[index]);
                }
                else if (preTime == cpuVector[index].Arrival && selectedQueueIndex == 2)
                { // right now is the current task end time, it's still in queue, need to manually dequeue it and execute
                    excute(1);
                    cpuQueue[1].enQueue(cpuVector[index]);
                }
                else
                {
                    addToAbortList(cpuVector[index], cpuVector[index].Arrival, 0);
                }
                index++;
            }

            if (anyQueueNotEmpty(cpuQueue))
            {
                for (int i = 0; i < cpuQueue.size(); ++i)
                {
                    if (cpuQueue[i].size() > 0)
                    {
                        excute(i);
                    }
                }
            }
            else
            {
                if (index < cpuVector.size())
                {
                    currentTime[0]++;
                    currentTime[1]++;
                }
            }
        }

        // Calculate average delay and success rate
        double totalDelay = 0;
        for (const Task &task : doneList)
        {
            totalDelay += task.Delay;
        }

        for (const Task &task : abortList)
        {
            totalDelay += task.Delay;
        }

        double averageDelay = totalDelay / (doneList.size() + abortList.size());
        double successRate = static_cast<double>(doneList.size()) / (doneList.size() + abortList.size()) * 100;

        // Output results
        AD = round(averageDelay * 100) / 100;
        SR = round(successRate * 100) / 100;
    }

    void writeResults(const string &filename)
    {
        outputFile.open(filename);
        if (!outputFile.is_open())
        {
            cerr << "Error creating file: " << filename << endl;
            return;
        }

        // Write headers

        outputFile << "\t[Abort Jobs]" << endl;
        outputFile << '\t' << "OID" << setw(10) << "Abort" << setw(8) << "Delay" << endl;
        int i = 1;
        for (const Task &task : abortList)
        {
            outputFile << '[' << i++ << left << setw(6) << ']' << setw(8) << task.OID << setw(8) << task.AbortorDeparture << task.Delay << endl;
        }

        outputFile << "\t[Jobs Done]" << endl;
        outputFile << right << '\t' << "OID" << setw(14) << "Departure" << setw(8) << "Delay" << endl;
        i = 1;
        for (const Task &task : doneList)
        {
            outputFile << '[' << i++ << left << setw(6) << ']' << setw(8);
            outputFile << left << task.OID << setw(8) << task.AbortorDeparture << setw(22) << task.Delay << endl;
        }

        outputFile << "[Average Delay] " << round(AD * 100) / 100 << " ms" << endl;
        outputFile << "[Success Rate]  " << round(SR * 100) / 100 << "%" << endl;

        outputFile.close();
        // cout << "Results written to " << filename << endl;
    }

private:
    ofstream outputFile;
    double AD;
    double SR;

    void excute(int queueIndex)
    {
        CPU currentTask = cpuQueue[queueIndex].getFront();
        cpuQueue[queueIndex].deQueue();
        // Check if the task is canceled due to timeout
        if (currentTask.TimeOut <= currentTime[queueIndex])
        {
            addToAbortList(currentTask, currentTime[queueIndex], currentTime[queueIndex] - currentTask.Arrival);
        }
        else if (currentTime[queueIndex] + currentTask.Duration > currentTask.TimeOut)
        {
            addToAbortList(currentTask, currentTask.TimeOut, currentTask.TimeOut - currentTask.Arrival);
            currentTime[queueIndex] = currentTask.TimeOut;
            preTime = currentTime[queueIndex];
        }
        else
        {
            // Task is executed
            int executionTime = min(currentTask.Duration, currentTask.TimeOut - currentTime[queueIndex]);
            addToDoneList(currentTask, currentTime[queueIndex] + executionTime, currentTime[queueIndex] - currentTask.Arrival);
            currentTime[queueIndex] += executionTime;
            preTime = currentTime[queueIndex];
        }
    }

    bool anyQueueNotEmpty(vector<Queue> queues)
    {
        for (Queue &queue : queues)
        {
            if (!queue.isEmpty())
            {
                return true; // 如果有任何一個佇列不為空，返回 true
            }
        }
        return false; // 所有佇列都為空，返回 false
    }

    int selectQueueIndex()
    {
        int selectedQueueIndex = -1;
        if (cpuQueue[0].size() == 3 && cpuQueue[1].size() == 3)
            return 2;

        // 找到所有空佇列
        vector<int> emptyQueues;
        for (int i = 0; i < cpuQueue.size(); ++i)
        {
            if (cpuQueue[i].isEmpty())
            {
                emptyQueues.push_back(i);
            }
        }

        if (emptyQueues.size() == 1)
        {
            // Case 1: 只有一個佇列是空的，選唯一閒置的CPU
            selectedQueueIndex = emptyQueues[0];
        }
        else if (emptyQueues.size() > 1)
        {
            // Case 2: 不只一個佇列是空的，選那些空佇列中「index」最小者
            int minCpuIndex = 0;
            selectedQueueIndex = emptyQueues[minCpuIndex];
        }
        else
        {
            // Case 3: 每個佇列都並非空的且至少一個佇列並非全滿，選佇列長度最短的
            int minQueueIndex = findMinQueueIndex();
            selectedQueueIndex = minQueueIndex;
        }

        return selectedQueueIndex;
    }

    int findMinQueueIndex()
    {
        int minIndex = 0;
        for (int i = 1; i < cpuQueue.size(); ++i)
        {
            if (cpuQueue[i].size() < cpuQueue[minIndex].size())
            {
                minIndex = i;
            }
        }
        return minIndex;
    }

    void addToDoneList(const CPU &cpu, int departure, int delay)
    {
        Task task;
        task.OID = cpu.OID;
        task.AbortorDeparture = departure;
        task.Delay = delay;
        doneList.push_back(task);
    }

    void addToAbortList(const CPU &cpu, int abortTime, int delay)
    {
        Task task;
        task.OID = cpu.OID;
        task.AbortorDeparture = abortTime;
        task.Delay = delay;
        abortList.push_back(task);
    }
};

int main(void)
{
    int command = 0; // user command

    do
    { // JobList orders;  // a list of orders
        Interface interfaces;
        interfaces.printInfo();
        int filenum = 0;
        string filename = "";
        cin >> command; // get the command
        switch (command)
        {
        case 0:
            break;
        case 1:
        {
            cout << "Input a file number (e.g., 301, 302, 303, ...): ";
            cin >> filenum;
            Readfile readfile;
            filename = "input" + InttoString(filenum) + ".txt";
            clock_t readingStart = clock();
            if (readfile.openFile(filename))
            {
                clock_t readingEnd = clock();
                filename = "sorted" + InttoString(filenum) + ".txt";
                ofstream outputFile(filename);
                Queue cpuList;
                cpuList.process = readfile.readData();
                clock_t sortingStart = clock();
                shellSort(cpuList.process);
                clock_t sortingEnd = clock();

                clock_t outputStart = clock();
                outputFile << "OID\tArrival\tDuration\tTimeOut" << endl;
                for (const CPU &cpu : cpuList.process)
                {
                    outputFile << cpu.OID << "\t" << cpu.Arrival << "\t" << cpu.Duration << "\t" << cpu.TimeOut << endl;
                }
                clock_t outputEnd = clock();

                outputFile.close();
                cout << "Reading time: " << measureTime(readingStart, readingEnd) << " ms" << endl;
                cout << "Sorting time: " << measureTime(sortingStart, sortingEnd) << " ms" << endl;
                cout << "Writing time: " << measureTime(outputStart, outputEnd) << " ms" << endl;

            } // if()
            break;
        }
        case 2:
        {
            cout << "Input a file number (e.g., 301, 302, 303, ...): ";
            cin >> filenum;
            Simulation simulation = Simulation(1);
            filename = "sorted" + InttoString(filenum) + ".txt";
            if (simulation.loadTasks(filename))
            { // Change the filename accordingly
                simulation.simulate();
                filename = "out" + InttoString(filenum) + ".txt";
                simulation.writeResults(filename);
            }
            break;
        }
        case 3:
        {
            cout << "Input a file number (e.g., 301, 302, 303, ...): ";
            cin >> filenum;
            Simulation simulation = Simulation(2);
            filename = "sorted" + InttoString(filenum) + ".txt";
            if (simulation.loadTasks(filename))
            { // Change the filename accordingly
                simulation.simulate();
                filename = "out" + InttoString(filenum) + ".txt";
                simulation.writeResults(filename);
            }
            break;
        }
        default:
            cout << endl
                 << "Command does not exist!" << endl;
        } // end switch
    } while (command != 0);
    return 0; // '0': stop the program
} // end of main