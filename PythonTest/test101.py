import socket

SERVER_INFO = ("127.0.0.1", 6942)
CMD_HELLO = "Hello"

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(SERVER_INFO)

        while True:
            server_msg = sock.recv(1024).decode()
            print("Recieved:", server_msg)

            if server_msg == CMD_HELLO:
                sock.sendall(CMD_HELLO.encode())
                print("Sent:", CMD_HELLO)


if __name__ == "__main__":
    main()
