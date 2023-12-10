// 11127207 ������ 11127210 �B�ۧ� 

#include <iostream>
#include <fstream> // ���Y�� 
#include <string>  
#include <vector>
#include <ctime>
#include <queue> 
#include <iomanip>
using namespace std ;

// ����CPU�u�@�Ӷ� 
typedef struct JobItem {
	int OID ;
	int arrival ;
	int duration ;
	int timeout ;
};
// ����CPU���Ƶ{ 
typedef struct EventList {
	char event ;
	int arrival ;
	int duration ;
	int dtime ;
	int timeout ;
};

// ����CPU�u�@�Ӷ� (�Ω��쵲��C) 
typedef struct JobType {
	int OID ;
	int arrival ;
	int duration ;
	int timeout ;
	JobType * next ;
};

// �Hlinked list ��@Queue 
class JobQueue{
	
	private:
		// Queue���Y�� 
		JobType *front ;
		JobType *back ;
		

	public:
		// constructor 
		JobQueue():back(NULL),front(NULL) {
		} 
		
		// �T�{����C�O�_���� 
		bool isEmpty(){	
			return back == NULL ;	
		} // isEmpty()
		
		// �^�Ǧ���C���j�p 
		int size() {
			JobType *temp = front ;
			int Size = 0 ;
			while ( temp != back ) {
				Size = Size + 1 ;
				temp = temp -> next ;
			} // while
			
			return Size ;
			
		} // size()
		 
		// �b��C���[�J��� (FIFO) 
		void enqueue( JobItem newItem ) {
			
			JobType * NewPtr = new JobType ;
			NewPtr->next = NULL ;
			
			NewPtr->OID = newItem.OID ;
			NewPtr->arrival = newItem.arrival ;
			NewPtr->duration = newItem.duration ;
			NewPtr->timeout = newItem.timeout ;
			
			
			if ( isEmpty() ) 
				front = NewPtr ;
			else
				back->next = NewPtr ;

			
			back = NewPtr ;
			
		} // enqueue()
		
		// �b��C���R�����(FIFO) 
		void dequeue() {
			JobType *temp = front ;
			
			if ( front = back ){
				front = NULL ;
				back = NULL ;
			} // if
			else 
				front = front->next ;
				
			temp->next = NULL ;
			delete temp ;
 
				
		} // dequeue()
		
		// �b��C���R����ƨæ^�Ǹ��(FIFO) 
		void dequeue( JobItem &Item ) {
			
			getFront( Item ) ;
			JobType *temp = front ;
			
			if ( front == back ){
				front = NULL ;
				back = NULL ;
			} // if
			else 
				front = front->next ;
				
			temp->next = NULL ;
			delete temp ;
 
				
		} // dequeue()
		
		// ���o���Ӹ�� 
		void getFront( JobItem &Item ) {
			Item.OID = front->OID ;
			Item.arrival = front->arrival ;
			Item.duration = front->duration ;
			Item.timeout = front->timeout ;
		} // getFront()
		
		// show�X�Y��������(DEBUG��) 
		void showQ() {
			cout << "Queue :" << endl ;
			JobType *temp = front ;
			while( temp != NULL ){
				cout << temp->OID << " "  << temp->arrival << " " << temp->duration << " " << temp->timeout << endl ;
				temp = temp->next ;
			} // while
		} // show()
};

class Simulation {
	// ����CPU��C 
	JobQueue Queue ;

	// Done List
	vector< vector< int > > Done ;
	vector< int > newDone ;
	
	// Abort List
	vector< vector< int > > Abort ;
	vector< int > newAbort ;
	
	// �x�s�U���u�@�Ƶ{��DATA	
	vector< vector< int > > Data ;
	vector< int > newData ;	
	
	// ����CPU���Ƶ{ 
	vector< EventList > evenList ;
	EventList newEven ;
	
public :
	

	

	// �]�m�s��event Arrival
	void setNewEvent( char type, int arrival, int duration, int timeout ) {
		newEven = {type,arrival,duration,-1,timeout } ;
	} // setNewEvent()
	
