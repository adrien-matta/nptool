---
layout: page 
title: Manual 
---
{% for manual in site.manual %}
  <a href="{{manual.url}}">{{manual.title}}</a>
{% endfor %}
