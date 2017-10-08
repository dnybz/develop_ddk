//
//========================================================================
//
#ifndef		__TYPEINFO__H__
#define		__TYPEINFO__H__
//
//========================================================================
//
typedef unsigned long	ULONG_PTR ;
typedef ULONG_PTR		RTSPINLOCK ;
typedef RTSPINLOCK		*PRTSPINLOCK ;
typedef unsigned char	UCHAR ;
typedef UCHAR			RTOLDIRQL ;
typedef RTOLDIRQL		*PRTOLDIRQL ;

typedef unsigned long	DWORD ;
typedef unsigned short	WORD ;
typedef unsigned long * PDWORD ;
typedef unsigned short * PWORD ;

typedef void * PVOID;
typedef unsigned char   BYTE;
typedef BYTE * PBYTE ;

#define	RTCALL		__stdcall

#define	MAX_PATH	256
//
//========================================================================
//
#if DBG
#define DebugPrint(arg)		DbgPrint arg
#else
#define DebugPrint(arg) 
#endif

#if DBG
#define TestForDebug( Message )		\
{									\
	DbgPrint(Message) ;				\
	DbgBreakPoint() ;				\
}
#else
#define TestForDebug( Message )
#endif
//
//========================================================================
//
#endif //	__TYPEINFO__H__
//
//========================================================================
//