// 11120117 �L�a�� 
// 11120127 ���ɥ� 

#include <stdio.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <iomanip>

using namespace std;

#define Max_LEN 8

struct Data{	
	int OID ;
	int Arrival ;
	int Duration ;
	int TimeOut ;
	int Abort ;
	int Delay ;	
	int Departure ;
};

class Queue{
	
	private:
		vector<Data> DataBase ;
		vector<Data> Copy ;
		string fileN;
		double startR;
		double startS;
		double startW;
	
	public:
		Queue(){}
		
		void Inqueue( Data input ) {
			
			DataBase.push_back( input );
			
			return ;
			
		}
		
		Data Dequeue() {
			
        	Data first = DataBase.front() ;
          	DataBase.erase( DataBase.begin() ) ; 
          	return first ;
          
		}
		
		int size() {
			return DataBase.size() ;
		}
		
		bool empty() {
			
		  if ( DataBase.empty() )
			return true ;
		
		  return false ;
			
		}
		
		Data GetData( int index ) {
			return DataBase.at( index ) ;
		}
		
		void show(){
			
		  int i = 0 ;	
		  int j = 1;
		  DataBase = Copy ; 
		  cout << "        OID     Arrival Duration        TimeOut" << endl;
		  
		  while( i < DataBase.size() ){
		  	cout << "(" << j << ")" << "\t";
		  	
		  	cout << DataBase[i].OID << "\t";
		  	
		  	cout << DataBase[i].Arrival << "\t";
		  	
		  	cout << DataBase[i].Duration << "\t";
		  	
		  	cout << DataBase[i].TimeOut << endl;
		  	i = i + 1 ;
		  	j = j + 1 ;
		  }
		  
		  cout << endl;
		  
		  int Read = static_cast<int>(startR);
		  int Sort = static_cast<int>(startS);
		  int Write = static_cast<int>(startW);
		  cout << "Reading data: " << Read << " clocks (" << fixed << setprecision(2) << startR << " ms)." << endl;
		  cout << "Sorting data: " << Sort << " clocks (" << fixed << setprecision(2) << startS << " ms)." << endl;
		  cout << "Writing data: " << Write << " clocks (" << fixed << setprecision(2) << startW << " ms)." << endl << endl;
		  cout << "See " << fileN << endl << endl;
		}
		
		void clear() {
			
			DataBase.clear() ;
			Copy.clear() ;
			return ;
		}
		
		bool load(string fileName){
		  FILE *infile = NULL;
          bool success = false;
          
          fileN = "sorted" + fileName + ".txt";
          fileName = "input"+ fileName +".txt";
          
          infile = fopen(fileName.c_str(), "r");
          if(infile == NULL) cout << "### " << fileName << " does not exist! ###" << endl << endl;
          else {
          	clock_t start, finish;   
		    start = clock();
          	string OID;
          	string Arrival;
          	string Duration;
          	string Timeout;
          	
            fscanf(infile,"%s %s %s %s", &OID, &Arrival, &Duration, &Timeout);
            Data newData;
            while ( fscanf(infile,"%d %d %d %d", &newData.OID, &newData.Arrival, &newData.Duration, &newData.TimeOut) != EOF ){
                DataBase.push_back(newData);
			}
			
			success = true;
			fclose(infile);
			finish = clock(); 
			startR = (double)(finish - start) / CLOCKS_PER_SEC;  
          }
          
          Copy = DataBase;
          return success;
		}
		
