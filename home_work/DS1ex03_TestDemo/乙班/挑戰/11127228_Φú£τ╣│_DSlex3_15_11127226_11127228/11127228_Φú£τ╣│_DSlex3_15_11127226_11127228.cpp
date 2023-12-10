// 11127226 �����h, 11127228 �P�q�x, 15��

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <vector>

#define MAX_LEN 255

using namespace std;

typedef struct jt {
    int OID;
    int arrival;
    int duration;
    int timeout;
} jobType;

class JobQueue {
    typedef struct jq{
        jobType data ;
        struct jq *next;
    } jobNode;

    jobNode *head, *tail ;

public:
    JobQueue():head(NULL), tail(NULL)
    {}

    bool isEmpty(){ // �P�_�O�_����
        if(head == NULL)
            return true ;
        else
            return false ;
    }

    void enqueue(jobType c){ // �[�J��Ʃ����
        if(isEmpty()){
            head = new jobNode() ;
            head->data = c;
            head->next = NULL ;
            tail = head ;
        }
        else{
            tail->next = new jobNode();
            tail = tail->next ;
            tail->data = c ;
            tail->next = NULL ;
        }
    }

    jobType dequeue(){ // �q��ƫe�����X
        jobType top = head->data;
        jobNode *del = head ;
        head = head->next;
        del->next = NULL;
        delete del;
        del = NULL;
        return top ;
    }

    jobType getfirst(){ // �q��ƫe�����X
        jobType top = head->data;
        return top ;
    }

    int length(){
        int n = 0 ;
        jobNode *current = head;
        while(current != NULL){
            n++;
            current = current->next;
        }
        return n ;
    }
};

class AnsList { // �s��abort�Mdone�����
    typedef struct aT {
        int OID;
        int CPU;
        int abort_time;
        int delay;
    } abortType;

    typedef struct dT {
        int OID;
        int CPU;
        int departure;
        int delay;
    } doneType;

    vector<abortType> abortJobs;
    vector<doneType> doneJobs;
    double avgDelay;
    double sucessRate;

public:
    string fileID;
    void add_abort(int ID, int atime, int delay){ // ��i�����M�椤
        abortType newdata;
        newdata.abort_time = atime, newdata.delay = delay, newdata.OID = ID ;
        abortJobs.push_back(newdata);
    }
    void add_abort_double(int ID, int CPU, int atime, int delay){ // ��i�����M�椤
        abortType newdata;
        newdata.abort_time = atime, newdata.delay = delay, newdata.OID = ID , newdata.CPU = CPU;
        abortJobs.push_back(newdata);
    }
    void add_done(int ID, int atime, int delay){ // ��i�����M�椤
        doneType newdata;
        newdata.departure = atime, newdata.delay = delay, newdata.OID = ID ;
        doneJobs.push_back(newdata);
    }
    void add_done_double(int ID, int CPU, int atime, int delay){ // ��i�����M�椤
        doneType newdata;
        newdata.departure = atime, newdata.delay = delay, newdata.OID = ID, newdata.CPU = CPU ;
        doneJobs.push_back(newdata);
    }
    bool export_file(){ // �g��
        ofstream out;
        fileID = "output" + fileID + ".txt";
        out.open(fileID);

        if (out.fail()) { // �ˬdŪ�ɬO�_���\
            cout << "Error: output file opening failed." << endl;
            return false;
        }
        double total_jobs = 0, total_delay = 0, sucess = 0;
        int n = abortJobs.size();
        out << "\t[Abort Jobs]" ;
        out << endl << "\tOID\tAbort\tDelay\n";
        for (int i = 0; i < n; i++) {
            out << "[" << i+1 << "]\t" << abortJobs[i].OID << "\t" << abortJobs[i].abort_time << "\t" << abortJobs[i].delay;
            if (i != n-1)
                out << endl;
            total_jobs++;
            total_delay += abortJobs[i].delay;
        }

        n = doneJobs.size();
        out << endl << "\t[Jobs Done]" ;
        out << endl << "\tOID\tDeparture\tDelay\n";
        for (int i = 0; i < n; i++) {
            out << "[" << i+1 << "]\t" << doneJobs[i].OID << "\t" << doneJobs[i].departure << "\t" << doneJobs[i].delay;
            if (i != n-1)
                out << endl;
            total_jobs++;
            total_delay += doneJobs[i].delay;
            sucess++;
        }

        avgDelay = total_delay / total_jobs ; // �p�⥭��delay
        sucessRate = 100 * sucess / total_jobs; // �p�⦨�\�v

        out << endl << "[Average Delay]\t" << fixed << setprecision(2) << avgDelay << " ms" << endl;
        out << "[Success Rate]\t" << fixed << setprecision(2) << sucessRate << " %" << endl;
        out.close();
        return true ;
    };

