# -*- coding: utf-8 -*-
import sys


def options(opt):
    opt.load('compiler_c')

    opt.add_option(
        '--build-type',
        action='store',
        choices=['release', 'debug'],
        default='debug',
        help='build type (release or debug)')


def configure(cfg):
    cfg.load('compiler_c')

    cfg.env.append_unique('CFLAGS', '-std=c99')
    cfg.env.append_unique('CFLAGS', '-Wall')
    cfg.env.append_unique('CFLAGS', '-Werror')
    cfg.env.append_unique('CFLAGS', '-fPIC')

    if cfg.options.build_type == 'debug':
        cfg.env.append_unique('CFLAGS', '-g')
        cfg.env.append_unique('DEFINES', 'DEBUG')
    else:
        cfg.env.append_unique('CFLAGS', '-O3')
        cfg.env.append_unique('DEFINES', 'NDEBUG')

    if sys.platform.startswith('linux'):
        # find libm (standard C math library)
        cfg.check_cc(
            msg=u'Checking for libm',
            lib='m',
            cflags='-Wall',
            uselib_store='libm')


def build(bld):
    # platform-specific dependencies and compile options
    deps = []
    kwargs = {}

    if sys.platform.startswith('linux'):
        deps.append('libm')

    # build library
    bld.shlib(
        target='mat',
        source=bld.path.ant_glob('src/**/*.c'),
        uselib=deps,
        install_path='${PREFIX}/lib',
        **kwargs)

    bld.install_files('${PREFIX}/include', ['src/matlib.h'])
