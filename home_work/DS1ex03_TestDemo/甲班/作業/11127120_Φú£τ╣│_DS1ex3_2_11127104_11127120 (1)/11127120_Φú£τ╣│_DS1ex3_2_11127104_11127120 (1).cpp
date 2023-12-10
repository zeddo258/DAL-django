// 11127104 陳昱安
// 11127120 林琮裕 
#include <iostream>
#include <fstream>
#include <string> 
#include <ctime>
#include <iomanip>

using namespace std ;

struct Job {
	int OID = 0 ;
	int Arrival = 0 ;
	int Duration = 0 ;
	int TimeOut = 0 ;
	Job * next ;
} ;

typedef Job * JobPtr ;

class file {
	
	public :
		file() {
		}
		
		bool Inputfile() {
			clock_t start_time = clock() ;
			ifstream in ;
			string temp ;
			string filename ;
			Job * current = NULL ;
			cout << endl << "Input a file number: " ;
			cin >> filenumber ;
			filename = "input" + filenumber + ".txt" ;
			in.open( filename.c_str() ) ;
			if ( in.fail() ) {
				cout << endl << "### " << filename << " does not exist! ###" << endl ; 
				return false ;
			}
			else {
				
				while ( in.peek() != EOF ) {
					
					if ( cpu == NULL ) {
						in >> temp >> temp >> temp >> temp ;
						if ( in.peek() == '\n' ) {
							in.ignore() ;
						}
						
						if ( in.peek() != EOF ) {
							cpu = new Job ;
							current = cpu ;
							current -> next = NULL ;
							in >> current -> OID >> current -> Arrival >> current -> Duration >> current -> TimeOut ;
							num = num + 1 ;
						}
						
					}
					else {
						current -> next = new Job ;
						current = current -> next ;
						current -> next = NULL ;
						in >> current -> OID >> current -> Arrival >> current -> Duration >> current -> TimeOut ;
						num = num + 1 ;
						
					}
					
					
					if ( in.peek() == '\n' ) {
						in.ignore() ;
					}
					
					
					
					
				}
				
			}
			
			in.close() ;
			clock_t end_time = clock() ;
			read_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000.0 ;
			return true ;
		}
		
		void ShellSort() {
			clock_t start_time = clock() ;
			Job * current1 = cpu ;
			Job * current2 = cpu ;
			Job * tempcur ;
			int step = num / 2 ;
			int temp ;
			bool start = false ;
			while ( step >= 1 ) {
				current1 = cpu ;
				
			    for ( int i = 0 ; i < step ; i ++ ) {
								
					current2 = current1 ;
					tempcur = current1 ;
					
					while ( current1 != NULL ) {
						if ( start ) {
							for ( int j = 0 ; j < step  ; j ++ ) {	
								current1 = current1 -> next ;
								if ( current1 == NULL ) {
									break ;
								}
							}
						
							if ( current1 == NULL ) {
								break ;
							}
						}
						
						
						while ( current2 != NULL ) {
					
							for ( int j = 0 ; j < step  ; j ++ ) {	
								current2 = current2 -> next ;
								if ( current2 == NULL ) {
									break ;
								}
							}
						
							if ( current2 == NULL ) {
								break ;
							}
						
							if ( current2 -> Arrival < current1 -> Arrival ) {
								temp = current1 -> OID ;
								current1 -> OID = current2 -> OID ;
								current2 -> OID = temp ;
								temp = current1 -> Arrival ;
								current1 -> Arrival = current2 -> Arrival ;
								current2 -> Arrival = temp ;
								temp = current1 -> Duration ;
								current1 -> Duration = current2 -> Duration ;
								current2 -> Duration = temp ;
								temp = current1 -> TimeOut ;
								current1 -> TimeOut = current2 -> TimeOut ;
								current2 -> TimeOut = temp ;
							
							}
							else if ( current2 -> Arrival == current1 -> Arrival ) {
								if ( current2 -> OID < current1 -> OID ) {
									temp = current1 -> OID ;
									current1 -> OID = current2 -> OID ;
									current2 -> OID = temp ;
									temp = current1 -> Arrival ;
									current1 -> Arrival = current2 -> Arrival ;
									current2 -> Arrival = temp ;
									temp = current1 -> Duration ;
									current1 -> Duration = current2 -> Duration ;
									current2 -> Duration = temp ;
									temp = current1 -> TimeOut ;
									current1 -> TimeOut = current2 -> TimeOut ;
									current2 -> TimeOut = temp ;
								
								}
							}
						
						}
						
						start = true ;
						current2 = current1 ;
					}
					
					current1 = tempcur -> next ;
					start = false ; 
				}
				
				step = step / 2 ;
			}
			
			clock_t end_time = clock() ;
			sort_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000.0 ;
			
		}
		
