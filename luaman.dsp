# Microsoft Developer Studio Project File - Name="luaman" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=luaman - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "luaman.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "luaman.mak" CFG="luaman - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "luaman - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "luaman - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "luaman - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "build\Release"
# PROP Intermediate_Dir "build\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\\" /I "frame\include" /I "cpy021\include\\" /I "cpy195\include\\" /I "cpy1853\include\\" /I "cpy000\include" /I "cpySignin\include" /I "cpySakura\include" /I "cpyDotask\include" /I "cpyfight\include" /I "cpySingle\include" /I "cpyPirate\include" /I "cpy175\include" /I "cpyFire\include" /I "cpysnapshot\include" /I "include\curl" /I "cpysimplefight\include" /I "cpyrefugeboat\include" /I "cpyexsignle\include" /I "cpyExPirate\include" /I "cpyEx175\include" /I "cpyTeamGroup\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "CURL_STATICLIB" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Ws2_32.lib libcurl.lib /nologo /subsystem:windows /machine:I386 /libpath:".\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=del                                     /q                                     /s                                     product\luaman.exe                                    	copy                                     /y                                     build\Release\luaman.exe                                     product\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "luaman - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "luaman - Win32 Release"
# Name "luaman - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "cpy021_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy021\source\snail_client_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\cpy021\source\snail_nettime.cpp
# End Source File
# Begin Source File

SOURCE=.\cpy021\source\task021.cpp
# End Source File
# End Group
# Begin Group "cpy195_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy195\source\task195.cpp
# End Source File
# Begin Source File

SOURCE=.\cpy195\source\task195dllext.cpp
# End Source File
# Begin Source File

SOURCE=.\cpy195\source\task195manager.cpp
# End Source File
# End Group
# Begin Group "frame_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\frame\source\alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\config.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\dm.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\keyboard_monitor.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\luaman.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\luamanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\param.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\param000Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\Param021Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\Param175Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\Param1853Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\Param195Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ParamEx175Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\paramexpiratedlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ParamExsignleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ParamFightDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ParamFireDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ParamPirateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ParamRefugeboatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ParamSigninDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ParamSignleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ParamTeamGroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\task.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\ThreadCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\xlist.cpp
# End Source File
# Begin Source File

SOURCE=.\frame\source\xlog.cpp
# End Source File
# End Group
# Begin Group "cpy1853_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy1853\source\task1853.cpp
# End Source File
# Begin Source File

SOURCE=.\cpy1853\source\task1853_client_manager.cpp
# End Source File
# End Group
# Begin Group "cpy000_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy000\source\task000.cpp
# End Source File
# End Group
# Begin Group "cpySignin_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpysignin\source\taskSignin.cpp
# End Source File
# End Group
# Begin Group "cpysakura_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\frame\source\paramsakuradlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cpysakura\source\taskSakura.cpp
# End Source File
# End Group
# Begin Group "cpyDotask_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\frame\source\ParamDotaskDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cpydotask\source\taskDotask.cpp
# End Source File
# End Group
# Begin Group "cpyFight_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyfight\source\taskFight.cpp
# End Source File
# End Group
# Begin Group "cpySingle_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpysingle\source\taskSignle.cpp
# End Source File
# Begin Source File

SOURCE=.\cpysingle\source\taskSignleCpy1.cpp
# End Source File
# End Group
# Begin Group "cpyPirate_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpypirate\source\taskPirate.cpp
# End Source File
# End Group
# Begin Group "cpy175_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy175\source\task175.cpp
# End Source File
# End Group
# Begin Group "cpyFire_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyfire\source\taskFire.cpp
# End Source File
# End Group
# Begin Group "cpysnapshot_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpysnapshot\source\tasksnapshot.cpp
# End Source File
# End Group
# Begin Group "cpySimpleFight_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpysimplefight\source\tasksimplefight.cpp
# End Source File
# End Group
# Begin Group "cpyRefugeboat_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyrefugeboat\source\taskrefugeboat.cpp
# End Source File
# End Group
# Begin Group "cpyExSignle_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyexsignle\source\taskExSignle.cpp
# End Source File
# Begin Source File

SOURCE=.\cpyexsignle\source\taskExSignle_dowork.cpp
# End Source File
# Begin Source File

SOURCE=.\cpyexsignle\source\taskSignleCpy1Pri.cpp
# End Source File
# Begin Source File

SOURCE=.\cpyexsignle\source\taskSignlePri.cpp
# End Source File
# End Group
# Begin Group "cpyExPirate_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyExPirate\source\taskExPirate.cpp
# End Source File
# End Group
# Begin Group "cpyEx175_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyex175\source\taskex175.cpp
# End Source File
# End Group
# Begin Group "cpyTeamGroup_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyTeamGroup\source\taskTeamGroup.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\luaman.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "cpy021_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy021\include\snail_client_context.h
# End Source File
# Begin Source File

SOURCE=.\cpy021\include\snail_client_manager.h
# End Source File
# Begin Source File

SOURCE=.\cpy021\include\snail_common.h
# End Source File
# Begin Source File

