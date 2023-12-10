import subprocess
import os
import shutil
from .models import Program
from difflib import HtmlDiff

"""
class Program:
    def __init__(self, name: str) -> None:
        self.name = name
        self.state = None
        self.error_message = None
"""

class dal_util:
    
    def compile_one(dir, program_list: Program, view_file_dest, hw_name):
        cwd = os.getcwd()
        list_dir = os.listdir(dir)
        if 'result.txt' not in list_dir:
            os.chdir(dir)
            for file in list_dir:
                if file.endswith('.cpp'):
                    exe_file = file.replace('.cpp','')
                    result = subprocess.run(["g++", file, "-o", exe_file], cwd=os.getcwd(), capture_output=True)
                    program = Program(name=exe_file)
                    if result.returncode == 0:
                        program.state = 'Compile success'
                        result = subprocess.run(["timeout", "1s", 'expect', '/home/ds/DAL-django/DAL/exp_script.exp', f"./{exe_file}", "../command.txt"], cwd=os.getcwd(), capture_output=True)
                        if result.returncode == 0:
                            with open("result.txt", '+w') as res:
                                res.write(result.stdout.decode('utf-8', errors='ignore').replace('\r', '').strip())
                        elif result.returncode == 124:
                            program.state = 'Runtime error'
                            with open("result.txt", '+w') as res:
                                res.write(result.stdout.decode('utf-8', errors='ignore').replace('\r', '').strip())
                        program.view_path = view_file_dest
                    else:
                        program.state = 'Compile error'
                        program.view_path = view_file_dest
                    program.hw_name = hw_name
                    program.save()
                    program_list.append(program)
            os.chdir(cwd)
        
                
    def compile_multiple(self,dir,program_list:Program, view_file_dest, hw_name):
        
        files = os.listdir(dir)
        for f in files:
            f_path = ''.join([dir, '/', f])
            if os.path.isdir(f_path):
                view_file_path = ''.join([view_file_dest, '/', f])
                self.compile_one(f_path, program_list, view_file_dest, hw_name)
                
    def compare(path_to_teacher, path_to_student, view_file_dest):
        
        
        #-------------Preprocessing-------------------#
        student_dir_files = os.listdir(path_to_student)
        
        student_dir_files.sort()
        for f in student_dir_files:
            if 'input' in f:
                student_dir_files.remove(f)    
            if '.cpp' in f:
                student_dir_files.remove(f)  
            if '.' not in f:
                student_dir_files.remove(f)
        
        
        teacher_dir_files = os.listdir(path_to_teacher)
        teacher_dir_files.sort()
        teacher_dir_files.remove('code.cpp')
        for f in teacher_dir_files:
            if 'input' in f:
                teacher_dir_files.remove(f)    
            if '.cpp' in f:
                teacher_dir_files.remove(f)  
            if '.' not in f:
                teacher_dir_files.remove(f)  
        #-----------------------------------------------#
        
        if len(student_dir_files) == 0:
            return False
        
        else:
            for i in range(len(teacher_dir_files)):
                if teacher_dir_files[i] in student_dir_files:
                    teacher_file = path_to_teacher + '/' + teacher_dir_files[i]
                    student_file = path_to_student + '/' + teacher_dir_files[i]
                    if os.path.exists(teacher_file) and os.path.exists(student_file):
                        with open(teacher_file, 'r') as t, open(student_file, 'r') as s:
                            teacher_output = t.read().split('\n')
                            student_output = s.read().split('\n')
                            diff = HtmlDiff().make_file(teacher_output, student_output)

                            diff_file_path = ''.join([view_file_dest, '/', 
                                                      teacher_dir_files[i],
                                                      '.html'
                                                      ])
                            with open(diff_file_path, '+w') as d:
                                d.write(diff)
                      
                else:
                    teacher_file = path_to_teacher + '/' + teacher_dir_files[i]
                    if os.path.exists(teacher_file):
                        with open(teacher_file, 'r') as t:
                            teacher_output = t.read().split('\n')
                            student_output = "學生的程式碼不產生此檔案\n學生的程式碼不產生此檔案\n學生的程式碼不產生此檔案\n學生的程式碼不產生此檔\n".split('\n')
                            diff_file_path = ''.join([view_file_dest, '/', 
                                                      teacher_dir_files[i],
                                                      '.html'
                                                      ])
                            diff = HtmlDiff().make_file(teacher_output, student_output)
                            with open(diff_file_path, '+w') as d:
                                d.write(diff)
            return True
                                
        return False
        
                    

        
