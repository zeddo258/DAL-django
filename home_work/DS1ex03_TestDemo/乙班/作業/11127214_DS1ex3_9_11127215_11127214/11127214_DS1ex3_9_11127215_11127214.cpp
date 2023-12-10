// 11127215 李宗翰 11127214 李赫宰

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>  // atoi, system
#include <iomanip>  // setw, setprecision      
#include <ctime>  // clock, CLOCKS_PER_SEC]
using namespace std;
static string uid;
typedef struct jT {  
  int OID; // order identifier
  int arrival;   // arrival time
  int duration;  // job duration
  int timeout;   // expire time
  int delay;     // wating time
  bool haveDeal;
  struct jT * next = NULL;
}   jobType;    

// #ifndef_JobList_HPP
// #define_JobList_HPP

class JobList {
  vector<jobType> aList; //     list of jobs with four columns
  string fileID;  // file identifier

  
  void showJob( jobType & ) {
  
  } // end display a job on screen 

  void sortByArrival( jobType & ) {
    
  } // sort all by arrival time(shellSort) 

  void showTime() {

  } // output time on screen

public:
  JobList() {} // constructor for initialization 

  void reset() {

    for ( int i = 0; i < aList.size() ; i++ )
      aList[i].haveDeal = false;

  } // end reset

  bool isEmpty() {
    if ( aList.size() == 0 )
      return true;
    else 
      return false;
  } // check whether it is empty or not

  int length () {
    return aList.size();
  }

  void print() {
    cout << "OID Arrival Duration       TimeOut" << endl;
    for ( int i = 0; i < aList.size(); i++ ) {
      cout << aList[i].OID << "\t" << aList[i].arrival << "\t" ;
      cout << aList[i].duration << "\t" ;
      cout << aList[i].timeout << endl;
    } // for
  } // end print

  bool getAll( string fileName )  { 
    int num;
    ifstream inputFile; // 創造物件
    inputFile.open( fileName ); // 開啟資料
	  if (!inputFile.is_open() ) {
      cerr << "### " << fileName <<  " doesn't exist! ###"   << endl; // 錯誤輸出流
      return false; // 返回錯誤碼
    }

    string title; 
    getline( inputFile, title ); // 讀掉標題
    
    // 讀取檔案內容
    while ( inputFile >> num ) {
      jobType jobtype;
      aList.push_back(jobtype);
      aList[aList.size() - 1].OID = num;
      inputFile >> num;
      aList[aList.size() - 1].arrival = num;
      inputFile >> num;
      aList[aList.size() - 1].duration = num;
      inputFile >> num;
      aList[aList.size() - 1].timeout = num;
    }

    // 關閉檔案
    inputFile.close();
    return true;  
  } // read all from a file

  void Sorted()  { 
    // 照arrival排序( shell sort )

    for ( int h = aList.size() / 2 ; h > 0 ; h = h / 2 ) {
      for ( int unsorted = h ; unsorted < aList.size() ; unsorted++ ) {
        int loc = unsorted;
        jobType nextItem = aList[unsorted];
        for ( ; ( loc >= h ) && (aList[loc-h].arrival > nextItem.arrival ) ; loc = loc - h )  {
          aList[loc] = aList[loc-h];
        }

        aList[loc] = nextItem;
      }    

    }

    // 同arrival的照OID排序

    int sameArrival = 1; // 有幾人同arrival, 自己算一個
    int firstsame = 0; // 第一個同arrival的位置
    bool notstored = true; // 是否紀錄過第一個相同的人
    for ( int i = 0 ; i < aList.size() ; i++ ) {
      for ( int j = i + 1 ; j < aList.size() - 1 ; j++ ) {
        if ( aList[i].arrival == aList[j].arrival ) {
          if ( notstored == true ) {
            firstsame = i ;
            notstored = false;
          }

          sameArrival++;   
        }
      } // for

      if ( sameArrival > 1 ) {
        // cout << sameArrival << endl << firstsame << endl;
        for ( int i = firstsame ; i < firstsame + sameArrival ; i++ ) {
          for ( int j = i + 1 ; j <= firstsame + sameArrival - 1 ; j++ ) {
            if ( aList[i].OID > aList[j].OID ) {
              jobType temp;
              temp = aList[i];
              aList[i] = aList[j];
              aList[j] = temp;
            }
              
          } // for
        }
        
        i = firstsame + sameArrival - 1 ;// 處理完的資料中位在最後一筆
        
      }
      
      notstored = true;
      firstsame = 0;
      sameArrival = 1;
    } // for
  } // sort a file( Shell sort )

