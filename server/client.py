#!/usr/bin/env python3


"""
Testing client that sends a request to the ClimaLlar server.
"""

import argparse
import datetime
import socket
import sys


def send(host, port, data):
    sock = socket.socket()
    sock.connect((host, port))
    sock.send(data.encode())
    sock.close()


def main():

    defserver = socket.gethostbyname(socket.gethostname())

    parser = argparse.ArgumentParser(description="Sends a request to the ClimaLlar server.")

    parser.add_argument('data', type=str, nargs=1, help='data to send to the server')
    parser.add_argument("-s", "--server", type=str, help="server host name", default=defserver)
    parser.add_argument("-p", "--port", type=int, help="port number", default=9876)

    args = parser.parse_args()

    send(args.server, args.port, args.data[0])


if __name__ == "__main__":
    main()




