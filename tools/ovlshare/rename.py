# usage: rename.py CANON COPY [--apply]  -> give COPY's func_ copies the CANON overlay names
import json,re,sys,glob
A,B=sys.argv[1],sys.argv[2]; apply='--apply' in sys.argv
G=json.load(open('build/ovlshare_groups.json'))
srcB=re.sub(r'/\*.*?\*/','',open((f'src/enemy/{B}.c' if B.startswith('em') else f'src/ovl/{B}.c')).read(),flags=re.S)
m=[]
for g in G:
    a=[x for x in g if x[0]==A]; b=[x for x in g if x[0]==B]
    if len(a)!=1 or len(b)!=1: continue
    na,nb=a[0][2],b[0][2]
    if na.startswith('func_') or not nb.startswith('func_'): continue
    if re.search(r'\b'+na+r'\b',srcB): print('collision',na); continue
    m.append((nb,na,b[0][1]))
for x in m: print(x)
if apply:
    with open(f'ovl/{B}.syms.txt','a') as f:
        f.write(f'// names shared with {A} (same code, src/enemy/common/{A})\n')
        for nb,na,addr in m: f.write(f'{na} = 0x{addr.upper()}; // type:func\n')
    for p in [(f'src/enemy/{B}.c' if B.startswith('em') else f'src/ovl/{B}.c'),(f'include/enemy/{B}.h' if B.startswith('em') else f'include/ovl/{B}.h')]:
        s=open(p).read()
        for nb,na,_ in m: s=re.sub(r'\b'+nb+r'\b',na,s)
        open(p,'w').write(s)
