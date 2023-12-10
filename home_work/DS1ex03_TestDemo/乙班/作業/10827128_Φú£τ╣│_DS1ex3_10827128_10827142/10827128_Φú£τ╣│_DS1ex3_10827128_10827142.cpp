//*********************************************************************//
//10827128 10827142
//********************************************************************************/
// #include "DS1Simulation.hpp" // class JobQueue
#include <stdio.h>
#include <string.h>  // string class
#include <time.h>
#include <windows.h>

#include <cstdlib>  // system, atoi
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>  // string class
#include <vector>
using namespace std;

static int sumoflist = 0;
static int processtime = 0;
double ReadingSTART, ReadingEND;
double SortingSTART, SortingEND;
double WritingSTART, WritingEND;

string InttoString(int num) {
    stringstream ss;
    ss << num;
    string out_string = ss.str();
    return out_string;

}  // ItoS()

fstream file;
string firstline;  // 嚙衝一嚙踝蕭]嚙瞇嚙瞌OID Arrival 嚙踝蕭嚙踝蕭
int num;           //嚙緯嚙賭的嚙褕殷蕭
bool Donotfile = false;

// 嚙踝蕭嚙?01 file
void FindFile(int num) {  // 嚙踝蕭嚙?01 file
    ReadingSTART = clock();

    string add = InttoString(num);
    string ss;
    ss = "input" + add;
    ss = ss + ".txt";
    // cout << "ss: " << ss << endl ;
    file.open(ss.c_str(), ios::in);

    if (!file)
        cout << "### " << ss << " does not exist! ###";
    else
        Donotfile = true;

    ReadingEND = clock();
}  // FindFile()

void FindFile2(int num) {  // 嚙踝蕭嚙衝二嚙瘩嚙豎要嚙踝蕭401 file
    ReadingSTART = clock();

    string add = InttoString(num);
    string ss;
    ss = "sorted" + add;
    ss = ss + ".txt";
    // cout << "ss: " << ss << endl ;
    file.open(ss.c_str(), ios::in);

    if (!file)
        cout << "### " << ss << " does not exist! ###";
    else
        Donotfile = true;

    ReadingEND = clock();
}  // FindFile()

struct Cook {  // 嚙稽嚙緩嚙瘠嚙踝蕭vector 嚙緯嚙編嚙踝蕭 CookList
    int OID = 0;
    int Arrival = 0;
    int Duration = 0;
    int TimeOut = 0;
};  // CookList

vector<Cook> CookList;

struct Abort {  // 嚙稽嚙緩嚙瘠嚙踝蕭vector 嚙緯嚙編嚙踝蕭 CookList
    int OID = 0;
    int Delay = 0;
    int Abort = 0;
};  // AbortList

vector<Abort> Abortlist;
struct TimeOut {  // 嚙稽嚙緩嚙瘠嚙踝蕭vector 嚙緯嚙編嚙踝蕭 CookList
    int OID = 0;
    int Delay = 0;
    int Departure = 0;
};  // OutputList

vector<TimeOut> Donelist;
struct Abort3 {  // 嚙稽嚙緩嚙瘠嚙踝蕭vector 嚙緯嚙編嚙踝蕭 CookList
    int OID = 0;
    int CID = 0;
    int Delay = 0;
    int Abort = 0;
};  // AbortList

vector<Abort3> Abortlist3;
struct TimeOut3 {  // 嚙稽嚙緩嚙瘠嚙踝蕭vector 嚙緯嚙編嚙踝蕭 CookList
    int OID = 0;
    int CID = 0;
    int Delay = 0;
    int Departure = 0;
};  // OutputList

vector<TimeOut3> Donelist3;
//********************************嚙罷嚙締嚙誕伐蕭VECTOR***************************************************/

int GetOID() {  // 嚙緻嚙踝蕭OID 嚙踝蕭嚙複字

    char input;
    char in[100];
    int i = 0;
    string ss;
    int OID;

    input = file.get();

    while (input != '\t' && input != '\n' && input != EOF && input != ' ') {
        in[i] = input;
        input = file.get();
        i++;
    }  // while

    ss = in;

    OID = atoi(ss.c_str());

    return OID;
}  //  GetOID()

