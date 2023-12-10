// 11127243 李光中
// 11127218 林岑翰 

#include <iostream>  // using namespace 
#include <fstream> //使用檔案 
#include <string>
#include <vector>
#include <sstream> //轉數字 
#include <ctime> //測時間 
#include <iomanip> //setw

using namespace std ;


typedef struct jT{
	
	int OID ;
	int CID ;
	int arrival ;
	int duration ;
	int timeout ;
	int wait ;
	int departure ;
	int worktime;
	
} jobType ;



ostream& operator<< ( ostream& os, const jobType& job) {
    os << job.OID << "\t" << job.arrival << "\t" << job.duration << "\t" << job.timeout;
    return os;
}

template <typename T>
class Queue {
private:
    vector<T> elements;

public:
	
	bool isFull() const {
		return ( elements.size() == 3 );
	}
    bool isEmpty() const {
        return elements.empty();
    }

    void enqueue(const T& item) {
        elements.push_back(item);
    }

    void dequeue() {
        if (!isEmpty()) {
            elements.erase(elements.begin());
        }
    }

    T& front() {
        return elements.front();
    }

    const T& front() const {
        return elements.front();
    }

    int size() const {
        return elements.size();
    }
};

class Joblist{
	
	
	public :
		vector<jobType> done ;
		vector<jobType> abort ;
					
		vector<jobType>nums ;
		int readtime ;
		int sorttime ;
		int creattime ;
		
		
		bool readfile( string &filenum ) {
		
			string temp ;
			string a;
			jobType job ;
			jobType data;
			ifstream File ;
			temp = "input" + filenum + ".txt" ;
	
			
			
			File.open( temp.c_str() ) ;
			
			if( !File.is_open() ) {
		
				cout << "### " << temp << " does not exist! ###"  ;
				return false ;		
			}
			
			string row;
			vector<string> input;
			
			clock_t start = clock(); //讀檔初始時間 
			
			
			getline( File, row )  ; //讀掉第一行 
  			while ( getline( File, row ) ) {
				
  				stringstream ss(row);  
  				while ( getline( ss, a, '\t' ) ) //分割string 
  					input.push_back(a);
  				
  				data.OID = stoi( input[0] ) ; 
  				data.arrival = stoi( input[1] ) ;
  				data.duration = stoi( input[2] ) ;
  				data.timeout = stoi( input[3] ) ;
  				
  				input.clear() ;
                nums.push_back( data );   //nums最終vector 
           		
    		}
    		
    		clock_t stop = clock(); //讀檔結束時間 
			
			readtime = double(stop - start) / CLOCKS_PER_SEC * 1000  ;
			
    		File.close() ;
			return true ;
			
		} 
		
		
		int stoi( string str ) {  //string轉int
		 
			int a = 0; 
 			for ( int b = 0; b < str.length() ; b++ )  
  				a = a *10 + str[ b ] - '0'; 
  		
 			return a; 
 	
		} // stoi
		
		
		void showJob() {
			
			cout << "	OID	Arrival	Duration	TimeOut" << endl ;
			
    		for (int i = 0; i < nums.size(); i++) {
        		
				cout << "(" << i + 1 << ")" << "\t" << nums[i]  << endl ;
    		}
    		
    		cout << endl ; 
		}
	
	
	
		void shellsort( ) {   // shell sort 
			
			int n = nums.size() ;
			
			clock_t start = clock();
			
			for( int gap = n/2 ; gap > 0 ; gap /= 2 ) {
				
				for ( int i = gap  ; i < n ; i++ ) {
					jobType temp = nums[i];
					
					int j;
            		for ( j = i ; j >= gap && ( nums [j - gap ].arrival > temp.arrival || ( nums[ j - gap ].arrival == temp.arrival && nums[ j - gap ].OID > temp.OID ) ) ; j -= gap ) 
                		nums[j] = nums[ j - gap ] ;
            		
            		
            		nums[j] = temp;
				}
				
			}
			
			clock_t stop = clock();
			
			sorttime = double(stop - start) / CLOCKS_PER_SEC * 1000  ;
			 	
		}
		
		
		
