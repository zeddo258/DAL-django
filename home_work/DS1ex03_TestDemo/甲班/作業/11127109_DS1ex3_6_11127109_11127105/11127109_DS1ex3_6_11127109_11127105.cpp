
// 資工二甲 
// 11127105毛起歆 
// 11127109魏盈姍 

# include <stdio.h>
# include <stdlib.h> 
# include <string.h>  
# include <iostream> // cout, endl
# include <fstream>  // open, is_open
# include <vector>   // vector
# include <sstream>  // stringstream
# include <ctime>
# include <iomanip>
# include <iomanip>  // float
using namespace std;



struct datatype {
	
	int OID ;
	int Arrival ;
	int Duration ;
	int TimeOut ; 
	
};


struct Abort {
	
	int OID ;
	int Abort ;
	int Delay ;
	 	
};


struct Done {
	
	int OID ;
	int Departure ;
	int Delay ;
	
};






class Queue {
	
	
	private :
		vector < datatype > queue ;
	
	
	public :


		bool isEmpty() {  // 判斷是否為空 
			
			if ( queue.empty() )
				return true ;
			else
				return false ;
					
		} // isEmpty()

	
		void enqueue( vector< datatype > data, int index ) {  // 新增到最後 

			queue.push_back( data[index] ) ;
			
		} // enqueue()
	
	
	
		void pop() {  // 刪掉第一個 
	
			queue.erase( queue.begin() ) ;	
			
		} // pop()




		int getFront_OID() {   //回傳第一組資料的OID 
			
			return queue[0].OID ;
			
		} // getFront()
		
		
		int getFront_Arrival() {   //回傳第一組資料的Arrival 
			
			return queue[0].Arrival ;
			
		} // getFront()
		
		
		int getFront_Duration() {  //回傳第一組資料的Duration
			
			return queue[0].Duration ;
			
		} // getFront()		
		
		
		int getFront_TimeOut() {  //回傳第一組資料的TimeOut
			
			return queue[0].TimeOut ;
			
		} // getFront()	
		



        bool isFull() {  // 判斷queue是否存放超過三個工作列 
        	
        	if ( queue.size() == 3 )
        		return true ;
        	else 
        		return false ;
        	
		} // isFull
	

};




	
bool isBack(  vector< datatype > data, int index ) {  // 判斷是否就是最後一個 
	
	if ( index == data.size() - 1 )
		return true ;
	else				
		return false ;
	
} // isBack()




