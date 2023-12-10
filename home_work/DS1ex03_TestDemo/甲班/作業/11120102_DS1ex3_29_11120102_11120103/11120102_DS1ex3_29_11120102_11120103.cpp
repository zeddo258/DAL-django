// 11120102���ͥ��B11120103��β[ 
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <iomanip>
using namespace std;

struct Sorted{
	int oID;
	int arrival;
	int duration;
	int timeOut;
}; 

struct SortedNode{
	int oID;
	int arrival;
	int duration;
	int timeOut;
	SortedNode *next;
}; 

struct DoneNode{
	int oID;
	int departure;
	int delay;
	DoneNode *next;
};

struct AbortNode{
	int oID;
	int absort;
	int delay;
	AbortNode *next;
};



class SortedList{
	private:
		SortedNode *head = NULL, *tail = NULL;
		int size = 0;
	
	
	public:
		void Push( SortedNode *push ){ // ��J�̫�
			size++;
			push->next = NULL;
			if( head == NULL )
				head = tail = push;
			else{
				tail->next = push;
				tail = tail->next;
			} 
		} // Push()
		
		
		SortedNode *Pop(){ // ���X�̫e����
			if( head == NULL )
				return NULL;
			size--;
			SortedNode *pop = head;
			head = head->next;
			pop->next = NULL;
			return pop;			
		} // Pop()
		
		
		SortedNode *Front(){
			return head;
		} // Front()
		
		
		SortedNode *Back(){
			return tail;
		} // Back()

		int Size(){
			return size;
		} // Size()

		void DeleteAll( SortedNode *del ){ // �q�̫᭱�}�l�R��
			if( del == NULL ) 
				return;
			DeleteAll(del->next);
			delete del;
		} // DeleteAll()
		
		
		void Initialize(){
			DeleteAll(head);
			head = tail = NULL;
			size = 0;
		} // Initialize()
	}; // class SortedList



class DoneList{
	private:
		DoneNode *head = NULL, *tail = NULL;
		int size = 0;
	
		
	public:
		void Push( DoneNode *push ){ // ��J�̫�
			size++;
			push->next = NULL;
			if( head == NULL )
				head = tail = push;
			else{
				tail->next = push;
				tail = tail->next;
			} 
		} // Push()
		
		
		DoneNode *Pop(){ // ���X�̫e���� 
			if( head == NULL )
				return NULL;
			size--;
			DoneNode *pop = head;
			head = head->next;
			pop->next = NULL;
			return pop;	
		} // Pop()
			
			
		DoneNode *Front(){
			return head;
		} // Front()
		
		
		DoneNode *Back(){
			return tail;
		} // Back()
		
		
		int Size(){
			return size;
		} // Size()
		
		
		void DeleteAll( DoneNode *del ){ // �q�̫᭱�}�l�R��
			if( del == NULL )
				return;
			DeleteAll(del->next);
			delete del;
		} // DeleteAll()
		
		void Initialize(){
			DeleteAll(head);
			head = tail = NULL;
			size = 0;
		} // Initialize()
}; // class DoneList



class AbortList{
	private:
		AbortNode *head = NULL, *tail = NULL;
		int size = 0;	
		
	public:		
		void Push( AbortNode *push ){ // ��J�̫�
			size++;
			push->next = NULL;
			if( head == NULL )
				head = tail = push;
			else{
				tail->next = push;
				tail = tail->next;
			} 
		} // Push()
		
		
		AbortNode *pop(){ // ���X�̫e���� 
			if( head == NULL )
				return NULL;
			size--;
			AbortNode *pop = head;
			head = head->next;
			pop->next = NULL;
			return pop;	
		} // Pop()
		
		
		AbortNode *Front(){
			return head;
		} // Front()
		
		
		AbortNode *Back(){
			return tail;
		} // Back()
		
		
		int Size(){
			return size;
		} // Suze()
		
		
		void DeleteAll( AbortNode *del ){ // �q�̫᭱�}�l�R��
			if( del == NULL )
				return;
			DeleteAll(del->next);
			delete del;
		} // DeleteAll()
		
		
		void Initialize(){
			DeleteAll(head);
			head = tail = NULL;
			size = 0;
		} // Initialize()
		
		
}; // class AbortList



