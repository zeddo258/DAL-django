//DS1ex3_20_11127247_11127244
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<time.h>
#include <iomanip>

using namespace std;

class Sort {
	
	public:
		vector<int>column;
		vector<string>type;
		vector< vector<int> >row,copyrow;
		int k=0;
		
		bool open(string input){
	        fstream file;
    	    file.open(input.c_str());
        
        	if ( !file.is_open())
        		return false;
        	
        	return true;
		}
		
		void load(string input){ //Ū�� 
			int k =0;
			string sort;
			int data;
			fstream file;
			file.open( input.c_str() );
			
			for(int i = 0; i<4 ; i++){
				file>>sort;
				if(!file.fail())
					type.push_back(sort) ;

			} 
			file>>data;
			while(!file.eof()){
				
				for(int i=0;i<4;i++){
					column.push_back(data);
					file>>data;
				}
					
				row.push_back(column);	
				column.clear();
				k++;
			}
			file.close();
			for(int t=0;t<type.size();t++){
				
        	       cout<<"\t"<<type[t];
        	}
        	
        	cout<<endl;
   			for(int t=0;t<row.size();t++){
				cout<<"("<<t+1<<")\t";
        	    cout<<row[t][0]<<"\t"<<row[t][1]<<"\t"<<row[t][2]<<"\t"<<row[t][3]<<endl;
        	}
			
		}
	    void shellsort() {  //�Ƨ� 
	    	vector< int > temp;
	    	int num,num2;
	    
  			for (int gap = row.size()/2; gap > 0; gap /= 2){ 

        		for (int i = gap; i <row.size() ;i++){
            		temp = row[i]; 
  
           			int j;        
            		for (j = i; j >= gap && row[j - gap][1] >= temp[1]; j -= gap) {
						if(row[j - gap][1] == temp[1]){
							if(row[j - gap][0] < temp[0]){
								row[j] = row[j - gap]; 
								row[j - gap]=temp;
								temp=row[j];
							}
						}
   						
						   row[j] = row[j - gap]; 
                			
        	        }
              		
            		row[j] = temp; 

        		} 
    		}

   		}

        string outfile(string input){
        	ofstream out; // �إ߿�X��y����
        	string file="sorted"+input+".txt";
        	out.open(file.c_str()); 
   			if(out.fail()){ //�T�{�O�_�}�Ҧ��\
        		
        		
        		exit(1);
    		}
    		string b ;
    		for(int i=0;i<type.size();i++){
    			b = type[i];
    			out<<b<<"\t";
    				
			}
			
			type.clear();
			out<<endl;
				
			int a ;
    		for(int i=0;i<row.size();i++){
    			for(int j=0;j<row[i].size();j++){
    				a = row[i][j];
    				out<<a<<"\t";
    				
				}
				
				out<<endl;
				
			}
			
			out.close();
   			row.clear();
   			return file;
		}
		
		
};

class Queue{
	
	public:
		vector<int>column;
		vector< vector<int> >row;
		vector< vector<int> >done;
		vector< vector<int> >abort;
		vector< vector<int> >queue;
		vector< string >type;
		
		int load(string input){
			int k =0;
			string sort;
			int data;
			fstream file;
			file.open( input.c_str() );
			
			for(int i = 0; i<4 ; i++){
				file>>sort;
				if(!file.fail())
					type.push_back(sort) ;

			} 
			file>>data;
			while(!file.eof()){
				
				for(int i=0;i<4;i++){
					column.push_back(data);
					file>>data;
				}
					
				row.push_back(column);	
				column.clear();
				k++;
			}
			file.close();	
			
			return k;
		}
		
		void abortt(int num,int abortt,int delay){ 
			column.push_back(queue[num][0]);
			column.push_back(abortt);
			column.push_back(delay);
				
			abort.push_back(column);
			column.clear();
			
		}
		
		void donee(int num,int departure,int delay){  
			
			column.push_back(row[num][0]);
			column.push_back(departure);
			column.push_back(delay);

			done.push_back(column);			
			column.clear();
			
		}
		
		void abortt2(int num,int abortt,int delay){
			column.push_back(row[num][0]);
			column.push_back(abortt);
			column.push_back(delay);
				
			abort.push_back(column);
			column.clear();
			
		}
		
		void donee2(int num,int departure,int delay){
			
			column.push_back(queue[num][0]);
			column.push_back(departure);
			column.push_back(delay);
						
			done.push_back(column);
			column.clear();
			
		}
		
