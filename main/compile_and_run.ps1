echo "2025 Nautilus4K"
echo "Project: ATOMIC-Automatic-JUDGER"
echo "Build with: MINGWW64, Qt"
echo "------------------------------"
echo ""

echo "Compiling PANEL RESOURCES...";
echo "> panel.rc -> panel_res.o"
windres -i panel.rc -o $env:TEMP\panel_res.o;
echo "> qres.qrc -> qres.cpp"
rcc -name qres -o $env:TEMP\qres.cpp qres.qrc;

echo "Compiling PANEL SOURCE FILES...";
echo "> qres.cpp -> qres.o";
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\qres.cpp -o $env:TEMP\qres.o;

echo "> CST_RadioButtonDialog.cpp -> CST_RadioButtonDialog.o";
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_RadioButtonDialog.cpp -o $env:TEMP\CST_RadioButtonDialog.o
echo "> CST_RadioButtonDialog.h -> moc_CST_RadioButtonDialog.cpp"
moc CST_RadioButtonDialog.h -o $env:TEMP\moc_CST_RadioButtonDialog.cpp
echo "> moc_CST_RadioButtonDialog.cpp -> moc_CST_RadioButtonDialog.o";
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_RadioButtonDialog.cpp -o $env:TEMP\moc_CST_RadioButtonDialog.o

echo "> CST_TextEditorDialog.cpp -> CST_TextEditorDialog.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_TextEditorDialog.cpp -o $env:TEMP\CST_TextEditorDialog.o
echo "> CST_TextEditorDialog.h -> moc_CST_TextEditorDialog.cpp"
moc CST_TextEditorDialog.h -o $env:TEMP\moc_CST_TextEditorDialog.cpp
echo "> moc_CST_TextEditorDialog.cpp -> moc_CST_TextEditorDialog.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_TextEditorDialog.cpp -o $env:TEMP\moc_CST_TextEditorDialog.o

echo "> CST_RichTextEdit.cpp -> CST_RichTextEdit.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_RichTextEdit.cpp -o $env:TEMP\CST_RichTextEdit.o
echo "> CST_RichTextEdit.h -> moc_CST_RichTextEdit.cpp"
moc CST_RichTextEdit.h -o $env:TEMP\moc_CST_RichTextEdit.cpp
echo "> moc_CST_RichTextEdit.cpp -> moc_CST_RichTextEdit.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_RichTextEdit.cpp -o $env:TEMP\moc_CST_RichTextEdit.o

echo "> CST_Listing.cpp -> CST_Listing.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_Listing.cpp -o $env:TEMP\CST_Listing.o
echo "> CST_Listing.h -> moc_CST_Listing.cpp"
moc CST_Listing.h -o $env:TEMP\moc_CST_Listing.cpp
echo "> moc_CST_Listing.cpp -> moc_CST_Listing.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_Listing.cpp -o $env:TEMP\moc_CST_Listing.o

echo "> CST_TestCaseDialog.cpp -> CST_TestCaseDialog.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_TestCaseDialog.cpp -o $env:TEMP\CST_TestCaseDialog.o
echo "> CST_TestCaseDialog.h -> moc_CST_TestCaseDialog.cpp"
moc CST_TestCaseDialog.h -o $env:TEMP\moc_CST_TestCaseDialog.cpp
echo "> moc_CST_TestCaseDialog.cpp -> moc_CST_TestCaseDialog.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_TestCaseDialog.cpp -o $env:TEMP\moc_CST_TestCaseDialog.o

echo "> CST_PlainTextDialog.cpp -> CST_PlainTextDialog.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_PlainTextDialog.cpp -o $env:TEMP\CST_PlainTextDialog.o
echo "> CST_PlainTextDialog.h -> moc_CST_PlainTextDialog.cpp"
moc CST_PlainTextDialog.h -o $env:TEMP\moc_CST_PlainTextDialog.cpp
echo "> moc_CST_PlainTextDialog.cpp -> moc_CST_PlainTextDialog.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_PlainTextDialog.cpp -o $env:TEMP\moc_CST_PlainTextDialog.o

echo "> WIN_ContestsSettings.cpp -> WIN_ContestsSettings.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c WIN_ContestsSettings.cpp -o $env:TEMP\WIN_ContestsSettings.o
echo "> WIN_ContestsSettings.h -> moc_WIN_ContestsSettings.cpp"
moc WIN_ContestsSettings.h -o $env:TEMP\moc_WIN_ContestsSettings.cpp
echo "> moc_WIN_ContestsSettings.cpp -> moc_WIN_ContestsSettings.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_WIN_ContestsSettings.cpp -o $env:TEMP\moc_WIN_ContestsSettings.o

