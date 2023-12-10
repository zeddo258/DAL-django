// 11127243 ������
// 11127218 �L�¿� 

#include <iostream>  // using namespace 
#include <fstream> //�ϥ��ɮ� 
#include <string>
#include <vector>
#include <sstream> //��Ʀr 
#include <ctime> //���ɶ� 
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
			
			clock_t start = clock(); //Ū�ɪ�l�ɶ� 
			
			
			getline( File, row )  ; //Ū���Ĥ@�� 
  			while ( getline( File, row ) ) {
				
  				stringstream ss(row);  
  				while ( getline( ss, a, '\t' ) ) //����string 
  					input.push_back(a);
  				
  				data.OID = stoi( input[0] ) ; 
  				data.arrival = stoi( input[1] ) ;
  				data.duration = stoi( input[2] ) ;
  				data.timeout = stoi( input[3] ) ;
  				
  				input.clear() ;
                nums.push_back( data );   //nums�̲�vector 
           		
    		}
    		
    		clock_t stop = clock(); //Ū�ɵ����ɶ� 
			
			readtime = double(stop - start) / CLOCKS_PER_SEC * 1000  ;
			
    		File.close() ;
			return true ;
			
		} 
		
		
		int stoi( string str ) {  //string��int
		 
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
		
		
		
		void CreatTxt( string &filenum ) { //�إ��ɮרüg�J  
		
		 //�����D�O�ƻ��]�S��k���\�إ߷s���ɮסA�ҥH�ڪ����N�����ɿ�X�b�����ɤ��C 
																						 
									
			ofstream newFile;
			string temp ;
			
		//	cout << "1" << endl;
			
			temp = "sorted" + filenum + ".txt" ;
			
			clock_t start = clock();

			//newFile.open( "C:\\Users\\88696\\OneDrive\\�ୱ\\DS�@�~3\\" + temp ) ;		
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
	
		void CreatTxt2( string &filenum ) { //�إߥ���2�ɮ� 
			
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
		
		void CreatTxt3( string &filenum ) { //�إߥ���3�ɮ� 
			
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
		
		
		bool checkfile( string &filenum ) { //�ˬdsorted filunum 
			
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
			

			
		
			
			for(int time = 0 ; !idle || !nums.empty() || !queue.isEmpty() ; time++ ) {  //�����ɶ��A��S���u�@�b�i��A�Bqueue�M�u�@�C��]�S�F�誺�ɭԡA����j�� 
				throwq = false;
				
				if( !idle ) {                         // ��u�@���b�i��ɡA�H��֪��覡�i�����u�@�ɶ������� 
					if ( throwq == false ){
						restTime -- ;
					}
					
					if ( restTime == 0 ) {                    // �u�@������Jdone list 
						doRightNow.departure = time ;
						done.push_back(doRightNow) ;
						idle = true ;
					}
					else if(  doRightNow.timeout <= time  ) {             // �p�G����O�� 
						doRightNow.departure =  time;
						abort.push_back(doRightNow)  ;
						idle = true ;
					}
				} // if
				
				while ( !queue.isEmpty() && idle ) {                        // ��{�bCPU�O���m���A�Bqueue���O�Ū� 
					
					doRightNow = queue.front() ;                            // queue���X�u�@<�}�l����
					 
					restTime = doRightNow.duration ;
					doRightNow.worktime = time;
					// cout << doRightNow.duration << "	" << doRightNow.OID << endl ;
					queue.dequeue() ;
					idle = false ;
					throwq = true;
					
					
					if( doRightNow.timeout <= time ) {         // ���X�ɤw�O�� 
						doRightNow.departure =  time;
						abort.push_back(doRightNow)  ;
						idle = true ;
					}
				} 
				
				
				while ( !nums.empty() && time == nums[0].arrival ) { // �n���n�ɪF���QUEUE 
					nums[0].wait = 0 ;
					nums[0].departure = 0 ;
					if ( queue.size() < 3 ) {                        // �ˬdQUEUE�O�_ ���F 
						if(  time > nums[0].timeout  ) {             // �i�Jqueue�e�w�gtimeout 
							nums[0].departure = time ;
							abort.push_back(nums[0]) ;
						}
						else
							
							queue.enqueue( nums[0] ) ;				//  �[�Jqueue	
					}
					else {
						
						nums[0].departure = time ;                       // queue�w���A��Jabort 
						abort.push_back(nums[0]) ;
					}
					
					
					nums.erase(nums.begin()) ;                // �B�z���u�@�C�����Ĥ@�� 
					
					while ( !queue.isEmpty() && idle ) {             // ��{�bCPU�O���m���A�Bqueue���O�Ū� 

						doRightNow = queue.front() ;                       // queue���X�u�@<�}�l���� 
						restTime = doRightNow.duration ;
						queue.dequeue() ;
						doRightNow.worktime = time;
						idle = false ;
						throwq = true;
						
						
						
						if( doRightNow.timeout <= time ) {               // ���X�ɤw�O�� 
							doRightNow.departure =  time;
							abort.push_back(doRightNow)  ;
							idle = true ;
						
						}
					}
					
				} //while
				
				
				
				
				
			} // for() 
			for(int i = 0 ; i < done.size() ; i++ ) {                    // �p��delay 
				done[i].wait = done[i].worktime - done[i].arrival;
 			}			
 			
 			for(int i = 0 ; i < abort.size() ; i++ ) {                   // �p��delay 
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
				
				if( !idle1 ) {                  // cpu1�b�i�椤
				 
					if ( throwq1 == false ){
						restTime1 -- ;
					}
					
					if ( restTime1 == 0 ) {      // cpu1���榨�\ 
						doRightNow1.departure = time ;
						done.push_back(doRightNow1) ;
						idle1 = true ;
					}
					else if(  doRightNow1.timeout <= time  ) {     // cpu1����O�� 
						doRightNow1.departure =  time;
						abort.push_back(doRightNow1)  ;
						idle1 = true ;
					}
				} // if
				
				if( !idle2 ) {                        // cpu2�b�i�椤 
					if ( throwq2 == false ){
						restTime2 -- ;
					}
					
					if ( restTime2 == 0 ) {            // cpu2���榨�\  
						doRightNow2.departure = time ;
						done.push_back(doRightNow2) ;
						idle2 = true ;
					}
					else if(  doRightNow2.timeout <= time  ) {                // cpu1����O�� 
						doRightNow2.departure =  time;
						abort.push_back(doRightNow2)  ;
						idle2 = true ;
					}
				} // if
				
				while ( !queue1.isEmpty() && idle1 ) {                       // ��{�bCPU1�O���m���A�Bqueue1���O�Ū� 
					
					doRightNow1 = queue1.front() ;                           // �qqueue1���X�A�}�l���� 
					restTime1 = doRightNow1.duration ;
					doRightNow1.worktime = time;
					
					queue1.dequeue() ;
					idle1 = false ;
					throwq1 = true;
					
					
					if( doRightNow1.timeout <= time ) {                         // ���X�w�O�� 
						doRightNow1.departure =  time;
						abort.push_back(doRightNow1)  ;
						idle1 = true ;
					}
				} 
				
				while ( !queue2.isEmpty() && idle2 ) {                           // ��{�bCPU2�O���m���A�Bqueue2���O�Ū� 
					
					doRightNow2 = queue2.front() ;                                   // �qqueue1���X�A�}�l����  
					restTime2 = doRightNow2.duration ;
					doRightNow2.worktime = time;
				
					queue2.dequeue() ;
					idle2 = false ;
					throwq2 = true;
					
					
					if( doRightNow2.timeout <= time ) {                          // ���X�w�O�� 
						doRightNow2.departure =  time;
						abort.push_back(doRightNow2)  ;
						idle2 = true ;
					}
				} 
				
				while ( !nums.empty() && time == nums[0].arrival ) { // �n���n�ɪF���QUEUE 
					nums[0].wait = 0 ;
					nums[0].departure = 0 ;
					
					if ( queue1.isFull() && queue2.isFull() ){        // �P�_���queue�O���O������ 
						nums[0].CID = 0 ;
						nums[0].departure = time ;
						abort.push_back(nums[0]) ;
					}
					else if ( queue1.size() <= queue2.size() ){                    // �P�_����queue����p 
						if ( queue1.size() < 3 ) {                                         // queue1���O���� 
							if(  time > nums[0].timeout  ) {                       // �P�_�O���O�O�� 
								nums[0].CID = 0 ;
								nums[0].departure = time ;
								abort.push_back(nums[0]) ;
							}
							else {
							
								nums[0].CID = 1 ;
								queue1.enqueue( nums[0] ) ;					// ��Jqueue1 
							}
						}
						else {                                               // �w���A���abort 
							nums[0].CID = 0 ;
							nums[0].departure = time ;
							abort.push_back(nums[0]) ;
						}
					
					
						nums.erase(nums.begin()) ;                             // �R���u�@�C���u�@ 
						while ( !queue1.isEmpty() && idle1 ) {                   // ��{�bCPU1�O���m���A�Bqueue1���O�Ū� 

							doRightNow1 = queue1.front() ;                               // �qqueue1���X�A�}�l����  
							restTime1 = doRightNow1.duration ;
							queue1.dequeue() ;
							doRightNow1.worktime = time;
							idle1 = false ;
							throwq1 = true;
						
						
						
							if( doRightNow1.timeout <= time ) {                    // ���X�w�O��   
								doRightNow1.departure =  time;
								abort.push_back(doRightNow1)  ;
								idle1 = true ;
						
							}
						}
					}
					
					
					else {
						if ( queue2.size() < 3 ) {                                    // queue2���O���� 
							if(  time > nums[0].timeout  ) {                            // �P�_�O���O�O�� 
								nums[0].CID = 0 ;
								nums[0].departure = time ;
								abort.push_back(nums[0]) ;
							}
							else
								nums[0].CID = 2 ;{                                    // ��Jqueue2
								 
								queue2.enqueue( nums[0] ) ;					
							}
						}
						else {
							nums[0].CID = 0 ;                                      // ���F�A��Jabort 
							nums[0].departure = time ;
							abort.push_back(nums[0]) ;
						}
					
				
						nums.erase(nums.begin()) ;                               // �R���u�@�C���u�@
					
						while ( !queue2.isEmpty() && idle2 ) {                              // ��{�bCPU2�O���m���A�Bqueue2���O�Ū�

							doRightNow2 = queue2.front() ;                                    // �qqueue2���X�A�}�l���� 
							restTime2 = doRightNow2.duration ;
							queue2.dequeue() ;
							doRightNow2.worktime = time;
							idle2 = false ;
							throwq2 = true;
						
						
						
							if( doRightNow2.timeout <= time ) {                       // ���X�w�O�� 
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
				//���ˬd���S��sortedfilenum
				
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
				//���ˬd���S��sortedfilenum
				
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



 
