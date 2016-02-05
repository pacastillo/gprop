
HOME=\MyFiles\progs\eo

ALL : pr-parsing.exe pr-stat.exe pr-ia-1.exe pr-eobin.exe pr-str-1.exe \
	pr-sa-1.exe pr-tourn-w-graph-1.exe pr-tourn-w-stat.exe\
	pr-lottery.exe pr-stat.exe \
	pr-vq-1.exe SGA.exe

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /GR /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS"  /YX /c /I \MyFiles\progs\eo

#CPP_PROJ=/nologo /ML /W3 /GX /GR /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
# "_MBCS"  /YX /c /I \MyFiles\progs\eo

.cpp.obj::
   $(CPP) $(CPP_PROJ) $< 

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib uuid.lib /nologo /subsystem:console /incremental:no\
 /machine:I386 /libpath:$(HOME)\util
LINK32_OBJS= $(HOME)/util/EOParser.obj

#Programs related to tools
pr-parsing.exe :  $(LINK32_OBJS)  pr-parsing.obj
    	$(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-parsing.obj

pr-stat.exe :   pr-stat.obj
    	$(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-stat.obj

#Programs that work with different kinds of EOs
pr-ia-1.exe:   pr-ia-1.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-ia-1.obj  /OUT:pr-ia-1.exe

pr-str-1.exe:   pr-str-1.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-str-1.obj  /OUT:pr-str-1.exe

pr-eobin.exe:   pr-eobin.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-eobin.obj  /OUT:pr-eobin.exe

#Pograms that run whole algorithms
pr-sa-1.exe:  $(LINK32_OBJS)  pr-sa-1.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-sa-1.obj /OUT:pr-sa-1.exe

pr-tourn-w-graph-1.exe:  $(LINK32_OBJS)  pr-tourn-w-graph-1.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-tourn-w-graph-1.obj  /OUT:pr-tourn-w-graph-1.exe

pr-tourn-w-stat.exe:  $(LINK32_OBJS)  pr-tourn-w-stat.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-tourn-w-stat.obj  /OUT:pr-tourn-w-stat.exe

pr-vq-1.exe:  $(LINK32_OBJS)  pr-vq-1.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-vq-1.obj  /OUT:pr-vq-1.exe

pr-lottery.exe:  $(LINK32_OBJS)  pr-lottery.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-lottery.obj /OUT:pr-lottery.exe

pr-sim-ann.exe:  $(LINK32_OBJS)  pr-sim-ann.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  pr-sim-ann.obj /OUT:pr-sim-ann.exe

SGA.exe:  $(LINK32_OBJS)  SGA.obj
	 $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)  SGA.obj /OUT:SGA.exe

$(HOME)\util\EOParser.obj:
	-cd $(HOME)\util ; nmake /f util.mak /d

