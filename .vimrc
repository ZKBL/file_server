" Maintainer:	Bram Moolenaar <Bram@vim.org>
" Last change:	2017 Jun 13
"
" This is loaded if no vimrc file was found.
" Except when Vim is run with "-u NONE" or "-C".
" Individual settings can be reverted with ":set option&".
" Other commands can be reverted as mentioned below.
"###############################################################################
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'
Plugin 'Valloric/YouCompleteMe'
Plugin 'lilydjwg/fcitx.vim'
Plugin 'aceofall/gtags.vim'
Plugin 'ctrlpvim/ctrlp.vim'

" The following are examples of different formats supported.
" Keep Plugin commands between vundle#begin/end.
" plugin on GitHub repo
"Plugin 'tpope/vim-fugitive'
" plugin from http://vim-scripts.org/vim/scripts.html
" Plugin 'L9'
" Git plugin not hosted on GitHub
"Plugin 'git://git.wincent.com/command-t.git'
" git repos on your local machine (i.e. when working on your own plugin)
"Plugin 'file:///home/gmarik/path/to/plugin'
" The sparkup vim script is in a subdirectory of this repo called vim.
" Pass the path to set the runtimepath properly.
"Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" Install L9 and avoid a Naming conflict if you've already installed a
" different version somewhere else.
" Plugin 'ascenator/L9', {'name': 'newL9'}

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line
" ######################################################

" When started as "evim", evim.vim will already have done these settings.
if v:progname =~? "evim"
  finish
endif

" Bail out if something that ran earlier, e.g. a system wide vimrc, does not
" want Vim to use these default values.
if exists('skip_defaults_vim')
  finish
endif

" Use Vim settings, rather than Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
" Avoid side effects when it was already reset.
if &compatible
  set nocompatible
endif

" When the +eval feature is missing, the set command above will be skipped.
" Use a trick to reset compatible only when the +eval feature is missing.
silent! while 0
  set nocompatible
silent! endwhile

" Allow backspacing over everything in insert mode.
set backspace=indent,eol,start

set history=200		" keep 200 lines of command line history
set ruler		" show the cursor position all the time
set showcmd		" display incomplete commands
set wildmenu		" display completion matches in a status line

set ttimeout		" time out for key codes
set ttimeoutlen=100	" wait up to 100ms after Esc for special key

" Show @@@ in the last line if it is truncated.
set display=truncate

" Show a few lines of context around the cursor.  Note that this makes the
" text scroll if you mouse-click near the start or end of the window.
set scrolloff=5

" Do incremental searching when it's possible to timeout.
if has('reltime')
  set incsearch
endif

" Do not recognize octal numbers for Ctrl-A and Ctrl-X, most users find it
" confusing.
set nrformats-=octal

" For Win32 GUI: remove 't' flag from 'guioptions': no tearoff menu entries.
if has('win32')
  set guioptions-=t
endif

" Don't use Ex mode, use Q for formatting.
" Revert with ":unmap Q".
map Q gq

" CTRL-U in insert mode deletes a lot.  Use CTRL-G u to first break undo,
" so that you can undo CTRL-U after inserting a line break.
" Revert with ":iunmap <C-U>".
inoremap <C-U> <C-G>u<C-U>

" In many terminal emulators the mouse works just fine.  By enabling it you
" can position the cursor, Visually select and scroll with the mouse.
if has('mouse')
  set mouse=a
endif

" Switch syntax highlighting on when the terminal has colors or when using the
" GUI (which always has colors).
if &t_Co > 2 || has("gui_running")
  " Revert with ":syntax off".
  syntax on

  " I like highlighting strings inside C comments.
  " Revert with ":unlet c_comment_strings".
  let c_comment_strings=1
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")

  " Enable file type detection.
  " Use the default filetype settings, so that mail gets 'tw' set to 72,
  " 'cindent' is on in C files, etc.
  " Also load indent files, to automatically do language-dependent indenting.
  " Revert with ":filetype off".
  filetype plugin indent on

  " Put these in an autocmd group, so that you can revert them with:
  " ":augroup vimStartup | au! | augroup END"
  augroup vimStartup
    au!

    " When editing a file, always jump to the last known cursor position.
    " Don't do it when the position is invalid, when inside an event handler
    " (happens when dropping a file on gvim) and for a commit message (it's
    " likely a different one than last time).
    autocmd BufReadPost *
      \ if line("'\"") >= 1 && line("'\"") <= line("$") && &ft !~# 'commit'
      \ |   exe "normal! g`\""
      \ | endif

  augroup END

endif " has("autocmd")

" Convenient command to see the difference between the current buffer and the
" file it was loaded from, thus the changes you made.
" Only define it when not defined already.
" Revert with: ":delcommand DiffOrig".
if !exists(":DiffOrig")
  command DiffOrig vert new | set bt=nofile | r ++edit # | 0d_ | diffthis
		  \ | wincmd p | diffthis
endif

if has('langmap') && exists('+langremap')
  " Prevent that the langmap option applies to characters that result from a
  " mapping.  If set (default), this may break plugins (but it's backward
  " compatible).
  set nolangremap
endif
"#####################YouComepleteMe##################################
 "YouCompleteMe
 " Python Semantic Completion
 let g:ycm_python_binary_path = '/usr/bin/python3'
 " C family Completion Path
