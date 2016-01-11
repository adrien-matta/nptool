---
layout: manual 
title: Manual 
permalink: /manual/
show_in_nav: true
---

{% assign manual = site.manual | sort:"manual_order"  %}
{% for m in manual %}
  {% if m.manual_order == 0 %}
   <h3> Getting started </h3>
  Important information on how to install and use nptool is given here.
  {% endif %} 
 
  {% if m.manual_order == 10 %}
   <h3> Tutorials </h3>
  Tutorials are listed here.
  {% endif %} 
 
  {% if m.manual_order == 100 %}
  <h3> Detectors documentation </h3> 
  The documentation for specific detectors is given here.
  {% endif %} 
  
  {% if m.manual_order == 1000 %}
  <h3> Annex </h3> 
  Everything not directly related to nptool.
  {% endif %} 

  
  * <a href="{{ m.url | replace:'' | prepend site.url,'' | prepend: site.baseurl | prepend: site.url }}">{{m.title}}</a>
{% endfor %}
