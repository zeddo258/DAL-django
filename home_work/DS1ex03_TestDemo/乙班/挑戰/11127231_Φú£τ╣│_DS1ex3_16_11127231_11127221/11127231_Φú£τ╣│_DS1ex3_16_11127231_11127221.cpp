// 11127231 蔡順理 
// 11127221   蘇芠? 
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string.h>
#include <vector>
#include<iomanip>
#include <ctime>
using namespace std;

string file;
struct D {
	int OID;
	int arrival;
	int duration;
	int timeout;
};	

struct c {
	int OID;
	int CID;
	int jobtime;
	int delay;
};

struct  CPU {
	int CID;
	int runtime;
	vector<D> cpuwait;
};
class Datalist {
	vector<D> datalist;
	double rstart, rend,sstart,send,wstart,wend;
	public:		
		void opfile ( string f){
			rstart = clock();
			string fname = f;
			ifstream in;
			in.open(fname.c_str());
			if (!in.is_open()) cout << endl<< "### " <<fname << " does not exist!" << " ###";
			else {
				string a;
				in >> a >> a >> a >> a;
				D data;
				while ( in >> data.OID ){
					in >> data.arrival >> data.duration >> data.timeout;
					datalist.push_back(data);
				}
			in.close();
			}
			rend = clock();
			sort();
		}
		void sort( ) {
			sstart = clock();
			for ( int 	i = datalist.size()/2 ; i > 0 ; i = i/2 ) {   // 切段直到=1  [[d0,d1,d2,d3],[d4,d5,d6,d7]] -> [[d0,d1],[d2,d3],[d4,d5],[d6,d7]]
				for ( int j = 0 ; j < i ; j++ ){  // 一段的contant  橫行 
					for ( int n = j ; n < datalist.size() ; n = n+i){
						D temp;
						for ( int m = n ; m < datalist.size() ; m = m + i) { // 主要更新 
							if ( m+i < datalist.size()){
								if ( datalist[m].arrival > datalist[m+i].arrival){
									temp = datalist[m];
									datalist[m] = datalist[m+i];
									datalist[m+i] = temp;
								}
								else if ( datalist[m].arrival == datalist[m+i].arrival){
									if ( datalist[m].OID > datalist[m+i].OID) {
										temp = datalist[m];
										datalist[m] = datalist[m+i];
										datalist[m+i] = temp;
									}
								}
							}	
						}	
					}
				}
			}
			send = clock();
		}
		void outfile ( string f) {
			wstart = clock();
			string fname = "sorted" + f + ".txt";
			ofstream out;
			out.open(fname.c_str());
			out << "OID" << '\t' << "Arrival" << '\t' << "Duration" << '\t' << "TimeOut" << '\n';
			for ( int i = 0 ; i < datalist.size() ; i++) {
				out << datalist[i].OID << '\t' << datalist[i].arrival << '\t' << datalist[i].duration << '\t' << datalist[i].timeout << '\n';
			}
			out.close();
			wend = clock();
			show(fname);
		}
		
		void show( string f) {
			cout << '\t' << "OID" << '\t' << "Arrival" << '\t' << "Duration" << '\t' << "TimeOut" << '\n';
			for ( int i = 0 ; i < datalist.size() ; i++ ) {
				cout << '(' << i+1 << ')' << '\t'<< datalist[i].OID << '\t' << datalist[i].arrival << '\t' << datalist[i].duration << '\t' << datalist[i].timeout << '\n';
			} 
			
			cout << endl << "Reading data: " << (rend-rstart) <<" clocks (" << (rend-rstart) << ".00 ms).";
			cout << endl << "Sorting data: " << (send-sstart) <<" clocks (" << (send-sstart) << ".00 ms).";
			cout << endl << "Writing data: " << (wend-wstart) <<" clocks (" << (wend-wstart) << ".00 ms).";
			cout << endl;
			cout << endl << "See " << f <<endl;
		}
		void updatedata( vector<D> d) {
			datalist = d;
		}
		 
