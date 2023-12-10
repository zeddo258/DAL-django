#include <iostream>
#include <fstream> // �ɮ�
#include <vector> // vector
#include <time.h> // �p��ɶ�
#include <sstream> // ���r��
#include <iomanip>

using namespace std ;

static string num; // �ɦW �Ʀr����
static string cmd;

struct Order { 	// �q��
	int oid = 0; // �q��s��
	int arrival = 0; // �U��ɨ�
	int duration = 0; // �s�@�Ӯ�
	int timeOut = 0; // �O�ɮɨ�
};
Order order ;

struct Abort { 	// �����q��
	int oid = 0; // �q��s��
	int abort = 0; // �����ɨ�
	int delay = 0; // ����Ӯ�
};
Abort abortjob ;

struct Done { 	// �����q��
	int oid = 0; // �q��s��
	int departure = 0; // �����ɨ�
	int delay = 0; // ����Ӯ�

};
Done done ;

vector <string> header; // ���ئW�� old�Aarrival�Aduration�A timeOut
vector <Order> data; // ���Ū�J��s��
vector <Order> cpu; // ���b������

bool keep = false;
string whichnum;

bool ReadTxt() ; // Ū��
void WriteFile() ; // �g��
void del() ; //�ܼƭ��s������O����Ŷ�

class Queue {
    typedef struct node{
        Order value;
        struct node *next;

    } node;

    node *head = NULL;
    node *tail = NULL;

    public:
        void enqueue( Order data ) {

            node *newnode;
            newnode = new node;
            newnode -> value = data;
            newnode -> next = NULL;

            //�Ĥ@�Ӹ�Ʀs�Jqueue
            if( head == NULL && tail == NULL ){
                head = newnode;
                tail = newnode;
            }

            //�D�Ĥ@�Ӹ�Ʀs�Jqueue
            else{
                tail -> next = newnode;
                tail = newnode;
            }
        }

        void dequeue() {
            Order result;
            //queue�O�_����
            if( head == NULL ){
                cpu.clear();
            }

            result = head -> value;
            node *temp = head;
            head = head -> next;
            if( head == NULL ){
                tail = NULL;
            }
            delete temp;
            cpu.push_back( result );
        }
};
Queue que;

class mission1 {
	public:
	    double readT = 0 , sortT = 0 , writeT = 0 ; // �ɶ�
		void ShellSort(); // �ƺ��Ƨ�
		void PrintData(); // �L�X���
		void PrintTime(); // �L�X�ɶ�
};
mission1 m1;

class mission2 {
    public:
        bool finish = false; // �|���Ҧ��u�@���槹
        bool datadone = false; // �|���Ҧ��u�@�U�槹��
        bool process = false; // cpu���椤

        vector <Abort> abortlist; // �����
        vector <Done> donelist; // �����q����

        int nowtime = 0; // �ثe�ɶ�
        int quesize = 0; // ��C�w���X��
        int cpubegin = 0; // ��cpu�}�l�ɶ�
        int whichdata = 0; // �ĴX���q��
        float donenums = 0; // �q�槹���ƶq
        float totaldelay = 0; // �`���~

        void logdata ( string x, int arrival, int which ); // �O��done��abort
        void newjob();
        void newque();
};
mission2 m2;

