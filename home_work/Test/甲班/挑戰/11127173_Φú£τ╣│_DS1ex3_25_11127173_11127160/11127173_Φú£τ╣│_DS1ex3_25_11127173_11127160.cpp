// 11127173 張穎峰 11127160 李承熹
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <time.h>
#include <chrono>
using namespace std ;

struct Node{
    int oid = 0 , arrival = 0 , duration = 0 , timeout = 0 ;
    Node * head = NULL ; // if tasks with same arrival time then store them in this link list "head of the list"
    Node * tail = NULL ; // "tail of the list"
    Node * next = NULL ;
};

struct Queue {
    Node* nodeQ = NULL ;
    Queue* next = NULL ;
};

struct Joblist {
    int time = 0, delay = 0, cpuNum = 0 ;
    Node* nodeJ = NULL ;
    Joblist* next = NULL ;
};

struct CPU {
    int clocktime = 0, queueMax = 3, taskenterTime = 0, cpuNum = 0  ;
    bool idle = true ;
    Node* task = NULL ;
    Queue* head = NULL ;
    Queue* tail = NULL ;
    CPU* next = NULL ;
};
Node* copyNode( Node* node ) {
    Node* newNode = new Node ;
    newNode->oid = node->oid ;
    newNode->arrival = node->arrival ;
    newNode->duration = node->duration ;
    newNode->timeout = node->timeout ;
    return newNode ;
}

bool isQEmpty( Queue* head ) {
    if ( head == NULL ) {
        return true ;
    }
    else {
        return false ;
    }
}

bool isEmpty( Node * head ) {
    if ( head == NULL ) {
        return true ;
    }

    return false ;
} // hsi


int listSize( Node * head ) {
    int i = 1 ;
    if ( !isEmpty(head) ) {
        while ( head->next != NULL ) {
            head = head->next ;
            i++ ;
        }

        return i ;
    }
    else {
        return 0 ;
    }
} // find the list size

bool allCPUidle( CPU* head ) {
    while ( head != NULL ) {
        if ( !head->idle ) {
            return false ;
        }

        head = head->next ;
    }

    return true ;
} // check if all cpu is idle


int queueSize( Queue * head ) {
    int i = 1 ;
    if ( !isQEmpty(head) ) {
        while ( head->next != NULL ) {
            head = head->next ;
            i++ ;
        }

        return i ;
    }
    else {
        return 0 ;
    }
} // find queue size

int joblistSize( Joblist* head ) {
    int i = 0 ;
    while( head != NULL ) {
        head = head->next ;
        i++ ;
    }

    return i ;

} // find job list size

int input( ifstream &file ) { // return one int value from the file
    int num = 0 ;
    file >> num ;
    return num ;
} // input int from file 

void inputTrash( ifstream &file ) {
    string temp ;
    getline( file, temp ) ;
 
} // read the label 

void outputfile( string fileName ) {
    string temp ;
    int i = 1 ;
    ifstream file(fileName) ;
    getline(file, temp) ;
    cout << "\n\t" << temp << endl ;
    while ( file ) {
        getline(file, temp) ;
        if ( file ) {
            cout << "(" << i << ")\t" << temp << endl ;
        }
        i++ ;
    }
}

void addJoblist( Joblist* &head, Joblist* &tail, Node* node, int clocktime ) {
    Joblist* newJob = new Joblist ;
    newJob->nodeJ = node ;
    if ( head == NULL  ) {
        head = newJob ;
        tail = newJob ;
    }
    else {
        tail->next = newJob ;
        tail = newJob ;
    }

} // add abort or done job into the job list

void deleteNode ( Node * &head, Node* &tail, Node * target ) {
    Node * temp1 = head ;
    if ( head == target ) {
        head = head->next ;
        delete target ;
    }
    while ( temp1->next != target && temp1->next != NULL) {
        temp1 = temp1->next ;
    }
    temp1 ->next= target->next ;
    tail = temp1 ;
    delete target ;
} // delete a node from the list

Node* dequeue( Queue* &headQueue ) {
    Queue* temp = headQueue ;
    Node* tempNode = NULL ;
    if ( !isQEmpty(headQueue) ) {
        tempNode = headQueue->nodeQ ;
        headQueue = headQueue->next ;
        delete temp ;
    }

    return tempNode ;


} //dequeue a node from queue

float averageDelay( Joblist* headA, Joblist* headD ) {
    float sum = 0 ;
    Joblist* tempA = headA, *tempD = headD ;
    while ( headA != NULL ) {
        sum = sum + headA->delay ;
        headA = headA->next ;
    }

    while ( headD != NULL ) {
        sum = sum + headD->delay ;
        headD = headD->next ;
    }    

    if ( joblistSize(tempA) != 0 || joblistSize(tempD) != 0 ) {
        return sum / ( joblistSize(tempA) + joblistSize(tempD) ) ;
    }
    else {
        return 0 ;
    }
} // calculate the average delay

float successRate( Joblist* headA, Joblist* headD ) {
    if ( joblistSize(headA) + joblistSize(headD) != 0 ) {
        float sr = joblistSize(headD) ;
        sr = sr / (joblistSize(headA) + joblistSize(headD)) * 100 ;
        return sr ;
    }
    else {
        return 0 ;
    }
} // calculate success rate
void outputNode( Node* node ) {
    cout  << node->oid << "\t" ;
    cout  << node->arrival << "\t" ;
    cout  << node->duration << "\t" ;
    cout  << node->timeout << "\n"  ;
}

void ofileNodeS( ofstream &file, Node* node ) {
    file << node->oid << "\t" << node->arrival << "\t" << node->duration << "\t" << node->timeout << endl ;
} // S stand for sorted ,write data of a node to the file sortedxxx.txt

void ofileNodeO( ofstream &file, Joblist* jobNode ) {
    file << jobNode->nodeJ->oid << "\t" << jobNode->time << "\t" << jobNode->delay << endl ;
} // O stands for output ,write data of a node to the file outputxxx.txt


void ofileNodeD( ofstream &file, Joblist* jobNode ) {
    file << jobNode->nodeJ->oid << "\t" << jobNode->cpuNum << "\t" << jobNode->time << "\t" << jobNode->delay << endl ;
}// D stands for double ,write data of a node to the file doublexxx.txt



void ofileAllS( ofstream &file, Node* head ) {
    file << "oid\tarrival\tduration\ttimeout\n" ;

    while ( head != NULL ) {
        if ( head->head != NULL ) {
            Node* temp = head->head ;
            while ( temp != NULL ) {
                ofileNodeS( file, temp ) ;
                temp = temp->next ;
            }
        }
        else {
            ofileNodeS( file, head ) ;
        }
        head = head->next ;
    }     
}// S stands for sorted ,write all data to the file sortedxxx.txt


void ofileAllO( ofstream &ofile, Joblist* headA, Joblist* headD ) {
    int i = 1 ;
    Joblist* tempA = headA ;
    Joblist* tempD = headD ;
    float sr = 100.00 ;
    ofile << "\t[Abort Jobs]\n" << "\tOID\tAbort\tDelay\n" ;
    while ( headA != NULL ) {
        ofile << "[" << i << "]\t" ; 
        ofileNodeO( ofile, headA ) ;
        headA = headA->next ;
        i++ ;
    }

    i = 1 ;
    ofile << "\t[Jobs Done]\n" << "\tOID\tDeparture\tDelay\n" ;
    while ( headD != NULL ) {
        ofile << "[" << i << "]\t" ; 
        ofileNodeO( ofile, headD ) ;
        headD = headD->next ;
        i++ ;
    }  

    ofile << "[Average Delay]\t" << roundf(averageDelay(tempA, tempD) * 100 ) /100<< " ms\n" ;
    ofile << "[Success Rate]\t" << roundf(successRate(tempA, tempD) * 100 ) / 100  << " %\n" ;

} // O stands for output ,write all data to the file outputxxx.txt

