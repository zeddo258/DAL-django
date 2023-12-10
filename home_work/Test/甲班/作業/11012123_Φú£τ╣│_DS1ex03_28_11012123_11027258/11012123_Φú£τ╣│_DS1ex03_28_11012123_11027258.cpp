#include <iostream> // 11012123 許益誠 、 11027258 何承諺  
#include <fstream>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <iomanip>
# include <string>
using namespace std ;

string open_file_name = "\0";
  
bool First_open = true;

class file {
  public:
    int oid = 0;
    int arrival = 0;
    int duration = 0;
    int timeout = 0;
    int abort = 0;
    int departure = 0;
    int delay = 0;
}; 

class Queue {
  struct QueueNode {
    int oid = 0;
  	int arrival = 0;
  	int duration = 0;
  	int timeout = 0;
  	QueueNode *next;
  };
  
  public:
  	QueueNode *front; 
  	QueueNode *back;
  
	Queue(){
	  front = NULL ;
	  back = NULL ;
	}
	
	bool isempty(){  
	  if( front == NULL )
		return true ;
	  else
		return false ;	
    }
    
	void enqueue( file a ){ 
	  if( isempty() ){	
	    front = new QueueNode ;
	    front->oid = a.oid ;
	    front->arrival = a.arrival ;
	    front->duration  = a.duration ;
	    front->timeout = a.timeout;
	    front->next = NULL ;
	    back = front ;
	  }
	  else{
		back->next = new QueueNode ;
		back = back->next ;
	    back->oid = a.oid ;
	    back->arrival = a.arrival ;
	    back->duration  = a.duration ;
	    back->timeout = a.timeout;
	    back->next = NULL ;
	  }	
	}
	
	void dequeue(){  
	  QueueNode *temp = NULL  ;
	  temp = front ;
	  front = front->next ;
	  temp->next = NULL ;
	  delete temp ;
	  temp = NULL ;
	}
	int size() { //q的大小 
	  QueueNode *walk = front ;
	  int i  = 0 ;
	  while ( walk != NULL ) {
		i = i + 1;
		walk = walk -> next;
	  }

	  return i;
	}
	
	void clear(){ //初始化 
	  QueueNode *run  ;
	  while ( front != NULL ){
		run = front ;
		front = front->next ;
		delete run ;
		run = NULL ;
	  }
	}		    
};


class mission1{
  public:
	vector<file>mis1 ;  
	string filename = "\0" ;
	string firstline;
	double readtime;
	double sorttime;
	double writetime;
	bool stop = false ;
	  
	void readfile() {  
	  file temp ;
	  ifstream ifs ;
	  cout << "Input a file number: " ;
	  cin >> filename ;
	  cout << endl;
	  open_file_name = filename;
	  clock_t star, end;
	  star = clock();
	  ifs.open(("input" + filename + ".txt").c_str());
	  stop = false ;

      if (ifs.is_open()) {  
		getline( ifs, firstline );
	    while( ifs >> temp.oid){
		  ifs >> temp.arrival; 
		  ifs >> temp.duration; 
		  ifs >> temp.timeout; 
		  mis1.push_back(temp) ; 	
		}
		ifs.close() ;
		end = clock();
		readtime = ( double )( end - star );
		cout << "\t" << firstline << endl;
		for( int i = 0, j = 1 ; i < mis1.size() ; i++, j++ ) {
		  cout << "(" << j << ")" << "\t" << mis1[i].oid << "\t" << mis1[i].arrival << "\t" << mis1[i].duration << "\t" << mis1[i].timeout << "\t" << endl ;
    	}
      }
      else{
        cout << "Failed to open file.\n";
        stop = true ;
	  }
	} // void
		
	void shellsort() { 
	  clock_t star, end;
	  star = clock();
	  for (int gap = mis1.size()/2; gap > 0; gap /= 2){
		for (int i = gap; i < mis1.size(); ++i) {	
		  for (int j = i-gap; j >= 0 && mis1[j].arrival >= mis1[j+gap].arrival; j -= gap) {
			if ( mis1[j].arrival == mis1[j+gap].arrival ){ 
			  if ( mis1[j].oid > mis1[j+gap].oid )
			    swap(mis1[j], mis1[j+gap]);	
			}
			else {
			  swap( mis1[j], mis1[j+gap]);
		    }
		  }
		}
	  }
	  end = clock() ;
	  sorttime = ( double )( end - star );
	}
		
