// 11127101 ÂÅ¦Ü«³, 11127121 ¾H«T´­ 
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;

class Queue {

    vector<string> labels;
    typedef struct queueOriginal {
        vector<int> data;
        queueOriginal *next;
    } queueOriginal;
    queueOriginal *front, *rear;
    int labelNumber;

public:
    Queue(): front(NULL), rear(NULL), labels(0), labelNumber(0) {};
    ~Queue() {
        while ( !isEmpty() ) deFront();
    };
    bool isEmpty();
    int input_Labels( string labels );
    void input_Queue( vector<int> labelData );
    vector<string> getLabel();
    vector<int> getFront();
    void deFront();
    void print_Queue( vector< vector<int> > unsortedData );

};

class DoneQueue {

    typedef struct queueDone {
        vector<int> doneData;
        queueDone *next;
    } queueDone;
    queueDone *front, *rear;

public:
    DoneQueue(): front(NULL), rear(NULL) {};
    ~DoneQueue() {
        while ( !isEmpty() ) deQueue();
    }
    void enQueue( vector<int> &doneData );
    void deQueue();
    bool isEmpty();
    vector<int> getFront();

};

class AbortQueue {

    typedef struct queueAbort {
        vector<int> abortData;
        queueAbort *next;
    } queueAbort;
    queueAbort *front, *rear;

public:
    AbortQueue(): front(NULL), rear(NULL) {};
    ~AbortQueue() {
        while ( !isEmpty() ) deQueue();
    }
    void enQueue( vector<int> &abortData );
    void deQueue();
    bool isEmpty();
    vector<int> getFront();

};

class WaitingQueue {

    typedef struct queueWaiting {
        vector<int> data;
        queueWaiting *next;
    } queueWaiting;
    queueWaiting *front, *rear;

public:
    WaitingQueue(): front(NULL), rear(NULL) {};
    ~WaitingQueue() {
        while ( !isEmpty() ) {
            deQueue();
        }
    };
    bool isEmpty();
    void enQueue( vector<int> &data );
    void deQueue();
    vector<int> getFront();

};

bool taskOne( const string& fileNumber );
void shellSort( vector< vector<int> > &unsortedData );
bool outputTaskOne( Queue &queue, const string& fileNumber );
bool singleCPUSimulation( const string& fileNumber );
bool multipleCPUSimulation( const string& fileNumber );
bool outputTaskTwo( AbortQueue &abortQueue, DoneQueue &doneQueue, const string& fileNumber );
bool outputTaskThree( AbortQueue &abortQueue, DoneQueue &doneQueue, const string& fileNumber );

int main() {

    int command = 0; // user command
    int beforeCommand = 0;
    string fileNumber, beforeFileNumber;
    do {
        string infixS;
        cout << endl << "**** Simulate FIFO Queues by SQF *****";
        cout << endl << "* 0. Quit                            *";
        cout << endl << "* 1. Sort a file                     *";
        cout << endl << "* 2. Simulate one FIFO queue         *";
        cout << endl << "* 3. Simulate two queues by SQF      *";
        cout << endl << "**************************************";
        cout << endl << "Input a command(0, 1, 2, 3): ";
        cin >> command; // get a command
        switch ( command ) {
            case 0: break;
            case 1: cout << endl << "Input a file number: ";
                cin >> fileNumber ; // get a file number
                beforeFileNumber = fileNumber;
                beforeCommand = command;
                if ( !taskOne( fileNumber ) )
                    break;
                break;
            case 2:
                if ( beforeCommand == 1 || beforeCommand == 3 ) fileNumber = beforeFileNumber;
                else if( beforeCommand != command ) {
                    cout << endl << "Input a file number: ";
                    cin >> fileNumber ; // get a file number
                }
                beforeCommand = command;
                beforeFileNumber = fileNumber;
                if ( !singleCPUSimulation( fileNumber ) )
                    break;
                break;
            case 3:
                if ( beforeCommand == 1 || beforeCommand == 2 ) fileNumber = beforeFileNumber;
                else if( beforeCommand != command ) {
                    cout << endl << "Input a file number: ";
                    cin >> fileNumber ; // get a file number
                }
                beforeCommand = command;
                beforeFileNumber = fileNumber;
                if ( !multipleCPUSimulation( fileNumber ) )
                    break;
                break;
            default: cout << endl << "Command does not exist!" << endl;
        } // end switch
    } while ( command != 0 ); // '0': stop the program
    return 0;
} // end main

