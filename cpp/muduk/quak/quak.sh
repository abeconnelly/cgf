#!/bin/bash

#url="http://localhost:8888"
url="http://localhost:8082"

#x=`cat req.json`
x="$1"

if [[ "$x" == "" ]] ; then
  echo "provide filename"
  exit 1
fi

cat $x

echo "---"

#time curl -H 'Content-Type: application/json' -X POST -d "$x" $url
#time curl -H 'Content-Type: application/json' -X POST -d @"$x" $url
#curl -s -H 'Content-Type: application/json' -X POST --data-binary @$x $url | jq .

#curl -s -H 'Content-Type: application/json' -X POST --data-binary @$x $url | jqf --fold 32 .
curl -s -H 'Content-Type: application/json' -X POST --data-binary @$x $url
