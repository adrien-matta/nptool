---
layout: post
title:  "Major Update"
date:   2016-12-08 20:00
author: Adrien Matta
categories: nptool update
---

Following last week workshop it has been identified that a lot of small issues were encountered with the parsing of the input file. The archaic method we used needed an update for quite some time, and a new version has just been pushed on the server. The last tag to use the old style input file is NPTool.2.1.45, all version above that use the new style input file.

The major difference is that the new input file rely on a identation to identify block of data. The file is parse once and then the interpreted block are passed to each detector or event generator. If you encounter issue with existing input file, please check they are indented correctly and that no tab is used (indent using only spaces).

The new file also support unit, if units are absent, a warning is issued, and a default unit is applied (identical to the implied one used before).

The new ReadConfiguration method are about one tenth of the old one and almost identical for simulation and analysis, which will make developpement of futur detector a lot easier.

In some cases the input file format had to be modified slightly. However all input file in $NPTOOL/Inputs has been modified and tested.

Contact Adrien Matta at matta@lpccaen.in2p3.fr if you encounter any issue.
