import tkinter as tk
from tkinter.scrolledtext import ScrolledText
import socket
import threading
import subprocess

MAX_CLIENTS = 10
BUFFER_SIZE = 1024

class ChatServer:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.clients = [None] * MAX_CLIENTS
        self.client_count = 0

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)

        self.root = tk.Tk()
        self.root.title("Chat Server")

        self.text_area = ScrolledText(self.root, height=20, width=50)
        self.text_area.pack()

        self.info_label = tk.Label(self.root, text=f"Server IP: {self.get_private_ip()}, Port: {self.port}")
        self.info_label.pack()

        self.entry_label = tk.Label(self.root, text="Enter message to broadcast:")
        self.entry_label.pack()

        self.message_entry = tk.Entry(self.root, width=50)
        self.message_entry.pack()

        self.send_button = tk.Button(self.root, text="Send", command=self.send_message)
        self.send_button.pack()

        self.runner_button = tk.Button(self.root, text="Run runner.sh", command=self.send_runner)
        self.runner_button.pack()

        self.status_button = tk.Button(self.root, text="Check status.sh", command=self.send_status)
        self.status_button.pack()

        self.stopper_button = tk.Button(self.root, text="Stop stopper.sh", command=self.send_stopper)
        self.stopper_button.pack()

        self.listen_thread = threading.Thread(target=self.listen_connections)
        self.listen_thread.daemon = True
        self.listen_thread.start()

        self.root.mainloop()

    def get_private_ip(self):
        try:
            result = subprocess.run(["ifconfig"], capture_output=True, text=True)
            output = result.stdout
            # Extract private IP address from ifconfig output
            ip_address = output.split("inet ")[2].split(" ")[0]
            return ip_address
        except Exception as e:
            print(f"Error getting private IP address: {e}")
            return "Unknown"

    def send_message(self):
        message = self.message_entry.get()
        if message:
            broadcast_message = f"Server: {message}\n"
            self.update_chat(broadcast_message)
            self.broadcast(message)
            self.message_entry.delete(0, tk.END)

    def send_runner(self):
        self.broadcast(f"./runner.sh {self.get_private_ip()}")

    def send_status(self):
        self.broadcast("./status.sh")

    def send_stopper(self):
        self.broadcast("./stopper.sh")

    def broadcast(self, message):
        for client_socket in self.clients:
            if client_socket:
                try:
                    client_socket.sendall(message.encode())
                except Exception as e:
                    print(f"Error broadcasting message: {e}")

    def listen_connections(self):
        while True:
            client_socket, _ = self.server_socket.accept()

            for i in range(MAX_CLIENTS):
                if self.clients[i] is None:
                    self.clients[i] = client_socket
                    self.client_count += 1
                    break

            threading.Thread(target=self.listen_messages, args=(client_socket,)).start()

            message = f"Client {client_socket.getpeername()} joined.\n"
            self.update_chat(message)

    def listen_messages(self, client_socket):
        while True:
            try:
                message = client_socket.recv(BUFFER_SIZE).decode("utf-8")
                if message:
                    broadcast_message = f"Client {client_socket.getpeername()}: {message}"
                    self.update_chat(broadcast_message)
            except Exception as e:
                print(f"Error: {e}")
                client_socket.close()
                return

    def update_chat(self, message):
        self.text_area.insert(tk.END, message)
        self.text_area.see(tk.END)

if __name__ == "__main__":
    port = 8090

    chat_server = ChatServer("", port)  # Host is empty to bind to all available interfaces