	// �]�m�s��evant Departure 
	void setNewEvent( char type, int dtime, int timeout ) {
		newEven = {type,-1,-1,dtime,timeout } ;
	} // setNewEvent()
	
	// ���oflie�̩Ҧ���event 
	bool getEvent( string fileNumber ) {

		ifstream in ;
		int value ;
		string a =  "sorted" + fileNumber + ".txt" ;   	
		in.open( a.c_str() ) ;
		if ( in.fail() ) {
			cout << "There is no file" << endl ;
			in.close() ;
			return false ;
		}
		
		string title ;	
		getline( in, title ) ;  
			
   		while (in >> value) {
     	   for (int i = 0; i < 3; i++) {
        	    newData.push_back(value);
            	in >> value;
        	} // for

       		newData.push_back(value);
        	Data.push_back(newData);

        	newData.clear();
    	} // while

		
		in.close() ;
		return true ;
		
	} // getEvent

	// ����CPU�B�@ 
	void Simulate( string fileNumber, bool &HaveFile ) {


		int time ; // �����ɶ� 
		HaveFile = getEvent( fileNumber ) ;
		JobItem temp ;

		temp.OID = Data[0][0] ;
		temp.arrival = Data[0][1] ;
		temp.duration = Data[0][2] ;
		temp.timeout = Data[0][3] ;
		Data.erase( Data.begin() ) ;
		
		
		// ��J�Ĥ@��event 
		time = temp.arrival ;
		
		setNewEvent('A',temp.arrival,temp.duration,temp.timeout) ;
		evenList.push_back( newEven ) ;
		
		// �H�j��B�zevent 
		while( !evenList.empty() ) {
			
			if ( newEven.event == 'A' )
				processArrival( temp, time ) ;
			else
				processDeparture( time ) ;
			
			
			newEven = evenList[0] ;
		} // while()
		

	} // Simulate()
	
	// print Abort and Done Lists
	void PrintAD( bool HaveFile, string fileNumber ) {
		
		
		string a = fileNumber + ".txt" ;

		string b = "output" + a ;
		ofstream out( b.c_str(), ios :: app ) ;
		if ( HaveFile ) {
			out << "    [Abort Job]" << endl ;
			out << "    OID	Abort	Delay" << endl ;
			for( int i = 0 ; i < Abort.size() ; i = i + 1 ) {
				for ( int k = 0; k < 3 ; k++ ) {
					if ( k == 0 )
						out << "(" << i + 1 << ") " ;
					out << Abort[i][k] ;
					if ( k == 2 )
						out << endl ;
					else
						out << "   " ;
				}
				
			} // for i
			
		out << "    [Jobs Done]" << endl ;
		out << " OID Departure	Delay" << endl ;
			for( int i = 0 ; i < Done.size() ; i = i + 1 ) {
				for( int j = 0 ; j < 3 ; j = j + 1 ) {
					if ( j == 0 )
						out << "(" << i + 1 << ") " ;
					out << Done[i][j] ;
					if ( j == 2 )
						out << endl ;
					else
						out << "   " ;
					
				} // for j
			
			} // for
			cout << "The simulation is running..." << endl ;
			cout << "see output" << a << endl ;
		}
		else {
			cout << "there is no sorted" << a << endl ;
		}
	
		float size = Done.size() + Abort.size() ;
		float delayRate = delay( size ) ;
		float successRate = success( size ) ;
		out << "[Average Delay]  ";
		out << fixed << setprecision(2) << delayRate; 
		out << " ms" << endl;

		out << "[Success Rate]  ";
		out << fixed << setprecision(2) << successRate; 
		out << "%" << endl;

		out.close();
		

		
	} // PrintAD

