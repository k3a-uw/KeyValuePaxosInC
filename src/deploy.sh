#!/bin/bash
SRC_FOLDER="./*"
DST_FOLDER="~/win/"
DST="172.22.71."
NETID="danielkr"

case "$1" in
	d)
		for COUNTER in {27..37}
		do
        scp ${SRC_FOLDER} "${NETID}@${DST}${COUNTER}:${DST_FOLDER}"
        echo "${COUNTER} check."
        echo "Files distributed to nodes."
    	done
		;;
	sever)
		make
		./tcss558 server
		;;
	client)
		make
		./tcss558 client
		;;
	n02)
		ssh "${NETID}@${DST}27"
		;;
	n03)
		ssh "${NETID}@${DST}28"
		;;
	n04)
		ssh "${NETID}@${DST}29"
		;;
	n05)
		ssh "${NETID}@${DST}30"
		;;
	n06)
		ssh "${NETID}@${DST}31"
		;;
	n07)
		ssh "${NETID}@${DST}32"
		;;
	n08)
		ssh "${NETID}@${DST}33"
		;;
	n09)
		ssh "${NETID}@${DST}34"
		;;
	n10)
		ssh "${NETID}@${DST}35"
		;;
	n11)
		ssh "${NETID}@${DST}36"
		;;
	n12)
		ssh "${NETID}@${DST}37"
		;;
esac