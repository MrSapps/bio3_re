# usage: share.py FAMILY CANON COPY... [--apply]
# Moves functions whose definitions are identical (ignoring comments/whitespace) in CANON and COPY overlays
# into src/ovl/shared/FAMILY/<name>.inc.c, included at the same place in each overlay.
import re,sys,os
args=[a for a in sys.argv[1:] if not a.startswith('--')]; apply='--apply' in sys.argv
fam,ovs=args[0],args[1:]
HDR=re.compile(r'^(?:static\s+)?[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{',re.M)
def norm(t): return re.sub(r'\s+','',re.sub(r'/\*.*?\*/|//[^\n]*','',t,flags=re.S))
def funcs(s):
    d={}
    for m in HDR.finditer(s):
        if m.group(1) in('if','while','for','switch'): continue
        j=m.end()-1;dep=0
        while True:
            if s[j]=='{':dep+=1
            elif s[j]=='}':
                dep-=1
                if dep==0:break
            j+=1
        st=m.start()
        # take an attached comment block immediately above
        k=s.rfind('\n',0,st-1)
        while True:
            line=s[k+1:st].strip() if False else None
            prev=s.rfind('\n',0,k)
            ln=s[prev+1:k].strip()
            if k>0 and (ln.startswith('/*') or ln.startswith('*') or ln.startswith('//')) and not ln.startswith('#'):
                k=prev; continue
            break
        d[m.group(1)]=(k+1 if k>0 else st,m.start(),j+1)
    return d
src={o:open(f'src/ovl/{o}/{o}.c').read() for o in ovs}
F={o:funcs(src[o]) for o in ovs}
A=ovs[0]; share={}
for n,(c0,h0,e0) in F[A].items():
    users=[o for o in ovs[1:] if n in F[o] and norm(src[o][F[o][n][1]:F[o][n][2]])==norm(src[A][h0:e0])]
    if users: share[n]=[A]+users
for n,u in share.items(): print(n,u)
print(len(share),'functions')
if apply:
    os.makedirs(f'src/ovl/shared/{fam}',exist_ok=True)
    for o in ovs:
        s=src[o]; reps=[]
        for n,u in share.items():
            if o not in u: continue
            c,h,e=F[o][n]
            if o==A:
                body=s[c:e].rstrip()+'\n'
                open(f'src/ovl/shared/{fam}/{n}.inc.c','w').write(f'/* shared by {", ".join(u)} */\n'+body)
            reps.append((c,e,f'#include "ovl/shared/{fam}/{n}.inc.c"'))
        for c,e,r in sorted(reps,reverse=True): s=s[:c]+r+s[e:]
        open(f'src/ovl/{o}/{o}.c','w').write(s)
