// 11127144 MA,YU-CHAO / 11127157 HUANG,ZI-YAO 

#include <iostream>
#include <time.h>

class MData
{
public :
	const int OID;  // 工作編號
	const int Arrival;  // 進入時刻
	const int Duration;  // 工作耗時
	const int TimeOut;  // 逾時時刻
	int Delay;  // 延遲時間
	int Departure ; // 工作起點

public :
	MData( const int oid , const int arrival , const int duration , const int timeOut );

	MData( const MData& data );

	void operator=( const MData& data );

	bool operator>( const MData& data ) const;

	char * Output( char * outStr ) const;
};

// Constructor
MData::MData( const int oid , const int arrival , const int duration , const int timeOut )
		: OID( oid )  // 將OID初始化為傳入的oid 
		, Arrival( arrival )  // 將Arrival初始化為傳入的arrival 
		, Duration( duration )  // 將Duration初始化為傳入的duration
		, TimeOut( timeOut )  // 將TimeOut初始化為傳入的timeOut
		, Delay( 0 )  // 將Delay初始化為傳入的delay 
{
	Delay = 0;
	Departure = 0 ;  // 工作起點預設為0 
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

// 比較抵達時間，若相同就比工作編號 
bool MData::operator>( const MData& data ) const
{
	if( this->Arrival != data.Arrival )
		return ( this->Arrival > data.Arrival );
	return ( this->OID > data.OID );
}

// 將MData的資訊用以下格式輸出  
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
	int _size; // buffer 的數量
	int _count; // 目前的數量
public :
	Queue();  // Constructor

	~Queue();  // Destructor

	// 清空佇列 
	void Clear();

	// 取得佇列數量
	int getCount() const;

	// 存取佇列中某index的MData物件 
	const MData * operator[]( int index ) const;

public :
	
	// 加入
	void Add( const int oid , const int arrival , const int duration , const int timeOut );

	void Add( const MData& data );

    // 插入 
	void Insert( int index , const MData& data );

    // 排序 
	void Sort();

    // 刪除 
	MData * Push();
};

// Constructor 
Queue::Queue()
{
	_count = 0;
	_size = 8;
	_data = new MData * [_size];  // 動態分配一個 MData 指標的陣列，用於存儲 MData 物件，大小為8 
}

// Destructor
Queue::~Queue()
{
	Clear();
}

// 實際移掉記憶體
void Queue::Clear()
{
	int i;
	for( i = 0 ; i < _count ; ++i )
		delete _data[i];
	_count = 0;
}

// 在特定索引插入新的物件，如果超出容量，將 _data 陣列的大小加倍。
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

// 加入 
void Queue::Add( const MData& data )
{
	Insert( _count , data );
}

void Queue::Add( const int oid , const int arrival , const int duration , const int timeOut )
{
	MData data( oid , arrival , duration , timeOut );
	Add( data );
}

// 獲取佇列的數量
int Queue::getCount() const
{
	return _count;
}

// 佇列的元素是指向MData的指標。
const MData * Queue::operator[]( int index ) const
{
	if( index < 0 ) return NULL;
	if( index >= _count ) return NULL;
	return _data[index];  // return _data中特定索引的MData的指標 
}

