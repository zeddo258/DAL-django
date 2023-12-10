// 11127249 郭承維 11127250 陳銘展
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <cstdlib>
# include <iostream>
# include <fstream>
# include <time.h>

using namespace std;

typedef char Str255[255] ;                                                                          // define "string which have max 256 char" is "Str255"  

struct JobType {                                                                                    // define a struct name JobType
  int OID ;                                                                                         // define a number name OID which store one type of this information
  int arrival ;                                                                                     // define a number name arrival which store one type of this information
  int duration ;                                                                                    // define a number name duration which store one type of this information
  int timeout ;                                                                                     // define a number name timeout which store one type of this information
  int row ;                                                                                         // define a number name row which store the row of this information
  JobType * next ;                                                                                  // define a pointer of JobType name next
} ;

typedef JobType * JobTypePtr ;                                                                      // define "JobType *" is "JobTypePtr" 
char gchar = '\n' ;                                                                                 // define a '\n' name gchar is global

/*

PrintInfo function is print job name and every information in this input file

*/

void PrintInfo( JobTypePtr head ) {
  int i = 1 ;	
  JobTypePtr temp = head ;
  cout << '\n' << '\t' << "OID" << '\t' << "Arrival" << '\t' << "Duration" << '\t'<< "TimeOut" << endl ;
  while ( temp != NULL ) {
    cout << "(" << i << ")" << '\t' << temp->OID << '\t' << temp->arrival << '\t'   
        <<  temp->duration << '\t' << temp->timeout << endl ;
    i = i + 1 ;    
    temp = temp->next ;
  } // while ( temp != NULL ) 	
} // PrintInfo()

/*

PrintTime function is print spend reading, sorting and writing time 

*/

void PrintTime( double readend, double sortend, double writeend ) {
  printf( "\nReading data: %f clocks (%f ms).\n", readend, readend ) ;
  printf( "Sorting data: %f clocks (%f ms).\n", sortend, sortend ) ;  
  printf( "Writing data: %f clocks (%f ms).\n", writeend, writeend ) ; 	
} // PrintTime()

/*

Swap function is compare 2 Arrival or OID and see whether need swap ,
first , swap1 need point node which must before swap2 point ,
second , find the node which swap1 point and find the node which swap2 point  
if the Arrival which swap1 point is later than swap2 point or
the Arrival which swap1 point is same but OID is bigger than swap2 point 
then this two row of node which swap1 and swap2 point need swap 

*/

void Swap( JobTypePtr swap1, JobTypePtr swap2 , int m , int mrow ) {
  while ( swap1->row != m ) {
    swap1 = swap1->next ;
  } // while ( swap1->row != m )
  
  while ( swap2->row != mrow ) {
    swap2 = swap2->next ;
  } // while ( swap2->row != mrow ) {
  
  int temp = 0 ;
  if ( swap1->arrival > swap2->arrival || ( swap1->arrival == swap2->arrival && swap1->OID > swap2->OID ) ) {
    temp = swap1->row ;
    swap1->row = swap2->row ;
    swap2->row = temp ;
  } // if ( swap1->arrival > swap2->arrival || ( swap1->arrival == swap2->arrival && swap1->OID > swap2->OID ) )
} // Swap()

/*

ReadFile function is read every information in the input file and store OID, Arrival, Duration, Timeout and 
see this is in which row information , use dynamic array to connect and print every information in this input file 

*/

void ReadFile( JobTypePtr &head, Str255 filenum, int &row ) {
  Str255 filename = {0} ;
  strcat( filename, "input" ) ;
  strcat( filename, filenum ) ;
  strcat( filename, ".txt" ) ;
  string line ;
  ifstream rownum ;
  rownum.open( filename ) ;
  while ( getline( rownum, line ) ) {
  	row = row + 1 ;
  } // while
  
  row = row - 2 ;
  rownum.close() ;
  ifstream in ;
  in.open( filename ) ;
  head = new JobType ; 
  JobTypePtr temp = head ;
  Str255 str ;
  int number = 0 ;
  int currow = 1 ;
  in >> str >> str >> str >> str ;
  do {
	if ( currow <= row ) {
	  in >> number ;  	
	  temp->OID = number ;
      in >> number ;
      temp->arrival = number ;
      in >> number ;
      temp->duration = number ;
      in >> number ;
      temp->timeout = number ;
	  temp->row = currow ;
    } // if ( currow <= row )
        
    currow = currow + 1 ;  
	if ( currow <= row ) {
      temp->next = new JobType ;
      temp = temp->next ;
    } // if ( currow <= row )
  } while ( currow <= row ) ;
  
  in.close() ;
  PrintInfo( head ) ;
} // ReadFile()

/* 

SortFile function is sort every information in the input file 
with shell sort , if the Arrival of this information is earlier   
then this information need row up , if have same Arrival then 
compare OID , OID is smaller then this information need row up , too ,
otherwise , this information need row down 

*/

