// 11127112 莊沛儒 11127133 陳乙菁
# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <cstdlib>
# include <iomanip>
# include <ctime>
# include <string.h>
# include <chrono>
# include <time.h>
# include <sstream>
# include <chrono>

using namespace std;

struct node{
    int oid;
    int arrival;
    int duration;
    int timeout;
    node *next;
} ; // pointer structure

struct doneabort{
    int OID;
    int abortdeparture;
    int delay ;
    doneabort *next;
} ; // done and abort pointer structure

typedef doneabort *Doneabort ;

typedef struct jT{
    int OID;
    int arrival ;
} jobType ; // vector structure

typedef node* Node;

template <class T>
class Queue {
    struct QueueNode {
        T value ;
        QueueNode *next;
    };
    QueueNode *backPtr = NULL , *frontPtr = NULL, *newPtr = NULL, *tempPtr = NULL ;

    public :
        Queue(){
            backPtr = NULL;
            frontPtr = NULL;
        }
        ~Queue(){
            while(frontPtr != NULL ){
                dequeue();
            }
            delete backPtr;
            backPtr = NULL;
        }

        bool isEmpty() const {
            if( frontPtr == NULL) return true;
            else return false;
        }

        void enqueue( T & Q ) { // input new data
            try {
                if( isEmpty() ) { // when the queue is empty
                    frontPtr = new QueueNode () ;
                    frontPtr -> value = Q ;
                    backPtr = frontPtr ;
                    frontPtr -> next = NULL ;
                }
                else {
                    backPtr -> next = new QueueNode () ;
                    backPtr =  backPtr -> next ;
                    backPtr -> value = Q ;
                    backPtr -> next = NULL ;
                }
            }
            catch(std::bad_alloc& ba) {
                std::cerr << endl << "bad_alloc on stack caught:" << ba.what()<< endl ;
            }
        }

        void getFront( T & queueFront) { // get the first one
            if ( !isEmpty() ){
                queueFront = frontPtr -> value ;
            }
        }
        void dequeue(){ // delete the first one
            if ( !isEmpty() ) {
                if( frontPtr == backPtr ){
                    frontPtr = NULL ;
                    backPtr = NULL ;
                }
                else {
                    tempPtr = frontPtr;
                    frontPtr = frontPtr -> next ;
                    delete tempPtr ;
                }
            }
        }

        void dequeue(T & out){ // get the first one and delete it
            if ( !isEmpty() ){
                out = frontPtr -> value ;
                dequeue() ;
            }
        }

        int length(){ // get the queue length
            int i = 0 ;
            tempPtr = frontPtr ;
            for( i = 0; tempPtr != NULL; i++ ) {
                tempPtr = tempPtr -> next ;
            }
            return i ;
        }

};

class JobList{
    private:
        Doneabort done = NULL; // done list
        Doneabort abort = NULL;// abort list
        Node file = NULL ; // inputfile list
        vector<jT>  jt;
        void showJob( Node & currentlist ){
            cout << currentlist -> oid << "\t"
                  << currentlist -> arrival << "\t"
                  << currentlist -> duration << "\t"
                  << currentlist -> timeout << endl;
        } // showJob()
    public:
        bool load(string filename, float & timeload){
            Node temp1;
            bool isfinish = false;
            string temp;
            string s;
            jT temp2;
            int num;
            fstream inputdata;
            auto begin = std::chrono::steady_clock::now(); // Start
            inputdata.open(filename);
            if( inputdata.is_open()){
                inputdata >> temp >> temp >> temp >> temp;
                while(!inputdata.eof()) {
                    inputdata >> temp;
                    if(!inputdata.fail()){
                        if( file == NULL ){
                            file = new node;
                            file -> next = NULL;
                            temp1 = file;
                        }
                        else{
                            temp1 -> next = new node;
                            temp1 = temp1 -> next ;
                            temp1 -> next = NULL;
                        }
                        num = stoi(temp);
                        temp1 -> oid = num;
                        temp2.OID = num;
                        for( int i = 1 ; i<= 3 ; i++ ){
                            inputdata >> temp;
                            num = stoi(temp);
                            switch(i){
                                case 1 : temp1 -> arrival = num;
                                          temp2.arrival = num;
                                          jt.push_back(temp2);
                                case 2: temp1 -> duration = num;
                                case 3: temp1 -> timeout = num;
                            }
                        }
                    }
                }
                inputdata.close();
                auto end = std::chrono::steady_clock::now(); // end time
                timeload = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                return true;
            }
            else {
                cout << "### " << filename << " does not exist! ###"<< endl;
                return false;
            }

        } // load()
        bool loadsorted(string filename){
            Node temp1;
            bool isfinish = false;
            string temp;
            string s;
            int num;
            fstream inputdata;
            inputdata.open(filename);
            if( inputdata.is_open()){ // make sure the file can open
                inputdata >> temp >> temp >> temp >> temp;
                while(!inputdata.eof()) {
                    inputdata >> temp;
                    if(!inputdata.fail()){
                        if( file == NULL ){
                            file = new node;
                            file -> next = NULL;
                            temp1 = file;
                        }
                        else{
                            temp1 -> next = new node;
                            temp1 = temp1 -> next ;
                            temp1 -> next = NULL;
                        }
                        num = stoi(temp);
                        temp1 -> oid = num;
                        for( int i = 1 ; i<= 3 ; i++ ){
                            inputdata >> temp;
                            num = stoi(temp);
                            switch(i){
                                case 1 : temp1 -> arrival = num;
                                case 2: temp1 -> duration = num;
                                case 3: temp1 -> timeout = num;
                            }
                        }
                    }
                }
                inputdata.close();
                return true;
            }
            else {
                cout << "### " << filename << " does not exist! ###"<< endl;
                return false;
            }

        }// loadsorted()

