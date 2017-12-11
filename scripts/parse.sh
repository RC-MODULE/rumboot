arm-rcm-eabihf-objdump -t rumboot-basis-Production-simple-rom-arinc_loopback  | awk '{if($3=="F") print $1,$6}' > ./outputfile.txt
python ./parse.py
