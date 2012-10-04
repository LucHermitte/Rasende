"=============================================================================
" $Id$
" File:		_vimrc_local for rasende_roboter {{{1
" Author:	Luc Hermitte <EMAIL:hermitte {at} free {dot} fr>
"		<URL:https://github.com/LucHermitte/Rasende>
" Created:	16th Apr 2010
" Last Update:	$Date$
let s:k_version = 54
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
    " ### Always loaded {{{1
    "=============================================================================
    " ### Initialization / Global Definitions {{{1
    " ## Avoid inclusion if in compatible mode {{{2
    if &cp
        finish " &cpo cleaned with :finally clause
    endif
    " ## Load external global definitions {{{2
    if ! (exists("g:loaded_rasende_code_vimrc_local")
                \ && (g:loaded_rasende_code_vimrc_local >= s:k_version)
                \ && (!exists('g:force_reload_rasende_code_vimrc_local')
                \     || g:force_reload_rasende_code_vimrc_local==0))
        source <sfile>:p:h/_vimrc_local_global_defs.vim
        let g:loaded_rasende_code_vimrc_local = s:k_version
    endif
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
    " ## &path related settings {{{2
    " Alternate
    let g:alternateSearchPath='sfr:.'
    " Don't search into included file how to complete
    setlocal complete-=i

    " No sub project
    " let b:project_crt_sub_project = matchstr(lh#path#strip_common([g:rasende_config.paths.trunk, expand('%:p:h')])[1], '[^/\\]*[/\\][^/\\]*')
    " Tells BTW the compilation directory
    let b:BTW_compilation_dir = g:rasende_config.paths._build
    " Local vimrc variable for source dir
    let b:project_sources_dir = g:rasende_config.paths.sources
    " Option for Mu-Template-> |s:path_from_root()|
    let b:sources_root = g:rasende_config.paths.sources

    " Configures lh-cpp complete includes sub-plugin
    let b:includes = [ b:project_sources_dir . '/**']
    if SystemDetected() == 'unix'
        call BTW_AddPathIfExists('b:includes', '/usr/local/include/**')
        call BTW_AddPathIfExists('b:includes', '/usr/include/**')
    endif

    let s:path = expand('<sfile>:p:h')
    " Option for Mu-Template-> |s:path_from_root()|
    let b:sources_root = g:rasende_config.paths.sources
    " Used by mu-template lh#cpp#file#IncludedPaths() function, to
    " auto-magically generate the name of header files associated to new
    " .c/.cpp files.
    let b:cpp_included_paths = [
                \  s:path, (s:path.'/bench/')
                \]
    " Setting &path
    exe 'setlocal path+='.FixPathName(b:project_sources_dir).'/**'
    for p in b:includes
        exe 'setlocal path+='.FixPathName(p)
    endfor
    "------------------------------------------------------------------------
    " ## C++ project style & settings {{{2
    setlocal sw=4
    if expand('%:p:r') !~ '\<Makefile\>'
        setlocal expandtab
    else
        " no expandtab with Makefiles
        setlocal noexpandtab
    endif

    let b:cpp_project_namespace = 'rr'
    let b:cpp_defines_to_ignore = 'LIB_EXPORT'

    "------------------------------------------------------------------------
    " ## Commands and mappings and options {{{2

    "------------------------------------------------------------------------
    " ## Menus {{{2
    call lh#menu#make('nic', '50.11', '&Project.Edit local &CMake file', '<localleader><F7>', '<buffer>', ':call g:rasende_config.functions.EditLocalCMakeFile()<cr>')
    call lh#menu#make('nic', '50.76', '&Project.Edit local &vimrc', '<localleader>le', '<buffer>', ':call lh#buffer#jump('.string(s:script).', "sp")<cr>' )
    "------------------------------------------------------------------------
    " ## common typos {{{2
    inoreab <buffer> logMEssage logMessage
    inoreab <buffer> verison    version

    "------------------------------------------------------------------------
    " ## Compilation rules {{{2
    " Settings for BTW
    silent! unlet b:BTW_project_executable
    LetIfUndef b:BTW_project_executable.type 'ctest'
    " sets b:BTW_project_executable.rule
    if has_key(g:rasende_config_menu, 'def_ctest_targets')
                \ && has_key(g:rasende_config_menu.def_ctest_targets, 'set_ctest_argument')
        call g:rasende_config_menu.def_ctest_targets.set_ctest_argument()
    endif
    let b:BTW_project_target = ''

    if SystemDetected() == 'msdos'
        :BTW setlocal cmake
        " echomsg SystemDetected()
        if SystemDetected() == 'unix' " cygwin
            :BTW addlocal cygwin
            " then cygwin's cmake does not work -> use win32 cmake
            let $PATH=substitute($PATH, '\(.*\);\([^;]*CMake[^;]*\)', '\2;\1', '')
        endif
    endif
    :BTW addlocal STLFilt

    " Project config -> ccmake
    let b:BTW_project_config = {
                \ 'type': 'ccmake',
                \ 'arg': (b:project_sources_dir), 
                \ 'wd' : (b:BTW_compilation_dir)}

    "=============================================================================
    " ### Global definitions {{{1
    " ## Avoid global reinclusion {{{2
    if exists("g:loaded_vimrc_local_4_rasende") 
                \ && !exists('g:force_reload_vimrc_local_4_rasende')
        finish 
    endif
    let g:loaded_vimrc_local_4_rasende = 1

    "=============================================================================
    " ## Special compilation rules {{{2
    finish
    if ! exists("g:inconfigure")
        function! Configure()
            try 
                let makeprg_save = &makeprg
                let &makeprg = './configure ' . (g:BTW_compil_mode == 'release' ? '' : '--enable-debug')
                let g:inconfigure=1
                :make
                :Copen
            finally
                let &makeprg = makeprg_save
                unlet g:inconfigure
            endtry
        endfunction

        function! Bootstrap()
            try 
                let makeprg_save = &makeprg
                let &makeprg = './bootstrap.sh '  
                let g:inconfigure=1
                :make
                :Copen
            finally
                let &makeprg = makeprg_save
                unlet g:inconfigure
            endtry
        endfunction
    endif

    call lh#menu#make('nic', '50.40.10', '&Project.&Build.&booststrap', '', '<buffer>', ':call Bootstrap()<cr>' )
    call lh#menu#make('nic', '50.40.15', '&Project.&Build.con&figure', '', '<buffer>', ':call Configure()<cr>' )

    amenu 50.40.19 &Project.&Build.---<sep>--- Nop
    call lh#menu#make('nic', '50.40.20', '&Project.&Build.&clean', '', '<buffer>', ':Make clean<cr>' )
    call lh#menu#make('nic', '50.40.30', '&Project.&Build.chec&k', '', '<buffer>', ':Make check<cr>' )
    call lh#menu#make('nic', '50.40.40', '&Project.&Build.b&zip2', '', '<buffer>', ':Make dist-bzip2<cr>' )

    let idx = exists('g:Compilers') ? Compilers.idx_crt_value : 1
    let Compilers = {
                \ "variable": "$CXX",
                \ "idx_crt_value": 1,
                \ "menu": {"priority": '50.110', "name": '&Project.&CXX'}
                \ }
    if $OSTYPE =~? 'solaris.*'
        let Compilers['values'] = ['CC', 'g++']
    else
        let Compilers['values'] = ['g++', 'g++-4']
    endif
    call lh#menu#def_toggle_item(Compilers)
    " todo: detect the current $CXX in the configuration
    " todo: propose to reconfigure on $CXX change event
    "
    let idx = exists('g:DebugMode') ? DebugMode.idx_crt_value : 0
    let DebugMode = {
                \ "variable": "BTW_compil_mode",
                \ "idx_crt_value": idx,
                \ "values" : ['release', 'debug'],
                \ "menu": {"priority": '50.120', "name": '&Project.Compilation &Mode'}
                \ }
    call lh#menu#def_toggle_item(DebugMode)
    " todo: apply the change to the makefile if it can be transparent
    " otherwise, have a way to notify the user the Makefile is not synched by
    " the option
    " todo: move this BTW_compil_mode+Configure to a plugin for
    " BuildToolsWrapper.vim
    " todo: add action for a quick overide: -g <-> -O3

    "=============================================================================

finally
    let &cpo=s:cpo_save
endtry
"=============================================================================
" vim600: set fdm=marker:
