// 11127213 凌鼎軒 11127245 陳岳崇 
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <vector>
# include <fstream>
# include <ctime>
# include <iomanip>

using namespace std;

class Node {

    private:

        int OID,arrival, duration, timeOut;

    public:

        Node() {
            this -> OID = 0;
            this -> arrival = 0;
            this -> duration =0;
            this -> timeOut =0;
        }

        Node(int OID, int arrival, int duration, int timeOut ) {
            this -> OID = OID;
            this -> arrival = arrival;
            this -> duration = duration;
            this -> timeOut = timeOut;
        }

        int getOID() {return this -> OID;}
        int getArrival() {return this -> arrival;}
        int getDuration() {return this -> duration;}
        int getTimeOut() {return this -> timeOut;}

        void setOID( int inVal ) {this -> OID = inVal;}
        void setArrival( int inVal ) {this -> arrival = inVal;}
        void setDuration( int inVal ) {this -> duration = inVal;}
        void setTimeOut( int inVal ) {this -> timeOut = inVal;}

};

class NodeSorter {

public:

    void shellSort( vector<Node>& inputData, double& timeSpent )  {

        int dataLength = inputData.size();

        clock_t start_t, end_t;

        start_t = clock();

        for ( int h = dataLength / 2 ; h > 0 ; h = h / 2 )  {

            for ( int unsorted = h ; unsorted < dataLength ; ++unsorted )  {

                int loc = unsorted;
                Node temp;
                temp = inputData[unsorted];            // save the inserted key

                while ( loc >= h )  {

                    if ( inputData[loc - h].getArrival() > temp.getArrival() )    // if the previous larger than current
                        inputData[loc] = inputData[loc - h];   // copy the previous to the current location

                    else if ( inputData[loc - h].getArrival() == temp.getArrival() )  {
                        if ( inputData[loc - h].getOID() > temp.getOID() )
                            inputData[loc] = inputData[loc - h];

                        else break;

                    }

                    else break;

                    loc = loc - h;      // shell sort when h != 1, insertion sort when h = 1

                }

                inputData[loc] = temp;  // insert to the correct position

            }

        }


        end_t = clock();

    }

};

class Queue {

    private:

        typedef struct QueueNode {

            Node node;
            struct QueueNode* next;

        } QueueNode;

        int nodeNum;
        int maxNode;
        QueueNode* frontPtr;
        QueueNode* backPtr;

    public:

        Queue()  {

			this -> nodeNum = 0;
			this -> maxNode = 3;
            frontPtr = NULL;
            backPtr = NULL;

        }

        bool IsEmpty()  {

            return ( frontPtr == NULL );

        }

        bool enqueue( Node inNode )  {

            if ( nodeNum == maxNode )  return false;

            if ( !frontPtr )  {

                backPtr = ( QueueNode* )calloc( 1, sizeof( QueueNode ) );
                frontPtr = backPtr;
                backPtr -> node = inNode;
                backPtr -> next = NULL;

            }

            else {

                QueueNode* newPtr = ( QueueNode* )calloc( 1, sizeof( QueueNode ) );
                newPtr -> node = inNode;
                newPtr -> next = NULL;
                backPtr -> next = newPtr;
                backPtr = backPtr -> next;


            }

            nodeNum = nodeNum + 1;
            return true;

        }

        void dequeue()  {

            if ( IsEmpty() )
                cout << "The queue is empty!\n";

            else {

                QueueNode* temp = frontPtr;
                frontPtr = frontPtr -> next;
                free( temp );
                nodeNum = nodeNum - 1;

            }

        }

        void dequeue( Node& outNode )  {

            if ( IsEmpty() )
                cout << "The queue is empty!\n";

            else {

                QueueNode* temp = frontPtr;
                frontPtr = frontPtr -> next;
                outNode = temp -> node;
                free( temp );
                nodeNum = nodeNum - 1;

            }


        }

        int getNodeNum() { return this -> nodeNum; }

        void getFront( Node& outNode )  {

            if ( IsEmpty() )  cout << "The queue is empty!\n";
            else
                outNode = frontPtr -> node;

        }

        ~Queue()  {

            while ( frontPtr )
                dequeue();

        }

};

class AbortedTask  {

private:
    int OID, abortedTime, delay;

public:
    AbortedTask() {

        this -> OID = 0;
        this -> abortedTime = 0;
        this -> delay = 0;

    }

