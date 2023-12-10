// 11127127 陳沛君 11127145 陳采妮
// mission 3 test

# include <iostream>
# include <string.h>
# include <vector>
# include <fstream>
# include <time.h>
# include <iomanip>

using namespace std ;


/** ======================= my function ======================= **/

ifstream in;
ofstream out;

struct jobType {

  int oid ;       // order identifier
  int arrival ;   // arrival time
  int duration ;  // job duration
  int timeout ;   // expire time

};

/** ======================= class JobList ======================= **/

class JobList {

  private:

    vector<jobType> aList ;  // list of jobs
    string fileID ;
    int mission ;

    double readingTime = 0 ;
    double sortingTime = 0 ;
    double writingTime = 0 ;


  public:

    // ---------------- isEmpty() ----------------

    // check whether it is empty or not, if yes, return true; else, return false.
    bool isEmpty() {
      if ( aList.size() == 0 )
        return true ;

      return false ;
    } // isEmpty()

    // ---------------- getAll() ----------------

    // read all from a file
    bool getAll( string fileID, int mission ) {

      double start, ends ;  // starting time, ending time

      this -> fileID = fileID ;
      this -> mission = mission ;
      string fileName ;

      if ( mission == 1 )
        fileName = "input" + fileID + ".txt" ;
      else
        fileName = "sorted" + fileID + ".txt" ;

      start = clock() ; // record the starting time

      // open the .txt file
      in.open( fileName );

      // check if the file is opened successfully
      if ( !in.is_open() ){

        cout << "### " << fileName << " does not exist! ###" << endl;
        return false;

      } // if

      string title ;
      getline(in, title) ;

      jobType temp ;

      while(!in.eof()){

    	in >> temp.oid;
    	in >> temp.arrival;
    	in >> temp.duration;
    	in >> temp.timeout;

    	if (in.fail())
    	  break ;

    	aList.push_back( temp ) ;

      } // while

      in.close() ;

      ends = clock() ; // record the ending time

      readingTime = (ends - start) / CLOCKS_PER_SEC * 1000.0 ;

      return true ;

    } // getAll()

    // ---------------- printList() ----------------

    // print all the jobs in the list
    void printList() {

      cout << "\tOID\tArrival\tDuration\tTimeOut" << endl;

      for ( int i = 0 ; i < aList.size() ; i++ ) {

        cout << "(" << i+1 << ")\t" ;
        cout << aList.at(i).oid << "\t" ;
        cout << aList.at(i).arrival << "\t" ;
        cout << aList.at(i).duration << "\t" ;
        cout << aList.at(i).timeout << endl ;

      } // for

      cout << endl ;

    } // printList()

    // ---------------- showTime() ----------------

    // output time on screen
    void showTime() {

      cout << "Reading data: " << (int)readingTime << " clocks" ;
      cout << " (" << fixed << setprecision(2) << readingTime << " ms)." << endl ;
      cout << "sorting data: " << (int)sortingTime << " clocks" ;
      cout << " (" << fixed << setprecision(2) << sortingTime << " ms)." << endl ;
      cout << "writing data: " << (int)writingTime << " clocks" ;
      cout << " (" << fixed << setprecision(2) << writingTime << " ms)." << endl ;

      cout << endl ;

    } // showTime()

    // ---------------- sortList() ----------------

    // sort the list by arrival time
    void sortList() {

      double start, ends ;  // starting time, ending time
      start = clock() ; // record the starting time

      // sorting
      for ( int h = aList.size()/2 ; h > 0 ; h = h/2 ) {

	    for ( int unsorted = h ; unsorted < aList.size() ; unsorted++ ) {

	      int loc = unsorted ;
	      jobType nextItem = aList.at(unsorted) ;

	      for ( ; (loc >= h) && (aList.at(loc-h).arrival >= nextItem.arrival) ; loc = loc - h ) {
	        if ( aList.at(loc-h).arrival == nextItem.arrival &&
	             aList.at(loc-h).oid < nextItem.oid ) {
	          break;
	        } // if
	        else {
	          aList.at(loc).oid = aList.at(loc-h).oid ;
	          aList.at(loc).arrival = aList.at(loc-h).arrival ;
	          aList.at(loc).duration = aList.at(loc-h).duration ;
	          aList.at(loc).timeout = aList.at(loc-h).timeout ;
	        } // else
	      }

	      aList.at(loc).oid = nextItem.oid ;
	      aList.at(loc).arrival = nextItem.arrival ;
	      aList.at(loc).duration = nextItem.duration ;
	      aList.at(loc).timeout = nextItem.timeout ;

        } // for

	  } // for

      ends = clock() ; // record the ending time
      sortingTime = (ends - start) / CLOCKS_PER_SEC * 1000.0 ;

    } // sortList()

