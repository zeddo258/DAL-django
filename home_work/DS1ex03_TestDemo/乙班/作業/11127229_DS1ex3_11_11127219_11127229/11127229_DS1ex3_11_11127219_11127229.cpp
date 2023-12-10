//********************************************************************************
// group11: 11127219 高泓鈞, 11127229 廖翊崴
//********************************************************************************

# include <stdio.h>
# include <stdlib.h>
# include <iostream> // cout, endl
# include <fstream> // open, is_open
# include <string> // string
# include <vector> // vector
# include <cstdlib> // atoi, system
# include <iomanip> // setw, setprecision
# include <ctime> // clock, CLOCKS_PER

using namespace std ;

struct jobType {

    int OID ; // order identifier
    int arrival ; // arrival time
    int duration ; // job duration
    int timeout ; // expire time

};


class JobList { // input job list by reading files, then sort the jobs, and output a sorted job list.

    vector <jobType> aList ; // list of jobs with four columns
    string fileID ; // file identifier
    double readtime ;
    double sorttime ;
    double writetime ;
    bool nofile ;

    void init() { // definition: initial set up

        nofile = false ;
        cout << "\nInput a file number: " ;
        cin >> fileID ;
        string full ;
        full = "input" + fileID + ".txt" ;
        string a = "" ;
        jobType temp ;

        clock_t readstart = clock() ;
        ifstream inputfile( full ) ;

        if ( !inputfile ) {
            cout << "\n### " << full << " does not exist! ###" << endl ;
            nofile = true ;
        }

        else {

            for ( int i = 0 ; i < 4 ; i++ )
                inputfile >> a ; // collecting garbage

            while ( inputfile >> temp.OID >> temp.arrival >> temp.duration >> temp.timeout )
                aList.push_back( temp ) ;

            // cout << "\nsize of list:" << aList.size() << endl ;
            inputfile.close() ;
            clock_t readend = clock() ;
            readtime = ( readend - readstart ) / static_cast<double>( CLOCKS_PER_SEC ) * 1000 ;

        }

    }

    void showJob() {

        cout << "\n\tOID\tArrival\tDuration\tTimeOut" << endl ;

        for ( int i = 0 ; i < aList.size() ; i++ ) {

            cout << "(" << i + 1 << ")\t" ;
            cout << aList.at(i).OID << "\t" << aList.at(i).arrival << "\t" << aList.at(i).duration << "\t" << aList.at(i).timeout << endl ;

        }

    } // declaration: display input job on screen

    void sortAll() {

        clock_t sortstart = clock() ;

        int n = aList.size() ;

        for ( int gap = n / 2 ; gap > 0 ; gap /= 2 ) {

            for ( int i = gap ; i < n ; ++i ) {

                jobType temp = aList[i] ;
                int j = 0 ;

                for ( j = i ; j >= gap && !comparejob( aList[j-gap], temp ) ; j -= gap )
                    aList[j] = aList[j-gap] ;

                aList[j] = temp ;
            }
        }

        clock_t sortend = clock() ;
        sorttime = ( sortend - sortstart ) / static_cast<double>( CLOCKS_PER_SEC ) * 1000 ;

    } // sort the aList in the data members

    void putAll() {

        string full = "sorted" + fileID + ".txt" ;

        clock_t writestart = clock() ;

        ofstream outputFile( full ) ;

        if ( outputFile.is_open() ) {

            outputFile << "OID\tArrival\tDuration\tTimeOut\n" ;
            for ( const auto & item : aList )
                outputFile << item.OID << "\t" << item.arrival << "\t" << item.duration << "\t" << item.timeout << "\n" ;

            outputFile.close() ;

        } else
            cerr << "Error opening the output file\n" ;

        clock_t writeend = clock() ;

        writetime = ( writeend - writestart ) / static_cast<double>( CLOCKS_PER_SEC ) * 1000 ;

    } // declaration: write all as a file

