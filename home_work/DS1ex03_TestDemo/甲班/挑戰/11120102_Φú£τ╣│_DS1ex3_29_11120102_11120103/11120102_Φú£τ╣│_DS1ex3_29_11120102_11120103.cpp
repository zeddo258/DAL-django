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
	int cID;
	int departure;
	int delay;
	DoneNode *next;
};


struct AbortNode{
	int oID;
	int cID;
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
		SortedList sorted, waiting, waiting2;
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
				cout << "* 3. Simulate two queues by SQF      *" << endl;
				cout << "**************************************" << endl;
				cout << "Input a command(0, 1, 2, 3): ";
				
				string get;
				cin >> get;
				if( stoi(get) == 0 || stoi(get) == 1 || stoi(get) == 2 || stoi(get) == 3 ){
					command = stoi(get);
					return command;
				}
				cout << "Command does not exist!" << endl;
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

		
		
		SortedList BackUp(){
			SortedList bu;
			for( SortedNode *temp = sorted.Front() ; temp != NULL ; temp = temp->next ){
				SortedNode *push = new SortedNode(*temp);
				bu.Push(push);
			}
			return bu;
		} // BackUp()
		
	
		
		
		void CPU(){	
			SortedList _sorted = BackUp();		
			SortedNode *curr = NULL; // �ثe���b���檺��� 			
			for( int time = 0, executionEnd = 0 ; sorted.Front() != NULL || waiting.Front() != NULL ; time++ ){		
					
				if( time == executionEnd ){ // �̶}�l(�S���������)�A�έ�n���浲��(���榨�\�A�ΰ���O��) 
					if( curr != NULL ){ // �ư��̶}�l�����p 
						delete curr;
						curr = NULL;
					}
										
					while( waiting.Front() != NULL ){ // waiting���F�� 
						if( waiting.Front()->timeOut <= time ) // �b���ݹL�{���O�� 
							WaitingTimeOut(waiting.Pop(),time,0); // �qwaiting���X�é��abort
						else{ // �b���ݹL�{���S���O��
							if( time + waiting.Front()->duration <= waiting.Front()->timeOut ) // ���|�����O��
								curr = ToDone(waiting.Pop(),time,executionEnd,0); // �|���\����A�qwaiting���X�é��done
							else // �|�����O��
								curr = ExecutionTimeOut(waiting.Pop(),time,executionEnd,0); // �|�����A�qwaiting���X�é��abort�A�٬O�|������
							break;
						} // else
					} // while
					
					if( curr == NULL && sorted.Front() != NULL ){ // curr�S����ƥi�H����A(waiting�S��ơA��waiting��Ƴ��Q�����F)
						time = sorted.Front()->arrival;
						curr = ToDone(sorted.Pop(),time,executionEnd,0); // �|���\����A�qsorted���X�é��done 
					}
				} // if
				
				if( curr != NULL ){ // �٨S���槹 
					while( sorted.Front() != NULL && time == sorted.Front()->arrival ){ // �N����ƨ�F(�i��P�ɦ��n�X��) 
						if( waiting.Size() == 3 )
							FullWait(sorted.Pop()); // �qsorted���X�A���abort
						else
							waiting.Push(sorted.Pop()); // �qsorted���X�A���waiting 
					}
				} // if
				
			} // for 
			sorted = _sorted;
		} // CPU()
		
		
		
		
		void CPU2(){
			SortedList _sorted = BackUp();		

			SortedNode *curr = NULL, *curr2 = NULL; // �ثe���b���檺��� 			
			for( int time = 0, executionEnd = 0,  executionEnd2 = 0 ; sorted.Front() != NULL || waiting.Front() != NULL || waiting2.Front() != NULL ; time++ ){		
					
				if( time == executionEnd ) // curr1��n���浲��
					curr = ExeEnd(waiting,time,executionEnd,1);
				
				if( time == executionEnd2 ) // curr2��n���浲��
					curr2 = ExeEnd(waiting2,time,executionEnd2,2);
									
				
				while( sorted.Front() != NULL && sorted.Front()->arrival == time ){ // ����ƨ�F 								
					int size1 = waiting.Size(), size2 = waiting2.Size();
					if( curr == NULL )
						curr = ToDone(sorted.Pop(),time,executionEnd,1);
					else if( curr2 == NULL )
						curr2 = ToDone(sorted.Pop(),time,executionEnd2,2);
					else if( size1 == 3 && size2 == 3 )
						FullWait(sorted.Pop());
					else if( size1 <= size2 )
						waiting.Push(sorted.Pop());
					else if( size2 < size1 )
						waiting2.Push(sorted.Pop());
				} // while 
				
			} // for
			sorted = _sorted;
		} // CPU2()
		
		
		
		
		SortedNode *ExeEnd( SortedList & w, int time, int & exeEnd, int cID ){
			SortedNode *c = NULL;
			
			while( w.Front() != NULL ){
				if( w.Front()->timeOut <= time ){ // �b���ݹL�{���O��
					WaitingTimeOut(w.Pop(),time,cID); // �qwaiting���X�é��abort 
				}
				else{
					if( time + w.Front()->duration <= w.Front()->timeOut )// ���|�����O�� 
						c = ToDone(w.Pop(),time,exeEnd,cID); // �qwaiting���X�é��done 
					else c = ExecutionTimeOut(w.Pop(),time,exeEnd,cID); // �qwaiting���X�é��abort 
					break;
				} // else
			} // while	

			return c;
		} // ExeEnd()
		
		
		
		
		SortedNode *ToDone( SortedNode *takeOut, int time, int & executionEnd, int cID ){	
			// ��qsorted��waiting���X�����done 
			DoneNode *push = new DoneNode();
			push->oID = takeOut->oID;
			push->cID = cID;
			push->departure = time +takeOut->duration;
			push->delay = time -takeOut->arrival;
			done.Push(push);
			executionEnd = push->departure;
			return takeOut;
		} // ToDone()
		
				
		
		
		void FullWait( SortedNode *takeOut ){ // �L�k�i�J���ݰϡA����(4.1)			
			// ��qsorted���X�����abort 
			AbortNode *push = new AbortNode();
			push->oID = takeOut->oID;
			push->cID = 0;
			push->absort = takeOut->arrival;
			push->delay = 0;
			abort.Push(push); 
			delete takeOut; 
		} // FullWait()
		
		
		
		
		void WaitingTimeOut( SortedNode *takeOut, int time, int cID ){ // �b���ݹL�{���O�ɡA����(4.2)
			// ��qwaiting���X�����abort  
			AbortNode *push = new AbortNode();
			push->oID = takeOut->oID;
			push->cID = cID;
			push->absort = time;
			push->delay = push->absort - takeOut->arrival;
			abort.Push(push);
			delete takeOut;
		} // WaitingTimeOut()
		
		
		
		
		SortedNode *ExecutionTimeOut( SortedNode *takeOut, int time, int & executionEnd, int cID ){ // �����O�ɡA����(4.3)
			// ��qwaiting���X�����abort 
			AbortNode *push = new AbortNode();
			push->oID = takeOut->oID;
			push->cID = cID;
			push->absort = takeOut->timeOut;
			push->delay = push->absort - takeOut->arrival;
			abort.Push(push);
			executionEnd = takeOut->timeOut;
			return takeOut;
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
			if( command == 3 )
				fout.open("double"+to_string(fileNum)+".txt", ios::out|ios::trunc);
			else fout.open("output"+to_string(fileNum)+".txt", ios::out|ios::trunc);
			
			fout << "\t[Abort Jobs]" << endl;
			fout << "\tOID";
			if( command == 3 )
				fout << "\tCID";
			fout << "\tAbort" << "\tDelay" << endl;
			
			int i = 1;
			for( AbortNode *curr = abort.Front() ; curr != NULL ; curr = curr->next, i++ ){
				fout << "[" << i << "]\t" << curr->oID;
				if( command == 3 )
					fout << "\t" << curr->cID;
				fout << "\t" << curr->absort << "\t" << curr->delay << endl;
			}
				
			fout << "\t[Jobs Done]" << endl;
			fout << "\tOID";
			if( command == 3 )
			fout << "\tCID";
			fout << "\tDeparture" << "\tDelay" << endl;
			
			i = 1;
			for( DoneNode *curr = done.Front() ; curr != NULL ; curr = curr->next, i++ ){
				fout << "[" << i << "]\t" << curr->oID;
				if( command == 3 )
					fout << "\t" << curr->cID;
				fout << "\t" << curr->departure << "\t" << curr->delay << endl;
			} 
				
			fout << "[Average Delay]\t" << fixed << setprecision(2) << AverageDelay() << " ms" << endl;
			fout << "[Success Rate]\t" << fixed << setprecision(2) << SuccessRate() << " %" << endl;
			fout.close();
		} // WriteFile2()
		
		
		
		
		void Print2(){
			cout << endl << "The simulation is running..." << endl;
			if( command == 3 )
				cout << "See double" << fileNum << ".txt"<< endl;
			else cout << "See output" << fileNum << ".txt"<< endl;
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
		else if( p.sorted.Front() != NULL || ( p.sorted.Front() == NULL && p.ReadFile() ) ){
			if( p.command == 2 ){
				p.ShellSort();
				p.CPU();
				p.WriteFile2();
				p.Print2();
			}
			else if( p.command == 3 ){
				// input304 3�����D 
				p.ShellSort();
				p.CPU2();
				p.WriteFile2();
				p.Print2();
			}
		} // else if
		
	} // while
} // main()