bool taskOne( const string& fileNumber ) {

    clock_t startRead, endRead, startSort, endSort, startWrite, endWrite;;
    fstream file;
    string fileName = ".\\input" + fileNumber + ".txt" ; // Get the file name
    string label;
    Queue queue;
    file.open( fileName.c_str() ); // Open the file
    if ( !file.is_open() ) /* Check whether the file is exists */{
        cout << "\n### input" << fileNumber << ".txt does not exist! ###" << endl;
        return false;
    }
    getline( file, label ); // Get the labels
    if ( label == "\0") {
    	cout << "\n### Get nothing from the file input" << fileNumber << ".txt ! ###" << endl;
    	return false;
	}
    int labelNum = queue.input_Labels( label ); // Store the labels
    vector<int> oneLineData; // Each line data
    vector< vector<int> > unsortedData; // Store the data
    int data;
    startRead = clock(); // Get the start time of reading
    bool nothing = false;
    while( file >> data ) /* Get the first label's data */ {
    	nothing = true;
        oneLineData.push_back( data );
        for ( int i = 0; i < labelNum - 1; i++ ) /* Get the data of each labels */ {
            file >> data;
            oneLineData.push_back( data );
        }
        unsortedData.push_back( oneLineData ); // Store the data
        oneLineData.clear();
    }
    endRead = clock(); // Get the end time of reading
    file.close(); // Close the file
    if ( !nothing ) {
    	cout << "\n### Get nothing from the file input" << fileNumber << ".txt ! ###" << endl;
    	return false;
	}
    queue.print_Queue( unsortedData );
    startSort = clock(); // Get the start time of sorting
    shellSort( unsortedData );
    endSort = clock(); // Get the end time of sorting
    for ( int i = 0; i < unsortedData.size(); i++ )
        queue.input_Queue( unsortedData[i] ); // Store the data in the queue
    unsortedData.clear();
    startWrite = clock(); // Get the start time of writing
    bool printSuccessful = outputTaskOne( queue, fileNumber ); // Print the queue
    endWrite = clock(); // Get the end time of writing
    double durationRead = ((double)(endRead - startRead)) / CLOCKS_PER_SEC * 1000;
    cout << "Reading data: " << setprecision(0) << durationRead << " clocks " << "(" << fixed << setprecision(2) << durationRead << " ms)." << endl; // Print the time of reading
    double durationSort = ((double)(endSort - startSort)) / CLOCKS_PER_SEC * 1000;
    cout << "Sorting data: " << setprecision(0) << durationSort << " clocks " << "(" << fixed << setprecision(2) << durationSort << " ms)." << endl; // Print the time of sorting
    double durationWrite = ((double)(endWrite - startWrite)) / CLOCKS_PER_SEC * 1000;
    cout << "Writing data: " << setprecision(0) << durationWrite << " clocks " << "(" << fixed << setprecision(2) << durationWrite << " ms)." << endl; // Print the time of writing
    cout << "\n" << "See sorted" + fileNumber + ".txt" << endl;
    return printSuccessful;
}

void shellSort( vector< vector<int> > &unsortedData ) {

    // shellSort arrival times
    for( int gap = unsortedData.size()/2 ; gap > 0 ; gap = gap/2 ) {
        for( int unsort = gap; unsort < unsortedData.size(); unsort++ ) {
            vector<int> temp = unsortedData[unsort];
            int insertSort = unsort;
            while ( insertSort >= gap && (unsortedData[insertSort - gap][1] > temp[1]) ) {
                // avoid index is negative and sorted number until correct.
                unsortedData[insertSort] = unsortedData[insertSort - gap];
                insertSort = insertSort - gap;
            }
            unsortedData[insertSort] = temp;
            // Place the current element at its correct position
            temp = unsortedData[insertSort];
            while (insertSort >= gap && unsortedData[insertSort - gap][1] == temp[1] && unsortedData[insertSort-gap][0] > temp[0] ) {
                unsortedData[insertSort] = unsortedData[insertSort - gap]; 
                insertSort = insertSort - gap;
            }
			unsortedData[insertSort] = temp;
        }
    }
    
}