void ofileAllD( ofstream &ofile, Joblist* headA, Joblist* headD ) {
    int i = 1 ;
    Joblist* tempA = headA ;
    Joblist* tempD = headD ;
    float sr = 100.00 ;
    ofile << "\t[Abort Jobs]\n" << "\tOID\tCID\tAbort\tDelay\n" ;
    while ( headA != NULL ) {
        ofile << "[" << i << "]\t" ; 
        ofileNodeD( ofile, headA ) ;
        headA = headA->next ;
        i++ ;
    }

    i = 1 ;
    ofile << "\t[Jobs Done]\n" << "\tOID\tCID\tDeparture\tDelay\n" ;
    while ( headD != NULL ) {
        ofile << "[" << i << "]\t" ; 
        ofileNodeD( ofile, headD ) ;
        headD = headD->next ;
        i++ ;
    }  

    ofile << "[Average Delay]\t" << roundf(averageDelay(tempA, tempD) * 100 ) /100<< " ms\n" ;
    ofile << "[Success Rate]\t" << roundf(successRate(tempA, tempD) * 100 ) / 100  << " %\n" ;

} // D stands for double ,write all data to the file doublexxx.txt

void outputAll( Node* head ) {
    int i = 1 ;
    cout << "\tOID\tArrival\tDuration\tTimeout\n" ;
    while ( head != NULL ) {
        if ( head->head != NULL ) {
            Node* temp = head->head ;
            while ( temp != NULL ) {
                cout << "(" << i << ")\t" ;
                outputNode(temp) ;
                temp = temp->next ;
                i++ ;
            }
        }
        else {
            cout << "(" << i << ")\t" ;
            outputNode(head) ;
            i++ ;
        }
        head = head->next ;
    }   
}

void outputJobList( Joblist* head ) {
    while ( head != NULL ) {
        cout << "oid: " << head->nodeJ->oid << endl ;
        cout << "delay: " << head->delay << endl ;
        cout << "time: " << head->time << "\n\n" ;
        head = head->next ;
    }
}

void inputDatatoNode( Node* node, ifstream &file ) { // input all data to the node
    node->oid = input(file) ;
    node->arrival = input(file) ;
    node->duration = input(file) ;
    node->timeout = input(file) ;
} 

bool checkIftimeout( CPU* cpuptr ) {
    if ( cpuptr->task->timeout <= cpuptr->clocktime ) {
        return true ;
    }
    else {
        return false ;
    }
} // check if the current task in cpu is time out

bool checkIfSameArr( Node* head, Node* &temp, Node* newNode ) {
    temp = head ;
    while ( temp->next != NULL && temp->arrival != newNode->arrival ) {
        temp = temp->next ;
    }

    if ( temp->arrival == newNode->arrival ) {
        return true ;
    }
    else {
        return false ;
    }

} // check if the new data has same arrival time with nodes in the list

void addCPU( CPU* &head, CPU* &tail, CPU* cpu ) {
    if ( head == NULL ) {
        head = cpu ;
        tail = cpu ;
    }
    else {
        tail->next = cpu ;
        tail = cpu ;
    }
} // add new cpu to cpu list 


void add( Node * &head, Node * &tail, ifstream &file ) {
    Node * newNode = new Node ;
    Node * temp = NULL ;
    inputDatatoNode( newNode, file ) ;
    if ( isEmpty(head) ) {
        head = newNode ;
        tail = newNode ; // when the link list is empty       
    }
    else if ( !checkIfSameArr( head, temp, newNode ) ) { //check if there is a node with same arrival time if there is temp will point to that node
        tail->next = newNode ;
        tail = newNode ; // when link list is not empty then the last node's "next" point to new node ;
    }
    else {
        if ( isEmpty( temp->head ) ) {
            Node* copy = copyNode(temp) ;
            temp->head = copy ;
            copy->next = newNode ; 
            temp->tail = newNode ;// adding node to the node with same arrival time 
        }
        else {
            temp->tail->next = newNode ;
            temp->tail = newNode ;
        }
    }
    
} // add data in link list

CPU* findLeastTaskQ( CPU* headCPU ) {
    CPU* temp = headCPU ;
    while ( headCPU != NULL ) {
        if ( queueSize(headCPU->head) < queueSize(temp->head) ) {
            temp = headCPU ;
        }

        headCPU = headCPU->next ;
    }

    return temp ;
} // find the smallest queue in cpu list

