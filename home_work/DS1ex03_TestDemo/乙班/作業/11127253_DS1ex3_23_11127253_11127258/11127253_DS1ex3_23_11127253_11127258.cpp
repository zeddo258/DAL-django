// 11127253�L�y�� 11127258²�\�[
# include <string>
# include <iostream>
# include <vector>
# include <fstream>
# include <cctype> // isdigit()
# include <ctime>  // �����
# include <cstdlib> // atoi
#include <iomanip> // �p���I����

using namespace std;

struct Data{
    int OID ;
    int arr ; // Arrival
    int dur ; // Duration
    int t_out ; // TimeOut
};

struct CPU_Data{
    int OID ;
    int time ; // departure or abort
    int delay ;
};

// Ū�ɰ�
void findData( vector <Data> &v_data, string line ) ; // ���getline���@�檺��ơA���x�s��vector
bool input_data( vector <Data> &v_data, string &fileName, string &outputName, int command ); // Ū��

// command1 funtion
void sort_data( vector <Data> &s_data ) ; // shell sort
void printfData( vector <Data> v_data, string first_line ) ; // command 1 print
void sortput( vector <Data> v_data, string first_line, string sortName  );  // command 1 �g��

// command2 funtion
void CPU_modeling( vector <Data> s_data, vector <CPU_Data> &done_data, vector <CPU_Data> &abort_data ) ;
void output( vector <CPU_Data> done_data, vector <CPU_Data> abort_data, string outputName ); // command 2 �g��
void input_list( int time, Data data, vector <CPU_Data> &list_data, int type ) ; // �N��Ʀs�Jdone&abort_data

class Queue {
	// �D��2�̦h�s�T��
	vector <Data> q_data ;

	public:
		// q_data.begin()=0�A�g�o�ˬO��ı�W����ŦXq

		bool isEmpty(){ // q�O�_����
			return q_data.empty() ;
		} // isEmpty

		void enqueue( Data data ){ // push���
			q_data.push_back( data ) ;
		} // enqueue()

		void makeEmpty(){ // �M��q
			q_data.clear() ;
		} // makeEmpty()

		bool IsFull(){
			if ( q_data.size() == 3 )
				return true ;
			else
				return false ;
		} // IsFull()

		Data dequeue(){ // �ᱼ�̫e�����A�P�ɦ^��
			Data Data_temp = getFront() ;
			q_data.erase(q_data.begin()) ;
			return Data_temp ;
		} // equeue()

		Data getFront(){ // ���o�̫e����
			return q_data[0] ;
		} // getFront()

};