    void showTime() {

        cout << '\n' ;
        cout << "Reading data: " << setprecision (2) << fixed << writetime << " ms " << endl ;
        cout << "Sorting data: " << setprecision (2) << fixed << sorttime << " ms " << endl ;
        cout << "Writing data: " << setprecision (2) << fixed << writetime << " ms " << endl << endl ;
        cout << "See sorted" << fileID << ".txt" << endl ;

    } // declaration: output time on screen

    bool comparejob( const jobType & x, const jobType & y ) {

        return ( x.arrival < y.arrival ) || ( x.arrival == y.arrival && x.OID < y.OID ) ;

    }

public:

    JobList() {

        init() ;
        if ( nofile == false ) {

            showJob() ;
            sortAll() ;
            putAll() ;
            showTime() ;
        }

    } // constructor for initialization

    ~JobList() {

        aList.clear() ;

    } // destructor for initialization

}; //end JobList


struct Node {
    int OID ; // order identifier
    int arrival ; // arrival time
    int duration ; // job duration
    int timeout ; // expire time
    Node * next ;
};

typedef Node * NodePtr ;


class Queue {

     NodePtr tail = NULL ;

public:

    void enqueue( int o, int a, int d, int t ) {
        NodePtr temp = NULL ;
        if ( tail == NULL ) {
            tail = new Node ;
            temp = tail ;
            temp -> OID = o ;
            temp -> arrival = a ;
            temp -> duration = d ;
            temp -> timeout = t ;
            temp -> next = NULL ;
        }
        else {
            temp = new Node ;
            temp -> OID = o ;
            temp -> arrival = a ;
            temp -> duration = d ;
            temp -> timeout = t ;
            temp -> next = tail ;
            tail = temp ;
        }
    }

    void dequeue() {
        NodePtr temp1 = tail, temp2 = tail -> next ;
        while ( 1 ) {
            if ( temp2 == NULL ) {
                free( temp1 ) ;
                tail = NULL ;
                return ;
            }
            else if ( temp2 -> next == NULL ) {
                temp1 -> next = NULL ;
                free( temp2 ) ;
                return ;
            }

            temp1 = temp1 -> next ;
            temp2 = temp2 -> next ;
        }
    }

    void getFront( int & o, int & a, int & d, int & t ) {
        NodePtr temp = tail ;
        while ( 1 ) {
            if ( temp -> next == NULL ) {
                o = temp -> OID ;
                a = temp -> arrival ;
                d = temp -> duration ;
                t = temp -> timeout ;
                return ;
            }

            temp = temp -> next ;
        }
    }

    bool isEmpty() {
        if ( tail == NULL ) return true ;
        return false ;
    }

    int getSize() {
        int len = 0 ;
        NodePtr temp = tail ;
        while ( temp != NULL ) {
            len++ ;
            temp = temp -> next ;
        }

        return len ;
    }
};


struct abortType {
    int OID ;
    int abort ;
    int delay ;
};

struct doneType {
    int OID ;
    int departure ;
    int delay ;
};


class Simulation {
    vector <jobType> aRegister ;
    vector <abortType> abortList ;
    vector <doneType> doneList ;
    Queue q ;
    bool nofile = false ;
    int time = 0 ;
    string inputID ;
    abortType a_temp ;
    doneType d_temp ;
    int temp_oid = 0, temp_arr = 0, temp_dur = 0, temp_tim = 0 ;
    double average_delay = 0.0, success_rate = 0.0 ;

    void readFile() {
        string fileID = "", file_name = "" ;
        cout << "\nInput a file number: " ;
        cin >> fileID ;
        file_name = "sorted" + fileID + ".txt" ;
        jobType j_temp ;

        ifstream inputfile( file_name ) ;

        if ( !inputfile ) {
            cout << "\n### " << file_name << " does not exist! ###" << endl ;
            nofile = true ;
        }
        else {
            string a = "" ;
            for ( int i = 0 ; i < 4 ; i++ )
                inputfile >> a ; // collecting garbage

            while ( inputfile >> j_temp.OID >> j_temp.arrival >> j_temp.duration >> j_temp.timeout )
                aRegister.push_back( j_temp ) ;

            inputfile.close() ;
            inputID = fileID ;
        }
    }

