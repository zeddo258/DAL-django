// 11127232 林鈺宸 and 11127252 雷國裕

#include<iostream> 
#include<string>
#include<vector> 
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<iomanip>  

using namespace std ;

struct JobType {        // JobType結構的定義(儲存各個型態的資料)
    int OID ;           // 資料的編號(ID)
    int arrival ;       // 到達時間
    int duration ;      // 處理時間
    int timeOut ;       // 臨界時間
} ;

class Queue {           // 佇列資料結構
    public:
        Queue() {       // 初始化
            front = NULL ;
            back = NULL ;
            avail = 3 ;
        } // Queue()

        int Avail() {
            return avail;
        }

        bool IsEmpty(){ // 判斷佇列是否為空
            if ( front == NULL ) {
                return true ;
            } // if

            return false ;
        } // IsEmpty()

        JobType GetFront() { // 取得第一份資料
            return front->Data ;
        } // GetFront()

        void Enqueue( JobType input ) { // 新增資料至佇列後
            if ( IsEmpty() ) {
                front = new List ;
                front->Data = input ;
                front->next = NULL ;
                back = front ;
            } // if
            else {
                back->next = new List ;
                back = back->next ;
                back->Data = input ;
                back->next = NULL ;
            } // else

            avail--;
        } // Enqueue()

        void Dequeue() {    // 移除佇列中第一份資料
            if ( front == back ) {
                delete front ;
                front = NULL ;
                back = NULL ;
            } // if
            else {
                List* tmp = front ;
                front = front->next ;
                delete tmp ;
            } // else

            avail++;
        } // Dequeue()

        bool IsFull() {    // 判斷佇列是否滿了
            if ( avail == 0 ) {
                return true;
            } // if ()

            return false;
        } // IsFull()

        void Clear() {     // 刪除所有資料
            while ( front != NULL ) {
                List* tmp = front ;
                front = front->next ;
                delete tmp ;
            } // while
        } // Clear()

    private:
        struct List {      // List結構的定義(儲存JobType的Queue)
            JobType Data ;
            List* next ;
        } ;

        List* front ;      // 指向front(頭)的指標
        List* back ;       // 指向back(尾)的指標
        int avail ;        // 目前佇列剩餘儲存空間( Max:3 )
} ; // class Queue

class JobList {
	private:
		vector<JobType> aList ; // 用來儲存資料的動態配置
		string fileID ;         // 檔案編號

    public:
		void reset() {          // 清除資料
			aList.clear() ;
		} // void()

        string getfile() {      // 取得檔案編號
            return fileID ;
        } // getfile()
        
		void sortByArrival() {  // arrival由小到大進行排序( 希爾排序 )
			clock_t start = clock() ;
			int length = aList.size() ;
			for ( int h = length / 2 ; h > 0 ; h = h / 2 ) {
				for ( int unsorted = h ; unsorted < length ; unsorted++ ) {
					int loc = unsorted ;
					JobType nextItem = aList[unsorted] ;
					int next_arr = aList[unsorted].arrival ;
					int next_OID = aList[unsorted].OID ;
					
					while ( ( ( loc >= h ) && ( aList[loc-h].arrival > next_arr ) ) ||
						    ( ( aList[loc-h].arrival == next_arr ) && aList[loc-h].OID > next_OID ) ) {
						aList[loc] = aList[loc-h] ;
                        loc = loc - h ;
                    } // while
					
					aList[loc] = nextItem ;
				} // for
			} // for
			
			clock_t end = clock() ;
			double times = (double)( end - start ) / CLOCKS_PER_SEC ;
            cout << "Sorting data: " << end - start << " clocks ";
			cout << fixed << setprecision(2) << "(" << times * 1000 << " ms)." << endl ;
		} // sortByArrival()
		
		void putAll() {         // 將排序完成之資料寫入檔案
			string name = "sorted" + fileID + ".txt";
			ofstream output( name );
            clock_t start = clock();
			output << "OID\tArrival\tDuration\tTimeOut\n";
			for( int i = 0; i < aList.size(); i++ ) {
				output << aList[i].OID << '\t';
				output << aList[i].arrival << '\t';
				output << aList[i].duration << '\t';
				output << aList[i].timeOut;
				output << endl;
			} // for
			
            clock_t end = clock();
			output.close();
			double times = (double)( end - start ) / CLOCKS_PER_SEC;
            cout << "Writing data: " << end - start << " clocks ";
			cout << fixed << setprecision(2) << "(" << times * 1000 << " ms)." << endl ;
            cout << "\nSee " << name << endl;
		} // putAll()
		
