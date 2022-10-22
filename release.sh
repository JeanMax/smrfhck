#!/bin/bash -xe

# TODO: improve that shit

SMRF_EXE_PREFIX=smrfhck
SMRF_ICO=smrf.ico
WINE_PREFIX="$HOME/Games/battlenet"
RESOURCEHACKER_EXE="$WINE_PREFIX/drive_c/Program Files (x86)/Resource Hacker/ResourceHacker.exe"

for i in "" dev; do
    make $i
    OS=Windows_NT make $i
    if test "$i"; then
        exe="$SMRF_EXE_PREFIX"_$i.exe
    else
        exe="$SMRF_EXE_PREFIX".exe
    fi
    wine "$RESOURCEHACKER_EXE" \
         -open "$exe" \
         -save "$exe" \
         -action addoverwrite \
         -res "$SMRF_ICO" \
         -mask ICONGROUP,IDR_MAINFRAME,MAINICON
    cp "$exe" "$WINE_PREFIX"
done

MOUNT_DIR=$(mktemp -d)
WIN_PART=/dev/sda4
WIN_DST=/Users/mc/Desktop/smrfhck

sudo mount $WIN_PART "$MOUNT_DIR"
rsync --delete -rltDvu \
      --modify-window=1 \
      --exclude '*.[od]' \
      "$PWD/" "$MOUNT_DIR/$WIN_DST"
sync
sudo umount "$MOUNT_DIR"
