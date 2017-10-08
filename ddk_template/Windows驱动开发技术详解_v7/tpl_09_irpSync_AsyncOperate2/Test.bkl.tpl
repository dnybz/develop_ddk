<?xml version="1.0" ?>
<!-- $Id: bakefile_quickstart.txt,v 1.5 2006/02/11 18:41:11 KO Exp $ -->
<makefile>
	<exe id = "Test">



	<cxxflags> /nologo </cxxflags>
	<cxxflags> /W3 </cxxflags>
	<cxxflags> /Gm </cxxflags>
	<cxxflags> /GX </cxxflags>
	<cxxflags> /ZI </cxxflags>
	<cxxflags> /Od </cxxflags>
	<cxxflags> /D "WIN32" </cxxflags>
	<cxxflags> /D "_DEBUG" </cxxflags>
	<cxxflags> /D "_CONSOLE" </cxxflags>
	<cxxflags> /D "_MBCS" </cxxflags>
	<cxxflags> /YX </cxxflags>
	<cxxflags> /FD </cxxflags>
	<cxxflags> /GZ </cxxflags>
	<cxxflags> /c </cxxflags>
	<cxxflags> /nologo </cxxflags>
	<cxxflags> /MDd </cxxflags>
	<cxxflags> /W3 </cxxflags>
	<cxxflags> /Gm </cxxflags>
	<cxxflags> /GX </cxxflags>
	<cxxflags> /ZI </cxxflags>
	<cxxflags> /Od </cxxflags>
	<cxxflags> /D "WIN32" </cxxflags>
	<cxxflags> /D "_DEBUG" </cxxflags>
	<cxxflags> /D "_CONSOLE" </cxxflags>
	<cxxflags> /D "_MBCS" </cxxflags>
	<cxxflags> /YX </cxxflags>
	<cxxflags> /FD </cxxflags>
	<cxxflags> /GZ </cxxflags>
	<cxxflags> /c </cxxflags>

	<sys-lib> kernel32 </sys-lib>
	<sys-lib> user32 </sys-lib>
	<sys-lib> gdi32 </sys-lib>
	<sys-lib> winspool </sys-lib>
	<sys-lib> comdlg32 </sys-lib>
	<sys-lib> advapi32 </sys-lib>
	<sys-lib> shell32 </sys-lib>
	<sys-lib> ole32 </sys-lib>
	<sys-lib> oleaut32 </sys-lib>
	<sys-lib> uuid </sys-lib>
	<sys-lib> odbc32 </sys-lib>
	<sys-lib> odbccp32 </sys-lib>
	<sys-lib> kernel32 </sys-lib>
	<sys-lib> user32 </sys-lib>
	<sys-lib> gdi32 </sys-lib>
	<sys-lib> winspool </sys-lib>
	<sys-lib> comdlg32 </sys-lib>
	<sys-lib> advapi32 </sys-lib>
	<sys-lib> shell32 </sys-lib>
	<sys-lib> ole32 </sys-lib>
	<sys-lib> oleaut32 </sys-lib>
	<sys-lib> uuid </sys-lib>
	<sys-lib> odbc32 </sys-lib>
	<sys-lib> odbccp32 </sys-lib>
	<ldflags> /nologo </ldflags>
	<ldflags> /subsystem:console </ldflags>
	<ldflags> /debug </ldflags>
	<ldflags> /machine:I386 </ldflags>
	<ldflags> /pdbtype:sept </ldflags>

	<sources>$(fileList('*.cpp'))</sources>
	</exe>
</makefile>
