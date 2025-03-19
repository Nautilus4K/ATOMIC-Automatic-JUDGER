# ATOMIC-Automatic-JUDGER

A Vietnamese automatic testing software built on Python. Supports online submission and offline tests.
This software uses Docker for its isolated environment, WSGI callable object for the server interface, and SQLite, JSON for its database.
This software is designed by Vietnamese people, for Vietnamese people. So the UI is all Vietnamese.
If you don't understand Vietnamese and want to add English. Feel free to contribute, give a pull request, I will add in (No backdoor pls).

**Note:** This software is intended for limited use. Not for public use. If you want to do that, consider editting the code and add a register page.

## Features

- Support XLSX format (EXCEL files) for accounts.
- Support isolated environment for reliability and safety (With Docker containers. Fully WSL support. Isolated environments).
- Dynamic webserver, with modern look and feel.
- Great management system, including a hardened admin panel.
- Security over performance (I'm not a security expert, just a hobbyist, so do not take that seriously).
- Arch Linux, btw (I use Arch btw).
- Login/logout. (Manual registering).

## Dependencies

- Python 3 (Probably 3.11).
- WSGI caller. Like gunicorn, for example.
- Docker (Both desktop and Python package).

If you don't want to waste your time trying to find these dependencies, do this command:

```bash
python -m pip install -r requirements.txt
```

## Using the software (in `/main`)

This thing requires Python, at least 3.10, I highly recommend **3.11.2** as it is tested, others I'm not sure.

### Download

Clone this repo, that's the file. Or wait until a release.

### Dependencies

Dependencies are listed in `requirements.txt` in the form of python packages.

In case you feels like you can run all the things manually, use the following command to install dependencies.

```bash
python -m pip install -r requirements.txt
```

### Start judging daemon manually

To enable actual judging, run the command:

```bash
python judge.py
```

### Start WSGI server.

If you wants to run webserver as an apache, you can! By running `apache.py`.

```bash
python apache.py
```

### Running an ALL-IN-ONE management Panel

There is a panel created to make it easier to run stuffs and browse through settings all done graphically with the Qt GUI Framework. To run it up, open it with Python:

```bash
python panel.py
```

### Main features

| Feature name      | Status      |
|:-----------------:| ----------- |
| Automatic judging | ✅ Ready     |
| Webserver         | ✅ Ready     |
| GUI               | In progress |
