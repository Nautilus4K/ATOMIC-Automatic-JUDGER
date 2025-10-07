# ATOMIC Automatic Judging System

![ATOMIC Automatic Judging System](/assets/fullname.png)

ATOMIC is a robust, automated testing software designed for both online submissions and offline tests. Built with Python, it leverages Docker for isolated environments, WSGI for server interfacing, and JSON for database management. The software is tailored for Vietnamese users, featuring a fully localized Vietnamese user interface. Contributions for additional language support, such as English, are welcome.

> **Note:** This software is intended for limited use. For public deployment, modifications such as a registration page are recommended.

---

## Key Features

- **Excel Integration**: Supports XLSX format for account management.
- **Isolated Environments**: Ensures reliability and safety using Docker containers (with full WSL support).
- **Dynamic Web Interface**: Modern, responsive webserver design.
- **Comprehensive Management**: Includes a secure and feature-rich admin panel.
- **Security-Focused**: Prioritizes security over performance (note: not professionally audited).
- **Cross-Platform**: Compatible with both Windows and Linux operating systems.

---

## System Requirements

**Operating System**:  
- Windows 10 or later  
- Linux (tested on Arch Linux in Docker)

**Hardware**:  
- **RAM**: Minimum 1GB (DDR2), Recommended 2GB (DDR4)  
- **GPU**: It depends:

| Use AI? | Recommended GPU specifications |
|---------|--------------------------------|
| No      | None                           |
| Yes     | 8GB of VRAM                    |

**Software**:  
- Python 3.10 or later (tested with Python 3.11.2)  
- WSGI server (e.g., Gunicorn)  
- Docker (Desktop and Python package)

---

## Installation

### Clone the Repository
To get started, clone this repository or download the latest release (if available).

```bash
git clone https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER.git
cd ATOMIC-Automatic-JUDGER
```

### Install Dependencies
Dependencies are listed in `requirements.txt`. Install them using the following command:

```bash
python -m pip install -r requirements.txt
```

---

## Usage

### Judging Daemon
To enable the judging functionality, run the following command:

```bash
python judge.py
```

### Webserver
To start the webserver (e.g., for Apache), execute:

```bash
python apache.py
```

### Management Panel
For a graphical interface to manage settings and operations, compile the management panel built with the Qt Framework

```bash
make
```

After compiling, you can invoke it with
```bash
./panel
```

---

## Features Overview

| Feature             | Status       |
|---------------------|--------------|
| Automatic Judging   | ✅ Available |
| Webserver           | ✅ Available |
| Management Panel    | 🚧 In Progress |

---

## Contribution
We welcome contributions to improve the software. If you wish to add features, fix bugs, or localize the interface, feel free to submit a pull request. Please ensure your contributions adhere to the repository's license and guidelines.

---

## License
This project is licensed under the terms specified in the `LICENSE` file. Proper attribution is required for any use or distribution.

---

For more information, visit the [GitHub repository](https://github.com/Nautilus4K/ATOMIC-Automatic-JUDGER).
