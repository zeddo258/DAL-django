//10911110-資工二乙-江侑倫 10920124 林雅琪
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fstream>
#include<time.h>
#include<iomanip>
#define MAX_LEN 255
using namespace std;

class List{

	private:
		struct DoneNode{
		    int CIO ;
			int OID ;
			int Departure ;
			int Delay ;
			DoneNode *next ;
		};

		DoneNode *Donehead = NULL ;
		DoneNode *Donetail = NULL ;

		struct AbortNode{
		    int CIO ;
			int OID ;
			int Abort ;
			int Delay ;
			AbortNode *next ;
		};

		AbortNode *Aborthead = NULL ;
		AbortNode *Aborttail = NULL ;

	public:
        //新增完成清單
		void enDone( int OID, int departure, int delay ) {
			DoneNode *newPtr = new DoneNode ;
			newPtr->OID = OID ;
			newPtr->Departure = departure ;
			newPtr->Delay = delay ;
			newPtr->next = NULL ;

			if( Donehead == NULL ) {
				Donehead = newPtr ;
				Donetail = newPtr ;
			}
			else {
				Donetail->next = newPtr ;
				Donetail = newPtr ;
			}
		}

        //新增執行完成的CPU編號
		void enDoenCIO( int CIO ){
            Donetail->CIO = CIO ;
		}

        //刪除完成清單頂端資料
		void deDoen(){
            DoneNode *temp = Donehead->next ;
            delete Donehead ;
            Donehead = temp ;
		}

        //新增終止清單
		void enAbort( int OID, int abort, int delay ) {
			AbortNode *newPtr = new AbortNode ;
			newPtr->OID = OID ;
			newPtr->Abort = abort ;
			newPtr->Delay = delay ;
			newPtr->next = NULL ;

			if( Aborthead == NULL ) {
				Aborthead = newPtr ;
				Aborttail = newPtr ;
			}
			else {
				Aborttail->next = newPtr ;
				Aborttail = newPtr ;
			}
		}

        //新增終止的CPU編號
		void enAbortCIO( int CIO ){
            Aborttail->CIO = CIO ;
		}

        //刪除終止清單頂端資料
		void deAbort(){
            AbortNode *temp = Aborthead->next ;
            delete Aborthead ;
            Aborthead = temp ;
		}

        //讀取完成清單頂端資料
		bool OutDone( int & num1, int & num2, int & num3 ) {
            if( Donehead == NULL ) return false ;
            else {
                num1 = Donehead->OID ;
                num2 = Donehead->Departure ;
                num3 = Donehead->Delay ;
                return true ;
            }
		}

        //讀取完成清單頂端資料的CPU編號
		bool OutDoneCIO( int & num4 ){
            if( Donehead == NULL ) return false ;
            else {
                num4 = Donehead->CIO ;
                return true ;
            }
		}

        //讀取終止清單頂端資料
		bool OutAbort( int & num1, int & num2, int & num3 ) {
			if( Aborthead == NULL ) return false ;
            else {
                num1 = Aborthead->OID ;
                num2 = Aborthead->Abort ;
                num3 = Aborthead->Delay ;
                return true ;
            }
		}

        //讀取終止清單頂端資料的CPU編號
		bool OutAbortCIO( int & num4 ){
            if( Aborthead == NULL ) return false ;
            else {
                num4 = Aborthead->CIO ;
                return true ;
            }
		}

        //列印終止列表
		void printfAbort(){
		    cout<<"\t[Abort List]\t\n";
            AbortNode *temp=Aborthead;
            int i=1;
            while(temp!=NULL){
                cout<<'['<<i<<"]\t"<<temp->OID<<'\t'<<temp->Abort<<'\t'<<temp->Delay<<endl;
                temp=temp->next;
                i++;
            }
		}

		//列印包含CPU編號的終止列表
		void printfAbortCIO(){
		    cout<<"\t[Abort List]\t\n";
            AbortNode *temp=Aborthead;
            int i=1;
            while(temp!=NULL){
                cout<<'['<<i<<"]\t"<<temp->OID<<'\t'<<temp->CIO<<'\t'<<temp->Abort<<'\t'<<temp->Delay<<endl;
                temp=temp->next;
                i++;
            }
		}

