// DS1ex3_30_11027172_11027115
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <windows.h>
#include <iomanip>
using std::cout; using std::cin; using std::ios_base;
#define endl '\n';
int Input_filter();
std::string Number_check(std::string word);
void shell_sort(std::vector< std::tuple<std::string, std::string, std::string, std::string> > &data);
void Sort_and_save();
void FIFO_Queue();
bool runed1 = false;
std::vector<std::tuple<std::string, std::string, std::string, std::string>> Order1;
std::string filename1;

template <typename T>
class Queue{
    private:
        struct node{
            T data;
            node* next = NULL;
        };
        int size = 0;
        node* dummy = new node;
        node* end = dummy;
    public:
        Queue(){
        }
        int Size(){return size;}
        bool push(T data){
            if(size<3){
                end -> next = new node;
                end = end -> next;
                end -> data = data;
                size++;
                return true;
            }
            else
                return false;
        }
        void pop(){
            node* bye = dummy->next;
            dummy->next = bye -> next;
            delete bye;
            if(size == 1)
                end = dummy;
            size--;
        }
        T front(){
            if (size > 0)
                return dummy -> next -> data;
        }
        T back(){
            if (size > 0)
                return end->data;
        }
        bool empty(){
            if(size == 0)
                return true;
            else
                return false;
        }
};

void Sort_and_save(){
    double start_timer;
    double end_timer;
    std::vector<double> time_record;
    std::vector< std::tuple<std::string, std::string, std::string, std::string> > Order;
    std::string filename;
    std::string word;
    std::ifstream Read;
    std::ofstream Write;
    cout << "Input a file number: ";
    getline(cin, filename);
    filename = "input" + filename + ".txt";
    Read.open(filename);
    if (!Read.is_open()){
        cout << '\n' << "### " << filename << " does not exist! ###" << '\n' <<endl;
        return;
    }
    filename.erase(0, 5);
    filename = "sorted" + filename; 
    Write.open(filename);

    //* ------------ READING ------------
    start_timer = clock();
    getline(Read, word);
    cout << "\t" <<word << endl;
    Write << word << endl;
    while(Read >> word){
        std::string OID = Number_check(word);
        Read >> word;
        std::string Arrival = Number_check(word);
        Read >> word;
        std::string Duration = Number_check(word);
        Read >> word;
        std::string TimeOut = Number_check(word);
        Order.push_back(std::make_tuple(OID,Arrival,Duration,TimeOut));
        cout << "(" << Order.size() << ")\t" << OID << "\t" << Arrival << "\t" << Duration << "\t" << TimeOut << endl;
    }
    end_timer = clock();
    time_record.push_back(end_timer - start_timer);

    //* ------------ SORTING ------------
    start_timer = clock();
    shell_sort(Order);
    end_timer = clock();
    time_record.push_back(end_timer - start_timer);

    //* ------------ WRITEING ------------
    start_timer = clock();
    for(std::tuple<std::string, std::string, std::string, std::string> info : Order)
        Write << std::get<0>(info) << "\t" << std::get<1>(info) << "\t" << std::get<2>(info) << "\t" << std::get<3>(info) << endl;
    end_timer = clock();
    time_record.push_back(end_timer - start_timer);

    Read.close();
    Write.close();

    //* ------------ OUTPUT ------------

    cout << endl;
    cout << "Reading data: " << time_record[0] << " clocks " << "( " << time_record[0] << " ms)." << endl;
    cout << "Sorting data: " << time_record[1] << " clocks " << "( " << time_record[1] << " ms)." << endl;
    cout << "Writing data: " << time_record[2] << " clocks " << "( " << time_record[2] << " ms)." << endl;
    cout << endl;
    cout << "see " << filename << endl;
    cout << endl;

    //* ------------ CHECKING ------------
    Order1 = Order;
    filename.erase(0, 6);
    filename1 = filename;
    runed1 = true;
}

