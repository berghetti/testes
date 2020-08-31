#!/bin/bash

# y
#
if [ $# -ne 1 ]; then
  echo 1>&2 Usage: $0 + \<link video\>
  exit 1
fi

# 0 to disable
RESOLUTION_DEFAULT=720

USER_AGENT="Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:75.0) \
            Gecko/20100101 Firefox/75.0"


curl --silent --location --fail "$1" -A "$USER_AGENT" -o /tmp/page.txt


LINK=$(grep "videoUrl" /tmp/page.txt |cut -d "," -f 3 | cut -d "\"" -f 4 | sed -e 's/\\//g' | sed -e "s/u0026/\&/g")

rm /tmp/page.txt


qvlc $LINK