		void show() {           // 列印原始之資料
			cout << endl << "\tOID\tArrival\tDuration\tTimeOut\n";
			for( int i = 0; i < aList.size(); i++ ) {
				cout << "(" << i + 1 << ")" << '\t'; 
				cout << aList[i].OID << '\t';
				cout << aList[i].arrival << '\t';
				cout << aList[i].duration << '\t';
				cout << aList[i].timeOut << '\t';
				cout << endl;
			} // for
			
			cout << "\n";
		} // show()
		
		void getJob( JobType & job ) {  // 取得aList中的第一項資料
            job = aList[0];
		} // getJob()
		
		bool isEmpty() {                // 判斷aList是否為空
			if ( aList.empty() )
				return true;
			else
				return false;
		} // isEmpty()
		
		bool getAll() {                 // 進行讀檔
			int OID, arrival, duration, timeout;
			string fileNumber, title;
			cout << "\nInput a file number: ";
			cin >> fileNumber;
			fileID = fileNumber;
			fileNumber = "input" + fileNumber + ".txt";
			ifstream input( fileNumber );
			
			if ( !input.is_open() ) {   // 判斷檔案是否開啟成功
				cout << "\n### " << fileNumber << " does not exist! ###\n";
				return false;
			} // if
			
            clock_t start = clock();

			getline( input, title );
			
			while ( input >> OID >> arrival >> duration >> timeout ) {  // 讀入檔案資料並儲存至aList
				JobType list;
				list.OID = OID;
				list.arrival = arrival;
				list.duration = duration;
				list.timeOut = timeout;
				aList.push_back( list );
			} // while
			
			clock_t end = clock();
            input.close();
            show();
            double times = (double)( end - start ) / CLOCKS_PER_SEC;
            cout << "Reading data: " << end - start << " clocks ";
            cout << fixed << setprecision(2) << "(" << times * 1000 << " ms)." << endl ;
			return true;
		} // getAll()
		
        bool getSorted() {             // 取得排序好的資料
			int OID, arrival, duration, timeout;
			string fileNumber, title;
            if ( fileID.empty() ) {
                cout << "\nInput a file number: ";
                cin >> fileNumber;
                fileID = fileNumber;
            } // if
			
			fileNumber = "sorted" + fileID + ".txt";
			ifstream input( fileNumber );
			if ( !input.is_open() ) {
				cout << "\n### " << fileNumber << " does not exist! ###\n";
				return false;
			} // if

			getline( input, title );
			while ( input >> OID >> arrival >> duration >> timeout ) {
				JobType list;
				list.OID = OID;
				list.arrival = arrival;
				list.duration = duration;
				list.timeOut = timeout;
				aList.push_back( list );
			} // while
			
            input.close();
            return true;
        } // getSorted()

		void del( JobType & job ) {     // 刪除並取得aList之首項資料
            job = aList[0]; 
            aList.erase( aList.begin() );
		} // del()
} ; // class JobList

class AnsList {     // 用來儲存完成及被取消的資料
    private:
        typedef struct aT {
            int OID ;
            int abort ;     // 取消時刻
            int CID = 0;
            int delay ;
        } abortType ;

        typedef struct dT {
            int OID ;
            int departure ; // 完成時刻
            int CID = 0;
            int delay ;
        } doneType ;

        vector<abortType> abortJobs ;
        vector<doneType> doneJobs ;
        float avgDealy ;           // 平均delay時間
        float successRate ;        // 事件成功率
        bool doub ;
    
    public:
        AnsList() {         // 初始化
            avgDealy = 0.0;
            successRate = 0.0;
            doub = false ;
        } // AnsList()

        void setDouble() {
            doub = true ;
        } // setDouble()

        void addAbortJob( int OID, int abort, int delay ) { // 新增取消之資料至Abort list
            abortType tmp;
            tmp.OID = OID;
            tmp.abort = abort;
            tmp.delay = delay;
            abortJobs.push_back( tmp );
        } // addAbortJob()

        void addAbortJob( int OID, int abort, int delay, int CID ) { // 新增取消之資料至Abort list
            abortType tmp;
            tmp.OID = OID;
            tmp.abort = abort;
            tmp.delay = delay;
            tmp.CID = CID ;
            abortJobs.push_back( tmp );
        } // addAbortJob()

        void addDoneJob( int OID, int departure, int delay ) {  // 新增完成之資料至Done list
            doneType tmp;
            tmp.OID = OID;
            tmp.departure = departure;
            tmp.delay = delay;
            doneJobs.push_back( tmp );
        } // addDoneJob()

        void addDoneJob( int OID, int departure, int delay, int CID ) {  // 新增完成之資料至Done list
            doneType tmp;
            tmp.OID = OID;
            tmp.departure = departure;
            tmp.delay = delay;
            tmp.CID = CID ;
            doneJobs.push_back( tmp );
        } // addDoneJob()
       
        void putAll( string fileID ) {  // 依完成處理之清單進行寫檔
			string name = "output" + fileID + ".txt";
            if ( doub ) {
                name = "double" + fileID + ".txt";
            } // if

			ofstream output( name );
            float delaytime = 0.0;
            output << "\t[Abort Jobs]\n";
            if ( doub ) {
                output << "\tOID\tCID\tAbort\tDelay\n";
            } // if
            else {
                output << "\tOID\tAbort\tDelay\n";
            } // else

            for( int i = 0; i < abortJobs.size(); i++ ) {
                output << "[" << i + 1 << "]";
                output << "\t"<< abortJobs[i].OID;
                if ( doub ) {
                    output << "\t"<< abortJobs[i].CID;
                } // if

                output << "\t"<< abortJobs[i].abort;
                output << "\t"<< abortJobs[i].delay;
                delaytime = delaytime + abortJobs[i].delay;
                output << endl;
            } // for

            output << "\t[Jobs Done]\n";
            if ( doub ) {
                output << "\tOID\tCID\tDeparture\tDelay\n";
            } // if
            else {
                output << "\tOID\tDeparture\tDelay\n";
            } // else

            for( int i = 0; i < doneJobs.size(); i++ ) {
                output << "[" << i + 1 << "]";
                output << "\t"<< doneJobs[i].OID;
                if ( doub ) {
                    output << "\t"<< doneJobs[i].CID;
                } // if

                output << "\t"<< doneJobs[i].departure;
                output << "\t"<< doneJobs[i].delay;
                delaytime = delaytime + doneJobs[i].delay;
                output << endl;
            } // for

            float jobAmount = abortJobs.size() + doneJobs.size();
            avgDealy = delaytime / jobAmount;
            successRate = doneJobs.size() / jobAmount;

            output << "[Average Delay]\t" << fixed << setprecision(2) << avgDealy << " ms" << endl;
            output << "[Success Rate]\t" << fixed << setprecision(2) << successRate * 100 << " %" << endl;

            cout << "See " << name << endl;
			output.close();
        } // putAll()
} ; // class AnsList

