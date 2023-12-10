/*

10942208 ���y�w 
03109309 �f�K�M 
DS1ex#3_26_03109309_10942208

*/

#include<stdio.h>
#include<string.h>
#include<string>
#include<iostream>
#include <fstream>
#include <ctime>
#include<vector>

using namespace std ;


struct Jobtype {
	// Queue item type
	
	int OID ; // task number
	int Arrival ; // �u�@�ɨ�(�ĴX�@��) 
	int Duration ; // �u�@�Ӯ� (�ĴX�@��) 
	int TimeOut ;  // �O�ɮɨ� (�ĴX�@��) 
	Jobtype* next ;
}; // JobtypeItem

//******************************************queue��C*********************************************

class Queue {
	
	private :
		int num = 0 ;
		Jobtype* backPtr ; // ���ݸ`�I 
		//Jobtype* frontPtr ;  // �Y�`�I 
		
	public :
		Queue() : backPtr(NULL) {
			
		} // constructor

		bool isEmpty() {
			if( backPtr == NULL ) return true ;
			else return false ;
		} // isEmpty
		
		Jobtype deQueue() {
		    
			if(isEmpty())  printf("Queue is empty") ;
			else {
				Jobtype* tempPtr = backPtr->next; // frontPtr
				if(backPtr==backPtr->next) {
					backPtr = NULL ;
				} // if
				else backPtr->next = tempPtr->next ;
				return *tempPtr ;
//				tempPtr->next = NULL ;
//				delete tempPtr ;
				num--;
			} 

		} // deQueue
		
		void enQueue( int OID, int Arrival, int Duration, int TimeOut ) {
			
			Jobtype* curPtr = new Jobtype;
			curPtr->OID = OID;
			curPtr-> Arrival =  Arrival;
			curPtr->Duration = Duration;
			curPtr->TimeOut = TimeOut;
			curPtr->next = NULL;
			
			if(this->isEmpty()) {
				curPtr->next = curPtr ; // circular	
				//frontPtr = curPtr ; // �Y�`�I���V�s�`�I 
			}
			else {
				curPtr->next = backPtr->next ;
				backPtr->next = curPtr ;
			} // else 
			
			backPtr = curPtr ;
			num++ ;
			
		} // enQueue
		
		int getqueueLength() {
			return num ;
//			int length = 0 ;
//			Jobtype* curPtr = new Jobtype;
//			curPtr = backPtr->next ;
//			
//			do{
//				
//				length++;
//				curPtr = curPtr->next ;	
//					
//					
//			} while( curPtr != backPtr) ; //c
//			
//			return length;
		} // getQueue
		
		void printQueue() {
		
			Jobtype* curPtr = new Jobtype;
			curPtr = backPtr->next ;
			//cout << curPtr ;
			cout << curPtr->OID << "\t" <<  curPtr->Arrival <<  "\t" << curPtr->Duration <<  "\t" << curPtr->TimeOut << endl ;
			do{
				if (curPtr->next != curPtr ) {
					curPtr = curPtr->next ;	
					cout << curPtr->OID << "\t" <<  curPtr->Arrival <<  "\t" << curPtr->Duration <<  "\t" << curPtr->TimeOut << endl ;
				}

				//curPtr = curPtr->next ;	
					
					
			} while( curPtr != backPtr) ; //c

		} // printQueue
		
		
		 
}; // Queue
//******************************************queue��C end*********************************************


//******************************************AnsList***************************************************
class AnsList {
	private:
		
		struct abortType {
			int OID;
			int Abort; // �˱�ɶ� 
			int Delay ; // ����ɶ� 
		};
		struct doneType {
			int OID;
			int Departure; // ��F�ɶ� 
			int Delay ; // ����ɶ� 
		};
		vector<abortType> abortJobs;
		vector<doneType> doneJobs;
		float avgDelay;
		float successRate;
		ifstream inFile ; 
        ofstream outFile;
        int Abortsize = 0 ;
		int Donesize = 0 ;
	public:
		AnsList(){
			this->avgDelay=0.00;
			this->successRate=0.00;
		}
		
