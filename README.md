# SSN_Firewall

## Project Description

- **Designed a firewall** to allow only certain **whitelisted IP addresses** to be accessible.
- Used **Linux Kernel Modules** to make the firewall comparatively less detectable and more robust.
- Implemented an interface for the administrators to **view logs** of the number of requests sent to each domain name.
- Used **NetFilterHooks** to filter and drop all packets other than DNS server requests and some whitelisted IP addresses.

## Setup and Run Instructions

### Setup

1. Clone the repository.

```bash
bash maker.sh
```

2. Enter the directory.

```bash
cd SSN_Firewall
```

3. Run the setup script.

```bash
python3 daWorker.py
```

### Running the Server

To run the server, execute the necessary command.

This will start the firewall with the specified configurations. Make sure you have the necessary permissions and dependencies installed.
