from hashlib import md5, sha1
import fileinput
import time

filename='./sample_1kb.txt'

def format_time(total_time):
    """Formats time in seconds for better readability"""
    return f"{total_time:.9f} s"

def my_md5(string):
    start = time.time()
    result = md5(string.encode()).hexdigest()
    end_time = time.time()
    total = end_time - start
    print(f"Time taken for MD5 for string: {format_time(total)}")
    return result

def my_sha1(string):
    start = time.time()
    result = sha1(string.encode()).hexdigest()
    end_time = time.time()
    total = end_time - start
    print(f"Time taken for SHA1 for string: {format_time(total)}")
    return result

# strings = ['1234567890', 'abcdefghijklmnopqrstuvwxyz', 'message digest']

# for string in strings:
#     print(f"MD5 output: {my_md5(string)}")
#     print(f"SHA1 output: {my_sha1(string)}")

for line in fileinput.input(files=filename):
   print(f"MD5 output: {my_md5(line)}")
   print(f"SHA1 output: {my_sha1(line)}")