void Simulate( JobList list ) {     // 模擬佇列處理動作
    Queue aQueue;
    AnsList ans;
    JobType tmp;
    int times = 0;                  // 目前之時間
    bool working = true;            // 用來判斷模式是否要切換( 處理/排隊 )

    while ( !list.isEmpty() ) {             // 如果aList不為空
        while ( working && !list.isEmpty() ) {  // 處理模式
            if ( aQueue.IsEmpty() ) {      // 處理模式時若佇列為空
                list.del( tmp );           // 取得並移除aList中目前位於首位的資料
                aQueue.Enqueue( tmp );     // 加入佇列隊伍
            } // if

            tmp = aQueue.GetFront();       // 取得front指向之data
            aQueue.Dequeue();              // 移除queue之fornt指向之資料 

            if ( times < tmp.arrival ) {   // 如果當前時間未到目前隊伍中第一位之到達時間
                times = tmp.arrival;       // 則將目前時間設定為成當前時間
            } // if

            if ( times >= tmp.timeOut ) {  // 目前時間等於或超過Timeout則將此資料取消(abort) 
                ans.addAbortJob( tmp.OID, times, times - tmp.arrival );
            } // if
            else if ( times + tmp.duration > tmp.timeOut ) {    // 目前時間加上處理時間超過Timeout則將此資料取消(abort) 
                times = tmp.timeOut;    // 並設定目前時間為Timeout
                ans.addAbortJob( tmp.OID, times, times - tmp.arrival );
            } // if
            else {  // 執行成功則此資料加入Done list
                ans.addDoneJob( tmp.OID, times + tmp.duration, times - tmp.arrival );
                times = times + tmp.duration;
            } // else

            if ( !list.isEmpty() ) {    // 如果aList不為空
                list.getJob( tmp );     // 取得aList中首位之資料
                if ( times > tmp.arrival ) {    // 判斷目前時間是否超過aList中首位資料的到達時間
                    working = false;    // 是的話切換到排隊模式
                } // if
            } // if
        } // while

        while ( !working && !list.isEmpty() ) { // 排隊模式
            list.del( tmp );    // 取得並移除aList中目前位於首位的資料

            if ( !aQueue.IsFull() ) {   // 若佇列還沒滿(不達3筆資料)
                aQueue.Enqueue( tmp );  // 將此資料存入序列
            } // if
            else {                      // 佇列滿了則將此資料取消
                ans.addAbortJob( tmp.OID, tmp.arrival, 0 );
            } // else

            if ( !list.isEmpty() ) {    // 如果aList不為空 
                list.getJob( tmp );     // 取得aList中首位之資料
                if ( times <= tmp.arrival ) {  // 判斷目前時間是否小於等於aList中首位資料的到達時間
                    working = true;     // 是的話切換到處理模式
                } // if
            } // if
        } // while
    } // while
    
    while ( !aQueue.IsEmpty() ) {   // 處理剩下來在排隊隊伍中的資料
        tmp = aQueue.GetFront();
        aQueue.Dequeue();

        if ( times < tmp.arrival ) {
            times = tmp.arrival;
        } // if

        if ( times >= tmp.timeOut ) {
            ans.addAbortJob( tmp.OID, times, times - tmp.arrival );
        } // if
        else if ( times + tmp.duration > tmp.timeOut ) {
            times = tmp.timeOut;
            ans.addAbortJob( tmp.OID, times, times - tmp.arrival );
        } // if
        else {
            ans.addDoneJob( tmp.OID, times + tmp.duration, times - tmp.arrival );
            times = times + tmp.duration;
        } // else
    } // while

    ans.putAll( list.getfile() );
} // Simulate()

