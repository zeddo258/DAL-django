from django.shortcuts import render, redirect
from django.http import HttpResponse

from django.contrib import messages
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.models import User
from django.contrib.sites.shortcuts import get_current_site
from django.contrib.auth.views import PasswordChangeView

from django.core.mail import EmailMessage, send_mail
from django.template.loader import render_to_string
from django.utils.http import urlsafe_base64_decode, urlsafe_base64_encode
from django.utils.encoding import force_bytes, force_str
from django.urls import reverse_lazy

from DAL import settings

from . models import UploadedFile, Program
from . forms import UploadFileForm
from . tokens import generate_token
from . hwutil import hwutil
from . dalutil import dal_util, Program

import shutil
import random
import string
import os 



def account(request):
    return render(request, '404.html')


def delete_hw(request):
    if request.user.is_authenticated:
        
        if request.method == 'POST':
            hw_name = request.POST.get('hw_name')
            try:
                shutil.rmtree('./home_work/' + hw_name)
                shutil.rmtree('./templates/' + hw_name)
                Program.objects.filter(hw_name=hw_name).delete()
            except OSError as e:
                print(e)
            return redirect('delete_hw')
        detail = {}
        detail['user'] = request.user
        detail['home_work'] = hwutil.get_all_hw() 
        detail['home_work'].remove('uploads')
        
        return render(request, 'delete_hw.html', detail)
    else: 
        return redirect('sign_in')

def add_hw(request):
    if request.user.is_authenticated:
        
        if request.method == 'POST':
            form = UploadFileForm(request.POST, request.FILES)
            hw_name = request.POST.get('hw_name')
            hw_list = hwutil.get_all_hw()
            if hw_name not in hw_list:
                if form.is_valid():
                    hwutil.create_hw(hw_name)
                    for uploaded_file in request.FILES.getlist('files'):
                        UploadedFile.objects.create(file=uploaded_file)
                    hwutil.process_test_files(hw_name)
                    return redirect('add_hw')
        else:
            form = UploadFileForm()
            
        detail = {}
        detail['user'] = request.user
        detail['form'] = form
        detail['home_work'] = hwutil.get_all_hw() 
        detail['home_work'].remove('uploads')
        return render(request, 'add_hw.html', detail)
    
    else:
        return redirect('sign_in')

def home_work(request, hw_name):
    program_list = []
    detail = {}
    detail['user'] = request.user
    detail['command_exist'] = True
    detail['std_code_exist'] = True
    detail['view_upload_code'] = True
    detail['view_code'] = False
    detail['selected'] = False
    if request.user.is_authenticated:
        if request.method == 'POST':
            
            # Get data from html name tag
            form = UploadFileForm(request.POST, request.FILES)
            class_type = request.POST.get('class')
            hw_type = request.POST.get('type')
            class_show = request.POST.get('class_show')
            type_show = request.POST.get('type_show')
        
            
            # If upload field is not empty
            if class_type is not None:
                code_file_dest = ''.join(['./home_work/', hw_name, '/', class_type, '/', hw_type])
                view_file_dest = ''.join(['./templates/', hw_name, '/', class_type, '/', hw_type])
                data_file_path = ''.join(['./home_work/', hw_name, '/src'])
                if form.is_valid():
                    for uploaded_file in request.FILES.getlist('files'):
                        UploadedFile.objects.create(file=uploaded_file)
                    hwutil.process_src_files(hwutil, code_file_dest, data_file_path, view_file_dest)
                    # Handle files that got upload to '/uploads'
                    # should be empty afterwardth, view_file_dest)

                    if not hwutil.check_for_requirement(code_file_dest, detail):
                            detail['form'] = form
                            detail['home_work'] = hwutil.get_all_hw() 
                            detail['home_work'].remove('uploads')
                            return render(request, 'hw_index.html', detail)
                    dal_util.compile_multiple(dal_util, code_file_dest, program_list, view_file_dest, hw_name)
                    
                    files = os.listdir(code_file_dest)
                    if ('std_code' in files ) and 'command.txt' in files:
                        files.remove('std_code')
                        files.remove('command.txt')
                    
                    teacher_res = ''.join([code_file_dest, '/', 'std_code'])
                    for f in files:
                        student_res = ''.join([code_file_dest, '/', f])
                        student_diff = ''.join([view_file_dest, '/', f])
                        if os.path.exists(student_diff):              
                            flag = dal_util.compare(teacher_res, student_res, student_diff)

            # If view field is not empty
            elif class_show is not None: 
                view_file_dest = ''.join(['./templates/', hw_name, '/', class_show, '/', type_show])
                html_list = []
                if (os.path.exists(view_file_dest)):
                    html_list = Program.objects.filter(view_path=view_file_dest)
            
                detail['hw_html'] = hw_name
                detail['class_html'] = class_show
                detail['type_html'] = type_show
                
                detail['form'] = form
                detail['home_work'] = hwutil.get_all_hw() 
                detail['home_work'].remove('uploads')
                detail['html_list'] = html_list
                        
                detail['program_list'] = program_list
                detail['view_code'] = True
                detail['view_upload_code'] = False
                detail['type_show'] = type_show
                detail['class_show'] = class_show
                detail['selected'] = True
                return render(request, 'hw_index.html', detail)
        else:
            form = UploadFileForm()

        detail['form'] = form
        detail['home_work'] = hwutil.get_all_hw() 
        detail['home_work'].remove('uploads')
        detail['program_list'] = program_list
        detail['view_code'] = False
        detail['view_upload_code'] = True
        return render(request, 'hw_index.html', detail)
    else:
        return redirect('sign_in')