		void showcpu() {
			Job * current = cpu ;
			int n = 1 ;
			cout << endl << "\t" << "OID" << "\t" << "Arrival" << "\t" << "Duration" << "\t" << "TimeOut" ;
			while ( current != NULL ) {
				cout << endl << "(" << n << ")" << "\t" << current -> OID << "\t" << current -> Arrival << "\t" << current -> Duration << "\t" << current -> TimeOut ;
				current = current -> next ;
				n = n + 1 ;
			}
			
			cout << endl ;
			
			 
		}
		
		void Onputsortedfile() {
			clock_t start_time = clock() ;
			Job * current ;
			current = cpu ;
			string filename = "sorted" + filenumber + ".txt" ;
			ofstream out( filename.c_str() ) ;
			out << "OID" << "\t" << "Arrival" << "\t" << "Duration" << "\t" << "TimeOut" << endl ;
			while ( current != NULL ) {
				out << current -> OID << "\t" << current -> Arrival << "\t" << current -> Duration << "\t" << current -> TimeOut << endl ;
				current = current -> next ;
			}
			
			out.close() ;
			clock_t end_time = clock() ;
			sort_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000.0 ;
		}
		
		void showtime() {
			cout << endl << "Reading data: " << read_time << " clocks (" << fixed << setprecision(2) << read_time << " ms)." ;
			cout << endl << "Sorting data: " << sort_time << " clocks (" << fixed << setprecision(2) << sort_time << " ms)." ;
			cout << endl << "Writing data: " << write_time << " clocks (" << fixed << setprecision(2) << write_time << " ms)." << endl ;
			
			string filename = "sorted" + filenumber + ".txt" ;
			cout << endl << "See " << filename << endl ;
		}
		
		JobPtr getcpu() {
			return cpu ;
		}
		
	private :
		
		string filenumber ;
		Job * cpu = NULL ;
		int num  = 0 ;
		double read_time = 0 ;
		double sort_time = 0 ;
		double write_time = 0 ;
	
};

class Queue {
	public :
		Queue() {
		}
		
		bool isEmpty() {
			if ( frontPtr == NULL ) {
				return true ;
			}
			
			return false ;
		}
		
		bool isFull() {
			
			int n = 0 ;
			QueueNode * curPtr = frontPtr ;
			while ( curPtr != NULL ) {
				n = n + 1 ;
				curPtr = curPtr -> next ; 
			}
			
			if ( n == 3 ) {
				return true ;
			}
			
			return false ;
		}
		
		void enqueue( Job * newItem ) {
			QueueNode * newPtr = new QueueNode ;
			newPtr -> item = newItem ;
			newPtr -> next = NULL ;
			if ( isEmpty() ) {
				frontPtr = newPtr ;
				backPtr = newPtr ;
			}
			else {
				backPtr -> next = newPtr ;
				backPtr = newPtr ;
			}
		}
		
		void dequeue() {
			QueueNode * tempPtr ;
			tempPtr = frontPtr ;
			if ( frontPtr == backPtr ) {
				frontPtr = NULL ;
				backPtr = NULL ;
			}
			else {
				frontPtr = frontPtr -> next ;
			}
				
			delete tempPtr ;
			
		}
		
		JobPtr getFront() {
			
			return frontPtr -> item ;
			
		} 
	private :
		struct QueueNode {
			Job * item ;
			QueueNode * next ;
		};
		
		QueueNode * backPtr = NULL ;
		QueueNode * frontPtr = NULL ; 
};

class simulation {
	
	public :
		simulation() {
		}
		
