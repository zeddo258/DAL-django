// 11127106 陳子昂  11127118 江翊寧 
#include<stdio.h>
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
#include <iomanip>
using namespace std;
struct jobType{  // 宣告工作資料的struct
	int OID;
	int arrival;
	int duration;
	int timeout;	
};
struct abortType{ // 取消清單的struct
	int OID;
	int abort;
	int delay;	
};
struct done{ // 完成清單的strcut
	int OID;
	int departure;
	int delay;	
};
class Queue {
private:
	vector<jobType> queue;
public:
	bool isQueueFull () { // 檢查queue是否為滿，但vector沒有大小上限，因此return false
		return false;
	} // isQueueFull()

	bool isEmpty () { // 檢查queue是否為空
		return queue.empty();
	} // isEmpty

	void enqueue (jobType data) { // 從佇列尾端加入元素
		queue.push_back (data);
	} // enqueue() 

	jobType dequeue () { // 從佇列前端取出元素
		if (!queue.empty ()) {
			jobType front_data = queue.front();
			queue.erase (queue.begin ());
			return front_data;
		} // if
		else {
			cout <<"The queue is empty."<< endl;
			jobType ret = {0};
			return ret;
		} // else
	} // dequeue()
}; // Queue
class runtasktwo{
		vector<done> dojob; //完成清單的vector 
		vector<jobType> aListt;
		vector<abortType> cannotdo; // 取消清單的vector 
		vector<jobType> decide;
	public:
		void dotasktwo( vector<jobType> aList , string fileName);		
};
class JobList {
		vector<jobType> aList;
	public:
		bool getAll(string a, int command  );
		bool getSorted( string fileName, double read, int command );
		bool tasktwo( string fileName);	
};
void runtasktwo::dotasktwo( vector<jobType> aList, string fileName) {
	Queue aQueue;
	abortType abt;
	jobType type;
	done D;
	string files = "sorted" + fileName + ".txt";
	ifstream ifs( files.c_str() );
	if ( ifs ) aListt = aList;
	else return;
	ifs.close();
	int cpu = aListt.at(0).arrival ; // cpu 開始時間為第一筆資料的arrival 
	int i = 0, time = 0;
	int dojobs = 0, abort = 0, nor = 0;
	for ( int i =0; i < aListt.size() ;i++){
		if ( aListt.at(i).duration + cpu <= aListt.at(i).timeout && aListt.at(i).arrival >=cpu   && aQueue.isEmpty() ) {
			dojob.push_back( D );
			dojob.at(dojobs).OID = aListt.at(i).OID;
			dojob.at(dojobs).departure = cpu + aListt.at(i).duration;
			dojob.at(dojobs).delay = cpu - aListt.at(i).arrival;
			cpu = cpu + aListt.at(i).duration;
			dojobs++;
		}
		else if (  aListt.at(i).arrival > cpu ){ //閒置
			decide.push_back(type);
			decide.at(0) = aQueue.dequeue();
			if ( cpu < decide.at(0).arrival ) cpu = decide.at(0).arrival;
			if ( cpu + decide.at(0).duration > decide.at(0).timeout){ // 若cpu + duration > timeout 則放到取消清單中 
				if ( cpu > decide.at(0).timeout ){ //若 cpu > timeout 
					cannotdo.push_back( abt );
					cannotdo.at(nor).OID = decide.at(0).OID;
					cannotdo.at(nor).abort = cpu;
					cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
					nor++;
				}
				else { // 若 cpu <= timeout   
					cannotdo.push_back( abt );
					cannotdo.at(nor).OID = decide.at(0).OID;
					cannotdo.at(nor).abort = decide.at(0).timeout;
					cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
					nor++;
					cpu = decide.at(0).timeout; // 改變 cpu時間為timeout 
				}
			}
			else if ( cpu + decide.at(0).duration <=  decide.at(0).timeout ){ // cpu + duration <= timeout 則放到完成清單中 
				dojob.push_back( D );
				dojob.at(dojobs).OID = decide.at(0).OID;
				dojob.at(dojobs).departure = cpu + decide.at(0).duration;
				dojob.at(dojobs).delay = cpu -  decide.at(0).arrival;
				cpu = cpu + decide.at(0).duration;
				dojobs++;
			}
			time--;
			aQueue.enqueue( aListt.at(i) );
			time++;
		}
		else if ( aListt.at(i).arrival == cpu ) { // 如果這筆資料會的arrival等於cpu, 則先清空佇列內的資料 
			if ( ! aQueue.isEmpty() ){ //代表佇列不為空 
				for( int j = 0; ! aQueue.isEmpty() ; j++ ){
					decide.push_back(type);
					decide.at(0) = aQueue.dequeue(); // 把佇列的第一個丟出來 
					if ( cpu + decide.at(0).duration > decide.at(0).timeout){ // 若cpu + duration > timeout 則放到取消清單中  
						if ( cpu > decide.at(0).timeout ){ //若 cpu > timeout  
							cannotdo.push_back( abt );
							cannotdo.at(nor).OID = decide.at(0).OID;
							cannotdo.at(nor).abort = cpu;
							cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
							nor++;
						}
						else { // 若 cpu <= timeout   
							cannotdo.push_back( abt );
							cannotdo.at(nor).OID = decide.at(0).OID;
							cannotdo.at(nor).abort = decide.at(0).timeout;
							cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
							nor++;
							cpu = decide.at(0).timeout; // 改變 cpu時間為timeout  
						}
					}
					else if ( cpu + decide.at(0).duration <=  decide.at(0).timeout ){ // cpu + duration <= timeout 則放到完成清單中  
						dojob.push_back( D );
						dojob.at(dojobs).OID = decide.at(0).OID;
						dojob.at(dojobs).departure = cpu + decide.at(0).duration;
						dojob.at(dojobs).delay = cpu -  decide.at(0).arrival;
						cpu = cpu + decide.at(0).duration;
						dojobs++;
					}
					time--;
				}
			}
			
			i--;
		}
		
		else {
			if ( time < 3 ) { //若佇列中小於三個人 
				aQueue.enqueue( aListt.at(i) ); // 存入陣列中 
				time++;
			}
			else {  // 若還要存入佇列,則直接存到取消清單中 
				cannotdo.push_back( abt );
				cannotdo.at(nor).OID = aListt.at(i).OID;
				cannotdo.at(nor).abort = aListt.at(i).arrival;
				cannotdo.at(nor).delay = 0;
				nor++;
			}
		}
	}
	if ( ! aQueue.isEmpty() ){
		for( int j = 0; ! aQueue.isEmpty() ; j++ ){
			decide.push_back(type);
			decide.at(0) = aQueue.dequeue(); // 把佇列的第一個丟出來 
			if ( cpu + decide.at(0).duration > decide.at(0).timeout){ // 若cpu + duration > timeout 則放到取消清單中  
				if ( cpu > decide.at(0).timeout ){ //若 cpu > timeout 
					cannotdo.push_back( abt );
					cannotdo.at(nor).OID = decide.at(0).OID;
					cannotdo.at(nor).abort = cpu;
					cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
					nor++;
				}
				else { // 若 cpu <= timeout 
					cannotdo.push_back( abt );
					cannotdo.at(nor).OID = decide.at(0).OID;
					cannotdo.at(nor).abort = decide.at(0).timeout;
					cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
					nor++;
					cpu = decide.at(0).timeout;  // 改變 cpu時間為timeout   
				}
			}
			else if ( cpu + decide.at(0).duration <=  decide.at(0).timeout ){ // cpu + duration <= timeout 則放到完成清單中 
				dojob.push_back( D );
				dojob.at(dojobs).OID = decide.at(0).OID;
				dojob.at(dojobs).departure = cpu + decide.at(0).duration;
				dojob.at(dojobs).delay = cpu -  decide.at(0).arrival;
				cpu = cpu + decide.at(0).duration;
				dojobs++;
			}
			time--;
		}			
	}
	int plusC = 0;
	int plusD = 0;
	for ( int k = 0; k < nor ; k++){ // 紀錄取消清單delay的總和 
		plusC = plusC + cannotdo.at(k).delay;  
	}								
	for ( int k = 0; k < dojobs ; k++){  // 紀錄完成清單delay的總和 
		plusD = plusD + dojob.at(k).delay;
	}
	
	int success = dojob.size() * 100 ;
	string file = "output"+ fileName +".txt";
	ofstream fout( file.c_str() ); //建檔 
	if ( fout ){  
    	fout << "	[Abort Jobs]" << endl;
    	fout << "	OID	Abort	Delay" << endl;
    	for ( int k = 0; k < nor ; k++){
			fout << "[" << k+1 <<  "]" << "	"<< cannotdo.at(k).OID << "	" << cannotdo.at(k).abort;
			fout << "	" << cannotdo.at(k).delay << endl;		
		}								
		fout << "	[Jobs Done]" << endl;
		fout << "	OID	Departure	Delay" << endl;
		for ( int k = 0; k < dojobs ; k++){
			fout << "[" << k+1 << "]" << "	"<< dojob.at(k).OID << "	" << dojob.at(k).departure;
			fout << "	" << dojob.at(k).delay << endl;
			
		}
		fout << "[Average Delay]" << "	" << setprecision(2) << fixed << (float) (plusD+plusC)/aListt.size() << " ms" << endl;
		fout << "[Success Rate]	";
		fout << setprecision(2) <<fixed << (float)success / aListt.size() << " %"<<endl;
		cout << "The simulation is running..." << endl;
		cout << "See " << file << endl ;
    }
	fout.close();
}
bool JobList::tasktwo( string fileName ){
	runtasktwo start;
	start.dotasktwo( aList, fileName );
}
bool JobList::getSorted( string fileName, double read, int command ){
	double sortstart, sortend;
	double writestart, writeend;
	int step = aList.size()/2; // 從中間開始 
	int test = 0;
	int tempO;
	int tempA;
	int tempD;
	int tempT;
	sortstart = clock();
	while ( step >= 1 ){ // sell sort 
		for( int i = step ; i < aList.size() ; i++ ){
			for ( int j = i - step; j >= 0; j-= step ){
				if( aList.at(j).arrival > aList.at(j+step).arrival) { // 如果前面大於後面，則互換  
                	tempO= aList.at(j+step).OID;
            	   	tempA= aList.at(j+step).arrival;
            	    tempD= aList.at(j+step).duration;
            	    tempT= aList.at(j+step).timeout;
                	aList.at(j+step).OID = aList.at(j).OID;
           	     	aList.at(j+step).arrival = aList.at(j).arrival;
                	aList.at(j+step).duration = aList.at(j).duration;
                	aList.at(j+step).timeout =  aList.at(j).timeout;
                	aList.at(j).OID = tempO;
                	aList.at(j).arrival = tempA;
                	aList.at(j).duration = tempD;
                	aList.at(j).timeout = tempT;
            	}
            	else if (  aList.at(j).arrival == aList.at(j+step).arrival ){ // 如果arrival相等 
            		if ( aList.at(j).OID > aList.at(j+step).OID  ){ // 如果前面的OID大於後面的OID，則互換 
               			tempO= aList.at(j+step).OID;
                		tempA= aList.at(j+step).arrival;
                		tempD= aList.at(j+step).duration;
            	    	tempT= aList.at(j+step).timeout;
            	    	aList.at(j+step).OID = aList.at(j).OID;
                		aList.at(j+step).arrival = aList.at(j).arrival;
           	    		aList.at(j+step).duration = aList.at(j).duration;
                		aList.at(j+step).timeout =  aList.at(j).timeout;
                		aList.at(j).OID = tempO;
                		aList.at(j).arrival = tempA;
                		aList.at(j).duration = tempD;
                		aList.at(j).timeout = tempT;
					}
				}
			}
		}
		step = step / 2;
	}
	sortend = clock();
	if ( command == 1 ){
		string file = "sorted"+ fileName +".txt";  // 檔案名稱 
		writestart = clock();  // 開始寫入檔案的時間 
		ofstream fout( file.c_str() ); // 建檔 
		if ( fout ){  
    		fout << "OID	Arrival	Duration	TimeOut" << endl;
    		while( test < aList.size() ){
    			fout << aList.at(test).OID << "	";
				fout << aList.at(test).arrival << "	" ;
				fout << aList.at(test).duration << "	" ;
				fout << aList.at(test).timeout << endl ;
				test++;
			}
    	}
		fout.close();
		writeend = clock(); // 停止寫入檔案的時間  
		test = 0;
		cout << endl <<  "Reading data: " << read <<  " clocks (" << read << ".00 ms).\n" ;
		cout << "Sorting data: " << sortend-sortstart << " clocks (" << sortend-sortstart << ".00 ms).\n" ;
		cout << "Writing data: " << writeend-writestart << " clocks (" << writeend-writestart << ".00 ms).\n" ;
		cout << endl;
		cout << "See " << file << endl;
	}
	else if ( command == 2 ){
		tasktwo( fileName );  
	}	
}
bool JobList::getAll( string fileName , int command ){ //讀入檔案 
	double start , end;
	double read;
	string file = "input" + fileName + ".txt"; 
	jobType type;
	string s; 
	int number = 0;
	int test = 0;
	int i = 0;
	int j = 0;
	ifstream ifs( file.c_str() );//打開檔案
	start = clock(); // 讀檔開始時間 
    if ( ifs ){  
    	getline(ifs, s); // 先把檔案的第一排讀近來 
		while ( ifs >> number ){
			aList.push_back( type );
			aList.at(i).OID = number;
			ifs >> aList.at(i).arrival;
			ifs >> aList.at(i).duration;
			ifs >> aList.at(i).timeout;
			i++;
		}
    	ifs.close(); // 關閉檔案 
    	end = clock(); // 讀檔結束時間 
    	if ( command == 1 ) {
    		cout <<"	" <<s << endl;
    		while( test < aList.size()){
    			cout << "(" << test+1 << ")	" << aList.at(test).OID << "	";
				cout << aList.at(test).arrival << "	" ;
				cout << aList.at(test).duration << "	" ;
				cout << aList.at(test).timeout << endl ;
				test++;
			}
		}
    	read = end - start;  // 讀檔耗時 
    	getSorted( fileName, read , command);  // 進行shell sorted 
        return true;
    }
    cout << "### " << file << " does not exist! ###" << endl;
}
int main(){
	int command = 0;
	string fileName;
	int determine = 0; // 判斷前面command是否等於1 
	do {
		JobList ajoblist; // 讓我們能使用class JobList裡面的function  
		cout << endl << "**** Simulate FIFO Queues by SQF *****" << endl;
		cout << "* 0. Quit                            *" << endl;
		cout << "* 1. Sort a file                     *" << endl;
		cout << "* 2. Simulate one FIFO queue         *" << endl;
		cout << "**************************************" << endl;
		cout << "Input a command(0, 1, 2): ";
		cin >> command;
		cout << endl;
		if ( determine == 0 || command == 1 ){
			cout <<  "Input a file number: ";
			cin >> fileName;
			cout << "\n";
			determine = 1;
		}
		if ( command == 1 ){
			ajoblist.getAll( fileName, command );
			determine = 1; // 紀錄command=1 
		}
		else if ( command == 2 ){
			string file = "sorted" + fileName + ".txt"; 
			ifstream ifs( file.c_str() ); // 打開file這個檔案 
			if ( ifs ){ // 如果有此檔案  
				ajoblist.getAll( fileName , command ); // 呼叫getAll()的function讀取資料  
			}
			else { // 如果沒有  
				cout << "### " << "sorted" << fileName << ".txt does not exist! ###" << endl;
			}
		}
		else if ( command == 0 )  break;
		else  cout << "Command does not exist!" << endl;
		
	} while(true);
	system( "pause" );
	return 0;
} 
