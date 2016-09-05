#!/usr/bin/env python3

import os, setuptools, sys, unittest
import Cython.Compiler.Options

SOURCE_PATH = 'src/py'
TIMEDATA_PATH = os.path.abspath('../timedata/src/py')

sys.path.extend((SOURCE_PATH, TIMEDATA_PATH))

from timedata_build.arguments import check_python, insert_dependencies
from timedata_build.config import CONFIG, FLAGS
from timedata_build import commands

COMMANDS = dict(
    build_ext=commands.BuildExt,
    clean=commands.Clean,
    )

# http://stackoverflow.com/a/37033551/43839
def test_suite():
    return unittest.TestLoader().discover(
        SOURCE_PATH, pattern=FLAGS.test_pattern)


check_python(FLAGS.minimum_python_version)
sys.argv = insert_dependencies(sys.argv, **CONFIG.dependencies)

# See: http://goo.gl/1kNY1n
Cython.Compiler.Options.annotate = True

print('About to build targets', *sys.argv[1:])
setuptools.setup(cmdclass=COMMANDS, **CONFIG.setuptools)