    // ---------------- putAll() ----------------

    // write all as a file
    void putAll( int mission ) {

      double start, ends ;

      string fileName ;

      if ( mission == 1 )
        fileName = "sorted" + fileID + ".txt" ;
      else
        fileName = "output" + fileID + ".txt" ;

      start = clock() ;

      out.open( fileName );

      if (!out.is_open()) {
        cout << "Failed to open file.\n";
        return ;
      } // if


      out << "OID\tArrival\tDuration\tTimeOut" << endl;

      for ( int i = 0 ; i < aList.size() ; i++ ) {

        out << aList.at(i).oid << "\t" ;
        out << aList.at(i).arrival << "\t" ;
        out << aList.at(i).duration << "\t" ;
        out << aList.at(i).timeout << endl ;

      } // for

      cout << "See " << fileName << endl ;

      out.close();


      ends = clock() ; // record the ending time
      writingTime = (ends - start) / CLOCKS_PER_SEC * 1000.0 ;

    } // putAll()

    // ---------------- getHead() ----------------

    // get the head of the list and delete it from the list
    jobType getHead() {

      jobType returnVal = aList.at(0) ;
      aList.erase( aList.begin( ) ) ;
      return returnVal ;

    } // printDone()


};

/** ======================= class AnsList ======================= **/

class AnsList {

  struct abortType {

    int oid ;       // order identifier
    int CID ;     // CPU ID
    int abort ;   // abort time
    int delay ;  // delay

  };

  struct doneType {

    int oid ;       // order identifier
    int CID ;     // CPU ID
    int departure ;  // departure time
    int delay ;  // delay

  };

  vector<abortType> abortJobs ;
  vector<doneType> doneJobs ;

  float sumDelay ;
  float avgDelay ;
  float successRate ;

  // ---------------- UD_AvgDelay() ----------------

  // update aveDelay
  void UD_AvgDelay( int delay ) {

    sumDelay = sumDelay + delay ;

    if ( ( abortJobs.size() + doneJobs.size() ) != 0 )
      avgDelay = sumDelay / ( (float)abortJobs.size() + (float)doneJobs.size() ) ;

  } // UD_AvgDelay()

  // ---------------- UD_AvgDelay() ----------------

  // update aveDelay
  void UD_successRate() {

    if ( ( abortJobs.size() + doneJobs.size() ) != 0 )
      successRate = (float)doneJobs.size() / ( (float)abortJobs.size() + (float)doneJobs.size() ) * 100 ;

  } // UD_AvgDelay()

  public:

    AnsList() {

      sumDelay = 0.0 ;
      avgDelay = 0.0 ;
      successRate = 0.0 ;

    } // Simulation()

    // ---------------- printAbort() ----------------

    void printAbort() {

      cout << "\t[Abort List]" << endl ;
      cout << "\tOID\tAbort\tDelay" << endl;

      for ( int i = 0 ; i < abortJobs.size() ; i++ ) {

        cout << "[" << i+1 << "]\t" ;
        cout << abortJobs.at(i).oid << "\t" ;
        cout << abortJobs.at(i).abort << "\t" ;
        cout << abortJobs.at(i).delay << endl ;

      } // for

      cout << endl ;

    } // printAbort()

    // ---------------- printDone() ----------------

    void printDone() {

      cout << "\t[Done List]" << endl ;
      cout << "\tOID\tDeparture\tDelay" << endl;

      for ( int i = 0 ; i < doneJobs.size() ; i++ ) {

        cout << "[" << i+1 << "]\t" ;
        cout << doneJobs.at(i).oid << "\t" ;
        cout << doneJobs.at(i).departure << "\t" ;
        cout << doneJobs.at(i).delay << endl ;

      } // for

      cout << endl ;

    } // printDone()

