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
		
		void Inputfile( string afilenumber ) {
			ifstream in ;
			string temp ;
			string filename ;
			Job * current = NULL ;
			filename = "input" + afilenumber + ".txt" ;
			in.open( filename.c_str() ) ;
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
			
			in.close() ;
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
		
		string returnfilenumber() {
			return filenumber ;
		}
		
		void claer() {
			if ( cpu != NULL ) {
				Job * temp = NULL ;
				Job * curcpu = cpu ;
				while ( curcpu != NULL ) {
					temp = curcpu ;
					curcpu = curcpu -> next ;
					delete temp ;
				}
				
			}
			
			cpu = NULL ;
			num  = 0 ;
			read_time = 0 ;
			sort_time = 0 ;
			write_time = 0 ;
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
		
		bool inputsortedfile() {
			ifstream in ;
			string filename ;
			
			Job * current = NULL ;
			cout << endl << "Input a file number: " ;
			cin >> filenumber ;
			filename = "sorted" + filenumber + ".txt" ;
			in.open( filename.c_str() ) ;
			if ( in.fail() ) {
				cout << endl << "### " << filename << " does not exist! ###" << endl ; 
				in.close() ;
				return false ;
			}
			
			in.close() ;
			return true ;
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
								else if ( time + aQueue.getFront() -> Duration > aQueue.getFront() -> TimeOut ) { //執行時逾時 
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
					else if ( time + aQueue.getFront() -> Duration > aQueue.getFront() -> TimeOut ) { //執行時逾時 
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
				
			}
			
			CalculateAverageDelay() ;
			CalculateSuccessRate() ;
			
			
			/*
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
			
			cout << endl << fixed << setprecision(2) << AverageDelay << endl << fixed << setprecision(2) << SuccessRate ;
			*/
			
		}
		
		void CalculateAverageDelay() {
			Aborts * curAbort = Abortlist ;
			Done * curDonelist = Donelist ;
			double n = 0 ;
			while ( curAbort != NULL ) {
				AverageDelay = AverageDelay + curAbort -> Delay ;
				n = n + 1 ;
				curAbort = curAbort -> next ;
			}
			
			while ( curDonelist != NULL ) {
				AverageDelay = AverageDelay + curDonelist -> Delay ;
				n = n + 1 ;
				curDonelist = curDonelist -> next ;
			}
			
			AverageDelay = AverageDelay / n ;
		}
		
		void CalculateSuccessRate() {
			Aborts * curAbort = Abortlist ;
			Done * curDonelist = Donelist ;
			double n = 0 ;
			double success = 0 ;
			while ( curAbort != NULL ) {
				n = n + 1 ;
				curAbort = curAbort -> next ;
			}
			
			while ( curDonelist != NULL ) {
				n = n + 1 ;
				success = success + 1 ;
				curDonelist = curDonelist -> next ;
			}
			
			SuccessRate = success / n * 100.0 ;
		}
		
		void output( string filenumber ) {
			
			Aborts * curAbort = Abortlist ;
			Done * curDonelist = Donelist ;
			string filename = "output" + filenumber + ".txt" ;
			cout << endl << "The simulation is running..." << endl ;
			cout << "See " << filename << endl ; 
			ofstream out( filename.c_str() ) ;
			out << "\t" << "[Abort Jobs]" << endl ;
			out << "\t" << "OID" << "\t" << "Abort" << "\t" << "Delay" << endl ;
			for ( int i = 1 ; curAbort != NULL ; i ++ ) {
				out << "[" << i << "]" << "\t" << curAbort -> OID << "\t" << curAbort -> Abort << "\t" << curAbort -> Delay << endl ;
				curAbort = curAbort -> next ; 
			} 
			
			out << "\t" << "[Jobs Done]" << endl ;
			out << "\t" << "OID" << "\t" << "Departure" << "\t" << "Delay" << endl ;
			for ( int i = 1 ; curDonelist != NULL ; i ++ ) {
				out << "[" << i << "]" << "\t" << curDonelist -> OID << "\t" << curDonelist -> Departure << "\t" << curDonelist -> Delay << endl ;
				curDonelist = curDonelist -> next ; 
			}
			
			out << "[AverageDelay]" << fixed << setprecision(2) << AverageDelay << " ms"<< endl ;
			out << "[SuccessRate]" << fixed << setprecision(2) << SuccessRate << " %" << endl ;
			
			out.close() ;
		}
		
		string returnfilenumber() {
			return filenumber ;
		}
		
		void claer() {
			if ( Abortlist!= NULL ) {
				Aborts * curAbortlist = Abortlist ;
				Aborts * Aborttemp = NULL ;
				while ( curAbortlist != NULL ) {
					Aborttemp = curAbortlist ;
					curAbortlist = curAbortlist -> next ;
					delete Aborttemp ;
				}
				
				
			}
			
			if ( Donelist != NULL ) {
				Done * curDonelist = Donelist ;
				Done * Donetemp = NULL ;
				while ( curDonelist != NULL ) {
					Donetemp = curDonelist ;
					curDonelist = curDonelist -> next ;
					delete Donetemp ;
				}
				
				
			}
			
			Abortlist = NULL ;
			Donelist = NULL ;
			nowjob = NULL ;
			time = 0 ;
			AverageDelay = 0 ;
			SuccessRate = 0 ;
			
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
		string filenumber ;
		int time = 0 ;
		//int success = 0 ;
		//int nowarrivaltime = 0 ;
		double AverageDelay = 0 ;
		double SuccessRate = 0 ;
		
};

int main() {
	
	int command = 0 ;
	bool iscommand1 = false ;
	file afile ;
	simulation asimulation ;
	
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
	
			afile.claer() ;
			
			if ( afile.Inputfile() ) {
				afile.showcpu() ;
				afile.ShellSort() ;
				afile.Onputsortedfile() ;
				afile.showtime() ;
				iscommand1 = true ;
			}
			
		}
		else if ( command == 2 ) {
			
			if ( iscommand1 ) {
				asimulation.run( afile.getcpu() ) ;
				asimulation.output( afile.returnfilenumber() ) ;
				
			}
			else {
				
				if ( asimulation.inputsortedfile() ) {
					afile.Inputfile(  asimulation.returnfilenumber() ) ;
					afile.ShellSort() ;
					asimulation.run( afile.getcpu() ) ;
					asimulation.output( asimulation.returnfilenumber() ) ;
				
				}
			}
			
			asimulation.claer() ;
		}
		else {
			cout << endl << "Command does not exist!" << endl ;
		}
	} while ( 1 ) ;
	





}

