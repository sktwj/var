#!/bin/bash

PID=`ps aux | grep xclock | grep -v grep`
if [ "$PID" != "" ]; then
	pkill xclock	
fi

PID=`ps aux | grep xclock | grep -v grep`
if [ "$PID" = "" ]; then
	echo "kill complete!"
else 
	echo "kill failure!"
fi
