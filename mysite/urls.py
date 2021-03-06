from django.conf.urls.defaults import *
from django.conf import settings
from DCT.views import upload

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

urlpatterns = patterns('',
    # Example:
    # (r'^mysite/', include('mysite.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # (r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # (r'^admin/', include(admin.site.urls)),
    # fua write
    # (r'^upload/$', 'DCT.views.upload'),
    (r'^upload/$', 'DCT.views.upload'),
    (r'^static_site/(?P<path>.*)$','django.views.static.serve', {'document_root': settings.MEDIA_ROOT})
)