int main() {
    int command = -1 ; // �Ҧ�
    float Rtime = 0 ; // read_time
    float Stime = 0 ; // sort_time
    float Wtime = 0 ; // write_time
    string first_line = "OID	Arrival	Duration	TimeOut" ;
    string fileName = "input000.txt"; // file���W�l
    string sortName = "sorted000.txt"; // file���W�l
    string outputName = "output000.txt"; // file���W�l
    string stemp_num ;  // �Ȧs�ܼ�
    vector <Data> v_data ; // data of vector
    vector <Data> s_data ; // sort of data
    clock_t startTime, endTime ;
    //  command2 CPU data
    vector <CPU_Data> done_cup_data ;
    vector <CPU_Data> abort_cup_data ;
    do {
        cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
        cout << endl << "* 0. Quit                            *" ;
        cout << endl << "* 1. Sort a file                     *" ;
        cout << endl << "* 2. Simulate one FIFO queue         *" ;
        cout << endl << "**************************************" ;
        cout << endl << "Input a command(0, 1, 2): " ;
        cin >> command ;

       if ( command > 2 || command < 0 ){
           cout << endl << "Command does not exist!" << endl;
       }
       else if (command != 0) {

            bool Exit ;

            if ( command == 1 ){

                startTime = clock();
                Exit = input_data( v_data, fileName, sortName, 1 ) ; // Ū��input
                endTime = clock();
                Rtime = (float)(endTime - startTime) *1000/ CLOCKS_PER_SEC ; // Ū�ɮɶ� ms

                if ( Exit ){

                    s_data = v_data ;

                 	startTime = clock();
				    sort_data( s_data ) ; // �Ƨ�
				    endTime = clock();
				    Stime = (float)(endTime - startTime) *1000/ CLOCKS_PER_SEC ; // �ƧǮɶ�

                    startTime = clock();
                    sortput(s_data, first_line, sortName) ; // �g��sorted
                    endTime = clock();
					Wtime = (float)(endTime - startTime) *1000/ CLOCKS_PER_SEC ; ; // �g�ɮɶ�

                    // ��X
                    printfData( v_data, first_line ) ;

					cout << "Reading data: " << Rtime << " clocks ("<< fixed << setprecision(2) << Rtime << " ms)." << endl ; // ����ɶ�
					cout << "Sorting data: " << Stime << " clocks ("<< fixed << setprecision(2) << Stime << " ms)." << endl ;
					cout << "Writing data: " << Wtime << " clocks ("<< fixed << setprecision(2) << Wtime << " ms)." << endl ;
                    cout << endl << "See "<< sortName << endl ;

                } // if()
            } // if()
            else if ( command == 2 ){ // ���ȤG

                Exit = input_data( s_data, sortName, outputName, 2 ) ; // Ū��sorted
                if ( Exit ){
                    CPU_modeling( s_data, done_cup_data, abort_cup_data ) ;
                    output( done_cup_data, abort_cup_data, outputName  ); // �g��output 
                    cout << endl << "The simulation is running..." << endl << "See " << outputName << endl ;
                }
            } // else if

        } // else if

        // reset�ܼ�
        v_data.clear() ;// �M��vdata
        s_data.clear() ;  // �M��sdata
        done_cup_data.clear() ;
        abort_cup_data.clear() ;
        stemp_num = "" ;
    } while( command != 0 );

    return 0;
}

void findData( vector <Data> &v_data, string line ){// ���getline���@�檺��ơA���x�s��vector

    int num = 1 ;// Ū��ĴX�Ӹ��(OID,arr,dur,tOut)
    string stemp_num = "" ; // �ȩ��ƪ��a��
    Data temp_data ; // �ȩ�Data���a��
    for( int i = 0 ; i < line.length() ; i++ ){
            if ( isdigit(line[i]) ){
                stemp_num.push_back(line[i]) ;
                i++ ;
                if ( i == line.length() ){
                    break ; // �]��i=�r��lengh���סA�������X�o��for�j��� // �]�i�H��if else�A�u�O�o�˼g����n��{
                }

                while( isdigit(line[i]) != false ){ // �֥[�Ʀr
                    stemp_num.push_back(line[i]) ;
                    i++ ;
                }

                // �ݬO�n�s���@�����
                if ( num == 1 ){
                    temp_data.OID = atoi( stemp_num.c_str() ) ;
                }
                else if ( num == 2 ){
                    temp_data.arr = atoi( stemp_num.c_str() ) ;
                }
                else if ( num == 3 ){
                    temp_data.dur = atoi( stemp_num.c_str() ) ;
                }
                else if ( num == 4 ){
                    temp_data.t_out = atoi( stemp_num.c_str() ) ;
                }

                stemp_num = "" ; // reset stemp_num
                num++ ; // �Ĥ@�����Ū��Ū�ĤG�Ӹ�ƥH������

            } // Ū��Ʀr

    } // for()

    v_data.push_back(temp_data) ;
} // findData

void sortput( vector <Data> v_data, string first_line, string sortName  ) {

    ofstream outfile (sortName.c_str());

    outfile << first_line << endl;

    for( int i = 0 ; i < v_data.size() ; i++ ){
        for ( int j = 1 ; j <= 4 ; j++ ){
            if ( j == 4 )
                outfile << v_data[i].t_out << endl;
            else if ( j == 1 )
                outfile << v_data[i].OID << "	";
            else if ( j == 2 )
                outfile << v_data[i].arr << "	";
            else if ( j == 3 )
                outfile << v_data[i].dur << "	";

        } // for
    }// for

    outfile.close() ;
} // sortput

