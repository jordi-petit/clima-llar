#!/usr/bin/env python3


"""
Testing client that sends a request to the ClimaLlar server.
"""

import argparse
import socket


def send(ip, port, info):
    print("send '%s' to %s:%s" % (info, ip, port))
    sock = socket.socket()
    sock.connect((ip, port))
    sock.send(info.encode())
    reply = sock.recv(1024).decode()
    print("reply: " + reply, flush=True)
    sock.close()


def main():

    server = socket.gethostname()

    parser = argparse.ArgumentParser(description="Sends a request to the ClimaLlar server.")

    parser.add_argument("info", type=str, nargs=1, help="information to send to the server")
    parser.add_argument("-s", "--server", type=str, help="server name or IP", default=server)
    parser.add_argument("-p", "--port", type=int, help="port number", default=9876)

    args = parser.parse_args()

    ip = socket.gethostbyname(args.server)
    send(ip, args.port, args.info[0])


if __name__ == "__main__":
    main()
