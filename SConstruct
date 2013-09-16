#shared
source_files = ['src/proc-com-demo.c']
env = Environment(CC = 'gcc', CCFLAGS=['-Wall'])

#optimized env
opt = env.Clone()
opt.Append(CCFLAGS = ['-O3'])

#debug env
#gdb info, set debug 1, child sleeps or 1 sec max
dbg = env.Clone()
dbg.Append(CCFLAGS = ['-g', '-DDEBUG=1', '-DNAP_MAX=1'])

#main program
shell = env.Program('build/proc-com-demo', source_files)

#optimized program
o = opt.Object('build/pcd-opt', source_files)
optimize = opt.Program(o)

#debug program
d = dbg.Object('build/pcd-dbg', source_files)
debug = dbg.Program(d)


