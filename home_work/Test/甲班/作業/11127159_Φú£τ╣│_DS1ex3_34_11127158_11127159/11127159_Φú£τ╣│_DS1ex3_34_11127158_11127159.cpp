// 資工二甲 第34組
// 11127158 周宗昱
// 11127159 廖為宥

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include <iomanip> // xxx.yy

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct DataPoint {
  string oid = "" ;
  string arrival;
  string duration;
  string timeout;
  string firstline ;
}; // DataPoint

int String2Int( string str ) {
	int result = 0 ;
	for( int i = 0 ; i < str.size() ; i++ ) { // 32392
	 if( str[i]-48 < 0 || str[i]-48 > 9 )
	  cout << "String2Int Error: Not digit" ;
	 else {
	  result = result*10 + (str[i]-48) ;
	 }
	} // for
	return result ;
}

class Mission1{
private:

 vector<DataPoint> data ;
 string fileNumber ;

 double readingTime = 0;
 double sortingTime = 0;
 double writingTime = 0;


public:

 Mission1(): data({}) {
 }

 bool ReadFile( string & fileNum ) {


   double start, endd;
   start = clock() ;

   string textName ;
   cin >> fileNumber ;
   fileNum = fileNumber ;
   textName = "input" + fileNumber + ".txt" ;
   ifstream inputFile( textName.c_str() ) ;

   if ( !inputFile.is_open() ) {
     cout << endl << "### " << textName << " does not exist! ###" << endl ;
    return false;
   } // if

   else
     cout << endl;


   char tempch ;
   string tempstr ;
   int oadt = 0 ;

   data.push_back( DataPoint() ) ;
   getline( inputFile, data[0].firstline ) ;

   data.push_back( DataPoint() ) ;

   while ( inputFile.get( tempch ) ) {
     if ( tempch == '\t' ) { // oadt == 0????
       oadt = oadt + 1 ;
     } // if

     else if ( tempch == '\n' ) { // oadt == 3
       data.push_back(DataPoint());
       oadt = 0 ;
     } // else if


     else {
       if ( oadt == 0 )
         data.back().oid = data.back().oid + tempch ;
       else if ( oadt == 1 )
         data.back().arrival = data.back().arrival + tempch ;
       else if ( oadt == 2 )
         data.back().duration = data.back().duration + tempch ;
       else if ( oadt == 3 )
         data.back().timeout = data.back().timeout + tempch ;
       else
         cout << "oadt ERROR!" ;

     } // else

   } // while

	if( data.back().oid == "" )
		data.pop_back() ;

    cout << '\t' << data[0].firstline << '\n' ;
    for( int i = 1 ; i < data.size() ; i ++ ) {
      cout << "(" << i << ")" << '\t' ;
      cout << data[i].oid << '\t' << data[i].arrival << '\t' ;
      cout << data[i].duration << '\t' << data[i].timeout << '\n' ;
    } // for


    endd = clock();
    double readingTime = ( endd - start ) / CLOCKS_PER_SEC  ;

    return true ;
 } // end of ReadFile






  int String2Int( string str ) {
    int result = 0 ;
    for( int i = 0 ; i < str.size() ; i++ ) { // 32392
     if( str[i]-48 < 0 || str[i]-48 > 9 )
      cout << "String2Int Error: Not digit" ;
     else {
      result = result*10 + (str[i]-48) ;
     }
    } // for
    return result ;
  }






void ShellSort() {
    int n = data.size();

    for (int h = n / 2; h > 0; h = h / 2) {
        for (int i = h; i < n; i++) {
            int loc = i;
            DataPoint nextItem = data[i];

            int arrivalValue = String2Int(nextItem.arrival);

            while ((loc >= h) && ((String2Int(data[loc - h].arrival) > arrivalValue) ||
                ((String2Int(data[loc - h].arrival) == arrivalValue) &&
                 (String2Int(data[loc - h].oid) > String2Int(nextItem.oid))))) {
                data[loc] = data[loc - h];
                loc = loc - h;
            }

            data[loc] = nextItem;
        }
    }
}