int GetArrival() {  // 嚙緻嚙踝蕭Arrival 嚙踝蕭嚙複字

    char input;
    char TOArrival[150];
    int i = 0;
    string ss;
    int Arrival;

    input = file.get();
    while (input != '\t' && input != '\n' && input != EOF && input != ' ') {
        TOArrival[i] = input;
        input = file.get();
        i++;
    }  // while

    ss = TOArrival;
    Arrival = atoi(ss.c_str());

    return Arrival;

}  //  GetArrival()

int GetDuration() {  // 嚙緻嚙踝蕭Duration 嚙踝蕭嚙複字

    char input;
    char inDuration[200];
    int i = 0;
    string ss;
    int Duration;

    input = file.get();
    while (input != '\t' && input != '\n' && input != EOF) {
        inDuration[i] = input;
        input = file.get();
        i++;
    }  // while

    ss = inDuration;
    Duration = atoi(ss.c_str());

    return Duration;

}  //  GetDuration()

int GetTimeOut() {  // 嚙緻嚙踝蕭TimeOut 嚙踝蕭嚙複字

    char input;
    char inTimeOut[250];
    int i = 0;
    string ss;
    int TimeOut;

    input = file.get();

    while (input != '\t' && input != '\n' && input != EOF) {
        inTimeOut[i] = input;
        input = file.get();
        i++;
    }  // while

    ss = inTimeOut;
    TimeOut = atoi(ss.c_str());

    return TimeOut;

}  //  GetTimeOut()

void SetCookList() {  // 嚙踝蕭@嚙踝蕭嚙瑾嚙踝蕭嚙踝蕭s嚙踝蕭vector

    WritingEND = clock();

    getline(file, firstline);  // 讀嚙踝蕭嚙衝一嚙踝蕭
    int i = 1;
    // cout << "input: " << input << endl ;
    // 嚙窯嚙瞇File嚙瞌嚙稻讀嚙踝蕭
    while (!file.eof()) {
        Cook List;
        List.OID = GetOID();
        List.Arrival = GetArrival();
        List.Duration = GetDuration();
        List.TimeOut = GetTimeOut();
        if (!file.eof())
            CookList.push_back(List);

    }  // while

    WritingEND = clock();
}  // SetCookList()

void saveFile() {
    SortingSTART = clock();

    string add = InttoString(num);
    string fileName;
    fileName = "sorted" + add;
    fileName = fileName + ".txt";

    int i;
    fstream newFile;
    newFile.open(fileName.c_str(), ios::out | ios::trunc);

    newFile << firstline << endl;
    // For each element in the vector

    int n = CookList.size();
    // if (num == 401 || num == 405) n--;

    for (i = 0; i < n; i++) {
        // Store each vehicle element in the file
        newFile << CookList[i].OID << "\t";
        newFile << CookList[i].Arrival << "\t";
        newFile << CookList[i].Duration << "\t";
        newFile << CookList[i].TimeOut << "\n";

    }  // for

    newFile.close();

    SortingEND = clock();

}  // saveFile()

void swap(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}  // swap()

int ShellSort(int n) {
    // Start with a big gap, then reduce the gap
    for (int gap = n / 2; gap > 0; gap /= 2) {
        // Do a gapped insertion sort for this gap size.
        // The first gap elements a[0..gap-1] are already in gapped order
        // keep adding one more element until the entire array is
        // gap sorted
        for (int i = gap; i < n; i += 1) {
            // add a[i] to the elements that have been gap sorted
            // save a[i] in temp and make a hole at position i
            int tempA = CookList[i].Arrival;
            int tempO = CookList[i].OID;
            int tempD = CookList[i].Duration;
            int tempT = CookList[i].TimeOut;

            // shift earlier gap-sorted elements up until the correct
            // location for a[i] is found
            int j;

            for (j = i; j >= gap && CookList[j - gap].Arrival > tempA; j -= gap) {
                CookList[j].Arrival = CookList[j - gap].Arrival;
                CookList[j].OID = CookList[j - gap].OID;
                CookList[j].Duration = CookList[j - gap].Duration;
                CookList[j].TimeOut = CookList[j - gap].TimeOut;
            }  // for

            //  put temp (the original a[i]) in its correct location
            CookList[j].Arrival = tempA;
            CookList[j].OID = tempO;
            CookList[j].Duration = tempD;
            CookList[j].TimeOut = tempT;
        }  // for()

    }  // for()
    return 0;
}  // ShellSort()