    // ---------------- printAvgDelay() ----------------

    void printAvgDelay() {

      cout << "[Average Delay]\t" << fixed  << setprecision(2) << avgDelay << " ms" << endl ;

    } // printAvgDelay()

    // ---------------- printSuccessRate() ----------------

    void printSuccessRate() {

      cout << "[success Rate]\t" << fixed  << setprecision(2) << successRate << " %" << endl ;

    } // printSuccessRate()

    // ---------------- storeInDone() ----------------

    void storeInDone( jobType job, int depTime ) {

      doneType temp ;
      temp.oid = job.oid ;
      temp.departure = depTime ;
      temp.delay = depTime - (job.arrival + job.duration) ;

      doneJobs.push_back( temp ) ;

      UD_AvgDelay( temp.delay ) ;
      UD_successRate() ;

    } // storeInDone()

    // ---------------- printDone() ----------------

    void storeInAbort( jobType job, int abortTime ) {

      abortType temp ;
      temp.oid = job.oid ;
      temp.abort = abortTime ;
      temp.delay = abortTime - job.arrival ;

      abortJobs.push_back( temp ) ;

      UD_AvgDelay( temp.delay ) ;
      UD_successRate() ;

    } // printDone()

    // ---------------- storeInDone() ----------------

    void storeInDone_mul( jobType job, int depTime, int id ) {

      doneType temp ;
      temp.oid = job.oid ;
      temp.CID = id ;
      temp.departure = depTime ;
      temp.delay = depTime - (job.arrival + job.duration) ;

      doneJobs.push_back( temp ) ;

      UD_AvgDelay( temp.delay ) ;
      UD_successRate() ;

    } // storeInDone()

    // ---------------- printDone() ----------------

    void storeInAbort_mul( jobType job, int abortTime, int id  ) {

      abortType temp ;
      temp.oid = job.oid ;
      temp.CID = id ;
      temp.abort = abortTime ;
      temp.delay = abortTime - job.arrival ;

      abortJobs.push_back( temp ) ;

      UD_AvgDelay( temp.delay ) ;
      UD_successRate() ;

    } // printDone()

    // ---------------- putAll() ----------------

    void putAll( string fileID ) {

      string fileName ;
      fileName = "output" + fileID + ".txt" ;

      out.open( fileName );

      if (!out.is_open()) {
        cout << "Failed to open file.\n";
        return ;
      } // if

      // out abort jobs
      out << "\t[Abort Jobs]" << endl;
      out << "\tOID\tAbort\tDelay" << endl;

      for ( int i = 0 ; i < abortJobs.size() ; i++ ) {

        out << "[" << i+1 << "]\t" ;
        out << abortJobs.at(i).oid << "\t" ;
        out << abortJobs.at(i).abort << "\t" ;
        out << abortJobs.at(i).delay << endl ;

      } // for

      // out done jobs
      out << "\t[Jobs Done]" << endl;
      out << "\tOID\tDeparture\tDelay" << endl;

      for ( int i = 0 ; i < doneJobs.size() ; i++ ) {

        out << "[" << i+1 << "]\t" ;
        out << doneJobs.at(i).oid << "\t" ;
        out << doneJobs.at(i).departure << "\t" ;
        out << doneJobs.at(i).delay << endl ;

      } // for

      // out avgDelay & successRate
      out << "[Average Delay]\t" << fixed  << setprecision(2) << avgDelay << " ms" << endl ;
      out << "[Success Rate]\t" << fixed  << setprecision(2) << successRate << " %" << endl ;

      out.close();

      cout << "See " << fileName << endl ;

    } // putAll()


    // ---------------- putAll() ----------------