    AbortedTask( int OID, int abortedTime, int delay ) {

        this -> OID = OID;
        this -> abortedTime = abortedTime;
        this -> delay = 0;

    }

    int getOID() {return this -> OID;}
    int getAbortedTime() {return this -> abortedTime;}
    int getDelay() {return this -> delay;}

    void setOID( int OID ) {this -> OID = OID;}
    void setAbortedTime( int abortedTime ) {this -> abortedTime = abortedTime;}
    void setDelay( int delay ) {this -> delay = delay;}

};

class DoneTask {

private:
    int OID, departure, delay;

public:

    DoneTask() {

        this -> OID = 0;
        this -> departure = 0;
        this -> delay = 0;

    }

    DoneTask( int OID, int departure, int delay ) {

        this -> OID = OID;
        this -> departure = departure;
        this -> delay = delay;

    }

    int getOID() { return this -> OID; }
    int getDeparture() { return this -> departure; }
    int getDelay() { return this -> delay; }

    void setOID( int inVal )  { this -> OID = inVal; }
    void setDeparture( int inVal )  { this -> departure = inVal; }
    void setDelay( int inVal )  { this -> delay = inVal; }

};

int SQF( vector<Queue>& jobQueue )  {

    int length = jobQueue.size();
    int leastJobCount = 3;
    int cpuIndex = -1;

    for ( int i = 0 ; i < length ; ++i )  {

        if ( jobQueue[i].getNodeNum() < leastJobCount )
            cpuIndex = i;

    }

    return cpuIndex;

}

class Simulation {

private:

    Queue jobQueue;
    vector<Node> eventList;     // sorted data copied to eventList
    vector<AbortedTask> abortedTask;
    vector<DoneTask> doneTask;
    int timePoint;    // event-driven

public:
	vector<AbortedTask> getAbtList(){
		return abortedTask;
	}

    vector<DoneTask> getDoneList(){
    	return doneTask;
	}

    void printAbtTask()  {

        int abt_length = ( this -> abortedTask ).size();
        cout << "Aborted Task:\n";
        for ( int i = 0 ; i < abt_length ; ++i )
            cout << abortedTask[i].getOID() << '\t' << abortedTask[i].getAbortedTime()
                 << '\t' << abortedTask[i].getDelay() << '\n';

    }

    void printDoneTask()  {

        int done_length = ( this -> doneTask ).size();
        cout << "Done Task:\n";
        for ( int i = 0 ; i < done_length ; ++i )
            cout << doneTask[i].getOID() << '\t' << doneTask[i].getDeparture()
                 << '\t' << doneTask[i].getDelay() << '\n';

    }