    bool export_file_double(){ // �g��
        ofstream out;
        fileID = "double" + fileID + ".txt";
        out.open(fileID);

        if (out.fail()) { // �ˬdŪ�ɬO�_���\
            cout << "Error: output file opening failed." << endl;
            return false;
        }
        double total_jobs = 0, total_delay = 0, sucess = 0;
        int n = abortJobs.size();
        out << "\t[Abort Jobs]" ;
        out << endl << "\tOID\tCID\tAbort\tDelay\n";
        for (int i = 0; i < n; i++) {
            out << "[" << i+1 << "]\t" << abortJobs[i].OID << "\t" << abortJobs[i].CPU << "\t" << abortJobs[i].abort_time << "\t" << abortJobs[i].delay;
            if (i != n-1)
                out << endl;
            total_jobs++;
            total_delay += abortJobs[i].delay;
        }

        n = doneJobs.size();
        out << endl << "\t[Jobs Done]" ;
        out << endl << "\tOID\tCID\tDeparture\tDelay\n";
        for (int i = 0; i < n; i++) {
            out << "[" << i+1 << "]\t" << doneJobs[i].OID << "\t" << doneJobs[i].CPU << "\t" << doneJobs[i].departure << "\t" << doneJobs[i].delay;
            if (i != n-1)
                out << endl;
            total_jobs++;
            total_delay += doneJobs[i].delay;
            sucess++;
        }

        avgDelay = total_delay / total_jobs ; // �p�⥭��delay
        sucessRate = 100 * sucess / total_jobs; // �p�⦨�\�v

        out << endl << "[Average Delay]\t" << fixed << setprecision(2) << avgDelay << " ms" << endl;
        out << "[Success Rate]\t" << fixed << setprecision(2) << sucessRate << " %" << endl;
        out.close();
        return true ;
    };
};

class Simulation { // ����CPU
    vector<jobType> event_list ; // �s����ȲM��
    AnsList alist ; // �s������P�����M��
    JobQueue jbq ; //��C
    int current_data = 0 ;
    int cpu_time = 0 ;
    int maxQueue = 3 ;

public:

    void S_copy_event( vector<jobType> &temp ){ // �N���ȲM��[�Jtemp
        for( int i = 0; i < event_list.size(); i++ ){
            temp.push_back(event_list[i]) ;
        }
    }

    void S_copy_temp( vector<jobType> temp, string fileName ){ // �Ntemp��ƥ[�J���ȲM��
        for( int i = 0; i < temp.size(); i++ ){
            event_list.push_back(temp[i]) ;
        }
        alist.fileID = fileName ;
    }

    bool import_file(const string& fileName) { //Ū��
        alist.fileID = fileName ;
        ifstream in;
        string fileID = "sorted" + fileName + ".txt";
        in.open(fileID);

        if (in.fail()) {
            cout << "Error: input file opening failed or does not exist." << endl;
            return false;
        }

        for (int i = 0; i < 4; i++) {
            string field;
            in >> field;
        }

        while (!in.eof()) {
            jobType newData;
            in >> newData.OID >> newData.arrival >> newData.duration >> newData.timeout;
            if (!in.fail())
                event_list.push_back(newData);
        }

        in.close();
        return true;
    }

    void cpu(jobType data){ // cpu�@�~
        if (data.arrival > cpu_time) // ��scpu_time
            cpu_time = data.arrival;
        if(cpu_time > data.timeout){ // ��ɶ��w�W�L�ɭ��h����
            int ID = data.OID, atime, delay ;
            atime = cpu_time ;
            delay = atime - data.arrival;
            alist.add_abort(ID, atime, delay);
        }
        else if((cpu_time + data.duration) > data.timeout){ // ������W�L�ɭ��h����
            int ID = data.OID, atime, delay ;
            atime = data.timeout ;
            delay = atime - data.arrival;
            alist.add_abort(ID, atime, delay);
            cpu_time = data.timeout; // ��s�ܨ����ɶ�
        }
        else{ // ���S���D�h����
            int ID = data.OID, atime, delay ;
            atime = data.duration + cpu_time;
            delay = cpu_time - data.arrival;
            alist.add_done(ID, atime, delay);
            cpu_time = atime; // ��s�ܧ����ɶ�
        }
    }