int main() {

	while ( true ) {

		cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
		cout << endl << "* 0. Quit                            *" ;
		cout << endl << "* 1. Sort a file                     *" ;
		cout << endl << "* 2. Simulate one FIFO queue         *" ;
		cout << endl << "**************************************" ;
		cout << endl << "Input a command(0, 1, 2): " ;
		cin >> cmd ;

        del();
		if ( cmd == "1" ) {
            keep = false;
			if ( ReadTxt() ) {
				m1.PrintData(); // �L�X��� (�L�Ƨ�)
				m1.ShellSort(); // �ƺ��Ƨ�
				WriteFile(); // �g��
				m1.PrintTime(); // �L�X�ɶ�

				cout << "See sorted"  << num << ".txt\n";

			} // if
			keep = true;
			whichnum = num;

		} // if(cmd=1)
		else if ( cmd == "2" ){

            if( ReadTxt() ) {
                m2.nowtime = data[0].arrival ;
                while (  m2.finish == false   ) {
                    //cpu�O�_���B�z���q��
                    if ( m2.process == true ) {
                        if ( m2.nowtime-m2.cpubegin == cpu[0].duration ) { // �ӭq��B�z����

                            m2.logdata( "done", cpu[0].arrival, 0 ); // �O����donelist
                            cpu.pop_back();
                            m2.process = false;
                            m2.newjob(); // �q��C����s�u�@

                        }
                        else if ( m2.nowtime == cpu[0].timeOut ) { // �ӭq��O��
                            m2.logdata( "abort", cpu[0].arrival, 0 ); // �O����adortlist
                            cpu.pop_back();
                            m2.process = false;
                            m2.newjob(); // �q��C����s�u�@

                        } //else if
                    } //if
                    else {
                        m2.newjob(); // �q��C����s�u�@
                    } //else

                    //�O�_���s�q��
                    m2.newque();
                    //�O�_�B�z���������q��
                    if ( m2.datadone == true && m2.quesize == 0 && m2.process == false ){
                        m2.finish=true;
                    }

                    m2.nowtime++;
                } //while

                WriteFile();
                cout << "The simulation is running...\nSee output"  << num << ".txt\n";
                keep = true;
                whichnum = num;
            }

		} //elif(cmd=2)

		else if ( cmd == "0" )
		    return 0;

		else
		    cout << endl << "Command does not exist!" << endl ;
	} // while
} // main()

void del(){
    header.clear() ; // ����O����Ŷ�
    data.clear() ; // ����O����Ŷ�
    cpu.clear() ;
    m2.totaldelay = 0;
    m2.donenums = 0;
    m2.abortlist.clear() ;
    m2.donelist.clear() ;
    m2.finish = false;
    m2.datadone = false;
    m2.process = false;
    m2.nowtime = 0;
    m2.quesize = 0;
    m2.cpubegin = 0;
    m2.whichdata = 0;
}

// Ū��
bool ReadTxt() {
	double start, end ;

	if( keep == false ){
        cout << "Input a file number (e.g., 301, 302, 303, ...): " ;
        cin >> num ;
        cout << endl ;
	}
	else{
        num = whichnum;
        cout << endl ;
	}

	string name ;
	if ( cmd == "1" )
	    name = "input" + num + ".txt" ;

	else if ( cmd == "2"  )
	    name = "sorted" + num + ".txt" ;

	ifstream file ;
	file.open(name, ios_base::in) ; // ��󥴶}

    if (!file.is_open()) {
        cout << "### " << name << " does not exist! ###\n" ;
        return false ;
    } // if

    else {
    	start = clock() ;
    	int temp, count ;
    	string line ;

    	getline( file, line ) ;
    	header.push_back(line) ; // Ū�J���ئW

    	while( getline( file, line ) ) { // Ū�J���
    		istringstream stream(line) ;
    		count = 0 ;
    		while( stream >> temp ) {

    			if ( count == 0 )
				    order.oid = temp ;

    			else if ( count == 1 )
				    order.arrival = temp ;

    			else if ( count == 2 )
				    order.duration = temp ;

    			else if ( count == 3 )
				    order.timeOut = temp ;

    			count++ ;
			} // while

			data.push_back(order);
			stream.clear() ;
		} // while

		file.close() ; // �������

		end = clock() ;
		m1.readT = end - start ;

		return true;
	} // else
} // ReadTxt()

// �g��
void WriteFile() {
    if( cmd == "1" ) {
        double start = clock() ;
        ofstream newFile ; // Create a new file
        string name = "sorted" + num + ".txt" ;
        newFile.open(name, ios_base::out) ; // ��󥴶}
        for ( int i = 0 ; i < header.size() ; i++ )  // �g�J���ئW
            if ( i == 0 )
                newFile << header[i] ;

        newFile << endl ;

        for ( int i = 0 ; i < data.size() ; i++ )   // �g�J���
            newFile << data[i].oid << '\t' << data[i].arrival << '\t' << data[i].duration << '\t' << data[i].timeOut << endl;

        double end = clock() ;
        m1.writeT = end - start ;
    }
    else if( cmd == "2" ) {
        ofstream newFile ; // Create a new file
        string name = "output" + num + ".txt" ;
        newFile.open(name, ios_base::out) ; // ��󥴶}

        newFile << "\t[Abort Jobs]\n\tOID\tAbort\tDelay\n" ;// �g�J���ئW

        for ( int i = 0 ; i < m2.abortlist.size() ; i++ ) {   // �g�J���
            newFile << "[" << i+1 << "]\t" << m2.abortlist[i].oid << '\t' << m2.abortlist[i].abort << '\t' << m2.abortlist[i].delay << endl;
        }
        newFile << "\t[Jobs Done]\n\tOID\tDeparture\tDelay\n" ;
        for ( int i = 0 ; i < m2.donelist.size() ; i++ ) {   // �g�J���
            newFile << "[" << i+1 << "]\t" << m2.donelist[i].oid << '\t' << m2.donelist[i].departure << '\t' << m2.donelist[i].delay << endl;
        }

        newFile << "[Average Delay]\t" << fixed  <<  setprecision(2) << m2.totaldelay/data.size() << " ms" << endl;
        newFile << "[Success Rate]\t" << fixed  <<  setprecision(2) << m2.donenums*100/data.size() << " %" <<endl;
    }

} // Writefile()