    Simulation( vector<Node> sortedData ) {

        Node temp = Node();
        AbortedTask abt_temp;
        DoneTask done_temp;
        timePoint = 0;
        bool enqueueSuccess = true;

        int length = sortedData.size();
        for ( int i = 0 ; i < length ; ++i )  {   // copy sortedData to eventList

            temp = sortedData[i];
            eventList.push_back( temp );

        }

        while ( !eventList.empty() )  { // 還有事件要處理 

            if ( jobQueue.IsEmpty() && !eventList.empty() ) { // 沒人在排隊 & 還有事件 

                if ( timePoint <= eventList[0].getArrival() )  
                    timePoint = eventList[0].getArrival() + eventList[0].getDuration();

                else  
                    timePoint += eventList[0].getDuration();

                done_temp.setOID( eventList[0].getOID() );
                done_temp.setDeparture( timePoint );
                done_temp.setDelay( 0 );
                eventList.erase( eventList.begin() );
                doneTask.push_back( done_temp );

            }

            int i = 0;
            while ( !eventList.empty() && eventList[i].getArrival() <= timePoint )  { // 還有事件 & 比 timepoint 早或同時到 

                if ( eventList[i].getArrival() == timePoint )  { // 同時到 
                    if ( jobQueue.getNodeNum() == 3 ) { // 滿了 

                        Node gNode;
                        while ( !jobQueue.IsEmpty() ) { // 處理排隊 

                            jobQueue.dequeue( gNode );
                            if ( timePoint > gNode.getTimeOut() ) { // 離開了 

                                abt_temp.setOID( gNode.getOID() );
                                abt_temp.setAbortedTime( timePoint );
                                abt_temp.setDelay( timePoint - gNode.getArrival() );
                                abortedTask.push_back( abt_temp );

                            }

                            else if ( ( timePoint + gNode.getDuration() ) > gNode.getTimeOut() )  { // 中途離開 

                                abt_temp.setOID( gNode.getOID() );
                                abt_temp.setAbortedTime( gNode.getTimeOut() );
                                abt_temp.setDelay( gNode.getTimeOut() - gNode.getArrival() );
                                abortedTask.push_back( abt_temp );
                                timePoint = gNode.getTimeOut(); // 更新 timepoint 
                                break;

                            }

                            else  { // 完成處理 

                                done_temp.setOID( gNode.getOID() );
                                done_temp.setDelay( timePoint - gNode.getArrival() );
                                done_temp.setDeparture( timePoint + gNode.getDuration() );
                                doneTask.push_back( done_temp );
                                timePoint += gNode.getDuration(); // 更新 timepoint 
                                break;

                            }

                        }

                    }

                    else { // 還沒滿 

                        enqueueSuccess = jobQueue.enqueue( eventList[i] ); // 是否可排 

                        if ( !enqueueSuccess ) { // 不可排(滿了) 

                            abt_temp.setOID( eventList[i].getOID() );
                            abt_temp.setDelay( 0 );
                            abt_temp.setAbortedTime( eventList[i].getArrival() );
                            abortedTask.push_back( abt_temp );

                        }

                        eventList.erase( eventList.begin() );
                    }

                }

                else { // 比 timepoint 早到 

                    enqueueSuccess = jobQueue.enqueue( eventList[i] ); // 是否可排 

                    if ( !enqueueSuccess ) { // 不可排(滿了)  

                        abt_temp.setOID( eventList[i].getOID() );
                        abt_temp.setDelay( 0 );
                        abt_temp.setAbortedTime( eventList[i].getArrival() );
                        abortedTask.push_back( abt_temp );

                    }

                    eventList.erase( eventList.begin() );

                }

            }

            while ( !jobQueue.IsEmpty() )  { // 完成處理，處理下一位(先從排隊的人處理) 

                jobQueue.dequeue( temp );

                if ( timePoint > temp.getTimeOut() ) {

                    abt_temp.setOID( temp.getOID() );
                    abt_temp.setAbortedTime( timePoint );
                    abt_temp.setDelay( timePoint - temp.getArrival() );
                    abortedTask.push_back( abt_temp );

                }

                else if ( ( timePoint + temp.getDuration() ) > temp.getTimeOut() )  {

                    abt_temp.setOID( temp.getOID() );
                    abt_temp.setAbortedTime( temp.getTimeOut() );
                    abt_temp.setDelay( temp.getTimeOut() - temp.getArrival() );
                    abortedTask.push_back( abt_temp );
                    timePoint = temp.getTimeOut();
                    break;

                }

                else  {

                    done_temp.setOID( temp.getOID() );
                    done_temp.setDelay( timePoint - temp.getArrival() );
                    done_temp.setDeparture( timePoint + temp.getDuration() );
                    doneTask.push_back( done_temp );
                    timePoint += temp.getDuration();
                    break;

                }

            }

        }

        while ( !jobQueue.IsEmpty() )  { // 處理完事件了 但還有人排隊

            jobQueue.dequeue( temp );

            if ( timePoint > temp.getTimeOut() ) { // 目前時間 > 吃進來的離開時間 
            									   
                abt_temp.setOID( temp.getOID() );
                abt_temp.setAbortedTime( timePoint );
                abt_temp.setDelay( timePoint - temp.getArrival() );
                abortedTask.push_back( abt_temp );

            }

            else if ( ( timePoint + temp.getDuration() ) > temp.getTimeOut() )  { // 目前時間 + 過程 > 離開時間 

                abt_temp.setOID( temp.getOID() );
                abt_temp.setAbortedTime( temp.getTimeOut() );
                abt_temp.setDelay( temp.getTimeOut() - temp.getArrival() );
                abortedTask.push_back( abt_temp );
                timePoint = temp.getTimeOut(); // 更新 目前時間 

            }

            else  { // 可完成處理 

                done_temp.setOID( temp.getOID() );
                done_temp.setDelay( timePoint - temp.getArrival() );
                done_temp.setDeparture( timePoint + temp.getDuration() );
                doneTask.push_back( done_temp );
                timePoint += temp.getDuration(); // 更新 目前時間 

            }

        }

    }

};