  void getHeadCheck( jobType & nowjob ) {
    nowjob =  aList[0];
  } // get the now job

  void getHead( jobType & nowjob ) {
    nowjob =  aList[0];
    aList.erase( aList.begin() );
  } // get and delete the now job
  /*
  void nextJob( jobType & nextjob ) {
    aList.erase( aList.begin() );

  } // get next job and remove the job put in the function

  void nextJobCheck( jobType & nextjob ) {

    int loc;

    if ( nextjob.OID == -999 ) {
        nextjob = aList[1];
        loc = 0;
    } 
    
    else {
        for (int i = 0; i < aList.size(); i++) {
            if (nextjob.OID == aList[i].OID) {
                loc = i;
                if (i + 1 < aList.size()) {
                    nextjob = aList[i + 1];
                }
                break;
            }
        }
    }

  } // get the next job without removal
  */
  void putall() {
    ofstream outputFile;
    string fileName = "sorted" + uid + ".txt";
    outputFile.open( fileName ); // 開啟資料
	  if (!outputFile.is_open() ) {
      cerr << "### " << fileName <<  "can not create! ###"   << endl; // 錯誤輸出流
      return; // 返回錯誤碼
    }

    outputFile << "OID Arrival Duration       TimeOut" << endl;
    for ( int i = 0; i < aList.size(); i++ ) {
      outputFile << aList[i].OID << "\t" << aList[i].arrival << "\t" ;
      outputFile << aList[i].duration << "\t" ;
      outputFile << aList[i].timeout << endl;
    } // for
  } // write all as a file

  ~JobList() {
     if ( !isEmpty() )
      aList.clear();
  }
}; // end JobList

// # endif // _JobList_HPP

// #ifndef_Simulation_HPP
// #define_Simulation_HPP

// #ifndef_JobQueue_HPP
// #define_JobQueue_HPP
template<class T> 
class JobQueue {
  // T  *cA; // circular array
  typedef struct qN {
    T invalue; // 裝型別為jobType的資料
    struct qN * next;
  } queueNode;

  queueNode * head, * tail;
  int qFront, qBack; // head and tail of queue
  int qSize, qMax; // current size and max size of queue 
  /*    
  int next( int idx ) {
    自行定義
  } // move to the next position in queue
  */
public:
  int avail;  // the time when CPU is available
  JobQueue(): head(NULL), tail(NULL), qFront(0), qBack(0),
              qSize(0), qMax(0), avail(0) {} // constructor of no-space queue
  
  JobQueue<T>( int maxS ) : qFront(0), qSize(0), avail(0) 
  { // constructor of an empty queue

    /*自行定義*/

  }  // end constructor 
  
  int length() {
    queueNode * temp = head;
    int i = 0;
    while (  i < 3  && temp != NULL ) {
      i++;
      temp = temp -> next;
    }  

    //cout << "length: " << i << endl;
    return i;

  } // get the current queue length

  /*
  jobType getQueue() {
    queueNode * temp = head;
    cout << value( temp );
      temp = temp -> next;
    
  }
  */

  bool isEmpty() {
    if ( head == NULL )
      return true;
    else 
      return false;
  } // check whether it is empty

  bool isFull() { // 三個就滿
    if ( length() == 3 )  
      return true;
    else 
      return false;
    
  } // check whether it is full

  void enQueue( T & value ) {
    queueNode * newPtr = new queueNode;
    newPtr -> next = NULL; 
    newPtr -> invalue = value;
    if ( isEmpty() ) 
      head = newPtr;
    else      
      tail -> next = newPtr;
       
    tail = newPtr; // 新進的固定給tail
  } // append a new element

  void getFront( T & value ) {
    value = head -> invalue; 
  } // get the first element

