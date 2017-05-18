import os
import time
import sqlite3

from flask import Flask, jsonify, g


app = Flask(__name__)


app.config['DATABASE'] = os.path.expanduser('~/clima-llar.db')



def connect_db():
    """Connects to the specific database."""
    rv = sqlite3.connect(app.config['DATABASE'])
    rv.row_factory = sqlite3.Row
    return rv


def get_db():
    """Opens a new database connection if there is none yet for the
    current application context.
    """
    if not hasattr(g, 'sqlite_db'):
        g.sqlite_db = connect_db()
    return g.sqlite_db


@app.teardown_appcontext
def close_db(error):
    """Closes the database again at the end of the request."""
    if hasattr(g, 'sqlite_db'):
        g.sqlite_db.close()


def day_readings(place_id, date):
    dbc = get_db()
    cur = dbc.execute('select moment, temperature, humidity, light from readings where date(moment) = ? and place_id = ? order by moment', [date, place_id])
    rows = cur.fetchall()

    output = []
    for row in rows:
        output.append({
            'moment': row[0],
            'temperature': row[1],
            'humidity': row[2],
            'light': row[3]
        })

    return jsonify(output)


@app.route('/')
def hello_world():
    return 'Hello, World!\n'


@app.route('/<place_id>/now')
def get_now(place_id):
    dbc = get_db()
    cur = dbc.execute('select moment, temperature, humidity, light from readings where place_id = ? order by moment desc limit 1', [place_id])
    row = cur.fetchone()

    return jsonify(
        moment=row[0],
        temperature=row[1],
        humidity=row[2],
        light=row[3]
    )


@app.route('/<place_id>/today')
def get_today(place_id):
    today = time.strftime("%Y-%m-%d")
    return day_readings(place_id, today)


@app.route('/<place_id>/date/<date>')
def get_date(place_id, date):
    return day_readings(place_id, date)



if __name__ == "__main__":
    app.run()