		vector<D> getdata() {
			return  datalist;
		}
		
};


class oneCPU {
	struct List {
		int OID;
		int jobtime;
		int delay;
	};
	vector<D> list;
	vector<D> cpuwait;
	vector<List> cpudone;
	vector<List> cpulost;
	double Adelay;
	double susrat;
	public:
		oneCPU( ) {
			Datalist data;	
			data.opfile("sorted"+file+".txt");
			list = data.getdata();
			if ( !list.empty()){
				cpurun();
				outp(file);	
			}
			
		}
		void cpudo( int cputime, int &count) {
			List temp;
			//cout << endl << cputime << '\t' << count << '\t'<< cpuwait[0].OID << '\t' << cpuwait[0].timeout;
			if ( count == cpuwait[0].duration) {
				if ( cputime <= cpuwait[0].timeout){
					temp.OID = cpuwait[0].OID;
					temp.jobtime =  cputime;
					temp.delay = cputime - cpuwait[0].arrival - cpuwait[0].duration;
					cpudone.push_back(temp);
					cpuwait.erase(cpuwait.begin());
					count = 0;
					if ( !cpuwait.empty() ) cpudo( cputime, count);
				}
			}
			else if ( cputime > cpuwait[0].timeout){
					temp.OID = cpuwait[0].OID;
					temp.jobtime =  cputime;
					temp.delay = cputime - cpuwait[0].arrival ;
					cpulost.push_back(temp);
					cpuwait.erase(cpuwait.begin());
					count = 0;
					if ( !cpuwait.empty() ) cpudo( cputime, count);
			}
			else if ( cputime == cpuwait[0].timeout){
				if ( cpuwait[0].duration != count) {
					temp.OID = cpuwait[0].OID;
					temp.jobtime =  cputime;
					temp.delay = cputime - cpuwait[0].arrival ;
					cpulost.push_back(temp);
					cpuwait.erase(cpuwait.begin());
					count = 0;
					if ( !cpuwait.empty() ) cpudo( cputime, count);
				}				
			}

		}
		void cpurun() {
			int cputime = 0;
			int cpucount = 0;
			int cputimeend = list[0].timeout;
			int job = 0;
			List temp;
			D now;
			while ( !list.empty() || cputime <= cputimeend ) {
					if ( !cpuwait.empty()) cpudo( cputime, cpucount);	
					while (  !list.empty()&&cputime == list[0].arrival ) {// 處理佇列
						if (  cpuwait.size() < 4) {
							if ( cpuwait.empty()) cpucount = 0;
							if ( list[0].timeout > cputimeend) cputimeend = list[0].timeout;
							
							cpuwait.push_back(list[0]);
							list.erase(list.begin());
						}		
						else {
							temp.OID = list[0].OID;
							temp.jobtime = cputime;
							temp.delay = 0;
							cpulost.push_back(temp); 
							list.erase(list.begin());
						}
					}
				if ( !cpuwait.empty()) cpudo( cputime, cpucount);	
				cputime++;
				cpucount++;
			}
			
			for ( int i = 0 ; i < cpulost.size() ; i++ ) {
				Adelay = Adelay+ cpulost[i].delay;
			}
			for ( int i = 0 ; i < cpudone.size() ; i++ ) {
				Adelay = Adelay+ cpudone[i].delay;
			}
			Adelay = (double)Adelay/ (cpulost.size() + cpudone.size() );
			susrat = (double)cpudone.size() * 100 / ( cpudone.size() + cpulost.size());
		}
		
		void cpushow() {
		}
		
