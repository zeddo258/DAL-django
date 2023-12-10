// 11127106 ���l��  11127118 ������ 
#include<stdio.h>
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
#include <iomanip>
using namespace std;
struct jobType{  // �ŧi�u�@��ƪ�struct
	int OID;
	int arrival;
	int duration;
	int timeout;	
};
struct abortType{ // �����M�檺struct
	int OID;
	int abort;
	int delay;	
};
struct done{ // �����M�檺strcut
	int OID;
	int departure;
	int delay;	
};
class Queue {
private:
	vector<jobType> queue;
public:
	bool isQueueFull () { // �ˬdqueue�O�_�����A��vector�S���j�p�W���A�]��return false
		return false;
	} // isQueueFull()

	bool isEmpty () { // �ˬdqueue�O�_����
		return queue.empty();
	} // isEmpty

	void enqueue (jobType data) { // �q��C���ݥ[�J����
		queue.push_back (data);
	} // enqueue() 

	jobType dequeue () { // �q��C�e�ݨ��X����
		if (!queue.empty ()) {
			jobType front_data = queue.front();
			queue.erase (queue.begin ());
			return front_data;
		} // if
		else {
			cout <<"The queue is empty."<< endl;
			jobType ret = {0};
			return ret;
		} // else
	} // dequeue()
}; // Queue
class runtasktwo{
		vector<done> dojob; //�����M�檺vector 
		vector<jobType> aListt;
		vector<abortType> cannotdo; // �����M�檺vector 
		vector<jobType> decide;
	public:
		void dotasktwo( vector<jobType> aList , string fileName);		
};
class JobList {
		vector<jobType> aList;
	public:
		bool getAll(string a, int command  );
		bool getSorted( string fileName, double read, int command );
		bool tasktwo( string fileName);	
};
void runtasktwo::dotasktwo( vector<jobType> aList, string fileName) {
	Queue aQueue;
	abortType abt;
	jobType type;
	done D;
	string files = "sorted" + fileName + ".txt";
	ifstream ifs( files.c_str() );
	if ( ifs ) aListt = aList;
	else return;
	ifs.close();
	int cpu = aListt.at(0).arrival ; // cpu �}�l�ɶ����Ĥ@����ƪ�arrival 
	int i = 0, time = 0;
	int dojobs = 0, abort = 0, nor = 0;
	for ( int i =0; i < aListt.size() ;i++){
		if ( aListt.at(i).duration + cpu <= aListt.at(i).timeout && aListt.at(i).arrival >=cpu   && aQueue.isEmpty() ) {
			dojob.push_back( D );
			dojob.at(dojobs).OID = aListt.at(i).OID;
			dojob.at(dojobs).departure = cpu + aListt.at(i).duration;
			dojob.at(dojobs).delay = cpu - aListt.at(i).arrival;
			cpu = cpu + aListt.at(i).duration;
			dojobs++;
		}
		else if (  aListt.at(i).arrival > cpu ){ //���m
			decide.push_back(type);
			decide.at(0) = aQueue.dequeue();
			if ( cpu < decide.at(0).arrival ) cpu = decide.at(0).arrival;
			if ( cpu + decide.at(0).duration > decide.at(0).timeout){ // �Ycpu + duration > timeout �h�������M�椤 
				if ( cpu > decide.at(0).timeout ){ //�Y cpu > timeout 
					cannotdo.push_back( abt );
					cannotdo.at(nor).OID = decide.at(0).OID;
					cannotdo.at(nor).abort = cpu;
					cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
					nor++;
				}
				else { // �Y cpu <= timeout   
					cannotdo.push_back( abt );
					cannotdo.at(nor).OID = decide.at(0).OID;
					cannotdo.at(nor).abort = decide.at(0).timeout;
					cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
					nor++;
					cpu = decide.at(0).timeout; // ���� cpu�ɶ���timeout 
				}
			}
			else if ( cpu + decide.at(0).duration <=  decide.at(0).timeout ){ // cpu + duration <= timeout �h��짹���M�椤 
				dojob.push_back( D );
				dojob.at(dojobs).OID = decide.at(0).OID;
				dojob.at(dojobs).departure = cpu + decide.at(0).duration;
				dojob.at(dojobs).delay = cpu -  decide.at(0).arrival;
				cpu = cpu + decide.at(0).duration;
				dojobs++;
			}
			time--;
			aQueue.enqueue( aListt.at(i) );
			time++;
		}
		else if ( aListt.at(i).arrival == cpu ) { // �p�G�o����Ʒ|��arrival����cpu, �h���M�Ŧ�C������� 
			if ( ! aQueue.isEmpty() ){ //�N���C������ 
				for( int j = 0; ! aQueue.isEmpty() ; j++ ){
					decide.push_back(type);
					decide.at(0) = aQueue.dequeue(); // ���C���Ĥ@�ӥ�X�� 
					if ( cpu + decide.at(0).duration > decide.at(0).timeout){ // �Ycpu + duration > timeout �h�������M�椤  
						if ( cpu > decide.at(0).timeout ){ //�Y cpu > timeout  
							cannotdo.push_back( abt );
							cannotdo.at(nor).OID = decide.at(0).OID;
							cannotdo.at(nor).abort = cpu;
							cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
							nor++;
						}
						else { // �Y cpu <= timeout   
							cannotdo.push_back( abt );
							cannotdo.at(nor).OID = decide.at(0).OID;
							cannotdo.at(nor).abort = decide.at(0).timeout;
							cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
							nor++;
							cpu = decide.at(0).timeout; // ���� cpu�ɶ���timeout  
						}
					}
					else if ( cpu + decide.at(0).duration <=  decide.at(0).timeout ){ // cpu + duration <= timeout �h��짹���M�椤  
						dojob.push_back( D );
						dojob.at(dojobs).OID = decide.at(0).OID;
						dojob.at(dojobs).departure = cpu + decide.at(0).duration;
						dojob.at(dojobs).delay = cpu -  decide.at(0).arrival;
						cpu = cpu + decide.at(0).duration;
						dojobs++;
					}
					time--;
				}
			}
			
			i--;
		}
		
		else {
			if ( time < 3 ) { //�Y��C���p��T�ӤH 
				aQueue.enqueue( aListt.at(i) ); // �s�J�}�C�� 
				time++;
			}
			else {  // �Y�٭n�s�J��C,�h�����s������M�椤 
				cannotdo.push_back( abt );
				cannotdo.at(nor).OID = aListt.at(i).OID;
				cannotdo.at(nor).abort = aListt.at(i).arrival;
				cannotdo.at(nor).delay = 0;
				nor++;
			}
		}
	}
	if ( ! aQueue.isEmpty() ){
		for( int j = 0; ! aQueue.isEmpty() ; j++ ){
			decide.push_back(type);
			decide.at(0) = aQueue.dequeue(); // ���C���Ĥ@�ӥ�X�� 
			if ( cpu + decide.at(0).duration > decide.at(0).timeout){ // �Ycpu + duration > timeout �h�������M�椤  
				if ( cpu > decide.at(0).timeout ){ //�Y cpu > timeout 
					cannotdo.push_back( abt );
					cannotdo.at(nor).OID = decide.at(0).OID;
					cannotdo.at(nor).abort = cpu;
					cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
					nor++;
				}
				else { // �Y cpu <= timeout 
					cannotdo.push_back( abt );
					cannotdo.at(nor).OID = decide.at(0).OID;
					cannotdo.at(nor).abort = decide.at(0).timeout;
					cannotdo.at(nor).delay = cannotdo.at(nor).abort - decide.at(0).arrival;
					nor++;
					cpu = decide.at(0).timeout;  // ���� cpu�ɶ���timeout   
				}
			}
			else if ( cpu + decide.at(0).duration <=  decide.at(0).timeout ){ // cpu + duration <= timeout �h��짹���M�椤 
				dojob.push_back( D );
				dojob.at(dojobs).OID = decide.at(0).OID;
				dojob.at(dojobs).departure = cpu + decide.at(0).duration;
				dojob.at(dojobs).delay = cpu -  decide.at(0).arrival;
				cpu = cpu + decide.at(0).duration;
				dojobs++;
			}
			time--;
		}			
	}
	int plusC = 0;
	int plusD = 0;
	for ( int k = 0; k < nor ; k++){ // ���������M��delay���`�M 
		plusC = plusC + cannotdo.at(k).delay;  
	}								
	for ( int k = 0; k < dojobs ; k++){  // ���������M��delay���`�M 
		plusD = plusD + dojob.at(k).delay;
	}
	
	int success = dojob.size() * 100 ;
	string file = "output"+ fileName +".txt";
	ofstream fout( file.c_str() ); //���� 
	if ( fout ){  
    	fout << "	[Abort Jobs]" << endl;
    	fout << "	OID	Abort	Delay" << endl;
    	for ( int k = 0; k < nor ; k++){
			fout << "[" << k+1 <<  "]" << "	"<< cannotdo.at(k).OID << "	" << cannotdo.at(k).abort;
			fout << "	" << cannotdo.at(k).delay << endl;		
		}								
		fout << "	[Jobs Done]" << endl;
		fout << "	OID	Departure	Delay" << endl;
		for ( int k = 0; k < dojobs ; k++){
			fout << "[" << k+1 << "]" << "	"<< dojob.at(k).OID << "	" << dojob.at(k).departure;
			fout << "	" << dojob.at(k).delay << endl;
			
		}
		fout << "[Average Delay]" << "	" << setprecision(2) << fixed << (float) (plusD+plusC)/aListt.size() << " ms" << endl;
		fout << "[Success Rate]	";
		fout << setprecision(2) <<fixed << (float)success / aListt.size() << " %"<<endl;
		cout << "The simulation is running..." << endl;
		cout << "See " << file << endl ;
    }
	fout.close();
}
bool JobList::tasktwo( string fileName ){
	runtasktwo start;
	start.dotasktwo( aList, fileName );
}
bool JobList::getSorted( string fileName, double read, int command ){
	double sortstart, sortend;
	double writestart, writeend;
	int step = aList.size()/2; // �q�����}�l 
	int test = 0;
	int tempO;
	int tempA;
	int tempD;
	int tempT;
	sortstart = clock();
	while ( step >= 1 ){ // sell sort 
		for( int i = step ; i < aList.size() ; i++ ){
			for ( int j = i - step; j >= 0; j-= step ){
				if( aList.at(j).arrival > aList.at(j+step).arrival) { // �p�G�e���j��᭱�A�h����  
                	tempO= aList.at(j+step).OID;
            	   	tempA= aList.at(j+step).arrival;
            	    tempD= aList.at(j+step).duration;
            	    tempT= aList.at(j+step).timeout;
                	aList.at(j+step).OID = aList.at(j).OID;
           	     	aList.at(j+step).arrival = aList.at(j).arrival;
                	aList.at(j+step).duration = aList.at(j).duration;
                	aList.at(j+step).timeout =  aList.at(j).timeout;
                	aList.at(j).OID = tempO;
                	aList.at(j).arrival = tempA;
                	aList.at(j).duration = tempD;
                	aList.at(j).timeout = tempT;
            	}
            	else if (  aList.at(j).arrival == aList.at(j+step).arrival ){ // �p�Garrival�۵� 
            		if ( aList.at(j).OID > aList.at(j+step).OID  ){ // �p�G�e����OID�j��᭱��OID�A�h���� 
               			tempO= aList.at(j+step).OID;
                		tempA= aList.at(j+step).arrival;
                		tempD= aList.at(j+step).duration;
            	    	tempT= aList.at(j+step).timeout;
            	    	aList.at(j+step).OID = aList.at(j).OID;
                		aList.at(j+step).arrival = aList.at(j).arrival;
           	    		aList.at(j+step).duration = aList.at(j).duration;
                		aList.at(j+step).timeout =  aList.at(j).timeout;
                		aList.at(j).OID = tempO;
                		aList.at(j).arrival = tempA;
                		aList.at(j).duration = tempD;
                		aList.at(j).timeout = tempT;
					}
				}
			}
		}
		step = step / 2;
	}
	sortend = clock();
	if ( command == 1 ){
		string file = "sorted"+ fileName +".txt";  // �ɮצW�� 
		writestart = clock();  // �}�l�g�J�ɮת��ɶ� 
		ofstream fout( file.c_str() ); // ���� 
		if ( fout ){  
    		fout << "OID	Arrival	Duration	TimeOut" << endl;
    		while( test < aList.size() ){
    			fout << aList.at(test).OID << "	";
				fout << aList.at(test).arrival << "	" ;
				fout << aList.at(test).duration << "	" ;
				fout << aList.at(test).timeout << endl ;
				test++;
			}
    	}
		fout.close();
		writeend = clock(); // ����g�J�ɮת��ɶ�  
		test = 0;
		cout << endl <<  "Reading data: " << read <<  " clocks (" << read << ".00 ms).\n" ;
		cout << "Sorting data: " << sortend-sortstart << " clocks (" << sortend-sortstart << ".00 ms).\n" ;
		cout << "Writing data: " << writeend-writestart << " clocks (" << writeend-writestart << ".00 ms).\n" ;
		cout << endl;
		cout << "See " << file << endl;
	}
	else if ( command == 2 ){
		tasktwo( fileName );  
	}	
}
bool JobList::getAll( string fileName , int command ){ //Ū�J�ɮ� 
	double start , end;
	double read;
	string file = "input" + fileName + ".txt"; 
	jobType type;
	string s; 
	int number = 0;
	int test = 0;
	int i = 0;
	int j = 0;
	ifstream ifs( file.c_str() );//���}�ɮ�
	start = clock(); // Ū�ɶ}�l�ɶ� 
    if ( ifs ){  
    	getline(ifs, s); // �����ɮת��Ĥ@��Ū��� 
		while ( ifs >> number ){
			aList.push_back( type );
			aList.at(i).OID = number;
			ifs >> aList.at(i).arrival;
			ifs >> aList.at(i).duration;
			ifs >> aList.at(i).timeout;
			i++;
		}
    	ifs.close(); // �����ɮ� 
    	end = clock(); // Ū�ɵ����ɶ� 
    	if ( command == 1 ) {
    		cout <<"	" <<s << endl;
    		while( test < aList.size()){
    			cout << "(" << test+1 << ")	" << aList.at(test).OID << "	";
				cout << aList.at(test).arrival << "	" ;
				cout << aList.at(test).duration << "	" ;
				cout << aList.at(test).timeout << endl ;
				test++;
			}
		}
    	read = end - start;  // Ū�ɯӮ� 
    	getSorted( fileName, read , command);  // �i��shell sorted 
        return true;
    }
    cout << "### " << file << " does not exist! ###" << endl;
}
int main(){
	int command = 0;
	string fileName;
	int determine = 0; // �P�_�e��command�O�_����1 
	do {
		JobList ajoblist; // ���ڭ̯�ϥ�class JobList�̭���function  
		cout << endl << "**** Simulate FIFO Queues by SQF *****" << endl;
		cout << "* 0. Quit                            *" << endl;
		cout << "* 1. Sort a file                     *" << endl;
		cout << "* 2. Simulate one FIFO queue         *" << endl;
		cout << "**************************************" << endl;
		cout << "Input a command(0, 1, 2): ";
		cin >> command;
		cout << endl;
		if ( determine == 0 || command == 1 ){
			cout <<  "Input a file number: ";
			cin >> fileName;
			cout << "\n";
			determine = 1;
		}
		if ( command == 1 ){
			ajoblist.getAll( fileName, command );
			determine = 1; // ����command=1 
		}
		else if ( command == 2 ){
			string file = "sorted" + fileName + ".txt"; 
			ifstream ifs( file.c_str() ); // ���}file�o���ɮ� 
			if ( ifs ){ // �p�G�����ɮ�  
				ajoblist.getAll( fileName , command ); // �I�sgetAll()��functionŪ�����  
			}
			else { // �p�G�S��  
				cout << "### " << "sorted" << fileName << ".txt does not exist! ###" << endl;
			}
		}
		else if ( command == 0 )  break;
		else  cout << "Command does not exist!" << endl;
		
	} while(true);
	system( "pause" );
	return 0;
} 