void SortFile( JobTypePtr &head, int row ) { 
  JobTypePtr temp = head ;
  JobTypePtr swap1 = head ;
  JobTypePtr swap2 = head ;
  int node = row ; 
  row = node / 2 ;
  int i = 1 ;
  int m = 1 ;
  int k = 0 ;
  int q = 0 ;
  while ( row >= 1 ) {
    while ( i <= row ) {
      m = i ;
      q = node / row + 1 ;
      k = 0 ;
      while ( k < q ) {
        m = 1 ;  
        while ( m <= node ) {
          if ( m + row <= node ) {
            Swap( swap1, swap2, m, m + row ) ;  
          } // if ( m + row <= node )
          
          m = m + row ;
        } // while ( m <= node )
      
        k = k + 1 ;
      } // while ( k < q )
    
      i = i + 1 ;
    } // while ( i <= row )
    
    i = 1 ;
    row = row / 2 ;
  } // while ( row >= 1 )
  
  JobTypePtr newhead = NULL ;
  newhead = new JobType ;
  JobTypePtr newtemp = newhead ;
  i = 1 ;
  while ( i <= node ) {
    temp = head ;
    while ( temp != NULL ) {
      if ( temp->row == i ) {
        newtemp->OID = temp->OID ;
        newtemp->arrival = temp->arrival ;
        newtemp->duration = temp->duration ;
        newtemp->timeout = temp->timeout ;
        newtemp->row = temp->row ;
        if ( i != node ) {
          newtemp->next = new JobType ;    
          newtemp = newtemp->next ;
        } // if ( i != node ) 
      } // if ( temp->node == i )
      
      temp = temp->next ;
    } // while ( temp != NULL )
    
    i = i + 1 ;
  } // while ( i <= node )     
  
  delete head ;
  head = newhead ;
} // SortFile()

/*

WriteFile function is write every information in the sort file 

*/

void WriteFile( JobTypePtr head, Str255 filenum ) {
  int i = 1 ;    
  JobTypePtr temp = head ;
  Str255 filename = {0} ;
  strcat( filename, "sort" ) ;
  strcat( filename, filenum ) ;
  strcat( filename, ".txt" ) ;
  ofstream out ;
  out.open( filename ) ;    
  out << '\t' << "OID" << '\t' << "Arrival" << '\t' << "Duration" << '\t'<< "TimeOut" << endl ;
  while ( temp != NULL ) {
    out << "(" << i << ")" << '\t' << temp->OID << '\t' << temp->arrival << '\t'   
        <<  temp->duration << '\t' << temp->timeout << endl ;
    i = i + 1 ;    
    temp = temp->next ;
  } // while ( temp != NULL )
  
  out.close() ;
} // WriteFile()

/*

Mission1 function is let the file which we want to sort , 
first , read every information in the input file and store OID, Arrival, Duration, Timeout and 
see this is in which row information , use dynamic array to connect and 
print job name and every information in this input file and calculate this spend time
second , accoring to Arrival and OID to sort every information in the input file 
with shell sort and calculate this spend time 
thrid , write every information in the sort file and calculate this spend time 
last , print spend reading, sorting and writing time 

*/

void Mission1() {
  JobTypePtr head = NULL ;
  head = new JobType ;    
  int row = 1 ;
  Str255 filenum ;
  printf( "Input a file number: " ) ; 
  scanf( "%s%c", filenum, &gchar ) ; 
  int readstart = clock() ;   
  ReadFile( head, filenum, row ) ;
  double readend = clock() - readstart ;
  int sortstart = clock() ; 
  SortFile( head, row ) ;
  double sortend = clock() - sortstart ;
  int writestart = clock() ; 
  WriteFile( head, filenum ) ;
  double writeend  = clock() - writestart ;
  PrintTime( readend, sortend, writeend ) ;
  printf ( "\nSee sorted%s.txt\n", filenum ) ;
  delete head ;
  head = NULL ;
} // Mission1()

int main(void) {
  int command = 0 ;                                                                                 // define a user command name command , initial value is 0
  do {                                                                                   
    printf( "%c", gchar ) ;
    printf( "**** Simulate FIFO Queues by SQF *****\n" ) ;
    printf( "* 0. Quit                            *\n" ) ;
    printf( "* 1. Sort a file                     *\n" ) ;
    printf( "**************************************\n" ) ;
    printf( "Input a command(0, 1): " ) ;
    scanf( "%d%c", &command, &gchar ) ;                                                             // get a command and '\n'
    if ( command == 0 ) {                                                                           // if command is 0 , then stop the program
      break ;                                                                                       // skip this while    
    } // if ( command == 0 )
    else if ( command == 1 ) {                                                                      // if command is 1 , then sort input file and calculate reading, sorting and writing time 
      printf( "%c", gchar ) ;
      Mission1() ;                                                                                  // go to the function of Mission1
    } // else if ( command == 1 )
    else {                                                                                          // if command is not 0 or 1 , then continue the program   
      printf( "%c", gchar ) ;
      printf( "Command does not exist!\n" ) ;     
    } // else
  } while ( command != 0 ) ;                                                                        // until command is 0
                                                                        
  return 0 ;
} // main()
