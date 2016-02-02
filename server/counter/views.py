from django.shortcuts import render
from .models import Counter
from django.http import HttpResponse

# Create your views here.
def query(request):
    counter = Counter.objects.get(pk=1)
    return HttpResponse(str(counter.count))

def increment(request):
    counter = Counter.objects.get(pk=1)
    counter.count = counter.count+1
    counter.save()
    return HttpResponse(str(counter.count))

def index(request):
    return increment(request)

def interactive(request):
    counter = Counter.objects.get(pk=1)
    return render(request, "interactive.html", {'count': counter.count})
