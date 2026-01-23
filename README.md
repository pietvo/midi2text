# MIDI to TEXT #

This is a revival of the programs mf2t and t2mf that I wrote starting in 1991, for the Atari ST (32-bit), MS-DOS (16-bit) and later Unix (probably 32-bits).

I recently picked this up again after some people asked me for executables for modern Windows (presumably 32- or 64-bit). I use a Mac, so I needed newer versions, for 64-bit OS's (in my case MacOS Ventura).

Modern C-compilers are much more picky than the Kernighan & Ritchie C-compilers that I first used. So the source code had to be adjusted to conform to more modern C standards (in this case C23). The bulk of this work had been done (as far as I know) by Mats Peterson (see below).

I also found a number of changed versions by different people, mainly for compiling on Windows 32-bit (for example Windows-XP, which still runs on Windows 11). I'll try to mention all people who contributed but some contributions were anonymous.

This repository contains as far as possible the history of the source code, but, for example, the Original RCS repository for the source code has been lost, so the history is a crude approximation.

All the text files have been converted to Unix format, using UTF-8 when ASCII isn't sufficient. If you need Windows format, please convert them yourselves.

## List of contributors ##

**NOTE:** Probably several email addresses are out of date. I have tried to replace the email addresses with current ones, and added website URLs where possible.
Some contributions have been superseded by later changes,

  * Tim Thompson, <me@timthompson.com> Original midi library. He has written a lot of MIDI software and compositions. See <https://timthompson.com/tjt/>.
  * Michael Czeiszperger <michael@czei.org> added writing capabilities to the midi library
  * Wiel Seuskens <wiel@wiels.com> http://wiels.com/ Amiga version (1994)
  * Erich Neuwirth <erich.neuwirth@univie.ac.at> Changes for GCC. (2003)
  * Norman Megill (deceased in 2021) Metamath https://us.metamath.org/mpeuni/mmmusic.html
  * Luca Ciciriello modified the program to run on MacOSX (ppc/Intel 386).
    Universal binaries can be found at https://us.metamath.org/downloads/mf2t.zip.
    Look in `mf2t/mf2t-Mac/Midi_MacOSX_Xcode/Midi_MacOSX_Xcode/` (2006)
    `midisrc` for source code, `midibin` for binaries. **Latest patches not applied**.
  * Nide Naoyuki (Japanese word order - Naoyuki Nide - European word order) <nide@ics.nara-wu.ac.jp>.
    Some patches which are superseded by Mats Peterson's version. (2009)
    From https://groups.google.com/g/fj.sources/c/MGThNnFxpEw
  * Vaclav Spirhanzl <vjs.blog@gmail.com>: Mac version, change of using strerror instead of sys_errlist (2011)
    From https://en.freedownloadmanager.org/Mac-OS/MacMIDIt2mf-FREE.html
    https://macdownload.informer.com/macmidit2mf/download/
    These changes are included in Mats Peterson's version.
  * Yin Dian <yindian@gmail.com> Some bugfixes, superseded by below versions (2014).
    https://github.com/yindian/midi2text
  * Mats Peterson <mats.peterson@gmail.com> C-99 compatible + other changes (open files).
    From https://github.com/codenotes/mf2t version 20150711. Ported to Windows.
  * Phil Budne <phil@ultimate.com> https://github.com/philbudne/mf2t (2021)
    Various changes, of which I used a few, especially using `perror` instead of `fprintf(stderr, ... strerror...`.

-------------------------------------------------------------------------------

Pieter van Oostrum  
(formerly Piet van Oostrum)  
email: pieter@vanoostrum.org  
www: <https://pieter.vanoostrum.org>
