// 11127142 范庭郡 ， 11127162 陳祈恩
#include<iostream>
#include<fstream>
#include<istream>
#include<iomanip>
#include<vector>
#include<ctime>

using namespace std;
struct JobList {
    int OID; // OID
    int arrival; // arrival
    int duration; // duration
    int Timeout; // Timeout
};

struct Abort { // 取消清單 
    int OID;
    int abort;
    int Delay;
    int Time;
};

struct Done { // 成功清單 
    int OID;
    int Depature;
    int Delay;
    int Time;
};



class Job {
public:
    vector<JobList> List; // The ADT that used to sorted
    string number; // The code number of the file.

    Job() { // Constructer
    }

    bool Load() { // Load a text file.
        clock_t start , end;// start time 
        start = clock();
        fstream myFile;
        myFile.open("input"+number+".txt", ios::in | ios :: out); // opend file
        if ( myFile.is_open() ) {
            string line;
            int item;
            getline(myFile,line); // Load the first line which does not used.
            int n = 0;
            int pos = -1; // Position
            while (myFile >> line){ // Load word file.
                item = Number(line);
                switch (n)
                {
                    case 0:
                        pos++;
                        List.push_back(JobList());
                        StoreOID(item, pos);
                        break;
                    case 1:
                        StoreArrival(item,pos);
                        break;
                    case 2:
                        StoreDuration(item,pos);
                        break;
                    case 3:
                        StoreTimeOut(item,pos);
                        break;
                }

                n = ( n + 1 ) % 4;
            }


            end = clock(); // end time
            double ReadTime = double(end)- double(start);
            FileOnScreen();
            cout << "Loading data: " << int(ReadTime) << " clocks (" << fixed << setprecision(2) << ReadTime << " ms)." << endl;
            myFile.close(); // close file
            return true;
        }

        else
            return false;
    }

    int Number( string number ) { // Turn the string into number.
        int total = 0;
        for ( int i = 0; i < number.length() ; i++ ) {
            total = total * 10 + ( number[i] - '0' );
        }

        return total;
    }

    void StoreOID( int OID, int pos ) { // To store the OID.
        List[pos].OID = OID;
    }

    void StoreArrival( int arrival, int pos ) { // To store the arrival.
        List[pos].arrival = arrival;
    }

    void StoreDuration( int duration, int pos ) { // To store the duration.
        List[pos].duration = duration;
    }

    void StoreTimeOut( int Timeout, int pos ) { // To store the Timeout.
        List[pos].Timeout = Timeout;
    }

    void ShellSort() {
        clock_t startSort, endSort;
        startSort = clock();
        int n = List.size(); // get size of List
        for (int h = n / 2; h > 0; h = h / 2) {
            for (int unsort = h; unsort < n; unsort++) {
                int tempOID = List[unsort].OID;
                int tempArrival = List[unsort].arrival;
                int tempDuration = List[unsort].duration;
                int tempTimeout = List[unsort].Timeout;

                int j;
                for ( j = unsort; j >= h && List[j - h].arrival > tempArrival ; j -= h) {
                    List[j].OID = List[j - h].OID;
                    List[j].arrival = List[j - h].arrival;
                    List[j].duration = List[j - h].duration;
                    List[j].Timeout = List[j - h].Timeout;


                } // for

                List[j].OID = tempOID;
                List[j].arrival = tempArrival;
                List[j].duration = tempDuration;
                List[j].Timeout = tempTimeout;
                for ( int k = 0; k < n - 1 ; k++ ) {

                    if ( List[k].arrival == List[k+1].arrival ) { // becuase Shell sort is unstable，so need to sort again 

                        if ( List[k].OID > List[k+1].OID ) {
                            tempOID = List[k].OID;
                            tempArrival = List[k].arrival;
                            tempDuration = List[k].duration;
                            tempTimeout = List[k].Timeout;
                            List[k].OID = List[k+1].OID;
                            List[k].arrival = List[k+1].arrival;
                            List[k].duration = List[k+1].duration;
                            List[k].Timeout = List[k+1].Timeout;
                            List[k+1].OID = tempOID;
                            List[k+1].arrival = tempArrival;
                            List[k+1].duration = tempDuration;
                            List[k+1].Timeout = tempTimeout;
                        } // if

                    } // if

                } // for
            } // for
        } // for

        endSort = clock();
        double elapsedSort = endSort - startSort;
        cout << "Sorting data: " << int(elapsedSort) << " clocks (" << fixed << setprecision(2) << elapsedSort<< " ms)." << endl;
    } // shellSort

