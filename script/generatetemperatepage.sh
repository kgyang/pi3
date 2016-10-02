#!/bin/bash

LOG_FILE=$1
HOMEPAGE_DIR=/$HOME/kgyang.github.io
TEMP_PAGE_FILE=index.html

write_to_html()
{
	echo $1 >> $HOMEPAGE_DIR/$TEMP_PAGE_FILE
}

[[ -n "$LOG_FILE" && -f "$LOG_FILE" ]] || exit 1

[ -d $HOMEPAGE_DIR ] || {
	git clone https://github.com/kgyang/kgyang.github.io || exit 1
	cd $HOMEPAGE_DIR
	git config credential.helper store
	cd -
}

rm -rf $HOMEPAGE_DIR/$TEMP_PAGE_FILE

write_to_html "<table border=\"1\">"
write_to_html "<tr>"
write_to_html "<th>PI's Temperature Data</th>"
write_to_html "</tr>"
write_to_html "<tr>"
write_to_html "<td>Date</td>"
write_to_html "<td>Temperature</td>"
write_to_html "<td>Humidity</td>"
write_to_html "</tr>"
while read temp_data
do
	date="$(awk '{print $1,$2,$3,$4,$5,$6}' <<< $temp_data)"
	temperature="$(awk '{print $8}' <<< $temp_data)"
	humidity="$(awk '{print $10}' <<< $temp_data)"
	write_to_html "<tr>"
	write_to_html "<td>$date</td>"
	write_to_html "<td>$temperature</td>"
	write_to_html "<td>$humidity</td>"
	write_to_html "</tr>"
done < $LOG_FILE
write_to_html "</table>"

cd $HOMEPAGE_DIR
git commit -a -m "update PI temperature"
git push


