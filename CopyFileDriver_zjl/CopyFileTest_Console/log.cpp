#ifdef WINDOWSCODE
#include "stdafx.h"
#include <windows.h>
#include <io.h>
#include <direct.h>
#else
#endif
#include "log.h"
#include <algorithm>
#include <fstream>
#include <iostream> 
#include <iterator>  
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string> 
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <vector> 

class log* log::m_data = NULL;
FILE *log::m_fp =NULL;
log::log(string filename=string("all.log"))
{
	#ifdef WINDOWSCODE
	int ret = _access("log",0);
	#else
	int ret = access("log",0);
	#endif
	if(ret == -1)
	
	#ifdef WINDOWSCODE
	_mkdir("log");
	#else
	mkdir("log", 0777);
	#endif

	if(m_fp==NULL)
	{
		//m_fp=fopen(filename.c_str(), "a"); 
		//日志文件打开的模式  对文件的大小是否变换起决定性的作用, a方式 一直会增大, 
		#ifdef WINDOWSCODE
		if(_access("all.log", 0) == 0)
		#else
		if(access("all.log", 0) == 0)
		#endif
			save_file(); 

		m_fp=fopen(filename.c_str(), "wb");
		if(m_fp==NULL)
		{
			printf("open file error \n");
			exit(-1);
		}
	}
}
void log::_clear_file()
{
	int number=20;
	char filename[1024]={0};
	
"log/all_100.log";

#ifdef WINDOWSCODE
	_snprintf(filename, sizeof(filename),  "log/all_%d.log", number);
	if(_access(filename, 0) == 0)
	{
		for(int i=0;i<=number; i++)
		{
			_snprintf(filename, sizeof(filename),  "log/all_%d.log", i);
			_unlink(filename);			
		}
	}
#else
	snprintf(filename, sizeof(filename),  "log/all_%d.log", number);
	if( access(filename, 0) == 0)		
	{
		for(int i=0;i<=number; i++)
		{
			snprintf(filename, sizeof(filename),  "log/all_%d.log", i);
			unlink(filename);			
		}
	}
#endif
}
void log::save_file()
{
	char file[64];
	int i = 0;
	_clear_file();
	memset(file, 0, sizeof(file));
	while(1)
	{
		#ifdef WINDOWSCODE
		_snprintf(file, sizeof(file),  "log/all_%d.log", i);
		#else
		snprintf(file, sizeof(file),  "log/all_%d.log", i);
		#endif
		//存在.
		#ifdef WINDOWSCODE
		if(_access(file, 0) == 0)
		#else
		if(access(file, 0) == 0)		
		#endif
		{	
			i++;
			continue;
		}
		else
		{
			rename("all.log", file);
			break;
		}
	}
	return;
}

//fixme 这种处理有问题, 后面的数据 有时打印错误. 
int log::loggerError(char *format, ...)
{
	int ret_len=fprintf(m_fp,"[错误]");
	va_list(marker);
	va_start(marker, format);
	ret_len+= logger(format,marker);
	va_end(marker);
	return ret_len;	
}

//fixme 这种处理有问题, 后面的数据 有时打印错误. 
int log::loggerWarn(char *format, ...)
{
	int ret_len=fprintf(m_fp,"[警告]");
	va_list(marker);
	va_start(marker, format);
	ret_len+= logger(format,marker);
	va_end(marker);
	return ret_len;
}

//fixme 这种处理有问题, 后面的数据 有时打印错误. 
int log::loggerNotice(char *format, ...)
{
	int ret_len=fprintf(m_fp,"[注意]");
	va_list(marker);
	va_start(marker, format);
	ret_len+= logger(format,marker);
	va_end(marker);
	return ret_len;
}

int log::logger( char *format, ...)
{	

	int string_len=0;
	static char tmp[1024];
	memset(tmp,'\0', sizeof(tmp));
	va_list(marker);
	va_start(marker, format);
	
	//vsprintf(tmp, format, marker);
	#ifdef WINDOWSCODE
	_vsnprintf(tmp,	sizeof(tmp), format, marker);
	#else
	vsnprintf(tmp,	sizeof(tmp), format, marker);
	#endif
	va_end(marker);

	char time_str[256]={0};
    time_t t;
    t=time(NULL);
	#ifdef WINDOWSCODE
	string_len = (int)strftime(time_str, sizeof(time_str), "%Y_%m_%d_%H:%M:%S", localtime(&t));
	#else
    string_len = (int)strftime(time_str, sizeof(time_str), "%Y_%m_%d_%T", localtime(&t));
	#endif
	//snprintf(tmp, sizeof(tmp), "%s: %s",  time_str, tmp);

	string_len+=fprintf(m_fp,"%s: %s",  time_str, tmp);
	// int len=strlen(tmp);
	// int ret=write(fileno(m_fp), tmp, len );
	// if(ret != len)
	// {
	// 	printf("写入的长度和 文件长度不一样\n");
	// }
	//
	if(ftell(m_fp)> 1024*1024*1)
	{
	
		fseek(m_fp, 0,SEEK_SET);
		fflush(m_fp);
		if(ftell(m_fp) != 0)
		{
			fprintf(m_fp, "超过1M 文件复位失败, \n");
		}
	}
	return string_len;
}


class log* log::Instance()
{
	if(m_data == NULL)//未被初始化
	{ 
		m_data = new log;
	}
	return m_data;
}
