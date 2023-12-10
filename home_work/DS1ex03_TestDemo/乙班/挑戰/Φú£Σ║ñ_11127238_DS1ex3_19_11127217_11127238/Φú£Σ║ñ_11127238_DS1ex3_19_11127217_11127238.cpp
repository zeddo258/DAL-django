//11127217 ���ۦ� 11127238 ����� 
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <time.h>
using namespace std;

typedef struct list{
	int OID;		// order identifier
	int arrival;	// arrival time
	int duration;	// job duration
	int timeout;	// expire time
	int head;		//�����}�l�B�z�ɶ� 
};

typedef struct job{
	int OID;
	int CID;
	int abort;
	int dep;
	int delay;
};

class Queue{
	private:
		vector<list> line1;
		vector<list> line2;
	public:
	
	int length() {			//�p��queue���� 
		return line1.size();
	} 
	
	int length2() {			//�p��queue���� 
		return line2.size();
	} 
	
	bool isEmpty() {		//queue�O�Ū� 
		if ( line1.size() == 0 ) {
			return true;
		} 
		else{
			return false; 
		}
	}  	
	
	bool isEmpty2() {		//queue�O�Ū� 
		if ( line2.size() == 0 ) {
			return true;
		} 
		else{
			return false; 
		}
	} 
	
	bool isFull() {  		// queue���F 
		if ( line1.size() == 4 )  {
			return true;
		}
		else{
			return false; 
		}
	} 
	
	bool isFull2() {  		// queue���F 
		if ( line2.size() == 4 )  {
			return true;
		}
		else{
			return false; 
		}
	} 
	
	void push( list u) {	//�bqueue�s�W 
		line1.push_back(u);
	}  
	
	void pop() {			//�R���}�Y 
		line1.erase( line1.begin() );
	} 
	
	void run(){				//�C�g�L�@���B�z������ƪ�duration��@ 
		line1[0].duration--;
	}
	
	void start(int m){		//�]�m�}�l�B�z�ɶ� 
		line1[0].head = m;
	}
	
	list getFront() {		//���o�}�Y 
		return line1[0] ;
	} 
	
	void push2( list u) {	//�bqueue�s�W 
		line2.push_back(u);
	}  
	
	void pop2() {			//�R���}�Y 
		line2.erase( line2.begin() );
	} 
	
	void run2(){				//�C�g�L�@���B�z������ƪ�duration��@ 
		line2[0].duration--;
	}
	
	void start2(int m){		//�]�m�}�l�B�z�ɶ� 
		line2[0].head = m;
	}
	
	list getFront2() {		//���o�}�Y 
		return line2[0] ;
	} 
	
};

void task1(){
	ifstream in;
	ofstream out;
	vector<list> txt;
	int oid, duration, timeout, arrival=-1;
	double reading, rend;
	double sorting, send;
	double writing, wend;
	string filename, read, input, output;
	cout << endl <<  "Input a file number: " ;
	cin >> filename;
	input = "input" + filename + ".txt";	//��_�ɮצW��  
	output = "sorted" + filename + ".txt";	//��X�ɦW�� 
	in.open(input.c_str());					//�}���ɮ� 
	if( in.fail() ){						//�ɮפ��s�b  
		cout << endl << "### " << filename << " does not exist! ###" << endl ;
	}
	else {									//�ɮצs�b 
		reading = clock();					//Ū�ɶ}�l�ɶ� 
		for(int i=0 ; i<4 ; i++){			//���rŪ�� 
				in >> read;
		}
		in >> oid >> arrival >> duration >> timeout; 
		while ( arrival != -1 ){				//Ū & �s�������  
			list line;
			line.OID = oid;
			line.duration = duration;
			line.arrival = arrival;
			line.timeout = timeout;
			txt.push_back(line);
			arrival = -1;
			in >> oid >> arrival >> duration >> timeout;
		}
		rend = clock() ;				//Ū�ɵ����ɶ� 
		cout << endl << "\t" << "OID	Arrival	Duration	TimeOut" << endl;
		for(int i=0;i<txt.size();i++){	 
			cout << "(" << i+1 << ")\t" << txt[i].OID << "\t" << txt[i].arrival << "\t" << txt[i].duration << "\t" << txt[i].timeout << endl; 
		}
		
		sorting = clock();				//�ƧǶ}�l�ɶ� 
		list store;
		for(int len= txt.size()/2;len>0;len=len/2){				//shell sort 
			for ( int d = 0 ; d < len ; d++ ){					//�C�ձƧ� 
				for(int i=0; i<txt.size();i=i+len){				//�դ��w�w�Ƨ� 
					for ( int j = i ; j<txt.size();j=j+len){
						if ( txt[j].arrival < txt[i].arrival){			//arrival�� > arrival�k 
							store = txt[i];
							txt[i] = txt[j];
							txt[j] = store;
						}
						else if( txt[j].arrival == txt[i].arrival){		//arrival�� == arrival�k
							if(txt[j].OID < txt[i].OID){				//���OID�� > OID�k
								store = txt[i];
								txt[i] = txt[j];
								txt[j] = store;
							}
						}
							
					}
				}
			}	
		}
		
		send = clock();					//�Ƨǵ����ɶ� 
		writing = clock();				//�g�ɶ}�l�ɶ� 
		
		//�g�� 
		out.open( output.c_str());
		out << "OID	Arrival	Duration	TimeOut" << endl;
		for(int i=0;i<txt.size();i++){	
			out << txt[i].OID << "\t" << txt[i].arrival << "\t" << txt[i].duration << "\t" << txt[i].timeout << endl; 
		}
		wend = clock();					//�g�ɵ����ɶ�
		 
		//��X
		int a = rend-reading;
		int b = send-sorting;
		int c = wend-writing;
		cout << endl;
		cout << "Reading data: " << a << " clocks (" << fixed << setprecision(2) << rend-reading <<  " ms).\n";
		cout << "Sorting data: " << b << " clocks (" << fixed << setprecision(2) << send-sorting <<  " ms).\n";
		cout << "Writing data: " << c << " clocks (" << fixed << setprecision(2) << wend-writing <<  " ms).\n";
		cout << endl << "See sorted" << filename << ".txt" << endl; 
		//�M�šB�����ɮ� 
		txt.clear();
		out.close();
		in.close();
	}	
}