  void deQueue() {
    queueNode * temp = head;
    if ( head -> next != NULL ) 
      head = head -> next;  
      
    else {
      head = NULL;
      tail = NULL;
    }

    temp -> next = NULL;
    delete temp;
  } // drop the first element 

  void deQueue( T & value ) {
    value = head -> invalue;
    queueNode * temp = head;
    if ( head -> next != NULL ) 
      head = head -> next;  
      
    else {
      head = NULL;
      tail = NULL;
    }

    temp -> next = NULL;
    delete temp;

  } // get and drop the first element 

  void clearQ() {
    while ( !isEmpty() )
      deQueue();
  } // clean up

  ~JobQueue()   {
    // destructor

    clearQ();
  } // end destructor

}; // end JobQueue

// # endif // _JobQueue_HPP

// #ifndef_AnsList_HPP
// #define_AnsList_HPP

class AnsList {
  typedef struct aT {  
  int order;
  int OID; // order identifier
  int abort ;  // arrival time
  int delay;  // job duration
}   abortType;    

  typedef struct dT {  
    int order;
    int OID; // order identifier
    int departure;   // arrival time
    int delay;
  }   doneType;  
  
  vector<abortType> abortJobs; //     list of aborted jobs with three columns
  vector<doneType> doneJobs; //     list of done jobs with three columns
  float avgDelay;  // average delay
  float successRate; // percentage of done jobs 

 
 
public:
  AnsList(): avgDelay(0), successRate(0)
  {
    /*自行定義*/
  } // constructor for initialization

  // 將答案寫成檔案

  void show( string ) {
    ofstream outputFile;
    string fileName = "output" + uid + ".txt";
    outputFile.open( fileName ); // 開啟資料
	  if (!outputFile.is_open() ) {
      cerr << "### " << fileName <<  "can not create! ###"   << endl; // 錯誤輸出流
      return; // 返回錯誤碼
    }

    // abort title
    outputFile << "       [Abort Jobs]" << endl;
    outputFile << "       OID     Abort   Delay" << endl;
    
    // out put abort
    for ( int i = 0; i < abortJobs.size(); i++ ) {
      outputFile <<  "[" << abortJobs[i].order << "]" << "\t";
      outputFile << abortJobs[i].OID << "\t";
      outputFile << abortJobs[i].abort << "\t" ;
      outputFile << abortJobs[i].delay << endl;
    } // for

    // done title

    outputFile << "       [Jobs Done]" << endl;
    outputFile << "       OID     Departure   Delay" << endl;

    //  output done

    for ( int i = 0; i < doneJobs.size(); i++ ) {
      outputFile <<  "[" << doneJobs[i].order << "]" << "\t";
      outputFile << doneJobs[i].OID << "\t";
      outputFile << doneJobs[i].departure << "\t" ;
      outputFile << doneJobs[i].delay << endl;
    } // for
    
    
    computeStat();
    outputFile << "[Average Delay] " << fixed << setprecision(2) << avgDelay << " ms" << endl;
    outputFile << "[Success Rate] " << fixed << setprecision(2) << successRate << " %" << endl;
    outputFile.close();
  } // write all as a file  

  void computeStat() {
    float sum = 0;
    for ( int i = 0; i < abortJobs.size() ; i++ )
      sum += abortJobs[i].delay;
    for ( int i = 0; i < doneJobs.size() ; i++ )
      sum += doneJobs[i].delay; 
    float all = ( abortJobs.size() + doneJobs.size() );
    avgDelay = sum / all;
    successRate = 100 * doneJobs.size() / all;
  } // compute the statistics

 
  void addAbortJob( int order, int OID, int abort, int delay ) {
    abortType abortjob;
    abortjob.order = order;
    abortjob.OID = OID;
    abortjob.abort = abort;
    abortjob.delay = delay;
    abortJobs.push_back( abortjob );
  } // add one aborted job

  void addDoneJob( int order, int OID, int departure, int delay ) {
    doneType donejob;
    donejob.order = order;
    donejob.OID = OID;
    donejob.departure = departure;
    donejob.delay = delay;
    doneJobs.push_back( donejob );
  } // add one done job
  
