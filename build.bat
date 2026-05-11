@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >/dev/null 2>&1
MSBuild.exe "C:\Users\hayah\source\repos\Project2\Project2\Project2.vcxproj" /p:Configuration=Debug /p:Platform=x64 /nologo /verbosity:minimal > "C:\Users\hayah\source\repos\Project2\build_output.txt" 2>&1
