//11127129張軒偉 11127136彭晨維 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>

#define MAX_LENGTH 100
#define MAX_QUEUE 3
static char num[MAX_LENGTH];//存使用者輸入的檔案號碼
static bool sucess = true;   //紀錄是否成功開啟檔案 

//讀入的資料 
struct Datain{
	int oid;
	int arrival;
	int duration;
	int timeout;
};
//完成清單的資料 
struct CompletedTask {
    int id;
    int completionTime;
    int delayTime;
};
//取消清單的資料 
struct CanceledTask {
    int id;
    int cancelTime;
    int delayTime;
};
 
//佇列 
class Queue {
private:
	//讀入的資料
    class Job {
    public:
        int jobNumber;
        int entryTime;
        int processingTime;
        int deadline;

        Job(int number, int entry, int processing, int due) 
            : jobNumber(number), entryTime(entry), processingTime(processing), deadline(due) {}
    };

	//資料節點 
    class Node {
    public:
        Job* data;
        Node* next;

        Node(Job* job) : data(job), next(NULL) {}
    };

    Node* front; //佇列頭 
    Node* rear;  //佇列尾 

public:
    Queue() : front(NULL), rear(NULL) {}

	//解構 
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

	//是否空 
    bool isEmpty() {
        return front == NULL;
    }
    
	//加入資料到頭 
    void enqueue(int number, int entry, int processing, int due) {
        Job* newJob = new Job(number, entry, processing, due);
        Node* newNode = new Node(newJob);

        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

	//刪除佇列頭 
    void dequeue() {
        if (!isEmpty()) {
            Node* temp = front;
            Job* job = temp->data;

            front = front->next;
            delete temp;

            if (front == NULL) {
                rear = NULL;
            }
    	}
    }
    
	// 看大小 
    int size() {
        int count = 0;
        Node* current = front;
        while (current != NULL) {
            count++;
            current = current->next;
        }
        return count;
    }
    
	//取得頭 
    Datain getFront() {
    	Datain temp;
        if (!isEmpty()) {
        	Datain temp;
        	temp.oid=front->data->jobNumber;
        	temp.arrival=front->data->entryTime;
        	temp.duration=front->data->processingTime;
        	temp.timeout=front->data->deadline;
            return temp;
        } 
        return temp;
    }
    
};

//讀取檔案中的資料 
std::vector<Datain> Read(std::vector<Datain> datain,FILE *file){
	char line[1024]; 
	fgets(line, sizeof(line), file);
	Datain temp;
	
	int i=0;
    
    while (fscanf(file, "%d\t%d\t%d\t%d", &temp.oid, &temp.arrival, &temp.duration, &temp.timeout) != EOF) {
		datain.push_back(temp);
    }
    
    

    return datain;
}

//開啟input檔案，成功則呼叫讀取 
std::vector<Datain> Set(std::vector<Datain> datain){ 
    char fname[200]="input";//存整個檔案的名字
	
	
	//串接檔名 
	strcat(fname,num);
	strcat(fname,".txt");
	//開啟相應文件 
	FILE *file;
	file = fopen(fname, "rb"); // 以唯讀模式打開文件
	if (file == NULL) {
		//找不到此文件則輸出錯誤訊息 
    	printf("### input%s.txt does not exist! ###\n",num);
    	printf("\n");
    	sucess=false;
	}
	else{ 
		sucess=true;
		datain=Read(datain,file);
		//設定完成關閉文件 
		fclose(file);
	}
	
	return datain; //將設定好的回傳 
}

//開啟sorted檔案，成功則呼叫讀取  
std::vector<Datain> Set2(std::vector<Datain> datain){ 
    char fname[200]="sorted";//存整個檔案的名字
	
	
	//串接檔名 
	strcat(fname,num);
	strcat(fname,".txt");
	//開啟相應文件 
	FILE *file;
	file = fopen(fname, "rb"); // 以唯讀模式打開文件
	if (file == NULL) {
		//找不到此文件則輸出錯誤訊息 
    	printf("### input%s.txt does not exist! ###\n",num);
    	printf("\n");
    	sucess=false;
	}
	else{ 
	    sucess=true;
		datain=Read(datain,file);
		//設定完成關閉文件 
		fclose(file);
	}
	
	return datain; //將設定好的回傳 
}

// 希爾排列 
std::vector<Datain> Sorted(std::vector<Datain> datain){
	int n = datain.size();
	
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            Datain temp = datain[i];
            int j;
            for (j = i; j >= gap && (datain[j - gap].arrival > temp.arrival || (datain[j - gap].arrival == temp.arrival && datain[j - gap].oid > temp.oid)); j -= gap) {
				datain[j] = datain[j - gap];
            }
			datain[j] = temp;
            
        }
    }
    
    return datain;
}