		void outp ( string f) {
			
			string fname = "output" + f + ".txt";
			ofstream out;
			out.open(fname.c_str());
			out << '\t' << "[Abort Jobs]" << '\n';
			out << '\t' << "OID" << '\t' << "Abort" << '\t' << "Delay" << '\n';
			for ( int i = 0 ; i < cpulost.size() ; i++) {
				out << '[' << i +1<< ']' << '\t'<< cpulost[i].OID << '\t' << cpulost[i].jobtime << '\t' << cpulost[i].delay << '\n';
			}
			out << '\t' << "[Jobs Done]" << '\n';
			out << '\t' << "OID" << '\t' << "Departure" << '\t' << "Delay" << '\n';
			for ( int i = 0 ; i < cpudone.size() ; i++) {
				out <<'[' << i+1 << ']' << '\t'<<  cpudone[i].OID << '\t' << cpudone[i].jobtime << '\t' << cpudone[i].delay << '\n';
			}
			out << "[Average Delay]	" <<  fixed << setprecision(2)<< Adelay << " ms";
			out << endl <<"[Success Rate]  " << susrat << " %";
			out.close();
			cout << endl << "The simulation is runninr..." << endl << "See " << "output"<< file << ".txt" << endl;
		}
};

class twoCPU {

	public:
		vector<c> done;
		vector<c> lost;
		int time;
		int timeout;
		vector<D> list;
		twoCPU() {
			vector<CPU> cpu(2);
			Datalist data;
			data.opfile("sorted" + file + ".txt");
			list = data.getdata();
			cpu[0].CID = 1, cpu[1].CID = 2;
			time = 0;
		  	if ( !list.empty()){
		  		timeout = list[0].timeout;
				cpurun( cpu);
		  		outp();	
			 }

		}

