#!/usr/bin/env python3


"""
Runs the ClimaLlar server.
"""

import argparse
import datetime
import socket
import sys


def serve(host, port, database):

    sock = socket.socket()
    sock.bind((host, port))
    sock.listen()
    print("ClimaLlar listening at %s:%s" % (host, port))

    while True:
        conn, addr = sock.accept()
        data = conn.recv(1024).decode()
        if data is not None:
            info = str(data)
            time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            print ("%s %s %s" % (time, addr[0], str(data)))
        conn.close()


def main():

    host = socket.gethostbyname(socket.gethostname())

    parser = argparse.ArgumentParser(description="Runs the ClimaLlar server.")

    parser.add_argument("-p", "--port", type=int, help="port number", default=9876)
    parser.add_argument("-d", "--database", type=str, help="database name", default="dbname")

    args = parser.parse_args()

    serve(host, args.port, args.database)


if __name__ == "__main__":
    main()