		void sort(){
		  int n = DataBase.size() / 2;
		  int temp = 0, temp1 = 0, temp2 = 0;
		  int temp3 = 0;
		  clock_t start, finish;   
		  start = clock();
		  
		  while (n >= 1) {
		  		// cout << "n = " << n << " " ; // ���զ� 
            for(int i = n ; i < DataBase.size(); i++) {
            	// cout << "i = " << i << " " ; // ���զ�
                // �M���U�դ��Ҧ������� , �B���� DataBase.size() / 2
                for(int j = i - n; j >= 0; j -= n) {
                    // cout << "j = " << j << " " ; // ���զ�
                    // �p�G��e�����j��[�W�B���᪺���Ӥ���,�����ݭn�洫
                    if(DataBase[j].Arrival >= DataBase[j+n].Arrival) { //�洫���� 
                    	if (DataBase[j].Arrival == DataBase[j+n].Arrival){ //�۵��h���OID�j�p 
                    		if (DataBase[j].OID > DataBase[j+n].OID){ // OID���j�N�n�� 
                    		  temp = DataBase[j].Arrival;
                              temp1 = DataBase[j].OID;
                              temp2 = DataBase[j].Duration;
                              temp3 = DataBase[j].TimeOut;
                          
                              DataBase[j].Arrival = DataBase[j+n].Arrival;
                              DataBase[j].OID = DataBase[j+n].OID;
                              DataBase[j].Duration = DataBase[j+n].Duration;
                              DataBase[j].TimeOut = DataBase[j+n].TimeOut;
                          
                              DataBase[j+n].Arrival = temp;
                              DataBase[j+n].OID = temp1;
                              DataBase[j+n].Duration = temp2;
                              DataBase[j+n].TimeOut = temp3;
							}
						}
                    	else { // ���`�洫 
                          temp = DataBase[j].Arrival;
                          temp1 = DataBase[j].OID;
                          temp2 = DataBase[j].Duration;
                          temp3 = DataBase[j].TimeOut;
                          
                          DataBase[j].Arrival = DataBase[j+n].Arrival;
                          DataBase[j].OID = DataBase[j+n].OID;
                          DataBase[j].Duration = DataBase[j+n].Duration;
                          DataBase[j].TimeOut = DataBase[j+n].TimeOut;
                          
                          DataBase[j+n].Arrival = temp;
                          DataBase[j+n].OID = temp1;
                          DataBase[j+n].Duration = temp2;
                          DataBase[j+n].TimeOut = temp3;
                        }
                        
                    }
                }
                // cout << endl ; // ���զ�
            }
            
            n = n / 2; // �C���B���A�~�򰣥H2
            
            // cout << endl ; // ���զ�
        }
			
		  finish = clock(); 
		  startS = (double)(finish - start) / CLOCKS_PER_SEC;
		  return;  	
		}
		
		void write(){
		  clock_t start, finish;
		  FILE *infile = NULL;
		  infile = fopen(fileN.c_str(), "w"); // �g�ɥ�w 
		  start = clock();
		  int i = 0;
		  
		  fprintf(infile,"OID\tArrival\tDuration\tTimeOut\n");
		  while (i < DataBase.size() ){
		  	fprintf(infile,"%d\t%d\t%d\t%d",DataBase[i].OID,DataBase[i].Arrival,DataBase[i].Duration,DataBase[i].TimeOut);
		  	fprintf(infile, "\n");
			i++;
		  }
		  
		  fclose(infile);
		  finish = clock(); 
		  startW = (double)(finish - start) / CLOCKS_PER_SEC ; 
		}
	
};

void OutputData( Queue AbortedJobs, Queue DoneJobs, string fileNum ) {
	
  int i =1;
	
  double totalJobs = AbortedJobs.size() + DoneJobs.size() ;
  double Delay = 0;
  
  double success = 0 ;
  
  string fileName = "output" + fileNum + ".txt" ;
		
  Data temp ; 
  
  FILE *infile = NULL;
  
  ofstream fout( fileName ) ; // C++ �g�� 
  
  fout << "\t[Abort Jobs]\n\tOID\tAbort\tDelay" << endl ; 

  while ( ! AbortedJobs.empty() )  {
  	
  	temp = AbortedJobs.Dequeue();
  	
	fout <<  "[" << i << "]\t" << temp.OID << "\t" << temp.Abort << "\t" << temp.Delay << endl ;
  	
  	Delay += temp.Delay ;
  	
  	i ++ ;
  	
  }
  
  fout << "\t[Jobs Done]\n\tOID\tDeparture\tDelay" << endl ;
  
  i = 1;
  while ( ! DoneJobs.empty() )  {
  	
  	temp = DoneJobs.Dequeue();
  	
	fout <<  "[" << i << "]\t" << temp.OID << "\t" << temp.Departure << "\t" << temp.Delay << endl ;
	
	Delay += temp.Delay ;
  	success ++ ;
  	
  	i ++ ;
  	
  }
  
  fout << "[Average Delay]\t" << fixed << setprecision(2) << Delay / totalJobs  << " ms" << endl ;
  fout << "[Success Rate]\t" << fixed << setprecision(2) << success / totalJobs * 100 << " %" << endl ;

  fout.close() ;
  	
  return ;
	
}