    //�p�⥭������ɶ� 
	float delay( float size ) {
		float Delay = 0.0 ;
		float DoneRate = 0.0 ;
		float AbortRate = 0.0 ;
		for( int i = 0 ; i < Done.size() ; i++ ) {
			DoneRate = DoneRate + (float)Done[i][2] ;
		} // for
		
		for ( int k = 0 ; k < Abort.size() ; k++ ) {
			AbortRate = AbortRate + (float)Abort[k][2] ;
		} // for
		
		Delay = ( DoneRate + AbortRate ) / size ;
		return Delay ;
		
	} // delay
	
	// ���\�v 
	float success( float size) {
		float work = 100 * (float)Done.size() / size ;
		return work ;
	} // success
	


    // �B�zArrival event 
	void processArrival( JobItem & item, int & time ) {
		 
		 time = item.arrival ;

		 
		 evenList.erase(evenList.begin()) ;
		 
		if ( Queue.isEmpty() ) {
			Queue.enqueue(item) ;
			setNewEvent( 'D', time + item.duration ,item.timeout ) ;
			evenList.push_back(newEven) ;
			

			
		} // if
		else if ( !Queue.isEmpty() ){
			if ( Queue.size() < 3 ) {
				Queue.enqueue(item) ;
			} // if
			else {
				
				newAbort.push_back( item.OID ) ;
				newAbort.push_back( item.arrival ) ;
				newAbort.push_back( 0 ) ;
				Abort.push_back(newAbort) ;
				newAbort.clear() ;
			} // else
		} // else if
		

		
		if ( !Data.empty() ) {
			item.OID = Data[0][0] ;
			item.arrival = Data[0][1] ;
			item.duration = Data[0][2] ;
			item.timeout = Data[0][3] ;
			Data.erase( Data.begin() ) ;
			setNewEvent('A',item.arrival,item.duration,item.timeout) ;
			
			if ( evenList[0].dtime > newEven.arrival ) {
				evenList.insert( evenList.begin(), newEven );
				time = item.arrival ;
			} // if
			else if ( evenList[0].dtime <= newEven.arrival ) {
				evenList.push_back( newEven ) ;
				time = evenList[0].dtime ;
			} // else if
			
		} // if ( !Data.empty() )

		

		

	} // processArrival()
	
	// �B�zDeparture event 
	void processDeparture( int & time ) {
		
		int delay ;
		JobItem item, done ;
        time = newEven.dtime ;
        bool find = false ;
		
		
		Queue.dequeue( done ) ;

		newDone.push_back( done.OID ) ;
		newDone.push_back( time ) ;
		newDone.push_back( time - ( done.arrival + done.duration ) ) ;
		Done.push_back( newDone ) ;
		newDone.clear() ;

		
		if( !Queue.isEmpty() ) {


			evenList.erase(evenList.begin()) ;	
			Queue.getFront( item ) ;
			
			if ( item.timeout >= time + item.duration ) {
				find = true ;
			} // if
			else {
				Queue.dequeue(item) ;
				if ( item.timeout >= time )
					time = item.timeout ;
			} // else
			
			while( !Queue.isEmpty() && !find ) {
				
				
				newAbort.push_back( item.OID ) ;
				newAbort.push_back( time ) ;
				newAbort.push_back( time - item.arrival ) ;
				Abort.push_back(newAbort) ;
				newAbort.clear() ;


				Queue.getFront( item ) ;

				if ( item.timeout > time + item.duration ) 
					find = true ;
				else {
					Queue.dequeue(item) ;
					if ( item.timeout >= time )
						time = item.timeout ;

				} // else
			} // while
			if ( Queue.isEmpty() ) {
				if ( item.timeout <= time + item.duration ) {
					newAbort.push_back( item.OID ) ;
					newAbort.push_back( item.timeout ) ;
					newAbort.push_back( time - item.arrival ) ;
					Abort.push_back(newAbort) ;
					newAbort.clear() ;
				} // if
			} // if

			if( find ) {	

				setNewEvent( 'D', time + item.duration, item.timeout ) ;
			
				if ( evenList[0].arrival >= newEven.dtime ) {
					evenList.insert( evenList.begin(), newEven );
					time = newEven.dtime ;
				} // if
				else if ( evenList[0].arrival < newEven.dtime ) {
					evenList.push_back( newEven ) ;
					time = evenList[0].arrival ;
				} // else if
			} // if find
			
		} // if
		
		else if ( Queue.isEmpty() ) {

			evenList.erase(evenList.begin()) ;
			

		} // else if
		
		
		
	} // processDeparture()
	


};

