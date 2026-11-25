import re,glob,collections,os
sig=collections.defaultdict(dict)
SRCN={os.path.basename(os.path.dirname(f)):set(re.findall(r'\b(\w+)\s*\(',open(f,errors='replace').read())) for f in glob.glob('src/ovl/*/*.c')}
for f in glob.glob('asm/nonmatchings/ovl/**/*.s',recursive=True)+glob.glob('asm/matchings/ovl/**/*.s',recursive=True):
    s=open(f,errors='replace').read()
    if 'glabel' not in s: continue
    ov=f.split('/')[3]; ins=[];addr=None
    s=s[s.index('glabel'):]
    for line in s.splitlines():
        m=re.match(r'\s*/\* \w+ (\w{8}) (\w{8}) \*/\s+(\S+)\s*(.*)',line)
        if not m: continue
        addr=addr or m.group(1)
        op=m.group(3); a=re.sub(r'(%hi|%lo)\([^)]*\)','REL',m.group(4)); a=re.sub(r'\.?L\w+|func_\w+|D_\w+|jtbl_\w+','SYM',a)
        if op in('jal','j') : a='SYM'
        ins.append(op+' '+a)
    if len(ins)<6: continue
    nm=os.path.basename(f)[:-2]; k=(ov,addr); d=sig[tuple(ins)]
    if k not in d or nm in SRCN.get(ov,set()): d[k]=nm
G=[v for v in sig.values() if len({k[0] for k in v})>1]
pairs=collections.Counter()
for v in G:
    ovs=sorted({k[0] for k in v})
    for i in range(len(ovs)):
        for j in range(i+1,len(ovs)): pairs[(ovs[i],ovs[j])]+=1
print('cross-overlay identical groups',len(G),'functions',sum(len(v) for v in G))
for p,c in pairs.most_common(30): print(c,p)
import json
inc=set()
for f in glob.glob('src/ovl/**/*.c',recursive=True):
    for m in re.finditer(r'INCLUDE_ASM\("([^"]+)",\s*(\w+)\)',open(f,errors='replace').read()): inc.add(m.group(2))
# map (ov,addr)->current name: prefer name present in src
out=[]
for v in G:
    mem=[]
    for (ov,addr),n in sorted(v.items()):
        mem.append([ov,addr,n,n in inc])
    out.append(mem)
json.dump(out,open('build/ovlshare_groups.json','w'),indent=0)
nm=sum(1 for g in out if all(not m[3] for m in g)); print('groups fully matched',nm,'partly',sum(1 for g in out if any(m[3] for m in g) and not all(m[3] for m in g)),'none',sum(1 for g in out if all(m[3] for m in g)))
for g in out[:80]: print(' | '.join(f"{m[0]}:{m[2]}{'*' if m[3] else ''}" for m in g))