		void showAbortJobs() {
			cout << "        [Abort List]" << endl ;
			cout <<"        OID     Abort   Delay"<<endl;
		    /*for (size_t i = 0; i < abortJobs.size(); ++i) {
		        cout << abortJobs[i].OID << endl;  
		    } // for*/
		    
		    for( int j = 0 ; j < abortJobs.size() ; j++ ) {
				printf("[%d]\t", j+1 );
				cout << abortJobs[j].OID << "\t" << abortJobs[j].Abort << "\t" << abortJobs[j].Delay << endl ;
			} // for
			
		    cout<<endl;
		} // showAbortJobs()
		void showDoneJobs() {
			cout << "        [Done List]" << endl ;
			cout<<"        OID     Departure       Delay"<<endl;
			/*for (size_t i = 0; i < doneJobs.size(); ++i) {
		        cout << doneJobs[i].OID << endl;  
		    } // for*/
		    
		    for( int j = 0 ; j < doneJobs.size() ; j++ ) {
				printf("[%d]\t", j+1 );
				cout << doneJobs[j].OID << "\t" << doneJobs[j].Departure << "\t" << doneJobs[j].Delay << endl ;
			} // for
			
		    cout<<endl;
		} // showDoneJobs()
		void computeStat() {
			int totalDelay = 0;
			if (abortJobs.empty() && doneJobs.empty() ) {
	            totalDelay = 0; // Avoid division by zero
	        }
	
	        for (size_t i = 0; i < abortJobs.size(); ++i) {
	            totalDelay += abortJobs[i].Delay;
	        } // �p�����`delay 
	        for (size_t i = 0; i < doneJobs.size(); ++i) {
	            totalDelay += doneJobs[i].Delay;
	        } // �p�⧹���`delay 
	        
			if((abortJobs.size() + doneJobs.size())!=0) {
				float aDelay = (float)(totalDelay) / (abortJobs.size() + doneJobs.size()) ;
	        	this->avgDelay = aDelay;
			}
			if((abortJobs.size() + doneJobs.size())!=0) {
	        	this->successRate = doneJobs.size()/(this->Abortsize + this->Donesize) ;
	    	} 
		} // computeStat()
		
		void showStat() {
			//[[Average Delay]        2.83 ms
			//[Success Rate]  16.67 %
			printf("[[Average Delay]        %.2f ms\n\n",this->avgDelay );
			printf("[Success Rate]  %.2f %s\n\n",this->successRate*100,"%" );

		} // showStat()
		
		void showAll() {
		} // showAll()
		
		void addAbortJob( int OID,int Abort, int Delay ) {
			abortJobs.push_back({OID, Abort, Delay});
			Abortsize++;
		} // addAbortJob()
		
		void addDoneJob( int OID,int Done, int Delay ) {
			doneJobs.push_back({ OID, Done, Delay });
			Donesize++;
		} // addDoneJob()
		int getAbortsize(){
			return Abortsize;
		}
		int getDonesize(){
			return Donesize;
		}
		
		void putAll( string fileID ) {
			//********************************************************�g��
                string outfile = "output" +  fileID  + ".txt";
                
				outFile.open(outfile.c_str()); 		    
            	
	            outFile << "        [Abort List]" << endl ;
				outFile <<"        OID     Abort   Delay"<<endl;
	    
			    for( int j = 0 ; j < abortJobs.size() ; j++ ) {
					outFile << j+1 << "\t" ;
					outFile << abortJobs[j].OID << "\t" << abortJobs[j].Abort << "\t" << abortJobs[j].Delay << endl ;
				} // for
				
			    outFile<<endl;
			    //-----------------------------
				outFile << "        [Done List]" << endl ;
				outFile<<"        OID     Departure       Delay"<<endl;	    
			    for( int j = 0 ; j < doneJobs.size() ; j++ ) {
					outFile << "[" << j+1 << "]"<< "\t" ;
					outFile << doneJobs[j].OID << "\t" << doneJobs[j].Departure << "\t" << doneJobs[j].Delay << endl ;
				} // for
				
			    outFile << endl;
			    //-----------------------------
			    
			    //outFile << this->showStat() << endl ;
				outFile << "[[Average Delay]        "  << std::fixed << this->avgDelay << " ms" << endl << endl;
    			outFile << "[Success Rate]  " << std::fixed << this->successRate * 100 << "%"<< endl << endl;
    			
			    outFile.close();

			//********************************************************�g�ɵ��� 
		} // putAll() 
	
} ; // class AnsList
//******************************************AnsList_end*********************************************

