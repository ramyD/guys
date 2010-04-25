#!/bin/sh
# Lists the contents of a directory
# requiers dialog package
# todo: check if dialog package is there, advise otherwise
# use a gtk/X dialogue when packaging into debs
dialog --title "Twitter Account!" \
   --inputbox "Enter your Username:" 8 40 `pwd`\
   2>./dialog.ans
if [ $? = 1 ]; then
   clear
   exit 0
fi
ANS=`cat ./dialog.ans`

echo 1234567890 > File
[i]<>./dialog.ans
exec 3<> File 
read -n 4 <&3
echo -n . >&3
exec 3>&-
cat File


#ls -al $ANS > /tmp/dialog.ans
#dialog --no-shadow \
#   --title "listing of"$ANS \
#   --textbox /tmp/dialog.ans 25 78
#clear
#rm -f /tmp/dialog.ans # don't litter !
#exit 0
