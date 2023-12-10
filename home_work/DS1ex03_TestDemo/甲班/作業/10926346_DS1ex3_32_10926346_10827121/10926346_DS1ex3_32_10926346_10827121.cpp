#include <iostream>
#include <fstream> // 檔案
#include <vector> // vector
#include <time.h> // 計算時間
#include <sstream> // 切字串
#include <iomanip>

using namespace std ;

static string num; // 檔名 數字部分
static string cmd;

struct Order { 	// 訂單
	int oid = 0; // 訂單編號
	int arrival = 0; // 下單時刻
	int duration = 0; // 製作耗時
	int timeOut = 0; // 逾時時刻
};
Order order ;

struct Abort { 	// 取消訂單
	int oid = 0; // 訂單編號
	int abort = 0; // 取消時刻
	int delay = 0; // 延遲耗時
};
Abort abortjob ;

struct Done { 	// 完成訂單
	int oid = 0; // 訂單編號
	int departure = 0; // 完成時刻
	int delay = 0; // 延遲耗時

};
Done done ;

vector <string> header; // 項目名稱 old，arrival，duration， timeOut
vector <Order> data; // 資料讀入後存於此
vector <Order> cpu; // 正在執行資料

bool keep = false;
string whichnum;

bool ReadTxt() ; // 讀檔
void WriteFile() ; // 寫檔
void del() ; //變數重製及釋放記憶體空間

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

            //第一個資料存入queue
            if( head == NULL && tail == NULL ){
                head = newnode;
                tail = newnode;
            }

            //非第一個資料存入queue
            else{
                tail -> next = newnode;
                tail = newnode;
            }
        }

        void dequeue() {
            Order result;
            //queue是否為空
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
	    double readT = 0 , sortT = 0 , writeT = 0 ; // 時間
		void ShellSort(); // 希爾排序
		void PrintData(); // 印出資料
		void PrintTime(); // 印出時間
};
mission1 m1;

class mission2 {
    public:
        bool finish = false; // 尚未所有工作執行完
        bool datadone = false; // 尚未所有工作下單完成
        bool process = false; // cpu執行中

        vector <Abort> abortlist; // 棄單資料
        vector <Done> donelist; // 完成訂單資料

        int nowtime = 0; // 目前時間
        int quesize = 0; // 佇列已有幾個
        int cpubegin = 0; // 該cpu開始時間
        int whichdata = 0; // 第幾筆訂單
        float donenums = 0; // 訂單完成數量
        float totaldelay = 0; // 總延誤

        void logdata ( string x, int arrival, int which ); // 記錄done或abort
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
				m1.PrintData(); // 印出資料 (無排序)
				m1.ShellSort(); // 希爾排序
				WriteFile(); // 寫檔
				m1.PrintTime(); // 印出時間

