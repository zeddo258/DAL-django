// 11127201³¯«~§± 11127205©}©É¦w 

#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<iomanip>
#include<ctime>

using namespace std ;

// information of each job
typedef struct jT {
    int OID ;
    int arrival ;
    int duration ;
    int timeout ;
} jobType;

string fileID ;

class JobList{
    vector<jobType> aList ; // list of all jobs
    clock_t start_reading, end_reading, start_writing, end_writing, start_sorting, end_sorting ; // counting time

    void reset() { // reset the list off all jobs
        aList.clear();
    }

    void showJob() { // print all jobs
    	cout << "\t" << "OID" << "\t" << "Arrival" << "\t" << "Duration" << "\t" << "TimeOut" << endl ;
		for ( int i = 0; i < aList.size(); i++ )
			cout << "(" << i + 1 << ")" << "\t" << aList[i].OID << "\t" << aList[i].arrival << "\t" << aList[i].duration << "\t" << aList[i].timeout << "\t" << endl ;
		cout << endl ;
	}

    void sortByArrival() { // sort by arrival using shell sort
        int n = aList.size() ; // size of all jobs
        start_sorting = clock() ; // start counting the time spent sorting
        for ( int h = n/2; h > 0; h = h/2 ) { // Decreasing interval h
            for ( int unsorted = h; unsorted < n; ++unsorted ) { // Iterate through unsorted sublist
                int loc = unsorted ;

                // the data of next job to be sorted
                int nextArrival = aList[unsorted].arrival ;
                int nextOID = aList[unsorted].OID ;
                int nextDuration = aList[unsorted].duration ;
                int nexttimeOut = aList[unsorted].timeout ;

                // Insertion sort for the current interval h
                // if two jobs have the same arrival, sort by OID
                for ( ; (loc>=h)&&(aList[loc-h].arrival>nextArrival||(aList[loc - h].arrival == nextArrival && aList[loc - h].OID > nextOID)); loc = loc-h ) {
                    aList[loc].arrival = aList[loc-h].arrival ;
                    aList[loc].OID = aList[loc-h].OID ;
                    aList[loc].duration = aList[loc-h].duration ;
                    aList[loc].timeout = aList[loc-h].timeout ;
                }
                // Place the next job at the correct position in the sorted sublist
                aList[loc].arrival = nextArrival ;
                aList[loc].OID = nextOID ;
                aList[loc].duration = nextDuration ;
                aList[loc].timeout = nexttimeOut ;
            }
        }
        end_sorting = clock() ; // stop counting the time spent sorting
    }

    void putAll() { // write list of sorted jobs into a text file and output it
        start_writing = clock() ; // start counting the time spent writing

        // output a text file named by input file's file number
        string sortedList = "sorted" + fileID + ".txt" ;
        ofstream outputFile(sortedList) ;
        if (outputFile.is_open()) {
            outputFile << "OID  Arrival Duration    Timeout\n" ; // write the title into file

            // write all jobs' data into file
            for ( int i = 0; i < aList.size(); i++ )
                outputFile << aList[i].OID << "\t" << aList[i].arrival << "\t" << aList[i].duration << "\t" << aList[i].timeout << "\n" ;

            outputFile.close() ; // Close the file
        }

		end_writing = clock() ; // stop counting the time spent writing
	}


    void showTime() { // calculate and print the time spent on reading, sorting, and writing
        double durationReading = ((double)(end_reading - start_reading)) / CLOCKS_PER_SEC * 1000.0;
        double durationSorting = ((double)(end_sorting - start_sorting)) / CLOCKS_PER_SEC * 1000.0;
        double durationWriting = ((double)(end_writing - start_writing)) / CLOCKS_PER_SEC * 1000.0;
        cout << "Reading data: " << end_reading - start_reading << " clocks (" << setprecision(2) << fixed << durationReading << " ms)." << endl;
        cout << "Sorting data: " << end_sorting - start_sorting << " clocks (" << setprecision(2) << fixed << durationSorting << " ms)." << endl;
        cout << "Writing data: " << end_writing - start_writing << " clocks (" << setprecision(2) << fixed << durationWriting << " ms)." << endl;
    }

public:
    JobList() { reset(); }
    ~JobList() { reset(); }
    JobList(const JobList& other) {
        aList = other.aList;
    }

    void getFirst( jobType& firstJob ) { // get the first job in the list
        firstJob.OID = aList[0].OID ;
        firstJob.arrival = aList[0].arrival ;
        firstJob.duration = aList[0].duration ;
        firstJob.timeout = aList[0].timeout ;
    }

    void deleteHead() { // delete the first job in the list
        aList.erase(aList.begin()) ;
    }

