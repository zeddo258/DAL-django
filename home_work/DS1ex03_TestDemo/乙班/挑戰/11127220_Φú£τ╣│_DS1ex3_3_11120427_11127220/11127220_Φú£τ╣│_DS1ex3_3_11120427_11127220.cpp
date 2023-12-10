// 11120247 ½²Úô¯ô 11127220 ³¢¤d»²
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <iostream>
# include <vector>
# include <cctype>  // calculate
# include <cstdlib>  // system, atoi
# include <fstream>  // read file
# include <algorithm>  // count
# include <chrono>
# include <iomanip>
# include <cmath>

using namespace std ;
using namespace chrono ;

struct sorted{      
	int OID ;
	int arrival ;
	int duration ;
	int timeOut ;
};

struct abortlist{
	int OID ;
	int CID ;
	int abort ;
	int delay ;
};

struct donelist{
	int OID ;
	int CID ;
	int departure ;
	int delay ;
};

class Queue{
    private:
        vector<sorted> sort ;
    public:
		sorted front() {
			return sort[0] ;  // return the first data
		}
		
		sorted back() {
			return sort.back() ;  // return the last data
		}
		
		void pop() {
			sort.erase(sort.begin()) ;  // delete first data
		}	
		
		void push(sorted temp) {
			sort.push_back(temp) ;  // enter a data
		}
		
		bool empty() {
			if ( sort.empty() ) 
                return true ;
			else
                return false ;
		}
		
		int size() {
			return sort.size() ;
		}
};

// shell sort using Knuth's gap (1, 4, 13, 40, ...)
template<typename T>
void shell_sort(vector<T> &array) {
    int step = 1, length = array.size();
    while (step < length / 3) {
        step = 3 * step + 1;
    }

    while (step >= 1) {
        for (int i = step; i < length; i++) {
            int j = i;
            T item = array[j];
            // check if moves are needed
            while(j >= step && item < array[j - step]){
                array[j] = array[j - step];
                j -= step;
            }
            array[j] = item;
        }
        step = step / 3;
    }
}

// Job definition and settings
class Job{
    public:
        int OID;
        int Arrival;
        int Duration;
        int TimeOut;

        Job(int oid, int arrival, int duration, int timeout): OID(oid), Arrival(arrival), Duration(duration), TimeOut(timeout) {}

        bool operator<(const Job& job){
            return this->Arrival < job.Arrival || (this->Arrival == job.Arrival && this->OID < job.OID);
        }
};

ostream &operator<<(std::ostream &os, Job const &job) { 
    return os << job.OID << '\t' << job.Arrival << '\t' << job.Duration << '\t' << job.TimeOut;
}

class Mission1 {
    public:
        int main1() {
            string fileNumber;
            cout << "\nInput a file number: ";
            cin >> fileNumber;

            // Measure file reading time
            auto startRead = steady_clock::now();
            ifstream inFile("input" + fileNumber + ".txt");
            if ( !inFile.is_open() ) {
                cout << endl << "input" + fileNumber + ".txt" << " does not exist!" << endl ;
                return false ;
            } // if
            string OID, Arrival, Duration, Timeout;
            inFile >> OID >> Arrival >> Duration >> Timeout;
            vector<Job> jobs;
            int oid, arrival, duration, timeout, jobCnt = 0;
            while (inFile >> oid >> arrival >> duration >> timeout)
                jobs.emplace_back(Job(oid, arrival, duration, timeout));
            inFile.close();
            auto stopRead = steady_clock::now();
            auto durationRead = duration_cast<microseconds>(stopRead - startRead);

            // Measure sorting time
            auto startSort = steady_clock::now();
            shell_sort(jobs);
            auto stopSort = steady_clock::now();
            auto durationSort = duration_cast<microseconds>(stopSort - startSort);

            // Display the sorted array in the terminal
            cout << endl;
            cout << '\t' << OID << '\t' << Arrival << '\t' << Duration << '\t' << Timeout << endl;
            for (int i = 0; i < jobs.size(); i++){
                cout << '(' << i + 1 << ')';
                cout << '\t' << jobs[i].OID << '\t' << jobs[i].Arrival << '\t' << jobs[i].Duration << '\t' << jobs[i].TimeOut << endl;
            }

            // Measure file writing time
            auto startWrite = steady_clock::now();
            ofstream outFile("sorted" + fileNumber + ".txt");
            outFile << OID << '\t' << Arrival << '\t' << Duration << '\t' << Timeout << endl;
            for (int i = 0; i < jobs.size(); i++)
                outFile << jobs[i] << endl;
            outFile.close();
            auto stopWrite = steady_clock::now();
            auto durationWrite = duration_cast<milliseconds>(stopWrite - startWrite);

            // Display execution times
            cout << "\nReading data: " << durationRead.count() << " clocks" << "(" << fixed << setprecision(2) << durationRead.count()/1.0 << " ms)." << endl;
        	cout << "Sorting data: " << durationSort.count() << " clocks" << "(" << fixed << setprecision(2) << durationSort.count()/1.0 << " ms)." << endl;
        	cout << "Writing data: " << durationWrite.count() << " clocks" << "(" << fixed << setprecision(2) << durationWrite.count()/1.0 << " ms)." << endl;
			cout << "\nSee sorted" << fileNumber << ".txt\n" ;
            return 0;
        } // main1
};

