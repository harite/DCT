from django import forms
from DCT.models import Media

class MediaForm(forms.ModelForm):
    class Meta:
        model = Media