bool singleCPUSimulation( const string& fileNumber ) {

    vector<int> myVector;
    fstream file;
    string fileName = ".\\sorted" + fileNumber + ".txt" ; // Get the file name
    string label;
    Queue queue;
    file.open( fileName.c_str() ); // Open the file
    if ( !file.is_open() ) /* Check whether the file it exists */{
        cout << "\n### sorted" << fileNumber << ".txt does not exist! ###" << endl;
        return false;
    }
    getline( file, label ); // Get the labels
    if ( label == "\0") {
    	cout << "\n### Get nothing from the file sorted" << fileNumber << ".txt ! ###" << endl;
    	return false;
	}
    int labelNum = queue.input_Labels( label ); // Store the labels
    vector<int> oneLineData; // Each line data
    vector< vector<int> > labelData; // Store the data
    int data;
    bool nothing = false;
    while( file >> data ) /* Get the first label's data */ {
    	nothing = true;
        oneLineData.push_back( data );
        for ( int i = 0; i < labelNum - 1; i++ ) /* Get the data of each labels */ {
            file >> data;
            oneLineData.push_back( data );
        }
        queue.input_Queue( oneLineData ); // Store the data
        oneLineData.clear();
    }
    file.close(); // Close the file
    if ( !nothing ) {
    	cout << "\n### Get nothing from the file sorted" << fileNumber << ".txt ! ###" << endl;
    	return false;
	}
    DoneQueue done;
    AbortQueue abort;
    WaitingQueue waiting;
    int clock = 0, queueLength = 0, queueMax = 3;
    vector<int> tempData, doingData;
    while ( !queue.isEmpty() || !waiting.isEmpty() ) /* While the original queue is not empty or the waiting queue is not empty */ {
        if ( waiting.isEmpty() && queue.getFront()[1] >= clock  ) /* If no one is doing and the time is not up */ {
            doingData.resize( queue.getFront().size() );
            doingData = queue.getFront(); // Get the front of the original queue
            doingData.push_back( 0 ); // Set the delay time in the location 4
            waiting.enQueue( doingData ); // Storing in waiting of the first for doing
            queueLength++;
            queue.deFront(); // Remove the front
        }
        else if ( doingData.empty() && !waiting.isEmpty() ) /* If no one is doing and the waiting queue is not empty */ {
            doingData.resize( waiting.getFront().size() );
            doingData = waiting.getFront(); // Get the front of the waiting queue
        }
        while ( !queue.isEmpty() && queue.getFront()[1] < clock ) /* If the time is up */ {
            tempData.resize( queue.getFront().size() );
            tempData = queue.getFront(); // Get the front of the original queue
            queue.deFront(); // Remove the front
            if ( queueLength < queueMax ) /* If the waiting queue is not empty */ {
                queueLength++;
                tempData.push_back( clock - tempData[1] ); // Set the delay time in the location [4]
                waiting.enQueue( tempData ); // Storing in waiting, but is not the doing
            }
            // 4.1
            else /* If the waiting queue is full */ {
                tempData[2] = 0; // Set the delay time to 0
                tempData.pop_back(); // Remove the timeout [3]
                abort.enQueue( tempData ); // Storing in the abort
            }
            tempData.clear(); // Clear the tempData
        }
        if ( !doingData.empty() ) /* If the doing queue is not empty */ {
            if ( ( doingData[1] + doingData[2] + doingData[4] ) <= doingData[3] ) /* If the doing queue will be done */ {
                doingData[1] += doingData[2] + doingData[4]; // Set the abort time [1] by the duration [2] + delay time [4] and discover the original arrival time [1]
                clock = doingData[1]; // Set the clock by the abort time [1]
                doingData[2] = doingData[4]; // Set the delay time [4] to discover the duration [2]
                doingData.pop_back(); // Remove the original delay time [4]
                doingData.pop_back(); // Remove the timeout [3]
                done.enQueue( doingData ); // Store the data in the done queue
                doingData.clear(); // Clear the doing
                doingData.resize(0);
                waiting.deQueue(); // Dequeue of the first in the waiting
                queueLength--; // Decrease the queue length
                if ( !waiting.isEmpty() ) /* If the waiting queue is not empty */ {
                    doingData.resize( waiting.getFront().size() );
                    doingData = waiting.getFront(); // Get the front of the waiting
                    if ( doingData.size() == 4 ) doingData.push_back( clock - doingData[1] ); // Set the delay time in the location [4] by using right now to subtract the arrival time [1]
                    else doingData[4] = clock - doingData[1];
                }
            }
            else if ( ( doingData[1] + doingData[2] + doingData[4] ) > doingData[3] ) /* If the doing queue will timeout */{
                // 4.2
                if ( doingData[3] > clock ) /* If the time is out */ {
                    doingData[2] = doingData[3] - doingData[1]; // Set the delay time by using the timeout [3] to subtract the arrival time [1] and discover the duration [2]
                    doingData[1] = doingData[3]; // Set the abort time [1] by using the timeout [3]
                }
                // 4.3
                else /* If the time will be out */ {
                    doingData[2] = clock - doingData[1]; // Set the delay time [2] by using right now to subtract the arrival time [1]
                    doingData[1] = clock; // Set the abort time [1] by using right now to discover the arrival time [1]
                }
                clock = doingData[1]; // Set the clock by the abort time [1]
                doingData.pop_back();// Remove the original delay time [4]
                doingData.pop_back(); // Remove the timeout [3]
                abort.enQueue( doingData ); // Store the data in the abort queue
                doingData.clear(); // Clear the doing
                doingData.resize(0);
                waiting.deQueue(); // Dequeue of the first in the waiting
                queueLength--; // Decrease the queue length
                if ( !waiting.isEmpty() ) /* If the waiting queue is not empty */ {
                    doingData.resize( waiting.getFront().size() );
                    doingData = waiting.getFront(); // Get the front of the waiting
                    if ( doingData.size() == 4 ) doingData.push_back( clock - doingData[1] ); // Set the delay time in the location [4] by using right now to subtract the arrival time [1]
                    else doingData[4] = clock - doingData[1];
                }
            }
        }
    }
    cout << "\nThe simulation is running..." << endl;
    outputTaskTwo( abort, done, fileNumber ); // Output the queue
    cout << "See output" + fileNumber + ".txt" << endl;
    return true;

}

