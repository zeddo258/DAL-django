import os
import shutil
from zipfile import ZipFile


class hwutil:
    
    def get_all_hw():
        return os.listdir('./home_work')
    
    def create_hw(hw_name):
        hw_dest = './home_work/' + hw_name; 
        os.mkdir(hw_dest)
        os.mkdir(hw_dest + '/' + "src" )
        
        os.mkdir(hw_dest + '/' + "甲班" )
        os.mkdir(hw_dest + '/' + "甲班" + '/' + "作業" )
        os.mkdir(hw_dest + '/' + "甲班" + '/' + "挑戰" )
    
        os.mkdir(hw_dest + '/' + "乙班" )
        os.mkdir(hw_dest + '/' + "乙班" + '/' + "作業" )
        os.mkdir(hw_dest + '/' + "乙班" + '/' + "挑戰" )
        
        view_dest = './templates/' + hw_name
        os.mkdir(view_dest)
        
        os.mkdir(view_dest + '/' + "甲班" )
        os.mkdir(view_dest + '/' + "甲班" + '/' + "作業" )
        os.mkdir(view_dest + '/' + "甲班" + '/' + "挑戰" )
    
        os.mkdir(view_dest + '/' + "乙班" )
        os.mkdir(view_dest + '/' + "乙班" + '/' + "作業" )
        os.mkdir(view_dest + '/' + "乙班" + '/' + "挑戰" )
        
    def process_test_files(hw_name):
        test_files_dest = './home_work/' + hw_name + '/src'
        file_list = os.listdir('./home_work/uploads')
        try:
            for file in file_list:
                shutil.move('./home_work/uploads/' + file, test_files_dest)
        except OSError as e:
            print(e)
            
     
            
    def process_src_files(self,src_dest, data_path, view_file_dest):
        cwd = './home_work/uploads'
        file_list = os.listdir(cwd)
        
        for file in file_list:
            # Process zip file
            file_path = ''.join([cwd, '/', file])
            if file.endswith('.zip'):
                with ZipFile(file_path, 'r') as zf:
                    zf.extractall(cwd)
                try:
                    os.remove(file_path)
                except OSError as e: 
                    print(e)
                    
            # Teacher's code
            elif file == 'code.cpp':
                std_code_path = ''.join([src_dest, '/', 'std_code'])
                if not os.path.exists(std_code_path):
                    os.mkdir(std_code_path)
                else:
                    shutil.rmtree(std_code_path)
                    os.mkdir(std_code_path)
                shutil.move(file_path, std_code_path)
                for f in os.listdir(data_path):
                    path = ''.join([data_path, '/', f])
                    shutil.copy(path, std_code_path)
                
            # Command for test case
            elif file == 'command.txt':
                if os.path.exists(src_dest + '/' + file):
                    os.remove(src_dest + '/' + file)
                shutil.move(file_path, src_dest)
            
            # Student singular file 
            elif file.endswith('.cpp'):
                code_path = ''.join([src_dest, '/', file.replace('.cpp', '')])
                if not os.path.exists(code_path):
                    os.mkdir(code_path)
                else:
                    shutil.rmtree(code_path)
                    os.mkdir(code_path)
                shutil.move(file_path, code_path)
                
        # Create  directories and move files from upload to these dir 
        file_list = os.listdir(cwd)
        for file in file_list:
            wd = cwd + '/' + file
            if file !=  "__MACOSX":
                self.get_src_files(self, wd, src_dest, data_path, view_file_dest)
                shutil.rmtree(wd)
            else:
                shutil.rmtree(wd)
                
                
    def get_src_files(self, wd, src_dest, data_path, view_file_dest):
        file_list = os.listdir(wd)
        for file in file_list:
            f = wd + '/' + file
            if not os.path.isfile(f) and '.cpp' not in f:
                self.get_src_files(self, f, src_dest, data_path, view_file_dest)   
            else:
                try:
                    final_dest = "".join([src_dest, '/', file.strip('.cpp')])
                    view_file_final_dest = "".join([view_file_dest, '/', file.strip('.cpp')])
                    
                    # Dir to store cpp file and output
                    if not os.path.exists(final_dest):
                        os.mkdir(final_dest)
                    else:
                        shutil.rmtree(final_dest)
                        os.mkdir(final_dest)
                        
                    # Dir to store the compare's result 
                    if not os.path.exists(view_file_final_dest):
                        os.mkdir(view_file_final_dest)
                    else:
                        shutil.rmtree(view_file_final_dest)
                        os.mkdir(view_file_final_dest)
                        
                        
                    shutil.move(f, final_dest)
                    for f in os.listdir(data_path):
                        path = ''.join([data_path, '/', f])
                        shutil.copy(path, final_dest)
                except OSError as e:
                    print(e)
                    
    def check_for_requirement(path_to_hw, detail):
        dirs = os.listdir(path_to_hw)
        print(dirs)
        if "std_code" in dirs and "command.txt" in dirs:
            return True
        elif "std_code" in dirs and "command.txt" not in dirs:
            detail["std_code_exist"] = True
            detail["command_exist"] = False
            return False
        elif "std_code" not in dirs and "command.txt" in dirs:
            detail["std_code_exist"] = False
            detail["command_exist"] = True
            return False
        elif "std_code" not in dirs and "command.txt" not in dirs:
            detail["std_code_exist"] = False
            detail["command_exist"] = False
            return False
        return False
            
        
            
            
                    
