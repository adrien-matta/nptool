---
layout: manual 
title: Status 
permalink: /status/
show_in_nav: true
---

The current status of the nptool repository as given by [Travis CI][travis] is the following:

* NPTool.2.dev: ![NPTool logo](https://travis-ci.org/adrien-matta/nptool.svg?branch=NPTool.2.dev)
* NPTool.pro: coming soon

You can check the build history of the project and the log with error for all of them on the [Page][build-log].
This status is also reported in the footer of each pages of this website.


What the status means:

* Automatic test the branch NPTool.2.dev with a virtual machine:
  * Ubuntu 12.04  
  * gcc 4.8
  * root-system debian package installed through apt-get
  * Geant4 10.1.2 (minimal so no QT and no GDML)
* Compile NPLib and run a first minimal test
* Run Example1 and Example2 with reference data file
* Compile NPS and run npsimulation and npanalysis for Example2

If all of this step succeed, the status will appears as passing, if any of those step fail, the status will appear as failling.

[build-log]: https://travis-ci.org/adrien-matta/nptool/builds
[travis]: https://travis-ci.org/