void enqueue( CPU* &cpuptr, Node* node ) {    
    Queue* newNode = new Queue ;
    newNode->nodeQ = node ;
    if ( isQEmpty(cpuptr->head) ) {
        cpuptr->head = newNode ;
        cpuptr->tail = newNode ;
    }
    else {
        cpuptr->tail->next = newNode ;
        cpuptr->tail = newNode ;
    }


} // enqueue

void enqueueCPU( CPU* &headCPU, Node* node, Joblist* &headA, Joblist* &tailA ) {
    CPU* leastQ = findLeastTaskQ(headCPU) ;
    if ( queueSize(leastQ->head) < leastQ->queueMax  ) {
        enqueue ( leastQ, node ) ;
    }
    else {
        addJoblist( headA, tailA, node, headCPU->clocktime ) ;
        tailA->time = headCPU->clocktime ;
        tailA->delay = tailA->time - tailA->nodeJ->arrival ;
    }
} // enqueue 


Node * accessNode( Node * head , int nodeNum ) { // access the node on the list by the number of node

    for ( int i = 1 ; i < nodeNum ; i++ ) { 
        head = head->next ;
    }   

    return head ;
} // access the node by the number of node 

void swap( Node * &head, Node* &tail, Node * &node1, Node * &node2 ) {
    Node * temp1 = head ;
    Node * temp2 = head ;
    if ( node1 == node2 ) {
        return ;
    }

    if ( node1 != head && node2 != head ) {
        while ( temp1->next != node1 || temp2->next != node2 ) {
            if ( temp1->next != node1 ) {
                temp1 = temp1->next ;
            } 

            if ( temp2->next != node2 ) {
                temp2 = temp2->next ;
            }
        } // 

        temp1->next = node2 ;
        temp2->next = node1 ;
        temp1 = node1->next ;
        node1->next = node2->next ;
        node2->next = temp1 ;
        if ( node1 == tail ) {
            tail = node2 ;
        }
        else if ( node2 == tail ) {
            tail = node1 ;
        }

    }
    else if ( node1 == head ) {
        while ( temp2->next != node2 ) {
            temp2 = temp2->next ;
        }

        temp2->next = node1 ;
        temp2 = node2->next ;
        node2->next = node1->next ;
        node1->next = temp2 ;
        head = node2 ;
        if ( head == tail ) {
            tail = node1 ;
        }
    }
    else if ( node2 == head ) {
        while ( temp1->next != node1 ) {
            temp1 = temp1->next ;
        }

        temp1->next = node2 ;
        temp1 = node1->next ;
        node1->next = node2->next ;
        node2->next = temp1 ;
        head = node1 ;
        if ( head == tail ) {
            tail = node2 ;
        }
    }

} // swap data in the link list


void assignTask( CPU* &cpuptr, Node* node ) {
    cpuptr->task = node ;
    cpuptr->idle = false ;
    cpuptr->taskenterTime = cpuptr->clocktime ;
}  // assign task to cpu and turn idle to false 

void assignTaskRepeat( CPU* &cpu, Joblist* &head, Joblist* &tail ) {
    while ( cpu->idle && !isQEmpty(cpu->head)) {
        assignTask( cpu, dequeue(cpu->head)) ; // assign task from queue
        if ( checkIftimeout( cpu ) ) { // if the task just assigned is time out then put it into abort list and set cpu idle as true let next task enter cpu
                addJoblist( head, tail, cpu->task, cpu->clocktime ) ;
                tail->time = cpu->clocktime ;
                tail->delay = tail->time - tail->nodeJ->arrival ;
                tail->cpuNum = cpu->cpuNum ;
                cpu->idle = true ;
        }
    }

} // assign task from queue until cpu idle or queue empty

