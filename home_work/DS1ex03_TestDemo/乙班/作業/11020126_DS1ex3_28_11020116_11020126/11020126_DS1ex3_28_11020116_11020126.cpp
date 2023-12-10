// 電資三 11020116 潘皓群  電資三 11020126 鄭祐昀 
// using c++ 17
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <tuple>
#include <iomanip>
#include <cmath>
#include <time.h>
#include <string>

class Job {
private:
    int OID;
    int Arrival;
    int Duration;
    int TimeOut;

public:
    Job(int oid, int arrival, int duration, int timeout)
        : OID(oid), Arrival(arrival), Duration(duration), TimeOut(timeout) {}

    // Getters for the private members
    int getOID() const { return OID; }
    int getArrival() const { return Arrival; }
    int getDuration() const { return Duration; }
    int getTimeOut() const { return TimeOut; }

    // Compare the order of two jobs using < operator as before
    bool operator<(const Job &other) const {
        if (Arrival != other.Arrival) {
            return Arrival < other.Arrival;
        }
        return OID < other.OID;
    }
};

class CpuQueue {
private:
    struct Node {
        Job data;
        Node* next;
        Node(const Job& d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t current_size;
    static const size_t MAX_SIZE = 3;

public:
    CpuQueue() : head(nullptr), tail(nullptr), current_size(0) {}

    ~CpuQueue() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    bool enqueue(const Job &job) {
        if (current_size < MAX_SIZE) {
            Node* newNode = new Node(job);
            if (tail != nullptr) { // Queue is not empty
                tail->next = newNode;
            } else { // Queue is empty
                head = newNode;
            }
            tail = newNode;
            ++current_size;
            return true;
        }
        return false;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    Job dequeue() {
        if (!isEmpty()) {
            Node* temp = head;
            Job job = head->data;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
            delete temp;
            --current_size;
            return job;
        }
        throw std::runtime_error("Queue is empty!");
    }

    size_t size() const {
        return current_size;
    }

    Job peek() const {
        if (!isEmpty()) {
            return head->data;
        }
        throw std::runtime_error("Queue is empty!");
    }
};

struct DoneJob {
    int OID;
    int Departure;
    int Delay;
};
struct AbortedJob {
    int OID;
    int Abort;
    int Delay;
};

void GetCommand(int &command) {
    while (true) {
        std::cout << "\n";
        std::cout << "**** Simulate FIFO Queues by SQF *****\n";
        std::cout << "* 0. Quit                            *\n";
        std::cout << "* 1. Sort a file                     *\n";
        std::cout << "* 2. Simulate one FIFO queue         *\n";
        std::cout << "**************************************\n";
        std::cout << "Input a command(0, 1, 2): ";
        std::cin >> command;

        // If the command is valid, exit the loop
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nCommand does not exist!\n";
            continue;
        }
        if (command >= 0 && command <= 2) {
            break;
        } else {
            std::cout << "\nCommand does not exist!" << "\n";
        }
    }
}

// Ensure that the file exists
std::pair<int, std::string> promptForFilename(const std::string &fileType) {
    int fileNumber;
    std::string fileName;
    std::stringstream ss;

    while (true) {
        std::cout << "\nInput a file number: ";
        if (!(std::cin >> fileNumber)) {
            std::cerr << "Invalid input. Please enter a numeric file number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        ss.str("");
        ss.clear();
        ss << fileType << std::setfill('0') << std::setw(3) << fileNumber << ".txt";
        fileName = ss.str();

        std::ifstream fileTest(fileName);
        if (fileTest.good()) {
            fileTest.close();
            break;
        } else {
            std::cerr << "### " << fileType << fileNumber << ".txt does not exist! ###" << std::endl;
            break;
        }
    }

    return {fileNumber, fileName};
}

// Read job data
bool readJobs(const std::string &filename, std::string &header, std::vector<Job> &jobs) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        return false;
    }

    std::getline(inputFile, header);
    // Ensure that the vector is empty
    jobs.clear();

    int oid, arrival, duration, timeout;
    while (inputFile >> oid >> arrival >> duration >> timeout) {
        jobs.emplace_back(oid, arrival, duration, timeout);
    }

    inputFile.close();

