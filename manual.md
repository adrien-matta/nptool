---
layout: manual 
title: Manual 
permalink: /manual/
show_in_nav: true
---

You will find in this section a link to the different pages of the nptool manual.

{% for manual in site.manual %}
  <a href="{{ manual.url | replace:'' | prepend site.url,'' | prepend: site.baseurl | prepend: site.url }}">{{manual.title}}</a>
{% endfor %}