class Procedure{
	public:
		int command, fileNum;
		SortedList sorted, waiting;
		DoneList done;
		AbortList abort;
		double readfirst = 0, readlast = 0, sortedfirst = 0, sortedlast = 0, writefirst = 0, writelast = 0 ;
		
		
		
		
		int GetCommand(){
			while( true ){
				cout << endl;
				cout << "**** Simulate FIFO Queues by SQF *****" << endl;
				cout << "* 0. Quit                            *" << endl;
				cout << "* 1. Sort a file                     *" << endl;
				cout << "* 2. Simulate one FIFO queue         *" << endl;
				cout << "**************************************" << endl;
				cout << "Input a command(0, 1, 2): ";
				
				string get;
				cin >> get;
				if( stoi(get) == 0 || stoi(get) == 1 || stoi(get) == 2 ){
					command = stoi(get);
					return command;
				}
				cout << endl << "Command does not exist!" << endl;
			}
		} // GeetCommand()
			
			
			
		
		bool ReadFile(){
			sorted.Initialize();
			waiting.Initialize();
			done.Initialize();
			abort.Initialize(); 
			
			cout << endl << "Input a file number: ";
			string get;
			cin >> get; 
			
			ifstream fin;
			fin.open("input"+get+".txt");
			if( ! fin.is_open() ) { 
				cout << endl << "### input" << get << ".txt does not exist! ###" << endl;
				return false;
			}
			fileNum = stoi(get);

			string garbage;
			fin >> garbage >> garbage >> garbage >> garbage;
			while( ! fin.eof() ){
				SortedNode *get = new SortedNode();
				fin >> get->oID >> get->arrival >> get->duration >> get->timeOut;
				if( get->arrival == 0 ){
					delete get;
					break;
				}
				sorted.Push(get);
			}
			fin.close();

		} // ReadFile()
		
		
		
		
		void To_Array( Sorted sort[], SortedNode * first ) {
			SortedNode * temp = first ;
			int i = 0 ;
			while ( i < sorted.Size() ) {
				sort[i].oID = temp -> oID ;
				sort[i].arrival = temp -> arrival ;
				sort[i].duration = temp -> duration ;
				sort[i].timeOut = temp -> timeOut ;
				temp = temp -> next ;
				i++ ;
			}
		}
		
		
		void Exchange( Sorted sort[], int k, int step ) {
			Sorted temp ;
			
			temp.oID = sort[k + step].oID ;
			temp.arrival = sort[k + step].arrival ;
			temp.duration = sort[k + step].duration ;
			temp.timeOut = sort[k + step].timeOut ;
			
			sort[k + step].oID = sort[k].oID ;
			sort[k + step].arrival = sort[k].arrival ;
			sort[k + step].duration = sort[k].duration ;
			sort[k + step].timeOut = sort[k].timeOut ;
			
			sort[k].oID = temp.oID ;
			sort[k].arrival = temp.arrival ;
			sort[k].duration = temp.duration ;
			sort[k].timeOut = temp.timeOut ;
		}
		
		void To_Linked_List( Sorted sort[] ) {
			int i = 0, j = 0 ;
			SortedNode * temp = sorted.Front() ;
			j = sorted.Size() ;
			
			while ( i < j ) {
				temp -> oID = sort[i].oID ;
				temp -> arrival = sort[i].arrival ;
				temp -> duration = sort[i].duration ;
				temp -> timeOut = sort[i].timeOut ;
				temp = temp -> next ; 
				i += 1 ;
			}
		}
		
		void ShellSort() {
			SortedNode * first = sorted.Front() ;
			int length = sorted.Size() ;
			Sorted sort[sorted.Size()] ;
			To_Array( sort ,first ) ;
			int step = length / 2 ;
			// cout << step << endl << num << endl ;
			
			sortedfirst = clock() ;
			for ( step ; step >= 1 ; step /= 2 ) {
				int num = length / step ;
				num = num + length % step ;
				
				for ( int i = 0 ; i < step ; i++ ) { // ??�X? 
					for ( int k = 0 ; k < num ; k += 1 ) {
						for ( int j = step ; j < length ; j += step ) {
							if ( sort[j - step].arrival > sort[j].arrival ) 
								Exchange( sort, j - step , step ) ;
							
							else if ( sort[j - step].arrival == sort[j].arrival ) {
								if ( sort[j - step].oID > sort[j].oID ) 
									Exchange( sort, j - step, step ) ;
							}
						}
					}
				}
			}
			sortedlast = clock() ;
			
			To_Linked_List( sort ) ;
			
			//cout << sortedfirst << endl << sortedlast << endl ;
		} // ShellSort

		
		
	
		