		void CreatTxt( string &filenum ) { //建立檔案並寫入  
		
		 //不知道是甚麼原因沒辦法成功建立新的檔案，所以我直接將執行檔輸出在執行檔內。 
																						 
									
			ofstream newFile;
			string temp ;
			
		//	cout << "1" << endl;
			
			temp = "sorted" + filenum + ".txt" ;
			
			clock_t start = clock();

			//newFile.open( "C:\\Users\\88696\\OneDrive\\桌面\\DS作業3\\" + temp ) ;		
			newFile.open( temp.c_str() ) ;
				
					
			newFile << "OID	Arrival	Duration	TimeOut" << endl ;
			
			for (int i = 0; i < nums.size(); i++) {
        		newFile << nums[i] << endl ;
    		}
			
			newFile.close() ;		
			
			clock_t stop = clock();
			
			creattime =  double(stop - start) / CLOCKS_PER_SEC * 1000  ;			

		}
		
		string convertIntToString(int number) {
    		ostringstream oss;
    		oss << number;
    		return oss.str();
}
	
		void CreatTxt2( string &filenum ) { //建立任務2檔案 
			
			ofstream newFile;
			string temp ;
			float time = 0 ;
			float rate = 0;
			
			int total =0;
			
			temp = "output" + filenum + ".txt"  ;
			
			newFile.open( temp.c_str() ) ;

			
			newFile << "\t[Abort Jobs]" << endl ;
			newFile << "\tOID\tAbort\tDelay" << endl ;
			
			
			for (int i = 0; i < abort.size(); i++) {
				
        		newFile << "[" << i + 1 << "]\t" << abort[i].OID << "\t" << abort[i].departure << "\t" << abort[i].wait << endl;
        		time += abort[i].wait ;
        		
    		}
    		
			
			newFile << "\t[Jobs Done]" << endl ;
			newFile << "\tOID\tDeparture\tDelay" << endl ;
			
			
			for (int i = 0; i < done.size(); i++) {
        		newFile << "[" << i + 1 << "]\t" << done[i].OID << "\t" << done[i].departure << "\t" << done[i].wait << endl;
        		time += done[i].wait ;
    		}
			
			total = abort.size() + done.size();
			
			
			time = time / total ;
			
			
			rate = 100 * done.size();
			rate = rate / total;
			
			
			newFile << "[Average Delay]\t" << setprecision(2) << fixed << time << "\tms" << endl ;
			newFile << "[Success Rate]\t" << setprecision(2) << fixed << rate << "\t%" ;
			
			newFile.close() ;		

		}
		
		void CreatTxt3( string &filenum ) { //建立任務3檔案 
			
			ofstream newFile;
			string temp ;
			float time = 0 ;
			float rate = 0;
			
			int total =0;
			
			temp = "double" + filenum + ".txt"  ;
			
			newFile.open( temp.c_str() ) ;

			
			newFile << "\t[Abort Jobs]" << endl ;
			newFile << "\tOID\tAbort\tDelay" << endl ;
			
			
			for (int i = 0; i < abort.size(); i++) {
				
        		newFile << "[" << i + 1 << "]\t" << abort[i].OID << "\t" << abort[i].CID << "\t" << abort[i].departure << "\t" << abort[i].wait << endl;
        		time += abort[i].wait ;
        		
    		}
    		
			
			newFile << "\t[Jobs Done]" << endl ;
			newFile << "\tOID\tDeparture\tDelay" << endl ;
			
			
			for (int i = 0; i < done.size(); i++) {
        		newFile << "[" << i + 1 << "]\t" << done[i].OID << "\t" << done[i].CID << "\t" << done[i].departure << "\t" << done[i].wait << endl;
        		time += done[i].wait ;
    		}
			
			total = abort.size() + done.size();
			
			
			time = time / total ;
			
			
			rate = 100 * done.size();
			rate = rate / total;
			
			
			newFile << "[Average Delay]\t" << setprecision(2) << fixed << time << "\tms" << endl ;
			newFile << "[Success Rate]\t" << setprecision(2) << fixed << rate << "\t%" ;
			
			newFile.close() ;		

		}
		
		
		int gettime( int n ) {
					
			if( n == 1 )
				return  readtime ; 
			else if( n == 2) 
				return sorttime ;
			else 
				return creattime ;
		}
		
		
		bool checkfile( string &filenum ) { //檢查sorted filunum 
			
			ofstream newFile;
			string temp ;
			
			temp = "sorted" + filenum + ".txt"  ;
			
			newFile.open( temp.c_str() ) ;
			
			if( !newFile.is_open() ) 
				return false ;		
	
			newFile.close() ;		
			return true ;
			
		}
		
		
	
		
		
