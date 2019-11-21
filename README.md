ZX11 - a wrapper for interface library to the X Window System
=================================================================
Copyright (C) Tomomichi Sugihara (Zhidao) since 1999

-----------------------------------------------------------------
## [What is this?]

ZX11 is a library that fascilitates programming on the interface
library to the X Window System (X11) including:
- X Window system handling
- Xpm and sprite class manipulation
- pixel and XImage manipulations
- simple widget classes

It supports the following image formats.

(Readable/Writable)
- PNG (Portable Network Graphics)
- DIB (BMP, Microsoft Device Independent Bitmap)
- XPM (X Pixmap)
- PBM (Portable Bitmap)
- PGM (Portable Graymap)
- PPM (Portable Pixmap)

(Readable)
- JPEG
- MAG (MAki-chan Graphics)
- PDT10/11 (format for AVG32)

libX11 and ZEDA are required to be installed.
libXpm is required for the use of XPM.
libpng and libz are required for the use of PNG.
libjpeg is required for the use of JPEG.
X extention (libXext) is recommended to be installed.

-----------------------------------------------------------------
## [Installation / Uninstallation]

### install

Install libX11, ZEDA and other libraries required for the use of
mage formats in advance.

Move to a directly under which you want to install ZX11, and run:

   ```
   % git clone https://github.com/zhidao/zx11.git
   % cd zx11
   ```

Edit *config* file if necessary.

   **PREFIX** : specify a directory where the header files, the
   library and some utilities are installed. (default: ~/usr)
   - header files: $PREFIX/include/zx11
   - library file: $PREFIX/lib
   - utilities: $PREFIX/bin

   **CONFIG\_USE\_WIDGET** : to use original simple widgets in
   this library. CONFIG\_USE\_XPM has to be validated for it.

   **CONFIG\_USE\_XEXT** : to use X11 extension.
   libXext is required.

   **CONFIG\_USE\_XPM** : to use XPM. libXpm is required.

   **COFIG\_USE\_PNG** : to use PNG files.
   libpng and libz are required.

   **CONFIG\_USE\_JPEG** : to use JPEG file (read only).
   libjpeg is required.

   **CONFIG\_USE\_DIB** : to use Microsoft BMP files.

   **CONFIG\_USE\_MAG** : to use MAG files.

   **CONFIG\_USE\_PDT** : to use PDT files.

Then, make and install.

   ```
   % make && make install
   ```

### uninstall

Do:

   ```
   % make uninstall
   ```

which removes $PREFIX/lib/libzx11.so and $PREFIX/include/zx11.

-----------------------------------------------------------------
## [How to use]

When you want to compile your code *test.c*, for example, the following
line will work.

   ```
   % gcc `zx11-config -L` `zx11-config -I` test.c `zx11-config -l`
   ```

-----------------------------------------------------------------
## [Acknowledgement]

The author would express a great appreciate to JAGARL (Mr. Kazunori
Ueno) for his support to PDT operation.

-----------------------------------------------------------------
## [Contact]

zhidao@ieee.org
