#!/bin/bash

filewatcher index.pug "pug -P index.pug" &
filewatcher app.coffee "coffee -c app.coffee && echo coffee done" &

wait