void FIFO_Queue(){
    std::vector<std::tuple<std::string, std::string, std::string, std::string>> Order;
    std::string filename;
    std::string word;
    std::ifstream Read;
    std::ofstream Write;
    if (runed1 == false){
        cout << "Input a file number: ";
        getline(cin, filename);
        filename = "sort" + filename + ".txt";
        Read.open(filename);
        if (!Read.is_open()){
            cout << '\n' << "### " << filename << " does not exist! ###" << '\n' <<endl;
            return;
        }
        getline(Read, word);
        //* ------------ READING ------------
        while(Read >> word){
            std::string OID = Number_check(word);
            Read >> word;
            std::string Arrival = Number_check(word);
            Read >> word;
            std::string Duration = Number_check(word);
            Read >> word;
            std::string TimeOut = Number_check(word);
            Order.push_back(std::make_tuple(OID,Arrival,Duration,TimeOut));
            cout << "(" << Order.size() << ")\t" << OID << "\t" << Arrival << "\t" << Duration << "\t" << TimeOut << endl;
        }
        Read.close();
        filename.erase(0, 4);
        filename = "output" + filename;
    }
    else {
        Order = Order1;
        filename = filename1;
        filename = "output" + filename ;
        cout << "The simulation is running..." << endl;
    }

    int order_count = 0;
    bool work_empty = true;
          Queue<std::tuple<std::string, std::string, std::string, std::string>> waiting;
          //               0.OID        1.Arrival    2.Duration   3.TimeOut
                std::tuple<std::string, std::string, std::string, std::string, int >  working;
                //         0.OID        1.Arrival     2.Duration  3.TimeOut    4.StartWorking    
    std::vector<std::tuple<std::string, std::string, std::string>> abort_list;
    //                     0.OID        1.Abort      2.Delay
    std::vector<std::tuple<std::string, std::string, std::string>> JobDone_list;
    //                     0.OID        1.Departure      2.Delay
    //* ------------ SIMULATION ------------
    for (int now = 0; order_count < Order.size() || !waiting.empty() || !work_empty ; now++) {

        //!  check process which is working is finished 
        if( !work_empty && std::get<4>(working) + std::stoi(std::get<2>(working)) == now ){
            /*
            if (now > std::stoi(std::get<3>(working))){  // Over time
                JobDone_list.push_back(std::make_tuple(std::get<0>(working), \
                std::to_string(now - std::stoi(std::get<1>(working)) - std::stoi(std::get<2>(working))), std::to_string(now)));
            }*/
            JobDone_list.push_back(std::make_tuple(std::get<0>(working), \
                std::to_string(now), std::to_string(now - std::stoi(std::get<1>(working)) - std::stoi(std::get<2>(working)))));
            work_empty = true;
        }

        if (!work_empty && now == std::stoi(std::get<3>(working))){  // Over time
            abort_list.push_back(std::make_tuple(std::get<0>(working), \
            std::to_string(now), std::to_string(now - std::stoi(std::get<1>(working)))));
            work_empty = true;
        }

        //! add process into work space 
        if (work_empty) {  // cpu is not working now
            while(!waiting.empty() && work_empty){   //* queue is not empty
                if(std::stoi(std::get<3>(waiting.front())) > now){  //? on time
                    working = std::make_tuple(std::get<0>(waiting.front()), std::get<1>(waiting.front()),\
                                    std::get<2>(waiting.front()), std::get<3>(waiting.front()), now);
                    waiting.pop();
                    work_empty = false;
                    if (now >= std::stoi(std::get<3>(working))){  // Over time
                        abort_list.push_back(std::make_tuple(std::get<0>(working), \
                        std::to_string(now), std::to_string(now - std::stoi(std::get<1>(working)))));
                    }
                }
                else{   
                    abort_list.push_back(std::make_tuple(std::get<0>(waiting.front()),\
                                    std::to_string(now), std::to_string(now - std::stoi(std::get<1>(waiting.front())))));
                    waiting.pop();
                }
            }
            if(work_empty && order_count < Order.size() && now == std::stoi(std::get<1>(Order[order_count]))){
                //* take the task from Order directly
                working = std::make_tuple(std::get<0>(Order[order_count]),std::get<1>(Order[order_count])\
                                    ,std::get<2>(Order[order_count]),std::get<3>(Order[order_count]), now);
                order_count++;
                work_empty = false;
            }
        }

        //! add process into queue
        while (order_count < Order.size() && now == std::stoi(std::get<1>(Order[order_count]))) {
            // cpu is working now, there have other order and order is arriving
            if (!waiting.push(Order[order_count])){
                if (waiting.Size() == 3){ // queue is full
                abort_list.push_back(std::make_tuple(std::get<0>(Order[order_count]), std::get<1>(Order[order_count]), "0"));
                }
            }
            order_count++;
        }
    }
    //* ------------ WRITEING ------------
    Write.open(filename);
    Write << "\t[Abort Jobs]\n\tOID\tAbort\tDelay\n";
    int delay_count = 0;
    for(int i = 0 ; i < abort_list.size() ; i++){
        Write <<"[" << i+1 << "]\t" \
        << std::get<0>(abort_list[i]) << "\t" << std::get<1>(abort_list[i]) << "\t" << std::get<2>(abort_list[i]) << endl;
        delay_count += std::stoi(std::get<2>(abort_list[i]));
    }
    Write <<"\t[Jobs Done]\n\tOID\tDeparture\tDelay\n";
    for (int i = 0; i < JobDone_list.size(); i++) {
        Write << "[" << i + 1 << "]\t" \
        << std::get<0>(JobDone_list[i]) << "\t" << std::get<1>(JobDone_list[i]) << "\t" << std::get<2>(JobDone_list[i]) << endl;
        delay_count += std::stoi(std::get<2>(JobDone_list[i]));
    }
    // aberage delay time output as e.g. 0.00 ms
    Write << "[Average Delay]\t" << std::fixed << std::setprecision(2) << round((float)(delay_count) / (float)Order.size()* 100)/100 << " ms" << endl;
    Write << "[Success Rate]\t" << std::fixed << std::setprecision(2) << round((float)(JobDone_list.size()) / (float)Order.size() * 10000)/100  <<" %";
    Write.close();
    cout << "See " << filename << endl;
    cout << endl;
}

