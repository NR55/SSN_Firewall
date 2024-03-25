import tkinter as tk
from tkinter import ttk
import os
import subprocess
from datetime import datetime, timedelta

class App(tk.Tk):
    def __init__(self):
        super().__init__()

        self.title("SSN Firewall")
        self.geometry("600x400")
        self.minsize(600, 400)  # Set minimum size

        self.view_logs_button = tk.Button(self, text="Reset", command=self.resetter)
        self.view_logs_button.pack(pady=5)

        self.text_area = tk.Text(self, height=5, width=70)
        self.text_area.pack(pady=10)

        self.should_run = True
        
        # Dropdown menus for selecting time
        self.hour_var = tk.StringVar(self)
        self.minute_var = tk.StringVar(self)
        self.second_var = tk.StringVar(self)

        hours = [str(i).zfill(2) for i in range(6, 25)]
        minutes = [str(i).zfill(2) for i in range(60)]
        seconds = [str(i).zfill(2) for i in range(60)]

        self.title_label = tk.Label(self, text="Schedule the firewall to start at:")
        self.title_label.pack()

        # Create a frame to contain the dropdown menus
        dropdown_frame = tk.Frame(self)
        dropdown_frame.pack(pady=10)

        # Hour dropdown menu
        self.hour_dropdown = ttk.Combobox(dropdown_frame, textvariable=self.hour_var, values=hours, width=5)
        self.hour_dropdown.pack(side=tk.LEFT, padx=10)
        self.hour_dropdown.current(0)

        # Minute dropdown menu
        self.minute_dropdown = ttk.Combobox(dropdown_frame, textvariable=self.minute_var, values=minutes, width=5)
        self.minute_dropdown.pack(side=tk.LEFT, padx=10)
        self.minute_dropdown.current(0)

        # Second dropdown menu
        self.second_dropdown = ttk.Combobox(dropdown_frame, textvariable=self.second_var, values=seconds, width=5)
        self.second_dropdown.pack(side=tk.LEFT, padx=10)
        self.second_dropdown.current(0)

        self.run_button = tk.Button(self, text="Run", command=self.run)
        self.run_button.pack(pady=5)

        self.stop_button = tk.Button(self, text="Stop", command=self.stop)
        self.stop_button.pack(pady=5)

        self.view_logs_button = tk.Button(self, text="View Logs", command=self.view_logs)
        self.view_logs_button.pack(pady=5)

        # self.hour_dropdown = ttk.Combobox(self, textvariable=self.hour_var, values=hours, width=5)
        # self.hour_dropdown.pack(side=tk.LEFT, padx=10)
        # self.hour_dropdown.current(0)

        # self.minute_dropdown = ttk.Combobox(self, textvariable=self.minute_var, values=minutes, width=5)
        # self.minute_dropdown.pack(side=tk.LEFT, padx=10)
        # self.minute_dropdown.current(0)

        # self.second_dropdown = ttk.Combobox(self, textvariable=self.second_var, values=seconds, width=5)
        # self.second_dropdown.pack(side=tk.LEFT, padx=10)
        # self.second_dropdown.current(0)

        self.update_status()

    def update_status(self):
        if self.should_run:
            output = subprocess.check_output("./status.sh", shell=True, universal_newlines=True)
            self.text_area.delete("1.0", tk.END) 
            self.text_area.insert(tk.END, output)
            self.after(1000, self.update_status)

    def run(self):
        selected_hour = int(self.hour_var.get())
        selected_minute = int(self.minute_var.get())
        selected_second = int(self.second_var.get())

        selected_time_ist = datetime.now().replace(hour=selected_hour, minute=selected_minute, second=selected_second)
        time_difference = timedelta(hours=5, minutes=30) 
        selected_time_utc = selected_time_ist - time_difference

        command = f"./runner.sh {selected_time_utc.hour} {selected_time_utc.minute} {selected_time_utc.second}"
        subprocess.call(command, shell=True)

    def stop(self):
        subprocess.call("./stopper.sh", shell=True)

    def resetter(self):
        subprocess.call("./maker.sh", shell=True)

    def view_logs(self):
        subprocess.Popen(["python3", "dnsser.py"])         


if __name__ == "__main__":
    app = App()
    app.mainloop()
