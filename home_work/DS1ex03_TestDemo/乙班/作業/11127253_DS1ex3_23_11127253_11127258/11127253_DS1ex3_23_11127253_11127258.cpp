// 11127253林語恬 11127258簡珮涵
# include <string>
# include <iostream>
# include <vector>
# include <fstream>
# include <cctype> // isdigit()
# include <ctime>  // 測秒用
# include <cstdlib> // atoi
#include <iomanip> // 小數點後兩位

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

// 讀檔區
void findData( vector <Data> &v_data, string line ) ; // 拆分getline中一行的資料，並儲存到vector
bool input_data( vector <Data> &v_data, string &fileName, string &outputName, int command ); // 讀檔

// command1 funtion
void sort_data( vector <Data> &s_data ) ; // shell sort
void printfData( vector <Data> v_data, string first_line ) ; // command 1 print
void sortput( vector <Data> v_data, string first_line, string sortName  );  // command 1 寫檔

// command2 funtion
void CPU_modeling( vector <Data> s_data, vector <CPU_Data> &done_data, vector <CPU_Data> &abort_data ) ;
void output( vector <CPU_Data> done_data, vector <CPU_Data> abort_data, string outputName ); // command 2 寫檔
void input_list( int time, Data data, vector <CPU_Data> &list_data, int type ) ; // 將資料存入done&abort_data

class Queue {
	// 題目2最多存三個
	vector <Data> q_data ;

	public:
		// q_data.begin()=0，寫這樣是視覺上比較符合q

		bool isEmpty(){ // q是否為空
			return q_data.empty() ;
		} // isEmpty

		void enqueue( Data data ){ // push資料
			q_data.push_back( data ) ;
		} // enqueue()

		void makeEmpty(){ // 清空q
			q_data.clear() ;
		} // makeEmpty()

		bool IsFull(){
			if ( q_data.size() == 3 )
				return true ;
			else
				return false ;
		} // IsFull()

		Data dequeue(){ // 丟掉最前面的，同時回傳
			Data Data_temp = getFront() ;
			q_data.erase(q_data.begin()) ;
			return Data_temp ;
		} // equeue()

		Data getFront(){ // 取得最前面的
			return q_data[0] ;
		} // getFront()

};

int main() {
    int command = -1 ; // 模式
    float Rtime = 0 ; // read_time
    float Stime = 0 ; // sort_time
    float Wtime = 0 ; // write_time
    string first_line = "OID	Arrival	Duration	TimeOut" ;
    string fileName = "input000.txt"; // file的名子
    string sortName = "sorted000.txt"; // file的名子
    string outputName = "output000.txt"; // file的名子
    string stemp_num ;  // 暫存變數
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
                Exit = input_data( v_data, fileName, sortName, 1 ) ; // 讀檔input
                endTime = clock();
                Rtime = (float)(endTime - startTime) *1000/ CLOCKS_PER_SEC ; // 讀檔時間 ms

                if ( Exit ){

                    s_data = v_data ;

                 	startTime = clock();
				    sort_data( s_data ) ; // 排序
				    endTime = clock();
				    Stime = (float)(endTime - startTime) *1000/ CLOCKS_PER_SEC ; // 排序時間

                    startTime = clock();
                    sortput(s_data, first_line, sortName) ; // 寫檔sorted
                    endTime = clock();
					Wtime = (float)(endTime - startTime) *1000/ CLOCKS_PER_SEC ; ; // 寫檔時間

                    // 輸出
                    printfData( v_data, first_line ) ;

					cout << "Reading data: " << Rtime << " clocks ("<< fixed << setprecision(2) << Rtime << " ms)." << endl ; // 執行時間
					cout << "Sorting data: " << Stime << " clocks ("<< fixed << setprecision(2) << Stime << " ms)." << endl ;
					cout << "Writing data: " << Wtime << " clocks ("<< fixed << setprecision(2) << Wtime << " ms)." << endl ;
                    cout << endl << "See "<< sortName << endl ;

                } // if()
            } // if()
            else if ( command == 2 ){ // 任務二

                Exit = input_data( s_data, sortName, outputName, 2 ) ; // 讀檔sorted
                if ( Exit ){
                    CPU_modeling( s_data, done_cup_data, abort_cup_data ) ;
                    output( done_cup_data, abort_cup_data, outputName  ); // 寫檔output 
                    cout << endl << "The simulation is running..." << endl << "See " << outputName << endl ;
                }
            } // else if

        } // else if

        // reset變數
        v_data.clear() ;// 清空vdata
        s_data.clear() ;  // 清空sdata
        done_cup_data.clear() ;
        abort_cup_data.clear() ;
        stemp_num = "" ;
    } while( command != 0 );

    return 0;
}