void shell_sort(std::vector<std::tuple<std::string, std::string, std::string, std::string>> &Order){
    for( int gap = Order.size()/2; gap > 0 ; gap >>=1){
        for (int i = gap; i < Order.size(); i++){   //! select which item sould be insert
            for(int j = i ;j >= gap && std::stoi(std::get<1>(Order[j])) <= std::stoi(std::get<1>(Order[j - gap])); j-=gap ){     
                //! select which position item sould insert
                if ((std::stoi(std::get<1>(Order[j])) == std::stoi(std::get<1>(Order[j - gap]))) &&\
                    (std::stoi(std::get<0>(Order[j])) > std::stoi(std::get<0>(Order[j - gap]))))
                        break;
                else
                    Order[j].swap(Order[j - gap]);
            }
        }
    }
}

std::string Number_check(std::string str){     // * TO check the string is number
    std::string correct;
    for(char ch:str){
        if(ch<='9' && ch>='0')
            correct+=ch;
    }
    return correct;
}

int Input_filter(){     // *To avoid the character which is not number and the number out of range of "int"
    std::string input = "";
    //cin.clear();
    getline(cin, input);
    for (int i = 0; i < input.size(); i++){     // If input has the character which is not number, it will return -1
        if ( input[i] < '0' || input[i] > '9'){
            return -1;            
        }
    }
    if (input.size() > 4)  // If input number out of range of 9999, it will return -2;
        return -2;
    else
        return std::stoi(input);
}

int main(void){
    int mode = -1;
    do{
        cout << "**** Simulate FIFO Queues by SQF *****" << endl;
        cout << "* 0. Quit                            *" << endl;
        cout << "* 1. Sort a file                     *" << endl;
        cout << "* 2. Simulate one FIFO queue         *" << endl;
        cout << "**************************************" << endl;
        cout << "Input a command(0, 1, 2): ";
        mode = Input_filter();
        cout << endl;
        if ( mode == 1 )
            Sort_and_save();   
        else if ( mode == 2 )
            FIFO_Queue();
        else if ( mode != 0 ){
            cout << "Command does not exist !"<< "\n" <<endl;
        }
    }while ( mode != 0 );
    return 0;
}