    void implement(){
        cout << endl << "The simulation is running..." << endl;
        while(current_data < event_list.size()){
            while(jbq.length() < maxQueue){ // queue�̳̦h3�����
                if(current_data >= event_list.size()) // ��Ҧ���Ʊ�������X
                    break;

                else if(event_list[current_data].arrival >= cpu_time ){ // �U�@����ƪ���F�ɶ��j��CPU�ɶ��h�[�J��C
                    jbq.enqueue(event_list[current_data]);
                    current_data++;
                }
                else{ // �p��h��������
                    int ID = event_list[current_data].OID, atime, delay ;
                    atime = event_list[current_data].arrival ;
                    delay = 0 ;
                    alist.add_abort(ID, atime, delay);
                    current_data++;
                }
            }

            if(jbq.length() > 0 && event_list[current_data].arrival >= cpu_time){ // queue�̭�����ƥB�U�@����Ʃ�F�ɶ��j��cpu���ɶ�
                jobType data = jbq.dequeue(); // �q��C���X���
                if(event_list[current_data].arrival >= cpu_time ){ // �s�K��Ʃ��C
                    jbq.enqueue(event_list[current_data]);
                    current_data++;
                }
                if(cpu_time > data.timeout){ // ���X����Ʈɭ��w��h����
                    int ID = data.OID, atime, delay ;
                    atime = cpu_time ;
                    delay = atime - data.arrival ;
                    alist.add_abort(ID, atime, delay);
                }
                else{ // �S���D�h�e�JCPU
                    cpu(data);
                }
            }
            else{ // �N�����ƵL�k���W�ثe�ɶ�������
                while( event_list[current_data].arrival < cpu_time ){
                    if(current_data >= event_list.size())
                        break;
                    int ID = event_list[current_data].OID, atime, delay ;
                    atime = event_list[current_data].arrival ;
                    delay = 0 ;
                    alist.add_abort(ID, atime, delay);
                    current_data++;
                }
            }
        }

        while(jbq.length() > 0){ // �ѨM�̫�queue�̪����
            jobType data = jbq.dequeue();
            if(cpu_time > data.timeout){
                int ID = data.OID, atime, delay ;
                atime = cpu_time ;
                delay = atime - data.arrival ;
                alist.add_abort(ID, atime, delay);
            }
            else{
                cpu(data);
            }
        }

        if(alist.export_file()){ // �ɥX�ɮ�
            cout << "See " << alist.fileID << endl;
        }
    }
};

class Simulation_double { // ����CPU
    vector<jobType> event_list ; // �s����ȲM��
    AnsList alist ; // �s������P�����M��
    JobQueue jbq1 ; //��C
    JobQueue jbq2 ;
    int current_data = 0 ;

public:

    void S_copy_event( vector<jobType> &temp ){ // �N���ȲM��[�Jtemp
        for( int i = 0; i < event_list.size(); i++ ){
            temp.push_back(event_list[i]) ;
        }
    }

    void S_copy_temp( vector<jobType> temp, string fileName ){ // �Ntemp��ƥ[�J���ȲM��
        for( int i = 0; i < temp.size(); i++ ){
            event_list.push_back(temp[i]) ;
        }
        alist.fileID = fileName ;
    }

    bool import_file(const string& fileName) { //Ū��
        alist.fileID = fileName ;
        ifstream in;
        string fileID = "sorted" + fileName + ".txt";
        in.open(fileID);

        if (in.fail()) {
            cout << "Error: input file opening failed or does not exist." << endl;
            return false;
        }

        for (int i = 0; i < 4; i++) {
            string field;
            in >> field;
        }

        while (!in.eof()) {
            jobType newData;
            in >> newData.OID >> newData.arrival >> newData.duration >> newData.timeout;
            if (!in.fail())
                event_list.push_back(newData);
        }

        in.close();
        return true;
    }

