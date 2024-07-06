# SSN_Firewall

## Project Description

- **Designed a firewall** to allow only certain **whitelisted IP addresses** to be accessible.
- Used **Linux Kernel Modules** to make the firewall comparatively less detectable and more robust.
- Implemented an interface for the administrators to **view logs** of the number of requests sent to each domain name.
- Used **NetFilterHooks** to filter and drop all packets other than DNS server requests and some whitelisted IP addresses.

## Requirements

This firewall is made exclusively for Linux based systems/virtual systems. Make sure you have superuser permissions and you are connected to a IPv4 network (preferrably NITK Net).


## Setup and Run Instructions

### Setup

1. Clone the repository.

```bash
git clone https://github.com/NR55/SSN_Firewall.git
```

2. Enter the directory.

```bash
cd SSN_Firewall
```

1. Run the setup script.

```bash
bash maker.sh
```

4. Start the program and customize your firewall using the below command.
```bash
python3 daWorker.py
```