		void enqueue(int i){ //push
			for(int j=0;j<row[i].size();j++){
				column.push_back(row[i][j]);

			}
			queue.push_back(column);
			column.clear();
					
		}
		
		void dequeue(){
			queue.erase(queue.begin());
		}
		
		void dequeue2(){
			row.erase(row.begin());
		}
		
		int Qsize(){
			return queue.size();
		}
		
		int Rsize(){
			return row.size();
		}
		
		int QArrival(int num){
			int temp=queue[num][1];
			return temp;
		}
		
		int QDuration(int num){
			int temp=queue[num][2];
			return temp;
		}
		
		int QTimeout(int num){
			int temp=queue[num][3];
			return temp;
		}
		
		int setArrival(int num){
			int temp=row[num][1];
			return temp;
		}
		
		int setDuration(int num){
			int temp=row[num][2];
			return temp;
		}
		
		int setTimeout(int num){
			int temp=row[num][3];
			return temp;
			
		}
		
		void outfile(string input){
        	ofstream out; // �إ߿�X��y����
        	string file="output"+input+".txt";
        	out.open(file.c_str()); 

   			if(out.fail()){ 
        		exit(1);
    		}
    		int b ;
    		
    		out<<"\t"<<"[Abort Jobs]"<<endl<<"\t"<<"OID"<<"\t"<<"Abort"<<"\t"<<"Delay"<<endl;
    		for(int i=0;i<abort.size();i++){
    			out<<"["<<i+1<<"]";
    			for(int j=0;j<abort[i].size();j++){
    				b = abort[i][j];
    				out<<"\t"<<b;   				
				}			
				out<<endl;		
			}
			
			out<<"\t"<<"[Jobs Done]"<<endl<<"\t"<<"OID"<<"\t"<<"Departure"<<"\t"<<"Delay"<<endl;
			int a ;
    		for(int i=0;i<done.size();i++){
    			out<<"["<<i+1<<"]";
    			for(int j=0;j<done[i].size();j++){
    				a = done[i][j];
    				out<<"\t"<<a;			
				}
				out<<endl;
			}
			
			double total = abort.size()+done.size();
			double sum = 0;
			
			for(int i=0;i<abort.size();i++){
				sum = sum + abort[i][2] ;	
			}
			for(int j=0;j<done.size();j++){
				sum = sum + done[j][2] ;
			}

			out << "[Average Delay] " << fixed << setprecision(2) << sum/(abort.size() + done.size())<< " ms";
			out << "\n[Success Rate]  "	<< 	fixed << setprecision(2) <<(done.size()/total)*100 << " %";
			
			abort.clear();
			done.clear();
			out.close();
   			
		}
		
		void task2(string input){
			cout << "\nThe simulation is running...\n";		
			cout << "See output" << input << ".txt\n\n";
		}
	

};


