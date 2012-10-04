"=============================================================================
" $Id$
" File:         _vimrc_local_global_defs.vim {{{1
" Author:       Luc Hermitte <EMAIL:hermitte {at} free {dot} fr>
"		<URL:https://github.com/LucHermitte/Rasende>
" Version:      001
" Created:      26th Sep 2012
" Last Update:  $Date$
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

" echomsg "loading: ".expand("<sfile>:p")
" unlet g:rasende_config
" Mandatory Project options
call lh#let#if_undef('g:rasende_config.paths.trunk', string(expand("<sfile>:p:h")))
LetIfUndef g:rasende_config.paths.project    fnamemodify(g:rasende_config.paths.trunk,':h')
LetIfUndef g:rasende_config.paths.doxyfile   g:rasende_config.paths.project
LetIfUndef g:rasende_config.build.Debug      'build-d'
LetIfUndef g:rasende_config.build.Release    'build-r'

" Here, this matches all the trunk => complete even with test files
LetIfUndef g:rasende_config.paths.sources    g:rasende_config.paths.project.'/src'
" Optional Project options
LetIfUndef g:rasende_config.compilation.mode 'Debug'
LetIfUndef g:rasende_config.tests.verbosity '-VV'

" ======================[ Menus {{{2
let s:menu_priority = '50.120.'
let s:menu_name     = '&Project.&Rasende Roboter.'

" Function: s:getSNR([func_name]) {{{3
function! s:getSNR(...)
  if !exists("s:SNR")
    let s:SNR=matchstr(expand('<sfile>'), '<SNR>\d\+_\zegetSNR$')
  endif
  return s:SNR . (a:0>0 ? (a:1) : '')
endfunction

" Function: s:EditLocalCMakeFile() {{{3
function! s:EditLocalCMakeFile()
  let file = lh#path#to_relative(expand('%:p:h').'/CMakeLists.txt')
  call lh#buffer#jump(file, 'sp')
endfunction

call lh#let#if_undef ('g:sea_config.functions',
      \ string({'EditLocalCMakeFile': function(s:getSNR('EditLocalCMakeFile'))}))

"------------------------------------------------------------------------
" Compilation mode, & CTest options {{{3
let g:rasende_config_menu = {
      \ '_project': 'rasende_config',
      \ 'menu': {'priority': s:menu_priority, 'name': s:menu_name}
      \ }
call lh#btw#cmake#def_options(g:rasende_config_menu, [
      \ 'def_toggable_compil_mode'
      \ ])
      " \ 'def_toggable_ctest_verbosity',
      " \ 'def_toggable_ctest_checkmem',
      " \ 'def_ctest_targets',
      " \ 'update_list'])

" ======================[ Local variables to automagically import in QuickFix buffer {{{2
QFImport b:tags_select
QFImport &l:path
QFImport b:BTW_project_target
QFImport b:BTW_compilation_dir
QFImport b:BTW_project_build_mode

" ======================[ Misc function {{{2
" Function: BTW_AddPathIfExists(listname, path) {{{3
function! BTW_AddPathIfExists(listname, path)
    let path = substitute(a:path, '[/\\]\*\*$', '', '')
    if isdirectory(path)
        let {a:listname} += [a:path]
    endif
endfunction

"=============================================================================
" vim600: set fdm=marker:
