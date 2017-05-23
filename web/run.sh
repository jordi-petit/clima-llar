#!/bin/bash

source venv/bin/activate

export FLASK_APP=web.py 
export DEBUG=1 

flask run