void output( vector <CPU_Data> done_data, vector <CPU_Data> abort_data, string outputName  ){

    float ave_delay = 0.00 ; // Average Delay
    float suc_rate = 0.00 ; // Success Rate

    ofstream outfile (outputName.c_str());

    outfile << "	[Abort Jobs]" << endl;
    outfile << "	OID	Abort	Delay" << endl ;

    for( int i = 0 ; i < abort_data.size() ; i++ ){
        ave_delay = ave_delay + (float)abort_data[i].delay ;
        outfile << "[" << i+1 << "]" ;
        for ( int j = 1 ; j <= 3 ; j++ ){
            if ( j == 1 )
                outfile << "	" << abort_data[i].OID;
            else if ( j == 2 )
                outfile << "	" << abort_data[i].time;
            else if ( j == 3 )
                outfile << "	" << abort_data[i].delay << endl ;
        } // for
    }// for

    outfile << "	[Jobs Done]" << endl;
    outfile << "	OID	Departure	Delay" << endl ;

    for( int i = 0 ; i < done_data.size() ; i++ ){
    	ave_delay = ave_delay + (float)done_data[i].delay ;
        outfile <<  "[" << i+1 << "]" ;
        for ( int j = 1 ; j <= 3 ; j++ ){
            if ( j == 1 )
                outfile << "	" << done_data[i].OID;
            else if ( j == 2 )
                outfile << "	" << done_data[i].time;
            else if ( j == 3 )
                outfile << "	" << done_data[i].delay << endl ;
        } // for
    }// for

	ave_delay = ave_delay / ( done_data.size() + abort_data.size() ) ;
    suc_rate = 100.00 *done_data.size() / ( done_data.size() + abort_data.size() ) ;

    outfile << "[Average Delay]	" << fixed << setprecision(2) << ave_delay << " ms" << endl;
    outfile << "[Success Rate]	" << fixed << setprecision(2) << suc_rate << " %" << endl ;
    outfile.close() ;

} // output

void printfData( vector <Data> v_data, string first_line) {

    cout << endl << "        " << first_line << endl ;

    for( int i = 0 ; i < v_data.size() ; i++ ){

        cout << "(" << i+1 << ")	" ;

        for ( int j = 1 ; j <= 4 ; j++ ){

            // ��X���
            if ( j == 4 )
                cout << v_data[i].t_out << endl;
            else if ( j == 1 )
                cout << v_data[i].OID << "	";
            else if ( j == 2 )
                cout << v_data[i].arr << "	";
            else if ( j == 3 )
                cout << v_data[i].dur << "	";

        } // for
    }// for

    cout << "" << endl ;
} //printfData

bool input_data( vector <Data> &v_data, string &fileName, string &outputName, int command ) {
    string stemp_num ; //�Ȧs�ܼ�

    cout << endl << "Input a file number: ";
    cin >> stemp_num ;
    fileName = "input" + stemp_num + ".txt";
    string name = "" ;
    ifstream in;


    if ( command == 2 ) {
        fileName = "sorted" + stemp_num + ".txt";
        outputName = "output" + stemp_num + ".txt";
        in.open(fileName .c_str());
    } // if
    else { // command == 1
        fileName = "input" + stemp_num + ".txt";
        outputName = "sorted" + stemp_num + ".txt";
        in.open(fileName.c_str());
    } // else


    if (in.fail()) {
        cout << "\n### "<< fileName << " does not exist! ###" ;
        cout << "" << endl ;
        return false ;
    } // if

    else {
        string line ;
        while( getline(in, line) ){
            if ( line[0] == 'O' ){ // �Ĥ@��~��
                getline(in, line) ;
            } // if

            findData( v_data, line ) ;
        } // while

        in.close(); // �������
        return true ;
    } // else

	return true ;
} // input_data

void sort_data(vector<Data> &s_data) {

	for (int len = s_data.size()/2; len > 0; len = len/2) { // �ƧǶ��jlen
		for (int i = len; i < s_data.size(); i++) { // insert sort
			for (int j = i-len; j >= 0; j = j-len) { // j = i�e�@�� // �C��Ƨ� 
				if(s_data[j].arr > s_data[j+len].arr) {
           			swap(s_data[j], s_data[j+len]);
           		}
           		else if(s_data[j].arr == s_data[j+len].arr && s_data[j].OID > s_data[j+len].OID) {
       				swap(s_data[j], s_data[j+len]);
      			}
			}
		}
	}

} // sort_data

void CPU_modeling( vector <Data> s_data, vector <CPU_Data> &done_data, vector <CPU_Data> &abort_data ){
	Queue q ;
    int time = 0 ; // �{�b�B��h�[
    int cpu_time = 0 ; // CPU���浲���ɶ�
    Data temp ;
    CPU_Data CPU_temp ;
    bool cpu_state = false ; // true=����; false=����

	// �B�zdata
	while (!s_data.empty()) {
		if (cpu_time == time && cpu_state == true) { // �٭�cpu
			cpu_state = false ;
		}
		if (cpu_state == false && !q.isEmpty()) { // cpu�B�z // ��zq
			while (!q.isEmpty() && q.getFront().t_out <= time) { // ���X�O��(4.2)
				temp = q.dequeue() ; // ����q
				input_list(time, temp, abort_data, 2) ;
			}
			if (!q.isEmpty()) { // �B�z�U�@q
				temp = q.dequeue() ;
				cpu_state = true ;
				if (temp.dur + time > temp.t_out) { // ����O��(4.3)
					input_list(time, temp, abort_data, 3) ;
					cpu_time = temp.t_out ; // cpu�����ɶ�
				}
				else {
					input_list(time, temp, done_data, 0) ;
					cpu_time = temp.dur + time ; // cpu�����ɶ�
				}
			}
		}

		if (time == s_data[0].arr) {
			if (!q.IsFull()) { // q < 3
				q.enqueue(s_data[0]) ; // ��Jq
				s_data.erase(s_data.begin()) ; // ����first data
			}
			else { // q isFull(4.1)
				temp = s_data[0] ;
				s_data.erase(s_data.begin()) ; // ����first data
				input_list(time, temp, abort_data, 1) ;
			}
		}
		else {
			time++ ;
		}

	}

	// �B�z�Ѿlq
	while (!q.isEmpty()) {
		time = cpu_time ;
		cpu_state == false ;
		while (!q.isEmpty() && q.getFront().t_out <= time) { // ���X�O��(4.2)
			temp = q.dequeue() ;
			input_list(time, temp, abort_data, 2) ;
		}
		if (!q.isEmpty()) { // �B�z�U�@q
			temp = q.dequeue() ;
			cpu_state = true ;
			if (temp.dur + time > temp.t_out) { // ����O��(4.3)
				input_list(time, temp, abort_data, 3) ;
				cpu_time = temp.t_out ; // cpu�����ɶ�
			}
			else {
				input_list(time, temp, done_data, 0) ;
				cpu_time = temp.dur + time ; // cpu�����ɶ�
			}
		}
	}
} // CPU_modeling

void input_list( int time, Data data, vector <CPU_Data> &list_data, int type ) {
	CPU_Data CPU_temp ;
	
	if (type == 0) { // ����
		CPU_temp.OID = data.OID ;
		CPU_temp.time = time + data.dur ;
		CPU_temp.delay = time - data.arr ;
		list_data.push_back(CPU_temp) ; // �����M��
	}
	else if (type == 1) { // 4.1
		CPU_temp.OID = data.OID ;
		CPU_temp.time = time ;
		CPU_temp.delay = 0 ;
		list_data.push_back(CPU_temp) ; // �����M��
	}
	else if (type == 2) { // 4.2
		CPU_temp.OID = data.OID ;
		CPU_temp.time = time ;
		CPU_temp.delay = CPU_temp.time - data.arr ;
		list_data.push_back(CPU_temp) ; // �����M��
	}
	else if (type == 3) { // 4.3
		CPU_temp.OID = data.OID ;
		CPU_temp.time = data.t_out ;
		CPU_temp.delay = CPU_temp.time - data.arr ;
		list_data.push_back(CPU_temp) ; // �����M��
	}
} // input_list