    void nextJob( jobType& nextData ) { // get the next job in the list
        if ( aList.size() >= 2 ) {
            nextData.OID = aList.at(1).OID ;
            nextData.arrival = aList.at(1).arrival ;
            nextData.duration = aList.at(1).duration ;
            nextData.timeout = aList.at(1).timeout ;
        }
    }

	int Length_Data() { // get the number of job items
        fstream file;
        string  temp ;

        file.open( ( "input" + fileID  +  ".txt" ).c_str()) ;
        int total_data = 0;
        for ( total_data ; file >> temp ;  total_data++ ) ;
        file.close() ;

        int list_data = 0 ;
        list_data  = total_data / 4 ;
        list_data -= 1 ;
        return list_data ;
 	}


	int Length_Data_sorted() { // get the number of job items
        fstream file;
        string  temp ;

        file.open( ( "sorted" + fileID  +  ".txt" ).c_str()) ;
        int total_data = 0;
        for ( total_data ; file >> temp ;  total_data++ ) ;
        file.close() ;

        int list_data = 0 ;
        list_data  = total_data / 4 ;
        list_data -= 1 ;
        return list_data ;
 	}


    int length(){ // get the size of list of jobs
        return aList.size();
    }

    bool isEmpty() { // check if the list of job is empty
        return aList.empty();
    }

	bool  getAll() { // check if the input file.txt exists, then read all jobs from it, store them into a list(aList) if it exists, and return true
		jobType job;
		string temp;
		fstream file_input ;
		int jobItems = 0;

		file_input.open( ( "input" + fileID +  ".txt" ).c_str() ) ; // open the input file

		// there's no such input file: print the error message and return false
		if ( !file_input.is_open() ) {
            cout << "### input" << fileID << ".txt does not exist! ###" << endl ;
			return false ;
		}

		// the input file exists: read all jobs from it and store them into a list(aList) if it exists
        else {
            jobItems = Length_Data() ;
			start_reading = clock() ; // start counting the time spent reading
			file_input >> temp >> temp >> temp >> temp ;
			for ( int i = 0 ; i < jobItems ; i++ ) {
                jobType inputJob ;
                file_input >> inputJob.OID >> inputJob.arrival >> inputJob.duration >> inputJob.timeout ;
                aList.push_back(inputJob) ;
			}
			end_reading = clock() ; // stop counting the time spent reading
			file_input.close() ; // close the input file

			// the input file exists but is empty: print the error message and return false
			if ( isEmpty() ) {
                cout << "### Get nothing from the file input0.txt ! ###\n" ;
                return false ;
			}

			return true  ;
        }
	}

	bool getSorted() { // check if the sorted file.txt exists, then read all sorted jobs from it, store them into a list(aList) if it exists, and return true
        fstream sorted_file ;
		string temp ;

		sorted_file.open( ( "sorted" +  fileID + ".txt"  ).c_str() ) ; // open the sorted file

		// there's no such sorted file:  print the error message and return false
		if ( !sorted_file.is_open() )  {
			sorted_file.close() ;
			cout << "### sorted" << fileID << ".txt does not exist! ###" << endl ;
			return false ;
		}

		// the sorted file exists: read all sorted jobs from it, store them into a list(aList), and return true
        else {
        	int sortsize = Length_Data_sorted() ;
        	sorted_file >> temp >> temp >> temp >> temp ;
        	jobType data ;
        	for ( int i = 0 ; i < sortsize; i++ ) {
        		sorted_file >> data.OID >> data.arrival >> data.duration >> data.timeout ;
                aList.push_back(data) ;
			}
            sorted_file.close() ; // close the sorted file
			return true  ;
        }
	}


	void read_sort_write() {
        bool fileExist = getAll() ; // check and read the input file
        if ( fileExist == true ) {
            showJob() ; // print all unsorted jobs
            sortByArrival() ; // sort all jobs
            putAll() ; // write sorted jobs into sorted file
            showTime() ; // print the time spent reading, sorting, and writing
            cout << "\nSee sorted" << fileID << ".txt\n" ;
            reset() ; // reset the job list
        }
	}

};


template<typename T>
class JobQueue{

    T *cA ;
    int qFront, qBack ;
    int qSize, qMax ;

//	int next( int idx ) {
//        return ( idx + 1 ) % qMax ;
//	}

public:

    int avail ;

    JobQueue():cA(NULL), qFront(0), qBack(0), qSize(0), qMax(0), avail(0) {}
    JobQueue<T>(int maxS): qFront(0), qSize(0), avail(0) {
    	cA =   new T[maxS] ;
        qMax = maxS;
	}