    void cpu( int CID, int cpu_time, int &runtime){
        jobType data;
        bool bl = false;
        if ( CID == 1 )
            data = jbq1.getfirst();
        else
            data = jbq2.getfirst();

        if ( runtime == data.duration && cpu_time <= data.timeout ){
            int ID = data.OID, atime, delay ;
            atime = cpu_time;
            delay = cpu_time - data.arrival - data.duration;
            alist.add_done_double(ID, CID, atime, delay);
            runtime = 0;
            bl = true;
        }
        else if(cpu_time > data.timeout){
            int ID = data.OID, atime, delay ;
            atime = cpu_time ;
            delay = cpu_time - data.arrival;
            alist.add_abort_double(ID, CID, atime, delay);
            runtime = 0;
            bl = true;
        }
        else if(cpu_time == data.timeout && data.duration != runtime){
            int ID = data.OID, atime, delay ;
            atime = cpu_time ;
            delay = cpu_time - data.arrival;
            alist.add_abort_double(ID, CID, atime, delay);
            runtime = 0;
            bl = true;
        }

        if ( bl == true && CID == 1 ){
            data = jbq1.dequeue();
            if( jbq1.length() != 0 )
                cpu(1, cpu_time, runtime);
            }
        else if ( bl == true && CID == 2 ){
            data = jbq2.dequeue();
            if( jbq2.length() != 0 )
                cpu(2, cpu_time, runtime);
        }
    }
    void implement(){
        cout << endl << "The simulation is running..." << endl;

        int time = 0;
        int timeout = event_list[current_data].timeout;
        int runtime1, runtime2;

        while(current_data < event_list.size() || time <= timeout){

            if( jbq1.length() != 0 )
                cpu(1, time, runtime1);
            if( jbq2.length() != 0 )
                cpu(2, time, runtime2);

            while( current_data < event_list.size() && time == event_list[current_data].arrival ){ // Ū�J���
                if ( jbq1.length() == 0 || jbq2.length() == 0 ){
                    if ( event_list[current_data].timeout > timeout )
                            timeout = event_list[current_data].timeout;

                    if ( jbq1.length() == 0 ){
                        runtime1 = 0;
                        jbq1.enqueue(event_list[current_data]);
                    }
                    else{ // ( jbq2.length() == 0 )
                        runtime2 = 0;
                        jbq2.enqueue(event_list[current_data]);
                    }
                }
                else{ // ( jbq1.length() != 0 && jbq2.length() != 0 )
                    if ( jbq1.length() < 4 || jbq2.length() < 4 ){
                        if ( event_list[current_data].timeout > timeout )
                                timeout = event_list[current_data].timeout;

                        if ( jbq1.length() <= jbq2.length() )
                            jbq1.enqueue(event_list[current_data]);
                        else
                            jbq2.enqueue(event_list[current_data]);
                    }
                    else{ // ( jbq1.length() >= 4 && jbq2.length() >= 4 )
                        int ID = event_list[current_data].OID, atime, delay ;
                        atime = event_list[current_data].arrival ;
                        delay = 0 ;
                        alist.add_abort_double(ID, 0, atime, delay);
                    }
                }
                current_data++;
            }
            time++;
            runtime1++;
            runtime2++;
        }
        if(alist.export_file_double()){ // �ɥX�ɮ�
            cout << "See " << alist.fileID << endl;
        }
    }
};

class JobList { // ���Ȥ@
    vector<jobType> alist;
    string fileID;

    clock_t import_file_time;
    clock_t shell_sort_time;
    clock_t export_file_time;

public:
    void J_copy(vector<jobType> &temp){
        for( int i = 0; i < alist.size(); i++ ){
            temp.push_back( alist[i] );
        }
    }

    void shell_sort() { // �ƺ��ƧǪk
        shell_sort_time = clock();

        int n = alist.size();
        for (int span = n / 2; span > 0; span /= 2) {
            for (int i = span; i < n; i++) {
                jobType temp = alist[i];
                int j;
                for (j = i; j >= span && alist[j - span].arrival >= temp.arrival; j -= span) {
                    if (alist[j - span].arrival > temp.arrival) {
                        alist[j] = alist[j - span];
                    }
                    else if (alist[j - span].arrival == temp.arrival) {
                        if (alist[j - span].OID > temp.OID)
                            alist[j] = alist[j - span];
                        else
                            break;
                    }
                }
                alist[j] = temp;
            }
        }

        shell_sort_time = clock() - shell_sort_time;
    }

