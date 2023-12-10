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
    int CID ;
    int time ; // departure or abort
    int delay ;
};

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

		int q_size(){
		    return q_data.size() ;
		}

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

// Ū�ɰ�
void findData( vector <Data> &v_data, string line ) ; // ���getline���@�檺��ơA���x�s��vector
bool input_data( vector <Data> &v_data, string &fileName, string &outputName, int command ); // Ū��

// command1 funtion
void sort_data( vector <Data> &s_data ) ; // shell sort
void printfData( vector <Data> v_data, string first_line ) ; // command 1 print
void sortput( vector <Data> v_data, string first_line, string sortName );  // command 1 �g��

// command2 funtion
void CPU_modeling( vector <Data> s_data, vector <CPU_Data> &done_data, vector <CPU_Data> &abort_data ) ;
void out_double_put( vector <CPU_Data> done_data, vector <CPU_Data> abort_data, string outputName, int command ); // command 2 3 �g��
void input_list( int time, Data data, vector <CPU_Data> &list_data, int type, int CPU ) ; // �N��Ʀs�Jdone&abort_data
// command3 funtion
void CPU_modeling_two( vector <Data> s_data, vector <CPU_Data> &done_data, vector <CPU_Data> &abort_data ) ;
void put_q( Queue &q, vector <Data> &s_data );
void proccess_q( bool &cpu_state, Queue &q, int cpu, int &time, int  &cpu_time, vector <CPU_Data> &done_data, vector <CPU_Data> &abort_data ) ; // CPU����and��z
int which_q( Queue q1, Queue q2, bool cpu_one_state, bool cpu_two_state ); // ��J����q

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
        cout << endl << "* 3. Simulate two queues by SQF      *" ;
        cout << endl << "**************************************" ;
        cout << endl << "Input a command(0, 1, 2, 3): " ;
        cin >> command ;

       if ( command > 3 || command < 0 ){
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
                    printfData( s_data, first_line ) ;

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
                    out_double_put( done_cup_data, abort_cup_data, outputName, 2 ); // �g��output
                    cout << endl << "The simulation is running..." << endl << "See " << outputName << endl ;
                }
            } // else if
            else if ( command == 3 ){

                Exit = input_data( s_data, sortName, outputName, 3 ) ; // Ū��sorted
                if ( Exit ){
                    CPU_modeling_two( s_data, done_cup_data, abort_cup_data ) ;
                    out_double_put( done_cup_data, abort_cup_data, outputName,3 ); // �g��output
                    cout << endl << "The simulation is running..." << endl << "See " << outputName << endl ;
                }
            }

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
            while ( isdigit(line[i]) ){
                stemp_num.push_back(line[i]) ;
                i++ ;
            }

            if (  num != 4 ){
                while( isdigit(line[i]) != false ){ // �֥[�Ʀr
                    stemp_num.push_back(line[i]) ;
                    i++ ;
                }
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

    } // for()

    v_data.push_back(temp_data) ;
} // findData

