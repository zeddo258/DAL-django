// 11027261 林書偉 11027231 謝家榆
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<math.h>
#include<string.h>
#include<fstream>
#include<vector>
#include<ctime> 
#include<iomanip>

#define MAX 3

using namespace std ;
struct Node
{
    int OID, Arrival, Duration, TimeOut ; 
};
typedef Node* Nodeptr ;
struct DataNode{
    int OID ;
    int Arrival ;
    int Duration;
    int Timeout;
    int Start ;
    int FT; 
    // recording when the Node will leave the queue
    // include you are stucked in the queue the item Timeout
    // and your Timeout
    int delay ;// if the job is finish it can be used
    bool possibleDone ;
    // if possibleDone is true it means it can be donelist
    bool possibleOut ;
    // if possibleOut is true it means it could be put in the abortlist
    DataNode *next;
    DataNode( DataNode* prev,  Node now ){
        next = NULL ;
        OID = now.OID;
        Arrival = now.Arrival;
        Duration = now.Duration;
        Timeout = now.TimeOut ;
        if ( prev != NULL ){
            Start = prev->FT;
            FT = Start + Duration ;// when will your finish the job
            if ( Timeout < FT ){
                if ( prev -> FT > now.TimeOut){
                    FT = prev->FT;
                }
                else
                    FT = Timeout ;
                // if it is impossible to be done
                // the FT can be set to Timout it means when the element will go to the abortlist
                possibleDone = false;
                possibleOut = true;
            }
            else{
                possibleDone = true;
                possibleOut = false ;
            }
            if ( possibleDone )
                delay = Start - Arrival ;
            else{
                if ( prev -> FT > now.TimeOut)
                    delay = prev -> FT - now.Arrival ;
                else
                    delay = Timeout- Arrival;
            }
        }
        else{
            delay = 0 ;
            Start = Arrival;
            FT = Duration + Arrival ;
            possibleDone = true ;
            possibleOut = false ;
        }

    }
};

typedef DataNode* Dataptr ;
struct Queue 
{
    Dataptr head , tail;
    double size;
    Queue(){
        
        size = 0 ;
        head = NULL ;
        tail = NULL ;
    }
    
    void Inqueue( Dataptr item ){
        if (size == 0 ){
            head = item ;
            tail = head ;
        }
        else{
            tail -> next = item;
            tail = item ;
        }
       
        size++;
    }
    Dataptr dequeue(){
        Dataptr temp  = head ;
        head = head -> next ;
        temp -> next = NULL ;
        size-- ;
        return temp ;
    }
};

class list 
{
    public:
	ifstream in;
	ofstream out;
	int read = 0, sort = 0, write = 0;
    string filename;
    bool open = 1;
    vector<Node> vec ; // 儲存一個.txt檔 
    
	void readin( string filename ) {
		Node temp; // 儲存一行 
		int num;
		string str;
		in.open( "input" + filename + ".txt") ;
        if ( !in.is_open()){
            cout << " ### input" + filename + ".txt does not exist! ### " << endl;
            open = 0;
            return ;
        }
		read = clock();	
		getline( in, str ); // 讀取第一行OID... 
		while ( in >> num ) { // 讀取資料 
			temp.OID = num;
			in >> temp.Arrival;
			in >> temp.Duration;
			in >> temp.TimeOut;
			vec.push_back(temp);
		}
		in.close();
		read = (clock() - read) / CLOCKS_PER_SEC ;
	}
	
	void output( string filename ){ // 輸出成sorted.txt 
		out.open( "sorted"+ filename + ".txt") ;
		write = clock();
 		out << "OID\tArrival\tDuration\tTimeOut" << endl;
		for ( int i = 0; i < vec.size(); i++ ) {
			out << left << vec[i].OID;
			out << left << '\t' << vec[i].Arrival;
			out << left << '\t' << vec[i].Duration;
			out << left << '\t' << vec[i].TimeOut << endl;
		}
		
		out.close();
		write = (clock() - write) / CLOCKS_PER_SEC;
	}
	
