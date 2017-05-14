#!/usr/bin/env python3.6


"""
Runs the ClimaLlar server.
"""

import argparse
import datetime
import os
import socket


def current_datetime():
    return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def serve(ip, port, directory):

    now = current_datetime()
    print(f"starting server {now}", flush=True)

    sock = socket.socket()
    sock.bind((ip, port))
    sock.listen()
    print("listening at %s:%s" % (ip, port))

    while True:
        conn, addr = sock.accept()
        now = current_datetime()
        date, time = now.split()
        info = conn.recv(1024).decode()
        key = info.split(":")[0]
        reply = "ok" if key == "home" else "error"  # TBD: Improve key verification
        conn.send(reply.encode())
        conn.close()

        if reply == "ok":
            os.makedirs(f"{directory}/{key}", exist_ok=True)
            with open(f"{directory}/{key}/{date}.txt", "a") as f:
                f.write(f"{now} {addr[0]} {info}\n")
        print(f"{reply} {now} {addr[0]} {info}", flush=True)


def main():

    server = socket.gethostname()
    ip = socket.gethostbyname(server)

    parser = argparse.ArgumentParser(description="Runs the ClimaLlar server.")

    parser.add_argument("-p", "--port", type=int, help="port number", default=9876)
    parser.add_argument("-d", "--directory", type=str, help="output directory", default=".")

    args = parser.parse_args()

    serve(ip, args.port, args.directory)


if __name__ == "__main__":
    main()
