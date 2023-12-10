// 11127108 陳柏蓁, 11127161 劉芷妘

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
#include <iomanip> // 用小數點的 

using namespace std ; 

struct Type {
	int oid = -1 ;
	int arrival = -1 ;
	int duration = -1 ;
	int timeout = -1 ;
};

struct TypeA { // 存失敗的 
	int oid = -1;
	int delay = -1;
	int abort = -1;
};

struct TypeD { // 存成功的 
	int oid = -1;
	int delay = -1;
	int Departure = -1;
};

class Queue {
	vector<Type> queue ;
	
public:
		
	int size() {
		return queue.size() ;
	}
	
	bool isEmpty() {
		if ( queue.size() == 0 )
			return true ;
		return false ;
	}
	
	bool isFull() {
		if ( queue.size() == 3 )
			return true ;
		return false ;
	}
	
	void enQueue( Type data ) {
		queue.push_back( data ) ;
	}
	
	void deQueue() {
		queue.erase( queue.begin() ) ;
	}
		
	Type getFront() {
		if ( queue.size() >= 1 )
			return queue[0] ;
	}
};


void ShellSort( vector<Type> &process ) {   // 希爾排序 
	for ( int h = process.size() / 2 ; h > 0 ; h = h / 2 ) {
		for ( int unsort = h ; unsort < process.size() ; unsort++ ) {
			int i = unsort ;
			int oid = process[unsort].oid ;
			int arrival = process[unsort].arrival ;
			int duration = process[unsort].duration ;
			int timeout = process[unsort].timeout ;
			
			// 比大小 
			while( ( i >= h && process[i-h].arrival > arrival ) || 
				   ( i >= h && process[i-h].arrival == arrival && process[i-h].oid > oid ) ) {
				process[i] = process[i-h] ;
				i = i - h ;
			}
			process[i].oid = oid ;
			process[i].arrival = arrival ;
			process[i].duration = duration ;
			process[i].timeout = timeout ;
		}
	}
}

void Sort() {
	vector<Type> process ;
	string fileName ;
	cout << endl << "Input a file number: " ;
	cin >> fileName ;
	
	Type data ;
	string str ;
	string num = "" ; 
	double sread, ssort, swrite ;  // 讀檔、排序、寫檔時間 
	double end ;  // 結束時間 
	
	string input = "input" + fileName + ".txt" ; 
	fstream file ;
	file.open( input.c_str() ) ;  // 開啟名為inputXXX.txt的檔案 
	
	if ( file == NULL ) {  // 找不到檔案 
		cout << endl << "### " << fileName << " does not exist! ###" << endl ; 		
		return ;
	} 
	
	ofstream newFile ;  // 建新檔案sorted 
	string sort = "sorted" + fileName + ".txt";
	newFile.open( sort.c_str() );  // 開啟名為sortedXXX.txt的檔案 
	
	sread = clock() ;  // 開始讀檔 
	getline( file, str ) ;  // 讀標題 
	cout << "	" << str << "\n"; 
	newFile << str << endl ;
	
	while ( getline( file, str ) ) {  // 將工作一筆一筆讀入 
		int i = 0 ;
		while ( i < str.length() ) {
			num = "" ;  //清空 1
			while ( str[i] != '\t' && str[i] != '\n' ) {  // 遇到tab就分類 分四類存進vector 
				num = num + str[i] ;
				i++ ;
			}
		
			// = -1 表示還沒存過 
			if ( data.oid == -1 )
				data.oid = stoi( num.c_str() ) ;
				
			else if ( data.arrival == -1 )
				data.arrival = stoi( num.c_str() ) ;
							
			else if ( data.duration == -1 )		
				data.duration = stoi( num.c_str() ) ;
					
			else if ( data.timeout == -1 ) 
				data.timeout = stoi( num.c_str() ) ;
			i++ ;
		}
		
		process.push_back( data ) ;
		data = {} ;  //初始化 
		str = "" ;
	}
	
	end = clock() ;  // 讀檔結束 
	sread = end - sread ;  // 計算讀檔執行時間  
	for ( int i = 0 ; i < process.size() ; i++ ) {  // 輸出input檔內容 
		cout << "(" << i+1 << ")	" << process[i].oid << "\t" ;
		cout << process[i].arrival << "\t";
		cout << process[i].duration << "\t" ;
		cout << process[i].timeout << endl ;
	}  
	
	ssort = clock() ;  // 開始排序 
	ShellSort( process ) ;  // 希爾排序 
	end = clock() ;  // 排序結束 
	ssort = end - ssort ;  // 計算排序執行時間  

	swrite = clock() ;  // 開始寫檔 
	int i = 0 ;
	while ( i < process.size() ) {  //寫檔 
		newFile << process[i].oid << '\t' ;
		newFile << process[i].arrival << '\t' ;
		newFile << process[i].duration << '\t' ;
		newFile << process[i].timeout << endl ;
		i++ ;
	}
	end = clock() ;   // 寫檔結束 
	swrite = end - swrite ;  // 計算寫檔執行時間 
	
	// 輸出執行時間 
	cout << "Reading data: " << static_cast<int>(sread) << " clocks" << " (" << fixed << setprecision(2) << static_cast<double>(sread) << " ms)." << endl ;
	cout << "Sorting data: " << static_cast<int>(ssort) << " clocks" << " (" << fixed << setprecision(2) << static_cast<double>(ssort) << " ms)." << endl ;
	cout << "Writing data: " << static_cast<int>(swrite) << " clocks" << " (" << fixed << setprecision(2) << static_cast<double>(swrite) << " ms)." << endl ;
	cout << endl << "See " << sort << endl ;
	
	newFile.close() ;
	file.close() ;
} 


