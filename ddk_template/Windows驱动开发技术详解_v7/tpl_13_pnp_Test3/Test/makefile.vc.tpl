# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.9 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================



# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# C++ compiler 
CXX = cl

# Standard flags for C++ 
CXXFLAGS = 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS = 

# Standard linker flags 
LDFLAGS = 



# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

### Variables: ###

TEST_CXXFLAGS = /MD /DWIN32 /GR /EHsc /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" \
	/D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D \
	"_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c $(CPPFLAGS) $(CXXFLAGS)
TEST_OBJECTS =  \
	Test_function.obj \
	Test_main.obj

### Conditionally set variables: ###



### Targets: ###

all: Test.exe

clean: 
	-if exist .\*.obj del .\*.obj
	-if exist .\*.res del .\*.res
	-if exist .\*.pch del .\*.pch
	-if exist Test.exe del Test.exe
	-if exist Test.ilk del Test.ilk
	-if exist Test.pdb del Test.pdb

Test.exe: $(TEST_OBJECTS)
	link /NOLOGO /OUT:$@  /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept $(LDFLAGS) @<<
	$(TEST_OBJECTS)   kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Setupapi.lib
<<

Test_function.obj: .\function.cpp
	$(CXX) /c /nologo /TP /Fo$@ $(TEST_CXXFLAGS) .\function.cpp

Test_main.obj: .\main.cpp
	$(CXX) /c /nologo /TP /Fo$@ $(TEST_CXXFLAGS) .\main.cpp

