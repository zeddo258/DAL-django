import subprocess
import os
import shutil
from .models import Program, CompareFile
from difflib import HtmlDiff
import difflib

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
        
        # Check if directories already contain exe files 
        exe_file = ''
        for file in list_dir:
            if file.endswith('.cpp'):
                exe_file = file.replace('.cpp', '')
                break 
        
        # If code already been compiled:
        if exe_file not in list_dir:
            os.chdir(dir)
            for file in list_dir:
                if file.endswith('.cpp'):
                    # Compile
                    exe_file = file.replace('.cpp','')
                    result = subprocess.run(["g++", file, "-o", exe_file], cwd=os.getcwd(), capture_output=True)
                    Program.objects.filter(name=exe_file, view_path=view_file_dest).delete()
                    program = Program(name=exe_file)
                    if result.returncode == 0:
                        program.state = 'Compile success'
                        '''
                        # Run 
                        result = subprocess.run(["timeout", "1s", 'expect', '/home/ds/DAL-django/DAL/exp_script.exp', f"./{exe_file}", "../command.txt"], 
                                                cwd=os.getcwd(), 
                                                capture_output=True)
                        
                        # Write terminal output to result.txt 
                        if result.returncode == 0:
                            with open("result.txt", '+w') as res:
                                res.write(result.stdout.decode('utf-8', errors='ignore').replace('\r', '').strip())
                                
                        # 124 is runtime error code status
                        elif result.returncode == 124:
                            program.state = 'Runtime error'
                            with open("result.txt", '+w') as res:
                                res.write(result.stdout.decode('utf-8', errors='ignore').replace('\r', '').strip())
                        '''
                    else:
                        program.state = 'Compile error'
            
                    program.view_path = view_file_dest
                    program.hw_name = hw_name
                    program.save()
                    program_list.append(program)
                    
            os.chdir(cwd)
        
    # compile and execute all student code 
    def compile_multiple(self,code_file_dest,program_list:Program, view_file_dest, hw_name):
        
        files = os.listdir(code_file_dest)
        for f in files:
            f_path = ''.join([code_file_dest, '/', f])
            if os.path.isdir(f_path):
                # view_file_path = ''.join([view_file_dest, '/', f])
                self.compile_one(f_path, program_list, view_file_dest, hw_name)
                
    def execute(code_file_dest, view_file_dest, hw_name):
        cwd = os.getcwd()

        # code_file_dest = '['./home_work/', hw_name, '/', class_type, '/', hw_type]'
        files = os.listdir(code_file_dest)
        for f in files:
            # f_path = '['./home_work/', hw_name, '/', class_type, '/', hw_type, '/', student_id]'
            f_path = f_path = ''.join([code_file_dest, '/', f])
            if os.path.isdir(f_path):
                exec_dir = os.listdir(f_path)
                
                # if student code already been successfully compiled
                exec_file = ''
                for fs in exec_dir:
                    if fs.endswith('.cpp'):
                        exec_file = fs.replace('.cpp', '')
                        break 
                    
                
                if exec_file in exec_dir:
                    os.chdir(f_path)
                    program = Program.objects.get(hw_name=hw_name, view_path=view_file_dest, name=exec_file)
                    
                    result = subprocess.run(["timeout", "1s", 'expect', '/home/ds/DAL-django/DAL/exp_script.exp', f"./{exec_file}", "../command.txt"], 
                                                cwd=os.getcwd(), 
                                                capture_output=True)
                        
                    # Write terminal output to result.txt 
                    if result.returncode == 0:
                        with open("result.txt", '+w') as res:
                            res.write(result.stdout.decode('utf-8', errors='ignore').replace('\r', '').strip())
                                
                    # 124 is runtime error code status
                    elif result.returncode == 124:
                        program.state = 'Runtime error'
                        with open("result.txt", '+w') as res:
                            res.write(result.stdout.decode('utf-8', errors='ignore').replace('\r', '').strip())
                    program.save()
                    os.chdir(cwd)
                    
    def execute_reupload(code_file_dest, view_file_dest, hw_name):
        cwd = os.getcwd()

        # code_file_dest = '['./home_work/', hw_name, '/', class_type, '/', hw_type]'
        files = os.listdir(code_file_dest)
        for f in files:
            # f_path = '['./home_work/', hw_name, '/', class_type, '/', hw_type, '/', student_id]'
            f_path = ''.join([code_file_dest, '/', f])
            if os.path.isdir(f_path):
                exec_dir = os.listdir(f_path)
                
                exec_file = ""
                # if student code already been successfully compiled
                for fs in exec_dir:
                    if fs.endswith('.cpp'):
                        exec_file = fs.replace('.cpp', '')
                        break 
                    
                
                if exec_file in exec_dir and 'result.txt' not in exec_dir:
                    os.chdir(f_path)
                    program = Program.objects.get(hw_name=hw_name, view_path=view_file_dest, name=exec_file)
                    
                    result = subprocess.run(["timeout", "1s", 'expect', '/home/ds/DAL-django/DAL/exp_script.exp', f"./{exec_file}", "../command.txt"], 
                                                cwd=os.getcwd(), 
                                                capture_output=True)
                        
                    # Write terminal output to result.txt 
                    if result.returncode == 0:
                        with open("result.txt", '+w') as res:
                            res.write(result.stdout.decode('utf-8', errors='ignore').replace('\r', '').strip())
                                
                    # 124 is runtime error code status
                    elif result.returncode == 124:
                        program.state = 'Runtime error'
                        with open("result.txt", '+w') as res:
                            res.write(result.stdout.decode('utf-8', errors='ignore').replace('\r', '').strip())
                    program.save()
                    os.chdir(cwd)
                    

                
    def compare(path_to_teacher, path_to_student, view_file_dest, hw_name, view_path, student_id):
        
        
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
            print(student_id)
            program = Program.objects.get(view_path=view_path, name=student_id)
            for i in range(len(teacher_dir_files)):
                if teacher_dir_files[i] in student_dir_files:
                    teacher_file = path_to_teacher + '/' + teacher_dir_files[i]
                    student_file = path_to_student + '/' + teacher_dir_files[i]
                    if os.path.exists(teacher_file) and os.path.exists(student_file):
                        with open(teacher_file, 'r', encoding='utf-8') as t, open(student_file, 'r', encoding='utf-8') as s:
                            teacher_output = t.read().split('\n')
                            student_output = s.read().split('\n')
                            diff = HtmlDiff().make_file(teacher_output, student_output)
                            
                            matcher = difflib.SequenceMatcher(None, teacher_output, student_output)
                            similarity_ratio = "{:.3f}".format(matcher.ratio())
                            similarity_ratio = str(float(similarity_ratio) * 100) + "%"
                            
                            diff_file_path = ''.join([view_file_dest, '/', 
                                                      teacher_dir_files[i],
                                                      '.html'
                                                      ])
                            with open(diff_file_path, '+w') as d:
                                d.write(diff)
                            
                            compare_file = CompareFile(name=teacher_dir_files[i]+'.html', similarity_ratio=similarity_ratio, program=program, view_path=diff_file_path)
                            compare_file.save()
                      
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
                                
                            compare_file = CompareFile(name=diff_file_path, similarity_ratio="0.0%", program=program, view_path=diff_file_path)
                            compare_file.save()
            return True
                                

        
                    

        
