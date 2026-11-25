# usage: align.py A B  -> print matched function pairs by asm similarity, in order
import re,glob,os,sys,difflib
def load(ov):
    fs=[]
    for f in glob.glob(f'asm/nonmatchings/ovl/{ov}/*/*.s')+glob.glob(f'asm/matchings/ovl/{ov}/*/*.s'):
        s=open(f,errors='replace').read()
        if 'glabel' not in s: continue
        s=s[s.index('glabel'):]; ins=[];addr=None
        for line in s.splitlines():
            m=re.match(r'\s*/\* \w+ (\w{8}) (\w{8}) \*/\s+(\S+)\s*(.*)',line)
            if m:
                addr=addr or m.group(1)
                a=re.sub(r'(%hi|%lo)\([^)]*\)','R',m.group(4)); a=re.sub(r'\.?L\w+|func_\w+|D_\w+|jtbl_\w+|\b[A-Z]\w+','S',a)
                ins.append(m.group(3)+' '+a)
        fs.append((addr,os.path.basename(f)[:-2],ins))
    d={}
    for a,n,i in fs: d.setdefault(a,(a,n,i))   # one entry per address
    srcn=set(re.findall(r'\b(\w+)\s*\(', open((f'src/enemy/{ov}.c' if ov.startswith('em') else f'src/ovl/{ov}.c')).read()))
    out=[]
    for a in sorted(d):
        n=d[a][1]
        # stale asm file name: prefer a name used in src
        cands=[x for x in fs if x[0]==a]; names=[x[1] for x in cands if x[1] in srcn] or [n]
        out.append((a,names[0],d[a][2]))
    return out
A,B=load(sys.argv[1]),load(sys.argv[2])
R=lambda x,y: difflib.SequenceMatcher(None,x[2],y[2],autojunk=False).ratio()
n,m=len(A),len(B); import functools
sim=[[R(A[i],B[j]) if abs(i-j)<=8 else 0 for j in range(m)] for i in range(n)]
D=[[0.0]*(m+1) for _ in range(n+1)]
for i in range(n-1,-1,-1):
    for j in range(m-1,-1,-1):
        D[i][j]=max(D[i+1][j],D[i][j+1],D[i+1][j+1]+sim[i][j]-0.3)
i=j=0
while i<n and j<m:
    if D[i][j]==D[i+1][j+1]+sim[i][j]-0.3: print(A[i][1],B[j][1],round(sim[i][j],2)); i+=1;j+=1
    elif D[i][j]==D[i+1][j]: print(A[i][1],'-',0); i+=1
    else: print('-',B[j][1],0); j+=1
