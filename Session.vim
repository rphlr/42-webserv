let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd /goinfre/rrouille
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +0 /Volumes/42SCHOOL/Code/webserv/srcs/main.cpp
badd +35 /Volumes/42SCHOOL/Code/webserv/srcs/webserv.cpp
badd +0 term:///Volumes/42SCHOOL/Code/webserv//78790:/bin/zsh
badd +0 /Volumes/42SCHOOL/Code/webserv/Session.vim
argglobal
%argdel
$argadd NvimTree_1
edit /Volumes/42SCHOOL/Code/webserv/Session.vim
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
2wincmd h
wincmd w
wincmd w
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
wincmd =
argglobal
balt /Volumes/42SCHOOL/Code/webserv/srcs/webserv.cpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 64 - ((0 * winheight(0) + 44) / 89)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 64
normal! 0
lcd /Volumes/42SCHOOL/Code/webserv
wincmd w
argglobal
if bufexists(fnamemodify("/Volumes/42SCHOOL/Code/webserv/srcs/main.cpp", ":p")) | buffer /Volumes/42SCHOOL/Code/webserv/srcs/main.cpp | else | edit /Volumes/42SCHOOL/Code/webserv/srcs/main.cpp | endif
if &buftype ==# 'terminal'
  silent file /Volumes/42SCHOOL/Code/webserv/srcs/main.cpp
endif
balt /Volumes/42SCHOOL/Code/webserv/srcs/webserv.cpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 44) / 89)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
lcd /Volumes/42SCHOOL/Code/webserv
wincmd w
argglobal
if bufexists(fnamemodify("/Volumes/42SCHOOL/Code/webserv/srcs/main.cpp", ":p")) | buffer /Volumes/42SCHOOL/Code/webserv/srcs/main.cpp | else | edit /Volumes/42SCHOOL/Code/webserv/srcs/main.cpp | endif
if &buftype ==# 'terminal'
  silent file /Volumes/42SCHOOL/Code/webserv/srcs/main.cpp
endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 2 - ((1 * winheight(0) + 44) / 89)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 2
normal! 072|
lcd /Volumes/42SCHOOL/Code/webserv
wincmd w
argglobal
if bufexists(fnamemodify("term:///Volumes/42SCHOOL/Code/webserv//78790:/bin/zsh", ":p")) | buffer term:///Volumes/42SCHOOL/Code/webserv//78790:/bin/zsh | else | edit term:///Volumes/42SCHOOL/Code/webserv//78790:/bin/zsh | endif
if &buftype ==# 'terminal'
  silent file term:///Volumes/42SCHOOL/Code/webserv//78790:/bin/zsh
endif
balt /Volumes/42SCHOOL/Code/webserv/srcs/webserv.cpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 92 - ((24 * winheight(0) + 18) / 37)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 92
normal! 038|
lcd /Volumes/42SCHOOL/Code/webserv
wincmd w
wincmd =
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
nohlsearch
let g:this_session = v:this_session
let g:this_obsession = v:this_session
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