    void shellSort(vector<Node> &vec){
        Node item ;
        sort = clock();
        for ( int gap = vec.size() / 2 ; gap > 0 ; gap = gap / 2 ){
            for ( int start = gap ; start < vec.size(); start++){
                int goalArrival = vec[start].Arrival ;
                item = vec[start];
                int loc = start ;
                int i = 0 ; 
                for ( ; (loc >= gap) && (vec[loc-gap].Arrival > goalArrival ) ; loc = loc - gap ){
                    if ( vec[loc-gap].Arrival > goalArrival ){
                        
                        vec[loc].Arrival = vec[loc-gap].Arrival;
                        vec[loc].Duration = vec[loc-gap].Duration ;
                        vec[loc].OID = vec[loc-gap].OID ;
                        vec[loc].TimeOut  = vec[loc-gap].TimeOut;
                       
                    }
                }
                vec[loc] = item ;
                for (; (loc >= gap ) && vec[loc-gap].Arrival == goalArrival; loc = loc - gap ){
                    if ( vec[loc-gap].OID > item.OID ){
                        swap( vec[loc].Arrival,vec[loc-gap].Arrival);
                        swap( vec[loc].Duration,vec[loc-gap].Duration);
                        swap( vec[loc].OID,vec[loc-gap].OID);
                        swap( vec[loc].TimeOut,vec[loc-gap].TimeOut);                      
                    }
                }
                
                 
            }
        }
        
        sort = (clock()- sort) / CLOCKS_PER_SEC;
    }
    
	void showTime() { // 顯示時間 
		cout << endl << "Reading data: " << read << " clocks (" << read << ".00 ms)." ;
		cout << endl << "Sorting data: " << sort << " clocks (" << sort << ".00 ms)." ;
		cout << endl << "Writing data: " << write << " clocks (" << write << ".00 ms)." << endl;
	}
	
	void showAll() { // 顯示還沒排序過的input 
		
		cout << endl << "   \tOID\tArrival\tDuration\tTimeOut";
		for ( int i = 0; i < vec.size(); i++ ) {
			cout << endl << "(" << i+1 << ")\t" << left << setw(6) << vec[i].OID;
			cout << left << '\t' << vec[i].Arrival;
			cout << left << '\t' << vec[i].Duration;
			cout << left << '\t' << vec[i].TimeOut;
		}
		
		cout << endl;
		
	}
    
