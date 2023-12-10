// 11127108 ���f�, 11127161 �B��ʳ

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
#include <iomanip> // �Τp���I�� 

using namespace std ; 

struct Type {
	int oid = -1 ;
	int arrival = -1 ;
	int duration = -1 ;
	int timeout = -1 ;
};

struct TypeA { // �s���Ѫ� 
	int oid = -1;
	int delay = -1;
	int abort = -1;
};

struct TypeD { // �s���\�� 
	int oid = -1;
	int delay = -1;
	int Departure = -1;
};

class Queue {
	vector<Type> queue ;
	
public:
		
	int size() {
		return queue.size() ;
	}
	
	bool isEmpty() {
		if ( queue.size() == 0 )
			return true ;
		return false ;
	}
	
	bool isFull() {
		if ( queue.size() == 3 )
			return true ;
		return false ;
	}
	
	void enQueue( Type data ) {
		queue.push_back( data ) ;
	}
	
	void deQueue() {
		queue.erase( queue.begin() ) ;
	}
		
	Type getFront() {
		if ( queue.size() >= 1 )
			return queue[0] ;
	}
};


void ShellSort( vector<Type> &process ) {   // �ƺ��Ƨ� 
	for ( int h = process.size() / 2 ; h > 0 ; h = h / 2 ) {
		for ( int unsort = h ; unsort < process.size() ; unsort++ ) {
			int i = unsort ;
			int oid = process[unsort].oid ;
			int arrival = process[unsort].arrival ;
			int duration = process[unsort].duration ;
			int timeout = process[unsort].timeout ;
			
			// ��j�p 
			while( ( i >= h && process[i-h].arrival > arrival ) || 
				   ( i >= h && process[i-h].arrival == arrival && process[i-h].oid > oid ) ) {
				process[i] = process[i-h] ;
				i = i - h ;
			}
			process[i].oid = oid ;
			process[i].arrival = arrival ;
			process[i].duration = duration ;
			process[i].timeout = timeout ;
		}
	}
}