void RunCPU( Queue sorted, Queue & AbortedJobs , Queue & DoneJobs ) {
	
  // �B�z���ǡG�B�zCPU������� >> �qCPU���N�B�z�n����ƴ����X��  >> �B�z�Ȧs����Delay >> �ˬdCPU����ƬO�_�O��  >> ��CPU���ŮɡA���ˬd�Ȧs��������ƬO�_�O�ɡA�A��iCPU��
  //         >> �Nsorted������ƴ��i�hCPU�άO�Ȧs�� >> �ɶ����i����sorted�MCPU����

  int i = 0;

  int currentTime = 0;
	
  Data temp ;
  
  Queue CPU ;
  Queue saver ;

	
  while ( ! CPU.empty() || ! saver.empty() || ! sorted.empty() ) {
		
	if ( ! CPU.empty() ) {

	  temp = CPU.Dequeue() ;
	  temp.Duration -- ;
			
	  CPU.Inqueue( temp ) ;
			
	} // �B�zCPU������� 
		
	if ( ! CPU.empty() && CPU.GetData(0).Duration == 0 )  {
	
	  temp = CPU.Dequeue();
	  temp.Departure = currentTime ;
	  
	  DoneJobs.Inqueue( temp );
	  
	} // �qCPU���N�B�z�n����ƴ����X�� 
	
	i = 0;
	while ( i < saver.size() ) {
		
	  temp = saver.Dequeue() ;
	  temp.Delay ++ ;
	  
	  saver.Inqueue( temp );
		
	  i ++ ;
	  
    } // �B�z�Ȧs����Delay (���i���i�hCPU��|�B�z���A���O���Q�Ի~�쪺���p) 
	
	
	if ( ! CPU.empty() && currentTime >= CPU.GetData(0).TimeOut ) {
	
	  temp = CPU.Dequeue() ;
	  temp.Abort = currentTime ;
	  temp.Delay = currentTime - temp.Arrival ;
			
	  AbortedJobs.Inqueue( temp ) ;
			
	} // �ˬdCPU����ƬO�_�O��

	while ( CPU.empty() && ! saver.empty() && currentTime >= saver.GetData(0).TimeOut ) {
		
	  temp = saver.Dequeue();
	  temp.Abort = currentTime ;
	  temp.Delay = currentTime - temp.Arrival ;
	  
	  AbortedJobs.Inqueue( temp ) ; 
	  
	} // ��CPU���ŮɡA�ˬd�Ȧs��������ƬO�_�O��

	if ( CPU.empty() && ! saver.empty() ) 
	  CPU.Inqueue( saver.Dequeue() ) ; // �q�Ȧs����������Ʀ�CPU�� 
	  
	while ( ! sorted.empty() && currentTime == sorted.GetData(0).Arrival ) {

	  if ( CPU.empty() ) {
	  	
	  	temp = sorted.Dequeue() ;
		temp.Delay = 0 ; 
	  	
	  	CPU.Inqueue( temp ) ;
	  	
	  } // �N��ƥ�iCPU���B�z 
	  	
	  else if ( saver.size() < 3 ) {
	  	
	  	temp = sorted.Dequeue() ;
		temp.Delay = 0; 
	  	
	    saver.Inqueue( temp ) ;
	    
      } // �N��ƥ���i�Ȧs�� 
	    
	    
	  else {
	  	
	    temp = sorted.Dequeue();
	    temp.Abort = currentTime ;
	    temp.Delay = 0;
	    
	    AbortedJobs.Inqueue( temp ) ;
	    
      } // �S���Ŧ�A�����B�z�{�� 
	
	} // �Nsorted������ƴ��i�hCPU�άO�Ȧs��
	
	currentTime ++ ;
	
  } 
	
  return ;
		
}