class Mission2 {
    private:
        int current = 0 ;  // current time
        int delay = 0 ;  // delay time
        int curdata = 0 ;  // current data
        int deal = 0 ;  // dealing with which data
        int size ;  // amount of data in a file
        char fileName[500] ;
        string fileNum ;  // input file number
        Queue data ;  // a queue
        vector<int> store ;  // store the OID of cancel data
        vector<abortlist> abort ;  // cancel list 
        vector<donelist> done ;  // list done
    public:
        bool load () {  // read file
            cout << "\nInput a file number: " ;
            cin >> fileNum ;
            string fullFileName = "sorted" + fileNum + ".txt" ;
            strcpy( fileName, fullFileName.c_str() ) ;
            ifstream file( fileName ) ;
            if ( !file.is_open() ) {
                cout << endl << fileName << " does not exist!" << endl ;
                return false ;
            } // if
            
            return true ;
        } // load
        
        void save( int lack ) {  // save queue
            string line ;
            sorted temp ;
            ifstream file ;
            file.open( fileName ) ;
            getline ( file, line ) ;
        	int i = 0, j = curdata ;
        	while ( getline(file, line) )  // count size
		        i = i + 1 ;
		    size = i ;
        	file.close() ;
        	if ( lack < size - curdata )
        	    i = j + lack ;
        	file.open( fileName ) ;
        	getline(file, line) ;
            for ( j = curdata ; j < i ; j++ ) {  // svae in queue
            	for ( int k = 0 ; k < curdata ; k = k + 1 ) {
		        	file >> temp.OID ;
		        	file >> temp.arrival ;
		        	file >> temp.duration ;
                	file >> temp.timeOut ;
				} // for
				
		        file >> temp.OID ;
		        file >> temp.arrival ;
		        file >> temp.duration ;
                file >> temp.timeOut ;
                bool found = binary_search( store.begin(), store.end(), temp.OID ) ; 
                if ( !found )
		        	data.push(temp) ;
		    } // for
		    
		    for ( curdata = j ; curdata < size ; curdata++ ) {  // check forth data
		        file >> temp.OID ;
		        file >> temp.arrival ;
		        file >> temp.duration ;
                file >> temp.timeOut ;
                if ( temp.arrival < current ) {  // 4.1
                	abortlist adata ;
                	adata.OID = temp.OID ; 
					adata.abort = temp.arrival ;
					adata.delay = 0 ;
					store.push_back( temp.OID ) ;
					abort.push_back( adata ) ;
					deal = deal + 1 ;
					j = j + 1 ;
				} // if
				else
					curdata = size ;
			} // for
			
			curdata = j ;
		    file.close() ;
        } // save()
        