class File {

private:

    string inputNum;
    string soFileName;
    string wrFileName;

public:

    File(){
		this -> inputNum = "";
		this -> soFileName = "";
		this -> wrFileName = "";
	}

    void getSoFileName( string &name ) {
    	name = soFileName;
	}

	string getWrongName() {
		return wrFileName;
	}

    bool readInFile( vector<Node>& inputData, double& measureTime )  {

        string firstLine;
        cin >> inputNum;
        string fileName = "input" + inputNum + ".txt";
        ifstream inFile(fileName.c_str());

        if ( !inFile.is_open() )  {
        	cout << "\n";
            cout << "### " << fileName << " does not exist! ###\n";
            wrFileName = "sorted" + inputNum + ".txt";
            this -> inputNum = "";
            return false;
        }

        int oid, arrival, duration, timeOut;
        clock_t start_t, end_t;

        start_t = clock();

        getline( inFile, firstLine );   // skip the first line

        while( inFile >> oid ) {

            inFile >> arrival;
            inFile >> duration;
            inFile >> timeOut;

            Node temp = Node( oid, arrival, duration, timeOut );
            inputData.push_back(temp);

        }

        end_t = clock();
        measureTime = ( end_t - start_t );

        inFile.close();
        return true;

    }

    void writeSortedFile( vector<Node>& sortedData, double& measureTime )  {

        if ( !( this -> inputNum ).compare( "" ) )  return;

        string fileName = "sorted" + inputNum + ".txt";
        ofstream outputFile( fileName.c_str() );

        clock_t start_t, end_t;

        start_t = clock();

        outputFile << "OID	Arrival	Duration	TimeOut\n";

        int length = sortedData.size();
        for ( int i = 0 ; i < length ; ++i )
            outputFile << sortedData[i].getOID() << '\t' << sortedData[i].getArrival() << '\t'
                       << sortedData[i].getDuration() << '\t' << sortedData[i].getTimeOut() << "\n";

        outputFile.close();

        end_t = clock();
        measureTime = ( end_t - start_t );
        soFileName = fileName;

    }

    bool readSortedFile( vector<Node>& inputData, string sortedName )  {


        if ( sortedName == "" ){
        	cin >> inputNum;
        	sortedName = "sorted" + inputNum + ".txt";
        	soFileName = sortedName;
		}
		ifstream soFile(sortedName.c_str());

        if ( !soFile.is_open() )  {
        	cout << "\n";
            cout << "### " << sortedName << " does not exist! ###\n";
            this -> inputNum = "";
            return false;
        }

        int oid, arrival, duration, timeOut;
		string firstLine;

        getline( soFile, firstLine );   // skip the first line

        while( soFile >> oid ) {

            soFile >> arrival;
            soFile >> duration;
            soFile >> timeOut;

            Node temp = Node( oid, arrival, duration, timeOut );
            inputData.push_back(temp);

        }

        soFile.close();
        return true;

    }

    string writeOutputFile( vector<AbortedTask> abortedTask, vector<DoneTask> doneTask, string onlyNum ) {

		
		string fileName = "output" + onlyNum + ".txt";
		ofstream outputFile( fileName.c_str() );

		outputFile << "\t[Abort Jobs]\n";
		outputFile << "	OID	Abort	Delay\n";

		double delay = 0;
		double total = 0;

        int abt_length = abortedTask.size();
        for ( int i = 0 ; i < abt_length ; ++i ) {
        	outputFile << "[" << i+1 << "]" << '\t' << abortedTask[i].getOID() << '\t' << abortedTask[i].getAbortedTime()
                 << '\t' << abortedTask[i].getDelay() << '\n';
            delay = delay + (double)abortedTask[i].getDelay();
		}


		outputFile << "\t[Jobs Done]\n";
		outputFile << "	OID	Departure	Delay\n";

        int done_length = doneTask.size();
        for ( int i = 0 ; i < done_length ; ++i ) {
        	outputFile << "[" << i+1 << "]" << '\t' << doneTask[i].getOID() << '\t' << doneTask[i].getDeparture()
                 << '\t' << doneTask[i].getDelay() << '\n';
        	delay = delay + (double)doneTask[i].getDelay();
		}

        total = abt_length + done_length;
        outputFile << "[Average Delay]	" << setprecision(2) << fixed <<  delay / total << " ms\n";
		outputFile << "[Success Rate]	" << setprecision(2) << fixed <<  done_length / total * 100 << " %\n";

		outputFile.close();
		return fileName;
	}
};




