# usage: trytext.py CANON COPY : for same-named functions whose C differs, try CANON's text in COPY; keep if build is all OK
import re,sys,subprocess,importlib.util
A,B=sys.argv[1],sys.argv[2]
spec=importlib.util.spec_from_file_location('sh','tools/ovlshare/share.py')
src=open((f'src/enemy/{A}.c' if A.startswith('em') else f'src/ovl/{A}.c')).read()
HDR=re.compile(r'^(?:static\s+)?[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{',re.M)
def funcs(s):
    d={}
    for m in HDR.finditer(s):
        j=m.end()-1;dep=0
        while True:
            if s[j]=='{':dep+=1
            elif s[j]=='}':
                dep-=1
                if dep==0:break
            j+=1
        d[m.group(1)]=(m.start(),j+1)
    return d
def norm(t): return re.sub(r'\s+','',re.sub(r'/\*.*?\*/','',t,flags=re.S))
def ok():
    r=subprocess.run('tools/capped make 2>&1',shell=True,capture_output=True,text=True).stdout
    return r.count(': OK')==60 and 'Error' not in r
FA=funcs(src); kept=[]
for n,(a0,a1) in FA.items():
    sb=open((f'src/enemy/{B}.c' if B.startswith('em') else f'src/ovl/{B}.c')).read(); FB=funcs(sb)
    if n not in FB: continue
    b0,b1=FB[n]
    if norm(sb[b0:b1])==norm(src[a0:a1]): continue
    new=sb[:b0]+src[a0:a1]+sb[b1:]
    open((f'src/enemy/{B}.c' if B.startswith('em') else f'src/ovl/{B}.c'),'w').write(new)
    if ok(): kept.append(n)
    else: open((f'src/enemy/{B}.c' if B.startswith('em') else f'src/ovl/{B}.c'),'w').write(sb)
assert ok()
print('switched to',A,'text:',kept)
