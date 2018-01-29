#!/usr/local/cs/bin/bash

cd /usr/bin
commands=`ls /usr/bin | awk 'NR%101==804793446%101'`

for command in $commands
do
    echo "ldd $command"
    ldd $command
done