int ShellSort2() {
    int i, j, flag;
    int length = CookList.size();  // length
    if (num == 401 || num == 405) length--;

    for (i = 0; i < length; i++) {  //n嚙諉數字嚙複序，嚙線嚙踝蕭 n-1 嚙稷

        flag = 0;  //嚙踝蕭嚙豌佗蕭嚙盤嚙緻嚙談交換

        for (j = 0; j < length - i; j++) {  //嚙緬嚙衝一嚙諉數字嚙罷嚙締嚙踝蕭嚙踝蕭A嚙踝蕭嚙踝蕭怮嚙瑾嚙諉數字
            if (CookList[j].Arrival == CookList[j + 1].Arrival) {
                if (CookList[j].OID > CookList[j + 1].OID) {
                    int tempT = CookList[j].TimeOut;
                    swap(&CookList[j].TimeOut, &CookList[j + 1].TimeOut);
                    swap(&CookList[j].OID, &CookList[j + 1].OID);
                    swap(&CookList[j].Arrival, &CookList[j + 1].Arrival);
                    swap(&CookList[j].Duration, &CookList[j + 1].Duration);

                    flag = 1;

                }  // if

            }  // if

        }  // for

        if (flag == 0) break;  //嚙踝蕭嚙稷嚙碼嚙磅嚙踝蕭嚙緻嚙談交換

    }  // for

    return 0;
}

void PrintRead_vector() {
    int length = CookList.size();  // length
    // if (num == 401 || num == 405) length--;
    cout << "\t" << firstline << endl;

    for (int i = 0; i < length; i++) {
        cout << "(" << i + 1 << ")"
             << "\t";
        cout << CookList[i].OID << "\t";       // 嚙踝蕭X
        cout << CookList[i].Arrival << "\t";   // 嚙踝蕭X
        cout << CookList[i].Duration << "\t";  // 嚙踝蕭X
        cout << CookList[i].TimeOut << "\n";
    }  // for

}  // PrintRead_vector()

void DoShellSort() {
    int n = CookList.size();
    // if (num == 401 || num == 405) n--;
    ShellSort(n);
    ShellSort2();
}  // DoShellSort*/

void CheckList() {
    // CookList.erase(CookList.begin() + CookList.size() -1);
    int i = 0;
    int length = CookList.size();

    for (int i = 0; i < length; i++) {
        if (CookList[i].Arrival + CookList[i].Duration > CookList[i].TimeOut) {  //嚙緘嚙瘦嚙緬嚙賣不嚙碼嚙緲
            CookList.erase(CookList.begin() + i);                                // 嚙磋嚙踝蕭嚙踝蕭嚙踝蕭嚙緬嚙踝蕭
            i--;

        }  // if()
    }      //for()

}  // CheckList()

class Queue {
   public:
    vector<Cook> process;

    bool isEmpty() {
        return process.empty();

    }  // isEmpty

    void enQueue(Cook cooklist) {
        process.push_back(cooklist);
    }

    void deQueue() {
        if (!process.empty()) {
            process.erase(process.begin());
        }  // 嚙緘嚙瘦嚙踝蕭嚙瞌嚙褐迎蕭
    }

    Cook getFront() {
        Cook temp;
        if (process.empty() == true)
            return temp;  // 嚙緘嚙瘦嚙瞌嚙褐迎蕭

        else
            temp = process[0];

        return temp;
    }

    int size() {
        return process.size();
    }
};

Queue processList;
Queue processList2;

class Chef {
   public:
    int busytime = 0;  //嚙豎廚嚙緞嚙瞌嚙踝蕭嚙瞌嚙褐塚蕭嚙踝蕭