		void run( Job * job ) {
			Done * curDonelist = NULL ;
			Aborts * curAbortlist = NULL ;
			Job * curjob = job ;
			
			
			while ( curjob != NULL ) {
				
				if( nowjob == NULL ) {
					nowjob = curjob ;
					curDonelist = new Done ;
					curDonelist -> next = NULL ;
					curDonelist -> OID = nowjob -> OID ;
					curDonelist -> Departure = nowjob -> Arrival + nowjob -> Duration ;
					curDonelist -> Delay = 0 ;
					time = curDonelist -> Departure ;
					Donelist = curDonelist ;
				}
				else {
				
					if ( aQueue.isFull() ) {
					
						if ( curjob -> Arrival < time ) {
							if ( curAbortlist == NULL ) {
								curAbortlist = new Aborts ;
								Abortlist = curAbortlist ;
							
							}
							else {
								curAbortlist -> next = new Aborts ;
								curAbortlist = curAbortlist -> next ;
							}
						
							curAbortlist -> next = NULL ;
							curAbortlist -> OID = curjob -> OID ;
							curAbortlist -> Abort = curjob -> Arrival ;
							curAbortlist -> Delay = 0 ;
						}
						else {
							
							
							if ( aQueue.getFront() -> Arrival > time ) {
								curDonelist -> next = new Done ;
								curDonelist = curDonelist -> next ;
								curDonelist -> next = NULL ;
								curDonelist -> OID = aQueue.getFront() -> OID ;
								curDonelist -> Departure = aQueue.getFront() -> Arrival + aQueue.getFront() -> Duration ;
								time = curDonelist -> Departure ;
								curDonelist -> Delay = 0 ;
								nowjob = aQueue.getFront() ;
								
							}
							else {
								if ( time > aQueue.getFront() -> TimeOut ) {
									if ( curAbortlist == NULL ) {
										curAbortlist = new Aborts ;
										Abortlist = curAbortlist ;
							
									}
									else {
										curAbortlist -> next = new Aborts ;
										curAbortlist = curAbortlist -> next ;
									}
						
									curAbortlist -> next = NULL ;
									curAbortlist -> OID = aQueue.getFront() -> OID ;
									curAbortlist -> Abort = time ;
									curAbortlist -> Delay = time - aQueue.getFront() -> Arrival ;
								}
								else if ( time + aQueue.getFront() -> Duration >= aQueue.getFront() -> TimeOut ) { //執行時逾時 
									if ( curAbortlist == NULL ) {
										curAbortlist = new Aborts ;
										Abortlist = curAbortlist ;
							
									}
									else {
										curAbortlist -> next = new Aborts ;
										curAbortlist = curAbortlist -> next ;
									}
						
									curAbortlist -> next = NULL ;
									curAbortlist -> OID = aQueue.getFront() -> OID ;
									curAbortlist -> Abort = aQueue.getFront() -> TimeOut ;
									

									curAbortlist -> Delay = aQueue.getFront() -> TimeOut - aQueue.getFront() -> Arrival ;
									time = curAbortlist -> Abort ;
							
								}
								else {
									curDonelist -> next = new Done ;
									curDonelist = curDonelist -> next ;
									curDonelist -> next = NULL ;
									curDonelist -> OID = aQueue.getFront() -> OID ;
									
									if ( aQueue.getFront() -> Arrival > time )
										curDonelist -> Departure = aQueue.getFront() -> Arrival + aQueue.getFront() -> Duration ;
									else
										curDonelist -> Departure = time + aQueue.getFront() -> Duration ;
										
							
					
									curDonelist -> Delay = time - aQueue.getFront() -> Arrival ;
									
									time = curDonelist -> Departure ;
									nowjob = aQueue.getFront() ;
				
								}	
									
							}
							
							aQueue.dequeue() ;
							aQueue.enqueue( curjob ) ;
						}
						
					}
				
					else {
						
						aQueue.enqueue( curjob ) ;
					
					}
				}
				
				curjob = curjob -> next ; 	
				
			}
			
			while ( !aQueue.isEmpty() ) {
				
				if ( aQueue.getFront() -> Arrival > time ) {
					curDonelist -> next = new Done ;
					curDonelist = curDonelist -> next ;
					curDonelist -> next = NULL ;
					curDonelist -> OID = aQueue.getFront() -> OID ;
					curDonelist -> Departure = aQueue.getFront() -> Arrival + aQueue.getFront() -> Duration ;
					time = curDonelist -> Departure ;
					curDonelist -> Delay = 0 ;
					nowjob = aQueue.getFront() ;
								
				}
				else {
					if ( time + aQueue.getFront() -> Duration >= aQueue.getFront() -> TimeOut ) {
						if ( curAbortlist == NULL ) {
							curAbortlist = new Aborts ;
							Abortlist = curAbortlist ;
							
						}
						else {
							curAbortlist -> next = new Aborts ;
							curAbortlist = curAbortlist -> next ;
						}
						
						curAbortlist -> next = NULL ;
						curAbortlist -> OID = aQueue.getFront() -> OID ;
						curAbortlist -> Abort = aQueue.getFront() -> TimeOut ;

						curAbortlist -> Delay = aQueue.getFront() -> TimeOut - aQueue.getFront() -> Arrival ;
							
					}
						else {
							curDonelist -> next = new Done ;
							curDonelist = curDonelist -> next ;
							curDonelist -> next = NULL ;
							curDonelist -> OID = aQueue.getFront() -> OID ;
							
							if ( aQueue.getFront() -> Arrival > time )
								curDonelist -> Departure = aQueue.getFront() -> Arrival + aQueue.getFront() -> Duration ;
							else
								curDonelist -> Departure = time + aQueue.getFront() -> Duration ;
										
							
					
							curDonelist -> Delay = time - aQueue.getFront() -> Arrival ;
									
							time = curDonelist -> Departure ;
							nowjob = aQueue.getFront() ;
				
					}	
									
				}
							
				aQueue.dequeue() ;	
				
			}
			/*
			if ( nowjob != NULL ) {
				curDonelist -> next = new Done ;
				curDonelist = curDonelist -> next ;
				curDonelist -> next = NULL ;
				curDonelist -> OID = nowjob -> OID ;
				curDonelist -> Departure = nowjob -> Arrival + nowjob -> Duration ;
				curDonelist -> Delay = nowjob -> Arrival + nowjob -> Duration ;
			}
			*/
			
			
			curAbortlist = Abortlist ;
			while ( curAbortlist != NULL ) {
				cout << curAbortlist -> OID << " " << curAbortlist -> Abort << " " << curAbortlist -> Delay << endl ;
				curAbortlist = curAbortlist -> next ;
			}
			
			cout << endl ;
			curDonelist = Donelist ;
			while ( curDonelist != NULL ) {
				cout << curDonelist -> OID << " " << curDonelist -> Departure << " " << curDonelist -> Delay << endl ;
				curDonelist = curDonelist -> next ;
			}
			
		}
		
