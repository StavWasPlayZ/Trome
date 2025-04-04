import socket
import json

LOGIN = 1
SIGNUP = 2

JSON_TEST_LOGIN = {"username": "user1", "password": "1234"}
JSON_TEST_SIGNUP = {"username": "user1", "password": "1234", "mail": "user1@gmail.com"}

SERVER_INFO = ("127.0.0.1", 6942)

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(SERVER_INFO)

        try:
            msg = serialize(LOGIN, JSON_TEST_LOGIN)
            sock.sendall(msg)
            print("Sent: Code:", LOGIN, "\nData:", JSON_TEST_LOGIN, "\nIn bytes:", msg)

            server_msg = sock.recv(1024).decode()
        
            print("Recieved:", server_msg)

        except:
            print("Something went wrong :(")
            return

def serialize(code, data):
    """
    serialze data
    :param code: code to send
    :param data: data to send
    :type code: int
    :type data: json
    :return: bytes to send
    :rtype: bytes
    """
    json_bytes = json.dumps(data).encode('utf-8')
    json_len_bytes = len(json_bytes).to_bytes(4, byteorder='big') # big means the format we need
    message = bytes([code]) + json_len_bytes + json_bytes
    return message

if __name__ == "__main__":
    main()
