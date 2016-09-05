#!/usr/bin/env python3

import setuptools, sys, unittest
import Cython.Compiler.Options

SOURCE_PATH = 'src/py'

sys.path.append(SOURCE_PATH)

from timedata_build.arguments import check_python, insert_dependencies
from timedata_build.context import CONFIG, FLAGS
from timedata_build.commands import COMMANDS

if False:

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
