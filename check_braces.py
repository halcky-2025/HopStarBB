import re
import sys

filepath = sys.argv[1] if len(sys.argv) > 1 else r'C:\Users\hayah\source\repos\Project2\Project2\Compile.h'

with open(filepath, 'r', encoding='utf-8') as f:
    content = f.read()

lines_orig = content.split('\n')

# State machine to strip strings, chars, comments
cleaned = []
i = 0
n = len(content)
while i < n:
    c = content[i]
    if c == '/' and i+1 < n and content[i+1] == '/':
        # single line comment
        while i < n and content[i] != '\n':
            cleaned.append(' ')
            i += 1
    elif c == '"':
        cleaned.append(' ')
        i += 1
        while i < n and content[i] != '"':
            if content[i] == '\\' and i+1 < n:
                cleaned.append(' ')
                i += 1
            cleaned.append(' ')
            i += 1
        if i < n:
            cleaned.append(' ')
            i += 1
    elif c == "'" and i+1 < n:
        # char literal
        cleaned.append(' ')
        i += 1
        if i < n and content[i] == '\\':
            cleaned.append(' ')
            i += 1
        if i < n:
            cleaned.append(' ')
            i += 1
        if i < n and content[i] == "'":
            cleaned.append(' ')
            i += 1
    else:
        cleaned.append(c)
        i += 1

cleaned_str = ''.join(cleaned)
cleaned_lines = cleaned_str.split('\n')

depth = 0
for idx, line in enumerate(cleaned_lines, 1):
    prev = depth
    for c in line:
        if c == '{': depth += 1
        elif c == '}': depth -= 1
    if depth != prev:
        if idx > 800 or depth < 0:
            print(f'Line {idx}: depth={depth}  | {lines_orig[idx-1].rstrip()[:90]}')
    if depth < 0:
        print('ERROR: negative depth')
        break

print(f'Final depth: {depth}')
