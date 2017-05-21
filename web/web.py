import os
import datetime
import sqlite3

from flask import Flask, jsonify, g, request, send_file, send_from_directory


app = Flask(__name__)


app.config["DATABASE"] = os.path.expanduser("~/clima-llar.db")


def current_datetime():
    """Returns the current date and time in my standard format."""
    return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def current_date():
    """Returns the current date in my standard format."""
    return datetime.datetime.now().strftime("%Y-%m-%d")


def connect_db():
    """Connects to the specific database."""
    rv = sqlite3.connect(app.config["DATABASE"])
    rv.row_factory = sqlite3.Row
    return rv


@app.teardown_appcontext
def close_db(error):
    """Closes the database at the end of the request."""
    if hasattr(g, "sqlite_db"):
        g.sqlite_db.close()


def get_db():
    """Opens a new database connection if necessary."""
    if not hasattr(g, "sqlite_db"):
        g.sqlite_db = connect_db()
    return g.sqlite_db


def get_row(*sql):
    """Gets one row of a sql query, returning a dict, or None."""
    dbc = get_db()
    cur = dbc.execute(*sql)
    row = cur.fetchone()
    if row is None:
        return None
    else:
        return dict(row)


def get_one(*sql):
    """Gets one element of a sql query, returning a value."""
    dbc = get_db()
    cur = dbc.execute(*sql)
    row = cur.fetchone()
    if row is None:
        return None
    else:
        return row[0]


def get_all(*sql):
    """Gets all rows of a sql query, returning a list of dicts."""
    dbc = get_db()
    cur = dbc.execute(*sql)
    rows = cur.fetchall()
    return [dict(row) for row in rows]


def get_col(*sql):
    """Gets a column of a sql query, returning a list of values."""
    dbc = get_db()
    cur = dbc.execute(*sql)
    rows = cur.fetchall()
    return [row[0] for row in rows]


def day_readings(place_id, date):
    """Returns th readings for a given place and date."""
    return jsonify(get_all(
        """
        SELECT moment, temperature, humidity, light
        FROM Readings
        WHERE DATE(moment) = ?
        AND place_id = ?
        ORDER BY moment
        """,
        [date, place_id]
    ))


def place_exists(place_id):
    c = get_one(
        """
        SELECT COUNT(*)
        FROM Places
        WHERE place_id = ?
        """,
        [place_id]
    )
    return c != 0


@app.route("/")
def root():
    """Returns the app web page."""
    return send_file("index.html")


@app.route("/app.js")
def app_js():
    """Returns the app script of the web page."""
    return send_file("app.js")


@app.route("/bower_components/<path:path>")
def bower_components(path):
    """Returns static files for bower_components."""
    return send_from_directory("bower_components", path)


@app.route("/api/<place_id>")
def get_current(place_id):
    """Returns the latest readings of a given place."""
    assert place_exists(place_id)
    return jsonify(get_row(
        """
        SELECT moment, temperature, humidity, light
        FROM Readings
        WHERE place_id = ?
        ORDER BY moment DESC
        LIMIT 1
        """,
        [place_id]
    ))


@app.route("/api/<place_id>/submit")
def submit(place_id):
    """Adds a new reading for a given place_id."""
    assert place_exists(place_id)
    try:
        moment = current_datetime()
        ip = request.remote_addr
        temperature = float(request.args["temperature"])
        humidity = float(request.args["humidity"])
        light = float(request.args["light"])
        db = get_db()
        db.execute(
            """
            INSERT INTO Readings (moment, ip, place_id,
                temperature, humidity, light)
            VALUES (?,?,?,?,?,?)
            """,
            [moment, ip, place_id, temperature, humidity, light]
        )
        db.commit()
        return "ok"
    except Exception as exc:
        print(exc)
        return f"error: {exc}"


@app.route("/api/<place_id>/dates")
def get_dates(place_id):
    """Returns the available dates with readings of a given place."""
    assert place_exists(place_id)
    return jsonify(get_col(
        """
        SELECT DISTINCT DATE(moment) AS date
        FROM readings
        WHERE place_id = ?
        ORDER BY date DESC
        """,
        [place_id]
    ))


@app.route("/api/<place_id>/dates/today")
def get_today(place_id):
    """Returns the readings of a given place for today."""
    assert place_exists(place_id)
    today = current_date()
    return day_readings(place_id, today)


@app.route("/api/<place_id>/dates/<date>")
def get_date(place_id, date):
    """Returns the readings of a given place and a given date."""
    assert place_exists(place_id)
    return day_readings(place_id, date)


if __name__ == "__main__":
    app.run()