	private :
		struct Aborts {
			int OID = 0 ;
			int Abort = 0 ;
			int Delay = 0 ;
			Aborts * next ;
		};
		
		struct Done {
			int OID = 0 ;
			int Departure = 0 ;
			int Delay = 0 ;
			Done * next ;
		};
		
		Aborts * Abortlist = NULL ;
		Done * Donelist = NULL ;
		Queue aQueue ;
		Job * nowjob = NULL ;
		int time = 0 ;
		int success = 0 ;
		int nowarrivaltime = 0 ;
		int AverageDelay = 0 ;
		int SuccessRate = 0 ;
		
};

int main() {
	
	int command = 0 ;
	
	do {
		cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
		cout << endl << "* 0. Quit                            *" ;
		cout << endl << "* 1. Sort a file                     *" ;
		cout << endl << "* 2. Simulate one FIFO queue         *" ;
		cout << endl << "**************************************" ;
		cout << endl << "Input a command(0, 1, 2): " ;
		cin >> command ;
		if ( command == 0 ) {
			break ;
		}
		else if ( command == 1 ) {
			file afile ;
			if ( afile.Inputfile() ) {
				afile.showcpu() ;
				afile.ShellSort() ;
				afile.Onputsortedfile() ;
				afile.showtime() ;
			}
			
		}
		else if ( command == 2 ) {
			file afile ;
			simulation asimulation ;
			if ( afile.Inputfile() ) {
				afile.ShellSort() ;
				asimulation.run( afile.getcpu() ) ;
				
			}
		}
		else {
			cout << endl << "Command does not exist!" << endl ;
		}
	} while ( 1 ) ;
	





}

