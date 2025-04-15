import socket
import json

from typing import *

LOGIN = 1
SIGNUP = 2

SEPARATOR =  "-----------------"

JSON_TEST_LOGIN = {
    "username": "user1",
    "password": "1234"
}
JSON_TEST_SIGNUP = {
    "username": "user1",
    "password": "Aa12345!",
    "email": "user1@gmail.com",
    "phone": "0555555555",
    "birthdate": "11/11/2011",
    "address": "some, thing, ig"
}
JSON_WRONG_PASSWORD = {
    "username": "user wrong password",
    "password": "Aa123456", # no special char
    "email": "user1@gmail.com",
    "phone": "0555555555",
    "birthdate": "11/11/2011",
    "address": "some, thing, ig"
}
JSON_WRONG_EMAIL = {
    "username": "user wrong email",
    "password": "Aa12345!",
    "email": "user1@gmail", # no .com
    "phone": "0555555555",
    "birthdate": "11/11/2011",
    "address": "some, thing, ig"
}
JSON_WRONG_PHONE = {
    "username": "user wrong phone",
    "password": "Aa12345!",
    "email": "user1@gmail.com",
    "phone": "11111", # illegal phone number
    "birthdate": "11/11/2011",
    "address": "some, thing, ig"
}
JSON_WRONG_DATE = {
    "username": "user wrong date",
    "password": "Aa12345!",
    "email": "user1@gmail.com",
    "phone": "0555555555",
    "birthdate": "11-11/2011", # not in format of DD/MM/YYYY
    "address": "some, thing, ig"
}
JSON_WRONG_ADDRESS = {
    "username": "user wrong address",
    "password": "Aa12345!",
    "email": "user1@gmail.com",
    "phone": "0555555555",
    "birthdate": "11/11/2011",
    "address": "some, thing" # no 3rd arg
}

SERVER_INFO = ("127.0.0.1", 6942)

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(SERVER_INFO)

        try:
            sendAndRecv(SIGNUP, JSON_TEST_SIGNUP, sock) # shouldn't signup - already in DB

            # all of those shouldn't signup
            sendAndRecv(SIGNUP, JSON_WRONG_PASSWORD, sock);
            sendAndRecv(SIGNUP, JSON_WRONG_EMAIL, sock);
            sendAndRecv(SIGNUP, JSON_WRONG_PHONE, sock);
            sendAndRecv(SIGNUP, JSON_WRONG_DATE, sock);
            sendAndRecv(SIGNUP, JSON_WRONG_ADDRESS, sock);
        except:
            print("Something went wrong :(")
            return
        
def sendAndRecv(code: int, data: json, sock: socket.socket):
    """
    Sends the provided data, and fetches the server's response
    :param data_to_send: The data to send
    :param code: The code to send
    :param sock: The server socket
    """
    msg = serialize(code, data)
    sock.sendall(msg)
    print(SEPARATOR)
    print("Sent: Code:", code, "\nData:", data, "\nIn bytes:", msg)

    server_msg = sock.recv(1024)
    raw_data = deserialzer(server_msg)

    print(SEPARATOR)
    print("Recieved: Code:", raw_data[0], "\nData Len:", raw_data[1], "\nData:", raw_data[2], "\nIn bytes:", server_msg)


def serialize(code: int, data: json) -> bytes:
    """
    Serialzes the provided data
    :param code: The code to send
    :param data: The data to send
    :return: bytes to send
    """
    json_bytes = json.dumps(data).encode('utf-8')
    json_len_bytes = len(json_bytes).to_bytes(4, byteorder='big') # big is the format we need
    message = bytes([code]) + json_len_bytes + json_bytes

    return message

def deserialzer(data: bytes) -> Tuple[int, int, Any]:
    """
    Deserialzes the provided data
    :param data: The data to send
    """
    code = data[0]
    json_len = int.from_bytes(data[1:5], byteorder='big')  # big is the format we need
    json_bytes = data[5:5+json_len]
    data_json = json.loads(json_bytes.decode('utf-8'))

    return (code, json_len, data_json)


if __name__ == "__main__":
    main()
