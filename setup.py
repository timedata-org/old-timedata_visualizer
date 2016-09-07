#!/usr/bin/env python3

import os, platform, setuptools, sys, unittest
import Cython.Compiler.Options

SOURCE_PATH = 'src/py'
TIMEDATA_PATH = os.path.abspath('../timedata/src/py')

sys.path.extend((SOURCE_PATH, TIMEDATA_PATH))

from timedata_build.arguments import check_python, insert_dependencies
from timedata_build.config import CONFIG, FLAGS
from timedata_build import commands, execute


_JUCE_DIR_MAP = dict(
    Darwin=('MacOSX', ),
    Linux=('LinuxMakefile',),
    Windows=('VisualStudio2015',),
    )


class BuildExt(commands.BuildExt):
    def _extension_dict(self):
        # Add the directory containing the Juce library to the compile args.
        base = os.path.join('build', 'juce', _JUCE_DIR_MAP[platform.system()][0])

        # TODO: this next line will be different for Windows.
        library_dir = os.path.join(base, 'build', 'Release')
        d = super()._extension_dict()
        d['extra_link_args'] = ['-L%s' % library_dir]
        return d

class BuildJuce(commands.Command.Command):
    description = 'Run Juce build'

    def run(self):
        print('Building Juce')
        if platform.system() == 'Linux':
            pass


COMMANDS = dict(
    build_ext=BuildExt,
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