		void CPU(){			
			SortedNode *curr = NULL; // �ثe���b���檺��� 			
			for( int time = 1, executionEnd = 0 ; sorted.Front() != NULL || waiting.Front() != NULL ; time++ ){		
					
				if( done.Front() == NULL || time >= executionEnd ){ // �̶}�l(�S���������)�A�έ�n���浲��(���榨�\�A�ΰ���O��) 
					if( curr != NULL ){ // �ư��̶}�l�����p 
						delete curr;
						curr = NULL;
					}
										
					while( waiting.Front() != NULL ){ // waiting���F�� 
						if( ! WaitingTimeOut(time) ){ // �b���ݹL�{���S���O�ɡA(�����ܡA�w�qwaiting���X�é��abort) 
							
							if( time + waiting.Front()->duration <= waiting.Front()->timeOut ){ // ���|�����O��
								curr = ToDone(time,'w'); // �|���\����A�w�qwauting���X�é��done�A
								executionEnd = done.Back()->departure;
							}
							
							else{ // �|�����O��
								curr = ExecutionTimeOut(time); // �w�qwaiting���X�é��abort�A�٬O�|����
								executionEnd = abort.Back()->absort;									
							}
							break;
						} // if
					} // while
					
					if( curr == NULL && sorted.Front() != NULL ){ // waiting�S��ơA��waiting����Ƴ��Q�����F
						time = sorted.Front()->arrival;
						curr = ToDone(time,'s'); // �|���\����A�w�qsorted���X�é��done 
						executionEnd = done.Back()->departure;
					}
				} // if
				
				if( curr != NULL ){ // �٨S���槹 
					while( sorted.Front() != NULL && time == sorted.Front()->arrival ){ // �N����ƨ�F(�i��P�ɦ��n�X��) 
						if( ! FullWait(time) ) // waiting�S���A(���F���ܡA�w�qsorted���X�é��abort) 
							waiting.Push(sorted.Pop()); // �qsorted���X�é��waiting 
					}
				} // if
			} // for 
			
		} // CPU()
		
		
		
		
		SortedNode *ToDone( int time, char from ){
			SortedNode *curr;
			if( from == 's' ) // SortedToDone	
				curr = sorted.Pop(); // �qsorted���X�A�å�curr���V 
			else // WaitingToDone 
				curr = waiting.Pop(); // �qwaiting���X�A�å�curr���V
			DoneNode *push = new DoneNode();
			push->oID = curr->oID;
			push->departure = time +curr->duration;
			push->delay = time -curr->arrival;
			done.Push(push); // (���e)���done
			return curr;
		} // ToDone()
		
				
		
		
		bool FullWait( int time ){ // �L�k�i�J���ݰϡA����(4.1)			
			if( waiting.Size() == 3 ){
				SortedNode *takeOut = sorted.Pop(); // �qsorted���X
				AbortNode *push = new AbortNode();
				push->oID = takeOut->oID;
				push->absort = time; // ==takeOut.arrival
				push->delay = 0;
				abort.Push(push); // ���abort
				delete takeOut; 
				return true;
			}
			return false;
		} // FullWait()
		
		
		
		
		bool WaitingTimeOut( int time ){ // �b���ݹL�{���O�ɡA����(4.2)
			if( waiting.Front()->timeOut <= time ){
				SortedNode *takeOut = waiting.Pop(); // �qwaiting���X
				AbortNode *push = new AbortNode();
				push->oID = takeOut->oID;
				push->absort = time;
				push->delay = push->absort - takeOut->arrival;
				abort.Push(push); // (���e)���abort 
				delete takeOut;
				return true;
			}
			return false;
		} // WaitingTimeOut()
		
		
		
		
		SortedNode *ExecutionTimeOut( int time ){ // �����O�ɡA����(4.3)
			SortedNode *curr = waiting.Pop(); // �qwaiting���X�A�å�curr���V 
			AbortNode *push = new AbortNode();
			push->oID = curr->oID;
			push->absort = curr->timeOut;
			push->delay = push->absort - curr->arrival;
			abort.Push(push); // (���e)���abort
			return curr;
		} // ExecutionTimeOut()		
		
		
		
		
		void WriteFile1(){
			ofstream fout;
			fout.open("sorted"+to_string(fileNum)+".txt", ios::out|ios::trunc);
			fout << "OID" << "\tArrival" << "\tDuration" << "\tTimeOut" << endl;
			for( SortedNode *curr = sorted.Front() ; curr != NULL ; curr = curr->next )
				fout << curr->oID << "\t" << curr->arrival << "\t" << curr->duration << "\t" << curr->timeOut << endl;
			fout.close();

		} // WriteFile1()	
		
		
		void Print_time() {
			printf( "Reading data: %d clocks (%.2f ms).\n", readlast - readfirst, readlast - readfirst ) ;
			printf( "Sorting data: %d clocks (%.2f ms).\n", sortedlast - sortedfirst, sortedlast - sortedfirst ) ;
			printf( "Writing data: %d clocks (%.2f ms).\n", writelast - writefirst, writelast - writefirst ) ;
		}

			
		void Print1(){
			cout << endl << "\tOID" << "\tArrival" << "\tDuration" << "\tTimeOut" << endl;
			int i = 1;
			for( SortedNode *curr = sorted.Front() ; curr != NULL ; curr = curr->next, i++ )
				cout << "[" << i << "]\t" << curr->oID << "\t" << curr->arrival << "\t" << curr->duration << "\t" << curr->timeOut << endl;

			cout << endl ;
			Print_time() ;
			cout << endl << "See sorted" << fileNum << ".txt" << endl;
		} // Print1()
		
		
		
		
		double AverageDelay(){
			double totalDelay = 0;
			for( DoneNode *curr = done.Front() ; curr != NULL ; curr = curr->next )
				totalDelay += curr->delay;
			for( AbortNode *curr = abort.Front() ; curr != NULL ; curr = curr->next )
				totalDelay += curr->delay;
			return totalDelay /( done.Size() + abort.Size() );
		} // AverageDelay()
		
		
		
		
		double SuccessRate(){
			double successNum = done.Size();
			return successNum /( done.Size() + abort.Size() ) *100;
		} // SuccessRate()
		
		
		