    void putAll_mul( string fileID ) {

      string fileName ;
      fileName = "doubled" + fileID + ".txt" ;

      out.open( fileName );

      if (!out.is_open()) {
        cout << "Failed to open file.\n";
        return ;
      } // if

      // out abort jobs
      out << "\t[Abort Jobs]" << endl;
      out << "\tOID\tCID\tAbort\tDelay" << endl;

      for ( int i = 0 ; i < abortJobs.size() ; i++ ) {

        out << "[" << i+1 << "]\t" ;
        out << abortJobs.at(i).oid << "\t" ;
        out << abortJobs.at(i).CID << "\t" ;
        out << abortJobs.at(i).abort << "\t" ;
        out << abortJobs.at(i).delay << endl ;

      } // for

      // out done jobs
      out << "\t[Jobs Done]" << endl;
      out << "\tOID\tCID\tDeparture\tDelay" << endl;

      for ( int i = 0 ; i < doneJobs.size() ; i++ ) {

        out << "[" << i+1 << "]\t" ;
        out << doneJobs.at(i).oid << "\t" ;
        out << doneJobs.at(i).CID << "\t" ;
        out << doneJobs.at(i).departure << "\t" ;
        out << doneJobs.at(i).delay << endl ;

      } // for

      // out avgDelay & successRate
      out << "[Average Delay]\t" << fixed  << setprecision(2) << avgDelay << " ms" << endl ;
      out << "[Success Rate]\t" << fixed  << setprecision(2) << successRate << " %" << endl ;

      out.close();

      cout << "See " << fileName << endl ;

    } // putAll()


};

/** ======================= class JobQueue ======================= **/

class JobQueue {

  vector<jobType> qJob ;
  int qMax ;
  int depTime ;

  public:

    JobQueue() {
      depTime = 0 ;
    } ;

    // ---------------- setMax() ----------------

    void setMax( int num ) {

      this -> qMax = num ;

    } // setMax()

    // ---------------- isEmpty() ----------------

    bool isEmpty() {

      if ( qJob.size() == 0 )
        return true ;

      return false ;

    } // isEmpty()

    // ---------------- enqueue() ----------------

    void enqueue( jobType job ) {

      qJob.push_back( job ) ;

    } // enqueue()

    // ---------------- enqueue() ----------------

    jobType dequeue() {

      jobType returnVal = qJob.at(0) ;
      qJob.erase( qJob.begin( ) ) ;
      return returnVal ;

    } // enqueue()

    // ---------------- printQ() ----------------

    void printQ() {

      for( int i = 0 ; i < qJob.size() ; i++ ) {
        cout << qJob.at(i).oid << "\t" ;
        cout << qJob.at(i).arrival << "\t" ;
        cout << qJob.at(i).duration << "\t" ;
        cout << qJob.at(i).timeout << endl ;

      } // for

      cout << endl ;

    } // printQ()

    // ---------------- getQSize() ----------------

    // get the size of the queue
    int getQSize() {

      return qJob.size() ;

    } // getQSize()

    // ---------------- getQSize() ----------------

    // get the maximum size of the queue(qMax)
    int getQMax() {

      return qMax ;

    } // getQSize()

    // ---------------- getDepTime() ----------------

    // get the depTime
    int getDepTime() {

      return depTime ;

    } // getDepTime()

    // ---------------- setDepTime() ----------------

    // set the depTime
    void setDepTime( int num ) {

      depTime = num ;

    } // setDepTime()

};

/** ======================= class Simulation ======================= **/

class Simulation {

  private:

    JobList jobs ;  // a list of jobs
    AnsList answers ;  // a set of answers
    JobQueue **allQ ;  // a set of queues
    int qNum ;  // number of queues (for multiple queues question use)
                // mission 2: 1 queue, mission 3: n queues(maybe...)

    // ---------------- printJobs() ----------------

    void printJobs() {

      jobs.printList() ;

    } // printJobs()

    // ---------------- printAns() ----------------

    void printAns() {

      answers.printAbort() ;
      answers.printDone() ;

    } // printAns()

    // ---------------- printQs() ----------------

    void printQs() {

      for ( int i = 0 ; i < qNum ; i++ ) {

        cout << "Queue-" << i+1 << "(" << allQ[i][0].getDepTime() << "):" << endl;
        allQ[i][0].printQ() ;

      } // for

    } // printAns()

  // ==================================================

  public:

    // ---------------- Simulation() ----------------

