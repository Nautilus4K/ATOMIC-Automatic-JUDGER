echo "Cleaning...";
rm .\panel.exe;
rm .\panel.o;
rm .\panel_res.o;
rm .\qres.cpp;
rm .\qres.o;

echo "Compiling PANEL RESOURCES...";
windres -i panel.rc -o panel_res.o;
rcc -name qres -o qres.cpp qres.qrc;

echo "Compiling PANEL SOURCE FILES...";
g++ -fdiagnostics-color=always -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c qres.cpp -o qres.o;
g++ -fdiagnostics-color=always -I"C:\Qt\6.5.3\mingw_64\include" -I"C:\Qt\Tools\mingw1120_64\include" -c panel.cpp -o panel.o;

echo "Linking PANEL";
g++ panel.o panel_res.o qres.o -L"C:\Qt\6.5.3\mingw_64\lib" -L"C:\Qt\Tools\mingw1120_64\lib" -lQt6Widgets -lQt6Core -lQt6Gui -lws2_32 -g -o panel.exe -mwindows;

echo "Executing...";
.\panel.exe --console;
