# include <iostream>
#include <string>
#include <fstream>
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
    int time = 0, delay = 0 ;
    Node* nodeJ = NULL ;
    Joblist* next = NULL ;
};

struct CPU {
    int clocktime = 0, queueMax = 3, taskenterTime = 0  ;
    bool idle = true ;
    Node* task = NULL ;
    Queue* head = NULL ;
    Queue* tail = NULL ;
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
}

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
} 

int joblistSize( Joblist* head ) {
    int i = 0 ;
    while( head != NULL ) {
        head = head->next ;
        i++ ;
    }

    return i ;

}

int input( ifstream &file ) { // return one int value from the file
    int num = 0 ;
    file >> num ;
    return num ;
}

void inputTrash( ifstream &file ) {
    string temp ;
    getline( file, temp ) ;
}

void outputfile( string fileName ) {
    string temp ;
    int i = 1 ;
    ifstream file(fileName) ;
    getline(file, temp) ;
    cout << "\t" << temp << endl ;
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

}

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
}

Node* dequeue( Queue* &headQueue ) {
    Queue* temp = headQueue ;
    Node* tempNode = NULL ;
    if ( !isQEmpty(headQueue) ) {
        tempNode = headQueue->nodeQ ;
        headQueue = headQueue->next ;
        delete temp ;
    }

    return tempNode ;


}

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
}

float successRate( Joblist* headA, Joblist* headD ) {
    if ( joblistSize(headA) + joblistSize(headD) != 0 ) {
        float sr = joblistSize(headD) ;
        sr = sr / (joblistSize(headA) + joblistSize(headD)) * 100 ;
        return sr ;
    }
    else {
        return 0 ;
    }
}
void outputNode( Node* node ) {
    cout  << node->oid << "\t" ;
    cout  << node->arrival << "\t" ;
    cout  << node->duration << "\t" ;
    cout  << node->timeout << "\n"  ;
}

void ofileNodeS( ofstream &file, Node* node ) {
    file << node->oid << "\t" << node->arrival << "\t\t" << node->duration << "\t\t\t" << node->timeout << endl ;
} // S stand for sorted 

void ofileNodeO( ofstream &file, Joblist* jobNode ) {
    file << jobNode->nodeJ->oid << "\t" << jobNode->time << "\t" << jobNode->delay << endl ;
}

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
}

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

    ofile << "[Average Delay]\t" << averageDelay(tempA, tempD) << " ms\n" ;
    ofile << "[Success Rate]\t" << successRate(tempA, tempD) << " %\n" ;

}



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
}

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


}

Node * accessNode( Node * head , int nodeNum ) { // access the node on the list by the number of node

    for ( int i = 1 ; i < nodeNum ; i++ ) { 
        head = head->next ;
    }   

    return head ;
}

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
        }

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

bool processData( CPU* &cpuptr ) {

    cpuptr->idle = true ;
    if ( cpuptr->clocktime + cpuptr->task->duration <= cpuptr->task->timeout ) {
        return true ;
    }
    else {
        return false ;
    }



}

void assignTask( CPU* &cpuptr, Node* node ) {
    cpuptr->task = node ;
    cpuptr->idle = false ;
    cpuptr->taskenterTime = cpuptr->clocktime ;
}

void assignTaskRepeat( CPU* &cpu, Joblist* &head, Joblist* &tail ) {
    while ( cpu->idle && !isQEmpty(cpu->head)) {
        assignTask( cpu, dequeue(cpu->head)) ;
        if ( checkIftimeout( cpu ) ) {
                addJoblist( head, tail, cpu->task, cpu->clocktime ) ;
                tail->time = cpu->clocktime ;
                tail->delay = tail->time - tail->nodeJ->arrival ;
                cpu->idle = true ;
        }
    }

    

}

void shell_sort( Node* &head, Node* &tail , bool cmpArr ) {
    int gap = listSize( head ) / 2 ;
    while ( gap > 0 ) {
        for ( int i = 1 ; i + gap <= listSize( head ) ; i++ ) {
            Node * node1 = accessNode( head, i ) ;
            Node * node2 = accessNode( head, i + gap ) ;
            int temp = i ;
            if ( cmpArr ) {
                while ( node1->arrival > node2->arrival && temp >= 1 ) {
                    swap( head, tail , node1, node2 ) ;
                    temp = temp - gap ; 
                    if ( temp >= 1 ) {
                        node1 = accessNode( head, temp ) ;
                    } 
                }

            }
            else {
                while ( node1->oid > node2->oid && temp >= 1 ) {
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
} // willis

void createList( Node* &head, Node* &tail, ifstream &file ) {
    while( file ) {
        add( head, tail, file ) ;
    }

    Node* node = accessNode( head, listSize( head ) ) ;
    deleteNode( head, tail, node ) ;
}

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

Node* mission1( ifstream &file, ofstream &ofile ) {
    Node *head = NULL, *tail = NULL, *temp = NULL ;
    bool cmpArr = true ;
    inputTrash( file ) ;
    createList( head, tail, file ) ;
    shell_sort( head, tail , cmpArr ) ;
    temp = head ;
    cmpArr = false ;
    while ( temp != NULL ) {
        if ( temp->head != NULL ) {
            shell_sort( temp->head, temp->tail, cmpArr ) ;
        }

        temp = temp->next ;
    }

    ofileAllS( ofile, head ) ;
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
    Node* newHead = rewriteList( head ) ;
    temp = newHead ;
    while ( temp != NULL || !cpu->idle ) {
        if ( !cpu->idle ) {
            if ( cpu->taskenterTime + cpu->task->duration == cpu->clocktime ) {
                addJoblist( headDone, tailDone, cpu->task, cpu->clocktime ) ;
                tailDone->time = cpu->clocktime ;
                tailDone->delay = cpu->taskenterTime - tailDone->nodeJ->arrival ;
                cpu->idle = true ;
                assignTaskRepeat( cpu, headAbort, tailAbort) ;
            }
            else if ( checkIftimeout( cpu ) ) {
                addJoblist( headAbort, tailAbort, cpu->task, cpu->clocktime ) ;
                tailAbort->time = cpu->clocktime ;
                tailAbort->delay = tailAbort->time - tailAbort->nodeJ->arrival ;
                cpu->idle = true ;
                assignTaskRepeat( cpu, headAbort, tailAbort) ;
            }


        }


        while ( temp != NULL && temp->arrival == cpu->clocktime ) {
            if ( cpu->idle ) {
                if ( isQEmpty( cpu->head ) ) {
                    assignTask( cpu, temp ) ;
                }
                else {
                    assignTask( cpu, dequeue(cpu->head)) ;
                }
            }
            else {
                if ( queueSize(cpu->head) < cpu->queueMax ) {
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
    cout << "**************************************" << endl ;
    cout << "Input a command(0, 1, 2):" ;
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
            ofstream oflie( "output" + filenum + ".txt" ) ;   
            mission2(file, oflie) ;
            cout << "\nsee output" << filenum << ".txt\n\n" ;     
        }
        else {
            cout << "file sorted" <<  filenum << ".txt does not exsista!\n" ;    
        }
    }
    else {
        cout << "\nCommand does not exist!\n\n" ;
    }

    return true ;
} // call in main() "user interface" : hsi



int main() {
    string filenum = "" ;
    while( interface( filenum ) ) {}
    return 0 ;
}

