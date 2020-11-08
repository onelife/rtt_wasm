''' RTT WebAssembly scons conifg
'''
import os

rttcfg = [
    # 'CONFIG_USING_CONSOLE=1',
]

env = Environment(toolpath=[os.environ.get('EMSCRIPTEN_TOOL_PATH')])
env.Tool('emscripten')
# env.Append(CXXFLAGS='-std=c++11')

env.Prepend(CPPPATH=['#'])
env.Prepend(CCFLAGS=['-Wall', '-fPIC'])
env.Prepend(CPPDEFINES=rttcfg)
env_worker = env.Clone(
    LINKFLAGS=['-s', 'MAIN_MODULE=2',
    '-s', 'BUILD_AS_WORKER=1',
    '--pre-js', 'pre_lib.js',
    # '-s', 'EXPORTED_FUNCTIONS=["_main"]',
    # '-s', 'EXTRA_EXPORTED_RUNTIME_METHODS=["ccall"]',
    # '-s', 'MODULARIZE=1',
],
)
env_main = env.Clone(
    LINKFLAGS=['-s', 'MAIN_MODULE=2',
    '--js-library', 'mergeinto.js',
    # '-s', 'EXPORTED_FUNCTIONS=["_main"]',
    '-s', 'EXTRA_EXPORTED_RUNTIME_METHODS=["ccall"]',
    # '-s', 'MODULARIZE=1',
],
)
env.Prepend(LINKFLAGS=['-s', 'SIDE_MODULE=2', '-O1'])

out = env.Program(
    'dist/rtt_kernel.wasm',
    [
        SConscript(os.path.join(d, 'SConscript'),
            exports='env',
            variant_dir='build/%s' % d,
            duplicate=0)
        for d in os.listdir('.') if os.path.isfile(os.path.join('.', d, 'SConscript'))
    ]
)
out += env_worker.Program(
    'dist/rtt_worker.js',
    [
        SConscript(os.path.join('.', 'SConscript_worker'),
            exports='env_worker',
            variant_dir='build',
            duplicate=0)
    ]
)
out += env_main.Program(
    'dist/rtt.html',
    [
        SConscript(os.path.join('.', 'SConscript_main'),
            exports='env_main',
            variant_dir='build',
            duplicate=0)
    ]
)

# clean
Clean(out, 'build')
