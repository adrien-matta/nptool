---
layout: manual 
title: Manual 
permalink: /manual/
show_in_nav: true
---

{% assign manual = site.manual | sort:"manual_order"  %}
{% for m in manual %}
  {% if m.manual_order == 0 %}
   
   <h3> Framework: </h3>

  The framework detailed from installation to workflow.
  {% endif %} 
 
  {% if m.manual_order == 100 %}

  <h3> Detector: </h3> 
  
  The specific of each detector (input file, reference, ...).
  {% endif %} 
  
  {% if m.manual_order == 1000 %}
   
  <h3> Annex: </h3> 
  
  Everything not directly related to nptool.
  {% endif %} 

  
  * <a href="{{ m.url | replace:'' | prepend site.url,'' | prepend: site.baseurl | prepend: site.url }}">{{m.title}}</a>
{% endfor %}
