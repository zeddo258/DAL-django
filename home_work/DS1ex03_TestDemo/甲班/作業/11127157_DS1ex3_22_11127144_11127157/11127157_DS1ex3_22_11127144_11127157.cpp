// 11127144 MA,YU-CHAO / 11127157 HUANG,ZI-YAO 

#include <iostream>
#include <time.h>

class MData
{
public :
	const int OID;  // �u�@�s��
	const int Arrival;  // �i�J�ɨ�
	const int Duration;  // �u�@�Ӯ�
	const int TimeOut;  // �O�ɮɨ�
	int Delay;  // ����ɶ�
	int Departure ; // �u�@�_�I

public :
	MData( const int oid , const int arrival , const int duration , const int timeOut );

	MData( const MData& data );

	void operator=( const MData& data );

	bool operator>( const MData& data ) const;

	char * Output( char * outStr ) const;
};

// Constructor
MData::MData( const int oid , const int arrival , const int duration , const int timeOut )
		: OID( oid )  // �NOID��l�Ƭ��ǤJ��oid 
		, Arrival( arrival )  // �NArrival��l�Ƭ��ǤJ��arrival 
		, Duration( duration )  // �NDuration��l�Ƭ��ǤJ��duration
		, TimeOut( timeOut )  // �NTimeOut��l�Ƭ��ǤJ��timeOut
		, Delay( 0 )  // �NDelay��l�Ƭ��ǤJ��delay 
{
	Delay = 0;
	Departure = 0 ;  // �u�@�_�I�w�]��0 
}

MData::MData( const MData& data )
		: OID( data.OID )  
		, Arrival( data.Arrival )  
		, Duration( data.Duration )  
		, TimeOut( data.TimeOut ) 
		, Delay( data.Delay )
		, Departure( data.Departure )
{

}

// �����F�ɶ��A�Y�ۦP�N��u�@�s�� 
bool MData::operator>( const MData& data ) const
{
	if( this->Arrival != data.Arrival )
		return ( this->Arrival > data.Arrival );
	return ( this->OID > data.OID );
}

// �NMData����T�ΥH�U�榡��X  
char * MData::Output( char * outStr ) const
{
	sprintf( outStr , "%d\t%d\t%d\t%d" ,
			OID , Arrival , Duration , TimeOut );
	return outStr;
}

class Queue
{
private :
	MData ** _data;
	int _size; // buffer ���ƶq
	int _count; // �ثe���ƶq
public :
	Queue();  // Constructor

	~Queue();  // Destructor

	// �M�Ŧ�C 
	void Clear();

	// ���o��C�ƶq
	int getCount() const;

	// �s����C���Yindex��MData���� 
	const MData * operator[]( int index ) const;

public :
	
	// �[�J
	void Add( const int oid , const int arrival , const int duration , const int timeOut );

	void Add( const MData& data );

    // ���J 
	void Insert( int index , const MData& data );

    // �Ƨ� 
	void Sort();

    // �R�� 
	MData * Push();
};

// Constructor 
Queue::Queue()
{
	_count = 0;
	_size = 8;
	_data = new MData * [_size];  // �ʺA���t�@�� MData ���Ъ��}�C�A�Ω�s�x MData ����A�j�p��8 
}

// Destructor
Queue::~Queue()
{
	Clear();
}

// ��ڲ����O����
void Queue::Clear()
{
	int i;
	for( i = 0 ; i < _count ; ++i )
		delete _data[i];
	_count = 0;
}

// �b�S�w���޴��J�s������A�p�G�W�X�e�q�A�N _data �}�C���j�p�[���C
void Queue::Insert( int index , const MData& data )
{
	int i ;
	
	if( ( _count + 1 ) >= _size )
	{
		int i;
		MData ** oldData = this->_data;
		int size = _size;
		_size *= 2;
		_data = new MData * [_size];
		for( i = 0 ; i < size ; ++i )
			_data[i] = oldData[i];
		delete oldData;
	}
	i = _count ;
	for( ; i > index ; --i )
		_data[i] = _data[i-1] ;
	_data[index] = new MData( data );
	_count++;
}

