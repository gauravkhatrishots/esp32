import socket

# === Configuration ===
esp32_ip = "10.89.145.66"  # Replace with your ESP32's IP
port = 8080                  # Port your ESP32 TCP server is listening on
message = "Hello ESP32!\n" # Message to send

# === Create and connect socket ===
try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((esp32_ip, port))
    print(f"Connected to {esp32_ip}:{port}")

    # === Send message ===
    sock.sendall(message.encode())
    print(f"Sent: {message.strip()}")

    # === Receive response ===
    response = sock.recv(1024)
    print(f"Received: {response.decode().strip()}")

except Exception as e:
    print(f"Error: {e}")
finally:
    sock.close()
    print("Connection closed.")