        void process() {
        	int lack = 0 ;
        	abortlist adata ;
        	donelist ddata ;
        	if ( data.size() < 3 && curdata < size ) {  // save data(queue)
        		lack = 3 - data.size() ;
        		save( lack ) ;
			} // if
			
			sorted one = data.front() ;
			if ( current > one.timeOut ) {  // 4.2
				adata.OID = one.OID ; 
				adata.abort = current ;
				adata.delay = adata.abort - one.arrival ;
				delay = delay + adata.delay ;
				abort.push_back( adata ) ;
			} // if
			else if ( current + one.duration > one.timeOut ) {  // 4.3
				adata.OID = one.OID ; 
				adata.abort = one.timeOut ;
				adata.delay = adata.abort - one.arrival ;
				delay = delay + adata.delay ;
				abort.push_back( adata ) ;
				current = one.timeOut ;
			} // else if
			else {  // 3.1
				if ( deal == 0 )
					current = one.arrival ;
				ddata.OID = one.OID ;
				if ( current < one.arrival )
					current = one.arrival ;
				ddata.departure = current + one.duration ;
				ddata.delay = current - one.arrival ;
				current = ddata.departure ;
				delay = delay + ddata.delay ;
				done.push_back( ddata ) ;
			} // else
			
			deal = deal + 1 ;
			data.pop() ;
		} // process()
		
		int main2() {
			save( 3 ) ;
			while ( deal < size )
				process() ;
			write() ;
			return 0 ;
		} // main2
		
		void write() {  // write file
			string fullFileName = "output" + fileNum + ".txt" ;
			ofstream outFile ;
			outFile.open(fullFileName.c_str()) ;
			string OID, Abort, Delay, Departure ;
            outFile << "\t[Abort Jobs]\n\t" << "OID\t" << "Abort\t" << "Delay\n" ;
            for (int i = 0 ; i < abort.size() ; i++)
                outFile << "[" << i + 1 << "]\t" << abort[i].OID << "\t" << abort[i].abort << "\t" << abort[i].delay << endl ;
                
            outFile << "\t[Jobs Done]\n\t" << "OID\t" << "Departure\t" << "Delay\n" ;
            for (int i = 0 ; i < done.size() ; i++)
                outFile << "[" << i + 1 << "]\t" << done[i].OID << "\t" << done[i].departure << "\t" << done[i].delay << endl ;
            float ave = delay * 1.0 / size * 1.0 ;
			outFile << "[Average Delay] " << fixed << setprecision(2) << ave << " ms" ;
			float successRate = done.size() * 100.0 / size ;
			outFile << "\n[Success Rate]  " << fixed << setprecision(2) << successRate << " %\n" ;
            outFile.close() ;
            cout << "\nThe simulation is running...\nSee " << fullFileName << endl ;
		} // write()
};

class Mission3 {
    private:
        int current = 0 ;  // current time of CPU1
        int current2 = 0 ;  // current time of CPU2
        int delay = 0 ;  // delay time
        int curdata = 0 ;  // current data
        int deal = 0 ;  // dealing with which data
        int size ;  // amount of data in a file
        char fileName[500] ;
        string fileNum ;  // input file number
        Queue data ;  // queue1
        Queue data2 ;  // queue2
        vector<int> store ;  // store the OID of cancel data
        vector<abortlist> abort ;  // cancel list 
        vector<donelist> done ;  // list done
    public:
        bool load () {  // read file
            cout << "\nInput a file number: " ;
            cin >> fileNum ;
            string fullFileName = "sorted" + fileNum + ".txt" ;
            strcpy( fileName, fullFileName.c_str() ) ;
            ifstream file( fileName ) ;
            if ( !file.is_open() ) {
                cout << endl << fileName << " does not exist!" << endl ;
                return false ;
            } // if
            
            return true ;
        } // load
        
