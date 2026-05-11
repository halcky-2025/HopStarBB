import sys

filepath = sys.argv[1]

with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

depth = 0
in_line_comment = False
in_block_comment = False
in_string = False
in_char = False
i = 0
line_num = 1
last_depth = 0

while i < len(content):
    c = content[i]
    if c == '\n':
        in_line_comment = False
        if depth != last_depth:
            if depth <= 0 or last_depth <= 0:
                print(f'Line {line_num}: depth {last_depth} -> {depth}')
            last_depth = depth
        line_num += 1
        i += 1
        continue

    if in_line_comment:
        i += 1
        continue

    if in_block_comment:
        if c == '*' and i+1 < len(content) and content[i+1] == '/':
            in_block_comment = False
            i += 2
        else:
            i += 1
        continue

    if in_string:
        if c == '\\' and i+1 < len(content):
            i += 2
            continue
        if c == '"':
            in_string = False
        i += 1
        continue

    if in_char:
        if c == '\\' and i+1 < len(content):
            i += 2
            continue
        if c == "'":
            in_char = False
        i += 1
        continue

    if c == '/' and i+1 < len(content):
        if content[i+1] == '/':
            in_line_comment = True
            i += 2
            continue
        elif content[i+1] == '*':
            in_block_comment = True
            i += 2
            continue

    if c == '"':
        in_string = True
        i += 1
        continue
    if c == "'":
        in_char = True
        i += 1
        continue

    if c == '{':
        depth += 1
    elif c == '}':
        depth -= 1

    i += 1

print(f'Final depth: {depth}')