void Sort() {
	vector<Type> process ;
	string fileName ;
	cout << endl << "Input a file number: " ;
	cin >> fileName ;
	
	Type data ;
	string str ;
	string num = "" ; 
	double sread, ssort, swrite ;  // Ū�ɡB�ƧǡB�g�ɮɶ� 
	double end ;  // �����ɶ� 
	
	string input = "input" + fileName + ".txt" ; 
	fstream file ;
	file.open( input.c_str() ) ;  // �}�ҦW��inputXXX.txt���ɮ� 
	
	if ( file == NULL ) {  // �䤣���ɮ� 
		cout << endl << "### " << fileName << " does not exist! ###" << endl ; 		
		return ;
	} 
	
	ofstream newFile ;  // �طs�ɮ�sorted 
	string sort = "sorted" + fileName + ".txt";
	newFile.open( sort.c_str() );  // �}�ҦW��sortedXXX.txt���ɮ� 
	
	sread = clock() ;  // �}�lŪ�� 
	getline( file, str ) ;  // Ū���D 
	cout << "	" << str << "\n"; 
	newFile << str << endl ;
	
	while ( getline( file, str ) ) {  // �N�u�@�@���@��Ū�J 
		int i = 0 ;
		while ( i < str.length() ) {
			num = "" ;  //�M�� 1
			while ( str[i] != '\t' && str[i] != '\n' ) {  // �J��tab�N���� ���|���s�ivector 
				num = num + str[i] ;
				i++ ;
			}
		
			// = -1 ����٨S�s�L 
			if ( data.oid == -1 )
				data.oid = stoi( num.c_str() ) ;
				
			else if ( data.arrival == -1 )
				data.arrival = stoi( num.c_str() ) ;
							
			else if ( data.duration == -1 )		
				data.duration = stoi( num.c_str() ) ;
					
			else if ( data.timeout == -1 ) 
				data.timeout = stoi( num.c_str() ) ;
			i++ ;
		}
		
		process.push_back( data ) ;
		data = {} ;  //��l�� 
		str = "" ;
	}
	
	end = clock() ;  // Ū�ɵ��� 
	sread = end - sread ;  // �p��Ū�ɰ���ɶ�  
	for ( int i = 0 ; i < process.size() ; i++ ) {  // ��Xinput�ɤ��e 
		cout << "(" << i+1 << ")	" << process[i].oid << "\t" ;
		cout << process[i].arrival << "\t";
		cout << process[i].duration << "\t" ;
		cout << process[i].timeout << endl ;
	}  
	
	ssort = clock() ;  // �}�l�Ƨ� 
	ShellSort( process ) ;  // �ƺ��Ƨ� 
	end = clock() ;  // �Ƨǵ��� 
	ssort = end - ssort ;  // �p��Ƨǰ���ɶ�  

	swrite = clock() ;  // �}�l�g�� 
	int i = 0 ;
	while ( i < process.size() ) {  //�g�� 
		newFile << process[i].oid << '\t' ;
		newFile << process[i].arrival << '\t' ;
		newFile << process[i].duration << '\t' ;
		newFile << process[i].timeout << endl ;
		i++ ;
	}
	end = clock() ;   // �g�ɵ��� 
	swrite = end - swrite ;  // �p��g�ɰ���ɶ� 
	
	// ��X����ɶ� 
	cout << "Reading data: " << static_cast<int>(sread) << " clocks" << " (" << fixed << setprecision(2) << static_cast<double>(sread) << " ms)." << endl ;
	cout << "Sorting data: " << static_cast<int>(ssort) << " clocks" << " (" << fixed << setprecision(2) << static_cast<double>(ssort) << " ms)." << endl ;
	cout << "Writing data: " << static_cast<int>(swrite) << " clocks" << " (" << fixed << setprecision(2) << static_cast<double>(swrite) << " ms)." << endl ;
	cout << endl << "See " << sort << endl ;
	
	newFile.close() ;
	file.close() ;
} 