		void work2() {
			bool idle = true ;
			bool throwq = false;
			jobType doRightNow ;
			Queue<jobType> queue ;
			vector<jobType> onWait ;
			int restTime ;
			

			
		
			
			for(int time = 0 ; !idle || !nums.empty() || !queue.isEmpty() ; time++ ) {  //紀錄時間，當沒有工作在進行，且queue和工作列表也沒東西的時候，停止迴圈 
				throwq = false;
				
				if( !idle ) {                         // 當工作正在進行時，以減少的方式進行執行工作時間的紀錄 
					if ( throwq == false ){
						restTime -- ;
					}
					
					if ( restTime == 0 ) {                    // 工作做完放入done list 
						doRightNow.departure = time ;
						done.push_back(doRightNow) ;
						idle = true ;
					}
					else if(  doRightNow.timeout <= time  ) {             // 如果執行逾時 
						doRightNow.departure =  time;
						abort.push_back(doRightNow)  ;
						idle = true ;
					}
				} // if
				
				while ( !queue.isEmpty() && idle ) {                        // 當現在CPU是閒置的，且queue不是空的 
					
					doRightNow = queue.front() ;                            // queue取出工作<開始執行
					 
					restTime = doRightNow.duration ;
					doRightNow.worktime = time;
					// cout << doRightNow.duration << "	" << doRightNow.OID << endl ;
					queue.dequeue() ;
					idle = false ;
					throwq = true;
					
					
					if( doRightNow.timeout <= time ) {         // 取出時已逾時 
						doRightNow.departure =  time;
						abort.push_back(doRightNow)  ;
						idle = true ;
					}
				} 
				
				
				while ( !nums.empty() && time == nums[0].arrival ) { // 要不要補東西到QUEUE 
					nums[0].wait = 0 ;
					nums[0].departure = 0 ;
					if ( queue.size() < 3 ) {                        // 檢查QUEUE是否 滿了 
						if(  time > nums[0].timeout  ) {             // 進入queue前已經timeout 
							nums[0].departure = time ;
							abort.push_back(nums[0]) ;
						}
						else
							
							queue.enqueue( nums[0] ) ;				//  加入queue	
					}
					else {
						
						nums[0].departure = time ;                       // queue已滿，放入abort 
						abort.push_back(nums[0]) ;
					}
					
					
					nums.erase(nums.begin()) ;                // 處理完工作列表中的第一筆 
					
					while ( !queue.isEmpty() && idle ) {             // 當現在CPU是閒置的，且queue不是空的 

						doRightNow = queue.front() ;                       // queue取出工作<開始執行 
						restTime = doRightNow.duration ;
						queue.dequeue() ;
						doRightNow.worktime = time;
						idle = false ;
						throwq = true;
						
						
						
						if( doRightNow.timeout <= time ) {               // 取出時已逾時 
							doRightNow.departure =  time;
							abort.push_back(doRightNow)  ;
							idle = true ;
						
						}
					}
					
				} //while
				
				
				
				
				
			} // for() 
			for(int i = 0 ; i < done.size() ; i++ ) {                    // 計算delay 
				done[i].wait = done[i].worktime - done[i].arrival;
 			}			
 			
 			for(int i = 0 ; i < abort.size() ; i++ ) {                   // 計算delay 
				abort[i].wait = abort[i].departure - abort[i].arrival;
 			}	
			
				
			
			
				
		} // work2
		