        //列印完成列表
		void printfDone(){
		    cout<<"\t[Done List]\t\n";
            DoneNode *temp=Donehead;
            int i=1;
            while(temp!=NULL){
                cout<<'['<<i<<"]\t"<<temp->OID<<'\t'<<temp->Departure<<'\t'<<temp->Delay<<endl;
                temp=temp->next;
                i++;
            }
		}

		//列印包含CPU編號的完成列表
		void printfDoneCIO(){
		    cout<<"\t[Done List]\t\n";
            DoneNode *temp=Donehead;
            int i=1;
            while(temp!=NULL){
                cout<<'['<<i<<"]\t"<<temp->OID<<'\t'<<temp->CIO<<'\t'<<temp->Departure<<'\t'<<temp->Delay<<endl;
                temp=temp->next;
                i++;
            }
		}
};

class Queue{

	private:
		struct QueueNode{

		int OID ;
		int Arrival ;
		int Duration ;
		int TimeOut ;
		QueueNode *next ;

		};

		QueueNode *head = NULL ;
		QueueNode *tail = NULL ;

		QueueNode *WaitHead = NULL ;
		QueueNode *WaitTail = NULL ;


		int NumInQueue = 3 ;
        double DelaySum;
        double Success;

	public :
        //新增排隊
		void enWait( QueueNode *Doing ) {
			QueueNode *temp = new QueueNode ;
			temp->OID = Doing->OID ;
			temp->Arrival = Doing->Arrival ;
			temp->Duration = Doing->Duration ;
			temp->TimeOut = Doing->TimeOut ;
			temp->next = NULL ;

			if( WaitHead == NULL ) {
				WaitHead = temp ;
				WaitTail = temp ;
			}
			else {
				WaitTail->next = temp ;
				WaitTail = WaitTail->next ;
			}
		}

        //刪除排隊
		void deWait() {
			if( WaitHead == WaitTail && WaitHead != NULL ) {
				WaitHead = NULL ;
				WaitTail = NULL ;
			}
			else {
                QueueNode *temp = WaitHead->next ;
				delete WaitHead ;
				WaitHead = temp ;
			}
		}

        //列印排隊隊伍
        void printfWait(int time){
            QueueNode *temp=WaitHead;
            cout<<"\nQueue ("<<time<<"):\n";
            while(temp!=NULL){
                cout<<temp->OID<<'\t'<<temp->Arrival<<'\t'<<temp->Duration<<'\t'<<temp->TimeOut<<endl;
                temp=temp->next;
            }
        }

        //檢查Queue是否為空
        bool QisEmpty(){
            if( head==tail && head==NULL) return 1;
            else return 0;
        }//檢查還有沒有剩餘資料

        //新增Queue資料
		void enQueue(int num1, int num2, int num3, int num4) {

			QueueNode *newPtr = new QueueNode ;
			newPtr->OID = num1 ;
			newPtr->Arrival = num2 ;
			newPtr->Duration = num3 ;
			newPtr->TimeOut = num4 ;
			newPtr->next = NULL ;

			if( head == NULL ) {
				head = newPtr ;
				tail = newPtr ;
			}
			else {
				tail->next = newPtr ;
				tail = newPtr ;
			}
		} // 加資料進Queue

        //刪除Queue頂端資料
		void deQueue(){
            if(head!=tail){
                QueueNode *temp = head->next ;
				delete head ;
				head = temp ;
			}else if( head!=NULL ){
                delete head ;
                head=NULL;
                tail=NULL;
			}
		}//刪除Queue頂端資料

        //檢查Queue長度
        int Qgetline(){
            int n=0;
            QueueNode *temp=head;
            while(temp!=NULL){
                n++;
                temp=temp->next;
            }
            return n;
        }//計算資料總數量

        //列印Queue資料
		void printQueue(){
			QueueNode *temp = head ;
			int i=1;
			while( temp != NULL ) {
				cout << endl <<"("<<i<<")\t"<< temp->OID << '\t' << temp->Arrival << '\t' << temp->Duration << '\t' << temp->TimeOut ;
				temp = temp->next ;
				i++;
			}
		} // 輸出Queue

		int getAri(int n){ //比Arival大小，用來做龜殼整理
			int i = 0 ;
			if(n==0){
				return head->Arrival ;
			}	//如果是比頭的話
			else { // 2,3,4 etc
			  QueueNode *temp = head ; //借顆頭來用
			  while (i<n){
			  	temp=temp->next ;
			  	i++ ;
			  }
			  return temp->Arrival ;
			 }
		}

		int getOID(int n){ //比OID大小，用來做龜殼整理
			int i = 0 ;
			if(n==0){
				return head->OID ;
			}	//如果是比頭的話
			else { // 2,3,4 etc
			  QueueNode *temp = head ; //借顆頭來用
			  while (i<n){
			  	temp=temp->next ;
			  	i++ ;
			  }
			  return temp->OID ;
			 }
		}

		void swapQ(int a, int b ){ //拿來交換Queue內的資料
			QueueNode *tempa = head ;
			QueueNode *tempb = head ;
			int na=0, nb=0 ; //拿來算術用
			int n1, n2, n3, n4 ;
			while(na < a) {
				tempa = tempa->next ;
				na++ ;
			}//把A點移到要交換位置 A

			while(nb < b){
				tempb = tempb->next ;
				nb++ ;
			}//把B點移到交換位置B

			n1 = tempa->OID ;
			n2 = tempa->Arrival ;
			n3 = tempa->Duration ;
			n4 = tempa->TimeOut ;

			tempa->OID = tempb->OID ;
			tempa->Arrival = tempb->Arrival ;
			tempa->Duration = tempb->Duration ;
			tempa->TimeOut = tempb->TimeOut ;

			tempb->OID = n1 ;
			tempb->Arrival = n2 ;
			tempb->Duration = n3 ;
			tempb->TimeOut = n4 ;

		}

		void shell_sort(int n){ // 希爾排序法 Shell Sort
            int span, i;
            span = n/2;
            while(span>=1){
                for(i = span; i<n ; i++){
                    for(int j=i; j>=span && getAri(j-span)>=getAri(j) ; j-=span){
                        if(getAri(j-span)==getAri(j) && getOID(j-span)<getOID(j));
                        else swapQ(j-span,j);
                    }
				  }
                span = span/2;
            }
		}

        //排序完寫檔輸出
		void SortedFile( const char *tempFile, string headLine ) {
			ofstream outFile ;
			outFile.open( tempFile ) ;

			outFile << headLine ;

			QueueNode *temp = head ;
			while( temp != NULL ) {
				outFile << endl << temp->OID << '\t' << temp->Arrival << '\t' << temp->Duration << '\t' << temp->TimeOut ;
				temp = temp->next ;
			}

			outFile.close() ;

		} // 建立sorted檔案

        //寫檔輸出
		void OutputFile( const char *tempFile, List list , int mode ) {
			ofstream outFile ;
			outFile.open( tempFile ) ;

			outFile << "	[Abort Jobs]" ;
			if(mode==3)
                outFile << endl << "	OID	CID	Abort	Delay";
			else
                outFile << endl << "	OID	Abort	Delay" ;

			int num1, num2, num3 ,num4;
			int files = 1 ;
			while( list.OutAbort( num1, num2, num3 ) != false ) {
                outFile << endl << "[" << files << "]" << "	" << num1 ;
                if(mode==3){
                    list.OutAbortCIO(num4);
                    outFile<<'\t'<<num4;
                }
                outFile << "	" << num2 << "	" << num3 ;
                files++ ;
                list.deAbort();
			}

			outFile << endl << "	[Jobs Done]" ;
			if(mode==3)
                outFile << endl << "	OID	CID	Departure	Delay" ;
            else
                outFile << endl << "	OID	Departure	Delay" ;

			files = 1 ;
			while( list.OutDone( num1, num2, num3 ) != false ) {
                outFile << endl << "[" << files << "]" << "	" << num1 ;
                if(mode==3){
                    list.OutDoneCIO(num4);
                    outFile<<'\t'<<num4;
                }
                outFile << "	" << num2 << "	" << num3 ;
                files++ ;
                list.deDoen();
			}

            //寫入平均延遲時間和平均成功數
            outFile << "\n[Average Delay]	"<<fixed<< setprecision(2)<<DelaySum<<" ms\n"<<"[Success Rate]	"<<Success*100<<" %\n";


			outFile.close() ;
		}

        //任務3處理
		void ImplementQueue3( List & list ) {
            Queue Waiting2;
            DelaySum=0;     //總延遲
            Success=1;      //成功數
            int n=Qgetline();

			list.enDone( head->OID, head->Arrival + head->Duration, 0 ) ;
			list.enDoenCIO(1);
			int time1 = head->Arrival + head->Duration ;   //CPU-1 處理結束時間
			int time2 = 0;          //CPU-2 處理結束時間
			int doing=0;            //執行CPU的優先序
			int things = 0 ;        //CPU-1 等待隊伍
			int things2 = 0 ;        //CPU-2 等待隊伍
			deQueue();

            //還有未抵達或等待隊伍未消完
			while( !QisEmpty() || things>0 ||things2>0 ) {

            /*/檢視每一個Step
            cout<<"------------------\n";
            if(!QisEmpty()) printQueue();  //未抵達和未處理隊伍
            cout<<endl<<"Waiting 1 :"<<things<<"\ttime1 :"<<time1;
            if(things>0) printfWait(time1); //排隊隊伍
            cout<<endl<<"Waiting 2 :"<<things2<<"\ttime2 :"<<time2;
            if(things2>0) Waiting2.printfWait(time2); //排隊隊伍
			cout<<endl;
            list.printfAbortCIO();     //中止掉的隊伍
            cout<<endl;
            list.printfDoneCIO();      //完成的隊伍
            cout<<"\nDOING="<<doing<<endl;
			system("pause");/*/


                //下一個抵達時間小於資料的處理結束的時間
                if( !QisEmpty() && head->Arrival < time1 && head->Arrival<time2){

                    if(things<NumInQueue||things2<NumInQueue){  //排隊長度小於3，可以排隊
                        //比較先排哪一個
                        if(things>things2){ //排CPU-2
                            Waiting2.enWait(head) ;
                            deQueue();
                            things2++;
                        }
                        else{   //排CPU-1
                            enWait( head ) ;
                            deQueue();
                            things++ ;
                        }
                    }

                    else{       //超過排隊上限
                        list.enAbort( head->OID , head->Arrival , 0 ) ;
                        list.enAbortCIO(0);
                        deQueue();
                    }
                }

                //下一個抵達時間大於或等於處理結束的時間
                else{
                    //分類目前先處理的CPU，0代表可以直接處理完成，1代表先處理CPU-1的排隊隊伍，2代表先處理CPU-2的排隊隊伍
                    if(!QisEmpty()&&(((head->Arrival>=time1&&things==0&&(head->Arrival+head->Duration<time2||things2==0)))
                       ||(head->Arrival>=time2&&things2==0&&(head->Arrival+head->Duration<time1||things==0))))doing=0;
                    else if((time1<=time2&&things>0)||things2==0)doing=1;
                    else doing=2;

                    //排隊隊伍為空，可以直接完成處理
                    if( doing==0 && (things==0 ||things2==0)){
                        //跳至下一個未處理資料，並完成處理
                        if(head->Arrival>=time1 && things==0){
                            time1=head->Duration+head->Arrival;
                            list.enDone(head->OID,time1,0);
                            list.enDoenCIO(1);
                            deQueue();
                            Success++;
                        }else{
                            time2=head->Duration+head->Arrival;
                            list.enDone(head->OID,time2,0);
                            list.enDoenCIO(2);
                            deQueue();
                            Success++;
                        }
                    }
                    //等待的第一位的TimeOut小於處理結束的時間，無法處理
                    else if( (doing==1 && WaitHead->TimeOut <= time1)
                            ||(doing==2 && Waiting2.WaitHead->TimeOut <= time2)){
                        //終止排隊
                        if( doing==1 && WaitHead->TimeOut <= time1){
                            list.enAbort( WaitHead->OID , time1 , time1-WaitHead->Arrival );
                            list.enAbortCIO(1);
                            DelaySum+=time1-WaitHead->Arrival;
                            deWait();
                            things--;
                        }
                        if( doing==2 && Waiting2.WaitHead->TimeOut <= time2){
                            list.enAbort( Waiting2.WaitHead->OID , time2 , time2-Waiting2.WaitHead->Arrival );
                            list.enAbortCIO(2);
                            DelaySum+=time2-Waiting2.WaitHead->Arrival;
                            Waiting2.deWait();
                            things2--;
                        }
                    }
                    //等待第一位有辦法在TimeOut的時限內完成
                    else if( (doing==1 && time1+WaitHead->Duration <= WaitHead->TimeOut)
                        ||(doing==2 && time2+Waiting2.WaitHead->Duration <= Waiting2.WaitHead->TimeOut)){
                        //更新時間，並新增完成隊伍
                        if(doing==1 && time1+WaitHead->Duration <= WaitHead->TimeOut){
                            time1+=WaitHead->Duration;
                            list.enDone( WaitHead->OID , time1 , time1-WaitHead->Arrival-WaitHead->Duration );
                            list.enDoenCIO(1);
                            Success++;
                            DelaySum+=time1-WaitHead->Arrival-WaitHead->Duration;
                            deWait();
                            things--;
                        }
                        else if(doing==2 && time2+Waiting2.WaitHead->Duration <= Waiting2.WaitHead->TimeOut){
                            time2+=Waiting2.WaitHead->Duration;
                            list.enDone( Waiting2.WaitHead->OID , time2 , time2-Waiting2.WaitHead->Arrival-Waiting2.WaitHead->Duration );
                            list.enDoenCIO(2);
                            Success++;
                            DelaySum+=time2-Waiting2.WaitHead->Arrival-Waiting2.WaitHead->Duration;
                            Waiting2.deWait();
                            things2--;
                        }
                    //等待第一位沒辦法在TimeOut的時限內完成，可以處理但無法完成
                    }else{
                        //終止處理，並更新時間
                        if(doing==1){
                            list.enAbort( WaitHead->OID , WaitHead->TimeOut ,WaitHead->TimeOut-WaitHead->Arrival );
                            list.enAbortCIO(1);
                            DelaySum+=WaitHead->TimeOut-WaitHead->Arrival;
                            time1=WaitHead->TimeOut;
                            deWait();
                            things--;
                        }
                        else if(doing==2){
                            list.enAbort( Waiting2.WaitHead->OID , Waiting2.WaitHead->TimeOut ,Waiting2.WaitHead->TimeOut-Waiting2.WaitHead->Arrival );
                            list.enAbortCIO(2);
                            DelaySum+=Waiting2.WaitHead->TimeOut-Waiting2.WaitHead->Arrival;
                            time2=Waiting2.WaitHead->TimeOut;
                            Waiting2.deWait();
                            things2--;
                        }
                    }
                }
            } //整理完全部資料

        DelaySum/=n; //計算平均延遲
        Success/=n;  //計算平均成功數

		}