bool multipleCPUSimulation( const string& fileNumber ) {

    fstream file;
    string fileName = ".\\sorted" + fileNumber + ".txt" ; // Get the file name
    string label;
    Queue queue;
    file.open( fileName.c_str() ); // Open the file
    if ( !file.is_open() ) /* Check whether the file it exists */{
        cout << "\n### sorted" << fileNumber << ".txt does not exist! ###" << endl;
        return false;
    }
    getline( file, label ); // Get the labels
    if ( label == "\0") {
    	cout << "\n### Get nothing from the file sorted" << fileNumber << ".txt ! ###" << endl;
    	return false;
	}
    int labelNum = queue.input_Labels( label ); // Store the labels
    vector<int> oneLineData; // Each line data
    vector< vector<int> > labelData; // Store the data
    int data;
    bool nothing = false;
    while( file >> data ) /* Get the first label's data */ {
    	nothing = true;
        oneLineData.push_back( data );
        for ( int i = 0; i < labelNum - 1; i++ ) /* Get the data of each labels */ {
            file >> data;
            oneLineData.push_back( data );
        }
        queue.input_Queue( oneLineData ); // Store the data
        oneLineData.clear();
    }
    file.close(); // Close the file
    if ( !nothing ) {
    	cout << "\n### Get nothing from the file sorted" << fileNumber << ".txt ! ###" << endl;
    	return false;
	}
    int cpuNum = 2;
    DoneQueue done;
    AbortQueue abort;
    vector<WaitingQueue> waiting;
    waiting.resize( cpuNum, WaitingQueue() );
    bool wait = false, noData = false;
    int clock[2] = {0}, queueLength[2] = {0}, queueMax = 3;
    int minimumCPU = 0, minimumTime = 99999;
    vector<int> tempData;
    vector<vector<int> > doingData;
    doingData.resize( cpuNum, vector<int>() );
    do {
        int otherTime = 99999, bothNew = 0, thisTime = 99999, tempTime = 0;
        noData = false;
        for ( int i = 0 ; i < cpuNum ; i++ ) /* Check whether there is data in the waiting queue */ {
            if ( !waiting.at(i).isEmpty() ) noData = true;
        }
        for ( int i = 0; i < cpuNum && ( !queue.isEmpty() || noData ) ; i++ ) /* Get the new time data for each CPU */ {
            otherTime = 99999, thisTime = 99999;
            for ( int j = 0; j < cpuNum ; j++ ) /* Get the current time */ {
                if ( !doingData[j].empty() && i != j )  {
                    if ( doingData.at(j)[1] + doingData.at(j)[2] + doingData.at(j)[4] < doingData.at(j)[3] ) // Will not be time out
                        otherTime = doingData.at(j)[1] + doingData.at(j)[2] + doingData.at(j)[4];
                    else if ( clock[j] > doingData.at(j)[3] ) // Will be time out
                        otherTime = clock[j];
                    else
                        otherTime = doingData.at(j)[3];
                }
            }
            thisTime = clock[i];
            if ( otherTime < thisTime ) // If current time is bigger than other time set do not setting
                otherTime = 0;
            if ( clock[i] == 0 ) /* If the first time input CPU */ {
                doingData.at(i).resize( queue.getFront().size() );
                doingData.at(i) = queue.getFront(); // Get the front of the original queue
                doingData.at(i).push_back( 0 ); // Set the delay time in the location 4
                doingData.at(i).push_back( i ); // Set the CPU number
                waiting.at(i).enQueue( doingData.at(i) ); // Storing in waiting of the first for doing
                queueLength[i]++;
                queue.deFront(); // Remove the front
                clock[i] = doingData.at(i)[1];
            }
            else if ( waiting.at(i).isEmpty() && !queue.isEmpty() && ( otherTime == 99999 || otherTime >= queue.getFront()[1] ) ) /* If no one is doing and the time is not up */ {
                doingData.at(i).resize( queue.getFront().size() );
                doingData.at(i) = queue.getFront(); // Get the front of the original queue
                doingData.at(i).push_back( 0 ); // Set the delay time in the location [4]
                doingData.at(i).push_back( i ); // Set the CPU number
                waiting.at(i).enQueue( doingData.at(i) ); // Storing in waiting of the first for doing
                queueLength[i]++;
                queue.deFront(); // Remove the front
                if ( clock[i] < doingData.at(i)[1] ) // Resect the time
                    clock[i] = doingData.at(i)[1];
                else
                    doingData.at(i)[4] = clock[i] - doingData.at(i)[1];
                bothNew++;
            }
            else if ( doingData.at(i).empty() && !waiting.at(i).isEmpty() ) /* If no one is doing and the waiting queue is not empty */ {
                doingData.at(i).resize( waiting.at(i).getFront().size() );
                doingData.at(i) = waiting.at(i).getFront(); // Get the front of the waiting queue
            }
        }
        for ( int i = 0; i < cpuNum ; i++ ) /* Update the time for each CPU */ {
            if ( !doingData[i].empty() ) /* If the doing queue is not empty */ {
                clock[i] = doingData[i][1] + doingData[i][4]; // Set the new time by the arrival time [1] and the delay time [4]
                do {
                    if ( ( !queue.isEmpty() && clock[i] > queue.getFront()[1] ) || queue.isEmpty() ) break; // If the time will be out of the next get data or the queue is empty
                    else if ( ( doingData[i][1] + doingData[i][2] + doingData[i][4] ) > doingData[i][3] ) /* If the doing queue will time out */{
                        // 4.3
                        if (doingData[i][3] > clock[i]) /* If the time is out */ {
                            doingData[i][2] = doingData[i][3] - doingData[i][1]; // Set the delay time by using the timeout [3] to subtract the arrival time [1] and discover the duration [2]
                            doingData[i][1] = doingData[i][3]; // Set the abort time [1] by using the timeout [3]
                        }
                        // 4.2
                        else /* If the time will be out */ {
                            doingData[i][2] = clock[i] - doingData[i][1]; // Set the delay time [2] by using right now to subtract the arrival time [1]
                            doingData[i][1] = clock[i]; // Set the abort time [1] by using right now to discover the arrival time [1]
                        }
                        clock[i] = doingData[i][1]; // Set the clock by the abort time [1]
                        doingData[i][3] = doingData[i][2]; // Set the new delay time [3]
                        doingData[i][2] = doingData[i][1]; // Set the new abort time [2]
                        doingData[i][1] = i + 1; // Set the CID [1]
                        doingData[i].pop_back(); // Remove the temp CID [5]
                        doingData[i].pop_back(); // Remove the original delay time [4]
                        abort.enQueue(doingData[i]); // Store the data in the abort queue
                        doingData[i].clear(); // Clear the doing
                        doingData[i].resize(0);
                        waiting[i].deQueue(); // Dequeue of the first in the waiting
                        queueLength[i]--; // Decrease the queue length
                        if (!waiting[i].isEmpty()) /* If the waiting queue is not empty */ {
                            doingData[i].resize(waiting[i].getFront().size());
                            doingData[i] = waiting[i].getFront(); // Get the front of the waiting
                            if (doingData[i].size() == 4) doingData[i].push_back(clock[i] - doingData[i][1]); // Set the delay time in the location 4 by using right now to subtract the arrival time 1
                            else doingData[i][4] = clock[i] - doingData[i][1]; // Update the delay time
                        }
                    }
                    else if ((doingData[i][1] + doingData[i][2] + doingData[i][4]) <= doingData[i][3]) /* If the doing queue will be done */ {
                        doingData[i][1] += doingData[i][2] + doingData[i][4]; // Set the abort time [1] by the duration [2] + delay time [4] and discover the arrival time [1]
                        clock[i] = doingData[i][1]; // Set the clock by the abort time [1]
                        doingData[i][3] = doingData[i][4]; // Set the new delay time [3] by using the original delay time [4]
                        doingData[i][2] = doingData[i][1]; // Set the new abort time [2]
                        doingData[i][1] = i + 1; // Set the CID [1]
                        doingData[i].pop_back(); // Remove the temp CID [5]
                        doingData[i].pop_back(); // Remove the original delay time [4]
                        done.enQueue(doingData[i]); // Store the data in the done queue
                        doingData[i].clear(); // Clear the doing
                        doingData[i].resize(0);
                        waiting[i].deQueue(); // Dequeue of the first in the waiting
                        queueLength[i]--; // Decrease the queue length
                        if (!waiting[i].isEmpty()) /* If the waiting queue is not empty */ {
                            doingData[i].resize(waiting[i].getFront().size());
                            doingData[i] = waiting[i].getFront(); // Get the front of the waiting
                            if (doingData[i].size() == 4) doingData[i].push_back(clock[i] - doingData[i][1]); // Set the delay time in the location [4] by using right now to subtract the arrival time [1]
                            else doingData[i][4] = clock[i] - doingData[i][1]; // Update the delay time
                        }
                    }
                } while ( ( !queue.isEmpty() && !doingData[i].empty() && clock[i] <= queue.getFront()[1] ) /*|| ( queue.isEmpty() && !doingData[i].empty() )*/ );
            }
        }
        minimumTime = 99999;
        for ( int i = 0; i < cpuNum ; i++ ) {
            tempTime = clock[i];
            if ( tempTime < minimumTime )
                minimumTime = tempTime;
        }
        while ( !queue.isEmpty() && queue.getFront()[1] < minimumTime ) /* If the time is up */ {
            int minimumQueue = 0;
            tempData.resize( queue.getFront().size() );
            tempData = queue.getFront(); // Get the front of the original queue
            queue.deFront(); // Remove the front
            for ( int i = 0; i < cpuNum; i++ ) {
                if ( queueLength[minimumQueue] > queueLength[i] ) minimumQueue = i;
            }
            if ( queueLength[minimumQueue] < queueMax ) /* If the waiting queue is not empty */ {
                queueLength[minimumQueue]++;
                tempData.push_back( clock[minimumQueue] - tempData[1] ); // Set the delay time in the location [4]
                tempData.push_back( minimumQueue ); // Set the CPU number
                waiting[minimumQueue].enQueue( tempData ); // Storing in waiting, but is not the doing
            }
            // 4.1
            else /* If the waiting queue is full */ {
                tempData[3] = 0; // Set the delay time to 0
                tempData[2] = tempData[1];
                tempData[1] = 0; // Set the CPU number to 0
                //tempData.pop_back(); // Remove the timeout [3]
                abort.enQueue( tempData ); // Storing in the abort
            }
            tempData.clear(); // Clear the tempData
        }
        minimumTime = 99999;
        if ( queue.isEmpty() ) /* If the queue is empty for the data still in the waiting queue */{
            minimumCPU = 0;
            for ( int i = 0; i < cpuNum ; i++ ) /* Find the minimum clock of the CPU */ {
                tempTime = clock[i];
                if ( tempTime < minimumTime && !doingData[i].empty() ) {
                    minimumCPU = i;
                    minimumTime = tempTime;
                }
            }
            if ( !doingData[minimumCPU].empty() && (doingData[minimumCPU][1] + doingData[minimumCPU][2] + doingData[minimumCPU][4]) > doingData[minimumCPU][3] ) /* If the doing queue will time out */{
                // 4.3
                if (doingData[minimumCPU][3] > clock[minimumCPU]) /* If the time is out */ {
                    doingData[minimumCPU][2] = doingData[minimumCPU][3] - doingData[minimumCPU][1]; // Set the delay time by using the timeout [3] to subtract the arrival time [1] and discover the duration [2]
                    doingData[minimumCPU][1] = doingData[minimumCPU][3]; // Set the abort time [1] by using the timeout [3]
                }
                    // 4.2
                else /* If the time will be out */ {
                    doingData[minimumCPU][2] = clock[minimumCPU] - doingData[minimumCPU][1]; // Set the delay time [2] by using right now to subtract the arrival time [1]
                    doingData[minimumCPU][1] = clock[minimumCPU]; // Set the abort time [1] by using right now to discover the arrival time [1]
                }
                clock[minimumCPU] = doingData[minimumCPU][1]; // Set the clock by the abort time [1]
                doingData[minimumCPU][3] = doingData[minimumCPU][2]; // Set the new delay time [3]
                doingData[minimumCPU][2] = doingData[minimumCPU][1]; // Set the new abort time [2]
                doingData[minimumCPU][1] = minimumCPU + 1; // Set the CID [1]
                doingData[minimumCPU].pop_back(); // Remove the original CID [5]
                doingData[minimumCPU].pop_back(); // Remove the original delay time [4]
                abort.enQueue(doingData[minimumCPU]); // Store the data in the abort queue
                doingData[minimumCPU].clear(); // Clear the doing
                doingData[minimumCPU].resize(0);
                waiting[minimumCPU].deQueue(); // Dequeue of the first in the waiting
                queueLength[minimumCPU]--; // Decrease the queue length
                if (!waiting[minimumCPU].isEmpty()) /* If the waiting queue is not empty */ {
                    doingData[minimumCPU].resize(waiting[minimumCPU].getFront().size());
                    doingData[minimumCPU] = waiting[minimumCPU].getFront(); // Get the front of the waiting
                    if (doingData[minimumCPU].size() == 4) doingData[minimumCPU].push_back(clock[minimumCPU] - doingData[minimumCPU][1]); // Set the delay time in the location 4 by using right now to subtract the arrival time 1
                    else doingData[minimumCPU][4] = clock[minimumCPU] - doingData[minimumCPU][1]; // Update the delay time
                }
            }
            else if ( !doingData[minimumCPU].empty() && (doingData[minimumCPU][1] + doingData[minimumCPU][2] + doingData[minimumCPU][4]) <= doingData[minimumCPU][3]) /* If the doing queue will be done */ {
                doingData[minimumCPU][1] += doingData[minimumCPU][2] + doingData[minimumCPU][4]; // Set the abort time [1] by the duration [2] + delay time [4] and discover the arrival time [1]
                clock[minimumCPU] = doingData[minimumCPU][1]; // Set the clock by the abort time [1]
                doingData[minimumCPU][3] = doingData[minimumCPU][4]; // Set the new delay time [3] by using the original delay time [4]
                doingData[minimumCPU][2] = doingData[minimumCPU][1]; // Set the new abort time [2]
                doingData[minimumCPU][1] = minimumCPU + 1; // Set the CID [1]
                doingData[minimumCPU].pop_back(); // Remove the original CID [5]
                doingData[minimumCPU].pop_back(); // Remove the original delay time [4]
                done.enQueue(doingData[minimumCPU]); // Store the data in the done queue
                doingData[minimumCPU].clear(); // Clear the doing
                doingData[minimumCPU].resize(0);
                waiting[minimumCPU].deQueue(); // Dequeue of the first in the waiting
                queueLength[minimumCPU]--; // Decrease the queue length
                if (!waiting[minimumCPU].isEmpty()) /* If the waiting queue is not empty */ {
                    doingData[minimumCPU].resize(waiting[minimumCPU].getFront().size());
                    doingData[minimumCPU] = waiting[minimumCPU].getFront(); // Get the front of the waiting
                    if (doingData[minimumCPU].size() == 4) doingData[minimumCPU].push_back(clock[minimumCPU] - doingData[minimumCPU][1]); // Set the delay time in the location [4] by using right now to subtract the arrival time [1]
                    else doingData[minimumCPU][4] = clock[minimumCPU] - doingData[minimumCPU][1]; // Update the delay time
                }
            }
        }
        wait = false;
        for ( int i = 0; i < cpuNum; i++ ) /* Check whether the waiting queue is not empty */ {
            if ( !waiting.at(i).isEmpty() ) {
                wait = true;
            }
        }
    } while ( !queue.isEmpty() || wait ); /* While the original queue is not empty or the waiting queue is not empty */
    cout << "\nThe simulation is running..." << endl;
    outputTaskThree( abort, done, fileNumber ); // Output the queue
    cout << "See double" + fileNumber + ".txt" << endl;
    return true;

}