    return true;
}


// Shell sort
void shellSort(std::vector<Job> &jobs) {
    int n = jobs.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Job temp = jobs[i];
            int j;
            for (j = i; j >= gap && temp < jobs[j - gap]; j -= gap) {
                jobs[j] = jobs[j - gap];
            }
            jobs[j] = temp;
        }
    }
}

// Write sorted job data to a new file
void writeSortedJobs(const std::string &filename, const std::string &header, const std::vector<Job> &jobs) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error writing file: " + filename);
    }

    outputFile << header << std::endl;
    for (const auto &job : jobs) {
        outputFile << job.getOID() << "\t" << job.getArrival() << "\t";
        outputFile << job.getDuration() << "\t" << job.getTimeOut() << std::endl;
    }

    outputFile.close();
}

void simulateSingleCpuQueue(const std::vector<Job> &sortedJobs, std::vector<DoneJob> &doneList, std::vector<AbortedJob> &abortList) {
    CpuQueue cpuQueue;
    int currentTime = 0;
    size_t index = 0; // To track the index of the job being processed

    // Loop until all jobs are processed
    while (index < sortedJobs.size() || !cpuQueue.isEmpty()) {
        // Enqueue jobs that have arrived
        while (index < sortedJobs.size() && sortedJobs[index].getArrival() <= currentTime) {
            const Job &job = sortedJobs[index++];
            if (!cpuQueue.enqueue(job)) {
                // If queue is full, job is aborted immediately
                abortList.push_back({job.getOID(), job.getArrival(), 0});
            }
        }

        // Process job from the queue
        if (!cpuQueue.isEmpty()) {
            Job job = cpuQueue.peek();
            // Check if the job has exceeded its timeout before starting
            if (currentTime >= job.getTimeOut()) {
                // Abort job with timeout at current time
                cpuQueue.dequeue(); // Remove job from queue
                abortList.push_back({job.getOID(), currentTime, currentTime - job.getArrival()});
            } else {
                // Start or continue processing the job
                currentTime = std::max(currentTime, job.getArrival());
                cpuQueue.dequeue(); // Remove job from queue to execute
                int finishTime = currentTime + job.getDuration();
                // Check for timeout during execution
                if (finishTime <= job.getTimeOut()) {
                    // Complete job normarlly
                    doneList.push_back({job.getOID(), finishTime, currentTime - job.getArrival()});
                    currentTime = finishTime; // Update current time
                } else {
                    // Abort job due to timeout during the execution
                    int timeoutDelay = job.getTimeOut() - job.getArrival();
                    abortList.push_back({job.getOID(), job.getTimeOut(), timeoutDelay});
                    currentTime = job.getTimeOut(); // Update current time to the timeout
                }
            }
        } else {
            // If queue is empty, move to the time of next arriving job
            if (index < sortedJobs.size()) {
                currentTime = sortedJobs[index].getArrival();
            }
        }
    }
}

void writeSimulationResults(const std::string &filename, const std::vector<DoneJob> &doneList, const std::vector<AbortedJob> &abortList, double averageDelay, double successRate) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error writing file: " + filename);
    }

    // Write aborted jobs
    outputFile << "	[Abort Jobs]" << std::endl;
    outputFile << "	OID	Abort	Delay" << std::endl;
    for (size_t i = 0; i < abortList.size(); ++i) {
        outputFile << "[" << (i+1) << "]	" << abortList[i].OID << "	" << abortList[i].Abort << "	" << abortList[i].Delay << std::endl;
    }

    // Write completed jobs
    outputFile << "	[Jobs Done]" << std::endl;
    outputFile << "	OID	Departure	Delay" << std::endl;
    for (size_t i = 0; i < doneList.size(); ++i) {
        outputFile << "[" << (i+1) << "]	" << doneList[i].OID << "	" << doneList[i].Departure << "	" << doneList[i].Delay << std::endl;
    }

    // Write average delay
    outputFile << "[Average Delay] " << std::setprecision(2) << std::fixed << averageDelay << " ms" << std::endl;
    // Write success rate
    outputFile << "[Success Rate]  " << std::setprecision(2) << std::fixed << successRate << " %" << std::endl;

    outputFile.close();
}

void printFileToTerminal(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();
}

double calculateAverageDelay(const std::vector<DoneJob> &doneList, const std::vector<AbortedJob> &abortList) {
    int totalDelay = 0;
    for (const auto &job : doneList) {
        totalDelay += job.Delay;
    }
    for (const auto &job : abortList) {
        totalDelay += job.Delay;
    }
    int totalJobs = doneList.size() + abortList.size();
    return totalJobs > 0 ? (double)totalDelay / totalJobs : 0.0;
}

