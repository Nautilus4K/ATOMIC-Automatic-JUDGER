echo "Compiling PANEL RESOURCES...";
windres -i panel.rc -o $env:TEMP\panel_res.o;
rcc -name qres -o $env:TEMP\qres.cpp qres.qrc;

echo "Compiling PANEL SOURCE FILES...";
echo "[*] qres.cpp -> qres.o";
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\qres.cpp -o $env:TEMP\qres.o;

echo "[*] CST_RadioButtonDialog.cpp -> CST_RadioButtonDialog.o";
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_RadioButtonDialog.cpp -o $env:TEMP\CST_RadioButtonDialog.o
echo "[*] CST_RadioButtonDialog.h -> moc_CST_RadioButtonDialog.cpp"
moc CST_RadioButtonDialog.h -o $env:TEMP\moc_CST_RadioButtonDialog.cpp
echo "[*] moc_CST_RadioButtonDialog.cpp -> moc_CST_RadioButtonDialog.o";
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_RadioButtonDialog.cpp -o $env:TEMP\moc_CST_RadioButtonDialog.o

echo "[*] CST_TextEditorDialog.cpp -> CST_TextEditorDialog.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_TextEditorDialog.cpp -o $env:TEMP\CST_TextEditorDialog.o
echo "[*] CST_TextEditorDialog.h -> moc_CST_TextEditorDialog.cpp"
moc CST_TextEditorDialog.h -o $env:TEMP\moc_CST_TextEditorDialog.cpp
echo "[*] moc_CST_TextEditorDialog.cpp -> moc_CST_TextEditorDialog.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_TextEditorDialog.cpp -o $env:TEMP\moc_CST_TextEditorDialog.o

echo "[*] CST_RichTextEdit.cpp -> CST_RichTextEdit.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_RichTextEdit.cpp -o $env:TEMP\CST_RichTextEdit.o
echo "[*] CST_RichTextEdit.h -> moc_CST_RichTextEdit.cpp"
moc CST_RichTextEdit.h -o $env:TEMP\moc_CST_RichTextEdit.cpp
echo "[*] moc_CST_RichTextEdit.cpp -> moc_CST_RichTextEdit.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_RichTextEdit.cpp -o $env:TEMP\moc_CST_RichTextEdit.o

echo "[*] CST_Listing.cpp -> CST_Listing.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c CST_Listing.cpp -o $env:TEMP\CST_Listing.o
echo "[*] CST_Listing.h -> moc_CST_Listing.cpp"
moc CST_Listing.h -o $env:TEMP\moc_CST_Listing.cpp
echo "[*] moc_CST_Listing.cpp -> moc_CST_Listing.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_CST_Listing.cpp -o $env:TEMP\moc_CST_Listing.o

echo "[*] WIN_ContestsSettings.cpp -> WIN_ContestsSettings.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c WIN_ContestsSettings.cpp -o $env:TEMP\WIN_ContestsSettings.o
echo "[*] WIN_ContestsSettings.h -> moc_WIN_ContestsSettings.cpp"
moc WIN_ContestsSettings.h -o $env:TEMP\moc_WIN_ContestsSettings.cpp
echo "[*] moc_CST_Listing.cpp -> moc_CST_Listing.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c $env:TEMP\moc_WIN_ContestsSettings.cpp -o $env:TEMP\moc_WIN_ContestsSettings.o

echo "[*] panel.cpp -> panel.o"
g++ -fdiagnostics-color=always -I. -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c panel.cpp -o $env:TEMP\panel.o;

echo "Linking PANEL";
g++ $env:TEMP\panel.o $env:TEMP\WIN_ContestsSettings.o $env:TEMP\moc_WIN_ContestsSettings.o $env:TEMP\CST_Listing.o $env:TEMP\moc_CST_Listing.o $env:TEMP\CST_RichTextEdit.o $env:TEMP\moc_CST_RichTextEdit.o $env:TEMP\CST_RadioButtonDialog.o $env:TEMP\moc_CST_RadioButtonDialog.o $env:TEMP\CST_TextEditorDialog.o $env:TEMP\moc_CST_TextEditorDialog.o $env:TEMP\panel_res.o $env:TEMP\qres.o -L"C:\Qt\6.5.3\mingw_64\lib" -L"C:\Qt\Tools\mingw1120_64\lib" -lQt6Widgets -lQt6Core -lQt6Gui -lws2_32 -g -o panel.exe -mwindows;

echo "Cleaning...";
Remove-Item -Path $env:TEMP\qres.cpp
Remove-Item -Path $env:TEMP\qres.o
Remove-Item -Path $env:TEMP\panel_res.o
Remove-Item -Path $env:TEMP\CST_RadioButtonDialog.o
Remove-Item -Path $env:TEMP\moc_CST_RadioButtonDialog.o
Remove-Item -Path $env:TEMP\CST_TextEditorDialog.o
Remove-Item -Path $env:TEMP\moc_CST_TextEditorDialog.o
Remove-Item -Path $env:TEMP\CST_RichTextEdit.o
Remove-Item -Path $env:TEMP\moc_CST_RichTextEdit.o
Remove-Item -Path $env:TEMP\CST_Listing.o
Remove-Item -Path $env:TEMP\moc_CST_Listing.o
Remove-Item -Path $env:TEMP\WIN_ContestsSettings.o
Remove-Item -Path $env:TEMP\moc_WIN_ContestsSettings.o
Remove-Item -Path $env:TEMP\panel.o

echo "Executing...";
.\panel.exe --console;