//******************************************�u�@�Ƶ{�M��*********************************************
class JobList {
	private :
		ifstream inFile ; 
        ofstream outFile;
        int OID ,Arrival,Duration,TimeOut = 0;

        string title;
		string fileID ; // CPU
		int max_records ;
		int delcur = 0 ;
		Jobtype* aList; // ��l�ưʺA�}�C (CPU�u�@�C��)
		int cur;
	public:
		JobList( const int max_records, const string fileNum ) {
			this->max_records= max_records;
			this->fileID = fileNum ;
			this->cur=0;
			aList = new Jobtype[max_records];
		} // constructor
		
		~JobList() {
			delete[] aList;
		}
		
		int getjoblistnum(){
			return max_records;
		} 
		
		bool isEmpty() const {
			if (this->aList == NULL ) return true ;
			else return false ;
		} // isEmpty() 
		bool getAll() {
			string fileName = "input" + fileID + ".txt";
			//********************************************************Ū�� 
    		//reading_start_clock = clock();
            inFile.open(fileName.c_str()) ;
            if (!inFile.is_open()) {
            cout << fileName << "### does not exist! ###" << endl << endl;
                // Clean up allocated memory before breaking
                //delete[] aList;
                return false ;
            }
			else {
				// Read file
	            getline(inFile, title);
	            int i = 0;
	        	while (inFile >> OID >> Arrival >> Duration >> TimeOut){
	                	//JobList1.enJobList(JobList,OID,Arrival,Duration,TimeOut);	
	                aList[i].OID = OID ;
	                aList[i].Arrival = Arrival ;
	                aList[i].Duration = Duration ;
	                aList[i].TimeOut = TimeOut ;
	                i++;
				} // while
				inFile.close();
				//reading_end_clock = clock();
				//*********************************************************Ū�ɵ��� 
				return true ;
			}

		}
		
		bool getSorted() {
			for (int h=(this->max_records)/2; h>0; h=h/2) {
				for(int unsorted=h; unsorted<this->max_records; ++unsorted){
					int loc=unsorted;
					Jobtype nextItem = aList[unsorted];
					for(;(loc>=h) && ((aList[loc-h].Arrival>nextItem.Arrival) || ((aList[loc-h].Arrival==nextItem.Arrival) && (aList[loc-h].OID>nextItem.OID)));loc=loc-h){
						aList[loc]=aList[loc-h];							
					}
					aList[loc]=nextItem;
				}	
			}
		}
		
		Jobtype* getList(){
			return aList;
		} 
		
		void showJob() const {
			cout << "\t"<< this->title << endl;
			for( int j = 0 ; j < this->max_records ; j++ ) {
				printf("(%d)\t", j+1 );
				cout << (*(aList+j)).OID << "\t" << (*(aList+j)).Arrival << "\t" << (*(aList+j)).Duration << "\t" << (*(aList+j)).TimeOut << endl ;
			} // for
			
			cout << endl ;
			
		} // showJob()
		
		void putAll() {
			//********************************************************�g��
                string outfile = "sorted" +  this->fileID  + ".txt";
				outFile.open(outfile.c_str()); 
			    //.c_str() �bC++�A std::ofstream����const char�A�Ӥ��Ostring
			    
			    //front = JobList1.getJobList() ;
			    outFile << title << endl ; // ��cout�ϥΤ�k����
			    for (int j = 0; j < max_records ; j++) {
                    outFile << (*(aList+j)).OID << "\t" << (*(aList+j)).Arrival << "\t" << (*(aList+j)).Duration << "\t" << (*(aList+j)).TimeOut << endl ;
                }
			    
			    outFile.close();

			//********************************************************�g�ɵ��� 
		} // putAll()
		
		Jobtype nextJob() {
			 
			cur++; 
			if(cur-1 >= 0) {
				cout<<"The next job:"<<"\t"<<(*(aList+cur-1)).OID<<"\t"<<(*(aList+cur-1)).Arrival<<"\t"<<(*(aList+cur-1)).Duration<<"\t"<<(*(aList+cur-1)).TimeOut<<endl<<endl;
				if( cur <= max_records ) {
					
					printf("%s%d%s\n\n","*** There are ", max_records-cur , " jobs in total." );
					cout << "\t" << title << endl ; 
				    for (int j = cur,y=1; j < max_records ; j++,y++) {
				    	printf("(%d)\t", y );
	                    cout << (*(aList+j)).OID << "\t" << (*(aList+j)).Arrival << "\t" << (*(aList+j)).Duration << "\t" << (*(aList+j)).TimeOut << endl ;
	                } // for
				}
	
				return (*(aList+cur-1));
			}

		}
	
	
		void deleteFirstElement() {
			
			if (max_records <= 0) {
		    }else {
				for( int i = 0 ; i+1 < max_records ; i++  ) {
					(*(aList+i)) = (*(aList+i+1)) ;
				}
			}
		}

		

		
		

		/*void operation(){
			cout<<"Queue 1 ("<<CPUtime<<"):"<<endl;
			
		}*/
	
}; // JobList
//******************************************************************************************************Stimulation Class
/*class Simulation{
	Joblist jobs; //a list of jobs
	AnsList answers;
	int qNum;
	void delQ(int){
		
	}
	void updateQ(int,int){
		
	}
	void finishQ(){
		
	}
	
	public:
		Simulation(JobList aList, int N): jobs(aList),qNum(N){ //�غcSimulation 
		}
		~Simulation(){ //�ѺcSimulation 
		}
		
};*/

//******************************************�u�@�Ƶ{�M��end********************************************* 

