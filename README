ZX11 - a wrapper for interface library to the X Window System
Copyright (C) 1999 Tomomichi Sugihara (Zhidao)

-----------------------------------------------------------------
[What is this?]

ZX11 is a library that fascilitates programming on the interface
library to the X Window System (X11) including:
 - X Window system handling
 - Xpm and sprite class manipulation
 - pixel and XImage manipulations
 - simple widget classes

It supports the following image formats.

(Readable/Writable)
PNG (Portable Network Graphics)
DIB (BMP, Microsoft Device Independent Bitmap)
XPM (X Pixmap)
PBM (Portable Bitmap)
PGM (Portable Graymap)
PPM (Portable Pixmap)

(Readable)
JPEG
MAG (MAki-chan Graphics)
PDT10/11 (format for AVG32)

libX11 and ZEDA are required.
libXpm is required for the use of XPM.
libpng and libz are required for the use of PNG.
libjpeg is required for the use of JPEG.
X extention (libXext) is recommended to be installed.

-----------------------------------------------------------------
[Installation / Uninstallation]

<install>
0. Install libX11, ZEDA and other libraries required for the use
   of image formats in advance.

1. Unpack the distributed archive where you want.

% zcat zx11-X.Y.Z.tgz | tar xvf
or, if you use GNU tar,
% tar xzvf zx11-X.Y.Z.tgz

X.Y.Z is for the revision.

2. Enter the directory.

% cd zx11-X.Y.Z

3. Edit config file if necessary.
  PREFIX          directory where the library is installed.
                  ~/usr as a default. In this case, header files
                  and library are installed under ~/usr/include
                  and ~/usr/lib, respectively.

  CONFIG_USE_WIDGET to use original simple widgets in this library.
                  Validate CONFIG_USE_XPM for it.

  CONFIG_USE_XEXT to use X11 extension. libXext is required.

  CONFIG_USE_XPM  to use XPM. libXpm is required.

  COFIG_USE_PNG   to use PNG files. libpng and libz are required.

  CONFIG_USE_JPEG to use JPEG file(read only). libjpeg is required.

  CONFIG_USE_DIB  to use Microsoft BMP files.

  CONFIG_USE_MAG  to use MAG files.

  CONFIG_USE_PDT  to use PDT files.

  Initially, the whole options are validated.

4. Make it.

% make

5. Install it.

% make install

Or,

% cp -a lib/libzx11.so $PREFIX/lib/
% cp -a include/zx11 $PREFIX/include/
% cp -a bin/* $PREFIX/bin/

<uninstall>
Delete $PREFIX/lib/libzx11.so and $PREFIX/include/zx11.

-----------------------------------------------------------------
[How to use]

You may need to set your PATH and LD_LIBRARY_PATH environment
variables. This is done by:
 export PATH=$PATH:$PREFIX/bin
 export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PREFIX/lib
if your working shell is Bourne shell (bash, zsh, etc.), or by:
 set path = ( $path $PREFIX/bin )
 setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH:$PREFIX/lib
if your working shell is C shell (csh, tcsh, etc.).

When you want to compile your code test.c, for example, the following
line will work.

% gcc `zx11-config -L` `zx11-config -I` test.c `zx11-config -l`

-----------------------------------------------------------------
[Acknowledgement]

The author would express a great appreciate to JAGARL (Mr. Kazunori
Ueno) for his support to PDT operation.

-----------------------------------------------------------------
[Contact]

zhidao@ieee.org