        //任務2處理
		void ImplementQueue( List & list ) {

            DelaySum=0;     //總延遲
            Success=1;      //成功數
            int n=Qgetline();

			list.enDone( head->OID, head->Arrival + head->Duration, 0 ) ;
			int time = head->Arrival + head->Duration ;   //第一筆資料的結束時間
			int things = 0 ;        //等待隊伍
			deQueue();

            //還有未抵達或等待隊伍未消完
			while( !QisEmpty() || things>0 ) {

            /*/檢視每一個Step
            cout<<"------------------\n";
            if(!QisEmpty()) printQueue();  //未抵達和未處理隊伍
            cout<<endl<<"Waiting : "<<things<<endl;
            if(things>0) printfWait(time); //排隊隊伍
			cout<<endl;
            list.printfAbort();     //中止掉的隊伍
            cout<<endl;
            list.printfDone();      //完成的隊伍
			system("pause");/*/

                //下一個抵達時間小於資料的處理結束的時間
                if( !QisEmpty() && head->Arrival < time ){

                    if(things<NumInQueue){  //排隊長度小於3
                        enWait( head ) ;
                        deQueue();
                        things++ ;
                    }
                    else{       //超過排隊上限
                        list.enAbort( head->OID , head->Arrival , 0 ) ;
                        deQueue();
                    }
                }

                //下一個抵達時間大於或等於處理結束的時間
                else{
                    //排隊隊伍為空
                    if( things==0 ){
                        //跳至下一個未處理資料，並完成
                        time=head->Duration+head->Arrival;
                        list.enDone(head->OID,time,0);
                        deQueue();
                        Success++;
                    }
                    //等待的第一位的TimeOut超過處理結束的時間
                    else if( WaitHead->TimeOut <= time ){
                        //終止排隊
                        list.enAbort( WaitHead->OID , time , time-WaitHead->Arrival );
                        DelaySum+=time-WaitHead->Arrival;
                        deWait();
                        things--;
                    }
                    //等待第一位有辦法在TimeOut的時限內完成
                    else if( time+WaitHead->Duration <= WaitHead->TimeOut ){
                        //更新時間，並新增完成隊伍
                        time+=WaitHead->Duration;
                        list.enDone( WaitHead->OID , time , time-WaitHead->Arrival-WaitHead->Duration );
                        Success++;
                        DelaySum+=time-WaitHead->Arrival-WaitHead->Duration;
                        deWait();
                        things--;
                    //等待第一位沒辦法在TimeOut的時限內完成
                    }else{
                        //終止處理，並更新時間
                        list.enAbort( WaitHead->OID , WaitHead->TimeOut ,WaitHead->TimeOut-WaitHead->Arrival );
                        DelaySum+=WaitHead->TimeOut-WaitHead->Arrival;
                        time=WaitHead->TimeOut;
                        deWait();
                        things--;
                    }
                }
            } //整理完全部資料

        DelaySum/=n; //計算平均延遲
        Success/=n;  //計算平均成功數

		}
};

