#!/usr/bin/env python3
"""Extract all files from an ISO9660 image into a directory."""
import os, sys, pycdlib
iso_path, out = sys.argv[1], sys.argv[2]
iso = pycdlib.PyCdlib(); iso.open(iso_path)
for root, dirs, files in iso.walk(iso_path="/"):
    for d in dirs:
        os.makedirs(os.path.join(out, root.lstrip("/"), d), exist_ok=True)
    for fn in files:
        p = f"{root.rstrip('/')}/{fn}"
        name = fn.split(";")[0]
        dest = os.path.join(out, root.lstrip("/"), name)
        os.makedirs(os.path.dirname(dest), exist_ok=True)
        try:
            with open(dest, "wb") as fo: iso.get_file_from_iso_fp(fo, iso_path=p)
        except Exception as e: print("skip", p, e)
iso.close()