    void Chiefcook() {
        Cook temp;
    Head:
        temp = processList.getFront();
        cout << endl << temp.OID << " " << temp.TimeOut << " " << processtime ;
        processList.deQueue();
        if (temp.OID != 0) {
            if (processtime >= temp.TimeOut) { 
                Abort aborttemp;
                aborttemp.OID = temp.OID;  
                aborttemp.Delay = processtime - temp.Arrival;
                aborttemp.Abort = processtime;
                Abortlist.push_back(aborttemp);
                busytime = 0;
                goto Head;
            } else if (processtime + temp.Duration > temp.TimeOut) {  
                Abort aborttemp;
                aborttemp.OID = temp.OID;  
                aborttemp.Delay = temp.TimeOut - temp.Arrival;
                aborttemp.Abort = temp.TimeOut  ;
                Abortlist.push_back(aborttemp);
                busytime = 0;
                //goto Head;
            }  // if ( time + temp.Duration> temp.TimeOut)

            else{
                busytime = temp.Duration ;
                TimeOut donetemp;
                donetemp.OID = temp.OID; 
                donetemp.Delay = processtime - temp.Arrival;
                donetemp.Departure = processtime + temp.Duration;
                Donelist.push_back(donetemp);
            }
        }
    }

    void Chiefcook31() {
        Cook temp;
    Head:
        temp = processList.getFront();
        processList.deQueue();
        if (temp.OID != 0) {
            if (processtime >= temp.TimeOut) {  // 嚙踝蕭嚙踝蕭嚙箴嚙瞇嚙緻嚙緹嚙磕嚙踝蕭
                Abort3 aborttemp;
                aborttemp.OID = temp.OID;  //嚙踝蕭嚙複存嚙箠aborttemp嚙諒哨蕭
                aborttemp.CID = 1;
                aborttemp.Delay = processtime - temp.Arrival;
                aborttemp.Abort = processtime;
                Abortlist3.push_back(aborttemp);
                busytime = 0;
                goto Head;
            } else if (processtime + temp.Duration > temp.TimeOut) {  // 嚙踝蕭嚙踝蕭嚙緻嚙緹嚙磕嚙碼嚙褕塚蕭
                TimeOut3 timeouttemp;
                timeouttemp.OID = temp.OID;  //嚙踝蕭嚙複存嚙箠timeouttemp嚙諒哨蕭
                timeouttemp.CID = 1;
                timeouttemp.Delay = processtime - temp.Arrival;
                timeouttemp.Departure = processtime + temp.Duration;
                Donelist3.push_back(timeouttemp);

                busytime = temp.Duration;
            }  // if ( time + temp.Duration> temp.TimeOut)

            else{
                busytime = temp.Duration;
                TimeOut donetemp;
                donetemp.OID = temp.OID; 
                donetemp.Delay = processtime - temp.Arrival;
                donetemp.Departure = processtime + temp.Duration;
                Donelist.push_back(donetemp);
            }
        }
    }

    void Chiefcook32() {
        Cook temp;
    Head:
        temp = processList2.getFront();
        processList2.deQueue();
        if (temp.OID != 0) {
            if (processtime >= temp.TimeOut) {  // 嚙踝蕭嚙踝蕭嚙箴嚙瞇嚙緻嚙緹嚙磕嚙踝蕭
                Abort3 aborttemp;
                aborttemp.OID = temp.OID;  //嚙踝蕭嚙複存嚙箠aborttemp嚙諒哨蕭
                aborttemp.CID = 2;
                aborttemp.Delay = processtime - temp.Arrival;
                aborttemp.Abort = processtime;
                Abortlist3.push_back(aborttemp);
                busytime = 0;
                goto Head;
            } else if (processtime + temp.Duration > temp.TimeOut) {  // 嚙踝蕭嚙踝蕭嚙緻嚙緹嚙磕嚙碼嚙褕塚蕭
                TimeOut3 timeouttemp;
                timeouttemp.OID = temp.OID;  //嚙踝蕭嚙複存嚙箠timeouttemp嚙諒哨蕭
                timeouttemp.CID = 2;
                timeouttemp.Delay = processtime - temp.Arrival;
                timeouttemp.Departure = processtime + temp.Duration;
                Donelist3.push_back(timeouttemp);
                busytime = temp.Duration;
            }  // if ( time + temp.Duration> temp.TimeOut)

            else
                busytime = temp.Duration;
        }
    }
};  //