echo "> WIN_UsersSettings.cpp -> WIN_UsersSettings.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c WIN_UsersSettings.cpp -o $env:TEMP\WIN_UsersSettings.o
echo "> WIN_UsersSettings.h -> moc_WIN_UsersSettings.cpp"
moc WIN_UsersSettings.h -o $env:TEMP\moc_WIN_UsersSettings.cpp
echo "> moc_WIN_UsersSettings.cpp -> moc_WIN_UsersSettings.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_WIN_UsersSettings.cpp -o $env:TEMP\moc_WIN_UsersSettings.o

echo "> panel.cpp -> panel.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c panel.cpp -o $env:TEMP\panel.o;

echo "> timezone_stub.c -> timezone_stub.o"
gcc -c timezone_stub.c -o $env:TEMP\timezone_stub.o

echo "Linking PANEL";
g++ $env:TEMP\timezone_stub.o $env:TEMP\panel.o $env:TEMP\CST_PlainTextDialog.o $env:TEMP\moc_CST_PlainTextDialog.o $env:TEMP\WIN_UsersSettings.o $env:TEMP\moc_WIN_UsersSettings.o $env:TEMP\CST_TestCaseDialog.o $env:TEMP\moc_CST_TestCaseDialog.o $env:TEMP\WIN_ContestsSettings.o $env:TEMP\moc_WIN_ContestsSettings.o $env:TEMP\CST_Listing.o $env:TEMP\moc_CST_Listing.o $env:TEMP\CST_RichTextEdit.o $env:TEMP\moc_CST_RichTextEdit.o $env:TEMP\CST_RadioButtonDialog.o $env:TEMP\moc_CST_RadioButtonDialog.o $env:TEMP\CST_TextEditorDialog.o $env:TEMP\moc_CST_TextEditorDialog.o $env:TEMP\panel_res.o $env:TEMP\qres.o -L"C:\Qt\6.5.3\mingw_64\lib" -L"C:\Qt\Tools\mingw1120_64\lib" -lQt6Widgets -lQt6Core -lQt6Gui -lssl -lcrypto -lcrypt32 -lgdi32 -luser32 -lws2_32 -lmingwex -lmsvcrt -lmingw32 -O2 -o panel.exe -mwindows;

echo "Cleaning...";
Remove-Item -Path $env:TEMP\qres.cpp
Remove-Item -Path $env:TEMP\qres.o
Remove-Item -Path $env:TEMP\panel_res.o
Remove-Item -Path $env:TEMP\CST_RadioButtonDialog.o
Remove-Item -Path $env:TEMP\moc_CST_RadioButtonDialog.o
Remove-Item -Path $env:TEMP\moc_CST_RadioButtonDialog.cpp
Remove-Item -Path $env:TEMP\CST_TextEditorDialog.o
Remove-Item -Path $env:TEMP\moc_CST_TextEditorDialog.o
Remove-Item -Path $env:TEMP\moc_CST_TextEditorDialog.cpp
Remove-Item -Path $env:TEMP\CST_RichTextEdit.o
Remove-Item -Path $env:TEMP\moc_CST_RichTextEdit.o
Remove-Item -Path $env:TEMP\moc_CST_RichTextEdit.cpp
Remove-Item -Path $env:TEMP\CST_Listing.o
Remove-Item -Path $env:TEMP\moc_CST_Listing.o
Remove-Item -Path $env:TEMP\moc_CST_Listing.cpp
Remove-Item -Path $env:TEMP\WIN_ContestsSettings.o
Remove-Item -Path $env:TEMP\moc_WIN_ContestsSettings.o
Remove-Item -Path $env:TEMP\moc_WIN_ContestsSettings.cpp
Remove-Item -Path $env:TEMP\CST_TestCaseDialog.o
Remove-Item -Path $env:TEMP\moc_CST_TestCaseDialog.o
Remove-Item -Path $env:TEMP\moc_CST_TestCaseDialog.cpp
Remove-Item -Path $env:TEMP\WIN_UsersSettings.o
Remove-Item -Path $env:TEMP\moc_WIN_UsersSettings.cpp
Remove-Item -Path $env:TEMP\moc_WIN_UsersSettings.o
Remove-Item -Path $env:TEMP\CST_PlainTextDialog.o
Remove-Item -Path $env:TEMP\moc_CST_PlainTextDialog.o
Remove-Item -Path $env:TEMP\moc_CST_PlainTextDialog.cpp
Remove-Item -Path $env:TEMP\panel.o

echo "Executing...";
echo "Target: panel.exe"
echo "Debugger: NONE"
echo "Args: ['--console']"
.\panel.exe --console;
