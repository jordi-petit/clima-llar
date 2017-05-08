#!/usr/bin/env python3


"""
Testing client that sends a request to the ClimaLlar server.
"""

import argparse
import socket


def send(ip, port, data):
    print("send '%s' to %s:%s" % (data, ip, port))
    sock = socket.socket()
    sock.connect((ip, port))
    sock.send(data.encode())
    sock.close()


def main():

    server = socket.gethostname()

    parser = argparse.ArgumentParser(description="Sends a request to the ClimaLlar server.")

    parser.add_argument("data", type=str, nargs=1, help="data to send to the server")
    parser.add_argument("-s", "--server", type=str, help="server name or IP", default=server)
    parser.add_argument("-p", "--port", type=int, help="port number", default=9876)

    args = parser.parse_args()

    ip = socket.gethostbyname(args.server)
    send(ip, args.port, args.data[0])


if __name__ == "__main__":
    main()