void findData( vector <Data> &v_data, string line ){// 拆分getline中一行的資料，並儲存到vector

    int num = 1 ;// 讀到第幾個資料(OID,arr,dur,tOut)
    string stemp_num = "" ; // 暫放資料的地方
    Data temp_data ; // 暫放Data的地方
    for( int i = 0 ; i < line.length() ; i++ ){
            if ( isdigit(line[i]) ){
                stemp_num.push_back(line[i]) ;
                i++ ;
                if ( i == line.length() ){
                    break ; // 因為i=字串lengh長度，直接跳出這個for迴圈當中 // 也可以用if else，只是這樣寫比較好辨認
                }

                while( isdigit(line[i]) != false ){ // 累加數字
                    stemp_num.push_back(line[i]) ;
                    i++ ;
                }

                // 看是要存哪一筆資料
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
                num++ ; // 第一筆資料讀完讀第二個資料以此類推

            } // 讀到數字

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

            // 輸出資料
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
    string stemp_num ; //暫存變數

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
            if ( line[0] == 'O' ){ // 第一行洗掉
                getline(in, line) ;
            } // if

            findData( v_data, line ) ;
        } // while

        in.close(); // 關閉文件
        return true ;
    } // else

	return true ;
} // input_data

void sort_data(vector<Data> &s_data) {

	for (int len = s_data.size()/2; len > 0; len = len/2) { // 排序間隔len
		for (int i = len; i < s_data.size(); i++) { // insert sort
			for (int j = i-len; j >= 0; j = j-len) { // j = i前一數 // 每行排序 
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
    int time = 0 ; // 現在運行多久
    int cpu_time = 0 ; // CPU執行結束時間
    Data temp ;
    CPU_Data CPU_temp ;
    bool cpu_state = false ; // true=執行; false=等待

	// 處理data
	while (!s_data.empty()) {
		if (cpu_time == time && cpu_state == true) { // 還原cpu
			cpu_state = false ;
		}
		if (cpu_state == false && !q.isEmpty()) { // cpu處理 // 整理q
			while (!q.isEmpty() && q.getFront().t_out <= time) { // 取出逾期(4.2)
				temp = q.dequeue() ; // 移除q
				input_list(time, temp, abort_data, 2) ;
			}
			if (!q.isEmpty()) { // 處理下一q
				temp = q.dequeue() ;
				cpu_state = true ;
				if (temp.dur + time > temp.t_out) { // 執行逾期(4.3)
					input_list(time, temp, abort_data, 3) ;
					cpu_time = temp.t_out ; // cpu結束時間
				}
				else {
					input_list(time, temp, done_data, 0) ;
					cpu_time = temp.dur + time ; // cpu結束時間
				}
			}
		}

		if (time == s_data[0].arr) {
			if (!q.IsFull()) { // q < 3
				q.enqueue(s_data[0]) ; // 放入q
				s_data.erase(s_data.begin()) ; // 移除first data
			}
			else { // q isFull(4.1)
				temp = s_data[0] ;
				s_data.erase(s_data.begin()) ; // 移除first data
				input_list(time, temp, abort_data, 1) ;
			}
		}
		else {
			time++ ;
		}

	}

	// 處理剩餘q
	while (!q.isEmpty()) {
		time = cpu_time ;
		cpu_state == false ;
		while (!q.isEmpty() && q.getFront().t_out <= time) { // 取出逾期(4.2)
			temp = q.dequeue() ;
			input_list(time, temp, abort_data, 2) ;
		}
		if (!q.isEmpty()) { // 處理下一q
			temp = q.dequeue() ;
			cpu_state = true ;
			if (temp.dur + time > temp.t_out) { // 執行逾期(4.3)
				input_list(time, temp, abort_data, 3) ;
				cpu_time = temp.t_out ; // cpu結束時間
			}
			else {
				input_list(time, temp, done_data, 0) ;
				cpu_time = temp.dur + time ; // cpu結束時間
			}
		}
	}
} // CPU_modeling

void input_list( int time, Data data, vector <CPU_Data> &list_data, int type ) {
	CPU_Data CPU_temp ;
	
	if (type == 0) { // 完成
		CPU_temp.OID = data.OID ;
		CPU_temp.time = time + data.dur ;
		CPU_temp.delay = time - data.arr ;
		list_data.push_back(CPU_temp) ; // 完成清單
	}
	else if (type == 1) { // 4.1
		CPU_temp.OID = data.OID ;
		CPU_temp.time = time ;
		CPU_temp.delay = 0 ;
		list_data.push_back(CPU_temp) ; // 取消清單
	}
	else if (type == 2) { // 4.2
		CPU_temp.OID = data.OID ;
		CPU_temp.time = time ;
		CPU_temp.delay = CPU_temp.time - data.arr ;
		list_data.push_back(CPU_temp) ; // 取消清單
	}
	else if (type == 3) { // 4.3
		CPU_temp.OID = data.OID ;
		CPU_temp.time = data.t_out ;
		CPU_temp.delay = CPU_temp.time - data.arr ;
		list_data.push_back(CPU_temp) ; // 取消清單
	}
} // input_list