//單處理器模擬 
void Onecpu(std::vector<Datain>& tasks,std::vector<CompletedTask>& completedTasks,std::vector<CanceledTask>& canceledTasks) {
    Queue taskQueue;     
	Datain temp;          //暫存資料 
    int currentTime = 0;  //當前時間 
    Datain currentTask ;  //當前任務 
    bool have=false;      //是否有任務要執行 
    
	//做到佇列和任務清單都為空 
    while (!taskQueue.isEmpty() || !tasks.empty()) {
    	
    	if(taskQueue.isEmpty()&&tasks.front().arrival==currentTime){
    		//佇列為空且有任務抵達
			//直接執行 
    		currentTask=tasks.front();
    		tasks.erase(tasks.begin());
    		have=true;
		}
		else if(taskQueue.isEmpty()){
			// 如果佇列為空，時間前進
            currentTime++;
		}
		else{
			// 如果佇列不為空，處理佇列中的任務
			currentTask=taskQueue.getFront();
            taskQueue.dequeue();
            have=true;
		}
		
		
        if (currentTime > currentTask.timeout&&have) {
        	//要執行的任務已超時 
            canceledTasks.push_back({currentTask.oid, currentTime, currentTime - currentTask.arrival});
            have=false;
        } 
		else if(have) {
			
            // 執行任務
            if (currentTime+currentTask.duration > currentTask.timeout) {
            	//執行中的任務超時 
                canceledTasks.push_back({currentTask.oid, currentTask.timeout, currentTask.timeout-currentTask.arrival});
                currentTime = currentTask.timeout;
                have=false;
            }
            else{
            	//成功執行並跳時 
                completedTasks.push_back({currentTask.oid, currentTime+currentTask.duration, currentTime - currentTask.arrival});
                currentTime += currentTask.duration;
                have=false;
			}
    	}
        
        // 將到達時間符合當前時間的任務加入佇列
        while (!tasks.empty() && tasks.front().arrival < currentTime) {
            if (taskQueue.size() < MAX_QUEUE) {
            	//佇列有空位 
            	temp=tasks.front();
            	taskQueue.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
			}
            else if(taskQueue.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime){
            	//佇列滿但到達時間和現在剛執行完的任務完成時間相同 
            	temp=tasks.front();
            	taskQueue.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
            } 
			else if(tasks.front().arrival<currentTime){
				//加入前已超時 
            	canceledTasks.push_back({tasks.front().oid, tasks.front().arrival, 0});
            }
            
			//刪除清單頭 
            tasks.erase(tasks.begin());
        }

        
    }
    
    
}