void Simulate3( JobList list ) {     // 模擬佇列處理動作
    Queue Queue1;
    Queue Queue2;
    AnsList ans;
    JobType tmp;
    JobType tmp2;
    ans.setDouble() ;
    int process = 1;
    int times1 = 0;
    int times2 = 0;
    bool working1 = true;            // 用來判斷模式是否要切換( 處理/排隊 )
    bool working2 = true;            // 用來判斷模式是否要切換( 處理/排隊 )
    bool nowdeal = false;            // 表示目前佇列中有應需立即處理之資料

    while ( !list.isEmpty() ) {             // 如果aList不為空
        while ( ( working1 || working2 ) && !list.isEmpty() ) {  // 處理模式
            nowdeal = false;
            if ( ( !Queue1.IsEmpty() && working1 && Queue1.GetFront().arrival <= times1 ) &&    // 佇列1跟住列2資料的arrival <= 分別的time，取資料中arrival較小的佇列處理
             ( !Queue2.IsEmpty() && working2 && Queue2.GetFront().arrival <= times2 ) ) {
                if ( Queue1.GetFront().arrival <= Queue2.GetFront().arrival ) {
                    tmp = Queue1.GetFront();       // 取得第一份資料
                    Queue1.Dequeue() ;
                    nowdeal = true;
                    process = 1 ;   
                }
                else {
                    tmp = Queue2.GetFront();       // 取得第一份資料
                    Queue2.Dequeue() ;
                    nowdeal = true;
                    process = 2 ;
                }
            }
            else if ( !Queue1.IsEmpty() && working1 && Queue1.GetFront().arrival <= times1 ) {   // 佇列1中第一筆資料的arrival <= 目前佇列1的time
                tmp = Queue1.GetFront();       // 取得第一份資料
                Queue1.Dequeue() ;
                nowdeal = true;
                process = 1 ;                 
            }
            else if ( !Queue2.IsEmpty() && working2 && Queue2.GetFront().arrival <= times2 ) {   // 佇列2中第一筆資料的arrival <= 目前佇列2的time
                tmp = Queue2.GetFront();       // 取得第一份資料
                Queue2.Dequeue() ;
                nowdeal = true;
                process = 2 ;
            }
            else if ( Queue1.IsEmpty() && working1 ) {      // 處理模式時若佇列1為空
                list.del( tmp );           // 取得並移除aList中目前位於首位的資料
                Queue1.Enqueue( tmp );     // 加入佇列隊伍
            } // if
            else if ( Queue2.IsEmpty() && working2 ) {      // 處理模式時若佇列2為空
                list.del( tmp );           // 取得並移除aList中目前位於首位的資料
                Queue2.Enqueue( tmp );     // 加入佇列隊伍
            }
            
            if ( working1 && working2 && !Queue2.IsEmpty() && !nowdeal ) {
                tmp = Queue1.GetFront();       // 取得第一份資料
                tmp2 = Queue2.GetFront();       // 取得第一份資料
                if ( tmp.arrival <= tmp2.arrival ) {
                    process = 1 ;
                    Queue1.Dequeue() ;
                }
                else {
                    process = 2 ;
                    Queue2.Dequeue() ;
                    tmp = tmp2 ;
                }
            }
            else if ( working1 && !nowdeal ) {
                tmp = Queue1.GetFront();       // 取得第一份資料
                Queue1.Dequeue();              // 移除queue中第一份資料
                process = 1;
            } // if
            else if ( working2 && !nowdeal ) { 
                tmp = Queue2.GetFront();       // 取得第一份資料
                Queue2.Dequeue();              // 移除queue中第一份資料
                process = 2;
            }

            if ( process == 1 ) {

                if ( times1 < tmp.arrival ) {   // 如果當前時間未到目前隊伍中第一位之到達時間
                    times1 = tmp.arrival;       // 則將目前時間設定為成當前時間
                } // if

                if ( times1 >= tmp.timeOut ) {  // 目前時間等於或超過Timeout則將此資料取消(abort) 
                    ans.addAbortJob( tmp.OID, times1, times1 - tmp.arrival, 1 );
                } // if
                else if ( times1 + tmp.duration > tmp.timeOut ) {    // 目前時間加上處理時間超過Timeout則將此資料取消(abort) 
                    times1 = tmp.timeOut;    // 並設定目前時間為Timeout
                    ans.addAbortJob( tmp.OID, times1, times1 - tmp.arrival, 1 );
                } // if
                else {  // 執行成功則此資料加入Done list
                    ans.addDoneJob( tmp.OID, times1 + tmp.duration, times1 - tmp.arrival, 1 );
                    times1 = times1 + tmp.duration;
                } // else

            }

            else {

                if ( times2 < tmp.arrival ) {   // 如果當前時間未到目前隊伍中第一位之到達時間
                    times2 = tmp.arrival;       // 則將目前時間設定為成當前時間
                } // if

                if ( times2 >= tmp.timeOut ) {  // 目前時間等於或超過Timeout則將此資料取消(abort) 
                    ans.addAbortJob( tmp.OID, times2, times2 - tmp.arrival, 2 );
                } // if
                else if ( times2 + tmp.duration > tmp.timeOut ) {    // 目前時間加上處理時間超過Timeout則將此資料取消(abort) 
                    times2 = tmp.timeOut;    // 並設定目前時間為Timeout
                    ans.addAbortJob( tmp.OID, times2, times2 - tmp.arrival, 2 );
                } // if
                else {  // 執行成功則此資料加入Done list
                    ans.addDoneJob( tmp.OID, times2 + tmp.duration, times2 - tmp.arrival, 2 );
                    times2 = times2 + tmp.duration;
                } // else

            }

            if ( !list.isEmpty() ) {    // 如果aList不為空
                list.getJob( tmp );     // 取得aList中首位之資料
                if ( times1 > tmp.arrival ) {    // 判斷目前時間是否超過aList中首位資料的到達時間
                    working1 = false;    // 是的話切換到排隊模式
                } // if
                else {
                    working1 = true;
                }

                if ( times2 > tmp.arrival ) {    // 判斷目前時間是否超過aList中首位資料的到達時間
                    working2 = false;    // 是的話切換到排隊模式
                } // if
                else {
                    working2 = true;
                }

            } // if
        } // while

        while ( !working1 && !working2 && !list.isEmpty() ) { // 排隊模式
            list.del( tmp );    // 取得並移除aList中目前位於首位的資料

            if ( !Queue1.IsFull() && Queue1.Avail() >= Queue2.Avail() ) {   // 若佇列還沒滿(不達3筆資料)
                Queue1.Enqueue( tmp );  // 將此資料存入序列
            } // if
            else if ( !Queue2.IsFull() && Queue1.Avail() < Queue2.Avail() ) {
                Queue2.Enqueue( tmp );  // 將此資料存入序列
            }
            else {                      // 佇列滿了則將此資料取消
                ans.addAbortJob( tmp.OID, tmp.arrival, 0, 0 );
            } // else

            if ( !list.isEmpty() ) {    // 如果aList不為空 
                list.getJob( tmp );     // 取得aList中首位之資料

                if ( times1 <= tmp.arrival ) {  // 判斷目前時間是否小於等於aList中首位資料的到達時間
                    working1 = true;     // 是的話切換到處理模式
                } // if

                if ( times2 <= tmp.arrival ) {  // 判斷目前時間是否小於等於aList中首位資料的到達時間
                    working2 = true;     // 是的話切換到處理模式
                } // if

            } // if
        } // while
    } // while

    while ( !Queue1.IsEmpty() ) {   // 處理剩下來在排隊隊伍中的資料
        tmp = Queue1.GetFront();
        Queue1.Dequeue();

        if ( times1 < tmp.arrival ) {
            times1 = tmp.arrival;
        } // if

        if ( times1 >= tmp.timeOut ) {
            ans.addAbortJob( tmp.OID, times1, times1 - tmp.arrival, 1 );
        } // if
        else if ( times1 + tmp.duration > tmp.timeOut ) {
            times1 = tmp.timeOut;
            ans.addAbortJob( tmp.OID, times1, times1 - tmp.arrival, 1 );
        } // if
        else {
            ans.addDoneJob( tmp.OID, times1 + tmp.duration, times1 - tmp.arrival, 1 );
            times1 = times1 + tmp.duration;
        } // else
    } // while

    while ( !Queue2.IsEmpty() ) {   // 處理剩下來在排隊隊伍中的資料
        tmp = Queue2.GetFront();
        Queue2.Dequeue();

        if ( times2 < tmp.arrival ) {
            times2 = tmp.arrival;
        } // if

        if ( times2 >= tmp.timeOut ) {
            ans.addAbortJob( tmp.OID, times2, times2 - tmp.arrival, 2 );
        } // if
        else if ( times2 + tmp.duration > tmp.timeOut ) {
            times2 = tmp.timeOut;
            ans.addAbortJob( tmp.OID, times2, times2 - tmp.arrival, 2 );
        } // if
        else {
            ans.addDoneJob( tmp.OID, times2 + tmp.duration, times2 - tmp.arrival, 2 );
            times2 = times2 + tmp.duration;
        } // else
    } // while

    ans.putAll( list.getfile() );
} // Simulate3()

