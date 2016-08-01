#!/bin/bash

set -eo pipefail

w="$1"

if [[ "$w" == "" ]] ; then
  w="srv"
fi

OPT="-O2"
#OPT="-O2 -W -Wall -std=c++11"

mkdir -p bin

rm -f index-html.h
cat html/index.html <( echo -e -n '\x0' ) > index_html
xxd -i index_html > index-html.h
rm -f index_html

if [[ "$w" == "srv" ]] || [[ "$w" == "both" ]] ; then

  #g++ -g \
  g++ $OPT \
    `pkg-config --cflags libconfig++` \
    -Ilib/duktape -I.. \
    -DDUK_OPT_INTERRUPT_COUNTER \
    '-DDUK_OPT_EXEC_TIMEOUT_CHECK(udata)=muduk_timeout_check(udata)' \
    '-DDUK_OPT_DECLARE=extern duk_bool_t muduk_timeout_check(void *udata);' \
    muduk.cpp \
    muduk_process.cpp muduk_native.cpp muduk_init.cpp muduk_native_cgf.cpp lib/duktape/duktape.c \
    ../cgb.cpp ../cgb_read.cpp ../cgb_print.cpp ../dlug.c \
    -lmicrohttpd \
    -o bin/muduk \
    `pkg-config --libs libconfig++`


fi

if [[ "$w" == "sh" ]] || [[ "$w" == "both" ]] ; then

  g++ -g -Ilib/duktape -I.. \
    -DDUK_OPT_INTERRUPT_COUNTER \
    '-DDUK_OPT_EXEC_TIMEOUT_CHECK(udata)=muduk_timeout_check(udata)' \
    '-DDUK_OPT_DECLARE=extern duk_bool_t muduk_timeout_check(void *udata);' \
    muduksh.cpp \
    muduk_init.cpp muduk_process.cpp muduk_native.cpp muduk_native_cgf.cpp lib/duktape/duktape.c \
    ../cgb.cpp ../cgb_read.cpp ../cgb_print.cpp ../dlug.c \
    -lmicrohttpd \
    -o bin/muduksh

fi
