/*********************
 學號 10924148  10731126 
 姓名 陳黃揚	陳冠吾 
*********************/ 

#include <stdio.h>
#include <string.h>  // string class
#include <time.h>
#include <windows.h>

#include <cstdlib>  // system, atoi
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>  // string class
#include <vector>

using namespace std;

// unclassed part ↓

string InttoString(int num) {
    stringstream ss;
    ss << num;
    string out_string = ss.str();
    return out_string;

}  // ItoS()

fstream file;
string firstline;  // 第一行也就是OID Arrival 那行 
bool doNotFile = false; // 檔案是否已經建檔 
int readS, readE ;
int sortS, sortE ;
int writeS, writeE ;
string fNum = "-1";

void findFile(int num) {  
    string add = InttoString(num);
    fNum = add;
    string ss;
    ss = "input" + add;
    ss = ss + ".txt";
    // cout << "ss: " << ss << endl ;
    file.open(ss.c_str(), ios::in);

    if (!file)
        cout << "### " << ss << " does not exist! ###" << endl;
    else{
        doNotFile = true;
    }
}  // FindFile()

class Queue {
  private:
    vector <int> oid;
    vector <int> arr;  // arrival 
    vector <int> dur;  // duration
    vector <int> time; // timeout
    vector <int> delay; 
    vector <int> aod; // abort or departure; 
    vector <int> cid;


  public:
    bool isEmpty();
    void enqueue( int Oid, int Arr, int Dur, int Time );
    void enqueue( int Oid, int AOD, int Delay ); // oid, abort or departure, delay
    void enqueueT3( int Oid, int Cid, int AOD, int Delay );
    void dequeue( int &Oid, int &AOD, int &Delay );
    void dequeue( int &Oid, int &Arr, int &Dur, int &Time );
    void dequeueT3( int &Oid, int &Cid, int &AOD, int &Delay );
    void getfront( int &Oid, int &Arr, int &Dur, int &Time ); 
    void printAll( int i );
    int getLength();
    float getTotalD();
    void clearList();

	void setQueue() {
		readS = clock();
		
		getline( file, firstline) ;
		int i = 1 ;
		
		int Oid, Arr, Dur, Time;
		cout << endl ;
		cout << "\t" <<"OID" << "\t" << "Arrival" << "\t" << "Duration" << "\t" << "TimeOut" << endl;
	    while (file >> Oid >> Arr >> Dur >> Time) {
	        // 逐個加進對列裡面
	        enqueue(Oid, Arr, Dur, Time);
	
	        // debug
	        
			cout << "(" << i << ")\t" << Oid << "\t" << Arr << "\t" << Dur << "\t" << Time << endl;
	        i++;
	    }
	    cout << endl ;
	    readE = clock() ;
	}
	
	void shellSort(){
		sortS = clock() ;
		int n = oid.size() ;
		for( int gap = n/2;gap > 0; gap /= 2 ){
			for( int i = gap; i < n; i += 1 ){
				int tempA = arr[i];
				int tempO = oid[i];
				int tempD = dur[i];
				int tempT = time[i];
				
				int j;
				
				for (j = i; j >= gap && (arr[j - gap] > tempA || (arr[j - gap] == tempA && oid[j - gap] > tempO)); j -= gap) {
	                arr[j] = arr[j-gap];
	                oid[j] = oid[j-gap];
	                dur[j] = dur[j-gap];
	                time[j] = time[j-gap];
            	}  // for
            	
            	arr[j] = tempA;
            	oid[j] = tempO;
            	dur[j] = tempD;
            	time[j] = tempT;
			}
		}
		
		sortE = clock();
	}
	