	void enqueue(const T& element) { // add a job to the end of queue
        if ( !isFull() ) {
            if ( isEmpty() )
                qFront = qBack = 0 ;
            else
                qBack = (qBack+1) % qMax ;

            cA[qBack] = element ;
            qSize ++ ;
		}
    }

    void dequeue() { // delete the first job in queue
        if ( !isEmpty() ) {
			qFront = (qFront + 1) % qMax;
            qSize--;
		}
    }

    void dequeue( T& element ) { // delete and get the first job in queue
        if (!isEmpty()) {
            element = cA[qFront];
            qFront = (qFront + 1) % qMax;
            qSize--;
        }
    }

    int length() const { // get the soze of queue
        return qSize;
    }

	bool isEmpty() const { // check if the queue is empty
		if ( qSize == 0 ) return true ;
    	else return false  ;
	}

	bool isFull() const { // check if the queue if full
		if( qSize == qMax )
            return true ;
    	else
            return false;
	}

    void clearQ() { // reset the queue
        qFront = qBack = qSize = 0;
        delete[] cA;
        cA = NULL ;
    }

    ~JobQueue() {
        clearQ();
    }
};


class AnsList {

    typedef struct aT{
        int aID ; // OID of aborted jobs
        int aCID ;
        int abortTime ; // abort of aborted jobs
        int aDelay ; // delay of aborted jibs
    } abortType;

    typedef struct dT{
        int dID ; // OID of done jobs
        int dCID ;
        int dTime ; // departure of dine jobs
        int dDelay ; // delay of done jobs
    } doneType;

	vector<abortType> abortJobs ;
	vector<doneType> doneJobs ;

	float avgDelay ;
	float successRate ;

    void reset() {
        abortJobs.clear();
        doneJobs.clear();
        avgDelay = 0.0 ;
        successRate = 0.0 ;
    }

	void done_computeStat( doneType& done, int OID, int arrival, int duration, int timeOut, int startTime, int CID ) { // compute each done job's departure and delay
	    if ( startTime == 0 ) { // the first job done ( first job of all )
            done.dDelay = 0 ;
            done.dTime = arrival + duration ;
	    }
        else {
            done.dDelay = startTime - arrival ;
            done.dTime = arrival + done.dDelay + duration;
        }
	    done.dID = OID ;
	    done.dCID = CID ;
	}

    void abort_computeStat( abortType& aborted, int OID, int arrival, int duration, int timeOut, int startTime, int CID, int n ) {
        if ( n == -1 ) { // jobs be aborted when the queue is full
            aborted.abortTime = arrival ;
            aborted.aDelay = 0 ;
            aborted.aCID = 0 ;
        }
        else { // jobs aborted from queue
            aborted.aDelay = startTime - arrival ;
            aborted.abortTime = startTime ;
        }
        aborted.aID = OID ;
        aborted.aCID = CID ;
	}

	void avgDelay_computeStat() { // calculate the average delay time

	    // add the total delay time of aborted jobs and done jobs, then divide by the total number of jobs
	    float sumOfDelay = 0.0 ;
        for ( int i = 0; i < abortJobs.size(); i++ )
            sumOfDelay = sumOfDelay + abortJobs[i].aDelay ;

        for ( int i = 0; i < doneJobs.size(); i++ )
            sumOfDelay = sumOfDelay + doneJobs[i].dDelay ;

        avgDelay = sumOfDelay / ( abortJobs.size()+doneJobs.size()) ;
	}

	void successRate_computeStat() { // calculate the success rate

	    // divide the number of done jobs by the total number of jobs
	    float allJobs = doneJobs.size() + abortJobs.size() ;
        successRate = ( doneJobs.size() / allJobs )*100 ;
	}

public:
    AnsList(): avgDelay(0.0), successRate(0.0) { reset(); }
    ~AnsList() { reset(); }
    AnsList(const AnsList& other) {
        abortJobs = other.abortJobs;
        doneJobs = other.doneJobs ;
    }


    void addAbortJob( int OID, int arrival, int duration, int timeOut, int startTime, int CID, int n ) {
        abortType aborted ;
        abort_computeStat( aborted, OID, arrival, duration, timeOut, startTime, CID, n ) ; // calculate aborted job's aborted time and delay
        abortJobs.push_back( aborted ) ; // add aborted job to list of aborted jobs
    }

    void addDoneJob( int OID, int arrival, int duration, int timeOut, int startTime, int CID ) {
        doneType done ;
        done_computeStat( done, OID, arrival, duration, timeOut, startTime, CID ) ; // calculate done job's departure time and delay
        doneJobs.push_back( done ) ; // add done job to list of done jobs
    }


