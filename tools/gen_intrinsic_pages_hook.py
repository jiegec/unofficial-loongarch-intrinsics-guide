"""MkDocs hook: generate the per-intrinsic pages before the site is built.

The generated directory (docs/intrinsic/) is git-ignored; this hook keeps
local builds and CI in sync without any extra dependency.
"""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import gen_intrinsic_pages  # noqa: E402


def on_pre_build(config, **kwargs):
    gen_intrinsic_pages.main()
