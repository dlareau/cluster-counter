from django.shortcuts import render
from .models import Counter
from django.http import HttpResponse

# Create your views here.
def query(request):
    counter = Counter.objects.get(pk=1)
    return HttpResponse(str(counter.count))

def increment(request):
    client_ip = request.META['REMOTE_ADDR']
    counter = Counter.objects.get(pk=1)
    if(len(client_ip) == 13 and client_ip[0:10] == '128.2.100.' and
       int(client_ip[-3:]) >= 134 and int(client_ip[-3:]) <= 157):
        counter.count = counter.count+1
        counter.save()
    return HttpResponse(str(counter.count))

def index(request):
    return increment(request)

def interactive(request):
    client_ip = request.META['REMOTE_ADDR']
    counter = Counter.objects.get(pk=1)
    button = False
    if(len(client_ip) == 13 and client_ip[0:10] == '128.2.100.' and
       int(client_ip[-3:]) >= 134 and int(client_ip[-3:]) <= 157):
        button = True
    return render(request, "interactive.html", {'count': counter.count, 'button': button})