string fileName="";

void mission1() {
	double StartTime ;
	double EndTime ;

	Queue queue ;
	ifstream ifs ;

	cin >> fileName ;

	string InfileName = "input" + fileName + ".txt" ;

	const char *tempFile = InfileName.c_str();

	ifs.open(tempFile) ; // 開檔案

	if(!ifs.is_open()) {
		cout << endl <<"### "<< InfileName << " does not exist! ###\n" ;
	}
	else {

		string headLine ;
		getline(ifs,headLine) ; // 讀第一行的格式

		int num1, num2, num3, num4 ;
		int i = 0 ;
		StartTime = clock() ;

		while( ifs >> num1 >> num2 >> num3 >> num4 ) {
			queue.enQueue(num1, num2, num3, num4) ;
			i++;
		}
        //如果檔案裡沒有資料
        if(i==0){ cout<< endl <<"### Get nothing from the file "<< InfileName <<" ! ###";   return ; }

		EndTime = clock() ;
		double ReadingTime = EndTime - StartTime ;

		ifs.close() ;
		// 讀檔
        cout << endl <<"\t"<<headLine ;
		queue.printQueue() ;


		StartTime = clock() ;
		queue.shell_sort(i) ;
		cout << endl ;


		EndTime = clock() ;
		double SortedTime = EndTime - StartTime ;
		// 排列

		string OutFileName = "sorted" + fileName + ".txt" ;
		tempFile = OutFileName.c_str() ;

		StartTime = clock() ;
    	queue.SortedFile( tempFile, headLine ) ;
    	EndTime = clock() ;
    	double WritingTime = EndTime - StartTime ;
    	// 建檔

        printf("\nReading data: %.0f clocks (%.2f ms).",ReadingTime,ReadingTime);
		printf("\nSorting data: %.0f clocks (%.2f ms).",SortedTime,SortedTime);
		printf("\nWriting data: %.0f clocks (%.2f ms).",WritingTime,WritingTime);


		cout << endl << endl << "see " << tempFile << endl ;
		// 輸出

	}

} // mission1