  void print() {
    //cout << "aSize: " << abortJobs.size();
    //cout << "dSize: "  << doneJobs.size();
    cout << "       [Abort Jobs]" << endl;
    cout << "       OID     Abort   Delay" << endl;
    for ( int i = 0; i < abortJobs.size(); i++ ) {
      cout <<  "[" << abortJobs[i].order << "]" << "\t";
      cout << abortJobs[i].OID << "\t";
      cout << abortJobs[i].abort << "\t" ;
      cout << abortJobs[i].delay << endl;
    } // for
   
    // done title

    cout << "       [Jobs Done]" << endl;
    cout << "       OID     Departure   Delay" << endl;

    //  output done

    for ( int i = 0; i < doneJobs.size(); i++ ) {
      cout <<  "[" << doneJobs[i].order << "]" << "\t";
      cout << doneJobs[i].OID << "\t";
      cout << doneJobs[i].departure << "\t" ;
      cout << doneJobs[i].delay << endl;
    } // for

    computeStat();
    cout << "[Average Delay] " << fixed << setprecision(2) << avgDelay << " ms" << endl;
    cout << "[Success Rate] " << fixed << setprecision(2) << successRate << " %" << endl;
  } // display answer on screen
  
  
  ~AnsList() {
     abortJobs.clear();
     doneJobs.clear();
  } // destructor for initialization
}; // end AnsList

// # endif // _AnsList_HPP

class Simulation {

  JobList  jobs;  // a list of jobs
  AnsList  answers;  // a set of answers
  JobQueue<jobType> jobqueue; // a set of queues

  void delQ(int) {   // delete an old job from a queue

  }

  void updateQ( int ) {  // update each queue

  }

  void finishQ() {  // finish the remaining job in queues

  }

public:
  Simulation( JobList alist ): jobs(alist) // copy constructor
  {} // end constructor

  // judge whether overtime and add to answers list
  /*
  void OvertimeOrNot( jobType job, int i  ) {
    if ( job.arrival + job.duration <= job.timeout ) {
        answers.addDoneJob( i, job.OID, job.arrival + job.duration, 0  );
        //nowTime = job.arrival + job.duration;
      }

      else {
        answers.addAbortJob( i, nowjob.OID, nowjob.timeout, 0  ); // departure == timeout
        //nowTime = nowjob.timeout;
      } 
  }
  */
  void deal() {
    jobs.reset();
    int departure; // 現在工作的完成時間
    int aOrder = 1;
    int dOrder = 1; // abortorder & doneorder
    int nowtime = 0;
    int delay; // now job delay
    bool noWait = false;
    bool haveDeleteJob = false; // 是否刪了nowjob
    bool nextIsQueue = false; // 判斷該人是否進過Queue
    bool nowIsQueue = false; // 判斷該人是否進過Queue
    jobType nextjob, nowjob, tempjob;
    //nextjob.OID = -999; // 亂數控制
    for ( int i = 1 ; !jobs.isEmpty(); i++ ) {
      
      // 判斷呼叫佇列 or joblist

      // 抓now

      // 佇列沒人
      if ( jobqueue.isEmpty() ) {
        jobs.getHead( nowjob ); // 取得並刪除第一筆
        delay = 0; 
        noWait = true; // 不用排隊
      }

      // 佇列有人
      else {
        jobqueue.getFront( nowjob );
        jobqueue.deQueue();
      }
      
    
      if ( i == 1 || nowjob.arrival > nowtime )  {
        nowtime = nowjob.arrival;
      
      }
      
      // 判斷現在工作廢除 or 完成

      if ( nowtime + nowjob.duration <= nowjob.timeout ) {
        departure = nowtime + nowjob.duration;
        if ( !noWait ) {
          delay = nowtime - nowjob.arrival;
        }



        answers.addDoneJob( dOrder, nowjob.OID, departure, delay  ); // 完成
        
        dOrder++;
      }

      else {
        if ( nowjob.timeout < nowtime ) 
          departure = nowtime;  
        else 
          departure = nowjob.timeout;
        delay = departure - nowjob.arrival;
        answers.addAbortJob( aOrder, nowjob.OID, departure, delay  ); // 廢除
        aOrder++;
      }

      // 抓next 

      // 佇列沒人
      if ( jobqueue.isEmpty() ) {
        jobs.getHeadCheck( nextjob );  
      }

      else {   
        jobqueue.getFront( nextjob );
        nextIsQueue = true;
      }
      
      // 判斷是否進入佇列

      // 比現在工作完成時間早且隊伍沒滿，就去排隊

      while ( ( nextjob.arrival < departure ) && jobs.length() > 0 && !nextIsQueue ) {
        if ( jobqueue.isFull() ) {
          if ( nextjob.OID != -100 )
            answers.addAbortJob( aOrder, nextjob.OID, nextjob.arrival, 0 );
            
          aOrder++;
        }
        // 沒滿

        else {
          jobqueue.enQueue( nextjob );
        }

        jobs.getHead( nextjob ); 
        jobs.getHeadCheck( nextjob ); 
      }
      
      
      nowtime = departure;

      nextIsQueue = false;
      nowIsQueue = false;
      haveDeleteJob = false;
      noWait = false;
    } // for

    // 把還在佇列的人丟出來

   
    while ( !jobqueue.isEmpty() ) {
      jobqueue.getFront( nowjob );

      // 判斷現在工作廢除 or 完成
      if ( nowtime + nowjob.duration <= nowjob.timeout ) {
        departure = nowtime + nowjob.duration;
        if ( !noWait ) {
          delay = nowtime - nowjob.arrival;
        }



        answers.addDoneJob( dOrder, nowjob.OID, departure, delay  ); // 完成
        
        dOrder++;
      }

      else {
        if ( nowjob.timeout < nowtime ) 
          departure = nowtime;  
        else 
          departure = nowjob.timeout;
          delay = departure - nowjob.arrival;
        answers.addAbortJob( aOrder, nowjob.OID, departure, delay  ); // 廢除
        aOrder++;
      }

      nowtime = departure;
      jobqueue.deQueue();
    }

    answers.show( uid );
  } // Deal