void Output () {
	vector<Type> process ;
	string fileName ;
	cout << endl << "Input a file number: " ;
	cin >> fileName ;
	
	vector <Type> alldata ;  // �s�ƧǹL���Ҧ���� 
	vector <TypeA> abortlist ;  // �s��������� 
	vector <TypeD> donelist ;  // �s���\����� 
	Type all ;
	TypeA abort ;
	TypeD done ;
	
	int nowtime = 0, totaldelay = 0 ; 
	Queue queue ;
	
	string str ;
	string num = "" ; 
	string sort = "sorted" + fileName + ".txt" ;
	fstream file ;
	file.open( sort.c_str() ) ;  // �}�ҦW��sortedXXX.txt���ɮ� 
	
	if ( file.is_open() ) {  // �p�GsortedXXX.txt�s�b 
		getline( file, str ) ;  // ��Ū�J���D
		
		while ( getline( file, str ) ) {  // �N�u�@�@���@��Ū�J 
			int i = 0 ;
			
			while ( i < str.length() ) {
				num = "" ;  //�M�� 1
				while ( str[i] != '\t' && str[i] != '\n' ) { // �J�� tab�N���� ���|���s�i vector 
					num = num + str[i] ;
					i++ ;
				}
			
				// = -1 ����٨S�s�L
				// �q�ɮפ�Ū����ƨ� all��������
				if ( all.oid == -1 )   
					all.oid = stoi( num.c_str() ) ;
					
				else if ( all.arrival == -1 )
					all.arrival = stoi( num.c_str() ) ;
								
				else if ( all.duration == -1 )		
					all.duration = stoi( num.c_str() ) ;
						
				else if ( all.timeout == -1 ) 
					all.timeout = stoi( num.c_str() ) ;
					
				i++ ;
			}
			
			alldata.push_back( all ) ;
			all = {} ; //��l�� 
			str = "" ;
			 
		} // while // �N��r�ɤ@���Ū�J 
		
		
		nowtime = alldata[0].arrival + alldata[0].duration;  // �{�b�ɶ��O��F�ɶ��[�W�Ӯ�
		done.oid = alldata[0].oid ;
		done.Departure = nowtime ;
		done.delay = 0 ;
		donelist.push_back( done ) ;
		
		int a = 1; // �q�Ĥ@��(�ĤG�Ӹ��)�}�l �Ĥ@�Ӹ�ƥi�H�����u�@
		while ( a < alldata.size() ) {
			if ( nowtime > alldata[a].arrival && a < alldata.size() ) { // �p�G�Ӥu�@�w��F�N�i�H�Q�B�z 
				
				if ( !queue.isFull() && nowtime > alldata[a].arrival )  // queue�S���s�� 
					queue.enQueue( alldata[a] ); // ���a�����|�ظ�Ƴ��s�i�h 
			
				else {  // queue���F�N���� 
					abort.oid = alldata[a].oid;
					abort.delay = 0;
					abort.abort = alldata[a].arrival;  // �����ɨ� = �i�J�ɶ� 
					abortlist.push_back(abort);
				} // else
				
				a++;
			} // if
			
			else {
				
				if ( !queue.isEmpty() ) {  // queue�̭����ݰ��檺�u�@ 
				
					if ( nowtime > queue.getFront().timeout ) {  // �q CPU���X�ɴN�O�ɤF  (���� 4-2)
						// �N�Ӥu�@�����A�p�⩵��ɶ��A�ðO���b�����M�椤
						abort.oid = queue.getFront().oid;
						abort.abort = nowtime;
						abort.delay = nowtime - queue.getFront().arrival;  // ����ɶ� = �����ɶ� - �s�u�@��F�ɶ� 
						abortlist.push_back(abort);
						nowtime = abort.abort;  // �{�b�ɶ��O�����ɶ� 
						totaldelay = totaldelay + abort.delay;
					} // if
					
					else if ( queue.getFront().timeout < nowtime + queue.getFront().duration ) {  // ����L�{���O�� (����4-3)
						// �N�Ӥu�@�����A�p�⩵��ɶ��A�ðO���b�����M�椤 
						abort.oid = queue.getFront().oid; 
						abort.abort = queue.getFront().timeout ;
						abort.delay = abort.abort - queue.getFront().arrival;  // ����ɶ� = �����ɶ� - �Ӥu�@����F�ɶ� 
						abortlist.push_back(abort);
						nowtime = abort.abort ;  // �{�b�ɶ��O�����ɶ� 
						totaldelay = totaldelay + abort.delay;		
					} // else if
							
					else {  // ���\���槹�� 
						done.oid = queue.getFront().oid;
						done.delay = nowtime - queue.getFront().arrival;  // �}�l�ɨ� - �i�J�ɨ� 
						nowtime = nowtime + queue.getFront().duration ; 
						done.Departure = nowtime ;
						donelist.push_back(done);
						totaldelay = totaldelay + done.delay;
					} 
					
					queue.deQueue() ;
				} // if
				
				else {  // queue�̭��S���ݰ��檺�u�@ --> �i��������  
					done.oid = alldata[a].oid;
					done.delay = 0;  // �}�l�ɨ� - �i�J�ɨ� 
					nowtime = alldata[a].arrival + alldata[a].duration ; 
					done.Departure = nowtime ;
					donelist.push_back(done);
					a++ ;
					totaldelay = totaldelay + done.delay;					
				} // else			
			}	// else			
		}  // while
		
		
		while( !queue.isEmpty() ) {   
			if ( nowtime > queue.getFront().timeout ) { // �q CPU���X�ɴN�O�ɤF  (���� 4-2)
				// �N�Ӥu�@�����A�p�⩵��ɶ��A�ðO���b�����M�椤
				abort.oid = queue.getFront().oid;
				abort.abort = nowtime;
				abort.delay = nowtime - queue.getFront().arrival;  // ����ɶ� = �����ɶ� - �s�u�@��F�ɶ� 
				abortlist.push_back(abort);
				nowtime = abort.abort;  // �{�b�ɶ��O�����ɶ� 
				totaldelay = totaldelay + abort.delay;
			} // if
			else if ( queue.getFront().timeout < nowtime + queue.getFront().duration ) {  // ����L�{���O�� (����4-3)
				// �N�Ӥu�@�����A�p�⩵��ɶ��A�ðO���b�����M�椤 
				abort.oid = queue.getFront().oid; 
				abort.abort = queue.getFront().timeout ;
				abort.delay = abort.abort - queue.getFront().arrival;  // ����ɶ� = �����ɶ� - �Ӥu�@����F�ɶ� 
				abortlist.push_back(abort);
				nowtime = abort.abort ;  // �{�b�ɶ��O�����ɶ� 
				totaldelay = totaldelay + abort.delay;						
			} // else if
								
			else {  // ���\���槹��  
				done.oid = queue.getFront().oid;
				done.delay = nowtime - queue.getFront().arrival;  // �}�l�ɨ� - �i�J�ɨ� 
				nowtime = nowtime + queue.getFront().duration ; 
				done.Departure = nowtime ;
				donelist.push_back(done);
				totaldelay = totaldelay + done.delay;
			} 
			
			queue.deQueue() ;
		}
		
		ofstream newFile ;  // �طs�ɮ�output
		string output = "output" + fileName + ".txt" ;
		newFile.open( output.c_str() ); // �}�ҦW��outputXXX.txt���ɮ�
		
		// �p�⥭������M���\��� 
		double averagedelay = static_cast<double>( totaldelay ) / static_cast<double>( alldata.size() ) ;
		double donerate = ( static_cast<double>( donelist.size() ) / alldata.size() ) * 100; 
	
		// �g�Joutput�� 
		newFile << "\t[Abort Jobs]" << endl ;
		newFile << "\tOID" << '\t' << "Abort" << '\t' << "Delay" << endl ;
		for ( int i = 0 ; i < abortlist.size() ; i++ ) { // ������X�s�bvector��Ū�J����r 
			newFile << "[" << i+1 << "]\t";   
			newFile << abortlist[i].oid << "\t" ;
			newFile << abortlist[i].abort << "\t";
			newFile << abortlist[i].delay << endl ;
		}  // for
		
		newFile << "\t[Jobs Done]" << endl;
		newFile << "\tOID" << '\t' << "Departure" << '\t' << "Delay" << endl;
		for ( int i = 0 ; i < donelist.size() ; i++ ) { // ������X�s�bvector��Ū�J����r 
			newFile << "[" << i+1 << "]\t"; 
			newFile << donelist[i].oid << "\t" ;
			newFile << donelist[i].Departure << "\t";
			newFile << donelist[i].delay << endl ;
		}  
		
		newFile << "[Average Delay]\t" << fixed << setprecision(2) << averagedelay << " ms" << endl;
		newFile << "[Success Rate]\t" << donerate << " %" << endl;
		
		cout << endl << "The simulation is running..." << endl ;
		cout << "See " << output << endl ;
		
	}	// if
	
	else  // �䤣���ɮ� sortedXXX.txt 
		cout << endl << "### " << sort << " does not exist! ###" << endl ;
	
} // Two


int main() {
	string command ;
	//vector<Type> process ;
	
	while ( command != "0" ) { 
		cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
		cout << endl << "* 0. Quit                            *" ;
		cout << endl << "* 1. Sort a file                     *" ;
		cout << endl << "* 2. Simulate one FIFO queue         *" ;
		cout << endl << "**************************************" ;
		cout << endl << "Input a command(0, 1, 2):" ;
		cin >> command ;
		
		if ( command == "0" )
			break ;
			
		else if ( command == "1" ) 
			Sort() ;
	
		else if ( command == "2" )
			Output();
		
		else 
			cout << "\nthis command does not exist";
		
	} 
}

