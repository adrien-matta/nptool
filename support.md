---
layout: manual 
title: Support 
permalink: /support/
show_in_nav: true
---

<h3> Support: </h3>

If you encounter issues while installing or using the package, please sent an email to our support team <mailto:support@nptool.org>. Feel free to contact us for feedback, request features, or to take part in the collaboration.
 

<h3> Status: </h3>

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