    void process() {
        for ( int i = 0 ; i < aRegister.size() ; i++ ) {
            if ( q.getSize() == 0 && time + aRegister.at(i).duration <= aRegister.at(i).timeout ) { // done
                if ( time == 0 ) { // first
                    d_temp.delay = 0 ;
                    time = aRegister.at(i).arrival + aRegister.at(i).duration ;
                }
                else {
                    d_temp.delay = time - aRegister.at(i).arrival ;
                    if ( d_temp.delay < 0 ) {
                        time = aRegister.at(i).timeout ;
                        d_temp.delay = 0 ;
                    }
                    else {
                        time = time + aRegister.at(i).duration ;
                    }
                }

                d_temp.OID = aRegister.at(i).OID ;
                d_temp.departure = time ;
                doneList.push_back( d_temp ) ;
            }
            else if ( aRegister.at(i).arrival >= time ) { // time not come
                while ( !q.isEmpty() ) {
                    q.getFront( temp_oid, temp_arr, temp_dur, temp_tim ) ;
                    q.dequeue() ;

                    // go to done or abort
                    if ( time + temp_dur < temp_tim ) { // done
                        d_temp.delay = time - temp_arr ;
                        time = time + temp_dur ;
                        d_temp.OID = temp_oid ;
                        d_temp.departure = time ;
                        doneList.push_back( d_temp ) ;
                    }
                    else if ( time > temp_tim ) { // abort
                        a_temp.OID = temp_oid ;
                        a_temp.abort = time ;
                        a_temp.delay = a_temp.abort - temp_arr ;
                        abortList.push_back( a_temp ) ;
                    }
                    else if ( time + temp_dur > temp_tim ) { // abort
                        time = temp_tim ;
                        a_temp.OID = temp_oid ;
                        a_temp.abort = time ;
                        a_temp.delay = a_temp.abort - temp_arr ;
                        abortList.push_back( a_temp ) ;
                    }
                }

                i-- ;
            }
            else if ( q.getSize() < 3 && aRegister.at(i).arrival <= time ) { // to queue
                q.enqueue( aRegister.at(i).OID, aRegister.at(i).arrival, aRegister.at(i).duration, aRegister.at(i).timeout ) ;
            }
            else if ( q.getSize() == 3 && aRegister.at(i).arrival == time ) { // to queue but queue full, out one, in one
                q.getFront( temp_oid, temp_arr, temp_dur, temp_tim ) ;
                q.dequeue() ;
                q.enqueue( aRegister.at(i).OID, aRegister.at(i).arrival, aRegister.at(i).duration, aRegister.at(i).timeout ) ;

                // go to done or abort
                if ( time + temp_dur < temp_tim ) { // done
                    d_temp.delay = time - temp_arr ;
                    time = time + temp_dur ;
                    d_temp.OID = temp_oid ;
                    d_temp.departure = time ;
                    doneList.push_back( d_temp ) ;
                }
                else if ( time > temp_tim ) { // abort
                    a_temp.OID = temp_oid ;
                    a_temp.abort = time ;
                    a_temp.delay = a_temp.abort - temp_arr ;
                    abortList.push_back( a_temp ) ;
                }
                else if ( time + temp_dur > temp_tim ) { // abort
                    time = temp_tim ;
                    a_temp.OID = temp_oid ;
                    a_temp.abort = time ;
                    a_temp.delay = a_temp.abort - temp_arr ;
                    abortList.push_back( a_temp ) ;
                }
            }
            else { // to abort
                a_temp.OID = aRegister.at(i).OID ;
                a_temp.abort = aRegister.at(i).arrival ;
                a_temp.delay = 0 ;
                abortList.push_back( a_temp ) ;
            }
        }

        while ( !q.isEmpty() ) {
            q.getFront( temp_oid, temp_arr, temp_dur, temp_tim ) ;
            q.dequeue() ;

            // go to done or abort
            if ( time + temp_dur < temp_tim ) { // done
                d_temp.delay = time - temp_arr ;
                time = time + temp_dur ;
                d_temp.OID = temp_oid ;
                d_temp.departure = time ;
                doneList.push_back( d_temp ) ;
            }
            else if ( time > temp_tim ) { // abort
                a_temp.OID = temp_oid ;
                a_temp.abort = time ;
                a_temp.delay = a_temp.abort - temp_arr ;
                abortList.push_back( a_temp ) ;
            }
            else if ( time + temp_dur > temp_tim ) { // abort
                time = temp_tim ;
                a_temp.OID = temp_oid ;
                a_temp.abort = time ;
                a_temp.delay = a_temp.abort - temp_arr ;
                abortList.push_back( a_temp ) ;
            }
        }
    }