  void SortData() {
    double start, endd;
    start = clock() ;

    DataPoint temphead ;

    temphead = data[0] ;

    data.erase( data.begin() ) ;

    ShellSort() ;

    data.insert( data.begin(), temphead ) ;


//    for ( int j = 1 ; j < data.size() - 1 ; j++ ) {
//      data[j] = data[j+1] ;
//    } // for

//    cout << '\t' << data[0].firstline << '\n' ;
//    for( int i = 1 ; i < data.size() ; i ++ ) {
//      cout << "(" << i << ")" << '\t' ;
//      cout << data[i].oid << '\t' << data[i].arrival << '\t' ;
//      cout << data[i].duration << '\t' << data[i].timeout << '\n' ;
//    } // for

    endd = clock() ;

    sortingTime = ( endd - start ) / CLOCKS_PER_SEC ;

  } // SortData







  void sortNewFile() {

    double start, endd;
    start = clock() ;

    string textname;
    textname = "sorted" + fileNumber + ".txt" ;

    ofstream sortedFile( textname.c_str() ) ;

    sortedFile << data[0].firstline << endl ;
    for( int i = 1 ; i < data.size() ; i++ ) {
      sortedFile << data[i].oid << '\t' << data[i].arrival << '\t' ;
      sortedFile << data[i].duration << '\t' << data[i].timeout << '\n' ;
    } // for

    sortedFile.close() ;

    endd = clock() ;
    writingTime = ( endd - start ) / CLOCKS_PER_SEC ;

  } // sortNewFile


  void printTime() {
    int read = readingTime ;
    int sortt = sortingTime ;
    int write = writingTime ;
    cout << endl ;
    cout << "Reading data: " << read << " clocks (" << readingTime << " ms)." << endl ;
    cout << "Sorting data: " << sortt << " clocks (" << sortingTime << " ms)." << endl ;
    cout << "Writing data: " << write << " clocks (" << writingTime << " ms)." << endl ;

    cout << endl << "See sorted" << fileNumber << ".txt" << endl ;

  } // printTime





}; // class Mission1


template <typename T>
class JobQueue {

private:

  vector<T> queues ;
  int qFront, qBack ;
  int qSize ;
  int qMax = 3 ;

public:
  int avail ; // public data member

  JobQueue<T>():queues({}),qFront(0), qSize(0), avail(0),qMax(3) {
  } // end of constructor

  int length() {
    return queues.size() ;
  } // length

  bool isEmpty() {
  	return qSize == 0 ;
  } // isEmpty

  bool isFull()  {
    if( qSize == qMax )
      return true ;
    else
      return false ;
  } // isFull

  void enQueue(T & element) {
    if( isFull() != true ) {
    	queues.push_back( element ) ;
    	qSize = qSize + 1 ;
	}

    else
      cout << "enQueue Error: queues is full" << endl ;
  }

  void getFront(T & element ) {
    if( ! isEmpty() )
      element = queues.front() ;
    else
      cout << "getFront Error: queues is empty" << endl ;
  }
  void deQueue() {
    if ( ! isEmpty() ) {
    	queues.erase(queues.begin()) ;
    	qSize = qSize - 1 ;
	}


    else{
      cout << "deQueue Error: queues is empty" << endl ;
    }

  } // deQueue

  void deQueue(T & element) {
    if ( ! queues.empty ) {
      element = queues[queues.begin()] ;
      queues.erase(queues.begin());
      qSize = qSize - 1 ;
    } // if

    else{
      cout << "deQueue Error: queues is empty" << endl ;
    }

  } // deQueue

  void clearQ() {
    if ( !isEmpty() ) {
      queues.clear() ;
    } // if

//    else {
//      cout << "Error : vector is empty!" << endl;
//    } // else

  } // clearQ

  ~JobQueue(){
    clearQ() ;
  } // end of destructor


}; // end JobQueue

struct DataPoint2 {
	string oid ;
	int abort ;
	int departure ;
	int delay ;
};

class mission2{

public:

 mission2(): data({}),aborts({}),dones({}) {
 }

bool ReadFile( string & fileNum ) {

   string textName ;
   if ( fileNum == "-1" ) {
      cout << endl << "Input a file number: " ;
      cin >> fileNumber ;
      fileNum = fileNumber ;
   } // if
   else
      fileNumber = fileNum ;

   textName = "sorted" + fileNumber + ".txt" ;
   ifstream inputFile( textName.c_str() ) ;

   if ( !inputFile.is_open() ) {
     cout << endl << "### " << textName << " does not exist! ###" << endl ;
    return false;
   } // if

   else
     cout << endl;


   char tempch ;
   string tempstr ;
   int oadt = 0 ;

   data.push_back( DataPoint() ) ;
   getline( inputFile, data[0].firstline ) ;

   data.push_back( DataPoint() ) ;

   while ( inputFile.get( tempch ) ) {
     if ( tempch == '\t' ) { // oadt == 0????
       oadt = oadt + 1 ;
     } // if

     else if ( tempch == '\n' ) { // oadt == 3
       data.push_back(DataPoint());
       oadt = 0 ;
     } // else if


     else {
       if ( oadt == 0 )
         data.back().oid = data.back().oid + tempch ;
       else if ( oadt == 1 )
         data.back().arrival = data.back().arrival + tempch ;
       else if ( oadt == 2 )
         data.back().duration = data.back().duration + tempch ;
       else if ( oadt == 3 )
         data.back().timeout = data.back().timeout + tempch ;
       else
         cout << "oadt ERROR!" ;

     } // else

   } // while

	if( data.back().oid == "" )
		data.pop_back() ;

//    cout << '\t' << data[0].firstline << '\n' ;
//    for( int i = 1 ; i < data.size() ; i ++ ) {
//      cout << "(" << i << ")" << '\t' ;
//      cout << data[i].oid << '\t' << data[i].arrival << '\t' ;
//      cout << data[i].duration << '\t' << data[i].timeout << '\n' ;
//    } // for



   inputFile.close() ;
    return true ;
 } // end of ReadFile

