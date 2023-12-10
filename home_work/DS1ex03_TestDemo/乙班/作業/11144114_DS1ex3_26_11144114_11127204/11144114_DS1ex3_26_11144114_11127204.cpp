//11144114 盧鈴臻  11127204 吳冠毅
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
using namespace std;

struct Item {
    int OID;
    int Arrival;
    int Duration;
    int Timeout;
};

bool compareItems(const Item& a, const Item& b) {
    if (a.Arrival == b.Arrival) {
        return a.OID > b.OID;
    }
    return a.Arrival > b.Arrival;
}

void shellSort(vector<Item>& arr) {
  int n = arr.size();

  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; i++) {
      Item temp = arr[i];
        int j;
        for (j = i; j >= gap && compareItems(arr[j - gap], temp); j -= gap) {
          arr[j] = arr[j - gap];
        }
      arr[j] = temp;
    }
  }
}


int main() {
  vector<Item> items;
  int command = 0;
  double Rstart, Rend;
  double Sstart, Send;
  double Wstart, Wend;
  string fileName;
  string fullFileName_in;
  string fullFileName_out;
  ifstream openFile;
  ofstream outputFile;

  do{
    vector<Item> items;
    cout << endl << "**** Simulate FIFO Queues by SQF *****";
    cout << endl << "* 0. Quit                            *";
    cout << endl << "* 1. Sort a file                     *";
    cout << endl << "* 2. Simulate one FIFO queue         *";
    cout << endl << "**************************************";
    cout << endl << "Input a command(0, 1, 2):";
    cin >> command;

    switch (command) {
      case 0:
        break;

      case 1:
        //filename
        cout << endl << "Input a file number:";
        cin >> fileName;
        cout << endl;
        fullFileName_in = "input" + fileName + ".txt";
        //time start to openfile
        Rstart = clock();
        //read file
        openFile.open(fullFileName_in);
        if (!openFile.is_open()) {
          cout << "### " << fileName << " does not exist! ###" << endl;
          break;
        }
        else {
          // read the title
          string oidLabel, arrivalLabel, durationLabel, timeoutLabel;
          openFile >> oidLabel >> arrivalLabel >> durationLabel >> timeoutLabel;

          int oid, arrival, duration, timeout;
          // read the data
          while (openFile >> oid >> arrival >> duration >> timeout) {
            items.push_back({oid, arrival, duration, timeout});
          }
          openFile.close();
          //time end to open file
          Rend = clock();
          // shell sort
          Sstart = clock();
          shellSort(items);
          Send = clock();

          // output file
          fullFileName_out = "sorted" + fileName + ".txt";
          outputFile.open(fullFileName_out);
          if (!outputFile.is_open()) {
            cout << "output file updating failed..." << endl;
          }
          //time start to write file
          Wstart = clock();
          // write title
          outputFile << oidLabel << "\t" << arrivalLabel << "\t" << durationLabel << "\t" << timeoutLabel << endl;
          // write data
          for (int i = 0; i < items.size(); ++i) {
            outputFile << items[i].OID << "\t" << items[i].Arrival << "\t" << items[i].Duration << "\t" << items[i].Timeout << endl;
          }
          //time end to write file
          Wend = clock();
          outputFile.close();
          cout << endl;
          cout << "Reading data: " << (Rend - Rstart) << " clocks. " << endl;
          cout << "Sorting data: " << (Send - Sstart) << " clocks. " << endl;
          cout << "Writing data: " << (Wend - Wstart) << " clocks. " << endl;
          cout << endl;
          cout << "See " << fullFileName_out << endl;
          items.clear();
          break;
        }
      default:
        cout << "Command does not exist!" << endl;
        break;
    }
  } while(command != 0);
  return 0;
}
