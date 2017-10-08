//
//========================================================================
//
#ifndef		__SRVFUNC__H__
#define		__SRVFUNC__H__
//
//========================================================================
//
DWORD __stdcall	RTC_LoadDriver( 
					IN PWCHAR	pwServiceName , 
					IN PWCHAR	pwDriverPath ) ;

DWORD __stdcall	RTC_UnloadDriver( 
					IN PWCHAR	pwServiceName ) ;
//
//========================================================================
//
#endif //	__SRVFUNC__H__
//
//========================================================================
//
