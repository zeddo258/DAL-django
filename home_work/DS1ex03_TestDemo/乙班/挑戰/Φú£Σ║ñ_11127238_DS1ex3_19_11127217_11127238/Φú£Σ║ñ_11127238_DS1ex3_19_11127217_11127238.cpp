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
	int CID;
	int abort;
	int dep;
	int delay;
};

class Queue{
	private:
		vector<list> line1;
		vector<list> line2;
	public:
	
	int length() {			//計算queue長度 
		return line1.size();
	} 
	
	int length2() {			//計算queue長度 
		return line2.size();
	} 
	
	bool isEmpty() {		//queue是空的 
		if ( line1.size() == 0 ) {
			return true;
		} 
		else{
			return false; 
		}
	}  	
	
	bool isEmpty2() {		//queue是空的 
		if ( line2.size() == 0 ) {
			return true;
		} 
		else{
			return false; 
		}
	} 
	
	bool isFull() {  		// queue滿了 
		if ( line1.size() == 4 )  {
			return true;
		}
		else{
			return false; 
		}
	} 
	
	bool isFull2() {  		// queue滿了 
		if ( line2.size() == 4 )  {
			return true;
		}
		else{
			return false; 
		}
	} 
	
	void push( list u) {	//在queue新增 
		line1.push_back(u);
	}  
	
	void pop() {			//刪除開頭 
		line1.erase( line1.begin() );
	} 
	
	void run(){				//每經過一秒把處理中的資料的duration減一 
		line1[0].duration--;
	}
	
	void start(int m){		//設置開始處理時間 
		line1[0].head = m;
	}
	
	list getFront() {		//取得開頭 
		return line1[0] ;
	} 
	
	void push2( list u) {	//在queue新增 
		line2.push_back(u);
	}  
	
	void pop2() {			//刪除開頭 
		line2.erase( line2.begin() );
	} 
	
	void run2(){				//每經過一秒把處理中的資料的duration減一 
		line2[0].duration--;
	}
	
	void start2(int m){		//設置開始處理時間 
		line2[0].head = m;
	}
	