    void putAll1() { // write done and aborted jobs into a text file and output it
        string ansList = "output" + fileID + ".txt" ;
        ofstream outputFile(ansList) ;

        if (outputFile.is_open()) { // open file
            outputFile << "\t" << "[Abort Jobs]" << endl << "\t" << "OID" << "\t" << "Abort" << "\t" << "Delay" << endl ; // write title into output file

            // write all aborted jobs' information into output file
            for ( int i = 0; i < abortJobs.size(); i++ )
                outputFile << "[" << i + 1 << "]" << "\t" <<  abortJobs[i].aID << "\t" <<  abortJobs[i].abortTime << "\t" <<  abortJobs[i].aDelay << endl ;

            outputFile << "\t" << "[Jobs Done]" << endl << "\t" << "OID" << "\t" << "Departure" << "\t" << "Delay" << endl ; // write title into output file

            // write all done jobs' information into output file
            for ( int i = 0; i < doneJobs.size(); i ++ )
                outputFile << "[" << i + 1 << "]" << "\t" <<  doneJobs[i].dID << "\t" <<  doneJobs[i].dTime << "\t" <<  doneJobs[i].dDelay << endl ;

            // calculate average delay and success rate
            avgDelay_computeStat() ;
            successRate_computeStat() ;

            // write average delay and success rate into output file
            outputFile << "[Average Delay]\t" << setprecision(2) << fixed << avgDelay << " ms\n" ;
            outputFile << "[Success Rate]\t" << setprecision(2) << fixed << successRate << " %\n" ;

            outputFile.close() ; // Close the file
        }
    }


    void putAll2() { // write done and aborted jobs into a text file and output it
        string ansList = "double" + fileID + ".txt" ;
        ofstream outputFile(ansList) ;

        if (outputFile.is_open()) { // open file
            outputFile << "\t" << "[Abort Jobs]" << endl << "\tOID\tCID\tAbort\tDelay" << endl ; // write title into output file

            // write all aborted jobs' information into output file
            for ( int i = 0; i < abortJobs.size(); i++ )
                outputFile << "[" << i + 1 << "]" << "\t" <<  abortJobs[i].aID << "\t" << abortJobs[i].aCID << "\t" <<  abortJobs[i].abortTime << "\t" <<  abortJobs[i].aDelay << endl ;

            outputFile << "\t" << "[Jobs Done]" << endl << "\tOID\tCID\tDeparture\tDelay" << endl ; // write title into output file

            // write all done jobs' information into output file
            for ( int i = 0; i < doneJobs.size(); i ++ )
                outputFile << "[" << i + 1 << "]" << "\t" <<  doneJobs[i].dID << "\t" << doneJobs[i].dCID << "\t" <<  doneJobs[i].dTime << "\t" <<  doneJobs[i].dDelay << endl ;

            // calculate average delay and success rate
            avgDelay_computeStat() ;
            successRate_computeStat() ;

            // write average delay and success rate into output file
            outputFile << "[Average Delay]\t" << setprecision(2) << fixed << avgDelay << " ms\n" ;
            outputFile << "[Success Rate]\t" << setprecision(2) << fixed << successRate << " %\n" ;

            outputFile.close() ; // Close the file
        }
    }


    void showAll() {
        cout << "[Abort]\n" ;
        for ( int i = 0; i < abortJobs.size(); i++ ) {
            cout << abortJobs[i].aID << "\t" ;
        }

        cout << "\n\n" ;

        cout << "[Done]\n" ;
        for ( int i = 0; i < doneJobs.size(); i++ ) {
            cout << doneJobs[i].dID << "\t" ;
        }
        cout << "\n" ;
    }

    bool hasBeenProcessed( int id ) {
        for ( int i = 0; i < doneJobs.size(); i++ ) {
            if ( id == doneJobs[i].dID )
                return true ;
        }
        for ( int i = 0; i < abortJobs.size(); i++ ) {
            if ( id == abortJobs[i].aID )
                return true ;
        }

        return false ;
    }

};


class Simulation {
    JobList job ;
    AnsList answers ;
    JobQueue <jobType> **allQ ;
    int qNum ; // number of queues

public:

    Simulation( JobList aList, int N, int& exist, int qSize ): job(aList), qNum(N) {

        allQ = new JobQueue<jobType>*[qNum];
        for (int i = 0; i < qNum; ++i)
            allQ[i] = new JobQueue<jobType> (qSize);

        if ( !job.getSorted() )
            exist = 0 ;
    }

