import re
import dns.resolver
import dns.reversename
import tkinter as tk
from tkinter import ttk

# Regex pattern to match dropped packets
pattern = r"\[.*\] DROP: (\d+\.\d+\.\d+\.\d+)"

# Function to resolve DNS names
def resolve_dns(ip):
    try:
        # Create a PTR record for reverse DNS lookup
        ptr = dns.reversename.from_address(ip)
        # Query DNS for PTR record
        result = dns.resolver.resolve(ptr, "PTR")
        return str(result[0])[:-1]  # remove trailing dot
    except (dns.resolver.NoAnswer, dns.resolver.NXDOMAIN, dns.resolver.NoNameservers, dns.resolver.Timeout):
        return "Unknown"


# Dictionary to store IP addresses, DNS names, and counts
ip_counts = {}

# Open the log file
with open("dropped.log", "r") as file:
    # Iterate through each line in the file
    for line in file:
        # Match the line against the pattern
        match = re.search(pattern, line)
        if match:
            ip = match.group(1)
            dns_name = resolve_dns(ip)
            if ip in ip_counts:
                ip_counts[ip]["count"] += 1
            else:
                ip_counts[ip] = {"dns_name": dns_name, "count": 1}

# Convert dictionary to a list of tuples for sorting
sorted_ips = sorted(ip_counts.items(), key=lambda x: x[1]["count"], reverse=True)

# Create a Tkinter window
root = tk.Tk()
root.title("Dropped Packets")

# Create a treeview widget to display the data
tree = ttk.Treeview(root)
tree["columns"] = ("IP Address", "DNS Name", "Count")
tree.heading("#0", text="Index")
tree.heading("IP Address", text="IP Address")
tree.heading("DNS Name", text="DNS Name")
tree.heading("Count", text="Count")

# Insert data into the treeview
for i, (ip, data) in enumerate(sorted_ips):
    tree.insert("", "end", text=str(i+1), values=(ip, data["dns_name"], data["count"]))

# Pack the treeview widget
tree.pack(expand=True, fill="both")

# Run the Tkinter event loop
root.mainloop()
