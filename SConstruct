''' RTT WebAssembly scons conifg
'''
import os

rttcfg = [
    # 'CONFIG_USING_CONSOLE=1',
]

# set options
opts = Variables(None, ARGUMENTS)
opts.Add(BoolVariable('debug', 'debug mode', 0))
opts.Add(BoolVariable('memfs', 'MEMFS support', 0))
opts.Add(BoolVariable('idbfs', 'IDBFS support', 0))

env = Environment(
    toolpath=[os.environ.get('EMSCRIPTEN_TOOL_PATH')],
    variables=opts,
)
env.Tool('emscripten')
# env.Append(CXXFLAGS='-std=c++11')

Help(opts.GenerateHelpText(env))


env.Prepend(CPPPATH=['#'])
if env['debug']:
    env.Prepend(CCFLAGS=['-Wall', '-fPIC', '-g3'])
    env.Prepend(LINKFLAGS=['-s', 'ASYNCIFY_ADVISE', '-g3'])
else:
    env.Prepend(CCFLAGS=['-Wall', '-fPIC'])
    env.Prepend(LINKFLAGS=['-O3'])
env.Prepend(CPPDEFINES=rttcfg)

env_worker = env.Clone(
    LINKFLAGS=['-s', 'MAIN_MODULE=2',
        '-s', 'BUILD_AS_WORKER=1',
        # '-s', 'SAFE_HEAP=1',
        '--pre-js', 'pre_lib.js',
        # '-s', 'EXPORTED_FUNCTIONS=["_main"]',
        # '-s', 'EXTRA_EXPORTED_RUNTIME_METHODS=["ccall"]',
        # '-s', 'MODULARIZE=1',
    ]
)

env_main = env.Clone(
    LINKFLAGS=['-s', 'MAIN_MODULE=2',
        '--js-library', 'mergeinto.js',
        # '-s', 'EXPORTED_FUNCTIONS=["_main"]',
        '-s', 'EXTRA_EXPORTED_RUNTIME_METHODS=["ccall"]',
        # '-s', 'MODULARIZE=1',
    ]
)

env.Prepend(LINKFLAGS=['-s', 'SIDE_MODULE=2'])

if env_worker['memfs'] or env_worker['idbfs']:
    env_worker.Prepend(LINKFLAGS=['-s', 'FORCE_FILESYSTEM=1'])
    if env_worker['memfs']:
        print('*** MEMFS ***')
        env_worker.Append(CPPDEFINES=['CONFIG_USING_MEMFS=1'])
        env.Append(CPPDEFINES=['CONFIG_USING_MEMFS=1'])
    if env_worker['idbfs']:
        print('*** IDBFS ***')
        env_worker.Append(CPPDEFINES=['CONFIG_USING_IDBFS=1'])
        # env_worker.Append(LINKFLAGS=['-s', 'ASYNCIFY'])
        env_worker.Append(LIBS=['idbfs.js']),
        env.Append(CPPDEFINES=['CONFIG_USING_IDBFS=1'])
else:
    print('*** NO FS ***')
# env_worker['ENV']['ASYNCIFY_ADVISE'] = 1

out = env.Program(
    'dist/rtt_kernel.wasm',
    [
        SConscript(os.path.join(d, 'SConscript'),
            exports='env',
            variant_dir=os.path.join('build', d),
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
            duplicate=0),
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
