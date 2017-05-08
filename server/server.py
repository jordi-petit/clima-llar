#!/usr/bin/env python3


"""
Runs the ClimaLlar server.
"""

import argparse
import datetime
import socket


def time():
    return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def serve(ip, port, database):

    sock = socket.socket()
    sock.bind((ip, port))
    sock.listen()
    print("listening at %s:%s" % (ip, port))

    while True:
        conn, addr = sock.accept()
        data = conn.recv(1024).decode()
        if data is not None:
            info = str(data)
            print("%s %s %s" % (time(), addr[0], info), flush=True)
        conn.close()


def main():

    server = socket.gethostname()
    ip = socket.gethostbyname(server)

    parser = argparse.ArgumentParser(description="Runs the ClimaLlar server.")

    parser.add_argument("-p", "--port", type=int, help="port number", default=9876)
    parser.add_argument("-d", "--database", type=str, help="database name", default="dbname")

    args = parser.parse_args()

    serve(ip, args.port, args.database)


if __name__ == "__main__":
    main()
