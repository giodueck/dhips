#!/bin/bash

# dhips
echo "Compiling dhips"
g++ -g src/hips.cpp src/detector.cpp src/preventer.cpp src/module.cpp src/dhipslib.cpp src/pgsql.c src/startup.cpp src/monitor.cpp src/m_i.cpp src/m_ii.cpp src/m_iii.cpp src/m_iv.cpp src/m_vii.cpp -o dhips -I/root/hips/dhips/headers -I/usr/include/postgresql -lcrypt -lpq

# main page
echo "Compiling main page"
g++ -g src/mainpage.cpp src/login.cpp src/pgsql.c src/web.c src/dhipsweb.cpp src/dhipslib.cpp -o cgi-bin/main -I/root/hips/dhips/headers -I/usr/include/postgresql -lcrypt -lpq

# login page
echo "Compiling login page"
g++ -g src/loginpage.cpp src/web.c -o cgi-bin/login -I/root/hips/dhips/headers -I/usr/include/postgresql -lcrypt -lpq

# logout page
echo "Compiling logout page"
g++ -g src/logoutpage.c src/web.c src/pgsql.c -o cgi-bin/logout -I/root/hips/dhips/headers -I/usr/include/postgresql -lcrypt -lpq

# manage users page
echo "Compiling manage users page"
g++ -g src/manageuserspage.c src/web.c src/pgsql.c src/login.cpp -o cgi-bin/user_mgmt -I/root/hips/dhips/headers -I/usr/include/postgresql -lcrypt -lpq

# config page
echo "Compiling config page"
g++ -g src/configpage.cpp src/web.c src/pgsql.c src/login.cpp -o cgi-bin/config -I/root/hips/dhips/headers -I/usr/include/postgresql -lcrypt -lpq

# Copy html
sudo cp -v html/* /var/www/html/index.html  # only one html file is copied, which is renamed to index.html

# Copy executables
sudo cp -v cgi-bin/* /var/www/cgi-bin/