def home(request):
    if request.user.is_authenticated:
        detail = {}
        detail['user'] = request.user
        detail['home_work'] = hwutil.get_all_hw()
        detail['home_work'].remove('uploads')
        return render(request, "index.html", detail)
    else:
        return redirect('sign_in')

def sign_out(request):
    logout(request)
    return redirect('sign_in')

def sign_in(request): 
    detail = {}
    detail['incorrect'] = False
    if request.method == 'POST':
        email = request.POST.get('email')
        password = request.POST.get('password')
        user = authenticate(request, username=email, password=password)
        
        if user is not None:
            login(request, user)
            return redirect('home')
        else:
            detail['incorrect'] = True
            detail['username'] = email
            return render(request, 'sign_in.html', detail)
        
    return render(request, "sign_in.html", detail)


def sign_up(request):
    if request.user.is_authenticated:
        detail = {}
        detail['user_exist'] = False
        detail['home_work'] = hwutil.get_all_hw()
        detail['home_work'].remove('uploads')
        detail['user'] = request.user
        if request.method == 'POST':
            # Account information
            user_name = request.POST.get('email')
            password = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(10))
            
            # If account already exist
            if User.objects.filter(username=user_name):
                detail['user_exist'] = True
                return render(request, "sign_up.html", detail)
            
            # Create account
            user = User.objects.create_user(username=user_name, password=password)
            user.is_active = False
            user.save()
            
            current_site = get_current_site(request)
            email_subject = "Confirmation email for DAL"
            message = render_to_string('email_confirmation.html', {
                'mail' : user.username,
                'domain' : current_site.domain,
                'uid' : urlsafe_base64_encode(force_bytes(user.pk)),
                'token' : generate_token.make_token(user),
                'password' : password, 
            })
            
            send_mail(email_subject, message, settings.EMAIL_HOST_USER,[user.username], fail_silently=True)
            
        return render(request, "sign_up.html", detail)
    
    else:
        return redirect('sign_in')

def activate(request, uidb64, token):
    try:
        uid = force_str(urlsafe_base64_decode(uidb64))
        user = User.objects.get(pk=uid)
    except (TypeError, ValueError, OverflowError, User.DoesNotExist):
        user = None
        
    if user is not None and generate_token.check_token(user, token):
        user.is_active = True 
        user.save()
        return redirect('sign_in')
    else:
        
        return render(request, 'activation_fail.html')
    

def list_html(request, hw_html, class_html, type_html, student_html):
    if request.user.is_authenticated:

        path = ''.join(['./templates/', hw_html, '/', class_html, '/', type_html, '/', student_html])
        html_list = []
        if os.path.exists(path):
            for f in os.listdir(path):
                if 'input' not in f:
                    html_list.append(f)
        html_list.sort()
                
        return render(request, 'list.html', {'html_list': html_list, 
                                             'hw_html' : hw_html,
                                             'class_html' : class_html, 
                                             'type_html' : type_html,
                                             'student_html' : student_html,
                                             'user' : request.user
                                             })
    else:
        redirect('sign_in')
        
def view_html(request, hw_html, class_html, type_html, student_html, file_name):
    if request.user.is_authenticated:
        path = ''.join([hw_html, '/', class_html, '/', type_html, '/', student_html, '/', file_name])
        return render(request, path)
    else:
        redirect('sign_in')