    // jobs:
    // qNum: number of queues
    Simulation( JobList & jobs, int qNum ) {

      this -> jobs = jobs ;
      this -> qNum = qNum ;

      allQ = new JobQueue*[qNum] ;
      for( int i = 0 ; i < qNum ; i++ ){
        allQ[i] = new JobQueue[1]() ;
        allQ[i][0].setMax( 3 ) ;
      } // for

    } // Simulation()
    // ---------------- runSimulation() ----------------

    void runSL() {

      int depTime = 0 ;

      cout << "The simulation is running..." << endl ;

      while( !jobs.isEmpty() ) {

        jobType temp = jobs.getHead() ;

        // arrival time 小於 depTime
        if ( temp.arrival < depTime ) {

          // 丟進Queue
          // 查看queue大小是否大於3
          if ( allQ[0][0].getQSize() < allQ[0][0].getQMax() )
            allQ[0][0].enqueue( temp ) ;
          else {
            // queue is filled, put job into abort list
            answers.storeInAbort( temp, temp.arrival ) ;
          } // else

        } // if
        else{

          // 存進Done
          if ( allQ[0][0].isEmpty() ) {
            depTime = temp.arrival + temp.duration ;
            answers.storeInDone( temp, depTime ) ;
          } // if
          else {
            /// ==================================================================

            // process jobs in the queue sequentially until the queue is empty
            while( !allQ[0][0].isEmpty() && depTime <= temp.arrival ) {

              jobType temp2 = allQ[0][0].dequeue() ;
              int temp_depTime = depTime + temp2.duration ;

              // check the job is timeout or not
              if ( temp_depTime <= temp2.timeout ){
                depTime = temp_depTime ;
                answers.storeInDone( temp2, depTime ) ;
              } // if
              else{
                if ( temp2.timeout > depTime )
                  depTime = temp2.timeout ;

                answers.storeInAbort( temp2, depTime ) ;
              } // else

            } // while
            /// ==================================================================

            // process the job(temp)
            if ( temp.arrival < depTime ) {
              allQ[0][0].enqueue( temp ) ;
            } // if
            else {
              int temp_depTime = temp.arrival + temp.duration ;

              // check the job is timeout or not
              if ( temp_depTime <= temp.timeout ) {
                depTime = temp_depTime ;
                answers.storeInDone( temp, depTime ) ;
              } // if
              else {
                if ( temp.timeout > depTime )
                  depTime = temp.timeout ;

                answers.storeInAbort( temp, depTime ) ;
              } // else

            } // else

          } // else

        } // else

      } // for

      while( !allQ[0][0].isEmpty() ) {

        jobType temp2 = allQ[0][0].dequeue() ;
        int temp_depTime = depTime + temp2.duration ;

        if ( temp_depTime <= temp2.timeout ) {
          depTime = temp_depTime ;
          answers.storeInDone( temp2, depTime ) ;
        } // if
        else {
          if ( temp2.timeout > depTime )
            depTime = temp2.timeout ;

          answers.storeInAbort( temp2, depTime ) ;
        } // else

      } // while

    } // runSL()

    /// ---------------- runSL() ----------------