bool Queue::isEmpty() /* Check whether the original queue is empty */ {
    return front == NULL; // If the queue is empty, return true
}

int Queue::input_Labels( string label ) /* Store the labels */ {

    int labelNum = 0;
    string temp;
    for (int i = 0; i < label.size(); i++) /* Get each labels */ {
        if ( label[i] != '\t' ) // If the character is not a tab
            temp += label[i];
        else {
            labels.push_back( temp ); // Store the labels
            temp = "";
            labelNum++; // Store the number of labels
        }
    }
    labels.push_back( temp );
    labelNum++;
    labelNumber = labelNum; // Store the number of labels
    return labelNumber; // Return the number of labels

}

void Queue::input_Queue( vector<int> labelData ) /* Store the data of the original queue */ {

    queueOriginal *newNode = new queueOriginal;
    for ( int i = 0; i < labelNumber; i++ ) // Store the data
        newNode->data.push_back( labelData[i] );
    if ( isEmpty() ) /* If the queue is empty */ {
        front = newNode;
        rear = newNode;
        rear->next = NULL;
    }
    else /* If the queue is not empty */ {
        rear->next = newNode;
        rear = newNode;
        rear->next = NULL;
    }

}

vector<string> Queue::getLabel() /* Get the label of the original queue */ {
    return labels;
}