void mission2() {

	Queue queue ;
	List list ;

	ifstream ifs ;

	if(fileName.empty()) cin >> fileName ; //如果沒有輸入過指令一，就輸入檔案名稱

	string InfileName = "sorted" + fileName + ".txt" ;

	const char *tempFile = InfileName.c_str();

	ifs.open(tempFile) ; // 開檔案

	if(!ifs.is_open()) {
		cout << endl <<"### "<< InfileName << " does not exist! ###\n" ;
	}
	else {

		string headLine ;
		getline(ifs,headLine) ; // 讀第一行的格式

		int OID, Arrival, Duration, TimeOut ;

		int n=0;

		while( ifs >> OID >> Arrival >> Duration >> TimeOut ) {
			queue.enQueue( OID, Arrival, Duration, TimeOut ) ;
			n++;
		}
		//如果沒有資料
		if(n==0){ cout << endl << "### Get nothing from the file "<<InfileName<<" ! ###" ; return ;}

		string OutFileName = "output" + fileName + ".txt" ;
		tempFile = OutFileName.c_str() ;

		queue.ImplementQueue( list ) ;

		queue.OutputFile( tempFile, list , 2 ) ;


        cout << endl << "The simulation is running..." ;
        cout << endl << "See output" << fileName << ".txt\n" ;
	}

} // mission2

void mission3(){

    Queue queue ;
	List list ;

	ifstream ifs ;

	if(fileName.empty()) cin >> fileName ; //如果沒有輸入過指令一，就輸入檔案名稱

	string InfileName = "sorted" + fileName + ".txt" ;

	const char *tempFile = InfileName.c_str();

	ifs.open(tempFile) ; // 開檔案

	if(!ifs.is_open()) {
		cout << endl <<"### "<< InfileName << " does not exist! ###\n" ;
	}
	else {

		string headLine ;
		getline(ifs,headLine) ; // 讀第一行的格式

		int OID, Arrival, Duration, TimeOut ;

		int n=0;

		while( ifs >> OID >> Arrival >> Duration >> TimeOut ) {
			queue.enQueue( OID, Arrival, Duration, TimeOut ) ;
			n++;
		}
		//如果沒有資料
		if(n==0){ cout << endl << "### Get nothing from the file "<<InfileName<<" ! ###" ; return ;}

		string OutFileName = "double" + fileName + ".txt" ;
		tempFile = OutFileName.c_str() ;

		queue.ImplementQueue3( list ) ;

		queue.OutputFile( tempFile, list , 3 ) ;


        cout << endl << "The simulation is running..." ;
        cout << endl << "See double" << fileName << ".txt\n" ;
	}


}


int main() {

    char command ;


    do{

    	cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
    	cout << endl << "* 0. Quit                            *" ;
    	cout << endl << "* 1. Sort a file                     *" ;
    	cout << endl << "* 2. Simulate one FIFO queue         *" ;
    	cout << endl << "* 3. Simulate two queues by SQF      *" ;
    	cout << endl << "**************************************" ;
    	cout << endl << "Input a command(0, 1, 2, 3): " ;
	    cin >> command ;

	    cin.ignore(MAX_LEN, '\n' ) ;
	    // 清掉在buffer的指令

	    if ( command == '1' ) {
	    	cout << endl << "Input a file number:" ;
	    	mission1() ;
		} // 跑練習一

		else if ( command == '2' ) {
			if(fileName.empty()) cout << endl << "Input a file number:" ;
	    	mission2() ;
		} // 跑練習一

        else if (command == '3' ){
            if(fileName.empty()) cout << endl << "Input a file number:" ;
	    	mission3() ;
        }
		else if( command == '0' ) break ;

		else cout << endl << "Command does not exist!\n" ;

	}while(true) ;

    system("pause") ;
    return 0 ;

} // main

