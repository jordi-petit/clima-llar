import os
import datetime
import sqlite3

from flask import Flask, jsonify, g, request, send_file, send_from_directory


app = Flask(__name__)


app.config["DATABASE"] = os.path.expanduser("~/clima-llar.db")


def current_datetime():
    return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def current_date():
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
    """Opens a new database connection if there is none yet for the current application context."""
    if not hasattr(g, "sqlite_db"):
        g.sqlite_db = connect_db()
    return g.sqlite_db


def fetchone(*sql):
    dbc = get_db()
    cur = dbc.execute(*sql)
    row = cur.fetchone()
    if row is None:
        return None
    else:
        return dict(row)


def fetchall(*sql):
    dbc = get_db()
    cur = dbc.execute(*sql)
    rows = cur.fetchall()
    return [dict(row) for row in rows]


def fetchcol(*sql):
    dbc = get_db()
    cur = dbc.execute(*sql)
    rows = cur.fetchall()
    return [row[0] for row in rows]


def day_readings(place_id, date):
    return jsonify(fetchall("select moment, temperature, humidity, light from readings where date(moment) = ? and place_id = ? order by moment", [date, place_id]))


@app.route("/")
def root():
    """Returns the app web page."""
    return send_file("index.html")


@app.route("/app.js")
def app_js():
    """Returns the app script."""
    return send_file("app.js")


@app.route("/bower_components/<path:path>")
def bower_components(path):
    """Returns static files for bower_components."""
    return send_from_directory("bower_components", path)


@app.route("/api/<place_id>")
def get_current(place_id):
    """Returns the latest readings of a given place."""
    return jsonify(fetchone("select moment, temperature, humidity, light from readings where place_id = ? order by moment desc limit 1", [place_id]))


@app.route("/api/<place_id>/submit")
def submit(place_id):
    """Adds a new reading for a given place_id."""
    try:
        moment = current_datetime()
        ip = request.remote_addr
        temperature = float(request.args["temperature"])
        humidity = float(request.args["humidity"])
        light = float(request.args["light"])
        db = get_db()
        db.execute('insert into readings (moment, ip, place_id, temperature, humidity, light) values (?,?,?,?,?,?)',
            [moment, ip, place_id, temperature, humidity, light])
        db.commit()
        return "ok"
    except Exception as exc:
        print(exc)
        return f"error: {exc}"


@app.route("/api/<place_id>/dates")
def get_dates(place_id):
    """Returns the available dates with readings of a given place."""
    return jsonify(fetchcol("select distinct date(moment) as date from readings where place_id = ? order by date desc", [place_id]))


@app.route("/api/<place_id>/dates/today")
def get_today(place_id):
    """Returns the readings of a given place for today."""
    today = current_date()
    return day_readings(place_id, today)


@app.route("/api/<place_id>/dates/<date>")
def get_date(place_id, date):
    """Returns the readings of a given place and a given date."""
    return day_readings(place_id, date)


if __name__ == "__main__":
    app.run()