vector<int> Queue::getFront() /* Get the front data of original queue */ {
    return front->data;
}

void Queue::deFront() /* Delete the front data of original queue */ {

    if ( isEmpty() ) /* If the queue is empty */ {
        cout << "The queue is empty!" << endl;
    }
    else if ( front == rear ) /* If the queue has only one data */ {
        delete front;
        front = NULL;
        rear = NULL;
    }
    else /* If the queue has more than one data */ {
        queueOriginal *tempNode = front;
        front = front->next;
        delete tempNode;
    }
}

void Queue::print_Queue( vector< vector<int> > unsortedData ) /* Print the unsorted queue */ {

    cout << "\n\t";
    for ( int i = 0; i < labelNumber; i++ ) // Print all the labels
        cout << labels[i] << "\t";
    cout << endl;
    int Num = 1;
    for ( int i = 0 ; i < unsortedData.size() ; i++ ) /* Print all the data */ {
        cout << "(" << Num++ << ")\t";
        for ( int j = 0; j < labelNumber; j++ ) // Print the data
            cout << unsortedData[i][j] << "\t";
        cout << endl;
    }
    cout << endl;

}

bool outputTaskOne( Queue &queue, const string& fileNumber ) /* Output the queue of task one */ {

    ofstream outputFile;
    string fileName = ".\\sorted" + fileNumber + ".txt" ; // Get the output file name
    outputFile.open( fileName.c_str() ); // Open the file
    if (!outputFile.is_open()) {
        cout << "Error opening the file for writing." << endl;
        return false;
    }
    for ( int i = 0; i < queue.getLabel().size(); i++ ) /* Print all the labels */ {
        outputFile << queue.getLabel()[i]; // Store the labels
        if ( i != queue.getLabel().size() - 1 ) // If it is not the last label
            outputFile << "\t";
    }
    outputFile << endl;
    int Num = 1;
    while ( !queue.isEmpty() ) /* Print all the data */ {
        for ( int i = 0; i < queue.getFront().size(); i++ ) /* Print the data */ {
            outputFile << queue.getFront()[i]; // Store the data
            if ( i != queue.getFront().size() - 1 ) // If it is not the last data
                outputFile << "\t";
        }
        outputFile << endl;
        queue.deFront(); // Delete the front data to get the next data
    }
    outputFile.close(); // Close the file
    return true;

}

