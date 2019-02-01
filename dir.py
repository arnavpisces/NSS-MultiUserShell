import sys
abc=sys.argv[1]
# abc='abc/xyz/haha/'
paths=abc.split('/')
# print('sdfasdfsadf')
paths.remove(paths[-1])
paths[-1]='\0'
str1 = '/'.join(paths)
print(str1)
# print("hi bithc")