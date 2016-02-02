from __future__ import unicode_literals

from django.db import models

# Create your models here.
class Counter(models.Model):
    name = models.CharField(max_length=40)
    count =  models.IntegerField()
        
