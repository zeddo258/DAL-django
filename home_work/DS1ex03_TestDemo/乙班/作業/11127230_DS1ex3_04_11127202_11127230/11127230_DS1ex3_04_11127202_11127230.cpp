// 11127202 郭立綸 11127230 鍾帛勳

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;

const int QUEUELIMIT = 3;

struct List { // struct of data
    int OID;
    int arrival;
    int duration;
    int timeout;
};

class Queue {
	struct node {
		int OID;
		int arrival;
		int duration;
		int timeout;
		node* next;
	};

	typedef node* nodeptr;

public:
    nodeptr tail = NULL;
	int size = 0;
	void push( List job ) { // push job into queue
		size++;
		nodeptr temp = new node;
		temp->OID = job.OID;
		temp->arrival = job.arrival;
		temp->duration = job.duration;
		temp->timeout = job.timeout;
		if (tail == NULL) {
			tail = temp;
			tail->next = temp;
		}
		else {
			temp->next = tail->next;
			tail->next = temp;
			tail = tail->next;
		}
	}

	void pop() { // pop the head of the queue
		size--;
        nodeptr temp;
        temp = tail->next;
        tail->next = tail->next->next;
        delete temp;
        temp = NULL;
		if ( size == 0 ) {
            tail = NULL;
        }
	}

	int getsize() {
		return size;
	}

	bool isempty() {
	    if ( size == 0 ) {
            return true;
        }

        return false;

	}

};


class JobList {
public:

	vector<struct List> job;
    List temp;

    bool comp(List a, List b) {
    	if (a.arrival < b.arrival) {
    		return true;
    	}
    	else if (a.OID < b.OID && a.arrival == b.arrival) {
    		return true;
    	}

    	return false;
    }

    void swap(List & a, List & b) {
    	List t;
    	t = b;
    	b = a;
    	a = t;
    }

    void shellsort(vector<struct List> & a) {
    	int gap = a.size()/2;
    	int i, j, k;
    	while (gap >= 1) {
    		for (i = gap; i < a.size(); i++) {
    			for (k = i, j = i-gap; j >= 0 && comp(a[k], a[j]); j-=gap, k-=gap) {
    				swap(a[k], a[j]);
    			}
    		}

    		gap/=2;
    	}
    }

    void Print1(string filenum){ // target1 output
    	ofstream myfile;
    	string filetxt = "sorted" + filenum + ".txt";
		myfile.open (filetxt);
    	int i;
    	myfile << "OID" << "\t" << "Arrival Duration" << "\t" << "TimeOut" << endl;
    	for (i = 1; i <= job.size(); i++) {
			myfile << job[i-1].OID << "\t" <<
			job[i-1].arrival << "\t" <<
			job[i-1].duration << "\t" <<
			job[i-1].timeout << endl;
		}
		
		myfile.close();
    }
    
    void printelse() { // target1 output
    	cout << endl << "\t" << "OID" << "\t" << "Arrival Duration" << "\t" << "TimeOut" << endl;
    	for (int i = 1; i <= job.size(); i++) {
			cout << "(" << i << ")\t" << job[i-1].OID << "\t" <<
			job[i-1].arrival << "\t" <<
			job[i-1].duration << "\t" <<
			job[i-1].timeout << endl;
		}
    }
};


class Ans{

	struct Abort{
		int OID, abort, delay;
	};

    struct Done{
		int OID, departure, delay;
	};

	vector< struct Abort > abort;
	vector< struct Done > done;
	float delay = 0;

public:
    void AddAbort( vector<struct List> & a, int use ){
        Abort temp;
        temp.OID = a[0].OID;
        if ( use == 1 ){
            temp.abort = a[0].arrival;
            temp.delay = 0;
        }

        else if ( use == 2 ){
            temp.abort = a[0].timeout;
            temp.delay = a[0].timeout - a[0].arrival;
        }
        delay += temp.delay;
        abort.push_back(temp);
        a.erase(a.begin());
    }

