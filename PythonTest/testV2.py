import communicator
import socket

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(communicator.SERVER_INFO)

        try:
            ## communicator.sendAndRecv(communicator.SIGNUP, communicator.JSON_TEST_SIGNUP, sock) ## do this if there isn't this user in db
            communicator.sendAndRecv(communicator.LOGIN, communicator.JSON_TEST_LOGIN, sock) ## remove this if there isn't this user in db
            communicator.sendAndRecv(communicator.GET_PERSONAL_STATISTICS, communicator.JSON_GET_PERSONAL_STATS, sock)
        except:
            print("Something went wrong :(")
        return

if __name__=="__main__":
    main()