	void writefile(){  
	  clock_t star, end;
	  ofstream ofs;
	  star = clock(); 
	  ofs.open(("sorted" + filename + ".txt").c_str());

	  if ( ofs.is_open()) {
		ofs <<  firstline << "\n";
		for( int i = 0 ; i < mis1.size() ; i++ ) {
		  ofs << mis1[i].oid << "\t" << mis1[i].arrival << "\t" << mis1[i].duration << "\t" << mis1[i].timeout << "\t" << endl ;	
	    }
		ofs.close();	
	  }
	  else {
		cout << "Failed to open file.\n";
	  }
		
	  end = clock() ;
	  writetime = ( double )( end - star );	
	  mis1.clear() ;  
	} // void
};

class mission2 {
  public:
	vector<file>mis2;  
	vector<file>donelist;
	vector<file>abortlist;
	string filename = "\0" ;
	string firstline;
	Queue queue;
	bool stop = false ;
		  
	void readfile() {  
	  file temp ;
	  ifstream ifs ;
	  // cout << endl;
	  
	  if ( First_open == true ) {
	  	cout << "Input a file number: " ;
	  	cin >> filename;
	  }
	  else {
	  	filename = open_file_name;
	  }
	  
	  ifs.open(("sorted" + filename + ".txt").c_str());
	  stop = false ;

      if (ifs.is_open()) {  
		getline( ifs, firstline );
	    while( ifs >> temp.oid){
		  ifs >> temp.arrival; 
		  ifs >> temp.duration; 
		  ifs >> temp.timeout; 
		  mis2.push_back(temp) ; 	
		}
		ifs.close() ;
      }
      else{
        cout << "Failed to open file.\n";
        stop = true ;
	  }
	} // void
	
	void simulation() {
	  file temp;
	  int current = 0;
	  int i = 0;
	  while ( i < mis2.size() ) {
	    if ( current <= mis2[i].arrival ) {
	      if ( queue.isempty() ) {
	      	temp.oid = mis2[i].oid;
	      	if ( current == mis2[i].arrival ) {
              temp.departure = current + mis2[i].duration;
			}
			else { 
			  temp.departure = mis2[i].arrival + mis2[i].duration;
			}
	      	temp.delay = 0;
			current = temp.departure;
			donelist.push_back(temp);
		  }	
		  else {
            if ( current == mis2[i].arrival ) {
              if ( queue.front -> timeout >= current + queue.front -> duration ) {
				temp.oid = queue.front -> oid;
				temp.departure = current + queue.front -> duration;
				temp.delay = current - queue.front -> arrival;
				current = temp.departure;
				donelist.push_back(temp);
			  }
			  else {
				if ( current < queue.front -> timeout ) {
				  current = queue.front -> timeout;
				}
				temp.oid = queue.front -> oid;
				temp.abort = current;
				temp.delay = current - queue.front -> arrival;
				abortlist.push_back(temp);
			  }
			  queue.dequeue();
			  queue.enqueue( mis2[i] );
			}
			else {
              while ( !queue.isempty() ) {
				if ( queue.front -> timeout >= current + queue.front -> duration ) {
				  temp.oid = queue.front -> oid;
				  temp.departure = current + queue.front -> duration;
				  temp.delay = current - queue.front -> arrival;
				  current = temp.departure;
				  donelist.push_back(temp);
			    }
			    else {
				  if ( current < queue.front -> timeout ) {
				   current = queue.front -> timeout;
				  }
				  temp.oid = queue.front -> oid;
				  temp.abort = current;
				  temp.delay = current - queue.front -> arrival;
				  abortlist.push_back(temp);
			    }
				queue.dequeue();
			  }
			  i = i - 1;
			}
		  }
		}
		else if ( current > mis2[i].arrival ) {
			if ( queue.size() < 3 ) {
			  queue.enqueue( mis2[i] );
			}
			else {
			  temp.oid = mis2[i].oid;
              temp.abort = mis2[i].arrival;
			  temp.delay = 0;
			  abortlist.push_back( temp );
			}
		}
	    i = i + 1;	
		temp.oid = 0;
		temp.arrival = 0;
		temp.duration = 0;
		temp.timeout = 0;
		temp.abort = 0;
		temp.departure = 0;
		temp.delay = 0;
	  } 
	  while ( !queue.isempty() ) {
	    if ( queue.front -> timeout >= current + queue.front -> duration ) {
	      temp.oid = queue.front -> oid;
	      temp.departure = current + queue.front -> duration;
	      temp.delay = current - queue.front -> arrival;
	      current = temp.departure;
	      donelist.push_back(temp);
	    }
	    else {
		  if ( current < queue.front -> timeout ) {
		    current = queue.front -> timeout;
		  }
		  temp.oid = queue.front -> oid;
		  temp.abort = current;
		  temp.delay = current - queue.front -> arrival;
		  abortlist.push_back(temp);
	    }
	  queue.dequeue();
	  	temp.oid = 0;
		temp.arrival = 0;
		temp.duration = 0;
		temp.timeout = 0;
		temp.abort = 0;
		temp.departure = 0;
		temp.delay = 0;
	  }
	}
	