				cout << "See sorted"  << num << ".txt\n";

			} // if
			keep = true;
			whichnum = num;

		} // if(cmd=1)
		else if ( cmd == "2" ){

            if( ReadTxt() ) {
                m2.nowtime = data[0].arrival ;
                while (  m2.finish == false   ) {
                    //cpu是否有處理中訂單
                    if ( m2.process == true ) {
                        if ( m2.nowtime-m2.cpubegin == cpu[0].duration ) { // 該訂單處理完成

                            m2.logdata( "done", cpu[0].arrival, 0 ); // 記錄至donelist
                            cpu.pop_back();
                            m2.process = false;
                            m2.newjob(); // 從佇列抓取新工作

                        }
                        else if ( m2.nowtime == cpu[0].timeOut ) { // 該訂單逾時
                            m2.logdata( "abort", cpu[0].arrival, 0 ); // 記錄至adortlist
                            cpu.pop_back();
                            m2.process = false;
                            m2.newjob(); // 從佇列抓取新工作

                        } //else if
                    } //if
                    else {
                        m2.newjob(); // 從佇列抓取新工作
                    } //else

                    //是否有新訂單
                    m2.newque();
                    //是否處理完成全部訂單
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
    header.clear() ; // 釋放記憶體空間
    data.clear() ; // 釋放記憶體空間
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

// 讀檔
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
	file.open(name, ios_base::in) ; // 文件打開

    if (!file.is_open()) {
        cout << "### " << name << " does not exist! ###\n" ;
        return false ;
    } // if

    else {
    	start = clock() ;
    	int temp, count ;
    	string line ;

    	getline( file, line ) ;
    	header.push_back(line) ; // 讀入項目名

    	while( getline( file, line ) ) { // 讀入資料
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

		file.close() ; // 文件關閉

		end = clock() ;
		m1.readT = end - start ;

		return true;
	} // else
} // ReadTxt()

// 寫檔
void WriteFile() {
    if( cmd == "1" ) {
        double start = clock() ;
        ofstream newFile ; // Create a new file
        string name = "sorted" + num + ".txt" ;
        newFile.open(name, ios_base::out) ; // 文件打開
        for ( int i = 0 ; i < header.size() ; i++ )  // 寫入項目名
            if ( i == 0 )
                newFile << header[i] ;

        newFile << endl ;

        for ( int i = 0 ; i < data.size() ; i++ )   // 寫入資料
            newFile << data[i].oid << '\t' << data[i].arrival << '\t' << data[i].duration << '\t' << data[i].timeOut << endl;

        double end = clock() ;
        m1.writeT = end - start ;
    }
    else if( cmd == "2" ) {
        ofstream newFile ; // Create a new file
        string name = "output" + num + ".txt" ;
        newFile.open(name, ios_base::out) ; // 文件打開

        newFile << "\t[Abort Jobs]\n\tOID\tAbort\tDelay\n" ;// 寫入項目名

        for ( int i = 0 ; i < m2.abortlist.size() ; i++ ) {   // 寫入資料
            newFile << "[" << i+1 << "]\t" << m2.abortlist[i].oid << '\t' << m2.abortlist[i].abort << '\t' << m2.abortlist[i].delay << endl;
        }
        newFile << "\t[Jobs Done]\n\tOID\tDeparture\tDelay\n" ;
        for ( int i = 0 ; i < m2.donelist.size() ; i++ ) {   // 寫入資料
            newFile << "[" << i+1 << "]\t" << m2.donelist[i].oid << '\t' << m2.donelist[i].departure << '\t' << m2.donelist[i].delay << endl;
        }

        newFile << "[Average Delay]\t" << fixed  <<  setprecision(2) << m2.totaldelay/data.size() << " ms" << endl;
        newFile << "[Success Rate]\t" << fixed  <<  setprecision(2) << m2.donenums*100/data.size() << " %" <<endl;
    }

} // Writefile()

// 希爾排序
void mission1::ShellSort() {
	double start = clock() ;

	for ( int h = data.size() / 2 ; h > 0 ; h = h / 2 ) { // h代表間隔數
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

// 印出資料
void mission1::PrintData() {
    for ( int i = 0 ; i < header.size() ; i++ ) { // 印出項目名
    	cout << '\t' << header[i] ;
	} // for
	cout << endl ;

	for ( int i = 0 ; i < data.size() ; i++ ) // 印出資料
		cout << "(" << i+1 << ")" << '\t' << data[i].oid << '\t' << data[i].arrival << '\t' << data[i].duration << '\t' << data[i].timeOut << endl;
} // Printdata()

// 印出時間
void mission1::PrintTime() {
	cout << endl ;
	cout << "Reading data:\t" << readT << " clocks(" << readT << ".00 ms).\n" ;
	cout << "Sorting data:\t" << sortT << " clocks(" << sortT << ".00 ms).\n" ;
	cout << "Writing data:\t" << writeT << " clocks(" << writeT << ".00 ms).\n" << endl ;
} // Printtime()

//存取訂單結果
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

//執行下一訂單
void mission2::newjob() {

    cpubegin = nowtime;
    while( quesize > 0 ) {//是否有佇列
        que.dequeue();
        quesize--;
        //新工作是否取消
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

//收到新訂單
void mission2::newque(){
    while ( nowtime == data[whichdata].arrival && datadone == false ) {

        if ( process == false && quesize==0 ) {//佇列及cpu皆沒東西
            cpu.push_back( data[whichdata] );
            cpubegin = nowtime;
            process = true;
        }
        else if ( quesize < 3 ) {//佇列有空
            que.enqueue( data[whichdata] );
            quesize++;
        }
        else if ( quesize >= 3 ) {//兩邊皆滿
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
