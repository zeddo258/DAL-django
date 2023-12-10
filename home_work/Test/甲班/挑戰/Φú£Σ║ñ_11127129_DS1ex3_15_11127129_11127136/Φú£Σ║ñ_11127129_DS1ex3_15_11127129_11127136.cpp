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

struct Datain{
	int oid;
	int arrival;
	int duration;
	int timeout;
};

struct CompletedTask {
    int id;
    int completionTime;
    int delayTime;
    int cpu;
};

struct CanceledTask {
    int id;
    int cancelTime;
    int delayTime;
    int cpu;
};

class Queue {
private:
    class Job {
    public:
        int jobNumber;
        int entryTime;
        int processingTime;
        int deadline;

        Job(int number, int entry, int processing, int due) 
            : jobNumber(number), entryTime(entry), processingTime(processing), deadline(due) {}
    };

    class Node {
    public:
        Job* data;
        Node* next;

        Node(Job* job) : data(job), next(NULL) {}
    };

    Node* front;
    Node* rear;

public:
    Queue() : front(NULL), rear(NULL) {}

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() {
        return front == NULL;
    }

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

    void dequeue() {
        if (!isEmpty()) {
            Node* temp = front;
            Job* job = temp->data;

            front = front->next;
            delete temp;

            if (front == NULL) {
                rear = NULL;
            }

        } else {
            //throw out_of_range("dequeue from an empty queue");
        }
    }

    int size() {
        int count = 0;
        Node* current = front;
        while (current != NULL) {
            count++;
            current = current->next;
        }
        return count;
    }

    Datain getFront() {
        if (!isEmpty()) {
        	Datain temp;
        	temp.oid=front->data->jobNumber;
        	temp.arrival=front->data->entryTime;
        	temp.duration=front->data->processingTime;
        	temp.timeout=front->data->deadline;
            return temp;
        } else {
            //throw out_of_range("getFront from an empty queue");
        }
    }
    
};


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

void Onecpu(std::vector<Datain>& tasks,std::vector<CompletedTask>& completedTasks,std::vector<CanceledTask>& canceledTasks) {
    Queue taskQueue;
	Datain temp;         
    int currentTime = 0;
    Datain currentTask ;
    bool have=false;
    
	
    while (!taskQueue.isEmpty() || !tasks.empty()) {
    	
    	if(taskQueue.isEmpty()&&tasks.front().arrival==currentTime){
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
            canceledTasks.push_back({currentTask.oid, currentTime, currentTime - currentTask.arrival});
            have=false;
        } 
		else if(have) {
			
            // 執行任務
            if (currentTime+currentTask.duration > currentTask.timeout) {
                canceledTasks.push_back({currentTask.oid, currentTask.timeout, currentTask.timeout-currentTask.arrival});
                currentTime = currentTask.timeout;
                have=false;
            }
            else{
            		
                completedTasks.push_back({currentTask.oid, currentTime+currentTask.duration, currentTime - currentTask.arrival});
                currentTime += currentTask.duration;
                have=false;
			}
    	}
        
        // 將到達時間符合當前時間的任務加入佇列
        while (!tasks.empty() && tasks.front().arrival < currentTime) {
            if (taskQueue.size() < MAX_QUEUE) {
            	temp=tasks.front();
            	taskQueue.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
			}
            else if(taskQueue.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime){
            	temp=tasks.front();
            	taskQueue.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
            } 
			else if(tasks.front().arrival<currentTime){
            	canceledTasks.push_back({tasks.front().oid, tasks.front().arrival, 0});
            }
            
			
            tasks.erase(tasks.begin());
        }

        
    }
    
    
}