    void Output() { // Output to the sorted file
        clock_t  startSort, endSort; // start time
        startSort = clock();
        ofstream ofs;
        ofs.open("sorted"+number+".txt");
        ofs << "OID\tArrival\tDuration\tTimeOut"<<"\n";
        for ( int i = 0 ; i < List.size(); i++  ) {
            ofs << List[i].OID << "\t" << List[i].arrival << "\t" << List[i].duration << "\t" << List[i].Timeout << "\n";
        }

        endSort = clock(); // end of time 
        double elapsedWrite = endSort - startSort;
        cout << "Writing data: " << int(elapsedWrite) << " clocks (" << fixed << setprecision(2) << elapsedWrite<< " ms)." << endl;
        ofs.close(); // close file

    } // Output

    void FileOnScreen() { // print the sorted file on screen  
        fstream File;
        File.open("sorted"+number+".txt", ios::in | ios :: out); // open file
        string line;
        int i = 0;
        while ( getline(File, line) ) {
            if ( i == 0 ) {
                cout << "\t"<< line << endl;
            } // if
            else {
                cout << "(" << i << ")" << "\t"<< line << endl;
            } // else

            i++;
        } // while

        cout << endl;
        File.close(); // close file
        return;

    } // FileOnScreen

    string formatTime(double elapsed ) { //時間格式化 
        return to_string(elapsed);
    } // formateTime

    ~Job() {

    }
}; // end of Job

class Queue { // methods of queue 
private:
    int TimeOut = 0;

public: // Constructors and Destructor
    bool isFirst = true;
    vector<JobList> list;
    // Queue Operations
    void SetTimeOut( int n ) { // set current time
        TimeOut = n;
    }

    int getTimeOut() { // get current time
        return TimeOut;
    }

    bool isEmpty() { // check the queue is empty
        if ( list.empty() ) {
            return true;
        } // if

        return false;
    } // isEmpty

    void enqueue( JobList temp ) { // enter into queue
        list.push_back( temp );
    } // enqueue

    void dequeue() { // delet front of queue
        list.erase( list.begin() );
    } // dequeue
    void dequeue( JobList &temp  ) { // to delete the fornt of queue and get front
        temp.arrival = list[0].arrival;
        temp.duration = list[0].duration;
        temp.OID = list[0].OID;
        temp.Timeout = list[0].Timeout;
        list.erase(list.begin());
    } // dequeue

    void getFront( JobList &temp ) { // to get the front of queue
        temp.arrival = list[0].arrival;
        temp.duration = list[0].duration;
        temp.OID = list[0].OID;
        temp.Timeout = list[0].Timeout;
    } // getFront

};

class Process : public Job{
private:
    vector<Queue> Cpu;

    int NumOfCpu  = 1; // if more cpu
public:
    vector<Abort> cancel;
    vector<Done> success;
    Queue q;
    JobList work;
    Abort abt;
    Done done;

    void Load() {
        fstream myFile;
        myFile.open("sorted" + number + ".txt",
                    ios::in | ios::out);
        if ( myFile.is_open() ) {
            string line;
            int item;
            getline(myFile, line); // Load the first line which does not used.
            int n = 0;
            int pos = -1; // Position
            while (myFile >> item) { // Load word file.
                switch (n) {
                    case 0:
                        pos++;
                        List.push_back(JobList());
                        StoreOID(item, pos);
                        break;
                    case 1:
                        StoreArrival(item, pos);
                        break;
                    case 2:
                        StoreDuration(item, pos);
                        break;
                    case 3:
                        StoreTimeOut(item, pos);
                        break;
                }

                n = (n + 1) % 4;
            }
            myFile.close();
        }
    }

