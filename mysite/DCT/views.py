import os
from django.shortcuts import get_object_or_404, render_to_response
from django.http import HttpResponse, Http404, HttpResponseRedirect
from django.template import Context, loader, RequestContext
from django.core.urlresolvers import reverse
from django.template.context import RequestContext
from DCT.models import Media
from DCT.upload import MediaForm

# Create your views here.

def upload(request):
    if request.POST or request.FILES:
        form = MediaForm(request.POST, request.FILES)
        if form.is_valid(): 
            m = Media(file=form.cleaned_data['file'],)
            m.save()
    else:
        form = MediaForm()

    medias = Media.objects.filter()
##no csrf##
#    return render_to_response('polls2/upload.html', {'form':form, 'medias':medias})

##csrf## 
    return render_to_response('html/upload.html',
                              {'form': form,
                               'medias': medias}, RequestContext(request))