class Print {

public:

    Print() {}

    void printUnsortedData( vector<Node>& unsortedData )  {

        int length = unsortedData.size();

		cout << "\n";
        cout << "        OID     Arrival Duration        TimeOut\n";
        for ( int i = 0 ; i < length ; ++i )
            cout << '(' << i + 1 << ')' <<'\t' << unsortedData[i].getOID() << '\t' << unsortedData[i].getArrival() << '\t'
                 << unsortedData[i].getDuration() << '\t' << unsortedData[i].getTimeOut() << "\n";

    }

};



int main()  {

	int command = 0;
	double measureTime, readTime, sortTime, writeTime;
	string inputCommand = "";
	string soFileName = "";
	string wrongName = "";
	do
	{
		vector<Node> nodes;
	    File fileProcessor = File();
	    Print printer = Print();

		cout<<endl<<"**** Simulate FIFO Queues by SQF *****";
		cout<<endl<<"* 0. Quit                            *";
		cout<<endl<<"* 1. Sort a file                     *";
		cout<<endl<<"* 2. Simulate one FIFO queue         *";
		cout<<endl<<"**************************************";
		cout<<endl<<"Input a command(0, 1, 2):";
		cin>>inputCommand;

		command = 0;

		for ( int i = 0 ; i < inputCommand.length() ; ++i ) {

            if ( !( inputCommand[i] <= '9' && inputCommand[i] >= '0' ) )
                command = -1;

		}

		if ( command != -1 )
            command = atoi( inputCommand.c_str() );

        inputCommand = "";

		switch (command)
		{
			case 0: break;
			case 1: cout << "\n" << "Input a file number: ";
					if ( fileProcessor.readInFile( nodes, measureTime ) ) {
                        readTime = ( ( ( measureTime / ( double ) CLOCKS_PER_SEC ) * 1000 ) );
						NodeSorter sorter = NodeSorter();
				        printer.printUnsortedData( nodes );
				        sorter.shellSort( nodes, measureTime );
                        sortTime = ( ( ( measureTime / ( double ) CLOCKS_PER_SEC ) * 1000 ) );
				        fileProcessor.writeSortedFile( nodes, measureTime );
				        writeTime = ( ( ( measureTime / ( double ) CLOCKS_PER_SEC ) * 1000 ) );
				        cout << "\n" ;
						cout << "Reading data: " << ( int )readTime << " clocks (" << fixed << setprecision(2) << readTime << " ms).\n";
				        cout << "Sorting data: " << ( int )sortTime << " clocks (" << fixed << setprecision(2) << sortTime << " ms).\n";
				        cout << "Writing data: " << ( int )writeTime << " clocks (" << fixed << setprecision(2) << writeTime << " ms).\n";
				        fileProcessor.getSoFileName(soFileName);
				        cout << "\n";
				        cout << "See " << soFileName << "\n";
				        wrongName = "";

					}
					else {
						soFileName = "";
						wrongName = fileProcessor.getWrongName();
					}

					break;
			case 2:	if ( soFileName == "" && wrongName == "" ) cout << endl << "Input a file number: ";

					if (wrongName != "" ) fileProcessor.readSortedFile( nodes, wrongName );
					else {
						if ( fileProcessor.readSortedFile( nodes, soFileName ) ) {
							Simulation start( nodes );
							if ( soFileName == "" ) fileProcessor.getSoFileName(soFileName);
							string onlyNum = "";
							for ( int i = 0 ; i < soFileName.length() ; i++) {
								if (soFileName[i] <= '9' && soFileName[i] >= '0')
									onlyNum = onlyNum + soFileName[i];
							}
	
						    string output = fileProcessor.writeOutputFile( start.getAbtList(), start.getDoneList(), onlyNum );
						    cout << "\n";
						    cout << "The simulation is running...\n";
						    cout << "See " << output << "\n";
						}
					}


					break;
			default: cout << endl << "Command does not exist!" << endl;
		}

	} while (command != 0);
	system("pause");
	return 0;




}