SOURCE=.\cpy021\include\snail_datatype.h
# End Source File
# Begin Source File

SOURCE=.\cpy021\include\snail_errno.h
# End Source File
# Begin Source File

SOURCE=.\cpy021\include\snail_log.h
# End Source File
# Begin Source File

SOURCE=.\cpy021\include\snail_nettime.h
# End Source File
# Begin Source File

SOURCE=.\cpy021\include\task021.h
# End Source File
# End Group
# Begin Group "cpy195_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy195\include\task195.h
# End Source File
# Begin Source File

SOURCE=.\cpy195\include\task195dllext.h
# End Source File
# Begin Source File

SOURCE=.\cpy195\include\task195manager.h
# End Source File
# End Group
# Begin Group "frame_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\frame\include\alarm.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\config.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\datatype.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\dm.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\keyboard_monitor.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\luaman.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\luamanDlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\Markup.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\param.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\param000Dlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\Param021Dlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\Param175Dlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\Param1853Dlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\Param195Dlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\paramex175dlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\paramexpiratedlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\ParamExsignleDlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\ParamFightDlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\ParamFireDlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\ParamPirateDlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\ParamRefugeboatDlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\ParamSigninDialog.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\ParamSignleDlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\ParamTeamGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\task.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\ThreadCtrl.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\xcommon.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\xlist.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\xlog.h
# End Source File
# Begin Source File

SOURCE=.\frame\include\xrrno.h
# End Source File
# End Group
# Begin Group "cpy1853_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy1853\include\task1853.h
# End Source File
# Begin Source File

SOURCE=.\cpy1853\include\task1853_client_context.h
# End Source File
# Begin Source File

SOURCE=.\cpy1853\include\task1853_client_manager.h
# End Source File
# Begin Source File

SOURCE=.\cpy1853\include\task1853_common.h
# End Source File
# Begin Source File

SOURCE=.\cpy1853\include\task1853_errno.h
# End Source File
# End Group
# Begin Group "cpy000_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy000\include\task000.h
# End Source File
# End Group
# Begin Group "cpySignin_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpysignin\include\taskSignin.h
# End Source File
# End Group
# Begin Group "cpysakura_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\frame\include\paramsakuradlg.h
# End Source File
# Begin Source File

SOURCE=.\cpysakura\include\taskSakura.h
# End Source File
# End Group
# Begin Group "cpyDotask_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\frame\include\ParamDotaskDlg.h
# End Source File
# Begin Source File

SOURCE=.\cpydotask\include\taskDotask.h
# End Source File
# End Group
# Begin Group "cpyFight_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyfight\include\taskFight.h
# End Source File
# End Group
# Begin Group "cpySingle_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpysingle\include\taskSignle.h
# End Source File
# Begin Source File

SOURCE=.\cpysingle\include\taskSignleCpy1.h
# End Source File
# End Group
# Begin Group "cpyPirate_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpypirate\include\taskPirate.h
# End Source File
# End Group
# Begin Group "cpy175_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpy175\include\task175.h
# End Source File
# End Group
# Begin Group "cpyFire_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyfire\include\taskFire.h
# End Source File
# End Group
# Begin Group "cpysnapshot_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpysnapshot\include\tasksnapshot.h
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Group "curl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\curl\curl.h
# End Source File
# Begin Source File

SOURCE=.\include\curl\curlbuild.h
# End Source File
# Begin Source File

SOURCE=.\include\curl\curlrules.h
# End Source File
# Begin Source File

SOURCE=.\include\curl\curlver.h
# End Source File
# Begin Source File

SOURCE=.\include\curl\easy.h
# End Source File
# Begin Source File

SOURCE=.\include\curl\mprintf.h
# End Source File
# Begin Source File

SOURCE=.\include\curl\multi.h
# End Source File
# Begin Source File

SOURCE=.\include\curl\stdcheaders.h
# End Source File
# Begin Source File

SOURCE=".\include\curl\typecheck-gcc.h"
# End Source File
# End Group
# End Group
# Begin Group "cpySimpleFight_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpysimplefight\include\tasksimplefight.h
# End Source File
# End Group
# Begin Group "cpyRefugeboat_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyrefugeboat\include\taskrefugeboat.h
# End Source File
# End Group
# Begin Group "cpyExSignle_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyexsignle\include\taskExSignle.h
# End Source File
# Begin Source File

SOURCE=.\cpyexsignle\include\taskExSignle_Pub.h
# End Source File
# Begin Source File

SOURCE=.\cpyexsignle\include\taskSignleCpy1Pri.h
# End Source File
# Begin Source File

SOURCE=.\cpyexsignle\include\taskSignlePri.h
# End Source File
# End Group
# Begin Group "cpyExPirate_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyExPirate\include\taskExPirate.h
# End Source File
# End Group
# Begin Group "cpyEx175_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyex175\include\taskex175.h
# End Source File
# End Group
# Begin Group "cpyTeamGroup_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cpyTeamGroup\include\taskTeamGroup.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\luaman.ico
# End Source File
# Begin Source File

SOURCE=.\res\luaman.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