	void saveFile(int num ) {
	//  SortingSTART = clock();
		writeS = clock();
		string add = InttoString( num ) ;
	    string fileName = "sorted" + add + ".txt" ;
	
	    int i;
	    fstream newFile;
	    newFile.open(fileName.c_str(), ios::out | ios::trunc);
	
	    newFile << "OID\tArrival\tDuration\tTimeOut\n";
	
	    int n = oid.size();
	
	    for (i = 0; i < n; i++) {
	        newFile << oid[i] << "\t";
	        newFile << arr[i] << "\t";
	        newFile << dur[i] << "\t";
	        newFile << time[i] << "\n";
	    }
	
	    newFile.close();
		
	//  SortingEND = clock();
		writeE = clock();
	}  // saveFile()

	
	
};

  void Queue::clearList() {
    oid.clear();
    arr.clear();
    dur.clear();
    time.clear();
    delay.clear();
    aod.clear();
    return;
  }

  float Queue::getTotalD() { // 取得總延遲
    float total = 0;
    for ( int i = 0 ; i < delay.size() ; i++ ) {
      total += delay.at(i);
    }
    return total;
  }

  void Queue::dequeue( int &Oid, int &AOD, int &Delay ) { // 取出工作(於判定工作完成及被丟棄時使用)
    if ( !isEmpty() ) {
      Oid = oid.front();
      AOD = aod.front();
      Delay = delay.front();
      oid.erase( oid.begin() );
      aod.erase( aod.begin() );
      delay.erase( delay.begin() );

    }
    return;
  }
  
  void Queue::dequeueT3( int &Oid, int &Cid, int &AOD, int &Delay ) { // two cores
    if ( !isEmpty() ) {
      Oid = oid.front();
      Cid = cid.front();
      AOD = aod.front();
      Delay = delay.front();
      oid.erase( oid.begin() );
      cid.erase( cid.begin() );  
      aod.erase( aod.begin() );
      delay.erase( delay.begin() );

    }
    return;
  }  

  void Queue::dequeue( int &Oid, int &Arr, int &Dur, int &Time ) { // 取出工作(於判斷工作階段使用)
    if ( !isEmpty() ) {
      Oid = oid.front();
      Arr = arr.front();
      Dur = dur.front();
      Time = time.front();
      oid.erase( oid.begin() );
      arr.erase( arr.begin() );
      dur.erase( dur.begin() );
      time.erase( time.begin() );

    }
  }

  bool Queue::isEmpty() { // 確認queue是否為空的
    if ( oid.empty() )
      return true;
    else 
      return false;
  }

  void Queue::getfront( int &Oid, int &Arr, int &Dur, int &Time ) { // 看第一筆資料(不丟出queue)
    if ( !isEmpty() ) {
      Oid = oid.front();
      Arr = arr.front();
      Dur = dur.front();
      Time = time.front();
    }
    return; 
  }

  int Queue::getLength() { // 取得資料長度(以oid為準)
    return oid.size();
  }
  
  void Queue::enqueueT3( int Oid, int Cid, int AOD, int Delay ) { // two cores
    oid.push_back( Oid );
    cid.push_back( Cid );
    aod.push_back( AOD );
    delay.push_back( Delay );    
  }  

  void Queue::enqueue( int Oid, int AOD, int Delay ) { // 工作完成或是被丟棄時用
    oid.push_back( Oid );
    aod.push_back( AOD );
    delay.push_back( Delay );    
  }

  void Queue::enqueue( int Oid, int Arr, int Dur, int Time ) { // 將工作列表讀入時用
    oid.push_back( Oid );
    arr.push_back( Arr );
    dur.push_back( Dur );
    time.push_back( Time );
  }

  void Queue::printAll( int i ) { // debu用 可忽略
    if ( i == 1 ) {
      cout << "oid\tarr\tdur\ttime\n";
      for ( int i = 0; i < oid.size(); i++ ) {
        cout << oid.at(i) << "\t" << arr.at(i) << "\t" << dur.at(i) << "\t" << time.at(i) << endl;
      }
    }
    else if ( i == 2 ) {
      cout << "oid\tAOD\tDelay\n";
      for ( int i = 0; i < oid.size(); i++ ) {
        cout << oid.at(i) << "\t" << aod.at(i) << "\t" << delay.at(i) << endl;
      }
    }
    
    else if ( i == 3 ) {
      cout << "oid\tcoreID\tAOD\tDelay\n";
      for ( int i = 0; i < oid.size(); i++ ) {
        cout << oid.at(i) << "\t" << cid.at(i) << "\t" << aod.at(i) << "\t" << delay.at(i) << endl;
      }
    	
	}
  }



class test2 {
  private:
    ifstream in;
    ofstream out;
    Queue list;
    Queue abort;
    Queue done;
    Queue working;
    string file;
    string fileNum;
    string c;
    int oid;
    int arr;
    int aod;
    int dur;
    int delay;
    int time;
    int timer;
    int useTime;
    float aveDelay;
    float sucRate;