Chef Chief;
Chef Chief2;

// =========嚙衝二嚙瘩=======

void SameArrival() {
    while (CookList[0].Arrival == processtime && CookList.size() != 0) {
        if (CookList[0].Arrival == processtime) {
            if (processList.size() < 3) {
                Cook temp;
                temp = CookList[0];
                processList.enQueue(temp);
                CookList.erase(CookList.begin());
            }  

            else {
                Abort temp;
                temp.OID = CookList[0].OID;
                temp.Delay = 0;
                temp.Abort = CookList[0].Arrival;
                Abortlist.push_back(temp);
                CookList.erase(CookList.begin());
            }  
        }     
    }

}  // arrival嚙褕塚蕭嚙瑾嚙誼同嚙褕處嚙緲

void SameArrival2() {
    while (CookList[0].Arrival == processtime && CookList.size() != 0) {
        if (CookList[0].Arrival == processtime) {
            if (processList.size() < 4) {
                Cook temp;
                temp = CookList[0];
                processList.enQueue(temp);
                CookList.erase(CookList.begin());
            }  // 嚙緘嚙瘦嚙豎處嚙緲嚙緬嚙踝蕭眯嚙確嚙踝蕭嚙璀嚙踝蕭U嚙瑾嚙踝蕭嚙緬嚙踝蕭s嚙箠嚙篁

            else {
                Abort temp;
                temp.OID = CookList[0].OID;
                temp.Delay = 0;
                temp.Abort = CookList[0].Arrival;
                Abortlist.push_back(temp);
                CookList.erase(CookList.begin());
            }  // 嚙緘嚙瘦嚙豎處嚙緲嚙緬嚙賣滿嚙瘤嚙瞇嚙踝蕭iabortlist
        }
    }

}  // arrival嚙褕塚蕭嚙瑾嚙誼同嚙褕處嚙緲 嚙璀嚙稼嚙磕嚙緘嚙緞嚙瞌嚙褐迎蕭嚙瘡嚙踝蕭queue嚙瞌嚙褐迎蕭
void Simulate() {
    Chef Chief;

    for (processtime = 0; Chief.busytime > 0 || !processList.isEmpty() || !CookList.empty(); processtime++, Chief.busytime--) {
        if (Chief.busytime <= 0) {
            Chief.Chiefcook();
        }  // 嚙緘嚙緞嚙畿嚙緲嚙緬嚙踝蕭

        if (Chief.busytime <= 0 && processList.isEmpty()) {
            SameArrival2();
        }  // 嚙緘嚙瘦嚙緘嚙緞嚙瞌嚙褐迎蕭嚙瘡嚙踝蕭queue嚙瞌嚙褐迎蕭
        else {
            SameArrival();
        }

        if (Chief.busytime <= 0) {
            Chief.Chiefcook();
        }  // 嚙緘嚙緞嚙畿嚙緲嚙緬嚙踝蕭

    }  // for

}  //Simulate()

//========嚙衝三嚙瘩======

