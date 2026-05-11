@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
where msbuild
msbuild "C:\Users\hayah\source\repos\Project2\Project2\Project2.vcxproj" /p:Configuration=Debug /p:Platform=x64 /nologo /verbosity:minimal