  public:
    bool getFile();
    void test();
    void workListing();
    void processing( bool &thisTime, bool &isWork );
    void calculate( float &aveDelay, float &sucRate );
    void outputF();
    void clear();
  
};

  void test2::test() {
    timer = 0;
    aveDelay = 0;
    sucRate = 0;
    bool isEnd = false;
    bool isWork = false;
    bool wasSet = true;
    bool thisTime = false;
    useTime = 0; // timer + dur 用來記目前工作處理時間需多久

      list.getfront( oid, arr, dur, time ); //list為排列好的工作列表
      workListing();                        //檢查time = 0時是否有工作可做
    while ( !isEnd ) {
      
      thisTime = false;
      processing( thisTime, isWork );

      list.getfront( oid, arr, dur, time ); //看看處理完目前工作後，該時刻是否有其他工作抵達
      workListing();
      thisTime = false;
      processing( thisTime, isWork );

      if ( list.isEmpty() && working.isEmpty() ) // 若列表為空，正要進行的工作queue也都空了，表示全做完
        isEnd = true;

      timer++;
      
    }

    calculate( aveDelay, sucRate );

    return;
  }

  void test2::calculate( float &aveDelay, float &sucRate ) { // 計算平均延遲及成功率
    aveDelay = 0;
    sucRate = 0;
    aveDelay += done.getTotalD();
    aveDelay += abort.getTotalD();

    aveDelay = (float)aveDelay / ( (float)done.getLength() + (float)abort.getLength() );
    sucRate = (float)done.getLength() / ( (float)done.getLength() + (float)abort.getLength() );
    return;
  }

  void test2::processing( bool &thisTime, bool &isWork ) { // 處理資料是否完成或逾期
      while ( !thisTime ) { // thisTime表示這個時刻是否該做的都做完了
        if ( !working.isEmpty() )
          working.getfront( oid, arr, dur, time ); // 若工作列表不為空，先看第一筆資料

        if ( !isWork && !working.isEmpty() && useTime == 0 ) { // 如果工作列表有工作且當前沒在工作
          isWork = true;                       // 開始工作( 設定完成工作的時間 )
          useTime = timer + dur;
        }
    
      
        if ( timer == useTime && !working.isEmpty() ) { // if done work
          working.dequeue( oid, arr, dur, time );
          done.enqueue( oid, timer, ( timer - arr - dur ) );
          useTime = 0;
          isWork = false;
        }
        else if ( timer >= time && !working.isEmpty() ) { // 如果timeout
          working.dequeue( oid, arr, dur, time );
          abort.enqueue( oid, timer, ( timer - arr ) );
          isWork = false;
          useTime = 0;
        }
        else { // 其他代表此時刻的工作都已完成
          thisTime = true;
        }
       
        if ( !isWork && !working.isEmpty() && useTime == 0 ) { // 如果工作列表有工作且當前沒在工作(若上面有完成OR丟掉)
          working.getfront( oid, arr, dur, time );
          
          isWork = true;                       // 開始工作( 設定完成工作的時間 )
          useTime = timer + dur;
        }

        
      }
    return;
  }

  void test2::workListing( ) { // 處理資料是否放入等待區或是丟棄區
    bool end = false;
    while ( !list.isEmpty() && !end ) {
      list.getfront( oid, arr, dur, time ); //取第一筆資料
      
      if ( timer == arr ) { // 若當前時間剛好為抵達時間
        
        list.dequeue( oid, arr, dur, time ); //將資料取出並做處理
        
        if ( working.getLength() <= 3 ) { // 第一筆為正在處裡的資料 (若工作queue還沒滿則放入queue中)
          working.enqueue( oid, arr, dur, time );
          
        }
        else if ( timer == useTime && working.getLength() <= 4 ) { // 0為正好結束的 1為正要處理的 因此queue中的是2~4
          working.enqueue( oid, arr, dur, time );
        }
        else { // 若queue滿了，就直接丟棄
          abort.enqueue( oid, timer, 0 );
        }
      }
      else
        end = true;
      }
    return;
  }

  bool test2::getFile() { //讀入工作列表
    file = "sorted";
    fileNum = "";
    if ( fNum.compare( "-1" ) == 0 ) {
      cout << "Input a file number: ";
      cin >> fileNum;
    }
    else {
      fileNum = fNum;
    }
    
    file.append( fileNum ).append( ".txt" );
    in.open( file.c_str() );
    

    if ( !in.is_open() ) {
      cout << endl << "### " << file << " does not exist! ###" << endl;
      return false;
    }   
    
    cout << "\nThe simulation is running...\n";
    getline( in, c ); // 讀掉第一行

    while ( !in.eof() ) { // 讀入資料
      in >> oid;
      in >> arr;
      in >> dur;
      in >> time;
      if ( !in.eof() )
        list.enqueue( oid, arr, dur, time ); // 讀完資料將其送入list中
    }

    in.close();
    in.clear();
    return true;
  }

  void test2::outputF( ) { // output the file 
    string outFile = "output";
    int size = 0;
    outFile.append( fileNum ).append( ".txt" ); // the file name
    out.open( outFile.c_str() );
    out << "\t" << "[Abort Jobs]\n\t" << "OID\tAbort\tDelay\n";
    size = abort.getLength();
    for ( int i = 0 ; i < size ; i++ ) {
      abort.dequeue( oid, aod, delay );
      out << "[" << i+1 << "]\t" <<  oid << "\t" << aod << "\t" << delay << endl;
    }

    size = done.getLength();
    out << "\t" << "[Jobs Done]\n\t" << "OID\tDeparture\tDelay\n";
    for ( int i = 0 ; i < size ; i++ ) {
      done.dequeue( oid, aod, delay );
      out << "[" << i+1 << "]\t" <<  oid << "\t" << aod << "\t" << delay << endl;
    }

    out << "[Average Delay]\t" << fixed << setprecision( 2 ) << aveDelay << " ms\n"; // setprecision指印出多少位數
    out << "[Success Rate]\t" << fixed << setprecision( 2 ) << sucRate*100 << " %\n"; // fixed則表示setprecision內的數字是指小數點後的位數
    
    cout << "See " << outFile << endl;
    out.close();
    out.clear();

  }

  void test2::clear() {
    file = "sorted";
    list.clearList();
    abort.clearList();
    done.clearList();
    working.clearList();
    fNum = "-1";
    return;
  }
  
  /*****************************************************************/

