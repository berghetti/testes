#!/bin/bash

# r
#
if [ $# -ne 1 ]; then
  echo 1>&2 Usage: $0 + \<link video\>
  exit 1
fi

# 0 to disable
RESOLUTION_DEFAULT=720

USER_AGENT="Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:75.0) \
            Gecko/20100101 Firefox/75.0"

echo "baixando page source"
curl --silent --location --fail "$1" -A "$USER_AGENT" -o /tmp/page.txt
if [ $? -ne 0 ]; then
	echo "falha ao baixar source page"
  exit 1
fi

echo "filtando source"
LINKS=$(grep "js_download_video" /tmp/page.txt | cut -d "\"" -f 4)
rm /tmp/page.txt
if [ $? -ne 0 ]; then
	echo "falha ao filtrar source"
  exit 1
fi

echo $LINKS

# se estiver definido resolução default tenta utiliza-la
if [[ $RESOLUTION_DEFAULT -ne 0 ]] && grep -q $RESOLUTION_DEFAULT <<< $LINKS; then
  LINK=$(grep $RESOLUTION_DEFAULT <<< $LINKS)
  echo "resolução padrão $RESOLUTION_DEFAULT selecionada"
elif grep -q "1080" <<< $LINKS; then
  LINK=$(grep "1080" <<< $LINKS)
  echo "resolução 1080p selecionada"
elif grep -q "720" <<< $LINKS; then
  LINK=$(grep "720" <<< $LINKS)
  echo "resolução 720p selecionada"
elif grep -q "480" <<< $LINKS; then
  LINK=$(grep "480" <<< $LINKS)
  echo "resolução 480 selecionada"
elif grep -q "360" <<< $LINKS; then
  LINK=$(grep "360" <<< $LINKS)
  echo "resolução 360 selecionada"
else
  echo "resolução não suportada"
  exit 1
fi

vlc $LINK