	double average_delay() {
	  double average = 0;
	  int total = 0;
	  int i = 0, j = 0;

	  while ( i < donelist.size() ) {
	  	total = total + donelist[i].delay;
	  	i++;
	  }
	  
	  while ( j < abortlist.size() ) {
	  	total = total + abortlist[j].delay;
	  	j++;
	  }	  
	  
	  average = (double)( total ) / (double)( i + j ) ;
	  return average;
	}
	
	double success_rate() {
	  double rate = 0;
	  int i = 0, j = 0;
	  while ( i < donelist.size() ) {
	  	i++;
	  }
	  
	  while ( j < abortlist.size() ) {
	  	j++;
	  }	 	  
	  	  
	  rate = (double)( i * 100 ) / (double)( i + j );
	  return rate;
	}
	
	void writefile(){  
	  ofstream ofs;
	  ofs.open(("output" + filename + ".txt").c_str());

	  if ( ofs.is_open()) {
    	ofs << "\t" << "[Abort Jobs]" << endl;
    	ofs << "\t" << "OID" << "\t" << "Abort" << "\t" << "Delay" << endl;
    	for( int i = 0, j = 1 ; i < abortlist.size() ; i++, j++ ) {
          ofs << "[" << j << "]" << "\t" << abortlist[i].oid << "\t" << abortlist[i].abort << "\t" << abortlist[i].delay << endl ;
        }
    	ofs << "\t"<<"[Jobs Done]" << endl;
    	ofs <<"\t"<< "OID" <<"\t"<< "Departure" <<"\t" << "Delay" << endl;	
		for( int i = 0, j = 1 ; i < donelist.size() ; i++, j++ ) {
		  ofs <<"["<<j<<"]"<<"\t"<< donelist[i].oid <<"\t"<< donelist[i].departure <<"\t"<< donelist[i].delay << endl ;
	    }
		ofs << "[Average Delay]" << "\t" << fixed << setprecision(2) << average_delay() << " " << "ms" << endl;	 
		ofs << "[Success Rate]" << "\t" << fixed << setprecision(2) << success_rate() << " " << "%" << endl;
		ofs.close(); 	
	  }
	  else {
		cout << "Failed to open file.\n";
	  }
		
	  mis2.clear() ;  
	} // void	
};

int main() {
  string a = "\0" ;
  mission1 m1;
  mission2 m2;
  int command = 0; 
  string open_file_name;
  do {
    cout << endl << "**** Simulate FIFO Queues by SQF *****";
    cout << endl << "* 0. Quit                            *";
    cout << endl << "* 1. Sort a file                     *";
    cout << endl << "*  2. Simulate one FIFO queue        *";
    cout << endl << "**************************************";
    cout << endl << "Input a command( 0, 1, 2 ): " ;
    cin  >> command ; // get the command
    cout << endl;
    if ( command == 1 || command == 2 || command ==3 ){
  	  if ( command == 1 )  {
	    m1.readfile();
	    if( m1.stop == false){
		  First_open = false;	      
		  m1.shellsort();	
		  m1.writefile();
		  cout << endl;
		  cout << "Reading data: "<< m1.readtime << " clocks (" << m1.readtime << " ms). "<< endl ;
		  cout << "Sorting data: "<< m1.sorttime << " clocks (" << m1.sorttime << " ms). "<< endl ;
		  cout << "Writing data: "<< m1.writetime << " clocks (" << m1.writetime << " ms). " << endl ;
		  cout << endl ;
		  cout << "See sorted" << m1.filename << ".txt" << endl;
	    }
      }
    else if( command == 2 )	{
      m2.readfile();
      if( m2.stop == false){
	    m2.simulation();
        m2.writefile(); 
        cout << "The simulation is running..." << endl;
        cout << "See output" << m2.filename << ".txt" << endl;
      }
    }													
    }
    else if ( command == 0 ) { 
      break ; 
    } // else if
    else { 
      cout << endl << " Command does not exist!" << endl ;	// inllegal command
    } // else 					
  } while(true);
  system("pause");
  return 0 ;	
}