		void cpudo(CPU &cpu) {
			c temp;
			if ( cpu.runtime == cpu.cpuwait[0].duration) {
				if ( time <= cpu.cpuwait[0].timeout){
					temp.OID = cpu.cpuwait[0].OID;
					temp.CID = cpu.CID;
					temp.jobtime =  time;
					temp.delay = time - cpu.cpuwait[0].arrival - cpu.cpuwait[0].duration;
					done.push_back(temp);
					cpu.cpuwait.erase(cpu.cpuwait.begin());
					cpu.runtime = 0;
					if ( !cpu.cpuwait.empty() ) cpudo( cpu);
				}
			}
			else if ( time > cpu.cpuwait[0].timeout){
					temp.OID = cpu.cpuwait[0].OID;
					temp.CID = cpu.CID;
					temp.jobtime =  time;
					temp.delay = time - cpu.cpuwait[0].arrival ;
					lost.push_back(temp);
					cpu.cpuwait.erase(cpu.cpuwait.begin());
					cpu.runtime = 0;
					if ( !cpu.cpuwait.empty() ) cpudo( cpu);
			}
			else if ( time == cpu.cpuwait[0].timeout){
				if ( cpu.cpuwait[0].duration != cpu.runtime) {
					temp.OID = cpu.cpuwait[0].OID;
					temp.CID = cpu.CID;
					temp.jobtime =  time;
					temp.delay = time - cpu.cpuwait[0].arrival ;
					lost.push_back(temp);
					cpu.cpuwait.erase(cpu.cpuwait.begin());
					cpu.runtime = 0;
					if ( !cpu.cpuwait.empty() ) cpudo( cpu);
				}				
			}
		}
		void cpurun( vector<CPU> &cpu) {
			c temp;
		 	while ( !list.empty() || time <= timeout ) {			
  				if ( !cpu[0].cpuwait.empty()) cpudo(cpu[0]);
  				if ( !cpu[1].cpuwait.empty())cpudo(cpu[1]);	
				while (  !list.empty() && time == list[0].arrival ) {// 處理佇列
					if ( cpu[0].cpuwait.empty() || cpu[1].cpuwait.empty()){
						if ( cpu[0].cpuwait.empty() ) {
							if ( list[0].timeout > timeout) 
								timeout = list[0].timeout;
							cpu[0].runtime = 0;
							cpu[0].cpuwait.push_back(list[0]);
							list.erase(list.begin());
						}
						else if ( cpu[1].cpuwait.empty() ) {
							if ( list[0].timeout > timeout) 
								timeout = list[0].timeout;
							cpu[1].runtime = 0;
							cpu[1].cpuwait.push_back(list[0]);
							list.erase(list.begin());
						}
					}
					else if (  !cpu[0].cpuwait.empty() && !cpu[1].cpuwait.empty()) {
						if ( cpu[0].cpuwait.size() < 4 || cpu[1].cpuwait.size() < 4) {
							if  ( cpu[0].cpuwait.size() <= cpu[1].cpuwait.size() ) {
								if ( list[0].timeout > timeout) timeout = list[0].timeout;
								cpu[0].cpuwait.push_back(list[0]);
								list.erase(list.begin());
							}	
							else if ( cpu[0].cpuwait.size() > cpu[1].cpuwait.size() ) {
								if ( list[0].timeout > timeout) timeout = list[0].timeout;
								cpu[1].cpuwait.push_back(list[0]);
								list.erase(list.begin());
							}
						}
						else if ( cpu[0].cpuwait.size() >= 4 && cpu[1].cpuwait.size() >= 4){
							temp.OID = list[0].OID;
							temp.CID = 0;
							temp.jobtime = time;
							temp.delay = 0;
							lost.push_back(temp); 
							list.erase(list.begin());
						}
					}
				}
					time++;
					cpu[0].runtime++;
					cpu[1].runtime++;
			}
		}
		void outp() {
			double ad,sr;
			ofstream out;
			string fn = "double" + file + ".txt";
			out.open(fn.c_str());
			out << '\t' << "[Abort Jobs]" << '\n';
			out << '\t' << "OID" << '\t' << "CID" << '\t'<< "Abort" << '\t' << "Delay" << '\n';
			for ( int i = 0 ; i < lost.size() ; i++) {
				ad = ad + lost[i].delay;
				out << '[' << i +1<< ']' << '\t'<< lost[i].OID << '\t'<< lost[i].CID<< '\t' << lost[i].jobtime << '\t' << lost[i].delay << '\n';
			}
			out << '\t' << "[Jobs Done]" << '\n';
			out << '\t' << "OID" << '\t'<< "CID" << '\t' << "Departure" << '\t' << "Delay" << '\n';
			for ( int i = 0 ; i < done.size() ; i++) {
				ad = ad + done[i].delay;
				out <<'[' << i+1 << ']' << '\t'<<  done[i].OID << '\t'<< done[i].CID << '\t' << done[i].jobtime << '\t' << done[i].delay << '\n';
			}
	
			ad = (double) ad / ( done.size()+ lost.size());
			sr = (double) done.size() * 100 / ( done.size()+ lost.size());
			out << "[Average Delay]	" <<  fixed << setprecision(2)<< ad << " ms";
			out << endl <<"[Success Rate]  " << sr << " %";
			cout << endl << "The simulation is runninr..." << endl << "See " << "double"<< file << ".txt" << endl;
		}

};

int main()
{
     int cmd = 0; // 輸入指令 
     Datalist data;
     do
     {
          string filename; // 檔案編號 
          cout << endl
               << "**** Simulate FIF0 Queues by SQF ****";
          cout << endl
               << "* 0. Quit                           *";
          cout << endl
               << "* 1. Sort a file                    *";
          cout << endl
               << "* 2. Simulate one FIF0 queue        *";
          cout << endl
               << "*****************";
          cout << endl
               << "Input a command(0,1,2,3):";
          cin >> cmd;
          if ( cmd == 0) break;
          else {
          	if ( cmd == 1 || cmd == 2 || cmd == 3 ) {
          		if ( data.getdata().empty()) {
          		    cout << endl << "Input a file number : ";
          			cin >> file;	
          			data.opfile("input"+file+".txt");
				}
				if ( cmd == 1) {
          			data.outfile(file);
		  		}
		  		else if ( cmd == 2){
		  			oneCPU cpu;
		  		}
		  		else if ( cmd == 3 ) {
					twoCPU cpu;
				}
		  		else cout << endl << "Command does not exist!"<< endl;
			}
		}
    } while (true);
}