//任務一 
void One(){          
	char fname[200]="sorted";//存整個檔案的名字 
	char trash;             //存不需要的換行
	FILE *sortedf;
	std::vector<Datain> datain;
	double read_time;
	double sort_time;
	double write_time;
	
	
	
	//讀入檔案號碼 
	printf("Input a file number:");
	scanf("%s",&num);
	//讀取前面讀取檔案號碼訊息多餘的換行
	scanf("%c",&trash);
	printf("\n");
	
	clock_t start_time = clock();
	datain=Set(datain);
	clock_t end_time = clock();
	// 計算執行時間（以毫秒為單位）
	read_time=((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;
	
	if(sucess){
		//有成功讀到檔案 
		int size=datain.size(); //資料大小 
		int i=0;
	
		//印出讀取到的資料 
		printf("\tOID\tArrival\tDuration\tTimeOut\n");
		while(i<size){
			printf("(%d)\t%d\t%d\t%d\t%d\n",i+1,datain[i].oid,datain[i].arrival,datain[i].duration,datain[i].timeout);
			i++;
		}
		printf("\n");
	
		//排序 
		clock_t start_time2 = clock();
		datain=Sorted(datain);
		clock_t end_time2 = clock();
		sort_time=((double)(end_time2 - start_time2) / CLOCKS_PER_SEC) * 1000.0;
	
		 
		strcat(fname,num);
		strcat(fname,".txt");
		sortedf=fopen(fname, "w");
		
		// 將排序後的資料寫入檔案 
		clock_t start_time3 = clock();
		i=0;
		fprintf(sortedf,"OID\tArrival\tDuration\tTimeOut\n");
		while(i<size){
			fprintf(sortedf,"%d\t%d\t%d\t%d\n",datain[i].oid,datain[i].arrival,datain[i].duration,datain[i].timeout);
			i++;
		}
		clock_t end_time3 = clock();
		write_time=((double)(end_time3 - start_time3) / CLOCKS_PER_SEC) * 1000.0;
	
		//印出三階段處理時間 
		printf("Reading data:%d clocks (%f ms).\n",(int)read_time,read_time);
		printf("Sorting data:%d clocks (%f ms).\n",(int)sort_time,sort_time);
		printf("Writing data:%d clocks (%f ms).\n",(int)write_time,write_time);
		printf("\n");
	
		printf("See %s\n",fname);
		printf("\n");
	
	    //關檔和解構 
		datain.clear();
		fclose(sortedf);
	}
}

//任務二 
void Two(){ 
    Queue taskQueue;   
	char fname[200]="output";//存整個檔案的名字 
	char trash;              //存不需要的換行
	FILE *outputf;           //要輸出的檔案 
	std::vector<Datain> datain;               //讀入的資料 
	std::vector<CompletedTask> completedTasks;//完成清單 
	std::vector<CanceledTask> canceledTasks;  //取消清單 
	double td;     //總delay 
	double ad;     //平均delay
	double rate;   //完成率 

	//若和任務一連續執行則直接開同一份檔案 
	if(num[0]=='\0'){
		//讀入檔案號碼 
		printf("Input a file number:");
		scanf("%s",&num);
		printf("\n");
	}
	//讀取前面讀取檔案號碼訊息多餘的換行
	scanf("%c",&trash);

    //讀檔設定 
	datain=Set2(datain);
	
	if(sucess){
		//有讀到檔案 
		
		//執行清單上的任務 
		Onecpu(datain,completedTasks,canceledTasks);
                  
		strcat(fname,num);
		strcat(fname,".txt");
		outputf=fopen(fname, "w");
	
		//計算數據 
		int totaltask=canceledTasks.size()+completedTasks.size();
		rate=completedTasks.size()/(double)totaltask*100;
	
		//寫入檔案 
		int i = 0;
		int size = canceledTasks.size();
		fprintf(outputf,"\t[Abort Jobs]\n");
		fprintf(outputf,"\tOID\tAbort\tDelay\n");
		while(i<size){
			td=td+canceledTasks[i].delayTime;
			fprintf(outputf,"[%d]\t%d\t%d\t%d\n",i+1,canceledTasks[i].id,canceledTasks[i].cancelTime,canceledTasks[i].delayTime);
			i++;
		}
	
		i = 0;
		size = completedTasks.size();
		fprintf(outputf,"\t[Jobs Done]\n");
		fprintf(outputf,"\tOID\tDeparture\tDelay\n");
		while(i<size){
			td=td+completedTasks[i].delayTime;
			fprintf(outputf,"[%d]\t%d\t%d\t%d\n",i+1,completedTasks[i].id,completedTasks[i].completionTime,completedTasks[i].delayTime);
			i++;
		}
	
		ad=td/(double)totaltask;
	
		fprintf(outputf,"[Average Delay]\t%.2f ms\n",(double)ad);
		fprintf(outputf,"[Success Rate]\t%.2f %%\n",(double)rate);
	
		printf("The simulation is running...\n");
		printf("See %s\n",fname);
		printf("\n");
	
		//關檔和解構 
		datain.clear();
		fclose(outputf);
	}
	
}

int main() {
	char trash;      //存不需要的換行
	char dowhat='\0';//存取指令   
	//不停地詢問要執行的動作直到輸入0退出 
	while(dowhat!='0'){
		//印出介面並讀取指令 
		printf("**** Simulate FIFO Queues by SQF *****\n");
    	printf("* 0. Quit                            *\n");
    	printf("* 1. Sort a file                     *\n");
    	printf("* 2. Simulate one FIFO queue         *\n");
    	printf("**************************************\n");
    	printf("Input a command(0, 1, 2):");
		scanf("%c",&dowhat);
		printf("\n");
	
	    //若非合法指令則輸出錯誤訊息，合法則執行相應動作 
		if(dowhat!='0'&&dowhat!='1'&&dowhat!='2'){
			printf("Command does not exist!\n");
			//讀取前面指令訊息多餘的換行 
			if(dowhat!='\n'){
				scanf("%c",&trash);	
			}
			
			printf("\n");
		}
		else if(dowhat=='1'){
			//指令為1則呼叫One()執行後續程序 
			One();
		}
		else if(dowhat=='2'){
			//指令為1則呼叫One()執行後續程序 
			Two();
		}
	}
	

}

