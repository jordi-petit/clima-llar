#!/usr/bin/env python3.6


"""
Runs the ClimaLlar server.
"""

import argparse
import datetime
import logging
import os
import socket
import time


def current_datetime():
    return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def serve(ip, port, directory):

    now = current_datetime()
    sock = socket.socket()
    sock.bind((ip, port))
    sock.listen()
    logging.info(f"clima-llar sensor starting at {ip}:{port}")

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
        logging.info(f"{reply} {addr[0]} {info}")


def main():

    server = socket.gethostname()
    ip = socket.gethostbyname(server)

    parser = argparse.ArgumentParser(description="Runs the ClimaLlar server.")

    parser.add_argument("-p", "--port", type=int, help="port number", default=9876)
    parser.add_argument("-d", "--directory", type=str, help="output directory (defaults to .)", default=".")
    parser.add_argument("-l", "--logfile", type=str, help="log file (defaults to server.log, use None for stdout)", default="server.log")

    args = parser.parse_args()
    if args.logfile == "None":
        args.logfile = None

    logging.basicConfig(level=logging.DEBUG,
        format="%(asctime)s %(levelname)s %(message)s",
        filename=args.logfile)

    while True:
        try:
            serve(ip, args.port, args.directory)
        except Exception as exc:
            logging.error(f"{exc}")
            time.sleep(1)


if __name__ == "__main__":
    main()