void SameArrival31() {
    if (CookList[0].Arrival == processtime) {
        if (processList.size() < 3) {
            Cook temp;
            temp = CookList[0];
            processList.enQueue(temp);
            CookList.erase(CookList.begin());
        }  // 嚙緘嚙瘦嚙豎處嚙緲嚙緬嚙踝蕭眯嚙確嚙踝蕭嚙璀嚙踝蕭U嚙瑾嚙踝蕭嚙緬嚙踝蕭s嚙箠嚙篁

        else {
            Abort3 temp;
            temp.OID = CookList[0].OID;
            temp.CID = 0;
            temp.Delay = 0;
            temp.Abort = CookList[0].Arrival;
            Abortlist3.push_back(temp);
            CookList.erase(CookList.begin());
        }  // 嚙緘嚙瘦嚙豎處嚙緲嚙緬嚙賣滿嚙瘤嚙瞇嚙踝蕭iabortlist
    }
}
void SameArrival32() {
    if (CookList[0].Arrival == processtime) {
        if (processList2.size() < 3) {
            Cook temp;
            temp = CookList[0];
            processList2.enQueue(temp);
            CookList.erase(CookList.begin());
        }  // 嚙緘嚙瘦嚙豎處嚙緲嚙緬嚙踝蕭眯嚙確嚙踝蕭嚙璀嚙踝蕭U嚙瑾嚙踝蕭嚙緬嚙踝蕭s嚙箠嚙篁

        else {
            Abort3 temp;
            temp.OID = CookList[0].OID;
            temp.CID = 0;
            temp.Delay = 0;
            temp.Abort = CookList[0].Arrival;
            Abortlist3.push_back(temp);
            CookList.erase(CookList.begin());
        }  // 嚙緘嚙瘦嚙豎處嚙緲嚙緬嚙賣滿嚙瘤嚙瞇嚙踝蕭iabortlist
    }

}  // arrival嚙褕塚蕭嚙瑾嚙誼同嚙褕處嚙緲 嚙璀嚙稼嚙磕嚙緘嚙緞嚙瞌嚙褐迎蕭嚙瘡嚙踝蕭queue嚙瞌嚙褐迎蕭
void Simulate3() {
    for (processtime = 0, Chief.busytime = 0, Chief2.busytime = 0; Chief.busytime > 0 || Chief2.busytime > 0 || !processList.isEmpty() || !processList2.isEmpty() || !CookList.empty(); processtime++, Chief.busytime--, Chief2.busytime--) {
        if (Chief.busytime <= 0) {
            Chief.Chiefcook31();
        }  // 嚙緘嚙緞嚙畿嚙緲嚙緬嚙踝蕭
        if (Chief2.busytime <= 0) {
            Chief2.Chiefcook32();
        }  // 嚙緘嚙緞嚙畿嚙緲嚙緬嚙踝蕭
        while (processtime == CookList[0].Arrival && CookList.size() != 0) {
            if (Chief.busytime <= 0) {
                Chief.Chiefcook31();
            }  // 嚙緘嚙緞嚙畿嚙緲嚙緬嚙踝蕭
            if (Chief2.busytime <= 0) {
                Chief2.Chiefcook32();
            }  // 嚙緘嚙緞嚙畿嚙緲嚙緬嚙踝蕭

            if (Chief.busytime <= 0 && processList.isEmpty()) {
                SameArrival31();

            }  // 嚙緘嚙瘦嚙緘嚙緞嚙瞌嚙褐迎蕭嚙瘡嚙踝蕭queue嚙瞌嚙褐迎蕭
            else if (Chief2.busytime <= 0 && processList2.isEmpty()) {
                SameArrival32();
            }

            else if (processList.process.size() <= processList2.process.size()) {
                SameArrival31();
            }

            else if (processList2.process.size() < processList.process.size()) {
                SameArrival32();
            }

            if (Chief.busytime <= 0) {
                Chief.Chiefcook31();
            }  // 嚙緘嚙緞嚙畿嚙緲嚙緬嚙踝蕭

            if (Chief2.busytime <= 0) {
                Chief2.Chiefcook32();
            }  // 嚙緘嚙緞嚙畿嚙緲嚙緬嚙踝蕭

            // cout << "11111" << endl;
        }
    }  // for
}  //Simulate()
//=========嚙衝三嚙瘩========

int CalculateDelay() {
    int delaytime = 0;
    int i = 0;
    int n;
    n = Abortlist.size();
    for (i = 0; i < n; i++) {
        delaytime = delaytime + Abortlist[i].Delay;

    }  // 嚙緘嚙踝蕭abortlist 嚙踝蕭嚙踝蕭delaytime

    n = Donelist.size();
    for (i = 0; i < n; i++) {
        delaytime = delaytime + Donelist[i].Delay;

    }  // 嚙緘嚙踝蕭Donelist 嚙踝蕭嚙踝蕭delaytime

    return delaytime;
}

