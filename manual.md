---
layout: manual 
title: Manual 
permalink: /manual/
show_in_nav: true
---

You will find in this section a link to the different pages of the nptool manual.

{% assign manual = site.manual | sort:"manual_order"  %}
{% for m in manual %}
  <a href="{{ m.url | replace:'' | prepend site.url,'' | prepend: site.baseurl | prepend: site.url }}">{{m.title}}</a>
{% endfor %}