        void save( int lack ) {  // save queue
            string line ;
            sorted temp ;
            ifstream file ;
            file.open( fileName ) ;
            getline ( file, line ) ;
        	int i = 0, j = curdata ;
        	while ( getline(file, line) )  // count size
		        i = i + 1 ;
		    size = i ;
        	file.close() ;
        	if ( lack < size - curdata )
        	    i = j + lack ;
        	file.open( fileName ) ;
        	getline(file, line) ;
            for ( j = curdata ; j < i ; j++ ) {  // svae in queue
            	for ( int k = 0 ; k < curdata ; k = k + 1 ) {
		        	file >> temp.OID ;
		        	file >> temp.arrival ;
		        	file >> temp.duration ;
                	file >> temp.timeOut ;
				} // for
				
		        file >> temp.OID ;
		        file >> temp.arrival ;
		        file >> temp.duration ;
                file >> temp.timeOut ;
                bool found = binary_search( store.begin(), store.end(), temp.OID ) ; 
                if ( !found )
		        	data.push(temp) ;
		    } // for
		    
		    for ( curdata = j ; curdata < size ; curdata++ ) {  // check forth data
		        file >> temp.OID ;
		        file >> temp.arrival ;
		        file >> temp.duration ;
                file >> temp.timeOut ;
                if ( temp.arrival < current ) {  // 4.1
                	abortlist adata ;
                	adata.OID = temp.OID ; 
					adata.abort = temp.arrival ;
					adata.delay = 0 ;
					store.push_back( temp.OID ) ;
					abort.push_back( adata ) ;
					deal = deal + 1 ;
					j = j + 1 ;
				} // if
				else
					curdata = size ;
			} // for
			
			curdata = j ;
		    file.close() ;
        } // save()

		void save2( int lack ) {  // save queue
            string line ;
            sorted temp ;
            ifstream file ;
            file.open( fileName ) ;
            getline ( file, line ) ;
        	int i = 0, j = curdata ;
        	while ( getline(file, line) )  // count size
		        i = i + 1 ;
		    size = i ;
        	file.close() ;
        	if ( lack < size - curdata )
        	    i = j + lack ;
        	file.open( fileName ) ;
        	getline(file, line) ;
            for ( j = curdata ; j < i ; j++ ) {  // svae in queue
            	for ( int k = 0 ; k < curdata ; k = k + 1 ) {
		        	file >> temp.OID ;
		        	file >> temp.arrival ;
		        	file >> temp.duration ;
                	file >> temp.timeOut ;
				} // for
				
		        file >> temp.OID ;
		        file >> temp.arrival ;
		        file >> temp.duration ;
                file >> temp.timeOut ;
                bool found = binary_search( store.begin(), store.end(), temp.OID ) ; 
                if ( !found )
		        	data2.push(temp) ;
		    } // for
		    
		    for ( curdata = j ; curdata < size ; curdata++ ) {  // check forth data
		        file >> temp.OID ;
		        file >> temp.arrival ;
		        file >> temp.duration ;
                file >> temp.timeOut ;
                if ( temp.arrival < current2 ) {  // 4.1
                	abortlist adata ;
                	adata.OID = temp.OID ; 
                	adata.CID = 0 ;
					adata.abort = temp.arrival ;
					adata.delay = 0 ;
					store.push_back( temp.OID ) ;
					abort.push_back( adata ) ;
					deal = deal + 1 ;
					j = j + 1 ;
				} // if
				else
					curdata = size ;
			} // for
			
			curdata = j ;
		    file.close() ;
        } // save2()
        
        void process1() {
        	abortlist adata ;
        	donelist ddata ;
        	sorted one = data.front() ;
			if ( current > one.timeOut ) {  // 4.2
				adata.OID = one.OID ;
				adata.CID = 1 ; 
				adata.abort = current ;
				adata.delay = adata.abort - one.arrival ;
				delay = delay + adata.delay ;
				abort.push_back( adata ) ;
			} // if
			else if ( current + one.duration > one.timeOut ) {  // 4.3
				adata.OID = one.OID ; 
				adata.CID = 1 ;
				adata.abort = one.timeOut ;
				adata.delay = adata.abort - one.arrival ;
				delay = delay + adata.delay ;
				abort.push_back( adata ) ;
				current = one.timeOut ;
			} // else if
			else {  // 3.1
				if ( deal == 0 )
					current = one.arrival ;
				ddata.OID = one.OID ;
				if ( current < one.arrival )
					current = one.arrival ;
				adata.CID = 1 ; 
				ddata.departure = current + one.duration ;
				ddata.delay = current - one.arrival ;
				current = ddata.departure ;
				delay = delay + ddata.delay ;
				done.push_back( ddata ) ;
			} // else
			
			deal = deal + 1 ;
			data.pop() ;
		} // process1()
        