class doubleCPU {
  private:
    ifstream in;
    ofstream out;
    Queue list;
    Queue abort;
    Queue done;
    Queue core1;
    Queue core2;
    Queue coreID;
    string file;
    string fileNum;
    string c;
    int coreNum;
    int oid;
    int cid;
    int arr;
    int aod;
    int dur;
    int delay;
    int time;
    int timer;
    int useTime;
    int useTime2;
    float aveDelay;
    float sucRate;

  public:
    bool getFile();
    void test();
    void workListing( Queue &working, bool &end );
    void processing( bool &thisTime, bool &isWork, Queue &working, int coreNum, int &needT );
    void calculate( float &aveDelay, float &sucRate );
    void outputF();
    void clear();
    Queue selCore( int &c );
};

  void doubleCPU::test() {
  	
  	aveDelay = 0;
  	sucRate = 0;
    timer = 0;
    int a;

    bool end = false;
    bool isEnd = false;
    bool isWork = false;
    bool isWork2 = false;
    bool wasSet = true;
    bool thisTime = false;
    useTime = 0; // timer + dur 用來記目前工作處理時間需多久
    useTime2 = 0;

      list.getfront( oid, arr, dur, time ); //list為排列好的工作列表
      end = false;
      while ( !list.isEmpty() && !end ) {
        selCore(a);
        if ( a == 1 ) 
          workListing( core1, end );                        //檢查time = 0時是否有工作可做
        else
          workListing( core2, end );
      }
    while ( !isEnd ) {
      
      thisTime = false;
      processing( thisTime, isWork, core1, 1, useTime );
  
      thisTime = false;
      processing( thisTime, isWork2, core2, 2, useTime2 );

      list.getfront( oid, arr, dur, time ); //看看處理完目前工作後，該時刻是否有其他工作抵達
      end = false;
      while ( !list.isEmpty() && !end ) {  
        selCore(a); // a = 空閒的CPU 
        if ( a == 1 )
          workListing( core1, end );
        else
          workListing( core2, end );
      }
      thisTime = false;
      processing( thisTime, isWork, core1, 1, useTime );
      thisTime = false;
      processing( thisTime, isWork2, core2, 2, useTime2 );

      if ( list.isEmpty() && core1.isEmpty() && core2.isEmpty() ) // 若列表為空，正要進行的工作queue也都空了，表示全做完
        isEnd = true;

      timer++;
      
    }
    
  
    
    calculate( aveDelay, sucRate );
    //abort.printAll( 3 );
    //done.printAll( 3 );

    return;
  }

  void doubleCPU::calculate( float &aveDelay, float &sucRate ) { // 計算平均延遲及成功率
    aveDelay = 0;
    sucRate = 0;
    aveDelay += done.getTotalD();
    aveDelay += abort.getTotalD();

    aveDelay = (float)aveDelay / ( (float)done.getLength() + (float)abort.getLength() );
    sucRate = (float)done.getLength() / ( (float)done.getLength() + (float)abort.getLength() );
    return;
  }

  void doubleCPU::processing( bool &thisTime, bool &isWork, Queue &working, int coreid, int &needT ) { // 處理資料是否完成或逾期

      while ( !thisTime ) { // thisTime表示這個時刻是否該做的都做完了
        if ( !working.isEmpty() )
          working.getfront( oid, arr, dur, time ); // 若工作列表不為空，先看第一筆資料

        if ( !isWork && !working.isEmpty() && needT == 0 ) { // 如果工作列表有工作且當前沒在工作
          isWork = true;                       // 開始工作( 設定完成工作的時間 )
          needT = timer + dur;
        }
    
      
        if ( timer == needT && !working.isEmpty() ) { // if done work
       
          working.dequeue( oid, arr, dur, time );
          done.enqueueT3( oid, coreid, timer, ( timer - arr - dur ) );
          
          needT = 0;
          isWork = false;
        }
        else if ( timer >= time && !working.isEmpty() ) { // 如果timeout
      
          working.dequeue( oid, arr, dur, time );
          abort.enqueueT3( oid, coreid, timer, ( timer - arr ) );
          isWork = false;
          needT = 0;
        }
        else { // 其他代表此時刻的工作都已完成
      
          thisTime = true;
        }
        //cout << " end work 2" << endl ;
       
        if ( !isWork && !working.isEmpty() && needT == 0 ) { // 如果工作列表有工作且當前沒在工作(若上面有完成OR丟掉)
          working.getfront( oid, arr, dur, time );
          
          isWork = true;                       // 開始工作( 設定完成工作的時間 )
          needT = timer + dur;
        }
        
        /*
        if ( coreid == 1 )
          useTime = needT;
        else if ( coreid == 2 )
          useTime2 = needT;
        */
        
      }
    return;
  }
  
  Queue doubleCPU::selCore( int &coreid ) {
  	Queue temp;
  	if ( core1.getLength() <= core2.getLength() ) {	  
  	  coreid = 1;
      return core1;
    }
  	else {
  	  coreid = 2;
      return core2;
    }


  }

  void doubleCPU::workListing( Queue &working, bool &outEnd ) { // 處理資料是否放入等待區或是丟棄區


      list.getfront( oid, arr, dur, time ); //取第一筆資料
        
      if ( timer == arr ) { // 若當前時間剛好為抵達時間
          
        list.dequeue( oid, arr, dur, time ); //將資料取出並做處理
          
        if ( working.getLength() <= 3 ) { // 第一筆為正在處裡的資料 (若工作queue還沒滿則放入queue中)
          working.enqueue( oid, arr, dur, time );
          
            
        }
        else if ( timer == useTime && working.getLength() <= 4 ) { // 0為正好結束的 1為正要處理的 因此queue中的是2~4
          working.enqueue( oid, arr, dur, time );
        }
        else { // 若queue滿了，就直接丟棄
          abort.enqueueT3( oid, 0, timer, 0 ); // cid = 0 表示直接被丟掉了
        }
      }
        
      else
          outEnd = true;
      
    
    return;
  }

  bool doubleCPU::getFile() { //讀入工作列表
    file = "sorted";
    fileNum = "";
    if ( fNum.compare( "-1" ) == 0 ) {
      cout << "Input a file number: ";
      cin >> fileNum;
    }
    else {
      fileNum = fNum;
    }
    
    file.append( fileNum ).append( ".txt" );
    in.open( file.c_str() );
    

    if ( !in.is_open() ) {
      cout << endl << "### " << file << " does not exist! ###" << endl;
      return false;
    }   
    
    cout << "\nThe simulation is running...\n";
    getline( in, c ); // 讀掉第一行

 
    while ( !in.eof() ) { // 讀入資料
      in >> oid;
      in >> arr;
      in >> dur;
      in >> time;

      if ( !in.eof() )
        list.enqueue( oid, arr, dur, time ); // 讀完資料將其送入list中
    }

    in.close();
    in.clear();
    return true;
  }

  void doubleCPU::outputF( ) { // output the file 
    string outFile = "double";
    int size = 0;
    outFile.append( fileNum ).append( ".txt" ); // the file name
    out.open( outFile.c_str() );
    out << "\t" << "[Abort Jobs]\n\t" << "OID\tCID\tAbort\tDelay\n";
    size = abort.getLength();
    for ( int i = 0 ; i < size ; i++ ) {
      abort.dequeueT3( oid, cid, aod, delay );
      out << "[" << i+1 << "]\t" <<  oid << "\t" << cid << "\t" << aod << "\t" << delay << endl;
    }

    size = done.getLength();
    out << "\t" << "[Jobs Done]\n\t" << "OID\tCID\tDeparture\tDelay\n";
    for ( int i = 0 ; i < size ; i++ ) {
      done.dequeueT3( oid, cid, aod, delay );
      out << "[" << i+1 << "]\t" <<  oid << "\t" << cid << "\t" << aod << "\t" << delay << endl;
    }

    out << "[Average Delay]\t" << fixed << setprecision( 2 ) << aveDelay << " ms\n"; // setprecision指印出多少位數
    out << "[Success Rate]\t" << fixed << setprecision( 2 ) << sucRate*100 << " %\n"; // fixed則表示setprecision內的數字是指小數點後的位數
    
    cout << "See " << outFile << endl;
    out.close();
    out.clear();

  }

  void doubleCPU::clear() {
    file = "sorted";
    list.clearList();
    abort.clearList();
    done.clearList();
    core1.clearList();
    core2.clearList();
    coreID.clearList();
    fNum = "-1";
    return;
  }



