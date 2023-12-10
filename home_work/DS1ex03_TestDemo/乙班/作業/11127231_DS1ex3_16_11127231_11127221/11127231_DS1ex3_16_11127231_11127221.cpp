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
class Datalist {
	vector<D> datalist;
	double rstart, rend,sstart,send,wstart,wend;
	public:		
		void opfile ( string f){
			rstart = clock();
			string fname = "input"+f+".txt";
			ifstream in;
			in.open(fname.c_str());
			if (!in.is_open()) cout << endl<< fname << " does not exist!";
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
		/*
		void desort( ) {
			for ( int 	i = datalist.size()/2 ; i > 0 ; i = i/2 ) {   // 切段直到=1  [[d0,d1,d2,d3],[d4,d5,d6,d7]] -> [[d0,d1],[d2,d3],[d4,d5],[d6,d7]]
				for ( int j = 0 ; j < i ; j++ ){  // 一段的contant  橫行 
					for ( int n = j ; n < datalist.size() ; n = n+i){
						D temp;
						for ( int m = n ; m < datalist.size() ; m = m + i) {
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
		}
		*/
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

class CPU {
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
		CPU( ) {
			Datalist data;
			data.opfile(file);
			list = data.getdata();
			cpurun();
			outp(file);
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
		}
};


int main()
{
     int cmd = 0; // 輸入指令 
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
               << "Input a command(0,1,2):";
          cin >> cmd;
          if ( cmd == 0) break;
          else {
          	cout << endl << "Input a file number : ";
          	cin >> file;
          	Datalist data;
          	if ( cmd == 1) {
          		data.opfile(file);
          		data.outfile(file);
		  	}
		  	else if ( cmd == 2){
		  		CPU cpu;
		  		cout << endl << "The simulation is runninr..." << endl << "See " << "output"<< file << ".txt" << endl;
		  	}
		  	else cout << endl << "Command does not exist!"<< endl;
		}

    } while (true);
}