void checkAllTask( CPU* head, Joblist* &headA, Joblist* &tailA, Joblist* &headD, Joblist* &tailD ) {
    while ( head != NULL ) {
        if ( !head->idle ) {
            if ( head->taskenterTime + head->task->duration == head->clocktime ) { // if task is done 
                addJoblist( headD, tailD, head->task, head->clocktime ) ; // add to done list
                tailD->time = head->clocktime ;
                tailD->delay = head->taskenterTime - tailD->nodeJ->arrival ;
                tailD->cpuNum = head->cpuNum ;
                head->idle = true ;
                assignTaskRepeat( head, headA, tailA) ;  // assign till queue empty or cpu idle
            }
            else if ( checkIftimeout( head ) ) {// if task is timeout 
                addJoblist( headA, tailA, head->task, head->clocktime ) ;// add to abort list
                tailA->time = head->clocktime ;
                tailA->delay = tailA->time - tailA->nodeJ->arrival ;
                tailA->cpuNum = head->cpuNum ;
                head->idle = true ;
                assignTaskRepeat( head, headA, tailA) ;  // assign till queue empty or cpu idle          
            }
        }

        head = head->next ;
    }
} // check all task in cpu list see if they are done or time out 

bool assignTaskCPU( CPU* cpuhead, Node* node ) {
    while ( cpuhead != NULL ) {    
        if ( cpuhead->idle ) {
            assignTask( cpuhead, node ) ;
            return true ;
        }

        cpuhead = cpuhead->next ;
    }

    return false ;
} // assign task to the smallest number cpu which is idle 

void allCPUclockAdd( CPU* cpuhead ) {
    while ( cpuhead != NULL ) {
        cpuhead->clocktime++ ;
        cpuhead = cpuhead->next ;
    }
} // add all cpu clocktime by 1

void shell_sort( Node* &head, Node* &tail , bool cmpArr ) {
    int gap = listSize( head ) / 2 ;
    while ( gap > 0 ) {
        for ( int i = 1 ; i + gap <= listSize( head ) ; i++ ) {
            Node * node1 = accessNode( head, i ) ;
            Node * node2 = accessNode( head, i + gap ) ;
            int temp = i ;
            if ( cmpArr ) { // compare with arrival
                while ( node1->arrival > node2->arrival && temp >= 1 ) { // if node2 still able to swap with the previous node
                    swap( head, tail , node1, node2 ) ;
                    temp = temp - gap ; 
                    if ( temp >= 1 ) {
                        node1 = accessNode( head, temp ) ; // set node1 as the previous node
                    } 
                }

            }
            else {
                while ( node1->oid > node2->oid && temp >= 1 ) { // compare with oid
                    swap( head, tail, node1, node2 ) ; 
                    temp = temp - gap ; 
                    if ( temp >= 1 ) {
                        node1 = accessNode( head, temp ) ;
                    } 
                }                
            }
        }


        gap = gap / 2 ;
    }
} // willis "shell sort"

void createList( Node* &head, Node* &tail, ifstream &file ) {
    while( file ) {
        add( head, tail, file ) ;
    }

    Node* node = accessNode( head, listSize( head ) ) ;
    deleteNode( head, tail, node ) ;
} // create a list that store all data 

void rewriteAdd( Node* &head, Node* &tail, Node* node ) {
    if (isEmpty(head)) {
        head = node ;
        tail = node ;
    }
    else {
        tail->next = node ;
        tail = node ;
    }
}

Node* rewriteList( Node* head ) {
    Node* newHead = NULL, *newTail = NULL ; 
    while ( head != NULL ) {
        if ( head->head != NULL ) {
            while (head->head != NULL) {
                rewriteAdd( newHead, newTail, head->head ) ;
                head->head = head->head->next ;
            }
        }
        else {
            rewriteAdd( newHead, newTail, head ) ;
        }

        head = head->next ;
    }

    return newHead ;
}

float duration(const chrono::high_resolution_clock::time_point& start) {
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return duration;
} // calculate the time spent by start time and end time

