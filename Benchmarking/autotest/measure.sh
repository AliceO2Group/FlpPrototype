#!/bin/bash
CPUnl=13

if [ "$#" -lt 2 ]; then
    echo "Usage: ${0} <NIC> <cpuList>"
    echo -e "\t NIC - name of interface"
    echo -e "\t cpuList - list of CPUs seperated with comma"
    exit
fi

echo `date +%Y-%m-%d`
echo "Started measurements of: throguhput(${1}) and CPU (${2})"
echo `uname -r`
echo `lscpu | grep 'name\|CPU(s)'`

# sets up initial values for receive and trasmit 
initNet=`grep ${1} /proc/net/dev`
initNetArr=($initNet)
initReceive=${initNetArr[1]}
initTransmit=${initNetArr[9]}

#counts number of commas in cpuList string
CPUNO=$(($(grep -o "," <<< ${2} | wc -l) + 1))

while true 
do
	# Get number of bytes transmitted and received
	net=`grep ${1} /proc/net/dev`
	# 1 - bytes received, 9 - bytes transmitted
	netArr=($net)	
	
	# Collects CPU usage for two CPUs
	cpu=`mpstat -P ${2} | tail -n${CPUNO}`
	
	# parse result to array
	# 2-CPU, 3-usr, 4-nice, 5-sys, 6-iowait, 7-irq, 8-soft, 9-steal, 10-guest, 11-gnice, 12-idle
	# +12 for next line
	cpuArr=($cpu)
	
	# seconds since the epoch
	time=`date +%s`
	
	I=0
	cpuString=""
	while [  $I -lt $CPUNO ]; do
		prefix=$((I * $CPUnl))
		cpuString+="{\"number\": ${cpuArr[$prefix + 2]}, \"usr\": ${cpuArr[$prefix + 3]}, \"sys\": ${cpuArr[$prefix + 5]}, \"irq\": ${cpuArr[$prefix + 7]}, \"idle\": ${cpuArr[prefix + 12]]}},"
		let I=$I+1 
	done
	
	echo "{\"time\": ${time}, \"net\":{\"receive\": $((${netArr[1]}-initReceive)), \"transmit\": $((${netArr[9]}-initTransmit)), \"cpu\": [${cpuString::-1}]}}"
	
	sleep 1
done