double calculateSuccessRate(const std::vector<DoneJob> &doneList, int totalJobsCount) {
    return totalJobsCount > 0 ? 100.0 * doneList.size() / totalJobsCount : 0.0;
}

std::string Clockms(double clock){ //將clock轉為指定格式的ms 
	std::string ms;
	if(clock == 0){
		ms = "0.00";
	}
	else{
		ms = std::to_string(clock);
		while(ms.length()!=4){
			ms.pop_back();
		}
	}
	return ms;	
}
int main() {
    int command = -1;
    while (true) {
        GetCommand(command);
        if (command == 0) {
            // Quit the program
            break;
        } else if (command == 1) {
            std::string header;
            std::vector<Job> jobs;
            std::pair<int, std::string> filePromptResult = promptForFilename("input");
            int fileNumber = filePromptResult.first;
            std::string filename = filePromptResult.second;

            // Perform read operation
            double start_time = clock();
            if (!readJobs(filename, header, jobs)) {
				continue;
            }
            else{
            	double end_time = clock();
            	double reading_time = (end_time - start_time);
            	// Print sorted data to the terminal
            	std::cout << "\n	" << header << '\n';
            	int count = 1;
            	for (const auto &job : jobs) {            		
            	    std::cout<< "(" << count << ")\t" << job.getOID() << "\t" << job.getArrival() ;
            	    std::cout<< "\t" << job.getDuration() << "\t" << job.getTimeOut() << "\n";
            	    count ++;
            	}
            	// Perform sort operation
            	start_time = clock();
            	shellSort(jobs);
            	end_time = clock();
            	double sorting_time = (end_time - start_time);

            	// Write sorted data to a file
            	std::stringstream ss;
            	ss << "sorted" << std::setfill('0') << std::setw(3) << fileNumber << ".txt";
            	std::string sortedFilename = ss.str();
            	ss.str("");
            	ss.clear();
            	start_time = clock();
            	writeSortedJobs(sortedFilename, header, jobs);
            	end_time = clock();
            	double writing_time = (end_time - start_time);


            	count = 1;
            	std::cout << std::fixed << std::setprecision(2);

				std::string readms = Clockms(reading_time);
				std::string sortms = Clockms(sorting_time);
				std::string writems = Clockms(writing_time);  //將clock轉換為指定格式的ms 

				std::cout << "\nReading data: " << reading_time << " clocks (" << readms <<" ms).\n";
				std::cout << "Sorting data: " << sorting_time << " clocks (" << sortms << " ms).\n";
				std::cout << "Writing data: " << writing_time << " clocks ("<< writems << " ms).\n";
				std::cout << "\nSee sorted" << fileNumber <<".txt\n";
        	}
        } else if (command == 2) {
            // Simulate one FIFO queue
            std::pair<int, std::string> filePromptResult = promptForFilename("sorted");
            int fileNumber = filePromptResult.first;
            std::string filename = filePromptResult.second;
            std::string header;
            std::vector<Job> sortedJobs;

            if (!readJobs(filename, header, sortedJobs)) {
                continue; // Or handle the error as desired
            }
			else{
			
            	// Variables to hold simulation results
           		std::vector<DoneJob> doneList;
            	std::vector<AbortedJob> abortList;

            	// Perform the simulation
            	simulateSingleCpuQueue(sortedJobs, doneList, abortList);

            	// Calculate metrics based on simulation results
            	double averageDelay = calculateAverageDelay(doneList, abortList);
            	double successRate = calculateSuccessRate(doneList, sortedJobs.size());

            	// Creating output filename based on input file number for simulation
            	std::stringstream ss;
            	ss << "output" << std::setfill('0') << std::setw(3) << fileNumber << ".txt";
            	std::string outputFilename = ss.str();
            	ss.str("");
            	ss.clear();

            	// Write simulation results to a file
            	std::cout << "\nThe simulation is running...";
            	writeSimulationResults(outputFilename, doneList, abortList, averageDelay, successRate);

            	// Print simulation results to the terminal
            	//printFileToTerminal(outputFilename);
            	std::cout << "\nSee output" << fileNumber <<".txt\n";
        	}
        }
    }

    return 0;
}