void read_sortedfile( vector< datatype > data,  Queue & queue, vector< Abort > & abort, vector< Done > & done, string filename ) {  // 單個CPU 
	

	bool have_same_Arrival = false ;  // 判斷是否有相同到達時間 
	Abort temp_abort ;
	Done temp_done ;


	int i = 0 ;
	int index_abort = 0 ;
	int index_done = 0 ;
	
	int last_done_Departure  = 0 ;
	int last_queue_Arrival = 0 ;
	int last_done_timeout = 0 ;
	
	int OID ;
	int Arrival ;
	int Duration ;
	int TimeOut ; 




	if ( queue.isEmpty() ) {  // 若queue為空則直接執行第一筆資料並存入後兩筆資料
	

		temp_done.OID = data[0].OID ;
		temp_done.Departure = data[0].Arrival + data[0].Duration ;
		temp_done.Delay = 0 ;
		done.push_back( temp_done ) ;

		last_done_Departure = temp_done.Departure ;
		i++ ;

		if ( i < data.size() ) {
			
			if ( !isBack( data, i ) ) { // 若還沒遍歷到data的最後一個資料 則持續新增資料進queue中  	

				while ( !queue.isFull() ) {
					
					last_queue_Arrival = data[i].Arrival ;   // 記住上一個佇列存的Arrival 
					queue.enqueue( data, i ) ;
					i++ ;
					
				} // while
				
				
				if ( queue.isFull() ) {    
				
					while (  last_queue_Arrival = data[i].Arrival && data[i].Arrival < last_done_Departure ) {  // 佇列已滿且CPU正在執行 
					
						temp_abort.OID =  data[i].OID ;
						temp_abort.Abort = data[i].Arrival ;
						temp_abort.Delay = 0 ;		
						abort.push_back( temp_abort ) ;
					
						i++ ;
						have_same_Arrival = true ;
					} // while
					
				
				
				} // if
				
				
				
				
			} // if
			

		} // if
	
	
	} // if	





	while ( queue.isEmpty() == false ) {   // 迴圈跑至queue為空才停止 
		


		// 取得佇列中第一個的資料 
		OID = queue.getFront_OID() ;
		Arrival = queue.getFront_Arrival() ;
		Duration = queue.getFront_Duration() ;
		TimeOut = queue.getFront_TimeOut() ;
		
		
		if ( last_done_Departure > TimeOut ) {   // 4.3從queue中取出時發現已逾時
			
			
			temp_abort.OID =  OID ;
			temp_abort.Abort = last_done_Departure ;
			temp_abort.Delay = last_done_Departure  - Arrival ;		
			abort.push_back( temp_abort ) ;
			queue.pop() ;

			
		} // if
		else if (  last_done_Departure + Duration > TimeOut ) {   // 4.4CPU執行過程中發生逾時 
		
			temp_abort.OID =  OID ;
			temp_abort.Abort = TimeOut ;
			temp_abort.Delay = TimeOut  - Arrival ;		
			abort.push_back( temp_abort ) ;
			last_done_Departure = TimeOut ;
			queue.pop() ;
			
		} // if
		else {
			

			temp_done.OID = OID ;
			
			if ( last_done_Departure > Arrival ) {   // 若上一資料執行完成時間超過目前資料到達時間 
				
				temp_done.Departure = last_done_Departure + Duration ;
				temp_done.Delay = last_done_Departure - Arrival ;
				
			} // if
			else {
				
				temp_done.Departure = Arrival + Duration ;	
				temp_done.Delay = 0 ;
				
			} // else

				
				

			done.push_back( temp_done ) ;

			last_done_Departure = temp_done.Departure ;
			last_done_timeout = data[i].TimeOut ;
			queue.pop() ;

			
		} // else
			
			
		have_same_Arrival = false ;	
		if ( i < data.size() ) {
			
			if ( !isBack( data, i ) ) { // 若還沒遍歷到data的最後一個資料 則持續新增資料進queue中  	


					
				last_queue_Arrival = data[i].Arrival ;  // 記住上一個佇列存的Arrival 
				queue.enqueue( data, i ) ;
				
				
				if ( queue.isFull() ) {
				
					while ( !isBack( data, i ) && data[i+1].Arrival < last_done_Departure ) {
					
						temp_abort.OID =  data[i+1].OID ;
						temp_abort.Abort = data[i+1].Arrival ;
						temp_abort.Delay = 0 ;		
						abort.push_back( temp_abort ) ;
					
						i++ ;
						have_same_Arrival = true ;
					} // while	
					
					
				
				} // if


					
				
			} // if
			else {
			
				if ( data[i].Arrival < last_done_Departure ) {
					
					temp_abort.OID =  data[i].OID ;
					temp_abort.Abort = data[i].Arrival ;
					temp_abort.Delay = 0 ;		
					abort.push_back( temp_abort ) ;
					
				} // if
				else {
					queue.enqueue( data, i ) ;
				} // else
			
			
			
			} // else
			
//			if ( have_same_Arrival == false )
			i++ ;
		} // if


		
	} // while
		
	cout << "The simulation is running...\n" ;
	cout << "See output" << filename << ".txt\n" ; 		

//檢測讀入的檔案是否正確 

//	cout << "OID" << '\t' << "Abort" << '\t' << "Delay" << "\n" ;
//    for ( int x = 0 ; x < abort.size() ; x++ ) {		
//
//		cout << abort[x].OID << '\t' << abort[x].Abort << '\t' << abort[x].Delay << "\n" ;            
//             
//    } // for	
//	
//	
//	cout << "\n" ;
//	
//	cout << "OID" << '\t' << "Departure" << '\t' << "Delay" << "\n" ;
//    for ( int x = 0 ; x < done.size() ; x++ ) {		
//
//		cout << done[x].OID << '\t' << done[x].Departure << '\t' << done[x].Delay << "\n" ;  
//
//    } // for		
	
		
	
	
} // read_sortedfile()






void write_outputfile ( vector< datatype > data, vector< Abort > abort, vector< Done > done , string outpurname ) {   //創建一個文件檔 (寫檔 ) 
	 

	double i = 0 ; 
	double j = 0 ;
	double Delay_sum = 0 ;
	double Average_Delay = 0 ;
	double Success_Rate = 0 ;

	ofstream newfile;
	newfile.open( outpurname.c_str() );
	
	newfile << "\t" << "[Abort Jobs]" << "\n" ;	
	newfile << "\t" << "OID" << "\t" << "Abort" << "\t" << "Delay" << "\n" ;
	
	for ( i = 0 ; i < abort.size() ; i++ ) {
		
		newfile << "[" << i+1 << "]" << "\t" << abort[i].OID << "\t" << abort[i].Abort << "\t" << abort[i].Delay << "\n" ;
		Delay_sum = Delay_sum + abort[i].Delay ;
		
	} // for
	
	
	newfile << "\n" ;
	newfile << "\t" << "[Jobs Done]" << "\n" ;	
	newfile << "\t" << "OID" << "\t" << "Departure" << "\t" << "Delay" << "\n" ;
	for ( j = 0 ; j < done.size() ; j++ ) {
		
		newfile << "[" << j+1 << "]" << "\t" << done[j].OID << "\t" << done[j].Departure << "\t" << done[j].Delay << "\n" ;
		Delay_sum = Delay_sum + done[j].Delay ;
		
	} // for	
	
	


	
		
	Average_Delay = Delay_sum / data.size()  ;
	Success_Rate = j / data.size() * 100 ;
	
	
	newfile << "[Average Delay] " << fixed << setprecision(2) << Average_Delay << " ms" << "\n" ;	
	newfile << "[Success Rate]  " << fixed << setprecision(2) << Success_Rate << " %" << "\n" ;	
	


	newfile.close();
	
} // write_sortedfile()