    bool import_file(const string& fileName) { //Ū��
        import_file_time = clock();

        ifstream in;
        fileID = "input" + fileName + ".txt";
        in.open(fileID);

        if (in.fail()) {
            cout << "Error: input file opening failed or does not exist." << endl;
            return false;
        }

        for (int i = 0; i < 4; i++) {
            string field;
            in >> field;
        }

        while (!in.eof()) {
            jobType newData;
            in >> newData.OID >> newData.arrival >> newData.duration >> newData.timeout;
            if (!in.fail())
                alist.push_back(newData);
        }

        in.close();

        import_file_time = clock() - import_file_time;
        return true;
    }

    bool export_file(const string& fileName) { // �g��
        export_file_time = clock();

        ofstream out;
        fileID = "sorted" + fileName + ".txt";
        out.open(fileID);

        if (out.fail()) {
            cout << "Error: output file opening failed." << endl;
            return false;
        }

        int n = alist.size();
        out << "OID\tArrival\tDuration\tTimeOut\n";
        for (int i = 0; i < n; i++) {
            out << alist[i].OID << "\t" << alist[i].arrival << "\t" << alist[i].duration << "\t" << alist[i].timeout << endl;
        }
        out.close();

        export_file_time = clock() - export_file_time;
        return true;
    }

    void show(string name) const {
        if ( name == "eventList" ){ // ��X�쥻�����
            int n = alist.size();
            cout << endl << "\tOID\tArrival\tDuration\tTimeOut\n";
            for (int i = 0; i < n; i++) {
                cout << "(" << i+1 << ")\t" << alist[i].OID << "\t" << alist[i].arrival << "\t" << alist[i].duration << "\t" << alist[i].timeout << endl;
            }
        }
        else if ( name == "time" ){ // ��Xfunction���B�@�ɶ�
            cout << endl << "Reading data: " << fixed << setprecision(0) << import_file_time << " clocks ("
                    << fixed << setprecision(2) << (double)import_file_time << " ms)." << endl;
            cout << "Sorting data: " << fixed << setprecision(0) << shell_sort_time << " clocks ("
                    << fixed << setprecision(2) << (double)shell_sort_time  << " ms)." << endl;
            cout << "Writing data: " << fixed << setprecision(0) << export_file_time << " clocks ("
                    << fixed << setprecision(2) << (double)export_file_time << " ms)." << endl;
        }
    }
};

int main() {
    int command = 10 ;
    string fileName;
    vector<jobType> temp ; // �Ȧs

    do{
        JobList ansList;
        Simulation question2;
        Simulation_double question3;
        cout << endl << "**** Simulate FIFO Queues by SQF *****";
        cout << endl << "* 0. Quit                            *";
        cout << endl << "* 1. Sort a file                     *";
        cout << endl << "* 2. Simulate one FIFO queue         *";
        cout << endl << "* 3. Simulate two queues by SQF      *";
        cout << endl << "**************************************";
        cout << endl << "Input a command(0, 1, 2, 3): ";
        cin >> command;
        cin.ignore(MAX_LEN, '\n');
        switch(command){ // �̷ӫ��O���
            case 0: break; // ���O0�A����
            case 1:
                cout << endl << "Input a file number: ";
                cin >> fileName;
                if (ansList.import_file(fileName)) {
                    ansList.show("eventList");
                    ansList.shell_sort();
                    ansList.export_file(fileName);
                    ansList.show("time");
                    cout << endl << "See sorted"<< fileName << ".txt" << endl;
                    temp.clear();
                    ansList.J_copy(temp) ;
                }
                break;
            case 2:
                if ( fileName.length() == 0 ){ // �p�G�@�}�l��J2�A�ӨS����J1
                    cout << endl << "Input a file number: ";
                    cin >> fileName;

                    if (question2.import_file(fileName)) {
                        question2.implement();
                        temp.clear();
                        question2.S_copy_event(temp) ;
                    }
                }
                else{ // ��J��1��A��J2
                    question2.S_copy_temp(temp, fileName);
                    question2.implement();
                }
                break;
            case 3:
                if ( fileName.length() == 0 ){
                    cout << endl << "Input a file number: ";
                    cin >> fileName;


                    if (question3.import_file(fileName)) {
                        question3.implement();
                        temp.clear();
                        question3.S_copy_event(temp) ;
                    }
                }
                else{
                    question3.S_copy_temp(temp, fileName);
                    question3.implement();
                }
                break;

            default: cout << endl << "Command does not exist!" << endl;
        }
    }while(command != 0);
    return 0;
}