void Output () {
	vector<Type> process ;
	string fileName ;
	cout << endl << "Input a file number: " ;
	cin >> fileName ;
	
	vector <Type> alldata ;  // 存排序過的所有資料 
	vector <TypeA> abortlist ;  // 存取消的資料 
	vector <TypeD> donelist ;  // 存成功的資料 
	Type all ;
	TypeA abort ;
	TypeD done ;
	
	int nowtime = 0, totaldelay = 0 ; 
	Queue queue ;
	
	string str ;
	string num = "" ; 
	string sort = "sorted" + fileName + ".txt" ;
	fstream file ;
	file.open( sort.c_str() ) ;  // 開啟名為sortedXXX.txt的檔案 
	
	if ( file.is_open() ) {  // 如果sortedXXX.txt存在 
		getline( file, str ) ;  // 先讀入標題
		
		while ( getline( file, str ) ) {  // 將工作一筆一筆讀入 
			int i = 0 ;
			
			while ( i < str.length() ) {
				num = "" ;  //清空 1
				while ( str[i] != '\t' && str[i] != '\n' ) { // 遇到 tab就分類 分四類存進 vector 
					num = num + str[i] ;
					i++ ;
				}
			
				// = -1 表示還沒存過
				// 從檔案中讀取資料到 all的成員中
				if ( all.oid == -1 )   
					all.oid = stoi( num.c_str() ) ;
					
				else if ( all.arrival == -1 )
					all.arrival = stoi( num.c_str() ) ;
								
				else if ( all.duration == -1 )		
					all.duration = stoi( num.c_str() ) ;
						
				else if ( all.timeout == -1 ) 
					all.timeout = stoi( num.c_str() ) ;
					
				i++ ;
			}
			
			alldata.push_back( all ) ;
			all = {} ; //初始化 
			str = "" ;
			 
		} // while // 將文字檔一行行讀入 
		
		
		nowtime = alldata[0].arrival + alldata[0].duration;  // 現在時間是到達時間加上耗時
		done.oid = alldata[0].oid ;
		done.Departure = nowtime ;
		done.delay = 0 ;
		donelist.push_back( done ) ;
		
		int a = 1; // 從第一項(第二個資料)開始 第一個資料可以直接工作
		while ( a < alldata.size() ) {
			if ( nowtime > alldata[a].arrival && a < alldata.size() ) { // 如果該工作已到達就可以被處理 
				
				if ( !queue.isFull() && nowtime > alldata[a].arrival )  // queue沒有存滿 
					queue.enQueue( alldata[a] ); // 把第a項的四種資料都存進去 
			
				else {  // queue滿了就取消 
					abort.oid = alldata[a].oid;
					abort.delay = 0;
					abort.abort = alldata[a].arrival;  // 取消時刻 = 進入時間 
					abortlist.push_back(abort);
				} // else
				
				a++;
			} // if
			
			else {
				
				if ( !queue.isEmpty() ) {  // queue裡面有待執行的工作 
				
					if ( nowtime > queue.getFront().timeout ) {  // 從 CPU取出時就逾時了  (條件 4-2)
						// 將該工作取消，計算延遲時間，並記錄在取消清單中
						abort.oid = queue.getFront().oid;
						abort.abort = nowtime;
						abort.delay = nowtime - queue.getFront().arrival;  // 延遲時間 = 取消時間 - 新工作抵達時間 
						abortlist.push_back(abort);
						nowtime = abort.abort;  // 現在時間是取消時間 
						totaldelay = totaldelay + abort.delay;
					} // if
					
					else if ( queue.getFront().timeout < nowtime + queue.getFront().duration ) {  // 執行過程中逾時 (條件4-3)
						// 將該工作取消，計算延遲時間，並記錄在取消清單中 
						abort.oid = queue.getFront().oid; 
						abort.abort = queue.getFront().timeout ;
						abort.delay = abort.abort - queue.getFront().arrival;  // 延遲時間 = 取消時間 - 該工作的抵達時間 
						abortlist.push_back(abort);
						nowtime = abort.abort ;  // 現在時間是取消時間 
						totaldelay = totaldelay + abort.delay;		
					} // else if
							
					else {  // 成功執行完成 
						done.oid = queue.getFront().oid;
						done.delay = nowtime - queue.getFront().arrival;  // 開始時刻 - 進入時刻 
						nowtime = nowtime + queue.getFront().duration ; 
						done.Departure = nowtime ;
						donelist.push_back(done);
						totaldelay = totaldelay + done.delay;
					} 
					
					queue.deQueue() ;
				} // if
				
				else {  // queue裡面沒有待執行的工作 --> 可直接執行  
					done.oid = alldata[a].oid;
					done.delay = 0;  // 開始時刻 - 進入時刻 
					nowtime = alldata[a].arrival + alldata[a].duration ; 
					done.Departure = nowtime ;
					donelist.push_back(done);
					a++ ;
					totaldelay = totaldelay + done.delay;					
				} // else			
			}	// else			
		}  // while
		
		
		while( !queue.isEmpty() ) {   
			if ( nowtime > queue.getFront().timeout ) { // 從 CPU取出時就逾時了  (條件 4-2)
				// 將該工作取消，計算延遲時間，並記錄在取消清單中
				abort.oid = queue.getFront().oid;
				abort.abort = nowtime;
				abort.delay = nowtime - queue.getFront().arrival;  // 延遲時間 = 取消時間 - 新工作抵達時間 
				abortlist.push_back(abort);
				nowtime = abort.abort;  // 現在時間是取消時間 
				totaldelay = totaldelay + abort.delay;
			} // if
			else if ( queue.getFront().timeout < nowtime + queue.getFront().duration ) {  // 執行過程中逾時 (條件4-3)
				// 將該工作取消，計算延遲時間，並記錄在取消清單中 
				abort.oid = queue.getFront().oid; 
				abort.abort = queue.getFront().timeout ;
				abort.delay = abort.abort - queue.getFront().arrival;  // 延遲時間 = 取消時間 - 該工作的抵達時間 
				abortlist.push_back(abort);
				nowtime = abort.abort ;  // 現在時間是取消時間 
				totaldelay = totaldelay + abort.delay;						
			} // else if
								
			else {  // 成功執行完成  
				done.oid = queue.getFront().oid;
				done.delay = nowtime - queue.getFront().arrival;  // 開始時刻 - 進入時刻 
				nowtime = nowtime + queue.getFront().duration ; 
				done.Departure = nowtime ;
				donelist.push_back(done);
				totaldelay = totaldelay + done.delay;
			} 
			
			queue.deQueue() ;
		}
		
		ofstream newFile ;  // 建新檔案output
		string output = "output" + fileName + ".txt" ;
		newFile.open( output.c_str() ); // 開啟名為outputXXX.txt的檔案
		
		// 計算平均延遲和成功比例 
		double averagedelay = static_cast<double>( totaldelay ) / static_cast<double>( alldata.size() ) ;
		double donerate = ( static_cast<double>( donelist.size() ) / alldata.size() ) * 100; 
	
		// 寫入output檔 
		newFile << "\t[Abort Jobs]" << endl ;
		newFile << "\tOID" << '\t' << "Abort" << '\t' << "Delay" << endl ;
		for ( int i = 0 ; i < abortlist.size() ; i++ ) { // 直接輸出存在vector的讀入的文字 
			newFile << "[" << i+1 << "]\t";   
			newFile << abortlist[i].oid << "\t" ;
			newFile << abortlist[i].abort << "\t";
			newFile << abortlist[i].delay << endl ;
		}  // for
		
		newFile << "\t[Jobs Done]" << endl;
		newFile << "\tOID" << '\t' << "Departure" << '\t' << "Delay" << endl;
		for ( int i = 0 ; i < donelist.size() ; i++ ) { // 直接輸出存在vector的讀入的文字 
			newFile << "[" << i+1 << "]\t"; 
			newFile << donelist[i].oid << "\t" ;
			newFile << donelist[i].Departure << "\t";
			newFile << donelist[i].delay << endl ;
		}  
		
		newFile << "[Average Delay]\t" << fixed << setprecision(2) << averagedelay << " ms" << endl;
		newFile << "[Success Rate]\t" << donerate << " %" << endl;
		
		cout << endl << "The simulation is running..." << endl ;
		cout << "See " << output << endl ;
		
	}	// if
	
	else  // 找不到檔案 sortedXXX.txt 
		cout << endl << "### " << sort << " does not exist! ###" << endl ;
	
} // Two


int main() {
	string command ;
	//vector<Type> process ;
	
	while ( command != "0" ) { 
		cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
		cout << endl << "* 0. Quit                            *" ;
		cout << endl << "* 1. Sort a file                     *" ;
		cout << endl << "* 2. Simulate one FIFO queue         *" ;
		cout << endl << "**************************************" ;
		cout << endl << "Input a command(0, 1, 2):" ;
		cin >> command ;
		
		if ( command == "0" )
			break ;
			
		else if ( command == "1" ) 
			Sort() ;
	
		else if ( command == "2" )
			Output();
		
		else 
			cout << "\nthis command does not exist";
		
	} 
}

