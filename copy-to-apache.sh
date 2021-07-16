#!/bin/bash

## This script copies the compiled scripts and html files into the folder where the httpd server is running

# Copy html
sudo cp /home/giodueck/vscode/dhips/html/* /var/www/html/*  # only one html file is copied, which is renamed to index.html

# Copy executables
sudo cp /home/giodueck/vscode/dhips/cgi-bin/* /usr/lib/cgi-bin/