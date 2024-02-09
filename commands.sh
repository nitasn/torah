
# print torah block
od -An -vtu1 torah.block | head -5

# convert iso-8859-8 to utf-8
iconv -f ISO-8859-8 -t UTF-8 books-iso-8859-8/Bereshit.txt > output_file.txt

make run | iconv -f ISO-8859-8 -t UTF-8

make > /dev/null; printf "> "; read pat; echo $pat | iconv -f UTF-8 -t ISO-8859-8 | build/main