// �Ƨ�arrival�ۦP����� 
void sortOID( vector< vector< int > > &data ) {
	
	int tempO ;
	int tempA ;
	int tempD ;
	int tempT ;
	
	for ( int i = 0 ; i < data.size() - 2 ; i++ ) {
		for ( int j = i+1 ; j < data.size() - 1 ; j++ ){
			if( data[i][1] == data[j][1]  ) {
				if ( data[i][0] > data[j][0] ) {
				
					tempO = data[i][0] ;
					tempA = data[i][1] ;
					tempD = data[i][2] ;
					tempT = data[i][3] ;
					
					data[i][0] = data[j][0] ;
					data[i][1] = data[j][1] ;
					data[i][2] = data[j][2] ;
					data[i][3] = data[j][3] ;
					
					data[j][0] = tempO ;
					data[j][1] = tempA ;
					data[j][2] = tempD ;
					data[j][3] = tempT ;
					
 				} // if
 			} //if
		} // for j
	} // for i
	
} // sortOID

// shellsort deal with Data 
void shellSort( vector< vector< int > > &data, int size ) {
	
	int OID = 0 ;
	int Arrival = 1 ;
	int Duration = 2 ;
	int Timeout = 3 ;
	int tempO ;
	int tempA ;
	int tempD ;
	int tempT ;
	bool same = false ;
	

	for( int h = size / 2 ; h > 0 ; h = h / 2 ) {
		
		
		for( int unsorted = h ; unsorted < size ; unsorted = unsorted + 1 ) {
			int loc = unsorted ;
			
			
			int nextItemO = data[unsorted][OID] ;
			int nextItemA = data[unsorted][Arrival] ;
			int nextItemD = data[unsorted][Duration] ;
			int nextItemT = data[unsorted][Timeout] ;
			

			

			
			for( ;( loc >= h ) && ( data[loc-h][Arrival] > nextItemA ) ; loc = loc - h ) {
				
				data[loc][OID] = data[loc-h][OID] ;
				data[loc][Arrival] = data[loc-h][Arrival] ;
				data[loc][Duration] = data[loc-h][Duration] ;
				data[loc][Timeout] = data[loc-h][Timeout] ;

			} // for loc

			data[loc][OID] = nextItemO ;
			data[loc][Arrival] = nextItemA ;
			data[loc][Duration] = nextItemD ;
			data[loc][Timeout] = nextItemT ;
		

		} // for unsorted
	} // for h
	
	sortOID(data) ;

} // shellSort



