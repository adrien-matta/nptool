---
layout: manual 
title: Manual 
permalink: /manual/
show_in_nav: true
---
{% for manual in site.manual %}
  <a href="{{ manual.url | replace:'' | prepend site.url,'' | prepend: site.baseurl | prepend: site.url }}">{{manual.title}}</a>
{% endfor %}