int CalculateDelay3() {
    int delaytime = 0;
    int i = 0;
    int n;
    n = Abortlist3.size();
    for (i = 0; i < n; i++) {
        delaytime = delaytime + Abortlist3[i].Delay;

    }  // 嚙緘嚙踝蕭abortlist 嚙踝蕭嚙踝蕭delaytime

    n = Donelist3.size();
    for (i = 0; i < n; i++) {
        delaytime = delaytime + Donelist3[i].Delay;

    }  // 嚙緘嚙踝蕭Donelist 嚙踝蕭嚙踝蕭delaytime

    return delaytime;
}

float Calculatepercent() {
    float sumofwronglist = 0;
    int i = 0;
    int n;
    float percent;
    n = Abortlist.size();
    sumofwronglist = sumofwronglist + n;
   
    percent = 100 * (float)sumofwronglist / (float)sumoflist;
    return percent;
}

float Calculatepercent3() {
    float sumofwronglist = 0;
    int i = 0;
    int n;
    float percent;
    n = Abortlist3.size();
    sumofwronglist = sumofwronglist + n;
    n = Donelist3.size();
    sumofwronglist = sumofwronglist + n;

    percent = 100 * (float)sumofwronglist / (float)sumoflist;
    return percent;
}

void saveFile2() {
    string add = InttoString(num);
    string fileName;
    fileName = "output" + add;
    fileName = fileName + ".txt";

    int i;
    fstream newFile;
    newFile.open(fileName.c_str(), ios::out);

    newFile << "\t[Abort List]" << endl;
    newFile << "\tOID\tAbort\tDelay" << endl;

    // For each element in the vector

    int n = Abortlist.size();
    int b = 0;
    for (i = 0; i < n; i++) {
        // Store each vehicle element in the file
        b = i + 1;
        newFile << "[" << b << "]"
                << "\t";
        newFile << Abortlist[i].OID << "\t";
        newFile << Abortlist[i].Abort << "\t";
        newFile << Abortlist[i].Delay << "\n";

    }  // for

    newFile << "\t[Job Done]" << endl;

    newFile << "\tOID\tDeparture\tDelay" << endl;

    // For each element in the vector

    n = Donelist.size();

    for (i = 0; i < n; i++) {
        // Store each vehicle element in the file
        b = i + 1;
        newFile << "[" << b << "]"
                << "\t";
        newFile << Donelist[i].OID << "\t";
        newFile << Donelist[i].Departure << "\t";
        newFile << Donelist[i].Delay << "\n";

    }  // for

    n = CookList.size();
    newFile << "[Average Delay]";
    float t = CalculateDelay()/sumoflist ;
    newFile << t << " ms.\n";
    newFile << "[Sucesses Rate]";
    float percent = 100-Calculatepercent();
    newFile << fixed << setprecision(2) << percent;
    newFile << " %\n";

    newFile.close();

}  // saveFile2()

void saveFile3() {
    string add = InttoString(num);
    string fileName;
    fileName = "cooks" + add;
    fileName = fileName + ".txt";

    int i;
    fstream newFile;
    newFile.open(fileName.c_str(), ios::out);

    newFile << "\t[Abort List]" << endl;
    newFile << "\tOID\tCID\tDelay\tAbort" << endl;

    // For each element in the vector

    int n = Abortlist3.size();
    int b = 0;
    for (i = 0; i < n; i++) {
        // Store each vehicle element in the file
        b = i + 1;
        newFile << "[" << b << "]"
                << "\t";
        newFile << Abortlist3[i].OID << "\t";
        newFile << Abortlist3[i].CID << "\t";
        newFile << Abortlist3[i].Delay << "\t";
        newFile << Abortlist3[i].Abort << "\n";

    }  // for

    newFile << "\t[Timeout List]" << endl;

    newFile << "\tOID\tCID\tDelay\tDeparture" << endl;

    // For each element in the vector

    n = Donelist3.size();

    for (i = 0; i < n; i++) {
        // Store each vehicle element in the file
        b = i + 1;
        newFile << "[" << b << "]"
                << "\t";
        newFile << Donelist3[i].OID << "\t";
        newFile << Donelist3[i].CID << "\t";
        newFile << Donelist3[i].Delay << "\t";
        newFile << Donelist3[i].Departure << "\n";

    }  // for

    newFile << "[Total Delay]" << endl;
    int t = CalculateDelay3();
    newFile << t << " min.\n";
    newFile << "[Failure Percentage]" << endl;
    float percent = Calculatepercent3();
    newFile << fixed << setprecision(2) << percent;
    newFile << " %\n";

    newFile.close();

}  // saveFile()

