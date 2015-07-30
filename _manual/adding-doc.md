---
layout: manual 
title: "Annex A: Adding documentation"
permalink: /manual/adding-documentation/
author: Adrien Matta <mailto:a.matta@surrey.ac.uk>
manual_order: 1000
show_in_nav: false
---
## Getting the gh-pages:

If you wish to add a new documentation section you first need to get the site source from github. The source are stored directly on the github repository, in a specific branch named gh-pages. Github automatically rebuild the website after each commit onto this branch. The change are visible a few minutes after the commit. It is therefore important to check your change locally first, and then only to push them to avoid breaking the website.

To get the pages you will need to clone the gh-pages branch in a new directory:

{% highlight console %}
$ mkdir gh-pages
$ cd gh-pages
$ git clone https://github.com/adrien-matta/nptool -b gh-pages
$ cd nptool
{% endhighlight %}  

## Getting Jekyll
You now have a local copy of the source. To preview your work you will need the version of [Jekyll][jekyll-gh] use by gh-pages (and therefore the gem utility):

{% highlight console %}  
$ gem install github-pages
{% endhighlight %}  

## Setting up locally
In order for your preview to be done locally, edit the file _config.yml by commenting line 8 and uncommenting line 9:

BEFORE:
{% highlight css linenos %}  
# Site settings
title: nptool
email: a.matta@surrey.ac.uk
description: > # this means to ignore newlines until "baseurl:"
  nptool, a Root and Geant4 framework for analysis and simulation of nuclear physics experiement.

baseurl: "" # the subpath of your site, e.g. /blog/
url: "http://nptool.org" # the base hostname & protocol for your site
#url: "" # use this line when using jekyll serve to preview locally

github_username: adrien-matta/nptool 

# Build settings
markdown: kramdown
include: [_manual]
source: ./
safe: true
lsi: false
# Defining collection
collections:
  manual:
    output: true
    permalink: /manual/:path/
    show_in_nav: false
{% endhighlight %}  



AFTER:
{% highlight css linenos %}  
# Site settings
title: nptool
email: a.matta@surrey.ac.uk
description: > # this means to ignore newlines until "baseurl:"
  nptool, a Root and Geant4 framework for analysis and simulation of nuclear physics experiement.

baseurl: "" # the subpath of your site, e.g. /blog/
#url: "http://nptool.org" # the base hostname & protocol for your site
url: "" # use this line when using jekyll serve to preview locally

github_username: adrien-matta/nptool 

# Build settings
markdown: kramdown
include: [_manual]
source: ./
safe: true
lsi: false
# Defining collection
collections:
  manual:
    output: true
    permalink: /manual/:path/
    show_in_nav: false
{% endhighlight %}  

IMPORTANT: This change will need to be reversed at the end.

## Generating a preview

In a terminal, start the Jekyll server. The server will provide a local image of the website, as well as keep looking at changes to update the page:

{% highlight console %}
$ jekyll serve
{% endhighlight %}   

If the build successed it will be available for preview in your web browser, usually at address 0.0.0.0:4000 (some time differ but Jekyll should tell your). If the build fail, Jekyll will show message in red telling you what the error is.

# Adding a new page to the manual

To create a new manual entry simply create a new file in the folder _manual. For example:

{% highlight console %}
$ vim _manual/NewSection.md 
{% endhighlight %}   

The file need to start (do not leave blank line at the top) with a specific header:

{% highlight text %}
---
layout: manual 
title: Your title
permalink: /manual/your-title/
author: your name
manual_order: 202
show_in_nav: false
---
{% endhighlight %}   

Those four line allow the website to know that your page goes in the manual category, the title under wich it should be display in the manual, the path that will show in the address bar, and that the page should not be shown in the top navigation bar. The manual order allow to place correctly the manual entry in the menu, the bigger the number, the later it will show in the menu. The ordering number does not need to be continuous, so we setup large jump in number for each of them, allowing easy addition in the future. The annexes, such as this page, start at 1000 with increment of 10. Normal entries, use increment of 100 starting from 0.


After that, the format of the text is [Markdown][markdown], that use easy to read marking to specify title and headings. Here is a small example of a file showing off some basic functionnality:

{% highlight text %}
---
layout: manual 
title: Showing-off
permalink: /manual/showing-off/
author: your name
show_in_nav: false
---
# Showing-off basics

## SubHeading

### SubSubHeading

#### SubSubSubHeading

## Typing some text

Just type your text!

## Highlighting code and command:

A Console line:
{{"{% highlight console "}}%}
$ git pull 
{{"{% endhighlight "}}%}   

A Console line embbeded in the text `$ git pull` is easy as well.

Some cpp code:
{{"{% highlight cpp "}}%}
 ClassBase* A = new ClasseA();
 delete A;
 return 0;
{{"{% endhighlight "}}%}   

And the same with line number:
{{"{% highlight cpp linenos"}}%}
 ClassBase* A = new ClasseA();
 delete A;
 return 0;
{{"{% endhighlight "}}%}   

{% endhighlight %}   

## Commiting your change:

First revert the change in _config.yml:

{% highlight console %}
$ git checkout _config.yml
{% endhighlight %}

Then add your new file to the repository:

{% highlight console %}
$ git add _manual/NewSection.md
{% endhighlight %}

Finnally commit the change:
{% highlight console %}
$ git commit -a
$ git push
{% endhighlight %}


[markdown]: http://daringfireball.net/projects/markdown/syntax
[jekyll-gh]: https://github.com/jekyll/jekyll