bool outputTaskTwo( AbortQueue &abortQueue, DoneQueue &doneQueue, const string& fileNumber ) /* Output the queue of the task two */ {

    ofstream outputFile;
    string fileName = ".\\output" + fileNumber + ".txt" ; // Get the output file name
    outputFile.open( fileName.c_str() ); // Open the file
    if (!outputFile.is_open())
        cout << "Error opening the file for writing." << endl; // If the file is not open
    int totalDelay = 0, totalData = 0, totalSuccess = 0;
    // output the abort queue
    outputFile << "\t" << "[Abort Jobs]" << endl;
    outputFile << "\tOID\tAbort\tDelay" << endl;
    int num = 1;
    while ( !abortQueue.isEmpty() ) /* Print all the data */ {
        outputFile << "[" << num++ << "]\t";
        for ( int i = 0; i < abortQueue.getFront().size(); i++ ) /* Print the data */ {
            outputFile << abortQueue.getFront()[i];
            if ( i != abortQueue.getFront().size() - 1 ) // If it is not the last data
                outputFile << "\t";
            else
                totalDelay += abortQueue.getFront()[i];
        }
        totalData++;
        outputFile << endl;
        abortQueue.deQueue(); // Delete the data to get the next data
    }
    // output the done queue
    outputFile << "\t[Jobs Done]" << endl;
    outputFile << "\tOID\tDeparture\tDelay" << endl;
    num = 1;
    while ( !doneQueue.isEmpty() ) /* Print all the data */ {
        outputFile << "[" << num++ << "]\t";
        for ( int i = 0; i < doneQueue.getFront().size(); i++ ) /* Print the data */ {
            outputFile << doneQueue.getFront()[i];
            if ( i != doneQueue.getFront().size() - 1 ) // If it is not the last data
                outputFile << "\t";
            else
                totalDelay += doneQueue.getFront()[i];
        }
        totalData++;
        totalSuccess++;
        outputFile << endl;
        doneQueue.deQueue(); // Delete the data to get the next data
    }
    // output the statistics
    outputFile << "[Average Delay]\t" << fixed << setprecision(2) << (float)totalDelay / (float)totalData << " ms" << endl;
    outputFile << "[Success Rate]\t" << fixed << setprecision(2) << (float)totalSuccess / (float)totalData * 100 << " %" << endl;
    outputFile.close(); // Close the file

}

