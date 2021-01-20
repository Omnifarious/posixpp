#!/usr/bin/env python3

import subprocess
import re
import itertools
from typing import Iterable, Callable, Generator


open_includes = """
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
"""
oflag_re = re.compile("^\s*#\s*define\s+(O_[A-Za-z0-9_]+)\s")


def re_replace(items: Iterable[str], regex: re.Pattern,
               subfunc: Callable[[re.Match], str]) -> Generator[str, None, None]:
    for item in items:
        m = regex.search(item)
        if m is not None:
            yield subfunc(m)


def oflag_codegen(m: re.Match) -> str:
    flagname = m.group(1)
    lowername = flagname[2:].lower()
    return f'print("constexpr const fdflags fdflags::{lowername}{{{{{{}}}}}};".format(strfry({flagname})))'


defs = subprocess.check_output(['gcc', '-dM', '-E', '-x', 'c++', '-'],
                               input=open_includes.encode('iso8859-1'))
defs = defs.decode('iso8859-1').splitlines()
defs = list(re_replace(defs, oflag_re, oflag_codegen))
head = f"""{open_includes}
#undef strfry2
#undef strfry
#define strfry2(x) #x
#define strfry(x) strfry2(x)
----------cut here----------"""
print('\n'.join(itertools.chain((head,), defs)))