    void runSL_mul() {

      cout << "The simulation is running..." << endl ;


      while( !jobs.isEmpty() ) {

        jobType aJob = jobs.getHead() ;
        bool isStored = false ;
        bool isAllFilled = true ;
        bool isAJobProcessed = true ;
        int cpu_id = 0 ;

        for( int i = 0 ; i < qNum ; i++ ) {

          // aJob has not been stored
          if ( !isStored ) {

            // aJob's arrival >= the depTime of the queue
            if ( aJob.arrival >= allQ[i][0].getDepTime() ) {

              // if the queue is empty, then store in done
              if ( allQ[i][0].isEmpty() ) {
                isStored = true ;
                allQ[i][0].setDepTime( aJob.arrival + aJob.duration ) ;
                answers.storeInDone_mul( aJob, allQ[i][0].getDepTime(), i + 1 ) ;
              } // if
              // if the queue is not empty, then process jobs in the queue sequentially until the queue is empty
              else {

                // process jobs in the queue sequentially until the queue is empty
                while( !allQ[i][0].isEmpty() && allQ[i][0].getDepTime() <= aJob.arrival ) {

                  jobType temp = allQ[i][0].dequeue() ;
                  int temp_depTime = allQ[i][0].getDepTime() + temp.duration ;

                  // check the job is timeout or not
                  if ( temp_depTime <= temp.timeout ){
                    allQ[i][0].setDepTime( temp_depTime ) ;
                    answers.storeInDone_mul( temp, allQ[i][0].getDepTime(), i + 1 ) ;
                  } // if
                  else{
                    if ( temp.timeout > allQ[i][0].getDepTime() )
                      allQ[i][0].setDepTime( temp.timeout ) ;

                    answers.storeInAbort_mul( temp, allQ[i][0].getDepTime(), i + 1 ) ;
                  } // else

                } // while

                isAJobProcessed = false ;
                isStored = true ;
                cpu_id = i ;


              } // else

            } // if( aJob.arrival >= allQ[i][0].getDepTime )
            // aJob's arrival < the depTime of the queue
            else {

              // check if the queue is filled !!
              if ( allQ[i][0].getQSize() < allQ[i][0].getQMax() )
                isAllFilled = false ;

            } // else

          } // if( !isStored )
          // aJob has been stored
          else {

            if ( aJob.arrival >= allQ[i][0].getDepTime() ) {

              // process jobs in the queue sequentially until the queue is empty
              while( !allQ[i][0].isEmpty() && allQ[i][0].getDepTime() <= aJob.arrival ) {

                jobType temp = allQ[i][0].dequeue() ;
                int temp_depTime = allQ[i][0].getDepTime() + temp.duration ;

                // check the job is timeout or not
                if ( temp_depTime <= temp.timeout ){
                  allQ[i][0].setDepTime( temp_depTime ) ;
                  answers.storeInDone_mul( temp, allQ[i][0].getDepTime(), i + 1 ) ;
                } // if
                else{
                  if ( temp.timeout > allQ[i][0].getDepTime() )
                    allQ[i][0].setDepTime( temp.timeout ) ;

                  answers.storeInAbort_mul( temp, allQ[i][0].getDepTime(), i + 1 ) ;
                } // else

              } // while

            } // if

          } // else

        } // for( int i = 0 ; i < qNum ; i++ )


        if ( !isAJobProcessed ) {

            if ( aJob.arrival >= allQ[cpu_id][0].getDepTime() ) {
                  int temp_depTime = aJob.arrival + aJob.duration ;

                  // check the job is timeout or not
                  if ( temp_depTime <= aJob.timeout ) {
                    allQ[cpu_id][0].setDepTime( temp_depTime ) ;
                    answers.storeInDone_mul( aJob, allQ[cpu_id][0].getDepTime(), cpu_id + 1 ) ;
                  } // if
                  else {
                    if ( aJob.timeout > allQ[cpu_id][0].getDepTime() )
                      allQ[cpu_id][0].setDepTime( aJob.timeout ) ;

                    answers.storeInAbort_mul( aJob, allQ[cpu_id][0].getDepTime(), cpu_id + 1 ) ;
                  } // else

                  isStored = true ;

                } // if
                else {

                  // allQ[i][0].enqueue( aJob ) ;
                  isStored = false ;

                  if ( allQ[cpu_id][0].getQSize() < allQ[cpu_id][0].getQMax() )
                    isAllFilled = false ;

                } // else

        } // if

        // all queues are filled
        if ( (!isStored) && isAllFilled ) {
          // put aJob into abort list
          answers.storeInAbort_mul( aJob, aJob.arrival, 0 ) ;
        } // if
        else if ( !isStored ) {
          int formerSize = 0 ;
          for ( int i = 0 ; i < qNum && !isStored ; i++ ) {

            if ( allQ[i][0].getQSize() == 0 ||
                 allQ[i][0].getQSize() < formerSize ||
                 qNum == 1 ) {

              if ( allQ[i][0].getDepTime() <= aJob.arrival ){
                allQ[i][0].setDepTime( aJob.arrival + aJob.duration ) ;
                answers.storeInDone_mul( aJob, allQ[i][0].getDepTime(), i + 1 ) ;

              } // if
              else
                allQ[i][0].enqueue( aJob ) ;
              isStored = true ;

            } // if

            formerSize = allQ[i][0].getQSize() ;

          } // for

          if ( !isStored ) {

            allQ[0][0].enqueue( aJob ) ;
            isStored = true ;

          } // if

        } // else if

      } // while

      // process jobs in all queues
      for ( int i = 0 ; i < qNum; i++ ) {

        // process jobs in the queue sequentially until the queue is empty
        while( !allQ[i][0].isEmpty() ) {

          jobType temp = allQ[i][0].dequeue() ;
          int temp_depTime = allQ[i][0].getDepTime() + temp.duration ;

          // check the job is timeout or not
          if ( temp_depTime <= temp.timeout ){
            allQ[i][0].setDepTime( temp_depTime ) ;
            answers.storeInDone_mul( temp, allQ[i][0].getDepTime(), i + 1 ) ;
          } // if
          else{
            if ( temp.timeout > allQ[i][0].getDepTime() )
              allQ[i][0].setDepTime( temp.timeout ) ;

            answers.storeInAbort_mul( temp, allQ[i][0].getDepTime(), i + 1 ) ;
          } // else

        } // while

      } // for

    } // runSL_mul()