	void Cpu() {
		int clock = 0 ;
		int temp = 1 ; // skip first line 地己見是
//		clock = String2Int(data[1].arrival) ;

		DataPoint working ;
		bool iswork = false ;
		int wbegin = 0 ;


//		cout<< "data.size:" << data.size() << endl ;
		while( temp < data.size()-1 || !cpu.isEmpty() ) {
//			cout << "clock: " << clock ;

			bool ischangetemp = false ;


//			if ( cpu.isEmpty() )
//				cout << "eeeeeee" ;

//			if ( clock >= String2Int(working.timeout)  ) { // 上一個做完這個就已超時
//				cout << "7" ;
//				aborts.push_back( DataPoint2() ) ;
//				aborts.back().oid = working.oid ;
//				aborts.back().abort = clock ;
//				aborts.back().delay = clock - String2Int(working.arrival) ;
//
//
//				if ( ! cpu.isEmpty() ) {
//					cout << "8" ;
//					cpu.getFront(working) ;
//					cpu.deQueue() ;
//
//				}
//				else {
//					iswork = false ;
//					break ;
//				}
//
//
//			} // if

			if( clock == wbegin + String2Int(working.duration) && clock != 0 && !ischangetemp && iswork ) { // A 做完了放入dones
//				cout << "4" ;
				dones.push_back(DataPoint2()) ;
				dones.back().oid = working.oid ;
				dones.back().departure = clock ;
				dones.back().delay = wbegin - String2Int(working.arrival) ;
				wbegin = clock ;

				if ( ! cpu.isEmpty() ) {
					cpu.getFront(working) ;
					cpu.deQueue() ;

					while ( clock >= String2Int(working.timeout)  ) { // 上一個做完這個就已超時
//						cout << "7" ;
						aborts.push_back( DataPoint2() ) ;
						aborts.back().oid = working.oid ;
						aborts.back().abort = clock ;
						aborts.back().delay = clock - String2Int(working.arrival) ;


						if ( ! cpu.isEmpty() ) {
//							cout << "8" ;
							cpu.getFront(working) ;
							cpu.deQueue() ;

						}
						else {
							iswork = false ;
							break ;
						}


					} // while
				} // if
				else
					iswork = false ;

			}

			if( clock == String2Int(data[temp].arrival) && cpu.isEmpty() && !iswork && !ischangetemp ) { // 沒東西working時，將剛進入的當作working
//				cout << "1" ;
				working = data[temp] ;
				wbegin = clock ;
				temp = temp + 1 ;
				iswork = true ; // 要再做完沒是實改成false
				ischangetemp = true ;
			}


			if( clock == String2Int(working.timeout) && !ischangetemp && iswork ) { // 時間到了做不完就放到abort
//				cout << "wbegin:" << wbegin << " clock: " << clock << " dudu: " << working.duration ;
				if ( clock - wbegin < String2Int(working.duration)  ) {
//					cout << "5" << "oid: " << working.oid ;
					aborts.push_back( DataPoint2() ) ;
					aborts.back().oid = working.oid ;
					aborts.back().abort = clock ;
					aborts.back().delay = clock - String2Int(working.arrival) ;
					wbegin = clock ;

					if ( ! cpu.isEmpty() ) {
						cpu.getFront(working) ;
						cpu.deQueue() ;
						while ( clock >= String2Int(working.timeout)  ) { // 上一個時間到了但這個就已超時
//							cout << "7" ;
							aborts.push_back( DataPoint2() ) ;
							aborts.back().oid = working.oid ;
							aborts.back().abort = clock ;
							aborts.back().delay = clock - String2Int(working.arrival) ;


							if ( ! cpu.isEmpty() ) {
//								cout << "8" ;
								cpu.getFront(working) ;
								cpu.deQueue() ;

							}
							else {
								iswork = false ;
								break ;
							}


						} // while
					} // if
					else {
//						cout << "9" ;
						iswork = false ;
					}
				}
			}

			if( clock == String2Int(data[temp].arrival) && iswork && !ischangetemp ) {  //  放進ＣＰＵ
				if( cpu.isFull() ) { // B 要進入ｃｐｕ時ｃｐｕ已滿
//					cout << "2" ;
					aborts.push_back( DataPoint2() ) ;
					aborts.back().oid = data[temp].oid ;
					aborts.back().abort = clock ;
					aborts.back().delay = 0 ;
					temp = temp + 1 ;
					ischangetemp = true ;
				}

				else{
//					cout << "3" ;
					cpu.enQueue(data[temp]) ;
					temp = temp + 1 ;
					ischangetemp = true ;
				}
			}

			if(temp!=0) {
				if( data[temp].arrival == data[temp-1].arrival || ischangetemp == true  ) { // for same arrival case
//					cout << "6" ;
					clock = clock - 1 ;
				} // if
			} // if

			clock = clock + 1 ;

//			cout << endl ;

//			cout << "datasize: " << data.size() << endl ;
//			cout << "temp: " << temp << endl ;

		} // while


		if ( working.oid != aborts.back().oid && working.oid != dones.back().oid ) { // 最後一筆ｗｏｒｋｉｎｇ
//			cout << "999" ;
			clock = clock + 1 ;
			if ( String2Int(working.timeout)  >= String2Int(working.duration) + wbegin ) {
				dones.push_back(DataPoint2()) ;
				dones.back().oid = working.oid ;
				dones.back().departure = clock ;
				dones.back().delay = wbegin - String2Int(working.arrival) ;

			}
			else if ( String2Int(working.timeout)  <= String2Int(working.duration) + wbegin ) {
				aborts.push_back( DataPoint2() ) ;
				aborts.back().oid = working.oid ;
				aborts.back().abort = clock ;
				aborts.back().delay = clock - String2Int(working.arrival) ;
			} // if
		} // if

//		cout << '\t' << "[Abort Jobs]" << endl ;
//		cout << '\t' << "OID" << '\t' << "Abort" << '\t' << "Delay" << endl ;

	    for( int i = 0 ; i < aborts.size() ; i ++ ) {
	      sumDelay = sumDelay + aborts[i].delay ;
//	      cout << "[" << i+1 << "]" << '\t' ;
//	      cout << aborts[i].oid << '\t' <
//	      cout << aborts[i].abort << '\t' << aborts[i].delay << '\n' ;
	    } // for

//	    cout << '\t' << "[Jobs Done]" << endl ;
//	    cout << '\t' << "OID" << '\t' << "Departure" << '\t' << "Delay" << endl ;

	    for( int i = 0 ; i < dones.size() ; i ++ ) {
	    	sumDelay = sumDelay + dones[i].delay ;
//	      cout << "[" << i+1 << "]" << '\t' ;
//	      cout << dones[i].oid << '\t' ;
//	      cout << dones[i].departure << '\t' << dones[i].delay << '\n' ;
	    } // for



	} // Cpu

