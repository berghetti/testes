" Global configurations
syntax on            " Enable syntax highlight
set nu               " Enable line numbers
set tabstop=4        " Show existing tab with 4 spaces width
set softtabstop=2    " Show existing tab with 2 spaces width
set shiftwidth=2     " When indenting with '>', use 2 spaces width
set autoindent
set expandtab        " On pressing tab, insert 2 spaces
set smarttab         " insert tabs on the start of a line according to shiftwidth
set smartindent      " Automatically inserts one extra level of indentation in some cases
set hidden           " Hides the current buffer when a new file is openned
set incsearch        " Incremental search
set ignorecase       " Ingore case in search
set smartcase        " Consider case if there is a upper case character
set scrolloff=8      " Minimum number of lines to keep above and below the cursor
set colorcolumn=80  " Draws a line at the given line to keep aware of the line size
"set highlight ColorColumn ctermbg=8 "Set gackground color vertival line
"set signcolumn=yes   " Add a column on the left. Useful for linting
set cmdheight=2      " Give more space for displaying messages
set updatetime=500   " Time in miliseconds to consider the changes
set encoding=utf-8   " The encoding should be utf-8 to activate the font icons
set nobackup         " No backup files
set nowritebackup    " No backup files
set splitright       " Create the vertical splits to the right
set splitbelow       " Create the horizontal splits below
set autoread         " Update vim after file update from outside
set mouse=a          " Enable mouse support
set nohlsearch       " disable highlight search
set path=.,/usr/lib/gcc/x86_64-linux-gnu/9/include,/usr/local/include,/usr/include/x86_64-linux-gnu,/usr/include
filetype on          " Detect and set the filetype option and trigger the FileType Event
filetype plugin on   " Load the plugin file for the file type, if any
filetype indent on   " Load the indent file for the file type, if any

" Plugin manager
" :PlugInstall to instaler new plugins
call plug#begin()
 Plug 'sainnhe/sonokai' "Theme
 Plug 'vim-airline/vim-airline'
 Plug 'vim-airline/vim-airline-themes'
 Plug 'sheerun/vim-polyglot' "Syntax highlight most languages
 "Plug 'preservim/nerdtree' "File explorer, usage with ctrl+a (with map)
 "Plug 'Xuyuanp/nerdtree-git-plugin'
 "Plug 'tiagofumo/vim-nerdtree-syntax-highlight' "some visual elements in nerdtree
 "Plug 'ryanoasis/vim-devicons' "dependency to nerdtree syntax
 Plug 'jiangmiao/auto-pairs' "automatic close ([{ ...
 Plug 'dense-analysis/ale' " clang-format support
 Plug 'neoclide/coc.nvim' , { 'branch' : 'release' } "auto complete and others
 Plug 'honza/vim-snippets',
 "Plug 'tell-k/vim-autopep8' " Python format code
 if (has("nvim"))
    Plug 'BurntSushi/ripgrep'
    Plug 'sharkdp/fd'
    Plug 'nvim-lua/plenary.nvim'
    Plug 'nvim-telescope/telescope.nvim'
 endif
call plug#end()


"Theme configurations
if exists('+termguicolors')
  let &t_8f = "\<Esc>[38;2;%lu;%lu;%lum"
  let &t_8b = "\<Esc>[48;2;%lu;%lu;%lum"
  set termguicolors
endif

let g:sonokai_style = 'andromeda'
let g:sonokai_enable_italic = 1
let g:sonokai_disable_italic_comment = 0
let g:sonokai_diagnostic_line_highlight = 1
let g:sonokai_current_word = 'bold'
let g:airline_theme = 'sonokai' "theme sonakai to airline
colorscheme sonokai

"if (has("nvim")) "Transparent background. Only for nvim
"    highlight Normal guibg=NONE ctermbg=NONE
"    highlight EndOfBuffer guibg=NONE ctermbg=NONE
"endif

"Airline configurations
let g:airline#extensions#tabline#enabled = 1
let g:airline_powerline_fonts = 1

"General maps """""""""""""""""""""""""""
" change between buffer
nmap ty :bn<CR>
nmap tr :bp<CR>

"close current buffer: q
nnoremap q :bd<CR>
"force close current buffer (loss changes on file)
nnoremap q! :bd!<CR>

"\vv split current buffer
nmap <silent> vv :vsplit<CR>

" goto implementation CTRL + mouse left click also work
" apt install ctags && ctags -R project/src
nmap <silent> gd <C-]>
" same above, but open in vertical split
map <silent> gdv :vsplit <CR>:exec("tag ".expand("<cword>"))<CR>

"goback of implementation
nmap <silent> gb <C-T>

"move current buffer to new tab
nmap nt <C-w> <S-T>

"exit termnal mode
tnoremap <Esc> <C-\><C-n>

"copy to clipboard
nnoremap Y "+y
vnoremap Y "+y
nnoremap yY ^"+y$
"""""""""""""""""""""""""""""""""""""""

"map to nerdtree
"Open file explorer with ctrl+a
"nmap <C-a> :NERDTreeToggle<CR>

"Navegate between split buffers with ctrl + arrow keys
nmap <silent> <C-Right> <c-w>l
nmap <silent> <C-Left> <c-w>h
nmap <silent> <C-Up> <c-w>k
nmap <silent> <C-Down> <c-w>jo

" Telescope plugin config
if (has("nvim"))
    nnoremap <leader>ff <cmd>Telescope find_files<cr>
    nnoremap <leader>fg <cmd>Telescope live_grep<cr>
    nnoremap <leader>fb <cmd>Telescope buffers<cr>
    nnoremap <leader>fh <cmd>Telescope help_tags<cr>
endif

" ALE conf(lintier disable)
let g:ale_linters = {
\   'cpp': [],
\   'c': [],
\   'python': [],
\}
"\   'python': ['flake8'],

let g:ale_fixers = {
\   '*': ['trim_whitespace'],
\   'cpp': ['clang-format'],
\   'c': ['clang-format'],
\}

let g:ale_c_clangformat_options = ''
let g:ale_c_clangformat_style_options = '"{BasedOnStyle: gnu}"'
let g:ale_c_clangformat_use_local_file = 1

" format code on save
let g:ale_fix_on_save = 1

" Python """"""""""""""""""""""""""""""""""""""""""""""
"let g:ale_python_flake8_options = '--max-line-length=80 --extend-ignore=E203'
let g:autopep8_on_save = 1
let g:autopep8_disable_show_diff=1
let g:autopep8_indent_size=2
autocmd FileType python setlocal noexpandtab shiftwidth=2 softtabstop=2

"Plugins CoC
" clangd show lintier
let g:coc_global_extensions = [ 'coc-snippets', 'coc-clangd' ]

let g:clang_library_path='/usr/lib/llvm-15/lib/libclang.so.1'

let g:coc_disable_startup_warning = 1

"Config coc.nvim
" https://github.com/neoclide/coc-snippets
" use <tab> to trigger completion and navigate to the next complete item
inoremap <silent><expr> <TAB>
      \ coc#pum#visible() ? coc#_select_confirm() :
      \ coc#expandableOrJumpable() ? "\<C-r>=coc#rpc#request('doKeymap', ['snippets-expand-jump',''])\<CR>" :
      \ CheckBackspace() ? "\<TAB>" :
      \ coc#refresh()

function! CheckBackspace() abort
  let col = col('.') - 1
  return !col || getline('.')[col - 1]  =~# '\s'
endfunction

let g:coc_snippet_next = '<tab>'

" GoTo code navigation
"nmap <silent> gd <Plug>(coc-definition)
"nmap <silent> gy <Plug>(coc-type-definition)
"nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)

" Use K to show documentation in preview window
nnoremap <silent> K :call ShowDocumentation()<CR>

function! ShowDocumentation()
  if CocAction('hasProvider', 'hover')
    call CocActionAsync('doHover')
  else
    call feedkeys('K', 'in')
  endif
endfunction

" Use <c-space> to trigger completion
if has('nvim')
  inoremap <silent><expr> <c-space> coc#refresh()
else
  inoremap <silent><expr> <c-@> coc#refresh()
endif
