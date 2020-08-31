#!/bin/bash

# x
#
if [ $# -ne 1 ]; then
  echo 1>&2 Usage: $0 + \<link video\>
  exit 1
fi

# 0 to disable
RESOLUTION_DEFAULT=720

USER_AGENT="Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:75.0) \
            Gecko/20100101 Firefox/75.0"

# pega o codigo fonte da pagina
echo "baixando source page... "
curl --silent --location --fail "$1" -A "$USER_AGENT" -o /tmp/page.txt
if [ $? -ne 0 ]; then
	echo "falha ao baixar source page"
  exit 1
fi

# separa o link do arquivo m3u8
# grep "m3u8" /tmp/page.txt | cut -d "'" -f 2 > /tmp/hls
echo "filtrando source... "
LINK_HLS=$(grep "m3u8" /tmp/page.txt | cut -d "'" -f 2)
if [ $? -ne 0 ]; then
	echo "falha ao filtrar source page"
  exit 1
fi
rm /tmp/page.txt

# baixa o conteudo do arquivo m3u8
# curl -sS $(cat /tmp/hls) -A "$USER_AGENT" -o /tmp/hls2
# echo "print link hls $LINK_HLS"
echo "baixando arquivo m3u8... "
HLS=$(curl --silent --location --fail "$LINK_HLS" -A "$USER_AGENT")
if [ $? -ne 0 ]; then
	echo "falha ao baixar arquivo m3u8"
  exit 1
fi

# se estiver definido resolução default tenta utiliza-la
if [[ $RESOLUTION_DEFAULT -ne 0 ]] && grep -q $RESOLUTION_DEFAULT <<< $HLS; then
  HLS=$(grep $RESOLUTION_DEFAULT <<< $HLS)
  echo "resolução padrão $RESOLUTION_DEFAULT selecionada"
elif grep -q "1080" <<< $HLS; then
  HLS=$(grep "1080" <<< $HLS)
  echo "resolução 1080p selecionada"
elif grep -q "720" <<< $HLS; then
  HLS=$(grep "720" <<< $HLS)
  echo "resolução 720p selecionada"
elif grep -q "480" <<< $HLS; then
  HLS=$(grep "480" <<< $HLS)
  echo "resolução 480 selecionada"
elif grep -q "360" <<< $HLS; then
  HLS=$(grep "360" <<< $HLS)
  echo "resolução 360 selecionada"
else
  echo "resolução não suportada"
  exit 1
fi

# pega a parte util do arquivo m3u8
# cat /tmp/hls2 | grep "hls" | cut -d . -f 1 > /tmp/hls3
echo "filtrando arquivo m3u8... "
HLS=$(grep "hls" <<< $HLS | cut -d . -f 1)
if [ $? -ne 0 ]; then
	echo "falha ao filtrar arquivo m3u8"
  exit 1
fi

# edita o link com conteudo util do arquivo m3u8
# sed -e "s/hls\./$(head -n1 /tmp/hls3)./" /tmp/hls > /tmp/link-final
echo "montando link... "
LINK_VIDEO=$(sed -e "s/hls\./$(head -n1 <<< $HLS)./" <<< $LINK_HLS)
if [ $? -ne 0 ]; then
	echo "falha ao montar link do video"
  exit 1
fi

# criar arquivo com links de todas as resoluções disponiveis
# porem o VLC pega o primeiro e roda, não altera conforme a banda como se fosse
# o navegador... :(
# while read linha
# do
#
# sed -e "s/hls\./$linha./" <<< $LINK_HLS >> /tmp/links-m3u8
#
# done <<< $HLS
# sed -e "s/hls\./$(head -n1 <<< $HLS)./" <<< $LINK_HLS

# cat /tmp/links-m3u8

# abre vlc
qvlc $LINK_VIDEO
# qvlc /tmp/links-m3u8

# rm /tmp/links-m3u8

exit 0
