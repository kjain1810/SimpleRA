cd src
for fn in $(find . -maxdepth 1 -type f -regex '.*\.h$' | cut -f 2 -d '/'); 
do 
   dn=$(echo $fn | cut -f 1 -d '.');
   sed -i  -e "1 i#ifndef ${dn}_H\n#define ${dn}_H" -e "$ a#endif" "$fn";  
done