Node* mission1( ifstream &file, ofstream &ofile ) {
    Node *head = NULL, *tail = NULL, *temp = NULL ;
    bool cmpArr = true ;
    float readtime = 0.0, sorttime = 0.0, outputtime = 0.0 ;
    int readI = 0, sortI = 0, outputI = 0 ;
    auto startTime = chrono::high_resolution_clock::now(); // store current time as start
    inputTrash( file ) ;
    createList( head, tail, file ) ;
    readtime = duration(startTime) ; // calculate time spent 
    startTime = chrono::high_resolution_clock::now(); // store current time as start
    shell_sort( head, tail , cmpArr ) ; // sort by arrival first
    temp = head ;
    cmpArr = false ;
    while ( temp != NULL ) {
        if ( temp->head != NULL ) { // if there are task with same arrival time than sort by oid
            shell_sort( temp->head, temp->tail, cmpArr ) ;
        }

        temp = temp->next ;
    }

    sorttime = duration(startTime) ; // calculate time spent 
    startTime = chrono::high_resolution_clock::now();// store current time as start
    ofileAllS( ofile, head ) ;
    outputtime = duration(startTime) ;// calculate time spent 
    readI = readtime ; // store as int
    sortI = sorttime ;
    outputI = outputtime ;
    cout << "\nReading data: " << readI << " clocks" << " (" ;
    printf( "%.2f%s\n", readtime, " ms).") ; // output as two decimal place
    cout << "Sorting data: " << sortI << " clocks" << " (" ;
    printf( "%.2f%s\n", sorttime, " ms).") ;// output as two decimal place
    cout << "Writing data: " << outputI << " clocks" << " (" ;
    printf( "%.2f%s\n", outputtime, " ms).") ;// output as two decimal place
    return head ;

}

void mission2( ifstream &file, ofstream &ofile ) {
    int numOfCPU = 1 ;
    CPU* cpu = new CPU ;
    Node *head = NULL, *tail = NULL, *temp = NULL ;
    Queue* headQueue = NULL, *tailQueue = NULL ;
    Joblist* headAbort = NULL, *tailAbort = NULL, *headDone = NULL, *tailDone = NULL ;
    inputTrash( file ) ;
    createList( head, tail, file ) ;
    Node* newHead = rewriteList( head ) ; // rewrite the list to linear link list
    temp = newHead ;
    while ( temp != NULL || !cpu->idle ) {
        if ( !cpu->idle ) {
            if ( cpu->taskenterTime + cpu->task->duration == cpu->clocktime ) { // if task in cpu is done
                addJoblist( headDone, tailDone, cpu->task, cpu->clocktime ) ;  // add task to done list
                tailDone->time = cpu->clocktime ;
                tailDone->delay = cpu->taskenterTime - tailDone->nodeJ->arrival ;
                cpu->idle = true ;
                assignTaskRepeat( cpu, headAbort, tailAbort) ; // assign task till cpu idle or queue empty
            }
            else if ( checkIftimeout( cpu ) ) { // if task in cpu is time out 
                addJoblist( headAbort, tailAbort, cpu->task, cpu->clocktime ) ; // add task to abort list
                tailAbort->time = cpu->clocktime ;
                tailAbort->delay = tailAbort->time - tailAbort->nodeJ->arrival ;
                cpu->idle = true ;
                assignTaskRepeat( cpu, headAbort, tailAbort) ; // assign task till cpu idle or queue empty
            }


        }


        while ( temp != NULL && temp->arrival == cpu->clocktime ) {
            if ( cpu->idle ) { // when cpu is idle
                if ( isQEmpty( cpu->head ) ) { // queue is empty
                    assignTask( cpu, temp ) ;  // assign task to cpu 
                }
                else { // queue is not empty
                    assignTask( cpu, dequeue(cpu->head)) ; // assign task from queue to cpu
                    if ( queueSize(cpu->head) < cpu->queueMax ) { // if queue is not full
                        enqueue(cpu, temp ) ; 
                    }
                    else { // if queue is full the add the task to abort list
                        addJoblist( headAbort, tailAbort, temp, cpu->clocktime ) ;
                        tailAbort->time = cpu->clocktime ;
                        tailAbort->delay = temp->arrival - tailAbort->time ;                    
                    }
                }
            }
            else {
                if ( queueSize(cpu->head) < cpu->queueMax ) { // same as the previous steps
                    enqueue(cpu, temp ) ;
                }
                else {
                    addJoblist( headAbort, tailAbort, temp, cpu->clocktime ) ;
                    tailAbort->time = cpu->clocktime ;
                    tailAbort->delay = temp->arrival - tailAbort->time ;                    
                }
            }

            temp = temp->next ;
        }



        cpu->clocktime++ ;
    }

    ofileAllO(ofile, headAbort, headDone) ;

}

