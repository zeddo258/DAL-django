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
	int arrival ;
	int duration ;
	int timeout ;
	
} jobType ;



ostream& operator<<(ostream& os, const jobType& job) {
    os << job.OID << "\t" << job.arrival << "\t" << job.duration << "\t" << job.timeout;
    return os;
}



class Joblist{
	
	
	public :
			
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
			
			cout << "        OID     Arrival Duration        TimeOut" << endl ;
			
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
		
	/*
		void CreatTxt2( string &filenum ) { //建立任務2檔案 
			
			ofstream newFile;
			string temp ;
			int time = 0 , rate = 0;
			
			temp = "output" + filenum + ".txt"  ;
			
			newFile.open( temp.c_str() ) ;

			
			newFile << "	" << "[Abort Jobs]" << endl ;
			newFile << "	OID	Abort	Delay" << endl ;
			
			for (int i = 0; i < abort.size(); i++) {
        		newFile << "[" << i +1 << "]" << "\t" << abort[i] << endl ;
        		time += abort[i].delay ;
    		}
			
			newFile << "	" << "[Jobs Done]" << endl ;
			newFile << "	OID	Departure	Delay" << endl ;
			
			for (int i = 0; i < done.size(); i++) {
        		newFile << "[" << i +1 << "]" << "\t" << done[i] << endl ;
        		time += done[i].delay ;
    		}
			
			time = time / ( abort.size() + done.size() ) ;
			
			rate = 100 * done.size() / ( done.size + abort.size() ) ;
			
			
			newFile << "[Average Delay] " << time << " ms." ;　 
			newFile << "[Success Rate] " << rate << " %" ;
			
			newFile.close() ;		

		}
		
		*/
		
		int gettime( int n ) {
					
			if( n == 1 )
				return  readtime ; 
			else if( n == 2) 
				return sorttime ;
			else 
				return creattime ;
		}
		
		/*
		bool checkfile( string &filenum ) { //檢查sorted filunum 
			
			ofstream newFile;
			string temp ;
			
			temp = = "sorted" + filenum + ".txt"  ;
			
			newFile.open( temp.c_str() ) ;
			
			if( !File.is_open() ) 
				return false ;		
	
			newFile.close() ;		
			return true ;
			
		}
		
		*/
		
		
};




int main(){
	
	Joblist job ;
	
	int command, text = 0 ;
	string filenum ;
	
	cout << "**** Simulate FIFO Queues by SQF *****" << endl ;
	cout << "* 0. Quit                            *" << endl ; 
	cout << "* 1. Sort a file                     *" << endl ;
	cout << "* 2. Simulate one FIFO queue         *" << endl ;
	cout << "**************************************" << endl ;
	cout << endl << "Input a command(0, 1, 2): "  ;
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
			/*
			if( text == 1 ){
				//	job.CreatTxt2( filenum ) ;
				cout << "The simulation is running..." << endl ;
				cout << "See output" << filenum << ".txt"  << endl << endl ;
				
			}
			else{
				
				cout << endl << "input a file number :" ;
				cin >> filenum ;
				//先檢查有沒有sortedfilenum
				
				if  ( job.checkfile( filenum ) ) {
				
					job.CreatTxt2( filenum ) ;				
					cout << "The simulation is running..." << endl ;
					cout << "See output" << filenum << ".txt"  << endl << endl ;
				}
				else
					cout <<  "### " << temp << " does not exist! ###"  ;
				
			}
			*/
			
		}
		else 
			cout << "Command does not exist!" ;
			
		cout << "**** Simulate FIFO Queues by SQF *****" << endl ;
		cout << "* 0. Quit                            *" << endl ; 
		cout << "* 1. Sort a file                     *" << endl ;
		cout << "* 2. Simulate one FIFO queue         *" << endl ;
		cout << "**************************************" << endl ;
		cout << endl << "Input a command(0, 1, 2): " << endl ;
		cin >> command ;
		
	}
	
	
	
	
} 



 