void Twocpu(std::vector<Datain>& tasks,std::vector<CompletedTask>& completedTasks,std::vector<CanceledTask>& canceledTasks) {
    Queue taskQueue1;    //cpu1佇列 
    Queue taskQueue2;    //cpu2佇列 
	Datain temp;         //暫存資料 
	int currentTime = 0;
    int currentTime1 = 0; //cpu1的時間 
    int currentTime2 = 0; //cpu2的時間 
    int start1=0;        //cpu1任務的開始時間 
    int start2=0;        //cpu2任務的開始時間
    Datain currentTask1 ;//cpu1任務 
    Datain currentTask2 ;//cpu2任務
    /*
    //設定個處理器的第一筆資料 
    if(!tasks.empty()){
    	if(tasks.size()>=2){
    		
    		currentTask1=tasks.front();
    		tasks.erase(tasks.begin());
    		currentTime1=currentTask1.arrival;
    		start1=currentTime1;
    		
    		completedTasks.push_back({currentTask1.oid, start1+currentTask1.duration, start1 - currentTask1.arrival,1});
		    currentTime1 =start1+ currentTask1.duration;
    		
    		currentTask2=tasks.front();
    		tasks.erase(tasks.begin());
    		currentTime2=currentTask2.arrival;
    		start2=currentTime2;
    		
    		completedTasks.push_back({currentTask2.oid, start2+currentTask2.duration, start2 - currentTask2.arrival,2});
		    currentTime2 =start2+ currentTask2.duration;

		}
		else{
			temp=tasks.front();
		    taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
    		tasks.erase(tasks.begin());
    		currentTime1=temp.arrival;
    		start1=currentTime1;
    		
    		completedTasks.push_back({currentTask1.oid, start1+currentTask1.duration, start1 - currentTask1.arrival,1});
		    currentTime1 =start1+ currentTask1.duration;
		}
	}
    */
    
    while (!taskQueue1.isEmpty()||!taskQueue2.isEmpty() ||!tasks.empty()) {
    	//debug用 
    	if(true){
    		//printf("%d %d\n",tasks.front().oid,currentTime);
    		//printf("%d\n",currentTime);
    		//taskQueue1.getFront().arrival
    		//printf("1\n");
		}
		
    	
    	while ((!tasks.empty() &&tasks.front().arrival == currentTime)) {
    		
    		if(taskQueue1.size()==taskQueue2.size()){
    			
    			while(!taskQueue1.isEmpty()&&taskQueue1.getFront().arrival>currentTime1){
    				currentTime1++;
    				//printf("%d %d\n",taskQueue1.getFront().arrival,currentTime1);
				}
				
    			
    			while(!taskQueue1.isEmpty()&&currentTime1<=currentTime&&(currentTime1+taskQueue1.getFront().duration<=currentTime||taskQueue1.getFront().timeout<=currentTime)){
    				
					currentTask1=taskQueue1.getFront();
			        taskQueue1.dequeue(); 
					
			        start1=currentTime1;
    				
    				if (currentTime1 >= currentTask1.timeout) {
		            	canceledTasks.push_back({currentTask1.oid ,currentTime1, start1 - currentTask1.arrival,1});
		        	} 
					else{
		            	// 執行任務
		            	if(start1+currentTask1.duration > currentTask1.timeout){
		            		canceledTasks.push_back({currentTask1.oid, currentTask1.timeout, currentTask1.timeout-currentTask1.arrival,1});
		            	    currentTime1 = currentTask1.timeout;
						}
		            	else {
		            	    completedTasks.push_back({currentTask1.oid, start1+currentTask1.duration, start1 - currentTask1.arrival,1});
		            	    currentTime1 =start1+ currentTask1.duration;
						}
		    		}
				}
				
				
				while(!taskQueue2.isEmpty()&&taskQueue2.getFront().arrival>currentTime2){
    				currentTime2++;
				}
    			
	    		while(!taskQueue2.isEmpty()&&currentTime2<=currentTime&&(currentTime2+taskQueue2.getFront().duration<=currentTime||taskQueue2.getFront().timeout<=currentTime)){
    				
					currentTask2=taskQueue2.getFront();
			        taskQueue2.dequeue(); 
					
			        start2=currentTime2;
    				
    				if (currentTime2 >= currentTask2.timeout) {
		            	canceledTasks.push_back({currentTask2.oid ,currentTime2, start2 - currentTask2.arrival,2});
		        	} 
					else{
		            	// 執行任務
		            	if(start2+currentTask2.duration > currentTask2.timeout){
		            		canceledTasks.push_back({currentTask2.oid, currentTask2.timeout, currentTask2.timeout-currentTask2.arrival,2});
		            	    currentTime2 = currentTask2.timeout;
						}
		            	else {
		            	    completedTasks.push_back({currentTask2.oid, start2+currentTask2.duration, start2 - currentTask2.arrival,2});
		            	    currentTime2 =start2+ currentTask2.duration;
						}
		    		}
				}
				
				
				
				while (!tasks.empty() && tasks.front().arrival <= currentTime) {
				
		            if (taskQueue1.size() < MAX_QUEUE&&taskQueue1.size()<=taskQueue2.size()) {
		            	temp=tasks.front();
		            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
					}
					else if (taskQueue2.size() < MAX_QUEUE) {
		            	temp=tasks.front();
		            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
					}
		            else if(taskQueue1.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime&&taskQueue1.size()<=taskQueue2.size()){
		            	temp=tasks.front();
		            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
		            } 
		            else if(taskQueue2.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime){
		            	temp=tasks.front();
		            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
		            }
					else {
		            	canceledTasks.push_back({tasks.front().oid, tasks.front().arrival, 0,0});
		            }
		            
					
		            tasks.erase(tasks.begin());
		            
		        }
		        
	    		
			}
			else if(taskQueue1.size()<taskQueue2.size()){
				
				while(!taskQueue2.isEmpty()&&taskQueue2.getFront().arrival>currentTime2){
    				currentTime2++;
				}
				//
				while(!taskQueue2.isEmpty()&&currentTime2<=currentTime&&(currentTime2+taskQueue2.getFront().duration<=currentTime||taskQueue2.getFront().timeout<=currentTime)){
    				
					currentTask2=taskQueue2.getFront();
			        taskQueue2.dequeue(); 
					
			        start2=currentTime;
    				
    				if (currentTime >= currentTask2.timeout) {
		            	canceledTasks.push_back({currentTask2.oid ,currentTime2, start2 - currentTask2.arrival,2});
		        	} 
					else{
		            	// 執行任務
		            	if(start2+currentTask2.duration > currentTask2.timeout){
		            		canceledTasks.push_back({currentTask2.oid, currentTask2.timeout, currentTask2.timeout-currentTask2.arrival,2});
		            	    currentTime2 = currentTask2.timeout;
						}
		            	else {
		            	    completedTasks.push_back({currentTask2.oid, start2+currentTask2.duration, start2 - currentTask2.arrival,2});
		            	    currentTime2 =start2+ currentTask2.duration;
						}
		    		}
				}
				
				while (currentTime1 >= currentTask1.timeout) {
		            canceledTasks.push_back({currentTask1.oid ,currentTask1.timeout, start1 - currentTask1.arrival,1});
		        }
				
				while(!taskQueue1.isEmpty()&&currentTime1<=currentTime&&(currentTime1+taskQueue1.getFront().duration<=currentTime||taskQueue1.getFront().timeout<=currentTime)){
    				
					currentTask1=taskQueue1.getFront();
			        taskQueue1.dequeue(); 
					
			        start1=currentTime;
    				
    				if (currentTime >= currentTask1.timeout) {
		            	canceledTasks.push_back({currentTask1.oid ,currentTime1, start1 - currentTask1.arrival,1});
		        	} 
					else{
		            	// 執行任務
		            	if(start1+currentTask1.duration > currentTask1.timeout){
		            		canceledTasks.push_back({currentTask1.oid, currentTask1.timeout, currentTask1.timeout-currentTask1.arrival,1});
		            	    currentTime1 = currentTask1.timeout;
						}
		            	else {
		            	    completedTasks.push_back({currentTask1.oid, start1+currentTask1.duration, start1 - currentTask1.arrival,1});
		            	    currentTime1 =start1+ currentTask1.duration;
						}
		    		}
				}
				
				while (!tasks.empty() && tasks.front().arrival <= currentTime) {
				
		            if (taskQueue1.size() < MAX_QUEUE&&taskQueue1.size()<=taskQueue2.size()) {
		            	temp=tasks.front();
		            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
					}
					else if (taskQueue2.size() < MAX_QUEUE) {
		            	temp=tasks.front();
		            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
					}
		            else if(taskQueue1.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime&&taskQueue1.size()<=taskQueue2.size()){
		            	temp=tasks.front();
		            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
		            } 
		            else if(taskQueue2.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime){
		            	temp=tasks.front();
		            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
		            }
					else if(tasks.front().arrival<currentTime){
		            	canceledTasks.push_back({tasks.front().oid, tasks.front().arrival, 0,0});
		            }
		            
					
		            tasks.erase(tasks.begin());
		            
		        }
				
			}
			else{
				while(!taskQueue1.isEmpty()&&taskQueue1.getFront().arrival>currentTime1){
    				currentTime1++;
    				//printf("%d %d\n",taskQueue1.getFront().arrival,currentTime1);
				}
				
    			//
    			while(!taskQueue1.isEmpty()&&currentTime1<=currentTime&&(currentTime1+taskQueue1.getFront().duration<=currentTime||taskQueue1.getFront().timeout<=currentTime)){
    				
					currentTask1=taskQueue1.getFront();
			        taskQueue1.dequeue(); 
					
			        start1=currentTime1;
    				
    				if (currentTime1 >= currentTask1.timeout) {
		            	canceledTasks.push_back({currentTask1.oid ,currentTime1, start1 - currentTask1.arrival,1});
		        	} 
					else{
		            	// 執行任務
		            	if(start1+currentTask1.duration > currentTask1.timeout){
		            		canceledTasks.push_back({currentTask1.oid, currentTask1.timeout, currentTask1.timeout-currentTask1.arrival,1});
		            	    currentTime1 = currentTask1.timeout;
						}
		            	else {
		            	    completedTasks.push_back({currentTask1.oid, start1+currentTask1.duration, start1 - currentTask1.arrival,1});
		            	    currentTime1 =start1+ currentTask1.duration;
						}
		    		}
				}
				
				
				while(!taskQueue2.isEmpty()&&taskQueue2.getFront().arrival>currentTime2){
    				currentTime2++;
				}
    			
	    		while(!taskQueue2.isEmpty()&&currentTime2<=currentTime&&(currentTime2+taskQueue2.getFront().duration<=currentTime||taskQueue2.getFront().timeout<=currentTime)){
    				
					currentTask2=taskQueue2.getFront();
			        taskQueue2.dequeue(); 
					
			        start2=currentTime2;
    				
    				if (currentTime2 >= currentTask2.timeout) {
		            	canceledTasks.push_back({currentTask2.oid ,currentTime2, start2 - currentTask2.arrival,2});
		        	} 
					else{
		            	// 執行任務
		            	if(start2+currentTask2.duration > currentTask2.timeout){
		            		canceledTasks.push_back({currentTask2.oid, currentTask2.timeout, currentTask2.timeout-currentTask2.arrival,2});
		            	    currentTime2 = currentTask2.timeout;
						}
		            	else {
		            	    completedTasks.push_back({currentTask2.oid, start2+currentTask2.duration, start2 - currentTask2.arrival,2});
		            	    currentTime2 =start2+ currentTask2.duration;
						}
		    		}
				}
				
				
				
				while (!tasks.empty() && tasks.front().arrival <= currentTime) {
				
		            if (taskQueue1.size() < MAX_QUEUE&&taskQueue1.size()<=taskQueue2.size()) {
		            	temp=tasks.front();
		            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
					}
					else if (taskQueue2.size() < MAX_QUEUE) {
		            	temp=tasks.front();
		            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
					}
		            else if(taskQueue1.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime&&taskQueue1.size()<=taskQueue2.size()){
		            	temp=tasks.front();
		            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
		            } 
		            else if(taskQueue2.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime){
		            	temp=tasks.front();
		            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
		            }
					else {
		            	canceledTasks.push_back({tasks.front().oid, tasks.front().arrival, 0,0});
		            }
		            
					
		            tasks.erase(tasks.begin());
		            
		        }
			}
    	}
    	
    	while ((!taskQueue1.isEmpty()||!taskQueue2.isEmpty()) &&tasks.empty()) {
    		
    		while(!taskQueue1.isEmpty()&&taskQueue1.getFront().arrival>currentTime1){
    			currentTime1++;
    			//printf("%d %d\n",taskQueue1.getFront().arrival,currentTime1);
			}
    			
    		while(!taskQueue1.isEmpty()&&currentTime1<=currentTime&&(currentTime1+taskQueue1.getFront().duration<=currentTime||taskQueue1.getFront().timeout<=currentTime)){
    				
				currentTask1=taskQueue1.getFront();
			    taskQueue1.dequeue(); 
					
			    start1=currentTime1;
    				
    			if (currentTime1 >= currentTask1.timeout) {
		           	canceledTasks.push_back({currentTask1.oid ,currentTime1, start1 - currentTask1.arrival,1});
		        } 
				else{
		            // 執行任務
		            if(start1+currentTask1.duration > currentTask1.timeout){
		            	canceledTasks.push_back({currentTask1.oid, currentTask1.timeout, currentTask1.timeout-currentTask1.arrival,1});
		                currentTime1 = currentTask1.timeout;
					}
		            else {
		                completedTasks.push_back({currentTask1.oid, start1+currentTask1.duration, start1 - currentTask1.arrival,1});
		                currentTime1 =start1+ currentTask1.duration;
					}
		    	}
			}
				
			while(!taskQueue2.isEmpty()&&taskQueue2.getFront().arrival>currentTime2){
    			currentTime2++;
			}
    			
	    	while(!taskQueue2.isEmpty()&&currentTime2<=currentTime&&(currentTime2+taskQueue2.getFront().duration<=currentTime||taskQueue2.getFront().timeout<=currentTime)){
    				
				currentTask2=taskQueue2.getFront();
			    taskQueue2.dequeue(); 
					
			    start2=currentTime2;
    				
    			if (currentTime2 >= currentTask2.timeout) {
		        	canceledTasks.push_back({currentTask2.oid ,currentTask2.timeout, start2 - currentTask2.arrival,2});
		       	} 
				else{
		           	// 執行任務
		           	if(start2+currentTask2.duration > currentTask2.timeout){
		           		canceledTasks.push_back({currentTask2.oid, currentTask2.timeout, currentTask2.timeout-currentTask2.arrival,2});
		           	    currentTime2 = currentTask2.timeout;
					}
		           	else {
		          	    completedTasks.push_back({currentTask2.oid, start2+currentTask2.duration, start2 - currentTask2.arrival,2});
		           	    currentTime2 =start2+ currentTask2.duration;
					}
		    	}
			}
			
			currentTime++;
    	}
    	
    	
    	currentTime++;
		
    	
    }
    
    
}
/*
void Twocpu(std::vector<Datain>& tasks,std::vector<CompletedTask>& completedTasks,std::vector<CanceledTask>& canceledTasks) {
    Queue taskQueue1;    //cpu1佇列 
    Queue taskQueue2;    //cpu2佇列 
	Datain temp;         //暫存資料 
    int currentTime1 = 0; //cpu1的時間 
    int currentTime2 = 0; //cpu2的時間 
    int start1=0;        //cpu1任務的開始時間 
    int start2=0;        //cpu2任務的開始時間  
    Datain currentTask1 ;//cpu1任務 
    Datain currentTask2 ;//cpu2任務
    bool have1=false;    //cpu1是否有任務 
    bool have2=false;    //cpu2是否有任務  
    bool do1=false;      //cpu1是否執行了 
    bool do2=false;      //cpu2是否執行了
    
    //設定個處理器的第一筆資料 
    if(!tasks.empty()){
    	if(tasks.size()>=2){
    		
    		currentTask1=tasks.front();
    		tasks.erase(tasks.begin());
    		have1=true;
    		currentTime1=currentTask1.arrival;
    		start1=currentTime1;
    		do1=false;
    		
    		currentTask2=tasks.front();
    		tasks.erase(tasks.begin());
    		have2=true;
    		currentTime2=currentTask2.arrival;
    		start2=currentTime2;
    		do2=false;
		}
		else{
			currentTask1=tasks.front();
    		tasks.erase(tasks.begin());
    		have1=true;
    		currentTime1=currentTask1.arrival;
    		start1=currentTime1;
    		do1=false;
		}
	}

    
	//做到佇列和任務清單都為空
    while (!taskQueue1.isEmpty()||!taskQueue2.isEmpty() ||!tasks.empty()|| have1||have2) {
    	
		//重製處理器狀態 
		if(taskQueue1.isEmpty()&&!have1&&currentTime1<=currentTime2){
    		
    		do1=false;
			
		}
		if(taskQueue2.isEmpty()&&!have2&&currentTime2<=currentTime1){
    		
    		do2=false;
			
		}
    	
    	if(taskQueue1.isEmpty()&&!have1&&!tasks.empty()&&currentTime1==tasks.front().arrival){
    		//佇列為空且有任務抵達
			//直接執行
    		currentTask1=tasks.front();
    		tasks.erase(tasks.begin());
    		have1=true;
    		start1=currentTime1;
    		do1=false;
    		
		}
		else if(!taskQueue1.isEmpty()&&!have1&&currentTime1<=currentTime2){ 
			//取消佇列中已超時工作 
			
			while(!taskQueue1.isEmpty()&&taskQueue1.getFront().timeout<=currentTime1){
				canceledTasks.push_back({taskQueue1.getFront().oid ,currentTime1, currentTime1- taskQueue1.getFront().arrival,1});
				taskQueue1.dequeue();
					
			}
			
			// 如果佇列1不為空，處理佇列中的任務
			if(!taskQueue1.isEmpty()&&taskQueue1.getFront().arrival<=currentTime1){
			
				currentTask1=taskQueue1.getFront();
	            taskQueue1.dequeue();
	            
	            have1=true;
	            start1=currentTime1;
	            do1=false;
	        }
		}
		
		
		
		if(taskQueue2.isEmpty()&&!have2&&!tasks.empty()&&currentTime2==tasks.front().arrival){
			//佇列為空且有任務抵達
			//直接執行
    		currentTask2=tasks.front();
    		tasks.erase(tasks.begin());
    		have2=true;
    		currentTime2=currentTask2.arrival;
    		start2=currentTime2;
    		do2=false;
		}
		else if(!taskQueue2.isEmpty()&&!have2&&currentTime2<=currentTime1){ 
			//取消佇列中已超時工作 
			
			while(!taskQueue2.isEmpty()&&taskQueue2.getFront().timeout<=currentTime2){
				canceledTasks.push_back({taskQueue2.getFront().oid ,currentTime2, currentTime2 - taskQueue2.getFront().arrival,2});
				taskQueue2.dequeue();
					
			}
			
			// 如果佇列2不為空，處理佇列中的任務
			if(!taskQueue2.isEmpty()&&taskQueue2.getFront().arrival<=currentTime2){
				currentTask2=taskQueue2.getFront();
	            taskQueue2.dequeue();
	            have2=true;
	            start2=currentTime2;
	            do2=false;
			}
			
		}
		
	
		//taskQueue1.size()>=taskQueue2.size()
		if(((have1&&start1<=start2)||(have1&&!have2))){
			// 處理器1的工作會先完成 
			
			if (currentTime1 >= currentTask1.timeout&&have1) {
            	canceledTasks.push_back({currentTask1.oid ,currentTask1.timeout, start1 - currentTask1.arrival,1});
            	
            	have1=false;
        	} 
			else if(have1) {
			
            	// 執行任務
            	if(start1+currentTask1.duration > currentTask1.timeout){
            		canceledTasks.push_back({currentTask1.oid, currentTask1.timeout, currentTask1.timeout-currentTask1.arrival,1});
            	    currentTime1 = currentTask1.timeout;
            	    have1=false;
            	    do1=true;
				}
            	else {
            	    completedTasks.push_back({currentTask1.oid, start1+currentTask1.duration, start1 - currentTask1.arrival,1});
            	    currentTime1 =start1+ currentTask1.duration;
           		    have1=false;
                	do1=true;
				}
    		}
    		
    
			
    		
    		
		} //taskQueue2.size()>=taskQueue1.size()
		else if(((have2&&start2<=start1)||(have2&&!have1))){
			// 處理器2的工作會先完成 
			
			if (currentTime2 >= currentTask2.timeout&&have2) {
        	    canceledTasks.push_back({currentTask2.oid, currentTask2.timeout, start2 - currentTask2.arrival,2});
        	    
        	    have2=false;
        	} 
			else if(have2) {
			
        	    // 執行任務
        	    if (start2+currentTask2.duration > currentTask2.timeout) {
        	        canceledTasks.push_back({currentTask2.oid, currentTask2.timeout, currentTask2.timeout-currentTask2.arrival,2});
        	        currentTime2 = currentTask2.timeout;
        	        have2=false;
        	        do2=true;
        	    }
        	    else{
            		
        	        completedTasks.push_back({currentTask2.oid, start2+currentTask2.duration, start2 - currentTask2.arrival,2});
        	        currentTime2 =start2+ currentTask2.duration;
        	        have2=false;
        	        do2=true;
				}
    		}
    		
    		
    		
    		
    	
		}
		
		if(taskQueue1.size()>=taskQueue2.size()){
			while(!taskQueue1.isEmpty()&&taskQueue1.getFront().timeout<=currentTime1){
				canceledTasks.push_back({taskQueue1.getFront().oid ,currentTime1, currentTime1- taskQueue1.getFront().arrival,1});
				taskQueue1.dequeue();
							
			}
			while(!taskQueue2.isEmpty()&&taskQueue2.getFront().timeout<=currentTime2){
				canceledTasks.push_back({taskQueue2.getFront().oid ,currentTime2, currentTime2 - taskQueue2.getFront().arrival,2});
				taskQueue2.dequeue();
							
			}
		}
		else{
			while(!taskQueue2.isEmpty()&&taskQueue2.getFront().timeout<=currentTime2){
				canceledTasks.push_back({taskQueue2.getFront().oid ,currentTime2, currentTime2 - taskQueue2.getFront().arrival,2});
				taskQueue2.dequeue();
							
			}
			while(!taskQueue1.isEmpty()&&taskQueue1.getFront().timeout<=currentTime1){
				canceledTasks.push_back({taskQueue1.getFront().oid ,currentTime1, currentTime1- taskQueue1.getFront().arrival,1});
				taskQueue1.dequeue();
							
			}
		}
			
		
		
		
		
		if(do1&&(currentTime1<=start2+ currentTask2.duration||currentTime1<=currentTask2.timeout)&&currentTime1<=currentTime2){
			//將任務加入佇列 
			
			while (!tasks.empty() && tasks.front().arrival < currentTime1) {
				
	            if (taskQueue1.size() < MAX_QUEUE&&taskQueue1.size()<=taskQueue2.size()) {
	            	temp=tasks.front();
	            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
				}
				else if (taskQueue2.size() < MAX_QUEUE) {
	            	temp=tasks.front();
	            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
				}
	            else if(taskQueue1.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime1&&taskQueue1.size()<=taskQueue2.size()){
	            	temp=tasks.front();
	            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
	            } 
	            else if(taskQueue2.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime2){
	            	temp=tasks.front();
	            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
	            }
				else if(tasks.front().arrival<currentTime1){
	            	canceledTasks.push_back({tasks.front().oid, tasks.front().arrival, 0,0});
	            }
	            
				
	            tasks.erase(tasks.begin());
	            
	        }
	        
			
		}
		else if(do2&&(currentTime2<=start1+ currentTask1.duration||currentTime2<=currentTask1.timeout)&&currentTime2<=currentTime1){
			//將任務加入佇列
			
			while (!tasks.empty() && (tasks.front().arrival < currentTime2)) {
			
	            if (taskQueue1.size() < MAX_QUEUE&&taskQueue1.size()<=taskQueue2.size()) {
	            	temp=tasks.front();
	            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
				}
				else if (taskQueue2.size() < MAX_QUEUE) {
	            	temp=tasks.front();
	            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);
				}
	            else if(taskQueue1.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime1&&taskQueue1.size()<=taskQueue2.size()){
	            	temp=tasks.front();
	            	taskQueue1.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
	            } 
	            else if(taskQueue2.size() <= MAX_QUEUE&&tasks.front().arrival==currentTime2){
	            	temp=tasks.front();
	            	taskQueue2.enqueue(temp.oid, temp.arrival, temp.duration, temp.timeout);	
	            }
				else if(tasks.front().arrival<currentTime2){
	            	canceledTasks.push_back({tasks.front().oid, tasks.front().arrival, 0,0});
	            }
	            
				
	            tasks.erase(tasks.begin());
	            
	        }
	        
		}
		
		
		
		
        if((taskQueue1.isEmpty()&&!have1)&&!do1){
			// 如果佇列為空，時間前進
            currentTime1++;
		}
		
		if((taskQueue2.isEmpty()&&!have2)&&!do2){
			// 如果佇列為空，時間前進
            currentTime2++;
		}
		
    }
    
    
}
*/
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
		int size=datain.size();
		int i=0;
	
		printf("\tOID\tArrival\tDuration\tTimeOut\n");
		while(i<size){
			printf("(%d)\t%d\t%d\t%d\t%d\n",i+1,datain[i].oid,datain[i].arrival,datain[i].duration,datain[i].timeout);
			i++;
		}
		printf("\n");
	
		clock_t start_time2 = clock();
		datain=Sorted(datain);
		clock_t end_time2 = clock();
		sort_time=((double)(end_time2 - start_time2) / CLOCKS_PER_SEC) * 1000.0;
	
		strcat(fname,num);
		strcat(fname,".txt");
		sortedf=fopen(fname, "w");
	
		clock_t start_time3 = clock();
		i=0;
		fprintf(sortedf,"OID\tArrival\tDuration\tTimeOut\n");
		while(i<size){
			fprintf(sortedf,"%d\t%d\t%d\t%d\n",datain[i].oid,datain[i].arrival,datain[i].duration,datain[i].timeout);
			i++;
		}
		clock_t end_time3 = clock();
		write_time=((double)(end_time3 - start_time3) / CLOCKS_PER_SEC) * 1000.0;
	
		printf("Reading data:%d clocks (%f ms).\n",(int)read_time,read_time);
		printf("Sorting data:%d clocks (%f ms).\n",(int)sort_time,sort_time);
		printf("Writing data:%d clocks (%f ms).\n",(int)write_time,write_time);
		printf("\n");
	
		printf("See %s\n",fname);
		printf("\n");
	
		datain.clear();
		fclose(sortedf);
	}
}

