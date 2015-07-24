---
layout: page 
title: Manual 
permalink: /manual/
---
{% for manual in site.manual %}
  <a href="{{manual.url}}">{{manual.title}}</a>
{% endfor %}