    void process () {
        Load();
        for ( int i = 0 ; i < NumOfCpu ; i++ ) // The amount of Cpu
            Cpu.push_back(q); // sort the queue

       

        for ( int i = 0 ; i < List.size() ; i++ ) { // Run every OID in List
            for ( int j = 0 ; j < NumOfCpu; j++ ) { // To check which cpu is using.
                if ( Cpu[j].list.size() == 4 ) { // If the Cpu is Full
                    Cpu[j].getFront(work);
                    if ( List[i].arrival != work.arrival ) // 同時只能進4筆資料 
                        ProcessInQueue(j,i);
                   
                    else { 
                        abt.OID = List[i].OID;
                        abt.abort = List[i].arrival;
                        abt.Delay = 0;
                        abt.Time = Cpu[j].getTimeOut();
                        cancel.push_back(abt);
                    } // else
                    
                    break;
                } // if
                else if ( Cpu[j].list.size() < 4 ) {  // If the Cpu is  not Full.
                    
                    Cpu[j].enqueue( List[i] );

                    break;
                } // else if
                
            } // if

        } // for

        for (int i = 0 ; i < NumOfCpu ; i++ )
           dequeueAll( i );
        output();
    } // process


    void ProcessInQueue( int current,int index ) {
        Cpu[current].getFront(work); // the front of queue
        int TimeOut = Cpu[current].getTimeOut(); // get current Timeout
        if ( TimeOut < List[index].arrival  && work.arrival + work.duration > List[index].arrival ) { // 加進來的arrival小於TimeOut，不能進queue 
            abt.OID = List[index].OID;
            abt.abort = List[index].arrival;
            abt.Delay = 0;
            abt.Time = TimeOut;
            cancel.push_back(abt);
            return;
        } // if

        else  { // 加進來的arrival大於TimeOut

            if ( Cpu[current].isFirst ) { // 設定第一個進來queue的時間 

                if ( work.Timeout > work.arrival + work.duration )
                    Cpu[current].SetTimeOut(work.arrival + work.duration );
                else
                    Cpu[current].SetTimeOut(work.Timeout );

                done.OID=work.OID;
                done.Depature=Cpu[current].getTimeOut();;
                done.Delay = 0;
                done.Time = done.Depature;
                success.push_back( done );
                Cpu[current].dequeue();
				if ( List.size() == 1 ) { // 如果只有一筆資料 
					return;
				}
				else
                	Cpu[current].enqueue(List[index]);
                	
                Cpu[current].isFirst=false;  
            } // if

            else { 
                if (  work.Timeout < TimeOut + work.duration ) { // 出去cpu時間小於現在時間加處理時間 
                    if ( TimeOut <= work.Timeout )
                        Cpu[current].SetTimeOut(work.Timeout );
                    abt.OID = work.OID;
                    abt.abort =  Cpu[current].getTimeOut() ;
                    abt.Delay = abt.abort - work.arrival;
                    abt.Time = Cpu[current].getTimeOut();
                    cancel.push_back(abt);
                    Cpu[current].dequeue(); // 刪除queue 
                    Cpu[current].enqueue(List[index]); // 加入下一筆資料 
                } // if

                else {                                      // queue裡可以處裡 
                    done.OID = work.OID;
                    if ( work.arrival + work.duration < work.Timeout ) { // 如果出去cpu時間小於出去cpu時間+抵達時間
                        done.Depature = TimeOut + work.duration;
                    }
                    else {
                        done.Depature = work.Timeout;
                    }
                    Cpu[current].SetTimeOut(done.Depature ); 
                    if ( TimeOut < work.arrival ) //現在時間小於抵達時間
                        done.Delay = 0;
                    else
                        done.Delay = TimeOut - work.arrival ;
                    done.Time = Cpu[current].getTimeOut();
                    success.push_back(done);
                    Cpu[current].dequeue();// 刪除queue 
                    Cpu[current].enqueue(List[index]);  // 加入下一筆資料  
                } // else

            } // else

        } // else if

    } // ProcessInQueue

