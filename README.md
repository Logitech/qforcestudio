QForceStudio - Explore Force Feedback on Linux
==============================================


QForceStudio is a Qt application with graphical editors for various
force effects. It is meant to bridge the gap between visualization of
a force and actual parameters sent to the kernel. The main purpose is
to be used as a development and testing tool for Linux Force Feedback
kernel drivers.


How to build on Linux
---------------------

* Install Qt and make sure to have QTDIR point to the installation.

* Install cmake

* Run cmake

~~~
$ cd qforcestudio
$ mkdir build
$ cd build
$ cmake .. -G "Unix Makefiles"
$ make
~~~


How to build and run QForceStudio on OSX
----------------------------------------

Why? Because I'm doing a lot of development on OSX and it's handy to
develop the UI aspects natively and not inside a VM. Device support is
non-existant though and forces will not play. But it's faster to develop
the widgets and debug the signals.


* Install Homebrew

~~~
from http://brew.sh
~~~

* Install Qt, make sure to have the environment variable QTDIR point
  to it.

~~~
$ brew install qt
~~~

* Install cmake and ninja

~~~
$ brew install cmake
$ brew install ninja
~~~

* Generate the make files and run ninja.

~~~
$ cd qforcestudio
$ mkdir build
$ cd build
$ cmake .. -G Ninja
$ ninja
~~~

The resulting application will run on OSX and one can test the UI and
all the various force effect parameters in the previews. There will
however be no device enumeration and hence there's no
downloading/starting of any force effects on OSX.