    void mission1( ) {
    	cout << endl << "Input a file number:" ;
    	cin >> filename;
    	readin(filename);
    	if (open) {
    		showAll();
    		shellSort(vec);
    		output(filename);
    		showTime();
    		cout << endl << "See sorted" << filename << ".txt" << endl;
		}
		open = 1;
	}
    void mission2( ){
        cout << "The simulation is running....." << endl ;
        cout << "See output"<<filename<<".txt"<<endl;
        string temp ;
        ifstream ifs ;
        Queue mainlist; //  index = 0 means doing
        Queue donelist, abortlist ;
        ofstream ofs( "output" + filename + ".txt");
        ifs.open("sorted"+ filename + ".txt");
        ifs >> temp ;
        ifs >> temp ;
        ifs >> temp ;
        ifs >> temp ;
        // these 4 lines are used to read trash
        vector<Node> vec ;
        while ( ifs >> temp ){
            // read the data from the sorted.txt 
            // keep it via a vector
            Node item ;
            item.OID = stoi(temp);
            ifs >> temp ;
            item.Arrival = stoi(temp);
            ifs >> temp ;
            item.Duration = stoi(temp);
            ifs >> temp ;
            item.TimeOut = stoi(temp);
            vec.push_back(item);
        }
        for ( int i = 0 ; i < vec.size(); i++ ){
            if ( mainlist.size == 0 ){
                Dataptr temp = new DataNode(NULL, vec[i]) ;
                mainlist.Inqueue(temp);
            }
            else if ( mainlist.size <= 4) {
                Dataptr temp = mainlist.head ;
                int index = 0 ;
                for (; index < mainlist.size&& vec[i].Arrival >= temp -> FT ; index++){
                    temp = temp -> next ;
                }
                if ( index == 0 && mainlist.size == 4){
                    Dataptr temp2 = new DataNode(NULL, vec[i]) ;
                    abortlist.Inqueue(temp2);
                }
                else {
                    for ( int k = 0 ; k < index ; k++){
                    
                        if ( mainlist.head->possibleDone){
                            Dataptr temp1 = mainlist.dequeue(); 
                            donelist.Inqueue(temp1);
                        }
                        else{
                            Dataptr temp1 = mainlist.dequeue(); 
                            abortlist.Inqueue(temp1);
                        }
                    }
                    if ( mainlist.size == 0){
                        Dataptr temp2 = new DataNode(NULL, vec[i]) ;
                        mainlist.Inqueue(temp2);
                    }
                    else{
                        Dataptr temp2 = new DataNode(mainlist.tail, vec[i]) ;
                        mainlist.Inqueue(temp2);
                    }
                }
            }
        
        }

        while (  mainlist.size!= 0){
            if ( mainlist.head->possibleDone){
                Dataptr temp1 = mainlist.dequeue(); 
                donelist.Inqueue(temp1);
            }
            else{
                Dataptr temp1 = mainlist.dequeue(); 
                abortlist.Inqueue(temp1);
            }
        }
        ofs << "\t[Abort Jobs]" << endl ;
        ofs << "\tOID\tAbort\tDelay" << endl;
        Dataptr tempp = abortlist.head ;
        for ( int i = 0 ; i< abortlist.size; i++ ){
            ofs <<  "[" << i+1 << "]\t" << tempp ->OID << "\t";
            if( tempp -> possibleDone ){
                ofs  <<tempp-> Arrival << "\t" << tempp -> delay << endl ;
            }
            else{
                ofs << tempp->FT << "\t"<< tempp ->delay << endl ;
            }
            tempp = tempp -> next ;
        }
        ofs << "\t[Done Jobs]" << endl ;
        ofs << "\tOID\tDeparture\tDelay" << endl;
        Dataptr tempp2 = donelist.head;
        for ( int i = 0 ; i< donelist.size; i++ ){
            ofs <<  "[" << i+1 << "]\t" << tempp2 ->OID << "\t";
            ofs <<  tempp2 -> FT << "\t"  <<tempp2 -> delay ;
            ofs << endl ;
            tempp2 = tempp2 -> next ;
        }
        double delay = 0 ;
        for ( int i = 0 ; i< donelist.size ; i++ ){
            delay = donelist.head->delay + delay ;
            donelist.head = donelist .head -> next ;
        }
        for ( int j = 0 ; j < abortlist.size; j++ ){
            delay = abortlist.head -> delay + delay;
            abortlist.head = abortlist.head -> next;
        }
        double pp = donelist.size / (donelist.size + abortlist.size) * 100 ;
        double dd = delay /( donelist.size + abortlist.size );
        ofs << "[Average Delay]\t" <<  dd << "  ms " << endl ;
        ofs << "[Success Rate]\t" << pp << " %";
    }

};

int main(void) { 	
    int command = 0; // user command
    list sort;
 	do
	{ 	
 		cout << endl << "**** Simulate FIFO Queues by SQF *****";
 		cout << endl << "* 0. Quit                            *";
 		cout << endl << "* 1. Sort a file                     *";
 		cout << endl << "* 2. Simulate one FIFO queue         *";
 		cout << endl << "**************************************";
		cout << endl << "Input a command(0, 1, 2): ";
 		cin >> command; // get the command
 		switch (command)
		{ 	case 0: break;
		 
 			case 1: sort.mission1();
 					break;
            case 2: sort.mission2();

                    break;
 					
 			
 					
 			default: cout << endl << "Command does not exist!" << endl;  
		} 
	} while (command != 0); 
 	return 0;
}