int main() {
	string command ;
	JobList joblist ;
	do {
		cout << "**** Simulate FIFO Queues by SQF *****\n" ;
        cout << "* 0. Quit                            *\n" ;
        cout << "* 1. Sort a file                     *\n" ;
        cout << "* 2. Simulate one FIFO queue         *\n" ;
        cout << "* 3. Simulate two queues by SQF      *\n" ;
        cout << "**************************************\n" ;
		cout << "Input a command(0, 1, 2, 3): " ;
		cin >> command ;
		
		if ( command.compare( "0" ) == 0 ){
			break ;
		} // if
		else if ( command.compare( "1" ) == 0 ) {
			if ( joblist.getAll() ) {       // 判斷讀檔至否成功
				joblist.sortByArrival() ;   // 排序
				joblist.putAll() ;          // 寫檔
            } // if
		} // if
		else if ( command.compare( "2" ) == 0 ) {
			if ( joblist.getSorted( ) ) {   // 判斷排序檔是否存在
                cout << "\nThe simulation is running..." << endl ;
				Simulate( joblist ) ;       // 模擬
			} // if
		} // if
		else if ( command.compare( "3" ) == 0 ) {
			if ( joblist.getSorted( ) ) {   // 判斷排序檔是否存在
                cout << "\nThe simulation is running..." << endl ;
				Simulate3( joblist ) ;       // 模擬
			} // if
		} // if
		else {	
			cout << "\nCommand does not exist!\n" ;
		} // else

        joblist.reset() ;
        cout << endl ;
	} while( 1 ) ;

    return 0;
} // main()