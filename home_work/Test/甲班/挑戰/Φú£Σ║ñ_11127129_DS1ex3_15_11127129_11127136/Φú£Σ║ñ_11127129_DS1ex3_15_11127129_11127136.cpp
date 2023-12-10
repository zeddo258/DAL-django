//11127129�i�a�� 11127136�^��� 
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
static char num[MAX_LENGTH];//�s�ϥΪ̿�J���ɮ׸��X
static bool sucess = true;   //�����O�_���\�}���ɮ� 

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
    char fname[200]="input";//�s����ɮת��W�r
	
	
	//�걵�ɦW 
	strcat(fname,num);
	strcat(fname,".txt");
	//�}�Ҭ������ 
	FILE *file;
	file = fopen(fname, "rb"); // �H��Ū�Ҧ����}���
	if (file == NULL) {
		//�䤣�즹���h��X���~�T�� 
    	printf("### input%s.txt does not exist! ###\n",num);
    	printf("\n");
    	sucess=false;
	}
	else{ 
		sucess=true;
		datain=Read(datain,file);
		//�]�w����������� 
		fclose(file);
	}
	
	return datain; //�N�]�w�n���^�� 
}

std::vector<Datain> Set2(std::vector<Datain> datain){ 
    char fname[200]="sorted";//�s����ɮת��W�r
	
	
	//�걵�ɦW 
	strcat(fname,num);
	strcat(fname,".txt");
	//�}�Ҭ������ 
	FILE *file;
	file = fopen(fname, "rb"); // �H��Ū�Ҧ����}���
	if (file == NULL) {
		//�䤣�즹���h��X���~�T�� 
    	printf("### input%s.txt does not exist! ###\n",num);
    	printf("\n");
    	sucess=false;
	}
	else{ 
	    sucess=true;
		datain=Read(datain,file);
		//�]�w����������� 
		fclose(file);
	}
	
	return datain; //�N�]�w�n���^�� 
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
			// �p�G��C���šA�ɶ��e�i
            currentTime++;
		}
		else{
			// �p�G��C�����šA�B�z��C��������
			currentTask=taskQueue.getFront();
            taskQueue.dequeue();
            have=true;
		}
		
		
        if (currentTime > currentTask.timeout&&have) {
            canceledTasks.push_back({currentTask.oid, currentTime, currentTime - currentTask.arrival});
            have=false;
        } 
		else if(have) {
			
            // �������
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
        
        // �N��F�ɶ��ŦX��e�ɶ������ȥ[�J��C
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
    Queue taskQueue1;    //cpu1��C 
    Queue taskQueue2;    //cpu2��C 
	Datain temp;         //�Ȧs��� 
	int currentTime = 0;
    int currentTime1 = 0; //cpu1���ɶ� 
    int currentTime2 = 0; //cpu2���ɶ� 
    int start1=0;        //cpu1���Ȫ��}�l�ɶ� 
    int start2=0;        //cpu2���Ȫ��}�l�ɶ�
    Datain currentTask1 ;//cpu1���� 
    Datain currentTask2 ;//cpu2����
    /*
    //�]�w�ӳB�z�����Ĥ@����� 
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
    	//debug�� 
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
		            	// �������
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
		            	// �������
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
		            	// �������
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
		            	// �������
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
		            	// �������
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
		            	// �������
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
		            // �������
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
		           	// �������
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
    Queue taskQueue1;    //cpu1��C 
    Queue taskQueue2;    //cpu2��C 
	Datain temp;         //�Ȧs��� 
    int currentTime1 = 0; //cpu1���ɶ� 
    int currentTime2 = 0; //cpu2���ɶ� 
    int start1=0;        //cpu1���Ȫ��}�l�ɶ� 
    int start2=0;        //cpu2���Ȫ��}�l�ɶ�  
    Datain currentTask1 ;//cpu1���� 
    Datain currentTask2 ;//cpu2����
    bool have1=false;    //cpu1�O�_������ 
    bool have2=false;    //cpu2�O�_������  
    bool do1=false;      //cpu1�O�_����F 
    bool do2=false;      //cpu2�O�_����F
    
    //�]�w�ӳB�z�����Ĥ@����� 
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

    
	//�����C�M���ȲM�泣����
    while (!taskQueue1.isEmpty()||!taskQueue2.isEmpty() ||!tasks.empty()|| have1||have2) {
    	
		//���s�B�z�����A 
		if(taskQueue1.isEmpty()&&!have1&&currentTime1<=currentTime2){
    		
    		do1=false;
			
		}
		if(taskQueue2.isEmpty()&&!have2&&currentTime2<=currentTime1){
    		
    		do2=false;
			
		}
    	
    	if(taskQueue1.isEmpty()&&!have1&&!tasks.empty()&&currentTime1==tasks.front().arrival){
    		//��C���ťB�����ȩ�F
			//��������
    		currentTask1=tasks.front();
    		tasks.erase(tasks.begin());
    		have1=true;
    		start1=currentTime1;
    		do1=false;
    		
		}
		else if(!taskQueue1.isEmpty()&&!have1&&currentTime1<=currentTime2){ 
			//������C���w�W�ɤu�@ 
			
			while(!taskQueue1.isEmpty()&&taskQueue1.getFront().timeout<=currentTime1){
				canceledTasks.push_back({taskQueue1.getFront().oid ,currentTime1, currentTime1- taskQueue1.getFront().arrival,1});
				taskQueue1.dequeue();
					
			}
			
			// �p�G��C1�����šA�B�z��C��������
			if(!taskQueue1.isEmpty()&&taskQueue1.getFront().arrival<=currentTime1){
			
				currentTask1=taskQueue1.getFront();
	            taskQueue1.dequeue();
	            
	            have1=true;
	            start1=currentTime1;
	            do1=false;
	        }
		}
		
		
		
		if(taskQueue2.isEmpty()&&!have2&&!tasks.empty()&&currentTime2==tasks.front().arrival){
			//��C���ťB�����ȩ�F
			//��������
    		currentTask2=tasks.front();
    		tasks.erase(tasks.begin());
    		have2=true;
    		currentTime2=currentTask2.arrival;
    		start2=currentTime2;
    		do2=false;
		}
		else if(!taskQueue2.isEmpty()&&!have2&&currentTime2<=currentTime1){ 
			//������C���w�W�ɤu�@ 
			
			while(!taskQueue2.isEmpty()&&taskQueue2.getFront().timeout<=currentTime2){
				canceledTasks.push_back({taskQueue2.getFront().oid ,currentTime2, currentTime2 - taskQueue2.getFront().arrival,2});
				taskQueue2.dequeue();
					
			}
			
			// �p�G��C2�����šA�B�z��C��������
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
			// �B�z��1���u�@�|������ 
			
			if (currentTime1 >= currentTask1.timeout&&have1) {
            	canceledTasks.push_back({currentTask1.oid ,currentTask1.timeout, start1 - currentTask1.arrival,1});
            	
            	have1=false;
        	} 
			else if(have1) {
			
            	// �������
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
			// �B�z��2���u�@�|������ 
			
			if (currentTime2 >= currentTask2.timeout&&have2) {
        	    canceledTasks.push_back({currentTask2.oid, currentTask2.timeout, start2 - currentTask2.arrival,2});
        	    
        	    have2=false;
        	} 
			else if(have2) {
			
        	    // �������
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
			//�N���ȥ[�J��C 
			
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
			//�N���ȥ[�J��C
			
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
			// �p�G��C���šA�ɶ��e�i
            currentTime1++;
		}
		
		if((taskQueue2.isEmpty()&&!have2)&&!do2){
			// �p�G��C���šA�ɶ��e�i
            currentTime2++;
		}
		
    }
    
    
}
*/
void One(){          
	char fname[200]="sorted";//�s����ɮת��W�r 
	char trash;             //�s���ݭn������
	FILE *sortedf;
	std::vector<Datain> datain;
	double read_time;
	double sort_time;
	double write_time;
	
	
	
	//Ū�J�ɮ׸��X 
	printf("Input a file number:");
	scanf("%s",&num);
	//Ū���e��Ū���ɮ׸��X�T���h�l������
	scanf("%c",&trash);
	printf("\n");
	
	clock_t start_time = clock();
	datain=Set(datain);
	clock_t end_time = clock();
	// �p�����ɶ��]�H�@�����^
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
	char fname[200]="output";//�s����ɮת��W�r 
	char trash;             //�s���ݭn������
	FILE *outputf;
	std::vector<Datain> datain;
	std::vector<CompletedTask> completedTasks;
	std::vector<CanceledTask> canceledTasks;
	double td;
	double ad;
	double rate;

	if(num[0]=='\0'){
		//Ū�J�ɮ׸��X 
		printf("Input a file number:");
		scanf("%s",&num);
		printf("\n");
	}
	//Ū���e��Ū���ɮ׸��X�T���h�l������
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
	char fname[200]="double";//�s����ɮת��W�r 
	char trash;             //�s���ݭn������
	FILE *outputf;
	std::vector<Datain> datain;
	std::vector<CompletedTask> completedTasks;
	std::vector<CanceledTask> canceledTasks;
	double td;
	double ad;
	double rate;

	if(num[0]=='\0'){
		//Ū�J�ɮ׸��X 
		printf("Input a file number:");
		scanf("%s",&num);
		printf("\n");
	}
	//Ū���e��Ū���ɮ׸��X�T���h�l������
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
	char trash;      //�s���ݭn������
	char dowhat='\0';//�s�����O   
	//�����a�߰ݭn���檺�ʧ@�����J0�h�X 
	while(dowhat!='0'){
		//�L�X������Ū�����O 
		printf("**** Simulate FIFO Queues by SQF *****\n");
    	printf("* 0. Quit                            *\n");
    	printf("* 1. Sort a file                     *\n");
    	printf("* 2. Simulate one FIFO queue         *\n");
    	printf("* 3. Simulate two queues by SQF      *\n");
    	printf("**************************************\n");
    	printf("Input a command(0, 1, 2, 3):");
		scanf("%c",&dowhat);
		printf("\n");
	
	    //�Y�D�X�k���O�h��X���~�T���A�X�k�h��������ʧ@ 
		if(dowhat!='0'&&dowhat!='1'&&dowhat!='2'&&dowhat!='3'){
			printf("Command does not exist!\n");
			//Ū���e�����O�T���h�l������ 
			if(dowhat!='\n'){
				scanf("%c",&trash);	
			}
			
			printf("\n");
		}
		else if(dowhat=='1'){
			//���O��1�h�I�sOne()�������{�� 
			One();
		}
		else if(dowhat=='2'){
			//���O��1�h�I�sOne()�������{�� 
			Two();
		}
		else if(dowhat=='3'){
			//���O��1�h�I�sOne()�������{�� 
			Three();
		}
	}
	

}