    void dequeueAll( int current ) { // 把queue裡的資料清除 

        if ( Cpu[current].list.size() == 0 )
            return;

        int TimeOut = Cpu[current].getTimeOut();
        Cpu[current].getFront(work);
        if (  work.Timeout < TimeOut + work.duration ) { // 出去cpu時間小於現在時間加處理時間 
            if ( TimeOut <= work.Timeout )
                Cpu[current].SetTimeOut(work.Timeout );
            abt.OID = work.OID;
            abt.abort =  Cpu[current].getTimeOut() ;
            abt.Delay = abt.abort - work.arrival;

            abt.Time = Cpu[current].getTimeOut();
            cancel.push_back(abt);
            Cpu[current].dequeue(); // 刪除queue 

        } // if

        else {                                      //出去cpu時間大於現在時間加處理時間
            done.OID = work.OID;
            if ( work.arrival + work.duration < work.Timeout ) { // 如果出去cpu時間小於出去cpu時間+抵達時間 
                done.Depature = TimeOut + work.duration; //離開時間=現在時間加處理時間 
            }
            else { 
                done.Depature = work.Timeout;//離開時間 = 出去cpu時間
            }
            Cpu[current].SetTimeOut(done.Depature ); 
            if ( TimeOut < work.arrival ) //現在時間小於抵達時間
                done.Delay = 0;
            else
                done.Delay = TimeOut - work.arrival ;
            done.Time = Cpu[current].getTimeOut();
            success.push_back(done);
            Cpu[current].dequeue();// 刪除queue 

        } // else

        dequeueAll(current);
    } // dequeueAll()

    double Delay() { // 計算延遲率 
        double total = 0;
        for (int i = 0 ; i < cancel.size(); i++)
            total = total + cancel[i].Delay;
        for ( int i = 0 ; i < success.size() ; i++)
            total = total + success[i].Delay;
        return total/(List.size());

    }

    double  Success() { // 計算成功率 

        double ggg = double (success.size())/double(List.size());
        return 100*ggg;
    }

    void output() { // Output to the sorted file
        ofstream ofs;
        ofs.open("output"+number+".txt");
        ofs << "\t[Abort Jobs]"<<"\n"<<"\tOID\tAbort\tDelay" << "\n";
        for ( int i = 0 ; i < cancel.size(); i++  ) {
            ofs <<"["<<i+1<<"]";
            if ( i < 9)
                ofs<<"\t";
            else
                ofs<<"\t";
            ofs<<cancel[i].OID <<"\t"<<cancel[i].abort << "\t" << cancel[i].Delay <<"\n";
        }

        ofs << "\t[Jobs Done]"<<"\n"<<"\tOID\tDeparture\tDelay" << "\n";
        for ( int i = 0 ; i < success.size(); i++  ) {
            ofs <<"["<<i+1<<"]";
            if ( i < 9)
                ofs<<"\t";
            else
                ofs<<"\t";
            ofs<<success[i].OID<<"\t"<<success[i].Depature << "\t" << success[i].Delay <<"\n";
        }

        ofs<<"[Average Delay]\t"<< fixed << setprecision(2) << Delay()<<"ms\n";
        ofs<<"[Success Rate]\t"<< fixed << setprecision(2) << Success()<<"%"<<endl;
        ofs.close();

    } // Output

};

int main() {
    int command = 0;
    string number ;
    do {
        cout << endl;
        cout << "**** Simulate FIFO Queues by SQF *****\n";
        cout << "* 0. Quit                            *\n";
        cout << "* 1. Sort a file                     *\n";
        cout << "* 2. Simulate one FIFO queue         *\n";
        cout << "**************************************\n";
        cout << "Input a command(0, 1, 2):";
        cin >> command;
        if ( command == 1 || command == 2) {
            Job job = Job();
            Process process;
            if ( command == 1 ) {
                cout << "\nInput a file number :";
                cin >> number;
                job.number = number;
                if ( job.Load() ){
                    job.ShellSort();
                    job.Output();
                    cout << endl;
                    cout << "See the sorted"+job.number+".txt!"<<endl;
                }

                else {
                    cout << "input"+job.number+".txt is not exist! ###" <<endl;
                }
            }
            else if ( command == 2 ) {
                process.number = number;
                process.process();
                cout << "seeing the output"+number+".txt!" << endl;
            }
        }
    } while( command != 0 );

} // main()