int getNO(int, int);  // get a number from user

int main(void) {
    int command = 0;  // user command

    do {  // JobList orders;  // a list of orders

        cout << endl
             << "**** Simulate FIFO Queues by SQF ****";
        cout << endl
             << "* 0. Quit                       *";
        cout << endl
             << "* 1. Sort a file                *";
        cout << endl
             << "* 2. Simulate one FIFO queue    *";
        cout << endl
             << "* 3. Simulate two queues by SQF *";
        cout << endl
             << "*******************************";
        cout << endl
             << "Input a command(0, 1, 2): ";
        cin >> command;  // get the command
        switch (command) {
            case 0:
                break;
            case 1:
                cout << "Input a file number (e.g., 301, 302, 303, ...): ";
                cin >> num;
                FindFile(num);  // 嚙踝蕭嚙踝蕭仵嚙?嚙衛開嚙締讀嚙踝蕭
                if (Donotfile == true) {
                    SetCookList();       // 嚙罷嚙締嚙誼進vector
                    PrintRead_vector();  // print 嚙踝蕭嚙複序迎蕭嚙罷嚙瘠
                    DoShellSort();       // 嚙罷嚙締嚙複改蕭
                    int no;
                    no = static_cast<int>((ReadingEND - ReadingSTART) / CLOCKS_PER_SEC);
                    cout << endl
                         << "Reading data: " << no;
                    cout << "clocks "
                         << "(" << (ReadingEND - ReadingSTART) / CLOCKS_PER_SEC << " ms). " << endl
                         << endl;

                    no = static_cast<int>((SortingEND - SortingSTART) / CLOCKS_PER_SEC);
                    cout << "Sorting data: " << no;
                    cout << "clocks "
                         << "(" << (SortingEND - SortingSTART) / CLOCKS_PER_SEC << " ms). " << endl
                         << endl;

                    no = static_cast<int>((WritingEND - WritingSTART) / CLOCKS_PER_SEC);
                    cout << "Writing data: " << no;
                    cout << "clocks "
                         << "(" << (WritingEND - WritingSTART) / CLOCKS_PER_SEC << " ms). " << endl
                         << endl;

                    saveFile();          //嚙篇嚙踝蕭
                    PrintRead_vector();  // print 嚙瘡嚙複序迎蕭嚙罷嚙瘠

                    CookList.clear();

                    file.close();
                    file.clear();

                }  // if()

                Donotfile = false;
                // Shell sort
                // 嚙盤嚙碼嚙踝蕭

                break;
            case 2:
                cout << "Input a file number (e.g., 301, 302, 303, ...): ";
                cin >> num;
                FindFile2(num);
                sumoflist = 0;
                processtime = 0;

                if (Donotfile == true) {
                    SetCookList();  // 嚙踝蕭嚙踝蕭仵嚙?嚙衛開嚙締讀嚙踝蕭
                    //PrintRead_vector();
                    CheckList();  // 嚙確嚙緹嚙緬嚙踝蕭X嚙緲嚙踝蕭
                    sumoflist = CookList.size();
                    Simulate();   // 嚙畿嚙緲嚙緬嚙踝蕭
                    saveFile2();  //嚙篇嚙踝蕭
                    CookList.clear();
                    Abortlist.clear();
                    Donelist.clear();
                    file.close();
                    file.clear();
                }  // if()

                Donotfile = false;

                break;
            
            default:
                cout << endl
                     << "Command does not exist!" << endl;
        }  // end switch
    } while (command != 0);
    return 0;  // '0': stop the program
}  // end of main