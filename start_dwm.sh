#! /bin/bash
# status bar 

# battery and time
FWBATT=0
SWBATT=0

BATT(){

    BATT=$(acpi -b | sed 's/.*[charging|unknown], \([0-9]*\)%.*/\1/gi')
    STATUS=$(acpi -b | sed 's/.*: \([a-zA-Z]*\),.*/\1/gi')
    if([ $BATT -le 15 ] && [ $STATUS == 'Discharging' ] && [ $FWBATT -eq 0 ]); then
	zenity --warning --text 'Low Battery $BATT%'
	FWBATT=1
    fi

    if([ $BATT -le 5 ] && [ $STATUS == 'Discharging' ] && [ $SWBATT -eq 0 ]); then
	zenity --warning --text 'Too Low Battery, $BATT%.'
    SWBATT=1
    fi
    echo "Batt:$BATT%"
}

Dat(){
    dat=$(date "+%d/%m/%y %R")
    echo $dat
}


while true
do
    #BATT=$(acpi -b | sed 's/.*[charging|unknown], \(0-9)*\)%.*/\1/gi')
     
    xsetroot -name "$(BATT)|$(Dat)"
    sleep 1m

done &
#pactl set-default-sink 0
dwm