//  Shell Sort 排序法 
void Queue::Sort()
{
	int i , k;
	MData * pDataTemp = NULL;  // 交換時暫存用 
	int step = _count / 2;   

	while( step >= 1 )
	{
		for( i = step ; i < _count ; i++ )  // 從 step 到 _count - 1，每次增加步長。
		{
			for( k = i - step ; k >= 0 ; k -= step )  // 在步長範圍內進行插入排序，每次減少步長 
			{
				if( ( *_data[k] ) > ( *_data[k + step] ) ) // 如果前一個元素大於後一個元素，進行交換
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

// 從佇列前端取出一個元素，每一個元素移到前一個位置 
MData * Queue::Push()
{
	int i ;
	MData * ret = _data[0] ;
	_count -- ;
	for( i = 0 ; i <= ( _count ) ; i++ )
		_data[i] = _data[i+1] ;

	return ret ;
}

// 排序
void runSort( int &outFileIndex , Queue& queue )
{
	clock_t startTime , endTime; // 計算程式時間
	clock_t runTime; // 執行長度
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

	startTime = clock(); // 開始計時 
	FILE * input = fopen( str , "r" );
	if( input == NULL )
	{
		printf( "### input%d.txt does not exist! ###\n" , fileIndex );
		return;
	}
	// 清空
	queue.Clear();
	outFileIndex = fileIndex ;
	// 第一行不要
	fgets( str , sizeof( str ) , input );
	// 開始，讀到結束為止
	while( 1 )
	{
		str[0] = 0;
		fgets( str , sizeof( str ) , input );
		if( str[0] == 0 )break;
		if( sscanf( str , "%d\t%d\t%d\t%d" , &oid , &arrival , &duration , &timeOut ) < 4 )
			break;

		queue.Add( oid , arrival , duration , timeOut );
	}
	// 結束
	fclose( input );
	// 輸出
	count = queue.getCount();
	printf( "\tOID\tArrival\tDuration\tTimeOut\n" );
	for( i = 0 ; i < count ; i++ )
	{
		printf( "(%d)\t%s\n" , ( i + 1 ) , queue[i]->Output( str ) );
	}
	printf( "\n" );

	endTime = clock(); // 結束
	runTime = endTime - startTime; // 運行
	double readTime = ( ( double ) runTime ) / CLOCKS_PER_SEC; // 時間轉換為秒 
	// 排序
	startTime = clock(); // 開始
	queue.Sort();
	endTime = clock(); // 結束
	runTime = endTime - startTime; // 計算排序時間 
	double sortTime = ( ( double ) runTime ) / CLOCKS_PER_SEC; // 時間轉換為秒
	// 寫入
	sprintf( fileName , "sorted%d.txt" , fileIndex );

	startTime = clock(); // 開始
	FILE * output = fopen( fileName , "w" );

	fprintf( output , "OID\tArrival\tDuration\tTimeOut\n" );
	count = queue.getCount();
	for( i = 0 ; i < count ; i++ )
	{
		fprintf( output , "%s\n" , queue[i]->Output( str ) );
	}
	endTime = clock(); // 結束
	runTime = endTime - startTime; // 運行
	double writingTime = ( ( double ) runTime ) / CLOCKS_PER_SEC; // 運行(double


	fclose( output );

	printf( "Reading data: %d clocks (%0.2lf ms).\n" , ( int ) readTime , readTime );
	printf( "Sorting data: %d clocks (%0.2lf ms).\n" , ( int ) sortTime , sortTime );
	printf( "Writing data: %d clocks (%0.2lf ms).\n" , ( int ) writingTime , writingTime );
	printf( "\n" );

	printf( "See %s\n" , fileName );

	startTime = clock(); // 開始
	endTime = clock(); // 結束
}

// 單 cpu 排序
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
        // 清空
        queue.Clear();
        // 第一行不要
        fgets( str , sizeof( str ) , input );
        // 開始，讀到結束為止
        while( 1 )
        {
            str[0] = 0;
            fgets( str , sizeof( str ) , input );
            if( str[0] == 0 )break;
            if( sscanf( str , "%d\t%d\t%d\t%d" , &oid , &arrival , &duration , &timeOut ) < 4 )
                break;

            queue.Add( oid , arrival , duration , timeOut );
        }
        // 結束
        fclose( input );
    }

	const MData * runPData = NULL;
	MData * ppRunning = NULL ;
	Queue readyQueue; // 序列
	Queue abortList ;// 輸出用 
	Queue doneList ;// 完成工作

	for( runTime = 0 ;; ++runTime )
	{
		// 工作結束
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
		// 都沒有了
		if( queueIndex >= queue.getCount() )
		{
			if( runPData == NULL )
				if( readyQueue.getCount() == 0 )
					break ;
		}
		// 如果沒有，就取出一個工作
		while( runPData == NULL )
		{
			if( readyQueue.getCount() == 0 )
				break ;
			ppRunning = readyQueue.Push();
			ppRunning->Departure = runTime ;
			// 執行時間 timeOut
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

		// 加入工作
		for( i = queueIndex ; i < queue.getCount() ; ++i )
		{
			const MData * nowData = queue[i];
			if( nowData->Arrival > runTime )
				break;
			queueIndex = i + 1 ;
			// 加入工作
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

	// 存檔
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