int main() {
  test2 t;
  doubleCPU t3;
  int fileName ;
  string com = "-1"; // commend;

  while ( !com.compare( "0" ) == 0 ) {
    cout << "\n**** Simulate FIFO Queues by SQF *****\n";
    cout << "* 0. Quit                            *\n";
    cout << "* 1. Sort a file                     *\n";
    cout << "* 2. Simulate one FIFO queue         *\n";
    cout << "* 3. Simulate two queues by SQF      *\n";
    cout << "**************************************\n";
    cout << "Input a command(0, 1, 2, 3): ";
    cin >> com;
    cout << endl;

    if ( com.compare("1") == 0 ) { // choose function 1
    	cout << "Input a file number: " ;
    	cin >> fileName ;
      cout << endl;
    	findFile( fileName ) ;
    	if ( doNotFile ) {
    		Queue queueReaded ;
    		queueReaded.setQueue();
    		queueReaded.shellSort();
    		queueReaded.saveFile(fileName);
    		
    		
    		cout << "Reading data: " << readE - readS<< "clocks (" << readE - readS << ".00ms)." << endl ;
    		cout << "Sorting data: " << sortE - sortS<< "clocks (" << sortE - sortS << ".00ms)." << endl ;
    		cout << "Writing data: " << writeE - writeS<< "clocks (" << writeE - writeS << ".00ms)." << endl ;
    		cout << endl<< "See sorted" << fileName << ".txt" << endl ;
    		queueReaded.clearList();
    		file.close();
        file.clear();
    	//queueReaded.saveFile(fileName);
		}
		
		doNotFile = false ;
    	
    }
    else if ( com.compare( "2" ) == 0 ) { // choose function 2
      
      if( t.getFile() ) {
        t.test();
        t.outputF();
        t.clear();
      }

    }
    else if ( com.compare( "3" ) == 0 ) {
      if( t3.getFile() ) {
      	
        t3.test();
        t3.outputF();
        
        t3.clear();
      }
	}
    else if ( com.compare( "0" ) == 0 ) {
      ; // do nothing and leave while
    }
    else {
      cout << "Command does not exist!\n";

    }


  }
  return 0;

}

