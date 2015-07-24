---
layout: page 
title: Manual 
---
{% for manual in site.manual %}
  <a href="{{ manual.url | replace:'' | prepend site.url,'' | prepend: site.baseurl | prepend: site.url }}">{{manual.title}}</a>
{% endfor %}
