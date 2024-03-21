#!/bin/bash

HTTPD_USER_NAME=picofoxweb
HTTPD_WORK_DIR=/var/www/foxweb
HTTPD_BIN_NAME=PICOFoxweb
HTTPD_SERVICE=picofoxweb.service

all() {
    PICOFoxweb
}

clean() {
    rm -rf $HTTPD_BIN_NAME
}

PICOFoxweb() {
    main_o
    httpd_o
    gcc -o $HTTPD_BIN_NAME main.o httpd.o
    rm -rf main.o httpd.o
}

main_o() {
    gcc -c -o main.o main.c
}

httpd_o() {
    gcc -c -o httpd.o httpd.c
}

hello() {
    echo "Harrow, everynyan!"
}

install() {
    PICOFoxweb
    useradd -c "$HTTPD_BIN_NAME user" -r -s /sbin/nologin -d $HTTPD_WORK_DIR $HTTPD_USER_NAME
    command install -o root -g root -m 0755 $HTTPD_BIN_NAME /usr/local/sbin/
    command install -o root -g root -m 0644 $HTTPD_SERVICE /etc/systemd/system/
    systemctl daemon-reload
    systemctl restart $HTTPD_SERVICE
    mkdir -p $HTTPD_WORK_DIR
    cp -r webroot -t $HTTPD_WORK_DIR/
    chown -R $HTTPD_USER_NAME:$HTTPD_USER_NAME $HTTPD_WORK_DIR
    rm -f ./$HTTPD_BIN_NAME
}

uninstall() {
    systemctl stop $HTTPD_SERVICE
    rm -rf $HTTPD_WORK_DIR
	rm -f /usr/local/sbin/$HTTPD_BIN_NAME
	rm -f /etc/systemd/system/$HTTPD_SERVICE
	systemctl daemon-reload
	userdel -f $HTTPD_USER_NAME
}

if [ $# -gt 0 ]; then
    f_call=$1; shift; $f_call "$@"
else
    all
fi


