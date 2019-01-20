#!/bin/bash
# Grep the log file for # of frames in source video
# Usage: scriptname <logfilename>
egrep '[0-9]+\ frames.*test[0-9][0-9].mp4' $1
for csvfile in `ls tensorflow/examples/label_image/predictions/*.csv`
do
	echo "Processing file $csvfile"
	echo "$csvfile # of Frames predicted: `tail -1 $csvfile | cut -d',' -f 1`"
done