void sort_abort( vector< Abort > & abort ) {  // 使用希爾排序排列abort 
	


	int k = abort.size() / 2 ;
	
	while ( k > 0 ) {
		
		for (int i = 0 ; i < abort.size() ; i++ ) {
			
			for ( int j = 0 ; j < ( abort.size() - k ) ; j++ ) {
			
				if (abort[j].Abort > abort[j+k].Abort)   //依照訂單到達時間排序 
					swap(abort[j],abort[j+k]);
					
				else if ( abort[j].Abort == abort[j+k].Abort && abort[j].Delay < abort[j+k].Delay )	// 若訂單到達時間相同則判斷延遲時間大小排序 
					swap(abort[j],abort[j+k]);
			
				else if (abort[j].Abort == abort[j+k].Abort && abort[j].Delay == abort[j+k].Delay ) {    // 若皆一樣則依照訂單編號排序 
				
					if ( abort[j].OID > abort[j+k].OID) 
						swap(abort[j],abort[j+k]);
				} // if
				
			} // for
			
			
		} // for
		
		k = k / 2 ;
	} // while	


	
	
} // sort_abort()




void sort_done( vector< Done > & done ) {   // 使用希爾排序排列done
	


	int k = done.size() / 2 ;
	
	while ( k > 0 ) {
		
		for (int i = 0 ; i < done.size() ; i++ ) {
			
			for ( int j = 0 ; j < ( done.size() - k ) ; j++ ) {
			
				if (done[j].Departure > done[j+k].Departure)   //依照訂單到達時間排序 
					swap(done[j],done[j+k]);
					
				else if ( done[j].Departure == done[j+k].Departure && done[j].Delay < done[j+k].Delay )	 // 若訂單到達時間相同則判斷延遲時間大小排序 
					swap(done[j],done[j+k]);
			
				else if ( done[j].Departure == done[j+k].Departure && done[j].Delay == done[j+k].Delay ){    //若皆一樣則依照訂單編號排序 
				
					if ( done[j].OID > done[j+k].OID) 
						swap(done[j],done[j+k]);
				} // if
				
			} // for
			
			
		} // for
		
		k = k / 2 ;
	} // while	


	
	
} // sort_done()



double rtime, stime, wtime ;
int rclock, sclock, wclock ;


bool read_datafile( string filename, vector< datatype > & data, int command ) {  // 讀檔 
	
	
    clock_t start = clock();

	char Filename[100] ;
	 
	filename = "input" + filename + ".txt" ;  // 檔案名稱是字串 
	strcpy( Filename, filename.c_str() ) ; 
	ifstream file (Filename) ;   // ifstream用來宣告檔案 ; file是檔案名稱 


	
	if ( !file.is_open() ) {  // 判斷檔案是否能夠開啟
	
        cout << endl << "### " << filename << " does not exist! ###" << endl ;
        return false ;

        
	} // if
	else {	// 若成功打開則將檔案數據存入vector二維陣列中 
	
	
		string inputline = "\0" ;
		getline( file, inputline ) ;  // 忽略第一行title的部分 
		
					
		while ( getline( file, inputline ) ) {  
		
			
			stringstream ss( inputline ) ;
			datatype temp ;
			ss >> temp.OID >> temp.Arrival >> temp.Duration >> temp.TimeOut ;  // 使用 stringstream將整行內容分割為整數
			data.push_back( temp ) ;   //將讀取到的數據存入 vector  



		} // while
		
		



		if ( command == 1 ) {
			
			cout << "OID" << '\t' << "Arrival" << '\t' << "Duration" << '\t' << "TimeOut" << "\n" ;
		    for ( int x = 0 ; x < data.size() ; x++ ) {		
	
			    cout << data[x].OID << '\t' << data[x].Arrival << '\t' ;
				cout << data[x].Duration << '\t' <<  data[x].TimeOut << "\n" ;                
		             
		    } // for
	
		} // if

		
		clock_t stop = clock() ;
		rclock = stop - start ;
		rtime = static_cast<double>(stop - start) / CLOCKS_PER_SEC * 1000.0 ;
		
		
		file.close() ;
		return true ;
				
	} // else	
	
	
	
} // read_datafile()





