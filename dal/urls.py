from django.contrib import admin
from django.urls import path, include
from django.contrib.auth.views import PasswordChangeView
from django.contrib.auth import views as auth_views
from . import views

urlpatterns = [
    path('home', views.home, name='home'),
    path('sign_out', views.sign_out, name='sign_out'),
    path('sign_up', views.sign_up, name='sign_up'),
    path('activate/<uidb64>/<token>', views.activate, name='activate'),
    path('', views.sign_in, name='sign_in'),
    path('home/<hw_name>', views.home_work, name='home_work'),
    path('add_hw', views.add_hw, name='add_hw'),
    path('delete_hw', views.delete_hw, name='delete_hw'),
    path('account', views.account, name='account'),
    path('list_html/<hw_html>/<class_html>/<type_html>/<student_html>', views.list_html, name='list_html'),
    path('view_html/<hw_html>/<class_html>/<type_html>/<student_html>/<file_name>', views.view_html, name='view_html'),
    path('password_change/', views.change_password, name='change_password'),
    #path('compare_code/<hw_name>/<class_type>/<hw_type>', views.compare_code, name='compare_code'),
]
