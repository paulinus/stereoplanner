for i in *.svg; do
	inkscape $i --export-pdf `basename $i .svg`.pdf
done
