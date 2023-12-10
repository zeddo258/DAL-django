//10911110-��u�G�A-���ݭ� 10920124 �L���X
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
			int OID ;
			int Departure ;
			int Delay ;
			DoneNode *next ;
		};

		DoneNode *Donehead = NULL ;
		DoneNode *Donetail = NULL ;

		struct AbortNode{
			int OID ;
			int Abort ;
			int Delay ;
			AbortNode *next ;
		};

		AbortNode *Aborthead = NULL ;
		AbortNode *Aborttail = NULL ;

	public:

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

		bool OutDone( int & num1, int & num2, int & num3 ) {
            if( Donehead == NULL ) return false ;
            else {
                num1 = Donehead->OID ;
                num2 = Donehead->Departure ;
                num3 = Donehead->Delay ;

                DoneNode *temp = Donehead->next ;
                delete Donehead ;
                Donehead = temp ;
                return true ;
            }

		}

		bool OutAbort( int & num1, int & num2, int & num3 ) {
			if( Aborthead == NULL ) return false ;
            else {
                num1 = Aborthead->OID ;
                num2 = Aborthead->Abort ;
                num3 = Aborthead->Delay ;

                AbortNode *temp = Aborthead->next ;
                delete Aborthead ;
                Aborthead = temp ;
                return true ;
            }

		}

        //��X�פ�C��
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

        //��X�����C��
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

        void printfWait(int time){
            QueueNode *temp=WaitHead;
            cout<<"\nQueue ("<<time<<"):\n";
            while(temp!=NULL){
                cout<<temp->OID<<'\t'<<temp->Arrival<<'\t'<<temp->Duration<<'\t'<<temp->TimeOut<<endl;
                temp=temp->next;
            }
        }

        bool QisEmpty(){
            if( head==tail && head==NULL) return 1;
            else return 0;
        }//�ˬd�٦��S���Ѿl���

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
		} // �[��ƶiQueue

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
		}//�R��Queue���ݸ��

        int Qgetline(){
            int n=0;
            QueueNode *temp=head;
            while(temp!=NULL){
                n++;
                temp=temp->next;
            }
            return n;
        }//�p�����`�ƶq

		void printQueue(){
			QueueNode *temp = head ;
			int i=1;
			while( temp != NULL ) {
				cout << endl <<"("<<i<<")\t"<< temp->OID << '\t' << temp->Arrival << '\t' << temp->Duration << '\t' << temp->TimeOut ;
				temp = temp->next ;
				i++;
			}
		} // ��XQueue

		int getAri(int n){ //��Arival�j�p�A�ΨӰ��t�߾�z
			int i = 0 ;
			if(n==0){
				return head->Arrival ;
			}	//�p�G�O���Y����
			else { // 2,3,4 etc
			  QueueNode *temp = head ; //�����Y�ӥ�
			  while (i<n){
			  	temp=temp->next ;
			  	i++ ;
			  }
			  return temp->Arrival ;
			 }
		}

		int getOID(int n){ //��OID�j�p�A�ΨӰ��t�߾�z
			int i = 0 ;
			if(n==0){
				return head->OID ;
			}	//�p�G�O���Y����
			else { // 2,3,4 etc
			  QueueNode *temp = head ; //�����Y�ӥ�
			  while (i<n){
			  	temp=temp->next ;
			  	i++ ;
			  }
			  return temp->OID ;
			 }
		}

		void swapQ(int a, int b ){ //���ӥ洫Queue�������
			QueueNode *tempa = head ;
			QueueNode *tempb = head ;
			int na=0, nb=0 ; //���Ӻ�N��
			int n1, n2, n3, n4 ;
			while(na < a) {
				tempa = tempa->next ;
				na++ ;
			}//��A�I����n�洫��m A

			while(nb < b){
				tempb = tempb->next ;
				nb++ ;
			}//��B�I����洫��mB

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

		void shell_sort(int n){ // �ƺ��ƧǪk Shell Sort
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

		} // �إ�sorted�ɮ�

		void OutputFile( const char *tempFile, List list ) {
			ofstream outFile ;
			outFile.open( tempFile ) ;

			outFile << "	[Abort Jobs]" ;
			outFile << endl << "	OID	Abort	Delay" ;
			int num1, num2, num3 ;
			int files = 1 ;
			while( list.OutAbort( num1, num2, num3 ) != false ) {
                outFile << endl << "[" << files << "]" << "	" << num1 << "	" << num2 << "	" << num3 ;
                files++ ;
			}

			outFile << endl << "	[Jobs Done]" ;
			outFile << endl << "	OID	Departure	Delay" ;
			files = 1 ;
			while( list.OutDone( num1, num2, num3 ) != false ) {
                outFile << endl << "[" << files << "]" << "	" << num1 << "	" << num2 << "	" << num3 ;
                files++ ;
			}
			double A;

            //�g�J��������ɶ��M�������\��
            outFile << "\n[Average Delay]	"<<fixed<< setprecision(2)<<DelaySum<<" ms\n"<<"[Success Rate]	"<<Success*100<<" %\n";


			outFile.close() ;
		}

		void ImplementQueue( List & list ) {

            DelaySum=0;     //�`����
            Success=1;      //���\��
            int n=Qgetline();

			list.enDone( head->OID, head->Arrival + head->Duration, 0 ) ;
			int time = head->Arrival + head->Duration ;   //�Ĥ@����ƪ������ɶ�
			int things = 0 ;        //���ݶ���
			deQueue();

            //�٦�����F�ε��ݶ������
			while( !QisEmpty() || things>0 ) {

            /*/�˵��C�@��Step
            cout<<"------------------\n";
            if(!QisEmpty()) printQueue();  //����F�M���B�z����
            cout<<endl<<"Waiting : "<<things<<endl;
            if(things>0) printfWait(time); //�ƶ�����
			cout<<endl;
            list.printfAbort();     //���������
            cout<<endl;
            list.printfDone();      //����������
			system("pause");/*/

                //�U�@�ө�F�ɶ��p���ƪ��B�z�������ɶ�
                if( !QisEmpty() && head->Arrival < time ){
                    if(things<NumInQueue){  //�ƶ����פp��3
                        enWait( head ) ;
                        deQueue();
                        things++ ;
                    }
                    else{       //�W�L�ƶ��W��
                        list.enAbort( head->OID , head->Arrival , 0 ) ;
                        deQueue();
                    }
                }

                //�U�@�ө�F�ɶ��j��ε���B�z�������ɶ�
                else{
                    //�ƶ������
                    if( things==0 ){
                        //���ܤU�@�ӥ��B�z��ơA�ç���
                        time=head->Duration+head->Arrival;
                        list.enDone(head->OID,time,0);
                        deQueue();
                        Success++;
                    }
                    //���ݪ��Ĥ@�쪺TimeOut�W�L�B�z�������ɶ�
                    else if( WaitHead->TimeOut <= time ){
                        //�פ�ƶ�
                        list.enAbort( WaitHead->OID , time , time-WaitHead->Arrival );
                        DelaySum+=time-WaitHead->Arrival;
                        deWait();
                        things--;
                    }
                    //���ݲĤ@�즳��k�bTimeOut���ɭ�������
                    else if( time+WaitHead->Duration <= WaitHead->TimeOut ){
                        //��s�ɶ��A�÷s�W��������
                        time+=WaitHead->Duration;
                        list.enDone( WaitHead->OID , time , time-WaitHead->Arrival-WaitHead->Duration );
                        Success++;
                        DelaySum+=time-WaitHead->Arrival-WaitHead->Duration;
                        deWait();
                        things--;
                    //���ݲĤ@��S��k�bTimeOut���ɭ�������
                    }else{
                        //�פ�B�z�A�ç�s�ɶ�
                        list.enAbort( WaitHead->OID , WaitHead->TimeOut ,WaitHead->TimeOut-WaitHead->Arrival );
                        DelaySum+=WaitHead->TimeOut-WaitHead->Arrival;
                        time=WaitHead->TimeOut;
                        deWait();
                        things--;
                    }
                }
            } //��z���������

        DelaySum/=n; //�p�⥭������
        Success/=n;  //�p�⥭�����\��

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

	ifs.open(tempFile) ; // �}�ɮ�

	if(!ifs.is_open()) {
		cout << endl <<"### "<< InfileName << " does not exist! ###\n" ;
	}
	else {

		string headLine ;
		getline(ifs,headLine) ; // Ū�Ĥ@�檺�榡

		int num1, num2, num3, num4 ;
		int i = 0 ;
		StartTime = clock() ;

		while( ifs >> num1 >> num2 >> num3 >> num4 ) {
			queue.enQueue(num1, num2, num3, num4) ;
			i++;
		}
        //�p�G�ɮ׸̨S�����
        if(i==0){ cout<< endl <<"### Get nothing from the file "<< InfileName <<" ! ###";   return ; }

		EndTime = clock() ;
		double ReadingTime = EndTime - StartTime ;

		ifs.close() ;
		// Ū��
        cout << endl <<"\t"<<headLine ;
		queue.printQueue() ;


		StartTime = clock() ;
		queue.shell_sort(i) ;
		cout << endl ;


		EndTime = clock() ;
		double SortedTime = EndTime - StartTime ;
		// �ƦC

		string OutFileName = "sorted" + fileName + ".txt" ;
		tempFile = OutFileName.c_str() ;

		StartTime = clock() ;
    	queue.SortedFile( tempFile, headLine ) ;
    	EndTime = clock() ;
    	double WritingTime = EndTime - StartTime ;
    	// ����

        printf("\nReading data: %.0f clocks (%.2f ms).",ReadingTime,ReadingTime);
		printf("\nSorting data: %.0f clocks (%.2f ms).",SortedTime,SortedTime);
		printf("\nWriting data: %.0f clocks (%.2f ms).",WritingTime,WritingTime);


		cout << endl << endl << "see " << tempFile << endl ;
		// ��X

	}

} // mission1

void mission2() {

	Queue queue ;
	List list ;

	ifstream ifs ;

	if(fileName.empty()) cin >> fileName ; //�p�G�S����J�L���O�@�A�N��J�ɮצW��

	string InfileName = "sorted" + fileName + ".txt" ;

	const char *tempFile = InfileName.c_str();

	ifs.open(tempFile) ; // �}�ɮ�

	if(!ifs.is_open()) {
		cout << endl <<"### "<< InfileName << " does not exist! ###\n" ;
	}
	else {

		string headLine ;
		getline(ifs,headLine) ; // Ū�Ĥ@�檺�榡

		int OID, Arrival, Duration, TimeOut ;

		int n=0;

		while( ifs >> OID >> Arrival >> Duration >> TimeOut ) {
			queue.enQueue( OID, Arrival, Duration, TimeOut ) ;
			n++;
		}
		//�p�G�S�����
		if(n==0){ cout << endl << "### Get nothing from the file "<<InfileName<<" ! ###" ; return ;}

		string OutFileName = "output" + fileName + ".txt" ;
		tempFile = OutFileName.c_str() ;

		queue.ImplementQueue( list ) ;

		queue.OutputFile( tempFile, list ) ;


        cout << endl << "The simulation is running..." ;
        cout << endl << "See output" << fileName << ".txt\n" ;
	}

} // mission2


int main() {

    char command ;


    do{

    	cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
    	cout << endl << "* 0. Quit                            *" ;
    	cout << endl << "* 1. Sort a file                     *" ;
    	cout << endl << "* 2. Simulate one FIFO queue         *" ;
    	cout << endl << "**************************************" ;
    	cout << endl << "Input a command(0, 1, 2): " ;
	    cin >> command ;

	    cin.ignore(MAX_LEN, '\n' ) ;
	    // �M���bbuffer�����O

	    if ( command == '1' ) {
	    	cout << endl << "Input a file number:" ;
	    	mission1() ;
		} // �]�m�ߤ@

		else if ( command == '2' ) {
			if(fileName.empty()) cout << endl << "Input a file number:" ;
	    	mission2() ;
		} // �]�m�ߤ@

		else if( command == '0' ) break ;

		else cout << endl << "Command does not exist!\n" ;

	}while(true) ;

    system("pause") ;
    return 0 ;

} // main

