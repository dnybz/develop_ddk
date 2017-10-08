//
//========================================================================
//
#define		UNICODE
#define		_UNICODE

#include	<windows.h>
#include <tchar.h>
#include	<stdio.h>
#include <conio.h>


DWORD __stdcall MyDeleteFile( 
							IN LPCTSTR lpFilePathName )
{
	DWORD	dwStatus = 0 ;
	if( !DeleteFile( lpFilePathName ))
	{
		dwStatus = GetLastError() ;
		if( ERROR_ACCESS_DENIED == dwStatus )
		{
			DWORD dwFileAttributes = GetFileAttributes( lpFilePathName ) ;
			dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY ;
			SetFileAttributes( lpFilePathName , dwFileAttributes ) ;

			dwStatus = 0 ;

			if( !DeleteFile( lpFilePathName ))
			{
				dwStatus = GetLastError() ;
			}
		}
	}

	return dwStatus ;
}

DWORD __stdcall MyRemoveDirectory( 
								  IN LPCTSTR lpszDirPath )
{
	DWORD	dwStatus = 0 ;
	if( !RemoveDirectory( lpszDirPath ))
	{
		dwStatus = GetLastError() ;
		if( ERROR_ACCESS_DENIED == dwStatus )
		{
			DWORD dwFileAttributes = GetFileAttributes( lpszDirPath ) ;
			dwFileAttributes &= ~FILE_ATTRIBUTE_READONLY ;
			SetFileAttributes( lpszDirPath , dwFileAttributes ) ;

			dwStatus = 0 ;

			if( !RemoveDirectory( lpszDirPath ))
			{
				dwStatus = GetLastError() ;
			}
		}
	}

	return dwStatus ;	
}

__inline BOOL __stdcall IsDots( LPCTSTR lpszFileName )
{
	BOOL	bResult = FALSE ;
	if( lpszFileName[0] == _T('.'))
	{
		if( lpszFileName[1] == _T('\0') ||
			( lpszFileName[1] == _T('.') &&
			lpszFileName[2] == _T('\0')))
		{
			bResult = TRUE;
		}
	}
	return bResult ;
}

BOOL __stdcall DXPE_DeleteDirTree(					 
						LPCTSTR	lpszScanDirPath )
{	
	DWORD				dwStatus = 0 ;
	WIN32_FIND_DATA		FindData;
	HANDLE				hFind = NULL ;
	BOOL				bFinished = FALSE;	

	int					iLen = 0 ;
	TCHAR				szFullPathName[MAX_PATH] ;
	
	_tcscpy( szFullPathName , lpszScanDirPath ) ;
	_tcscat( szFullPathName , _T( "\\*.*" )) ;
	
	ZeroMemory( &FindData,sizeof(FindData)) ;
	
	hFind = FindFirstFile( szFullPathName,&FindData ) ;
	 
	if( INVALID_HANDLE_VALUE == hFind ) 
	{
		return GetLastError() ;
	}
	
	while( !bFinished )
	{
		if( !IsDots( FindData.cFileName ))
		{			
			_stprintf( szFullPathName , _T("%s\\%s") , lpszScanDirPath , FindData.cFileName ) ;

			if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				dwStatus = DXPE_DeleteDirTree( szFullPathName ) ;
			}
			else
			{				
				dwStatus = MyDeleteFile( szFullPathName ) ;
			}
		}
		
		if( !FindNextFile( hFind , &FindData ))
		{
			if(ERROR_NO_MORE_FILES == GetLastError())
			{				
				bFinished = TRUE;
			}
		}
	}

	FindClose( hFind ) ;

	if( _tcslen( lpszScanDirPath ) > 3 )
	{
		MyRemoveDirectory( lpszScanDirPath ) ;
	}

	return 0 ;
}

DWORD __stdcall SUB_RemoveWin7RecycleBin( 
								IN WCHAR wRichDrive )
{
	DWORD	dwStatus = 0 ;
	WCHAR	wszRecycleBinPath[] = L"C:\\$Recycle.Bin" ;

	wszRecycleBinPath[0] = wRichDrive ;

	dwStatus = DXPE_DeleteDirTree( wszRecycleBinPath ) ;

	return dwStatus ;
}
//
//========================================================================
//
int main( int argc , char * argv[] )
{
	DWORD	dwVDiskMask = 0 ;

	DWORD	dwStatus = SUB_RemoveWin7RecycleBin( L'E' ) ;
	printf( "Result: %d\n" , dwStatus ) ;

	_getch() ;

	return 0 ;
}