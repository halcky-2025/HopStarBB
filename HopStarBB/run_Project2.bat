@echo off
rem === Project2.exe 起動用バッチ ===
rem VS が裏でやっている DLL パス設定を再現する。
rem 直クリック / cmd から動かすときに「DLL が見つからない」を回避。

setlocal

rem --- 依存 DLL の在処を PATH に追加 ---
set "PATH=C:\C2;%PATH%"
set "PATH=C:\Users\hayah\Downloads\libtorch-win-shared-with-deps-debug-2.6.0+cu126\libtorch\lib;%PATH%"
set "PATH=C:\Users\hayah\Downloads\clang+llvm-21.1.5-x86_64-pc-windows-msvc\clang+llvm-21.1.5-x86_64-pc-windows-msvc\bin;%PATH%"
set "PATH=C:\Users\hayah\AppData\Local\Programs\Python\Python310;%PATH%"
set "PATH=%~dp0;%PATH%"

rem --- ビルド出力のディレクトリへ移動して起動 ---
rem (working directory は Project2/Project2 = main.cpp と同じ場所)
cd /d "%~dp0"
"%~dp0..\x64\Debug\Project2.exe" %*

endlocal
