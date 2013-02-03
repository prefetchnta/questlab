#-*- coding: utf-8 -*-

import os

def delBOM ():
    bom_files = []
    file_count = 0
    for dirpath, dirnames, filenames in os.walk('.'):
        if 'tmp' in dirnames:
            dirnames.remove('tmp')
        if '.hg' in dirnames:
            dirnames.remove('.hg')
        if '.git' in dirnames:
            dirnames.remove('.git')
        if '.svn' in dirnames:
            dirnames.remove('.svn')
        if len(filenames) == 0:
            continue
        for filename in filenames:
            ext = os.path.splitext(filename)[1]
            if ext == '.lib' or ext == '.res' or ext == '.bpi' or ext == '.a':
                continue
            if ext == '.exe' or ext == '.dll' or ext == '.bpl' or ext == '.sys':
                continue
            if ext == '.bmp' or ext == '.gif' or ext == '.png' or ext == '.ico':
                continue
            file_count += 1
            full_path = dirpath + "/" + filename
            file = open(full_path, 'rb')
            text = file.read()
            file.close()
            if len(text) <= 3:
                continue
            if ord(text[0]) == 239 and ord(text[1]) == 187 and ord(text[2]) == 191:
                bom_files.append(full_path)
                file = open(full_path, 'wb')
                file.write(text[3:])
                file.close()
                print bom_files[-1], "BOM found. Deleted."
    print file_count, "file(s) found.", len(bom_files), "file(s) have a BOM. Deleted."
    raw_input()

if __name__ == "__main__":
    delBOM()