bool outputTaskThree( AbortQueue &abortQueue, DoneQueue &doneQueue, const string& fileNumber ) /* Output the queue of the task three */ {

    ofstream outputFile;
    string fileName = ".\\double" + fileNumber + ".txt" ; // Get the output file name
    outputFile.open( fileName.c_str() ); // Open the file
    if (!outputFile.is_open())
        cout << "Error opening the file for writing." << endl; // If the file is not open
    int totalDelay = 0, totalData = 0, totalSuccess = 0;
    // output the abort queue
    outputFile << "\t" << "[Abort Jobs]" << endl;
    outputFile << "\tOID\tCID\tAbort\tDelay" << endl;
    int num = 1;
    while ( !abortQueue.isEmpty() ) /* Print all the data */ {
        outputFile << "[" << num++ << "]\t";
        for ( int i = 0; i < abortQueue.getFront().size(); i++ ) /* Print the data */ {
            outputFile << abortQueue.getFront()[i];
            if ( i != abortQueue.getFront().size() - 1 ) // If it is not the last data
                outputFile << "\t";
            else
                totalDelay += abortQueue.getFront()[i];
        }
        totalData++;
        outputFile << endl;
        abortQueue.deQueue(); // Delete the data to get the next data
    }
    // output the done queue
    outputFile << "\t[Jobs Done]" << endl;
    outputFile << "\tOID\tCID\tDeparture\tDelay" << endl;
    num = 1;
    while ( !doneQueue.isEmpty() ) /* Print all the data */ {
        outputFile << "[" << num++ << "]\t";
        for ( int i = 0; i < doneQueue.getFront().size(); i++ ) /* Print the data */ {
            outputFile << doneQueue.getFront()[i];
            if ( i != doneQueue.getFront().size() - 1 ) // If it is not the last data
                outputFile << "\t";
            else
                totalDelay += doneQueue.getFront()[i];
        }
        totalData++;
        totalSuccess++;
        outputFile << endl;
        doneQueue.deQueue(); // Delete the data to get the next data
    }
    // output the statistics
    outputFile << "[Average Delay]\t" << fixed << setprecision(2) << (float)totalDelay / (float)totalData << " ms" << endl;
    outputFile << "[Success Rate]\t" << fixed << setprecision(2) << (float)totalSuccess / (float)totalData * 100 << " %" << endl;
    outputFile.close(); // Close the file

}

bool AbortQueue::isEmpty() /* Check whether the aborting queue is empty */ {
    return front == NULL; // If the queue is empty, return true
}

void AbortQueue::enQueue( vector<int> &abortData ) /* Store the data of aborting */ {

    queueAbort *newNode = new queueAbort;
    newNode->abortData.clear();
    for ( int i = 0; i < abortData.size(); i++ ) // Store the data
        newNode->abortData.push_back( abortData[i] );
    if ( isEmpty() ) /* If the queue is empty */ {
        front = newNode;
        rear = newNode;
        rear->next = NULL;
    }
    else /* If the queue is not empty */ {
        rear->next = newNode;
        rear = newNode;
        rear->next = NULL;
    }
}

void AbortQueue::deQueue() /* Delete the front data of aborting */ {

    if ( isEmpty() ) /* If the queue is empty */ {
        cout << "The queue is empty!" << endl;
    }
    else if ( front == rear ) /* If the queue has only one data */ {
        delete front;
        front = NULL;
        rear = NULL;
    }
    else /* If the queue has more than one data */ {
        queueAbort *tempNode = front;
        front = front->next;
        delete tempNode;
    }
}

vector<int> AbortQueue::getFront() /* Get the front data of aborting */ {
    return front->abortData;
}

bool DoneQueue::isEmpty() /* Check whether the done queue is empty */ {
    return front == NULL;
}

void DoneQueue::enQueue( vector<int> &doneData ) /* Store the data of done */ {

    queueDone *newNode = new queueDone;
    for ( int i = 0; i < doneData.size(); i++ ) // Store the data
        newNode->doneData.push_back( doneData[i] );
    if ( isEmpty() ) /* If the queue is empty */ {
        front = newNode;
        rear = newNode;
        rear->next = NULL;
    }
    else /* If the queue is not empty */ {
        rear->next = newNode;
        rear = newNode;
        rear->next = NULL;
    }
}

void DoneQueue::deQueue() /* Delete the front data of done */ {

    if ( isEmpty() ) /* If the queue is empty */ {
        cout << "The queue is empty!" << endl;
    }
    else if ( front == rear ) /* If the queue has only one data */ {
        delete front;
        front = NULL;
        rear = NULL;
    }
    else /* If the queue has more than one data */ {
        queueDone *tempNode = front;
        front = front->next;
        delete tempNode;
    }
}

vector<int> DoneQueue::getFront() /* Get the front data of done */ {
    return front->doneData;
}

bool WaitingQueue::isEmpty() /* Check whether the waitingQueue is empty */ {
    return front == NULL; // If the queue is empty, return true
}

void WaitingQueue::enQueue( vector<int> &doneData ) /* Store the data of waiting */ {

    queueWaiting *newNode = new queueWaiting;
    for ( int i = 0; i < doneData.size(); i++ ) // Store the data of waiting
        newNode->data.push_back( doneData[i] );
    if ( isEmpty() ) /* If the queue is empty */ {
        front = newNode;
        rear = newNode;
        rear->next = NULL;
    }
    else /* If the queue is not empty */ {
        rear->next = newNode;
        rear = newNode;
        rear->next = NULL;
    }
}

void WaitingQueue::deQueue() /* Delete the front data of waiting */ {

    if ( isEmpty() ) /* If the queue is empty */ {
        cout << "The queue is empty!" << endl;
    }
    else if ( front == rear ) /* If the queue has only one data */ {
        delete front;
        front = NULL;
        rear = NULL;
    }
    else /* If the queue has more than one data */ {
        queueWaiting *tempNode = front;
        front = front->next;
        delete tempNode;
    }
}

vector<int> WaitingQueue::getFront() /* Get the front data of waiting */ {
    return front->data;
}
