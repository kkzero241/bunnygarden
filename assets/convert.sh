#!/bin/sh

grit bunspr.png -ftb -fh! -gTFF00FF -gt -gB8 -m!

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img ../nitrofiles/sprite

grit nfl.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit bunnybg.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit cubetextest.png -gx -gb -gB16 -gT! -o bunnycube.bin

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img *.map ../nitrofiles/bg