int main() {
	
	do {
		ifstream in ;
		string fileName ;   // �ɮצW 
		int command = 0 ;  // ���O 
		vector< vector< int > > data ;
		vector<int> newData ;
		string title ;
		int value = 0 ;
		ofstream out ;
		bool HaveFile = false ;

		out.open( "output.txt") ;
		if ( out.fail() )
			cout << "There is no file" << endl ;
		
		cout << endl << "*** Simulate FIFO Queues by SQF ***" ;
		cout << endl << "*0. Quit                          *" ;   // �ϥΪ̩Ҭݨ쪺���� 
		cout << endl << "*1. Sort a file                   *" ;
		cout << endl << "*2. Simulate one FIFO queue       *" ;
		cout << endl << "***********************************" ;
		cout << endl << "Input a command( 0, 1, 2 ): " ;
		cin >> command ;
		cout << endl ;
		
		
		if ( command == 1 || command == 2 ) {  // �p�G��J���O1��2 
		
		
            cout << "Input a file number(ex.301, 302,... ) : "  ;
            string fileNumber ;
            cin >> fileNumber ;  // �����ɮצW�� 
			string a =  "input" + fileNumber + ".txt" ;  
			
			if ( command == 1 ) {
			
				clock_t startREAD = clock(); // �}�l�p�� 
			
				in.open( a.c_str() ) ;
				if ( in.fail() ) {  // �p�G�S���o�Ӹ�� 
					cout << endl << "input" << fileNumber << ".txt does not exist" << endl ;
					in.close() ; // ���� 
				} // if
			
				getline( in, title ) ;  
				

				 // Ūdata��� 
				while ( !in.eof() ) {
					for ( int i = 0 ; i < 4 ; i++ ) {
						in >> value ;
						newData.push_back(value) ;
					} // for
				
					data.push_back(newData) ;
				
					for ( int i = 0 ; i < 4 ; i++ ) {
					
						newData.pop_back() ;
					} // for
							
				} // while

				for ( int i = 0 ; i < 4 ; i++ ) {
						in >> value ;
						newData.push_back(value) ;
				} // for
				data.push_back(newData) ;
				for ( int i = 0 ; i < 4 ; i++ ) {
					newData.pop_back() ;
				} // for
			
				clock_t endREAD = clock(); // �����p�� 
				float durationR = (float)(endREAD - startREAD) / CLOCKS_PER_SEC * 1000.0;

				
			
				clock_t startSORT = clock();
				shellSort( data, data.size() - 1 ) ; // �ƺ��Ƨ� 
				clock_t endSORT = clock();
			
				float durationS = (float)(endSORT - startSORT) / CLOCKS_PER_SEC * 1000.0;

			
				cout << title << endl ; // �L�X 
				for ( int i = 0 ; i < data.size() - 1 ; i++ ) {
					for ( int j = 0 ; j < 4 ; j++ ) {
						if ( j == 0 ) {
							cout << "(" << i + 1 << ")" ;
							for( int k = 0 ; k < 4 - ((i+1)/10) ; k = k + 1 )
								cout << " " ;
								
						} // if
						cout << data[i][j] ;
 
						if ( j == 3 )
							cout << endl ;
						else
							cout << "   " ;	
					} // for
				}
			
			
				clock_t startWRITE = clock();
			
				string sortedFile = "sorted" + fileNumber + ".txt" ; // �n�� 
				ofstream out( sortedFile.c_str(), ios :: app ) ;
				out << title << endl ;
				for ( int i = 0 ; i < data.size() - 1 ; i++ ) {
					for ( int j = 0 ; j < 4 ; j++ ) {
						
							
						out << data[i][j] ;
						
						if ( j == 3 )
							out << endl ;
						else
							out << " " ;	
							
					} // for i
				} // for j
			
				clock_t endWRITE = clock();

				float durationW = (float)(endWRITE - startWRITE) / CLOCKS_PER_SEC * 1000.0;
				// �L�X�p�⪺�ɶ� 
   				cout << "Reading data: " << (int)(durationR)  << " clocks (" ;
				cout << (" %.2f ", durationR );
				cout << "ms)." << endl ;
	          	float IDS = durationS ;
				cout << "Sorting data: " << (int)(durationS)  << " clocks (" ;
				cout << (" %.2f ", IDS ) ;
				cout << "ms)." << endl ;
				cout << "Writing data: " << (int)(durationW)  << " clocks (" ;
				cout << (" %.2f ", durationW );
				cout << "ms)." << endl ;
				
				cout << "see " << sortedFile << endl ;
			
			}
			// command 2 part
			else if ( command == 2  ) {
				Simulation simulation ;
				simulation.Simulate( fileNumber, HaveFile) ;
				simulation.PrintAD( HaveFile, fileNumber ) ;
			} // else if
			
		} // if
    
    	in.close() ; // ����
    	out.close() ;
	} while (true) ;
	
	system("pause") ;
	return 0 ; 
} // main



 
