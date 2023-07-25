#!/bin/sh

MODE=x$1
TIMES=$2
DATAFILE=$3
NUMTHREADS=$4

case $MODE in
	xserial)
		echo "run serial"
		make clean
		make serial_mode

		for i in `seq $TIMES`; do
			./connect_flow_serial < $DATAFILE
			sleep 1
		done
		;;
	xthread)
		echo "run thread"
		make clean
		make thread_mode

		for i in `seq $TIMES`; do
			./connect_flow_thread_mode -n $NUMTHREADS < $DATAFILE
			sleep 3
		done
		;;
	xopenmp)
		echo "run openmp thread"
		make clean
		make openmp_mode

		for i in `seq $TIMES`; do
			./connect_flow_thread_openmp -n $NUMTHREADS < $DATAFILE
			sleep 1
		done
		;;
	*)
esac