// �ƺ��Ƨ�
void mission1::ShellSort() {
	double start = clock() ;

	for ( int h = data.size() / 2 ; h > 0 ; h = h / 2 ) { // h�N���j��
		for ( int unsorted = h ; unsorted < data.size() ; unsorted++ ) {
			for ( int loc = unsorted ; loc >= h ; loc = loc - h ) {
				if ( data[loc-h].arrival > data[loc].arrival )
					swap( data[loc-h], data[loc] ) ;

				else if ( data[loc-h].arrival == data[loc].arrival )
					if ( data[loc-h].oid > data[loc].oid )
						swap( data[loc-h], data[loc] ) ;
			} // for
		} // for
	} // for

	double end = clock() ;
	sortT = end - start ;
} // ShellSort()

// �L�X���
void mission1::PrintData() {
    for ( int i = 0 ; i < header.size() ; i++ ) { // �L�X���ئW
    	cout << '\t' << header[i] ;
	} // for
	cout << endl ;

	for ( int i = 0 ; i < data.size() ; i++ ) // �L�X���
		cout << "(" << i+1 << ")" << '\t' << data[i].oid << '\t' << data[i].arrival << '\t' << data[i].duration << '\t' << data[i].timeOut << endl;
} // Printdata()

// �L�X�ɶ�
void mission1::PrintTime() {
	cout << endl ;
	cout << "Reading data:\t" << readT << " clocks(" << readT << ".00 ms).\n" ;
	cout << "Sorting data:\t" << sortT << " clocks(" << sortT << ".00 ms).\n" ;
	cout << "Writing data:\t" << writeT << " clocks(" << writeT << ".00 ms).\n" << endl ;
} // Printtime()

//�s���q�浲�G
void mission2::logdata ( string x , int arrival , int which ){

    if ( x=="done" ) {
        done.oid = cpu[0].oid;
        done.departure = nowtime;
        done.delay = cpubegin-arrival;
        donelist.push_back(done);
        donenums++;
        totaldelay=totaldelay+cpubegin-arrival;
    }
    else if( x=="abort" ){
        if ( which == 0 ) {
            abortjob.oid = cpu[0].oid;
        }
        else {
            abortjob.oid = data[whichdata].oid;
        }

        abortjob.abort = nowtime;
        abortjob.delay = nowtime-arrival;
        abortlist.push_back(abortjob);
        totaldelay=totaldelay+nowtime-arrival;
    }
}

//����U�@�q��
void mission2::newjob() {

    cpubegin = nowtime;
    while( quesize > 0 ) {//�O�_����C
        que.dequeue();
        quesize--;
        //�s�u�@�O�_����
        if( nowtime < cpu[0].timeOut ) {
            process = true;
            break;
        }
        else {
            logdata( "abort", cpu[0].arrival, 0 );
            cpu.pop_back();
        }

    }//while

}

//����s�q��
void mission2::newque(){
    while ( nowtime == data[whichdata].arrival && datadone == false ) {

        if ( process == false && quesize==0 ) {//��C��cpu�ҨS�F��
            cpu.push_back( data[whichdata] );
            cpubegin = nowtime;
            process = true;
        }
        else if ( quesize < 3 ) {//��C����
            que.enqueue( data[whichdata] );
            quesize++;
        }
        else if ( quesize >= 3 ) {//����Һ�
            logdata( "abort", data[whichdata].arrival, 1 );
        }

        if( whichdata<data.size()-1 ){
            whichdata++;
        }
        else{
            datadone = true;
        }
    }
}