void task2(){
	ifstream in;
	ofstream out;
	vector<list> txt;
	vector<job> abort, done; 
	int oid, arrival, duration, timeout;
	int locate = 0, time = 0, end = 0;
	double average;
	string filename, read, input, output;
	Queue call;
	cout << endl <<  "Input a file number: " ;
	cin >> filename;
	input = "sorted" + filename + ".txt";	//��_�ɮצW��  
	output = "output" + filename + ".txt";	//��X�ɦW�� 
	in.open(input.c_str());					//�}���ɮ� 
	if( in.fail() ){						//�ɮפ��s�b  
		cout << endl << "### " << filename << " does not exist! ###" << endl ;
	}
	else {									//�ɮצs�b 
		for(int i=0 ; i<4 ; i++){			//���rŪ�� 
				in >> read;
		}
		in >> oid >> arrival >> duration >> timeout; 
		while ( !in.eof() ){				//Ū & �s�������  
			if( timeout > end){		//end�����Ҧ���Ƥ��̱ߪ�timeout�ɶ� 
				end = timeout;
			}
			list line;
			line.OID = oid;
			line.duration = duration;
			line.arrival = arrival;
			line.timeout = timeout;
			line.head = 0;
			txt.push_back(line);
			in >> oid >> arrival >> duration >> timeout;
		}
	}
	
	for(time=txt[0].arrival ; time<=end ; time++){
		if(! call.isEmpty()){	//���B�z�����u�@���ܡA�C���n�N����duration��@ 
			call.run();
		}
		while( ! call.isFull() &&time==txt[0].arrival ){	//��queue�٦��Ŷ��B��Farrival���ɶ��A���ƥ[��queue�� 
			call.push(txt[0]);
			txt.erase( txt.begin() );
		}
		while( !call.isEmpty() && (time>=call.getFront().timeout||call.getFront().duration==0)){		//�ثe���b�B�z���u�@�B�w�B�z�����άO��timeout 
			if(call.getFront().duration==0&& call.getFront().timeout>=time){ 			//�w���� �ݥ[�Jdone�C�� 
				job store;
				oid = call.getFront().head-call.getFront().arrival; //�p��delay 
				if(oid < 0){	//�B�z�ɶ��S���Ի~�� 
					store.delay = 0;
				}
				else{
					store.delay = oid;
				}
				store.dep=time;
				store.OID=call.getFront().OID;			
				done.push_back(store);
				call.pop();
				call.start(time);
				if( txt.size()!=0&&time==txt[0].arrival){
					call.push(txt[0]);
					txt.erase( txt.begin() );
				}
			}
			else{										//������ �ݥ[�Jabort�C�� 
				job store;
				store.delay=time-call.getFront().arrival;
				store.abort=time;
				store.OID=call.getFront().OID;
				abort.push_back(store);
				call.pop();
				call.start(time);
				if( txt.size()!=0&&time==txt[0].arrival){
					call.push(txt[0]);
					txt.erase( txt.begin() );
				}
			}
		}

		while(time==txt[0].arrival&&txt.size()!=0){  	//��arrival���ɶ���F�S�i�Jqueue���N�[�Jabort 
			job store;
			store.delay=0;
			store.abort=time;
			store.OID=txt[0].OID;
			abort.push_back(store);
			txt.erase( txt.begin() );
		}
	}
	
	//�p��average delay 
	for(int i = 0; i<abort.size();i++){
		average = average + abort[i].delay;
	}
	for(int i = 0; i<done.size();i++){
		average = average + done[i].delay;
	}
	double total = abort.size()+done.size();
	
	//�g��  
	out.open( output.c_str());
	out << "\t[Abort Jobs]" << endl;
	out << "\t" << "OID	 Abort	Delay" << endl;
	for(int i = 0; i<abort.size();i++){
		out <<"[" << i+1 << "]\t"<< abort[i].OID << "\t" << abort[i].abort << "\t" << abort[i].delay << endl;
	}
	out << "\t" << "[Jobs Done]" << endl;
	out << "\t" << "OID	 Departure	Delay" << endl;
	for(int i = 0; i<done.size();i++){
		out <<"[" << i+1 << "]\t"<< done[i].OID << "\t" << done[i].dep << "\t" << done[i].delay << endl;
	}
	out << "[Average Delay] "<< fixed << setprecision(2) << average / total << " ms"<<endl;
	out << "[Success Rate] "<< fixed << setprecision(2) << (done.size() / total)*100<< " %"<<endl;
		
	//��X 
	cout << endl << "The simulation is running..." << endl;
	cout << "See output" << filename << ".txt" << endl;
	txt.clear();
	out.close();
	in.close();
}

