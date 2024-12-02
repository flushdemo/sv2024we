#!/usr/bin/env python3
import sys

LINES_PER_PAGE = 3
CHARS_PER_LINE = 13

def build_phrase(phrase):
    res = []
    for l in phrase:
        s = " "*((CHARS_PER_LINE - len(l)) // 2)
        s+= l
        res.append(s)
    return "\\n"*(3 - len(res)) + "\\n\\n".join(res)
        
def process_file(fname):
    with open(fname) as fd:
        data = fd.read().strip().upper().split("\n")

    talk    = []
    phrase  = []
    timing  = []
    for ln in data:
        if ln.startswith("\\"):
            timing.append(ln[1:])
        elif ln == "":
            talk.append(phrase)
            phrase = []
        else:
            phrase.append(ln)

    if phrase: # Talk last phrases into account
        talk.append(phrase)
            
    c_strings = [build_phrase(p) for p in talk]
    print(
        "char *printer_talk[] = {\n" +
        ",\n".join(['  "'+cs+'"' for cs in c_strings]) +
        ",\n  \"\"" +
        "\n};"
    )
    print()
    print(
        "// A value of 25 corresponds to 2 seconds\n" +
        "short printer_timing[] = {\n  " +
        ", ".join(timing) +
        "\n};"
    )
        
process_file(sys.argv[1])
