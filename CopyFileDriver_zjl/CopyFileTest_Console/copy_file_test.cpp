//########################################################################
#include "stdafx.h"

#include <fstream>
#include <string> 
#include <iostream>
#include <map>
#include <iostream> 
#include <algorithm>
#include <vector> 
#include <iterator>  
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <Shlwapi.h>
#include <direct.h>

#include "..\CopyFileWrapper\CopyFileWrapper.h"
#include <string>
#include "log.h"

//---------------------驱动是否加载----------------------------------------
bool check_driver_installed()
{
	BOOL bInstalled=FALSE;
	string strFileName("CopyFileDriver");

	IsDriverInstalled((LPTSTR)strFileName.c_str(),&bInstalled);
	if(bInstalled)
	{
		StartDriver((LPTSTR)strFileName.c_str());
		log::Instance()->logger("已经加载了驱动:%s \n",strFileName.c_str() );
		printf("已经加载了驱动:%s \n",strFileName.c_str());
		return true;
	}
	else
	{
		log::Instance()->logger("还没有加载驱动:%s \n",strFileName.c_str() );
		log::Instance()->logger("运行rich_addvalue3\\tools\\DriverInstall工具加载驱动\n");
		printf("还没有加载驱动:%s \n",strFileName.c_str());
		printf("运行rich_addvalue3\\tools\\DriverInstall工具加载驱动\n");
		return false;
	}
}
//------------------------------------------------------------------------
bool check_is_directory(LPSTR path_in)
{
	return PathIsDirectory(path_in);
}

//------------------创建目录树-------------------------------------------
bool  CreateDirectoryTree(LPSTR DirTreeName)
{
    char                dirtree[256];
    char                tempbuf[256];
    register int        i;
    register int        errorcode;

    strncpy(dirtree,DirTreeName,256);
    errorcode = strlen(dirtree);
    for ( i=0;i<=errorcode-1;i++ )
    {
        if ( i > 0 && (dirtree[i] == '\\' || dirtree[i] == '/') && dirtree[i-1] != '.' && dirtree[i-1] != ':' )
        {
            memcpy(tempbuf,dirtree,i);
            tempbuf[i] = 0;

            if(0 != _mkdir(tempbuf))
			{
				return false;
			}
        }
    }
    if(0 != _mkdir(dirtree))
	{
		return false;
	}

    return true;
}

//--------------------遍历目录所以文件--------------------------------------
int get_dir_allFile(LPSTR dir_path,vector<string > &out)
{
	HANDLE hSearch;
	WIN32_FIND_DATA data; 
	int total = 0;

	SetCurrentDirectory(dir_path);		
    hSearch=FindFirstFile("*",&data); 
	do
	{
		if(data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}
    
		out.push_back(data.cFileName);
		total ++;
	}while(FindNextFile(hSearch,&data)); 
    FindClose(hSearch); 
	return total;	
}

//-------------------执行文件或目录拷贝--------------------------------------------
bool kernel_copy_file(LPSTR src_path,LPSTR dst_path)
{
	vector<string > vec_filename;
	CString   dir_templated= "C:\\WINDOWS\\system32"; 	
	if(true == check_is_directory(src_path))
	{
		log::Instance()->logger("目录拷贝\n");
		if(false == check_is_directory(dst_path))
		{
			if(false == CreateDirectoryTree(dst_path))
			{
				log::Instance()->logger("创建目标目录失败\n");
				printf("创建目标目录失败\n");
				return false;
			}
		}

		int sum_file = get_dir_allFile(src_path,vec_filename);
		if(0 != sum_file)
		{
			vector<string>::iterator it;
			for(it=vec_filename.begin(); it!=vec_filename.end(); it++)
			{	
				char src_full_path[64] = {0};
				char dst_full_path[64] = {0};
				
				strcat(src_full_path,src_path);
				strncat(src_full_path,(*it).c_str(),sizeof(*it));
				printf("src == %s\n",src_full_path);
		
				strcat(dst_full_path,dst_path);
				strncat(dst_full_path,(*it).c_str(),sizeof(*it));
				printf("dst == %s\n",dst_full_path);
				KernelCopyFile(src_full_path, dst_full_path);
			}
			printf("一共拷贝了 %d 个文件\n",sum_file);
		}
		else
		{
			log::Instance()->logger("该目录下没有文件\n");
			printf("该目录下没有文件\n");
			return false;			
		}//sum_file
	}
	else//check_is_directory(src_path)
	{
		log::Instance()->logger("文件拷贝\n");
	 	return KernelCopyFile(src_path, dst_path);
	}	

	return true;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	vector<string > vec_filename;
	if(argc<2||argv[1]==NULL||argv[2]==NULL)
	{
		log::Instance()->logger("拷贝文件，参数不准确\n");
		printf("Usage: %s  [source_file] [dst_file]\n", argv[0] );
		printf("Example: %s  C:\\pagefile.sys D:\\tmp.sys \n",argv[0]);
		printf("Example: %s  C:\\WINDOWS\\system32\\config\\SAM D:\\SAM \n",argv[0]);
		printf("Example: %s  C:\\WINDOWS\\system32\\config\\system D:\\system\n",argv[0]);
		printf("Example: %s  C:\\WINDOWS\\system32\\config\\software D:\\software \n",argv[0]);
		
		printf("Example: %s  C:\\bin\\ D:\\bin\\ \n",argv[0]);

		return -1;
	}

	if(false == check_driver_installed())
	{
		return -1;
	}
	
	bool bOk = kernel_copy_file(argv[1],argv[2]);
	if (!bOk)
	{
		log::Instance()->logger("文件拷贝失败,没有写绝对路径或创建目标路径失败\n");
		printf("文件拷贝失败,没有写绝对路径或创建目标路径失败\n");
		return -1;
	}
	else
	{
		log::Instance()->logger("文件拷贝成功\n");
		printf("文件拷贝成功\n");
		return 0;
	}
}
