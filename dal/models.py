from django.db import models

class UploadedFile(models.Model):
    file = models.FileField(upload_to='uploads/')
    uploaded_at = models.DateTimeField(auto_now_add=True)
    
class Program(models.Model):
    name = models.CharField(max_length=100)
    state = models.CharField(max_length=100)
    view_path = models.CharField(max_length=200, null=True)
    hw_name = models.CharField(max_length=200, null=True)
    def __str__(self):
        return self.name