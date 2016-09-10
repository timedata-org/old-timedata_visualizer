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
    Darwin=(
        'MacOSX',
        'xcodebuild'
        ' -project juce/MacOSX/timedata_visualizer_juce.xcodeproj'
        ' -configuration Release'
        ' -jobs 6'),
    Linux=('LinuxMakefile', 'make'),  # TODO: need to cd and set CONFIG=Release.
    Windows=('VisualStudio2015', 'TODO'),
    )

_JUCE_DIR, _JUCE_COMMAND = _JUCE_DIR_MAP[platform.system()]


class BuildExt(commands.BuildExt):
    def _extension_dict(self):
        # Add the directory containing the Juce library to the compile args.
        base = os.path.join('juce', _JUCE_DIR)

        # TODO: this next line will be different for Windows.
        library_dir = os.path.join(base, 'build', 'Release')
        d = super()._extension_dict()
        d['extra_link_args'] = ['-L%s' % library_dir]
        return d


class BuildJuce(commands.Command.Command):
    description = 'Run Juce build'

    def run(self):
        print(execute.run(*_JUCE_COMMAND.split()))


class CleanJuce(commands.Clean):
    NAME = 'juce_files'

class CleanCython(commands.Clean):
    NAME = 'cython_files'


COMMANDS = dict(
    build_ext=BuildExt,
    build_juce=BuildJuce,
    clean=commands.Clean,
    clean_juce=CleanJuce,
    clean_cython=CleanCython,
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
