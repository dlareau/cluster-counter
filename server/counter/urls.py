from django.conf.urls import url
from . import views

urlpatterns = [
        url(r'^$', views.interactive, name='index'),
        url(r'^query/$', views.query, name='query'),
        url(r'^query$', views.query, name='query'),
        url(r'^increment/$', views.increment, name='increment'),
        url(r'^interactive/$', views.interactive, name='interactive'),
    ]
