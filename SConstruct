# Graphics Language Renderer (glr) SConstruct file

import os, sys
import glob
import json

import subprocess
import shlex


def parseShadersIntoHeader():
	"""Parse the OpenGL Shader files into a single C++ Header file."""
	print('Parsing Shaders into header ShaderData.h')
	
	shaderDataOutputFilename = "ShaderData.h"
	shaderListLocation = "data/shaders/"
	#shaderListExtension = ".json"
	
	fileList = glob.glob( os.path.join(shaderListLocation, '*') )
	
	file = open(fileList[0], 'r')
	data = file.read()
	
	# Save json as a std::string in a .h file
	shaderDataFile = open( os.path.join('src/glw/shaders', shaderDataOutputFilename), 'w' )
	
	cpp = """
/**
 * This file is automatically generated.  Changes made to this file will
 * not be reflected after compiling.
 *
 * If you wish to make changes to shader information for Glr, edit the shaders
 * and shader programs in the 'data/shaders/' directory.
 *
 */
 
#include <map> 
 
namespace glr {

namespace shaders {

static std::map<std::string, std::string> SHADER_DATA = {
"""
	current = 0
	for filename in fileList:
		file = open(filename, 'r')
		data = file.read()
		
		if (current > 0):
			cpp += ", "
		
		cpp += """
{\""""

		filename = filename.replace(shaderListLocation, "")
		filename = filename.replace(".glsl", "")
		cpp += filename
		
		cpp +="""\", std::string(
	"\\
"""

		cpp += data.replace("\n", "\\\n") + "\\"

		cpp += """
"
)}	
"""
		current += 1
	
	cpp += """
};

}

}
"""
		
	shaderDataFile.write(cpp)
	shaderDataFile.close()
	
	print('Done parsing Shaders into header ShaderData.h')



# Parse our shader programs and create .h files out of them
parseShadersIntoHeader()



# Tell SCons to create our build files in the 'build' directory
VariantDir('build', 'src', duplicate=0)



### Set our source files
source_files = Glob('build/*.cpp', 'build/*.h')

source_files = source_files + Glob('build/common/compatibility/*.cpp', 'build/common/compatibility/*.h')
source_files = source_files + Glob('build/common/math/*.cpp', 'build/common/math/*.h')
source_files = source_files + Glob('build/common/utilities/*.cpp', 'build/common/utilities/*.h')
source_files = source_files + Glob('build/common/io/*.cpp', 'build/common/io/*.h')
#source_files = source_files + Glob('build/vmath/*.cpp', 'build/vmath/*.h')

source_files = source_files + Glob('build/*.cpp', 'build/*.h')
source_files = source_files + Glob('build/exceptions/*.cpp', 'build/exceptions/*.h')
source_files = source_files + Glob('build/gui/*.cpp', 'build/gui/*.h')
source_files = source_files + Glob('build/models/*.cpp', 'build/models/*.h')

# OpenGL Wrapper stuff
source_files = source_files + Glob('build/glw/*.cpp', 'build/glw/*.h')
source_files = source_files + Glob('build/glw/shaders/*.cpp', 'build/glw/shaders/*.h')



### Set our required libraries
libraries = [
'GL',
'GLU',
'liblibberkelium_d',
'Xi',
'sfml-window',
'assimp',
'boost_log', 
'boost_date_time', 
'boost_thread',
'boost_wave',
'boost_regex'
]



### Set our general compiler variables
library_paths = [
]

# Set our g++ compiler flags
cpp_flags = [
'-Wall',
#'-D_GLIBCXX_DEBUG'
]

cpp_defines = []
#debug = ARGUMENTS.get('debug', 0)
#if int(debug):
#	cpp_flags.append('-g')
#	cpp_defines.append('DEBUG')
#else:
#	cpp_defines.append('NDEBUG')
cpp_defines.append('DEBUG')	

cpp_paths = []



# Set our compiler
compiler = ARGUMENTS.get('compiler')
if (compiler is None or compiler == ''):
	compiler = 'default'
if (compiler == 'gcc' and os.name == 'nt'):
	compiler = 'mingw'
if (compiler == 'msvc' and os.name == 'nt'):
	compiler = 'default'

### Set our OS specific compiler variables
if (os.name != 'nt'):
	if (compiler == 'gcc'):
		cpp_flags.append('-g')
		cpp_flags.append('-O0') # optimization level 0
		cpp_flags.append('-std=c++11')
		
	# Dynamically link to boost log
	cpp_defines.append('BOOST_LOG_DYN_LINK')
else:
	if os.name == 'nt':
		if (compiler == 'default'):
			cpp_flags.append('/w') # disables warnings (Windows)
			cpp_flags.append('/wd4350') # disables the specific warning C4350
			cpp_flags.append('/EHsc') # Enable 'unwind semantics' for exception handling (Windows)
		elif (compiler == 'mingw'):
			cpp_flags.append('-g')
			cpp_flags.append('-O0') # optimization level 0
			cpp_flags.append('-std=c++11')
		
	
	#cpp_paths.append('C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.1A\\Include')
	#cpp_paths.append('C:\\Program Files (x86)\\Microsoft Visual Studio 11.0\\VC\\include')
	
	cpp_paths.append('C:\\Program Files (x86)\\Boost\\include\\boost-1_54')
	cpp_paths.append('C:\\Program Files\\Assimp\\include')
	cpp_paths.append('C:\\Users\\Jarrett\\projects\\glm')
	cpp_paths.append('C:\\Users\\Jarrett\\projects\\FreeImage\\Dist')
	cpp_paths.append('C:\\Users\\Jarrett\\projects\\berkelium\\include')
	cpp_paths.append('C:\\Users\\Jarrett\\projects\\SFML\\include')



# Create our environment
env = Environment(ENV = os.environ, TOOLS = [compiler], CCFLAGS=[]) 

### Set our environment variables
env.Append( CPPFLAGS = cpp_flags )
env.Append( CPPDEFINES = cpp_defines )
env.Append( CPPPATH = cpp_paths )

env.SetOption('num_jobs', 4)



# Tell SCons the library to build
env.StaticLibrary('build/glr', source_files, LIBS = libraries, LIBPATH = library_paths)
#env.Library('build/glr', source_files, LIBS = libraries, LIBPATH = library_paths)