		void work3() {
			bool idle1 = true ;
			bool idle2 = true ;
			bool throwq1 = false;
			bool throwq2 = false;
			jobType doRightNow1 ;
			jobType doRightNow2 ;
			Queue<jobType> queue1 ;
			Queue<jobType> queue2 ;
			
			int restTime1 ;
			int restTime2 ;
			

			
		
			
			for(int time = 0 ; !idle1 || !idle2 || !queue1.isEmpty() || !queue2.isEmpty() || !nums.empty() ; time++ ) {
				throwq1 = false;
				throwq2 = false;
				
				if( !idle1 ) {                  // cpu1在進行中
				 
					if ( throwq1 == false ){
						restTime1 -- ;
					}
					
					if ( restTime1 == 0 ) {      // cpu1執行成功 
						doRightNow1.departure = time ;
						done.push_back(doRightNow1) ;
						idle1 = true ;
					}
					else if(  doRightNow1.timeout <= time  ) {     // cpu1執行逾時 
						doRightNow1.departure =  time;
						abort.push_back(doRightNow1)  ;
						idle1 = true ;
					}
				} // if
				
				if( !idle2 ) {                        // cpu2在進行中 
					if ( throwq2 == false ){
						restTime2 -- ;
					}
					
					if ( restTime2 == 0 ) {            // cpu2執行成功  
						doRightNow2.departure = time ;
						done.push_back(doRightNow2) ;
						idle2 = true ;
					}
					else if(  doRightNow2.timeout <= time  ) {                // cpu1執行逾時 
						doRightNow2.departure =  time;
						abort.push_back(doRightNow2)  ;
						idle2 = true ;
					}
				} // if
				
				while ( !queue1.isEmpty() && idle1 ) {                       // 當現在CPU1是閒置的，且queue1不是空的 
					
					doRightNow1 = queue1.front() ;                           // 從queue1取出，開始執行 
					restTime1 = doRightNow1.duration ;
					doRightNow1.worktime = time;
					
					queue1.dequeue() ;
					idle1 = false ;
					throwq1 = true;
					
					
					if( doRightNow1.timeout <= time ) {                         // 取出已逾時 
						doRightNow1.departure =  time;
						abort.push_back(doRightNow1)  ;
						idle1 = true ;
					}
				} 
				
				while ( !queue2.isEmpty() && idle2 ) {                           // 當現在CPU2是閒置的，且queue2不是空的 
					
					doRightNow2 = queue2.front() ;                                   // 從queue1取出，開始執行  
					restTime2 = doRightNow2.duration ;
					doRightNow2.worktime = time;
				
					queue2.dequeue() ;
					idle2 = false ;
					throwq2 = true;
					
					
					if( doRightNow2.timeout <= time ) {                          // 取出已逾時 
						doRightNow2.departure =  time;
						abort.push_back(doRightNow2)  ;
						idle2 = true ;
					}
				} 
				
				while ( !nums.empty() && time == nums[0].arrival ) { // 要不要補東西到QUEUE 
					nums[0].wait = 0 ;
					nums[0].departure = 0 ;
					
					if ( queue1.isFull() && queue2.isFull() ){        // 判斷兩個queue是不是都滿的 
						nums[0].CID = 0 ;
						nums[0].departure = time ;
						abort.push_back(nums[0]) ;
					}
					else if ( queue1.size() <= queue2.size() ){                    // 判斷哪個queue比較小 
						if ( queue1.size() < 3 ) {                                         // queue1不是滿的 
							if(  time > nums[0].timeout  ) {                       // 判斷是不是逾時 
								nums[0].CID = 0 ;
								nums[0].departure = time ;
								abort.push_back(nums[0]) ;
							}
							else {
							
								nums[0].CID = 1 ;
								queue1.enqueue( nums[0] ) ;					// 放入queue1 
							}
						}
						else {                                               // 已滿，丟到abort 
							nums[0].CID = 0 ;
							nums[0].departure = time ;
							abort.push_back(nums[0]) ;
						}
					
					
						nums.erase(nums.begin()) ;                             // 刪除工作列表的工作 
						while ( !queue1.isEmpty() && idle1 ) {                   // 當現在CPU1是閒置的，且queue1不是空的 

							doRightNow1 = queue1.front() ;                               // 從queue1取出，開始執行  
							restTime1 = doRightNow1.duration ;
							queue1.dequeue() ;
							doRightNow1.worktime = time;
							idle1 = false ;
							throwq1 = true;
						
						
						
							if( doRightNow1.timeout <= time ) {                    // 取出已逾時   
								doRightNow1.departure =  time;
								abort.push_back(doRightNow1)  ;
								idle1 = true ;
						
							}
						}
					}
					
					
					else {
						if ( queue2.size() < 3 ) {                                    // queue2不是滿的 
							if(  time > nums[0].timeout  ) {                            // 判斷是不是逾時 
								nums[0].CID = 0 ;
								nums[0].departure = time ;
								abort.push_back(nums[0]) ;
							}
							else
								nums[0].CID = 2 ;{                                    // 放入queue2
								 
								queue2.enqueue( nums[0] ) ;					
							}
						}
						else {
							nums[0].CID = 0 ;                                      // 滿了，放入abort 
							nums[0].departure = time ;
							abort.push_back(nums[0]) ;
						}
					
				
						nums.erase(nums.begin()) ;                               // 刪除工作列表的工作
					
						while ( !queue2.isEmpty() && idle2 ) {                              // 當現在CPU2是閒置的，且queue2不是空的

							doRightNow2 = queue2.front() ;                                    // 從queue2取出，開始執行 
							restTime2 = doRightNow2.duration ;
							queue2.dequeue() ;
							doRightNow2.worktime = time;
							idle2 = false ;
							throwq2 = true;
						
						
						
							if( doRightNow2.timeout <= time ) {                       // 取出已逾時 
								doRightNow2.departure =  time;
								abort.push_back(doRightNow2)  ;
								idle2 = true ;
						
							}
						}
					}
					
					
					
					
				} //while
				
				
				
				
				
			} // for() 
			for( int i = 0 ; i < done.size() ; i++ ) {
				done[i].wait = done[i].worktime - done[i].arrival;
 			}			
 			
 			for( int i = 0 ; i < abort.size() ; i++ ) {
				abort[i].wait = abort[i].departure - abort[i].arrival;
 			}	
			
					
				
			
			
				
		} // work3
		
			
		
		
		
};