int main() {
    int cmd = -1;
    Queue Queue_3memb;
    int CPUtime=0;
    
    do {
        cout << "**** Simulate FIFO Queues by SQF *****" << endl;
        cout << "* 0. Quit                            *" << endl;
        cout << "* 1. Sort a file                     *" << endl;
        cout << "* 2. Simulate one FIFO JobList       *" << endl;
        cout << "**************************************" << endl;
        cout << "Input a command (0, 1, 2): ";
        cin >> cmd;
        
        //ArrayQueue JobList1;
    	//Jobtype* front ;
        ifstream inFile ; 
        ofstream outFile;
        int max_records = 0 ;
        int OID ,Arrival,Duration,TimeOut = 0;
        string fileName;
        string fileNum;
        bool successOpen = -1 ;
		bool cpuavil ;
        string title;
        //��l�ƭp�� 
        clock_t reading_start_clock ;
        clock_t reading_end_clock ;
        clock_t sort_start_clock ;
        clock_t sort_end_clock ;
        clock_t writing_start_clock ;
        clock_t writing_end_clock ;
		double elapsed_reading_time_ms ;
		double elapsed_sort_time_ms ;
		double elapsed_writing_time_ms ;
        
        if ( cmd == 1 or cmd == 2 ) {
        	
        	cout << "\n" << "Input a file number:";
            cin >> fileNum;
            cout << endl ;
            fileName = "input" + fileNum + ".txt";
			inFile.open(fileName.c_str()) ; 
			
	        if (inFile.is_open()) {
	
	        	getline(inFile, title);
	        	while (inFile >> OID >> Arrival >> Duration >> TimeOut){
	        		max_records++;
		        } // while
		        //cout << "�զ@" << max_records << "�� �öi��}�C��l��" << endl ;
		        inFile.close();
			}
		
		} // if
		JobList joblist(max_records, fileNum);
        AnsList anslist;
        Jobtype nextjob;
        Jobtype CPUtask;
        int alistleft=max_records;
        switch (cmd) {
            case 0: 
                break;
            case 1: 
	
                // Open file
                //timeStr = ctime(&before_open_Time)
    			//cout << "Current time: " << timeStr;
    			
    			//Ū�� 
    			reading_start_clock = clock();
    			successOpen = joblist.getAll();
    			reading_end_clock = clock();

 				if (successOpen) {
	 				//Ū��end
	    			//���cpu�u�@�Ƶ{ 
	    			joblist.showJob();
	    			//���cpu�u�@�Ƶ{ end
	    			//�ƧǤu�@�Ƶ{
	    			sort_start_clock = clock();
	    			joblist.getSorted();
	    			sort_end_clock = clock();
					//�ƧǤu�@�Ƶ{end
					//�g�X �ƧǤu�@�Ƶ{
					writing_start_clock = clock();
					joblist.putAll();
					writing_end_clock = clock();
					//�g�X �ƧǤu�@�Ƶ{end
	 				//******************************************************�L�X���q�ɶ�
					elapsed_reading_time_ms = double(reading_end_clock - reading_start_clock) / CLOCKS_PER_SEC * 1000;
					elapsed_sort_time_ms = double(sort_end_clock - sort_start_clock) / CLOCKS_PER_SEC * 1000;
					elapsed_writing_time_ms = double(writing_end_clock - writing_start_clock) / CLOCKS_PER_SEC * 1000;
					
	   				printf("%s %d %s (%.2f ms)\n", "Reading data:", (reading_end_clock - reading_start_clock) ,"clocks", elapsed_reading_time_ms);
	   				printf("%s %d %s (%.2f ms)\n", "Sorting data:", (sort_end_clock - sort_start_clock) ,"clocks", elapsed_sort_time_ms);
	   				printf("%s %d %s (%.2f ms)\n\n", "Writing data:", (writing_end_clock - writing_start_clock) ,"clocks", elapsed_writing_time_ms);
	   				
	   				cout << "See " << "sort" << fileNum <<  ".txt" << endl << endl;
	   			}
	   			break;
					//******************************************************�L�X���q�ɶ�end
				
			case 2:	 
				successOpen = joblist.getAll();
 				if (successOpen) {
	    			joblist.getSorted();
	    		}
	    			cout<<"Queue 1 ("<<CPUtime<<"):"<<endl;
					if(!Queue_3memb.isEmpty()){	
						Queue_3memb.printQueue();
					}
					anslist.showAbortJobs();
					anslist.showDoneJobs();
					anslist.showStat();
						
					nextjob=joblist.nextJob();
					
					if(nextjob.Arrival+nextjob.Duration>nextjob.TimeOut){
						if(nextjob.Arrival>nextjob.TimeOut){//���X��Abort
							anslist.addAbortJob( nextjob.OID, nextjob.Arrival , nextjob.Arrival-nextjob.Arrival );
							CPUtime=nextjob.TimeOut;
						}
						else{//�����@�bAbort 
							anslist.addAbortJob( nextjob.OID, nextjob.TimeOut , nextjob.TimeOut-nextjob.Arrival );
							CPUtime=nextjob.TimeOut;
						}
					} 
					else{
						anslist.addDoneJob(nextjob.OID, nextjob.Arrival+nextjob.Duration , nextjob.Arrival-nextjob.Arrival);
						CPUtime=nextjob.Arrival+nextjob.Duration;
					}
					
					//joblist.printnextJob();
					system("pause");
					
					do{
						if(alistleft==0){
							cout<<"Queue 1 ("<<CPUtime<<"):"<<endl;
							
							
								nextjob=Queue_3memb.deQueue();
								if(!Queue_3memb.isEmpty()){
								Queue_3memb.printQueue();	}
								if(CPUtime+nextjob.Duration>nextjob.TimeOut){
									if(CPUtime>nextjob.TimeOut){//���X��Abort 
										anslist.addAbortJob(nextjob.OID,CPUtime,CPUtime-nextjob.Arrival);
									} 
									else{//�����@�bAbort
										anslist.addAbortJob(nextjob.OID,nextjob.TimeOut,nextjob.TimeOut-nextjob.Arrival);
										CPUtime=nextjob.TimeOut;
									}
									
								} 
					
								/*if(CPUtime+nextjob.Duration<=nextjob.TimeOut){
									nextjob�i�H���槹 
								}*/
						
							anslist.showAbortJobs();
							anslist.showDoneJobs();
							anslist.showStat();
							system("pause");
						}
						else if(alistleft>0){
								cout<<"Queue 1 ("<<CPUtime<<"):"<<endl;
							if(!Queue_3memb.isEmpty()){	
								Queue_3memb.printQueue();
							}
							anslist.showAbortJobs();
							anslist.showDoneJobs();
							anslist.showStat();
							nextjob=joblist.nextJob();
							if(nextjob.Arrival<CPUtime){
								if(Queue_3memb.getqueueLength()<3){
									Queue_3memb.enQueue( nextjob.OID, nextjob.Arrival, nextjob.Duration, nextjob.TimeOut );							 
								} 
								else{
									anslist.addAbortJob(nextjob.OID,nextjob.Arrival,0); //Abort�]��Queue���F 
								} 	
							} 
							//joblist.printnextJob();
							--alistleft;
							system("pause");
						}
						
					}while((anslist.getAbortsize()+anslist.getDonesize())<joblist.getjoblistnum());
					
					anslist.computeStat();
					anslist.putAll(fileNum);
					
					
					//cout<<"***************************************************************************************************"<<endl;
					/*CPUtime = 0 ; // cpu ��l�ɨ�
					cout<<"Queue 1 ("<<CPUtime<<"):" << endl;  //  �C��cpu�ɨ�
					if(!Queue_3memb.isEmpty()){	// queue�����ŦL�Xqueue���e 
						Queue_3memb.printQueue();
					} // if
					
					anslist.showAbortJobs(); // ��ܥ��M�� 
					anslist.showDoneJobs(); // ��ܧ����M�� 
					anslist.computeStat(); // �p��ƭ� 
					anslist.showStat(); // ��ܲέp��� 
					system("pause");
					CPUtask.OID =0 ; // CPU ����
					cpuavil = false ;
					nextjob = joblist.nextJob(); // ���Ĥ@���u�@ 
					CPUtime = nextjob.Arrival+nextjob.Duration; // �u�@�@�X�Ӯɶ� 
					system("pause");
					if((nextjob.Arrival+nextjob.Duration) > nextjob.TimeOut) {
						anslist.addAbortJob( nextjob.OID, nextjob.TimeOut, 0 );
						
					}else {
						anslist.addDoneJob( nextjob.OID, CPUtime, 0 );
					}
					anslist.showAbortJobs(); // ��ܥ��M�� 
					anslist.showDoneJobs(); // ��ܧ����M�� 
					anslist.computeStat(); // �p��ƭ� 
					anslist.showStat(); // ��ܲέp��� 
					anslist.putAll(fileNum);
					system("pause");
					for(int x=0 ; x < max_records or ! Queue_3memb.isEmpty() ; x++ ) {  // ���Ȩ����Bqueue�M��
					
					
						cout<<"Queue 1 ("<<CPUtime<<"):" << endl;  //  �C��cpu�ɨ�
						if(!Queue_3memb.isEmpty()){	// queue�����ŦL�Xqueue���e 
							Queue_3memb.printQueue();
						} // if 
						
						 
						if(x < max_records) nextjob = joblist.nextJob();
						

						
						

//						anslist.showAbortJobs(); // ��ܥ��M�� 
//						anslist.showDoneJobs(); // ��ܧ����M�� 
//						anslist.computeStat(); // �p��ƭ� 
//						anslist.showStat(); // ��ܲέp��� 
					 
						//cout<<"*********************************************************************************************"<<endl;				
						system("pause");
					} // for
					*/
                
                break;
            default: 
                cout << "\n" << "Command does not exist! " << "\n" << "\n";
        }

    } while (cmd != 0);

    // Clean up allocated memory outside the loop
    //delete[] JobList;
    system("pause");

    return 0;
    
    
} // main()
    