        void process2() {
        	abortlist adata ;
        	donelist ddata ;
        	sorted one = data2.front() ;
			if ( current2 > one.timeOut ) {  // 4.2
				adata.OID = one.OID ;
				adata.CID = 2 ; 
				adata.abort = current2 ;
				adata.delay = adata.abort - one.arrival ;
				delay = delay + adata.delay ;
				abort.push_back( adata ) ;
			} // if
			else if ( current2 + one.duration > one.timeOut ) {  // 4.3
				adata.OID = one.OID ; 
				adata.CID = 2 ; 
				adata.abort = one.timeOut ;
				adata.delay = adata.abort - one.arrival ;
				delay = delay + adata.delay ;
				abort.push_back( adata ) ;
				current2 = one.timeOut ;
			} // else if
			else {  // 3.1
				if ( deal == 0 )
					current2 = one.arrival ;
				ddata.OID = one.OID ;
				if ( current2 < one.arrival )
					current2 = one.arrival ;
				adata.CID = 2 ; 
				ddata.departure = current2 + one.duration ;
				ddata.delay = current2 - one.arrival ;
				current2 = ddata.departure ;
				delay = delay + ddata.delay ;
				done.push_back( ddata ) ;
			} // else
			
			deal = deal + 1 ;
			data2.pop() ;
		} // process2()
        
        void process() {
	    	if ( ( data.size() < 3 || data2.size() < 3 ) && curdata < size ) {  // save data(queue)
        		if ( ( data.size() == data2.size() || data.size() < data2.size() ) )  // data save in queue1
               		save( 1 ) ;
               	else  // data save in queue2
               		save2( 1 ) ;
			} // if
			else {
				if ( current < current2 )
        			process1() ;
				else if ( current > current2 )
					process2() ;
				else {
					process1() ;
					process2() ;
				} // else
			}
        	
		} // process()
		
		int main3() {
			while ( deal < size )
				process() ;
			write() ;
			return 0 ;
		} // main3
		
		void write() {  // write file
			string fullFileName = "double" + fileNum + ".txt" ;
			ofstream outFile ;
			outFile.open(fullFileName.c_str()) ;
            outFile << "\t[Abort Jobs]\n\t" << "OID\t" << "CID\t" << "Abort\t" << "Delay\n" ;
            for (int i = 0 ; i < abort.size() ; i++)
                outFile << "[" << i + 1 << "]\t" << abort[i].OID << "\t" << abort[i].CID << "\t" << abort[i].abort << "\t" << abort[i].delay << endl ;
                
            outFile << "\t[Jobs Done]\n\t" << "OID\t" << "Departure\t" << "Delay\n" ;
            for (int i = 0 ; i < done.size() ; i++)
                outFile << "[" << i + 1 << "]\t" << done[i].OID << "\t" << done[i].CID << "\t" << done[i].departure << "\t" << done[i].delay << endl ;
            float ave = delay * 1.0 / size * 1.0 ;
			outFile << "[Average Delay] " << fixed << setprecision(2) << ave << " ms" ;
			float successRate = done.size() * 100.0 / size ;
			outFile << "\n[Success Rate]  " << fixed << setprecision(2) << successRate << " %\n" ;
            outFile.close() ;
            cout << "\nThe simulation is running...\nSee " << fullFileName << endl ;
		} // write()
};

int main(void)
{   int command = 0 ;  // user command
    Mission1 mission1 ;
    Mission2 mission2 ;
    Mission3 mission3 ;
    do
    {   cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
        cout << endl << "* 0. Quit                            *" ;
        cout << endl << "* 1. Sort a file                     *" ;
        cout << endl << "* 2. Simulate one FIFO queue         *" ;
        cout << endl << "* 3. Simulate two queues by SQF      *" ;
        cout << endl << "**************************************" ;
        cout << endl << "Input a choice(0, 1, 2, 3): " ;
        cin >> command ;  // get the command
        switch (command){ 
			case 0: 
				break;
 			case 1: 
 				mission1.main1() ;
 				break;
 			case 2: 
 				mission2.load() ;
 				mission2.main2() ;
 				break;
 			case 3:
 				mission3.load() ;
 				mission3.main3() ; 
 				break ;
			default: cout << endl << "Command does not exist!" << endl;
 			
 		} // end switch
 	} while (command != 0); // '0': stop the program
 	
 	return 0;
} // end of main

