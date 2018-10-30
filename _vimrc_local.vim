"=============================================================================
" File:		_vimrc_local for rasende_roboter {{{1
" Author:	Luc Hermitte <EMAIL:hermitte {at} free {dot} fr>
"		<URL:https://github.com/LucHermitte/Rasende>
" Created:	16th Apr 2010
" Last Update:	30th Oct 2018
let s:k_version = 98
"------------------------------------------------------------------------
" Description:
"       Vim configuration for Rasende Roboter Project
" }}}1
"=============================================================================

let s:cpo_save=&cpo
set cpo&vim
" When configuration procedure fails (yes, it happens!), we need to restore vim
" cpo setting
try
    " ### Avoid inclusion if in compatible mode {{{1
    if &cp
        finish " &cpo cleaned with :finally clause
    endif
    " ### Always loaded {{{1
    " ## Name of *this* script {{{2
    let s:script = expand("<sfile>:p")
    "=============================================================================
    " ### Buffer local definitions {{{1
    " ## Avoid buffer reinclusion {{{2
    if (exists("b:loaded_rasende_code_vimrc_local")
                \ && (b:loaded_rasende_code_vimrc_local >= s:k_version)
                \ && (!exists('g:force_reload_rasende_code_vimrc_local')
                \ || g:force_reload_rasende_code_vimrc_local==0))
        finish
    endif
    let b:loaded_rasende_code_vimrc_local = s:k_version
    "------------------------------------------------------------------------
    " ## Define the project {{{2
    Project --define Rasende

    "------------------------------------------------------------------------
    " ## Settings for compil_hints {{{2
    LetTo p:compil_hints_autostart = 1

    "------------------------------------------------------------------------
    " ## &path related settings {{{2
    " # Alternate
    LetTo p:alternates.searchpath='sfr:.'
    " # Completion
    " Don't search into included file how to complete
    LetTo p:&complete-=i

    "------------------------------------------------------------------------
    " ## C++ project style & settings {{{2
    LetTo p:&sw=4
    if expand('%:p:r') !~ '\<Makefile\>'
        setlocal expandtab
    else
        " no expandtab with Makefiles
        setlocal noexpandtab
    endif

    LetTo p:cpp_project_namespace = 'rr'
    LetTo p:cpp_defines_to_ignore = 'LIB_EXPORT'

    "------------------------------------------------------------------------
    " ## common typos {{{2
    inoreab <buffer> logMEssage logMessage
    inoreab <buffer> verison    version

    "=============================================================================
    " ### Initialization / Global Definitions {{{1
    " ## Load external global definitions {{{2
    if ! (exists("g:loaded_rasende_code_vimrc_local")
          \ && (g:loaded_rasende_code_vimrc_local >= s:k_version)
          \ && (!exists('g:force_reload_rasende_code_vimrc_local')
          \     || g:force_reload_rasende_code_vimrc_local==0))
      source <sfile>:p:h/_vimrc_local_global_defs.vim
      let g:loaded_rasende_code_vimrc_local = s:k_version

      " # lh-cpp
      " Configures lh-cpp complete includes sub-plugin
      " If the project has .h.in files that are generated in the build
      " directory, uncomment the next line
      " let s:cpp_included_paths += [lh#option#get('BTW.compilation_dir')]

      " Configures lh-cpp complete includes sub-plugin -> ftplugin/c/c_complete_include.vim
      " -> :h i_CTRL-X_
      " Accessed through lh#cpp#tags#get_included_paths()
      " Note:
      " - p:paths.sources is implicit
      " - lh#btw#compilation_dir() -> To follow generated config.h

      " TODO: register hook to lh#cmake#get_includes()/reconfig to
      " update paths in p:includes/&paths
      " This way, looping on p:includes in
      " lh#cpp#tags#get_included_paths() won't trigger
      " config.bootstrap()
      call lh#let#to('p:includes',
                  \ [function('lh#cpp#tags#compiler_includes')]
                  \+[function('lh#cmake#get_includes')]
                  \+[function('lh#btw#compilation_dir')]
                  \)

      " # Setting &path
      for p in lh#cpp#tags#get_included_paths()
       exe 'LetTo p:&path+='.lh#path#fix(p)
      endfor

      "------------------------------------------------------------------------
      " ## Compilation rules {{{2
      " Settings for BTW
      " sets b:BTW_project_executable.rule
      if has_key(g:rasende_config_menu, 'def_ctest_targets')
            \ && has_key(g:rasende_config_menu.def_ctest_targets, 'set_ctest_argument')
        call g:rasende_config_menu.def_ctest_targets.set_ctest_argument()
      endif

      if SystemDetected() == 'msdos'
        "" :BTW setlocal cmake
        " echomsg SystemDetected()
        if SystemDetected() == 'unix' " cygwin
          :BTW addlocal cygwin
          " then cygwin's cmake does not work -> use win32 cmake
          let $PATH=substitute($PATH, '\(.*\);\([^;]*CMake[^;]*\)', '\2;\1', '')
        endif
      endif
      :BTW addlocal STLFilt

    endif
" }}}1
finally
    let &cpo=s:cpo_save
endtry
"=============================================================================
" vim600: set fdm=marker:
