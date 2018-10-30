"=============================================================================
" File:         _vimrc_local_global_defs.vim {{{1
" Author:       Luc Hermitte <EMAIL:hermitte {at} free {dot} fr>
"		<URL:https://github.com/LucHermitte/Rasende>
" Version:      001
" Created:      26th Sep 2012
" Last Update:  30th Oct 2018
"------------------------------------------------------------------------
" Description:
"       Contains the global definitions that need to exist before defining
"       buffer-local definitions.
"       Meant to be sourced from _vimrc_local.vim
"
"------------------------------------------------------------------------
" }}}1
"=============================================================================

" ======================[ Global project configuration {{{2
let s:sources_dir = 'HEAD'

LetIfUndef p:BTW.config.name 'Rasende'

" echomsg "loading: ".expand("<sfile>:p")
" unlet g:rasende_config
" Mandatory Project options
" LetIfUndef p:paths.project    = fnamemodify(lh#option#get('paths.sources'), ':p:h:h')
" LetIfUndef p:paths.build_root_dir = 'build'

if exists(':Module')
    Module load gcc/8.2.0
    if v:shell_error == 0
        LetIfUndef p:BTW.build.mode.bootstrap[gcc8.2] = '-DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG -march=native"'
        LetIfUndef p:BTW.build.mode.current = 'gcc8.2'
    endif
    Module load intel/parallel_studio_xe_2018u3
    if v:shell_error == 0
        LetIfUndef p:BTW.build.mode.bootstrap.sanitize = '-DCMAKE_BUILD_TYPE=Release -DCMAKE_CC_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG -xHOST"'
    endif
    Module load llvm
    if v:shell_error == 0
        LetIfUndef p:BTW.build.mode.bootstrap.intel = '-DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CC_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG -march=native -g -fno-omit-frame-pointer -fsanitize=address,undefined"'
    endif

else
    LetIfUndef p:BTW.build.mode.current = 'release'
    LetIfUndef p:BTW.build.mode.bootstrap.release = '-DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG -march=native"'
    if executable('clang++')
        LetIfUndef p:BTW.build.mode.bootstrap.sanitize = '-DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG -march=native -g -fno-omit-frame-pointer -fsanitize=address,undefined"'
    endif

endif

" Optional Project options
LetTo p:BTW.executable.type 'ctest'
LetIfUndef p:BTW.tests.verbosity '-VV'

call lh#btw#chain#load_config()

" ======================[ Menus {{{2
let s:menu_priority = '50.120.'
let s:menu_name     = '&Project.&Rasende Roboter.'

"------------------------------------------------------------------------
" Compilation mode, & CTest options {{{3
let g:rasende_config_menu = {
      \ '_project': 'rasende_config',
      \ 'menu': {'priority': s:menu_priority, 'name': s:menu_name}
      \ }
call lh#let#if_undef('p:menu', g:rasende_config_menu)

call lh#btw#cmake#define_options([
      \ 'auto_detect_compil_modes'
      \ ])
      " \ 'def_toggable_ctest_verbosity',
      " \ 'def_toggable_ctest_checkmem',
      " \ 'def_ctest_targets',
      " \ 'update_list'])

LetIfUndef p:paths.doxyfile   = lh#option#get('paths.project')

" ======================[ Local variables to automagically import in QuickFix buffer {{{2
""QFImport b:tags_select
""QFImport &l:path
""QFImport b:BTW_project_target
""QFImport b:BTW_compilation_dir
""QFImport b:BTW_project_build_mode
" }}}1
"=============================================================================
" vim600: set fdm=marker:
