import os
import subprocess
import sys

# ANSI颜色代码
COLOR_GREEN = "\033[92m"
COLOR_RED = "\033[91m"
COLOR_RESET = "\033[0m"

BUILD_DIR = "../cmake-build-debug/"  # 替换为实际的构建目录
folder_path = ""

def compile_and_run_cminusfc(source_file):
    input_option = None
    input_file = os.path.splitext(source_file)[0] + ".in"
    if os.path.exists(input_file):
        print("input_file: ", input_file)
        with open(input_file, "rb") as fin:
            input_option = fin.read()
    llvm_output = os.path.splitext(source_file)[0] + ".ll"
    result_cminusfc = subprocess.run([BUILD_DIR + "cminusfc", "-o", llvm_output, "-emit-llvm", source_file])
    if result_cminusfc.returncode == 0:
        print(["clang", "-O0", "-w", "-no-pie", llvm_output, "-o", os.path.splitext(source_file)[0], "-L", BUILD_DIR, "-lcminus_io"])
        result_clang = subprocess.run(["clang", "-O0", "-w", "-no-pie", llvm_output, "-o", os.path.splitext(source_file)[0], "-L", BUILD_DIR, "-lcminus_io"])
        if result_clang.returncode == 0:
            result_run = subprocess.run([os.path.splitext(source_file)[0]], input=input_option, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=5)
            os.remove(os.path.splitext(source_file)[0])
            return result_run
    os.remove(llvm_output)
    return None

def main(folder_path):
    for file in os.listdir(folder_path):
        if file.endswith(".sy"):
            sy_file = os.path.join(folder_path, file)
            out_file = os.path.join(folder_path, os.path.splitext(file)[0] + ".out")
            if os.path.exists(out_file):
                result = compile_and_run_cminusfc(sy_file)
                if result is not None:
                    with open(out_file, "r") as f:
                        expected_output = f.read()
                    user_ouput = result.stdout.decode() + str(result.returncode) + "\n"
                    if user_ouput == expected_output:
                        print(f"{COLOR_GREEN}Comparing output for {sy_file}: Success{COLOR_RESET}")
                    else:
                        print("your output: ", user_ouput)
                        print(f"{COLOR_RED}Comparing output for {sy_file}: Failed{COLOR_RESET}")
                        return
                else:
                    print(f"{COLOR_RED}Compilation failed for {sy_file}{COLOR_RESET}")
                    return
            else:
                print(f"{COLOR_RED}No corresponding .out file found for {sy_file}{COLOR_RESET}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 script.py folder_path")
        sys.exit(1)
    folder_path = sys.argv[1]
    main(folder_path)