"  let g:ycm_global_ycm_extra_conf='~/.vim/bundle/YouCompleteMe/third_party/ycmd/.ycm_extra_conf.py'
 let g:ycm_global_ycm_extra_conf='~/.ycm_extra_conf.py'
 " 跳转快捷键
 nnoremap <c-k> :YcmCompleter GoToDeclaration<CR>|
 nnoremap <c-h> :YcmCompleter GoToDefinition<CR>| 
 nnoremap <c-j> :YcmCompleter GoToDefinitionElseDeclaration<CR>|
 " 停止提示是否载入本地ycm_extra_conf文件
 "let g:ycm_confirm_extra_conf = 0
 " 语法关键字补全
 let g:ycm_seed_identifiers_with_syntax = 1
 " 开启 YCM 基于标签引擎
 let g:ycm_collect_identifiers_from_tags_files = 1
 " 从第2个键入字符就开始罗列匹配项
 let g:ycm_min_num_of_chars_for_completion=2
 " 在注释输入中也能补全
 let g:ycm_complete_in_comments = 1
 " 在字符串输入中也能补全
 let g:ycm_complete_in_strings = 1
 " 注释和字符串中的文字也会被收入补全
 let g:ycm_collect_identifiers_from_comments_and_strings = 1
 " 弹出列表时选择第1项的快捷键(默认为<TAB>和<Down>)
 "let g:ycm_key_list_select_completion = ['<C-n>', '<Down>']
 " 弹出列表时选择前1项的快捷键(默认为<S-TAB>和<UP>)
 "let g:ycm_key_list_previous_completion = ['<C-p>', '<Up>']
 " 主动补全, 默认为<C-Space>
 "let g:ycm_key_invoke_completion = ['<C-Space>']
 " 停止显示补全列表(防止列表影响视野), 可以按<C-Space>重新弹出
 "let g:ycm_key_list_stop_completion = ['<C-y>']

"========================================================
" add header comments for .h .c .hpp .cpp .mk .sh new file
" auto call SetTitle func
autocmd BufNewFile *.[ch],*.cc,*.hpp,*.cpp,Makefile,*.mk,*.sh,*.py exec ":call SetTitle()"

" add comment for cpp
func SetComment_ch()
	call setline(1,"/*================================================================")
	call append(line("."),   "*   Copyright (C) ".strftime("%Y")." * Ltd. All rights reserved.")
	call append(line(".")+1, "*   ")
	call append(line(".")+2, "*   File name   : ".expand("%:t"))
	call append(line(".")+3, "*   Author      : looming")
	call append(line(".")+4, "*   Created date: ".strftime("%F %T"))
	call append(line(".")+5, "*   Description : ")
	call append(line(".")+6, "*		Email				: flm919841090@gmail.com")
	call append(line(".")+7, "*")
	call append(line(".")+8, "*===============================================================*/")
	call append(line(".")+9, "")
	call append(line(".")+10, "")
endfunc

" add comment for shell,Makefile
func SetComment_sh()
	call setline(3, "#================================================================")
	call setline(4, "#   Copyright (C) ".strftime("%Y")." * Ltd. All rights reserved.")
	call setline(5, "#   ")
	call setline(6, "#   File name   : ".expand("%:t"))
	call setline(7, "#   Author      : looming")
	call setline(8, "#   Created date: ".strftime("%F %T"))
	call setline(9, "#   Description : ")
	call setline(10, "#	 Email			 :flm919841090@gmail.com")
	call setline(11, "#")
	call setline(12, "#================================================================")
	call setline(12, "")
	call setline(13, "")
endfunc

" SetTitle func, add comment
func SetTitle()
	if &filetype == 'make'
		call setline(1,"")
		call setline(2,"")
		call SetComment_sh()

	elseif &filetype == 'sh'
		call setline(1,"#! /bin/bash")
		call setline(2,"")
		call SetComment_sh()

	elseif &filetype == 'python'
		call setline(1,"#! /usr/bin/env python")
		call setline(2,"# coding=utf-8")
		call setline(3,"")
		call SetComment_sh()

	else
		call SetComment_ch()
		if expand("%:e") == 'hpp'
			call append(line(".")+10, "#ifndef _".toupper(expand("%:t:r"))."_H")
			call append(line(".")+11, "#define _".toupper(expand("%:t:r"))."_H")
			call append(line(".")+12, "#ifdef __cplusplus")
			call append(line(".")+13, "extern \"C\"")
			call append(line(".")+14, "{")
			call append(line(".")+15, "#endif")
			call append(line(".")+16, "")
			call append(line(".")+17, "#ifdef __cplusplus")
			call append(line(".")+18, "}")
			call append(line(".")+19, "#endif")
			call append(line(".")+20, "#endif //".toupper(expand("%:t:r"))."_H")
		elseif expand("%:e") == 'cpp'
			call append(line(".")+10,"#include <iostream>")
			call append(line(".")+11, "using std::cout;")
			call append(line(".")+12, "using std::cin;")
			call append(line(".")+13, "using std::endl;")
		elseif expand("%:e") == 'cc'
			call append(line(".")+10,"#include <iostream>")
			call append(line(".")+11, "using std::cout;")
			call append(line(".")+12, "using std::cin;")
			call append(line(".")+13, "using std::endl;")
			call append(line(".")+14, "")
			call append(line(".")+15, "")
			call append(line(".")+16, "int main(){")
			call append(line(".")+17, "}")
      

		endif

	endif
endfunc


"========================================================


set nu
set rtp+=/usr/lib/python3.7/site-packages/powerline/bindings/vim
set laststatus=2
set t_Co=256
set showtabline=2
set autoindent
set cindent
set cursorline
set tabstop=2
set softtabstop=2
set shiftwidth=2
set expandtab
set fileencodings=utf-8,gbk,ucs-bom,gb18030,gb2312,cp936
set termencoding=utf-8
set encoding=utf-8

set cscopetag
set cscopeprg='gtags-cscope'
let GtagsCscope_Auto_Load =1
let CtagsCscope_Auto_Map=1
let GtagsCscope_quiet=1

