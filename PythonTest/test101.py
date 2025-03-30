import socket
SERVER_INFO = ("127.0.0.1", 6942)
EXPECTED_RESULT = "Hello"
def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(SERVER_INFO)
        while(True):
            server_msg = sock.recv(1024)
            server_msg = server_msg.decode()
            if server_msg == EXPECTED_RESULT:
                sock.sendall(EXPECTED_RESULT.encode())
                break


if __name__ == "__main__":
    main()