void missionmultiCPU( ifstream &file, ofstream &ofile, int numOfCPU ) {
    Node *head = NULL, *tail = NULL, *temp = NULL ;
    CPU* headCPU = NULL, *tailCPU = NULL ;
    Joblist* headAbort = NULL, *tailAbort = NULL, *headDone = NULL, *tailDone = NULL ;
    int clocktime = 0 ;
    inputTrash( file ) ;
    createList( head, tail, file ) ;
    Node* newHead = rewriteList( head ) ;
    temp = newHead ;    
    for ( int i = 1 ; i <= numOfCPU ; i++ ) {
        CPU* newCPU = new CPU ;
        newCPU->cpuNum = i ;
        addCPU( headCPU, tailCPU, newCPU ) ; // create list contant cpus 
    }

    while ( temp != NULL || !allCPUidle(headCPU) ) {
        checkAllTask( headCPU, headAbort, tailAbort, headDone, tailDone ) ; // check all cpu's tasks see if they are done or time out
        while ( temp != NULL && temp->arrival == headCPU->clocktime ) {
            if ( !assignTaskCPU( headCPU, temp ) ) { // the function assignTaskCPU will return bool if it is true then task assigned to one of the cpus
                enqueueCPU( headCPU, temp, headAbort, tailAbort ) ; // if failed to assign then enqueueCPU will try to enqueue to one of the cpu's queue
            }

            temp = temp->next ;
        }

        allCPUclockAdd( headCPU ) ; // all cpu clock time + 1
    }

    ofileAllD( ofile, headAbort, headDone ) ; // write to file 
    
}


void inputfileName( string &filenum ) {
    cout << endl << "input file number:" ;
    cin >> filenum ;  ;

}


bool interface( string &filenum ) {
    int command = 0 ;
    Node* head = NULL ;
    cout << "**** Simulate FIFO Queues by SQF *****" << endl ;
    cout << "* 0. Quit                            *" << endl ;
    cout << "* 1. Sort a file                     *" << endl ;
    cout << "* 2. Simulate one FIFO queue         *" << endl ;
    cout << "* 3. Simulate two queues by SQF      *" << endl ;
    cout << "**************************************" << endl ;
    cout << "Input a command(0, 1, 2, 3):" ;
    cin >> command ;
    if ( command == 0 ) {
        return false ;
    }
    else if ( command == 1 ) {
        inputfileName( filenum ) ;
        ifstream file( "input" + filenum + ".txt" ) ;
        if ( file ) {
            outputfile( "input" + filenum + ".txt" ) ;
            ofstream ofile( "sorted" + filenum + ".txt" ) ;
            mission1( file, ofile ) ;
            cout << "\nsee sorted" << filenum << ".txt\n\n" ;
        }
        else {
            cout << "file input" <<  filenum << ".txt does not exsist\n" ;
        }
    }
    else if ( command == 2 ) {
        if ( filenum == "" ) {
            inputfileName(filenum) ;
        }

        ifstream file( "sorted" + filenum + ".txt" ) ;
        if ( file ) {
            int numOfCPU = 1 ;
            ofstream oflie( "output" + filenum + ".txt" ) ; 
            cout << "\nSimulation is running...\n" ;  
            missionmultiCPU(file, oflie, numOfCPU ) ;
            cout << "see output" << filenum << ".txt\n\n" ;     
        }
        else {
            cout << "file sorted" <<  filenum << ".txt does not exsista!\n" ;    
        }
    }
    else if ( command == 3 ) {
        if ( filenum == "" ) {
            inputfileName(filenum) ;
        }

        ifstream file( "sorted" + filenum + ".txt" ) ;
        if ( file ) {
            int numOfCPU = 2 ;
            ofstream oflie( "double" + filenum + ".txt" ) ;  
            cout << "\nSimulation is running...\n" ;   
            missionmultiCPU(file, oflie, numOfCPU ) ;
            cout << "see double" << filenum << ".txt\n\n" ;     
        }
        else {
            cout << "file sorted" <<  filenum << ".txt does not exsista!\n" ;    
        }
    }
    return true ;
} // call in main() "user interface" : hsi



int main() {
    string filenum = "" ;
    while( interface( filenum ) ) {}
    return 0 ;
}

