version := ATOMIC_RYVENIA

# Shell
CMDLINE := powershell -Command
ECHO := Write-Host
RM := Remove-Item -ErrorAction SilentlyContinue -Path
MKDIR := mkdir

# Compiler and flags
CXX := /Qt/Tools/mingw1310_64/bin/g++.exe
CXXFLAGS := -fdiagnostics-color=always -Wall -O2 -std=c++17 -fPIC -mwindows
CXXFLAGSDEBUG := -fdiagnostics-color=always -Wall -O0 -g -std=c++17 -fPIC
LDFLAGS  = -L/Qt/Tools/mingw1310_64/lib -L/Qt/6.10.1/mingw_64/lib -L/msys64/mingw64/lib -lxlnt -lQt6Widgets -lQt6Core -lQt6Gui -lssl -lcrypto -lpthread -lws2_32
# INCLUDES := -I./include -I. -I/usr/include/qt6 
INCLUDES := -I./include -I. -I/msys64/mingw64/include -I/Qt/6.10.1/mingw_64/include -I/Qt/Tools/mingw1310_64/include  # MSYS2-based
TARGETFOLDER := ./bin
WINDEPLOYQT := /Qt/6.10.1/mingw_64/bin/windeployqt.exe

# Qt6 tools (stopping system from using whatever the fuck the Qt5 tools are)
# RCC := /usr/lib/qt6/rcc
# MOC := /usr/lib/qt6/moc
# MSYS2
RCC := /Qt/6.10.1/mingw_64/bin/rcc.exe
MOC := /Qt/6.10.1/mingw_64/bin/moc.exe

# Formatting strings
# Define color variables for better readability
# RED := \033[0;31m
# GREEN := \033[0;32m
# BLUE := \033[0;34m
# NC := \033[0m # No Color (reset)
# PS version
RED   := ""
GREEN := ""
BLUE  := ""
NC    := " "

# Project scope
all: $(TARGETFOLDER)/panel.exe

# Header dependencies
utilities.h: consts.h
CST_Listing.h:                  # Looks like these dont have any dependencies (as far as this project is concerned)
CST_PlainTextDialog.h: 
CST_RadioButtonDialog.h: 
CST_RichTextEdit.h: 
CST_Separator.h: 
CST_TestCaseDialog.h: 
CST_TextEditorDialog.h:

WIN_ClassesSettings.h: 
WIN_ContestsSettings.h: CST_RichTextEdit.h CST_Listing.h WIN_GenerateTestCasesDialog.h
WIN_UsersSettings.h: CST_Listing.h 
WIN_GenerateTestCasesDialog.h: WIN_ContestsSettings.h utilities.h

# Object files list
OBJECTS := panel.o timezone_stub.o qres.o \
		   CST_Listing.o moc_CST_Listing.o \
		   CST_PlainTextDialog.o moc_CST_PlainTextDialog.o \
		   CST_RadioButtonDialog.o moc_CST_RadioButtonDialog.o \
		   CST_RichTextEdit.o moc_CST_RichTextEdit.o \
		   CST_Separator.o moc_CST_Separator.o \
		   CST_TestCaseDialog.o moc_CST_TestCaseDialog.o \
		   CST_TextEditorDialog.o moc_CST_TextEditorDialog.o \
		   WIN_ClassesSettings.o moc_WIN_ClassesSettings.o \
		   WIN_ContestsSettings.o moc_WIN_ContestsSettings.o \
		   WIN_UsersSettings.o moc_WIN_UsersSettings.o \
		   WIN_GenerateTestCasesDialog.o moc_WIN_GenerateTestCasesDialog.o

# Main target - link all objects
$(TARGETFOLDER)/panel.exe: $(OBJECTS) $(TARGETFOLDER)
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Linking panel executable\""
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)
	$(WINDEPLOYQT) $@

$(TARGETFOLDER):
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Creating $(TARGETFOLDER) because it doesn't exist\""
	$(CMDLINE) "$(MKDIR) $(TARGETFOLDER)"

# Compile main panel source
panel.o: panel.cpp utilities.h consts.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling panel.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile timezone stub
timezone_stub.o: timezone_stub.c
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling timezone_stub.c\""
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Qt resource file
qres.o: qres.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling qres.cpp\""
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate Qt resource cpp file
qres.cpp: qres.qrc
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating qres.cpp from qres.qrc\""
	$(RCC) -name qres -o $@ $<

# Custom objects. I do know this looks bad. But I if it works I won't complain.
## CST_Listing
CST_Listing.o: CST_Listing.cpp CST_Listing.h CST_PlainTextDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling CST_Listing.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_Listing.o: moc_CST_Listing.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_CST_Listing.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_Listing.cpp: CST_Listing.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for CST_Listing.h\""
	$(MOC) -o $@ $<