    vector<jobType> Swap(vector<jobType> & str, int a, int b){ // change the order of two number

        int arrival;
        int OID ;

        arrival = str[b].arrival ;
        str[b].arrival = str[a].arrival ;
        str[a].arrival = arrival ;
        OID = str[b].OID ;
        str[b].OID = str[a].OID ;
        str[a].OID = OID ;

        return str ;
    } // Swap()

    void Shellsort( vector<jobType> & str ,float& timesort){

        int a, b = 0 ; // b is the front number, a is the behind number
        int j, k; // j is the front number, k is the behind number
        int length = str.size() ; // vector size
        bool back = false ; // whether go back

        auto begin = std::chrono::steady_clock::now(); // Start

        for( int gap = length / 2 ; gap > 0 ; gap = gap / 2 ) { // decide the gap size
            if( gap == 1 ) a = 1 ;
            else a = gap - 1 ;

            for( a ; a < length ; a++,b++ ) { // smaller than the length
                if( ( str[b].arrival > str[a].arrival ) || ( str[b].arrival == str[a].arrival && str[b].OID > str[a].OID )
                    && back == false ) {
                    // (the front one arrival is bigger than the back one) or (the front one arrival is equal to the back one but the front one OID is bigger than the back one )
                    j = a ;
                    k = b ;
                    str = Swap( str, a, b) ;

                    if( (b-gap+1) >= 0 && b > 0 ) {
                        a = b-1;
                        b = b -(gap-1) -1 ;
                        if(gap==1){
                            a--;
                            b=b-2;
                        }
                        back = true ;
                    }
                }
                else if ( ( str[b].arrival > str[a].arrival ) || ( str[b].arrival == str[a].arrival && str[b].OID > str[a].OID )
                        && back == true ) {
                        // (the front one arrival is bigger than the back one) or (the front one arrival is equal to the back one but the front one OID is bigger than the back one )
                        //  during turn back
                    str = Swap( str, a, b) ;

                    if( (b-gap+1) >= 0 && b > 0 ) {
                        a = a - (gap-1) ;
                        b = b - (gap-1) ;
                        back = true ;
                    }
                    else {
                        back = false ;
                        b = a - 1 ;
                        a = a + (gap-1) -1 ;
                    }
                }
                else if ( str[b].arrival < str[a].arrival && back == true ) {  // stop go back
                    back = false ;
                }
            }
            b = 0;
        }

        auto end = std::chrono::steady_clock::now(); // end time
        timesort = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }

    void change( Node & sortfile, vector<jobType> & jtr ){ // Put the sorted result into sortfile
        Node temp, temp2;
        temp = file; // 原本的頭
        bool found = false;
        for( int i = 0 ; i < jtr.size() ; i++ ){
                while( temp != NULL && found != true ){ // 沒找到就繼續找
                    if( temp -> oid == jtr[i].OID ){
                        if( sortfile == NULL ){
                            sortfile = new node;
                            sortfile -> next = NULL;
                            temp2 = sortfile;
                            temp2-> oid = temp -> oid;
                            temp2 -> arrival = temp -> arrival;
                            temp2 -> timeout = temp -> timeout;
                            temp2 -> duration = temp -> duration;
                        }
                        else{
                            temp2 -> next = new node;
                            temp2 = temp2 -> next;
                            temp2 -> next = NULL;
                            temp2-> oid = temp -> oid;
                            temp2 -> arrival = temp -> arrival;
                            temp2 -> timeout = temp -> timeout;
                            temp2 -> duration = temp -> duration;
                        }
                        found = true;
                    }
                    else{
                        temp = temp -> next;
                    }
                } // while

                found = false;
                temp = file;
        }
    } // change() // create a sort list from sort vector

    vector<jT> GetVector( ){
        return jt;
    }

    Node GetFile(){
        return file;
    }

    void outputfile(Node & sortfile, string num ,float & timewrite){
        auto begin = std::chrono::steady_clock::now(); // Start
        string  filename = "sorted";
        filename = filename + num + ".txt";
        ofstream sortedfile(filename); // create a txt.
        sortedfile << "OID\tArrival\tDuration\tTimeOut\n";
        Node temp = sortfile;
        while(temp != NULL){
            int num1,num2,num3,num4;
            num1 = temp -> oid;
            num2 = temp ->arrival;
            num3 = temp -> duration;
            num4 = temp -> timeout;
            string t = "";
            string tem;
            tem = to_string(num1);
            t = t + tem +"\t";
            tem = to_string(num2);
            t = t + tem +"\t";
            tem = to_string(num3);
            t = t + tem+ "\t";
            tem = to_string(num4);
            t = t + tem+ "\n";
            sortedfile << t;
            temp = temp -> next ;
        }
        sortedfile.close();
        auto end = std::chrono::steady_clock::now(); // end time
        timewrite = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    } // Outputfile()

    void showsort( Node & file){
        Node temp;
        temp = file;
        int i = 1;
        cout << "\tOID\tArrival\tDuration\tTimeOut\n";
        while( temp != NULL ){
            cout << "(" << i << ")\t";
            showJob(temp);
            i++;
            temp = temp -> next;
        }
    } // showsort()

    void eventlistsort( Node tempinputfile, Node & eventList ){ // record the CPU data
        eventList = new node() ;
        eventList -> oid = tempinputfile -> oid ;
        eventList -> arrival = tempinputfile -> arrival ;
        eventList -> duration = tempinputfile -> duration ;
        eventList -> timeout = tempinputfile -> timeout ;
        eventList -> next = NULL ;
    }

    void abortlist(Doneabort & abort, Node tempinputfile, int abortdepartures, int delays ){ // record all the data in abort list
        Doneabort tail ;

        if( abort == NULL ) {
            abort = new doneabort() ;
            abort -> OID = tempinputfile -> oid ;
            abort -> abortdeparture = abortdepartures ;
            abort -> delay = delays ;
            abort -> next = NULL ;
            tail = abort ;
        }
        else {
            for( tail = abort; tail -> next != NULL; tail = tail -> next );
            tail -> next = new doneabort() ;
            tail = tail -> next ;
            tail -> OID = tempinputfile -> oid ;
            tail -> abortdeparture = abortdepartures ;
            tail -> delay = delays ;
            tail -> next = NULL ;
        }
    }

    void donelist(Doneabort & done, Node tempinputfile, int num, int num2 ){ // record all the data in done list
        Doneabort tail, temp;

        if( done == NULL ) {
            done = new doneabort() ;
            done -> OID = tempinputfile -> oid ;
            done -> abortdeparture = num ;
            done -> delay = num2 ;
            done -> next = NULL ;
        }
        else {
            for( tail = done; tail -> next != NULL; tail = tail -> next );
            tail -> next = new doneabort() ;
            tail = tail -> next ;
            tail -> OID = tempinputfile -> oid ;
            tail -> abortdeparture = num ;
            tail -> delay = num2 ;
            tail -> next = NULL ;
        }
    }