    void countData() {
        double delay_sum = 0.0 ;

        for ( int i = 0 ; i < abortList.size() ; i++ )
            delay_sum = delay_sum + abortList.at(i).delay ;

        for ( int i = 0 ; i < doneList.size() ; i++ )
            delay_sum = delay_sum + doneList.at(i).delay ;

        average_delay = delay_sum / aRegister.size() ;
        double done_num = doneList.size() ;
        success_rate = done_num * 100 / aRegister.size() ;
    }

    void writeFile() {

        string full = "output" + inputID + ".txt" ;

        ofstream outputFile( full ) ;

        if ( outputFile.is_open() ) {

            outputFile << "\t[Abort Jobs]\n\tOID\tAbort\tDelay\n" ;
            for ( int i = 0 ; i < abortList.size() ; i++ )
                outputFile << "["<< i + 1 << "]\t" << abortList.at(i).OID << "\t" << abortList.at(i).abort << "\t" << abortList.at(i).delay << "\n" ;

            outputFile << "\t[Jobs Done]\n\tOID\tDeparture\tDelay\n" ;
            for ( int i = 0 ; i < doneList.size() ; i++ )
                outputFile << "["<< i + 1 << "]\t" << doneList.at(i).OID << "\t" << doneList.at(i).departure << "\t" << doneList.at(i).delay << "\n" ;


            outputFile << "[Average Delay] " << setprecision(2) << fixed << average_delay << " ms" << endl ;
            outputFile << "[Success Rate]	" << setprecision(2) << fixed << success_rate << " %" << endl ;

            outputFile.close() ;

        }
        else
            cerr << "Error opening the output file\n" ;
    }

public:

    Simulation() {
        readFile() ;
        if ( nofile == false ) {
            process() ;
            countData() ;
            string full = "output" + inputID + ".txt" ;
            cout << endl ;
            cout << "The simulation is running...\nSee " << full << endl ;
            writeFile() ;
        }
    }

    ~Simulation() {
        aRegister.clear() ;
        abortList.clear() ;
        doneList.clear() ;
    }
};


int main() {

    int command = 0 ;

    do {

        cout << "**** Simulate FIFO Queues by SQF *****" << endl ;
        cout << "* 0. Quit                            *" << endl ;
        cout << "* 1. Sort a file                     *" << endl ;
        cout << "* 2. Simulate one FIFO queue         *" << endl ;
        cout << "**************************************" << endl ;
        cout << "Input a command(0, 1, 2): " ;
        cin >> command ;

        if( command != 0 ) {

            if ( command == 1 ) {

                JobList mission1 ;

            }
            else if ( command == 2 ) {

                Simulation mission2 ;

            }
            else
                cout << endl << "Command does not exist!" << endl ;
        }

        cout << '\n' ;

    } while ( command != 0 ) ;

    // system("pause"); // pause the display
    return 0 ;

}
