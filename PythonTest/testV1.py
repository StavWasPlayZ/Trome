import communicator
import socket

LOGIN = 1
SIGNUP = 2

SEPARATOR =  "-----------------"

JSON_TEST_LOGIN = {
    "username": "user1",
    "password": "Aa12345!"
}
JSON_TEST_SIGNUP = {
    "username": "user1",
    "password": "Aa12345!",
    "email": "user1@gmail.com",
    "phone": "0555555555",
    "birthdate": "11/11/2011",
    "address": "some, 69, ig"
}
JSON_WRONG_PASSWORD = {
    "username": "user wrong password",
    "password": "Aa123456", # no special char
    "email": "user1@gmail.com",
    "phone": "0555555555",
    "birthdate": "11/11/2011",
    "address": "some, 69, ig"
}
JSON_WRONG_EMAIL = {
    "username": "user wrong email",
    "password": "Aa12345!",
    "email": "user1@gmail", # no .com
    "phone": "0555555555",
    "birthdate": "11/11/2011",
    "address": "some, 69, ig"
}
JSON_WRONG_PHONE = {
    "username": "user wrong phone",
    "password": "Aa12345!",
    "email": "user1@gmail.com",
    "phone": "11111", # illegal phone number
    "birthdate": "11/11/2011",
    "address": "some, 69, ig"
}
JSON_WRONG_DATE = {
    "username": "user wrong date",
    "password": "Aa12345!",
    "email": "user1@gmail.com",
    "phone": "0555555555",
    "birthdate": "11-11/2011", # not in format of DD/MM/YYYY
    "address": "some, 69, ig"
}
JSON_WRONG_ADDRESS = {
    "username": "user wrong address",
    "password": "Aa12345!",
    "email": "user1@gmail.com",
    "phone": "0555555555",
    "birthdate": "11/11/2011",
    "address": "some, 69" # no 3rd arg
}

SERVER_INFO = ("127.0.0.1", 6942)

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(SERVER_INFO)

        try:
            communicator.sendAndRecv(SIGNUP, JSON_TEST_SIGNUP, sock) # shouldn't signup - already in DB

            # all of those shouldn't signup
            communicator.sendAndRecv(SIGNUP, JSON_WRONG_PASSWORD, sock)
            communicator.sendAndRecv(SIGNUP, JSON_WRONG_EMAIL, sock)
            communicator.sendAndRecv(SIGNUP, JSON_WRONG_PHONE, sock)
            communicator.sendAndRecv(SIGNUP, JSON_WRONG_DATE, sock)
            communicator.sendAndRecv(SIGNUP, JSON_WRONG_ADDRESS, sock)
        except:
            print("Something went wrong :(")
            return

if __name__ == "__main__":
    main()
