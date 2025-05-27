import communicator
import socket

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(communicator.SERVER_INFO)

        try:
            communicator.sendAndRecv(communicator.SIGNUP, communicator.JSON_TEST_SIGNUP, sock) # shouldn't signup - already in DB

            # all of those shouldn't signup
            communicator.sendAndRecv(communicator.SIGNUP, communicator.JSON_WRONG_PASSWORD, sock)
            communicator.sendAndRecv(communicator.SIGNUP, communicator.JSON_WRONG_EMAIL, sock)
            communicator.sendAndRecv(communicator.SIGNUP, communicator.JSON_WRONG_PHONE, sock)
            communicator.sendAndRecv(communicator.SIGNUP, communicator.JSON_WRONG_DATE, sock)
            communicator.sendAndRecv(communicator.SIGNUP, communicator.JSON_WRONG_ADDRESS, sock)
        except:
            print("Something went wrong :(")
            return

if __name__ == "__main__":
    main()