    void printdonelist(Doneabort & done) {
        Doneabort temp = done ;
        for( temp; temp != NULL; temp = temp -> next ){
            cout << temp -> OID << " done " << temp -> abortdeparture << " done " << temp -> delay << endl ;
        }
    }

    void printabortlist(Doneabort & abort) {
        Doneabort temp = abort ;
        for( temp; temp != NULL; temp = temp -> next ){
            cout << temp -> OID << " abort " << temp -> abortdeparture << " abort " << temp -> delay << endl ;
        }
    }

    void simulation(Node & inputfile, Doneabort & done, Doneabort & abort ) {
        Queue<Node> bankQuene ; // store queue
        Node temp, show ;
        temp = new node() ;
        Node tempinputfile = inputfile ;
        Node tt = inputfile ;

        Node eventList = NULL ; // store GPU
        done = NULL ;
        abort = NULL ;
        bool end = false ;

        int count = 0 ;

        int time = inputfile -> arrival ;
        eventlistsort( tempinputfile, eventList ) ;
        count = time ;
        tempinputfile = tempinputfile -> next ;

        for( time ; bankQuene.length() != 0 || eventList != NULL ; time++ ){
            bool empty = false ;
            if( time == ( count + eventList -> duration ) ) { // success
                donelist( done, eventList , ( count + eventList -> duration), ( count - eventList -> arrival ) ) ;
                eventList = NULL ;
                empty = true ;
            }
            else if ( eventList -> timeout <= time ){ // over timeout during still running
                abortlist( abort, eventList, eventList -> timeout, eventList -> timeout - eventList -> arrival ) ;
                eventList = NULL ;
                empty = true ;
            }

            bool find = false ;
            if( empty == true ) {
                if( bankQuene.length() != 0 ) {
                    while( find == false && bankQuene.length() != 0 ) {
                        bankQuene.getFront(show) ;
                        if ( show -> timeout <= time ) { // take out data from queue but is overtime
                            abortlist( abort, show, time, ( time - show -> arrival ) ) ;
                            bankQuene.dequeue() ;
                            find = false ;
                        }
                        else{ // take out data from queue which is not overtime
                            find = true ;
                            bankQuene.dequeue(temp) ;
                            eventlistsort( temp, eventList ) ;
                            count = time ;
                        }
                    }
                }
            }

            if ( tempinputfile != NULL && time == tempinputfile -> arrival ){ // read from inputfile
                if( eventList == NULL ) {
                    eventlistsort( tempinputfile, eventList ) ;
                    count = time ; // 161
                }
                else if( bankQuene.length() < 3 ) bankQuene.enqueue(tempinputfile) ; // put data in line
                else abortlist( abort, tempinputfile, tempinputfile -> arrival, 0 ) ;// over three data in line

                tempinputfile = tempinputfile -> next ;
            }

            if ( eventList == NULL && bankQuene.length() == 0 && tempinputfile != NULL){
                eventlistsort( tempinputfile, eventList ) ;
                time = tempinputfile -> arrival ;
                count = time ;
                tempinputfile = tempinputfile -> next ;
            }

            if ( tempinputfile != NULL && tempinputfile -> arrival == time ) time = time - 1 ; // if next data is in the same time
        }
    };

    Doneabort Getabort(){
        return abort ;
    }

    Doneabort Getdone(){
        return done ;
    }

    void cccount(Doneabort & abort,Doneabort & done, float & averagedelay, float & successrate){ // count average delay and success rate
        Doneabort temp = abort;
        Doneabort temp1 = done;
        int num = 0, sum = 0;
        int i = 0, j = 0 ;

        for( i = 0; temp != NULL; temp = temp -> next, i++ ){
            num = num + temp->delay ;
        }

        for( j = 0; temp1 != NULL; temp1 = temp1 -> next, j++ ){
            sum = sum + temp1->delay ;
        }

        averagedelay = (float)(num+sum)/(i+j);
        successrate = (float)100*j/(i+j);
    }