	list getFront2() {		//取得開頭 
		return line2[0] ;
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
	
	for(time=txt[0].arrival ; time<=end ; time++){
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

void task3(){
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
	output = "double" + filename + ".txt";	//輸出檔名稱 
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
	
	for(time=txt[0].arrival ; time<=end ; time++){
		if(! call.isEmpty()){	//有處理中的工作的話，每秒都要將它的duration減一 
			call.run();
		}
		if(! call.isEmpty2()){	//有處理中的工作的話，每秒都要將它的duration減一 
			call.run2();
		}
		while( (!call.isFull() || !call.isFull2())&& time==txt[0].arrival && txt.size()!=0){	//當queue還有空間且到了arrival的時間，把資料加到queue中 
			if( (call.length() <= call.length2() ) && !call.isFull()){
				call.push(txt[0]);
				txt.erase( txt.begin() );
				if(call.length()==1){
					job store;
					store.CID = 1;
					oid = call.getFront().timeout-call.getFront().arrival; //計算delay 
					store.OID = call.getFront().OID;
					if((call.getFront().arrival+call.getFront().duration) <= call.getFront().timeout){
						store.delay=0;
						store.dep=call.getFront().arrival+call.getFront().duration;
						done.push_back(store);
					}
					else{
						oid = call.getFront().timeout-call.getFront().arrival; //計算delay 
						if(oid < 0){	//處理時間沒有耽誤到 
							store.delay = 0;
						}
						else{
							store.delay = oid;
						}
						store.abort=call.getFront().timeout;
						abort.push_back(store);
					}	
				} 		
			}
			else {
				call.push2(txt[0]);
				txt.erase( txt.begin() );
				if(call.length2()==1){
					job store;
					store.CID = 2;
					oid = call.getFront2().timeout-call.getFront2().arrival; //計算delay 
					store.OID = call.getFront2().OID;
					if((call.getFront2().arrival+call.getFront2().duration) <= call.getFront2().timeout){
						store.delay=0;
						store.dep=call.getFront2().arrival+call.getFront2().duration;
						done.push_back(store);
					}
					else{
						if(oid < 0){	//處理時間沒有耽誤到 
							store.delay = 0;
						}
						else{
							store.delay = oid;
						}
						store.abort=call.getFront2().timeout;
						abort.push_back(store);
					}	
				}
				
			}
		}
		while( !call.isEmpty() && (time>=call.getFront().timeout||call.getFront().duration==0)){		//目前有在處理的工作且已處理完成或是到timeout 
			call.pop();
			call.start(time);
			if(!call.isEmpty()){
				job store;
				store.CID = 1;
				oid = call.getFront().timeout-call.getFront().arrival; //計算delay 
				if(oid < 0){	//處理時間沒有耽誤到 
					store.delay = 0;
				}
				else{
					store.delay = oid;
				}
				store.OID = call.getFront().OID;
				if((call.getFront().head+call.getFront().duration) <= call.getFront().timeout){
					store.dep=call.getFront().arrival+call.getFront().duration;
					done.push_back(store);
				}
				else{
					store.abort=call.getFront().timeout;
					abort.push_back(store);
				}
			}
			if( txt.size()!=0&&time==txt[0].arrival){
				call.push(txt[0]);
				txt.erase( txt.begin() );
			}
		
		}
		while( !call.isEmpty2() && (time>=call.getFront2().timeout||call.getFront2().duration==0)){		//目前有在處理的工作且已處理完成或是到timeout 
			call.pop2();
			call.start2(time);
			if(!call.isEmpty2()){
				job store;
				store.CID = 2;
				oid = call.getFront2().timeout-call.getFront2().arrival; //計算delay 
				if(oid < 0){	//處理時間沒有耽誤到 
					store.delay = 0;
				}
				else{
					store.delay = oid;
				}
				store.OID = call.getFront2().OID;
				if((call.getFront2().head+call.getFront2().duration) <= call.getFront2().timeout){
					store.dep=call.getFront2().arrival+call.getFront2().duration;
					done.push_back(store);
				}
				else{
					store.abort=call.getFront2().timeout;
					abort.push_back(store);
				}
			}
			if( txt.size()!=0&&time==txt[0].arrival){
				call.push2(txt[0]);
				txt.erase( txt.begin() );
			}
		}
		while(time==txt[0].arrival&&txt.size()!=0){  	//當arrival的時間到了沒進入queue中就加入abort 
			job store;
			store.delay=0;
			store.abort=time;
			store.OID=txt[0].OID;
			store.CID=0;
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
	out << "\t" << "OID	CID	Abort	Delay" << endl;
	for(int i = 0; i<abort.size();i++){
		out <<"[" << i+1 << "]\t"<< abort[i].OID << "\t" << abort[i].CID << "\t" << abort[i].abort << "\t" << abort[i].delay << endl;
	}
	out << "\t" << "[Jobs Done]" << endl;
	out << "\t" << "OID	CID	Departure	Delay" << endl;
	for(int i = 0; i<done.size();i++){
		out <<"[" << i+1 << "]\t"<< done[i].OID << "\t" << done[i].CID << "\t" << done[i].dep << "\t" << done[i].delay << endl;
	}
	out << "[Average Delay] "<< fixed << setprecision(2) << average / total << " ms"<<endl;
	out << "[Success Rate] "<< fixed << setprecision(2) << (done.size() / total)*100<< " %"<<endl;
		
	//輸出 
	cout << endl << "The simulation is running..." << endl;
	cout << "See double" << filename << ".txt" << endl;
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
		cout << "* 3. Simulate two queues by SQF      *" << endl;
		cout << "**************************************" << endl;
		cout << "Input a command(0, 1, 2, 3): " ;
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
		else if ( command == "3" ) {
			task3();
		}
		else{
			cout << endl << "Command does not exist!" << endl;
		}
	}
}