int main(){
	Sort shell;
	Queue doqueue;
	double START,END;
	cout<<"**** Simulate FIFO Queues by SQF *****"<<endl;
    cout<<"* 0. Quit                            *"<<endl;
    cout<<"* 1. Sort a file                     *"<<endl;
    cout<<"* 2. Simulate one FIFO queue         *"<<endl;		
	cout<<"**************************************"<<endl;
		
	cout<<"Input a command(0, 1, 2):";
	int command;
	string num;
	cin>>command;
	
	while(command!=0){
		
		if(command==1){
			
			cout<<endl<<"Input a file number:";
			cin>>num;
			cout << endl;
	
        	string file = "Input" + num + ".txt";
        	if ( !shell.open(file))
        		cout << "### input" << num << ".txt does not exist! ###\n\n";
			else {
				START=clock();
				shell.load(file);
				END=clock();
				cout<<"\nReading data:"<<(END-START)<< " clocks (" <<(END-START)/100 << " ms)" << endl;
				START=clock();
				shell.shellsort();
				END=clock();
				cout<<"Sorting data:"<<(END-START)<< " clocks (" <<(END-START)/100 << " ms)" << endl;
	   			START=clock();
				string saveas=shell.outfile(num);
				END=clock();
				cout<<"Writing data:"<<(END-START)<< " clocks (" <<(END-START)/100 << " ms)" << endl;
				cout<<endl<<"See "<<saveas<<endl<<endl; 
			}
			
		}
		else if(command==2){
			string file="sorted"+num+".txt";
			int total=doqueue.load(file);
			int departure,duration,timeout,abort,delay;
			int i=0;
			departure=doqueue.setArrival(i)+doqueue.setDuration(i);//�Ĥ@����ƪ����}�ɶ� 
			delay=0;
			doqueue.donee(i,departure,delay); //�����M��(�ĴX�����,���}�ɶ�,����) 
			doqueue.dequeue2();
			i=0;
			while(doqueue.Rsize()!=0){
				
				if(doqueue.setArrival(i)<departure&&doqueue.Qsize()<3){ //�U�@����ƨ�F�ɶ�<�W�@�����}�ɶ� 
					doqueue.enqueue(i); //�ƶ� 
				}
				else if(doqueue.setArrival(i)<departure&&doqueue.Qsize()==3){
					doqueue.abortt2(i,doqueue.setArrival(i),0); //�U�@����ƨ�F�ɶ�<�W�@�����}�ɶ� ���ƶ����F�ҥHabort
				}
				else if(doqueue.setArrival(i)>=departure&&doqueue.Qsize()!=0){ //�@����ƨ�F�ɶ�>�W�@�����}�ɶ� 
					//�B�z�ƶ������ 
					int j=0;  
					 j=0;
					while(doqueue.setArrival(i)>=departure&&doqueue.Qsize()>0){ 
						
						if(departure>doqueue.QTimeout(j)&&doqueue.Qsize()!=0){ //�W�@��������}�ɶ�>�o�@���O���ɶ� 
							doqueue.abortt(j,departure,departure-doqueue.QArrival(j)); 
							doqueue.dequeue();
						}
						else if(departure<=doqueue.QTimeout(j)&&doqueue.Qsize()!=0){ //�W�@��������}�ɶ�<�o�@���O���ɶ�  

								if(departure+doqueue.QDuration(j)>doqueue.QTimeout(j)){
									doqueue.abortt(j,doqueue.QTimeout(j),doqueue.QTimeout(j)-doqueue.QArrival(j));  
									departure=doqueue.QTimeout(j);
									doqueue.dequeue();
										
								}
								else if (departure+doqueue.QDuration(j)<=doqueue.QTimeout(j)){
									doqueue.donee2(j,departure+doqueue.QDuration(j),departure+doqueue.QDuration(j)-doqueue.QDuration(j)-doqueue.QArrival(j));
									departure= departure+doqueue.QDuration(j);
									doqueue.dequeue();
								}
						}
						
					}
					
								
					if(doqueue.Qsize()==0){
					
						doqueue.donee(i,doqueue.setArrival(i)+doqueue.setDuration(i),0);
						departure=doqueue.setArrival(i)+doqueue.setDuration(i);
						
					}
					else
						doqueue.enqueue(i);	
	
				}

				doqueue.dequeue2();		
				
			}
			int j=doqueue.Qsize();
			int k = 0;
				while(doqueue.Qsize()!=0){ //�C����ƳB�z�����C�٦��F�� 
				
					if(departure>=doqueue.QTimeout(k)){ //�W�@��������}�ɶ�>�o�@���O���ɶ� 
							doqueue.abortt(k,departure,departure-doqueue.QArrival(k)); //���� 
							doqueue.dequeue();
					}
					else if(departure<doqueue.QTimeout(k)){ //�W�@��������}�ɶ�<�o�@���O���ɶ�  
						if(departure+doqueue.QDuration(k)>doqueue.QTimeout(k)){
							doqueue.abortt(k,doqueue.QTimeout(k),doqueue.QTimeout(k)-doqueue.QArrival(k)); 
							departure=doqueue.QTimeout(k);	 
							doqueue.dequeue();
							
						}
						else if (departure+doqueue.QDuration(k)<doqueue.QTimeout(k)){
							doqueue.donee2(k,departure+doqueue.QDuration(k),departure+doqueue.QDuration(j)-doqueue.QDuration(j)-doqueue.QArrival(j));
							departure= departure+doqueue.QDuration(k);
							doqueue.dequeue();
						}						
      				}	
      			}
			doqueue.outfile(num);
			doqueue.task2(num);

		}
		
		cout<<"**** Simulate FIFO Queues by SQF *****"<<endl;
    	cout<<"* 0. Quit                            *"<<endl;
    	cout<<"* 1. Sort a file                     *"<<endl;
    	cout<<"* 2. Simulate one FIFO queue         *"<<endl;		
		cout<<"**************************************"<<endl;
		
		cout<<"Input a command(0, 1, 2):";
		cin>>command;
    }
    return 0;
}
