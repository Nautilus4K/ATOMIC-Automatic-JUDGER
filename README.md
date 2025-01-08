# ATOMIC-Automatic-JUDGER
A Vietnamese automatic testing software built on Python. Supports online submission and offline tests.
This software uses Docker for its isolated environment, WSGI callable object and Flask for its web server, and SQLite, JSON for its database.
This software is designed by Vietnamese people, for Vietnamese people. So the UI is all Vietnamese
If you don't understand Vietnamese and want to add English. Feel free to contribute, give a pull request, I will add in (No backdoor pls).

## Features
- Support XLSX format (EXCEL files) for accounts (even though yes, XML is wayyy better for development. Most people are just gonna be blind to XML format)
- Support isolated environment for reliability and safety (With Docker containers. Fully WSL support. Isolated environments)
- Dynamic webserver, with modern look and feel
- Great management system, including a hardened admin panel
- Cross-platform support (Windows, Linux, macOS)
- Security over performance (I'm not a security expert, just a hobbyist, so do not take that seriously)
- Arch Linux, btw (I use Arch btw)


## Dependencies
- Python 3 (Probably 3.11)
- Flask
- WSGI caller. Like gunicorn, for example.
- Docker (Both desktop and Python package)

If you don't want to waste your time trying to find these dependencies, do this command:
```bash
python -m pip install -r requirements.txt
```

## Using the software
This thing requires Python, at least 3.10, I highly recommend **3.11.2** as it is tested, others I'm not sure.

### Download
Clone this repo, that's the file.

### Start judging daemon manually
To enable the judging system, navigate to the downloaded directory, then run the command:
```bash
python -m pip install -r requirements.txt
```
Thats the command to install required libraries
To enable actual judging, run the command:
```bash
python judge.py
```