void sortput( vector <Data> v_data, string first_line, string sortName ) {

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

void out_double_put( vector <CPU_Data> done_data, vector <CPU_Data> abort_data, string outputName, int command ){

    float ave_delay = 0.00 ; // Average Delay
    float suc_rate = 0.00 ; // Success Rate

    ofstream outfile (outputName.c_str());

	if (command == 2) {
	    outfile << "	[Abort Jobs]" << endl;
    	outfile << "	OID	Abort	Delay" << endl ;
    }
    else if (command == 3) {
    	outfile << "	[Abort Jobs]" << endl;
    	outfile << "	OID	CID	Abort	Delay" << endl ;
	}

    for( int i = 0 ; i < abort_data.size() ; i++ ){
        ave_delay = ave_delay + (float)abort_data[i].delay ;
        outfile << "[" << i+1 << "]" ;
        for ( int j = 1 ; j <= 3 ; j++ ){
            if ( j == 1 ) {
                outfile << "	" << abort_data[i].OID;
                if (command == 3) {
                	outfile << "	" << abort_data[i].CID;
				}
			}
            else if ( j == 2 )
                outfile << "	" << abort_data[i].time;
            else if ( j == 3 )
                outfile << "	" << abort_data[i].delay << endl ;
        } // for
    }// for

	if (command == 2) {
	    outfile << "	[Jobs Done]" << endl;
    	outfile << "	OID	Departure	Delay" << endl ;
    }
	else if (command == 3) {
		outfile << "	[Jobs Done]" << endl;
	    outfile << "	OID	CID	Departure	Delay" << endl ;
	}

    for( int i = 0 ; i < done_data.size() ; i++ ){
    	ave_delay = ave_delay + (float)done_data[i].delay ;
        outfile <<  "[" << i+1 << "]" ;
        for ( int j = 1 ; j <= 3 ; j++ ){
            if ( j == 1 ) {
                outfile << "	" << done_data[i].OID;
                if (command == 3) {
                	outfile << "	" << abort_data[i].CID;
				}
			}
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

void printfData( vector <Data> v_data, string first_line ) {

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

    if (command == 1) {
    	fileName = "input" + stemp_num + ".txt";
        outputName = "sorted" + stemp_num + ".txt";
        in.open(fileName.c_str());
    } // if
    else if (command == 2) { // command == 1
        fileName = "sorted" + stemp_num + ".txt";
        outputName = "output" + stemp_num + ".txt";
        in.open(fileName .c_str());
    } // else
    else if (command == 3) {
        fileName = "sorted" + stemp_num + ".txt";
        outputName = "double" + stemp_num + ".txt";
        in.open(fileName .c_str());
    }


    if (in.fail()) { // �ɮפ��s�b 
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

void sort_data( vector<Data> &s_data ) {

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
	int cpu = 1 ; // �u���@��CPU
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
				input_list(time, temp, abort_data, 2, cpu) ;
			}
			if (!q.isEmpty()) { // �B�z�U�@q
				temp = q.dequeue() ;
				cpu_state = true ;
				if (temp.dur + time > temp.t_out) { // ����O��(4.3)
					input_list(time, temp, abort_data, 3, cpu) ;
					cpu_time = temp.t_out ; // cpu�����ɶ�
				}
				else {
					input_list(time, temp, done_data, 0, cpu) ;
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
				input_list(time, temp, abort_data, 1, cpu) ;
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
			input_list(time, temp, abort_data, 2, cpu) ;
		}
		if (!q.isEmpty()) { // �B�z�U�@q
			temp = q.dequeue() ;
			cpu_state = true ;
			if (temp.dur + time > temp.t_out) { // ����O��(4.3)
				input_list(time, temp, abort_data, 3, cpu) ;
				cpu_time = temp.t_out ; // cpu�����ɶ�
			}
			else {
				input_list(time, temp, done_data, 0, cpu) ;
				cpu_time = temp.dur + time ; // cpu�����ɶ�
			}
		}
	}
} // CPU_modeling

void CPU_modeling_two( vector <Data> s_data, vector <CPU_Data> &done_data, vector <CPU_Data> &abort_data ){
	Queue q1 ;
	Queue q2 ;
    int time = 0 ; // �{�b�B��h�[
    int cpu_one_time = 0 ; // CPU1���浲���ɶ�
    int cpu_two_time = 0 ; // CPU2���浲���ɶ�
    Data temp ;
    CPU_Data CPU_temp ;
    bool cpu_one_state = false ; // true=����; false=����
    bool cpu_two_state = false ; // true=����; false=����

	// �B�zdata
	while ( !s_data.empty() ) {

        if(cpu_one_time == time && cpu_one_state == true) { // �٭�cpu1,cpu1���浲��
			cpu_one_state = false ;
		}
        if (cpu_two_time == time && cpu_two_state == true) { // �٭�cpu2,cpu2���浲��
			cpu_two_state = false ;
		}

		if (!q1.isEmpty()) { // cpu1�B�z // ��zq1
            proccess_q( cpu_one_state, q1, 1, time, cpu_one_time, done_data, abort_data ) ;
	    }
        if (!q2.isEmpty()) { // cpu2�B�z // ��zq2
            proccess_q( cpu_two_state, q2, 2, time, cpu_two_time, done_data, abort_data ) ;
	    }

	    if ( time == s_data[0].arr  ){ // �p�G��Ʃ�F���ɶ����{�b�ɶ�

            int type = which_q( q1, q2, cpu_one_state, cpu_two_state ) ;
            if ( type == 0 ){
                temp = s_data[0] ;
                s_data.erase(s_data.begin()) ; // ����first data
                input_list( time, temp, abort_data, 4, 0 ) ;
            }
            else if ( type == 1 ){ // ��q1
                put_q( q1, s_data ) ;
            }
            else if ( type == 2 ){
                put_q( q2, s_data ) ; // ��q2
            }

	    } // if
	    else{
            time++ ;
	    }

	} // while

	// �B�z�Ѿlq 
	while( !q1.isEmpty() || !q2.isEmpty() ){
        if(cpu_one_time == time && cpu_one_state == true) { // �٭�cpu1,cpu1���浲��
			cpu_one_state = false ;
		}
        if (cpu_two_time == time && cpu_two_state == true) { // �٭�cpu2,cpu2���浲��
			cpu_two_state = false ;
		}

        proccess_q( cpu_one_state, q1, 1, time, cpu_one_time, done_data, abort_data ) ; // cpu1�B�z // ��zq1
        proccess_q( cpu_two_state, q2, 2, time, cpu_two_time, done_data, abort_data ) ; // cpu2�B�z // ��zq2
        time++ ;
	}

} // CPU_modeling

void put_q (Queue &q, vector <Data> &s_data ){

    if (!q.IsFull()) { // q < 3
        q.enqueue(s_data[0]) ; // ��Jq
        s_data.erase(s_data.begin()) ; // ����first data
    }
    
}

int which_q( Queue q1, Queue q2, bool cpu_one_state, bool cpu_two_state ){

	if ( q1.isEmpty() && q2.isEmpty() && cpu_one_state == false && cpu_two_state == false ){ //���CPU���B�󶢸m���A(Case2)
    	return 1 ;
    }
    else if ( q1.isEmpty() && cpu_one_state == false ){ // 1��CPU�B�󶢸m���A�A2�bwork(Case1)
        return 1 ;
    }
    else if ( q2.isEmpty() && cpu_two_state == false ){ // 2��CPU�B�󶢸m���A�A1�bwork(Case1)
    	return 2 ;
    }
    else if ( q1.IsFull() && q2.IsFull() ){ // ��ӳ����A����(CASE3)
    	return 0 ;
    }
    else if ( q1.q_size() <= q2.q_size() ){ // ���b�u�@�A1�����פ���֡A��q1(Case4)
    	return 1 ;
    }
    else if( q1.q_size() > q2.q_size() ){ // ���b�u�@�A1�����פ���֡A��q2(Case4)
    	return 2 ;
    }

    return -1 ;
}

void proccess_q( bool &cpu_state, Queue &q, int cpu, int &time, int  &cpu_time, vector <CPU_Data> &done_data, vector <CPU_Data>&abort_data ){
    Data temp ;

	if (cpu_state == false && !q.isEmpty()) { // �p�Gcpu�S���b����F���

		while (!q.isEmpty() && q.getFront().t_out <= time) { // ���X�O��(4.2):�٨S����ɡA�N�w�g�w���F
			temp = q.dequeue() ; // ����q
			input_list(time, temp, abort_data, 2, cpu) ;
		}
		if (!q.isEmpty()) { // ���Xq��ư���
			temp = q.dequeue() ;
			cpu_state = true ;
			if (temp.dur + time > temp.t_out) { // ����O��(4.3):����L�{���|�O��
				input_list(time, temp, abort_data, 3, cpu) ;
				cpu_time = temp.t_out ; // cpu�����ɶ�
			}
			else { // ���榨�\
				input_list(time, temp, done_data, 0, cpu) ;
				cpu_time = temp.dur + time ; // cpu�����ɶ�
			}
		}
	}
}

void input_list( int time, Data data, vector <CPU_Data> &list_data, int type, int CPU ) {
    CPU_Data CPU_temp ;

    if (type == 0) { // ����
        CPU_temp.OID = data.OID ;
        CPU_temp.time = time + data.dur ;
        CPU_temp.CID  = CPU ;
        CPU_temp.delay = time - data.arr ;
        list_data.push_back(CPU_temp) ; // �����M��
    }
    else if (type == 1) { // 4.1
        CPU_temp.OID = data.OID ;
        CPU_temp.time = time ;
        CPU_temp.CID  = CPU ;
        CPU_temp.delay = 0 ;
        list_data.push_back(CPU_temp) ; // �����M��
    }
    else if (type == 2) { // 4.2
        CPU_temp.OID = data.OID ;
        CPU_temp.time = time ;
        CPU_temp.CID  = CPU ;
        CPU_temp.delay = CPU_temp.time - data.arr ;
        list_data.push_back(CPU_temp) ; // �����M��
    }
    else if (type == 3) { // 4.3
        CPU_temp.OID = data.OID ;
        CPU_temp.time = data.t_out ;
        CPU_temp.CID = CPU ;
        CPU_temp.delay = CPU_temp.time - data.arr ;
        list_data.push_back(CPU_temp) ; // �����M��
    }
    else if (type == 4){ // q1&q2 isFull()
        CPU_temp.OID = data.OID ;
        CPU_temp.CID = 0 ;
        CPU_temp.time = data.arr ;
        CPU_temp.delay = 0;
        list_data.push_back(CPU_temp) ; // �����M��
    }
} // input_list
