#!/bin/bash
if [[ "$1"=="gdb" ]]; then
      qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512  -d int -audiodev none,id=noaudio
else
      qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512  -audiodev none,id=noaudio

fi