    void process1() { // simulate the process stage
        jobType currentJob ; // the current processed job
        jobType nextJob ; // the job be processed next
        int preEnd = 0;
        int size_input = job.Length_Data_sorted() ; // input Data size  
        while ( !job.isEmpty() ) { // list is not empty
            if ( allQ[0] -> isEmpty()  ) {
                // Get the first job in the list
                job.getFirst( currentJob ) ; 
                job.deleteHead() ; // remove the first done job from the list
                answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout,0, 0 ) ; // add the first job into done list
                preEnd =  currentJob.arrival + currentJob.duration ; // the first job's departure time is the next job's start time
                if ( job.length() > 0 )
                    job.getFirst( nextJob ) ;// the first job in the list now is the next processed job
                else break ;
            }


            while ( !( allQ[0] -> isFull() ) )  {
            	// queue is empty means the process stage just get started
                if (  preEnd > nextJob.arrival ) {
                    if  (  !job.isEmpty() ) {
                    	// It needs to be greater than the arrival time of the next , and then it will be stuffed in the queue.
                        allQ[0] -> enqueue( nextJob ) ;// stuffed in the queue.
                        if (  job.length() > 1 && preEnd > nextJob.arrival ) {
                            job.nextJob( nextJob ) ;// more than 1 piece of data to get the next piece of data.
                        }
                        else {
                            job.deleteHead() ;
                            break;
                        }
                    }
                    job.deleteHead() ;

                }
                else  { //  preEnd <= nextJob.arrival
                    if ( !( allQ[0] -> isEmpty() )  ) {
                        allQ[0]->dequeue( currentJob ) ; // process the first job in queue
                        if ( (preEnd+currentJob.duration) > currentJob.timeout ) {// abort
                            if ( preEnd <= currentJob.timeout  ) {
                                preEnd = currentJob.timeout ; // next job's start time: current job's timeout
                                answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1, 0 ) ;
                            }
                            else
                                answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1, 0 ) ;
                        }
                        else { // Done
                            if ( (preEnd+currentJob.duration) ==  currentJob.timeout ) {
                                answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1 ) ;
                                preEnd = currentJob.timeout ; // next job's start time: current job's departure time

                            }
                            else if ( (preEnd+currentJob.duration) < currentJob.timeout ) {
                                answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1 ) ;
                                preEnd = preEnd + currentJob.duration ; // next job's start time: current job's departure time

                            }
                        }
                    }
                    else  {// The queue is all done and a new round is about to start.

                        preEnd =  nextJob.arrival + nextJob.duration ;//  departure time  of First job in the next round is the next job's start time
                        job.deleteHead() ; // remove the first done job from the list
                        answers.addDoneJob( nextJob.OID, nextJob.arrival, nextJob.duration, nextJob.timeout, 0, -1 ) ; // add the first job  of the next round in the list now into done list
                        if ( job.length() > 0 )
                            job.getFirst( nextJob ) ; // more than 1 piece of data to get the next piece of data.
                        else {
                            break ;
                        }
                    }
                }
            }

            if ( allQ[0] -> isFull() ) {
                // abort job in the list that arrived before the previous processed job finished
                if ( preEnd > nextJob.arrival ) {
                    answers.addAbortJob( nextJob.OID, nextJob.arrival, nextJob.duration, nextJob.timeout, preEnd , -1, -1 ) ;
                    if ( job.length() > 1 )
                        job.nextJob( nextJob ) ; // Get the next job
                    job.deleteHead() ;
                }


                else {
                    //  if it arrived exactly or later then the process the queue data first
                    allQ[0]->dequeue( currentJob ) ; // process the first job in queue

                    if ( (preEnd+currentJob.duration) > currentJob.timeout ) { // abort
                        if ( preEnd <= currentJob.timeout  ) {
                            preEnd = currentJob.timeout ; // next job's start time: current job's timeout
                            answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1, 0 ) ;
                        }
                        else
                            answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1, 0 ) ;
                    }
                    else { // Done

                    // current job is able to finish before it's timeout --> add it into done list
                        if ( (preEnd+currentJob.duration) ==  currentJob.timeout ) {
                            answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1 ) ;
                            preEnd = currentJob.timeout ; // next job's start time: current job's departure time
                        }
                        else if ( (preEnd+currentJob.duration) < currentJob.timeout ) {
                            answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1 ) ;
                            preEnd = preEnd + currentJob.duration ; // next job's start time: current job's departure time plus next job's start time

                        }
                    }
                }
            }
        }

        // the job in list is all processed
        // process the remaining jobs in queue
        while ( !(allQ[0]->isEmpty()) ) {
            allQ[0]->dequeue( currentJob ) ; // process the first job in queue
            if ( (preEnd+currentJob.duration) > currentJob.timeout ) {// abort
                if ( preEnd <= currentJob.timeout  ) {
                    preEnd = currentJob.timeout ; // next job's start time: current job's timeout
                    answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1, 0 ) ;
                }
                else
                    answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1, 0 ) ;
            }
            else { // Done
                if ( (preEnd+currentJob.duration) ==  currentJob.timeout ) {
                    answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1 ) ;
                    preEnd = currentJob.timeout ; // next job's start time: current job's departure time
                }
                else if ( (preEnd+currentJob.duration) < currentJob.timeout ) {
                    answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd, -1 ) ;
                    preEnd = preEnd + currentJob.duration ; // next job's start time: current job's departure time plus next job's start time
                }
            }
        }
        answers.putAll1() ;
    }

    void process2() { // simulate the process stage
        jobType currentJob ; // the current processed job
        jobType nextJob ; // the job will be processed next
        int preEnd1 = 0, preEnd2 = 0 ; // the previous processed job's ending time
        int cpu1 = 0, cpu2 = 0 ; // if cpu occupied

        job.getFirst( nextJob ) ;

        while ( !job.isEmpty() ) { // list is not empty

            while ( nextJob.arrival >= preEnd1 && !(allQ[0]->isEmpty()) ) { // clear all jobs that finished earlier than the next job arrives
                allQ[0]->dequeue( currentJob ) ; // process the first job in queue

                if ( (preEnd1+currentJob.duration) > currentJob.timeout ) { // abort
                    if ( preEnd1 <= currentJob.timeout  ) { // couldn't finish in time
                        preEnd1 = currentJob.timeout ; // next job's start time: current job's timeout
                        answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd1, 1, 0 ) ;
                    }
                    else // couldn't even start // directly abort
                        answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd1, 1, 0 ) ;
                }
                else { // Done
                    if ( preEnd1 < currentJob.arrival  ) { // previous job has left
                        answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, 0, 1 ) ;
                        preEnd1 = currentJob.arrival + currentJob.duration ; // next job's start time: current job's departure time
                    }
                    else {
                        if ( (preEnd1+currentJob.duration) ==  currentJob.timeout ) { // done just in time
                            answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd1, 1 ) ;
                            preEnd1 = currentJob.timeout ; // next job's start time: current job's departure time
                        }
                        else if ( (preEnd1+currentJob.duration) < currentJob.timeout ) { // done earlier than timeout
                            answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd1, 1 ) ;
                            preEnd1 = preEnd1 + currentJob.duration ; // next job's start time: current job's departure time
                        }
                    }
                }

                if ( allQ[0]->isEmpty() && preEnd1 <= nextJob.arrival ) // cpu and queue are clear
                    cpu1 = 0 ;
            }


            while ( nextJob.arrival >= preEnd2 && !(allQ[1]->isEmpty()) ) { // clear all jobs that finished earlier than the next job arrives
                allQ[1]->dequeue( currentJob ) ; // process the first job in queue

                if ( (preEnd2+currentJob.duration) > currentJob.timeout ) {// abort
                    if ( preEnd2 <= currentJob.timeout  ) { // couldn't finish in time
                        preEnd2 = currentJob.timeout ; // next job's start time: current job's timeout
                        answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd2, 2, 0 ) ;
                    }
                    else // couldn't even start // directly abort
                        answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd2, 2, 0 ) ;
                }
                else { // Done
                    if ( preEnd2 < currentJob.arrival ) { // previous job has left
                        answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, 0, 2 ) ;
                        preEnd2 = currentJob.arrival + currentJob.duration ; // next job's start time: current job's departure time
                    }
                    else {
                        if ( (preEnd2+currentJob.duration) ==  currentJob.timeout ) { // done just in time
                            answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd2, 2 ) ;
                            preEnd2 = currentJob.timeout ; // next job's start time: current job's departure time
                        }
                        else if ( (preEnd2+currentJob.duration) < currentJob.timeout ) { // done leave earlier than timeout
                            answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd2, 2 ) ;
                            preEnd2 = preEnd2 + currentJob.duration ; // next job's start time: current job's departure time
                        }
                    }
                }

                if ( allQ[1]->isEmpty() && preEnd2 <= nextJob.arrival ) // cpu and queue are clear
                    cpu2 = 0 ;
            }


            if ( allQ[0] -> isEmpty() && allQ[1] -> isEmpty() ) { // case2 // both empty
                if ( nextJob.arrival >= preEnd1 ) // cpu1 is done and clear
                    cpu1 = 0 ;

                if ( nextJob.arrival >= preEnd2 ) // cpu2 is done and clear
                    cpu2 = 0 ;

                if ( cpu1 == 0 && cpu2 == 0 ) { // all queues are empty // all cpu are empty // job done in cpu1
                    cpu1 = 1 ; // cpu1 is occupied
                    answers.addDoneJob( nextJob.OID, nextJob.arrival, nextJob.duration, nextJob.timeout, 0, 1 ) ;
                    preEnd1 = nextJob.arrival + nextJob.duration ;
                    job.deleteHead() ;
                    if ( job.length() > 0 )
                        job.getFirst( nextJob ) ;
                }
                else if ( cpu1 == 0 && cpu2 == 1 ) { // all queues are empty // cpu2 is occupied // job done in cpu1
                    cpu1 = 1 ;
                    answers.addDoneJob( nextJob.OID, nextJob.arrival, nextJob.duration, nextJob.timeout, 0, 1 ) ;
                    preEnd1 = nextJob.arrival + nextJob.duration ;
                    job.deleteHead() ;
                    if ( job.length() > 0 )
                        job.getFirst( nextJob ) ;
                }
                else if ( cpu1 == 1 && cpu2 == 0 ) { // all queues are empty // cpu1 is occupied // job done in cpu2
                    cpu2 = 1 ;
                    answers.addDoneJob( nextJob.OID, nextJob.arrival, nextJob.duration, nextJob.timeout, 0, 2 ) ;
                    preEnd2 = nextJob.arrival + nextJob.duration ;
                    job.deleteHead() ;
                    if ( job.length() > 0 )
                        job.getFirst( nextJob ) ;
                }
                else if ( cpu1 == 1 && cpu2 == 1 ) { // all queues are empty // all cpus are occupied // job into queue1
                    allQ[0] -> enqueue( nextJob ) ;
                    job.deleteHead() ;
                    if ( job.length() > 0 )
                        job.getFirst( nextJob ) ;
                }
            }

            else if (  allQ[0] ->isEmpty() && !(allQ[1] ->isEmpty()) ) { // case1: queue1 is empty and queue2 is not // into queue1
                allQ[0] -> enqueue( nextJob ) ;
                job.deleteHead() ;
                if ( job.length() > 0 )
                        job.getFirst( nextJob ) ;
            }

            else if ( !(allQ[0] ->isEmpty()) && allQ[1] ->isEmpty() ) { // case1: queue2 is empty and queue1 is not // into queue2
                allQ[1] -> enqueue( nextJob ) ;
                job.deleteHead() ;
                if ( job.length() > 0 )
                        job.getFirst( nextJob ) ;
            }

            else if ( (!(allQ[0] ->isEmpty()) && !(allQ[1] ->isEmpty())) ) { // case3 // all queues are not empty
                if ( !(allQ[0] ->isFull()) && !(allQ[1] ->isFull() )  ) { // all queues are not full
                    if ( allQ[0]->length() > allQ[1]->length() ) { // queue2 is shorter than queue1 // into queue2
                        allQ[1] -> enqueue( nextJob ) ;
                        job.deleteHead() ;
                        if ( job.length() > 0 )
                            job.getFirst( nextJob ) ;
                    }
                    else { // queue1 is shorter(or as long as) than queue2 // into queue1
                        allQ[0] -> enqueue( nextJob ) ;
                        job.deleteHead() ;
                        if ( job.length() > 0 )
                            job.getFirst( nextJob ) ;
                    }
                }

                else if ( !(allQ[0] ->isFull()) && allQ[1] ->isFull() ){ // queue1 is not empty // queue2 is empty // into queue1
                    allQ[0] -> enqueue( nextJob ) ;
                    job.deleteHead() ;
                    if ( job.length() > 0 )
                        job.getFirst( nextJob ) ;
                }
                else if ( allQ[0] ->isFull() && !( allQ[1] ->isFull() ) ) { // queue1 is empty // queue2 is not empty // into queue2
                    allQ[1] -> enqueue( nextJob ) ;
                    job.deleteHead() ;
                    if ( job.length() > 0 )
                        job.getFirst( nextJob ) ;

                }

                else if ( allQ[0] ->isFull() && allQ[1] ->isFull() ) { // case 4 // all queues are full // abort job directly // CID=0
                    answers.addAbortJob( nextJob.OID, nextJob.arrival, nextJob.duration, nextJob.timeout, preEnd1, 0, -1 ) ;
                    job.deleteHead() ;
                    if ( job.length() > 0 )
                        job.getFirst( nextJob ) ;
                }
            }
        }


        while ( !(allQ[0]->isEmpty()) ) { // process all jobs remains in queue1
            allQ[0]->dequeue( currentJob ) ; // process the first job in queue and remove it from queue

            if ( (preEnd1+currentJob.duration) > currentJob.timeout ) { // abort
                if ( preEnd1 <= currentJob.timeout  ) { // couldn't finish in time
                    preEnd1 = currentJob.timeout ; // next job's start time: current job's timeout
                    answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd1, 1, 0 ) ;
                }
                else // couldn't even start // abort directly
                    answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd1, 1, 0 ) ;
            }
            else { // Done
                if ( (preEnd1+currentJob.duration) ==  currentJob.timeout ) { // done just in time
                    answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd1, 1 ) ;
                    preEnd1 = currentJob.timeout ; // next job's start time: current job's departure time
                }
                else if ( (preEnd1+currentJob.duration) < currentJob.timeout ) { // done earlier than timeout
                    answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd1, 1 ) ;
                    preEnd1 = preEnd1 + currentJob.duration ; // next job's start time: current job's departure time
                }
            }
        }



        while ( !(allQ[1]->isEmpty()) ) { // process all jobs remains in queue2
            allQ[1]->dequeue( currentJob ) ; // process the first job in queue and remove it from queue

            if ( (preEnd2+currentJob.duration) > currentJob.timeout ) { // abort
                if ( preEnd2 <= currentJob.timeout  ) { // couldn't finish in time
                    preEnd2 = currentJob.timeout ; // next job's start time: current job's timeout
                    answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd2, 2, 0 ) ;
                }
                else // // couldn't even start // abort directly
                    answers.addAbortJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd2, 2, 0 ) ;
            }
            else { // Done
                if ( (preEnd2+currentJob.duration) ==  currentJob.timeout ) { // done just in time
                    answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd2, 2 ) ;
                    preEnd2 = currentJob.timeout ; // next job's start time: current job's departure time
                }
                else if ( (preEnd2+currentJob.duration) < currentJob.timeout ) { // done earlier than timeout
                    answers.addDoneJob( currentJob.OID, currentJob.arrival, currentJob.duration, currentJob.timeout, preEnd2, 2 ) ;
                    preEnd2 = preEnd2 + currentJob.duration ; // next job's start time: current job's departure time
                }
            }
        }

        answers.putAll2() ; // write answers into double.txt and output it
    }

    ~Simulation() {
        for ( int i = 0; i < qNum ; i++ ) {
            allQ[i] -> clearQ() ;
            delete allQ[i] ;
        }
        delete[] allQ ;
    }

};