    void AddAbort2( Queue &q, int curtime, int use ){
        Abort temp;
        temp.OID = q.tail->next->OID;
        if ( use == 1 ){
            temp.abort = q.tail->next->arrival;
            temp.delay = 0;
        }

        else if ( use == 2 ){
            temp.abort = q.tail->next->timeout;
            temp.delay = q.tail->next->timeout - q.tail->next->arrival;
        }

        else if ( use == 3 ){
            temp.abort = curtime;
            temp.delay = curtime - q.tail->next->arrival;
        }
        delay += temp.delay;
        abort.push_back(temp);
        q.pop();
    }

    void AddDone( List running, int curtime ){
        Done temp;
        temp.OID = running.OID;
        temp.departure = curtime;
        temp.delay = curtime - running.arrival -running.duration;
        delay += temp.delay;
        done.push_back(temp);
    }

    void print2(string filenum){
    	ofstream myfile;
    	string filetxt = "output" + filenum + ".txt";
		myfile.open (filetxt);
        myfile << "\t[Abort Jobs]" << endl << "\tOID\tAbort\tDelay" << endl;
        for( int i = 0; i < abort.size(); i++ ){
            myfile << "[" << i+1 << "]\t" << abort[i].OID << "\t" << abort[i].abort << "\t" << abort[i].delay << endl;
        }
        myfile << "\t[Done Jobs]" << endl << "\tOID\tDeparture\tDelay" << endl;
        for( int i = 0; i < done.size(); i++ ){
            myfile << "[" << i+1 << "]\t" << done[i].OID << "\t" << done[i].departure << "\t" << done[i].delay << endl;
        }

        double avgdel = (double)delay/(abort.size()+done.size());
        double sucrate = (double)done.size()/(abort.size()+done.size())*100;
        myfile << "[Average Delay]\t" << fixed << setprecision(2) << avgdel << " ms" << endl;
        myfile << "[Success Rate]\t" << fixed << setprecision(2) << sucrate << " %" << endl;
        myfile.close();
    }
    

};

class Simulation {

    Queue q; 
    Ans ans; // save abortlist and donelsit 
	List running; // job that will be add to done list next
    int curtime = 0; // arrival time of the last data
    int departure = 0; // the cpu current time

public:

	void Simulate(vector<struct List> job) {
		ChangeRunning( running, job[0].OID, job[0].arrival, job[0].duration, job[0].timeout ); // set the top of the job into running 
		job.erase(job.begin()); 
		departure = running.arrival + running.duration; 
		curtime = running.arrival; 

		while( !job.empty() ){
            if ( job[0].arrival < departure ){ // arrival too early go to queue
                curtime = job[0].arrival; // this line is not neccessary
                if ( q.getsize() < QUEUELIMIT ){ // queue is not full 
                    q.push(job[0]); // push into queue
                    job.erase(job.begin()); // go to next job
                }
                else
                    ans.AddAbort( job, 1 ); // queue is full abort
            }
            else if ( running.OID == 0 ){ // there are not thing to run
            	if ( curtime + job[0].duration <= job[0].timeout ){ // see top of job is timeout or not
                	ChangeRunning( running, job[0].OID, job[0].arrival, job[0].duration, job[0].timeout ); // change it into next running item
                	job.erase(job.begin()); // pop it
                	if ( curtime < running.arrival ) // renew the curtime
                		curtime = running.arrival;

                	departure = running.duration + curtime; // renew departure
                }
                else { // this else will never do
                    curtime = job[0].arrival; 
                	ans.AddAbort( job, 2 );
                }
            }
            else{
                Doqueue( running, curtime, departure ); // push the running into done
            }
		}

		while ( !q.isempty() ){ // queue have things
            Doqueue( running, curtime, departure );
		}

		if ( running.OID != 0 ) { // if the last thing islegal do it 
			ans.AddDone( running, curtime );
		}


	}

	void Print2(string filenum) {
		ans.print2(filenum);
	}

	void ChangeRunning( List &running, int OID, int arrival, int duration, int timeout ){
		running.OID = OID;
		running.arrival = arrival;
		running.duration = duration;
		running.timeout = timeout;
	}