void shell_sort( vector< datatype > & data ) {
	
	clock_t start = clock();

	int k = data.size() / 2 ;
	
	while ( k > 0 ) {
		
		for (int i = 0 ; i < data.size() ; i++ ) {
			
			for ( int j = 0 ; j < ( data.size() - k ) ; j++ ) {
			
				if (data[j].Arrival > data[j+k].Arrival)   //依照訂單到達時間排序 
					swap(data[j],data[j+k]);
			
				else if (data[j].Arrival == data[j+k].Arrival ){    //若到達時間一樣則依照訂單編號排序 
				
					if ( data[j].OID > data[j+k].OID) 
						swap(data[j],data[j+k]);
				} // if
				
			} // for
			
			
		} // for
		
		k = k / 2 ;
	} // while	

	
	clock_t stop = clock() ;
	sclock = stop - start ;
	stime = static_cast<double>(stop - start) / CLOCKS_PER_SEC * 1000.0 ;
	
	
} // shell_sort()





void write_sortedfile ( vector< datatype > data , string name ) {   //創建一個文件檔 (寫檔 ) 
	 
    clock_t start = clock();

	ofstream newfile;
	newfile.open( name.c_str() );
	newfile << "\t" << "OID" << "\t" << "Arrival" << "\t" << "Duration" << "\t" << "TimeOut" << "\n" ;
	
	for ( int i = 0 ; i < data.size() ; i++ ) {
		
		newfile << "(" << i+1 << ")" << "\t" << data[i].OID << "\t" << data[i].Arrival << "\t" << data[i].Duration << "\t" << data[i].TimeOut << "\n" ;
		
	} // for
	

	newfile.close();
	
	clock_t stop = clock() ;
	wclock = stop - start ;
	wtime = static_cast<double>(stop - start) / CLOCKS_PER_SEC * 1000.0 ;
	
} // write_sortedfile()
	





void print_time( vector< datatype > data, string newname ) {
	
	
	cout << "\n" ;
	cout << "Reading data: " << rclock << " clocks ( " << fixed << setprecision(2) << rtime << " ms).\n" ;
	cout << "Sorting data: " << sclock << " clocks ( " << fixed << setprecision(2) << stime << " ms).\n" ;
	cout << "Writing data: " << wclock << " clocks ( " << fixed << setprecision(2) << wtime << " ms).\n" ;	
	cout << "\n" ;
	
	cout << "See " << newname << "\n"  ;
	
} // print_time()












int main() {
	
	bool hasrun = false ;
	int command = 0 ;
	vector< datatype > data ;
	vector< Abort > abort ;
	vector< Done > done ;
	Queue queue ;
	string filename ;
	string newname ;
	string outputname ; 



	do {


	    cout << "\n" ;
	 	cout << "**** Simulate FIFO Queues by SQF *****" << "\n" ;
		cout << "* 0. Quit                            *" << "\n" ;
		cout << "* 1. Sort a file                     *" << "\n" ;
		cout << "* 2. Simulate one FIFO queue         *" << "\n" ;
		cout << "**************************************" << "\n" ;
		cout << "Input a command(0, 1, 2): "  ;
		cin >> command ;          // 給使用者輸入指令
		cout << "\n" ;

		
		
		if ( command == 0 )
			break ;
			
		else if ( command == 1 ) {
			
			cout << "Input a file number(ex: 301, 302...) :"  ;
			cin >> filename ;
			cout << "\n" ;
			
			if ( read_datafile( filename,  data, 1 ) ) {  // 讀檔並印出未排序的檔案 
			
				shell_sort( data ) ;			// 將檔案希爾排序 
				newname = "sorted" + filename + ".txt" ;  // 檔案名稱是字串 
				write_sortedfile ( data , newname ) ;  // 寫檔 
				print_time( data, newname ) ;    //印出原本的檔案 
				
		    } // if
			
			data.clear() ;
						
		} // if
		else if ( command == 2 ) {
			

			cout << "Input a file number(ex: 301, 302...) :"  ;
			cin >> filename ;
			cout << "\n" ;
			
			if ( read_datafile( filename,  data, 2 ) ) {  // 讀檔 
			
				shell_sort( data ) ;			// 將檔案希爾排序 
				read_sortedfile( data, queue, abort, done, filename ) ;
				
				outputname = "output" + filename + ".txt" ;  // 檔案名稱是字串
				
				sort_abort( abort ) ;
				sort_done( done ) ;
							
				write_outputfile ( data, abort, done, outputname ) ;
		
		    } // if

			// 清除陣列中資料 
			data.clear() ;
			done.clear() ;
			abort.clear() ;   
			
			
		} // if 
		else {
            cout << endl << "Command does not exist!" << "\n" ;			
			break ;
		} // else

		
		
		
	} while( true ) ;
	
	
	
	
} // main()