  void NewFile() {
    string textname;
    textname = "output" + fileNumber + ".txt" ;

    cout << "The simulation is running..." << endl;
    cout << "See " << textname << endl ;

    ofstream outputFile( textname.c_str() ) ;

    outputFile << '\t' << "[Abort Jobs]" << endl << '\t' << "OID" << '\t' << "Abort" << '\t' << "Delay" << endl ;
    for( int i = 0 ; i < aborts.size() ; i++ ) {
      outputFile << "[" << i+1 << "]" << '\t' << aborts[i].oid << '\t' ;
      outputFile << aborts[i].abort << '\t' << aborts[i].delay << '\n' ;
    } // for

    outputFile << '\t' << "[Jobs Done]" << endl << '\t' << "OID" << '\t' << "Departure" << '\t' << "Delay" << endl ;
    for( int i = 0 ; i < dones.size() ; i++ ) {
      outputFile << "[" << i+1 << "]" << '\t' << dones[i].oid << '\t' ;
      outputFile << dones[i].departure << '\t' << dones[i].delay << '\n' ;
    } // for

    float averageDelay = static_cast<float>(sumDelay) / static_cast<float>(( aborts.size() + dones.size() )) ;
    float successRate = static_cast<float>(dones.size()) / static_cast<float>(( aborts.size() + dones.size() )) ;
    successRate = successRate * 100 ;

//	    cout << "aborts.size: " << aborts.size() << " dones.size " << dones.size() ;
//	    cout << " sumDelay: " << sumDelay ;

    outputFile << "[Average Delay]" << '\t' << fixed << setprecision(2) << averageDelay << " ms" << endl ;
    outputFile << "[Success Rate]" << '\t' << fixed << setprecision(2) << successRate << " %" << endl ;


    outputFile.close() ;

  } // NewFile

  ~mission2(){
  	data.clear() ;
  	aborts.clear() ;
  	dones.clear() ;
  }

	private:
	  	string fileNumber ;
	  	vector<DataPoint> data ;
	  	JobQueue<DataPoint> cpu ;
	  	vector<DataPoint2> aborts ;
	  	vector<DataPoint2> dones ;
		int sumDelay = 0 ;
		int sumSucess = 0 ;

}; // mission2



int main(int argc, char** argv) {
  string command = "\0";
    string fileNum = "-1";

  while ( command != "0" ) {

    cout << endl << "**** Simulate FIFO Queues by SQF *****" ;
    cout << endl << "* 0. Quit                            *" ;
    cout << endl << "* 1. Sort a file                     *" ;
    cout << endl << "* 2. Simulate one FIFO queue         *" ;
    cout << endl << "**************************************" ;
    cout << endl << "Input a command(0, 1, 2): " ;
    cin >> command ;

    if ( isdigit( command[0] ) ) {

      if ( command == "0" )
        break ;

      else if ( command == "1" )
      {
      	cout << endl << "Input a file number: " ;

        Mission1 mission1 ;
        if ( mission1.ReadFile(fileNum) ) {
          mission1.SortData() ;
          mission1.sortNewFile() ;
          mission1.printTime() ;
        } // if

      } // if

      else if ( command == "2" )
      {
        mission2 m2 ;
        if ( m2.ReadFile(fileNum) ) {
          m2.Cpu() ;
          m2.NewFile() ;
        } // if

      } // else if

      else {
        cout << endl << "Command does not exist!" << endl ;
      } // else


    } // if


    else {
      cout << endl << "Command does not exist!" << endl ;
    } // else



  } // while ( command[0] != '0' )

  return 0 ;

}