// �[�J 
void Queue::Add( const MData& data )
{
	Insert( _count , data );
}

void Queue::Add( const int oid , const int arrival , const int duration , const int timeOut )
{
	MData data( oid , arrival , duration , timeOut );
	Add( data );
}

// �����C���ƶq
int Queue::getCount() const
{
	return _count;
}

// ��C�������O���VMData�����СC
const MData * Queue::operator[]( int index ) const
{
	if( index < 0 ) return NULL;
	if( index >= _count ) return NULL;
	return _data[index];  // return _data���S�w���ު�MData������ 
}

//  Shell Sort �ƧǪk 
void Queue::Sort()
{
	int i , k;
	MData * pDataTemp = NULL;  // �洫�ɼȦs�� 
	int step = _count / 2;   

	while( step >= 1 )
	{
		for( i = step ; i < _count ; i++ )  // �q step �� _count - 1�A�C���W�[�B���C
		{
			for( k = i - step ; k >= 0 ; k -= step )  // �b�B���d�򤺶i�洡�J�ƧǡA�C����֨B�� 
			{
				if( ( *_data[k] ) > ( *_data[k + step] ) ) // �p�G�e�@�Ӥ����j���@�Ӥ����A�i��洫
				{
					pDataTemp = _data[k];
					_data[k] = _data[k + step];
					_data[k + step] = pDataTemp;
				}
			}
		}
		step /= 2;
	}
}

// �q��C�e�ݨ��X�@�Ӥ����A�C�@�Ӥ�������e�@�Ӧ�m 
MData * Queue::Push()
{
	int i ;
	MData * ret = _data[0] ;
	_count -- ;
	for( i = 0 ; i <= ( _count ) ; i++ )
		_data[i] = _data[i+1] ;

	return ret ;
}

// �Ƨ�
void runSort( int &outFileIndex , Queue& queue )
{
	clock_t startTime , endTime; // �p��{���ɶ�
	clock_t runTime; // �������
	int oid , arrival , duration , timeOut;
	int fileIndex;
	int count;
	int i;

	char str[1024];
	char fileName[1024];
	printf( "\n" );
	printf( "Input a file number: " );
	scanf( "%d" , &fileIndex );
	printf( "\n" );
	sprintf( str , "input%d.txt" , fileIndex );

	startTime = clock(); // �}�l�p�� 
	FILE * input = fopen( str , "r" );
	if( input == NULL )
	{
		printf( "### input%d.txt does not exist! ###\n" , fileIndex );
		return;
	}
	// �M��
	queue.Clear();
	outFileIndex = fileIndex ;
	// �Ĥ@�椣�n
	fgets( str , sizeof( str ) , input );
	// �}�l�AŪ�쵲������
	while( 1 )
	{
		str[0] = 0;
		fgets( str , sizeof( str ) , input );
		if( str[0] == 0 )break;
		if( sscanf( str , "%d\t%d\t%d\t%d" , &oid , &arrival , &duration , &timeOut ) < 4 )
			break;

		queue.Add( oid , arrival , duration , timeOut );
	}
	// ����
	fclose( input );
	// ��X
	count = queue.getCount();
	printf( "\tOID\tArrival\tDuration\tTimeOut\n" );
	for( i = 0 ; i < count ; i++ )
	{
		printf( "(%d)\t%s\n" , ( i + 1 ) , queue[i]->Output( str ) );
	}
	printf( "\n" );

	endTime = clock(); // ����
	runTime = endTime - startTime; // �B��
	double readTime = ( ( double ) runTime ) / CLOCKS_PER_SEC; // �ɶ��ഫ���� 
	// �Ƨ�
	startTime = clock(); // �}�l
	queue.Sort();
	endTime = clock(); // ����
	runTime = endTime - startTime; // �p��ƧǮɶ� 
	double sortTime = ( ( double ) runTime ) / CLOCKS_PER_SEC; // �ɶ��ഫ����
	// �g�J
	sprintf( fileName , "sorted%d.txt" , fileIndex );

	startTime = clock(); // �}�l
	FILE * output = fopen( fileName , "w" );

	fprintf( output , "OID\tArrival\tDuration\tTimeOut\n" );
	count = queue.getCount();
	for( i = 0 ; i < count ; i++ )
	{
		fprintf( output , "%s\n" , queue[i]->Output( str ) );
	}
	endTime = clock(); // ����
	runTime = endTime - startTime; // �B��
	double writingTime = ( ( double ) runTime ) / CLOCKS_PER_SEC; // �B��(double


	fclose( output );

	printf( "Reading data: %d clocks (%0.2lf ms).\n" , ( int ) readTime , readTime );
	printf( "Sorting data: %d clocks (%0.2lf ms).\n" , ( int ) sortTime , sortTime );
	printf( "Writing data: %d clocks (%0.2lf ms).\n" , ( int ) writingTime , writingTime );
	printf( "\n" );

	printf( "See %s\n" , fileName );

	startTime = clock(); // �}�l
	endTime = clock(); // ����
}

