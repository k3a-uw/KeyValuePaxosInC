#!bin/bash

	SRC_FOLDER="/Users/daniel/git/TCSS558/src"
	DST_FOLDER="~/win/"
	DST="cssgate.insttech.washington.edu"
	NETID="danielkr"
	cd $SRC_FOLDER
	SRC_FOLDER="${SRC_FOLDER}/*"
    scp ${SRC_FOLDER} "${NETID}@${DST}${COUNTER}:${DST_FOLDER}"
	echo "DONE."
	ssh "${NETID}@${DST}"