  ~Simulation() // destructor
  {
    /*自行完成定義*/
  } // end destructor 

}; //class SImulation

int main() {
	
  char command = '0';
  do {
	  string fileName = "\0";
    JobList joblist;
    AnsList anslist;
	  cout << endl << "**** Simulate FIFO Queues by SQF ****";
	  cout << endl << "* 0. Quit                           *";
	  cout << endl << "* 1. Sort a file                    *";
	  cout << endl << "* 2. Simulate one FIFO queue        *";
    cout << endl << "*************************************";
	  cout << endl << "Input a command( 0, 1, 2 ): ";
  	cin >> command ;
    if ( command == '1' ) {
	    cout << endl << "input a file number: ";
	    cin >> uid;
      fileName = "input" + uid + ".txt";
      clock_t start1 = clock();
      if ( !joblist.getAll( fileName ) ) 
        continue;
      clock_t end1 = clock();
      int read = end1 - start1;
      double readms = end1 - start1;

      joblist.print();

      clock_t start2 = clock();
      joblist.Sorted();
      clock_t end2 = clock();
      int sort = end2 - start2;
      double sortms = end2 - start2;

      clock_t start3 = clock();
      joblist.putall();
      clock_t end3 = clock();
      int write =  end3 - start3;
      double writems =  end3 - start3;

      cout << endl << "Reading data: " << read << " clocks (" ;
      cout << setprecision(2) << fixed << readms << " ms)." << endl;
      cout << "Sorting data: " << sort << " clocks (" ;
      cout << setprecision(2) << fixed << sortms << " ms)." << endl;
      cout << "Writing data: " << write << " clocks (" ;
      cout << setprecision(2) << fixed << writems << " ms)." << endl;

      cout << endl << "See sorted" << uid << ".txt" << endl ;
    } // if
	  
    
    else if ( command == '2' ) {
      fileName = "sorted" + uid + ".txt";
      joblist.getAll( fileName );
      Simulation simulation( joblist );
      // joblist.print();
      simulation.deal(); 
      cout << endl << "The simulation is running..." << endl;
      cout << "See output" << uid << ".txt" << endl;
    }  
    
    else if (command == '0' ) 
	    break;
    
    else 
	    cout << endl << "Command does not exist" << endl;	
    
	} while(true);
	
	return 0;
}
