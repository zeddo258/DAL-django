// 11120102莊凱任、11120103游舒涵 
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
		void Push( SortedNode *push ){ // 放入最後
			size++;
			push->next = NULL;
			if( head == NULL )
				head = tail = push;
			else{
				tail->next = push;
				tail = tail->next;
			} 
		} // Push()
		
		
		SortedNode *Pop(){ // 取出最前面的
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

		void DeleteAll( SortedNode *del ){ // 從最後面開始刪除
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
		void Push( DoneNode *push ){ // 放入最後
			size++;
			push->next = NULL;
			if( head == NULL )
				head = tail = push;
			else{
				tail->next = push;
				tail = tail->next;
			} 
		} // Push()
		
		
		DoneNode *Pop(){ // 取出最前面的 
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
		
		
		void DeleteAll( DoneNode *del ){ // 從最後面開始刪除
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
		void Push( AbortNode *push ){ // 放入最後
			size++;
			push->next = NULL;
			if( head == NULL )
				head = tail = push;
			else{
				tail->next = push;
				tail = tail->next;
			} 
		} // Push()
		
		
		AbortNode *pop(){ // 取出最前面的 
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
		
		
		void DeleteAll( AbortNode *del ){ // 從最後面開始刪除
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
				
				for ( int i = 0 ; i < step ; i++ ) { // ??°? 
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
			SortedNode *curr = NULL; // 目前正在執行的資料 			
			for( int time = 1, executionEnd = 0 ; sorted.Front() != NULL || waiting.Front() != NULL ; time++ ){		
					
				if( done.Front() == NULL || time >= executionEnd ){ // 最開始(沒有任何執行)，或剛好執行結束(執行成功，或執行逾時) 
					if( curr != NULL ){ // 排除最開始的情況 
						delete curr;
						curr = NULL;
					}
										
					while( waiting.Front() != NULL ){ // waiting有東西 
						if( ! WaitingTimeOut(time) ){ // 在等待過程中沒有逾時，(有的話，已從waiting取出並放到abort) 
							
							if( time + waiting.Front()->duration <= waiting.Front()->timeOut ){ // 不會執行到逾時
								curr = ToDone(time,'w'); // 會成功執行，已從wauting取出並放到done，
								executionEnd = done.Back()->departure;
							}
							
							else{ // 會執行到逾時
								curr = ExecutionTimeOut(time); // 已從waiting取出並放到abort，還是會執行
								executionEnd = abort.Back()->absort;									
							}
							break;
						} // if
					} // while
					
					if( curr == NULL && sorted.Front() != NULL ){ // waiting沒資料，或waiting的資料都被取消了
						time = sorted.Front()->arrival;
						curr = ToDone(time,'s'); // 會成功執行，已從sorted取出並放到done 
						executionEnd = done.Back()->departure;
					}
				} // if
				
				if( curr != NULL ){ // 還沒執行完 
					while( sorted.Front() != NULL && time == sorted.Front()->arrival ){ // 就有資料到達(可能同時有好幾筆) 
						if( ! FullWait(time) ) // waiting沒滿，(滿了的話，已從sorted取出並放到abort) 
							waiting.Push(sorted.Pop()); // 從sorted取出並放到waiting 
					}
				} // if
			} // for 
			
		} // CPU()
		
		
		
		
		SortedNode *ToDone( int time, char from ){
			SortedNode *curr;
			if( from == 's' ) // SortedToDone	
				curr = sorted.Pop(); // 從sorted取出，並用curr指向 
			else // WaitingToDone 
				curr = waiting.Pop(); // 從waiting取出，並用curr指向
			DoneNode *push = new DoneNode();
			push->oID = curr->oID;
			push->departure = time +curr->duration;
			push->delay = time -curr->arrival;
			done.Push(push); // (提前)放到done
			return curr;
		} // ToDone()
		
				
		
		
		bool FullWait( int time ){ // 無法進入等待區，取消(4.1)			
			if( waiting.Size() == 3 ){
				SortedNode *takeOut = sorted.Pop(); // 從sorted取出
				AbortNode *push = new AbortNode();
				push->oID = takeOut->oID;
				push->absort = time; // ==takeOut.arrival
				push->delay = 0;
				abort.Push(push); // 放到abort
				delete takeOut; 
				return true;
			}
			return false;
		} // FullWait()
		
		
		
		
		bool WaitingTimeOut( int time ){ // 在等待過程中逾時，取消(4.2)
			if( waiting.Front()->timeOut <= time ){
				SortedNode *takeOut = waiting.Pop(); // 從waiting取出
				AbortNode *push = new AbortNode();
				push->oID = takeOut->oID;
				push->absort = time;
				push->delay = push->absort - takeOut->arrival;
				abort.Push(push); // (提前)放到abort 
				delete takeOut;
				return true;
			}
			return false;
		} // WaitingTimeOut()
		
		
		
		
		SortedNode *ExecutionTimeOut( int time ){ // 執行到逾時，取消(4.3)
			SortedNode *curr = waiting.Pop(); // 從waiting取出，並用curr指向 
			AbortNode *push = new AbortNode();
			push->oID = curr->oID;
			push->absort = curr->timeOut;
			push->delay = push->absort - curr->arrival;
			abort.Push(push); // (提前)放到abort
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
	
	while( p.GetCommand() ){ // command為0會結束 
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