    void outputsimulation(Doneabort & done, Doneabort & abort,string num, float averagedelay, float successrate ){ // create simulation output.txt
        string  filename = "output";
        filename = filename + num + ".txt";
        ofstream sortedfile(filename);
        sortedfile << "\t[Abort Jobs]\n"<<"\tOID\tAbort\tDelay\n";
        Doneabort temp = abort;
        int i = 1;
        while(temp != NULL){
            int num1,num2,num3;
            num1 = temp ->OID;
            num2 = temp ->abortdeparture;
            num3 = temp ->delay;
            string t = "";
            string tem;
            tem = to_string(i);
            t = t + "[" + tem + "]" + "\t";
            tem = to_string(num1);
            t = t + tem +"\t";
            tem = to_string(num2);
            t = t + tem +"\t";
            tem = to_string(num3);
            t = t + tem+ "\n";
            sortedfile << t;
            temp = temp -> next ;
            i = i + 1;
        }
        sortedfile << "\t[Jobs Done]\n"<<"\tOID\tDeparture\tDelay\n";
        temp = done;
        i = 1;
        while(temp != NULL){
            int num1,num2,num3;
            num1 = temp ->OID;
            num2 = temp ->abortdeparture;
            num3 = temp ->delay;
            string t = "";
            string tem;
            tem = to_string(i);
            t = t + "["+tem+"]" + "\t";
            tem = to_string(num1);
            t = t + tem +"\t";
            tem = to_string(num2);
            t = t + tem +"\t";
            tem = to_string(num3);
            t = t + tem+ "\n";
            sortedfile << t;
            temp = temp -> next ;
            i = i + 1;
        }
        string t = "";
        stringstream tem;
        tem.precision(2);
        tem.setf(std::ios::fixed);
        tem << averagedelay;
        t = tem.str();
        sortedfile << "[Average Delay]"<< t << " ms\n";
        tem.str("");
        tem.precision(2);
        tem.setf(std::ios::fixed);
        tem << successrate;
        t = tem.str();
        sortedfile << "[Success Rate]"<< t << " %\n";
        sortedfile.close();
    }
};



int main(){
    int command = 0;
    float timeread,timesort,timewrite;
    string fileName = "input";
    string num;
    bool firsttime = true; // firsttime to input
    Node file;
    Node sortfile = NULL;
    vector<jT> jt;
    JobList inputfile;
    Doneabort done ;
    Doneabort abort ;
    float averagedelay, successrate;
    do {
            Node file = NULL;
            Node sortfile = NULL;
            vector<jT> jt;
            Doneabort done = NULL;
            Doneabort abort = NULL;
            JobList inputfile;
            cout << endl << "**** Simulate FIFO Queues by SQF *****";
            cout << endl << "*0.Quit                              *";
            cout << endl << "*1.Sort a file                       *";
            cout << endl << "*2.Simulate one FIFO queue           *";
            cout << endl << "**************************************";
            cout << endl << "Input a command(0, 1, 2):";
            cin >> command; // get the command
            if( ( command == 1 || command == 2 ))
            {
                if( firsttime == true || command == 1){
                    fileName = "input";
                    cout << endl << "Input a file number:";
                    cin >> num;
                    cout<<endl;
                    fileName = fileName + num + ".txt";
                }
                if( command == 1){
                    if(inputfile.load(fileName,timeread)){
                        file = inputfile.GetFile();
                        jt = inputfile.GetVector();
                        inputfile.Shellsort(jt,timesort);
                        inputfile.change(sortfile, jt);
                        inputfile.outputfile(sortfile,num,timewrite);
                        inputfile.showsort(file);
                        cout << endl<<"Reading data: "<< (int)timeread << " clocks" << "("
                              << fixed<<setprecision(2) << timeread <<"ms)."<< endl;
                        cout << "Sorting data: "<< (int)timesort << " clocks" << "("
                              << fixed<<setprecision(2) << timesort <<"ms)."<< endl;
                        cout << "Writing data: "<< (int)timewrite << " clocks" << "("
                              << fixed<<setprecision(2) << timewrite <<"ms)."<< endl;
                        fileName = "See sorted"+num+".txt";
                        cout << fileName << endl;
                    }
                }
                else if (command == 2){
                    fileName = "sorted";
                    fileName = fileName + num + ".txt";
                    if(inputfile.loadsorted(fileName)){
                        file = inputfile.GetFile();
                        inputfile.simulation(file, done, abort);
                        inputfile.cccount(abort,done,averagedelay,successrate);
                        inputfile.outputsimulation(done,abort,num,averagedelay,successrate);
                        cout << endl << "The simulation is running...\n";
                        cout << "See output" << num << ".txt\n";
                    }

                }

                firsttime = false;
        }


    } while( command != 0);
}