		void WriteFile2(){
			if( done.Front() == NULL )
				return;
				
			ofstream fout;
			fout.open("output"+to_string(fileNum)+".txt", ios::out|ios::trunc);
			
			fout << "\t[Abort Jobs]" << endl;
			fout << "\tOID" << "\tAbort" << "\tDelay" << endl;
			int i = 1;
			for( AbortNode *curr = abort.Front() ; curr != NULL ; curr = curr->next, i++ )
				fout << "[" << i << "]\t" << curr->oID << "\t" << curr->absort << "\t" << curr->delay << endl;
				
			fout << "\t[Jobs Done]" << endl;
			fout << "\tOID" << "\tDeparture" << "\tDelay" << endl;
			i = 1;
			for( DoneNode *curr = done.Front() ; curr != NULL ; curr = curr->next, i++ )
				fout << "[" << i << "]\t" << curr->oID << "\t" << curr->departure << "\t" << curr->delay << endl;
				
			
			fout << "[Average Delay]\t" << fixed << setprecision(2) << AverageDelay() << " ms" << endl;
			fout << "[Success Rate]\t" << fixed << setprecision(2) << SuccessRate() << " %" << endl;
			fout.close();
		} // WriteFile2()
		
		
		
		
		void Print2(){
			cout << endl << "The simulation is running..." << endl;
			cout << "See output" << fileNum << ".txt"<< endl;
		} // Print2()
}; // class Procedure



int main(){
	Procedure p;
	
	while( p.GetCommand() ){ // command��0�|���� 
		if( p.command == 1 && p.ReadFile() ){
			p.ShellSort();
			p.WriteFile1();
			p.Print1();
		}
		else if( p.command == 2 ){
			p.CPU();
			p.WriteFile2();
			p.Print2();
		}
	} // while
}
