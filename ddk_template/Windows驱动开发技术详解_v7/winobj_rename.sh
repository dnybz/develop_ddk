#!/bin/bash

for each in $( find . -type f |grep tpl$)
do
	grep '\\Device\\'  $each 
done


#HelloDDK

#      1 MyDDKDevice2
#      4 MyDDKDeviceB
#     22 MyDDKDeviceA
#     36 MyDDKDevice
#      6 MyWDMDevice
#      8 MyDDKDevicB



	for each in $( find . -type f |grep tpl$); 
	do  echo $each;     
	sed '/RtlInitUnicodeString/{s/MyWDMDevice/\[%Name%\]/}'   $each -i ; 
	sed '/RtlInitUnicodeString/{s/MyDDKDevice/\[%Name%\]/}'   $each -i ; 
	sed '/RtlInitUnicodeString/{s/MyDDKDevic/\[%Name%\]/}'   $each -i ; 
	done  



function delete_create_link()
{
	for each in $( find . -type f |grep tpl$); 
	do  echo $each;     
	sed '/RtlInitUnicodeString/{s/HelloDDK/\[%Name%\]/}'   $each -i ; 
	done  
}
