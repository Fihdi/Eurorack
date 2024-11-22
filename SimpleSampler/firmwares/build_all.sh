#!/bin/bash

START_DIR=$PWD

for dir in ./*/
do
	# Remove the trailing "/"
	dir=${dir%*/}

	# Check if there is a Makefile in this directory
	if [ -f ${dir}/Makefile ]; then
		echo " "
		echo " -------> Build '${dir##*/}'"
		cd "$dir" ; make

		# Copy binary
		cp build/*.bin ../../binaries/

		cd ..
	fi
done

echo "Done."