void Two(){ 
    Queue taskQueue;   
	char fname[200]="output";//存整個檔案的名字 
	char trash;             //存不需要的換行
	FILE *outputf;
	std::vector<Datain> datain;
	std::vector<CompletedTask> completedTasks;
	std::vector<CanceledTask> canceledTasks;
	double td;
	double ad;
	double rate;

	if(num[0]=='\0'){
		//讀入檔案號碼 
		printf("Input a file number:");
		scanf("%s",&num);
		printf("\n");
	}
	//讀取前面讀取檔案號碼訊息多餘的換行
	scanf("%c",&trash);

	datain=Set2(datain);
	
	if(sucess){
		Onecpu(datain,completedTasks,canceledTasks);
                  
		strcat(fname,num);
		strcat(fname,".txt");
		outputf=fopen(fname, "w");
	
		int totaltask=canceledTasks.size()+completedTasks.size();
		rate=completedTasks.size()/(double)totaltask*100;
	
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
			fprintf(outputf,"[%d]\t%d\t%d\t%d\t%d\n",i+1,completedTasks[i].cpu,completedTasks[i].id,completedTasks[i].completionTime,completedTasks[i].delayTime);
			i++;
		}
	
		ad=td/(double)totaltask;
	
		fprintf(outputf,"[Average Delay]\t%.2f ms\n",(double)ad);
		fprintf(outputf,"[Success Rate]\t%.2f %%\n",(double)rate);
	
		printf("The simulation is running...\n");
		printf("See %s\n",fname);
		printf("\n");
	
		datain.clear();
		fclose(outputf);
	}
	
}

void Three(){ 
    Queue taskQueue;   
	char fname[200]="double";//存整個檔案的名字 
	char trash;             //存不需要的換行
	FILE *outputf;
	std::vector<Datain> datain;
	std::vector<CompletedTask> completedTasks;
	std::vector<CanceledTask> canceledTasks;
	double td;
	double ad;
	double rate;

	if(num[0]=='\0'){
		//讀入檔案號碼 
		printf("Input a file number:");
		scanf("%s",&num);
		printf("\n");
	}
	//讀取前面讀取檔案號碼訊息多餘的換行
	scanf("%c",&trash);

	datain=Set2(datain);
	
	if(sucess){
		Twocpu(datain,completedTasks,canceledTasks);
                  
		strcat(fname,num);
		strcat(fname,".txt");
		outputf=fopen(fname, "w");
	
		int totaltask=canceledTasks.size()+completedTasks.size();
		rate=completedTasks.size()/(double)totaltask*100;
	
		int i = 0;
		int size = canceledTasks.size();
		fprintf(outputf,"\t[Abort Jobs]\n");
		fprintf(outputf,"\tOID\tCID\tAbort\tDelay\n");
		while(i<size){
			td=td+canceledTasks[i].delayTime;
			fprintf(outputf,"[%d]\t%d\t%d\t%d\t%d\n",i+1,canceledTasks[i].id,canceledTasks[i].cpu,canceledTasks[i].cancelTime,canceledTasks[i].delayTime);
			i++;
		}
	
		i = 0;
		size = completedTasks.size();
		fprintf(outputf,"\t[Jobs Done]\n");
		fprintf(outputf,"\tOID\tCID\tDeparture\tDelay\n");
		while(i<size){
			td=td+completedTasks[i].delayTime;
			fprintf(outputf,"[%d]\t%d\t%d\t%d\t%d\n",i+1,completedTasks[i].id,completedTasks[i].cpu,completedTasks[i].completionTime,completedTasks[i].delayTime);
			i++;
		}
	
		ad=td/(double)totaltask;
	
		fprintf(outputf,"[Average Delay]\t%.2f ms\n",(double)ad);
		fprintf(outputf,"[Success Rate]\t%.2f %%\n",(double)rate);
	
		printf("The simulation is running...\n");
		printf("See %s\n",fname);
		printf("\n");
	
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
    	printf("* 3. Simulate two queues by SQF      *\n");
    	printf("**************************************\n");
    	printf("Input a command(0, 1, 2, 3):");
		scanf("%c",&dowhat);
		printf("\n");
	
	    //若非合法指令則輸出錯誤訊息，合法則執行相應動作 
		if(dowhat!='0'&&dowhat!='1'&&dowhat!='2'&&dowhat!='3'){
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
		else if(dowhat=='3'){
			//指令為1則呼叫One()執行後續程序 
			Three();
		}
	}
	

}