    /// ====================================================================

    // ---------------- clrQ() ----------------

    // clear the queue
    void clrQ() {

      // clear the second layer first
      for ( int i = 0; i < qNum; i++ )
        delete[] allQ[i];

      // then clear the first layer
      delete[] allQ ;

    } // clr()

    // ---------------- putAll() ----------------

    void putAll( string fileID, int mission ) {

      if ( mission == 2 )
        answers.putAll( fileID ) ;
      else
        answers.putAll_mul( fileID ) ;

    } // putAll()

};

/** ======================= main ======================= **/

int main() {

  int command = 0 ;
  int fileID = -1 ;

  do {

    JobList jobList ;

    cout << endl ;
    cout << "**** Simulate FIFO Queues by SQF *****" << endl ;
    cout << "* 0. Quit                            *" << endl ;
    cout << "* 1. Sort a file                     *" << endl ;
    cout << "* 2. Simulate one FIFO queue         *" << endl ;
    cout << "* 3. Simulate multiple FIFO queues   *" << endl ;
    cout << "**************************************" << endl ;
    cout << "Input a command(0, 1, 2, 3): " ;

    // input a command
    cin >> command ;
    cout << endl ;

    // quit if the command is 0
    if ( command == 0 )
      break ;

    /// mission 1
    if ( command == 1 ) {

      // input a fileID
      cout << "Input a file number: " ;
      cin >> fileID ;
      cout << endl ;

      bool success = jobList.getAll( to_string(fileID), 1 ) ;
      if ( success ) {
        jobList.printList() ;
        jobList.sortList() ;
        jobList.showTime() ;
        jobList.putAll( 1 ) ;
      } // if

    } // if
    /// mission 2
    else if ( command == 2 ) {

      if ( fileID == -1 ){
        // input a fileID
        cout << "Input a file number: " ;
        cin >> fileID ;
        cout << endl ;
      } // if

      bool success = jobList.getAll( to_string(fileID), 2 ) ;

      if ( success ) {

        Simulation SL( jobList, 1 ) ;
        SL.runSL() ;
        SL.putAll( to_string(fileID), 2 ) ;
        SL.clrQ() ;

      } // if
      else {
        fileID = -1 ;
      } // else

    } // else if
    else if( command == 3 ) {

      if ( fileID == -1 ){
        // input a fileID
        cout << "Input a file number: " ;
        cin >> fileID ;
        cout << endl ;
      } // if

      bool success = jobList.getAll( to_string(fileID), 2 ) ;

      if ( success ) {

        Simulation SL( jobList, 2 ) ;
        SL.runSL_mul() ;
        SL.putAll( to_string(fileID), 3 ) ;
        SL.clrQ() ;

      } // if
      else {
        fileID = -1 ;
      } // else

    } // else if
    else {
      cout << "Command does not exist!" << endl ;
    } // else


  } while( true ) ;

  return 0 ;


} // main()