bool LoadSortedFile( string fileName, Queue & sorted ){
	
	string fileSorted = "\0" ;
	
    bool success = false;
    
    Data newData ;
	
	FILE *infile = NULL;
	
	newData.OID = 0;
	newData.Arrival = 0;
	newData.Duration = 0;
	
	newData.TimeOut = 0;
	newData.Abort = 0;
	newData.Delay = 0;
	
	newData.Departure = 0;

    fileSorted = "sorted" + fileName + ".txt";
          
	infile = fopen(fileSorted.c_str(), "r");
	
	if(infile == NULL) 
		cout << "### " << fileSorted << " does not exist! ###" << endl << endl;
	
    else {   

    	string OID;
        string Arrival;
        string Duration;
        string Timeout;
          	
        fscanf(infile,"%s %s %s %s", &OID, &Arrival, &Duration, &Timeout); // Ū���@�}�l����r 
        
        Data newData;
        
        while ( fscanf(infile,"%d %d %d %d", &newData.OID, &newData.Arrival, &newData.Duration, &newData.TimeOut) != EOF ) 
          sorted.Inqueue( newData );

		success = true;
		fclose(infile);
		
    }
          
    return success;
    
	}

int main() {
	
	Queue input ;
	Queue sorted ;	
	Queue AbortedJobs ;
	Queue DoneJobs ;
	
	int command = 0;
	string fileNum = "\0" ;
	cout << endl; 
	
	do{
	  cout << "**** Simulate FIFO Queues by SQF *****" << endl;
      cout << "* 0. Quit                            *" << endl;
   	  cout << "* 1. Sort a file                     *" << endl;
      cout << "* 2. Simulate one FIFO queue         *" << endl;
      cout << "**************************************" << endl;
      cout << "Input a command(0, 1, 2): " ;
      
      cin >> command;
      cout << endl;
      cin.ignore( Max_LEN, '\n' );
      
      if ( command == 0 ) break;
      else if ( command == 1 ) { 
      
      	cout << "Input a file number: " ;
      	cin >> fileNum;
      	cout << endl;
      	
      	if ( input.load(fileNum) ) {
      		
      		input.sort(); // shell sort 
      		input.write(); //�g�� 
      		input.show(); // output
      		input.clear(); //initialization
      		
		}
      } // if command == 1
      
      else if ( command == 2 ) { 
      	
      	if ( fileNum.compare( "\0" ) == 0 ) {
      		
      		cout << "Input a file number: " ;
      		cin >> fileNum;
      		cout << endl;
      		
		} // ��������{���ÿ�J2 
		
      	if ( LoadSortedFile( fileNum, sorted ) ) {
      		
      		cout << "The simulation is running..." << endl ; 

      		RunCPU( sorted, AbortedJobs, DoneJobs );
      		
      		OutputData( AbortedJobs, DoneJobs, fileNum ); 
      		
      		sorted.clear();
      		AbortedJobs.clear() ;
      		DoneJobs.clear();
      		
      		cout << "See output" << fileNum << ".txt" << endl << endl ;
      		
		}
			
      } // if command == 2
      
      else cout << "Command does not exist!" << endl << endl;
      
    } while (command != 0);
    
    system("pause");
    return 0;
    
}

//   input a file (inputXXX.txt)
//
//	 scan data and put data into input
//
//	 sort data with shell sort (�ƺ��Ƨ�) and save file as sortedXXX.txt
//	
//	 print elapse time
//
 
// 	 run cpu and save into output (outputXXX.txt)
//	
//	 print result
//
//
// clear every Queue 
//	