int main(){
	
	Joblist job ;
	
	int command, text  ;
	string temp ;
	string filenum ;
	ifstream File ;
	
	cout << "**** Simulate FIFO Queues by SQF *****" << endl ;
	cout << "* 0. Quit                            *" << endl ; 
	cout << "* 1. Sort a file                     *" << endl ;
	cout << "* 2. Simulate one FIFO queue         *" << endl ;
	cout << "* 3. Simulate two queues by SQF      *" << endl ;
	cout << "**************************************" << endl ;
	cout << endl << "Input a command(0, 1, 2, 3): "  ;
	cin >> command ;
	
	while( command != 0 ) {
		
		if( command == 1 ) {
			
			text = 1 ;
			
			cout << endl << "input a file number :" ;
			cin >> filenum ;	
			if ( job.readfile( filenum ) ) {
			
				job.showJob() ;
				job.shellsort() ;
				//job.showJob() ;
				job.CreatTxt( filenum ) ;
				
				cout << "Reading data: " << job.gettime(1) << " clocks (" << job.gettime(1) << ".00 ms)." << endl ;
				cout << "Sorting data: " << job.gettime(2) << " clocks (" << job.gettime(2) << ".00 ms)." << endl ;	
				cout << "Writing data: " << job.gettime(3) << " clocks (" << job.gettime(3) << ".00 ms)." << endl << endl ;
				
				cout << "See sorted" << filenum << ".txt"  << endl << endl ;
			}
				
		}
		else if ( command == 2 ) {
			
			if( text == 1 ){
				job.work2();
				
				job.CreatTxt2( filenum ) ;
				
				job.abort.clear();
				job.done.clear();
				
				cout << "The simulation is running..." << endl ;
				cout << "See output" << filenum << ".txt"  << endl << endl ;
				
			}
			else{
				
				cout << endl << "input a file number :" ;
				cin >> filenum ;
				//先檢查有沒有sortedfilenum
				
				if  ( job.checkfile( filenum ) ) {
					job.work2();
					job.CreatTxt2( filenum ) ;		
					job.abort.clear();
					job.done.clear();		
					cout << "The simulation is running..." << endl ;
					cout << "See output" << filenum << ".txt"  << endl << endl ;
				}
				else
					cout <<  "### " << temp << " does not exist! ###"  ;
				
			}
			
			
		}
		
		else if ( command == 3 ) {
			
			if( text == 1 ){
				job.work3();
				
				job.CreatTxt3( filenum ) ;
				
				job.abort.clear();
				
				job.done.clear();
				
				
				cout << "The simulation is running..." << endl ;
				cout << "See double" << filenum << ".txt"  << endl << endl ;
				
			}
			else{
				
				cout << endl << "input a file number :" ;
				cin >> filenum ;
				//先檢查有沒有sortedfilenum
				
				if  ( job.checkfile( filenum ) ) {
					job.work3();
					job.CreatTxt3( filenum ) ;		
					job.abort.clear();
					
					job.done.clear();
							
					cout << "The simulation is running..." << endl ;
					cout << "See double" << filenum << ".txt"  << endl << endl ;
				}
				else
					cout <<  "### " << temp << " does not exist! ###"  ;
				
			}
			
			
		}
		else 
			cout << "Command does not exist!" ;
			
			
		cout << "**** Simulate FIFO Queues by SQF *****" << endl ;
		cout << "* 0. Quit                            *" << endl ; 
		cout << "* 1. Sort a file                     *" << endl ;
		cout << "* 2. Simulate one FIFO queue         *" << endl ;
		cout << "* 3. Simulate two queues by SQF      *" << endl ;
		cout << "**************************************" << endl ;
		cout << endl << "Input a command(0, 1, 2, 3): "  ;
		cin >> command ;
		
	}
	
	
	
	
} 



 