// �� cpu �Ƨ�
void runSimulation( int &fileIndex , Queue& queue )
{
    int oid , arrival , duration , timeOut;
	int count;
	int i;
	int queueIndex = 0;
	int runTime;
	char str[1024];
	char fileName[1024] ;
	int startAbortPos = 0 ;

    if( fileIndex == -1){
        printf( "\n" );
        printf( "Input a file number: " );
        scanf( "%d" , &fileIndex );
        printf( "\n" );
        sprintf( str , "sorted%d.txt" , fileIndex );

        FILE * input = fopen( str , "r" );
        if( input == NULL )
        {
            printf( "### sorted%d.txt does not exist! ###\n" , fileIndex );
            return;
        }
        // �M��
        queue.Clear();
        // �Ĥ@�椣�n
        fgets( str , sizeof( str ) , input );
        // �}�l�AŪ�쵲������
        while( 1 )
        {
            str[0] = 0;
            fgets( str , sizeof( str ) , input );
            if( str[0] == 0 )break;
            if( sscanf( str , "%d\t%d\t%d\t%d" , &oid , &arrival , &duration , &timeOut ) < 4 )
                break;

            queue.Add( oid , arrival , duration , timeOut );
        }
        // ����
        fclose( input );
    }

	const MData * runPData = NULL;
	MData * ppRunning = NULL ;
	Queue readyQueue; // �ǦC
	Queue abortList ;// ��X�� 
	Queue doneList ;// �����u�@

	for( runTime = 0 ;; ++runTime )
	{
		// �u�@����
		if( runPData != NULL )
		{
			if( ( runPData->Departure + runPData->Duration ) == runTime )
			{
				MData saveData( *runPData );
				if( saveData.Departure > saveData.Arrival )
					saveData.Delay = runTime - ( saveData.Arrival + saveData.Duration );
				saveData.Departure += saveData.Duration ;
				doneList.Add( saveData );
				if( ppRunning != NULL )
					delete ppRunning ;
				runPData = NULL ;
				ppRunning = NULL ;

			}else if(runTime == runPData->TimeOut )
			{
				ppRunning->Departure = runTime ;
				ppRunning->Delay = ppRunning->TimeOut - ppRunning->Arrival ;
				abortList.Insert( startAbortPos , *ppRunning );
				if( ppRunning != NULL )
					delete ppRunning ;
				runPData = NULL ;
				ppRunning = NULL ;
			}
		}
		// ���S���F
		if( queueIndex >= queue.getCount() )
		{
			if( runPData == NULL )
				if( readyQueue.getCount() == 0 )
					break ;
		}
		// �p�G�S���A�N���X�@�Ӥu�@
		while( runPData == NULL )
		{
			if( readyQueue.getCount() == 0 )
				break ;
			ppRunning = readyQueue.Push();
			ppRunning->Departure = runTime ;
			// ����ɶ� timeOut
			if( runTime >= ppRunning->TimeOut )
			{
				ppRunning->Delay = runTime - ppRunning->Arrival ;
				abortList.Add( *ppRunning );
				delete ppRunning ;
				ppRunning = NULL ;
				continue ;
			}
			if( 0 )
			{
				int timeout = 0;
				if( ppRunning->OID == 106 )
					timeout = ppRunning->OID;
				timeout = runTime + ppRunning->Duration;

				if( ppRunning->TimeOut <= timeout )
				{
					ppRunning->Departure = runTime;
					ppRunning->Delay = ppRunning->TimeOut - ppRunning->Arrival;
					abortList.Add( *ppRunning );
					if( ppRunning != NULL )
						delete ppRunning;
					runPData = NULL;
					ppRunning = NULL;
					continue;
				}
			}
			runPData = ppRunning ;
			startAbortPos = abortList.getCount();
			break ;
		}

		// �[�J�u�@
		for( i = queueIndex ; i < queue.getCount() ; ++i )
		{
			const MData * nowData = queue[i];
			if( nowData->Arrival > runTime )
				break;
			queueIndex = i + 1 ;
			// �[�J�u�@
			if( runPData == NULL )
			{
				ppRunning = new MData( *nowData );
				ppRunning->Departure = runTime ;
				int departure = ppRunning->Duration + runTime ;
				if( departure > ppRunning->TimeOut )
				{
					ppRunning->Delay = ppRunning->TimeOut - ppRunning->Arrival ;
					abortList.Add( *ppRunning );
					delete ppRunning ;
					ppRunning = NULL ;
					continue ;
				}
				runPData = ppRunning;
				startAbortPos = abortList.getCount();
			}else if( readyQueue.getCount() < 3 )
			{
				readyQueue.Add( *nowData );
			}else
			{
				MData runData( *nowData );
				runData.Departure = runTime ;
				runData.Delay = 0 ;
				abortList.Add( runData );
			}
		}
	}

	// �s��
	int delay = 0 ;
	int delayCount = 0 ;
	sprintf( fileName , "output%d.txt" , fileIndex );
    printf( "\n" );
	printf( "The simulation is running...\n" );
	printf( "See output%d.txt\n" , fileIndex );
	FILE * output = fopen( fileName , "w" );
	fprintf( output , "\t[Abort Jobs]\n"  );
	fprintf( output , "\tOID\tAbort\tDelay\n"  );
	for( i = 0 ; i < abortList.getCount() ; ++i )
	{
		runPData = abortList[i] ;
		fprintf( output , "[%d]\t%d\t%d\t%d\n" , ( i + 1 )
				, runPData->OID , runPData->Departure , runPData->Delay
		);
		if(  runPData->Delay > 0 )
		{
			delay += runPData->Delay ;
			delayCount ++ ;
		}
	}
	fprintf( output , "\t[Jobs Done]\n"  );
	fprintf( output , "\tOID\tDeparture\tDelay\n"  );
	for( i = 0 ; i < doneList.getCount() ; ++i )
	{
		runPData = doneList[i] ;
		fprintf( output , "[%d]\t%d\t%d\t%d\n" , ( i + 1 )
				, runPData->OID , runPData->Departure , runPData->Delay
		);
		if(  runPData->Delay > 0 )
		{
			delay += runPData->Delay ;
			delayCount ++ ;
		}
	}

	double averageDelay = 0 ;
	if( abortList.getCount() > 0 )
		averageDelay = (double)delay / (double)queue.getCount();
	double successRate = (double)doneList.getCount() / (double) queue.getCount() ;
	fprintf( output ,  "[Average Delay]	%0.2lf ms\n" , averageDelay  );
	fprintf( output ,  "[Success Rate]	%0.2lf %%\n" , successRate * 100 );


	fclose( output );
}

int main()
{
	Queue queue;
	int fileIndex = -1 ;
	int command = 0;

	do{

        printf( "\n" );
		printf( "**** Simulate FIFO Queues by SQF *****\n" );
		printf( "* 0. Quit                            *\n" );
		printf( "* 1. Sort a file                     *\n" );
		printf( "* 2. Simulate one FIFO queue         *\n" );
		printf( "**************************************\n" );
		printf( "Input a command(0, 1, 2): " );
		scanf( "%d" , &command );
		if (command == 1)
            runSort( fileIndex , queue );
		else if(command == 2)
            runSimulation( fileIndex , queue );
		else if(!command)
            break;
        else
            printf( "\nCommand does not exist!\n" );

	}while(true);

	system("pause");
	return 0;

}