void task3(){
	ifstream in;
	ofstream out;
	vector<list> txt;
	vector<job> abort, done; 
	int oid, arrival, duration, timeout;
	int locate = 0, time = 0, end = 0;
	double average;
	string filename, read, input, output;
	Queue call;
	cout << endl <<  "Input a file number: " ;
	cin >> filename;
	input = "sorted" + filename + ".txt";	//��_�ɮצW��  
	output = "double" + filename + ".txt";	//��X�ɦW�� 
	in.open(input.c_str());					//�}���ɮ� 
	if( in.fail() ){						//�ɮפ��s�b  
		cout << endl << "### " << filename << " does not exist! ###" << endl ;
	}
	else {									//�ɮצs�b 
		for(int i=0 ; i<4 ; i++){			//���rŪ�� 
				in >> read;
		}
		in >> oid >> arrival >> duration >> timeout; 
		while ( !in.eof() ){				//Ū & �s�������  
			if( timeout > end){		//end�����Ҧ���Ƥ��̱ߪ�timeout�ɶ� 
				end = timeout;
			}
			list line;
			line.OID = oid;
			line.duration = duration;
			line.arrival = arrival;
			line.timeout = timeout;
			line.head = 0;
			txt.push_back(line);
			in >> oid >> arrival >> duration >> timeout;
		}
	}
	
	for(time=txt[0].arrival ; time<=end ; time++){
		if(! call.isEmpty()){	//���B�z�����u�@���ܡA�C���n�N����duration��@ 
			call.run();
		}
		if(! call.isEmpty2()){	//���B�z�����u�@���ܡA�C���n�N����duration��@ 
			call.run2();
		}
		while( (!call.isFull() || !call.isFull2())&& time==txt[0].arrival && txt.size()!=0){	//��queue�٦��Ŷ��B��Farrival���ɶ��A���ƥ[��queue�� 
			if( (call.length() <= call.length2() ) && !call.isFull()){
				call.push(txt[0]);
				txt.erase( txt.begin() );
				if(call.length()==1){
					job store;
					store.CID = 1;
					oid = call.getFront().timeout-call.getFront().arrival; //�p��delay 
					store.OID = call.getFront().OID;
					if((call.getFront().arrival+call.getFront().duration) <= call.getFront().timeout){
						store.delay=0;
						store.dep=call.getFront().arrival+call.getFront().duration;
						done.push_back(store);
					}
					else{
						oid = call.getFront().timeout-call.getFront().arrival; //�p��delay 
						if(oid < 0){	//�B�z�ɶ��S���Ի~�� 
							store.delay = 0;
						}
						else{
							store.delay = oid;
						}
						store.abort=call.getFront().timeout;
						abort.push_back(store);
					}	
				} 		
			}
			else {
				call.push2(txt[0]);
				txt.erase( txt.begin() );
				if(call.length2()==1){
					job store;
					store.CID = 2;
					oid = call.getFront2().timeout-call.getFront2().arrival; //�p��delay 
					store.OID = call.getFront2().OID;
					if((call.getFront2().arrival+call.getFront2().duration) <= call.getFront2().timeout){
						store.delay=0;
						store.dep=call.getFront2().arrival+call.getFront2().duration;
						done.push_back(store);
					}
					else{
						if(oid < 0){	//�B�z�ɶ��S���Ի~�� 
							store.delay = 0;
						}
						else{
							store.delay = oid;
						}
						store.abort=call.getFront2().timeout;
						abort.push_back(store);
					}	
				}
				
			}
		}
		while( !call.isEmpty() && (time>=call.getFront().timeout||call.getFront().duration==0)){		//�ثe���b�B�z���u�@�B�w�B�z�����άO��timeout 
			call.pop();
			call.start(time);
			if(!call.isEmpty()){
				job store;
				store.CID = 1;
				oid = call.getFront().timeout-call.getFront().arrival; //�p��delay 
				if(oid < 0){	//�B�z�ɶ��S���Ի~�� 
					store.delay = 0;
				}
				else{
					store.delay = oid;
				}
				store.OID = call.getFront().OID;
				if((call.getFront().head+call.getFront().duration) <= call.getFront().timeout){
					store.dep=call.getFront().arrival+call.getFront().duration;
					done.push_back(store);
				}
				else{
					store.abort=call.getFront().timeout;
					abort.push_back(store);
				}
			}
			if( txt.size()!=0&&time==txt[0].arrival){
				call.push(txt[0]);
				txt.erase( txt.begin() );
			}
		
		}
		while( !call.isEmpty2() && (time>=call.getFront2().timeout||call.getFront2().duration==0)){		//�ثe���b�B�z���u�@�B�w�B�z�����άO��timeout 
			call.pop2();
			call.start2(time);
			if(!call.isEmpty2()){
				job store;
				store.CID = 2;
				oid = call.getFront2().timeout-call.getFront2().arrival; //�p��delay 
				if(oid < 0){	//�B�z�ɶ��S���Ի~�� 
					store.delay = 0;
				}
				else{
					store.delay = oid;
				}
				store.OID = call.getFront2().OID;
				if((call.getFront2().head+call.getFront2().duration) <= call.getFront2().timeout){
					store.dep=call.getFront2().arrival+call.getFront2().duration;
					done.push_back(store);
				}
				else{
					store.abort=call.getFront2().timeout;
					abort.push_back(store);
				}
			}
			if( txt.size()!=0&&time==txt[0].arrival){
				call.push2(txt[0]);
				txt.erase( txt.begin() );
			}
		}
		while(time==txt[0].arrival&&txt.size()!=0){  	//��arrival���ɶ���F�S�i�Jqueue���N�[�Jabort 
			job store;
			store.delay=0;
			store.abort=time;
			store.OID=txt[0].OID;
			store.CID=0;
			abort.push_back(store);
			txt.erase( txt.begin() );
		}
	}
	
	//�p��average delay 
	for(int i = 0; i<abort.size();i++){
		average = average + abort[i].delay;
	}
	for(int i = 0; i<done.size();i++){
		average = average + done[i].delay;
	}
	double total = abort.size()+done.size();
	
	//�g��  
	out.open( output.c_str());
	out << "\t[Abort Jobs]" << endl;
	out << "\t" << "OID	CID	Abort	Delay" << endl;
	for(int i = 0; i<abort.size();i++){
		out <<"[" << i+1 << "]\t"<< abort[i].OID << "\t" << abort[i].CID << "\t" << abort[i].abort << "\t" << abort[i].delay << endl;
	}
	out << "\t" << "[Jobs Done]" << endl;
	out << "\t" << "OID	CID	Departure	Delay" << endl;
	for(int i = 0; i<done.size();i++){
		out <<"[" << i+1 << "]\t"<< done[i].OID << "\t" << done[i].CID << "\t" << done[i].dep << "\t" << done[i].delay << endl;
	}
	out << "[Average Delay] "<< fixed << setprecision(2) << average / total << " ms"<<endl;
	out << "[Success Rate] "<< fixed << setprecision(2) << (done.size() / total)*100<< " %"<<endl;
		
	//��X 
	cout << endl << "The simulation is running..." << endl;
	cout << "See double" << filename << ".txt" << endl;
	txt.clear();
	out.close();
	in.close();
}

int main(int argc, char** argv) {
	int run = 0;
    while ( run == 0 ) {
		string command;
		cout << endl;
    	cout << "**** Simulate FIFO Queues by SQF *****" << endl;
		cout << "* 0. Quit                            *" << endl;
    	cout << "* 1. Sort a file                     *" << endl;
		cout << "* 2. Simulate one FIFO queue         *" << endl;
		cout << "* 3. Simulate two queues by SQF      *" << endl;
		cout << "**************************************" << endl;
		cout << "Input a command(0, 1, 2, 3): " ;
		cin >> command;	  
    	if ( command == "0" )  {
    		return 0;
		}
    	else if ( command == "1" ) {
    		task1();
	    }    
		else if ( command == "2" ) {
			task2();
		}
		else if ( command == "3" ) {
			task3();
		}
		else{
			cout << endl << "Command does not exist!" << endl;
		}
	}
}
