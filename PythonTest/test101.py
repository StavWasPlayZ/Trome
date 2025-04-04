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
            sendAndRecv(LOGIN, JSON_TEST_LOGIN, sock)
            sendAndRecv(SIGNUP, JSON_TEST_SIGNUP, sock)
        except:
            print("Something went wrong :(")
            return
        
def sendAndRecv(code, data_to_send, sock):
    """
    sends then data and recv server's response
    :param data_to_send: data to send
    :param code: code to send
    :param sock: socket
    :type data: json
    :type code: int
    :type sock: socket
    :return: none
    :rtype: none
    """
    msg = serialize(code, data_to_send)
    sock.sendall(msg)
    print("Sent: Code:", code, "\nData:", JSON_TEST_LOGIN, "\nIn bytes:", msg)

    server_msg = sock.recv(1024)
    data = deserialzer(server_msg)

    print("Sent: Code:", data[0], "\nData Len:", data[1], "\nData:", data[2], "\nIn bytes:", server_msg)
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
    json_len_bytes = len(json_bytes).to_bytes(4, byteorder='big') # big is the format we need
    message = bytes([code]) + json_len_bytes + json_bytes
    return message

def deserialzer(data):
    """
    deserialze data
    :param data: data to send
    :type data: bytes
    :return: code, json_len, json
    :rtype: tuple
    """
    code = data[0]
    json_len = int.from_bytes(data[1:5], byteorder='big')  # big is the format we need
    json_bytes = data[5:5+json_len]
    data_json = json.loads(json_bytes.decode('utf-8'))
    return code, json_len, data_json


if __name__ == "__main__":
    main()
