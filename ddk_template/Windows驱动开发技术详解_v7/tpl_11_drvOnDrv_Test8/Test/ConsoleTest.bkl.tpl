<?xml version="1.0" ?>
<!-- $Id: bakefile_quickstart.txt,v 1.5 2006/02/11 18:41:11 KO Exp $ -->
<makefile>
	<exe id = "ConsoleTest">



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
	<cxxflags> /W3 </cxxflags>
	<cxxflags> /Gm </cxxflags>
	<cxxflags> /GR </cxxflags>
	<cxxflags> /GX </cxxflags>
	<cxxflags> /ZI </cxxflags>
	<cxxflags> /Od </cxxflags>
	<cxxflags> /D "WIN32" </cxxflags>
	<cxxflags> /D "_DEBUG" </cxxflags>
	<cxxflags> /D "_CONSOLE" </cxxflags>
	<cxxflags> /D "_MBCS" </cxxflags>
	<cxxflags> /FA </cxxflags>
	<cxxflags> /YX </cxxflags>
	<cxxflags> /FD </cxxflags>
	<cxxflags> /GZ </cxxflags>
	<cxxflags> /c </cxxflags>

	<sys-lib> Strmiids </sys-lib>
	<sys-lib> ole32 </sys-lib>
	<sys-lib> User32 </sys-lib>
	<sys-lib> setupapi </sys-lib>
	<sys-lib> Advapi32 </sys-lib>
	<ldflags> /nologo </ldflags>
	<ldflags> /stack:0x300000 </ldflags>
	<ldflags> /subsystem:console </ldflags>
	<ldflags> /map </ldflags>
	<ldflags> /debug </ldflags>
	<ldflags> /machine:I386 </ldflags>
	<ldflags> /pdbtype:sept </ldflags>

	<sources>$(fileList('*.cpp'))</sources>
	</exe>
</makefile>