## CST_PlainTextDialog
CST_PlainTextDialog.o: CST_PlainTextDialog.cpp CST_PlainTextDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling CST_PlainTextDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_PlainTextDialog.o: moc_CST_PlainTextDialog.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_CST_PlainTextDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_PlainTextDialog.cpp: CST_PlainTextDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for CST_PlainTextDialog.h\""
	$(MOC) -o $@ $<

## CST_RadioButtonDialog
CST_RadioButtonDialog.o: CST_RadioButtonDialog.cpp CST_RadioButtonDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling CST_RadioButtonDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_RadioButtonDialog.o: moc_CST_RadioButtonDialog.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_CST_RadioButtonDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_RadioButtonDialog.cpp: CST_RadioButtonDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for CST_RadioButtonDialog.h\""
	$(MOC) -o $@ $<

## CST_RichTextEdit
CST_RichTextEdit.o: CST_RichTextEdit.cpp CST_RichTextEdit.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling CST_RichTextEdit.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_RichTextEdit.o: moc_CST_RichTextEdit.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_CST_RichTextEdit.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_RichTextEdit.cpp: CST_RichTextEdit.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for CST_RichTextEdit.h\""
	$(MOC) -o $@ $<

## CST_Separator
CST_Separator.o: CST_Separator.cpp CST_Separator.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling CST_Separator.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_Separator.o: moc_CST_Separator.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_CST_Separator.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_Separator.cpp: CST_Separator.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for CST_Separator.h\""
	$(MOC) -o $@ $<

## CST_TestCaseDialog
CST_TestCaseDialog.o: CST_TestCaseDialog.cpp CST_TestCaseDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling CST_TestCaseDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_TestCaseDialog.o: moc_CST_TestCaseDialog.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_CST_TestCaseDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_TestCaseDialog.cpp: CST_TestCaseDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for CST_TestCaseDialog.h\""
	$(MOC) -o $@ $<

## CST_TextEditorDialog
CST_TextEditorDialog.o: CST_TextEditorDialog.cpp CST_TextEditorDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling CST_TextEditorDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_TextEditorDialog.o: moc_CST_TextEditorDialog.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_CST_TextEditorDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_CST_TextEditorDialog.cpp: CST_TextEditorDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for CST_TextEditorDialog.h\""
	$(MOC) -o $@ $<

## WIN_ClassesSettings
WIN_ClassesSettings.o: WIN_ClassesSettings.cpp WIN_ClassesSettings.h CST_PlainTextDialog.h utilities.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling WIN_ClassesSettings.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_WIN_ClassesSettings.o: moc_WIN_ClassesSettings.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_WIN_ClassesSettings.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_WIN_ClassesSettings.cpp: WIN_ClassesSettings.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for WIN_ClassesSettings.h\""
	$(MOC) -o $@ $<

## WIN_ContestsSettings
WIN_ContestsSettings.o: WIN_ContestsSettings.cpp WIN_ContestsSettings.h CST_TestCaseDialog.h CST_PlainTextDialog.h utilities.h consts.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling WIN_ContestsSettings.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_WIN_ContestsSettings.o: moc_WIN_ContestsSettings.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_WIN_ContestsSettings.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_WIN_ContestsSettings.cpp: WIN_ContestsSettings.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for WIN_ContestsSettings.h\""
	$(MOC) -o $@ $<

## WIN_UsersSettings
WIN_UsersSettings.o: WIN_UsersSettings.cpp WIN_UsersSettings.h CST_PlainTextDialog.h consts.h utilities.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling WIN_UsersSettings.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_WIN_UsersSettings.o: moc_WIN_UsersSettings.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_WIN_UsersSettings.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_WIN_UsersSettings.cpp: WIN_UsersSettings.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for WIN_UsersSettings.h\""
	$(MOC) -o $@ $<

## WIN_GenerateTestCasesDialog
WIN_GenerateTestCasesDialog.o: WIN_GenerateTestCasesDialog.cpp WIN_GenerateTestCasesDialog.h utilities.h consts.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling WIN_GenerateTestCasesDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_WIN_GenerateTestCasesDialog.o: moc_WIN_GenerateTestCasesDialog.cpp
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Compiling moc_WIN_GenerateTestCasesDialog.cpp\""
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

moc_WIN_GenerateTestCasesDialog.cpp: WIN_GenerateTestCasesDialog.h
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Generating moc for WIN_GenerateTestCasesDialog.h\""
	$(MOC) -o $@ $<

# Build and run
brun: all
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Executing panel\""
	./bin/$(TARGETFOLDER)/panel.exe

# Clean
clean:
	@$(CMDLINE) "$(ECHO) \"$(BLUE)[$(version)]$(NC)Cleaning up build files\""
	-$(CMDLINE) "$(RM) $(TARGETFOLDER)"
	-$(CMDLINE) "$(RM) *.o"
	-$(CMDLINE) "$(RM) *.obj"
	-$(CMDLINE) "$(RM) moc_*.cpp"
	-$(CMDLINE) "$(RM) qres.cpp"

# Force rebuild
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild