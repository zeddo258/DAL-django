//11127217 陳彥汝 11127238 王詩霈 
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <time.h>
using namespace std;

typedef struct list{
	int OID;		// order identifier
	int arrival;	// arrival time
	int duration;	// job duration
	int timeout;	// expire time
	int head;		//紀錄開始處理時間 
};

typedef struct job{
	int OID;
	int abort;
	int dep;
	int delay;
};

class Queue{
	private:
		vector<list> line;
	public:
	
	int length() {			//計算queue長度 
		return line.size();
	} 
	
	bool isEmpty() {		//queue是空的 
		if ( line.size() == 0 ) {
			return true;
		} 
		else{
			return false; 
		}
	}  	
	
	bool isFull() {  		// queue滿了 
		if ( line.size() == 4 )  {
			return true;
		}
		else{
			return false; 
		}
	} 
	
	void push( list u) {	//在queue新增 
		line.push_back(u);
	}  
	
	void pop() {			//刪除開頭 
		line.erase( line.begin() );
	} 
	
	void run(){				//每經過一秒把處理中的資料的duration減一 
		line[0].duration--;
	}
	
	void start(int m){		//設置開始處理時間 
		line[0].head = m;
	}
	
	list getFront() {		//取得開頭 
		return line[0] ;
	} 
	
};

void task1(){
	ifstream in;
	ofstream out;
	vector<list> txt;
	int oid, duration, timeout, arrival=-1;
	double reading, rend;
	double sorting, send;
	double writing, wend;
	string filename, read, input, output;
	cout << endl <<  "Input a file number: " ;
	cin >> filename;
	input = "input" + filename + ".txt";	//串起檔案名稱  
	output = "sorted" + filename + ".txt";	//輸出檔名稱 
	in.open(input.c_str());					//開啟檔案 
	if( in.fail() ){						//檔案不存在  
		cout << endl << "### " << filename << " does not exist! ###" << endl ;
	}
	else {									//檔案存在 
		reading = clock();					//讀檔開始時間 
		for(int i=0 ; i<4 ; i++){			//把文字讀掉 
				in >> read;
		}
		in >> oid >> arrival >> duration >> timeout; 
		while ( arrival != -1 ){				//讀 & 存全部資料  
			list line;
			line.OID = oid;
			line.duration = duration;
			line.arrival = arrival;
			line.timeout = timeout;
			txt.push_back(line);
			arrival = -1;
			in >> oid >> arrival >> duration >> timeout;
		}
		rend = clock() ;				//讀檔結束時間 
		cout << endl << "\t" << "OID	Arrival	Duration	TimeOut" << endl;
		for(int i=0;i<txt.size();i++){	 
			cout << "(" << i+1 << ")\t" << txt[i].OID << "\t" << txt[i].arrival << "\t" << txt[i].duration << "\t" << txt[i].timeout << endl; 
		}
		
		sorting = clock();				//排序開始時間 
		list store;
		for(int len= txt.size()/2;len>0;len=len/2){				//shell sort 
			for ( int d = 0 ; d < len ; d++ ){					//每組排序 
				for(int i=0; i<txt.size();i=i+len){				//組內泡泡排序 
					for ( int j = i ; j<txt.size();j=j+len){
						if ( txt[j].arrival < txt[i].arrival){			//arrival左 > arrival右 
							store = txt[i];
							txt[i] = txt[j];
							txt[j] = store;
						}
						else if( txt[j].arrival == txt[i].arrival){		//arrival左 == arrival右
							if(txt[j].OID < txt[i].OID){				//比較OID左 > OID右
								store = txt[i];
								txt[i] = txt[j];
								txt[j] = store;
							}
						}
							
					}
				}
			}	
		}
		
		send = clock();					//排序結束時間 
		writing = clock();				//寫檔開始時間 
		
		//寫檔 
		out.open( output.c_str());
		out << "OID	Arrival	Duration	TimeOut" << endl;
		for(int i=0;i<txt.size();i++){	
			out << txt[i].OID << "\t" << txt[i].arrival << "\t" << txt[i].duration << "\t" << txt[i].timeout << endl; 
		}
		wend = clock();					//寫檔結束時間
		 
		//輸出
		int a = rend-reading;
		int b = send-sorting;
		int c = wend-writing;
		cout << endl;
		cout << "Reading data: " << a << " clocks (" << fixed << setprecision(2) << rend-reading <<  " ms).\n";
		cout << "Sorting data: " << b << " clocks (" << fixed << setprecision(2) << send-sorting <<  " ms).\n";
		cout << "Writing data: " << c << " clocks (" << fixed << setprecision(2) << wend-writing <<  " ms).\n";
		cout << endl << "See sorted" << filename << ".txt" << endl; 
		//清空、關閉檔案 
		txt.clear();
		out.close();
		in.close();
	}	
}

void task2(){
	ifstream in;
	ofstream out;
	vector<list> txt;
	vector<job> abort, done; 
	int oid, arrival, duration, timeout;
	int locate = 0, time = 0, end = 0;
	double average;
	string filename, read, input, output;
	Queue call;
	cout << endl <<  "Input a file number: " ;
	cin >> filename;
	input = "sorted" + filename + ".txt";	//串起檔案名稱  
	output = "output" + filename + ".txt";	//輸出檔名稱 
	in.open(input.c_str());					//開啟檔案 
	if( in.fail() ){						//檔案不存在  
		cout << endl << "### " << filename << " does not exist! ###" << endl ;
	}
	else {									//檔案存在 
		for(int i=0 ; i<4 ; i++){			//把文字讀掉 
				in >> read;
		}
		in >> oid >> arrival >> duration >> timeout; 
		while ( !in.eof() ){				//讀 & 存全部資料  
			if( timeout > end){		//end紀錄所有資料中最晚的timeout時間 
				end = timeout;
			}
			list line;
			line.OID = oid;
			line.duration = duration;
			line.arrival = arrival;
			line.timeout = timeout;
			line.head = 0;
			txt.push_back(line);
			in >> oid >> arrival >> duration >> timeout;
		}
	}
	
	for( time = txt[0].arrival ; time<=end;time++){
		if(! call.isEmpty()){	//有處理中的工作的話，每秒都要將它的duration減一 
			call.run();
		}
		while( ! call.isFull() &&time==txt[0].arrival ){	//當queue還有空間且到了arrival的時間，把資料加到queue中 
			call.push(txt[0]);
			txt.erase( txt.begin() );
		}
		while( !call.isEmpty() && (time>=call.getFront().timeout||call.getFront().duration==0)){		//目前有在處理的工作且已處理完成或是到timeout 
			if(call.getFront().duration==0&& call.getFront().timeout>=time){ 			//已做完 需加入done列表 
				job store;
				oid = call.getFront().head-call.getFront().arrival; //計算delay 
				if(oid < 0){	//處理時間沒有耽誤到 
					store.delay = 0;
				}
				else{
					store.delay = oid;
				}
				store.dep=time;
				store.OID=call.getFront().OID;			
				done.push_back(store);
				call.pop();
				call.start(time);
				if( txt.size()!=0&&time==txt[0].arrival){
					call.push(txt[0]);
					txt.erase( txt.begin() );
				}
			}
			else{										//未完成 需加入abort列表 
				job store;
				store.delay=time-call.getFront().arrival;
				store.abort=time;
				store.OID=call.getFront().OID;
				abort.push_back(store);
				call.pop();
				call.start(time);
				if( txt.size()!=0&&time==txt[0].arrival){
					call.push(txt[0]);
					txt.erase( txt.begin() );
				}
			}
		}

		while(time==txt[0].arrival&&txt.size()!=0){  	//當arrival的時間到了沒進入queue中就加入abort 
			job store;
			store.delay=0;
			store.abort=time;
			store.OID=txt[0].OID;
			abort.push_back(store);
			txt.erase( txt.begin() );
		}
	}
	
	//計算average delay 
	for(int i = 0; i<abort.size();i++){
		average = average + abort[i].delay;
	}
	for(int i = 0; i<done.size();i++){
		average = average + done[i].delay;
	}
	double total = abort.size()+done.size();
	
	//寫檔  
	out.open( output.c_str());
	out << "\t[Abort Jobs]" << endl;
	out << "\t" << "OID	 Abort	Delay" << endl;
	for(int i = 0; i<abort.size();i++){
		out <<"[" << i+1 << "]\t"<< abort[i].OID << "\t" << abort[i].abort << "\t" << abort[i].delay << endl;
	}
	out << "\t" << "[Jobs Done]" << endl;
	out << "\t" << "OID	 Departure	Delay" << endl;
	for(int i = 0; i<done.size();i++){
		out <<"[" << i+1 << "]\t"<< done[i].OID << "\t" << done[i].dep << "\t" << done[i].delay << endl;
	}
	out << "[Average Delay] "<< fixed << setprecision(2) << average / total << " ms"<<endl;
	out << "[Success Rate] "<< fixed << setprecision(2) << (done.size() / total)*100<< " %"<<endl;
		
	//輸出 
	cout << endl << "The simulation is running..." << endl;
	cout << "See output" << filename << ".txt" << endl;
	txt.clear();
	out.close();
	in.close();
}

int main(int argc, char** argv) {
	int run = 0;
    while ( run == 0 ) {
		string command;
		cout << endl;
    	cout << "**** Simulate FIFO Queues by SQF *****" << endl;
		cout << "* 0. Quit                            *" << endl;
    	cout << "* 1. Sort a file                     *" << endl;
		cout << "* 2. Simulate one FIFO queue         *" << endl;
		cout << "**************************************" << endl;
		cout << "Input a command(0, 1, 2): " ;
		cin >> command;	  
    	if ( command == "0" )  {
    		return 0;
		}
    	else if ( command == "1" ) {
    		task1();
	    }    
		else if ( command == "2" ) {
			task2();
		}
		else{
			cout << endl << "Command does not exist!" << endl;
		}
	}
}