    void Doqueue( List &running, int &curtime, int &departure ){
        curtime = departure; // set curtime same to last item departure
        ans.AddDone( running, curtime );
        running.OID = 0; // set default velue to zero
        int dotime = q.getsize();
        for ( int i = 0; i < dotime; i++ ){
            if ( curtime + q.tail->next->duration <= q.tail->next->timeout ){ // if find the legal item in queue
                ChangeRunning( running, q.tail->next->OID, q.tail->next->arrival, q.tail->next->duration, q.tail->next->timeout );
                q.pop();
                departure = curtime + running.duration;
                i = dotime;
            }
            else{
                if ( q.tail->next->timeout > curtime ){ // timeout when running
                    curtime = q.tail->next->timeout;
                    ans.AddAbort2( q, curtime, 2 );
                }

                else{ // timeout when get
                    ans.AddAbort2( q, curtime, 3 );
                }
            }

        }
    }

};


bool input(JobList & jobs, string & filenum, string & com) {
	ifstream in;
	string filestr = "";
	bool getinput = false;
	while (true) {
		cout << endl << "**** Simulate FIFO Queues by SQF *****" << endl;
		cout << "* 0. Quit                            *" << endl;
		cout << "* 1. Sort a file                     *" << endl;
		cout << "* 2. Simulate one FIFO queue         *" << endl;
		cout << "**************************************" << endl;
		cout << "Input a command(0, 1, 2): ";
		cin >> com;

		
		if (com == "0") {
			return false;
		}

		if (com == "1" || (com == "2" && filenum == "NO")) {
			cout << endl << "Input a file number: ";
			cin >> filenum;
		}
		else if (!(com == "1" || com == "2")){
			cout << endl << "Command does not exist!" << endl;
			continue;
		}

		if (com == "1") {
			filestr = "input" + filenum + ".txt";
		}
		else {
			filestr = "sorted" + filenum + ".txt";
		}
		
		in.open(filestr);
		if (in.fail()) {
		     cout << endl << "### " << filestr + " does not exist!" << " ###" << endl;
		     continue;
		}

		int i, j;
		int a, b, c, d;
		string sa, sb, sc, sd;
		in >> sa >> sb >> sc >> sd;
		while (in >> a >> b >> c >> d) {
			jobs.temp.OID = a;
			jobs.temp.arrival = b;
			jobs.temp.duration = c;
			jobs.temp.timeout = d;
			jobs.job.push_back(jobs.temp);
		}
		
		in.close();
		return true;
	}
}



int main() {
	string filenum = "NO";
	string com;
	time_t rst, red, sst, sed, wst, wed;
	while (true) {
		JobList jobs; // save input data
		rst = clock();
		if (!(input(jobs, filenum, com))) { // input unsorted data into jobs
			break;
		}
		red = clock();

		if (com == "1") {
			jobs.printelse();
			sst = clock();
			jobs.shellsort(jobs.job);  // save sorted data into jobs
			sed = clock();
			wst = clock();
			jobs.Print1(filenum); // target1 write data 
			wed = clock();
			
			cout << endl << "Reading data: " << (int)(red - rst) / CLOCKS_PER_SEC
    		<< " clocks" << " (" << fixed << setprecision(2) << (double)(red - rst) / CLOCKS_PER_SEC <<  " ms)." << endl;
			cout << "Sorting data: " << (int)(sed - sst) / CLOCKS_PER_SEC
    		<< " clocks" << " (" << fixed << setprecision(2) << (double)(sed - sst) / CLOCKS_PER_SEC <<  " ms)." << endl;
    		cout << "Writing data: " << (int)(wed - wst) / CLOCKS_PER_SEC
    		<< " clocks" << " (" << fixed << setprecision(2) << (double)(wed - wst) / CLOCKS_PER_SEC <<  " ms)." << endl;
    		cout << endl << "See sorted" << filenum << ".txt" << endl;
		}
		else {
			Simulation solve;
			solve.Simulate(jobs.job); 
			solve.Print2(filenum); // target2 write data
			cout << endl << "The simulation is running..." << endl;
			cout << "See output" << filenum << ".txt" << endl;
		}

	}

	return 0;
}
