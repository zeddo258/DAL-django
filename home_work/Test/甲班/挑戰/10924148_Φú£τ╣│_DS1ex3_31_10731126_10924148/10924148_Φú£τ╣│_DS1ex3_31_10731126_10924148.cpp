/*********************
 �Ǹ� 10924148  10731126 
 �m�W ������	���a�^ 
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

// unclassed part ��

string InttoString(int num) {
    stringstream ss;
    ss << num;
    string out_string = ss.str();
    return out_string;

}  // ItoS()

fstream file;
string firstline;  // �Ĥ@��]�N�OOID Arrival ���� 
bool doNotFile = false; // �ɮ׬O�_�w�g���� 
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
	        // �v�ӥ[�i��C�̭�
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

  float Queue::getTotalD() { // ���o�`����
    float total = 0;
    for ( int i = 0 ; i < delay.size() ; i++ ) {
      total += delay.at(i);
    }
    return total;
  }

  void Queue::dequeue( int &Oid, int &AOD, int &Delay ) { // ���X�u�@(��P�w�u�@�����γQ���ɨϥ�)
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

  void Queue::dequeue( int &Oid, int &Arr, int &Dur, int &Time ) { // ���X�u�@(��P�_�u�@���q�ϥ�)
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

  bool Queue::isEmpty() { // �T�{queue�O�_���Ū�
    if ( oid.empty() )
      return true;
    else 
      return false;
  }

  void Queue::getfront( int &Oid, int &Arr, int &Dur, int &Time ) { // �ݲĤ@�����(����Xqueue)
    if ( !isEmpty() ) {
      Oid = oid.front();
      Arr = arr.front();
      Dur = dur.front();
      Time = time.front();
    }
    return; 
  }

  int Queue::getLength() { // ���o��ƪ���(�Hoid����)
    return oid.size();
  }
  
  void Queue::enqueueT3( int Oid, int Cid, int AOD, int Delay ) { // two cores
    oid.push_back( Oid );
    cid.push_back( Cid );
    aod.push_back( AOD );
    delay.push_back( Delay );    
  }  

  void Queue::enqueue( int Oid, int AOD, int Delay ) { // �u�@�����άO�Q���ɥ�
    oid.push_back( Oid );
    aod.push_back( AOD );
    delay.push_back( Delay );    
  }

  void Queue::enqueue( int Oid, int Arr, int Dur, int Time ) { // �N�u�@�C��Ū�J�ɥ�
    oid.push_back( Oid );
    arr.push_back( Arr );
    dur.push_back( Dur );
    time.push_back( Time );
  }

  void Queue::printAll( int i ) { // debu�� �i����
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
    useTime = 0; // timer + dur �ΨӰO�ثe�u�@�B�z�ɶ��ݦh�[

      list.getfront( oid, arr, dur, time ); //list���ƦC�n���u�@�C��
      workListing();                        //�ˬdtime = 0�ɬO�_���u�@�i��
    while ( !isEnd ) {
      
      thisTime = false;
      processing( thisTime, isWork );

      list.getfront( oid, arr, dur, time ); //�ݬݳB�z���ثe�u�@��A�Ӯɨ�O�_����L�u�@��F
      workListing();
      thisTime = false;
      processing( thisTime, isWork );

      if ( list.isEmpty() && working.isEmpty() ) // �Y�C���šA���n�i�檺�u�@queue�]���ŤF�A��ܥ�����
        isEnd = true;

      timer++;
      
    }

    calculate( aveDelay, sucRate );

    return;
  }

  void test2::calculate( float &aveDelay, float &sucRate ) { // �p�⥭������Φ��\�v
    aveDelay = 0;
    sucRate = 0;
    aveDelay += done.getTotalD();
    aveDelay += abort.getTotalD();

    aveDelay = (float)aveDelay / ( (float)done.getLength() + (float)abort.getLength() );
    sucRate = (float)done.getLength() / ( (float)done.getLength() + (float)abort.getLength() );
    return;
  }

  void test2::processing( bool &thisTime, bool &isWork ) { // �B�z��ƬO�_�����ιO��
      while ( !thisTime ) { // thisTime��ܳo�Ӯɨ�O�_�Ӱ����������F
        if ( !working.isEmpty() )
          working.getfront( oid, arr, dur, time ); // �Y�u�@�C�����šA���ݲĤ@�����

        if ( !isWork && !working.isEmpty() && useTime == 0 ) { // �p�G�u�@�C���u�@�B��e�S�b�u�@
          isWork = true;                       // �}�l�u�@( �]�w�����u�@���ɶ� )
          useTime = timer + dur;
        }
    
      
        if ( timer == useTime && !working.isEmpty() ) { // if done work
          working.dequeue( oid, arr, dur, time );
          done.enqueue( oid, timer, ( timer - arr - dur ) );
          useTime = 0;
          isWork = false;
        }
        else if ( timer >= time && !working.isEmpty() ) { // �p�Gtimeout
          working.dequeue( oid, arr, dur, time );
          abort.enqueue( oid, timer, ( timer - arr ) );
          isWork = false;
          useTime = 0;
        }
        else { // ��L�N���ɨ誺�u�@���w����
          thisTime = true;
        }
       
        if ( !isWork && !working.isEmpty() && useTime == 0 ) { // �p�G�u�@�C���u�@�B��e�S�b�u�@(�Y�W��������OR�ᱼ)
          working.getfront( oid, arr, dur, time );
          
          isWork = true;                       // �}�l�u�@( �]�w�����u�@���ɶ� )
          useTime = timer + dur;
        }

        
      }
    return;
  }

  void test2::workListing( ) { // �B�z��ƬO�_��J���ݰϩάO����
    bool end = false;
    while ( !list.isEmpty() && !end ) {
      list.getfront( oid, arr, dur, time ); //���Ĥ@�����
      
      if ( timer == arr ) { // �Y��e�ɶ���n����F�ɶ�
        
        list.dequeue( oid, arr, dur, time ); //�N��ƨ��X�ð��B�z
        
        if ( working.getLength() <= 3 ) { // �Ĥ@�������b�B�̪���� (�Y�u�@queue�٨S���h��Jqueue��)
          working.enqueue( oid, arr, dur, time );
          
        }
        else if ( timer == useTime && working.getLength() <= 4 ) { // 0�����n������ 1�����n�B�z�� �]��queue�����O2~4
          working.enqueue( oid, arr, dur, time );
        }
        else { // �Yqueue���F�A�N�������
          abort.enqueue( oid, timer, 0 );
        }
      }
      else
        end = true;
      }
    return;
  }

  bool test2::getFile() { //Ū�J�u�@�C��
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
    getline( in, c ); // Ū���Ĥ@��

    while ( !in.eof() ) { // Ū�J���
      in >> oid;
      in >> arr;
      in >> dur;
      in >> time;
      if ( !in.eof() )
        list.enqueue( oid, arr, dur, time ); // Ū����ƱN��e�Jlist��
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

    out << "[Average Delay]\t" << fixed << setprecision( 2 ) << aveDelay << " ms\n"; // setprecision���L�X�h�֦��
    out << "[Success Rate]\t" << fixed << setprecision( 2 ) << sucRate*100 << " %\n"; // fixed�h���setprecision�����Ʀr�O���p���I�᪺���
    
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
    useTime = 0; // timer + dur �ΨӰO�ثe�u�@�B�z�ɶ��ݦh�[
    useTime2 = 0;

      list.getfront( oid, arr, dur, time ); //list���ƦC�n���u�@�C��
      end = false;
      while ( !list.isEmpty() && !end ) {
        selCore(a);
        if ( a == 1 ) 
          workListing( core1, end );                        //�ˬdtime = 0�ɬO�_���u�@�i��
        else
          workListing( core2, end );
      }
    while ( !isEnd ) {
      
      thisTime = false;
      processing( thisTime, isWork, core1, 1, useTime );
  
      thisTime = false;
      processing( thisTime, isWork2, core2, 2, useTime2 );

      list.getfront( oid, arr, dur, time ); //�ݬݳB�z���ثe�u�@��A�Ӯɨ�O�_����L�u�@��F
      end = false;
      while ( !list.isEmpty() && !end ) {  
        selCore(a); // a = �Ŷ���CPU 
        if ( a == 1 )
          workListing( core1, end );
        else
          workListing( core2, end );
      }
      thisTime = false;
      processing( thisTime, isWork, core1, 1, useTime );
      thisTime = false;
      processing( thisTime, isWork2, core2, 2, useTime2 );

      if ( list.isEmpty() && core1.isEmpty() && core2.isEmpty() ) // �Y�C���šA���n�i�檺�u�@queue�]���ŤF�A��ܥ�����
        isEnd = true;

      timer++;
      
    }
    
  
    
    calculate( aveDelay, sucRate );
    //abort.printAll( 3 );
    //done.printAll( 3 );

    return;
  }

  void doubleCPU::calculate( float &aveDelay, float &sucRate ) { // �p�⥭������Φ��\�v
    aveDelay = 0;
    sucRate = 0;
    aveDelay += done.getTotalD();
    aveDelay += abort.getTotalD();

    aveDelay = (float)aveDelay / ( (float)done.getLength() + (float)abort.getLength() );
    sucRate = (float)done.getLength() / ( (float)done.getLength() + (float)abort.getLength() );
    return;
  }

  void doubleCPU::processing( bool &thisTime, bool &isWork, Queue &working, int coreid, int &needT ) { // �B�z��ƬO�_�����ιO��

      while ( !thisTime ) { // thisTime��ܳo�Ӯɨ�O�_�Ӱ����������F
        if ( !working.isEmpty() )
          working.getfront( oid, arr, dur, time ); // �Y�u�@�C�����šA���ݲĤ@�����

        if ( !isWork && !working.isEmpty() && needT == 0 ) { // �p�G�u�@�C���u�@�B��e�S�b�u�@
          isWork = true;                       // �}�l�u�@( �]�w�����u�@���ɶ� )
          needT = timer + dur;
        }
    
      
        if ( timer == needT && !working.isEmpty() ) { // if done work
       
          working.dequeue( oid, arr, dur, time );
          done.enqueueT3( oid, coreid, timer, ( timer - arr - dur ) );
          
          needT = 0;
          isWork = false;
        }
        else if ( timer >= time && !working.isEmpty() ) { // �p�Gtimeout
      
          working.dequeue( oid, arr, dur, time );
          abort.enqueueT3( oid, coreid, timer, ( timer - arr ) );
          isWork = false;
          needT = 0;
        }
        else { // ��L�N���ɨ誺�u�@���w����
      
          thisTime = true;
        }
        //cout << " end work 2" << endl ;
       
        if ( !isWork && !working.isEmpty() && needT == 0 ) { // �p�G�u�@�C���u�@�B��e�S�b�u�@(�Y�W��������OR�ᱼ)
          working.getfront( oid, arr, dur, time );
          
          isWork = true;                       // �}�l�u�@( �]�w�����u�@���ɶ� )
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

  void doubleCPU::workListing( Queue &working, bool &outEnd ) { // �B�z��ƬO�_��J���ݰϩάO����


      list.getfront( oid, arr, dur, time ); //���Ĥ@�����
        
      if ( timer == arr ) { // �Y��e�ɶ���n����F�ɶ�
          
        list.dequeue( oid, arr, dur, time ); //�N��ƨ��X�ð��B�z
          
        if ( working.getLength() <= 3 ) { // �Ĥ@�������b�B�̪���� (�Y�u�@queue�٨S���h��Jqueue��)
          working.enqueue( oid, arr, dur, time );
          
            
        }
        else if ( timer == useTime && working.getLength() <= 4 ) { // 0�����n������ 1�����n�B�z�� �]��queue�����O2~4
          working.enqueue( oid, arr, dur, time );
        }
        else { // �Yqueue���F�A�N�������
          abort.enqueueT3( oid, 0, timer, 0 ); // cid = 0 ��ܪ����Q�ᱼ�F
        }
      }
        
      else
          outEnd = true;
      
    
    return;
  }

  bool doubleCPU::getFile() { //Ū�J�u�@�C��
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
    getline( in, c ); // Ū���Ĥ@��

 
    while ( !in.eof() ) { // Ū�J���
      in >> oid;
      in >> arr;
      in >> dur;
      in >> time;

      if ( !in.eof() )
        list.enqueue( oid, arr, dur, time ); // Ū����ƱN��e�Jlist��
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

    out << "[Average Delay]\t" << fixed << setprecision( 2 ) << aveDelay << " ms\n"; // setprecision���L�X�h�֦��
    out << "[Success Rate]\t" << fixed << setprecision( 2 ) << sucRate*100 << " %\n"; // fixed�h���setprecision�����Ʀr�O���p���I�᪺���
    
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