int main() {

    JobList jobList ;
    AnsList ans ;
    int command ;
    int countOfLoop = 0 ;

    // keep simulating until command is 0
    while ( true ) {
        cout << "\n**** Simulation FIFO Queues by SQF *****\n" ;
        cout << "* 0. Quit                              *\n" ;
        cout << "* 1. Sort a file                       *\n" ;
        cout << "* 2. Simulate one FIFO queue           *\n" ;
        cout << "* 3. Simulate two queues by SQF        *\n" ;
        cout << "****************************************\n" ;
        cout << "Input a command(0, 1, 2, 3): " ;
        cin >> command ;

        if ( command == 0 ) // stop simulation if command is 0
            break ;

        else if ( command == 1 ) { // command is 1
            cout << "\nInput a file number: " ;
            cin >> fileID ;
            cout << "\n" ;
            jobList.read_sort_write() ;
            // check if there's such filed named as input fileID
            // exists: read the file, add all jobs into list, print jobs, sort them, write sorted jobs into a file, then output it
            // doesn't exist: print error message and re-receive command

        }


        else if ( command == 2 ) { // process sorted jobs with 1 cpu
            if ( countOfLoop == 0 ) { // the first round of simulation: let user enter a sorted file number and process it if the file exists
                cout << "\nInput a file number: " ;
                cin >> fileID ;
            }
            cout << "\n" ;
            int exist = 1 ;
            Simulation sim( jobList, 1, exist, 3 ) ;
            if ( exist == 1 ) { // the file exists
                sim.process1() ; // start simulating
                cout << "The simulation is running...\n" ;
                cout << "See output" << fileID << ".txt\n" ;
            }
        }

        else if ( command == 3) { // process sorted jobs with 2 cpus
            if ( countOfLoop == 0 ) { // the first round of simulation: let user enter a sorted file number and process it if the file exists
                cout << "\nInput a file number: " ;
                cin >> fileID ;
            }
            cout << "\n" ;
            int exist = 1 ;
            Simulation sim( jobList, 2, exist, 3 ) ;
            if ( exist == 1 ) { // the file exists
                sim.process2() ; // start simulating
                cout << "The simulation is running...\n" ;
                cout << "See double" << fileID << ".txt\n" ;
            }
        }

        else {
            cout << "\nCommand does not exist!\n" ; // re-receive command if command is not 1 or 2
        }

        countOfLoop ++ ;
    }
}
