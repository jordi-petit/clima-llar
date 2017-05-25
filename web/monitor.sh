#!/bin/bash

filewatcher index.pug "pug -P index.pug" &
filewatcher app.coffee "echo coffee ; coffee -c app.coffee" &

wait


