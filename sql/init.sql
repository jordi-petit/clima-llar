
CREATE TABLE Places (
    place_id    TEXT UNIQUE NOT NULL,
    name        TEXT NOT NULL,

    PRIMARY KEY (place_id)
);


CREATE TABLE Readings (
    moment      TEXT NOT NULL,
    place_id    TEXT NOT NULL,
    ip          TEXT,
    temperature REAL,
    humidity    REAL,
    light       REAL,

    FOREIGN KEY (place_id) REFERENCES Places
);
