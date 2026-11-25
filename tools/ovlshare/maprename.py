# usage: maprename.py OVL MAPFILE   (lines "old new"; func_ olds get a syms entry, named olds are renamed in syms)
import re,sys
ov,mf=sys.argv[1],sys.argv[2]
M=[l.split() for l in open(mf) if l.strip()]
syms=open(f'ovl/{ov}.syms.txt').read()
add=[]
for old,new in M:
    if re.search(r'^'+old+r'\s*=',syms,re.M): syms=re.sub(r'^'+old+r'(\s*=)',new+r'\1',syms,flags=re.M)
    elif old.startswith('func_'): add.append(f'{new} = 0x{old[5:].upper()}; // type:func')
    else: print('no addr for',old)
open(f'ovl/{ov}.syms.txt','w').write(syms.rstrip('\n')+'\n'+'\n'.join(add)+('\n' if add else ''))
for p in [(f'src/enemy/{ov}.c' if ov.startswith('em') else f'src/ovl/{ov}.c'),(f'include/enemy/{ov}.h' if ov.startswith('em') else f'include/ovl/{ov}.h')]:
    s=open(p).read()
    for old,new in M: s=re.sub(r'\b'+old+r'\b',new,s)
    open(p,'w').write(s)
