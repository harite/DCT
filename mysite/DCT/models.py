from django.db import models
from django.forms import ModelForm
import datetime

# Create your models here.

class Media(models.Model):
    file = models.FileField(upload_to="file/%Y/%m/%d/")
